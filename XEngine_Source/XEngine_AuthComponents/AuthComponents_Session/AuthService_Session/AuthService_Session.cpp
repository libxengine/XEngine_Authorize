#include "pch.h"
#include "AuthService_Session.h"
/********************************************************************
//    Created:     2021/07/14  13:20:34
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_Session\AuthService_Session\AuthService_Session.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_Session\AuthService_Session
//    File Base:   AuthService_Session
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     会话管理器
//    History:
*********************************************************************/
CAuthService_Session::CAuthService_Session()
{
    bIsRun = FALSE;
}
CAuthService_Session::~CAuthService_Session()
{
}
//////////////////////////////////////////////////////////////////////////
//                     公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：AuthService_Session_Init
函数功能：初始化会话
 参数.一：fpCall_AuthEvent
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：验证服务器事件时间回调函数，通过此来通知上层用户时间更新
 参数.二：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数自定义参数
返回值
  类型：逻辑型
  意思：是否初始化成功
备注：
*********************************************************************/
BOOL CAuthService_Session::AuthService_Session_Init(CALLBACK_XENGIEN_AUTHREG_SERVICE_EVENTS fpCall_AuthEvent,LPVOID lParam /* = NULL */)
{
    Session_IsErrorOccur = FALSE;

    if (NULL == fpCall_AuthEvent)
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_INIT_PARAMENT;
        return FALSE;
    }
    m_lParam = lParam;
    lpCall_AuthregEvents = fpCall_AuthEvent;

    bIsRun = TRUE;
    //创建线程
    pSTDThread_hActive = make_shared<std::thread>(AuthService_Session_ActiveThread, this);
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
函数名称：AuthService_Session_GetClient
函数功能：获取客户端信息
 参数.一：pppSt_ListClient
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出获取到的客户端信息
 参数.二：pInt_ListCount
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输出列表个数
 参数.三：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要获取的客户端地址，为空表示获取所有
返回值
  类型：逻辑型
  意思：是否获取成功
备注：参数一必须通过基础库的内存释放函数BaseLib_OperatorMemory_Free进行释放内存
*********************************************************************/
BOOL CAuthService_Session::AuthService_Session_GetClient(AUTHREG_USERTABLE*** pppSt_ListClient, int* pInt_ListCount, LPCTSTR lpszClientAddr /* = NULL */)
{
    Session_IsErrorOccur = FALSE;

    if ((NULL == pppSt_ListClient) || (NULL == pInt_ListCount))
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_GETCLIENT_PARAMENT;
        return FALSE;
    }
    st_Locker.lock_shared();
    if (NULL == lpszClientAddr)
    {
        BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListClient, stl_MapNetClient.size(), sizeof(AUTHREG_USERTABLE));

        unordered_map<tstring, AUTHREGSERVICE_NETCLIENT>::const_iterator stl_MapIterator = stl_MapNetClient.begin();
        for (int i = 0; stl_MapIterator != stl_MapNetClient.cend(); stl_MapIterator++, i++)
        {
            *(*pppSt_ListClient)[i] = stl_MapIterator->second.st_AuthUser;
        }
        *pInt_ListCount = stl_MapNetClient.size();
    }
    else
    {
        unordered_map<tstring, AUTHREGSERVICE_NETCLIENT>::const_iterator stl_MapIterator = stl_MapNetClient.find(lpszClientAddr);
        if (stl_MapIterator == stl_MapNetClient.cend())
        {
            Session_IsErrorOccur = TRUE;
            Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_GETCLIENT_NOTFOUND;
            st_Locker.unlock_shared();
            return FALSE;
        }
        BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListClient, stl_MapNetClient.size(), sizeof(AUTHREG_USERTABLE));

        *(*pppSt_ListClient)[0] = stl_MapIterator->second.st_AuthUser;
        *pInt_ListCount = 1;
    }
    st_Locker.unlock_shared();

    return TRUE;
}
/********************************************************************
函数名称：AuthService_Session_GetTimer
函数功能：获取客户端时间信息
 参数.一：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要查找用户名
 参数.二：pSt_AuthTime
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：用户时间信息结构体
返回值
  类型：逻辑型
  意思：是否获取成功
备注：通过卡类型来判断导出的时间是分钟还是天
*********************************************************************/
BOOL CAuthService_Session::AuthService_Session_GetTimer(LPCTSTR lpszUserName,AUTHREG_PROTOCOL_TIME *pSt_AuthTime)
{
    Session_IsErrorOccur = FALSE;

    if ((NULL == lpszUserName) || (NULL == pSt_AuthTime))
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_GETTIME_PARAMENT;
        return FALSE;
    }
    st_Locker.lock_shared();
    unordered_map<tstring, AUTHREGSERVICE_NETCLIENT>::const_iterator stl_MapIterator = stl_MapNetClient.find(lpszUserName);
    if (stl_MapIterator == stl_MapNetClient.end())
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_GETTIME_NOTFOUND;
        st_Locker.unlock_shared();
        return FALSE;
    }
    pSt_AuthTime->nTimeLeft = stl_MapIterator->second.nLeftTime;
    pSt_AuthTime->nTimeONLine = stl_MapIterator->second.nOnlineTime;
    pSt_AuthTime->enSerialType = stl_MapIterator->second.st_AuthUser.en_AuthRegSerialType;

    _tcscpy(pSt_AuthTime->tszUserName, lpszUserName);
    _tcscpy(pSt_AuthTime->tszLeftTime, stl_MapIterator->second.tszLeftTime);
    _tcscpy(pSt_AuthTime->tszUserAddr, stl_MapIterator->second.tszClientAddr);
    st_Locker.unlock_shared();

    return TRUE;
}
/********************************************************************
函数名称：AuthService_Session_GetAddrForUser
函数功能：通过用户名获取对应地址
 参数.一：lpszClientUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.二：ptszClientAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出获取到的用户套接字地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAuthService_Session::AuthService_Session_GetAddrForUser(LPCTSTR lpszClientUser, TCHAR *ptszClientAddr)
{
    Session_IsErrorOccur = FALSE;

    if ((NULL == lpszClientUser) || (NULL == ptszClientAddr))
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_GETADDR_PARAMENT;
        return FALSE;
    }
    st_Locker.lock_shared();
    unordered_map<tstring, AUTHREGSERVICE_NETCLIENT>::const_iterator stl_MapIterator = stl_MapNetClient.find(lpszClientUser);
    if (stl_MapIterator == stl_MapNetClient.end())
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_GETADDR_NOTFOUND;
        st_Locker.unlock_shared();
        return FALSE;
    }
    _tcscpy(ptszClientAddr,stl_MapIterator->second.tszClientAddr);
    st_Locker.unlock_shared();

    return TRUE;
}
/********************************************************************
函数名称：AuthService_Session_GetUserForAddr
函数功能：通过IP地址获取对应用户名
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入IP地址端口
 参数.二：ptszClientUser
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出获取到的用户名
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAuthService_Session::AuthService_Session_GetUserForAddr(LPCTSTR lpszClientAddr, TCHAR *ptszClientUser)
{
    Session_IsErrorOccur = FALSE;

    if ((NULL == lpszClientAddr) || (NULL == ptszClientUser))
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_GETUSER_PARAMENT;
        return FALSE;
    }
    BOOL bIsFound = FALSE;
    st_Locker.lock_shared();
    unordered_map<tstring, AUTHREGSERVICE_NETCLIENT>::const_iterator stl_MapIterator = stl_MapNetClient.begin();
    for (;stl_MapIterator != stl_MapNetClient.end();stl_MapIterator++)
    {
        if (0 == _tcsncmp(lpszClientAddr, stl_MapIterator->second.tszClientAddr, _tcslen(lpszClientAddr)))
        {
            bIsFound = TRUE;
            break;
        }
    }
    if (!bIsFound)
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_GETUSER_NOTFOUND;
        st_Locker.unlock_shared();
        return FALSE;
    }
    _tcscpy(ptszClientUser, stl_MapIterator->first.c_str());
    st_Locker.unlock_shared();

    return TRUE;
}
/********************************************************************
函数名称：AuthService_Session_CloseClient
函数功能：移除一个客户端
 参数.一：lpszClientUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要移除的用户名
返回值
  类型：逻辑型
  意思：是否移除成功
备注：
*********************************************************************/
BOOL CAuthService_Session::AuthService_Session_CloseClient(LPCTSTR lpszClientAddr)
{
    Session_IsErrorOccur = FALSE;

    st_Locker.lock();
    unordered_map<tstring,AUTHREGSERVICE_NETCLIENT>::iterator stl_MapIterator = stl_MapNetClient.find(lpszClientAddr);
    if (stl_MapIterator != stl_MapNetClient.end())
    {
        //移除元素
        stl_MapNetClient.erase(stl_MapIterator);
    }
    st_Locker.unlock();
    return TRUE;
}
/********************************************************************
函数名称：AuthService_Session_Destroy
函数功能：销毁网络服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
BOOL CAuthService_Session::AuthService_Session_Destroy()
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
    stl_MapNetClient.clear();
    st_Locker.unlock();
    return TRUE;
}
/********************************************************************
函数名称：AuthService_Session_Insert
函数功能：用户登陆协议分析
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户地址
 参数.二：pSt_AuthProtocol
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：用户传递的协议数据
返回值
  类型：逻辑型
  意思：是否允许登陆
备注：如果成功，服务器会自动进行计时
*********************************************************************/
BOOL CAuthService_Session::AuthService_Session_Insert(LPCTSTR lpszClientAddr, AUTHREG_USERTABLE *pSt_UserTable)
{
    Session_IsErrorOccur = FALSE;

    if ((NULL == lpszClientAddr) || (NULL == pSt_UserTable))
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_INSERT_PARAMENT;
        return FALSE;
    }
    //验证是否登陆
    st_Locker.lock_shared();
    unordered_map<tstring,AUTHREGSERVICE_NETCLIENT>::iterator stl_MapIterator = stl_MapNetClient.find(pSt_UserTable->st_UserInfo.tszUserName);
    if (stl_MapIterator != stl_MapNetClient.end())
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_INSERT_ISLOGIN;
        st_Locker.unlock_shared();
        return FALSE;
    }
    st_Locker.unlock_shared();

    AUTHREGSERVICE_NETCLIENT st_Client;
    memset(&st_Client,'\0',sizeof(AUTHREGSERVICE_NETCLIENT));

    BaseLib_OperatorTime_GetSysTime(&st_Client.st_LibTimer);
    _tcscpy(st_Client.tszClientAddr,lpszClientAddr);
    memcpy(&st_Client.st_AuthUser, pSt_UserTable, sizeof(AUTHREG_USERTABLE));

    st_Locker.lock();
    stl_MapNetClient.insert(make_pair(pSt_UserTable->st_UserInfo.tszUserName, st_Client));
    st_Locker.unlock();
    return TRUE;
}
/********************************************************************
函数名称：AuthService_Session_SetUser
函数功能：设置用户信息
 参数.一：pSt_UserTable
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要设置的用户信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAuthService_Session::AuthService_Session_SetUser(AUTHREG_USERTABLE* pSt_UserTable)
{
    Session_IsErrorOccur = FALSE;

    if (NULL == pSt_UserTable)
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_SETUSER_PARAMENT;
        return FALSE;
    }
    //验证是否登陆
    st_Locker.lock_shared();
    unordered_map<tstring, AUTHREGSERVICE_NETCLIENT>::iterator stl_MapIterator = stl_MapNetClient.find(pSt_UserTable->st_UserInfo.tszUserName);
    if (stl_MapIterator == stl_MapNetClient.end())
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SESSION_SETUSER_NOTFOUND;
        st_Locker.unlock_shared();
        return FALSE;
    }
    memcpy(&stl_MapIterator->second.st_AuthUser, pSt_UserTable, sizeof(AUTHREG_USERTABLE));
    st_Locker.unlock_shared();
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                     线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CAuthService_Session::AuthService_Session_ActiveThread(LPVOID lParam)
{
    CAuthService_Session *pClass_This = (CAuthService_Session *)lParam;
    XENGINE_LIBTIMER st_LibTimer;
    AUTHREG_PROTOCOL_TIME st_ProtocolTimer;
    list<AUTHREG_PROTOCOL_TIME> stl_ListNotify;

    while (pClass_This->bIsRun)
    {
        //开始轮训用户
        pClass_This->st_Locker.lock_shared();
        unordered_map<tstring, AUTHREGSERVICE_NETCLIENT>::iterator stl_MapIterator = pClass_This->stl_MapNetClient.begin();
        for (; stl_MapIterator != pClass_This->stl_MapNetClient.end(); stl_MapIterator++)
        {
            memset(&st_ProtocolTimer, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
            BaseLib_OperatorTime_GetSysTime(&st_LibTimer);                  //获取现在的系统时间
            __int64x nOnlineSpan = 0;                                        //在线时间
            //用户登录了多少分钟
            BaseLib_OperatorTimeSpan_GetForStu(&stl_MapIterator->second.st_LibTimer, &st_LibTimer, &nOnlineSpan, ENUM_XENGINE_BASELIB_TIME_SPAN_TYPE_MINUTE);
            //登陆成功的。我们要处理他过期
            switch (stl_MapIterator->second.st_AuthUser.en_AuthRegSerialType)
            {
            case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_MINUTE:
            {
                //分钟处理
                __int64x nLeftTimer = _tcstoi64(stl_MapIterator->second.st_AuthUser.tszLeftTime, NULL, 10);
                //获得过期日期
                XENGINE_LIBTIMER st_TimeCal;
                memset(&st_TimeCal, '\0', sizeof(XENGINE_LIBTIMER));

                st_TimeCal.wMinute = (int)nLeftTimer;
                BaseLib_OperatorTimeSpan_CalForStu(&stl_MapIterator->second.st_LibTimer, &st_TimeCal);
                BaseLib_OperatorTime_ToStringTimer(stl_MapIterator->second.tszLeftTime, &st_TimeCal);
                //赋值给管理器
                if (nLeftTimer > nOnlineSpan)
                {
                    stl_MapIterator->second.nLeftTime = nLeftTimer - nOnlineSpan;
                }
                else
                {
                    stl_MapIterator->second.nLeftTime = 0;
                }
                stl_MapIterator->second.nOnlineTime = nOnlineSpan;
                //赋值给回调函数
                st_ProtocolTimer.nTimeONLine = nOnlineSpan;
                st_ProtocolTimer.nTimeLeft = stl_MapIterator->second.nLeftTime;
                st_ProtocolTimer.enSerialType = stl_MapIterator->second.st_AuthUser.en_AuthRegSerialType;
                st_ProtocolTimer.enDeviceType = stl_MapIterator->second.st_AuthUser.enDeviceType;
                _tcscpy(st_ProtocolTimer.tszLeftTime, stl_MapIterator->second.tszLeftTime);
                _tcscpy(st_ProtocolTimer.tszUserAddr, stl_MapIterator->second.tszClientAddr);
                _tcscpy(st_ProtocolTimer.tszUserName, stl_MapIterator->second.st_AuthUser.st_UserInfo.tszUserName);
                stl_ListNotify.push_back(st_ProtocolTimer);
            }
            break;
            case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME:
            {
                //次数处理
                stl_MapIterator->second.nOnlineTime = nOnlineSpan;
                stl_MapIterator->second.nLeftTime = _tcstoi64(stl_MapIterator->second.st_AuthUser.tszLeftTime, NULL, 10);
                _stprintf(stl_MapIterator->second.tszLeftTime, _T("%lld"), stl_MapIterator->second.nLeftTime);
                //次数处理不做任何时间操作
                st_ProtocolTimer.nTimeONLine = nOnlineSpan;
                st_ProtocolTimer.nTimeLeft = stl_MapIterator->second.nLeftTime;
                st_ProtocolTimer.enSerialType = stl_MapIterator->second.st_AuthUser.en_AuthRegSerialType;
                st_ProtocolTimer.enDeviceType = stl_MapIterator->second.st_AuthUser.enDeviceType;
                _tcscpy(st_ProtocolTimer.tszLeftTime, stl_MapIterator->second.st_AuthUser.tszLeftTime);
                _tcscpy(st_ProtocolTimer.tszUserAddr, stl_MapIterator->second.tszClientAddr);
                _tcscpy(st_ProtocolTimer.tszUserName, stl_MapIterator->second.st_AuthUser.st_UserInfo.tszUserName);
                stl_ListNotify.push_back(st_ProtocolTimer);
            }
            break;
            default:
                //天卡，自定义 都是用相同的函数处理
                __int64x nLeftTime = 0;                                          //剩余时间
                XENGINE_LIBTIMER st_TimeLeft;
                memset(&st_TimeLeft, '\0', sizeof(XENGINE_LIBTIMER));
                //获取指定过期日期
                if (6 != _stscanf_s(stl_MapIterator->second.st_AuthUser.tszLeftTime, _T("%04d-%02d-%02d %02d:%02d:%02d"), &st_TimeLeft.wYear, &st_TimeLeft.wMonth, &st_TimeLeft.wDay, &st_TimeLeft.wHour, &st_TimeLeft.wMinute, &st_TimeLeft.wSecond))
                {
                    break;
                }
                //剩余天数,通过分钟来处理
                BaseLib_OperatorTimeSpan_GetForStu(&st_LibTimer, &st_TimeLeft, &nLeftTime, ENUM_XENGINE_BASELIB_TIME_SPAN_TYPE_MINUTE);
                //获取过期时间
                stl_MapIterator->second.nLeftTime = nLeftTime;
                stl_MapIterator->second.nOnlineTime = nOnlineSpan;
                _tcscpy(stl_MapIterator->second.tszLeftTime, stl_MapIterator->second.st_AuthUser.tszLeftTime);
                //计算时间是否超过！
                AUTHREG_PROTOCOL_TIME st_ProtocolTimer;
                memset(&st_ProtocolTimer, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

                st_ProtocolTimer.nTimeONLine = nOnlineSpan;
                st_ProtocolTimer.nTimeLeft = nLeftTime;
                st_ProtocolTimer.enSerialType = stl_MapIterator->second.st_AuthUser.en_AuthRegSerialType;
                st_ProtocolTimer.enDeviceType = stl_MapIterator->second.st_AuthUser.enDeviceType;
                _tcscpy(st_ProtocolTimer.tszLeftTime, stl_MapIterator->second.st_AuthUser.tszLeftTime);
                _tcscpy(st_ProtocolTimer.tszUserAddr, stl_MapIterator->second.tszClientAddr);
                _tcscpy(st_ProtocolTimer.tszUserName, stl_MapIterator->second.st_AuthUser.st_UserInfo.tszUserName);
                stl_ListNotify.push_back(st_ProtocolTimer);
                break;
            }
        }
        pClass_This->st_Locker.unlock_shared();
        //判断是否有需要关闭的客户端
        if (!stl_ListNotify.empty())
        {
            list<AUTHREG_PROTOCOL_TIME>::iterator stl_ListIterator = stl_ListNotify.begin();
            for (; stl_ListIterator != stl_ListNotify.end(); stl_ListIterator++)
            {
                pClass_This->lpCall_AuthregEvents(stl_ListIterator->tszUserAddr, stl_ListIterator->tszUserName, stl_ListIterator->nTimeONLine, stl_ListIterator->nTimeLeft, stl_ListIterator->tszLeftTime, stl_ListIterator->enSerialType, stl_ListIterator->enDeviceType, pClass_This->m_lParam);
            }
            stl_ListNotify.clear();        //清理元素
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
