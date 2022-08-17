#include "../Authorize_Hdr.h"

BOOL XEngine_AuthorizeHTTP_Serial(LPCTSTR lpszClientAddr, LPCTSTR lpszAPIName, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	LPCTSTR lpszAPIList = _T("list");
	LPCTSTR lpszAPIInsert = _T("insert");
	LPCTSTR lpszAPIDelete = _T("delete");
	int nSDLen = 4096;
	TCHAR tszSDBuffer[4096];

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsnicmp(lpszAPIList, lpszAPIName, _tcslen(lpszAPIList)))
	{
		int nListCount = 0;
		TCHAR* ptszMsgBuffer = (TCHAR*)malloc(XENGINE_AUTH_MAX_BUFFER);
		if (NULL == ptszMsgBuffer)
		{
			return FALSE;
		}
		memset(ptszMsgBuffer, '\0', XENGINE_AUTH_MAX_BUFFER);

		AUTHREG_SERIALTABLE** ppSt_SerialTable;
		Database_SQLite_SerialQueryAll(&ppSt_SerialTable, &nListCount);
		Protocol_Packet_HttpSerialList(ptszMsgBuffer, &nSDLen, &ppSt_SerialTable, nListCount);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_SerialTable, nListCount);
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, ptszMsgBuffer, nSDLen);
		free(ptszMsgBuffer);
		ptszMsgBuffer = NULL;
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求序列号列表成功,个数:%d"), lpszClientAddr, nListCount);
	}
	else if (0 == _tcsnicmp(lpszAPIInsert, lpszAPIName, _tcslen(lpszAPIInsert)))
	{
		int nNumberCount = 0;
		int nSerialCount = 0;
		TCHAR tszHasTime[128];
		XENGINE_LIBTIMER st_AuthTimer;
		ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType;

		memset(&st_AuthTimer, '\0', sizeof(st_AuthTimer));
		memset(tszHasTime, '\0', sizeof(tszHasTime));

		Protocol_Parse_HttpParseSerial2(lpszMsgBuffer, nMsgLen, &enSerialType, &nNumberCount, &nSerialCount, tszHasTime);
		//解析类型
		if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_MINUTE == enSerialType)
		{
			st_AuthTimer.wMinute = _ttoi(tszHasTime);
		}
		else if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_DAY == enSerialType)
		{
			st_AuthTimer.wDay = _ttoi(tszHasTime);
		}
		else if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == enSerialType)
		{
			st_AuthTimer.wFlags = _ttoi(tszHasTime);
		}
		else if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_CUSTOM == enSerialType)
		{
			if (6 != _stscanf_s(tszHasTime, _T("%04d-%02d-%02d %02d:%02d:%02d"), &st_AuthTimer.wYear, &st_AuthTimer.wMonth, &st_AuthTimer.wDay, &st_AuthTimer.wHour, &st_AuthTimer.wMinute, &st_AuthTimer.wSecond))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "time request is failed");
				XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求插入序列卡失败,时间格式错误"), lpszClientAddr);
				return FALSE;
			}
		}
		else
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 415, "not support serial types");
			XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求插入序列卡失败,不支持的类型格式:%d"), lpszClientAddr, enSerialType);
			return FALSE;
		}
		//生成卡
		TCHAR** pptszSerialNumber;
		LPCTSTR lpszUserHdr = _T("XAUTH");
		if (!Authorize_Serial_Creator(&pptszSerialNumber, lpszUserHdr, nSerialCount, nNumberCount, &st_AuthTimer, enSerialType))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 500, "Internal Server Error");
			XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,创建序列卡失败,错误码:%lX"), lpszClientAddr, Authorize_GetLastError());
			return FALSE;
		}
		//导入序列卡
		for (int i = 0; i < nSerialCount; i++)
		{
			Database_SQLite_SerialInsert(pptszSerialNumber[i]);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszSerialNumber, nSerialCount);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求插入序列号成功,个数:%d"), lpszClientAddr, nSerialCount);
	}
	else if (0 == _tcsnicmp(lpszAPIDelete, lpszAPIName, _tcslen(lpszAPIDelete)))
	{
		int nListCount = 0;
		AUTHREG_SERIALTABLE** ppSt_SerialTable;

		Protocol_Parse_HttpParseSerial(lpszMsgBuffer, nMsgLen, &ppSt_SerialTable, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			Database_SQLite_SerialDelete(ppSt_SerialTable[i]->tszSerialNumber);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_SerialTable, nListCount);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求删除序列号成功,删除个数:%d"), lpszClientAddr, nListCount);
	}
	return TRUE;
}