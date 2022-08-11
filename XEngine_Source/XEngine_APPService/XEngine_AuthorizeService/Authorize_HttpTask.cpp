#include "Authorize_Hdr.h"

XHTHREAD CALLBACK XEngine_AuthService_HttpThread(LPVOID lParam)
{
	int nPoolIndex = *(int*)lParam;
	int nThreadPos = nPoolIndex + 1;

	while (bIsRun)
	{
		if (!RfcComponents_HttpServer_EventWaitEx(xhHttpPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_ListClient;
		RfcComponents_HttpServer_GetPoolEx(xhHttpPacket, nThreadPos, &ppSt_ListClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			int nMsgLen = 0;
			CHAR* ptszMsgBuffer = NULL;
			RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParament;

			memset(&st_HTTPParament, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));

			if (RfcComponents_HttpServer_GetMemoryEx(xhHttpPacket, ppSt_ListClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPParament))
			{
				XEngine_Client_HttpTask(ppSt_ListClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &st_HTTPParament);
			}
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	return 0;
}

BOOL XEngine_Client_HttpTask(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM *pSt_HTTPParament)
{
	int nSDLen = 2048;
	TCHAR tszSDBuffer[2048];
	LPCTSTR lpszMethodGet = _T("POST");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 != _tcsnicmp(lpszMethodGet, pSt_HTTPParament->tszHttpMethod, _tcslen(lpszMethodGet)))
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 405, "method not allow");
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,发送的方法:%s 不支持"), lpszClientAddr, pSt_HTTPParament->tszHttpMethod);
		return FALSE;
	}
	TCHAR tszAPIType[64];
	TCHAR tszAPIVer[64];
	TCHAR tszAPIName[64];
	LPCTSTR lpszAPIType = _T("auth");
	LPCTSTR lpszAPIVerClient = _T("client");
	LPCTSTR lpszAPIVerSerial = _T("serial");

	memset(tszAPIType, '\0', sizeof(tszAPIType));
	memset(tszAPIVer, '\0', sizeof(tszAPIVer));
	memset(tszAPIName, '\0', sizeof(tszAPIName));

	RfcComponents_HttpHelp_GetUrlApi(pSt_HTTPParament->tszHttpUri, tszAPIType, tszAPIVer, tszAPIName);
	if (0 != _tcsnicmp(lpszAPIType, tszAPIType, _tcslen(lpszAPIType)))
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "request url is incorrent");
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 不支持"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
		return FALSE;
	}
	if (0 == _tcsnicmp(lpszAPIVerClient, tszAPIVer, _tcslen(lpszAPIVerClient)))
	{
		LPCTSTR lpszAPIGet = _T("get");
		LPCTSTR lpszAPIList = _T("list");
		LPCTSTR lpszAPIClose = _T("close");
		LPCTSTR lpszAPIModify = _T("modify");
		LPCTSTR lpszAPIDelete = _T("delete");
		
		if (0 == _tcsnicmp(lpszAPIGet, tszAPIName, _tcslen(lpszAPIGet)))
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
		else if (0 == _tcsnicmp(lpszAPIList, tszAPIName, _tcslen(lpszAPIList)))
		{
			//得到在线用户
			int nOnCount = 0;
			AUTHREG_USERTABLE** ppSt_ListClient;
			Session_Authorize_GetClient(&ppSt_ListClient, &nOnCount);
			//得到所有用户
			int nOffCount = 0;
			AUTHREG_USERTABLE** ppSt_UserInfo;
			Database_SQLite_UserList(&ppSt_UserInfo, &nOffCount);

			Protocol_Packet_HttpClientList(tszSDBuffer, &nSDLen, &ppSt_ListClient, nOnCount, &ppSt_UserInfo, nOffCount);

			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nOnCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_UserInfo, nOffCount);
			XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求客户端列表成功,在线用户:%d,离线用户:%d"), lpszClientAddr, nOnCount, nOffCount);
		}
		else if (0 == _tcsnicmp(lpszAPIClose, tszAPIName, _tcslen(lpszAPIClose)))
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
		else if (0 == _tcsnicmp(lpszAPIModify, tszAPIName, _tcslen(lpszAPIModify)))
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
		else if (0 == _tcsnicmp(lpszAPIDelete, tszAPIName, _tcslen(lpszAPIDelete)))
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
	}
	else if (0 == _tcsnicmp(lpszAPIVerSerial, tszAPIVer, _tcslen(lpszAPIVerSerial)))
	{
		LPCTSTR lpszAPIList = _T("list");
		LPCTSTR lpszAPIInsert = _T("insert");
		LPCTSTR lpszAPIDelete = _T("delete");

		if (0 == _tcsnicmp(lpszAPIList, tszAPIName, _tcslen(lpszAPIList)))
		{
			int nListCount = 0;
			AUTHREG_SERIALTABLE** ppSt_SerialTable;
			Database_SQLite_SerialQueryAll(&ppSt_SerialTable, &nListCount);
			Protocol_Packet_HttpSerialList(tszSDBuffer, &nSDLen, &ppSt_SerialTable, nListCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_SerialTable, nListCount);
			XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求序列号列表成功,个数:%d"), lpszClientAddr, nListCount);
		}
		else if (0 == _tcsnicmp(lpszAPIInsert, tszAPIName, _tcslen(lpszAPIInsert)))
		{
			AUTHREG_SERIALTABLE st_SerialTable;
			memset(&st_SerialTable, '\0', sizeof(AUTHREG_SERIALTABLE));

			Protocol_Parse_HttpParseSerial(lpszMsgBuffer, nMsgLen, &st_SerialTable);
			Database_SQLite_SerialInsert(st_SerialTable.tszSerialNumber);
			XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求插入序列号:%s 成功"), lpszClientAddr, st_SerialTable.tszSerialNumber);
		}
		else if (0 == _tcsnicmp(lpszAPIDelete, tszAPIName, _tcslen(lpszAPIDelete)))
		{
			AUTHREG_SERIALTABLE st_SerialTable;
			memset(&st_SerialTable, '\0', sizeof(AUTHREG_SERIALTABLE));

			Protocol_Parse_HttpParseSerial(lpszMsgBuffer, nMsgLen, &st_SerialTable);
			Database_SQLite_SerialDelete(st_SerialTable.tszSerialNumber);
			XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求删除序列号:%s 成功"), lpszClientAddr, st_SerialTable.tszSerialNumber);
		}
	}
	return TRUE;
}