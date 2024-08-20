#pragma once
#include "afxdialogex.h"


// CDialog_Switch 对话框

class CDialog_Switch : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_Switch)

public:
	CDialog_Switch(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_Switch();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SWITCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CButton m_RadioDelOPen;
	CButton m_RadioDelClose;
	CButton m_RadioRegOPen;
	CButton m_RadioRegClose;
	CButton m_RadioLoginOPen;
	CButton m_RadioLoginClose;
	CButton m_RadioPayOPen;
	CButton m_RadioPayClose;
	CButton m_RadioPassOPen;
	CButton m_RadioPassClose;
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	CButton m_BtnSetConfigure;
	CButton m_BtnGetConfigure;
	CButton m_RadioCDkeyOPen;
	CButton m_RadioCDKeyClose;
	CButton m_RadioNoticeOPen;
	CButton m_RadioNoticeClose;
	CButton m_RadioDCodeOPen;
	CButton m_RadioDCodeClose;
	CButton m_RadioMultiOPen;
	CButton m_RadioMultiClose;
	CButton m_RadioTryOPen;
	CButton m_RadioTryClose;
	CButton m_RadioBlackOPen;
	CButton m_RadioBlackClose;
	CButton m_RadioTokenOPen;
	CButton m_RadioTokenClose;
};
