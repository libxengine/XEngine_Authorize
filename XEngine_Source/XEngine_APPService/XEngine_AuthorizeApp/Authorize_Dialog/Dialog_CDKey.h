#pragma once
#include "afxdialogex.h"


// CDialog_CDKey 对话框

class CDialog_CDKey : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_CDKey)

public:
	CDialog_CDKey(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_CDKey();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CDKEY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool Dialog_CDKey_Read(XENGINE_AUTHORIZE_LOCAL* pSt_AuthorizeCDKey);
	bool Dialog_CDKey_Write(XENGINE_AUTHORIZE_LOCAL* pSt_AuthorizeCDKey);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	CIPAddressCtrl m_EditIPAddr;
	CEdit m_EditPort;
	CDateTimeCtrl m_DateTimeCreate;
	CDateTimeCtrl m_DateTimeRegister;
	CDateTimeCtrl m_DateTimeStart;
	CEdit m_EditSoftName;
	CEdit m_EditSoftVer;
	CEdit m_EditSoftTime;
	CButton m_EditSoftInit;
	CEdit m_EditRegHardCode;
	CEdit m_EditRegLeftTime;
	CEdit m_EditRegHaveTime;
	CComboBox m_ComboRegSerial;
	CComboBox m_ComboRegType;
	CComboBox m_ComboRegHard;
	CComboBox m_ComboRegVer;
	CEdit m_EditSerialTimeNumber;
	CEdit m_EditSerialTimeCount;
	CEdit m_EditSerialTimeUse;
	CEdit m_EditSerialDataNumber;
	CDateTimeCtrl m_DataTimeSerial;
	CEdit m_EditSerialUnlimitNumber;
	CEdit m_EditUserInfo;
	CEdit m_EditUserContact;
	CEdit m_EditUserCustom;
	afx_msg void OnBnClickedButton9();
	CDateTimeCtrl m_DataTimeRegExpiry;
};
