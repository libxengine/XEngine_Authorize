#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Token(LPCXSTR lpszClientAddr, XCHAR** pptszList, int nListCount)
{
	int nSDLen = 4096;
	XCHAR tszSDBuffer[4096] = {};
	XCHAR tszURLKey[128] = {};
	XCHAR tszURLValue[128] = {};
	LPCXSTR lpszAPILogin = _X("login");
	LPCXSTR lpszAPIUPDate = _X("update");
	LPCXSTR lpszAPIClose = _X("close");
	LPCXSTR lpszAPICreate = _X("create");

	BaseLib_String_GetKeyValue(pptszList[0], "=", tszURLKey, tszURLValue);

	if (0 == _tcsxncmp(lpszAPILogin, tszURLValue, _tcsxlen(lpszAPILogin)))
	{
		//http://app.xyry.org:5302/api?function=login&user=123123aa&pass=123123
		XCHAR tszUserName[128] = {};
		XCHAR tszUserPass[128] = {};
		XNETHANDLE xhToken = 0;
		AUTHREG_USERTABLE st_UserTable = {};

		if (nListCount < 3)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "request parament is incorrent");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，登录失败，请求参数不正确"), lpszClientAddr);
			return false;
		}
		BaseLib_String_GetKeyValue(pptszList[1], "=", tszURLKey, tszUserName);
		BaseLib_String_GetKeyValue(pptszList[2], "=", tszURLKey, tszUserPass);
		//是否启用了动态码
		if (st_FunSwitch.bSwitchDCode)
		{
			//+ &token=1000112345&dcode=123456
			if (nListCount != 6)
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "request parament is incorrent");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s,登录失败,登录参数错误,验证码或者TOKEN未填写"), lpszClientAddr);
				return false;
			}
			XCHAR tszTokenStr[128];
			XCHAR tszDCodeStr[128];

			memset(tszTokenStr, '\0', sizeof(tszTokenStr));
			memset(tszDCodeStr, '\0', sizeof(tszDCodeStr));

			BaseLib_String_GetKeyValue(pptszList[4], "=", tszURLKey, tszTokenStr);
			BaseLib_String_GetKeyValue(pptszList[5], "=", tszURLKey, tszDCodeStr);

			xhToken = _ttxoll(tszTokenStr);
			if (!AuthHelp_DynamicCode_Get(xhToken, _ttxoi(tszDCodeStr)))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_DYNAMICCODE, "request parament is incorrent");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s,登录失败,验证动态码失败,句柄:%llu,动态码;%s,错误码:%lX"), lpszClientAddr, xhToken, tszDCodeStr, AuthHelp_GetLastError());
				return false;
			}
		}
		bool bSuccess = false;
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			bSuccess = DBModule_SQLite_UserQuery(tszUserName, &st_UserTable);
		}
		else
		{
			bSuccess = DBModule_MySQL_UserQuery(tszUserName, &st_UserTable);
		}
		if (!bSuccess) 
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_USER, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，登录失败，用户名不存在"), lpszClientAddr);
			return false;
		}
		if ((_tcsxlen(tszUserPass) != _tcsxlen(st_UserTable.st_UserInfo.tszUserPass)) || (0 != _tcsxncmp(tszUserPass, st_UserTable.st_UserInfo.tszUserPass, _tcsxlen(tszUserPass))))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_PASS, "password is incorrent");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，登录失败，密码错误"), lpszClientAddr);
			return false;
		}
		//用户是否存在会话,存在就返回,并且更新TOKEN
		if (Session_Token_GetUser(tszUserName, tszUserPass, &xhToken))
		{
			Protocol_Packet_HttpToken(tszSDBuffer, &nSDLen, xhToken, st_AuthConfig.st_XVerification.nTokenTimeout);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求登录发现会话已经存在,直接返回TOKEN:%lld 成功"), lpszClientAddr, xhToken);
			return true;
		}
		//权限是否正确
		if (0 != st_UserTable.st_UserInfo.nUserLevel)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_BANNED, "User Permission Verification is failed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，用户名：%s，登录失败，客户端权限不足够"), lpszClientAddr, tszUserName);
			return false;
		}
		if (0 == xhToken)
		{
			BaseLib_Handle_Create(&xhToken);
		}
		Session_Token_Insert(xhToken, &st_UserTable.st_UserInfo);
		Protocol_Packet_HttpToken(tszSDBuffer, &nSDLen, xhToken, st_AuthConfig.st_XVerification.nTokenTimeout);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求登录获得TOKEN:%lld 成功,用户级别:%d"), lpszClientAddr, xhToken, st_UserTable.st_UserInfo.nUserLevel);
	}
	else if (0 == _tcsxncmp(lpszAPIUPDate, tszURLValue, _tcsxlen(lpszAPIUPDate)))
	{
		//http://app.xyry.org:5302/api?function=update&token=1000112345
		XCHAR tszUserToken[128];
		memset(tszUserToken, '\0', sizeof(tszUserToken));

		BaseLib_String_GetKeyValue(pptszList[1], "=", tszURLKey, tszUserToken);

		if (!Session_Token_UPDateStr(tszUserToken))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_NOTFOUND, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，更新TOKEN失败，不存在的Token:%s"), lpszClientAddr, tszUserToken);
			return false;
		}
		Protocol_Packet_HttpToken(tszSDBuffer, &nSDLen, _ttxoll(tszUserToken), st_AuthConfig.st_XVerification.nTokenTimeout);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求更新TOKEN:%s 成功"), lpszClientAddr, tszUserToken);
	}
	else if (0 == _tcsxncmp(lpszAPIClose, tszURLValue, _tcsxlen(lpszAPIClose)))
	{
		//http://app.xyry.org:5302/api?function=close&token=1000112345
		XCHAR tszUserToken[128];
		memset(tszUserToken, '\0', sizeof(tszUserToken));

		BaseLib_String_GetKeyValue(pptszList[1], "=", tszURLKey, tszUserToken);
		//主动关闭
		Session_Token_DeleteStr(tszUserToken);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求关闭TOKEN:%s 成功"), lpszClientAddr, tszUserToken);
	}
	else if (0 == _tcsxncmp(lpszAPICreate, tszURLValue, _tcsxlen(lpszAPICreate)))
	{
		//http://127.0.0.1:5302/api?function=create&token=1000112345&timeout=3600
		XCHAR tszTokenStr[128] = {};
		XCHAR tszTimeout[128] = {};

		BaseLib_String_GetKeyValue(pptszList[1], "=", tszURLKey, tszTokenStr);
		BaseLib_String_GetKeyValue(pptszList[2], "=", tszURLKey, tszTimeout);
		
		XENGINE_PROTOCOL_USERINFO st_UserInfo = {};
		if (!Session_Token_GetStr(tszTokenStr, &st_UserInfo))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_NOTFOUND, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，获取TOKEN失败，不存在的Token:%s"), lpszClientAddr, tszTokenStr);
			return false;
		}
		AUTHREG_OAUTHINFO st_OAuthInfo = {};
		_tcsxcpy(st_OAuthInfo.tszUserName, st_UserInfo.tszUserName);

		st_OAuthInfo.nExpirationTime = _ttxoi(tszTimeout);
		BaseLib_Handle_CreateStr(st_OAuthInfo.tszClientID, 16, 1);
		BaseLib_Handle_CreateStr(st_OAuthInfo.tszClientKey);
		BaseLib_Time_TimeToStr(st_OAuthInfo.tszCreateTime);

		if (0 == st_AuthConfig.st_XSql.nDBType)
		{
			DBModule_SQLite_OAuthInsert(&st_OAuthInfo);
		}
		else
		{
			DBModule_MySQL_OAuthInsert(&st_OAuthInfo);
		}
		Protocol_Packet_HttpOAuth(tszSDBuffer, &nSDLen, &st_OAuthInfo);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求创建OAuth的TOKEN:%s 成功"), lpszClientAddr, tszTokenStr);
	}
	return true;
}