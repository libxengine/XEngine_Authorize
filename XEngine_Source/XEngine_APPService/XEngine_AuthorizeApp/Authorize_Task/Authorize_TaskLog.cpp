#include "pch.h"

#define XLOG_BUFFER_SIZE 16384

BOOL XEngine_Authorize_LogPrint(LPVOID lParam, LPCSTR lpszLog, ...)
{
	CXEngineAuthorizeAppDlg* pClass_This = (CXEngineAuthorizeAppDlg*)lParam;
	CString m_StrLog;

	TCHAR tszLogBuffer[XLOG_BUFFER_SIZE];
	memset(tszLogBuffer, '\0', sizeof(tszLogBuffer));

	va_list pArgList;
	va_start(pArgList, lpszLog);
	vsnprintf(tszLogBuffer, XLOG_BUFFER_SIZE, lpszLog, pArgList);
	va_end(pArgList);

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, tszLogBuffer);
	pClass_This->m_EditLog.GetWindowText(m_StrLog);
	if (m_StrLog.GetLength() > 1024000)
	{
		m_StrLog.ReleaseBuffer();
		pClass_This->m_EditLog.SetWindowText("");
	}
	m_StrLog += tszLogBuffer;
	m_StrLog += _T("\r\n");
	pClass_This->m_EditLog.SetWindowText(m_StrLog);
	pClass_This->m_EditLog.LineScroll(pClass_This->m_EditLog.GetLineCount(), -1);
	return TRUE;
}