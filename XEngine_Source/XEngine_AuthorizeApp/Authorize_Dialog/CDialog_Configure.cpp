// CDialog_Configure.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "CDialog_Configure.h"
#include "afxdialogex.h"


// CDialog_Configure 对话框

IMPLEMENT_DYNAMIC(CDialog_Configure, CDialogEx)

CDialog_Configure::CDialog_Configure(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CONFIG, pParent)
{

}

CDialog_Configure::~CDialog_Configure()
{
}

void CDialog_Configure::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditServicePort);
	DDX_Control(pDX, IDC_EDIT3, m_EditThreadPool);
	DDX_Control(pDX, IDC_EDIT2, m_EditVerTimedout);
	DDX_Control(pDX, IDC_EDIT5, m_EditPass);
	DDX_Control(pDX, IDC_RADIO3, m_BtnAutoStart);
	DDX_Control(pDX, IDC_RADIO4, m_BtnAutoStop);
	DDX_Control(pDX, IDC_COMBO1, m_ComboRegTry);
	DDX_Control(pDX, IDC_COMBO2, m_ComboListAuth);
	DDX_Control(pDX, IDC_RADIO1, m_RadioKeyPass);
	DDX_Control(pDX, IDC_RADIO2, m_RadioKeyDisable);
	DDX_Control(pDX, IDC_EDIT4, m_EditTryTime);
	DDX_Control(pDX, IDC_EDIT6, m_EditAuthTime);
	DDX_Control(pDX, IDC_EDIT7, m_EditSmtpAddr);
	DDX_Control(pDX, IDC_EDIT8, m_EditFromAddr);
	DDX_Control(pDX, IDC_EDIT9, m_EditSmtpUser);
	DDX_Control(pDX, IDC_EDIT10, m_EditSmtpPass);
}


BEGIN_MESSAGE_MAP(CDialog_Configure, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_Configure::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_Configure::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialog_Configure::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_RADIO2, &CDialog_Configure::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CDialog_Configure::OnBnClickedRadio1)
END_MESSAGE_MAP()


// CDialog_Configure 消息处理程序


BOOL CDialog_Configure::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	AuthorizeService_ReadConfigure();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDialog_Configure::AuthorizeService_ReadConfigure()
{
	LPCTSTR lpszConfigFile = _T("./XEngine_Config/XEngine_Config.ini");
	CString m_StrConfigPort;               //服务器端口
	CString m_StrConfigThread;             //启动的线程个数
	CString m_StrTimedOut;                 //用户验证超时时间
	int m_nConfigAutoStart;                //自启动

	CString m_StrConfigTryMode;           //验证模式
	CString m_StrConfigTryTime;           //验证时间
	CString m_StrConfigAuthMode;           //验证模式
	CString m_StrConfigAuthTime;           //验证时间

	BOOL m_bConfigCrypt;
	CString m_StrConfigCrypt;              //是否启用加密传输 

	CString m_StrConfigSmtpAddr;           //SMTP服务器
	CString m_StrConfigSmtpFrom;           //回复地址
	CString m_StrConfigSmtpUser;           //用户名
	CString m_StrConfigSmtpPass;           //密码

	GetPrivateProfileString(_T("ServiceConfig"), _T("ListenPort"), NULL, m_StrConfigPort.GetBuffer(MAX_PATH), MAX_PATH, lpszConfigFile);
	GetPrivateProfileString(_T("ServiceConfig"), _T("ThreadPool"), NULL, m_StrConfigThread.GetBuffer(MAX_PATH), MAX_PATH, lpszConfigFile);
	GetPrivateProfileString(_T("ServiceConfig"), _T("UserVerTimed"), NULL, m_StrTimedOut.GetBuffer(MAX_PATH), MAX_PATH, lpszConfigFile);
	m_nConfigAutoStart = GetPrivateProfileInt(_T("ServiceConfig"), _T("AutoStart"), 0, lpszConfigFile);

	GetPrivateProfileString(_T("Verification"), _T("VerTime"), NULL, m_StrConfigAuthTime.GetBuffer(MAX_PATH), MAX_PATH, lpszConfigFile);
	GetPrivateProfileString(_T("Verification"), _T("VerMode"), NULL, m_StrConfigAuthMode.GetBuffer(MAX_PATH), MAX_PATH, lpszConfigFile);
	GetPrivateProfileString(_T("Verification"), _T("TryTime"), NULL, m_StrConfigTryTime.GetBuffer(MAX_PATH), MAX_PATH, lpszConfigFile);
	GetPrivateProfileString(_T("Verification"), _T("TryMode"), NULL, m_StrConfigTryMode.GetBuffer(MAX_PATH), MAX_PATH, lpszConfigFile);

	m_bConfigCrypt = GetPrivateProfileInt(_T("Crypto"), _T("Enable"), 0, lpszConfigFile);
	GetPrivateProfileString(_T("Crypto"), _T("Crypto"), NULL, m_StrConfigCrypt.GetBuffer(MAX_PATH), MAX_PATH, lpszConfigFile);

	GetPrivateProfileString(_T("SmtpConfig"), _T("SmtpService"), NULL, m_StrConfigSmtpAddr.GetBuffer(MAX_PATH), MAX_PATH, lpszConfigFile);
	GetPrivateProfileString(_T("SmtpConfig"), _T("SmtpFromAddr"), NULL, m_StrConfigSmtpFrom.GetBuffer(MAX_PATH), MAX_PATH, lpszConfigFile);
	GetPrivateProfileString(_T("SmtpConfig"), _T("SmtpUser"), NULL, m_StrConfigSmtpUser.GetBuffer(MAX_PATH), MAX_PATH, lpszConfigFile);
	GetPrivateProfileString(_T("SmtpConfig"), _T("SmtpPass"), NULL, m_StrConfigSmtpPass.GetBuffer(MAX_PATH), MAX_PATH, lpszConfigFile);

	//配置
	m_EditServicePort.SetWindowText(m_StrConfigPort);
	m_EditThreadPool.SetWindowText(m_StrConfigThread);
	m_EditVerTimedout.SetWindowText(m_StrTimedOut);
	if (m_nConfigAutoStart)
	{
		m_BtnAutoStart.SetCheck(1);
		m_BtnAutoStop.SetCheck(0);
	}
	else
	{
		m_BtnAutoStart.SetCheck(0);
		m_BtnAutoStop.SetCheck(1);
	}

	if (m_bConfigCrypt)
	{
		m_EditPass.EnableWindow(TRUE);
		m_RadioKeyDisable.SetCheck(FALSE);
		m_RadioKeyPass.SetCheck(TRUE);
	}
	else
	{
		m_EditPass.EnableWindow(FALSE);
		m_RadioKeyDisable.SetCheck(TRUE);
		m_RadioKeyPass.SetCheck(FALSE);
	}
	m_EditPass.SetWindowText(m_StrConfigCrypt.GetBuffer());

	m_EditTryTime.SetWindowText(m_StrConfigTryTime);
	m_ComboRegTry.AddString(_T("不支持"));
	m_ComboRegTry.AddString(_T("分钟"));
	m_ComboRegTry.AddString(_T("次数"));
	m_ComboRegTry.SetCurSel(_ttoi(m_StrConfigAuthMode.GetBuffer()));

	m_EditAuthTime.SetWindowText(m_StrConfigAuthTime);
	m_ComboListAuth.AddString(_T("不支持"));
	m_ComboListAuth.AddString(_T("分钟"));
	m_ComboListAuth.AddString(_T("天数"));
	m_ComboListAuth.AddString(_T("次数"));
	m_ComboListAuth.SetCurSel(_ttoi(m_StrConfigAuthMode.GetBuffer()));

	m_EditSmtpAddr.SetWindowText(m_StrConfigSmtpAddr);
	m_EditFromAddr.SetWindowText(m_StrConfigSmtpFrom);
	m_EditSmtpUser.SetWindowText(m_StrConfigSmtpUser);
	m_EditSmtpPass.SetWindowText(m_StrConfigSmtpPass);
}
void CDialog_Configure::AuthorizeService_WriteConfigure()
{
	LPCTSTR lpszConfigFile = _T("./XEngine_Config/XEngine_Config.ini");
	CString m_StrConfigPort;               //服务器端口
	CString m_StrConfigThread;             //启动的线程个数
	CString m_StrTimedOut;                 //用户验证超时时间

	CString m_StrConfigTryMode;           //验证模式
	CString m_StrConfigTryTime;           //验证时间
	CString m_StrConfigAuthMode;           //验证模式
	CString m_StrConfigAuthTime;           //验证时间

	CString m_StrConfigCrypt;              //是否启用加密传输       

	CString m_StrConfigSmtpAddr;           //SMTP服务器
	CString m_StrConfigSmtpFrom;           //SMTP回复地址
	CString m_StrConfigSmtpUser;           //用户名
	CString m_StrConfigSmtpPass;           //密码

	m_EditServicePort.GetWindowText(m_StrConfigPort);	
	m_EditThreadPool.GetWindowText(m_StrConfigThread);
	m_EditVerTimedout.GetWindowText(m_StrTimedOut);
	if (BST_CHECKED == m_BtnAutoStart.GetCheck())
	{
		WritePrivateProfileString(_T("ServiceConfig"), _T("AutoStart"), _T("1"), lpszConfigFile);
		if (!SystemApi_Process_AutoStart(_T("XEngine_Auth Service"), _T("XEngineAuth")))
		{
			AfxMessageBox(_T("开启自启动失败,可能没有权限,此错误不会影响其他配置！"));
		}
	}
	else
	{
		WritePrivateProfileString(_T("ServiceConfig"), _T("AutoStart"), _T("0"), lpszConfigFile);
		if (!SystemApi_Process_AutoStart(_T("XEngine_Auth Service"), _T("XEngineAuth"), FALSE))
		{
			AfxMessageBox(_T("关闭自启动失败,可能没有权限,此错误不会影响其他配置！"));
		}
	}

	m_StrConfigTryMode.Format(_T("%d"), m_ComboRegTry.GetCurSel());
	m_EditTryTime.GetWindowText(m_StrConfigTryTime);
	m_StrConfigAuthMode.Format(_T("%d"), m_ComboListAuth.GetCurSel());
	m_EditAuthTime.GetWindowText(m_StrConfigAuthTime);

	m_EditPass.GetWindowText(m_StrConfigCrypt);

	m_EditSmtpAddr.GetWindowText(m_StrConfigSmtpAddr);
	m_EditFromAddr.GetWindowText(m_StrConfigSmtpFrom);
	m_EditSmtpUser.GetWindowText(m_StrConfigSmtpUser);
	m_EditSmtpPass.GetWindowText(m_StrConfigSmtpPass);

	WritePrivateProfileString(_T("ServiceConfig"), _T("ListenPort"), m_StrConfigPort.GetBuffer(), lpszConfigFile);
	WritePrivateProfileString(_T("ServiceConfig"), _T("ThreadPool"), m_StrConfigThread.GetBuffer(), lpszConfigFile);
	WritePrivateProfileString(_T("ServiceConfig"), _T("UserVerTimed"), m_StrTimedOut.GetBuffer(), lpszConfigFile);

	WritePrivateProfileString(_T("Verification"), _T("TryTime"), m_StrConfigTryTime.GetBuffer(), lpszConfigFile);
	WritePrivateProfileString(_T("Verification"), _T("TryMode"), m_StrConfigTryMode.GetBuffer(), lpszConfigFile);
	WritePrivateProfileString(_T("Verification"), _T("VerTime"), m_StrConfigAuthTime.GetBuffer(), lpszConfigFile);
	WritePrivateProfileString(_T("Verification"), _T("VerMode"), m_StrConfigAuthMode.GetBuffer(), lpszConfigFile);

	if (BST_CHECKED == m_RadioKeyPass.GetCheck())
	{
		WritePrivateProfileString(_T("Crypto"), _T("Enable"), _T("1"), lpszConfigFile);
		WritePrivateProfileString(_T("Crypto"), _T("Pass"), m_StrConfigCrypt.GetBuffer(), lpszConfigFile);
	}
	else
	{
		WritePrivateProfileString(_T("Crypto"), _T("Enable"), _T("0"), lpszConfigFile);
	}

	WritePrivateProfileString(_T("SmtpConfig"), _T("SmtpService"), m_StrConfigSmtpAddr.GetBuffer(), lpszConfigFile);
	WritePrivateProfileString(_T("SmtpConfig"), _T("SmtpFromAddr"), m_StrConfigSmtpFrom.GetBuffer(), lpszConfigFile);
	WritePrivateProfileString(_T("SmtpConfig"), _T("SmtpUser"), m_StrConfigSmtpUser.GetBuffer(), lpszConfigFile);
	WritePrivateProfileString(_T("SmtpConfig"), _T("SmtpPassword"), m_StrConfigSmtpPass.GetBuffer(), lpszConfigFile);
}

void CDialog_Configure::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	AuthorizeService_WriteConfigure();
}


void CDialog_Configure::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	LPCTSTR lpszConfigFile = _T("./XEngine_Config/XEngine_Config.ini");
	CopyFile(lpszConfigFile, _T("./XEngine_Config/XEngine_Config.bak"), FALSE);
}


void CDialog_Configure::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	LPCTSTR lpszConfigFile = _T("./XEngine_Config/XEngine_Config.ini");
	DeleteFile(lpszConfigFile);
	CopyFile(_T("./XEngine_Config/XEngine_Config.bak"), lpszConfigFile, FALSE);
}


void CDialog_Configure::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RadioKeyPass.SetCheck(0);
	m_RadioKeyDisable.SetCheck(1);

	m_EditPass.EnableWindow(FALSE);
}


void CDialog_Configure::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RadioKeyPass.SetCheck(1);
	m_RadioKeyDisable.SetCheck(0);

	m_EditPass.EnableWindow(TRUE);
}
