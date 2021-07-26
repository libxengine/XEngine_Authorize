#include "pch.h"
#include "Configure_ini/Configure_ini.h"
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
CConfigure_IniFile m_Config;
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
extern "C" BOOL Configure_IniFile_Read(LPCTSTR lpszFile, AUTHORIZE_CONFIGURE * pSt_AuthConfig)
{
	return m_Config.Configure_IniFile_Read(lpszFile, pSt_AuthConfig);
}
extern "C" BOOL Configure_IniFile_Write(LPCTSTR lpszFile, AUTHORIZE_CONFIGURE * pSt_AuthConfig)
{
	return m_Config.Configure_IniFile_Write(lpszFile, pSt_AuthConfig);
}