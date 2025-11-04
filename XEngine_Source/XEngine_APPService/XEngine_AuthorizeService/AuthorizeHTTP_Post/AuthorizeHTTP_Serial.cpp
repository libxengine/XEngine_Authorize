#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Serial(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 0;
	LPCXSTR lpszAPIList = _X("list");
	LPCXSTR lpszAPIInsert = _X("insert");
	LPCXSTR lpszAPIDelete = _X("delete");

	CHttpMemory_PoolEx m_MemoryPool(XENGINE_MEMORY_SIZE_MAX);
	if (0 == _tcsxnicmp(lpszAPIList, lpszAPIName, _tcsxlen(lpszAPIList)))
	{
		int nPosStart = 0;
		int nPosEnd = 0;
		int nListCount = 0;
		XCHAR* ptszMsgBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
		if (NULL == ptszMsgBuffer)
		{
			return false;
		}
		memset(ptszMsgBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);

		Protocol_Parse_HttpParsePos(lpszMsgBuffer, nMsgLen, &nPosStart, &nPosEnd);
		if ((nPosEnd - nPosStart) > 100)
		{
			Protocol_Packet_HttpComm(m_MemoryPool.get(), &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "pos parament is not rigth");
			XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPool.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求序列号列表失败,POS参数不正确,%d - %d"), lpszClientAddr, nPosStart, nPosEnd);
			return false;
		}
		AUTHREG_SERIALTABLE** ppSt_SerialTable;
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_SerialQueryAll(&ppSt_SerialTable, &nListCount, nPosStart, nPosEnd);
		}
		else
		{
			DBModule_MySQL_SerialQueryAll(&ppSt_SerialTable, &nListCount, nPosStart, nPosEnd);
		}
		Protocol_Packet_HttpSerialList(ptszMsgBuffer, &nSDLen, &ppSt_SerialTable, nListCount);
		BaseLib_Memory_Free((XPPPMEM)&ppSt_SerialTable, nListCount);
		XEngine_Client_TaskSend(lpszClientAddr, ptszMsgBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		free(ptszMsgBuffer);
		ptszMsgBuffer = NULL;
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求序列号列表成功,个数:%d"), lpszClientAddr, nListCount);
	}
	else if (0 == _tcsxnicmp(lpszAPIDelete, lpszAPIName, _tcsxlen(lpszAPIDelete)))
	{
		int nListCount = 0;
		AUTHREG_SERIALTABLE** ppSt_SerialTable;

		Protocol_Parse_HttpParseSerial(lpszMsgBuffer, nMsgLen, &ppSt_SerialTable, &nListCount);
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			for (int i = 0; i < nListCount; i++)
			{
				DBModule_SQLite_SerialDelete(ppSt_SerialTable[i]->tszSerialNumber);
			}
		}
		else 
		{
			for (int i = 0; i < nListCount; i++)
			{
				DBModule_MySQL_SerialDelete(ppSt_SerialTable[i]->tszSerialNumber);
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_SerialTable, nListCount);
		Protocol_Packet_HttpComm(m_MemoryPool.get(), &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPool.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求删除序列号成功,删除个数:%d"), lpszClientAddr, nListCount);
	}
	else if (0 == _tcsxnicmp(lpszAPIInsert, lpszAPIName, _tcsxlen(lpszAPIInsert)))
	{
		int nListCount = 0;
		AUTHREG_SERIALTABLE** ppSt_SerialTable;

		Protocol_Parse_HttpParseSerial(lpszMsgBuffer, nMsgLen, &ppSt_SerialTable, &nListCount);
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			for (int i = 0; i < nListCount; i++)
			{
				DBModule_SQLite_SerialInsert(ppSt_SerialTable[i]);
			}
		}
		else 
		{
			for (int i = 0; i < nListCount; i++)
			{
				DBModule_SQLite_SerialInsert(ppSt_SerialTable[i]);
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_SerialTable, nListCount);
		Protocol_Packet_HttpComm(m_MemoryPool.get(), &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPool.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求插入自定义序列号成功,个数:%d"), lpszClientAddr, nListCount);
	}
	return true;
}