#pragma once
/********************************************************************
//    Created:     2022/05/26  11:01:06
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database\Database_MySQL\Database_MySQL.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database\Database_MySQL
//    File Base:   Database_MySQL
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
class CDBModule_MySQL
{
public:
    CDBModule_MySQL();
    ~CDBModule_MySQL();
public:
    bool DBModule_MySQL_Init(DATABASE_MYSQL_CONNECTINFO* pSt_DBConnector, bool bIsChange = true);                           //初始化
    bool DBModule_MySQL_Destroy();                                                                  //销毁
public:
    bool DBModule_MySQL_UserDelete(LPCXSTR lpszUserName);                                           //删除用户
    bool DBModule_MySQL_UserRegister(AUTHREG_USERTABLE* pSt_UserInfo);                               //用户注册
    bool DBModule_MySQL_UserQuery(LPCXSTR lpszUserName, AUTHREG_USERTABLE* pSt_UserInfo = NULL);    //用户查询
    bool DBModule_MySQL_UserPay(LPCXSTR lpszUserName, LPCXSTR lpszSerialName);                       //充值卡充值
    bool DBModule_MySQL_UserLeave(AUTHREG_PROTOCOL_TIME* pSt_TimeProtocol);            //用户离开更新表
    bool DBModule_MySQL_UserSet(AUTHREG_USERTABLE* pSt_UserTable);                      //设置用户信息
    bool DBModule_MySQL_UserList(AUTHREG_USERTABLE*** pppSt_UserInfo, int* pInt_ListCount, int nPosStart, int nPosEnd); //获取用户列表
public:
    bool DBModule_MySQL_SerialInsert(LPCXSTR lpszSerialNumber);                                     //插入序列卡
    bool DBModule_MySQL_SerialDelete(LPCXSTR lpszSerialNumber);                                     //删除一个序列号
    bool DBModule_MySQL_SerialQuery(LPCXSTR lpszSerialNumber, LPAUTHREG_SERIALTABLE pSt_SerialTable = NULL);                      //按照卡号或者用户查询
    bool DBModule_MySQL_SerialQueryAll(AUTHREG_SERIALTABLE*** pppSt_SerialTable, int* pInt_ListCount, int nPosStart, int nPosEnd);//查询所有序列卡
    bool DBModule_MySQL_SerialPush(AUTHREG_SERIALTABLE* pSt_SerialTable);//插入一条指定的序列号信息到服务器
public:
    bool DBModule_MySQL_TryInsert(AUTHREG_TEMPVER* pSt_AuthVer);        //网络使用模式插入一条数据
    bool DBModule_MySQL_TryQuery(AUTHREG_TEMPVER* pSt_AuthVer);         //试用序列号查询函数
    bool DBModule_MySQL_TryDelete(LPCXSTR lpszSerial);                  //删除一条指定的试用数据
    bool DBModule_MySQL_TryClear(int nThanValue, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode = ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_UNKNOW);               //清理函数,自动清理过期数据
    bool DBModule_MySQL_TrySet(AUTHREG_TEMPVER* pSt_AuthVer);           //设置用户信息函数
    bool DBModule_MySQL_TryList(AUTHREG_TEMPVER*** pppSt_AuthVer, int* pInt_ListCount, int nPosStart = 0, int nPosEnd = 1000); //请求试用期列表
public:
    bool DBModule_MySQL_BannedInsert(AUTHREG_BANNED* pSt_Banned);   //黑名单列表插入
    bool DBModule_MySQL_BannedDelete(AUTHREG_BANNED* pSt_Banned);   //黑名单列表删除
    bool DBModule_MySQL_BannedList(AUTHREG_BANNED*** pppSt_BannedUser, int* pInt_UserCount, AUTHREG_BANNED*** pppSt_BannedAddr, int* pInt_AddrCount, int nPosStart, int nPosEnd);                   //黑名单列表查询
    bool DBModule_MySQL_BannedExist(AUTHREG_BANNED* pSt_Banned);    //名单是否存在黑名单列表
    bool DBModule_MySQL_BannedUPDate(AUTHREG_BANNED* pSt_Banned);   //更新名单列表信息
public:
    bool DBModule_MySQL_AnnouncementInsert(AUTHREG_ANNOUNCEMENT* pSt_Announcement);  //公告插入
    bool DBModule_MySQL_AnnouncementDelete(AUTHREG_ANNOUNCEMENT* pSt_Announcement);  //公告删除
    bool DBModule_MySQL_AnnouncementList(AUTHREG_ANNOUNCEMENT*** ppppSt_Announcement, int* pInt_ListCount);  //列举所有公告
protected:
    bool DBModule_MySQL_UserPayTime(LPCXSTR lpszUserName, LPCXSTR lpszUserTime, LPCXSTR lpszCardTime, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthSerialType, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthUserType);  //日期方式充值方式
private:
    bool m_bChange;
    XNETHANDLE xhData;                 //数据库句柄
};
