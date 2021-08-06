
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
	ON_BN_CLICKED(IDC_BUTTON2, &CXEngineAuthorizeAppDlg::OnBnClickedButton2)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CXEngineAuthorizeAppDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON4, &CXEngineAuthorizeAppDlg::OnBnClickedButton4)
	ON_WM_DESTROY()
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
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);

	HANDLE hMutex = CreateMutex(NULL, TRUE, _T("XEngine_AuthorizeApp"));
	if (NULL != hMutex)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			AfxMessageBox(_T("程序已经运行!"));
			CloseHandle(hMutex);
			return FALSE;
		}
	}
	CloseHandle(hMutex);

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

	if (!SystemApi_Skin_CreateTrayTip(m_hWnd, _T("XEngine网络验证服务"), WINSDK_SKIN_USERMSG_TRAY, IDR_MAINFRAME))
	{
		AfxMessageBox(_T("添加托盘失败"));
		return FALSE;
	}
	m_BtnStartService.EnableWindow(TRUE);
	m_BtnStopService.EnableWindow(FALSE);

	if (st_AuthConfig.bAutoStart)
	{
		OnBnClickedButton1();
		ShowWindow(SW_HIDE);
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
	CString m_StrPort;
	CString m_StrNumber;
	CString m_StrThreads;

	m_DlgConfig.m_EditServicePort.GetWindowText(m_StrPort);
	m_DlgConfig.m_EditVerTimedout.GetWindowText(m_StrNumber);
	m_DlgConfig.m_EditThreadPool.GetWindowText(m_StrThreads);

	nThreadCount = _ttoi(m_StrThreads.GetBuffer());
	if (!AuthService_SQLPacket_Init(st_AuthConfig.st_XSql.tszSQLite))
	{
		AfxMessageBox(_T("初始化数据库失败！"));
		return;
	}
	if (!AuthService_Session_Init(XEngine_TaskEvent_Client, this))
	{
		AfxMessageBox(_T("初始化网络失败！"));
		return;
	}
	if (!HelpComponents_Datas_Init(&xhPacket, 10000, 0, nThreadCount))
	{
		AfxMessageBox(_T("启动服务器失败，初始化包管理器失败"));
		return;
	}
	if (!NetCore_TCPXCore_StartEx(&xhSocket, _ttoi(m_StrPort.GetBuffer()), 10000, nThreadCount))
	{
		CString m_StrEror;
		m_StrEror.Format(_T("启动服务器失败，启动验证网络服务失败:%lX %d"), NetCore_GetLastError(), WSAGetLastError());
		AfxMessageBox(m_StrEror);
		return;
	}
	NetCore_TCPXCore_RegisterCallBackEx(xhSocket, XEngine_Client_Accept, XEngine_Client_Recv, XEngine_Client_Close, this, this, this);

	bThread = TRUE;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ThreadParament, nThreadCount, sizeof(THREADPOOL_PARAMENT));
	for (int i = 0; i < nThreadCount; i++)
	{
		XENGINE_THREADINFO* pSt_AuthThread = new XENGINE_THREADINFO;

		pSt_AuthThread->nPoolIndex = i;
		pSt_AuthThread->lPClass = this;
		ppSt_ThreadParament[i]->lParam = pSt_AuthThread;
		ppSt_ThreadParament[i]->fpCall_ThreadsTask = XEngine_AuthService_ThreadClient;
	}
	ManagePool_Thread_NQCreate(&xhPool, &ppSt_ThreadParament, nThreadCount);

	m_DlgSerial.SerialManage_Flush();
	m_BtnStartService.EnableWindow(FALSE);
	m_BtnStopService.EnableWindow(TRUE);
}


void CXEngineAuthorizeAppDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditLog.SetWindowText(_T(""));
}


void CXEngineAuthorizeAppDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog m_FileDlg(FALSE, _T(".log"), NULL, NULL, _T("Log日志文件(*.log)|*.log|文本文件(*.txt)|(*.txt)||"));//生成一个对话框
	if (IDCANCEL == m_FileDlg.DoModal())
	{
		return;
	}
	CString m_StrSaveFile = m_FileDlg.GetPathName();
	CFile m_File;
	m_File.Open(m_StrSaveFile.GetBuffer(), CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);

	CString m_StrMsg;
	m_EditLog.GetWindowText(m_StrMsg);
	m_File.Write(m_StrMsg.GetBuffer(), m_StrMsg.GetLength());
	m_File.Close();
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

	HelpComponents_Datas_Destory(xhPacket);
	NetCore_TCPXCore_DestroyEx(xhSocket);
	ManagePool_Thread_NQDestroy(xhPool);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ThreadParament, st_AuthConfig.nThreads);

	AuthService_Session_Destroy();
	AuthService_SQLPacket_Destroy();

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
