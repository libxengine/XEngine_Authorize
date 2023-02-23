// Dialog_Switch.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "afxdialogex.h"
#include "Dialog_Switch.h"


// CDialog_Switch 对话框

IMPLEMENT_DYNAMIC(CDialog_Switch, CDialogEx)

CDialog_Switch::CDialog_Switch(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SWITCH, pParent)
{

}

CDialog_Switch::~CDialog_Switch()
{
}

void CDialog_Switch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_RadioDelOPen);
	DDX_Control(pDX, IDC_RADIO2, m_RadioDelClose);
	DDX_Control(pDX, IDC_RADIO3, m_RadioRegOPen);
	DDX_Control(pDX, IDC_RADIO4, m_RadioRegClose);
	DDX_Control(pDX, IDC_RADIO5, m_RadioLoginOPen);
	DDX_Control(pDX, IDC_RADIO6, m_RadioLoginClose);
	DDX_Control(pDX, IDC_RADIO7, m_RadioPayOPen);
	DDX_Control(pDX, IDC_RADIO8, m_RadioPayClose);
	DDX_Control(pDX, IDC_RADIO9, m_RadioPassOPen);
	DDX_Control(pDX, IDC_RADIO10, m_RadioPassClose);
	DDX_Control(pDX, IDC_BUTTON2, m_BtnSetConfigure);
	DDX_Control(pDX, IDC_BUTTON1, m_BtnGetConfigure);
	DDX_Control(pDX, IDC_RADIO11, m_RadioCDkeyOPen);
	DDX_Control(pDX, IDC_RADIO12, m_RadioCDKeyClose);
}


BEGIN_MESSAGE_MAP(CDialog_Switch, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_Switch::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_Switch::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDialog_Switch 消息处理程序


void CDialog_Switch::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	TCHAR tszUrlAddr[MAX_PATH];
	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);

	memset(tszUrlAddr, '\0', MAX_PATH);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	Json::Value st_JsonRoot;
	st_JsonRoot["xhToken"] = _ttoi64(m_StrToken.GetBuffer());

	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	_stprintf(tszUrlAddr, _T("http://%s:%s/auth/switch/get"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());
	//是否加密
	TCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));

	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		nMsgLen = st_JsonRoot.toStyledString().length();
		OPenSsl_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (UCHAR*)tszMsgBuffer, tszPassBuffer);
		APIClient_Http_Request(_T("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIClient_Http_Request(_T("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
	}
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		OPenSsl_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, tszPassBuffer);
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析接口数据错误,无法继续"));
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析接口数据错误,无法继续"));
			return;
		}
	}

	if (0 == st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("查询服务功能开关成功"));

		if (st_JsonRoot["bSwitchDelete"].asBool())
		{
			m_RadioDelOPen.SetCheck(BST_CHECKED);
			m_RadioDelClose.SetCheck(BST_UNCHECKED);
		}
		else
		{
			m_RadioDelOPen.SetCheck(BST_UNCHECKED);
			m_RadioDelClose.SetCheck(BST_CHECKED);
		}

		if (st_JsonRoot["bSwitchRegister"].asBool())
		{
			m_RadioRegOPen.SetCheck(BST_CHECKED);
			m_RadioRegClose.SetCheck(BST_UNCHECKED);
		}
		else
		{
			m_RadioRegOPen.SetCheck(BST_UNCHECKED);
			m_RadioRegClose.SetCheck(BST_CHECKED);
		}

		if (st_JsonRoot["bSwitchLogin"].asBool())
		{
			m_RadioLoginOPen.SetCheck(BST_CHECKED);
			m_RadioLoginClose.SetCheck(BST_UNCHECKED);
		}
		else
		{
			m_RadioLoginOPen.SetCheck(BST_UNCHECKED);
			m_RadioLoginClose.SetCheck(BST_CHECKED);
		}

		if (st_JsonRoot["bSwitchPay"].asBool())
		{
			m_RadioPayOPen.SetCheck(BST_CHECKED);
			m_RadioPayClose.SetCheck(BST_UNCHECKED);
		}
		else
		{
			m_RadioPayOPen.SetCheck(BST_UNCHECKED);
			m_RadioPayClose.SetCheck(BST_CHECKED);
		}

		if (st_JsonRoot["bSwitchPass"].asBool())
		{
			m_RadioPassOPen.SetCheck(BST_CHECKED);
			m_RadioPassClose.SetCheck(BST_UNCHECKED);
		}
		else
		{
			m_RadioPassOPen.SetCheck(BST_UNCHECKED);
			m_RadioPassClose.SetCheck(BST_CHECKED);
		}

		if (st_JsonRoot["bSwitchCDKey"].asBool())
		{
			m_RadioCDkeyOPen.SetCheck(BST_CHECKED);
			m_RadioCDKeyClose.SetCheck(BST_UNCHECKED);
		}
		else
		{
			m_RadioCDkeyOPen.SetCheck(BST_UNCHECKED);
			m_RadioCDKeyClose.SetCheck(BST_CHECKED);
		}
		m_BtnSetConfigure.EnableWindow(TRUE);
	}
	else
	{
		Authorize_Help_LogPrint(_T("查询服务功能开关失败"));
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}


void CDialog_Switch::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	TCHAR tszUrlAddr[MAX_PATH];
	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);

	memset(tszUrlAddr, '\0', MAX_PATH);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	if (BST_CHECKED == m_RadioDelOPen.GetCheck())
	{
		st_JsonObject["bSwitchDelete"] = true;
	}
	else
	{
		st_JsonObject["bSwitchDelete"] = false;
	}

	if (BST_CHECKED == m_RadioRegOPen.GetCheck())
	{
		st_JsonObject["bSwitchRegister"] = true;
	}
	else
	{
		st_JsonObject["bSwitchRegister"] = false;
	}

	if (BST_CHECKED == m_RadioLoginOPen.GetCheck())
	{
		st_JsonObject["bSwitchLogin"] = true;
	}
	else
	{
		st_JsonObject["bSwitchLogin"] = false;
	}

	if (BST_CHECKED == m_RadioPayOPen.GetCheck())
	{
		st_JsonObject["bSwitchPay"] = true;
	}
	else
	{
		st_JsonObject["bSwitchPay"] = false;
	}

	if (BST_CHECKED == m_RadioPassOPen.GetCheck())
	{
		st_JsonObject["bSwitchPass"] = true;
	}
	else
	{
		st_JsonObject["bSwitchPass"] = false;
	}

	if (BST_CHECKED == m_RadioCDkeyOPen.GetCheck())
	{
		st_JsonObject["bSwitchCDKey"] = true;
	}
	else
	{
		st_JsonObject["bSwitchCDKey"] = false;
	}

	st_JsonRoot["st_SwitchInfo"] = st_JsonObject;
	st_JsonRoot["xhToken"] = _ttoi64(m_StrToken.GetBuffer());

	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	_stprintf(tszUrlAddr, _T("http://%s:%s/auth/switch/set"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());
	//是否加密
	TCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));

	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		nMsgLen = st_JsonRoot.toStyledString().length();
		OPenSsl_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (UCHAR*)tszMsgBuffer, tszPassBuffer);
		APIClient_Http_Request(_T("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIClient_Http_Request(_T("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
	}
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		OPenSsl_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, tszPassBuffer);
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析接口数据错误,无法继续"));
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析接口数据错误,无法继续"));
			return;
		}
	}

	if (0 == st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("设置功能开关成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("插入功能开关失败"));
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}


BOOL CDialog_Switch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_BtnSetConfigure.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
