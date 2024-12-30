#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Banned(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 8196;
	XCHAR tszSDBuffer[8196];
	LPCXSTR lpszAPIInsert = _X("insert");
	LPCXSTR lpszAPIDelete = _X("delete");
	LPCXSTR lpszAPIList = _X("list");
	LPCXSTR lpszAPIModify = _X("modify");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (!st_FunSwitch.bSwitchBanned)
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 503, "the function is closed");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，黑名单协议处理失败，功能已经被服务器关闭!"), lpszClientAddr);
		return false;
	}

	if (0 == _tcsxnicmp(lpszAPIInsert, lpszAPIName, _tcsxlen(lpszAPIInsert)))
	{
		AUTHREG_BANNED st_Banned;
		memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));

		Protocol_Parse_HttpParseBanned(lpszMsgBuffer, nMsgLen, &st_Banned);
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_BannedInsert(&st_Banned);
		}
		else
		{
			DBModule_MySQL_BannedInsert(&st_Banned);
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,插入禁用列表成功,禁用的用户:%s,禁用的IP地址:%s"), lpszClientAddr, st_Banned.tszUserName, st_Banned.tszIPAddr);
	}
	else if (0 == _tcsxnicmp(lpszAPIDelete, lpszAPIName, _tcsxlen(lpszAPIDelete)))
	{
		AUTHREG_BANNED st_Banned;
		memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));

		Protocol_Parse_HttpParseBanned(lpszMsgBuffer, nMsgLen, &st_Banned);
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_BannedDelete(&st_Banned);
		}
		else
		{
			DBModule_MySQL_BannedDelete(&st_Banned);
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,删除禁用列表成功,删除禁用的用户:%s,删除禁用的IP地址:%s"), lpszClientAddr, st_Banned.tszUserName, st_Banned.tszIPAddr);
	}
	else if (0 == _tcsxnicmp(lpszAPIList, lpszAPIName, _tcsxlen(lpszAPIList)))
	{
		int nCountAddr = 0;
		int nCountUser = 0;
		int nPosStart = 0;
		int nPosEnd = 0;
		AUTHREG_BANNED** ppSt_BannedUser;
		AUTHREG_BANNED** ppSt_BannedAddr;

		Protocol_Parse_HttpParseBanned2(lpszMsgBuffer, nMsgLen, &nPosStart, &nPosEnd);

		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_BannedList(&ppSt_BannedUser, &nCountUser, &ppSt_BannedAddr, &nCountAddr, nPosStart, nPosEnd);
		}
		else
		{
			DBModule_MySQL_BannedList(&ppSt_BannedUser, &nCountUser, &ppSt_BannedAddr, &nCountAddr, nPosStart, nPosEnd);
		}
		Protocol_Packet_HttpBanned(tszSDBuffer, &nSDLen, &ppSt_BannedUser, nCountUser, &ppSt_BannedAddr, nCountAddr);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		BaseLib_Memory_Free((XPPPMEM)&ppSt_BannedAddr, nCountAddr);
		BaseLib_Memory_Free((XPPPMEM)&ppSt_BannedUser, nCountUser);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,查询禁用列表成功,禁用的用户个数:%d,禁用的IP地址个数:%d"), lpszClientAddr, nCountUser, nCountAddr);
	}
	else if (0 == _tcsxnicmp(lpszAPIModify, lpszAPIName, _tcsxlen(lpszAPIModify)))
	{
		AUTHREG_BANNED st_Banned;
		memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));

		Protocol_Parse_HttpParseBanned(lpszMsgBuffer, nMsgLen, &st_Banned);
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_BannedUPDate(&st_Banned);
		}
		else
		{
			DBModule_MySQL_BannedUPDate(&st_Banned);
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,修改禁用列表成功,修改的用户:%s,修改的IP地址:%s"), lpszClientAddr, st_Banned.tszUserName, st_Banned.tszIPAddr);
	}
	return true;
}