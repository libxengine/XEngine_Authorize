#include "../Authorize_Hdr.h"

XBOOL XEngine_AuthorizeHTTP_Pass(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 4096;
	XCHAR tszSDBuffer[4096];
	LPCXSTR lpszAPILogin = _T("login");
	LPCXSTR lpszAPILogout = _T("logout");
	LPCXSTR lpszAPITimeout = _T("timeout");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsnicmp(lpszAPIName, lpszAPILogin, _tcslen(lpszAPIName)))
	{
		XENGINE_PROTOCOL_USERAUTH st_UserAuth;
		AUTHREG_USERTABLE st_UserTable;

		memset(&st_UserAuth, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		Protocol_Parse_HttpParseAuth(lpszMsgBuffer, nMsgLen, &st_UserAuth);
		if (!Database_SQLite_UserQuery(st_UserAuth.tszUserName, &st_UserTable))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user notfound");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("PASS客户端：%s，用户名：%s，登录失败，用户名不存在"), lpszClientAddr, st_UserAuth.tszUserName);
			return XFALSE;
		}
		if ((_tcslen(st_UserAuth.tszUserPass) != _tcslen(st_UserTable.st_UserInfo.tszUserPass)) || (0 != _tcsncmp(st_UserAuth.tszUserPass, st_UserTable.st_UserInfo.tszUserPass, _tcslen(st_UserTable.st_UserInfo.tszUserPass))))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "password is incorrent");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("PASS客户端：%s，用户名：%s，登录失败，密码错误"), lpszClientAddr, st_UserAuth.tszUserName);
			return XFALSE;
		}
		Protocol_Packet_HttpClientInfo(tszSDBuffer, &nSDLen, &st_UserTable);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("PASS客户端:%s,请求登录用户:%s 成功"), lpszClientAddr, st_UserAuth.tszUserName);
	}
	else if (0 == _tcsnicmp(lpszAPIName, lpszAPILogout, _tcslen(lpszAPIName)))
	{
		AUTHREG_PROTOCOL_TIME st_ProtocolTime;
		memset(&st_ProtocolTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

		Protocol_Parse_HttpParseTime(lpszMsgBuffer, nMsgLen, &st_ProtocolTime);
		Database_SQLite_UserLeave(&st_ProtocolTime);

		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("PASS客户端：%s，用户名：%s，请求离开服务成功"), lpszClientAddr, st_ProtocolTime.tszUserName);
	}
	else if (0 == _tcsnicmp(lpszAPIName, lpszAPITimeout, _tcslen(lpszAPIName)))
	{
		AUTHREG_PROTOCOL_TIME st_ProtocolTime;
		memset(&st_ProtocolTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

		Protocol_Parse_HttpParseTime(lpszMsgBuffer, nMsgLen, &st_ProtocolTime);
		Database_SQLite_UserLeave(&st_ProtocolTime);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("PASS客户端：%s，用户名：%s，超时通知成功"), lpszClientAddr, st_ProtocolTime.tszUserName);
	}
	return XTRUE;
}