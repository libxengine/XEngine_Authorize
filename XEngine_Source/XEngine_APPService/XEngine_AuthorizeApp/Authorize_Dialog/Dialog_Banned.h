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
};
