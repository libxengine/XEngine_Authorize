#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Switch(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 8196;
	XCHAR tszSDBuffer[8196];
	LPCXSTR lpszAPIGet = _X("get");
	LPCXSTR lpszAPISet = _X("set");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsxnicmp(lpszAPIGet, lpszAPIName, _tcsxlen(lpszAPIGet)))
	{
		Protocol_Packet_HttpSwitch(tszSDBuffer, &nSDLen, &st_FunSwitch);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求获取功能开关成功,删除功能:%d,登录功能:%d,找回密码:%d,充值功能:%d,注册功能:%d,CDKey功能:%d,公告系统:%d,多端登录:%d,临时试用:%d"), lpszClientAddr, st_FunSwitch.bSwitchDelete, st_FunSwitch.bSwitchLogin, st_FunSwitch.bSwitchPass, st_FunSwitch.bSwitchPay, st_FunSwitch.bSwitchRegister, st_FunSwitch.bSwitchCDKey, st_FunSwitch.bSwitchNotice, st_FunSwitch.bSwitchMulti, st_FunSwitch.bSwitchTry);
	}
	else if (0 == _tcsxnicmp(lpszAPISet, lpszAPIName, _tcsxlen(lpszAPISet)))
	{
		Protocol_Parse_HttpParseSwitch(lpszMsgBuffer, nMsgLen, &st_FunSwitch);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求设置功能开关成功,删除功能:%d,登录功能:%d,找回密码:%d,充值功能:%d,注册功能:%d,CDKey功能:%d,公告系统:%d,多端登录:%d,临时试用:%d"), lpszClientAddr, st_FunSwitch.bSwitchDelete, st_FunSwitch.bSwitchLogin, st_FunSwitch.bSwitchPass, st_FunSwitch.bSwitchPay, st_FunSwitch.bSwitchRegister, st_FunSwitch.bSwitchCDKey, st_FunSwitch.bSwitchNotice, st_FunSwitch.bSwitchMulti, st_FunSwitch.bSwitchTry);
	}
	
	return true;
}