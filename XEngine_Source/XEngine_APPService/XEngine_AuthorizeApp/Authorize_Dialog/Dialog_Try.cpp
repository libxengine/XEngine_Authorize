// Dialog_Try.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "afxdialogex.h"
#include "Dialog_Try.h"


// CDialog_Try 对话框

IMPLEMENT_DYNAMIC(CDialog_Try, CDialogEx)

CDialog_Try::CDialog_Try(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TRY, pParent)
{

}

CDialog_Try::~CDialog_Try()
{
}

void CDialog_Try::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListTry);
	DDX_Control(pDX, IDC_EDIT2, m_EditPosStart);
	DDX_Control(pDX, IDC_EDIT3, m_EditPosEnd);
}


BEGIN_MESSAGE_MAP(CDialog_Try, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_Try::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_Try::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDialog_Try 消息处理程序

BOOL CDialog_Try::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListTry.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 40);
	m_ListTry.InsertColumn(1, _T("序列号"), LVCFMT_LEFT, 120);
	m_ListTry.InsertColumn(2, _T("试用类型"), LVCFMT_LEFT, 100);
	m_ListTry.InsertColumn(3, _T("试用天数"), LVCFMT_LEFT, 70);
	m_ListTry.InsertColumn(4, _T("剩余时间"), LVCFMT_LEFT, 100);
	m_ListTry.InsertColumn(5, _T("创建时间"), LVCFMT_LEFT, 120);
	m_ListTry.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_EditPosStart.SetWindowText("0");
	m_EditPosEnd.SetWindowText("100");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDialog_Try::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListTry.DeleteAllItems();
	TCHAR tszIPAddr[MAX_PATH];
	TCHAR tszIPPort[MAX_PATH];
	TCHAR tszToken[MAX_PATH];
	TCHAR tszUrlAddr[MAX_PATH];
	CString m_StrPosStart;
	CString m_StrPosEnd;

	memset(tszIPAddr, '\0', MAX_PATH);
	memset(tszIPPort, '\0', MAX_PATH);
	memset(tszToken, '\0', MAX_PATH);
	memset(tszUrlAddr, '\0', MAX_PATH);

	m_EditPosStart.GetWindowText(m_StrPosStart);
	m_EditPosEnd.GetWindowText(m_StrPosEnd);
	::GetWindowText(::GetDlgItem(hConfigWnd, IDC_EDIT1), tszIPAddr, MAX_PATH);
	::GetWindowText(::GetDlgItem(hConfigWnd, IDC_EDIT2), tszIPPort, MAX_PATH);
	::GetWindowText(::GetDlgItem(hConfigWnd, IDC_EDIT9), tszToken, MAX_PATH);

	_xstprintf(tszUrlAddr, _T("http://%s:%s/auth/try/list"), tszIPAddr, tszIPPort);
	int nMsgLen = 0;
	TCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	st_JsonRoot["xhToken"] = _ttxoll(tszToken);
	st_JsonRoot["PosStart"] = _ttxoi(m_StrPosStart.GetBuffer());
	st_JsonRoot["PosEnd"] = _ttxoi(m_StrPosEnd.GetBuffer());

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
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	st_JsonRoot.clear();
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		OPenSsl_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, tszPassBuffer);
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析客户列表接口数据错误,无法继续"));
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析客户列表接口数据错误,无法继续"));
			return;
		}
	}

	for (unsigned int i = 0, j = 0; i < st_JsonRoot["Array"].size(); i++)
	{
		Json::Value st_JsonArray = st_JsonRoot["Array"][i];

		m_ListTry.InsertItem(i, _T(""));

		XCHAR tszTmpStr[64] = {};
		_xstprintf(tszTmpStr, _X("%lld"), st_JsonArray["nID"].asInt64());
		m_ListTry.SetItemText(i, 0, tszTmpStr);
		m_ListTry.SetItemText(i, 1, st_JsonArray["tszVSerial"].asCString());
		m_ListTry.SetItemText(i, 2, lpszXSerialType[st_JsonArray["enVMode"].asInt()]);
		_xstprintf(tszTmpStr, _X("%d"), st_JsonArray["nVTime"].asInt());
		m_ListTry.SetItemText(i, 3, tszTmpStr);
		m_ListTry.SetItemText(i, 5, st_JsonArray["tszVDate"].asCString());
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	UpdateWindow();
}

void CDialog_Try::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListTry.GetFirstSelectedItemPosition();
	int nSelect = m_ListTry.GetNextSelectedItem(pSt_Sition);
	if (nSelect < 0)
	{
		Authorize_Help_LogPrint(_T("你没有选择任何数据！"));
		return;
	}
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	CString m_StrSerial = m_ListTry.GetItemText(nSelect, 1);
	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	TCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);
	_xstprintf(tszUrlAddr, _T("http://%s:%s/auth/try/delete"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());

	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonObject["tszVSerial"] = m_StrSerial.GetBuffer();
	st_JsonRoot["st_VERTemp"] = st_JsonObject;
	st_JsonRoot["xhToken"] = _ttxoll(m_StrToken.GetBuffer());
	//是否加密
	int nMsgLen = 0;
	TCHAR* ptszMsgBuffer = NULL;
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
	//查看返回值是否正确
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		OPenSsl_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, tszPassBuffer);
		if (!pSt_JsonReader->parse(ptszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析客户接口数据错误,无法继续"));
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析客户接口数据错误,无法继续"));
			return;
		}
	}

	if (0 == st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("删除客户端成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("删除客户端失败"));
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	//需要刷新客户列表
	OnBnClickedButton1();
}
