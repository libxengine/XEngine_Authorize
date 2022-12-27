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
    BOOL Database_SQLite_Init(LPCTSTR lpszSQLFile, BOOL bIsChange = TRUE);                           //初始化
    BOOL Database_SQLite_Destroy();                                                                  //销毁
public:
    BOOL Database_SQLite_UserDelete(LPCTSTR lpszUserName);                                           //删除用户
    BOOL Database_SQLite_UserRegister(AUTHREG_USERTABLE*pSt_UserInfo);                             //用户注册
    BOOL Database_SQLite_UserQuery(LPCTSTR lpszUserName, AUTHREG_USERTABLE* pSt_UserInfo = NULL);    //用户查询
    BOOL Database_SQLite_UserPay(LPCTSTR lpszUserName,LPCTSTR lpszSerialName);                       //充值卡充值
    BOOL Database_SQLite_UserLeave(AUTHREG_PROTOCOL_TIME* pSt_TimeProtocol);            //用户离开更新表
    BOOL Database_SQLite_UserSet(AUTHREG_USERTABLE* pSt_UserTable);
    BOOL Database_SQLite_UserList(AUTHREG_USERTABLE*** pppSt_UserInfo, int* pInt_ListCount, int nPosStart, int nPosEnd);
public:
    BOOL Database_SQLite_SerialInsert(LPCTSTR lpszSerialNumber);                                     //插入序列卡
    BOOL Database_SQLite_SerialDelete(LPCTSTR lpszSerialNumber);                                     //删除一个序列号
    BOOL Database_SQLite_SerialQuery(LPCTSTR lpszSerialNumber,LPAUTHREG_SERIALTABLE pSt_SerialTable = NULL);                      //按照卡号或者用户查询
    BOOL Database_SQLite_SerialQueryAll(AUTHREG_SERIALTABLE ***pppSt_SerialTable,int *pInt_ListCount, int nPosStart, int nPosEnd);//查询所有序列卡
    BOOL Database_SQLite_SerialPush(AUTHREG_SERIALTABLE* pSt_SerialTable);
public:
    BOOL Database_SQLite_TryInsert(AUTHREG_NETVER* pSt_AuthVer);
    BOOL Database_SQLite_TryQuery(AUTHREG_NETVER* pSt_AuthVer);
    BOOL Database_SQLite_TryDelete(LPCTSTR lpszSerial);
    BOOL Database_SQLite_TryClear(int nThanValue, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode = ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW);
    BOOL Database_SQLite_TrySet(AUTHREG_NETVER* pSt_AuthVer);
public:
    BOOL Database_SQLite_BannedInsert(AUTHREG_BANNED* pSt_Banned);
    BOOL Database_SQLite_BannedDelete(AUTHREG_BANNED* pSt_Banned);
    BOOL Database_SQLite_BannedList(AUTHREG_BANNED*** pppSt_BannedUser, int* pInt_UserCount, AUTHREG_BANNED*** pppSt_BannedAddr, int* pInt_AddrCount);
    BOOL Database_SQLite_BannedExist(AUTHREG_BANNED* pSt_Banned);
protected:
    BOOL Database_SQLite_UserPayTime(LPCTSTR lpszUserName, LPCTSTR lpszUserTime, LPCTSTR lpszCardTime, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthSerialType, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthUserType);
private:
    BOOL m_bChange;
    XHDATA xhData;                 //数据库句柄
};
