#pragma once
/********************************************************************
//    Created:     2023/08/09  15:10:42
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_MultiLogin\AuthHelp_MultiLogin.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_MultiLogin
//    File Base:   AuthHelp_MultiLogin
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     多端登录处理代码
//    History:
*********************************************************************/

class CAuthHelp_MultiLogin
{
public:
	CAuthHelp_MultiLogin();
	~CAuthHelp_MultiLogin();
public:
	bool AuthHelp_MultiLogin_GetRange(int nClientDevice, int* pInt_IDType);
	bool AuthHelp_MultiLogin_TimeMatch(LPCXSTR lpszLoginTime);
protected:
private:
};