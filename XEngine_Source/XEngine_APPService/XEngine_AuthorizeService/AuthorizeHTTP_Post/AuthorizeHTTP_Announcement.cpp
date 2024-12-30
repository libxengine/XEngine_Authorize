#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Announcement(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 8196;
	XCHAR tszSDBuffer[8196];
	LPCXSTR lpszAPIInsert = _X("insert");
	LPCXSTR lpszAPIDelete = _X("delete");
	LPCXSTR lpszAPIList = _X("list");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (!st_FunSwitch.bSwitchNotice)
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 503, "the function is closed");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，公告系统协议处理失败，功能已经被服务器关闭!"), lpszClientAddr);
		return false;
	}
	if (0 == _tcsxnicmp(lpszAPIInsert, lpszAPIName, _tcsxlen(lpszAPIInsert)))
	{
		int nListCount = 0;
		AUTHREG_ANNOUNCEMENT st_Announcement;
		memset(&st_Announcement, '\0', sizeof(AUTHREG_ANNOUNCEMENT));

		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_AnnouncementList(NULL, &nListCount);
		}
		else
		{
			DBModule_MySQL_AnnouncementList(NULL, &nListCount);
		}
		if (nListCount > 10)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 510, "server limited");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，公告系统协议处理失败，超过了服务器限制的公告数量!"), lpszClientAddr);
			return false;
		}
		Protocol_Parse_HttpParseAnnouncement(lpszMsgBuffer, nMsgLen, &st_Announcement);
		if (_tcsxlen(st_Announcement.tszContext) <= 1)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 510, "server limited");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，公告系统协议处理失败,内容不能小于1个字节!"), lpszClientAddr);
			return false;
		}
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_AnnouncementInsert(&st_Announcement);
		}
		else
		{
			DBModule_MySQL_AnnouncementInsert(&st_Announcement);
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,插入公告成功,公告信息:%s"), lpszClientAddr, st_Announcement.tszContext);
	}
	else if (0 == _tcsxnicmp(lpszAPIDelete, lpszAPIName, _tcsxlen(lpszAPIDelete)))
	{
		AUTHREG_ANNOUNCEMENT st_Announcement;
		memset(&st_Announcement, '\0', sizeof(AUTHREG_ANNOUNCEMENT));

		Protocol_Parse_HttpParseAnnouncement(lpszMsgBuffer, nMsgLen, &st_Announcement);
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_AnnouncementDelete(&st_Announcement);
		}
		else
		{
			DBModule_MySQL_AnnouncementDelete(&st_Announcement);
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,删除公告成功.删除公告ID:%lld"), lpszClientAddr, st_Announcement.nID);
	}
	else if (0 == _tcsxnicmp(lpszAPIList, lpszAPIName, _tcsxlen(lpszAPIList)))
	{
		int nListCount = 0;
		AUTHREG_ANNOUNCEMENT** ppSt_Announcement;

		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_AnnouncementList(&ppSt_Announcement, &nListCount);
		}
		else
		{
			DBModule_MySQL_AnnouncementList(&ppSt_Announcement, &nListCount);
		}
		Protocol_Packet_HttpAnnouncement(tszSDBuffer, &nSDLen, &ppSt_Announcement, nListCount);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		BaseLib_Memory_Free((XPPPMEM)&ppSt_Announcement, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,查询公告列表成功,公告个数:%d"), lpszClientAddr, nListCount);
	}
	return true;
}