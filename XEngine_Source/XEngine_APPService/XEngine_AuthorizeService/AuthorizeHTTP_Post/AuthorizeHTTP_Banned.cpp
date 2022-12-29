#include "../Authorize_Hdr.h"

BOOL XEngine_AuthorizeHTTP_Banned(LPCTSTR lpszClientAddr, LPCTSTR lpszAPIName, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 4096;
	TCHAR tszSDBuffer[4096];
	LPCTSTR lpszAPIInsert = _T("insert");
	LPCTSTR lpszAPIDelete = _T("delete");
	LPCTSTR lpszAPIList = _T("list");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsnicmp(lpszAPIInsert, lpszAPIName, _tcslen(lpszAPIInsert)))
	{
		AUTHREG_BANNED st_Banned;
		memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));

		Protocol_Packet_HttpSwitch(tszSDBuffer, &nSDLen, &st_FunSwitch);
		Database_SQLite_BannedInsert(&st_Banned);

		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求获取功能开关成功,删除功能:%d,登录功能:%d,找回密码:%d,充值功能:%d,注册功能:%d"), lpszClientAddr, st_FunSwitch.bSwitchDelete, st_FunSwitch.bSwitchLogin, st_FunSwitch.bSwitchPass, st_FunSwitch.bSwitchPay, st_FunSwitch.bSwitchRegister);
	}
	else if (0 == _tcsnicmp(lpszAPIDelete, lpszAPIName, _tcslen(lpszAPIDelete)))
	{
	}
	
	return TRUE;
}