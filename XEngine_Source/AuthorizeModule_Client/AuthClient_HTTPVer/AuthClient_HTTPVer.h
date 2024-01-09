#pragma once
/********************************************************************
//    Created:     2024/01/04  09:42:55
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client\AuthClient_HTTPVer\AuthClient_HTTPVer.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client\AuthClient_HTTPVer
//    File Base:   AuthClient_HTTPVer
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     临时授权客户端
//    History:
*********************************************************************/

class CAuthClient_HTTPVer
{
public:
	CAuthClient_HTTPVer();
	~CAuthClient_HTTPVer();
public:
	bool AuthClient_HTTPVer_TryRequest(LPCXSTR lpszURLAddr);
protected:
private:
};