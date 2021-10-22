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
	DDX_Control(pDX, IDC_EDIT15, m_EditWSPort);
	DDX_Control(pDX, IDC_CHECK2, m_EditTimeNotift);
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
	LPCTSTR lpszFile = _T("./XEngine_Config/XEngine_Config.ini");
	memset(&st_AuthConfig, '\0', sizeof(AUTHORIZE_CONFIGURE));
	Configure_IniFile_Read(lpszFile, &st_AuthConfig);

	AuthorizeService_ReadConfigure();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDialog_Configure::AuthorizeService_ReadConfigure()
{
	CString m_StrConfigTCPPort;            //服务器端口
	CString m_StrConfigWSPort;             //WS端口
	CString m_StrConfigThread;             //启动的线程个数
	CString m_StrTimedOut;                 //用户验证超时时间

	CString m_StrConfigTryMode;            //验证模式
	CString m_StrConfigTryTime;            //验证时间
	CString m_StrConfigAuthMode;           //验证模式
	CString m_StrConfigAuthTime;           //验证时间

	CString m_StrConfigCrypt;              //是否启用加密传输 
	//配置
	m_StrConfigTCPPort.Format(_T("%d"), st_AuthConfig.nTCPPort);
	m_StrConfigWSPort.Format(_T("%d"), st_AuthConfig.nWSPort);
	m_StrConfigThread.Format(_T("%d"), st_AuthConfig.nThreads);
	m_StrTimedOut.Format(_T("%d"), st_AuthConfig.nVerTimeout);

	m_EditServicePort.SetWindowText(m_StrConfigTCPPort);
	m_EditWSPort.SetWindowText(m_StrConfigWSPort);
	m_EditThreadPool.SetWindowText(m_StrConfigThread);
	m_EditVerTimedout.SetWindowText(m_StrTimedOut);
	if (st_AuthConfig.bAutoStart)
	{
		m_BtnAutoStart.SetCheck(1);
		m_BtnAutoStop.SetCheck(0);
	}
	else
	{
		m_BtnAutoStart.SetCheck(0);
		m_BtnAutoStop.SetCheck(1);
	}

	if (st_AuthConfig.bTimeNotify)
	{
		m_EditTimeNotift.SetCheck(TRUE);
	}

	if (st_AuthConfig.st_Crypto.bEnable)
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
	m_StrConfigCrypt.Format(_T("%d"), st_AuthConfig.st_Crypto.nPassword);
	m_EditPass.SetWindowText(m_StrConfigCrypt.GetBuffer());

	m_StrConfigTryTime.Format(_T("%d"), st_AuthConfig.st_Verification.nTryTime);
	m_EditTryTime.SetWindowText(m_StrConfigTryTime);
	m_ComboRegTry.InsertString(0, _T("不支持"));
	m_ComboRegTry.InsertString(1, _T("分钟"));
	m_ComboRegTry.InsertString(2, _T("次数"));
	m_ComboRegTry.SetCurSel(st_AuthConfig.st_Verification.nTryMode);

	m_StrConfigAuthTime.Format(_T("%d"), st_AuthConfig.st_Verification.nVerTime);
	m_EditAuthTime.SetWindowText(m_StrConfigAuthTime);
	m_ComboListAuth.InsertString(0, _T("不支持"));
	m_ComboListAuth.InsertString(1, _T("分钟"));
	m_ComboListAuth.InsertString(2, _T("天数"));
	m_ComboListAuth.InsertString(3, _T("次数"));
	m_ComboListAuth.SetCurSel(st_AuthConfig.st_Verification.nVerMode);

	m_EditSmtpAddr.SetWindowText(st_AuthConfig.st_EMail.tszSmtpAddr);
	m_EditFromAddr.SetWindowText(st_AuthConfig.st_EMail.tszSmtpFrom);
	m_EditSmtpUser.SetWindowText(st_AuthConfig.st_EMail.tszSmtpUser);
	m_EditSmtpPass.SetWindowText(st_AuthConfig.st_EMail.tszSmtpPass);
}
void CDialog_Configure::AuthorizeService_WriteConfigure()
{
	CString m_StrConfigTCPPort;               //服务器端口
	CString m_StrConfigWSPort;
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

	if (BST_CHECKED == m_EditTimeNotift.GetCheck())
	{
		st_AuthConfig.bTimeNotify = TRUE;
	}
	else
	{
		st_AuthConfig.bTimeNotify = FALSE;
	}

	m_EditServicePort.GetWindowText(m_StrConfigTCPPort);
	m_EditWSPort.GetWindowText(m_StrConfigWSPort);
	m_EditThreadPool.GetWindowText(m_StrConfigThread);
	m_EditVerTimedout.GetWindowText(m_StrTimedOut);
	st_AuthConfig.nTCPPort = _ttoi(m_StrConfigTCPPort.GetBuffer());
	st_AuthConfig.nWSPort = _ttoi(m_StrConfigWSPort.GetBuffer());
	st_AuthConfig.nThreads = _ttoi(m_StrConfigThread.GetBuffer());
	st_AuthConfig.nVerTimeout = _ttoi(m_StrTimedOut.GetBuffer());
	st_AuthConfig.bAutoStart = m_BtnAutoStart.GetCheck();

	m_EditTryTime.GetWindowText(m_StrConfigTryTime);
	m_EditAuthTime.GetWindowText(m_StrConfigAuthTime);
	st_AuthConfig.st_Verification.nTryMode = m_ComboRegTry.GetCurSel();
	st_AuthConfig.st_Verification.nVerMode = m_ComboListAuth.GetCurSel();
	st_AuthConfig.st_Verification.nTryTime = _ttoi(m_StrConfigTryTime.GetBuffer());
	st_AuthConfig.st_Verification.nVerTime = _ttoi(m_StrConfigAuthTime.GetBuffer());

	m_EditPass.GetWindowText(m_StrConfigCrypt);
	st_AuthConfig.st_Crypto.bEnable = m_RadioKeyPass.GetCheck();
	st_AuthConfig.st_Crypto.nPassword = _ttoi(m_StrConfigCrypt.GetBuffer());

	m_EditSmtpAddr.GetWindowText(st_AuthConfig.st_EMail.tszSmtpAddr, MAX_PATH);
	m_EditFromAddr.GetWindowText(st_AuthConfig.st_EMail.tszSmtpFrom, MAX_PATH);
	m_EditSmtpUser.GetWindowText(st_AuthConfig.st_EMail.tszSmtpUser, MAX_PATH);
	m_EditSmtpPass.GetWindowText(st_AuthConfig.st_EMail.tszSmtpPass, MAX_PATH);
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
