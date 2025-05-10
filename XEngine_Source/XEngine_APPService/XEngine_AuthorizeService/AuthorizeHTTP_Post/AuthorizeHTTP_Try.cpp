#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Try(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 8196;
	XCHAR tszSDBuffer[8196];

	LPCXSTR lpszAPIList = _X("list");
	LPCXSTR lpszAPIDelete = _X("delete");
	LPCXSTR lpszAPIModify = _X("modify");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsxnicmp(lpszAPIName, lpszAPIList, _tcsxlen(lpszAPIName)))
	{
		int nPosStart = 0;
		int nPosEnd = 0;

		CHttpMemory_PoolEx m_HTTPMemoryPool(XENGINE_MEMORY_SIZE_MAX);
		Protocol_Parse_HttpParsePos(lpszMsgBuffer, nMsgLen, &nPosStart, &nPosEnd);

		int nListCount = 0;
		AUTHREG_TEMPVER** ppSt_AuthVer;
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_TryList(&ppSt_AuthVer, &nListCount);
		}
		else
		{
			DBModule_MySQL_TryList(&ppSt_AuthVer, &nListCount);
		}
		Protocol_Packet_HttpTryList(m_HTTPMemoryPool.get(), &nSDLen, &ppSt_AuthVer, nListCount);
		BaseLib_Memory_Free((XPPPMEM)&ppSt_AuthVer, nListCount);
		XEngine_Client_TaskSend(lpszClientAddr, m_HTTPMemoryPool.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端：%s，临时验证获取列表成功,列表个数:%d"), lpszClientAddr, nListCount);
	}
	else if (0 == _tcsxnicmp(lpszAPIName, lpszAPIDelete, _tcsxlen(lpszAPIName)))
	{
		AUTHREG_TEMPVER st_VERTemp;
		memset(&st_VERTemp, '\0', sizeof(AUTHREG_TEMPVER));

		Protocol_Parse_HttpParseTry(lpszMsgBuffer, nMsgLen, &st_VERTemp);
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_TryDelete(st_VERTemp.tszVSerial);
		}
		else
		{
			DBModule_MySQL_TryDelete(st_VERTemp.tszVSerial);
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端：%s，序列号：%s，临时验证删除成功"), lpszClientAddr, st_VERTemp.tszVSerial);
	}
	else if (0 == _tcsxnicmp(lpszAPIName, lpszAPIModify, _tcsxlen(lpszAPIName)))
	{
		AUTHREG_TEMPVER st_VERTemp;
		memset(&st_VERTemp, '\0', sizeof(AUTHREG_TEMPVER));

		Protocol_Parse_HttpParseTry(lpszMsgBuffer, nMsgLen, &st_VERTemp);
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_TrySet(&st_VERTemp);
		}
		else
		{
			DBModule_MySQL_TrySet(&st_VERTemp);
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端：%s，序列号：%s，临时验证修改成功"), lpszClientAddr, st_VERTemp.tszVSerial);
	}
	return true;
}