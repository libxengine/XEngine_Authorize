#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_CDKey(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 8196;
	int nRVLen = 8196;
	XCHAR tszSDBuffer[8196];
	XCHAR tszRVBuffer[8196];
	LPCXSTR lpszAPICreate = _X("create");
	LPCXSTR lpszAPIAuth = _X("auth");
	LPCXSTR lpszAPIVer = _X("ver");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));

	if (!st_FunSwitch.bSwitchCDKey)
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 503, "the function is closed");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，CDKey验证授权失败，功能已经被服务器关闭!"), lpszClientAddr);
		return false;
	}
	if (0 == _tcsxnicmp(lpszAPICreate, lpszAPIName, _tcsxlen(lpszAPICreate)))
	{
		XENGINE_AUTHORIZE_LOCAL st_Authorize;
		memset(&st_Authorize, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));

		Protocol_Parse_HttpParseCDKey(lpszMsgBuffer, nMsgLen, &st_Authorize);

		if (_tcsxlen(st_Authorize.st_AuthSerial.st_TimeLimit.tszTimeSerial) <= 0)
		{
			int nSerialCount = 3;
			XCHAR** pptszSerialList;
			Authorize_Serial_Create(&pptszSerialList, _X("XAUTH"), nSerialCount, 9);

			st_Authorize.st_AuthSerial.st_TimeLimit.nTimeCount = 9999;
			_tcsxcpy(st_Authorize.st_AuthSerial.st_TimeLimit.tszTimeSerial, pptszSerialList[0]);

			st_Authorize.st_AuthSerial.st_DataLimit.bTimeAdd = false;
			_tcsxcpy(st_Authorize.st_AuthSerial.st_DataLimit.tszDataSerial, pptszSerialList[1]);
			XCHAR tszTimeStr[128] = {};
			XENGINE_LIBTIMER st_LibTime = {};
			BaseLib_OperatorTime_GetSysTime(&st_LibTime);
			st_LibTime.wYear += 1; //一年后过期
			BaseLib_OperatorTime_TimeToStr(tszTimeStr, NULL, true, &st_LibTime);

			_tcsxcpy(st_Authorize.st_AuthSerial.st_DataLimit.tszDataTime, tszTimeStr);

			_tcsxcpy(st_Authorize.st_AuthSerial.st_UNLimit.tszUNLimitSerial, pptszSerialList[2]);
			BaseLib_OperatorMemory_Free((XPPPMEM)&pptszSerialList, nSerialCount);
		}
		
		if (!Authorize_CDKey_WriteMemory(tszRVBuffer, &nRVLen, &st_Authorize))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 406, "Not Acceptable,write key failed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,请求创建CDKEY协议失败,服务器内部错误：%lX"), lpszClientAddr, Authorize_GetLastError());
			return false;
		}
		XEngine_Client_TaskSend(lpszClientAddr, tszRVBuffer, nRVLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,请求创建CDKEY成功,APP名:%s,APP版本:%s"), lpszClientAddr, st_Authorize.st_AuthAppInfo.tszAppName, st_Authorize.st_AuthAppInfo.tszAppVer);
	}
	else if (0 == _tcsxnicmp(lpszAPIAuth, lpszAPIName, _tcsxlen(lpszAPIAuth)))
	{
		XENGINE_AUTHORIZE_LOCAL st_Authorize;
		memset(&st_Authorize, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));

		Authorize_CDKey_ReadMemory(lpszMsgBuffer, nMsgLen, &st_Authorize);
		//授权
		if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_CUSTOM == st_Authorize.st_AuthRegInfo.enSerialType)
		{
			XENGINE_LIBTIMER st_LibTime;
			memset(&st_LibTime, '\0', sizeof(XENGINE_LIBTIMER));

			BaseLib_OperatorTime_StrToTime(st_Authorize.st_AuthRegInfo.tszLeftTime, &st_LibTime);
			Authorize_CDKey_BuildKeyTime(&st_Authorize, 0, &st_LibTime);
		}
		else
		{
			Authorize_CDKey_BuildKeyTime(&st_Authorize, _ttxoll(st_Authorize.st_AuthRegInfo.tszLeftTime));
		}
		Authorize_CDKey_WriteMemory(tszRVBuffer, &nRVLen, &st_Authorize);
		XEngine_Client_TaskSend(lpszClientAddr, tszRVBuffer, nRVLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,请求授权CDKEY成功,APP名:%s,APP版本:%s,授权期限:%s"), lpszClientAddr, st_Authorize.st_AuthAppInfo.tszAppName, st_Authorize.st_AuthAppInfo.tszAppVer, st_Authorize.st_AuthRegInfo.tszLeftTime);
	}
	else if (0 == _tcsxnicmp(lpszAPIVer, lpszAPIName, _tcsxlen(lpszAPIVer)))
	{
		XENGINE_AUTHORIZE_LOCAL st_Authorize;
		memset(&st_Authorize, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));

		Authorize_CDKey_ReadMemory(lpszMsgBuffer, nMsgLen, &st_Authorize);
		if (!Authorize_CDKey_GetLeftTimer(&st_Authorize))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "Unauthorized,cdkey is not authorized");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,请求验证CDKEY失败,cdkey未授权或者已超时,错误：%lX"), lpszClientAddr, Authorize_GetLastError());
			return false;
		}
		Authorize_CDKey_WriteMemory(tszRVBuffer, &nRVLen, &st_Authorize);
		XEngine_Client_TaskSend(lpszClientAddr, tszRVBuffer, nRVLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,请求验证CDKEY成功,APP名:%s,APP版本:%s,授权期限:%s"), lpszClientAddr, st_Authorize.st_AuthAppInfo.tszAppName, st_Authorize.st_AuthAppInfo.tszAppVer, st_Authorize.st_AuthRegInfo.tszLeftTime);
	}
	else
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "Not support protocol");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,请求了一条未知的子协议：%s"), lpszClientAddr, lpszAPIName);
	}
	return true;
}