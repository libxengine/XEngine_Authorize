#pragma once


// CDialog_Serial 对话框

class CDialog_Serial : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_Serial)

public:
	CDialog_Serial(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_Serial();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SERIAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SerialManage_Flush();
public:
	CListCtrl m_ListSerial;
	CComboBox m_ComboSerialType;
	CComboBox m_ComboNumber;
	CEdit m_EditHowCard;
	CEdit m_EditCardTime;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
