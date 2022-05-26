#pragma once


// CDialog_Info 对话框

class CDialog_Info : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_Info)

public:
	CDialog_Info(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_Info();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODIFY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	AUTHREG_USERTABLE st_UserTable;

	CEdit m_EditUser;
	CEdit m_EditPass;
	CEdit m_EditEMail;
	CEdit m_EditPhone;
	CEdit m_EditCardID;
	CEdit m_EditLeftTime;
	CComboBox m_ComboType;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	CComboBox m_ComboLeave;
};
