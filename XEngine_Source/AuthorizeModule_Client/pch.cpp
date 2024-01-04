#include "pch.h"
#include "AuthClient_Connector/AuthClient_Connector.h"
/********************************************************************
//    Created:     2023/11/15  11:28:22
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client\pch.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool AuthClient_IsErrorOccur = false;
XLONG AuthClient_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CAuthClient_Connector m_Connector;
//////////////////////////////////////////////////////////////////////////
//                         导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG AuthClient_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return AuthClient_dwErrorCode;
}
/************************************************************************/
/*                         验证客户端导出函数                           */
/************************************************************************/
#if (1 == _XAUTH_BUILD_SWITCH_CLIENT_NETWORK)
extern "C" bool AuthClient_Connector_Connect(LPCXSTR lpszClientAddr, int nPort, LPCXSTR lpszPass)
{
	return m_Connector.AuthClient_Connector_Connect(lpszClientAddr, nPort, lpszPass);
}
extern "C" bool AuthClient_Connector_Close()
{
	return m_Connector.AuthClient_Connector_Close();
}
extern "C" bool AuthClient_Connector_GetAuth(bool* pbAuth)
{
	return m_Connector.AuthClient_Connector_GetAuth(pbAuth);
}
extern "C" bool AuthClient_Connector_Login(LPCXSTR lpszUser, LPCXSTR lpszPass, int nDYCode, XNETHANDLE xhToken)
{
	return m_Connector.AuthClient_Connector_Login(lpszUser, lpszPass, nDYCode, xhToken);
}
#endif