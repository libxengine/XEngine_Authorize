#include "../Authorize_Hdr.h"

XBOOL XEngine_AuthorizeHTTP_Announcement(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 4096;
	XCHAR tszSDBuffer[4096];
	LPCXSTR lpszAPIInsert = _T("insert");
	LPCXSTR lpszAPIDelete = _T("delete");
	LPCXSTR lpszAPIList = _T("list");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (!st_FunSwitch.bSwitchNotice)
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 503, "the function is closed");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，公告系统协议处理失败，功能已经被服务器关闭!"), lpszClientAddr);
		return XFALSE;
	}
	if (0 == _tcsnicmp(lpszAPIInsert, lpszAPIName, _tcslen(lpszAPIInsert)))
	{
		AUTHREG_ANNOUNCEMENT st_Announcement;
		memset(&st_Announcement, '\0', sizeof(AUTHREG_ANNOUNCEMENT));

		Protocol_Parse_HttpParseAnnouncement(lpszMsgBuffer, nMsgLen, &st_Announcement);
		Database_SQLite_AnnouncementInsert(&st_Announcement);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,插入公告成功,公告信息:%s"), lpszClientAddr, st_Announcement.tszContext);
	}
	else if (0 == _tcsnicmp(lpszAPIDelete, lpszAPIName, _tcslen(lpszAPIDelete)))
	{
		AUTHREG_ANNOUNCEMENT st_Announcement;
		memset(&st_Announcement, '\0', sizeof(AUTHREG_ANNOUNCEMENT));

		Protocol_Parse_HttpParseAnnouncement(lpszMsgBuffer, nMsgLen, &st_Announcement);
		Database_SQLite_AnnouncementDelete(&st_Announcement);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,删除公告成功.删除公告ID:%lld"), lpszClientAddr, st_Announcement.nID);
	}
	else if (0 == _tcsnicmp(lpszAPIList, lpszAPIName, _tcslen(lpszAPIList)))
	{
		int nListCount = 0;
		AUTHREG_ANNOUNCEMENT** ppSt_Announcement;

		Database_SQLite_AnnouncementList(&ppSt_Announcement, &nListCount);
		Protocol_Packet_HttpAnnouncement(tszSDBuffer, &nSDLen, &ppSt_Announcement, nListCount);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Announcement, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,查询公告列表成功,公告个数:%d"), lpszClientAddr, nListCount);
	}
	return XTRUE;
}