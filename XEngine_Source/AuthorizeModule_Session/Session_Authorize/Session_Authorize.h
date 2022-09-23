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
    BOOL Session_Authorize_Init(CALLBACK_XENGIEN_AUTHORIZE_SESSION_CLIENT_EVENTS fpCall_AuthEvent,LPVOID lParam = NULL);
    BOOL Session_Authorize_GetClient(AUTHSESSION_NETCLIENT*** pppSt_ListClient, int* pInt_ListCount, LPCTSTR lpszClientAddr = NULL);
    BOOL Session_Authorize_GetClientForUser(LPCTSTR lpszUserName, AUTHSESSION_NETCLIENT* pSt_Client);
    BOOL Session_Authorize_GetAddrForUser(LPCTSTR lpszClientUser,TCHAR *ptszClientAddr);
    BOOL Session_Authorize_GetUserForAddr(LPCTSTR lpszClientAddr, TCHAR *ptszClientUser);
    BOOL Session_Authorize_CloseClient(LPCTSTR lpszClientAddr);
    BOOL Session_Authorize_Destroy();
public:
    BOOL Session_Authorize_Insert(LPCTSTR lpszClientAddr, AUTHREG_USERTABLE* pSt_UserTable, int nNetType = 0);
    BOOL Session_Authorize_SetUser(AUTHREG_USERTABLE* pSt_UserTable);
protected:
    static XHTHREAD Session_Authorize_ActiveThread(LPVOID lParam);            //计时器线程
private:
    BOOL bIsRun;                          //运行标志
    shared_ptr<std::thread> pSTDThread_hActive;                 //时间计算器线程句柄
private:
    LPVOID m_lParam;
    CALLBACK_XENGIEN_AUTHORIZE_SESSION_CLIENT_EVENTS lpCall_AuthregEvents;
private:
    shared_mutex st_Locker;
private:
    unordered_map<tstring, AUTHSESSION_NETCLIENT> stl_MapNetClient;
};
