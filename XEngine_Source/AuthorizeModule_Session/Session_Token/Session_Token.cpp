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
    bIsRun = XFALSE;
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
XBOOL CSession_Token::Session_Token_Init(int nTimeout, CALLBACK_XENGIEN_AUTHORIZE_SESSION_TOKEN_EVENTS fpCall_AuthEvent, XPVOID lParam /* = NULL */)
{
    Session_IsErrorOccur = XFALSE;

    if (NULL == fpCall_AuthEvent)
    {
        Session_IsErrorOccur = XTRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return XFALSE;
    }
    m_nTimeout = nTimeout;
    m_lParam = lParam;
    lpCall_AuthregEvents = fpCall_AuthEvent;

    bIsRun = XTRUE;
    //创建线程
    pSTDThread_hActive = make_shared<std::thread>(Session_Token_Thread, this);
    if (!pSTDThread_hActive->joinable())
    {
        bIsRun = XFALSE;
        Session_IsErrorOccur = XTRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_CREATETHREAD;
        return XFALSE;
    }
    return XTRUE;
}
/********************************************************************
函数名称：Session_Token_Destroy
函数功能：销毁服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
XBOOL CSession_Token::Session_Token_Destroy()
{
    Session_IsErrorOccur = XFALSE;

    bIsRun = XFALSE;        
    //结束线程
    if (NULL != pSTDThread_hActive)
    {
        pSTDThread_hActive->join();
    }
    //释放客户端资源
    st_Locker.lock();
    stl_MapToken.clear();
    st_Locker.unlock();
    return XTRUE;
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
 参数.三：nTimeout
  In/Out：In
  类型：整数型
  可空：Y
  意思：大于0单独指定TOKEN超时时间
返回值
  类型：逻辑型
  意思：是否允许登陆
备注：
*********************************************************************/
XBOOL CSession_Token::Session_Token_Insert(XNETHANDLE xhToken, AUTHREG_USERTABLE* pSt_UserTable, int nTimeout /* = 0 */)
{
    Session_IsErrorOccur = XFALSE;

    if (NULL == pSt_UserTable)
    {
        Session_IsErrorOccur = XTRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return XFALSE;
    }
    AUTHSESSION_TOKENCLIENT st_TokenClient;
    memset(&st_TokenClient,'\0',sizeof(AUTHSESSION_TOKENCLIENT));

    st_TokenClient.nOnlineTime = nTimeout;
    BaseLib_OperatorTime_GetSysTime(&st_TokenClient.st_LibTimer);
    memcpy(&st_TokenClient.st_UserTable, pSt_UserTable, sizeof(AUTHREG_USERTABLE));

    st_Locker.lock();
    stl_MapToken.insert(make_pair(xhToken, st_TokenClient));
    st_Locker.unlock();
    return XTRUE;
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
XBOOL CSession_Token::Session_Token_Delete(XNETHANDLE xhToken)
{
    Session_IsErrorOccur = XFALSE;

    st_Locker.lock();
    unordered_map<XNETHANDLE, AUTHSESSION_TOKENCLIENT>::iterator stl_MapIterator = stl_MapToken.find(xhToken);
    if (stl_MapIterator != stl_MapToken.end())
    {
        //移除元素
        stl_MapToken.erase(stl_MapIterator);
    }
    st_Locker.unlock();
    return XTRUE;
}
/********************************************************************
函数名称：Session_Token_UPDate
函数功能：更新一个客户端的TOKEN时间
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CSession_Token::Session_Token_UPDate(XNETHANDLE xhToken)
{
	Session_IsErrorOccur = XFALSE;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, AUTHSESSION_TOKENCLIENT>::iterator stl_MapIterator = stl_MapToken.find(xhToken);
	if (stl_MapIterator == stl_MapToken.end())
	{
        Session_IsErrorOccur = XTRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        st_Locker.unlock_shared();
        return XFALSE;
	}
    BaseLib_OperatorTime_GetSysTime(&stl_MapIterator->second.st_LibTimer);
	st_Locker.unlock_shared();
	return XTRUE;
}
/********************************************************************
函数名称：Session_Token_Get
函数功能：获取客户端信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
 参数.二：pSt_UserTable
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：输出TOKEN对应的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CSession_Token::Session_Token_Get(XNETHANDLE xhToken, AUTHREG_USERTABLE* pSt_UserTable /* = NULL */)
{
	Session_IsErrorOccur = XFALSE;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, AUTHSESSION_TOKENCLIENT>::iterator stl_MapIterator = stl_MapToken.find(xhToken);
	if (stl_MapIterator == stl_MapToken.end())
	{
		Session_IsErrorOccur = XTRUE;
		Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return XFALSE;
	}
    if (NULL != pSt_UserTable)
    {
        *pSt_UserTable = stl_MapIterator->second.st_UserTable;
    }
	st_Locker.unlock_shared();
	return XTRUE;
}
/********************************************************************
函数名称：Session_Token_GetUser
函数功能：获取用户的TOKEN
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：密码
 参数.三：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：导出获取到的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CSession_Token::Session_Token_GetUser(LPCXSTR lpszUser, LPCXSTR lpszPass, XNETHANDLE* pxhToken)
{
	Session_IsErrorOccur = XFALSE;

    if ((NULL == lpszUser) || (NULL == lpszPass))
    {
		Session_IsErrorOccur = XTRUE;
		Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
		return XFALSE;
    }
    XBOOL bFound = XFALSE;
	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, AUTHSESSION_TOKENCLIENT>::iterator stl_MapIterator = stl_MapToken.begin();
    for (; stl_MapIterator != stl_MapToken.end(); stl_MapIterator++)
    {
        //用户名
        if (0 == _tcsncmp(lpszUser, stl_MapIterator->second.st_UserTable.st_UserInfo.tszUserName, _tcslen(lpszUser)))
        {
            //密码,验证密码防治冲突
			if (0 == _tcsncmp(lpszPass, stl_MapIterator->second.st_UserTable.st_UserInfo.tszUserPass, _tcslen(lpszPass)))
			{
                *pxhToken = stl_MapIterator->first;
                bFound = XTRUE;
                break;
			}
        }
    }
    st_Locker.unlock_shared();
	if (!bFound)
	{
		Session_IsErrorOccur = XTRUE;
		Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_NOTFOUND;
		return XFALSE;
	}
	return XTRUE;
}
//////////////////////////////////////////////////////////////////////////
//                     线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CSession_Token::Session_Token_Thread(XPVOID lParam)
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
            if (stl_MapIterator->second.nTimeout > 0)
            {
				if (nOnlineSpan > stl_MapIterator->second.nTimeout)
				{
					stl_ListNotify.push_back(stl_MapIterator->first);
				}
            }
            else
            {
				if (nOnlineSpan > pClass_This->m_nTimeout)
				{
					stl_ListNotify.push_back(stl_MapIterator->first);
				}
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
