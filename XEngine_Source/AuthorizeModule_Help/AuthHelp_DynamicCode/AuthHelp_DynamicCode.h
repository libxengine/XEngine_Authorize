#pragma once
/********************************************************************
//    Created:     2023/04/13  10:50:29
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_DynamicCode\AuthHelp_DynamicCode.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_DynamicCode
//    File Base:   AuthHelp_DynamicCode
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     动态码
//    History:
*********************************************************************/
typedef struct  
{
	XNETHANDLE xhToken;
	XSHOT nDynamicCode;
	time_t nTimeStart;
}AUTHHELP_DYNAMICCODE;

class CAuthHelp_DynamicCode
{
public:
	CAuthHelp_DynamicCode();
	~CAuthHelp_DynamicCode();
public:
	bool AuthHelp_DynamicCode_Init(int nSecond);
	bool AuthHelp_DynamicCode_Destory();
	bool AuthHelp_DynamicCode_Create(XNETHANDLE* pxhToken, XSHOT* pInt_DynamicCode);
	bool AuthHelp_DynamicCode_Get(XNETHANDLE xhToken, int nDynamicCode);
protected:
	static XHTHREAD CALLBACK AuthHelp_DynamicCode_Thread(XPVOID lParam);
private:
	int m_nSescond = 0;
	bool m_bRun = false;
private:
	shared_mutex st_Locker;
	shared_ptr<thread> pSTDThread;
private:
	unordered_map<XNETHANDLE, AUTHHELP_DYNAMICCODE> stl_MapDynamicCode;
};