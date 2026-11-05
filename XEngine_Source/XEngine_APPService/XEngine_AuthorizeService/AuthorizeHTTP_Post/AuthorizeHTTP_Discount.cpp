#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Discount(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	int nSDLen = 4096;
	XCHAR tszSDBuffer[4096] = {};
	LPCXSTR lpszAPISet = _X("set");
	LPCXSTR lpszAPIGet = _X("get");

	if (!st_FunSwitch.bSwitchDiscount)
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_CLOSED, "the function is closed");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，设置失败，设置功能已经被服务器关闭!"), lpszClientAddr);
		return false;
	}
	if (0 == _tcsxncmp(lpszAPISet, lpszAPIName, _tcsxlen(lpszAPISet)))
	{
		//http://127.0.0.1:5302/api/discount/set
		AUTHREG_DISCOUNT st_AuthDiscount = {};
		if (!Protocol_Parse_HttpParseDiscount(lpszMSGBuffer, nMSGLen, &st_AuthDiscount))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "request failure,json parse incorrect");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求折扣系统数据失败,解析失败:%s"), lpszClientAddr, lpszMSGBuffer);
			return false;
		}
		st_AuthConfig.st_XDiscount.dlPayDiscount = st_AuthDiscount.dlDiscount;
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求设置折扣系统数据成功, 用户:%s, 类型:%d, 折扣:%lf"), lpszClientAddr, st_AuthDiscount.tszUserName, st_AuthDiscount.nType, st_AuthDiscount.dlDiscount);
	}
	else if (0 == _tcsxncmp(lpszAPIGet, lpszAPIName, _tcsxlen(lpszAPIGet)))
	{
		//http://127.0.0.1:5302/api/discount/get
		AUTHREG_DISCOUNT st_AuthDiscount = {};

		st_AuthDiscount.dlDiscount = st_AuthConfig.st_XDiscount.dlPayDiscount;
		Protocol_Packet_HttpDiscount(tszSDBuffer, &nSDLen, &st_AuthDiscount);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求获取折扣系统数据成功"), lpszClientAddr);
	}

	return true;
}