#include "Authorize_Hdr.h"

XHTHREAD XCALLBACK XEngine_AuthService_HttpThread(XPVOID lParam)
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
			int nHdrCount = 0;
			XCHAR** ppszListHdr = NULL;
			XCHAR* ptszMsgBuffer = NULL;
			RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParament = {};

			if (HttpProtocol_Server_GetMemoryEx(xhHttpPacket, ppSt_ListClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPParament, &ppszListHdr, &nHdrCount))
			{
				if (st_AuthConfig.st_XCrypto.bEnable)
				{
					XCHAR tszPassword[64];
					XCHAR tszDeBuffer[2048];

					memset(tszPassword, '\0', sizeof(tszPassword));
					memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

					_xstprintf(tszPassword, _X("%d"), st_AuthConfig.st_XCrypto.nPassword);
					Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszDeBuffer, tszPassword);
					XEngine_Client_HttpTask(ppSt_ListClient[i]->tszClientAddr, tszDeBuffer, nMsgLen, &st_HTTPParament, ppszListHdr, nHdrCount);
				}
				else
				{
					XEngine_Client_HttpTask(ppSt_ListClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &st_HTTPParament, ppszListHdr, nHdrCount);
				}
			}
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			BaseLib_Memory_Free((XPPPMEM)&ppszListHdr, nHdrCount);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	return 0;
}

bool XEngine_Client_HttpTask(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParament, XCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 4096;
	XCHAR tszSDBuffer[4096];
	LPCXSTR lpszMethodPost = _X("POST");
	LPCXSTR lpszMethodGet = _X("GET");
	LPCXSTR lpszMethodOPtion = _X("OPTION");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	AUTHREG_BANNED st_Banned;
	memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));
	_tcsxcpy(st_Banned.tszIPAddr, lpszClientAddr);
	APIAddr_IPAddr_SegAddr(st_Banned.tszIPAddr);

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
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_BANNED, "ip address is banned");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，登录连接被阻止，用户名或IP地址被禁用!"), lpszClientAddr);
		return false;
	}
	//HTTP验证
	if (st_AuthConfig.st_XApiVer.bEnable)
	{
		int nVType = 0;
		RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};

		st_HDRParam.nHttpCode = 401;
		st_HDRParam.bIsClose = true;
		st_HDRParam.bAuth = true;
		//打包验证信息
		int nHDRLen = 0;
		XCHAR tszHDRBuffer[XPATH_MAX] = {};
		if (1 == st_AuthConfig.st_XApiVer.nVType)
		{
			Verification_HTTP_BasicServerPacket(tszHDRBuffer, &nHDRLen);
		}
		else
		{
			XCHAR tszNonceStr[64] = {};
			XCHAR tszOpaqueStr[64] = {};
			Verification_HTTP_DigestServerPacket(tszHDRBuffer, &nHDRLen, tszNonceStr, tszOpaqueStr);
		}
		//后去验证方法
		if (!Verification_HTTP_GetType(pptszListHdr, nHdrCount, &nVType))
		{
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
			NetCore_TCPXCore_SendEx(xhHttpSocket, lpszClientAddr, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,验证方式:%d,错误:%lX"), lpszClientAddr, st_AuthConfig.st_XApiVer.nVType, Verification_GetLastError());
			return false;
		}
		//验证方式是否一致
		if (st_AuthConfig.st_XApiVer.nVType != nVType)
		{
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
			NetCore_TCPXCore_SendEx(xhHttpSocket, lpszClientAddr, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,验证方式错误,请求:%d,需求:%d"), lpszClientAddr, nVType, st_AuthConfig.st_XApiVer.nVType);
			return false;
		}
		bool bRet = false;
		//通过什么方法获得用户密码
		if (_tcsxlen(st_AuthConfig.st_XApiVer.tszAPIUrl) > 0)
		{
			int nHTTPCode = 0;
			int nMSGLen = 0;
			XCLIENT_APIHTTP st_APIHttp = {};
			XCHAR* ptszMSGBuffer = NULL;
			if (!APIClient_Http_Request(_X("GET"), st_AuthConfig.st_XApiVer.tszAPIUrl, NULL, &nHTTPCode, &ptszMSGBuffer, &nMSGLen, NULL, NULL, &st_APIHttp))
			{
				st_HDRParam.nHttpCode = 500;
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE, "api server is down,cant verification");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,GET请求验证服务:%s 失败,错误码:%lX"), lpszClientAddr, st_AuthConfig.st_XApiVer.tszAPIUrl, APIClient_GetLastError());
				return false;
			}
			if (200 != nHTTPCode)
			{
				st_HDRParam.nHttpCode = 500;
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE, "api server is down,cant verification");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,GET请求验证服务:%s 失败,错误:%d"), lpszClientAddr, st_AuthConfig.st_XApiVer.tszAPIUrl, nHTTPCode);
				return false;
			}
			XENGINE_PROTOCOL_USERAUTHEX st_UserAuth = {};
			if (!Protocol_Parse_HttpParseAuth(ptszMSGBuffer, nMsgLen, &st_UserAuth))
			{
				st_HDRParam.nHttpCode = 500;
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE, "api server reply failure,cant verification");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,返回内容:%s 错误,无法继续"), lpszClientAddr, ptszMSGBuffer);
				BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);
				return false;
			}
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);

			if (1 == nVType)
			{
				bRet = Verification_HTTP_Basic(st_UserAuth.tszUserName, st_UserAuth.tszUserPass, pptszListHdr, nHdrCount);
			}
			else if (2 == nVType)
			{
				bRet = Verification_HTTP_Digest(st_UserAuth.tszUserName, st_UserAuth.tszUserPass, pSt_HTTPParament->tszHttpMethod, pptszListHdr, nHdrCount);
			}
		}
		else
		{
			if (1 == nVType)
			{
				bRet = Verification_HTTP_Basic(st_AuthConfig.st_XApiVer.tszUserName, st_AuthConfig.st_XApiVer.tszUserPass, pptszListHdr, nHdrCount);
			}
			else if (2 == nVType)
			{
				bRet = Verification_HTTP_Digest(st_AuthConfig.st_XApiVer.tszUserName, st_AuthConfig.st_XApiVer.tszUserPass, pSt_HTTPParament->tszHttpMethod, pptszListHdr, nHdrCount);
			}
		}
		
		if (!bRet)
		{
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
			NetCore_TCPXCore_SendEx(xhHttpSocket, lpszClientAddr, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,验证处理错误,可能用户密码登信息不匹配,类型:%d"), lpszClientAddr, nVType);
			return false;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,HTTP验证类型:%d 通过"), lpszClientAddr, nVType);
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
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "request url is incorrent");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求的API:%s 不支持"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
			return false;
		}
		AUTHREG_USERTABLE st_UserTable = {};
		//得到TOKEN
		if (Protocol_Parse_HttpParseToken(lpszMsgBuffer, nMsgLen, &xhToken))
		{
			if (!Session_Token_Get(xhToken, &st_UserTable))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE, "Unauthorized");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求的API:%s 失败,因为没有经过验证"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return false;
			}

			if (0 == _tcsxnicmp(lpszAPIVerClient, tszAPIVer, _tcsxlen(lpszAPIVerClient)))
			{
				//验证权限
				if (0 != st_UserTable.st_UserInfo.nUserLevel)
				{
					Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE, "permission is failed");
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
					Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE, "permission is failed");
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
					Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE, "permission is failed");
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
					Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE, "permission is failed");
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
					Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE, "permission is failed");
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
					Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE, "permission is failed");
					XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求的API:%s 失败,因为TOKEN权限不足"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
					return false;
				}
				XEngine_AuthorizeHTTP_Try(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
			}
			else
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "reqeust api is not support");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求的API:%s 失败,因为不被支持"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return false;
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
			else if (0 == _tcsxnicmp(lpszAPIVerUser, tszAPIVer, _tcsxlen(lpszAPIVerUser)))
			{
				XEngine_AuthorizeHTTP_User(xhToken, lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
			}
			else
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "reqeust api is not support");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求的API:%s 失败,因为不被支持"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return false;
			}
		}
	}
	else if (0 == _tcsxnicmp(lpszMethodGet, pSt_HTTPParament->tszHttpMethod, _tcsxlen(lpszMethodGet)))
	{
		int nListCount = 0;
		XCHAR** pptszList;
		XCHAR tszUrlName[128];
		LPCXSTR lpszFuncName = _X("api");
		LPCXSTR lpszAPIVerDCode = _X("dcode");
		LPCXSTR lpszAPIVerTime = _X("time");
		LPCXSTR lpszAPIVerNotice = _X("notice");

		memset(tszUrlName, '\0', sizeof(tszUrlName));
		HttpProtocol_ServerHelp_GetParament(pSt_HTTPParament->tszHttpUri, &pptszList, &nListCount, tszUrlName);
		if ((nListCount < 1) || (0 != _tcsxnicmp(lpszFuncName, tszUrlName, _tcsxlen(lpszFuncName))))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "request is failed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			BaseLib_Memory_Free((XPPPMEM)&pptszList, nListCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
			return false;
		}
		XCHAR tszURLKey[128];
		XCHAR tszURLValue[128];

		memset(tszURLKey, '\0', sizeof(tszURLKey));
		memset(tszURLValue, '\0', sizeof(tszURLValue));

		BaseLib_String_GetKeyValue(pptszList[0], "=", tszURLKey, tszURLValue);
		if (0 == _tcsxnicmp(lpszAPIVerDCode, tszURLValue, _tcsxlen(lpszAPIVerDCode)) || 0 == _tcsxnicmp(lpszAPIVerTime, tszURLValue, _tcsxlen(lpszAPIVerTime)) ||
			0 == _tcsxnicmp(lpszAPIVerNotice, tszURLValue, _tcsxlen(lpszAPIVerNotice)))
		{
			XEngine_AuthorizeHTTP_GetTask(lpszClientAddr, pptszList, nListCount);
		}
		else
		{
			XEngine_AuthorizeHTTP_Token(lpszClientAddr, pptszList, nListCount);
		}
		BaseLib_Memory_Free((XPPPMEM)&pptszList, nListCount);
	}
	else if (0 == _tcsxnicmp(lpszMethodOPtion, pSt_HTTPParament->tszHttpMethod, _tcsxlen(lpszMethodOPtion)))
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,发送的OPTION方法处理成功.此为心跳请求"), lpszClientAddr);
	}
	else
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "method not allow");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的方法:%s 不支持"), lpszClientAddr, pSt_HTTPParament->tszHttpMethod);
		return false;
	}
	
	return true;
}