#include "../Authorize_Hdr.h"

XBOOL XEngine_AuthorizeHTTP_Banned(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 4096;
	XCHAR tszSDBuffer[4096];
	LPCXSTR lpszAPIInsert = _T("insert");
	LPCXSTR lpszAPIDelete = _T("delete");
	LPCXSTR lpszAPIList = _T("list");
	LPCXSTR lpszAPIModify = _T("modify");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsnicmp(lpszAPIInsert, lpszAPIName, _tcslen(lpszAPIInsert)))
	{
		AUTHREG_BANNED st_Banned;
		memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));

		Protocol_Parse_HttpParseBanned(lpszMsgBuffer, nMsgLen, &st_Banned);
		Database_SQLite_BannedInsert(&st_Banned);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,插入禁用列表成功,禁用的用户:%s,禁用的IP地址:%s"), lpszClientAddr, st_Banned.tszUserName, st_Banned.tszIPAddr);
	}
	else if (0 == _tcsnicmp(lpszAPIDelete, lpszAPIName, _tcslen(lpszAPIDelete)))
	{
		AUTHREG_BANNED st_Banned;
		memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));

		Protocol_Parse_HttpParseBanned(lpszMsgBuffer, nMsgLen, &st_Banned);
		Database_SQLite_BannedDelete(&st_Banned);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,删除禁用列表成功,删除禁用的用户:%s,删除禁用的IP地址:%s"), lpszClientAddr, st_Banned.tszUserName, st_Banned.tszIPAddr);
	}
	else if (0 == _tcsnicmp(lpszAPIList, lpszAPIName, _tcslen(lpszAPIList)))
	{
		int nCountAddr = 0;
		int nCountUser = 0;
		AUTHREG_BANNED** ppSt_BannedUser;
		AUTHREG_BANNED** ppSt_BannedAddr;

		Database_SQLite_BannedList(&ppSt_BannedUser, &nCountUser, &ppSt_BannedAddr, &nCountAddr);
		Protocol_Packet_HttpBanned(tszSDBuffer, &nSDLen, &ppSt_BannedUser, nCountUser, &ppSt_BannedAddr, nCountAddr);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_BannedAddr, nCountAddr);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_BannedUser, nCountUser);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,查询禁用列表成功,禁用的用户个数:%d,禁用的IP地址个数:%d"), lpszClientAddr, nCountUser, nCountAddr);
	}
	else if (0 == _tcsnicmp(lpszAPIModify, lpszAPIName, _tcslen(lpszAPIModify)))
	{
		AUTHREG_BANNED st_Banned;
		memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));

		Protocol_Parse_HttpParseBanned(lpszMsgBuffer, nMsgLen, &st_Banned);
		Database_SQLite_BannedUPDate(&st_Banned);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,修改禁用列表成功,修改的用户:%s,修改的IP地址:%s"), lpszClientAddr, st_Banned.tszUserName, st_Banned.tszIPAddr);
	}
	return XTRUE;
}