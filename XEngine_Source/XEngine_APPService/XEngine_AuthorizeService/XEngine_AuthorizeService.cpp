#include "Authorize_Hdr.h"

BOOL bIsRun = FALSE;
XLOG xhLog = NULL;
XNETHANDLE xhSocket = 0;
XNETHANDLE xhPacket = 0;
XNETHANDLE xhPool = 0;
AUTHORIZE_CONFIGURE st_AuthConfig;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("网络验证服务器退出..."));
		bIsRun = FALSE;

		HelpComponents_Datas_Destory(xhPacket);
		NetCore_TCPXCore_DestroyEx(xhSocket);
		ManagePool_Thread_NQDestroy(xhPool);
		HelpComponents_XLog_Destroy(xhLog);

		AuthService_Session_Destroy();
		AuthService_SQLPacket_Destroy();
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
	THREADPOOL_PARAMENT** ppSt_ListThread;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_AuthConfig, '\0', sizeof(AUTHORIZE_CONFIGURE));

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

	if (!AuthService_SQLPacket_Init(st_AuthConfig.st_XSql.tszSQLite))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化数据库服务失败，错误：%lX"), SQLPacket_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化数据库服务成功,数据库:%s"), st_AuthConfig.st_XSql.tszSQLite);

	if (!AuthService_Session_Init(XEngine_TaskEvent_Client))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化会话服务失败，错误：%lX"), Session_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化会话服务成功"));

	if (!HelpComponents_Datas_Init(&xhPacket, 10000, 0, st_AuthConfig.nThreads))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化组包器失败，错误：%lX"), Packets_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化组包器成功,句柄:%llu,任务池个数:%d"), xhPacket, st_AuthConfig.nThreads);

	if (!NetCore_TCPXCore_StartEx(&xhSocket, st_AuthConfig.nPort, 10000, st_AuthConfig.nThreads))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动验证网络服务失败，错误：%lX"), NetCore_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化验证网络服务成功,句柄:%llu,端口:%d,网络池个数:%d"), xhSocket, st_AuthConfig.nPort, st_AuthConfig.nThreads);
	NetCore_TCPXCore_RegisterCallBackEx(xhSocket, XEngine_Client_Accept, XEngine_Client_Recv, XEngine_Client_Close);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化验证网络事件成功"));

	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListThread, st_AuthConfig.nThreads, sizeof(THREADPOOL_PARAMENT));
	for (int i = 0; i < st_AuthConfig.nThreads; i++)
	{
		XENGINE_THREADINFO* pSt_AuthThread = new XENGINE_THREADINFO;

		pSt_AuthThread->nPoolIndex = i;
		ppSt_ListThread[i]->lParam = pSt_AuthThread;
		ppSt_ListThread[i]->fpCall_ThreadsTask = XEngine_AuthService_TCPThread;
	}
	ManagePool_Thread_NQCreate(&xhPool, &ppSt_ListThread, st_AuthConfig.nThreads);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动线程池成功,线程个数:%d"), st_AuthConfig.nThreads);

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("所有服务成功启动，网络验证服务运行中,当前运行版本：2.2.0.1001。。。"));
	while (bIsRun)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

XENGINE_EXITAPP:

	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("网络验证服务器退出..."));
		bIsRun = FALSE;

		HelpComponents_Datas_Destory(xhPacket);
		NetCore_TCPXCore_DestroyEx(xhSocket);
		ManagePool_Thread_NQDestroy(xhPool);
		HelpComponents_XLog_Destroy(xhLog);

		AuthService_Session_Destroy();
		AuthService_SQLPacket_Destroy();
		exit(0);
	}
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}
