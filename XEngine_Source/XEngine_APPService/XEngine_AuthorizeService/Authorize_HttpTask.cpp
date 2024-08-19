#include "Authorize_Hdr.h"

XHTHREAD CALLBACK XEngine_AuthService_HttpThread(XPVOID lParam)
{
	int nPoolIndex = *(int*)lParam;
	int nThreadPos = nPoolIndex + 1;

	while (bIsRun)
	{
		if (!HttpProtocol_Server_EventWaitEx(xhHttpPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		XENGINE_MANAGEPOOL_TASKEVENT** ppSt_ListClient;
		HttpProtocol_Server_GetPoolEx(xhHttpPacket, nThreadPos, &ppSt_ListClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			int nMsgLen = 0;
			XCHAR* ptszMsgBuffer = NULL;
			RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParament;

			memset(&st_HTTPParament, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));

			if (HttpProtocol_Server_GetMemoryEx(xhHttpPacket, ppSt_ListClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPParament))
			{
				if (st_AuthConfig.st_XCrypto.bEnable)
				{
					XCHAR tszPassword[64];
					XCHAR tszDeBuffer[2048];

					memset(tszPassword, '\0', sizeof(tszPassword));
					memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

					_xstprintf(tszPassword, _X("%d"), st_AuthConfig.st_XCrypto.nPassword);
					OPenSsl_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszDeBuffer, tszPassword);
					XEngine_Client_HttpTask(ppSt_ListClient[i]->tszClientAddr, tszDeBuffer, nMsgLen, &st_HTTPParament);
				}
				else
				{
					XEngine_Client_HttpTask(ppSt_ListClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &st_HTTPParament);
				}
			}
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	return 0;
}

bool XEngine_Client_HttpTask(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM *pSt_HTTPParament)
{
	int nSDLen = 4096;
	XCHAR tszSDBuffer[4096];
	LPCXSTR lpszMethodPost = _X("POST");
	LPCXSTR lpszMethodGet = _X("GET");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	AUTHREG_BANNED st_Banned;
	memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));
	_tcsxcpy(st_Banned.tszIPAddr, lpszClientAddr);
	BaseLib_OperatorIPAddr_SegAddr(st_Banned.tszIPAddr);

	//是否在黑名单
	bool bSuccess = false;
	if (0 == st_AuthConfig.st_XSql.nDBType) 
	{
		bSuccess = DBModule_SQLite_BannedExist(&st_Banned); //IP地址是否在黑名单
	}
	else 
	{
		bSuccess = DBModule_MySQL_BannedExist(&st_Banned);//IP地址是否在黑名单
	}
	if (!bSuccess && st_FunSwitch.bSwitchBanned)
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 423, "ip address is banned");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，登录连接被阻止，用户名或IP地址被禁用!"), lpszClientAddr);
		return false;
	}
	if (0 == _tcsxnicmp(lpszMethodPost, pSt_HTTPParament->tszHttpMethod, _tcsxlen(lpszMethodPost)))
	{
		XCHAR tszAPIType[64];
		XCHAR tszAPIVer[64];
		XCHAR tszAPIName[64];
		XNETHANDLE xhToken = 0;
		LPCXSTR lpszAPIType = _X("auth");
		LPCXSTR lpszAPIVerClient = _X("client");
		LPCXSTR lpszAPIVerSerial = _X("serial");
		LPCXSTR lpszAPIVerUser = _X("user");
		LPCXSTR lpszAPIVerPass = _X("pass");
		LPCXSTR lpszAPIVerSwitch = _X("switch");
		LPCXSTR lpszAPIVerBanned = _X("banned");
		LPCXSTR lpszAPIVerCDKey = _X("cdkey");
		LPCXSTR lpszAPIVerNotice = _X("notice");
		LPCXSTR lpszAPIVerTry = _X("try");

		memset(tszAPIType, '\0', sizeof(tszAPIType));
		memset(tszAPIVer, '\0', sizeof(tszAPIVer));
		memset(tszAPIName, '\0', sizeof(tszAPIName));

		HttpProtocol_ServerHelp_GetUrlApi(pSt_HTTPParament->tszHttpUri, tszAPIType, tszAPIVer, tszAPIName);
		if (0 != _tcsxnicmp(lpszAPIType, tszAPIType, _tcsxlen(lpszAPIType)))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "request url is incorrent");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求的API:%s 不支持"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
			return false;
		}
		AUTHREG_USERTABLE st_UserTable;
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
		//得到TOKEN
		if (Protocol_Parse_HttpParseToken(lpszMsgBuffer, nMsgLen, &xhToken))
		{
			if (!Session_Token_Get(xhToken, &st_UserTable))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "Unauthorized");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求的API:%s 失败,因为没有经过验证"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return false;
			}

			if (0 == _tcsxnicmp(lpszAPIVerClient, tszAPIVer, _tcsxlen(lpszAPIVerClient)))
			{
				//验证权限
				if (0 != st_UserTable.st_UserInfo.nUserLevel)
				{
					Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "permission is failed");
					XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求的API:%s 失败,因为TOKEN权限不足"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
					return false;
				}
				XEngine_AuthorizeHTTP_Client(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
			}
			else if (0 == _tcsxnicmp(lpszAPIVerSerial, tszAPIVer, _tcsxlen(lpszAPIVerSerial)))
			{
				//验证权限
				if (0 != st_UserTable.st_UserInfo.nUserLevel)
				{
					Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "permission is failed");
					XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求的API:%s 失败,因为TOKEN权限不足"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
					return false;
				}
				XEngine_AuthorizeHTTP_Serial(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
			}
			else if (0 == _tcsxnicmp(lpszAPIVerSwitch, tszAPIVer, _tcsxlen(lpszAPIVerSwitch)))
			{
				//验证权限
				if (0 != st_UserTable.st_UserInfo.nUserLevel)
				{
					Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "permission is failed");
					XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求的API:%s 失败,因为TOKEN权限不足"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
					return false;
				}
				XEngine_AuthorizeHTTP_Switch(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
			}
			else if (0 == _tcsxnicmp(lpszAPIVerBanned, tszAPIVer, _tcsxlen(lpszAPIVerBanned)))
			{
				//验证权限
				if (0 != st_UserTable.st_UserInfo.nUserLevel)
				{
					Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "permission is failed");
					XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求的API:%s 失败,因为TOKEN权限不足"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
					return false;
				}
				XEngine_AuthorizeHTTP_Banned(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
			}
			else if (0 == _tcsxnicmp(lpszAPIVerNotice, tszAPIVer, _tcsxlen(lpszAPIVerNotice)))
			{
				//验证权限
				if (0 != st_UserTable.st_UserInfo.nUserLevel)
				{
					Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "permission is failed");
					XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求的API:%s 失败,因为TOKEN权限不足"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
					return false;
				}
				XEngine_AuthorizeHTTP_Announcement(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
			}
			else if (0 == _tcsxnicmp(lpszAPIVerTry, tszAPIVer, _tcsxlen(lpszAPIVerTry)))
			{
				//验证权限
				if (0 != st_UserTable.st_UserInfo.nUserLevel)
				{
					Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "permission is failed");
					XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求的API:%s 失败,因为TOKEN权限不足"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
					return false;
				}
				XEngine_AuthorizeHTTP_Try(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
			}
			else if (0 == _tcsxnicmp(lpszAPIVerUser, tszAPIVer, _tcsxlen(lpszAPIVerUser)))
			{
				XEngine_AuthorizeHTTP_User(xhToken, lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
			}
		}
		else
		{
			if (0 == _tcsxnicmp(lpszAPIVerPass, tszAPIVer, _tcsxlen(lpszAPIVerPass)))
			{
				XEngine_AuthorizeHTTP_Pass(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
			}
			else if (0 == _tcsxnicmp(lpszAPIVerCDKey, tszAPIVer, _tcsxlen(lpszAPIVerCDKey)))
			{
				XEngine_AuthorizeHTTP_CDKey(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
			}
		}
	}
	else if (0 == _tcsxnicmp(lpszMethodGet, pSt_HTTPParament->tszHttpMethod, _tcsxlen(lpszMethodGet)))
	{
		int nListCount = 0;
		XCHAR** pptszList;
		XCHAR tszUrlName[128];
		LPCXSTR lpszFuncName = _X("api");
		LPCXSTR lpszAPIVerNotice = _X("notice");
		LPCXSTR lpszAPIVerDCode = _X("dcode");
		memset(tszUrlName, '\0', sizeof(tszUrlName));
		HttpProtocol_ServerHelp_GetParament(pSt_HTTPParament->tszHttpUri, &pptszList, &nListCount, tszUrlName);
		if ((nListCount < 1) || (0 != _tcsxnicmp(lpszFuncName, tszUrlName, _tcsxlen(lpszFuncName))))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "request is failed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
			return false;
		}
		XCHAR tszURLKey[128];
		XCHAR tszURLValue[128];

		memset(tszURLKey, '\0', sizeof(tszURLKey));
		memset(tszURLValue, '\0', sizeof(tszURLValue));

		BaseLib_OperatorString_GetKeyValue(pptszList[0], "=", tszURLKey, tszURLValue);
		if (0 == _tcsxnicmp(lpszAPIVerDCode, tszURLValue, _tcsxlen(lpszAPIVerDCode)))
		{
			XEngine_AuthorizeHTTP_DynamicCode(lpszClientAddr, pptszList, nListCount);
		}
		else
		{
			XEngine_AuthorizeHTTP_Token(lpszClientAddr, pptszList, nListCount);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
	}
	else
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 405, "method not allow");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的方法:%s 不支持"), lpszClientAddr, pSt_HTTPParament->tszHttpMethod);
		return false;
	}
	
	return true;
}