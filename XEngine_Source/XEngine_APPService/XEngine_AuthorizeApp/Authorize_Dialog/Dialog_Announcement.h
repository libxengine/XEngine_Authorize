#pragma once
#include "afxdialogex.h"


// CDialog_Announcement 对话框

class CDialog_Announcement : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_Announcement)

public:
	CDialog_Announcement(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_Announcement();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANNOUNCEMENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ListAnnouncement;
	afx_msg void OnBnClickedButton1();
	CEdit m_EditNoticeStr;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
