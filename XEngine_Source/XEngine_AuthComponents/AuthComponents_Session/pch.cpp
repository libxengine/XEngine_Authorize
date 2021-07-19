#include "pch.h"
#include "AuthService_Session/AuthService_Session.h"
/********************************************************************
//    Created:     2021/07/14  13:23:35
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_Session\pch.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_Session
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
BOOL Session_IsErrorOccur = FALSE;
DWORD Session_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CAuthService_Session m_Session;
//////////////////////////////////////////////////////////////////////////
//                         导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Session_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Session_dwErrorCode;
}
/************************************************************************/
/*                         网络服务导出函数                             */
/************************************************************************/
extern "C" BOOL AuthService_Session_Init(CALLBACK_XENGIEN_AUTHREG_SERVICE_EVENTS fpCall_AuthEvent, LPVOID lParam)
{
	return m_Session.AuthService_Session_Init(fpCall_AuthEvent, lParam);
}
extern "C" BOOL AuthService_Session_GetClient(AUTHREG_USERTABLE * **pppSt_ListClient, int* pInt_ListCount, LPCTSTR lpszClientAddr)
{
	return m_Session.AuthService_Session_GetClient(pppSt_ListClient, pInt_ListCount, lpszClientAddr);
}
extern "C" BOOL AuthService_Session_GetTimer(LPCTSTR lpszUserName, AUTHREG_PROTOCOL_TIME * pSt_AuthTime)
{
	return m_Session.AuthService_Session_GetTimer(lpszUserName, pSt_AuthTime);
}
extern "C" BOOL AuthService_Session_GetAddrForUser(LPCTSTR lpszClientUser, TCHAR * ptszClientAddr)
{
	return m_Session.AuthService_Session_GetAddrForUser(lpszClientUser, ptszClientAddr);
}
extern "C" BOOL AuthService_Session_GetUserForAddr(LPCTSTR lpszClientAddr, TCHAR * ptszClientUser)
{
	return m_Session.AuthService_Session_GetUserForAddr(lpszClientAddr, ptszClientUser);
}
extern "C" BOOL AuthService_Session_CloseClient(LPCTSTR lpszClientUser)
{
	return m_Session.AuthService_Session_CloseClient(lpszClientUser);
}
extern "C" BOOL AuthService_Session_Destroy()
{
	return m_Session.AuthService_Session_Destroy();
}
extern "C" BOOL AuthService_Session_Insert(LPCTSTR lpszClientAddr, AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_Session.AuthService_Session_Insert(lpszClientAddr, pSt_UserTable);
}
extern "C" BOOL AuthService_Session_SetUser(AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_Session.AuthService_Session_SetUser(pSt_UserTable);
}
