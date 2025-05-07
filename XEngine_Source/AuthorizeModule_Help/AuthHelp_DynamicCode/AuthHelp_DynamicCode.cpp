#include "pch.h"
#include "AuthHelp_DynamicCode.h"
/********************************************************************
//    Created:     2023/04/13  10:50:54
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_DynamicCode\AuthHelp_DynamicCode.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_DynamicCode
//    File Base:   AuthHelp_DynamicCode
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     动态码
//    History:
*********************************************************************/
CAuthHelp_DynamicCode::CAuthHelp_DynamicCode()
{
}
CAuthHelp_DynamicCode::~CAuthHelp_DynamicCode()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：AuthHelp_DynamicCode_Init
函数功能：初始化动态码
 参数.一：nSecond
  In/Out：In
  类型：整数型
  可空：N
  意思：动态码超时时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthHelp_DynamicCode::AuthHelp_DynamicCode_Init(int nSecond)
{
	Help_IsErrorOccur = true;

	m_bRun = true;
	m_nSescond = nSecond;
	pSTDThread = make_shared<thread>(AuthHelp_DynamicCode_Thread, this);
	if (NULL == pSTDThread)
	{
		Help_IsErrorOccur = true;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_DYNAMIC_THREAD;
		return false;
	}
	
	return true;
}
/********************************************************************
函数名称：AuthHelp_DynamicCode_Destory
函数功能：销毁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthHelp_DynamicCode::AuthHelp_DynamicCode_Destory()
{
	Help_IsErrorOccur = false;

	if (NULL != pSTDThread)
	{
		m_bRun = false;
		pSTDThread->join();
	}
	st_Locker.lock();
	stl_MapDynamicCode.clear();
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：AuthHelp_DynamicCode_Create
函数功能：创建一个动态码
 参数.一：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出动态码绑定的句柄
 参数.二：pInt_DynamicCode
  In/Out：Out
  类型：整数型
  可空：N
  意思：输出动态码
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthHelp_DynamicCode::AuthHelp_DynamicCode_Create(XNETHANDLE* pxhToken, XSHOT* pInt_DynamicCode)
{
	Help_IsErrorOccur = false;
	
	if ((NULL == pxhToken) || (NULL == pInt_DynamicCode))
	{
		Help_IsErrorOccur = true;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_DYNAMIC_PARAMENT;
		return false;
	}
	AUTHHELP_DYNAMICCODE st_DynamicCode;
	memset(&st_DynamicCode, '\0', sizeof(AUTHHELP_DYNAMICCODE));

	st_DynamicCode.nTimeStart = time(NULL);
	BaseLib_Handle_Create(&st_DynamicCode.xhToken);
	BaseLib_Handle_Create((XNETHANDLE*)&st_DynamicCode.nDynamicCode, 10001, 65535);
	
	*pxhToken = st_DynamicCode.xhToken;
	*pInt_DynamicCode = st_DynamicCode.nDynamicCode;
	st_Locker.lock();
	stl_MapDynamicCode.insert(make_pair(*pxhToken, st_DynamicCode));
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：AuthHelp_DynamicCode_Get
函数功能：获取句柄和动态码绑定信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入操作的句柄
 参数.二：nDynamicCode
  In/Out：In
  类型：整数型
  可空：N
  意思：输入绑定的动态码
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthHelp_DynamicCode::AuthHelp_DynamicCode_Get(XNETHANDLE xhToken, int nDynamicCode)
{
	Help_IsErrorOccur = false;

	st_Locker.lock();
	auto stl_MapIterator = stl_MapDynamicCode.find(xhToken);
	if (stl_MapIterator == stl_MapDynamicCode.end())
	{
		Help_IsErrorOccur = true;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_DYNAMIC_NOTFOUND;
		st_Locker.unlock();
		return false;
	}
	if (nDynamicCode != stl_MapIterator->second.nDynamicCode)
	{
		Help_IsErrorOccur = true;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_DYNAMIC_NOTMATCH;
		st_Locker.unlock();
		return false;
	}
	stl_MapDynamicCode.erase(stl_MapIterator);
	st_Locker.unlock();
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                      保护函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CALLBACK CAuthHelp_DynamicCode::AuthHelp_DynamicCode_Thread(XPVOID lParam)
{
	CAuthHelp_DynamicCode* pClass_This = (CAuthHelp_DynamicCode*)lParam;
	//用于处理超时的token
	while (pClass_This->m_bRun)
	{
		pClass_This->st_Locker.lock();
		for (auto stl_MapIterator = pClass_This->stl_MapDynamicCode.begin(); stl_MapIterator != pClass_This->stl_MapDynamicCode.end(); )
		{
			time_t nTimeEnd = time(NULL);
			if ((nTimeEnd - stl_MapIterator->second.nTimeStart) > pClass_This->m_nSescond)
			{
				stl_MapIterator = pClass_This->stl_MapDynamicCode.erase(stl_MapIterator);
			}
			else
			{
				stl_MapIterator++;
			}
		}
		pClass_This->st_Locker.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
}