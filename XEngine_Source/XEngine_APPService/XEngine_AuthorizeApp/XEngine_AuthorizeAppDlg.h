
// XEngine_AuthorizeAppDlg.h: 头文件
//

#pragma once
// CXEngineAuthorizeAppDlg 对话框
class CXEngineAuthorizeAppDlg : public CDialogEx
{
// 构造
public:
	CXEngineAuthorizeAppDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XENGINE_AUTHORIZEAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnAddTrayIcon(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CDialog_Configure m_DlgConfig;
	CDialog_Serial m_DlgSerial;
	CDialog_User m_DlgUser;
	CDialog_Local m_DlgLocal;
public:
	BOOL bThread;
	int nThreadCount;
	THREADPOOL_PARAMENT** ppSt_ThreadTCPParament;
	THREADPOOL_PARAMENT** ppSt_ThreadWSParament;
public:
	BOOL bShow;
	CEdit m_EditLog;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CButton m_BtnStartService;
	CButton m_BtnStopService;
	CTabCtrl m_TabCtrl;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnDestroy();
	afx_msg void On32771();
	afx_msg void On32772();
	afx_msg void On32773();
	afx_msg void On32774();
	afx_msg void On32775();
};
