#include "Authorize_Hdr.h"

BOOL bIsRun = FALSE;
XLOG xhLog = NULL;
XHANDLE xhTCPSocket = NULL;
XHANDLE xhWSSocket = NULL;
XHANDLE xhHttpSocket = NULL;
XHANDLE xhTCPPacket = NULL;
XHANDLE xhWSPacket = NULL;
XHANDLE xhHttpPacket = NULL;
XNETHANDLE xhTCPPool = 0;
XNETHANDLE xhWSPool = 0;
XNETHANDLE xhHttpPool = 0;
XENGINE_SERVICECONFIG st_AuthConfig;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("网络验证服务器退出..."));
		bIsRun = FALSE;

		int nListCount = 0;
		AUTHSESSION_NETCLIENT** ppSt_ListClient;
		Session_Authorize_GetClient(&ppSt_ListClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			AUTHREG_PROTOCOL_TIME st_TimeProtocol;
			memset(&st_TimeProtocol, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

			if (Session_Authorize_GetTimer(ppSt_ListClient[i]->st_UserTable.st_UserInfo.tszUserName, &st_TimeProtocol))
			{
				Database_SQLite_UserLeave(&st_TimeProtocol);
			}
			Session_Authorize_CloseClient(ppSt_ListClient[i]->st_UserTable.st_UserInfo.tszUserName);
		}
		HelpComponents_Datas_Destory(xhTCPPacket);
		RfcComponents_WSPacket_DestoryEx(xhWSPacket);
		RfcComponents_HttpServer_DestroyEx(xhHttpPacket);

		NetCore_TCPXCore_DestroyEx(xhTCPSocket);
		NetCore_TCPXCore_DestroyEx(xhWSSocket);
		NetCore_TCPXCore_DestroyEx(xhHttpSocket);

		ManagePool_Thread_NQDestroy(xhTCPPool);
		ManagePool_Thread_NQDestroy(xhWSPool);
		ManagePool_Thread_NQDestroy(xhHttpPool);

		HelpComponents_XLog_Destroy(xhLog);
		Session_Authorize_Destroy();
		Session_Token_Destroy();
		Database_SQLite_Destroy();
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
	bIsRun = TRUE;
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;
	THREADPOOL_PARAMENT** ppSt_ListTCPThread;
	THREADPOOL_PARAMENT** ppSt_ListWSThread;
	THREADPOOL_PARAMENT** ppSt_ListHttpThread;
	LPCTSTR lpszHTTPMime = _T("./XEngine_Config/HttpMime.types");
	LPCTSTR lpszHTTPCode = _T("./XEngine_Config/HttpCode.types");

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_AuthConfig, '\0', sizeof(XENGINE_SERVICECONFIG));

	if (!Authorize_Service_Parament(argc, argv))
	{
		return -1;
	}
	st_XLogConfig.XLog_MaxBackupFile = st_AuthConfig.st_XLog.nMaxCount;
	st_XLogConfig.XLog_MaxSize = st_AuthConfig.st_XLog.nMaxSize;
	_tcscpy(st_XLogConfig.tszFileName, st_AuthConfig.st_XLog.tszLogFile);

	xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_FILE | HELPCOMPONENTS_XLOG_OUTTYPE_STD, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("启动服务器失败，启动日志失败，错误：%lX", XLog_GetLastError());
		goto XENGINE_EXITAPP;
	}
	HelpComponents_XLog_SetLogPriority(xhLog, st_AuthConfig.st_XLog.nLogLeave);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化日志系统成功,日志文件:%s"), st_AuthConfig.st_XLog.tszLogFile);

	if (st_AuthConfig.bDeamon)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，使用守护进程启动服务..."));
		ServiceApp_Deamon(1);
	}

	signal(SIGINT, ServiceApp_Stop);
	signal(SIGTERM, ServiceApp_Stop);
	signal(SIGABRT, ServiceApp_Stop);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化服务器信号管理成功"));

	if (!Database_SQLite_Init(st_AuthConfig.st_XSql.tszSQLite))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化数据库服务失败，错误：%lX"), DBModule_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化数据库服务成功,数据库:%s"), st_AuthConfig.st_XSql.tszSQLite);

	if (!Session_Authorize_Init(XEngine_TaskEvent_Client))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化会话客户端服务失败，错误：%lX"), Session_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化会话客户端服务成功"));
	if (!Session_Token_Init(st_AuthConfig.st_XVerification.nTokenTimeout, XEngine_TaskEvent_Token))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化会话TOKEN服务失败，错误：%lX"), Session_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化会话TOKEN服务成功"));
	
	xhTCPPacket = HelpComponents_Datas_Init(st_AuthConfig.st_XMax.nMaxQueue, st_AuthConfig.st_XMax.nTCPThread);
	if (NULL == xhTCPPacket)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化TCP组包器失败，错误：%lX"), Packets_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化TCP组包器成功,任务池个数:%d"), st_AuthConfig.st_XMax.nTCPThread);
	xhWSPacket = RfcComponents_WSPacket_InitEx(st_AuthConfig.st_XMax.nWSThread, st_AuthConfig.st_XMax.nMaxQueue);
	if (NULL == xhWSPacket)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化WebSocket组包器失败，错误：%lX"), WSFrame_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化WebSocket组包器成功,任务池个数:%d"), st_AuthConfig.st_XMax.nWSThread);
	xhHttpPacket = RfcComponents_HttpServer_InitEx(lpszHTTPCode, lpszHTTPMime, st_AuthConfig.st_XMax.nHTTPThread);
	if (NULL == xhHttpPacket)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化HTTP组包器失败，错误：%lX"), HttpServer_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化HTTP组包器成功,任务池个数:%d"), st_AuthConfig.st_XMax.nHTTPThread);

	xhTCPSocket = NetCore_TCPXCore_StartEx(st_AuthConfig.nTCPPort, st_AuthConfig.st_XMax.nMaxClient, st_AuthConfig.st_XMax.nIOThread);
	if (NULL == xhTCPSocket)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动TCP验证网络服务失败，错误：%lX"), NetCore_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化TCP验证网络服务成功,端口:%d,网络池个数:%d"), st_AuthConfig.nTCPPort, st_AuthConfig.st_XMax.nIOThread);
	NetCore_TCPXCore_RegisterCallBackEx(xhTCPSocket, XEngine_Client_TCPAccept, XEngine_Client_TCPRecv, XEngine_Client_TCPClose);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化TCP验证网络事件成功"));

	xhWSSocket = NetCore_TCPXCore_StartEx(st_AuthConfig.nWSPort, st_AuthConfig.st_XMax.nMaxQueue, st_AuthConfig.st_XMax.nIOThread);
	if (NULL == xhWSSocket)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动WEBSOCKET验证网络服务失败，错误：%lX"), NetCore_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化WEBSOCKET验证网络服务成功,端口:%d,网络池个数:%d"), st_AuthConfig.nWSPort, st_AuthConfig.st_XMax.nIOThread);
	NetCore_TCPXCore_RegisterCallBackEx(xhWSSocket, XEngine_Client_WSAccept, XEngine_Client_WSRecv, XEngine_Client_WSClose);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化WEBSOCKET验证网络事件成功"));

	xhHttpSocket = NetCore_TCPXCore_StartEx(st_AuthConfig.nHTTPPort, st_AuthConfig.st_XMax.nMaxQueue, st_AuthConfig.st_XMax.nIOThread);
	if (NULL == xhHttpSocket)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动HTTP管理网络服务失败，错误：%lX"), NetCore_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化HTTP管理网络服务成功,端口:%d,网络池个数:%d"), st_AuthConfig.nHTTPPort, st_AuthConfig.st_XMax.nIOThread);
	NetCore_TCPXCore_RegisterCallBackEx(xhHttpSocket, XEngine_Client_HttpAccept, XEngine_Client_HttpRecv, XEngine_Client_HttpClose);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化HTTP管理网络事件成功"));

	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListTCPThread, st_AuthConfig.st_XMax.nTCPThread, sizeof(THREADPOOL_PARAMENT));
	for (int i = 0; i < st_AuthConfig.st_XMax.nTCPThread; i++)
	{
		int* pInt_Index = new int;
		*pInt_Index = i;
		ppSt_ListTCPThread[i]->lParam = pInt_Index;
		ppSt_ListTCPThread[i]->fpCall_ThreadsTask = XEngine_AuthService_TCPThread;
	}
	ManagePool_Thread_NQCreate(&xhTCPPool, &ppSt_ListTCPThread, st_AuthConfig.st_XMax.nTCPThread);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动TCP任务线程池成功,线程个数:%d"), st_AuthConfig.st_XMax.nTCPThread);

	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListWSThread, st_AuthConfig.st_XMax.nWSThread, sizeof(THREADPOOL_PARAMENT));
	for (int i = 0; i < st_AuthConfig.st_XMax.nWSThread; i++)
	{
		int* pInt_Index = new int;
		*pInt_Index = i;
		ppSt_ListWSThread[i]->lParam = pInt_Index;
		ppSt_ListWSThread[i]->fpCall_ThreadsTask = XEngine_AuthService_WSThread;
	}
	ManagePool_Thread_NQCreate(&xhWSPool, &ppSt_ListWSThread, st_AuthConfig.st_XMax.nWSThread);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动WEBSOCKET任务线程池成功,线程个数:%d"), st_AuthConfig.st_XMax.nWSThread);

	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListHttpThread, st_AuthConfig.st_XMax.nHTTPThread, sizeof(THREADPOOL_PARAMENT));
	for (int i = 0; i < st_AuthConfig.st_XMax.nHTTPThread; i++)
	{
		int* pInt_Index = new int;
		*pInt_Index = i;
		ppSt_ListHttpThread[i]->lParam = pInt_Index;
		ppSt_ListHttpThread[i]->fpCall_ThreadsTask = XEngine_AuthService_HttpThread;
	}
	ManagePool_Thread_NQCreate(&xhHttpPool, &ppSt_ListHttpThread, st_AuthConfig.st_XMax.nHTTPThread);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动HTTP任务线程池成功,线程个数:%d"), st_AuthConfig.st_XMax.nHTTPThread);

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("所有服务成功启动，网络验证服务运行中,XEngien版本:%s,发行版本次数:%d,当前运行版本：%s。。。"), BaseLib_OperatorVer_XGetStr(), st_AuthConfig.st_XVer.pStl_ListVer->size(), st_AuthConfig.st_XVer.pStl_ListVer->front().c_str());

	while (TRUE)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

XENGINE_EXITAPP:

	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("网络验证服务器退出..."));
		bIsRun = FALSE;

		HelpComponents_Datas_Destory(xhTCPPacket);
		RfcComponents_WSPacket_DestoryEx(xhWSPacket);
		RfcComponents_HttpServer_DestroyEx(xhHttpPacket);

		NetCore_TCPXCore_DestroyEx(xhTCPSocket);
		NetCore_TCPXCore_DestroyEx(xhWSSocket);
		NetCore_TCPXCore_DestroyEx(xhHttpSocket);

		ManagePool_Thread_NQDestroy(xhTCPPool);
		ManagePool_Thread_NQDestroy(xhWSPool);
		ManagePool_Thread_NQDestroy(xhHttpPool);

		HelpComponents_XLog_Destroy(xhLog);
		Session_Authorize_Destroy();
		Session_Token_Destroy();
		Database_SQLite_Destroy();
		exit(0);
	}
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}
