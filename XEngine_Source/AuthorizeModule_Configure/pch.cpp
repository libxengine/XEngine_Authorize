#include "pch.h"
#include "ModuleConfigure_Json/ModuleConfigure_Json.h"
/********************************************************************
//    Created:     2021/07/26  10:20:03
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Configure\pch.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Configure
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
BOOL Config_IsErrorOccur = FALSE;
DWORD Config_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleConfigure_Json m_Config;
//////////////////////////////////////////////////////////////////////////
//                      导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Config_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Config_dwErrorCode;
}
/************************************************************************/
/*                              配置文件导出函数                        */
/************************************************************************/
extern "C" BOOL ModuleConfigure_Json_File(LPCTSTR lpszConfigFile, XENGINE_SERVICECONFIG * pSt_ServerConfig)
{
	return m_Config.ModuleConfigure_Json_File(lpszConfigFile, pSt_ServerConfig);
}
extern "C" BOOL ModuleConfigure_Json_Switch(LPCTSTR lpszConfigFile, XENGINE_FUNCTIONSWITCH * pSt_ServerConfig)
{
	return m_Config.ModuleConfigure_Json_Switch(lpszConfigFile, pSt_ServerConfig);
}