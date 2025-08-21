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
	DDX_Control(pDX, IDC_BUTTON5, m_BtnUpdate);
	DDX_Control(pDX, IDC_RADIO1, m_CheckCodecEnable);
	DDX_Control(pDX, IDC_RADIO2, m_CheckCodecDisable);
	DDX_Control(pDX, IDC_COMBO2, m_ListEncrypto);
	DDX_Control(pDX, IDC_EDIT6, m_EditPassword);
	DDX_Control(pDX, IDC_EDIT11, m_EditDCode);
	DDX_Control(pDX, IDC_COMBO7, m_ComboPassCodec);
	DDX_Control(pDX, IDC_RADIO4, m_RadioPassDisable);
	DDX_Control(pDX, IDC_RADIO3, m_RadioPassEnable);
}


BEGIN_MESSAGE_MAP(CDialog_Config, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_Config::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_Config::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CDialog_Config::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_RADIO2, &CDialog_Config::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CDialog_Config::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON8, &CDialog_Config::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_RADIO3, &CDialog_Config::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CDialog_Config::OnBnClickedRadio4)
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

	m_BtnLogout.EnableWindow(false);
	m_BtnUpdate.EnableWindow(false);

	m_CheckCodecEnable.SetCheck(BST_UNCHECKED);
	m_CheckCodecDisable.SetCheck(BST_CHECKED);

	m_RadioPassDisable.SetCheck(BST_CHECKED);
	m_ComboPassCodec.EnableWindow(false);
	m_ComboPassCodec.InsertString(0, _T("MD4"));
	m_ComboPassCodec.InsertString(1, _T("MD5"));
	m_ComboPassCodec.InsertString(2, _T("SHA1"));
	m_ComboPassCodec.InsertString(3, _T("SHA256"));
	m_ComboPassCodec.SetCurSel(1);

	m_ListEncrypto.AddString(_T("XCrypto(X加密)"));
	m_ListEncrypto.SetCurSel(0);
	m_ListEncrypto.EnableWindow(false);

	m_EditPassword.SetWindowText(_T("123123"));
	m_EditPassword.EnableWindow(false);

	hConfigWnd = m_hWnd;
	return TRUE;  // return true unless you set the focus to a control
	// 异常: OCX 属性页应返回 false
}


void CDialog_Config::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrUser;
	CString m_StrPass;
	CString m_StrToken;
	CString m_StrDCode;
	XCHAR tszUrlAddr[XPATH_MAX];
	memset(tszUrlAddr, '\0', XPATH_MAX);
	//组合请求URL
	m_EditIPAddr.GetWindowText(m_StrIPAddr);
	m_EditIPPort.GetWindowText(m_StrIPPort);
	m_EditUser.GetWindowText(m_StrUser);
	m_EditPass.GetWindowText(m_StrPass);
	m_EditDCode.GetWindowText(m_StrDCode);
	
	USES_CONVERSION;
	XCHAR tszMDBuffer[64] = {};
	if (m_RadioPassEnable.GetCheck() == BST_CHECKED)
	{
		int nPLen = m_StrPass.GetLength();
		XBYTE byMD5Buffer[XPATH_MAX] = {};
		Cryption_Api_Digest(W2A(m_StrPass.GetBuffer()), byMD5Buffer, &nPLen, false, m_ComboPassCodec.GetCurSel() + 1);
		BaseLib_String_StrToHex((LPCXSTR)byMD5Buffer, nPLen, tszMDBuffer);
	}
	else
	{
		_tcsxcpy(tszMDBuffer, W2A(m_StrPass.GetBuffer()));
	}
	if (m_StrDCode.GetLength() > 0)
	{
		m_EditToken.GetWindowText(m_StrToken);
		_xstprintf(tszUrlAddr, _X("http://%s:%s/api?function=login&user=%s&pass=%s&device=%d&token=%s&dcode=%s"), W2A(m_StrIPAddr.GetBuffer()), W2A(m_StrIPPort.GetBuffer()), W2A(m_StrUser.GetBuffer()), tszMDBuffer, ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC_WINDOWS, W2A(m_StrToken.GetBuffer()), W2A(m_StrDCode.GetBuffer()));
	}
	else
	{
		_xstprintf(tszUrlAddr, _X("http://%s:%s/api?function=login&user=%s&pass=%s&device=%d"), W2A(m_StrIPAddr.GetBuffer()), W2A(m_StrIPPort.GetBuffer()), W2A(m_StrUser.GetBuffer()), tszMDBuffer, ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC_WINDOWS);
	}
	//请求用户信息
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	APIClient_Http_Request(_X("GET"), tszUrlAddr, NULL, NULL, &ptszMsgBuffer, &nMsgLen);

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (BST_CHECKED == m_CheckCodecEnable.GetCheck())
	{
		CString m_StrCodecPass;
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		m_EditPassword.GetWindowText(m_StrCodecPass);
		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, W2A(m_StrCodecPass.GetBuffer()));
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("登录失败,无法继续"));
			AuthHelp_Windows_Dithering(hMainWnd);
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("登录失败,无法继续"));
			AuthHelp_Windows_Dithering(hMainWnd);
			return;
		}
	}
	if (0 != st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("登录失败,无法继续"));
		AuthHelp_Windows_Dithering(hMainWnd);
		return;
	}
	m_StrToken.Format(_T("%lld"), st_JsonRoot["xhToken"].asUInt64());
	m_EditToken.SetWindowText(m_StrToken);
	m_EditTimeout.SetWindowText(A2W(st_JsonRoot["tszTimeEnd"].asCString()));

	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	Authorize_Help_LogPrint(_T("登录成功"));

	m_BtnLogin.EnableWindow(false);
	m_BtnLogout.EnableWindow(true);
	m_BtnUpdate.EnableWindow(true);
}


void CDialog_Config::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	XCHAR tszUrlAddr[XPATH_MAX];
	memset(tszUrlAddr, '\0', XPATH_MAX);
	//组合请求URL
	m_EditIPAddr.GetWindowText(m_StrIPAddr);
	m_EditIPPort.GetWindowText(m_StrIPPort);
	m_EditToken.GetWindowText(m_StrToken);

	USES_CONVERSION;
	_xstprintf(tszUrlAddr, _X("http://%s:%s/api?function=close&token=%s"), W2A(m_StrIPAddr.GetBuffer()), W2A(m_StrIPPort.GetBuffer()), W2A(m_StrToken.GetBuffer()));
	//请求用户信息
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	APIClient_Http_Request(_X("GET"), tszUrlAddr, NULL, NULL, &ptszMsgBuffer, &nMsgLen);

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (BST_CHECKED == m_CheckCodecEnable.GetCheck())
	{
		CString m_StrCodecPass;
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		m_EditPassword.GetWindowText(m_StrCodecPass);
		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, W2A(m_StrCodecPass.GetBuffer()));
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("关闭失败,无法继续"));
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("关闭失败,无法继续"));
			return;
		}
	}
	if (0 != st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("关闭失败,无法继续"));
		return;
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	m_EditToken.SetWindowText(_T(""));
	m_EditTimeout.SetWindowText(_T(""));

	m_BtnLogin.EnableWindow(true);
	m_BtnLogout.EnableWindow(false);
	m_BtnUpdate.EnableWindow(false);
}


void CDialog_Config::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	XCHAR tszUrlAddr[XPATH_MAX];
	memset(tszUrlAddr, '\0', XPATH_MAX);
	//组合请求URL
	m_EditIPAddr.GetWindowText(m_StrIPAddr);
	m_EditIPPort.GetWindowText(m_StrIPPort);
	m_EditToken.GetWindowText(m_StrToken);
	USES_CONVERSION;
	_xstprintf(tszUrlAddr, _X("http://%s:%s/api?function=update&token=%s"), W2A(m_StrIPAddr.GetBuffer()), W2A(m_StrIPPort.GetBuffer()), W2A(m_StrToken.GetBuffer()));
	//请求用户信息
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	APIClient_Http_Request(_X("GET"), tszUrlAddr, NULL, NULL, &ptszMsgBuffer, &nMsgLen);

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (BST_CHECKED == m_CheckCodecEnable.GetCheck())
	{
		CString m_StrCodecPass;
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		m_EditPassword.GetWindowText(m_StrCodecPass);
		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, W2A(m_StrCodecPass.GetBuffer()));
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("续期失败,无法继续"));
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("续期失败,无法继续"));
			return;
		}
	}
	if (0 != st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("续期失败,无法继续"));
		return;
	}
	m_EditTimeout.SetWindowText(A2W(st_JsonRoot["tszTimeEnd"].asCString()));
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}


void CDialog_Config::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListEncrypto.EnableWindow(false);
	m_EditPassword.EnableWindow(false);
	bCrypto = false;
}


void CDialog_Config::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListEncrypto.EnableWindow(true);
	m_EditPassword.EnableWindow(true);
	bCrypto = true;
}


void CDialog_Config::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrIPPort;
	XCHAR tszUrlAddr[XPATH_MAX];
	memset(tszUrlAddr, '\0', XPATH_MAX);
	//组合请求URL
	m_EditIPAddr.GetWindowText(m_StrIPAddr);
	m_EditIPPort.GetWindowText(m_StrIPPort);

	USES_CONVERSION;
	_xstprintf(tszUrlAddr, _X("http://%s:%s/api?function=dcode&user=get"), W2A(m_StrIPAddr.GetBuffer()), W2A(m_StrIPPort.GetBuffer()));
	//请求用户信息
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	APIClient_Http_Request(_X("GET"), tszUrlAddr, NULL, NULL, &ptszMsgBuffer, &nMsgLen);

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (BST_CHECKED == m_CheckCodecEnable.GetCheck())
	{
		CString m_StrCodecPass;
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		m_EditPassword.GetWindowText(m_StrCodecPass);
		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, W2A(m_StrCodecPass.GetBuffer()));
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("获取验证码失败,无法继续"));
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("获取验证码失败,无法继续"));
			return;
		}
	}
	if (0 != st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("获取验证码失败,无法继续"));
		return;
	}
	TCHAR tszDCodeStr[64];
	TCHAR tszTokenStr[64];

	memset(tszDCodeStr, '\0', sizeof(tszDCodeStr));
	memset(tszTokenStr, '\0', sizeof(tszTokenStr));

	_stprintf(tszDCodeStr, _T("%d"), st_JsonRoot["nDynamicCode"].asUInt());
	_stprintf(tszTokenStr, _T("%llu"), st_JsonRoot["xhToken"].asUInt64());

	m_EditToken.SetWindowText(tszTokenStr);
	m_EditDCode.SetWindowText(tszDCodeStr);
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}


void CDialog_Config::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ComboPassCodec.EnableWindow(TRUE);
}


void CDialog_Config::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ComboPassCodec.EnableWindow(FALSE);
}
