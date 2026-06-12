#include "../Authorize_Hdr.h"

// Handle HTTP serial-number related APIs.
// Supported operations:
// 1) list   : query serials by page/range and return packed HTTP response.
// 2) delete : parse serial list from request and delete each entry.
// 3) insert : parse and insert serial entries (handled in the insert branch below).
// Notes:
// - Database backend is selected by st_AuthConfig.st_XSql.nDBType.
// - Response data is sent back through XEngine_Client_TaskSend.
// - Dynamically allocated parse/query buffers must be released after use.
bool XEngine_AuthorizeHTTP_Serial(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 0;
	LPCXSTR lpszAPIList = _X("list");
	LPCXSTR lpszAPIInsert = _X("insert");
	LPCXSTR lpszAPIDelete = _X("delete");

	CHttpMemory_PoolEx m_MemoryPool(XENGINE_MEMORY_SIZE_MAX);
	// list: parse paging range, query DB, package list response, then return to client.
	if (0 == _tcsxncmp(lpszAPIList, lpszAPIName, _tcsxlen(lpszAPIList)))
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

		// Read list window from HTTP payload and limit single request size.
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
	// delete: parse requested serials and delete one-by-one in configured DB backend.
	else if (0 == _tcsxncmp(lpszAPIDelete, lpszAPIName, _tcsxlen(lpszAPIDelete)))
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
		// Release parsed serial table and return common success response.
		BaseLib_Memory_Free((XPPPMEM)&ppSt_SerialTable, nListCount);
		Protocol_Packet_HttpComm(m_MemoryPool.get(), &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPool.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求删除序列号成功,删除个数:%d"), lpszClientAddr, nListCount);
	}
	// insert: parse and insert serial data, then reply to client (implementation below).
	else if (0 == _tcsxncmp(lpszAPIInsert, lpszAPIName, _tcsxlen(lpszAPIInsert)))
	{
		int nSerialCount = 0;
		int nFieldCount = 0;
		ENUM_VERIFICATION_MODULE_SERIAL_TYPE enSerialType;
		XCHAR tszExpiredTime[64] = {};
		XCHAR tszMaxTime[64] = {};
		XCHAR tszCreateTime[64] = {};

		Protocol_Parse_HttpParseSerial2(lpszMsgBuffer, nMsgLen, tszExpiredTime, tszMaxTime, &nSerialCount, &nFieldCount, &enSerialType);
		BaseLib_Time_TimeToStr(tszCreateTime);

		for (int i = 0; i < nSerialCount; i++)
		{
			AUTHREG_SERIALTABLE st_SerialTable = {};
			st_SerialTable.bIsUsed = false;
			st_SerialTable.enSerialType = enSerialType;
			_xstprintf(st_SerialTable.tszUserName, _X("NOT"));
			_tcsxcpy(st_SerialTable.tszCreateTime, tszCreateTime);
			_tcsxcpy(st_SerialTable.tszExpiredTime, tszExpiredTime);
			_tcsxcpy(st_SerialTable.tszMaxTime, tszMaxTime);
			Verification_XAuthKey_KeySerial(st_SerialTable.tszSerialNumber, nFieldCount, 0);

			if (0 == st_AuthConfig.st_XSql.nDBType)
			{
				DBModule_SQLite_SerialInsert(&st_SerialTable);
			}
			else
			{
				DBModule_SQLite_SerialInsert(&st_SerialTable);
			}
		}
		Protocol_Packet_HttpComm(m_MemoryPool.get(), &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPool.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求插入自定义序列号成功,个数:%d"), lpszClientAddr, nSerialCount);
	}
	return true;
}