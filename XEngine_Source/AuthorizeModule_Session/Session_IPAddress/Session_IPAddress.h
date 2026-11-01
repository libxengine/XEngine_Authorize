#pragma once
/********************************************************************
//    Created:     2026-11-01 16:16:43
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_IPAddress\Session_IPAddress.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_IPAddress
//    File Base:   Session_IPAddress
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     IP会话管理器
//    History:
*********************************************************************/
typedef struct
{
	time_t nTimeStart;
}SESSION_IPADDRESS;


class CSession_IPAddress
{
public:
	CSession_IPAddress();
	~CSession_IPAddress();
public:
	bool Session_IPAddress_Init(CALLBACK_AUTHORIZE_MODULE_SESSION_IPADDR_EVENTS fpCall_IPAddrEvent, int nTimeout = 10, XPVOID lParam = NULL);
	bool Session_IPAddress_Get(LPCXSTR lpszIPAddr);
	bool Session_IPAddress_Insert(LPCXSTR lpszClientAddr);
	bool Session_IPAddress_Close(LPCXSTR lpszClientAddr);
	bool Session_IPAddress_Destroy();
protected:
	static XHTHREAD Session_IPAddress_Thread(XPVOID lParam);            //计时器线程
private:
	int m_nTimeout = 0;
	bool bIsRun = false;                          //运行标志
	std::unique_ptr<std::thread> pSTDThread;                 //时间计算器线程句柄
private:
	XPVOID m_lParam = NULL;
	CALLBACK_AUTHORIZE_MODULE_SESSION_IPADDR_EVENTS lpCall_IPAddrEvents = NULL;
private:
	std::shared_mutex st_Locker;
private:
	std::unordered_map<xstring, SESSION_IPADDRESS> stl_MapIPClient;
};
