// Dialog_Banned.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "afxdialogex.h"
#include "Dialog_Banned.h"


// CDialog_Banned 对话框

IMPLEMENT_DYNAMIC(CDialog_Banned, CDialogEx)

CDialog_Banned::CDialog_Banned(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BANNED, pParent)
{

}

CDialog_Banned::~CDialog_Banned()
{
}

void CDialog_Banned::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_RadioIPAddr);
	DDX_Control(pDX, IDC_RADIO2, m_RadioUser);
	DDX_Control(pDX, IDC_STATIC_TIPS, m_StaticTips);
	DDX_Control(pDX, IDC_LIST1, m_ListAddr);
	DDX_Control(pDX, IDC_LIST2, m_ListUser);
	DDX_Control(pDX, IDC_EDIT1, m_EditUser);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DataTime);
	DDX_Control(pDX, IDC_CHECK1, m_BtnCheckTime);
	DDX_Control(pDX, IDC_RADIO4, m_RadioEnable);
	DDX_Control(pDX, IDC_RADIO6, m_RadioDisable);
}


BEGIN_MESSAGE_MAP(CDialog_Banned, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CDialog_Banned::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDialog_Banned::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_Banned::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CDialog_Banned::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialog_Banned::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK1, &CDialog_Banned::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON5, &CDialog_Banned::OnBnClickedButton5)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &CDialog_Banned::OnNMClickList2)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDialog_Banned::OnNMClickList1)
END_MESSAGE_MAP()


// CDialog_Banned 消息处理程序


void CDialog_Banned::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_StaticTips.SetWindowText(_T("要禁用的IP地址:"));
}


void CDialog_Banned::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_StaticTips.SetWindowText(_T("要禁用的用户名:"));
}


XBOOL CDialog_Banned::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_StaticTips.SetWindowText(_T("要禁用的用户名:"));
	m_RadioUser.SetCheck(BST_CHECKED);

	m_ListAddr.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 80);
	m_ListAddr.InsertColumn(1, _T("是否启用"), LVCFMT_LEFT, 60);
	m_ListAddr.InsertColumn(2, _T("IP地址"), LVCFMT_LEFT, 120);
	m_ListAddr.InsertColumn(3, _T("过期日期"), LVCFMT_LEFT, 120);
	m_ListAddr.InsertColumn(4, _T("创建日期"), LVCFMT_LEFT, 120);
	m_ListAddr.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_ListUser.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 80);
	m_ListUser.InsertColumn(1, _T("是否启用"), LVCFMT_LEFT, 60);
	m_ListUser.InsertColumn(2, _T("用户名"), LVCFMT_LEFT, 120);
	m_ListUser.InsertColumn(3, _T("过期日期"), LVCFMT_LEFT, 120);
	m_ListUser.InsertColumn(4, _T("创建日期"), LVCFMT_LEFT, 120);
	m_ListUser.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_DataTime.EnableWindow(XFALSE);
	m_RadioEnable.SetCheck(BST_CHECKED);
	m_DataTime.SetFormat(_T("yyyy-mm-dd hh:mm:ss"));
	return XTRUE;  // return XTRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 XFALSE
}


void CDialog_Banned::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	CString m_StrUser;
	CString m_StrTime;

	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);
	m_EditUser.GetWindowText(m_StrUser);
	m_DataTime.GetWindowText(m_StrTime);

	XCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);

	_stprintf(tszUrlAddr, _T("http://%s:%s/auth/banned/insert"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	st_JsonRoot["xhToken"] = _ttoi64(m_StrToken.GetBuffer());
	if (BST_CHECKED == m_RadioUser.GetCheck())
	{
		st_JsonObject["tszUserName"] = m_StrUser.GetBuffer();
	}
	else
	{
		st_JsonObject["tszIPAddr"] = m_StrUser.GetBuffer();
	}
	if (BST_CHECKED == m_RadioEnable.GetCheck())
	{
		st_JsonObject["bEnable"] = true;
	}
	else
	{
		st_JsonObject["bEnable"] = false;
	}
	if (BST_CHECKED == m_BtnCheckTime.GetCheck())
	{
		st_JsonObject["tszLeftTime"] = m_StrTime.GetBuffer();
	}
	st_JsonRoot["st_Banned"] = st_JsonObject;
	//是否加密
	XCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
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
		XCHAR tszMsgBuffer[2048];
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

	if (0 == st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("插入黑名单信息成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("插入黑名单信息成功"));
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	OnBnClickedButton4();
}


void CDialog_Banned::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListUser.DeleteAllItems();
	m_ListAddr.DeleteAllItems();

	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;

	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	XCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);

	_stprintf(tszUrlAddr, _T("http://%s:%s/auth/banned/list"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	st_JsonRoot["xhToken"] = _ttoi64(m_StrToken.GetBuffer());
	//是否加密
	XCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
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
		XCHAR tszMsgBuffer[2048];
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

	for (unsigned int i = 0; i < st_JsonRoot["ArrayAddr"].size(); i++)
	{
		XCHAR tszIndex[10];
		memset(tszIndex, '\0', 10);
		_itot_s(i, tszIndex, 10);

		Json::Value st_JsonArray = st_JsonRoot["ArrayAddr"][i];

		m_ListAddr.InsertItem(i, _T(""));
		m_ListAddr.SetItemText(i, 0, tszIndex);
		if (st_JsonArray["bEnable"].asBool())
		{
			m_ListAddr.SetItemText(i, 1, "启用");
		}
		else
		{
			m_ListAddr.SetItemText(i, 1, "禁用");
		}
		m_ListAddr.SetItemText(i, 2, st_JsonArray["tszIPAddr"].asCString());
		m_ListAddr.SetItemText(i, 3, st_JsonArray["tszLeftTime"].asCString());
		m_ListAddr.SetItemText(i, 4, st_JsonArray["tszCreateTime"].asCString());
	}
	for (unsigned int i = 0; i < st_JsonRoot["ArrayUser"].size(); i++)
	{
		XCHAR tszIndex[10];
		memset(tszIndex, '\0', 10);
		_itot_s(i, tszIndex, 10);

		Json::Value st_JsonArray = st_JsonRoot["ArrayUser"][i];

		m_ListUser.InsertItem(i, _T(""));
		m_ListUser.SetItemText(i, 0, tszIndex);
		if (st_JsonArray["bEnable"].asBool())
		{
			m_ListUser.SetItemText(i, 1, "启用");
		}
		else
		{
			m_ListUser.SetItemText(i, 1, "禁用");
		}
		m_ListUser.SetItemText(i, 2, st_JsonArray["tszUserName"].asCString());
		m_ListUser.SetItemText(i, 3, st_JsonArray["tszLeftTime"].asCString());
		m_ListUser.SetItemText(i, 4, st_JsonArray["tszCreateTime"].asCString());
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}


void CDialog_Banned::OnBnClickedButton3()
{
	CString m_StrUser;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListUser.GetFirstSelectedItemPosition();
	int nSelect = m_ListUser.GetNextSelectedItem(pSt_Sition);
	if (nSelect >= 0)
	{
		m_StrUser = m_ListUser.GetItemText(nSelect, 2);
		st_JsonObject["tszUserName"] = m_StrUser.GetBuffer();
	}
	pSt_Sition = m_ListAddr.GetFirstSelectedItemPosition();
	nSelect = m_ListAddr.GetNextSelectedItem(pSt_Sition);
	if (nSelect >= 0)
	{
		m_StrUser = m_ListAddr.GetItemText(nSelect, 2);
		st_JsonObject["tszIPAddr"] = m_StrUser.GetBuffer();
	}
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	XCHAR tszUrlAddr[MAX_PATH];
	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);

	memset(tszUrlAddr, '\0', MAX_PATH);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	st_JsonRoot["xhToken"] = _ttoi64(m_StrToken.GetBuffer());
	st_JsonRoot["st_Banned"] = st_JsonObject;
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	_stprintf(tszUrlAddr, _T("http://%s:%s/auth/banned/delete"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());
	//是否加密
	XCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));

	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
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
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		OPenSsl_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, tszPassBuffer);
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析删除黑名单接口数据错误,无法继续"));
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析删除黑名单接口数据错误,无法继续"));
			return;
		}
	}

	if (0 == st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("删除黑名单数据成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("删除黑名单数据失败"));
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	//刷新
	OnBnClickedButton4();
}


void CDialog_Banned::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == m_BtnCheckTime.GetCheck())
	{
		m_DataTime.EnableWindow(XTRUE);
		m_BtnCheckTime.SetCheck(BST_CHECKED);
	}
	else
	{
		m_DataTime.EnableWindow(XFALSE);
		m_BtnCheckTime.SetCheck(BST_UNCHECKED);
	}
}


void CDialog_Banned::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	CString m_StrUser;
	CString m_StrTime;

	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);
	m_EditUser.GetWindowText(m_StrUser);
	m_DataTime.GetWindowText(m_StrTime);

	XCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);

	_stprintf(tszUrlAddr, _T("http://%s:%s/auth/banned/modify"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	st_JsonRoot["xhToken"] = _ttoi64(m_StrToken.GetBuffer());
	if (BST_CHECKED == m_RadioUser.GetCheck())
	{
		st_JsonObject["tszUserName"] = m_StrUser.GetBuffer();
	}
	else
	{
		st_JsonObject["tszIPAddr"] = m_StrUser.GetBuffer();
	}
	if (BST_CHECKED == m_RadioEnable.GetCheck())
	{
		st_JsonObject["bEnable"] = true;
	}
	else
	{
		st_JsonObject["bEnable"] = false;
	}
	if (BST_CHECKED == m_BtnCheckTime.GetCheck())
	{
		st_JsonObject["tszLeftTime"] = m_StrTime.GetBuffer();
	}
	st_JsonRoot["st_Banned"] = st_JsonObject;
	//是否加密
	XCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
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
		XCHAR tszMsgBuffer[2048];
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

	if (0 == st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("更新黑名单信息成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("更新黑名单信息成功"));
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	OnBnClickedButton4();
}


void CDialog_Banned::OnNMClickList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListUser.GetFirstSelectedItemPosition();
	int nItemCount = m_ListUser.GetNextSelectedItem(pSt_Sition);
	if (nItemCount < 0)
	{
		return;
	}
	CString m_StrEnable = m_ListUser.GetItemText(nItemCount, 1);
	CString m_StrUser = m_ListUser.GetItemText(nItemCount, 2);
	CString m_StrTime = m_ListUser.GetItemText(nItemCount, 3);

	if (0 == _tcsnicmp(m_StrEnable.GetBuffer(), _T("启用"), m_StrEnable.GetLength()))
	{
		m_RadioEnable.SetCheck(BST_CHECKED);
		m_RadioDisable.SetCheck(BST_UNCHECKED);
	}
	else
	{
		m_RadioEnable.SetCheck(BST_UNCHECKED);
		m_RadioDisable.SetCheck(BST_CHECKED);
	}
	m_RadioIPAddr.SetCheck(BST_UNCHECKED);
	m_RadioUser.SetCheck(BST_CHECKED);

	m_EditUser.SetWindowText(m_StrUser.GetBuffer());
	if (m_StrTime.GetLength() > 0)
	{
		SYSTEMTIME st_SysTime;
		XENGINE_LIBTIMER st_LibTime;

		memset(&st_SysTime, '\0', sizeof(SYSTEMTIME));
		memset(&st_LibTime, '\0', sizeof(XENGINE_LIBTIMER));

		BaseLib_OperatorTime_StrToTime(m_StrTime.GetBuffer(), &st_LibTime);

		st_SysTime.wYear = st_LibTime.wYear;
		st_SysTime.wMonth = st_LibTime.wMonth;
		st_SysTime.wDay = st_LibTime.wDay;
		st_SysTime.wHour = st_LibTime.wHour;
		st_SysTime.wMinute = st_LibTime.wMinute;
		st_SysTime.wSecond = st_LibTime.wSecond;

		m_BtnCheckTime.SetCheck(BST_CHECKED);
		m_DataTime.EnableWindow(XTRUE);
		m_DataTime.SetTime(&st_SysTime);
	}
	else
	{
		m_BtnCheckTime.SetCheck(BST_UNCHECKED);
		m_DataTime.EnableWindow(XFALSE);
	}
	*pResult = 0;
}


void CDialog_Banned::OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListAddr.GetFirstSelectedItemPosition();
	int nItemCount = m_ListAddr.GetNextSelectedItem(pSt_Sition);
	if (nItemCount < 0)
	{
		return;
	}
	CString m_StrEnable = m_ListAddr.GetItemText(nItemCount, 1);
	CString m_StrUser = m_ListAddr.GetItemText(nItemCount, 2);
	CString m_StrTime = m_ListAddr.GetItemText(nItemCount, 3);

	if (0 == _tcsnicmp(m_StrEnable.GetBuffer(), _T("启用"), m_StrEnable.GetLength()))
	{
		m_RadioEnable.SetCheck(BST_CHECKED);
		m_RadioDisable.SetCheck(BST_UNCHECKED);
	}
	else
	{
		m_RadioEnable.SetCheck(BST_UNCHECKED);
		m_RadioDisable.SetCheck(BST_CHECKED);
	}
	m_RadioIPAddr.SetCheck(BST_CHECKED);
	m_RadioUser.SetCheck(BST_UNCHECKED);

	m_EditUser.SetWindowText(m_StrUser.GetBuffer());
	if (m_StrTime.GetLength() > 0)
	{
		SYSTEMTIME st_SysTime;
		XENGINE_LIBTIMER st_LibTime;

		memset(&st_SysTime, '\0', sizeof(SYSTEMTIME));
		memset(&st_LibTime, '\0', sizeof(XENGINE_LIBTIMER));

		BaseLib_OperatorTime_StrToTime(m_StrTime.GetBuffer(), &st_LibTime);

		st_SysTime.wYear = st_LibTime.wYear;
		st_SysTime.wMonth = st_LibTime.wMonth;
		st_SysTime.wDay = st_LibTime.wDay;
		st_SysTime.wHour = st_LibTime.wHour;
		st_SysTime.wMinute = st_LibTime.wMinute;
		st_SysTime.wSecond = st_LibTime.wSecond;

		m_BtnCheckTime.SetCheck(BST_CHECKED);
		m_DataTime.EnableWindow(XTRUE);
		m_DataTime.SetTime(&st_SysTime);
	}
	else
	{
		m_BtnCheckTime.SetCheck(BST_UNCHECKED);
		m_DataTime.EnableWindow(XFALSE);
	}

	*pResult = 0;
}
