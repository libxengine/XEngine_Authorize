#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_OAuth(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	int nSDLen = 4096;
	XCHAR tszSDBuffer[4096] = {};
	XCHAR tszURLKey[128] = {};
	XCHAR tszURLValue[128] = {};
	LPCXSTR lpszAPICreate = _X("create");
	LPCXSTR lpszAPIUPDate = _X("update");
	LPCXSTR lpszAPIClose = _X("close");

	if (0 == _tcsxncmp(lpszAPICreate, lpszAPIName, _tcsxlen(lpszAPICreate)))
	{
		//http://app.xyry.org:5302/api/oauth/create
		VERIFICATION_OAUTHINFO st_VerificationInfo = {};
		AUTHREG_OAUTHINFO st_OAuthInfo = {};

		Verification_OAuth_Parse(&st_VerificationInfo, NULL, lpszMSGBuffer);
		_tcsxcpy(st_OAuthInfo.tszClientID, st_VerificationInfo.tszClientID);
		_tcsxcpy(st_OAuthInfo.tszClientKey, st_VerificationInfo.tszClientSecert);

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
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求更新OAuth的TOKEN失败,内部错误：%lX"), lpszClientAddr, DBModule_GetLastError());
			return false;
		}
		Protocol_Packet_HttpOAuth2(tszSDBuffer, &nSDLen, &st_OAuthInfo);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s"), lpszClientAddr);
	}
	
	return true;
}