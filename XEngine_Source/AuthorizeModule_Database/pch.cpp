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
bool SQLPacket_IsErrorOccur = false;
XLONG SQLPacket_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CDatabase_SQLite m_SQLAuth;
//////////////////////////////////////////////////////////////////////////
//                         导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG DBModule_GetLastError(int* pInt_SysError)
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
extern "C" bool Database_SQLite_Init(LPCXSTR lpszSQLFile, bool bIsChange)
{
	return m_SQLAuth.Database_SQLite_Init(lpszSQLFile, bIsChange);
}
extern "C" bool Database_SQLite_Destroy()
{
	return m_SQLAuth.Database_SQLite_Destroy();
}
extern "C" bool Database_SQLite_UserDelete(LPCXSTR lpszUserName)
{
	return m_SQLAuth.Database_SQLite_UserDelete(lpszUserName);
}
extern "C" bool Database_SQLite_UserRegister(AUTHREG_USERTABLE * pSt_UserInfo)
{
	return m_SQLAuth.Database_SQLite_UserRegister(pSt_UserInfo);
}
extern "C" bool Database_SQLite_UserQuery(LPCXSTR lpszUserName, AUTHREG_USERTABLE * pSt_UserInfo)
{
	return m_SQLAuth.Database_SQLite_UserQuery(lpszUserName, pSt_UserInfo);
}
extern "C" bool Database_SQLite_UserPay(LPCXSTR lpszUserName, LPCXSTR lpszSerialName)
{
	return m_SQLAuth.Database_SQLite_UserPay(lpszUserName, lpszSerialName);
}
extern "C" bool Database_SQLite_UserLeave(AUTHREG_PROTOCOL_TIME * pSt_TimeProtocol)
{
	return m_SQLAuth.Database_SQLite_UserLeave(pSt_TimeProtocol);
}
extern "C" bool Database_SQLite_UserSet(AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_SQLAuth.Database_SQLite_UserSet(pSt_UserTable);
}
extern "C" bool Database_SQLite_UserList(AUTHREG_USERTABLE * **pppSt_UserInfo, int* pInt_ListCount, int nPosStart, int nPosEnd)
{
	return m_SQLAuth.Database_SQLite_UserList(pppSt_UserInfo, pInt_ListCount, nPosStart, nPosEnd);
}
extern "C" bool Database_SQLite_SerialInsert(LPCXSTR lpszSerialNumber)
{
	return m_SQLAuth.Database_SQLite_SerialInsert(lpszSerialNumber);
}
extern "C" bool Database_SQLite_SerialDelete(LPCXSTR lpszSerialNumber)
{
	return m_SQLAuth.Database_SQLite_SerialDelete(lpszSerialNumber);
}
extern "C" bool Database_SQLite_SerialQuery(LPCXSTR lpszSerialNumber, LPAUTHREG_SERIALTABLE pSt_SerialTable)
{
	return m_SQLAuth.Database_SQLite_SerialQuery(lpszSerialNumber, pSt_SerialTable);
}
extern "C" bool Database_SQLite_SerialQueryAll(AUTHREG_SERIALTABLE * **pppSt_SerialTable, int* pInt_ListCount, int nPosStart, int nPosEnd)
{
	return m_SQLAuth.Database_SQLite_SerialQueryAll(pppSt_SerialTable, pInt_ListCount, nPosStart, nPosEnd);
}
extern "C" bool Database_SQLite_SerialPush(AUTHREG_SERIALTABLE * pSt_SerialTable)
{
	return m_SQLAuth.Database_SQLite_SerialPush(pSt_SerialTable);
}
extern "C" bool Database_SQLite_TryInsert(AUTHREG_NETVER * pSt_AuthVer)
{
	return m_SQLAuth.Database_SQLite_TryInsert(pSt_AuthVer);
}
extern "C" bool Database_SQLite_TryQuery(AUTHREG_NETVER * pSt_AuthVer)
{
	return m_SQLAuth.Database_SQLite_TryQuery(pSt_AuthVer);
}
extern "C" bool Database_SQLite_TryDelete(LPCXSTR lpszSerial)
{
	return m_SQLAuth.Database_SQLite_TryDelete(lpszSerial);
}
extern "C" bool Database_SQLite_TryClear(int nThanValue, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode)
{
	return m_SQLAuth.Database_SQLite_TryClear(nThanValue, enVerMode);
}
extern "C" bool Database_SQLite_TrySet(AUTHREG_NETVER * pSt_AuthVer)
{
	return m_SQLAuth.Database_SQLite_TrySet(pSt_AuthVer);
}
extern "C" bool Database_SQLite_BannedInsert(AUTHREG_BANNED * pSt_Banned)
{
	return m_SQLAuth.Database_SQLite_BannedInsert(pSt_Banned);
}
extern "C" bool Database_SQLite_BannedDelete(AUTHREG_BANNED * pSt_Banned)
{
	return m_SQLAuth.Database_SQLite_BannedDelete(pSt_Banned);
}
extern "C" bool Database_SQLite_BannedList(AUTHREG_BANNED * **pppSt_BannedUser, int* pInt_UserCount, AUTHREG_BANNED * **pppSt_BannedAddr, int* pInt_AddrCount, int nPosStart, int nPosEnd)
{
	return m_SQLAuth.Database_SQLite_BannedList(pppSt_BannedUser, pInt_UserCount, pppSt_BannedAddr, pInt_AddrCount, nPosStart, nPosEnd);
}
extern "C" bool Database_SQLite_BannedExist(AUTHREG_BANNED * pSt_Banned)
{
	return m_SQLAuth.Database_SQLite_BannedExist(pSt_Banned);
}
extern "C" bool Database_SQLite_BannedUPDate(AUTHREG_BANNED * pSt_Banned)
{
	return m_SQLAuth.Database_SQLite_BannedUPDate(pSt_Banned);
}
extern "C" bool Database_SQLite_AnnouncementInsert(AUTHREG_ANNOUNCEMENT * pSt_Announcement)
{
	return m_SQLAuth.Database_SQLite_AnnouncementInsert(pSt_Announcement);
}
extern "C" bool Database_SQLite_AnnouncementDelete(AUTHREG_ANNOUNCEMENT * pSt_Announcement)
{
	return m_SQLAuth.Database_SQLite_AnnouncementDelete(pSt_Announcement);
}
extern "C" bool Database_SQLite_AnnouncementList(AUTHREG_ANNOUNCEMENT * **ppppSt_Announcement, int* pInt_ListCount)
{
	return m_SQLAuth.Database_SQLite_AnnouncementList(ppppSt_Announcement, pInt_ListCount);
}