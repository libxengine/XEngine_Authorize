#pragma once
/********************************************************************
//    Created:     2022/05/26  11:01:06
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database\Database_SQLite\Database_SQLite.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database\Database_SQLite
//    File Base:   Database_SQLite
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     数据库管理器
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
// AuthReg_UserTable内容：ID 用户名 密码 注册日期 剩余时间 硬件码 注册文件密码 QQ 身份证
// AuthReg_SerialTable内容：ID 使用者 序列号 使用时间 最大可允许时间 是否使用
//////////////////////////////////////////////////////////////////////////
class CDatabase_SQLite
{
public:
    CDatabase_SQLite();
    ~CDatabase_SQLite();
public:
    bool Database_SQLite_Init(LPCXSTR lpszSQLFile, bool bIsChange = true);                           //初始化
    bool Database_SQLite_Destroy();                                                                  //销毁
public:
    bool Database_SQLite_UserDelete(LPCXSTR lpszUserName);                                           //删除用户
    bool Database_SQLite_UserRegister(AUTHREG_USERTABLE*pSt_UserInfo);                             //用户注册
    bool Database_SQLite_UserQuery(LPCXSTR lpszUserName, AUTHREG_USERTABLE* pSt_UserInfo = NULL);    //用户查询
    bool Database_SQLite_UserPay(LPCXSTR lpszUserName,LPCXSTR lpszSerialName);                       //充值卡充值
    bool Database_SQLite_UserLeave(AUTHREG_PROTOCOL_TIME* pSt_TimeProtocol);            //用户离开更新表
    bool Database_SQLite_UserSet(AUTHREG_USERTABLE* pSt_UserTable);
    bool Database_SQLite_UserList(AUTHREG_USERTABLE*** pppSt_UserInfo, int* pInt_ListCount, int nPosStart, int nPosEnd);
public:
    bool Database_SQLite_SerialInsert(LPCXSTR lpszSerialNumber);                                     //插入序列卡
    bool Database_SQLite_SerialDelete(LPCXSTR lpszSerialNumber);                                     //删除一个序列号
    bool Database_SQLite_SerialQuery(LPCXSTR lpszSerialNumber,LPAUTHREG_SERIALTABLE pSt_SerialTable = NULL);                      //按照卡号或者用户查询
    bool Database_SQLite_SerialQueryAll(AUTHREG_SERIALTABLE ***pppSt_SerialTable,int *pInt_ListCount, int nPosStart, int nPosEnd);//查询所有序列卡
    bool Database_SQLite_SerialPush(AUTHREG_SERIALTABLE* pSt_SerialTable);
public:
    bool Database_SQLite_TryInsert(AUTHREG_NETVER* pSt_AuthVer);
    bool Database_SQLite_TryQuery(AUTHREG_NETVER* pSt_AuthVer);
    bool Database_SQLite_TryDelete(LPCXSTR lpszSerial);
    bool Database_SQLite_TryClear(int nThanValue, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode = ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW);
    bool Database_SQLite_TrySet(AUTHREG_NETVER* pSt_AuthVer);
public:
    bool Database_SQLite_BannedInsert(AUTHREG_BANNED* pSt_Banned);
    bool Database_SQLite_BannedDelete(AUTHREG_BANNED* pSt_Banned);
    bool Database_SQLite_BannedList(AUTHREG_BANNED*** pppSt_BannedUser, int* pInt_UserCount, AUTHREG_BANNED*** pppSt_BannedAddr, int* pInt_AddrCount, int nPosStart, int nPosEnd);
    bool Database_SQLite_BannedExist(AUTHREG_BANNED* pSt_Banned);
    bool Database_SQLite_BannedUPDate(AUTHREG_BANNED* pSt_Banned);
public:
	bool Database_SQLite_AnnouncementInsert(AUTHREG_ANNOUNCEMENT* pSt_Announcement);
	bool Database_SQLite_AnnouncementDelete(AUTHREG_ANNOUNCEMENT* pSt_Announcement);
    bool Database_SQLite_AnnouncementList(AUTHREG_ANNOUNCEMENT*** ppppSt_Announcement, int* pInt_ListCount);
protected:
    bool Database_SQLite_UserPayTime(LPCXSTR lpszUserName, LPCXSTR lpszUserTime, LPCXSTR lpszCardTime, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthSerialType, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthUserType);
private:
    bool m_bChange;
    XNETHANDLE xhData;                 //数据库句柄
};
