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
		TCHAR tszIndex[10];
		memset(tszIndex, '\0', 10);
		_itot_s(i, tszIndex, 10);

		Json::Value st_JsonArray = st_JsonRoot["Array"][i];

		m_ListTry.InsertItem(i, _T(""));
		m_ListTry.SetItemText(i, 0, tszIndex);
		m_ListTry.SetItemText(i, 1, st_JsonArray["tszVerSerial"].asCString());
		m_ListTry.SetItemText(i, 2, lpszXSerialType[st_JsonArray["enVerMode"].asInt()]);

		XCHAR tszTmpStr[64] = {};
		_xstprintf(tszTmpStr, _X("%d"), st_JsonArray["nTryTime"].asInt());
		m_ListTry.SetItemText(i, 3, tszTmpStr);
		m_ListTry.SetItemText(i, 5, st_JsonArray["tszVerData"].asCString());
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	UpdateWindow();
}