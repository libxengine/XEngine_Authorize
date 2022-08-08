
// XEngine_AuthorizeAppDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "XEngine_AuthorizeApp.h"
#include "XEngine_AuthorizeAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXEngineAuthorizeAppDlg 对话框



CXEngineAuthorizeAppDlg::CXEngineAuthorizeAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XENGINE_AUTHORIZEAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXEngineAuthorizeAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditLog);
	DDX_Control(pDX, IDC_BUTTON3, m_BtnStartService);
	DDX_Control(pDX, IDC_BUTTON4, m_BtnStopService);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CXEngineAuthorizeAppDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CXEngineAuthorizeAppDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CXEngineAuthorizeAppDlg::OnBnClickedButton1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CXEngineAuthorizeAppDlg::OnTcnSelchangeTab1)
	ON_MESSAGE(WINSDK_SKIN_USERMSG_TRAY, OnAddTrayIcon)//添加消息映射
	ON_BN_CLICKED(IDC_BUTTON4, &CXEngineAuthorizeAppDlg::OnBnClickedButton4)
	ON_WM_DESTROY()
	ON_COMMAND(ID_32771, &CXEngineAuthorizeAppDlg::On32771)
	ON_COMMAND(ID_32772, &CXEngineAuthorizeAppDlg::On32772)
	ON_COMMAND(ID_32773, &CXEngineAuthorizeAppDlg::On32773)
	ON_COMMAND(ID_32774, &CXEngineAuthorizeAppDlg::On32774)
	ON_COMMAND(ID_32775, &CXEngineAuthorizeAppDlg::On32775)
END_MESSAGE_MAP()


// CXEngineAuthorizeAppDlg 消息处理程序

BOOL CXEngineAuthorizeAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	HANDLE hMutex = CreateMutex(NULL, TRUE, _T("XEngine_AuthorizeApp"));
	if (NULL != hMutex)
	{
		DWORD dwRet = GetLastError();
		if (dwRet == ERROR_ALREADY_EXISTS)
		{
			AfxMessageBox(_T("程序已经运行,无法继续!"));
			CloseHandle(hMutex);
			OnCancel();
		}
	}
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);

	m_TabCtrl.InsertItem(0, _T("服务配置"));
	m_TabCtrl.InsertItem(1, _T("用户管理"));
	m_TabCtrl.InsertItem(2, _T("序列号管理"));
	m_TabCtrl.InsertItem(3, _T("本地注册验证"));

	m_DlgConfig.Create(IDD_DIALOG_CONFIG, &m_TabCtrl);
	m_DlgUser.Create(IDD_DIALOG_USER, &m_TabCtrl);
	m_DlgSerial.Create(IDD_DIALOG_SERIAL, &m_TabCtrl);
	m_DlgLocal.Create(IDD_DIALOG_LOCAL, &m_TabCtrl);
	//调整子对话框在父窗口中的位置 
	CRect st_Rect;
	m_TabCtrl.GetClientRect(&st_Rect);
	st_Rect.top += 20;
	st_Rect.bottom -= 1;
	st_Rect.left += 1;
	st_Rect.right -= 1;
	//设置子对话框尺寸并移动到指定位置
	m_DlgConfig.MoveWindow(&st_Rect);
	m_DlgUser.MoveWindow(&st_Rect);
	m_DlgSerial.MoveWindow(&st_Rect);
	m_DlgLocal.MoveWindow(&st_Rect);
	//分别设置隐藏和显示 
	m_DlgConfig.ShowWindow(TRUE);
	m_DlgUser.ShowWindow(FALSE);
	m_DlgSerial.ShowWindow(FALSE);
	m_DlgLocal.ShowWindow(FALSE);
	m_TabCtrl.SetCurSel(0);
	//设置窗口标题
	CString m_StrVersion;
	m_StrVersion.Format(_T("XEngine 网络授权验证服务器V%s"), st_AuthConfig.st_XVer.tszVersion);
	SetWindowText(m_StrVersion);

	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;
	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));

	st_XLogConfig.XLog_MaxBackupFile = st_AuthConfig.st_XLog.nMaxCount;
	st_XLogConfig.XLog_MaxSize = st_AuthConfig.st_XLog.nMaxSize;
	_tcscpy(st_XLogConfig.tszFileName, st_AuthConfig.st_XLog.tszLogFile);

	xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_FILE | HELPCOMPONENTS_XLOG_OUTTYPE_STD, &st_XLogConfig);
	if (NULL == xhLog)
	{
		XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动日志系统失败！"));
		return FALSE;
	}
	HelpComponents_XLog_SetLogPriority(xhLog, st_AuthConfig.st_XLog.nLogLeave);
	XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务器中，启动日志系统成功！"));

	if (!SystemApi_Skin_CreateTrayTip(m_hWnd, _T("XEngine网络验证服务"), WINSDK_SKIN_USERMSG_TRAY, IDR_MAINFRAME))
	{
		XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("添加托盘失败"));
		return FALSE;
	}
	m_BtnStartService.EnableWindow(TRUE);
	m_BtnStopService.EnableWindow(FALSE);

	if (st_AuthConfig.bAutoStart)
	{
		OnBnClickedButton3();
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CXEngineAuthorizeAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CXEngineAuthorizeAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CXEngineAuthorizeAppDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!Database_SQLite_Init(st_AuthConfig.st_XSql.tszSQLite))
	{
		XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，初始化数据库失败！"));
		return;
	}
	XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务器中，启动数据库服务成功！"));

	if (!Session_Authorize_Init(XEngine_TaskEvent_Client, this))
	{
		XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中,启动会话服务失败！"));
		return;
	}
	XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务器中，启动数据库服务成功！"));

	xhTCPPacket = HelpComponents_Datas_Init(10000, st_AuthConfig.nThreads);
	if (NULL == xhTCPPacket)
	{
		XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，初始化TCP包管理器失败"));
		return;
	}
	XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务器中，初始化TCP包管理器成功！"));

	xhWSPacket = RfcComponents_WSPacket_InitEx(10000, TRUE, st_AuthConfig.nThreads);
	if (NULL == xhWSPacket)
	{
		XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，初始化WEBSOCKET包管理器失败"));
		return;
	}
	XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务器中，初始化WEBSOCKET包管理器成功！"));

	xhTCPSocket = NetCore_TCPXCore_StartEx(st_AuthConfig.nTCPPort, 10000, st_AuthConfig.nThreads);
	if (NULL == xhTCPSocket)
	{
		XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动TCP验证网络服务失败:%lX %d"), NetCore_GetLastError(), WSAGetLastError());
		return;
	}
	NetCore_TCPXCore_RegisterCallBackEx(xhTCPSocket, XEngine_Client_TCPAccept, XEngine_Client_TCPRecv, XEngine_Client_TCPClose, this, this, this);
	XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务器中，启动TCP验证网络服务成功！"));

	xhWSSocket = NetCore_TCPXCore_StartEx(st_AuthConfig.nWSPort, 10000, st_AuthConfig.nThreads);
	if (NULL == xhWSSocket)
	{
		XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动WEBSOCKET验证网络服务失败:%lX %d"), NetCore_GetLastError(), WSAGetLastError());
		return;
	}
	NetCore_TCPXCore_RegisterCallBackEx(xhWSSocket, XEngine_Client_WSAccept, XEngine_Client_WSRecv, XEngine_Client_WSClose, this, this, this);
	XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务器中，启动WEBSOCKET验证网络服务成功！"));

	bThread = TRUE;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ThreadTCPParament, nThreadCount, sizeof(THREADPOOL_PARAMENT));
	for (int i = 0; i < nThreadCount; i++)
	{
		XENGINE_THREADINFO* pSt_AuthThread = new XENGINE_THREADINFO;

		pSt_AuthThread->nPoolIndex = i;
		pSt_AuthThread->lPClass = this;
		ppSt_ThreadTCPParament[i]->lParam = pSt_AuthThread;
		ppSt_ThreadTCPParament[i]->fpCall_ThreadsTask = XEngine_AuthService_ThreadClient;
	}
	ManagePool_Thread_NQCreate(&xhTCPPool, &ppSt_ThreadTCPParament, nThreadCount);

	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ThreadWSParament, nThreadCount, sizeof(THREADPOOL_PARAMENT));
	for (int i = 0; i < nThreadCount; i++)
	{
		XENGINE_THREADINFO* pSt_AuthThread = new XENGINE_THREADINFO;

		pSt_AuthThread->nPoolIndex = i;
		pSt_AuthThread->lPClass = this;
		ppSt_ThreadWSParament[i]->lParam = pSt_AuthThread;
		ppSt_ThreadWSParament[i]->fpCall_ThreadsTask = XEngine_AuthService_WSThread;
	}
	ManagePool_Thread_NQCreate(&xhWSPool, &ppSt_ThreadWSParament, nThreadCount);
	m_DlgSerial.SerialManage_Flush();
	m_BtnStartService.EnableWindow(FALSE);
	m_BtnStopService.EnableWindow(TRUE);
}


void CXEngineAuthorizeAppDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditLog.SetWindowText(_T(""));
}


void CXEngineAuthorizeAppDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch (m_TabCtrl.GetCurSel())
	{
	case 0:
		m_DlgConfig.ShowWindow(TRUE);
		m_DlgUser.ShowWindow(FALSE);
		m_DlgSerial.ShowWindow(FALSE);
		m_DlgLocal.ShowWindow(FALSE);
		break;
	case 1:
		m_DlgConfig.ShowWindow(FALSE);
		m_DlgUser.ShowWindow(TRUE);
		m_DlgSerial.ShowWindow(FALSE);
		m_DlgLocal.ShowWindow(FALSE);
		break;
	case 2:
		m_DlgConfig.ShowWindow(FALSE);
		m_DlgUser.ShowWindow(FALSE);
		m_DlgSerial.ShowWindow(TRUE);
		m_DlgLocal.ShowWindow(FALSE);
		break;
	case 3:
		m_DlgConfig.ShowWindow(FALSE);
		m_DlgUser.ShowWindow(FALSE);
		m_DlgSerial.ShowWindow(FALSE);
		m_DlgLocal.ShowWindow(TRUE);
		break;
	default:
		break;
	}

	*pResult = 0;
}


void CXEngineAuthorizeAppDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_BtnStartService.EnableWindow(TRUE);
	m_BtnStopService.EnableWindow(FALSE);
	bThread = FALSE;
	XEngine_Authorize_LogPrint(this, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("服务器关闭！"));

	int nListCount = 0;
	AUTHREG_USERTABLE** ppSt_ListClient;
	Session_Authorize_GetClient(&ppSt_ListClient, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		AUTHREG_PROTOCOL_TIME st_TimeProtocol;
		memset(&st_TimeProtocol, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

		if (Session_Authorize_GetTimer(ppSt_ListClient[i]->st_UserInfo.tszUserName, &st_TimeProtocol))
		{
			Database_SQLite_UserLeave(&st_TimeProtocol);
		}
		Session_Authorize_CloseClient(ppSt_ListClient[i]->st_UserInfo.tszUserName);
	}

	HelpComponents_Datas_Destory(xhTCPPacket);
	RfcComponents_WSPacket_DestoryEx(xhWSPacket);
	NetCore_TCPXCore_DestroyEx(xhTCPSocket);
	NetCore_TCPXCore_DestroyEx(xhWSSocket);
	ManagePool_Thread_NQDestroy(xhTCPPool);
	ManagePool_Thread_NQDestroy(xhWSPool);
	HelpComponents_XLog_Destroy(xhLog);

	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ThreadTCPParament, st_AuthConfig.nThreads);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ThreadWSParament, st_AuthConfig.nThreads);

	Session_Authorize_Destroy();
	Database_SQLite_Destroy();

	m_DlgSerial.m_ListSerial.DeleteAllItems();
	m_DlgUser.m_ListCtrlOnlineClient.DeleteAllItems();
}


void CXEngineAuthorizeAppDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	SystemApi_Skin_CreateTrayTip(m_hWnd, _T("血与荣誉网络验证服务"), WINSDK_SKIN_USERMSG_TRAY, IDR_MAINFRAME, FALSE);
	WSACleanup();
}
LRESULT CXEngineAuthorizeAppDlg::OnAddTrayIcon(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDR_MAINFRAME)
	{
		switch (lParam)
		{
		case WM_RBUTTONDOWN:
			CMenu m_Menu;
			CPoint m_Point;
			m_Menu.LoadMenu(IDR_MENU1);
			CMenu* pSubMenu = m_Menu.GetSubMenu(0);
			GetCursorPos(&m_Point);
			SetForegroundWindow(); //解决托盘菜单不消失
			pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, m_Point.x, m_Point.y, this);
			PostMessage(WM_NULL, 0, 0); //解决托盘菜单不消失
			break;
		}
	}
	return S_OK;
}

void CXEngineAuthorizeAppDlg::On32771()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_SHOW);
}


void CXEngineAuthorizeAppDlg::On32772()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_HIDE);
}


void CXEngineAuthorizeAppDlg::On32773()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedButton3();
}


void CXEngineAuthorizeAppDlg::On32774()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedButton4();
}


void CXEngineAuthorizeAppDlg::On32775()
{
	// TODO: 在此添加命令处理程序代码
	OnOK();
}
