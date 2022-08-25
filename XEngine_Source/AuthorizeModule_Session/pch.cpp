#include "pch.h"
#include "Session_Authorize/Session_Authorize.h"
#include "Session_Token/Session_Token.h"
/********************************************************************
//    Created:     2022/05/26  10:53:58
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\pch.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session
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
CSession_Authorize m_SessionAuth;
CSession_Token m_SessionToken;
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
extern "C" BOOL Session_Authorize_Init(CALLBACK_XENGIEN_AUTHORIZE_SESSION_CLIENT_EVENTS fpCall_AuthEvent, LPVOID lParam)
{
	return m_SessionAuth.Session_Authorize_Init(fpCall_AuthEvent, lParam);
}
extern "C" BOOL Session_Authorize_GetClient(AUTHREG_USERTABLE * **pppSt_ListClient, int* pInt_ListCount, LPCTSTR lpszClientAddr)
{
	return m_SessionAuth.Session_Authorize_GetClient(pppSt_ListClient, pInt_ListCount, lpszClientAddr);
}
extern "C" BOOL Session_Authorize_GetTimer(LPCTSTR lpszUserName, AUTHREG_PROTOCOL_TIME * pSt_AuthTime)
{
	return m_SessionAuth.Session_Authorize_GetTimer(lpszUserName, pSt_AuthTime);
}
extern "C" BOOL Session_Authorize_GetAddrForUser(LPCTSTR lpszClientUser, TCHAR * ptszClientAddr)
{
	return m_SessionAuth.Session_Authorize_GetAddrForUser(lpszClientUser, ptszClientAddr);
}
extern "C" BOOL Session_Authorize_GetUserForAddr(LPCTSTR lpszClientAddr, TCHAR * ptszClientUser)
{
	return m_SessionAuth.Session_Authorize_GetUserForAddr(lpszClientAddr, ptszClientUser);
}
extern "C" BOOL Session_Authorize_CloseClient(LPCTSTR lpszClientUser)
{
	return m_SessionAuth.Session_Authorize_CloseClient(lpszClientUser);
}
extern "C" BOOL Session_Authorize_Destroy()
{
	return m_SessionAuth.Session_Authorize_Destroy();
}
extern "C" BOOL Session_Authorize_Insert(LPCTSTR lpszClientAddr, AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_SessionAuth.Session_Authorize_Insert(lpszClientAddr, pSt_UserTable);
}
extern "C" BOOL Session_Authorize_SetUser(AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_SessionAuth.Session_Authorize_SetUser(pSt_UserTable);
}
/************************************************************************/
/*                         TOKEN会话导出函数                            */
/************************************************************************/
extern "C" BOOL Session_Token_Init(int nTimeout, CALLBACK_XENGIEN_AUTHORIZE_SESSION_TOKEN_EVENTS fpCall_AuthEvent, LPVOID lParam)
{
	return m_SessionToken.Session_Token_Init(nTimeout, fpCall_AuthEvent, lParam);
}
extern "C" BOOL Session_Token_Destroy()
{
	return m_SessionToken.Session_Token_Destroy();
}
extern "C" BOOL Session_Token_Insert(XNETHANDLE xhToken, AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_SessionToken.Session_Token_Insert(xhToken, pSt_UserTable);
}
extern "C" BOOL Session_Token_Delete(XNETHANDLE xhToken)
{
	return m_SessionToken.Session_Token_Delete(xhToken);
}
extern "C" BOOL Session_Token_UPDate(XNETHANDLE xhToken)
{
	return m_SessionToken.Session_Token_UPDate(xhToken);
}
extern "C" BOOL Session_Token_Get(XNETHANDLE xhToken, AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_SessionToken.Session_Token_Get(xhToken, pSt_UserTable);
}
extern "C" BOOL Session_Token_GetUser(LPCTSTR lpszUser, LPCTSTR lpszPass)
{
	return m_SessionToken.Session_Token_GetUser(lpszUser, lpszPass);
}