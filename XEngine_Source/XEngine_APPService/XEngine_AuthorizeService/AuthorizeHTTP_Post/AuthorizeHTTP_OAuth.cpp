#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_OAuth(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	int nSDLen = 4096;
	XCHAR tszSDBuffer[4096] = {};
	XCHAR tszURLKey[128] = {};
	XCHAR tszURLValue[128] = {};
	LPCXSTR lpszAPIToken = _X("token");   //更新，创建
	LPCXSTR lpszAPIRevoke = _X("revoke");  //撤销
	LPCXSTR lpszAPIIntrospect = _X("introspect");   //检查

	if (0 == _tcsxncmp(lpszAPIToken, lpszAPIName, _tcsxlen(lpszAPIToken)))
	{
		//http://127.0.0.1:5302/api/oauth/token
		VERIFICATION_OAUTHINFO st_VerificationInfo = {};
		AUTHREG_OAUTHINFO st_OAuthInfo = {};
		XCHAR tszFreshToken[XPATH_MIN] = {};

		Verification_OAuth_Parse(&st_VerificationInfo, NULL, lpszMSGBuffer);
		_tcsxcpy(st_OAuthInfo.tszClientID, st_VerificationInfo.tszClientID);
		_tcsxcpy(st_OAuthInfo.tszClientKey, st_VerificationInfo.tszClientSecert);

		_tcsxcpy(tszFreshToken, st_VerificationInfo.tszClientRefresh);

		bool bRet = false;
		if (0 == st_AuthConfig.st_XSql.nDBType)
		{
			bRet = DBModule_SQLite_OAuthQuery(&st_OAuthInfo);
		}
		else
		{
			bRet = DBModule_MySQL_OAuthQuery(&st_OAuthInfo);
		}
		if (!bRet)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "request info is correct,maybe client id or key is incorrect");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求创建OAuth的TOKEN失败,查询用户信息id:%s key:%s失败：%lX"), lpszClientAddr, st_OAuthInfo.tszClientID, st_OAuthInfo.tszClientKey, DBModule_GetLastError());
			return false;
		}
		//是否为刷新TOKEN
		if (_tcsxlen(tszFreshToken) > 0)
		{
			//如果是
			if (0 == _tcsxnicmp(tszFreshToken, st_VerificationInfo.tszClientRefresh, _tcsxlen(st_VerificationInfo.tszClientRefresh)))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE, "token verification failure");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求刷新OAuth的TOKEN失败,查询用户fresh:%s token:%s 不匹配"), lpszClientAddr, tszFreshToken, st_VerificationInfo.tszClientRefresh);
				return false;
			}
		}
		BaseLib_Handle_CreateStr(st_OAuthInfo.tszTokenStr, 32);
		BaseLib_Handle_CreateStr(st_OAuthInfo.tszUPToken, 32);
		if (0 == st_AuthConfig.st_XSql.nDBType)
		{
			bRet = DBModule_SQLite_OAuthUPDate(&st_OAuthInfo);
		}
		else
		{
			bRet = DBModule_MySQL_OAuthUPDate(&st_OAuthInfo);
		}
		if (!bRet)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_SERVER, "internal server failure");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求创建OAuth的TOKEN失败,内部错误：%lX"), lpszClientAddr, DBModule_GetLastError());
			return false;
		}
		Protocol_Packet_HttpOAuth2(tszSDBuffer, &nSDLen, &st_OAuthInfo);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求创建OAuth Token:%s 成功"), lpszClientAddr, st_OAuthInfo.tszTokenStr);
	}
	else if (0 == _tcsxncmp(lpszAPIRevoke, lpszAPIName, _tcsxlen(lpszAPIRevoke)))
	{
		//http://127.0.0.1:5302/api/oauth/revoke
		VERIFICATION_OAUTHINFO st_VerificationInfo = {};
		AUTHREG_OAUTHINFO st_OAuthInfo = {};

		Verification_OAuth_Parse(&st_VerificationInfo, NULL, lpszMSGBuffer);
		_tcsxcpy(st_OAuthInfo.tszTokenStr, st_VerificationInfo.tszTokenStr);

		bool bRet = false;
		if (0 == st_AuthConfig.st_XSql.nDBType)
		{
			bRet = DBModule_SQLite_OAuthDelete(&st_OAuthInfo);
		}
		else
		{
			bRet = DBModule_MySQL_OAuthDelete(&st_OAuthInfo);
		}
		if (!bRet)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "request info is correct,maybe token is incorrect");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求删除OAuth的TOKEN失败,可能TOKEN:%s 错误：%lX"), lpszClientAddr, st_OAuthInfo.tszTokenStr, DBModule_GetLastError());
			return false;
		}
		
		Protocol_Packet_HttpOAuth2(tszSDBuffer, &nSDLen, &st_OAuthInfo);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求撤销删除OAuth的token成功,撤销的token:%s"), lpszClientAddr, st_OAuthInfo.tszTokenStr);
	}
	
	return true;
}