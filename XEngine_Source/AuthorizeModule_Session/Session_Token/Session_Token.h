#pragma once
/********************************************************************
//    Created:     2022/08/18  14:29:38
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_Token\Session_Token.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_Token
//    File Base:   Session_Token
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     TOKEN会话
//    History:
*********************************************************************/
typedef struct
{
	AUTHREG_USERTABLE st_UserTable;                                        //用户表
	XENGINE_LIBTIMER st_LibTimer;                                         //登录时间结构
	__int64x nOnlineTime;                                                 //在线时间
}AUTHSESSION_TOKENCLIENT, * LPAUTHSESSION_TOKENCLIENT;
//////////////////////////////////////////////////////////////////////////
class CSession_Token
{
public:
    CSession_Token();
    ~CSession_Token();
public:
    BOOL Session_Token_Init(int nTimeout, CALLBACK_XENGIEN_AUTHORIZE_SESSION_TOKEN_EVENTS fpCall_AuthEvent, LPVOID lParam = NULL);
    BOOL Session_Token_Destroy();
    BOOL Session_Token_Insert(XNETHANDLE xhToken, AUTHREG_USERTABLE* pSt_UserTable);
    BOOL Session_Token_Delete(XNETHANDLE xhToken);
    BOOL Session_Token_UPDate(XNETHANDLE xhToken);
    BOOL Session_Token_Get(XNETHANDLE xhToken, AUTHREG_USERTABLE* pSt_UserTable = NULL);
    BOOL Session_Token_GetUser(LPCTSTR lpszUser, LPCTSTR lpszPass);
protected:
    static XHTHREAD Session_Token_Thread(LPVOID lParam);            
private:
    int m_nTimeout;
    BOOL bIsRun;                      
    shared_ptr<std::thread> pSTDThread_hActive;            
private:
    LPVOID m_lParam;
    CALLBACK_XENGIEN_AUTHORIZE_SESSION_TOKEN_EVENTS lpCall_AuthregEvents;
private:
    shared_mutex st_Locker;
private:
    unordered_map<XNETHANDLE, AUTHSESSION_TOKENCLIENT> stl_MapToken;
};
