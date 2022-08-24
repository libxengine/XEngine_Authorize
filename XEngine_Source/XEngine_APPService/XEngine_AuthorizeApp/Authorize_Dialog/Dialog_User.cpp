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
}


BEGIN_MESSAGE_MAP(CDialog_User, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_User::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_User::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialog_User::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDialog_User::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDialog_User 消息处理程序


BOOL CDialog_User::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListCtrlClient.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
	m_ListCtrlClient.InsertColumn(1, _T("用户名"), LVCFMT_LEFT, 85);
	m_ListCtrlClient.InsertColumn(2, _T("级别"), LVCFMT_LEFT, 90);
	m_ListCtrlClient.InsertColumn(3, _T("在线时间(分钟)"), LVCFMT_LEFT, 100);
	m_ListCtrlClient.InsertColumn(4, _T("剩余时间/过期时间"), LVCFMT_LEFT, 110);
	m_ListCtrlClient.InsertColumn(5, _T("充值类型"), LVCFMT_LEFT, 80);
	m_ListCtrlClient.InsertColumn(6, _T("设备类型"), LVCFMT_LEFT, 60);
	m_ListCtrlClient.InsertColumn(7, _T("是否在线"), LVCFMT_LEFT, 60);
	m_ListCtrlClient.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	hUserWnd = m_hWnd;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDialog_User::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListCtrlClient.DeleteAllItems();
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	TCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);

	_stprintf(tszUrlAddr, _T("http://%s:%s/auth/client/list"), m_StrIPAddr.GetBuffer(),m_StrIPPort.GetBuffer());
	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	st_JsonRoot["xhToken"] = _ttoi64(m_StrToken.GetBuffer());

	APIHelp_HttpRequest_Post(tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);

	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	st_JsonRoot.clear();

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		AfxMessageBox(_T("解析客户列表接口数据错误,无法继续"));
		return;
	}
	for (unsigned int i = 0; i < st_JsonRoot["Array"].size(); i++)
	{
		m_ListCtrlClient.InsertItem(i, _T(""));
		TCHAR tszIndex[10];
		memset(tszIndex, '\0', 10);
		_itot_s(i, tszIndex, 10);

		Json::Value st_JsonArray = st_JsonRoot["Array"][i];
		Json::Value st_JsonObject = st_JsonArray["st_UserInfo"];

		m_ListCtrlClient.SetItemText(i, 0, tszIndex);
		m_ListCtrlClient.SetItemText(i, 1, st_JsonObject["tszUserName"].asCString());
		m_ListCtrlClient.SetItemText(i, 2, lpszXLevelType[st_JsonObject["nUserLevel"].asInt() + 1]);

		if (1 == st_JsonObject["nUserState"].asInt())
		{
			__int64x nTime = 0;
			TCHAR tszTimeStr[64];
			memset(tszTimeStr, '\0', sizeof(tszTimeStr));

			BaseLib_OperatorTime_TimeToStr(tszTimeStr);
			BaseLib_OperatorTimeSpan_GetForStr(tszTimeStr, st_JsonObject["tszLoginTime"].asCString(), &nTime, 2);
			_stprintf(tszTimeStr, _T("%lld"), nTime);
			m_ListCtrlClient.SetItemText(i, 3, tszTimeStr);
		}
		m_ListCtrlClient.SetItemText(i, 4, st_JsonArray["tszLeftTime"].asCString());
		m_ListCtrlClient.SetItemText(i, 5, lpszXSerialType[st_JsonArray["enSerialType"].asInt()]);
		m_ListCtrlClient.SetItemText(i, 6, lpszXDevType[st_JsonArray["enDeviceType"].asInt()]);
		m_ListCtrlClient.SetItemText(i, 7, lpszStuType[st_JsonObject["nUserState"].asInt()]);
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}


void CDialog_User::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListCtrlClient.GetFirstSelectedItemPosition();
	int nSelect = m_ListCtrlClient.GetNextSelectedItem(pSt_Sition);
	if (nSelect < 0)
	{
		AfxMessageBox(_T("你没有选择任何客户！"));
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

	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	APIHelp_HttpRequest_Post(tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
	//查看返回值是否正确
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		AfxMessageBox(_T("解析客户接口数据错误,无法继续"));
		return;
	}
	if (0 == st_JsonRoot["code"].asInt())
	{
		AfxMessageBox(_T("关闭客户端成功"));
	}
	else
	{
		AfxMessageBox(_T("关闭客户端失败"));
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
		AfxMessageBox(_T("你没有选择任何客户！"));
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

	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	APIHelp_HttpRequest_Post(tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
	//查看返回值是否正确
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		AfxMessageBox(_T("解析客户接口数据错误,无法继续"));
		return;
	}
	if (0 == st_JsonRoot["code"].asInt())
	{
		AfxMessageBox(_T("删除客户端成功"));
	}
	else
	{
		AfxMessageBox(_T("删除客户端失败"));
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
		AfxMessageBox(_T("你没有选择任何用户！"));
		return;
	}
	CDialog_Modify m_DlgModify;
	m_DlgModify.DoModal();
}
