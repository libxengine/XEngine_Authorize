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
	bool bDeamon;                                           //守护进程,LINUX
	bool bTimeNotify;                                       //超时通知设置,真为一直通知,假为通知一次关闭客户端
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
		int nTokenTimeout;                                  //TOKEN登录超时时间
		int nDynamicTimeout;                                //动态码超时时间
		int nVerTime;                                       //验证时间
		int nVerMode;                                       //验证模式
		int nTryTime;                                       //验证时间
		int nTryMode;                                       //验证模式
	}st_XVerification;
	struct 
	{
		bool bHTTPAuth;                                     //是否开启HTTP授权登录,支持HTTP授权验证
		bool bPassAuth;                                     //是否启用三方认证
		int nHTTPAuthTime;                                  //HTTP验证超时时间,单位秒
		int nMultiMode;                                     //多端登录模式,0按照平台(PC,WEB,PAD,PHONE 4种),1按照类型(每种都可以)
		struct  
		{
			XCHAR tszPassLogin[MAX_PATH];                   //三方认证登录验证
			XCHAR tszPassLogout[MAX_PATH];                  //三方认证登出通知
			XCHAR tszPassTimeout[MAX_PATH];                 //三方认证超时通知
		}st_PassUrl;
		//多端登录支持的计时方式
		struct  
		{
			bool bMinute;
			bool bDay;
			bool bTime;
			bool bCustom;                                   
		}st_MulitLogin;
	}st_XLogin;
	struct  
	{
		bool bEnable;                                       //是否启用加密传输 
		int nPassword;                                      //密码
	}st_XCrypto;
	struct
	{ 
		XCHAR tszSQLite[MAX_PATH];                          //数据库文件位置
	}st_XSql;
	struct
	{
		XCHAR tszLogFile[MAX_PATH];                         //日志文件
		int nMaxSize;                                       //最大大小
		int nMaxCount;                                      //备份个数
		int nLogLeave;                                      //日志级别
	}st_XLog;                                 
	struct  
	{
		list<string> *pStl_ListVer;
	}st_XVer;
}XENGINE_SERVICECONFIG;
//功能开关
typedef struct
{
	bool bSwitchDelete;                                     //删除开关
	bool bSwitchRegister;                                   //注册开关
	bool bSwitchLogin;                                      //登录开关
	bool bSwitchPay;                                        //充值开关
	bool bSwitchPass;                                       //找回密码开关
	bool bSwitchTime;                                       //计时开关,关闭后客户端不会消耗时间(仅分钟卡有效)
	bool bSwitchCDKey;                                      //是否允许本地CDKEY创建使用
	bool bSwitchNotice;                                     //是否开启公告系统
	bool bSwitchDCode;                                      //动态验证码
	bool bSwitchMulti;                                      //多端登录开关
}XENGINE_FUNCTIONSWITCH;
//////////////////////////////////////////////////////////////////////////
//                              导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Config_GetLastError(int* pInt_SysError = NULL);
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
extern "C" bool ModuleConfigure_Json_File(LPCXSTR lpszConfigFile, XENGINE_SERVICECONFIG * pSt_ServerConfig);
/********************************************************************
函数名称：ModuleConfigure_Json_Switch
函数功能：功能开关配置文件
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
extern "C" bool ModuleConfigure_Json_Switch(LPCXSTR lpszConfigFile, XENGINE_FUNCTIONSWITCH * pSt_ServerConfig);