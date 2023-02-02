#include "Authorize_Hdr.h"

XHTHREAD CALLBACK XEngine_AuthService_HttpThread(LPVOID lParam)
{
	int nPoolIndex = *(int*)lParam;
	int nThreadPos = nPoolIndex + 1;

	while (bIsRun)
	{
		if (!RfcComponents_HttpServer_EventWaitEx(xhHttpPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_ListClient;
		RfcComponents_HttpServer_GetPoolEx(xhHttpPacket, nThreadPos, &ppSt_ListClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			int nMsgLen = 0;
			CHAR* ptszMsgBuffer = NULL;
			RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParament;

			memset(&st_HTTPParament, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));

			if (RfcComponents_HttpServer_GetMemoryEx(xhHttpPacket, ppSt_ListClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPParament))
			{
				if (st_AuthConfig.st_XCrypto.bEnable)
				{
					TCHAR tszPassword[64];
					TCHAR tszDeBuffer[2048];

					memset(tszPassword, '\0', sizeof(tszPassword));
					memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

					_stprintf(tszPassword, _T("%d"), st_AuthConfig.st_XCrypto.nPassword);
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

BOOL XEngine_Client_HttpTask(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM *pSt_HTTPParament)
{
	int nSDLen = 4096;
	TCHAR tszSDBuffer[4096];
	LPCTSTR lpszMethodPost = _T("POST");
	LPCTSTR lpszMethodGet = _T("GET");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	AUTHREG_BANNED st_Banned;
	memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));

	_tcscpy(st_Banned.tszIPAddr, lpszClientAddr);
	BaseLib_OperatorIPAddr_SegAddr(st_Banned.tszIPAddr);
	//是否在黑名单
	if (Database_SQLite_BannedExist(&st_Banned))
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 423, "ip address is banned");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，登录连接被阻止，IP地址被禁用!"), lpszClientAddr);
		return FALSE;
	}
	if (0 == _tcsnicmp(lpszMethodPost, pSt_HTTPParament->tszHttpMethod, _tcslen(lpszMethodPost)))
	{
		TCHAR tszAPIType[64];
		TCHAR tszAPIVer[64];
		TCHAR tszAPIName[64];
		XNETHANDLE xhToken = 0;
		LPCTSTR lpszAPIType = _T("auth");
		LPCTSTR lpszAPIVerClient = _T("client");
		LPCTSTR lpszAPIVerSerial = _T("serial");
		LPCTSTR lpszAPIVerUser = _T("user");
		LPCTSTR lpszAPIVerPass = _T("pass");
		LPCTSTR lpszAPIVerSwitch = _T("switch");
		LPCTSTR lpszAPIVerBanned = _T("banned");
		LPCTSTR lpszAPIVerCDKey = _T("cdkey");

		memset(tszAPIType, '\0', sizeof(tszAPIType));
		memset(tszAPIVer, '\0', sizeof(tszAPIVer));
		memset(tszAPIName, '\0', sizeof(tszAPIName));

		RfcComponents_HttpHelp_GetUrlApi(pSt_HTTPParament->tszHttpUri, tszAPIType, tszAPIVer, tszAPIName);
		if (0 != _tcsnicmp(lpszAPIType, tszAPIType, _tcslen(lpszAPIType)))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "request url is incorrent");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 不支持"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
			return FALSE;
		}

		if (0 == _tcsnicmp(lpszAPIVerClient, tszAPIVer, _tcslen(lpszAPIVerClient)))
		{
			AUTHREG_USERTABLE st_UserTable;
			memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
			//验证权限
			Protocol_Parse_HttpParseToken(lpszMsgBuffer, nMsgLen, &xhToken);
			if (!Session_Token_Get(xhToken, &st_UserTable))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "Unauthorized");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 失败,因为没有经过验证"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return FALSE;
			}
			if (0 != st_UserTable.st_UserInfo.nUserLevel)
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "permission is failed");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 失败,因为TOKEN权限不足"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return FALSE;
			}
			XEngine_AuthorizeHTTP_Client(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
		}
		else if (0 == _tcsnicmp(lpszAPIVerSerial, tszAPIVer, _tcslen(lpszAPIVerSerial)))
		{
			AUTHREG_USERTABLE st_UserTable;
			memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
			//验证权限
			Protocol_Parse_HttpParseToken(lpszMsgBuffer, nMsgLen, &xhToken);
			if (!Session_Token_Get(xhToken, &st_UserTable))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "Unauthorized");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 失败,因为没有经过验证"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return FALSE;
			}
			if (0 != st_UserTable.st_UserInfo.nUserLevel)
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "permission is failed");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 失败,因为TOKEN权限不足"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return FALSE;
			}
			XEngine_AuthorizeHTTP_Serial(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
		}
		else if (0 == _tcsnicmp(lpszAPIVerUser, tszAPIVer, _tcslen(lpszAPIVerUser)))
		{
			XEngine_AuthorizeHTTP_User(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
		}
		else if (0 == _tcsnicmp(lpszAPIVerPass, tszAPIVer, _tcslen(lpszAPIVerPass)))
		{
			XEngine_AuthorizeHTTP_Pass(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
		}
		else if (0 == _tcsnicmp(lpszAPIVerSwitch, tszAPIVer, _tcslen(lpszAPIVerSwitch)))
		{
			AUTHREG_USERTABLE st_UserTable;
			memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
			//验证权限
			Protocol_Parse_HttpParseToken(lpszMsgBuffer, nMsgLen, &xhToken);
			if (!Session_Token_Get(xhToken, &st_UserTable))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "Unauthorized");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 失败,因为没有经过验证"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return FALSE;
			}
			if (0 != st_UserTable.st_UserInfo.nUserLevel)
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "permission is failed");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 失败,因为TOKEN权限不足"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return FALSE;
			}
			XEngine_AuthorizeHTTP_Switch(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
		}
		else if (0 == _tcsnicmp(lpszAPIVerBanned, tszAPIVer, _tcslen(lpszAPIVerBanned)))
		{
			AUTHREG_USERTABLE st_UserTable;
			memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
			//验证权限
			Protocol_Parse_HttpParseToken(lpszMsgBuffer, nMsgLen, &xhToken);
			if (!Session_Token_Get(xhToken, &st_UserTable))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "Unauthorized");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 失败,因为没有经过验证"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return FALSE;
			}
			if (0 != st_UserTable.st_UserInfo.nUserLevel)
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "permission is failed");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 失败,因为TOKEN权限不足"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return FALSE;
			}
			XEngine_AuthorizeHTTP_Banned(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
		}
		else if (0 == _tcsnicmp(lpszAPIVerCDKey, tszAPIVer, _tcslen(lpszAPIVerCDKey)))
		{
			XEngine_AuthorizeHTTP_CDKey(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
		}
	}
	else if (0 == _tcsnicmp(lpszMethodGet, pSt_HTTPParament->tszHttpMethod, _tcslen(lpszMethodGet)))
	{
		int nListCount = 0;
		TCHAR** pptszList;
		TCHAR tszUrlName[128];
		LPCTSTR lpszFuncName = _T("api");

		memset(tszUrlName, '\0', sizeof(tszUrlName));
		RfcComponents_HttpHelp_GetParament(pSt_HTTPParament->tszHttpUri, &pptszList, &nListCount, tszUrlName);
		if ((nListCount < 1) || (0 != _tcsnicmp(lpszFuncName, tszUrlName, _tcslen(lpszFuncName))))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "request is failed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
			return FALSE;
		}
		XEngine_AuthorizeHTTP_Token(lpszClientAddr, pptszList, nListCount);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
	}
	else
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 405, "method not allow");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,发送的方法:%s 不支持"), lpszClientAddr, pSt_HTTPParament->tszHttpMethod);
		return FALSE;
	}
	
	return TRUE;
}