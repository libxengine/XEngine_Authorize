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
bool CSession_Authorize::Session_Authorize_Init(CALLBACK_XENGIEN_AUTHORIZE_SESSION_CLIENT_EVENTS fpCall_AuthEvent, XPVOID lParam /* = NULL */)
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
bool CSession_Authorize::Session_Authorize_GetClient(AUTHSESSION_NETCLIENT*** pppSt_ListClient, int* pInt_ListCount, LPCXSTR lpszClientUser /* = NULL */)
{
    Session_IsErrorOccur = false;

    if ((NULL == pppSt_ListClient) || (NULL == pInt_ListCount))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return false;
    }
    st_Locker.lock_shared();
    if (NULL == lpszClientUser)
    {
        list<AUTHSESSION_NETCLIENT> stl_ListClient;
        for (auto stl_MapIterator = stl_MapNetClient.begin(); stl_MapIterator != stl_MapNetClient.cend(); stl_MapIterator++)
        {
            list<AUTHSESSION_NETCLIENT>::iterator stl_ListIterator = stl_MapIterator->second.begin();
            for (; stl_ListIterator != stl_MapIterator->second.end(); stl_ListIterator++)
            {
                stl_ListClient.push_back(*stl_ListIterator);
            }
        }
        *pInt_ListCount = stl_ListClient.size();
        BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListClient, stl_ListClient.size(), sizeof(AUTHSESSION_NETCLIENT));

        auto stl_ListIterator = stl_ListClient.begin();
        for (int i = 0; stl_ListIterator != stl_ListClient.end(); stl_ListIterator++, i++)
        {
            *(*pppSt_ListClient)[i] = *stl_ListIterator;
        }
    }
    else
    {
        auto stl_MapIterator = stl_MapNetClient.find(lpszClientUser);
        if (stl_MapIterator != stl_MapNetClient.end())
        {
            *pInt_ListCount = stl_MapIterator->second.size();
            BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListClient, stl_MapIterator->second.size(), sizeof(AUTHSESSION_NETCLIENT));

            auto stl_ListIterator = stl_MapIterator->second.begin();
            for (int i = 0; stl_ListIterator != stl_MapIterator->second.end(); stl_ListIterator++, i++)
            {
                *(*pppSt_ListClient)[i] = *stl_ListIterator;
            }
        }
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
函数名称：Session_Authorize_GetUserForAddr
函数功能：通过IP地址获取对应用户名
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入IP地址端口
 参数.二：pSt_Client
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：导出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Authorize::Session_Authorize_GetUserForAddr(LPCXSTR lpszClientAddr, AUTHSESSION_NETCLIENT* pSt_Client /* = NULL */)
{
    Session_IsErrorOccur = false;

    if ((NULL == lpszClientAddr))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return false;
    }
    bool bFound = false;
    st_Locker.lock_shared();
    for (auto stl_MapIterator = stl_MapNetClient.begin(); stl_MapIterator != stl_MapNetClient.end(); stl_MapIterator++)
    {
        for (auto stl_ListIterator = stl_MapIterator->second.begin(); stl_ListIterator != stl_MapIterator->second.end(); stl_ListIterator++)
        {
            if (0 == _tcsxnicmp(lpszClientAddr, stl_ListIterator->tszClientAddr, _tcsxlen(lpszClientAddr)))
            {
                if (NULL != pSt_Client)
                {
                    *pSt_Client = *stl_ListIterator;
                }
                bFound = true;
                break;
            }
        }
        if (bFound)
        {
            break;
        }
    }
    st_Locker.unlock_shared();
    if (!bFound)
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_NOTFOUND;
        return false;
    }
    return true;
}
/********************************************************************
函数名称：Session_Authorize_CloseAddr
函数功能：移除一个客户端
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要移除的地址
返回值
  类型：逻辑型
  意思：是否移除成功
备注：
*********************************************************************/
bool CSession_Authorize::Session_Authorize_CloseAddr(LPCXSTR lpszClientAddr)
{
    Session_IsErrorOccur = false;

    bool bFound = false;
    st_Locker.lock();
    for (auto stl_MapIterator = stl_MapNetClient.begin(); stl_MapIterator != stl_MapNetClient.end(); stl_MapIterator++)
    {
		for (auto stl_ListIterator = stl_MapIterator->second.begin(); stl_ListIterator != stl_MapIterator->second.end(); stl_ListIterator++)
		{
			if (0 == _tcsxnicmp(lpszClientAddr, stl_ListIterator->tszClientAddr, _tcsxlen(lpszClientAddr)))
			{
                bFound = true;
				stl_MapIterator->second.erase(stl_ListIterator);
				break;
			}
		}
		if (stl_MapIterator->second.empty())
		{
			//移除元素
			stl_MapNetClient.erase(stl_MapIterator);
		}
        if (bFound)
        {
            break;
        }
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
bool CSession_Authorize::Session_Authorize_Insert(LPCXSTR lpszClientAddr, AUTHREG_USERTABLE* pSt_UserTable, int nNetType)
{
    Session_IsErrorOccur = false;

    if ((NULL == lpszClientAddr) || (NULL == pSt_UserTable))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return false;
    }
    //验证是否登陆
    if (Session_Authorize_GetUserForAddr(lpszClientAddr))
    {
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_ISLOGIN;
		return false;
    }
	AUTHSESSION_NETCLIENT st_Client;
	memset(&st_Client, '\0', sizeof(AUTHSESSION_NETCLIENT));

	st_Client.nNetType = nNetType;
    st_Client.nLeftTime = _ttxoll(pSt_UserTable->tszLeftTime);

	BaseLib_OperatorTime_GetSysTime(&st_Client.st_LibTimer);
	_tcsxcpy(st_Client.tszClientAddr, lpszClientAddr);
	memcpy(&st_Client.st_UserTable, pSt_UserTable, sizeof(AUTHREG_USERTABLE));

    st_Locker.lock();
    auto stl_MapIterator = stl_MapNetClient.find(pSt_UserTable->st_UserInfo.tszUserName);
    if (stl_MapIterator == stl_MapNetClient.end())
    {
        //没找到,插入
        list<AUTHSESSION_NETCLIENT> stl_ListClient = { st_Client };
		stl_MapNetClient.insert(make_pair(pSt_UserTable->st_UserInfo.tszUserName, stl_ListClient));
    }
    else
    {
        //找到了
        stl_MapIterator->second.push_back(st_Client);
    }
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
    auto stl_MapIterator = stl_MapNetClient.find(pSt_UserTable->st_UserInfo.tszUserName);
    if (stl_MapIterator == stl_MapNetClient.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    for (auto stl_ListIterator = stl_MapIterator->second.begin(); stl_ListIterator != stl_MapIterator->second.end(); stl_ListIterator++)
    {
        stl_ListIterator->st_UserTable = *pSt_UserTable;
    }
    st_Locker.unlock_shared();
    return true;
}
//////////////////////////////////////////////////////////////////////////
//                     线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CSession_Authorize::Session_Authorize_ActiveThread(XPVOID lParam)
{
    CSession_Authorize* pClass_This = (CSession_Authorize*)lParam;
	list<AUTHREG_PROTOCOL_TIME> stl_ListNotify;

    while (pClass_This->bIsRun)
    {
        //开始轮训用户
        pClass_This->st_Locker.lock_shared();
        for (auto stl_MapIterator = pClass_This->stl_MapNetClient.begin(); stl_MapIterator != pClass_This->stl_MapNetClient.end(); stl_MapIterator++)
        {
            __int64x nTimeCount = 0;
            AUTHREG_PROTOCOL_TIME st_ProtocolTimer;
            
            for (auto stl_ListIterator = stl_MapIterator->second.begin(); stl_ListIterator != stl_MapIterator->second.end(); stl_ListIterator++)
            {
                __int64x nOnlineSpan = 0;                                        //在线时间
				XENGINE_LIBTIMER st_LibTimer;
				memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));
                memset(&st_ProtocolTimer, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
                //获取现在的系统时间
				BaseLib_OperatorTime_GetSysTime(&st_LibTimer);                 
				//用户登录了多少分钟
				BaseLib_OperatorTimeSpan_GetForStu(&stl_ListIterator->st_LibTimer, &st_LibTimer, &nOnlineSpan, ENUM_XENGINE_BASELIB_TIME_SPAN_TYPE_SECOND);
                nTimeCount += nOnlineSpan;
				//登陆成功的。我们要处理他过期
				switch (stl_ListIterator->st_UserTable.enSerialType)
				{
				case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_SECOND:
				{
					//秒钟处理
					stl_ListIterator->nOnlineTime = nOnlineSpan;
					//赋值给回调函数
					st_ProtocolTimer.nTimeONLine = nOnlineSpan;
					st_ProtocolTimer.nNetType = stl_ListIterator->nNetType;
					st_ProtocolTimer.nTimeLeft = stl_ListIterator->nLeftTime;
					st_ProtocolTimer.enSerialType = stl_ListIterator->st_UserTable.enSerialType;
					st_ProtocolTimer.enDeviceType = stl_ListIterator->st_UserTable.enDeviceType;
					_tcsxcpy(st_ProtocolTimer.tszLeftTime, stl_ListIterator->tszLeftTime);
					_tcsxcpy(st_ProtocolTimer.tszUserAddr, stl_ListIterator->tszClientAddr);
					_tcsxcpy(st_ProtocolTimer.tszUserName, stl_ListIterator->st_UserTable.st_UserInfo.tszUserName);
                    break;
				}
				case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME:
				{
					//次数处理
					stl_ListIterator->nOnlineTime = nOnlineSpan;
					_xstprintf(stl_ListIterator->tszLeftTime, _X("%lld"), stl_ListIterator->nLeftTime);
					//次数处理不做任何时间操作
					st_ProtocolTimer.nTimeONLine = nOnlineSpan;
					st_ProtocolTimer.nNetType = stl_ListIterator->nNetType;
					st_ProtocolTimer.nTimeLeft = stl_ListIterator->nLeftTime;
					st_ProtocolTimer.enSerialType = stl_ListIterator->st_UserTable.enSerialType;
					st_ProtocolTimer.enDeviceType = stl_ListIterator->st_UserTable.enDeviceType;
					_tcsxcpy(st_ProtocolTimer.tszLeftTime, stl_ListIterator->st_UserTable.tszLeftTime);
					_tcsxcpy(st_ProtocolTimer.tszUserAddr, stl_ListIterator->tszClientAddr);
					_tcsxcpy(st_ProtocolTimer.tszUserName, stl_ListIterator->st_UserTable.st_UserInfo.tszUserName);
                    break;
				}
				case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_DAY:
				{
					//天数卡处理,天数卡需要特殊处理
					break;
				}
				default:
                {
					//自定义
					__int64x nLeftTime = 0;                                          //剩余时间
					XENGINE_LIBTIMER st_TimeLeft;
					memset(&st_TimeLeft, '\0', sizeof(XENGINE_LIBTIMER));
					//获取指定过期日期
					if (6 != _stxscanf(stl_ListIterator->st_UserTable.tszLeftTime, _X("%04d-%02d-%02d %02d:%02d:%02d"), &st_TimeLeft.wYear, &st_TimeLeft.wMonth, &st_TimeLeft.wDay, &st_TimeLeft.wHour, &st_TimeLeft.wMinute, &st_TimeLeft.wSecond))
					{
						break;
					}
					//剩余天数,通过秒钟来处理
					BaseLib_OperatorTimeSpan_GetForStu(&st_LibTimer, &st_TimeLeft, &nLeftTime, ENUM_XENGINE_BASELIB_TIME_SPAN_TYPE_SECOND);
					//获取过期时间
					stl_ListIterator->nLeftTime = nLeftTime;
					stl_ListIterator->nOnlineTime = nOnlineSpan;
					_tcsxcpy(stl_ListIterator->tszLeftTime, stl_ListIterator->st_UserTable.tszLeftTime);
					//计算时间是否超过！
					AUTHREG_PROTOCOL_TIME st_ProtocolTimer;
					memset(&st_ProtocolTimer, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

					st_ProtocolTimer.nTimeONLine = nOnlineSpan;
					st_ProtocolTimer.nTimeLeft = nLeftTime;
					st_ProtocolTimer.nNetType = stl_ListIterator->nNetType;
					st_ProtocolTimer.enSerialType = stl_ListIterator->st_UserTable.enSerialType;
					st_ProtocolTimer.enDeviceType = stl_ListIterator->st_UserTable.enDeviceType;
					_tcsxcpy(st_ProtocolTimer.tszLeftTime, stl_ListIterator->st_UserTable.tszLeftTime);
					_tcsxcpy(st_ProtocolTimer.tszUserAddr, stl_ListIterator->tszClientAddr);
					_tcsxcpy(st_ProtocolTimer.tszUserName, stl_ListIterator->st_UserTable.st_UserInfo.tszUserName);
					break;
                }
				}
            }
            //处理多端登录情况
            if (nTimeCount > 0)
            {
                st_ProtocolTimer.nTimeLeft -= nTimeCount;
                for (auto stl_ListIterator = stl_MapIterator->second.begin(); stl_ListIterator != stl_MapIterator->second.end(); stl_ListIterator++)
                {
                    _xstprintf(stl_ListIterator->tszLeftTime, _X("%lld"), st_ProtocolTimer.nTimeLeft);
                }
            }
            stl_ListNotify.push_back(st_ProtocolTimer);
        }
        pClass_This->st_Locker.unlock_shared();
        //返回数据
        if (!stl_ListNotify.empty())
        {
            list<AUTHREG_PROTOCOL_TIME>::iterator stl_ListIterator = stl_ListNotify.begin();
            for (; stl_ListIterator != stl_ListNotify.end(); stl_ListIterator++)
            {
                pClass_This->lpCall_AuthregEvents(stl_ListIterator->tszUserAddr, stl_ListIterator->tszUserName, stl_ListIterator->nTimeONLine, stl_ListIterator->nTimeLeft, stl_ListIterator->tszLeftTime, stl_ListIterator->enSerialType, stl_ListIterator->enDeviceType, stl_ListIterator->nNetType, pClass_This->m_lParam);
            }
        }
        //清理元素
        stl_ListNotify.clear();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
