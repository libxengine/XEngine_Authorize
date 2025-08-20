#include "pch.h"
#include "DBModule_SQLite/DBModule_SQLite.h"
#include "DBModule_MySQL/DBModule_MySQL.h"
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
CDBModule_SQLite m_DBSQLite;
CDBModule_MySQL m_DBMySQL;
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
extern "C" bool DBModule_SQLite_Init(LPCXSTR lpszSQLFile, bool bIsChange)
{
	return m_DBSQLite.DBModule_SQLite_Init(lpszSQLFile, bIsChange);
}
extern "C" bool DBModule_SQLite_Destroy()
{
	return m_DBSQLite.DBModule_SQLite_Destroy();
}
extern "C" bool DBModule_SQLite_UserDelete(XENGINE_PROTOCOL_USERINFO* pSt_UserInfo)
{
	return m_DBSQLite.DBModule_SQLite_UserDelete(pSt_UserInfo);
}
extern "C" bool DBModule_SQLite_UserRegister(AUTHREG_USERTABLE * pSt_UserInfo)
{
	return m_DBSQLite.DBModule_SQLite_UserRegister(pSt_UserInfo);
}
extern "C" bool DBModule_SQLite_UserQuery(LPCXSTR lpszUserName, AUTHREG_USERTABLE * pSt_UserInfo, bool bUser)
{
	return m_DBSQLite.DBModule_SQLite_UserQuery(lpszUserName, pSt_UserInfo, bUser);
}
extern "C" bool DBModule_SQLite_UserPay(LPCXSTR lpszUserName, LPCXSTR lpszSerialName)
{
	return m_DBSQLite.DBModule_SQLite_UserPay(lpszUserName, lpszSerialName);
}
extern "C" bool DBModule_SQLite_UserLeave(AUTHREG_PROTOCOL_TIME * pSt_TimeProtocol)
{
	return m_DBSQLite.DBModule_SQLite_UserLeave(pSt_TimeProtocol);
}
extern "C" bool DBModule_SQLite_UserSet(AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_DBSQLite.DBModule_SQLite_UserSet(pSt_UserTable);
}
extern "C" bool DBModule_SQLite_UserList(AUTHREG_USERTABLE * **pppSt_UserInfo, int* pInt_ListCount, int nPosStart, int nPosEnd)
{
	return m_DBSQLite.DBModule_SQLite_UserList(pppSt_UserInfo, pInt_ListCount, nPosStart, nPosEnd);
}
extern "C" bool DBModule_SQLite_UserLogin(LPCXSTR lpszUserName, LPCXSTR lpszUserAddr)
{
	return m_DBSQLite.DBModule_SQLite_UserLogin(lpszUserName, lpszUserAddr);
}
extern "C" bool DBModule_SQLite_QueryLogin(LPCXSTR lpszUserName, LPCXSTR lpszUserAddr)
{
	return m_DBSQLite.DBModule_SQLite_QueryLogin(lpszUserName, lpszUserAddr);
}
extern "C" bool DBModule_SQLite_SerialInsert(LPCXSTR lpszSerialNumber, LPCXSTR lpszExpiredTime)
{
	return m_DBSQLite.DBModule_SQLite_SerialInsert(lpszSerialNumber, lpszExpiredTime);
}
extern "C" bool DBModule_SQLite_SerialDelete(LPCXSTR lpszSerialNumber)
{
	return m_DBSQLite.DBModule_SQLite_SerialDelete(lpszSerialNumber);
}
extern "C" bool DBModule_SQLite_SerialQuery(LPCXSTR lpszSerialNumber, LPAUTHREG_SERIALTABLE pSt_SerialTable)
{
	return m_DBSQLite.DBModule_SQLite_SerialQuery(lpszSerialNumber, pSt_SerialTable);
}
extern "C" bool DBModule_SQLite_SerialQueryAll(AUTHREG_SERIALTABLE * **pppSt_SerialTable, int* pInt_ListCount, int nPosStart, int nPosEnd)
{
	return m_DBSQLite.DBModule_SQLite_SerialQueryAll(pppSt_SerialTable, pInt_ListCount, nPosStart, nPosEnd);
}
extern "C" bool DBModule_SQLite_SerialPush(AUTHREG_SERIALTABLE * pSt_SerialTable)
{
	return m_DBSQLite.DBModule_SQLite_SerialPush(pSt_SerialTable);
}
extern "C" bool DBModule_SQLite_TryInsert(AUTHREG_TEMPVER * pSt_AuthVer)
{
	return m_DBSQLite.DBModule_SQLite_TryInsert(pSt_AuthVer);
}
extern "C" bool DBModule_SQLite_TryQuery(AUTHREG_TEMPVER * pSt_AuthVer)
{
	return m_DBSQLite.DBModule_SQLite_TryQuery(pSt_AuthVer);
}
extern "C" bool DBModule_SQLite_TryDelete(LPCXSTR lpszSerial)
{
	return m_DBSQLite.DBModule_SQLite_TryDelete(lpszSerial);
}
extern "C" bool DBModule_SQLite_TryClear(int nThanValue, ENUM_AUTHORIZE_MODULE_SERIAL_TYPE enVerMode)
{
	return m_DBSQLite.DBModule_SQLite_TryClear(nThanValue, enVerMode);
}
extern "C" bool DBModule_SQLite_TrySet(AUTHREG_TEMPVER * pSt_AuthVer)
{
	return m_DBSQLite.DBModule_SQLite_TrySet(pSt_AuthVer);
}
extern "C" bool DBModule_SQLite_TryList(AUTHREG_TEMPVER * **pppSt_AuthVer, int* pInt_ListCount, int nPosStart, int nPosEnd)
{
	return m_DBSQLite.DBModule_SQLite_TryList(pppSt_AuthVer, pInt_ListCount, nPosStart, nPosEnd);
}
extern "C" bool DBModule_SQLite_BannedInsert(AUTHREG_BANNED * pSt_Banned)
{
	return m_DBSQLite.DBModule_SQLite_BannedInsert(pSt_Banned);
}
extern "C" bool DBModule_SQLite_BannedDelete(AUTHREG_BANNED * pSt_Banned)
{
	return m_DBSQLite.DBModule_SQLite_BannedDelete(pSt_Banned);
}
extern "C" bool DBModule_SQLite_BannedList(AUTHREG_BANNED * **pppSt_BannedUser, int* pInt_UserCount, AUTHREG_BANNED * **pppSt_BannedAddr, int* pInt_AddrCount, int nPosStart, int nPosEnd)
{
	return m_DBSQLite.DBModule_SQLite_BannedList(pppSt_BannedUser, pInt_UserCount, pppSt_BannedAddr, pInt_AddrCount, nPosStart, nPosEnd);
}
extern "C" bool DBModule_SQLite_BannedExist(AUTHREG_BANNED * pSt_Banned)
{
	return m_DBSQLite.DBModule_SQLite_BannedExist(pSt_Banned);
}
extern "C" bool DBModule_SQLite_BannedUPDate(AUTHREG_BANNED * pSt_Banned)
{
	return m_DBSQLite.DBModule_SQLite_BannedUPDate(pSt_Banned);
}
extern "C" bool DBModule_SQLite_AnnouncementInsert(AUTHREG_ANNOUNCEMENT * pSt_Announcement)
{
	return m_DBSQLite.DBModule_SQLite_AnnouncementInsert(pSt_Announcement);
}
extern "C" bool DBModule_SQLite_AnnouncementDelete(AUTHREG_ANNOUNCEMENT * pSt_Announcement)
{
	return m_DBSQLite.DBModule_SQLite_AnnouncementDelete(pSt_Announcement);
}
extern "C" bool DBModule_SQLite_AnnouncementList(AUTHREG_ANNOUNCEMENT * **ppppSt_Announcement, int* pInt_ListCount)
{
	return m_DBSQLite.DBModule_SQLite_AnnouncementList(ppppSt_Announcement, pInt_ListCount);
}
/************************************************************************/
/*                      MYSQL数据库服务导出函数                         */
/************************************************************************/
extern "C" bool DBModule_MySQL_Init(DATABASE_MYSQL_CONNECTINFO* pSt_DBConnector, bool bIsChange)
{
	return m_DBMySQL.DBModule_MySQL_Init(pSt_DBConnector, bIsChange);
}
extern "C" bool DBModule_MySQL_Destroy()
{
	return m_DBMySQL.DBModule_MySQL_Destroy();
}
extern "C" bool DBModule_MySQL_UserDelete(XENGINE_PROTOCOL_USERINFO* pSt_UserInfo)
{
	return m_DBMySQL.DBModule_MySQL_UserDelete(pSt_UserInfo);
}
extern "C" bool DBModule_MySQL_UserRegister(AUTHREG_USERTABLE* pSt_UserInfo)
{
	return m_DBMySQL.DBModule_MySQL_UserRegister(pSt_UserInfo);
}
extern "C" bool DBModule_MySQL_UserQuery(LPCXSTR lpszUserName, AUTHREG_USERTABLE* pSt_UserInfo, bool bUser)
{
	return m_DBMySQL.DBModule_MySQL_UserQuery(lpszUserName, pSt_UserInfo, bUser);
}
extern "C" bool DBModule_MySQL_UserPay(LPCXSTR lpszUserName, LPCXSTR lpszSerialName)
{
	return m_DBMySQL.DBModule_MySQL_UserPay(lpszUserName, lpszSerialName);
}
extern "C" bool DBModule_MySQL_UserLeave(AUTHREG_PROTOCOL_TIME* pSt_TimeProtocol)
{
	return m_DBMySQL.DBModule_MySQL_UserLeave(pSt_TimeProtocol);
}
extern "C" bool DBModule_MySQL_UserSet(AUTHREG_USERTABLE* pSt_UserTable)
{
	return m_DBMySQL.DBModule_MySQL_UserSet(pSt_UserTable);
}
extern "C" bool DBModule_MySQL_UserList(AUTHREG_USERTABLE*** pppSt_UserInfo, int* pInt_ListCount, int nPosStart, int nPosEnd)
{
	return m_DBMySQL.DBModule_MySQL_UserList(pppSt_UserInfo, pInt_ListCount, nPosStart, nPosEnd);
}
extern "C" bool DBModule_MySQL_SerialInsert(LPCXSTR lpszSerialNumber, LPCXSTR lpszExpiredTime)
{
	return m_DBMySQL.DBModule_MySQL_SerialInsert(lpszSerialNumber, lpszExpiredTime);
}
extern "C" bool DBModule_MySQL_SerialDelete(LPCXSTR lpszSerialNumber)
{
	return m_DBMySQL.DBModule_MySQL_SerialDelete(lpszSerialNumber);
}
extern "C" bool DBModule_MySQL_SerialQuery(LPCXSTR lpszSerialNumber, LPAUTHREG_SERIALTABLE pSt_SerialTable)
{
	return m_DBMySQL.DBModule_MySQL_SerialQuery(lpszSerialNumber, pSt_SerialTable);
}
extern "C" bool DBModule_MySQL_SerialQueryAll(AUTHREG_SERIALTABLE*** pppSt_SerialTable, int* pInt_ListCount, int nPosStart, int nPosEnd)
{
	return m_DBMySQL.DBModule_MySQL_SerialQueryAll(pppSt_SerialTable, pInt_ListCount, nPosStart, nPosEnd);
}
extern "C" bool DBModule_MySQL_SerialPush(AUTHREG_SERIALTABLE* pSt_SerialTable)
{
	return m_DBMySQL.DBModule_MySQL_SerialPush(pSt_SerialTable);
}
extern "C" bool DBModule_MySQL_TryInsert(AUTHREG_TEMPVER* pSt_AuthVer)
{
	return m_DBMySQL.DBModule_MySQL_TryInsert(pSt_AuthVer);
}
extern "C" bool DBModule_MySQL_TryQuery(AUTHREG_TEMPVER* pSt_AuthVer)
{
	return m_DBMySQL.DBModule_MySQL_TryQuery(pSt_AuthVer);
}
extern "C" bool DBModule_MySQL_TryDelete(LPCXSTR lpszSerial)
{
	return m_DBMySQL.DBModule_MySQL_TryDelete(lpszSerial);
}
extern "C" bool DBModule_MySQL_TryClear(int nThanValue, ENUM_AUTHORIZE_MODULE_SERIAL_TYPE enVerMode)
{
	return m_DBMySQL.DBModule_MySQL_TryClear(nThanValue, enVerMode);
}
extern "C" bool DBModule_MySQL_TrySet(AUTHREG_TEMPVER* pSt_AuthVer)
{
	return m_DBMySQL.DBModule_MySQL_TrySet(pSt_AuthVer);
}
extern "C" bool DBModule_MySQL_TryList(AUTHREG_TEMPVER*** pppSt_AuthVer, int* pInt_ListCount, int nPosStart, int nPosEnd)
{
	return m_DBMySQL.DBModule_MySQL_TryList(pppSt_AuthVer, pInt_ListCount, nPosStart, nPosEnd);
}
extern "C" bool DBModule_MySQL_BannedInsert(AUTHREG_BANNED* pSt_Banned)
{
	return m_DBMySQL.DBModule_MySQL_BannedInsert(pSt_Banned);
}
extern "C" bool DBModule_MySQL_BannedDelete(AUTHREG_BANNED* pSt_Banned)
{
	return m_DBMySQL.DBModule_MySQL_BannedDelete(pSt_Banned);
}
extern "C" bool DBModule_MySQL_BannedList(AUTHREG_BANNED*** pppSt_BannedUser, int* pInt_UserCount, AUTHREG_BANNED*** pppSt_BannedAddr, int* pInt_AddrCount, int nPosStart, int nPosEnd)
{
	return m_DBMySQL.DBModule_MySQL_BannedList(pppSt_BannedUser, pInt_UserCount, pppSt_BannedAddr, pInt_AddrCount, nPosStart, nPosEnd);
}
extern "C" bool DBModule_MySQL_BannedExist(AUTHREG_BANNED* pSt_Banned)
{
	return m_DBMySQL.DBModule_MySQL_BannedExist(pSt_Banned);
}
extern "C" bool DBModule_MySQL_BannedUPDate(AUTHREG_BANNED* pSt_Banned)
{
	return m_DBMySQL.DBModule_MySQL_BannedUPDate(pSt_Banned);
}
extern "C" bool DBModule_MySQL_AnnouncementInsert(AUTHREG_ANNOUNCEMENT* pSt_Announcement)
{
	return m_DBMySQL.DBModule_MySQL_AnnouncementInsert(pSt_Announcement);
}
extern "C" bool DBModule_MySQL_AnnouncementDelete(AUTHREG_ANNOUNCEMENT* pSt_Announcement)
{
	return m_DBMySQL.DBModule_MySQL_AnnouncementDelete(pSt_Announcement);
}
extern "C" bool DBModule_MySQL_AnnouncementList(AUTHREG_ANNOUNCEMENT*** ppppSt_Announcement, int* pInt_ListCount)
{
	return m_DBMySQL.DBModule_MySQL_AnnouncementList(ppppSt_Announcement, pInt_ListCount);
}
extern "C" bool DBModule_MySQL_UserLogin(LPCXSTR lpszUserName, LPCXSTR lpszUserAddr)
{
	return m_DBMySQL.DBModule_MySQL_UserLogin(lpszUserName, lpszUserAddr);
}
extern "C" bool DBModule_MySQL_QueryLogin(LPCXSTR lpszUserName, LPCXSTR lpszUserAddr)
{
	return m_DBMySQL.DBModule_MySQL_QueryLogin(lpszUserName, lpszUserAddr);
}