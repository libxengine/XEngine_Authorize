#pragma once
/********************************************************************
//    Created:     2021/07/26  09:55:11
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Configure\Config_Define.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Configure
//    File Base:   Config_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出的函数
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                              导出的数据结构
//////////////////////////////////////////////////////////////////////////
typedef struct  
{
	int nTCPPort;                                           //TCP端口
	int nWSPort;                                            //WEBSOCKET端口
	int nThreads;                                           //启动的线程个数
	int nVerTimeout;                                        //用户验证超时时间
	BOOL bAutoStart;                                        //自启动
	BOOL bDeamon;                                           //守护进程,LINUX
	BOOL bTimeNotify;                                       //超时通知设置,真为一直通知,假为通知一次关闭客户端
	struct   
	{
		int nVerTime;                                       //验证时间
		int nVerMode;                                       //验证模式
		int nTryTime;                                       //验证时间
		int nTryMode;                                       //验证模式
	}st_Verification;
	struct  
	{
		BOOL bEnable;                                       //是否启用加密传输 
		int nPassword;                                      //密码
	}st_Crypto;
	struct  
	{
		BOOL bSmtpEnable;                                   //是否启用SMTP
		TCHAR tszSmtpAddr[MAX_PATH];                        //SMTP服务器
		TCHAR tszSmtpFrom[MAX_PATH];                        //回复地址
		TCHAR tszSmtpUser[MAX_PATH];                        //用户名
		TCHAR tszSmtpPass[MAX_PATH];                        //密码
	}st_EMail;
	struct
	{
		TCHAR tszSQLite[MAX_PATH];                          
	}st_XSql;
	struct
	{
		TCHAR tszLogFile[MAX_PATH];
		int nMaxSize;
		int nMaxCount;
		int nLogLeave;
	}st_XLog;
	struct  
	{
		TCHAR tszVersion[MAX_PATH];
	}st_XVer;
}AUTHORIZE_CONFIGURE;
//////////////////////////////////////////////////////////////////////////
//                              导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Config_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                              配置文件导出函数                        */
/************************************************************************/
/********************************************************************
函数名称：Configure_IniFile_Read
函数功能：读取配置文件信息
 参数.一：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入配置文件位置
 参数.二：pSt_AuthConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出读取到的配置信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Configure_IniFile_Read(LPCTSTR lpszFile, AUTHORIZE_CONFIGURE* pSt_AuthConfig);
/********************************************************************
函数名称：Configure_IniFile_Write
函数功能：写入配置文件信息
 参数.一：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入配置文件位置
 参数.二：pSt_AuthConfig
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入写入的配置信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Configure_IniFile_Write(LPCTSTR lpszFile, AUTHORIZE_CONFIGURE* pSt_AuthConfig);