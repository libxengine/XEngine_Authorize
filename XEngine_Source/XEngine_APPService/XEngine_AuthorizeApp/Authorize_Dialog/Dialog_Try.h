#pragma once
#include "afxdialogex.h"


// CDialog_Try 对话框

class CDialog_Try : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_Try)

public:
	CDialog_Try(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_Try();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TRY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	CListCtrl m_ListTry;
	CEdit m_EditPosStart;
	CEdit m_EditPosEnd;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	CEdit m_EditSerial;
	CEdit m_EditTime;
	CEdit m_EditDate;
	CComboBox m_ComboType;
	afx_msg void OnBnClickedButton3();
};
