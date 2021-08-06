#include "pch.h"
#include "AuthService_SQLPacket/AuthService_SQLPacket.h"
/********************************************************************
//    Created:     2021/07/14  13:09:15
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_SQLPacket\pch.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_SQLPacket
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
BOOL SQLPacket_IsErrorOccur = FALSE;
DWORD SQLPacket_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CAuthService_SQLPacket m_SQLAuth;
//////////////////////////////////////////////////////////////////////////
//                         导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD SQLPacket_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return SQLPacket_dwErrorCode;
}
/************************************************************************/
/*                         数据库服务导出函数                           */
/************************************************************************/
extern "C" BOOL AuthService_SQLPacket_Init(LPCTSTR lpszSQLFile, BOOL bIsChange)
{
	return m_SQLAuth.AuthService_SQLPacket_Init(lpszSQLFile, bIsChange);
}
extern "C" BOOL AuthService_SQLPacket_Destroy()
{
	return m_SQLAuth.AuthService_SQLPacket_Destroy();
}
extern "C" BOOL AuthService_SQLPacket_UserDelete(LPCTSTR lpszUserName)
{
	return m_SQLAuth.AuthService_SQLPacket_UserDelete(lpszUserName);
}
extern "C" BOOL AuthService_SQLPacket_UserRegister(AUTHREG_USERTABLE *pSt_UserInfo)
{
	return m_SQLAuth.AuthService_SQLPacket_UserRegister(pSt_UserInfo);
}
extern "C" BOOL AuthService_SQLPacket_UserQuery(LPCTSTR lpszUserName, AUTHREG_USERTABLE * pSt_UserInfo)
{
	return m_SQLAuth.AuthService_SQLPacket_UserQuery(lpszUserName, pSt_UserInfo);
}
extern "C" BOOL AuthService_SQLPacket_UserPay(LPCTSTR lpszUserName, LPCTSTR lpszSerialName)
{
	return m_SQLAuth.AuthService_SQLPacket_UserPay(lpszUserName, lpszSerialName);
}
extern "C" BOOL AuthService_SQLPacket_UserLeave(AUTHREG_PROTOCOL_TIME * pSt_TimeProtocol)
{
	return m_SQLAuth.AuthService_SQLPacket_UserLeave(pSt_TimeProtocol);
}
extern "C" BOOL AuthService_SQLPacket_UserSet(AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_SQLAuth.AuthService_SQLPacket_UserSet(pSt_UserTable);
}
extern "C" BOOL AuthService_SQLPacket_SerialInsert(LPCTSTR lpszSerialNumber)
{
	return m_SQLAuth.AuthService_SQLPacket_SerialInsert(lpszSerialNumber);
}
extern "C" BOOL AuthService_SQLPacket_SerialDelete(LPCTSTR lpszSerialNumber)
{
	return m_SQLAuth.AuthService_SQLPacket_SerialDelete(lpszSerialNumber);
}
extern "C" BOOL AuthService_SQLPacket_SerialQuery(LPCTSTR lpszSerialNumber, LPAUTHREG_SERIALTABLE pSt_SerialTable)
{
	return m_SQLAuth.AuthService_SQLPacket_SerialQuery(lpszSerialNumber, pSt_SerialTable);
}
extern "C" BOOL AuthService_SQLPacket_SerialQueryAll(AUTHREG_SERIALTABLE * **pppSt_SerialTable, int* pInt_ListCount)
{
	return m_SQLAuth.AuthService_SQLPacket_SerialQueryAll(pppSt_SerialTable, pInt_ListCount);
}
extern "C" BOOL AuthService_SQLPacket_TryInsert(AUTHREG_NETVER * pSt_AuthVer)
{
	return m_SQLAuth.AuthService_SQLPacket_TryInsert(pSt_AuthVer);
}
extern "C" BOOL AuthService_SQLPacket_TryQuery(AUTHREG_NETVER * pSt_AuthVer)
{
	return m_SQLAuth.AuthService_SQLPacket_TryQuery(pSt_AuthVer);
}
extern "C" BOOL AuthService_SQLPacket_TryDelete(LPCTSTR lpszSerial)
{
	return m_SQLAuth.AuthService_SQLPacket_TryDelete(lpszSerial);
}
extern "C" BOOL AuthService_SQLPacket_TryClear(int nThanValue, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode)
{
	return m_SQLAuth.AuthService_SQLPacket_TryClear(nThanValue, enVerMode);
}
extern "C" BOOL AuthService_SQLPacket_TrySet(AUTHREG_NETVER * pSt_AuthVer)
{
	return m_SQLAuth.AuthService_SQLPacket_TrySet(pSt_AuthVer);
}