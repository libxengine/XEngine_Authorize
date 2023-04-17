#include "pch.h"
#include "Session_Authorize.h"
/********************************************************************
//    Created:     2022/05/26  11:22:36
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_Authorize\Session_Authorize.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_Authorize
//    File Base:   Session_Authorize
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     会话管理器
//    History:
*********************************************************************/
CSession_Authorize::CSession_Authorize()
{
    bIsRun = false;
}
CSession_Authorize::~CSession_Authorize()
{
}
//////////////////////////////////////////////////////////////////////////
//                     公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Session_Authorize_Init
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
bool CSession_Authorize::Session_Authorize_Init(CALLBACK_XENGIEN_AUTHORIZE_SESSION_CLIENT_EVENTS fpCall_AuthEvent,XPVOID lParam /* = NULL */)
{
    Session_IsErrorOccur = false;

    if (NULL == fpCall_AuthEvent)
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return false;
    }
    m_lParam = lParam;
    lpCall_AuthregEvents = fpCall_AuthEvent;

    bIsRun = true;
    //创建线程
    pSTDThread_hActive = make_shared<std::thread>(Session_Authorize_ActiveThread, this);
    if (!pSTDThread_hActive->joinable())
    {
        bIsRun = false;
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_CREATETHREAD;
        return false;
    }
    return true;
}
/********************************************************************
函数名称：Session_Authorize_GetClient
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
bool CSession_Authorize::Session_Authorize_GetClient(AUTHSESSION_NETCLIENT*** pppSt_ListClient, int* pInt_ListCount, LPCXSTR lpszClientAddr /* = NULL */)
{
    Session_IsErrorOccur = false;

    if ((NULL == pppSt_ListClient) || (NULL == pInt_ListCount))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return false;
    }
    st_Locker.lock_shared();
    if (NULL == lpszClientAddr)
    {
        BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListClient, stl_MapNetClient.size(), sizeof(AUTHSESSION_NETCLIENT));

        unordered_map<xstring, AUTHSESSION_NETCLIENT>::const_iterator stl_MapIterator = stl_MapNetClient.begin();
        for (int i = 0; stl_MapIterator != stl_MapNetClient.cend(); stl_MapIterator++, i++)
        {
            *(*pppSt_ListClient)[i] = stl_MapIterator->second;
        }
        *pInt_ListCount = stl_MapNetClient.size();
    }
    else
    {
        unordered_map<xstring, AUTHSESSION_NETCLIENT>::const_iterator stl_MapIterator = stl_MapNetClient.find(lpszClientAddr);
        if (stl_MapIterator == stl_MapNetClient.cend())
        {
            Session_IsErrorOccur = true;
            Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_NOTFOUND;
            st_Locker.unlock_shared();
            return false;
        }
        BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListClient, stl_MapNetClient.size(), sizeof(AUTHSESSION_NETCLIENT));

        *(*pppSt_ListClient)[0] = stl_MapIterator->second;
        *pInt_ListCount = 1;
    }
    st_Locker.unlock_shared();

    if (0 == *pInt_ListCount)
    {
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_NOTFOUND;
		return false;
    }
    return true;
}
/********************************************************************
函数名称：Session_Authorize_GetClientForUser
函数功能：获取客户端信息
 参数.一：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要查找用户名
 参数.二：pSt_Client
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：用户信息结构体
返回值
  类型：逻辑型
  意思：是否获取成功
备注：通过卡类型来判断导出的时间是分钟还是天
*********************************************************************/
bool CSession_Authorize::Session_Authorize_GetClientForUser(LPCXSTR lpszUserName, AUTHSESSION_NETCLIENT* pSt_Client)
{
    Session_IsErrorOccur = false;

    if ((NULL == lpszUserName) || (NULL == pSt_Client))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return false;
    }
    st_Locker.lock_shared();
    unordered_map<xstring, AUTHSESSION_NETCLIENT>::const_iterator stl_MapIterator = stl_MapNetClient.find(lpszUserName);
    if (stl_MapIterator == stl_MapNetClient.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    *pSt_Client = stl_MapIterator->second;
    st_Locker.unlock_shared();

    return true;
}
/********************************************************************
函数名称：Session_Authorize_GetAddrForUser
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
bool CSession_Authorize::Session_Authorize_GetAddrForUser(LPCXSTR lpszClientUser, XCHAR *ptszClientAddr)
{
    Session_IsErrorOccur = false;

    if ((NULL == lpszClientUser) || (NULL == ptszClientAddr))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return false;
    }
    st_Locker.lock_shared();
    unordered_map<xstring, AUTHSESSION_NETCLIENT>::const_iterator stl_MapIterator = stl_MapNetClient.find(lpszClientUser);
    if (stl_MapIterator == stl_MapNetClient.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    _tcsxcpy(ptszClientAddr,stl_MapIterator->second.tszClientAddr);
    st_Locker.unlock_shared();

    return true;
}
/********************************************************************
函数名称：Session_Authorize_GetUserForAddr
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
bool CSession_Authorize::Session_Authorize_GetUserForAddr(LPCXSTR lpszClientAddr, XCHAR *ptszClientUser)
{
    Session_IsErrorOccur = false;

    if ((NULL == lpszClientAddr) || (NULL == ptszClientUser))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return false;
    }
    bool bIsFound = false;
    st_Locker.lock_shared();
    unordered_map<xstring, AUTHSESSION_NETCLIENT>::const_iterator stl_MapIterator = stl_MapNetClient.begin();
    for (;stl_MapIterator != stl_MapNetClient.end();stl_MapIterator++)
    {
        if (0 == _tcsxncmp(lpszClientAddr, stl_MapIterator->second.tszClientAddr, _tcsxlen(lpszClientAddr)))
        {
            bIsFound = true;
            break;
        }
    }
    if (!bIsFound)
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    _tcsxcpy(ptszClientUser, stl_MapIterator->first.c_str());
    st_Locker.unlock_shared();

    return true;
}
/********************************************************************
函数名称：Session_Authorize_CloseClient
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
bool CSession_Authorize::Session_Authorize_CloseClient(LPCXSTR lpszClientAddr)
{
    Session_IsErrorOccur = false;

    st_Locker.lock();
    unordered_map<xstring,AUTHSESSION_NETCLIENT>::iterator stl_MapIterator = stl_MapNetClient.find(lpszClientAddr);
    if (stl_MapIterator != stl_MapNetClient.end())
    {
        //移除元素
        stl_MapNetClient.erase(stl_MapIterator);
    }
    st_Locker.unlock();
    return true;
}
/********************************************************************
函数名称：Session_Authorize_Destroy
函数功能：销毁网络服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
bool CSession_Authorize::Session_Authorize_Destroy()
{
    Session_IsErrorOccur = false;

    bIsRun = false;        
    //结束线程
    if (NULL != pSTDThread_hActive)
    {
        pSTDThread_hActive->join();
    }
    //释放客户端资源
    st_Locker.lock();
    stl_MapNetClient.clear();
    st_Locker.unlock();
    return true;
}
/********************************************************************
函数名称：Session_Authorize_Insert
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
 参数.三：nNetType
  In/Out：In
  类型：整数型
  可空：Y
  意思：用户连接的类型
返回值
  类型：逻辑型
  意思：是否允许登陆
备注：如果成功，服务器会自动进行计时
*********************************************************************/
bool CSession_Authorize::Session_Authorize_Insert(LPCXSTR lpszClientAddr, AUTHREG_USERTABLE *pSt_UserTable, int nNetType)
{
    Session_IsErrorOccur = false;

    if ((NULL == lpszClientAddr) || (NULL == pSt_UserTable))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return false;
    }
    //验证是否登陆
    st_Locker.lock_shared();
    unordered_map<xstring,AUTHSESSION_NETCLIENT>::iterator stl_MapIterator = stl_MapNetClient.find(pSt_UserTable->st_UserInfo.tszUserName);
    if (stl_MapIterator != stl_MapNetClient.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_ISLOGIN;
        st_Locker.unlock_shared();
        return false;
    }
    st_Locker.unlock_shared();

    AUTHSESSION_NETCLIENT st_Client;
    memset(&st_Client,'\0',sizeof(AUTHSESSION_NETCLIENT));

    st_Client.nNetType = nNetType;
    BaseLib_OperatorTime_GetSysTime(&st_Client.st_LibTimer);
    _tcsxcpy(st_Client.tszClientAddr,lpszClientAddr);
    memcpy(&st_Client.st_UserTable, pSt_UserTable, sizeof(AUTHREG_USERTABLE));

    st_Locker.lock();
    stl_MapNetClient.insert(make_pair(pSt_UserTable->st_UserInfo.tszUserName, st_Client));
    st_Locker.unlock();
    return true;
}
/********************************************************************
函数名称：Session_Authorize_SetUser
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
bool CSession_Authorize::Session_Authorize_SetUser(AUTHREG_USERTABLE* pSt_UserTable)
{
    Session_IsErrorOccur = false;

    if (NULL == pSt_UserTable)
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return false;
    }
    //验证是否登陆
    st_Locker.lock_shared();
    unordered_map<xstring, AUTHSESSION_NETCLIENT>::iterator stl_MapIterator = stl_MapNetClient.find(pSt_UserTable->st_UserInfo.tszUserName);
    if (stl_MapIterator == stl_MapNetClient.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    memcpy(&stl_MapIterator->second.st_UserTable, pSt_UserTable, sizeof(AUTHREG_USERTABLE));
    st_Locker.unlock_shared();
    return true;
}
//////////////////////////////////////////////////////////////////////////
//                     线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CSession_Authorize::Session_Authorize_ActiveThread(XPVOID lParam)
{
    CSession_Authorize *pClass_This = (CSession_Authorize *)lParam;
    XENGINE_LIBTIMER st_LibTimer;
    AUTHREG_PROTOCOL_TIME st_ProtocolTimer;
    list<AUTHREG_PROTOCOL_TIME> stl_ListNotify;

    while (pClass_This->bIsRun)
    {
        //开始轮训用户
        pClass_This->st_Locker.lock_shared();
        unordered_map<xstring, AUTHSESSION_NETCLIENT>::iterator stl_MapIterator = pClass_This->stl_MapNetClient.begin();
        for (; stl_MapIterator != pClass_This->stl_MapNetClient.end(); stl_MapIterator++)
        {
            memset(&st_ProtocolTimer, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
            BaseLib_OperatorTime_GetSysTime(&st_LibTimer);                  //获取现在的系统时间
            __int64x nOnlineSpan = 0;                                        //在线时间
            //用户登录了多少分钟
            BaseLib_OperatorTimeSpan_GetForStu(&stl_MapIterator->second.st_LibTimer, &st_LibTimer, &nOnlineSpan, ENUM_XENGINE_BASELIB_TIME_SPAN_TYPE_MINUTE);
            //登陆成功的。我们要处理他过期
            switch (stl_MapIterator->second.st_UserTable.enSerialType)
            {
            case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_MINUTE:
            {
                //分钟处理
                __int64x nLeftTimer = _ttxoll(stl_MapIterator->second.st_UserTable.tszLeftTime);
                //获得过期日期
                XENGINE_LIBTIMER st_TimeCal;
                memset(&st_TimeCal, '\0', sizeof(XENGINE_LIBTIMER));

                st_TimeCal.wMinute = (int)nLeftTimer;
                BaseLib_OperatorTimeSpan_CalForStu(&stl_MapIterator->second.st_LibTimer, &st_TimeCal);
                BaseLib_OperatorTime_TimeToStr(stl_MapIterator->second.tszLeftTime, NULL, true, &st_TimeCal);
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
                st_ProtocolTimer.nNetType = stl_MapIterator->second.nNetType;
                st_ProtocolTimer.nTimeLeft = stl_MapIterator->second.nLeftTime;
                st_ProtocolTimer.enSerialType = stl_MapIterator->second.st_UserTable.enSerialType;
                st_ProtocolTimer.enDeviceType = stl_MapIterator->second.st_UserTable.enDeviceType;
                _tcsxcpy(st_ProtocolTimer.tszLeftTime, stl_MapIterator->second.tszLeftTime);
                _tcsxcpy(st_ProtocolTimer.tszUserAddr, stl_MapIterator->second.tszClientAddr);
                _tcsxcpy(st_ProtocolTimer.tszUserName, stl_MapIterator->second.st_UserTable.st_UserInfo.tszUserName);
                stl_ListNotify.push_back(st_ProtocolTimer);
            }
            break;
            case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME:
            {
                //次数处理
                stl_MapIterator->second.nOnlineTime = nOnlineSpan;
                stl_MapIterator->second.nLeftTime = _ttxoll(stl_MapIterator->second.st_UserTable.tszLeftTime);
                _xstprintf(stl_MapIterator->second.tszLeftTime, _X("%lld"), stl_MapIterator->second.nLeftTime);
                //次数处理不做任何时间操作
                st_ProtocolTimer.nTimeONLine = nOnlineSpan;
                st_ProtocolTimer.nNetType = stl_MapIterator->second.nNetType;
                st_ProtocolTimer.nTimeLeft = stl_MapIterator->second.nLeftTime;
                st_ProtocolTimer.enSerialType = stl_MapIterator->second.st_UserTable.enSerialType;
                st_ProtocolTimer.enDeviceType = stl_MapIterator->second.st_UserTable.enDeviceType;
                _tcsxcpy(st_ProtocolTimer.tszLeftTime, stl_MapIterator->second.st_UserTable.tszLeftTime);
                _tcsxcpy(st_ProtocolTimer.tszUserAddr, stl_MapIterator->second.tszClientAddr);
                _tcsxcpy(st_ProtocolTimer.tszUserName, stl_MapIterator->second.st_UserTable.st_UserInfo.tszUserName);
                stl_ListNotify.push_back(st_ProtocolTimer);
            }
            break;
            default:
                //天卡，自定义 都是用相同的函数处理
                __int64x nLeftTime = 0;                                          //剩余时间
                XENGINE_LIBTIMER st_TimeLeft;
                memset(&st_TimeLeft, '\0', sizeof(XENGINE_LIBTIMER));
                //获取指定过期日期
                if (6 != _stxscanf(stl_MapIterator->second.st_UserTable.tszLeftTime, _X("%04d-%02d-%02d %02d:%02d:%02d"), &st_TimeLeft.wYear, &st_TimeLeft.wMonth, &st_TimeLeft.wDay, &st_TimeLeft.wHour, &st_TimeLeft.wMinute, &st_TimeLeft.wSecond))
                {
                    break;
                }
                //剩余天数,通过分钟来处理
                BaseLib_OperatorTimeSpan_GetForStu(&st_LibTimer, &st_TimeLeft, &nLeftTime, ENUM_XENGINE_BASELIB_TIME_SPAN_TYPE_MINUTE);
                //获取过期时间
                stl_MapIterator->second.nLeftTime = nLeftTime;
                stl_MapIterator->second.nOnlineTime = nOnlineSpan;
                _tcsxcpy(stl_MapIterator->second.tszLeftTime, stl_MapIterator->second.st_UserTable.tszLeftTime);
                //计算时间是否超过！
                AUTHREG_PROTOCOL_TIME st_ProtocolTimer;
                memset(&st_ProtocolTimer, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

                st_ProtocolTimer.nTimeONLine = nOnlineSpan;
                st_ProtocolTimer.nTimeLeft = nLeftTime;
                st_ProtocolTimer.nNetType = stl_MapIterator->second.nNetType;
                st_ProtocolTimer.enSerialType = stl_MapIterator->second.st_UserTable.enSerialType;
                st_ProtocolTimer.enDeviceType = stl_MapIterator->second.st_UserTable.enDeviceType;
                _tcsxcpy(st_ProtocolTimer.tszLeftTime, stl_MapIterator->second.st_UserTable.tszLeftTime);
                _tcsxcpy(st_ProtocolTimer.tszUserAddr, stl_MapIterator->second.tszClientAddr);
                _tcsxcpy(st_ProtocolTimer.tszUserName, stl_MapIterator->second.st_UserTable.st_UserInfo.tszUserName);
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
                pClass_This->lpCall_AuthregEvents(stl_ListIterator->tszUserAddr, stl_ListIterator->tszUserName, stl_ListIterator->nTimeONLine, stl_ListIterator->nTimeLeft, stl_ListIterator->tszLeftTime, stl_ListIterator->enSerialType, stl_ListIterator->enDeviceType, stl_ListIterator->nNetType, pClass_This->m_lParam);
            }
            stl_ListNotify.clear();        //清理元素
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
