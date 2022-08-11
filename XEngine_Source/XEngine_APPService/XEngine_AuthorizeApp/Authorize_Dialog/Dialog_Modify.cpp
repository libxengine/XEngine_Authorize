// Dialog_Modify.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "afxdialogex.h"
#include "Dialog_Modify.h"


// CDialog_Modify 对话框

IMPLEMENT_DYNAMIC(CDialog_Modify, CDialogEx)

CDialog_Modify::CDialog_Modify(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MODIFY, pParent)
{

}

CDialog_Modify::~CDialog_Modify()
{
}

void CDialog_Modify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditUser);
	DDX_Control(pDX, IDC_EDIT2, m_EditPass);
	DDX_Control(pDX, IDC_EDIT3, m_EditEMail);
	DDX_Control(pDX, IDC_EDIT5, m_EditCardID);
	DDX_Control(pDX, IDC_EDIT4, m_EditPhone);
	DDX_Control(pDX, IDC_EDIT6, m_EditLeftTime);
	DDX_Control(pDX, IDC_COMBO1, m_ComboSerial);
	DDX_Control(pDX, IDC_COMBO2, m_ComboLeave);
	DDX_Control(pDX, IDC_EDIT7, m_EditHardCode);
}


BEGIN_MESSAGE_MAP(CDialog_Modify, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_Modify::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDialog_Modify 消息处理程序


BOOL CDialog_Modify::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDialog_Config* pConfigWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	CDialog_User* pUserWnd = (CDialog_User*)CDialog_User::FromHandle(hUserWnd);

	POSITION pSt_Sition = pUserWnd->m_ListCtrlClient.GetFirstSelectedItemPosition();
	int nItemCount = pUserWnd->m_ListCtrlClient.GetNextSelectedItem(pSt_Sition);
	CString m_StrUser = pUserWnd->m_ListCtrlClient.GetItemText(nItemCount, 1);

	CString m_StrIPAddr;
	CString m_StrIPPort;
	TCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);
	//组合请求URL
	pConfigWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pConfigWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	_stprintf(tszUrlAddr, _T("http://%s:%s/auth/client/get"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());
	//请求用户信息
	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonObject["tszUserName"] = m_StrUser.GetBuffer();
	st_JsonRoot["st_UserInfo"] = st_JsonObject;
	APIHelp_HttpRequest_Post(tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
	
	st_JsonObject.clear();
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		AfxMessageBox(_T("解析客户接口数据错误,无法继续"));
		return FALSE;
	}
	st_JsonObject = st_JsonRoot["st_UserTable"];
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	m_EditUser.SetWindowText(st_JsonObject["st_UserInfo"]["tszUserName"].asCString());
	m_EditPass.SetWindowText(st_JsonObject["st_UserInfo"]["tszUserPass"].asCString());
	m_EditEMail.SetWindowText(st_JsonObject["st_UserInfo"]["tszEMailAddr"].asCString());

	CString m_StrNumber;
	m_StrNumber.Format(_T("%lld"), st_JsonObject["st_UserInfo"]["nIDNumber"].asUInt64());
	m_EditCardID.SetWindowText(m_StrNumber);

	m_StrNumber.ReleaseBuffer();
	m_StrNumber.Format(_T("%lld"), st_JsonObject["st_UserInfo"]["nPhoneNumber"].asUInt64());
	m_EditPhone.SetWindowText(m_StrNumber);

	m_EditHardCode.SetWindowText(st_JsonObject["tszHardCode"].asCString());
	m_EditLeftTime.SetWindowText(st_JsonObject["tszLeftTime"].asCString());

	for (int i = 0; i < 5; i++)
	{
		m_ComboSerial.InsertString(i, lpszKeyType[i]);
	}
	m_ComboSerial.SetCurSel(st_JsonObject["enSerialType"].asInt());

	for (int i = 0; i < 7; i++)
	{
		m_ComboLeave.InsertString(i, lpszLelType[i]);
	}
	m_ComboLeave.SetCurSel(st_JsonObject["st_UserInfo"]["nUserLevel"].asInt() + 1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDialog_Modify::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
}
