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
bool Session_IsErrorOccur = false;
XLONG Session_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CSession_Authorize m_SessionAuth;
CSession_Token m_SessionToken;
//////////////////////////////////////////////////////////////////////////
//                         导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Session_GetLastError(int* pInt_SysError)
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
extern "C" bool Session_Authorize_Init(CALLBACK_XENGIEN_AUTHORIZE_SESSION_CLIENT_EVENTS fpCall_AuthEvent, XPVOID lParam)
{
	return m_SessionAuth.Session_Authorize_Init(fpCall_AuthEvent, lParam);
}
extern "C" bool Session_Authorize_GetClient(AUTHSESSION_NETCLIENT * **pppSt_ListClient, int* pInt_ListCount, LPCXSTR lpszClientAddr)
{
	return m_SessionAuth.Session_Authorize_GetClient(pppSt_ListClient, pInt_ListCount, lpszClientAddr);
}
extern "C" bool Session_Authorize_GetUserForAddr(LPCXSTR lpszClientAddr, AUTHSESSION_NETCLIENT * pSt_Client)
{
	return m_SessionAuth.Session_Authorize_GetUserForAddr(lpszClientAddr, pSt_Client);
}
extern "C" bool Session_Authorize_CloseAddr(LPCXSTR lpszClientAddr)
{
	return m_SessionAuth.Session_Authorize_CloseAddr(lpszClientAddr);
}
extern "C" bool Session_Authorize_Destroy()
{
	return m_SessionAuth.Session_Authorize_Destroy();
}
extern "C" bool Session_Authorize_Insert(LPCXSTR lpszClientAddr, AUTHREG_USERTABLE * pSt_UserTable, int nNetType)
{
	return m_SessionAuth.Session_Authorize_Insert(lpszClientAddr, pSt_UserTable, nNetType);
}
extern "C" bool Session_Authorize_SetUser(AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_SessionAuth.Session_Authorize_SetUser(pSt_UserTable);
}
/************************************************************************/
/*                         TOKEN会话导出函数                            */
/************************************************************************/
extern "C" bool Session_Token_Init(int nTimeout, CALLBACK_XENGIEN_AUTHORIZE_SESSION_TOKEN_EVENTS fpCall_AuthEvent, XPVOID lParam)
{
	return m_SessionToken.Session_Token_Init(nTimeout, fpCall_AuthEvent, lParam);
}
extern "C" bool Session_Token_Destroy()
{
	return m_SessionToken.Session_Token_Destroy();
}
extern "C" bool Session_Token_Insert(XNETHANDLE xhToken, AUTHREG_USERTABLE * pSt_UserTable, int nTimeout)
{
	return m_SessionToken.Session_Token_Insert(xhToken, pSt_UserTable, nTimeout);
}
extern "C" bool Session_Token_Delete(XNETHANDLE xhToken)
{
	return m_SessionToken.Session_Token_Delete(xhToken);
}
extern "C" bool Session_Token_UPDate(XNETHANDLE xhToken)
{
	return m_SessionToken.Session_Token_UPDate(xhToken);
}
extern "C" bool Session_Token_Get(XNETHANDLE xhToken, AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_SessionToken.Session_Token_Get(xhToken, pSt_UserTable);
}
extern "C" bool Session_Token_GetUser(LPCXSTR lpszUser, LPCXSTR lpszPass, XNETHANDLE * pxhToken)
{
	return m_SessionToken.Session_Token_GetUser(lpszUser, lpszPass, pxhToken);
}