#include "../Authorize_Hdr.h"

XBOOL XEngine_AuthorizeHTTP_Switch(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 4096;
	XCHAR tszSDBuffer[4096];
	LPCXSTR lpszAPIGet = _T("get");
	LPCXSTR lpszAPISet = _T("set");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsnicmp(lpszAPIGet, lpszAPIName, _tcslen(lpszAPIGet)))
	{
		Protocol_Packet_HttpSwitch(tszSDBuffer, &nSDLen, &st_FunSwitch);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求获取功能开关成功,删除功能:%d,登录功能:%d,找回密码:%d,充值功能:%d,注册功能:%d,CDKey功能:%d,公告系统:%d"), lpszClientAddr, st_FunSwitch.bSwitchDelete, st_FunSwitch.bSwitchLogin, st_FunSwitch.bSwitchPass, st_FunSwitch.bSwitchPay, st_FunSwitch.bSwitchRegister, st_FunSwitch.bSwitchCDKey, st_FunSwitch.bSwitchNotice);
	}
	else if (0 == _tcsnicmp(lpszAPISet, lpszAPIName, _tcslen(lpszAPISet)))
	{
		Protocol_Parse_HttpParseSwitch(lpszMsgBuffer, nMsgLen, &st_FunSwitch);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求设置功能开关成功,删除功能:%d,登录功能:%d,找回密码:%d,充值功能:%d,注册功能:%d,CDKey功能:%d,公告系统:%d"), lpszClientAddr, st_FunSwitch.bSwitchDelete, st_FunSwitch.bSwitchLogin, st_FunSwitch.bSwitchPass, st_FunSwitch.bSwitchPay, st_FunSwitch.bSwitchRegister, st_FunSwitch.bSwitchCDKey, st_FunSwitch.bSwitchNotice);
	}
	
	return XTRUE;
}