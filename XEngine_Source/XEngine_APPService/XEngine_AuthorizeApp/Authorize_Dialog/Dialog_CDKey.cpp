// Dialog_CDKey.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "afxdialogex.h"
#include "Dialog_CDKey.h"


// CDialog_CDKey 对话框

IMPLEMENT_DYNAMIC(CDialog_CDKey, CDialogEx)

CDialog_CDKey::CDialog_CDKey(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CDKEY, pParent)
{

}

CDialog_CDKey::~CDialog_CDKey()
{
}

void CDialog_CDKey::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_EditIPAddr);
	DDX_Control(pDX, IDC_EDIT1, m_EditPort);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateTimeCreate);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_DateTimeRegister);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_DateTimeStart);
	DDX_Control(pDX, IDC_EDIT2, m_EditSoftName);
	DDX_Control(pDX, IDC_EDIT3, m_EditSoftVer);
	DDX_Control(pDX, IDC_EDIT4, m_EditSoftTime);
	DDX_Control(pDX, IDC_CHECK1, m_EditSoftInit);
	DDX_Control(pDX, IDC_EDIT15, m_EditRegHardCode);
	DDX_Control(pDX, IDC_EDIT18, m_EditRegLeftTime);
	DDX_Control(pDX, IDC_EDIT19, m_EditRegHaveTime);
	DDX_Control(pDX, IDC_COMBO3, m_ComboRegSerial);
	DDX_Control(pDX, IDC_COMBO4, m_ComboRegType);
	DDX_Control(pDX, IDC_COMBO5, m_ComboRegHard);
	DDX_Control(pDX, IDC_COMBO6, m_ComboRegVer);
	DDX_Control(pDX, IDC_EDIT17, m_EditSerialTimeNumber);
	DDX_Control(pDX, IDC_EDIT16, m_EditSerialTimeCount);
	DDX_Control(pDX, IDC_EDIT20, m_EditSerialTimeUse);
	DDX_Control(pDX, IDC_EDIT22, m_EditSerialDataNumber);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_DataTimeSerial);
	DDX_Control(pDX, IDC_EDIT21, m_EditSerialUnlimitNumber);
	DDX_Control(pDX, IDC_EDIT23, m_EditUserInfo);
	DDX_Control(pDX, IDC_EDIT24, m_EditUserContact);
	DDX_Control(pDX, IDC_EDIT25, m_EditUserCustom);
}


BEGIN_MESSAGE_MAP(CDialog_CDKey, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_CDKey::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON9, &CDialog_CDKey::OnBnClickedButton9)
END_MESSAGE_MAP()


// CDialog_CDKey 消息处理程序


BOOL CDialog_CDKey::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_EditIPAddr.SetWindowText(_T("127.0.0.1"));
	m_EditPort.SetWindowText(_T("5300"));

	m_EditSoftName.SetWindowText(_T("XEngine_Authorze"));
	m_EditSoftVer.SetWindowText(_T("V1.0.0.1001"));
	m_EditSoftTime.SetWindowText(_T("0"));

	m_DateTimeCreate.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_DateTimeRegister.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_DateTimeStart.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_DataTimeSerial.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));

	for (int i = 0; i < 5; i++)
	{
		m_ComboRegSerial.InsertString(i, lpszXSerialType[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		m_ComboRegType.InsertString(i, lpszXRegType[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		m_ComboRegHard.InsertString(i, lpszXHDType[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		m_ComboRegVer.InsertString(i, lpszXVerType[i]);
	}
	m_ComboRegSerial.SetCurSel(0);
	m_ComboRegType.SetCurSel(0);
	m_ComboRegHard.SetCurSel(0);
	m_ComboRegVer.SetCurSel(0);

	int nSerialCount = 3;
	XCHAR** pptszSerialList;
	Authorize_Serial_Create(&pptszSerialList, _T("XAUTH"), nSerialCount, 9);
	m_EditSerialTimeNumber.SetWindowText(pptszSerialList[0]);
	m_EditSerialDataNumber.SetWindowText(pptszSerialList[1]);
	m_EditSerialUnlimitNumber.SetWindowText(pptszSerialList[2]);
	BaseLib_OperatorMemory_Free((XPPPMEM)&pptszSerialList, nSerialCount);
	m_EditSerialTimeCount.SetWindowText(_T("10"));
	m_EditSerialTimeUse.SetWindowText(_T("0"));

	m_EditUserInfo.SetWindowText(_T("XEngine"));
	m_EditUserContact.SetWindowText(_T("www.xyry.org"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

bool CDialog_CDKey::Dialog_CDKey_Read(XENGINE_AUTHORIZE_LOCAL* pSt_AuthorizeCDKey)
{
	//网络信息
	CString m_StrIPPort;
	m_EditIPAddr.GetWindowText(pSt_AuthorizeCDKey->tszAddr, sizeof(pSt_AuthorizeCDKey->tszAddr));
	m_EditPort.GetWindowText(m_StrIPPort);
	pSt_AuthorizeCDKey->nPort = _ttoi(m_StrIPPort.GetString());
	//程序信息
	CString m_StrSoftTime;
	m_EditSoftName.GetWindowText(pSt_AuthorizeCDKey->st_AuthAppInfo.tszAppName, sizeof(pSt_AuthorizeCDKey->st_AuthAppInfo.tszAppName));
	m_EditSoftVer.GetWindowText(pSt_AuthorizeCDKey->st_AuthAppInfo.tszAppVer, sizeof(pSt_AuthorizeCDKey->st_AuthAppInfo.tszAppVer));
	m_EditSoftTime.GetWindowText(m_StrSoftTime);
	if (BST_CHECKED == m_EditSoftInit.GetCheck())
	{
		pSt_AuthorizeCDKey->st_AuthAppInfo.bInit = true;
	}
	//注册信息
	CString m_StrRegHaveTime;
	m_EditRegHardCode.GetWindowText(pSt_AuthorizeCDKey->st_AuthRegInfo.tszHardware, sizeof(pSt_AuthorizeCDKey->st_AuthRegInfo.tszHardware));
	m_EditRegLeftTime.GetWindowText(pSt_AuthorizeCDKey->st_AuthRegInfo.tszLeftTime, sizeof(pSt_AuthorizeCDKey->st_AuthRegInfo.tszLeftTime));
	m_EditRegHaveTime.GetWindowText(m_StrRegHaveTime);
	pSt_AuthorizeCDKey->st_AuthRegInfo.nHasTime = _ttoi64(m_StrRegHaveTime.GetBuffer());
	m_DateTimeCreate.GetWindowText(pSt_AuthorizeCDKey->st_AuthRegInfo.tszCreateTime, sizeof(pSt_AuthorizeCDKey->st_AuthRegInfo.tszCreateTime));
	m_DateTimeRegister.GetWindowText(pSt_AuthorizeCDKey->st_AuthRegInfo.tszRegisterTime, sizeof(pSt_AuthorizeCDKey->st_AuthRegInfo.tszRegisterTime));
	m_DateTimeStart.GetWindowText(pSt_AuthorizeCDKey->st_AuthRegInfo.tszStartTime, sizeof(pSt_AuthorizeCDKey->st_AuthRegInfo.tszStartTime));
	pSt_AuthorizeCDKey->st_AuthRegInfo.enSerialType = (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE)m_ComboRegSerial.GetCurSel();
	pSt_AuthorizeCDKey->st_AuthRegInfo.enRegType = (ENUM_AUTHORIZE_MODULE_CDKEY_TYPE)m_ComboRegType.GetCurSel();
	pSt_AuthorizeCDKey->st_AuthRegInfo.enHWType = (ENUM_AUTHORIZE_MODULE_HW_TYPE)m_ComboRegHard.GetCurSel();
	pSt_AuthorizeCDKey->st_AuthRegInfo.enVModeType = (ENUM_AUTHORIZE_MODULE_VERMODE_TYPE)m_ComboRegVer.GetCurSel();
	//序列信息
	CString m_StrSerialCount;
	CString m_StrSerialUsed;
	m_EditSerialTimeCount.GetWindowText(m_StrSerialCount);
	m_EditSerialTimeUse.GetWindowText(m_StrSerialUsed);
	m_EditSerialTimeNumber.GetWindowText(pSt_AuthorizeCDKey->st_AuthSerial.st_TimeLimit.tszTimeSerial, sizeof(pSt_AuthorizeCDKey->st_AuthSerial.st_TimeLimit.tszTimeSerial));
	pSt_AuthorizeCDKey->st_AuthSerial.st_TimeLimit.nTimeCount = _ttoi(m_StrSerialCount.GetBuffer());
	pSt_AuthorizeCDKey->st_AuthSerial.st_TimeLimit.nTimeNow = _ttoi(m_StrSerialUsed.GetBuffer());

	m_EditSerialDataNumber.GetWindowText(pSt_AuthorizeCDKey->st_AuthSerial.st_DataLimit.tszDataSerial, sizeof(pSt_AuthorizeCDKey->st_AuthSerial.st_DataLimit.tszDataSerial));
	m_DataTimeSerial.GetWindowText(pSt_AuthorizeCDKey->st_AuthSerial.st_DataLimit.tszDataTime, sizeof(pSt_AuthorizeCDKey->st_AuthSerial.st_DataLimit.tszDataTime));

	m_EditSerialUnlimitNumber.GetWindowText(pSt_AuthorizeCDKey->st_AuthSerial.st_UNLimit.tszUNLimitSerial, sizeof(pSt_AuthorizeCDKey->st_AuthSerial.st_UNLimit.tszUNLimitSerial));
	//用户信息
	m_EditUserInfo.GetWindowText(pSt_AuthorizeCDKey->st_AuthUserInfo.tszUserName, sizeof(pSt_AuthorizeCDKey->st_AuthUserInfo.tszUserName));
	m_EditUserContact.GetWindowText(pSt_AuthorizeCDKey->st_AuthUserInfo.tszUserContact, sizeof(pSt_AuthorizeCDKey->st_AuthUserInfo.tszUserContact));
	m_EditUserCustom.GetWindowText(pSt_AuthorizeCDKey->st_AuthUserInfo.tszCustom, sizeof(pSt_AuthorizeCDKey->st_AuthUserInfo.tszCustom));
	return true;
}
bool CDialog_CDKey::Dialog_CDKey_Write(XENGINE_AUTHORIZE_LOCAL* pSt_AuthorizeCDKey)
{
	CString m_StrFormat;
	//网络信息
	return true;
}
void CDialog_CDKey::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (0 == m_ComboRegHard.GetCurSel() || 0 == m_ComboRegSerial.GetCurSel() || 0 == m_ComboRegType.GetCurSel() || 0 == m_ComboRegVer.GetCurSel())
	{
		AfxMessageBox(_T("创建失败,注册验证类型没选择"));
		return;
	}

	XENGINE_AUTHORIZE_LOCAL st_AuthorizeCDKey = {};
	Dialog_CDKey_Read(&st_AuthorizeCDKey);

	CFileDialog m_FileDlg(false, _T(".key"),_T("CDKey"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CDKey文件(*.key)|*.key|"));
	if (IDOK == m_FileDlg.DoModal())
	{
		Authorize_CDKey_WriteKey(m_FileDlg.GetPathName(), &st_AuthorizeCDKey);
	}
}

void CDialog_CDKey::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	XENGINE_AUTHORIZE_LOCAL st_AuthorizeCDKey = {};

	CFileDialog m_FileDlg(true, _T(".key"), _T("CDKey"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CDKey文件(*.key)|*.key|"));
	if (IDOK == m_FileDlg.DoModal())
	{
		if (!Authorize_CDKey_ReadKey(m_FileDlg.GetPathName(), &st_AuthorizeCDKey))
		{
			AfxMessageBox(_T("CDKEY读取失败"));
			return;
		}

	}
}
