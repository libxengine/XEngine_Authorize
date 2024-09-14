#include "Authorize_Hdr.h"

bool bIsRun = false;
bool bIsTest = false;

XHANDLE xhLog = NULL;
XHANDLE xhTCPSocket = NULL;
XHANDLE xhWSSocket = NULL;
XHANDLE xhHttpSocket = NULL;

XHANDLE xhTCPPacket = NULL;
XHANDLE xhWSPacket = NULL;
XHANDLE xhHttpPacket = NULL;

XHANDLE xhTCPHeart = NULL;
XHANDLE xhWSHeart = NULL;
XHANDLE xhHTTPHeart = NULL;

XHANDLE xhMemPool = NULL;
XHANDLE xhTCPPool = NULL;
XHANDLE xhWSPool = NULL;
XHANDLE xhHttpPool = NULL;

XENGINE_SERVICECONFIG st_AuthConfig;
XENGINE_FUNCTIONSWITCH st_FunSwitch;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("网络验证服务器退出..."));
		bIsRun = false;

		HelpComponents_Datas_Destory(xhTCPPacket);
		RfcComponents_WSPacket_DestoryEx(xhWSPacket);
		HttpProtocol_Server_DestroyEx(xhHttpPacket);

		NetCore_TCPXCore_DestroyEx(xhTCPSocket);
		NetCore_TCPXCore_DestroyEx(xhWSSocket);
		NetCore_TCPXCore_DestroyEx(xhHttpSocket);

		SocketOpt_HeartBeat_DestoryEx(xhTCPHeart);
		SocketOpt_HeartBeat_DestoryEx(xhWSHeart);
		SocketOpt_HeartBeat_DestoryEx(xhHTTPHeart);

		ManagePool_Thread_NQDestroy(xhTCPPool);
		ManagePool_Thread_NQDestroy(xhWSPool);
		ManagePool_Thread_NQDestroy(xhHttpPool);
		ManagePool_Memory_Destory(xhMemPool);

		HelpComponents_XLog_Destroy(xhLog);
		Session_Authorize_Destroy();
		Session_Token_Destroy();
		AuthHelp_DynamicCode_Destory();
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_Destroy();//销毁DB数据库服务
		}
		else
		{
			DBModule_MySQL_Destroy();// 销毁MYsql数据库服务
		}
		exit(0);
	}
}
static int ServiceApp_Deamon(int wait)
{
#ifndef _WINDOWS
	pid_t pid = 0;
	int status;
	pid = fork();
	if (pid > 0)
	{
		exit(0);
	}

	close(2);
	while (1)
	{

		pid = fork();
		if (pid < 0)
			exit(1);
		if (pid == 0)
		{
			return 0;
		}
		waitpid(pid, &status, 0);

		if (wait > 0)
			sleep(wait);
	}
#endif
	return 0;
}

int main(int argc, char** argv)
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	bIsRun = true;
	int nRet = -1;
	FILE* pSt_File = NULL;
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;
	THREADPOOL_PARAMENT** ppSt_ListTCPThread;
	THREADPOOL_PARAMENT** ppSt_ListWSThread;
	THREADPOOL_PARAMENT** ppSt_ListHttpThread;
	LPCXSTR lpszHTTPMime = _X("./XEngine_Config/HttpMime.types");
	LPCXSTR lpszHTTPCode = _X("./XEngine_Config/HttpCode.types");

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_AuthConfig, '\0', sizeof(XENGINE_SERVICECONFIG));
	memset(&st_FunSwitch, '\0', sizeof(XENGINE_FUNCTIONSWITCH));

	if (!Authorize_Service_Parament(argc, argv))
	{
		return -1;
	}
	st_XLogConfig.XLog_MaxBackupFile = st_AuthConfig.st_XLog.nMaxCount;
	st_XLogConfig.XLog_MaxSize = st_AuthConfig.st_XLog.nMaxSize;
	_tcsxcpy(st_XLogConfig.tszFileName, st_AuthConfig.st_XLog.tszLogFile);

	xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_FILE | HELPCOMPONENTS_XLOG_OUTTYPE_STD, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("启动服务器失败，启动日志失败，错误：%lX", XLog_GetLastError());
		goto XENGINE_EXITAPP;
	}
	HelpComponents_XLog_SetLogPriority(xhLog, st_AuthConfig.st_XLog.nLogLeave);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化日志系统成功,日志文件:%s"), st_AuthConfig.st_XLog.tszLogFile);

	if (st_AuthConfig.bDeamon)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，使用守护进程启动服务..."));
		ServiceApp_Deamon(1);
	}

	signal(SIGINT, ServiceApp_Stop);
	signal(SIGTERM, ServiceApp_Stop);
	signal(SIGABRT, ServiceApp_Stop);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化服务器信号管理成功"));

	xhMemPool = ManagePool_Memory_Create();
	if (NULL == xhMemPool)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化内存池失败，错误：%lX"), ManagePool_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化内存池成功"));

	/***********  初始化 MySql/DB数据库  ***********/
	if (0 == st_AuthConfig.st_XSql.nDBType)
	{ //SQLite数据库
		if (!DBModule_SQLite_Init(st_AuthConfig.st_XSql.st_SQLite.tszSQLite))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化DB数据库服务失败，错误：%lX"), DBModule_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化DB数据库服务成功,数据库:%s"), st_AuthConfig.st_XSql.st_SQLite.tszSQLite);
	}
	else
	{
		if (!DBModule_MySQL_Init((DATABASE_MYSQL_CONNECTINFO *)&st_AuthConfig.st_XSql.st_MYSQL))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化MySql数据库失败，错误：%lX"), DBModule_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化MySql数据库服务成功,数据库:%s"), st_AuthConfig.st_XSql.st_MYSQL.tszDBName);
	}
	if (!Session_Authorize_Init(XEngine_TaskEvent_Client))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化会话客户端服务失败，错误：%lX"), Session_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化会话客户端服务成功"));
	if (!Session_Token_Init(st_AuthConfig.st_XVerification.nTokenTimeout, XEngine_TaskEvent_Token))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化会话TOKEN服务失败，错误：%lX"), Session_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化会话TOKEN服务成功"));

	if (!AuthHelp_DynamicCode_Init(st_AuthConfig.st_XVerification.nDynamicTimeout))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化动态验证码服务失败，错误：%lX"), AuthHelp_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化动态验证码服务成功,超时时间:%d"), st_AuthConfig.st_XVerification.nDynamicTimeout);
	//是否开启TCP服务
	if (st_AuthConfig.nTCPPort > 0)
	{
		xhTCPPacket = HelpComponents_Datas_Init(st_AuthConfig.st_XMax.nMaxQueue, st_AuthConfig.st_XMax.nTCPThread);
		if (NULL == xhTCPPacket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化TCP组包器失败，错误：%lX"), Packets_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化TCP组包器成功,任务池个数:%d"), st_AuthConfig.st_XMax.nTCPThread);

		xhTCPSocket = NetCore_TCPXCore_StartEx(st_AuthConfig.nTCPPort, st_AuthConfig.st_XMax.nMaxClient, st_AuthConfig.st_XMax.nIOThread);
		if (NULL == xhTCPSocket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化TCP验证网络服务失败，错误：%lX"), NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化TCP验证网络服务成功,端口:%d,网络池个数:%d"), st_AuthConfig.nTCPPort, st_AuthConfig.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhTCPSocket, XEngine_Client_TCPAccept, XEngine_Client_TCPRecv, XEngine_Client_TCPClose);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化TCP验证网络事件成功"));


		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListTCPThread, st_AuthConfig.st_XMax.nTCPThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_AuthConfig.st_XMax.nTCPThread; i++)
		{
			int* pInt_Index = new int;
			*pInt_Index = i;
			ppSt_ListTCPThread[i]->lParam = pInt_Index;
			ppSt_ListTCPThread[i]->fpCall_ThreadsTask = XEngine_AuthService_TCPThread;
		}
		xhTCPPool = ManagePool_Thread_NQCreate(&ppSt_ListTCPThread, st_AuthConfig.st_XMax.nTCPThread);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化TCP任务线程池成功,线程个数:%d"), st_AuthConfig.st_XMax.nTCPThread);
	}
	//是否开启WEBSOCEKT服务
	if (st_AuthConfig.nWSPort > 0)
	{
		xhWSPacket = RfcComponents_WSPacket_InitEx(st_AuthConfig.st_XMax.nWSThread, st_AuthConfig.st_XMax.nMaxQueue);
		if (NULL == xhWSPacket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化WebSocket组包器失败，错误：%lX"), WSFrame_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化WebSocket组包器成功,任务池个数:%d"), st_AuthConfig.st_XMax.nWSThread);

		xhWSSocket = NetCore_TCPXCore_StartEx(st_AuthConfig.nWSPort, st_AuthConfig.st_XMax.nMaxQueue, st_AuthConfig.st_XMax.nIOThread);
		if (NULL == xhWSSocket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化WEBSOCKET验证网络服务失败，错误：%lX"), NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化WEBSOCKET验证网络服务成功,端口:%d,网络池个数:%d"), st_AuthConfig.nWSPort, st_AuthConfig.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhWSSocket, XEngine_Client_WSAccept, XEngine_Client_WSRecv, XEngine_Client_WSClose);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化WEBSOCKET验证网络事件成功"));

		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListWSThread, st_AuthConfig.st_XMax.nWSThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_AuthConfig.st_XMax.nWSThread; i++)
		{
			int* pInt_Index = new int;
			*pInt_Index = i;
			ppSt_ListWSThread[i]->lParam = pInt_Index;
			ppSt_ListWSThread[i]->fpCall_ThreadsTask = XEngine_AuthService_WSThread;
		}
		xhWSPool = ManagePool_Thread_NQCreate(&ppSt_ListWSThread, st_AuthConfig.st_XMax.nWSThread);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化WEBSOCKET任务线程池成功,线程个数:%d"), st_AuthConfig.st_XMax.nWSThread);
	}
	//是否开启HTTP服务
	if (st_AuthConfig.nHTTPPort > 0)
	{
		xhHttpPacket = HttpProtocol_Server_InitEx(lpszHTTPCode, lpszHTTPMime, st_AuthConfig.st_XMax.nHTTPThread);
		if (NULL == xhHttpPacket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化HTTP组包器失败，错误：%lX"), HttpProtocol_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化HTTP组包器成功,任务池个数:%d"), st_AuthConfig.st_XMax.nHTTPThread);

		xhHttpSocket = NetCore_TCPXCore_StartEx(st_AuthConfig.nHTTPPort, st_AuthConfig.st_XMax.nMaxQueue, st_AuthConfig.st_XMax.nIOThread);
		if (NULL == xhHttpSocket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化HTTP管理网络服务失败，错误：%lX"), NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化HTTP管理网络服务成功,端口:%d,网络池个数:%d"), st_AuthConfig.nHTTPPort, st_AuthConfig.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhHttpSocket, XEngine_Client_HttpAccept, XEngine_Client_HttpRecv, XEngine_Client_HttpClose);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化HTTP管理网络事件成功"));

		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListHttpThread, st_AuthConfig.st_XMax.nHTTPThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_AuthConfig.st_XMax.nHTTPThread; i++)
		{
			int* pInt_Index = new int;
			*pInt_Index = i;
			ppSt_ListHttpThread[i]->lParam = pInt_Index;
			ppSt_ListHttpThread[i]->fpCall_ThreadsTask = XEngine_AuthService_HttpThread;
		}
		xhHttpPool = ManagePool_Thread_NQCreate(&ppSt_ListHttpThread, st_AuthConfig.st_XMax.nHTTPThread);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化HTTP任务线程池成功,线程个数:%d"), st_AuthConfig.st_XMax.nHTTPThread);
	}

	if (st_AuthConfig.st_XCrypto.bEnable)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，加密传输设置为开启,密码:%d"), st_AuthConfig.st_XCrypto.nPassword);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，加密传输设置为关闭,采用明文传输"));
	}
	//发送信息报告
	if (st_AuthConfig.st_XReport.bEnable)
	{
		if (InfoReport_APIMachine_Send(st_AuthConfig.st_XReport.tszAPIUrl, st_AuthConfig.st_XReport.tszServiceName))
		{
			__int64x nTimeCount = 0;
			if (InfoReport_APIMachine_GetTime(st_AuthConfig.st_XReport.tszAPIUrl, st_AuthConfig.st_XReport.tszServiceName, &nTimeCount))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动信息报告给API服务器:%s 成功,报告次数:%lld"), st_AuthConfig.st_XReport.tszAPIUrl, nTimeCount);
			}
			else
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动信息报告给API服务器:%s 成功,获取报告次数失败,错误:%lX"), st_AuthConfig.st_XReport.tszAPIUrl, InfoReport_GetLastError());
			}
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动信息报告给API服务器:%s 失败，错误：%lX"), st_AuthConfig.st_XReport.tszAPIUrl, InfoReport_GetLastError());
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，信息报告给API服务器没有启用"));
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，功能开关选项,删除功能:%d,登录功能:%d,找回密码:%d,充值功能:%d,注册功能:%d,CDKey功能:%d,公告系统:%d,动态验证:%d,多端登录:%d,临时试用:%d,黑名单功能:%d,普通TOKEN:%d"), st_FunSwitch.bSwitchDelete, st_FunSwitch.bSwitchLogin, st_FunSwitch.bSwitchPass, st_FunSwitch.bSwitchPay, st_FunSwitch.bSwitchRegister, st_FunSwitch.bSwitchCDKey, st_FunSwitch.bSwitchNotice, st_FunSwitch.bSwitchDCode, st_FunSwitch.bSwitchMulti, st_FunSwitch.bSwitchTry, st_FunSwitch.bSwitchBanned, st_FunSwitch.bSwitchTokenLogin);

	pSt_File = _xtfopen(st_AuthConfig.st_XVerification.st_XCDKey.tszKeyFile, _X("rb"));
	if (NULL == pSt_File)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，授权文件验证失败，授权文件没有找到"));
	}
	else
	{
		//一个简单的示例,没有验证硬件码
		XCHAR tszENCodecBuffer[4096] = {};
		XCHAR tszDECodecBuffer[4096] = {};
		XENGINE_AUTHORIZE_LOCAL st_AuthLocal = {};

		int nRet = fread(tszENCodecBuffer, 1, sizeof(tszENCodecBuffer), pSt_File);
		fclose(pSt_File);

		if (OPenSsl_XCrypto_Decoder(tszENCodecBuffer, &nRet, tszDECodecBuffer, st_AuthConfig.st_XVerification.st_XCDKey.tszKeyPass))
		{
			Authorize_CDKey_ReadMemory(tszDECodecBuffer, nRet, &st_AuthLocal);
			bool bRet = Authorize_CDKey_GetLeftTimer(&st_AuthLocal);
			//无论成功失败需要重写CDKEY
			memset(tszENCodecBuffer, '\0', sizeof(tszENCodecBuffer));
			memset(tszDECodecBuffer, '\0', sizeof(tszDECodecBuffer));
			Authorize_CDKey_WriteMemory(tszDECodecBuffer, &nRet, &st_AuthLocal);
			OPenSsl_XCrypto_Encoder(tszDECodecBuffer, &nRet, (XBYTE*)tszENCodecBuffer, st_AuthConfig.st_XVerification.st_XCDKey.tszKeyPass);
			pSt_File = _xtfopen(st_AuthConfig.st_XVerification.st_XCDKey.tszKeyFile, _X("wb"));
			fwrite(tszENCodecBuffer, 1, nRet, pSt_File);
			fclose(pSt_File);
			if (bRet)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，授权文件验证成功，总可运行次数:%s,剩余可运行次数:%lld"), st_AuthLocal.st_AuthRegInfo.tszLeftTime, st_AuthLocal.st_AuthRegInfo.nHasTime);
			}
			else
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，授权文件验证失败，总可运行次数:%s,剩余可运行次数:%lld，错误码:%lX"), st_AuthLocal.st_AuthRegInfo.tszLeftTime, st_AuthLocal.st_AuthRegInfo.nHasTime, Authorize_GetLastError());
			}
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，授权文件失败，解密失败,数据不正确"));
		}
	}
	
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("所有服务成功启动，网络验证服务运行中,XEngien版本:%s%s,发行版本次数:%d,当前运行版本：%s。。。"), BaseLib_OperatorVer_XNumberStr(), BaseLib_OperatorVer_XTypeStr(), st_AuthConfig.st_XVer.pStl_ListVer->size(), st_AuthConfig.st_XVer.pStl_ListVer->front().c_str());

	while (true)
	{
		if (bIsTest)
		{
			nRet = 0;
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

XENGINE_EXITAPP:

	if (bIsRun)
	{
		if (bIsTest && 0 == nRet)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("服务启动完毕，测试程序退出..."));
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("服务启动失败，服务器退出..."));
		}

		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动失败,按任意键网络验证服务器退出..."));
		bIsRun = false;

		HelpComponents_Datas_Destory(xhTCPPacket);
		RfcComponents_WSPacket_DestoryEx(xhWSPacket);
		HttpProtocol_Server_DestroyEx(xhHttpPacket);

		NetCore_TCPXCore_DestroyEx(xhTCPSocket);
		NetCore_TCPXCore_DestroyEx(xhWSSocket);
		NetCore_TCPXCore_DestroyEx(xhHttpSocket);

		SocketOpt_HeartBeat_DestoryEx(xhTCPHeart);
		SocketOpt_HeartBeat_DestoryEx(xhWSHeart);
		SocketOpt_HeartBeat_DestoryEx(xhHTTPHeart);

		ManagePool_Thread_NQDestroy(xhTCPPool);
		ManagePool_Thread_NQDestroy(xhWSPool);
		ManagePool_Thread_NQDestroy(xhHttpPool);
		ManagePool_Memory_Destory(xhMemPool);

		HelpComponents_XLog_Destroy(xhLog);
		Session_Authorize_Destroy();
		Session_Token_Destroy();
		AuthHelp_DynamicCode_Destory();
		if (0 == st_AuthConfig.st_XSql.nDBType)
		{
			DBModule_SQLite_Destroy();
		}
		else
		{
			DBModule_MySQL_Destroy();
		}
	}
#ifdef _WINDOWS
	WSACleanup();
#endif
	return nRet;
}
