#pragma once
/********************************************************************
//    Created:     2021/07/14  13:20:24
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_Session\AuthService_Session\AuthService_Session.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_Session\AuthService_Session
//    File Base:   AuthService_Session
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     会话管理器
//    History:
*********************************************************************/
typedef struct
{
    AUTHREG_USERTABLE st_AuthUser;                                        //用户表
    XENGINE_LIBTIMER st_LibTimer;                                       //登录时间结构
    TCHAR tszClientAddr[64];
    TCHAR tszLeftTime[64];                                                //过期日期
    __int64x nOnlineTime;                                                  //在线时间
    __int64x nLeftTime;                                                    //剩余时间
}AUTHREGSERVICE_NETCLIENT,*LPAUTHREGSERVICE_NETCLIENT;
//////////////////////////////////////////////////////////////////////////
class CAuthService_Session
{
public:
    CAuthService_Session();
    ~CAuthService_Session();
public:
    BOOL AuthService_Session_Init(CALLBACK_XENGIEN_AUTHREG_SERVICE_EVENTS fpCall_AuthEvent,LPVOID lParam = NULL);
    BOOL AuthService_Session_GetClient(AUTHREG_USERTABLE*** pppSt_ListClient, int* pInt_ListCount, LPCTSTR lpszClientAddr = NULL);
    BOOL AuthService_Session_GetTimer(LPCTSTR lpszUserName, AUTHREG_PROTOCOL_TIME* pSt_AuthTime);
    BOOL AuthService_Session_GetAddrForUser(LPCTSTR lpszClientUser,TCHAR *ptszClientAddr);
    BOOL AuthService_Session_GetUserForAddr(LPCTSTR lpszClientAddr, TCHAR *ptszClientUser);
    BOOL AuthService_Session_CloseClient(LPCTSTR lpszClientAddr);
    BOOL AuthService_Session_Destroy();
public:
    BOOL AuthService_Session_Insert(LPCTSTR lpszClientAddr, AUTHREG_USERTABLE* pSt_UserTable);
    BOOL AuthService_Session_SetUser(AUTHREG_USERTABLE* pSt_UserTable);
protected:
    static XHTHREAD AuthService_Session_ActiveThread(LPVOID lParam);            //计时器线程
private:
    BOOL bIsRun;                          //运行标志
    shared_ptr<std::thread> pSTDThread_hActive;                 //时间计算器线程句柄
private:
    LPVOID m_lParam;
    CALLBACK_XENGIEN_AUTHREG_SERVICE_EVENTS lpCall_AuthregEvents;
private:
    shared_mutex st_Locker;
private:
    unordered_map<tstring, AUTHREGSERVICE_NETCLIENT> stl_MapNetClient;
};
