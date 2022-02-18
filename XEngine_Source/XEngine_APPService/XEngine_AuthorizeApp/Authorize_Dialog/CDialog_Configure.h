#pragma once


// CDialog_Configure 对话框

class CDialog_Configure : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_Configure)

public:
	CDialog_Configure(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_Configure();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void AuthorizeService_ReadConfigure();
	void AuthorizeService_WriteConfigure();
public:
	virtual BOOL OnInitDialog();
	CEdit m_EditServicePort;
	CEdit m_EditThreadPool;
	CEdit m_EditVerTimedout;
	CEdit m_EditPass;
	CButton m_BtnAutoStart;
	CButton m_BtnAutoStop;
	CComboBox m_ComboRegTry;
	CComboBox m_ComboListAuth;
	CButton m_RadioKeyPass;
	CButton m_RadioKeyDisable;
	CEdit m_EditTryTime;
	CEdit m_EditAuthTime;
	CEdit m_EditSmtpAddr;
	CEdit m_EditFromAddr;
	CEdit m_EditSmtpUser;
	CEdit m_EditSmtpPass;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	CEdit m_EditWSPort;
	CButton m_EditTimeNotift;
	CButton m_CheckEnableEmail;
	afx_msg void OnBnClickedCheck3();
};
