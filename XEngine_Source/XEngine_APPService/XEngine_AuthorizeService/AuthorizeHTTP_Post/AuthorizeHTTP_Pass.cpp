#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Pass(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 8196;
	XCHAR tszSDBuffer[8196];
	LPCXSTR lpszAPILogin = _X("login");
	LPCXSTR lpszAPILogout = _X("logout");
	LPCXSTR lpszAPITimeout = _X("timeout");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsxnicmp(lpszAPIName, lpszAPILogin, _tcsxlen(lpszAPIName)))
	{
		XENGINE_PROTOCOL_USERAUTH st_UserAuth;
		AUTHREG_USERTABLE st_UserTable;

		memset(&st_UserAuth, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		Protocol_Parse_HttpParseAuth(lpszMsgBuffer, nMsgLen, &st_UserAuth);
		bool bSuccess = false;
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			bSuccess = DBModule_SQLite_UserQuery(st_UserAuth.tszUserName, &st_UserTable);
		}
		else 
		{
			bSuccess = DBModule_MySQL_UserQuery(st_UserAuth.tszUserName, &st_UserTable);
		}
		if (!bSuccess) 
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_USER, "user notfound");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("PASS客户端：%s，用户名：%s，登录失败，用户名不存在"), lpszClientAddr, st_UserAuth.tszUserName);
			return false;
		}
		if ((_tcsxlen(st_UserAuth.tszUserPass) != _tcsxlen(st_UserTable.st_UserInfo.tszUserPass)) || (0 != _tcsxncmp(st_UserAuth.tszUserPass, st_UserTable.st_UserInfo.tszUserPass, _tcsxlen(st_UserTable.st_UserInfo.tszUserPass))))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_PASS, "password is incorrent");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("PASS客户端：%s，用户名：%s，登录失败，密码错误"), lpszClientAddr, st_UserAuth.tszUserName);
			return false;
		}
		Protocol_Packet_HttpClientInfo(tszSDBuffer, &nSDLen, &st_UserTable);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("PASS客户端:%s,请求登录用户:%s 成功"), lpszClientAddr, st_UserAuth.tszUserName);
	}
	else if (0 == _tcsxnicmp(lpszAPIName, lpszAPILogout, _tcsxlen(lpszAPIName)))
	{
		AUTHREG_PROTOCOL_TIME st_ProtocolTime;
		memset(&st_ProtocolTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

		Protocol_Parse_HttpParseTime(lpszMsgBuffer, nMsgLen, &st_ProtocolTime);
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_UserLeave(&st_ProtocolTime);
		}
		else
		{
			DBModule_MySQL_UserLeave(&st_ProtocolTime);
		}

		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("PASS客户端：%s，用户名：%s，请求离开服务成功"), lpszClientAddr, st_ProtocolTime.tszUserName);
	}
	else if (0 == _tcsxnicmp(lpszAPIName, lpszAPITimeout, _tcsxlen(lpszAPIName)))
	{
		AUTHREG_PROTOCOL_TIME st_ProtocolTime;
		memset(&st_ProtocolTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

		Protocol_Parse_HttpParseTime(lpszMsgBuffer, nMsgLen, &st_ProtocolTime);
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_UserLeave(&st_ProtocolTime);
		}
		else
		{
			DBModule_MySQL_UserLeave(&st_ProtocolTime);
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("PASS客户端：%s，用户名：%s，超时通知成功"), lpszClientAddr, st_ProtocolTime.tszUserName);
	}
	return true;
}