﻿#include "../Authorize_Hdr.h"

BOOL XEngine_AuthorizeHTTP_CDKey(LPCTSTR lpszClientAddr, LPCTSTR lpszAPIName, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 4096;
	int nRVLen = 4096;
	TCHAR tszSDBuffer[4096];
	TCHAR tszRVBuffer[4096];
	LPCTSTR lpszAPICreate = _T("create");
	LPCTSTR lpszAPIAuth = _T("auth");
	LPCTSTR lpszAPIVer = _T("ver");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));

	if (!st_FunSwitch.bSwitchCDKey)
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 503, "the function is closed");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，CDKey验证授权失败，功能已经被服务器关闭!"), lpszClientAddr);
		return FALSE;
	}
	if (0 == _tcsnicmp(lpszAPICreate, lpszAPIName, _tcslen(lpszAPICreate)))
	{
		XENGINE_AUTHORIZE_LOCAL st_Authorize;
		memset(&st_Authorize, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));

		Protocol_Parse_HttpParseCDKey(lpszMsgBuffer, nMsgLen, &st_Authorize);
		if (!Authorize_Local_WriteMemory(tszRVBuffer, &nRVLen, &st_Authorize))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 406, "Not Acceptable,write key failed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("HTTP客户端:%s,请求创建CDKEY协议失败,服务器内部错误：%lX"), lpszClientAddr, Authorize_GetLastError());
			return FALSE;
		}
		XEngine_Client_TaskSend(lpszClientAddr, tszRVBuffer, nRVLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("HTTP客户端:%s,请求创建CDKEY成功,APP名:%s,APP版本:%s"), lpszClientAddr, st_Authorize.st_AuthAppInfo.tszAppName, st_Authorize.st_AuthAppInfo.tszAppVer);
	}
	else if (0 == _tcsnicmp(lpszAPIAuth, lpszAPIName, _tcslen(lpszAPIAuth)))
	{
		XENGINE_AUTHORIZE_LOCAL st_Authorize;
		memset(&st_Authorize, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));

		Authorize_Local_ReadMemory(lpszMsgBuffer, nMsgLen, &st_Authorize);
		//授权
		if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_CUSTOM == st_Authorize.st_AuthRegInfo.enSerialType)
		{
			XENGINE_LIBTIMER st_LibTime;
			memset(&st_LibTime, '\0', sizeof(XENGINE_LIBTIMER));

			BaseLib_OperatorTime_StrToTime(st_Authorize.st_AuthRegInfo.tszLeftTime, &st_LibTime);
			Authorize_Local_BuildKeyTime(&st_Authorize, 0, &st_LibTime);
		}
		else
		{
			Authorize_Local_BuildKeyTime(&st_Authorize, _ttoi64(st_Authorize.st_AuthRegInfo.tszLeftTime));
		}
		Authorize_Local_WriteMemory(tszRVBuffer, &nRVLen, &st_Authorize);
		XEngine_Client_TaskSend(lpszClientAddr, tszRVBuffer, nRVLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("HTTP客户端:%s,请求授权CDKEY成功,APP名:%s,APP版本:%s,授权期限:%s"), lpszClientAddr, st_Authorize.st_AuthAppInfo.tszAppName, st_Authorize.st_AuthAppInfo.tszAppVer, st_Authorize.st_AuthRegInfo.tszLeftTime);
	}
	else if (0 == _tcsnicmp(lpszAPIVer, lpszAPIName, _tcslen(lpszAPIVer)))
	{
		XENGINE_AUTHORIZE_LOCAL st_Authorize;
		memset(&st_Authorize, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));

		Authorize_Local_ReadMemory(lpszMsgBuffer, nMsgLen, &st_Authorize);
		if (!Authorize_Local_GetLeftTimer(&st_Authorize))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "Unauthorized,cdkey is not authorized");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("HTTP客户端:%s,请求验证CDKEY失败,cdkey未授权或者已超时,错误：%lX"), lpszClientAddr, Authorize_GetLastError());
			return FALSE;
		}
		Authorize_Local_WriteMemory(tszRVBuffer, &nRVLen, &st_Authorize);
		XEngine_Client_TaskSend(lpszClientAddr, tszRVBuffer, nRVLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("HTTP客户端:%s,请求验证CDKEY成功,APP名:%s,APP版本:%s,授权期限:%s"), lpszClientAddr, st_Authorize.st_AuthAppInfo.tszAppName, st_Authorize.st_AuthAppInfo.tszAppVer, st_Authorize.st_AuthRegInfo.tszLeftTime);
	}
	else
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "Not support protocol");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("HTTP客户端:%s,请求了一条未知的子协议：%s"), lpszClientAddr, lpszAPIName);
	}
	return TRUE;
}