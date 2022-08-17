﻿#pragma once
#include "afxdialogex.h"


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
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtrlClient;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};