#pragma once
/********************************************************************
//    Created:     2023/11/15  10:34:36
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client\AuthClient_Connector\AuthClient_Connector.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client\AuthClient_Connector
//    File Base:   AuthClient_Connector
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     验证客户端示例模块
//    History:
*********************************************************************/
typedef struct  
{
	XNETHANDLE xhToken;
	int nDynamicCode;
	time_t nTimeStart;
}AUTHHELP_DYNAMICCODE;

class CAuthClient_Connector
{
public:
	CAuthClient_Connector();
	~CAuthClient_Connector();
public:
	bool AuthClient_Connector_Connect(LPCXSTR lpszClientAddr, int nPort, LPCXSTR lpszPass = NULL);
	bool AuthClient_Connector_Close();
	bool AuthClient_Connector_GetAuth(bool* pbAuth = NULL);
	bool AuthClient_Connector_Login(LPCXSTR lpszUser, LPCXSTR lpszPass, int nDYCode = 0, XNETHANDLE xhToken = 0);
	bool AuthClient_Connector_Heart(bool bEnable = true);
protected:
	static XHTHREAD CALLBACK AuthClient_Connector_Thread(XPVOID lParam);
private:
	bool m_bRun = false;
	bool m_bLogin = false;
	bool m_bAuth = false;
	bool m_bHeart = false;
	XSOCKET m_hSocket = 0;
	XCHAR tszPassStr[128] = {};
private:
	shared_ptr<thread> pSTDThread;
};