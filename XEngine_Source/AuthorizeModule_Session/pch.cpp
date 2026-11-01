#include "pch.h"
#include "Session_Authorize/Session_Authorize.h"
#include "Session_IPAddress/Session_IPAddress.h"
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
CSession_IPAddress m_SessionIPAddr;
//////////////////////////////////////////////////////////////////////////
//                         导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG AuthSession_GetLastError(int* pInt_SysError)
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
extern "C" bool Session_Authorize_Insert(LPCXSTR lpszClientAddr, AUTHREG_USERTABLE * pSt_UserTable, XNETHANDLE xhToken, int nNetType)
{
	return m_SessionAuth.Session_Authorize_Insert(lpszClientAddr, pSt_UserTable, xhToken, nNetType);
}
extern "C" bool Session_Authorize_SetUser(AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_SessionAuth.Session_Authorize_SetUser(pSt_UserTable);
}
/************************************************************************/
/*                         IP地址会话导出函数                           */
/************************************************************************/
extern "C" bool Session_IPAddress_Init(CALLBACK_AUTHORIZE_MODULE_SESSION_IPADDR_EVENTS fpCall_IPAddrEvent, int nTimeout, XPVOID lParam)
{
	return m_SessionIPAddr.Session_IPAddress_Init(fpCall_IPAddrEvent, nTimeout, lParam);
}
extern "C" bool Session_IPAddress_Get(LPCXSTR lpszIPAddr)
{
	return m_SessionIPAddr.Session_IPAddress_Get(lpszIPAddr);
}
extern "C" bool Session_IPAddress_Insert(LPCXSTR lpszClientAddr)
{
	return m_SessionIPAddr.Session_IPAddress_Insert(lpszClientAddr);
}
extern "C" bool Session_IPAddress_Close(LPCXSTR lpszClientAddr)
{
	return m_SessionIPAddr.Session_IPAddress_Close(lpszClientAddr);
}
extern "C" bool Session_IPAddress_Destroy()
{
	return m_SessionIPAddr.Session_IPAddress_Destroy();
}