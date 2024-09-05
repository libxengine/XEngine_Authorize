#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_TokenTask(LPCXSTR lpszClientAddr, XCHAR** pptszList, int nListCount)
{
	int nSDLen = 4096;
	XCHAR tszSDBuffer[4096] = {};
	XCHAR tszURLKey[128] = {};
	XCHAR tszURLValue[128] = {};
	LPCXSTR lpszAPITime = _X("time");
	LPCXSTR lpszAPIDCode = _X("dcode");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	BaseLib_OperatorString_GetKeyValue(pptszList[0], "=", tszURLKey, tszURLValue);

	if (0 == _tcsxnicmp(lpszAPITime, tszURLValue, _tcsxlen(lpszAPITime)))
	{
		XCHAR tszUserToken[128];
		memset(tszUserToken, '\0', sizeof(tszUserToken));
		BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszURLKey, tszUserToken);

		AUTHREG_USERTABLE st_UserTable = {};
		if (!Session_Token_Get(_ttxoll(tszUserToken), &st_UserTable))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，用户名：%s，获取时间失败，无法继续，错误：%X"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName, Session_GetLastError());
			return false;
		}
		int nListCount = 0;
		AUTHSESSION_NETCLIENT** ppSt_ListClient;
		if (!Session_Authorize_GetClient(&ppSt_ListClient, &nListCount, st_UserTable.st_UserInfo.tszUserName))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，用户名：%s，获取时间失败，无法继续，错误：%X"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName, Session_GetLastError());
			return false;
		}
		Protocol_Packet_UserTime(tszSDBuffer, &nSDLen, &ppSt_ListClient, nListCount);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端：%s，用户名：%s，获取时间成功，用户同时在线数：%d"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName, nListCount);
	}
	else if (0 == _tcsxnicmp(lpszAPIDCode, tszURLValue, _tcsxlen(lpszAPIDCode)))
	{
		if (!st_FunSwitch.bSwitchDCode)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 503, "the function is closed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，请求获得动态验证码失败,服务器已经关闭此功能!"), lpszClientAddr);
			return false;
		}
		//http://app.xyry.org:5302/api?function=dcode
		int nDCode = 0;
		XNETHANDLE xhToken = 0;
		AuthHelp_DynamicCode_Create(&xhToken, &nDCode);
		Protocol_Packet_HttpToken(tszSDBuffer, &nSDLen, xhToken, st_AuthConfig.st_XVerification.nDynamicTimeout, nDCode);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求获得动态验证码成功,TOKEN:%lld,验证码:%d"), lpszClientAddr, xhToken, nDCode);
	}
	return true;
}