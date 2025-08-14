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
	DDX_Control(pDX, IDC_EDIT9, m_EditSerial);
	DDX_Control(pDX, IDC_EDIT12, m_EditTime);
	DDX_Control(pDX, IDC_EDIT13, m_EditDate);
	DDX_Control(pDX, IDC_COMBO1, m_ComboType);
	DDX_Control(pDX, IDC_EDIT14, m_EditLeftTime);
}


BEGIN_MESSAGE_MAP(CDialog_Try, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_Try::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_Try::OnBnClickedButton2)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDialog_Try::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialog_Try::OnBnClickedButton3)
END_MESSAGE_MAP()


// CDialog_Try 消息处理程序

BOOL CDialog_Try::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListTry.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 40);
	m_ListTry.InsertColumn(1, _T("序列号"), LVCFMT_LEFT, 120);
	m_ListTry.InsertColumn(2, _T("试用类型"), LVCFMT_LEFT, 100);
	m_ListTry.InsertColumn(3, _T("试用时间"), LVCFMT_LEFT, 100);
	m_ListTry.InsertColumn(4, _T("剩余时间"), LVCFMT_LEFT, 100);
	m_ListTry.InsertColumn(5, _T("创建时间"), LVCFMT_LEFT, 120);
	m_ListTry.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	for (int i = 0; i < 5; i++)
	{
		USES_CONVERSION;
		m_ComboType.InsertString(i, A2W(lpszXSerialType[i]));
	}

	m_EditPosStart.SetWindowText(_T("0"));
	m_EditPosEnd.SetWindowText(_T("100"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDialog_Try::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListTry.DeleteAllItems();
	TCHAR tszIPAddr[XPATH_MAX];
	TCHAR tszIPPort[XPATH_MAX];
	TCHAR tszToken[XPATH_MAX];
	XCHAR tszUrlAddr[XPATH_MAX];
	CString m_StrPosStart;
	CString m_StrPosEnd;

	memset(tszIPAddr, '\0', XPATH_MAX);
	memset(tszIPPort, '\0', XPATH_MAX);
	memset(tszToken, '\0', XPATH_MAX);
	memset(tszUrlAddr, '\0', XPATH_MAX);

	m_EditPosStart.GetWindowText(m_StrPosStart);
	m_EditPosEnd.GetWindowText(m_StrPosEnd);
	::GetWindowText(::GetDlgItem(hConfigWnd, IDC_EDIT1), tszIPAddr, XPATH_MAX);
	::GetWindowText(::GetDlgItem(hConfigWnd, IDC_EDIT2), tszIPPort, XPATH_MAX);
	::GetWindowText(::GetDlgItem(hConfigWnd, IDC_EDIT9), tszToken, XPATH_MAX);

	USES_CONVERSION;
	_xstprintf(tszUrlAddr, _X("http://%s:%s/auth/try/list"), W2A(tszIPAddr), W2A(tszIPPort));
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	st_JsonRoot["xhToken"] = _ttoll(tszToken);
	st_JsonRoot["PosStart"] = _ttoi(m_StrPosStart.GetBuffer());
	st_JsonRoot["PosEnd"] = _ttoi(m_StrPosEnd.GetBuffer());

	//是否加密
	TCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));

	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		nMsgLen = st_JsonRoot.toStyledString().length();
		Cryption_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (UCHAR*)tszMsgBuffer, W2A(tszPassBuffer));
		APIClient_Http_Request(_X("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIClient_Http_Request(_X("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
	}
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	st_JsonRoot.clear();
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, W2A(tszPassBuffer));
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

		TCHAR tszTmpStr[64] = {};
		_stprintf(tszTmpStr, _T("%lld"), st_JsonArray["nID"].asInt64());
		m_ListTry.SetItemText(i, 0, tszTmpStr);
		m_ListTry.SetItemText(i, 1, A2W(st_JsonArray["tszVSerial"].asCString()));
		m_ListTry.SetItemText(i, 2, A2W(lpszXSerialType[st_JsonArray["enVMode"].asInt()]));
		_stprintf(tszTmpStr, _T("%d"), st_JsonArray["nVTime"].asInt());
		m_ListTry.SetItemText(i, 3, tszTmpStr);
		_stprintf(tszTmpStr, _T("%d"), st_JsonArray["nLTime"].asInt());
		m_ListTry.SetItemText(i, 4, tszTmpStr);
		m_ListTry.SetItemText(i, 5, A2W(st_JsonArray["tszVDate"].asCString()));
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
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

	XCHAR tszUrlAddr[XPATH_MAX];
	memset(tszUrlAddr, '\0', XPATH_MAX);
	USES_CONVERSION;
	_xstprintf(tszUrlAddr, _X("http://%s:%s/auth/try/delete"), W2A(m_StrIPAddr.GetBuffer()), W2A(m_StrIPPort.GetBuffer()));

	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonObject["tszVSerial"] = W2A(m_StrSerial.GetBuffer());
	st_JsonRoot["st_VERTemp"] = st_JsonObject;
	st_JsonRoot["xhToken"] = _ttoll(m_StrToken.GetBuffer());
	//是否加密
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	TCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		nMsgLen = st_JsonRoot.toStyledString().length();
		Cryption_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (UCHAR*)tszMsgBuffer, W2A(tszPassBuffer));
		APIClient_Http_Request(_X("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIClient_Http_Request(_X("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
	}
	//查看返回值是否正确
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, W2A(tszPassBuffer));
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
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
		Authorize_Help_LogPrint(_T("删除临时验证信息成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("删除临时验证信息失败"));
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	//需要刷新客户列表
	OnBnClickedButton1();
}


void CDialog_Try::OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListTry.GetFirstSelectedItemPosition();
	int nItemCount = m_ListTry.GetNextSelectedItem(pSt_Sition);
	if (nItemCount < 0)
	{
		return;
	}
	CString m_StrID = m_ListTry.GetItemText(nItemCount, 0);
	CString m_StrSerial = m_ListTry.GetItemText(nItemCount, 1);
	CString m_StrType = m_ListTry.GetItemText(nItemCount, 2);
	CString m_StrTime = m_ListTry.GetItemText(nItemCount, 3);
	CString m_StrLeft = m_ListTry.GetItemText(nItemCount, 4);
	CString m_StrDate = m_ListTry.GetItemText(nItemCount, 5);

	m_EditSerial.SetWindowText(m_StrSerial.GetBuffer());
	m_EditTime.SetWindowText(m_StrTime.GetBuffer());
	m_EditLeftTime.SetWindowText(m_StrLeft.GetBuffer());
	m_EditDate.SetWindowText(m_StrDate.GetBuffer());

	for (int i = 0; i < m_ComboType.GetCount(); i++)
	{
		CString m_StrTmp;
		m_ComboType.GetLBText(i, m_StrTmp);
		if (0 == _tcsnicmp(m_StrType.GetBuffer(), m_StrTmp.GetBuffer(), m_StrTmp.GetLength()))
		{
			m_ComboType.SetCurSel(i);
			break;
		}
	}
	
	*pResult = 0;
}


void CDialog_Try::OnBnClickedButton3()
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
	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	XCHAR tszUrlAddr[XPATH_MAX];
	memset(tszUrlAddr, '\0', XPATH_MAX);
	USES_CONVERSION;
	_xstprintf(tszUrlAddr, _X("http://%s:%s/auth/try/modify"), W2A(m_StrIPAddr.GetBuffer()), W2A(m_StrIPPort.GetBuffer()));

	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	CString m_StrTime;
	CString m_StrLeft;
	CString m_StrDate;
	CString m_StrID = m_ListTry.GetItemText(nSelect, 0);
	CString m_StrSerail = m_ListTry.GetItemText(nSelect, 1);

	m_EditTime.GetWindowText(m_StrTime);
	m_EditLeftTime.GetWindowText(m_StrLeft);
	m_EditDate.GetWindowText(m_StrDate);

	st_JsonObject["nID"] = _ttoi64(m_StrID.GetBuffer());
	st_JsonObject["enVMode"] = (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE)m_ComboType.GetCurSel();
	st_JsonObject["nVTime"] = _ttoi(m_StrTime.GetBuffer());
	st_JsonObject["nLTime"] = _ttoi(m_StrLeft.GetBuffer());
	st_JsonObject["tszVSerial"] = W2A(m_StrSerail.GetBuffer());
	st_JsonObject["tszVDate"] = W2A(m_StrDate.GetBuffer());
	st_JsonRoot["st_VERTemp"] = st_JsonObject;
	st_JsonRoot["xhToken"] = _ttoll(m_StrToken.GetBuffer());
	//是否加密
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	TCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		nMsgLen = st_JsonRoot.toStyledString().length();
		Cryption_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (UCHAR*)tszMsgBuffer, W2A(tszPassBuffer));
		APIClient_Http_Request(_X("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIClient_Http_Request(_X("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
	}
	//查看返回值是否正确
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, W2A(tszPassBuffer));
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
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
		Authorize_Help_LogPrint(_T("修改临时验证信息成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("修改临时验证信息失败"));
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	//需要刷新客户列表
	OnBnClickedButton1();
}
