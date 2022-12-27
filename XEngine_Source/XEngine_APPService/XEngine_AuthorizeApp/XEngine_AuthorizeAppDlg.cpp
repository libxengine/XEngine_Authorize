
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
	DDX_Control(pDX, IDC_TAB1, m_TabWindows);
	DDX_Control(pDX, IDC_EDIT1, m_EditLog);
	DDX_Control(pDX, IDC_BUTTON1, m_BtnLogSave);
	DDX_Control(pDX, IDC_BUTTON2, m_BtnLogClear);
}

BEGIN_MESSAGE_MAP(CXEngineAuthorizeAppDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CXEngineAuthorizeAppDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON1, &CXEngineAuthorizeAppDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CXEngineAuthorizeAppDlg::OnBnClickedButton2)
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

	m_TabWindows.InsertItem(0, _T("服务配置"));
	m_TabWindows.InsertItem(1, _T("用户管理"));
	m_TabWindows.InsertItem(2, _T("序列号管理"));
	m_TabWindows.InsertItem(3, _T("功能开关"));

	m_DlgConfig.Create(IDD_DIALOG_CONFIG, &m_TabWindows);
	m_DlgUser.Create(IDD_DIALOG_USER, &m_TabWindows);
	m_DlgSerial.Create(IDD_DIALOG_SERIAL, &m_TabWindows);
	m_DlgSwitch.Create(IDD_DIALOG_SWITCH, &m_TabWindows);
	//调整子对话框在父窗口中的位置 
	CRect st_Rect;
	m_TabWindows.GetClientRect(&st_Rect);
	st_Rect.top += 20;
	st_Rect.bottom -= 1;
	st_Rect.left += 1;
	st_Rect.right -= 1;
	//设置子对话框尺寸并移动到指定位置
	m_DlgConfig.MoveWindow(&st_Rect);
	m_DlgUser.MoveWindow(&st_Rect);
	m_DlgSerial.MoveWindow(&st_Rect);
	m_DlgSwitch.MoveWindow(&st_Rect);
	//分别设置隐藏和显示 
	m_DlgConfig.ShowWindow(TRUE);
	m_DlgUser.ShowWindow(FALSE);
	m_DlgSerial.ShowWindow(FALSE);
	m_DlgSwitch.ShowWindow(FALSE);
	m_TabWindows.SetCurSel(0);

	hMainWnd = m_hWnd;

	AuthHelp_Windows_CreateTooltip(m_BtnLogSave.m_hWnd, _T("保存日志为文件"));
	AuthHelp_Windows_CreateTooltip(m_BtnLogClear.m_hWnd, _T("清空日志窗口"));
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



void CXEngineAuthorizeAppDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch (m_TabWindows.GetCurSel())
	{
	case 0:
		m_DlgConfig.ShowWindow(TRUE);
		m_DlgUser.ShowWindow(FALSE);
		m_DlgSerial.ShowWindow(FALSE);
		m_DlgSwitch.ShowWindow(FALSE);
		break;
	case 1:
		m_DlgConfig.ShowWindow(FALSE);
		m_DlgUser.ShowWindow(TRUE);
		m_DlgSerial.ShowWindow(FALSE);
		m_DlgSwitch.ShowWindow(FALSE);
		break;
	case 2:
		m_DlgConfig.ShowWindow(FALSE);
		m_DlgUser.ShowWindow(FALSE);
		m_DlgSerial.ShowWindow(TRUE);
		m_DlgSwitch.ShowWindow(FALSE);
		break;
	case 3:
		m_DlgConfig.ShowWindow(FALSE);
		m_DlgUser.ShowWindow(FALSE);
		m_DlgSerial.ShowWindow(FALSE);
		m_DlgSwitch.ShowWindow(TRUE);
	default:
		break;
	}

	*pResult = 0;
}


void CXEngineAuthorizeAppDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR tszFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");
	// 构造保存文件对话框   
	CFileDialog m_FileDlg(FALSE, _T("txt"), _T("log"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, tszFilter, this);
	
	// 显示保存文件对话框   
	if (IDOK == m_FileDlg.DoModal())
	{
		FILE* pSt_File = _tfopen(m_FileDlg.GetPathName(), _T("wb"));
		
		TCHAR tszLogBuffer[8196];
		memset(tszLogBuffer, '\0', sizeof(tszLogBuffer));
		int nSize = m_EditLog.GetWindowText(tszLogBuffer, sizeof(tszLogBuffer));
		fwrite(tszLogBuffer, 1, nSize, pSt_File);
		fclose(pSt_File);
	}
}


void CXEngineAuthorizeAppDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditLog.SetWindowText("");
}
