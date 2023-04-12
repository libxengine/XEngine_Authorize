#pragma once
#include "afxdialogex.h"


// CDialog_Banned 对话框

class CDialog_Banned : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_Banned)

public:
	CDialog_Banned(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_Banned();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BANNED };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	CButton m_RadioIPAddr;
	CButton m_RadioUser;
	CStatic m_StaticTips;
	afx_msg void OnBnClickedRadio2();
	virtual XBOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	CListCtrl m_ListAddr;
	CListCtrl m_ListUser;
	CEdit m_EditUser;
	CDateTimeCtrl m_DataTime;
	afx_msg void OnBnClickedCheck1();
	CButton m_BtnCheckTime;
	CButton m_RadioEnable;
	CButton m_RadioDisable;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnNMClickList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult);
};
