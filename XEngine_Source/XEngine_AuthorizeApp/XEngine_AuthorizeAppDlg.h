
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
	DECLARE_MESSAGE_MAP()
public:
	CDialog_Configure m_DlgConfig;
	CDialog_Serial m_DlgSerial;
	CDialog_User m_DlgUser;
public:
	BOOL bThread;
	int nThreadCount;
	AUTHORIZE_CONFIGURE st_AuthConfig;
	THREADPOOL_PARAMENT** ppSt_ThreadParament;
public:
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
};
