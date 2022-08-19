#include "pch.h"
#include "Session_Token.h"
/********************************************************************
//    Created:     2022/08/18  14:43:33
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_Token\Session_Token.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_Token
//    File Base:   Session_Token
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     TOKEN会话管理
//    History:
*********************************************************************/
CSession_Token::CSession_Token()
{
    bIsRun = FALSE;
}
CSession_Token::~CSession_Token()
{
}
//////////////////////////////////////////////////////////////////////////
//                     公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Session_Token_Init
函数功能：初始化会话
 参数.一：nTimeout
  In/Out：In
  类型：整数型
  可空：N
  意思：超时时间,单位秒
 参数.二：fpCall_AuthEvent
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：TOKEN登录的客户端连接超时回调
 参数.三：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数自定义参数
返回值
  类型：逻辑型
  意思：是否初始化成功
备注：
*********************************************************************/
BOOL CSession_Token::Session_Token_Init(int nTimeout, CALLBACK_XENGIEN_AUTHORIZE_SESSION_TOKEN_EVENTS fpCall_AuthEvent, LPVOID lParam /* = NULL */)
{
    Session_IsErrorOccur = FALSE;

    if (NULL == fpCall_AuthEvent)
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_INIT_PARAMENT;
        return FALSE;
    }
    m_nTimeout = nTimeout;
    m_lParam = lParam;
    lpCall_AuthregEvents = fpCall_AuthEvent;

    bIsRun = TRUE;
    //创建线程
    pSTDThread_hActive = make_shared<std::thread>(Session_Token_Thread, this);
    if (!pSTDThread_hActive->joinable())
    {
        bIsRun = FALSE;
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_INIT_CREATETHREAD;
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Session_Token_Destroy
函数功能：销毁服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
BOOL CSession_Token::Session_Token_Destroy()
{
    Session_IsErrorOccur = FALSE;

    bIsRun = FALSE;        
    //结束线程
    if (NULL != pSTDThread_hActive)
    {
        pSTDThread_hActive->join();
    }
    //释放客户端资源
    st_Locker.lock();
    stl_MapToken.clear();
    st_Locker.unlock();
    return TRUE;
}
/********************************************************************
函数名称：Session_Authorize_Insert
函数功能：用户登陆协议分析
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
 参数.二：pSt_UserTable
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：用户信息表
返回值
  类型：逻辑型
  意思：是否允许登陆
备注：
*********************************************************************/
BOOL CSession_Token::Session_Token_Insert(XNETHANDLE xhToken, AUTHREG_USERTABLE* pSt_UserTable)
{
    Session_IsErrorOccur = FALSE;

    if (NULL == pSt_UserTable)
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_INSERT_PARAMENT;
        return FALSE;
    }
    AUTHSESSION_TOKENCLIENT st_TokenClient;
    memset(&st_TokenClient,'\0',sizeof(AUTHSESSION_TOKENCLIENT));

    BaseLib_OperatorTime_GetSysTime(&st_TokenClient.st_LibTimer);
    memcpy(&st_TokenClient.st_AuthUser, pSt_UserTable, sizeof(AUTHREG_USERTABLE));

    st_Locker.lock();
    stl_MapToken.insert(make_pair(xhToken, st_TokenClient));
    st_Locker.unlock();
    return TRUE;
}
/********************************************************************
函数名称：Session_Token_Delete
函数功能：移除一个客户端
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
返回值
  类型：逻辑型
  意思：是否移除成功
备注：
*********************************************************************/
BOOL CSession_Token::Session_Token_Delete(XNETHANDLE xhToken)
{
    Session_IsErrorOccur = FALSE;

    st_Locker.lock();
    unordered_map<XNETHANDLE, AUTHSESSION_TOKENCLIENT>::iterator stl_MapIterator = stl_MapToken.find(xhToken);
    if (stl_MapIterator != stl_MapToken.end())
    {
        //移除元素
        stl_MapToken.erase(stl_MapIterator);
    }
    st_Locker.unlock();
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                     线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CSession_Token::Session_Token_Thread(LPVOID lParam)
{
    CSession_Token *pClass_This = (CSession_Token *)lParam;
	XENGINE_LIBTIMER st_LibTimer;
	list<XNETHANDLE> stl_ListNotify;

    while (pClass_This->bIsRun)
    {
        //开始轮训用户
        pClass_This->st_Locker.lock_shared();
        unordered_map<XNETHANDLE, AUTHSESSION_TOKENCLIENT>::iterator stl_MapIterator = pClass_This->stl_MapToken.begin();
        for (; stl_MapIterator != pClass_This->stl_MapToken.end(); stl_MapIterator++)
        {
            BaseLib_OperatorTime_GetSysTime(&st_LibTimer);                  //获取现在的系统时间
            __int64x nOnlineSpan = 0;                                       //在线时间
            //用户登录了多少秒
            BaseLib_OperatorTimeSpan_GetForStu(&stl_MapIterator->second.st_LibTimer, &st_LibTimer, &nOnlineSpan, ENUM_XENGINE_BASELIB_TIME_SPAN_TYPE_SECOND);
            if (nOnlineSpan > pClass_This->m_nTimeout)
            {
                stl_ListNotify.push_back(stl_MapIterator->first);
            }
        }
        pClass_This->st_Locker.unlock_shared();
        //判断是否有需要关闭的客户端
        if (!stl_ListNotify.empty())
        {
            list<XNETHANDLE>::iterator stl_ListIterator = stl_ListNotify.begin();
            for (; stl_ListIterator != stl_ListNotify.end(); stl_ListIterator++)
            {
                pClass_This->lpCall_AuthregEvents(*stl_ListIterator, pClass_This->m_lParam);
            }
            stl_ListNotify.clear();        //清理元素
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
