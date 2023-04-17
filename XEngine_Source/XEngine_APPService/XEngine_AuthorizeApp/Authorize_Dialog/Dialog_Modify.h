#pragma once
#include "afxdialogex.h"


// CDialog_Modify 对话框

class CDialog_Modify : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_Modify)

public:
	CDialog_Modify(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_Modify();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODIFY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_EditUser;
	CEdit m_EditPass;
	CEdit m_EditEMail;
	CEdit m_EditCardID;
	CEdit m_EditPhone;
	CEdit m_EditLeftTime;
	CComboBox m_ComboSerial;
	CComboBox m_ComboLeave;
	afx_msg void OnBnClickedButton2();
	CEdit m_EditHardCode;
	CEdit m_EditCreateTime;
	CButton m_BtnModify;
	afx_msg void OnBnClickedButton3();
};
