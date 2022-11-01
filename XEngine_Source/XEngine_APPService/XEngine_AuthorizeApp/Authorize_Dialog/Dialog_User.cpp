// Dialog_User.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "afxdialogex.h"
#include "Dialog_User.h"


// CDialog_User 对话框

IMPLEMENT_DYNAMIC(CDialog_User, CDialogEx)

CDialog_User::CDialog_User(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_USER, pParent)
{

}

CDialog_User::~CDialog_User()
{
}

void CDialog_User::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrlClient);
	DDX_Control(pDX, IDC_CHECK1, m_CheckOnlineList);
	DDX_Control(pDX, IDC_EDIT1, m_EditFlushTime);
	DDX_Control(pDX, IDC_CHECK2, m_CheckAuto);
	DDX_Control(pDX, IDC_BUTTON4, m_BtnModifyClient);
	DDX_Control(pDX, IDC_EDIT2, m_EditPosStart);
	DDX_Control(pDX, IDC_EDIT3, m_EditPosEnd);
}


BEGIN_MESSAGE_MAP(CDialog_User, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_User::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_User::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialog_User::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDialog_User::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK2, &CDialog_User::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON5, &CDialog_User::OnBnClickedButton5)
END_MESSAGE_MAP()


// CDialog_User 消息处理程序


BOOL CDialog_User::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListCtrlClient.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
	m_ListCtrlClient.InsertColumn(1, _T("用户名"), LVCFMT_LEFT, 85);
	m_ListCtrlClient.InsertColumn(2, _T("级别"), LVCFMT_LEFT, 70);
	m_ListCtrlClient.InsertColumn(3, _T("在线时间(分钟)"), LVCFMT_LEFT, 100);
	m_ListCtrlClient.InsertColumn(4, _T("剩余时间/过期时间"), LVCFMT_LEFT, 120);
	m_ListCtrlClient.InsertColumn(5, _T("充值类型"), LVCFMT_LEFT, 80);
	m_ListCtrlClient.InsertColumn(6, _T("设备类型"), LVCFMT_LEFT, 60);
	m_ListCtrlClient.InsertColumn(7, _T("是否在线"), LVCFMT_LEFT, 60);
	m_ListCtrlClient.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_EditFlushTime.SetWindowText("1");
	m_EditPosStart.SetWindowText("0");
	m_EditPosEnd.SetWindowText("50");
	hUserWnd = m_hWnd;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDialog_User::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListCtrlClient.DeleteAllItems();
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

	_stprintf(tszUrlAddr, _T("http://%s:%s/auth/client/list"), tszIPAddr, tszIPPort);
	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	st_JsonRoot["xhToken"] = _ttoi64(tszToken);
	st_JsonRoot["PosStart"] = _ttoi(m_StrPosStart.GetBuffer());
	st_JsonRoot["PosEnd"] = _ttoi(m_StrPosEnd.GetBuffer());
	if (BST_CHECKED == m_CheckOnlineList.GetCheck())
	{
		st_JsonRoot["Online"] = true;
	}
	else
	{
		st_JsonRoot["Online"] = false;
	}
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
		APIHelp_HttpRequest_Custom(_T("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIHelp_HttpRequest_Custom(_T("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
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
		Json::Value st_JsonObject = st_JsonArray["st_UserInfo"];

		m_ListCtrlClient.InsertItem(i, _T(""));
		m_ListCtrlClient.SetItemText(i, 0, tszIndex);
		m_ListCtrlClient.SetItemText(i, 1, st_JsonObject["tszUserName"].asCString());
		m_ListCtrlClient.SetItemText(i, 2, lpszXLevelType[st_JsonObject["nUserLevel"].asInt() + 1]);

		if (1 == st_JsonObject["nUserState"].asInt())
		{
			TCHAR tszTimeStr[64];
			memset(tszTimeStr, '\0', sizeof(tszTimeStr));

			__int64x nTime = st_JsonArray["nOnlineTime"].asUInt64();
			_stprintf(tszTimeStr, _T("%lld"), nTime);
			m_ListCtrlClient.SetItemText(i, 3, tszTimeStr);
		}
		m_ListCtrlClient.SetItemText(i, 4, st_JsonArray["tszLeftTime"].asCString());
		m_ListCtrlClient.SetItemText(i, 5, lpszXSerialType[st_JsonArray["enSerialType"].asInt()]);
		m_ListCtrlClient.SetItemText(i, 6, lpszXDevType[st_JsonArray["enDeviceType"].asInt()]);
		m_ListCtrlClient.SetItemText(i, 7, lpszStuType[st_JsonObject["nUserState"].asInt()]);
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	UpdateWindow();
}


void CDialog_User::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListCtrlClient.GetFirstSelectedItemPosition();
	int nSelect = m_ListCtrlClient.GetNextSelectedItem(pSt_Sition);
	if (nSelect < 0)
	{
		Authorize_Help_LogPrint(_T("你没有选择任何客户！"));
		return;
	}
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	CString m_StrUser = m_ListCtrlClient.GetItemText(nSelect, 1);
	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);

	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	TCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);
	_stprintf(tszUrlAddr, _T("http://%s:%s/auth/client/close"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());

	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonObject["tszUserName"] = m_StrUser.GetBuffer();
	st_JsonRoot["st_UserInfo"] = st_JsonObject;
	st_JsonRoot["xhToken"] = _ttoi64(m_StrToken.GetBuffer());

	//是否加密
	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	TCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));

	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		nMsgLen = st_JsonRoot.toStyledString().length();
		OPenSsl_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (UCHAR*)tszMsgBuffer, tszPassBuffer);
		APIHelp_HttpRequest_Custom(_T("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIHelp_HttpRequest_Custom(_T("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
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
		Authorize_Help_LogPrint(_T("关闭客户端成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("关闭客户端失败"));
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	//剔除客户需要刷新客户列表
	OnBnClickedButton1();
}


void CDialog_User::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListCtrlClient.GetFirstSelectedItemPosition();
	int nSelect = m_ListCtrlClient.GetNextSelectedItem(pSt_Sition);
	if (nSelect < 0)
	{
		Authorize_Help_LogPrint(_T("你没有选择任何客户！"));
		return;
	}
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	CString m_StrUser = m_ListCtrlClient.GetItemText(nSelect, 1);
	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	TCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);
	_stprintf(tszUrlAddr, _T("http://%s:%s/auth/client/delete"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());

	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonObject["tszUserName"] = m_StrUser.GetBuffer();
	st_JsonRoot["st_UserInfo"] = st_JsonObject;
	st_JsonRoot["xhToken"] = _ttoi64(m_StrToken.GetBuffer());
	//是否加密
	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	TCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));
	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		nMsgLen = st_JsonRoot.toStyledString().length();
		OPenSsl_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (UCHAR*)tszMsgBuffer, tszPassBuffer);
		APIHelp_HttpRequest_Custom(_T("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIHelp_HttpRequest_Custom(_T("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
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


void CDialog_User::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListCtrlClient.GetFirstSelectedItemPosition();
	int nItemCount = m_ListCtrlClient.GetNextSelectedItem(pSt_Sition);
	if (nItemCount < 0)
	{
		Authorize_Help_LogPrint(_T("你没有选择任何用户！"));
		return;
	}
	CDialog_Modify m_DlgModify;
	m_DlgModify.DoModal();
}


void CDialog_User::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == m_CheckAuto.GetCheck())
	{
		bThread = TRUE;
		hThread = CreateThread(NULL, 0, Dialog_User_Thread, this, 0, NULL);
	}
	else
	{
		bThread = FALSE;
		DWORD dwRet = WaitForSingleObject(hThread, INFINITE);
		if (WAIT_OBJECT_0 != dwRet)
		{
			TerminateThread(hThread, -1);
		}
		CloseHandle(hThread);
	}
}

DWORD CDialog_User::Dialog_User_Thread(LPVOID lParam)
{
	CDialog_User* pClass_This = (CDialog_User*)lParam;
	time_t nTimeStart = time(NULL);
	CString m_StrTime;
	
	pClass_This->m_EditFlushTime.GetWindowText(m_StrTime);
	int nTimeSecond = _ttoi(m_StrTime.GetBuffer());

	while (pClass_This->bThread)
	{
		time_t nTimeEnd = time(NULL);
		if ((nTimeEnd - nTimeStart) > nTimeSecond)
		{
			pClass_This->OnBnClickedButton1();
			nTimeStart = time(NULL);
		}
		Sleep(100);
	}
	return 0;
}

void CDialog_User::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListCtrlClient.GetFirstSelectedItemPosition();
	int nItemCount = m_ListCtrlClient.GetNextSelectedItem(pSt_Sition);
	if (nItemCount >= 0)
	{
		m_ListCtrlClient.SetItemState(nItemCount, 0, -1);
	}
	CDialog_Modify m_DlgModify;
	m_DlgModify.DoModal();
}
