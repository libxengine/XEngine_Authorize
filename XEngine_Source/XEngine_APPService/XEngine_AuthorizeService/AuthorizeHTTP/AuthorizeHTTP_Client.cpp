#include "../Authorize_Hdr.h"

BOOL XEngine_AuthorizeHTTP_Client(LPCTSTR lpszClientAddr, LPCTSTR lpszAPIName, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	LPCTSTR lpszAPIGet = _T("get");
	LPCTSTR lpszAPIList = _T("list");
	LPCTSTR lpszAPIClose = _T("close");
	LPCTSTR lpszAPIModify = _T("modify");
	LPCTSTR lpszAPIDelete = _T("delete");
	int nSDLen = 4096;
	TCHAR tszSDBuffer[4096];

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsnicmp(lpszAPIGet, lpszAPIName, _tcslen(lpszAPIGet)))
	{
		AUTHREG_USERTABLE st_UserTable;
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, &st_UserTable.st_UserInfo);
		if (!Database_SQLite_UserQuery(st_UserTable.st_UserInfo.tszUserName, &st_UserTable))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "not found client");
			XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求用户:%s 信息失败,错误码:%lX"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName, DBModule_GetLastError());
			return FALSE;
		}
		Protocol_Packet_HttpClientInfo(tszSDBuffer, &nSDLen, &st_UserTable);
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求客户端:%s 信息成功"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
	}
	else if (0 == _tcsnicmp(lpszAPIList, lpszAPIName, _tcslen(lpszAPIList)))
	{
		TCHAR* ptszMsgBuffer = (TCHAR*)malloc(XENGINE_AUTH_MAX_BUFFER);
		if (NULL == ptszMsgBuffer)
		{
			return FALSE;
		}
		memset(ptszMsgBuffer, '\0', XENGINE_AUTH_MAX_BUFFER);
		//得到在线用户
		int nOnCount = 0;
		AUTHREG_USERTABLE** ppSt_ListClient;
		Session_Authorize_GetClient(&ppSt_ListClient, &nOnCount);
		//得到所有用户
		int nOffCount = 0;
		AUTHREG_USERTABLE** ppSt_UserInfo;
		Database_SQLite_UserList(&ppSt_UserInfo, &nOffCount);

		Protocol_Packet_HttpClientList(ptszMsgBuffer, &nSDLen, &ppSt_ListClient, nOnCount, &ppSt_UserInfo, nOffCount);

		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nOnCount);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_UserInfo, nOffCount);
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, ptszMsgBuffer, nSDLen);
		free(ptszMsgBuffer);
		ptszMsgBuffer = NULL;
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求客户端列表成功,在线用户:%d,离线用户:%d"), lpszClientAddr, nOnCount, nOffCount);
	}
	else if (0 == _tcsnicmp(lpszAPIClose, lpszAPIName, _tcslen(lpszAPIClose)))
	{
		TCHAR tszClientAddr[128];
		XENGINE_PROTOCOL_USERINFO st_UserInfo;

		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));

		Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, &st_UserInfo);
		if (!Session_Authorize_GetAddrForUser(st_UserInfo.tszUserName, tszClientAddr))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "not found client");
			XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求剔除用户:%s 没有找到,可能不在线"), lpszClientAddr, st_UserInfo.tszUserName);
			return FALSE;
		}
		XEngine_CloseClient(tszClientAddr);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求剔除用户:%s 成功"), lpszClientAddr, st_UserInfo.tszUserName);
	}
	else if (0 == _tcsnicmp(lpszAPIModify, lpszAPIName, _tcslen(lpszAPIModify)))
	{
		AUTHREG_USERTABLE st_UserTable;
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		Protocol_Parse_HttpParseTable(lpszMsgBuffer, nMsgLen, &st_UserTable);
		if (!Database_SQLite_UserSet(&st_UserTable))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "not found client");
			XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求修改用户信息失败:%s 错误码:%lX"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName, DBModule_GetLastError());
			return FALSE;
		}
		Session_Authorize_SetUser(&st_UserTable);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求修改用户信息:%s 成功"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
	}
	else if (0 == _tcsnicmp(lpszAPIDelete, lpszAPIName, _tcslen(lpszAPIDelete)))
	{
		TCHAR tszClientAddr[128];
		XENGINE_PROTOCOL_USERINFO st_UserInfo;

		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));

		Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, &st_UserInfo);

		Session_Authorize_GetAddrForUser(st_UserInfo.tszUserName, tszClientAddr);
		XEngine_CloseClient(tszClientAddr);
		Database_SQLite_UserDelete(st_UserInfo.tszUserName);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求删除用户:%s 成功"), lpszClientAddr, st_UserInfo.tszUserName);
	}
	return TRUE;
}