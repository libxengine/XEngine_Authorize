#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Serial(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 8196;
	XCHAR tszSDBuffer[8196];
	LPCXSTR lpszAPIList = _X("list");
	LPCXSTR lpszAPIInsert = _X("insert");
	LPCXSTR lpszAPIDelete = _X("delete");
	LPCXSTR lpszAPIPush = _X("push");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsxnicmp(lpszAPIList, lpszAPIName, _tcsxlen(lpszAPIList)))
	{
		int nPosStart = 0;
		int nPosEnd = 0;
		int nListCount = 0;
		XCHAR* ptszMsgBuffer = (XCHAR*)malloc(XENGINE_AUTH_MAX_BUFFER);
		if (NULL == ptszMsgBuffer)
		{
			return false;
		}
		memset(ptszMsgBuffer, '\0', XENGINE_AUTH_MAX_BUFFER);

		Protocol_Parse_HttpParsePos(lpszMsgBuffer, nMsgLen, &nPosStart, &nPosEnd);
		if ((nPosEnd - nPosStart) > 100)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "pos parament is not rigth");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
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
	else if (0 == _tcsxnicmp(lpszAPIInsert, lpszAPIName, _tcsxlen(lpszAPIInsert)))
	{
		int nNumberCount = 0;
		int nSerialCount = 0;
		XCHAR tszHasTime[128];
		XENGINE_LIBTIMER st_AuthTimer;
		ENUM_AUTHORIZE_MODULE_SERIAL_TYPE enSerialType;

		memset(&st_AuthTimer, '\0', sizeof(st_AuthTimer));
		memset(tszHasTime, '\0', sizeof(tszHasTime));

		Protocol_Parse_HttpParseSerial2(lpszMsgBuffer, nMsgLen, &enSerialType, &nNumberCount, &nSerialCount, tszHasTime);
		//解析类型
		if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_SECOND == enSerialType)
		{
			st_AuthTimer.wSecond = _ttxoi(tszHasTime);
		}
		else if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_DAY == enSerialType)
		{
			st_AuthTimer.wDay = _ttxoi(tszHasTime);
		}
		else if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_TIME == enSerialType)
		{
			st_AuthTimer.wFlags = _ttxoi(tszHasTime);
		}
		else if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_CUSTOM == enSerialType)
		{
			if (6 != _stxscanf(tszHasTime, _X("%04d-%02d-%02d %02d:%02d:%02d"), &st_AuthTimer.wYear, &st_AuthTimer.wMonth, &st_AuthTimer.wDay, &st_AuthTimer.wHour, &st_AuthTimer.wMinute, &st_AuthTimer.wSecond))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "time request is failed");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求插入序列卡失败,时间格式错误"), lpszClientAddr);
				return false;
			}
		}
		else
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 415, "not support serial types");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求插入序列卡失败,不支持的类型格式:%d"), lpszClientAddr, enSerialType);
			return false;
		}
		//生成卡
		XCHAR** pptszSerialNumber;
		LPCXSTR lpszUserHdr = _X("XAUTH");
		if (!Authorize_Serial_Creator(&pptszSerialNumber, lpszUserHdr, nSerialCount, nNumberCount, &st_AuthTimer, enSerialType))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 500, "Internal Server Error");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,创建序列卡失败,错误码:%lX"), lpszClientAddr, Authorize_GetLastError());
			return false;
		}
		//导入序列卡
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			for (int i = 0; i < nSerialCount; i++) //导入序列卡
			{
				DBModule_SQLite_SerialInsert(pptszSerialNumber[i]);
			}
		}
		else 
		{
			for (int i = 0; i < nSerialCount; i++) 
			{
				DBModule_MySQL_SerialInsert(pptszSerialNumber[i]);
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&pptszSerialNumber, nSerialCount);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求插入序列号成功,个数:%d"), lpszClientAddr, nSerialCount);
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
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求删除序列号成功,删除个数:%d"), lpszClientAddr, nListCount);
	}
	else if (0 == _tcsxnicmp(lpszAPIPush, lpszAPIName, _tcsxlen(lpszAPIPush)))
	{
		int nListCount = 0;
		AUTHREG_SERIALTABLE** ppSt_SerialTable;

		Protocol_Parse_HttpParseSerial(lpszMsgBuffer, nMsgLen, &ppSt_SerialTable, &nListCount);
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			for (int i = 0; i < nListCount; i++)
			{
				DBModule_SQLite_SerialPush(ppSt_SerialTable[i]);
			}
		}
		else 
		{
			for (int i = 0; i < nListCount; i++)
			{
				DBModule_MySQL_SerialPush(ppSt_SerialTable[i]);
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_SerialTable, nListCount);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求推送自定义序列号成功,个数:%d"), lpszClientAddr, nListCount);
	}
	return true;
}