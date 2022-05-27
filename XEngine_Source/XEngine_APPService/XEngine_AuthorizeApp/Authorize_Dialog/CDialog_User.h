#pragma once


// CDialog_User 对话框

class CDialog_User : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_User)

public:
	CDialog_User(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_User();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_USER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void Dialog_User_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszRecvMsg, int nMsgLen);
	void Dialog_User_Set(LPCTSTR lpszClientAddr, LPAUTHREG_USERTABLE pSt_UserTable);
public:
	CListCtrl m_ListCtrlOnlineClient;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CEdit m_EidtSendMsg;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedCheck1();
	CButton m_CheckAllUser;
};
