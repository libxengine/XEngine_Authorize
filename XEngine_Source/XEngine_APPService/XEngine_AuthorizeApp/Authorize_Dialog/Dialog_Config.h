#pragma once
#include "afxdialogex.h"


// CDialog_Config 对话框

class CDialog_Config : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_Config)

public:
	CDialog_Config(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_Config();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_EditIPAddr;
	CEdit m_EditIPPort;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CEdit m_EditUser;
	CEdit m_EditPass;
	CEdit m_EditToken;
	CButton m_BtnLogin;
	CButton m_BtnLogout;
	CEdit m_EditTimeout;
	afx_msg void OnBnClickedButton5();
	CButton m_BtnUpdate;
	CButton m_CheckCodecEnable;
	CButton m_CheckCodecDisable;
	CComboBox m_ListEncrypto;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	CEdit m_EditPassword;
	CEdit m_EditDCode;
	afx_msg void OnBnClickedButton8();
	CComboBox m_ComboPassCodec;
	CButton m_RadioPassDisable;
	CButton m_RadioPassEnable;
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
};
