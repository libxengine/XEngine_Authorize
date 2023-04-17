#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Token(LPCXSTR lpszClientAddr, XCHAR** pptszList, int nListCount)
{
	int nSDLen = 4096;
	XCHAR tszSDBuffer[4096];
	XCHAR tszURLKey[128];
	XCHAR tszURLValue[128];
	LPCXSTR lpszAPILogin = _X("login");
	LPCXSTR lpszAPIUPDate = _X("update");
	LPCXSTR lpszAPIClose = _X("close");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszURLKey, '\0', sizeof(tszURLKey));
	memset(tszURLValue, '\0', sizeof(tszURLValue));
	BaseLib_OperatorString_GetKeyValue(pptszList[0], "=", tszURLKey, tszURLValue);

	if (0 == _tcsxnicmp(lpszAPILogin, tszURLValue, _tcsxlen(lpszAPILogin)))
	{
		//http://app.xyry.org:5302/api?function=login&user=123123aa&pass=123123&device=1
		XCHAR tszUserName[128];
		XCHAR tszUserPass[128];
		XCHAR tszDeviceType[128];
		XNETHANDLE xhToken = 0;
		AUTHREG_USERTABLE st_UserTable;

		memset(tszUserName, '\0', sizeof(tszUserName));
		memset(tszUserPass, '\0', sizeof(tszUserPass));
		memset(tszDeviceType, '\0', sizeof(tszDeviceType));
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		if (nListCount < 4)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "request parament is incorrent");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，登录失败，请求参数不正确"), lpszClientAddr);
			return false;
		}
		BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszURLKey, tszUserName);
		BaseLib_OperatorString_GetKeyValue(pptszList[2], "=", tszURLKey, tszUserPass);
		BaseLib_OperatorString_GetKeyValue(pptszList[3], "=", tszURLKey, tszDeviceType);
		//是否启用了动态码
		if (st_FunSwitch.bSwitchDCode)
		{
			//+ &token=1000112345&dcode=123456
			if (nListCount != 6)
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "request parament is incorrent");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s,登录失败,登录参数错误,验证码或者TOKEN未填写"), lpszClientAddr);
				return false;
			}
			XCHAR tszTokenStr[128];
			XCHAR tszDCodeStr[128];

			memset(tszTokenStr, '\0', sizeof(tszTokenStr));
			memset(tszDCodeStr, '\0', sizeof(tszDCodeStr));

			BaseLib_OperatorString_GetKeyValue(pptszList[4], "=", tszURLKey, tszTokenStr);
			BaseLib_OperatorString_GetKeyValue(pptszList[5], "=", tszURLKey, tszDCodeStr);

			xhToken = _ttxoll(tszTokenStr);
			if (!AuthHelp_DynamicCode_Get(xhToken, _ttxoi(tszDCodeStr)))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "request parament is incorrent");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s,登录失败,验证动态码失败,句柄:%llu,动态码;%s,错误码:%lX"), lpszClientAddr, xhToken, tszDCodeStr, AuthHelp_GetLastError());
				return false;
			}
		}
		//是否使用了第三方验证
		if (st_AuthConfig.st_XLogin.bPassAuth)
		{
			//启用三方验证
			int nHTTPCode = 0;
			int nHTTPLen = 0;
			XCHAR* ptszMsgBuffer = NULL;
			XENGINE_PROTOCOL_USERAUTH st_AuthProtocol;
			memset(&st_AuthProtocol, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

			_tcsxcpy(st_AuthProtocol.tszUserName, tszUserName);
			_tcsxcpy(st_AuthProtocol.tszUserPass, tszUserPass);
			st_AuthProtocol.enDeviceType = (ENUM_PROTOCOLDEVICE_TYPE)_ttxoi(tszDeviceType);

			Protocol_Packet_HttpUserPass(tszSDBuffer, &nSDLen, &st_AuthProtocol);
			APIClient_Http_Request(_X("POST"), st_AuthConfig.st_XLogin.st_PassUrl.tszPassLogin, tszSDBuffer, &nHTTPCode, &ptszMsgBuffer, &nHTTPLen);
			if (200 != nHTTPCode)
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，用户名：%s，登录失败，三方验证失败,错误码:%d"), lpszClientAddr, st_AuthProtocol.tszUserName, nHTTPCode);
				return false;
			}
			Protocol_Parse_HttpParseTable(ptszMsgBuffer, nHTTPLen, &st_UserTable);
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		else
		{
			if (!Database_SQLite_UserQuery(tszUserName, &st_UserTable))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，登录失败，用户名不存在"), lpszClientAddr);
				return false;
			}
			if ((_tcsxlen(tszUserPass) != _tcsxlen(st_UserTable.st_UserInfo.tszUserPass)) || (0 != _tcsxncmp(tszUserPass, st_UserTable.st_UserInfo.tszUserPass, _tcsxlen(tszUserPass))))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "password is incorrent");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，登录失败，密码错误"), lpszClientAddr);
				return false;
			}
		}
		//用户是否存在会话,存在就返回,并且更新TOKEN
		if (Session_Token_GetUser(tszUserName, tszUserPass, &xhToken))
		{
			if (st_AuthConfig.st_XLogin.bMultiLogin)
			{
				Protocol_Packet_HttpToken(tszSDBuffer, &nSDLen, xhToken, st_AuthConfig.st_XVerification.nTokenTimeout);
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求登录发现会话已经存在,直接返回TOKEN:%lld 成功"), lpszClientAddr, xhToken);
				return true;
			}
			else
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "user was login");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求登录发现会话已经存在,服务器不允许重读登录"), lpszClientAddr);
				return false;
			}
		}
		//是否被封禁
		if (-1 == st_UserTable.st_UserInfo.nUserLevel)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "User was banned");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，用户名：%s，登录失败，客户端已被封禁"), lpszClientAddr, tszUserName);
			return false;
		}
		//处理权限
		if (st_UserTable.st_UserInfo.nUserLevel > 0)
		{
			if (!st_FunSwitch.bSwitchLogin)
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 503, "Function does not to enable");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，登录失败，因为登录功能被服务器关闭!"), lpszClientAddr);
				return false;
			}
			//普通用户
			if (!st_AuthConfig.st_XLogin.bHTTPAuth)
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "User permission error");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，登录失败，客户端权限不足"), lpszClientAddr);
				return false;
			}
			//是否已经登录
			XCHAR tszClientAddr[128];
			if (Session_Authorize_GetAddrForUser(tszUserName, tszClientAddr))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "User was login");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，用户名：%s，登录失败，用户名已经登录"), lpszClientAddr, tszUserName);
				return false;
			}
			//分析充值类型
			if ((ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_UserTable.enSerialType) || ('0' == st_UserTable.tszLeftTime[0]))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "User not time");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，用户名：%s，登录失败，客户端类型错误"), lpszClientAddr, tszUserName);
				return false;
			}
			st_UserTable.enDeviceType = (ENUM_PROTOCOLDEVICE_TYPE)_ttxoi(tszDeviceType);
			if (!Session_Authorize_Insert(lpszClientAddr, &st_UserTable, XENGINE_AUTH_APP_NETTYPE_HTTP))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 500, "server is error");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，用户名：%s，登录失败，插入会话管理失败,错误:%lX"), lpszClientAddr, tszUserName);
				return false;
			}
		}
		if (0 == xhToken)
		{
			BaseLib_OperatorHandle_Create(&xhToken);
		}
		Session_Token_Insert(xhToken, &st_UserTable, st_UserTable.st_UserInfo.nUserLevel > 0 ? st_AuthConfig.st_XLogin.nHTTPAuthTime : 0);
		Protocol_Packet_HttpToken(tszSDBuffer, &nSDLen, xhToken, st_AuthConfig.st_XVerification.nTokenTimeout);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求登录获得TOKEN:%lld 成功,用户级别:%d"), lpszClientAddr, xhToken, st_UserTable.st_UserInfo.nUserLevel);
	}
	else if (0 == _tcsxnicmp(lpszAPIUPDate, tszURLValue, _tcsxlen(lpszAPIUPDate)))
	{
		//http://app.xyry.org:5302/api?function=update&token=1000112345
		XCHAR tszUserToken[128];
		memset(tszUserToken, '\0', sizeof(tszUserToken));

		BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszURLKey, tszUserToken);

		if (!Session_Token_UPDate(_ttxoll(tszUserToken)))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，更新TOKEN失败，不存在的Token:%s"), lpszClientAddr, tszUserToken);
			return false;
		}
		Protocol_Packet_HttpToken(tszSDBuffer, &nSDLen, _ttxoll(tszUserToken), st_AuthConfig.st_XVerification.nTokenTimeout);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求更新TOKEN:%s 成功"), lpszClientAddr, tszUserToken);
	}
	else if (0 == _tcsxnicmp(lpszAPIClose, tszURLValue, _tcsxlen(lpszAPIClose)))
	{
		//http://app.xyry.org:5302/api?function=close&token=1000112345
		XCHAR tszUserToken[128];
		AUTHREG_USERTABLE st_UserTable;

		memset(tszUserToken, '\0', sizeof(tszUserToken));
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszURLKey, tszUserToken);
		//主动关闭,更新用户时间
		Session_Token_Get(_ttxoll(tszUserToken), &st_UserTable);
		if (st_UserTable.st_UserInfo.nUserLevel > 1)
		{
			//如果权限是普通用户
			AUTHREG_PROTOCOL_TIME st_AuthTime;
			AUTHSESSION_NETCLIENT st_NETClient;
			memset(&st_AuthTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
			memset(&st_NETClient, '\0', sizeof(AUTHSESSION_NETCLIENT));
			//需要设置时间并且关闭会话
			if (Session_Authorize_GetClientForUser(st_UserTable.st_UserInfo.tszUserName, &st_NETClient))
			{
				st_AuthTime.nTimeLeft = st_NETClient.nLeftTime;
				st_AuthTime.nTimeONLine = st_NETClient.nOnlineTime;
				st_AuthTime.enSerialType = st_NETClient.st_UserTable.enSerialType;
				_tcsxcpy(st_AuthTime.tszUserName, st_UserTable.st_UserInfo.tszUserName);
				_tcsxcpy(st_AuthTime.tszLeftTime, st_NETClient.tszLeftTime);
				_tcsxcpy(st_AuthTime.tszUserAddr, st_NETClient.tszClientAddr);
				//是否需要通知
				if (st_AuthConfig.st_XLogin.bHTTPAuth)
				{
					int nSDLen = 0;
					XCHAR tszSDBuffer[MAX_PATH];
					memset(tszSDBuffer, '\0', MAX_PATH);

					Protocol_Packet_HttpUserTime(tszSDBuffer, &nSDLen, &st_AuthTime);
					APIClient_Http_Request(_X("POST"), st_AuthConfig.st_XLogin.st_PassUrl.tszPassLogout, tszSDBuffer);
				}
				Database_SQLite_UserLeave(&st_AuthTime);
			}
			Session_Authorize_CloseClient(st_UserTable.st_UserInfo.tszUserName);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Token：%s，用户名：%s，主动关闭,在线时长:%d"), tszUserToken, st_UserTable.st_UserInfo.tszUserName, st_AuthTime.nTimeONLine);
		}
		else
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求关闭TOKEN:%s 成功"), lpszClientAddr, tszUserToken);
		}
		Session_Token_Delete(_ttxoll(tszUserToken));
	}
	return true;
}