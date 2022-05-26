// CDialog_Info.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "CDialog_Info.h"
#include "afxdialogex.h"


// CDialog_Info 对话框

IMPLEMENT_DYNAMIC(CDialog_Info, CDialogEx)

CDialog_Info::CDialog_Info(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MODIFY, pParent)
{

}

CDialog_Info::~CDialog_Info()
{
}

void CDialog_Info::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditUser);
	DDX_Control(pDX, IDC_EDIT11, m_EditPass);
	DDX_Control(pDX, IDC_EDIT16, m_EditEMail);
	DDX_Control(pDX, IDC_EDIT17, m_EditPhone);
	DDX_Control(pDX, IDC_EDIT18, m_EditCardID);
	DDX_Control(pDX, IDC_EDIT19, m_EditLeftTime);
	DDX_Control(pDX, IDC_COMBO1, m_ComboType);
}


BEGIN_MESSAGE_MAP(CDialog_Info, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON6, &CDialog_Info::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CDialog_Info::OnBnClickedButton7)
END_MESSAGE_MAP()


// CDialog_Info 消息处理程序


BOOL CDialog_Info::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDialog_User* pWnd = (CDialog_User*)CDialog_User::FromHandle(hUserWnd);
	POSITION pSt_Sition = pWnd->m_ListCtrlOnlineClient.GetFirstSelectedItemPosition();
	int nItemCount = pWnd->m_ListCtrlOnlineClient.GetNextSelectedItem(pSt_Sition);

	CString m_StrUser = pWnd->m_ListCtrlOnlineClient.GetItemText(nItemCount, 1);
	memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

	Database_SQLite_UserQuery(m_StrUser.GetBuffer(), &st_UserTable);

	m_EditUser.SetWindowText(m_StrUser);
	m_EditPass.SetWindowText(st_UserTable.st_UserInfo.tszUserPass);
	m_EditEMail.SetWindowText(st_UserTable.st_UserInfo.tszEMailAddr);

	CString m_StrNumber;
	m_StrNumber.Format(_T("%lld"), st_UserTable.st_UserInfo.nIDNumber);
	m_EditCardID.SetWindowText(m_StrNumber);

	m_StrNumber.ReleaseBuffer();
	m_StrNumber.Format(_T("%lld"), st_UserTable.st_UserInfo.nPhoneNumber);
	m_EditPhone.SetWindowText(m_StrNumber);

	m_EditLeftTime.SetWindowText(st_UserTable.tszLeftTime);
	m_ComboType.InsertString(0, _T("未知"));
	m_ComboType.InsertString(1, _T("分钟"));
	m_ComboType.InsertString(2, _T("天数"));
	m_ComboType.InsertString(3, _T("次数"));
	m_ComboType.InsertString(4, _T("自定义"));
	m_ComboType.SetCurSel(st_UserTable.en_AuthRegSerialType);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDialog_Info::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	memset(st_UserTable.st_UserInfo.tszUserName, '\0', sizeof(st_UserTable.st_UserInfo.tszUserName));
	memset(st_UserTable.st_UserInfo.tszUserPass, '\0', sizeof(st_UserTable.st_UserInfo.tszUserPass));
	memset(st_UserTable.st_UserInfo.tszEMailAddr, '\0', sizeof(st_UserTable.st_UserInfo.tszEMailAddr));
	memset(st_UserTable.tszLeftTime, '\0', sizeof(st_UserTable.tszLeftTime));

	m_EditUser.GetWindowText(st_UserTable.st_UserInfo.tszUserName, sizeof(st_UserTable.st_UserInfo.tszUserName));
	m_EditPass.GetWindowText(st_UserTable.st_UserInfo.tszUserPass, sizeof(st_UserTable.st_UserInfo.tszUserPass));
	m_EditEMail.GetWindowText(st_UserTable.st_UserInfo.tszEMailAddr, sizeof(st_UserTable.st_UserInfo.tszEMailAddr));
	m_EditLeftTime.GetWindowText(st_UserTable.tszLeftTime, sizeof(st_UserTable.tszLeftTime));

	CString m_StrNumber;
	m_EditPhone.GetWindowText(m_StrNumber);
	st_UserTable.st_UserInfo.nPhoneNumber = _ttoi64(m_StrNumber.GetBuffer());

	m_StrNumber.ReleaseBuffer();
	m_EditCardID.GetWindowText(m_StrNumber);
	st_UserTable.st_UserInfo.nIDNumber = _ttoi64(m_StrNumber.GetBuffer());

	st_UserTable.en_AuthRegSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)m_ComboType.GetCurSel();
	if (Database_SQLite_UserSet(&st_UserTable))
	{
		Session_Authorize_SetUser(&st_UserTable);
		AfxMessageBox(_T("修改成功"));
	}
	else
	{
		AfxMessageBox(_T("修改失败"));
	}
	OnOK();
}


void CDialog_Info::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
