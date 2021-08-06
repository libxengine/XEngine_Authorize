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

	pClass_This->m_EditLog.GetWindowText(m_StrLog);
	if (m_StrLog.GetLength() > 1024000)
	{
		m_StrLog.ReleaseBuffer();
		pClass_This->m_EditLog.SetWindowText("");
	}
	m_StrLog += tszLogBuffer;
	m_StrLog += _T("\r\n");
	pClass_This->m_EditLog.SetWindowText(m_StrLog);
	return TRUE;
}