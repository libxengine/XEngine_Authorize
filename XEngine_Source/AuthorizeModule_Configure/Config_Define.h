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
	int nHTTPPort;                                          //HTTP管理端口
	BOOL bDeamon;                                           //守护进程,LINUX
	BOOL bTimeNotify;                                       //超时通知设置,真为一直通知,假为通知一次关闭客户端
	struct
	{
		int nMaxClient;                                     //最大客户端个数
		int nMaxQueue;                                      //最大队列深度
		int nIOThread;                                      //网络IO线程
		int nTCPThread;                                     //TCP业务线程个数
		int nWSThread;                                      //WEBSOCKET业务线程个数
		int nHTTPThread;                                    //HTTP业务线程个数
	}st_XMax;
	struct   
	{
		int nUserTimeout;                                   //用户登录超时时间
		int nVerTime;                                       //验证时间
		int nVerMode;                                       //验证模式
		int nTryTime;                                       //验证时间
		int nTryMode;                                       //验证模式
	}st_XVerification;
	struct  
	{
		BOOL bEnable;                                       //是否启用加密传输 
		int nPassword;                                      //密码
	}st_XCrypto;
	struct
	{ 
		TCHAR tszSQLite[MAX_PATH];                          //数据库文件位置
	}st_XSql;
	struct
	{
		TCHAR tszLogFile[MAX_PATH];                         //日志文件
		int nMaxSize;                                       //最大大小
		int nMaxCount;                                      //备份个数
		int nLogLeave;                                      //日志级别
	}st_XLog;
	struct  
	{
		list<string> *pStl_ListVer;
	}st_XVer;
}XENGINE_SERVICECONFIG;
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
extern "C" BOOL ModuleConfigure_Json_File(LPCTSTR lpszConfigFile, XENGINE_SERVICECONFIG * pSt_ServerConfig);