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
};
