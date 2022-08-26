#include "../Authorize_Hdr.h"

BOOL XEngine_AuthorizeHTTP_Token(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR** pptszList, int nListCount)
{
	int nSDLen = 4096;
	TCHAR tszSDBuffer[4096];
	TCHAR tszURLKey[128];
	TCHAR tszURLValue[128];
	LPCTSTR lpszAPILogin = _T("login");
	LPCTSTR lpszAPIUPDate = _T("update");
	LPCTSTR lpszAPIClose = _T("close");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszURLKey, '\0', sizeof(tszURLKey));
	memset(tszURLValue, '\0', sizeof(tszURLValue));
	BaseLib_OperatorString_GetKeyValue(pptszList[0], "=", tszURLKey, tszURLValue);

	if (0 == _tcsnicmp(lpszAPILogin, tszURLValue, _tcslen(lpszAPILogin)))
	{
		//http://app.xyry.org:5302/api?function=login&user=123123aa&pass=123123
		TCHAR tszUserName[128];
		TCHAR tszUserPass[128];
		AUTHREG_USERTABLE st_UserTable;

		memset(tszUserName, '\0', sizeof(tszUserName));
		memset(tszUserPass, '\0', sizeof(tszUserPass));
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszURLKey, tszUserName);
		BaseLib_OperatorString_GetKeyValue(pptszList[2], "=", tszURLKey, tszUserPass);

		//禁止重复登录
		if (!Session_Token_GetUser(tszUserName, tszUserPass))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 423, "user locked");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，登录失败，用户:%s ,已经登录,无法继续"), lpszClientAddr, tszUserName);
			return FALSE;
		}

		if (!Database_SQLite_UserQuery(tszUserName, &st_UserTable))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，登录失败，用户名不存在"), lpszClientAddr);
			return FALSE;
		}
		if ((_tcslen(tszUserPass) != _tcslen(st_UserTable.st_UserInfo.tszUserPass)) || (0 != _tcsncmp(tszUserPass, st_UserTable.st_UserInfo.tszUserPass, _tcslen(tszUserPass))))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "password is incorrent");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，登录失败，密码错误"), lpszClientAddr);
			return FALSE;
		}
		//权限必须足够
		if (0 != st_UserTable.st_UserInfo.nUserLevel)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "User permission error");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，登录失败，客户端权限不足"), lpszClientAddr);
			return FALSE;
		}
		XNETHANDLE xhToken = 0;
		BaseLib_OperatorHandle_Create(&xhToken);
		Session_Token_Insert(xhToken, &st_UserTable);
		Protocol_Packet_HttpToken(tszSDBuffer, &nSDLen, xhToken, st_AuthConfig.st_XVerification.nTokenTimeout);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求登录获得TOKEN:%lld 成功"), lpszClientAddr, xhToken);
	}
	else if (0 == _tcsnicmp(lpszAPIUPDate, tszURLValue, _tcslen(lpszAPIUPDate)))
	{
		//http://app.xyry.org:5302/api?function=update&token=1000112345
		TCHAR tszUserToken[128];
		memset(tszUserToken, '\0', sizeof(tszUserToken));

		BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszURLKey, tszUserToken);

		if (!Session_Token_UPDate(_ttoi64(tszUserToken)))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，更新TOKEN失败，不存在的Token:%s"), lpszClientAddr, tszUserToken);
			return FALSE;
		}
		Protocol_Packet_HttpToken(tszSDBuffer, &nSDLen, _ttoi64(tszUserToken), st_AuthConfig.st_XVerification.nTokenTimeout);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求更新TOKEN:%s 成功"), lpszClientAddr, tszUserToken);
	}
	else if (0 == _tcsnicmp(lpszAPIClose, tszURLValue, _tcslen(lpszAPIClose)))
	{
		//http://app.xyry.org:5302/api?function=close&token=1000112345
		TCHAR tszUserToken[128];
		memset(tszUserToken, '\0', sizeof(tszUserToken));

		BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszURLKey, tszUserToken);
		Session_Token_Delete(_ttoi64(tszUserToken));
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求关闭TOKEN:%s 成功"), lpszClientAddr, tszUserToken);
	}
	return TRUE;
}