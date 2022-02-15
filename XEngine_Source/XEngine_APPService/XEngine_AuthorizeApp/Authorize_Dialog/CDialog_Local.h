#pragma once


// CDialog_Local 对话框

class CDialog_Local : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_Local)

public:
	CDialog_Local(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_Local();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOCAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	CEdit m_EditKeyFile;
	CEdit m_EditPass;
	CEdit m_EditAPPName;
	CEdit m_EditAPPVersion;
	CComboBox m_ComboHWType;
	CComboBox m_ComboRegType;
	virtual BOOL OnInitDialog();
	CComboBox m_ComboSerialType;
	afx_msg void OnEnChangeEdit4();
	CEdit m_EditMachineCode;
	CEdit m_EditRegTime;
	CEdit m_EditHaveTime;
	CEdit m_EditLeftTime;
	CEdit m_EditUser;
	CEdit m_EditContact;
	CEdit m_EditCustomInfo;
	CEdit m_EditPort;
	CIPAddressCtrl m_EditIPAddr;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnChangeEdit12();
	void Dialog_Local_GetInfo(XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal);
	CEdit m_EditCreateTime;
	CButton m_BtnKeySave;
};
