// CDialog_User.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "CDialog_User.h"
#include "afxdialogex.h"


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
	DDX_Control(pDX, IDC_LIST1, m_ListCtrlOnlineClient);
	DDX_Control(pDX, IDC_EDIT1, m_EidtSendMsg);
}


BEGIN_MESSAGE_MAP(CDialog_User, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_User::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_User::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialog_User::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDialog_User::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDialog_User::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK1, &CDialog_User::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CDialog_User 消息处理程序


BOOL CDialog_User::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListCtrlOnlineClient.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
	m_ListCtrlOnlineClient.InsertColumn(1, _T("用户名"), LVCFMT_LEFT, 85);
	m_ListCtrlOnlineClient.InsertColumn(2, _T("地址"), LVCFMT_LEFT, 110);
	m_ListCtrlOnlineClient.InsertColumn(3, _T("在线时间(分钟)"), LVCFMT_LEFT, 90);
	m_ListCtrlOnlineClient.InsertColumn(4, _T("剩余时间/过期时间"), LVCFMT_LEFT, 100);
	m_ListCtrlOnlineClient.InsertColumn(5, _T("充值类型"), LVCFMT_LEFT, 80);
	m_ListCtrlOnlineClient.InsertColumn(6, _T("设备类型"), LVCFMT_LEFT, 60);
	m_ListCtrlOnlineClient.InsertColumn(7, _T("是否在线"), LVCFMT_LEFT, 60);
	m_ListCtrlOnlineClient.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	hUserWnd = this->m_hWnd;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDialog_User::Dialog_User_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszRecvMsg, int nMsgLen)
{
	AUTHREG_USERTABLE st_UserTable;
	memset(&st_UserTable, '\0', sizeof(st_UserTable));

	memcpy_s(&st_UserTable, sizeof(st_UserTable), lpszRecvMsg, sizeof(st_UserTable));

	int nItemCount = m_ListCtrlOnlineClient.GetItemCount();
	m_ListCtrlOnlineClient.InsertItem(nItemCount, _T(""));

	TCHAR tszIndex[10];
	memset(tszIndex, '\0', 10);
	_itot_s(nItemCount, tszIndex, 10);
	m_ListCtrlOnlineClient.SetItemText(nItemCount, 0, tszIndex);
	m_ListCtrlOnlineClient.SetItemText(nItemCount, 1, st_UserTable.st_UserInfo.tszUserName);
	m_ListCtrlOnlineClient.SetItemText(nItemCount, 2, lpszClientAddr);
	m_ListCtrlOnlineClient.SetItemText(nItemCount, 3, st_UserTable.tszLeftTime);
	m_ListCtrlOnlineClient.SetItemText(nItemCount, 4, _T("0"));
}

void CDialog_User::Dialog_User_Leave(LPCTSTR lpszClientAddr)
{
	CString m_StrAddr;
	for (int i = 0; i < m_ListCtrlOnlineClient.GetItemCount(); i++)
	{
		m_StrAddr = m_ListCtrlOnlineClient.GetItemText(i, 2);
		if (0 == _tcsncmp(m_StrAddr.GetBuffer(), lpszClientAddr, m_StrAddr.GetLength()))
		{
			m_ListCtrlOnlineClient.DeleteItem(i);
			break;
		}
	}
}

void CDialog_User::Dialog_User_Set(LPCTSTR lpszClientAddr, LPAUTHREG_USERTABLE pSt_UserTable)
{
	CString m_StrAddr;
	for (int i = 0; i < m_ListCtrlOnlineClient.GetItemCount(); i++)
	{
		m_StrAddr = m_ListCtrlOnlineClient.GetItemText(i, 2);
		if (0 == _tcsncmp(m_StrAddr.GetBuffer(), lpszClientAddr, m_StrAddr.GetLength()))
		{
			m_ListCtrlOnlineClient.SetItemText(i, 3, pSt_UserTable->tszLeftTime);
			break;
		}
	}
}

void CDialog_User::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListCtrlOnlineClient.GetFirstSelectedItemPosition();
	int nSelect = m_ListCtrlOnlineClient.GetNextSelectedItem(pSt_Sition);
	if (nSelect < 0)
	{
		AfxMessageBox(_T("你没有选择任何客户！"));
		return;
	}
	CString m_Str = m_ListCtrlOnlineClient.GetItemText(nSelect, 2);
	if (XEngine_CloseClient(m_Str.GetBuffer(), this))
	{
		AfxMessageBox(_T("成功移除客户端"));
	}
	else
	{
		AfxMessageBox(_T("移除客户端失败"));
	}
}


void CDialog_User::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListCtrlOnlineClient.GetFirstSelectedItemPosition();
	int nItemCount = m_ListCtrlOnlineClient.GetNextSelectedItem(pSt_Sition);
	if (nItemCount < 0)
	{
		AfxMessageBox(_T("你没有选择任何用户！"));
		return;
	}
	CString m_StrSerial = m_ListCtrlOnlineClient.GetItemText(nItemCount, 4);
	CDialog_Info m_DlgInfo;
	m_DlgInfo.DoModal();
}


void CDialog_User::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListCtrlOnlineClient.GetFirstSelectedItemPosition();
	int nSelect = m_ListCtrlOnlineClient.GetNextSelectedItem(pSt_Sition);
	
	if (nSelect < 0)
	{
		AfxMessageBox(_T("你没有选择任何用户！"));
		return;
	}
	if (IDOK != AfxMessageBox(_T("删除用户后此用户不可恢复，是否确定删除？"), MB_OKCANCEL))
	{
		return;
	}
	CString m_StrUser = m_ListCtrlOnlineClient.GetItemText(nSelect, 1);
	CString m_StrIPAddr = m_ListCtrlOnlineClient.GetItemText(nSelect, 2);
	//先关闭
	XEngine_CloseClient(m_StrIPAddr.GetBuffer(), this);
	//在删除
	Database_SQLite_UserDelete(m_StrUser.GetBuffer());
	AfxMessageBox(_T("删除客户成功！"));
}


void CDialog_User::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListCtrlOnlineClient.GetFirstSelectedItemPosition();
	int nSelect = m_ListCtrlOnlineClient.GetNextSelectedItem(pSt_Sition);
	CString m_StrMsg;
	m_EidtSendMsg.GetWindowText(m_StrMsg);
	if (nSelect < 0)
	{
		AfxMessageBox(_T("发送消息失败！未选择客户端"));
		return;
	}
	else
	{
		int nMsgLen = 4096;
		TCHAR tszMsgBuffer[4096];
		XENGINE_PROTOCOLHDR st_ProtocolHdr;

		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

		st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
		st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
		st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_NOTIFYMSG;
		st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
		st_ProtocolHdr.unPacketSize = m_StrMsg.GetLength();

		CString m_StrAddr = m_ListCtrlOnlineClient.GetItemText(nSelect, 2);
		CString m_StrType = m_ListCtrlOnlineClient.GetItemText(nSelect, 6);
		int nNetType = 0;
		if (0 == _tcsncmp(m_StrType.GetBuffer(), _T("WEB"), m_StrType.GetLength()))
		{
			nNetType = XENGINE_AUTH_APP_NETTYPE_WS;
		}
		else
		{
			nNetType = XENGINE_AUTH_APP_NETTYPE_TCP;
		}
		
		if (XEngine_SendMsg(m_StrAddr.GetBuffer(), &st_ProtocolHdr, nNetType, m_StrMsg.GetBuffer(), m_StrMsg.GetLength()))
		{
			AfxMessageBox(_T("发送消息成功！"));
		}
	}
	m_EidtSendMsg.SetWindowText(_T(""));
}


void CDialog_User::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrMsg;
	int nMsgLen = 4096;
	TCHAR tszMsgBuffer[4096];
	XENGINE_PROTOCOLHDR st_ProtocolHdr;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	m_EidtSendMsg.GetWindowText(m_StrMsg);

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_NOTIFYMSG;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	st_ProtocolHdr.unPacketSize = m_StrMsg.GetLength();

	for (int i = 0; i < m_ListCtrlOnlineClient.GetItemCount(); i++)
	{
		CString m_StrAddr = m_ListCtrlOnlineClient.GetItemText(i, 2);
		CString m_StrType = m_ListCtrlOnlineClient.GetItemText(i, 6);
		int nNetType = 0;
		if (0 == _tcsncmp(m_StrType.GetBuffer(), _T("WEB"), m_StrType.GetLength()))
		{
			nNetType = XENGINE_AUTH_APP_NETTYPE_WS;
		}
		else
		{
			nNetType = XENGINE_AUTH_APP_NETTYPE_TCP;
		}
		XEngine_SendMsg(m_StrAddr.GetBuffer(), &st_ProtocolHdr, nNetType, m_StrMsg.GetBuffer(), m_StrMsg.GetLength());
	}
	m_EidtSendMsg.SetWindowText(_T(""));
	AfxMessageBox(_T("发送消息成功！"));
}


void CDialog_User::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
}
