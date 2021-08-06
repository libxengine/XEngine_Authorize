#pragma once
/********************************************************************
//    Created:     2021/07/26  09:53:58
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Configure\Configure_ini\Configure_ini.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Configure\Configure_ini
//    File Base:   Configure_ini
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     配置文件类
//    History:
*********************************************************************/

class CConfigure_IniFile
{
public:
	CConfigure_IniFile();
	~CConfigure_IniFile();
public:
	BOOL Configure_IniFile_Read(LPCTSTR lpszFile, AUTHORIZE_CONFIGURE* pSt_AuthConfig);
	BOOL Configure_IniFile_Write(LPCTSTR lpszFile, AUTHORIZE_CONFIGURE* pSt_AuthConfig);
protected:
private:
};