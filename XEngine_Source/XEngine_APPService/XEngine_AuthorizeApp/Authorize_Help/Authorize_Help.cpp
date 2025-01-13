#include "pch.h"
#include "../resource.h"
#include "Authorize_Help.h"
/********************************************************************
//    Created:     2022/09/15  09:40:05
//    File Name:   H:\XEngine_Authorize\XEngine_Source\XEngine_APPService\XEngine_AuthorizeApp\Authorize_Help\Authorize_Help.cpp
//    File Path:   H:\XEngine_Authorize\XEngine_Source\XEngine_APPService\XEngine_AuthorizeApp\Authorize_Help
//    File Base:   Authorize_Help
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     帮助函数类
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                           公有函数
//////////////////////////////////////////////////////////////////////////
bool Authorize_Help_LogPrint(LPCTSTR lpszLogBuffer)
{
	TCHAR tszLogBuffer[8196];
	TCHAR tszAddBuffer[8196];
	XCHAR tszTimeStr[64];

	memset(tszLogBuffer, '\0', sizeof(tszLogBuffer));
	memset(tszAddBuffer, '\0', sizeof(tszAddBuffer));
	memset(tszTimeStr, '\0', sizeof(tszTimeStr));

	::GetDlgItemText(hMainWnd, IDC_EDIT1, tszAddBuffer, sizeof(tszAddBuffer));

	USES_CONVERSION;
	BaseLib_Time_TimeToStr(tszTimeStr);
	if (_tcslen(tszAddBuffer) <= 0)
	{
		_stprintf(tszLogBuffer, _T("%s:%s"), A2W(tszTimeStr), lpszLogBuffer);
	}
	else if (_tcslen(tszAddBuffer) >= 8196)
	{
		_stprintf(tszLogBuffer, _T("%s:%s"), A2W(tszTimeStr), lpszLogBuffer);
	}
	else
	{
		_stprintf(tszLogBuffer, _T("%s\r\n%s:%s"), tszAddBuffer, A2W(tszTimeStr), lpszLogBuffer);
	}
	::SetDlgItemText(hMainWnd, IDC_EDIT1, tszLogBuffer);
	return true;
}