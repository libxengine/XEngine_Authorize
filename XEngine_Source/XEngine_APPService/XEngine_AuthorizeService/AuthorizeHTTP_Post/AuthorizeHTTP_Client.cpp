#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Client(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 8196;
	XCHAR tszSDBuffer[8196];
	LPCXSTR lpszAPIGet = _X("get");
	LPCXSTR lpszAPIList = _X("list");
	LPCXSTR lpszAPIClose = _X("close");
	LPCXSTR lpszAPIModify = _X("modify");
	LPCXSTR lpszAPIDelete = _X("delete");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsxnicmp(lpszAPIGet, lpszAPIName, _tcsxlen(lpszAPIGet)))
	{
		AUTHREG_USERTABLE st_UserTable;
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, &st_UserTable.st_UserInfo);
		bool bSuccess = false;
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			bSuccess = DBModule_SQLite_UserQuery(st_UserTable.st_UserInfo.tszUserName, &st_UserTable);
		}
		else 
		{
			bSuccess = DBModule_MySQL_UserQuery(st_UserTable.st_UserInfo.tszUserName, &st_UserTable);
		}
		if (!bSuccess) 
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
		
		XCHAR* ptszMsgBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
		if (NULL == ptszMsgBuffer)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 500, "internal server error");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求用户列表失败,申请内存失败,错误:%d"), lpszClientAddr, errno);
			return false;
		}
		memset(ptszMsgBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);

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
			if (0 == st_AuthConfig.st_XSql.nDBType) 
			{
				DBModule_SQLite_UserList(&ppSt_UserInfo, &nOffCount, nPosStart, nPosEnd);
			}
			else
			{
				DBModule_MySQL_UserList(&ppSt_UserInfo, &nOffCount, nPosStart, nPosEnd);
			}
		}
		Protocol_Packet_HttpClientList(ptszMsgBuffer, &nSDLen, &ppSt_ListClient, nOnCount, &ppSt_UserInfo, nOffCount);

		BaseLib_Memory_Free((XPPPMEM)&ppSt_ListClient, nOnCount);
		BaseLib_Memory_Free((XPPPMEM)&ppSt_UserInfo, nOffCount);
		XEngine_Client_TaskSend(lpszClientAddr, ptszMsgBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		free(ptszMsgBuffer);
		ptszMsgBuffer = NULL;
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求客户端列表成功,在线用户:%d,离线用户:%d,显示标志:%d"), lpszClientAddr, nOnCount, nOffCount - nOnCount, bOnline);
	}
	else if (0 == _tcsxnicmp(lpszAPIClose, lpszAPIName, _tcsxlen(lpszAPIClose)))
	{
		int nListCount = 0;
		AUTHSESSION_NETCLIENT** ppSt_ListClient;
		XENGINE_PROTOCOL_USERINFO st_UserInfo;
		memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));

		Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, &st_UserInfo);
		if (!Session_Authorize_GetClient(&ppSt_ListClient, &nListCount, st_UserInfo.tszUserName))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "not found client");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求剔除用户:%s 没有找到,可能不在线"), lpszClientAddr, st_UserInfo.tszUserName);
			return false;
		}
		for (int i = 0; i < nListCount; i++)
		{
			XEngine_CloseClient(ppSt_ListClient[i]->tszClientAddr, true);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求剔除用户:%s 成功,在线用户数:%d"), lpszClientAddr, st_UserInfo.tszUserName, nListCount);
	}
	else if (0 == _tcsxnicmp(lpszAPIModify, lpszAPIName, _tcsxlen(lpszAPIModify)))
	{
		AUTHREG_USERTABLE st_UserTable;
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		Protocol_Parse_HttpParseTable(lpszMsgBuffer, nMsgLen, &st_UserTable);

		if (st_AuthConfig.st_XVerification.st_PassCrypto.bEnable)
		{
			int nPLen = _tcsxlen(st_UserTable.st_UserInfo.tszUserPass);
			XBYTE byMD5Buffer[MAX_PATH] = {};
			Cryption_Api_Digest(st_UserTable.st_UserInfo.tszUserPass, byMD5Buffer, &nPLen, false, st_AuthConfig.st_XVerification.st_PassCrypto.nCodec);
			memset(st_UserTable.st_UserInfo.tszUserPass, '\0', sizeof(st_UserTable.st_UserInfo.tszUserPass));
			BaseLib_String_StrToHex((LPCXSTR)byMD5Buffer, nPLen, st_UserTable.st_UserInfo.tszUserPass);
		}
		bool bSuccess = false;
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			bSuccess = DBModule_SQLite_UserSet(&st_UserTable);
		}
		else 
		{
			bSuccess = DBModule_MySQL_UserSet(&st_UserTable);
		}
		if (!bSuccess) 
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
	else if (0 == _tcsxnicmp(lpszAPIDelete, lpszAPIName, _tcsxlen(lpszAPIDelete)))
	{
		XENGINE_PROTOCOL_USERINFO st_UserInfo = {};
		Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, &st_UserInfo);

		//关闭链接
		int nListCount = 0;
		AUTHSESSION_NETCLIENT** ppSt_ListClient;
		Session_Authorize_GetClient(&ppSt_ListClient, &nListCount, st_UserInfo.tszUserName);
		for (int i = 0; i < nListCount; i++)
		{
			XEngine_CloseClient(ppSt_ListClient[i]->tszClientAddr, true);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
		//删除数据库
		bool bSuccess = false;
		if (0 == st_AuthConfig.st_XSql.nDBType)
		{
			bSuccess = DBModule_SQLite_UserDelete(&st_UserInfo);
		}
		else
		{
			bSuccess = DBModule_MySQL_UserDelete(&st_UserInfo);
		}
		if (!bSuccess)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "not found client");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求删除用户失败:%s 错误码:%lX"), lpszClientAddr, st_UserInfo.tszUserName, DBModule_GetLastError());
			return false;
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求删除用户:%s 成功"), lpszClientAddr, st_UserInfo.tszUserName);
	}
	return true;
}