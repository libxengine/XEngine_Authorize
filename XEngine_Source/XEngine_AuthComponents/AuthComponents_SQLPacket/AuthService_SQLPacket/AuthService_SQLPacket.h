#pragma once
/********************************************************************
//    Created:     2021/07/14  11:40:09
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_SQLPacket\AuthService_SQLPacket\AuthService_SQLPacket.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_SQLPacket\AuthService_SQLPacket
//    File Base:   AuthService_SQLPacket
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
class CAuthService_SQLPacket
{
public:
    CAuthService_SQLPacket();
    ~CAuthService_SQLPacket();
public:
    BOOL AuthService_SQLPacket_Init(LPCTSTR lpszSQLFile, BOOL bIsChange = TRUE);                           //初始化
    BOOL AuthService_SQLPacket_Destroy();                                                                  //销毁
public:
    BOOL AuthService_SQLPacket_UserDelete(LPCTSTR lpszUserName);                                           //删除用户
    BOOL AuthService_SQLPacket_UserRegister(AUTHREG_USERTABLE*pSt_UserInfo);                             //用户注册
    BOOL AuthService_SQLPacket_UserQuery(LPCTSTR lpszUserName, AUTHREG_USERTABLE* pSt_UserInfo = NULL);    //用户查询
    BOOL AuthService_SQLPacket_UserPay(LPCTSTR lpszUserName,LPCTSTR lpszSerialName);                       //充值卡充值
    BOOL AuthService_SQLPacket_UserLeave(AUTHREG_PROTOCOL_TIME* pSt_TimeProtocol);            //用户离开更新表
    BOOL AuthService_SQLPacket_UserSet(AUTHREG_USERTABLE* pSt_UserTable);
public:
    BOOL AuthService_SQLPacket_SerialInsert(LPCTSTR lpszSerialNumber);                                     //插入序列卡
    BOOL AuthService_SQLPacket_SerialDelete(LPCTSTR lpszSerialNumber);                                     //删除一个序列号
    BOOL AuthService_SQLPacket_SerialQuery(LPCTSTR lpszSerialNumber,LPAUTHREG_SERIALTABLE pSt_SerialTable = NULL);                     //按照卡号或者用户查询
    BOOL AuthService_SQLPacket_SerialQueryAll(AUTHREG_SERIALTABLE ***pppSt_SerialTable,int *pInt_ListCount);                           //查询所有序列卡
public:
    BOOL AuthService_SQLPacket_TryInsert(AUTHREG_NETVER* pSt_AuthVer);
    BOOL AuthService_SQLPacket_TryQuery(AUTHREG_NETVER* pSt_AuthVer);
    BOOL AuthService_SQLPacket_TryDelete(LPCTSTR lpszSerial);
    BOOL AuthService_SQLPacket_TryClear(int nThanValue, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode = ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW);
    BOOL AuthService_SQLPacket_TrySet(AUTHREG_NETVER* pSt_AuthVer);
protected:
    BOOL AuthService_SQLPacket_UserPayTime(LPCTSTR lpszUserName, LPCTSTR lpszUserTime, LPCTSTR lpszCardTime, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthSerialType, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthUserType);
private:
    BOOL m_bChange;
    XHDATA xhData;                 //数据库句柄
};
