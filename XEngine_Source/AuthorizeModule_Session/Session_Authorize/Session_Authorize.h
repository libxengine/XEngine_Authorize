#pragma once
/********************************************************************
//    Created:     2022/05/26  11:22:22
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_Authorize\Session_Authorize.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_Authorize
//    File Base:   Session_Authorize
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
class CSession_Authorize
{
public:
    CSession_Authorize();
    ~CSession_Authorize();
public:
    BOOL Session_Authorize_Init(CALLBACK_XENGIEN_AUTHREG_SERVICE_EVENTS fpCall_AuthEvent,LPVOID lParam = NULL);
    BOOL Session_Authorize_GetClient(AUTHREG_USERTABLE*** pppSt_ListClient, int* pInt_ListCount, LPCTSTR lpszClientAddr = NULL);
    BOOL Session_Authorize_GetTimer(LPCTSTR lpszUserName, AUTHREG_PROTOCOL_TIME* pSt_AuthTime);
    BOOL Session_Authorize_GetAddrForUser(LPCTSTR lpszClientUser,TCHAR *ptszClientAddr);
    BOOL Session_Authorize_GetUserForAddr(LPCTSTR lpszClientAddr, TCHAR *ptszClientUser);
    BOOL Session_Authorize_CloseClient(LPCTSTR lpszClientAddr);
    BOOL Session_Authorize_Destroy();
public:
    BOOL Session_Authorize_Insert(LPCTSTR lpszClientAddr, AUTHREG_USERTABLE* pSt_UserTable);
    BOOL Session_Authorize_SetUser(AUTHREG_USERTABLE* pSt_UserTable);
protected:
    static XHTHREAD Session_Authorize_ActiveThread(LPVOID lParam);            //计时器线程
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
