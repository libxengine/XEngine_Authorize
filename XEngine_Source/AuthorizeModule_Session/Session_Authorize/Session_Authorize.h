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

class CSession_Authorize
{
public:
    CSession_Authorize();
    ~CSession_Authorize();
public:
    XBOOL Session_Authorize_Init(CALLBACK_XENGIEN_AUTHORIZE_SESSION_CLIENT_EVENTS fpCall_AuthEvent,XPVOID lParam = NULL);
    XBOOL Session_Authorize_GetClient(AUTHSESSION_NETCLIENT*** pppSt_ListClient, int* pInt_ListCount, LPCXSTR lpszClientAddr = NULL);
    XBOOL Session_Authorize_GetClientForUser(LPCXSTR lpszUserName, AUTHSESSION_NETCLIENT* pSt_Client);
    XBOOL Session_Authorize_GetAddrForUser(LPCXSTR lpszClientUser,XCHAR *ptszClientAddr);
    XBOOL Session_Authorize_GetUserForAddr(LPCXSTR lpszClientAddr, XCHAR *ptszClientUser);
    XBOOL Session_Authorize_CloseClient(LPCXSTR lpszClientAddr);
    XBOOL Session_Authorize_Destroy();
public:
    XBOOL Session_Authorize_Insert(LPCXSTR lpszClientAddr, AUTHREG_USERTABLE* pSt_UserTable, int nNetType = 0);
    XBOOL Session_Authorize_SetUser(AUTHREG_USERTABLE* pSt_UserTable);
protected:
    static XHTHREAD Session_Authorize_ActiveThread(XPVOID lParam);            //计时器线程
private:
    XBOOL bIsRun;                          //运行标志
    shared_ptr<std::thread> pSTDThread_hActive;                 //时间计算器线程句柄
private:
    XPVOID m_lParam;
    CALLBACK_XENGIEN_AUTHORIZE_SESSION_CLIENT_EVENTS lpCall_AuthregEvents;
private:
    shared_mutex st_Locker;
private:
    unordered_map<tstring, AUTHSESSION_NETCLIENT> stl_MapNetClient;
};
