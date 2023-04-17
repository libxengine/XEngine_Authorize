#pragma once
/********************************************************************
//    Created:     2021/12/02  16:08:16
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure\ModuleConfigure_Json\ModuleConfigure_Json.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure\ModuleConfigure_Json
//    File Base:   ModuleConfigure_Json
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JSON配置文件读写
//    History:
*********************************************************************/

class CModuleConfigure_Json
{
public:
	CModuleConfigure_Json();
	~CModuleConfigure_Json();
public:
	bool ModuleConfigure_Json_File(LPCXSTR lpszConfigFile, XENGINE_SERVICECONFIG* pSt_ServerConfig);
	bool ModuleConfigure_Json_Switch(LPCXSTR lpszConfigFile, XENGINE_FUNCTIONSWITCH* pSt_ServerConfig);
protected:
private:
};