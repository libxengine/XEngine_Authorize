﻿#include "pch.h"
#include "Authorize_CDKey.h"
/********************************************************************
//    Created:     2024/07/08  17:30:39
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey\Authorize_CDKey\Authorize_CDKey.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey\Authorize_CDKey
//    File Base:   Authorize_CDKey
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     本地验证实现
//    History:
*********************************************************************/
CAuthorize_CDKey::CAuthorize_CDKey()
{
}
CAuthorize_CDKey::~CAuthorize_CDKey()
{

}
//////////////////////////////////////////////////////////////////////////
//                         公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Authorize_CDKey_WriteKey
函数功能：写一个CDKey文件
 参数.一：lpszFileKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要操作的文件路径
 参数.二：pSt_AuthLocal
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要写入的文件信息
返回值
  类型：逻辑型
  意思：是否成功
备注：写入读取必须是明文,建议你加密处理CDKEY,通过OPENSSL模块,来加解密,在读写
*********************************************************************/
bool CAuthorize_CDKey::Authorize_CDKey_WriteKey(LPCXSTR lpszFileKey, XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal)
{
    Authorize_IsErrorOccur = false;

    if ((NULL == lpszFileKey) || (NULL == pSt_AuthLocal))
    {
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
		return false;
    }
    //添加连接信息文本
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("Connection"), _X("tszAddr"), pSt_AuthLocal->tszAddr))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_CONNECT;
        return false;
    }
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("Connection"), _X("nPort"), pSt_AuthLocal->nPort))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_CONNECT;
        return false;
    }
    //添加程序名称文本
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AppInfo"), _X("tszAppName"), pSt_AuthLocal->st_AuthAppInfo.tszAppName))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_APPINFO;
        return false;
    }
    if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey,_X("AppInfo"), _X("tszAppVer"), pSt_AuthLocal->st_AuthAppInfo.tszAppVer))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_APPINFO;
        return false;
    }
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("AppInfo"), _X("nExecTime"), pSt_AuthLocal->st_AuthAppInfo.nExecTime))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_APPINFO;
		return false;
	}
    if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey,_X("AppInfo"), _X("bInit"), pSt_AuthLocal->st_AuthAppInfo.bInit))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_APPINFO;
        return false;
    }
	//更新使用时间
	if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_SECOND == pSt_AuthLocal->st_AuthRegInfo.enSerialType)
	{
		XCHAR tszTimeStart[64];
		XCHAR tszTimeEnd[64];
		__int64x nUsedTime = 0;

		memset(tszTimeStart, '\0', sizeof(tszTimeStart));
		memset(tszTimeEnd, '\0', sizeof(tszTimeEnd));
		BaseLib_Time_TimeToStr(tszTimeEnd);
		BaseLib_TimeSpan_GetForStr(pSt_AuthLocal->st_AuthRegInfo.tszStartTime, tszTimeEnd, &nUsedTime, ENUM_XENGINE_BASELIB_TIME_TYPE_SECOND);
		pSt_AuthLocal->st_AuthRegInfo.nHasTime -= nUsedTime;
	}
    //添加注册信息
    if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey,_X("AuthReg"), _X("nHasTime"), pSt_AuthLocal->st_AuthRegInfo.nHasTime))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
        return false;
    }
    if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey,_X("AuthReg"), _X("enHWType"), pSt_AuthLocal->st_AuthRegInfo.enHWType))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
        return false;
    }
    if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey,_X("AuthReg"), _X("enRegType"), pSt_AuthLocal->st_AuthRegInfo.enRegType))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
        return false;
    }
    if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey,_X("AuthReg"), _X("enSerialType"), pSt_AuthLocal->st_AuthRegInfo.enSerialType))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
        return false;
    }
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey,_X("AuthReg"), _X("enVModeType"), pSt_AuthLocal->st_AuthRegInfo.enVModeType))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
		return false;
	}
    if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey,_X("AuthReg"), _X("tszHardware"), pSt_AuthLocal->st_AuthRegInfo.tszHardware))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
        return false;
    }
    BaseLib_Time_TimeToStr(pSt_AuthLocal->st_AuthRegInfo.tszCreateTime);
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey,_X("AuthReg"), _X("tszCreateTime"), pSt_AuthLocal->st_AuthRegInfo.tszCreateTime))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
		return false;
	}
    if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey,_X("AuthReg"), _X("tszRegisterTime"), pSt_AuthLocal->st_AuthRegInfo.tszRegisterTime))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
        return false;
    }
    if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey,_X("AuthReg"), _X("tszLeftTime"), pSt_AuthLocal->st_AuthRegInfo.tszLeftTime))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
        return false;
    }
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey,_X("AuthReg"), _X("tszStartTime"), pSt_AuthLocal->st_AuthRegInfo.tszStartTime))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey,_X("AuthReg"), _X("tszExpiryTime"), pSt_AuthLocal->st_AuthRegInfo.tszExpiryTime))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
		return false;
	}
	//临时序列号
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszTimeSerial"), pSt_AuthLocal->st_AuthSerial.st_TimeLimit.tszTimeSerial))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_SERIAL;
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("AuthSerial"), _X("nTimeCount"), pSt_AuthLocal->st_AuthSerial.st_TimeLimit.nTimeCount))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_SERIAL;
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszDataSerial"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataSerial))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_SERIAL;
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszDataTime"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_SERIAL;
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromFile(lpszFileKey, _X("AuthSerial"), _X("bTimeAdd"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.bTimeAdd))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_SERIAL;
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszUNLimitSerial"), pSt_AuthLocal->st_AuthSerial.st_UNLimit.tszUNLimitSerial))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_SERIAL;
		return false;
	}
	//自定义用户信息
    if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthUser"), _X("tszUserName"), pSt_AuthLocal->st_AuthUserInfo.tszUserName))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_USER;
        return false;
    }
    if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthUser"), _X("tszUserContact"), pSt_AuthLocal->st_AuthUserInfo.tszUserContact))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_USER;
        return false;
    }
    if (!SystemConfig_File_WriteProfileFromFile(lpszFileKey, _X("AuthUser"), _X("tszCustom"), pSt_AuthLocal->st_AuthUserInfo.tszCustom))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_USER;
        return false;
    }
    return true;
}
/********************************************************************
函数名称：Authorize_CDKey_ReadKey
函数功能：读一个数据文件
 参数.一：lpszFileKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要操作的文件路径
 参数.二：pSt_AuthLocal
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：导出获取到的文件信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthorize_CDKey::Authorize_CDKey_ReadKey(LPCXSTR lpszFileKey, XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal)
{
    Authorize_IsErrorOccur = false;

    if ((NULL == lpszFileKey) || (NULL == pSt_AuthLocal))
    {
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
		return false;
    }
    //添加连接信息文本
    if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("Connection"), _X("tszAddr"), pSt_AuthLocal->tszAddr) <= 0)
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_CONNECT;
        return false;
    }
	pSt_AuthLocal->nPort = SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("Connection"), _X("nPort"));
    //添加程序名称文本
    if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AppInfo"), _X("tszAppName"), pSt_AuthLocal->st_AuthAppInfo.tszAppName) <= 0)
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_APPINFO;
        return false;
    }
    if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AppInfo"), _X("tszAppVer"), pSt_AuthLocal->st_AuthAppInfo.tszAppVer) <= 0)
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_APPINFO;
        return false;
    }
    pSt_AuthLocal->st_AuthAppInfo.nExecTime = SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AppInfo"), _X("nExecTime"));
    pSt_AuthLocal->st_AuthAppInfo.bInit = SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AppInfo"), _X("bInit"));
    //添加注册信息
    pSt_AuthLocal->st_AuthRegInfo.enHWType = (ENUM_AUTHORIZE_MODULE_HW_TYPE)SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AuthReg"), _X("enHWType"));
    pSt_AuthLocal->st_AuthRegInfo.enRegType = (ENUM_AUTHORIZE_MODULE_CDKEY_TYPE)SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AuthReg"), _X("enRegType"));
    pSt_AuthLocal->st_AuthRegInfo.enSerialType = (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE)SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AuthReg"), _X("enSerialType"));
	pSt_AuthLocal->st_AuthRegInfo.enVModeType = (ENUM_AUTHORIZE_MODULE_VERMODE_TYPE)SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AuthReg"), _X("enVModeType"));
	pSt_AuthLocal->st_AuthRegInfo.nHasTime = SystemConfig_File_ReadInt64FromFile(lpszFileKey, _X("AuthReg"), _X("nHasTime"));
    if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszHardware"), pSt_AuthLocal->st_AuthRegInfo.tszHardware) <= 0)
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
        return false;
    }
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszCreateTime"), pSt_AuthLocal->st_AuthRegInfo.tszCreateTime) <= 0)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
		return false;
	}
    if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszRegisterTime"), pSt_AuthLocal->st_AuthRegInfo.tszRegisterTime) < 0)
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
        return false;
    }
    if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszLeftTime"), pSt_AuthLocal->st_AuthRegInfo.tszLeftTime) < 0)
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
        return false;
    }
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszStartTime"), pSt_AuthLocal->st_AuthRegInfo.tszStartTime) < 0)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
		return false;
	}
	//设置启动时间
	if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_DAY == pSt_AuthLocal->st_AuthRegInfo.enSerialType)
	{
		XENGINE_LIBTIME st_EndTimer;
		XENGINE_LIBTIME st_StartTime;

		memset(&st_EndTimer, '\0', sizeof(XENGINE_LIBTIME));
		memset(&st_StartTime, '\0', sizeof(XENGINE_LIBTIME));

		BaseLib_Time_GetSysTime(&st_EndTimer);
		BaseLib_Time_StrToTime(pSt_AuthLocal->st_AuthRegInfo.tszStartTime, &st_StartTime);
		if ((st_EndTimer.wYear != st_StartTime.wYear) || (st_EndTimer.wMonth != st_StartTime.wMonth) || (st_EndTimer.wDay != st_StartTime.wDay))
		{
			pSt_AuthLocal->st_AuthRegInfo.nHasTime--;
		}
		BaseLib_Time_TimeToStr(pSt_AuthLocal->st_AuthRegInfo.tszStartTime);
	}
	else if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_SECOND == pSt_AuthLocal->st_AuthRegInfo.enSerialType)
	{
		memset(pSt_AuthLocal->st_AuthRegInfo.tszStartTime, '\0', sizeof(pSt_AuthLocal->st_AuthRegInfo.tszStartTime));
		BaseLib_Time_TimeToStr(pSt_AuthLocal->st_AuthRegInfo.tszStartTime);
	}
	
    if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthReg"), _X("tszExpiryTime"), pSt_AuthLocal->st_AuthRegInfo.tszExpiryTime) < 0)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG;
		return false;
	}
	//临时序列号
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszTimeSerial"), pSt_AuthLocal->st_AuthSerial.st_TimeLimit.tszTimeSerial) <= 0)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_SERIAL;
		return false;
	}
	pSt_AuthLocal->st_AuthSerial.st_TimeLimit.nTimeCount = SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AuthSerial"), _X("nTimeCount"));
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszDataSerial"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataSerial) <= 0)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_SERIAL;
		return false;
	}
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszDataTime"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime) <= 0)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_SERIAL;
		return false;
	}
	pSt_AuthLocal->st_AuthSerial.st_DataLimit.bTimeAdd = SystemConfig_File_ReadIntFromFile(lpszFileKey, _X("AuthSerial"), _X("bAddTime"));
	if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthSerial"), _X("tszUNLimitSerial"), pSt_AuthLocal->st_AuthSerial.st_UNLimit.tszUNLimitSerial) <= 0)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_SERIAL;
		return false;
	}
    //用户信息
    if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthUser"), _X("tszUserName"), pSt_AuthLocal->st_AuthUserInfo.tszUserName) <= 0)
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_USER;
        return false;
    }
    if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthUser"), _X("tszUserContact"), pSt_AuthLocal->st_AuthUserInfo.tszUserContact) <= 0)
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_USER;
        return false;
    }
    if (SystemConfig_File_ReadProfileFromFile(lpszFileKey, _X("AuthUser"), _X("tszCustom"), pSt_AuthLocal->st_AuthUserInfo.tszCustom) < 0)
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_USER;
        return false;
    }
    return true;
}
/********************************************************************
函数名称：Authorize_CDKey_WriteMemory
函数功能：写配置信息到内存
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：写到的内存
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：写到的内存大小
 参数.三：pSt_AuthLocal
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要写的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthorize_CDKey::Authorize_CDKey_WriteMemory(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal)
{
	Authorize_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen) || (NULL == pSt_AuthLocal))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
		return false;
	}
    int nMsgLen = 0;
    //添加连接信息文本
    if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("Connection"), _X("tszAddr"), pSt_AuthLocal->tszAddr, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
    if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("Connection"), _X("nPort"), pSt_AuthLocal->nPort, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	//添加程序名称文本
    if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AppInfo"), _X("tszAppName"), pSt_AuthLocal->st_AuthAppInfo.tszAppName, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
    if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AppInfo"), _X("tszAppVer"), pSt_AuthLocal->st_AuthAppInfo.tszAppVer, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AppInfo"), _X("nExecTime"), pSt_AuthLocal->st_AuthAppInfo.nExecTime, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AppInfo"), _X("bInit"), pSt_AuthLocal->st_AuthAppInfo.bInit, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	//更新使用时间
	if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_SECOND == pSt_AuthLocal->st_AuthRegInfo.enSerialType)
	{
		XCHAR tszTimeStart[64];
		XCHAR tszTimeEnd[64];
		__int64x nUsedTime = 0;

		memset(tszTimeStart, '\0', sizeof(tszTimeStart));
		memset(tszTimeEnd, '\0', sizeof(tszTimeEnd));
		BaseLib_Time_TimeToStr(tszTimeEnd);
		BaseLib_TimeSpan_GetForStr(pSt_AuthLocal->st_AuthRegInfo.tszStartTime, tszTimeEnd, &nUsedTime, ENUM_XENGINE_BASELIB_TIME_TYPE_SECOND);
		pSt_AuthLocal->st_AuthRegInfo.nHasTime -= nUsedTime;
	}
	//添加注册信息
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("nHasTime"), pSt_AuthLocal->st_AuthRegInfo.nHasTime, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enHWType"), pSt_AuthLocal->st_AuthRegInfo.enHWType, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enRegType"), pSt_AuthLocal->st_AuthRegInfo.enRegType, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enSerialType"), pSt_AuthLocal->st_AuthRegInfo.enSerialType, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enVModeType"), pSt_AuthLocal->st_AuthRegInfo.enVModeType, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszHardware"), pSt_AuthLocal->st_AuthRegInfo.tszHardware, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
    BaseLib_Time_TimeToStr(pSt_AuthLocal->st_AuthRegInfo.tszCreateTime);
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszCreateTime"), pSt_AuthLocal->st_AuthRegInfo.tszCreateTime, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszRegisterTime"), pSt_AuthLocal->st_AuthRegInfo.tszRegisterTime, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszLeftTime"), pSt_AuthLocal->st_AuthRegInfo.tszLeftTime, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszStartTime"), pSt_AuthLocal->st_AuthRegInfo.tszStartTime, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszExpiryTime"), pSt_AuthLocal->st_AuthRegInfo.tszExpiryTime, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	//临时序列号
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszTimeSerial"), pSt_AuthLocal->st_AuthSerial.st_TimeLimit.tszTimeSerial, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("nTimeCount"), pSt_AuthLocal->st_AuthSerial.st_TimeLimit.nTimeCount, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszDataSerial"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataSerial, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszDataTime"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteInt64FromMemory(ptszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("bAddTime"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.bTimeAdd, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszUNLimitSerial"), pSt_AuthLocal->st_AuthSerial.st_UNLimit.tszUNLimitSerial, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	//用户信息
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthUser"), _X("tszUserName"), pSt_AuthLocal->st_AuthUserInfo.tszUserName, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthUser"), _X("tszUserContact"), pSt_AuthLocal->st_AuthUserInfo.tszUserContact, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_WriteProfileFromMemory(ptszMsgBuffer, nMsgLen, _X("AuthUser"), _X("tszCustom"), pSt_AuthLocal->st_AuthUserInfo.tszCustom, ptszMsgBuffer, &nMsgLen))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
    *pInt_MsgLen = nMsgLen;
	return true;
}
/********************************************************************
函数名称：Authorize_CDKey_ReadMemory
函数功能：内存配置文件读取
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要读取配置的内存
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入读取内存大小
 参数.三：pSt_AuthLocal
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出读取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthorize_CDKey::Authorize_CDKey_ReadMemory(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal)
{
	Authorize_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_AuthLocal))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
		return false;
	}
	//添加连接信息文本
    if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("Connection"), _X("tszAddr"), pSt_AuthLocal->tszAddr))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
    if (!SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("Connection"), _X("nPort"), &pSt_AuthLocal->nPort))
    {
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
    }
	//添加程序名称文本
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AppInfo"), _X("tszAppName"), pSt_AuthLocal->st_AuthAppInfo.tszAppName))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AppInfo"), _X("tszAppVer"), pSt_AuthLocal->st_AuthAppInfo.tszAppVer))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AppInfo"), _X("bInit"), (int*)&pSt_AuthLocal->st_AuthAppInfo.bInit);
	SystemConfig_File_ReadInt64FromMemory(lpszMsgBuffer, nMsgLen, _X("AppInfo"), _X("nExecTime"), &pSt_AuthLocal->st_AuthAppInfo.nExecTime);
	//添加注册信息
	SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enHWType"), (int*)&pSt_AuthLocal->st_AuthRegInfo.enHWType);
	SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enRegType"), (int*)&pSt_AuthLocal->st_AuthRegInfo.enRegType);
	SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enSerialType"), (int*)&pSt_AuthLocal->st_AuthRegInfo.enSerialType);
	SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("enVModeType"), (int*)&pSt_AuthLocal->st_AuthRegInfo.enVModeType);
	SystemConfig_File_ReadInt64FromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("nHasTime"), &pSt_AuthLocal->st_AuthRegInfo.nHasTime);

	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszHardware"), pSt_AuthLocal->st_AuthRegInfo.tszHardware))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszCreateTime"), pSt_AuthLocal->st_AuthRegInfo.tszCreateTime))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszRegisterTime"), pSt_AuthLocal->st_AuthRegInfo.tszRegisterTime))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszLeftTime"), pSt_AuthLocal->st_AuthRegInfo.tszLeftTime))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszStartTime"), pSt_AuthLocal->st_AuthRegInfo.tszStartTime))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	//设置启动时间
	if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_DAY == pSt_AuthLocal->st_AuthRegInfo.enSerialType)
	{
		XENGINE_LIBTIME st_EndTimer;
		XENGINE_LIBTIME st_StartTime;

		memset(&st_EndTimer, '\0', sizeof(XENGINE_LIBTIME));
		memset(&st_StartTime, '\0', sizeof(XENGINE_LIBTIME));

		BaseLib_Time_GetSysTime(&st_EndTimer);
		BaseLib_Time_StrToTime(pSt_AuthLocal->st_AuthRegInfo.tszStartTime, &st_StartTime);
		if ((st_EndTimer.wYear != st_StartTime.wYear) || (st_EndTimer.wMonth != st_StartTime.wMonth) || (st_EndTimer.wDay != st_StartTime.wDay))
		{
			pSt_AuthLocal->st_AuthRegInfo.nHasTime--;
		}
		BaseLib_Time_TimeToStr(pSt_AuthLocal->st_AuthRegInfo.tszStartTime);
	}
	else if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_SECOND == pSt_AuthLocal->st_AuthRegInfo.enSerialType)
	{
		memset(pSt_AuthLocal->st_AuthRegInfo.tszStartTime, '\0', sizeof(pSt_AuthLocal->st_AuthRegInfo.tszStartTime));
		BaseLib_Time_TimeToStr(pSt_AuthLocal->st_AuthRegInfo.tszStartTime);
	}

	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthReg"), _X("tszExpiryTime"), pSt_AuthLocal->st_AuthRegInfo.tszExpiryTime))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	//序列号信息
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszTimeSerial"), pSt_AuthLocal->st_AuthSerial.st_TimeLimit.tszTimeSerial))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("nTimeCount"), &pSt_AuthLocal->st_AuthSerial.st_TimeLimit.nTimeCount))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszDataSerial"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataSerial))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszDataTime"), pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	SystemConfig_File_ReadIntFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("bAddTime"), (int*)&pSt_AuthLocal->st_AuthSerial.st_DataLimit.bTimeAdd);
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthSerial"), _X("tszUNLimitSerial"), pSt_AuthLocal->st_AuthSerial.st_UNLimit.tszUNLimitSerial))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	//用户信息
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthUser"), _X("tszUserName"), pSt_AuthLocal->st_AuthUserInfo.tszUserName))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthUser"), _X("tszUserContact"), pSt_AuthLocal->st_AuthUserInfo.tszUserContact))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	if (!SystemConfig_File_ReadProfileFromMemory(lpszMsgBuffer, nMsgLen, _X("AuthUser"), _X("tszCustom"), pSt_AuthLocal->st_AuthUserInfo.tszCustom))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = BaseLib_GetLastError();
		return false;
	}
	return true;
}
/********************************************************************
函数名称：Authorize_CDKey_BuildKeyTime
函数功能：构造注册的时间结构体信息
 参数.一：pSt_AuthLocal
  In/Out：In
  类型：结构体指针
  可空：N
  意思：要构造的结构体
 参数.二：nDayTimer
  In/Out：In
  类型：整数型
  可空：Y
  意思：可用时间或者次数.非自定义时间需要设置此值
 参数.三：pSt_DayTimer
  In/Out：In
  类型：结构体指针
  可空：Y
  意思：如果是自定义时间,这个参数需要设置,其他类型请设置参数二
返回值
  类型：逻辑型
  意思：是否构造成功
备注：此函数会修改st_AuthRegInfo的时间信息成员,必须重写CDKEY
*********************************************************************/
bool CAuthorize_CDKey::Authorize_CDKey_BuildKeyTime(XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal, __int64x nDayTimer /* = 0 */, XENGINE_LIBTIME* pSt_DayTimer /* = NULL */)
{
    Authorize_IsErrorOccur = false;

	pSt_AuthLocal->st_AuthAppInfo.bInit = true;
    //判断注册时间方式
    if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_CUSTOM == pSt_AuthLocal->st_AuthRegInfo.enSerialType)
    {
		//按照到期时间计算
		_xstprintf(pSt_AuthLocal->st_AuthRegInfo.tszLeftTime, _X("%04d-%02d-%02d %02d:%02d:%02d"), pSt_DayTimer->wYear, pSt_DayTimer->wMonth, pSt_DayTimer->wDay, pSt_DayTimer->wHour, pSt_DayTimer->wMinute, pSt_DayTimer->wSecond);
    }
    else
    {
		pSt_AuthLocal->st_AuthRegInfo.nHasTime = nDayTimer;
		_xstprintf(pSt_AuthLocal->st_AuthRegInfo.tszLeftTime, _X("%lld"), nDayTimer);
    }
    return true;
}
/********************************************************************
函数名称：Authorize_CDKey_UserRegister
函数功能：用户注册CDKEY函数
 参数.一：pSt_AuthLocal
  In/Out：In
  类型：结构体指针
  可空：N
  意思：要构造的结构体
 参数.二：lpszSerialStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入注册验证的序列号
返回值
  类型：逻辑型
  意思：是否构造成功
备注：此函数会修改st_AuthRegInfo的时间信息成员,必须重写CDKEY
*********************************************************************/
bool CAuthorize_CDKey::Authorize_CDKey_UserRegister(XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal, LPCXSTR lpszSerialStr)
{
	Authorize_IsErrorOccur = false;

	if (NULL == pSt_AuthLocal || NULL == lpszSerialStr)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
		return false;
	}
	bool bSerial = false;

	if (0 == _tcsxncmp(pSt_AuthLocal->st_AuthSerial.st_TimeLimit.tszTimeSerial, lpszSerialStr, _tcsxlen(pSt_AuthLocal->st_AuthSerial.st_TimeLimit.tszTimeSerial)))
	{
		//次数验证
		bSerial = true;
		pSt_AuthLocal->st_AuthRegInfo.enSerialType = ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_TIME;
		pSt_AuthLocal->st_AuthRegInfo.enRegType = ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_OFFICIAL;
		Authorize_CDKey_BuildKeyTime(pSt_AuthLocal, pSt_AuthLocal->st_AuthSerial.st_TimeLimit.nTimeCount);
	}
	else if (0 == _tcsxncmp(pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataSerial, lpszSerialStr, _tcsxlen(pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataSerial)))
	{
		//日期序列
		bSerial = true;
		pSt_AuthLocal->st_AuthRegInfo.enSerialType = ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_CUSTOM;
		pSt_AuthLocal->st_AuthRegInfo.enRegType = ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_OFFICIAL;
		XENGINE_LIBTIME st_LibTime = {};
		BaseLib_Time_GetSysTime(&st_LibTime);
		if (pSt_AuthLocal->st_AuthSerial.st_DataLimit.bTimeAdd)
		{
			__int64x nTimeCount = 0;
			XENGINE_LIBTIME st_AddTime = {};

			BaseLib_TimeSpan_GetForStr(pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime, pSt_AuthLocal->st_AuthRegInfo.tszCreateTime, &nTimeCount);
			st_AddTime.wDay = (int)nTimeCount;
			BaseLib_TimeSpan_CalForStu(&st_LibTime, &st_AddTime);

			Authorize_CDKey_BuildKeyTime(pSt_AuthLocal, 0, &st_AddTime);
		}
		else
		{
			__int64x nTimeCount = 0;
			XCHAR tszTimeStr[128] = {};
			XENGINE_LIBTIME st_LibTime = {};
			BaseLib_Time_TimeToStr(tszTimeStr);

			BaseLib_TimeSpan_GetForStr(tszTimeStr, pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime, &nTimeCount);
			if (nTimeCount <= 0)
			{
				Authorize_IsErrorOccur = true;
				Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_TIMEOUT;
				return false;
			}
			BaseLib_Time_StrToTime(pSt_AuthLocal->st_AuthSerial.st_DataLimit.tszDataTime, &st_LibTime);
			Authorize_CDKey_BuildKeyTime(pSt_AuthLocal, 0, &st_LibTime);
		}
	}
	else if (0 == _tcsxncmp(pSt_AuthLocal->st_AuthSerial.st_UNLimit.tszUNLimitSerial, lpszSerialStr, _tcsxlen(pSt_AuthLocal->st_AuthSerial.st_UNLimit.tszUNLimitSerial)))
	{
		//无限制
		bSerial = true;
		pSt_AuthLocal->st_AuthRegInfo.enRegType = ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_UNLIMIT;
	}

	if (!bSerial)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_FAILED;
		return false;
	}
	BaseLib_Time_TimeToStr(pSt_AuthLocal->st_AuthRegInfo.tszRegisterTime);
	return true;
}
/********************************************************************
函数名称：Authorize_CDKey_GetLeftTimer
函数功能：验证CDKey
 参数.一：pSt_AuthLocal
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入Authorize_CDKey_ReadKey获取到的值
 参数.二：lpszSerialNumber
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：如果CDKEY带序列号,可以输入序列号进行验证(优先序列号验证,失败进行常规CDKEY验证)
返回值
  类型：逻辑型
  意思：是否成功
备注：无限制版本不做验证
      其他验证nHasTime将被设置还拥有时间
	  此函数在程序启动的时候必须调用,可以设置更新CDKEY信息也可以验证CDKEY
*********************************************************************/
bool CAuthorize_CDKey::Authorize_CDKey_GetLeftTimer(XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal, LPCXSTR lpszSerialNumber /* = NULL */)
{
    Authorize_IsErrorOccur = false;

	if (NULL == pSt_AuthLocal)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
		return false;
	}
	//验证CDKEY本身情况
	XENGINE_LIBTIME st_StartTimer;
	XENGINE_LIBTIME st_EndTimer;
	XENGINE_LIBTIME st_SysTimer;

	memset(&st_StartTimer, '\0', sizeof(XENGINE_LIBTIME));
	memset(&st_EndTimer, '\0', sizeof(XENGINE_LIBTIME));
	memset(&st_SysTimer, '\0', sizeof(XENGINE_LIBTIME));

	BaseLib_Time_TimeToStr(pSt_AuthLocal->st_AuthRegInfo.tszStartTime);
	//处理注册类型
	if (ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_UNLIMIT == pSt_AuthLocal->st_AuthRegInfo.enRegType)
	{
		return true;
	}
	else if (ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_UNKNOW == pSt_AuthLocal->st_AuthRegInfo.enRegType)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_UNKNOW;
		return false;
	}
	else if (ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_TEMP == pSt_AuthLocal->st_AuthRegInfo.enRegType)
	{
		if (_tcsxlen(pSt_AuthLocal->st_AuthRegInfo.tszExpiryTime) > 1)
		{
			Authorize_IsErrorOccur = true;
			Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_TEMP;
			return false;
		}
		else
		{
			pSt_AuthLocal->st_AuthRegInfo.enRegType = ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_EXPIRED;
			_xstprintf(pSt_AuthLocal->st_AuthRegInfo.tszExpiryTime, _X("%04d-%02d-%02d %02d:%02d:%02d"), st_SysTimer.wYear, st_SysTimer.wMonth, st_SysTimer.wDay, st_SysTimer.wHour, st_SysTimer.wMinute, st_SysTimer.wSecond);
		}
	}
	else if (ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_EXPIRED == pSt_AuthLocal->st_AuthRegInfo.enRegType)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_EXPIRED;
		return false;
	}
	else
	{
		//其他类型的注册机制
		BaseLib_Time_GetSysTime(&st_SysTimer);
		//计算超时时间
		if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_SECOND == pSt_AuthLocal->st_AuthRegInfo.enSerialType)
		{
			Authorize_IsErrorOccur = true;
			Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_NOTSUPPORT;
			return false;
		}
		else if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_DAY == pSt_AuthLocal->st_AuthRegInfo.enSerialType)
		{
			Authorize_IsErrorOccur = true;
			Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_NOTSUPPORT;
			return false;
		}
		else if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_TIME == pSt_AuthLocal->st_AuthRegInfo.enSerialType)
		{
			pSt_AuthLocal->st_AuthRegInfo.nHasTime--;
		}
		else if (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_CUSTOM == pSt_AuthLocal->st_AuthRegInfo.enSerialType)
		{
			_stxscanf(pSt_AuthLocal->st_AuthRegInfo.tszLeftTime, _X("%04d-%02d-%02d %02d:%02d:%02d"), &st_EndTimer.wYear, &st_EndTimer.wMonth, &st_EndTimer.wDay, &st_EndTimer.wHour, &st_EndTimer.wMinute, &st_EndTimer.wSecond);
			BaseLib_TimeSpan_GetForStu(&st_SysTimer, &st_EndTimer, &pSt_AuthLocal->st_AuthRegInfo.nHasTime, ENUM_XENGINE_BASELIB_TIME_TYPE_SECOND);
		}
		else
		{
			Authorize_IsErrorOccur = true;
			Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_NOTSUPPORT;
			return false;
		}
		if (pSt_AuthLocal->st_AuthRegInfo.nHasTime < 0)
		{
			Authorize_IsErrorOccur = true;
			Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_TIMEOUT;
			pSt_AuthLocal->st_AuthRegInfo.enRegType = ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_EXPIRED;
			_xstprintf(pSt_AuthLocal->st_AuthRegInfo.tszExpiryTime, _X("%04d-%02d-%02d %02d:%02d:%02d"), st_SysTimer.wYear, st_SysTimer.wMonth, st_SysTimer.wDay, st_SysTimer.wHour, st_SysTimer.wMinute, st_SysTimer.wSecond);
			return false;
		}
	}
	pSt_AuthLocal->st_AuthAppInfo.nExecTime++;
    return true;
}
/********************************************************************
函数名称：Authorize_CDKey_WriteTime
函数功能：记录一次执行时间
 参数.一：lpszFileKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的文件
 参数.二：nCount
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入最大允许记录个数,0不限制
返回值
  类型：逻辑型
  意思：是否成功
备注：记录次数越多,文件越大.读取需要的内存就越多
*********************************************************************/
bool CAuthorize_CDKey::Authorize_CDKey_WriteTime(LPCXSTR lpszFileKey, int nCount /* = 0 */)
{
    Authorize_IsErrorOccur = false;

    if (NULL == lpszFileKey)
    {
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
		return false;
    }
	//添加执行信息
	bool bFound = false;
	LPCXSTR lpszTimeStr = _X("[TimeList]");
	FILE* pSt_File = _xtfopen(lpszFileKey, _X("r+"));
	if (NULL == pSt_File)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_OPENFILE;
		return false;
	}
    XCHAR tszMsgBuffer[MAX_PATH];
	memset(tszMsgBuffer, '\0', MAX_PATH);
	//查找是否有时间列表字段
	while (1)
	{
		if (NULL == fgets(tszMsgBuffer, MAX_PATH, pSt_File))
		{
			break;
		}
		if (0 == _tcsxncmp(lpszTimeStr, tszMsgBuffer, _tcsxlen(lpszTimeStr)))
		{
			bFound = true;
			break;
		}
	}
	//设置文件指针
	if (bFound)
	{
		//如果设置了最大个数
		if (nCount > 0)
		{
			int nListCount = 0;
			XCHAR** pptszListTime = NULL;
			//读取现有的
			Authorize_CDKey_ReadTime(lpszFileKey, &pptszListTime, &nListCount);
			//是否大于14 >= 10
			if (nListCount >= nCount)
			{
                long lFSize = ftell(pSt_File);
                //需要截断文件
                fclose(pSt_File);
                int hFile = _xtopen(lpszFileKey, O_RDWR | O_CREAT, S_IREAD | S_IWRITE);
#ifdef _MSC_BUILD
                if (0 != _chsize_s(hFile, lFSize))
#else
                if (0 != ftruncate(hFile, lFSize))
#endif
                {
					Authorize_IsErrorOccur = true;
					Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_SETFILE;
					return false;
                }
                _close(hFile);
                //重新打开文件
                pSt_File = _xtfopen(lpszFileKey, _X("r+"));
                fseek(pSt_File, 0, SEEK_END);
				//从我们要保留的地方开始轮训
                for (int i = (nListCount - nCount + 1); i < nListCount; i++)
				{
                    fwrite(pptszListTime[i], 1, _tcsxlen(pptszListTime[i]), pSt_File);
                    fwrite(_X("|"), 1, 1, pSt_File);
				}
			}
			else
			{
				//在末尾添加
				fseek(pSt_File, -1, SEEK_END);
				XCHAR cChar = fgetc(pSt_File);
				if (cChar == '\n')
				{
					fseek(pSt_File, -2, SEEK_END);
				}
				else
				{
					fseek(pSt_File, 0, SEEK_END);
				}
			}
		}
        else
        {
			//在末尾添加
			fseek(pSt_File, -1, SEEK_END);
			XCHAR cChar = fgetc(pSt_File);
			if (cChar == '\n')
			{
				fseek(pSt_File, -2, SEEK_END);
			}
			else
			{
				fseek(pSt_File, 0, SEEK_END);
			}
        }
	}
	else
	{
		fseek(pSt_File, 0, SEEK_END);
		if (tszMsgBuffer[_tcsxlen(tszMsgBuffer) - 1] != '\n')
		{
			fwrite(_X("\n"), 1, 1, pSt_File);
		}
		fwrite(lpszTimeStr, 1, _tcsxlen(lpszTimeStr), pSt_File);
		fwrite(_X("\n"), 1, 1, pSt_File);
	}
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
#ifdef _MSC_BUILD
    _xstprintf(tszMsgBuffer, _X("%lld|"), time(NULL));
#else
    _xstprintf(tszMsgBuffer, _X("%ld|"), time(NULL));
#endif
	//追加
	fwrite(tszMsgBuffer, 1, _tcsxlen(tszMsgBuffer), pSt_File);
	fclose(pSt_File);
    return true;
}
/********************************************************************
函数名称：Authorize_CDKey_ReadTime
函数功能：读取记录的时间列表信息
 参数.一：lpszFileKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要读取的文件
 参数.二：ppptszTimeList
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出时间信息列表
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthorize_CDKey::Authorize_CDKey_ReadTime(LPCXSTR lpszFileKey, XCHAR*** ppptszTimeList, int* pInt_ListCount)
{
    Authorize_IsErrorOccur = false;

    if ((NULL == lpszFileKey) || (NULL == pInt_ListCount))
    {
        return false;
    }
	bool bFound = false;
	LPCXSTR lpszTimeStr = _X("[TimeList]");
	FILE* pSt_File = _xtfopen(lpszFileKey, _X("rb"));
	if (NULL == pSt_File)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_OPENFILE;
		return false;
	}
    XCHAR tszMsgBuffer[MAX_PATH];
	//查找是否有时间列表字段
	while (1)
	{
        memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		if (NULL == fgets(tszMsgBuffer, MAX_PATH, pSt_File))
		{
			break;
		}
		if (0 == _tcsxncmp(lpszTimeStr, tszMsgBuffer, _tcsxlen(lpszTimeStr)))
		{
			bFound = true;
			break;
		}
	}
	//设置文件指针
	if (!bFound)
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_TIMELIST;
		return false;
	}
    list<xstring> stl_ListTime;
    struct stat st_FileStat;
    stat(lpszFileKey, &st_FileStat);
    //申请足够的内存
    XCHAR* ptszMsgBuffer = (XCHAR*)malloc(st_FileStat.st_size);
    if (NULL == ptszMsgBuffer)
    {
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_MALLOC;
		return false;
    }
    memset(ptszMsgBuffer, '\0', st_FileStat.st_size);
    if (fread(ptszMsgBuffer, 1, st_FileStat.st_size, pSt_File) <= 0)
    {
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_READ;
		free(ptszMsgBuffer);
		ptszMsgBuffer = NULL;
		return false;
    }
    //申请内存
	XCHAR* ptszTokStr = _tcsxtok(ptszMsgBuffer, _X("|"));
	while (NULL != ptszTokStr)
	{
        if (_tcsxlen(ptszTokStr) > 4)
        {
            stl_ListTime.push_back(ptszTokStr);
        }
		ptszTokStr = _tcsxtok(NULL, _X("|"));
	}
	fclose(pSt_File);
    free(ptszMsgBuffer);
    ptszMsgBuffer = NULL;
    //导出
    *pInt_ListCount = (int)stl_ListTime.size();
    BaseLib_Memory_Malloc((XPPPMEM)ppptszTimeList, *pInt_ListCount, 64);
    list<xstring>::const_iterator stl_ListIterator = stl_ListTime.begin();
    for (int i = 0; stl_ListIterator != stl_ListTime.end(); stl_ListIterator++, i++)
    {
        _tcsxcpy((*ppptszTimeList)[i], stl_ListIterator->c_str());
    }
    stl_ListTime.clear();
    return true;
}