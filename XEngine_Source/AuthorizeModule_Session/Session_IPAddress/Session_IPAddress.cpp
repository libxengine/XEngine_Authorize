#include "pch.h"
#include "Session_IPAddress.h"
/********************************************************************
//    Created:     2022/05/26  10:53:14
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_IPAddress\Session_IPAddress.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_IPAddress
//    File Base:   Session_IPAddress
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     IP会话管理器
//    History:
*********************************************************************/
CSession_IPAddress::CSession_IPAddress()
{
}
CSession_IPAddress::~CSession_IPAddress()
{
}
//////////////////////////////////////////////////////////////////////////
//                     公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Session_IPAddress_Init
函数功能：初始化会话
 参数.一：fpCall_IPAddrEvent
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：IP地址超出事件
 参数.二：nTimeout
  In/Out：In
  类型：整数型
  可空：Y
  意思：超时时间,单位秒
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
bool CSession_IPAddress::Session_IPAddress_Init(CALLBACK_AUTHORIZE_MODULE_SESSION_IPADDR_EVENTS fpCall_IPAddrEvent, int nTimeout, XPVOID lParam /* = NULL */)
{
    Session_IsErrorOccur = false;

    if (NULL == fpCall_IPAddrEvent)
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return false;
    }
    m_lParam = lParam;
    lpCall_IPAddrEvents = fpCall_IPAddrEvent;

    bIsRun = true;
    m_nTimeout = nTimeout;
    //创建线程
    pSTDThread = std::make_unique<std::thread>(Session_IPAddress_Thread, this);
    if (!pSTDThread->joinable())
    {
        bIsRun = false;
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_CREATETHREAD;
        return false;
    }
    return true;
}
/********************************************************************
函数名称：Session_IPAddress_Get
函数功能：获取IP是否存在
 参数.一：lpszIPAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入判断的IP地址
返回值
  类型：逻辑型
  意思：是否存在
备注：
*********************************************************************/
bool CSession_IPAddress::Session_IPAddress_Get(LPCXSTR lpszIPAddr)
{
    Session_IsErrorOccur = false;

    if (NULL == lpszIPAddr)
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return false;
    }
    std::unique_lock<std::shared_mutex> m_Locker(st_Locker);

    XCHAR tszIPAddr[XPATH_MIN] = {};
	_tcsxcpy(tszIPAddr, lpszIPAddr);
    APIAddr_IPAddr_SegAddr(tszIPAddr);

	auto stl_SetIterator = stl_MapIPClient.find(tszIPAddr);
    if (stl_SetIterator != stl_MapIPClient.end())
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_EXIST;
        return false;
    }
    return true;
}
/********************************************************************
函数名称：Session_IPAddress_Insert
函数功能：插入IP地址到IP限定管理器
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入IP地址端口
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_IPAddress::Session_IPAddress_Insert(LPCXSTR lpszClientAddr)
{
    Session_IsErrorOccur = false;

    if ((NULL == lpszClientAddr))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT;
        return false;
    }
    std::unique_lock<std::shared_mutex> m_Locker(st_Locker);

    XCHAR tszIPAddr[XPATH_MIN] = {};
    _tcsxcpy(tszIPAddr, lpszClientAddr);
    APIAddr_IPAddr_SegAddr(tszIPAddr);

	stl_MapIPClient.insert(std::make_pair(tszIPAddr, SESSION_IPADDRESS(time(NULL))));
    return true;
}
/********************************************************************
函数名称：Session_IPAddress_Close
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
bool CSession_IPAddress::Session_IPAddress_Close(LPCXSTR lpszClientAddr)
{
    Session_IsErrorOccur = false;

    std::unique_lock<std::shared_mutex> m_Locker(st_Locker);

    XCHAR tszIPAddr[XPATH_MIN] = {};
    _tcsxcpy(tszIPAddr, lpszClientAddr);
    APIAddr_IPAddr_SegAddr(tszIPAddr);

	auto stl_SetIterator = stl_MapIPClient.find(tszIPAddr);
	if (stl_SetIterator == stl_MapIPClient.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_AUTHORIZE_MODULE_SESSION_NOTFOUND;
		return false;
	}
	stl_MapIPClient.erase(stl_SetIterator);

    return true;
}
/********************************************************************
函数名称：Session_IPAddress_Destroy
函数功能：销毁服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
bool CSession_IPAddress::Session_IPAddress_Destroy()
{
    Session_IsErrorOccur = false;

    bIsRun = false;
    //结束线程
    if (bIsRun)
    {
        bIsRun = false;
        if (NULL != pSTDThread)
        {
            pSTDThread->join();
        }
    }
    //释放客户端资源
    st_Locker.lock();
    stl_MapIPClient.clear();
    st_Locker.unlock();
    return true;
}
//////////////////////////////////////////////////////////////////////////
//                     线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CSession_IPAddress::Session_IPAddress_Thread(XPVOID lParam)
{
    CSession_IPAddress* pClass_This = (CSession_IPAddress*)lParam;
    std::list<xstring> stl_ListIPClient;

    while (pClass_This->bIsRun)
    {
		time_t nTimeEnd = time(NULL);
        //开始轮训用户
        pClass_This->st_Locker.lock_shared();
        for (auto stl_MapIterator = pClass_This->stl_MapIPClient.begin(); stl_MapIterator != pClass_This->stl_MapIPClient.end(); stl_MapIterator++)
        {
            if (nTimeEnd - stl_MapIterator->second.nTimeStart > pClass_This->m_nTimeout)
            {
                stl_ListIPClient.push_back(stl_MapIterator->first);
            }
        }
        pClass_This->st_Locker.unlock_shared();
        //返回数据
        if (!stl_ListIPClient.empty())
        {
            for (const auto& stl_ListIterator : stl_ListIPClient)
            {
                pClass_This->lpCall_IPAddrEvents(stl_ListIterator.c_str(), pClass_This->m_lParam);
            }
        }
        //清理元素
        stl_ListIPClient.clear();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
