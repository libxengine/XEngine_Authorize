#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Client(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 8196;
	XCHAR tszSDBuffer[8196];
	LPCXSTR lpszAPIGet = _X("get");
	LPCXSTR lpszAPIList = _X("list");
	LPCXSTR lpszAPIClose = _X("close");
	LPCXSTR lpszAPIModify = _X("modify");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsxnicmp(lpszAPIGet, lpszAPIName, _tcsxlen(lpszAPIGet)))
	{
		AUTHREG_USERTABLE st_UserTable;
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, &st_UserTable.st_UserInfo);
		if (!Database_SQLite_UserQuery(st_UserTable.st_UserInfo.tszUserName, &st_UserTable))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "not found client");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求用户:%s 信息失败,错误码:%lX"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName, DBModule_GetLastError());
			return false;
		}
		Protocol_Packet_HttpClientInfo(tszSDBuffer, &nSDLen, &st_UserTable);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求客户端:%s 信息成功"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
	}
	else if (0 == _tcsxnicmp(lpszAPIList, lpszAPIName, _tcsxlen(lpszAPIList)))
	{
		int nOnCount = 0;
		int nOffCount = 0;
		int nPosStart = 0;
		int nPosEnd = 0;
		bool bOnline = false;
		AUTHREG_USERTABLE** ppSt_UserInfo;
		AUTHSESSION_NETCLIENT** ppSt_ListClient;
		
		XCHAR* ptszMsgBuffer = (XCHAR*)malloc(XENGINE_AUTH_MAX_BUFFER);
		if (NULL == ptszMsgBuffer)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 500, "internal server error");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求用户列表失败,申请内存失败,错误:%d"), lpszClientAddr, errno);
			return false;
		}
		memset(ptszMsgBuffer, '\0', XENGINE_AUTH_MAX_BUFFER);

		Protocol_Parse_HttpParsePos(lpszMsgBuffer, nMsgLen, &nPosStart, &nPosEnd);
		if ((nPosEnd - nPosStart) > 100)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "pos parament is not rigth");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求用户列表失败,POS参数不正确,%d - %d"), lpszClientAddr, nPosStart, nPosEnd);
			return false;
		}
		Protocol_Parse_HttpParseOnline(lpszMsgBuffer, nMsgLen, &bOnline);
		//得到在线用户
		Session_Authorize_GetClient(&ppSt_ListClient, &nOnCount);
		//得到所有用户
		if (!bOnline)
		{
			//只有bOnline不是在线列表的时候才执行
			Database_SQLite_UserList(&ppSt_UserInfo, &nOffCount, nPosStart, nPosEnd);
		}
		Protocol_Packet_HttpClientList(ptszMsgBuffer, &nSDLen, &ppSt_ListClient, nOnCount, &ppSt_UserInfo, nOffCount);

		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nOnCount);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_UserInfo, nOffCount);
		XEngine_Client_TaskSend(lpszClientAddr, ptszMsgBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		free(ptszMsgBuffer);
		ptszMsgBuffer = NULL;
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求客户端列表成功,在线用户:%d,离线用户:%d,显示标志:%d"), lpszClientAddr, nOnCount, nOffCount - nOnCount, bOnline);
	}
	else if (0 == _tcsxnicmp(lpszAPIClose, lpszAPIName, _tcsxlen(lpszAPIClose)))
	{
		XCHAR tszClientAddr[128];
		XENGINE_PROTOCOL_USERINFO st_UserInfo;

		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));

		Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, &st_UserInfo);
		if (!Session_Authorize_GetAddrForUser(st_UserInfo.tszUserName, tszClientAddr))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "not found client");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求剔除用户:%s 没有找到,可能不在线"), lpszClientAddr, st_UserInfo.tszUserName);
			return false;
		}
		XEngine_CloseClient(tszClientAddr);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求剔除用户:%s 成功"), lpszClientAddr, st_UserInfo.tszUserName);
	}
	else if (0 == _tcsxnicmp(lpszAPIModify, lpszAPIName, _tcsxlen(lpszAPIModify)))
	{
		AUTHREG_USERTABLE st_UserTable;
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		Protocol_Parse_HttpParseTable(lpszMsgBuffer, nMsgLen, &st_UserTable);
		if (!Database_SQLite_UserSet(&st_UserTable))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "not found client");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求修改用户信息失败:%s 错误码:%lX"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName, DBModule_GetLastError());
			return false;
		}
		Session_Authorize_SetUser(&st_UserTable);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求修改用户信息:%s 成功"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
	}
	return true;
}