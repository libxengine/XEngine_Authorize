#pragma once
/********************************************************************
//    Created:     2025/04/19  14:02:23
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XEngine_APPService\XEngine_AuthorizeService\Authorize_HttpMemory.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\XEngine_APPService\XEngine_AuthorizeService
//    File Base:   Authorize_HttpMemory
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     内存池封装
//    History:
*********************************************************************/
class CHttpMemory_PoolEx
{
public:
	CHttpMemory_PoolEx(size_t nSize)
	{
		lPtr = ManagePool_Memory_Alloc(xhMemPool, nSize);
		if (NULL == lPtr)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ALERT, _X("内存池分配失败，系统面临崩溃!"));
			return;
		}
	}
	~CHttpMemory_PoolEx() 
	{
		if (NULL != lPtr)
		{
			ManagePool_Memory_Free(xhMemPool, lPtr);
		}
	}
public:
	// 支持类型转换
	template<typename T>
	T* as() {
		return static_cast<T*>(lPtr);
	}
	XCHAR* get()
	{
		return static_cast<XCHAR*>(lPtr);
	}
private:
	XPVOID lPtr = NULL;
};