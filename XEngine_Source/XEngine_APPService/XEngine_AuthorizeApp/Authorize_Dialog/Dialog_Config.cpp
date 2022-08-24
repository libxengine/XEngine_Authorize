// Dialog_Config.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "afxdialogex.h"
#include "Dialog_Config.h"


// CDialog_Config 对话框

IMPLEMENT_DYNAMIC(CDialog_Config, CDialogEx)

CDialog_Config::CDialog_Config(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CONFIG, pParent)
{

}

CDialog_Config::~CDialog_Config()
{
}

void CDialog_Config::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditIPAddr);
	DDX_Control(pDX, IDC_EDIT2, m_EditIPPort);
	DDX_Control(pDX, IDC_EDIT3, m_EditUser);
	DDX_Control(pDX, IDC_EDIT4, m_EditPass);
	DDX_Control(pDX, IDC_EDIT9, m_EditToken);
	DDX_Control(pDX, IDC_BUTTON1, m_BtnLogin);
	DDX_Control(pDX, IDC_BUTTON2, m_BtnLogout);
	DDX_Control(pDX, IDC_EDIT10, m_EditTimeout);
}


BEGIN_MESSAGE_MAP(CDialog_Config, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_Config::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_Config::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDialog_Config 消息处理程序


BOOL CDialog_Config::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_EditIPAddr.SetWindowText(_T("127.0.0.1"));
	m_EditIPPort.SetWindowText(_T("5302"));

	m_EditUser.SetWindowText(_T("admin"));
	m_EditPass.SetWindowText(_T("123123aa"));

	m_BtnLogout.EnableWindow(FALSE);
	hConfigWnd = m_hWnd;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDialog_Config::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrUser;
	CString m_StrPass;
	CString m_StrToken;
	TCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);
	//组合请求URL
	m_EditIPAddr.GetWindowText(m_StrIPAddr);
	m_EditIPPort.GetWindowText(m_StrIPPort);
	m_EditUser.GetWindowText(m_StrUser);
	m_EditPass.GetWindowText(m_StrPass);

	_stprintf(tszUrlAddr, _T("http://%s:%s/api?function=login&user=%s&pass=%s"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer(), m_StrUser.GetBuffer(), m_StrPass.GetBuffer());
	//请求用户信息
	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	APIHelp_HttpRequest_Get(tszUrlAddr, &ptszMsgBuffer, &nMsgLen);

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		AfxMessageBox(_T("登录失败,无法继续"));
		return;
	}
	if (0 != st_JsonRoot["code"].asInt())
	{
		AfxMessageBox(_T("登录失败,无法继续"));
		return;
	}
	m_StrToken.Format(_T("%lld"), st_JsonRoot["xhToken"].asUInt64());
	m_EditToken.SetWindowText(m_StrToken);
	m_EditTimeout.SetWindowText(st_JsonRoot["tszTimeEnd"].asCString());

	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	AfxMessageBox(_T("登录成功"));

	m_BtnLogin.EnableWindow(FALSE);
	m_BtnLogout.EnableWindow(TRUE);
}


void CDialog_Config::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_BtnLogin.EnableWindow(TRUE);
	m_BtnLogout.EnableWindow(FALSE);
}
