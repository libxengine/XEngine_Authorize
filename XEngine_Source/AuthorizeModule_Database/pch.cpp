#include "pch.h"
#include "Database_SQLite/Database_SQLite.h"
/********************************************************************
//    Created:     2022/05/26  11:02:23
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database\pch.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database
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
CDatabase_SQLite m_SQLAuth;
//////////////////////////////////////////////////////////////////////////
//                         导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD DBModule_GetLastError(int* pInt_SysError)
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
extern "C" BOOL Database_SQLite_Init(LPCTSTR lpszSQLFile, BOOL bIsChange)
{
	return m_SQLAuth.Database_SQLite_Init(lpszSQLFile, bIsChange);
}
extern "C" BOOL Database_SQLite_Destroy()
{
	return m_SQLAuth.Database_SQLite_Destroy();
}
extern "C" BOOL Database_SQLite_UserDelete(LPCTSTR lpszUserName)
{
	return m_SQLAuth.Database_SQLite_UserDelete(lpszUserName);
}
extern "C" BOOL Database_SQLite_UserRegister(AUTHREG_USERTABLE * pSt_UserInfo)
{
	return m_SQLAuth.Database_SQLite_UserRegister(pSt_UserInfo);
}
extern "C" BOOL Database_SQLite_UserQuery(LPCTSTR lpszUserName, AUTHREG_USERTABLE * pSt_UserInfo)
{
	return m_SQLAuth.Database_SQLite_UserQuery(lpszUserName, pSt_UserInfo);
}
extern "C" BOOL Database_SQLite_UserPay(LPCTSTR lpszUserName, LPCTSTR lpszSerialName)
{
	return m_SQLAuth.Database_SQLite_UserPay(lpszUserName, lpszSerialName);
}
extern "C" BOOL Database_SQLite_UserLeave(AUTHREG_PROTOCOL_TIME * pSt_TimeProtocol)
{
	return m_SQLAuth.Database_SQLite_UserLeave(pSt_TimeProtocol);
}
extern "C" BOOL Database_SQLite_UserSet(AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_SQLAuth.Database_SQLite_UserSet(pSt_UserTable);
}
extern "C" BOOL Database_SQLite_UserList(AUTHREG_USERTABLE * **pppSt_UserInfo, int* pInt_ListCount)
{
	return m_SQLAuth.Database_SQLite_UserList(pppSt_UserInfo, pInt_ListCount);
}
extern "C" BOOL Database_SQLite_SerialInsert(LPCTSTR lpszSerialNumber)
{
	return m_SQLAuth.Database_SQLite_SerialInsert(lpszSerialNumber);
}
extern "C" BOOL Database_SQLite_SerialDelete(LPCTSTR lpszSerialNumber)
{
	return m_SQLAuth.Database_SQLite_SerialDelete(lpszSerialNumber);
}
extern "C" BOOL Database_SQLite_SerialQuery(LPCTSTR lpszSerialNumber, LPAUTHREG_SERIALTABLE pSt_SerialTable)
{
	return m_SQLAuth.Database_SQLite_SerialQuery(lpszSerialNumber, pSt_SerialTable);
}
extern "C" BOOL Database_SQLite_SerialQueryAll(AUTHREG_SERIALTABLE * **pppSt_SerialTable, int* pInt_ListCount)
{
	return m_SQLAuth.Database_SQLite_SerialQueryAll(pppSt_SerialTable, pInt_ListCount);
}
extern "C" BOOL Database_SQLite_TryInsert(AUTHREG_NETVER * pSt_AuthVer)
{
	return m_SQLAuth.Database_SQLite_TryInsert(pSt_AuthVer);
}
extern "C" BOOL Database_SQLite_TryQuery(AUTHREG_NETVER * pSt_AuthVer)
{
	return m_SQLAuth.Database_SQLite_TryQuery(pSt_AuthVer);
}
extern "C" BOOL Database_SQLite_TryDelete(LPCTSTR lpszSerial)
{
	return m_SQLAuth.Database_SQLite_TryDelete(lpszSerial);
}
extern "C" BOOL Database_SQLite_TryClear(int nThanValue, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode)
{
	return m_SQLAuth.Database_SQLite_TryClear(nThanValue, enVerMode);
}
extern "C" BOOL Database_SQLite_TrySet(AUTHREG_NETVER * pSt_AuthVer)
{
	return m_SQLAuth.Database_SQLite_TrySet(pSt_AuthVer);
}