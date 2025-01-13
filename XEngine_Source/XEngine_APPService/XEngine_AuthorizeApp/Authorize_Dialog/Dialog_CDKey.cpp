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
	DDX_Control(pDX, IDC_EDIT22, m_EditSerialDataNumber);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_DataTimeSerial);
	DDX_Control(pDX, IDC_EDIT21, m_EditSerialUnlimitNumber);
	DDX_Control(pDX, IDC_EDIT23, m_EditUserInfo);
	DDX_Control(pDX, IDC_EDIT24, m_EditUserContact);
	DDX_Control(pDX, IDC_EDIT25, m_EditUserCustom);
	DDX_Control(pDX, IDC_DATETIMEPICKER5, m_DataTimeRegExpiry);
	DDX_Control(pDX, IDC_EDIT26, m_EditKeyPass);
	DDX_Control(pDX, IDC_CHECK3, m_CheckSerialDataAdd);
	DDX_Control(pDX, IDC_EDIT11, m_EditIPAddr);
}


BEGIN_MESSAGE_MAP(CDialog_CDKey, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_CDKey::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON9, &CDialog_CDKey::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CDialog_CDKey::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CDialog_CDKey::OnBnClickedButton11)
END_MESSAGE_MAP()


// CDialog_CDKey 消息处理程序


BOOL CDialog_CDKey::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_DateTimeCreate.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_DateTimeRegister.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_DateTimeStart.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_DataTimeSerial.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_DataTimeRegExpiry.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));

	for (int i = 0; i < 5; i++)
	{
		USES_CONVERSION;
		m_ComboRegSerial.InsertString(i, A2W(lpszXSerialType[i]));
	}
	for (int i = 0; i < 6; i++)
	{
		USES_CONVERSION;
		m_ComboRegType.InsertString(i, A2W(lpszXRegType[i]));
	}
	for (int i = 0; i < 6; i++)
	{
		USES_CONVERSION;
		m_ComboRegHard.InsertString(i, A2W(lpszXHDType[i]));
	}
	for (int i = 0; i < 3; i++)
	{
		USES_CONVERSION;
		m_ComboRegVer.InsertString(i, A2W(lpszXVerType[i]));
	}
	m_ComboRegSerial.SetCurSel(0);
	m_ComboRegType.SetCurSel(0);
	m_ComboRegHard.SetCurSel(0);
	m_ComboRegVer.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

bool CDialog_CDKey::Dialog_CDKey_Init()
{
	m_ComboRegSerial.SetCurSel(3);
	m_ComboRegType.SetCurSel(2);
	m_ComboRegHard.SetCurSel(4);
	m_ComboRegVer.SetCurSel(1);

	m_EditIPAddr.SetWindowText(_T("http://app.xyry.org"));
	m_EditPort.SetWindowText(_T("5302"));

	m_EditSoftName.SetWindowText(_T("XEngine_AuthorzeApp"));
	m_EditSoftVer.SetWindowText(_T("V1.0.0.1001"));
	m_EditSoftTime.SetWindowText(_T("0"));

	m_EditRegHaveTime.SetWindowText(_T("5"));
	m_EditRegLeftTime.SetWindowText(_T("5"));

	int nSerialCount = 3;
	XCHAR** pptszSerialList;
	USES_CONVERSION;
	Authorize_Serial_Create(&pptszSerialList, _X("XAUTH"), nSerialCount, 9);
	m_EditSerialTimeNumber.SetWindowText(A2W(pptszSerialList[0]));
	m_EditSerialDataNumber.SetWindowText(A2W(pptszSerialList[1]));
	m_EditSerialUnlimitNumber.SetWindowText(A2W(pptszSerialList[2]));
	BaseLib_Memory_Free((XPPPMEM)&pptszSerialList, nSerialCount);
	m_EditSerialTimeCount.SetWindowText(_T("9999"));
	m_CheckSerialDataAdd.SetCheck(BST_CHECKED);

	XCHAR tszTimeStr[128] = {};
	XENGINE_LIBTIME st_LibTime = {};
	BaseLib_Time_GetSysTime(&st_LibTime);

	st_LibTime.wYear += 1; //一年后过期
	BaseLib_Time_TimeToStr(tszTimeStr, NULL, true, &st_LibTime);

	COleDateTime m_OleDTime;
	// 尝试解析字符串为日期和时间
	if (m_OleDTime.ParseDateTime(A2W(tszTimeStr)))
	{
		m_DataTimeSerial.SetTime(m_OleDTime);
	}

	m_EditUserInfo.SetWindowText(_T("XEngine"));
	m_EditUserContact.SetWindowText(_T("www.xyry.org"));
	return true;
}
bool CDialog_CDKey::Dialog_CDKey_Read(XENGINE_AUTHORIZE_LOCAL* pSt_AuthorizeCDKey)
{
	//网络信息
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrAPPName;
	CString m_StrAPPVer;
	m_EditIPAddr.GetWindowText(m_StrIPAddr);
	m_EditPort.GetWindowText(m_StrIPPort);

	USES_CONVERSION;
	strcpy(pSt_AuthorizeCDKey->tszAddr, W2A(m_StrIPAddr.GetBuffer()));
	pSt_AuthorizeCDKey->nPort = _ttoi(m_StrIPPort.GetString());
	//程序信息
	CString m_StrSoftTime;
	m_EditSoftName.GetWindowText(m_StrAPPName);
	m_EditSoftVer.GetWindowText(m_StrAPPVer);
	m_EditSoftTime.GetWindowText(m_StrSoftTime);
	if (BST_CHECKED == m_EditSoftInit.GetCheck())
	{
		pSt_AuthorizeCDKey->st_AuthAppInfo.bInit = true;
	}
	strcpy(pSt_AuthorizeCDKey->st_AuthAppInfo.tszAppName, W2A(m_StrAPPName.GetBuffer()));
	strcpy(pSt_AuthorizeCDKey->st_AuthAppInfo.tszAppVer, W2A(m_StrAPPVer.GetBuffer()));
	//注册信息
	CString m_StrRegInfo;
	m_EditRegHardCode.GetWindowText(m_StrRegInfo);
	strcpy(pSt_AuthorizeCDKey->st_AuthRegInfo.tszHardware, W2A(m_StrRegInfo.GetBuffer()));
	m_StrRegInfo.ReleaseBuffer();
	m_EditRegLeftTime.GetWindowText(m_StrRegInfo);
	strcpy(pSt_AuthorizeCDKey->st_AuthRegInfo.tszLeftTime, W2A(m_StrRegInfo.GetBuffer()));
	m_StrRegInfo.ReleaseBuffer();
	m_EditRegHaveTime.GetWindowText(m_StrRegInfo);
	pSt_AuthorizeCDKey->st_AuthRegInfo.nHasTime = _ttoi64(m_StrRegInfo.GetBuffer());
	m_StrRegInfo.ReleaseBuffer();
	m_DateTimeCreate.GetWindowText(m_StrRegInfo);
	strcpy(pSt_AuthorizeCDKey->st_AuthRegInfo.tszCreateTime, W2A(m_StrRegInfo.GetBuffer()));
	//m_DateTimeRegister.GetWindowText(pSt_AuthorizeCDKey->st_AuthRegInfo.tszRegisterTime, sizeof(pSt_AuthorizeCDKey->st_AuthRegInfo.tszRegisterTime));
	m_StrRegInfo.ReleaseBuffer();
	m_DateTimeStart.GetWindowText(m_StrRegInfo);
	strcpy(pSt_AuthorizeCDKey->st_AuthRegInfo.tszStartTime, W2A(m_StrRegInfo.GetBuffer()));
	pSt_AuthorizeCDKey->st_AuthRegInfo.enSerialType = (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE)m_ComboRegSerial.GetCurSel();
	pSt_AuthorizeCDKey->st_AuthRegInfo.enRegType = (ENUM_AUTHORIZE_MODULE_CDKEY_TYPE)m_ComboRegType.GetCurSel();
	pSt_AuthorizeCDKey->st_AuthRegInfo.enHWType = (ENUM_AUTHORIZE_MODULE_HW_TYPE)m_ComboRegHard.GetCurSel();
	pSt_AuthorizeCDKey->st_AuthRegInfo.enVModeType = (ENUM_AUTHORIZE_MODULE_VERMODE_TYPE)m_ComboRegVer.GetCurSel();
	//序列信息
	CString m_StrSerialInfo;
	m_EditSerialTimeCount.GetWindowText(m_StrSerialInfo);
	pSt_AuthorizeCDKey->st_AuthSerial.st_TimeLimit.nTimeCount = _ttoi(m_StrSerialInfo.GetBuffer());
	m_StrSerialInfo.ReleaseBuffer();
	m_EditSerialTimeNumber.GetWindowText(m_StrSerialInfo);
	strcpy(pSt_AuthorizeCDKey->st_AuthSerial.st_TimeLimit.tszTimeSerial, W2A(m_StrSerialInfo.GetBuffer()));
	m_StrSerialInfo.ReleaseBuffer();
	m_EditSerialDataNumber.GetWindowText(m_StrSerialInfo);
	strcpy(pSt_AuthorizeCDKey->st_AuthSerial.st_DataLimit.tszDataSerial, W2A(m_StrSerialInfo.GetBuffer()));
	m_StrSerialInfo.ReleaseBuffer();
	m_DataTimeSerial.GetWindowText(m_StrSerialInfo);
	strcpy(pSt_AuthorizeCDKey->st_AuthSerial.st_DataLimit.tszDataTime, W2A(m_StrSerialInfo.GetBuffer()));

	if (BST_CHECKED == m_CheckSerialDataAdd.GetCheck())
	{
		pSt_AuthorizeCDKey->st_AuthSerial.st_DataLimit.bTimeAdd = true;
	}
	m_StrSerialInfo.ReleaseBuffer();
	m_EditSerialUnlimitNumber.GetWindowText(m_StrSerialInfo);
	strcpy(pSt_AuthorizeCDKey->st_AuthSerial.st_UNLimit.tszUNLimitSerial, W2A(m_StrSerialInfo.GetBuffer()));
	//用户信息
	CString m_StrUserInfo;
	m_EditUserInfo.GetWindowText(m_StrUserInfo);
	strcpy(pSt_AuthorizeCDKey->st_AuthUserInfo.tszUserName, W2A(m_StrUserInfo.GetBuffer()));
	m_StrUserInfo.ReleaseBuffer();
	m_EditUserContact.GetWindowText(m_StrUserInfo);
	strcpy(pSt_AuthorizeCDKey->st_AuthUserInfo.tszUserContact, W2A(m_StrUserInfo.GetBuffer()));
	m_EditUserContact.GetWindowText(m_StrUserInfo);
	m_EditUserCustom.GetWindowText(m_StrUserInfo);
	strcpy(pSt_AuthorizeCDKey->st_AuthUserInfo.tszCustom, W2A(m_StrUserInfo.GetBuffer()));
	return true;
}
bool CDialog_CDKey::Dialog_CDKey_Write(XENGINE_AUTHORIZE_LOCAL* pSt_AuthorizeCDKey)
{
	CString m_StrFormat;
	//网络信息
	USES_CONVERSION;
	m_EditIPAddr.SetWindowText(A2W(pSt_AuthorizeCDKey->tszAddr));
	m_StrFormat.Format(_T("%d"), pSt_AuthorizeCDKey->nPort);
	m_EditPort.SetWindowText(m_StrFormat);
	//软件信息
	m_EditSoftName.SetWindowText(A2W(pSt_AuthorizeCDKey->st_AuthAppInfo.tszAppName));
	m_EditSoftVer.SetWindowText(A2W(pSt_AuthorizeCDKey->st_AuthAppInfo.tszAppVer));
	m_StrFormat.Format(_T("%lld"), pSt_AuthorizeCDKey->st_AuthAppInfo.nExecTime);
	m_EditSoftTime.SetWindowText(m_StrFormat);
	if (pSt_AuthorizeCDKey->st_AuthAppInfo.bInit)
	{
		m_EditSoftInit.SetCheck(BST_CHECKED);
	}
	//注册信息
	m_EditRegHardCode.SetWindowText(A2W(pSt_AuthorizeCDKey->st_AuthRegInfo.tszHardware));
	m_EditRegLeftTime.SetWindowText(A2W(pSt_AuthorizeCDKey->st_AuthRegInfo.tszLeftTime));
	m_StrFormat.Format(_T("%lld"), pSt_AuthorizeCDKey->st_AuthRegInfo.nHasTime);
	m_EditRegHaveTime.SetWindowText(m_StrFormat);
	COleDateTime m_OleDTime;
	// 尝试解析字符串为日期和时间
	if (m_OleDTime.ParseDateTime(A2W(pSt_AuthorizeCDKey->st_AuthRegInfo.tszCreateTime)))
	{
		m_DateTimeCreate.SetTime(m_OleDTime);
	}
	if (m_OleDTime.ParseDateTime(A2W(pSt_AuthorizeCDKey->st_AuthRegInfo.tszRegisterTime)))
	{
		m_DateTimeRegister.SetTime(m_OleDTime);
	}
	if (m_OleDTime.ParseDateTime(A2W(pSt_AuthorizeCDKey->st_AuthRegInfo.tszStartTime)))
	{
		m_DateTimeStart.SetTime(m_OleDTime);
	}
	if (m_OleDTime.ParseDateTime(A2W(pSt_AuthorizeCDKey->st_AuthRegInfo.tszExpiryTime)))
	{
		m_DataTimeSerial.SetTime(m_OleDTime);
	}

	m_ComboRegSerial.SetCurSel(pSt_AuthorizeCDKey->st_AuthRegInfo.enSerialType);
	m_ComboRegType.SetCurSel(pSt_AuthorizeCDKey->st_AuthRegInfo.enRegType);
	m_ComboRegHard.SetCurSel(pSt_AuthorizeCDKey->st_AuthRegInfo.enHWType);
	m_ComboRegVer.SetCurSel(pSt_AuthorizeCDKey->st_AuthRegInfo.enVModeType);
	//序列信息
	m_StrFormat.Format(_T("%d"), pSt_AuthorizeCDKey->st_AuthSerial.st_TimeLimit.nTimeCount);
	m_EditSerialTimeCount.SetWindowText(m_StrFormat);
	m_EditSerialTimeNumber.SetWindowText(A2W(pSt_AuthorizeCDKey->st_AuthSerial.st_TimeLimit.tszTimeSerial));

	if (pSt_AuthorizeCDKey->st_AuthSerial.st_DataLimit.bTimeAdd)
	{
		m_CheckSerialDataAdd.SetCheck(BST_CHECKED);
	}
	m_EditSerialDataNumber.SetWindowText(A2W(pSt_AuthorizeCDKey->st_AuthSerial.st_DataLimit.tszDataSerial));
	// 尝试解析字符串为日期和时间
	if (m_OleDTime.ParseDateTime(A2W(pSt_AuthorizeCDKey->st_AuthSerial.st_DataLimit.tszDataTime)))
	{
		m_DataTimeSerial.SetTime(m_OleDTime);
	}

	m_EditSerialUnlimitNumber.SetWindowText(A2W(pSt_AuthorizeCDKey->st_AuthSerial.st_UNLimit.tszUNLimitSerial));
	//用户信息
	m_EditUserInfo.SetWindowText(A2W(pSt_AuthorizeCDKey->st_AuthUserInfo.tszUserName));
	m_EditUserContact.SetWindowText(A2W(pSt_AuthorizeCDKey->st_AuthUserInfo.tszUserContact));
	m_EditUserCustom.SetWindowText(A2W(pSt_AuthorizeCDKey->st_AuthUserInfo.tszCustom));
	return true;
}
void CDialog_CDKey::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (0 == m_ComboRegSerial.GetCurSel() || 0 == m_ComboRegType.GetCurSel() || 0 == m_ComboRegVer.GetCurSel())
	{
		AfxMessageBox(_T("创建失败,注册验证类型没选择"));
		return;
	}
	if (3 != m_ComboRegSerial.GetCurSel() && 4 != m_ComboRegSerial.GetCurSel())
	{
		AfxMessageBox(_T("创建失败,注册类型仅支持次数卡和自定义时间卡"));
		return;
	}
	CString m_StrPass;
	m_EditKeyPass.GetWindowText(m_StrPass);
	if (m_StrPass.IsEmpty())
	{
		AfxMessageBox(_T("密码必须设置"));
		return;
	}
	XENGINE_AUTHORIZE_LOCAL st_AuthorizeCDKey = {};
	Dialog_CDKey_Read(&st_AuthorizeCDKey);

	CFileDialog m_FileDlg(false, _T(".key"),_T("CDKey"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CDKey文件(*.key)|*.key|"));
	if (IDOK == m_FileDlg.DoModal())
	{
		int nMSGLen = 0;
		XCHAR tszDEBuffer[4096] = {};
		XBYTE tszENBuffer[4096] = {};
		Authorize_CDKey_WriteMemory(tszDEBuffer, &nMSGLen, &st_AuthorizeCDKey);

		USES_CONVERSION;
		Cryption_XCrypto_Encoder(tszDEBuffer, &nMSGLen, tszENBuffer, W2A(m_StrPass.GetBuffer()));
		FILE* pSt_File = _tfopen(m_FileDlg.GetPathName(), _T("wb"));
		if (NULL == pSt_File)
		{
			AfxMessageBox(_T("创建CDKEY文件失败"));
			return;
		}
		fwrite(tszENBuffer, 1, nMSGLen, pSt_File);
		fclose(pSt_File);
	}
}

void CDialog_CDKey::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrPass;
	m_EditKeyPass.GetWindowText(m_StrPass);
	if (m_StrPass.IsEmpty())
	{
		AfxMessageBox(_T("密码必须设置"));
		return;
	}
	XENGINE_AUTHORIZE_LOCAL st_AuthorizeCDKey = {};

	CFileDialog m_FileDlg(true, _T(".key"), _T("CDKey"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CDKey文件(*.key)|*.key|"));
	if (IDOK == m_FileDlg.DoModal())
	{
		int nMSGLen = 0;
		XCHAR tszDEBuffer[4096] = {};
		XCHAR tszENBuffer[4096] = {};

		FILE* pSt_File = _tfopen(m_FileDlg.GetPathName(), _T("rb"));
		nMSGLen = fread(tszENBuffer, 1, sizeof(tszENBuffer), pSt_File);
		fclose(pSt_File);

		USES_CONVERSION;
		if (!Cryption_XCrypto_Decoder(tszENBuffer, &nMSGLen, tszDEBuffer, W2A(m_StrPass.GetBuffer())))
		{
			AfxMessageBox(_T("解密CDKEY失败"));
			return;
		}

		if (!Authorize_CDKey_ReadMemory(tszDEBuffer, nMSGLen, &st_AuthorizeCDKey))
		{
			AfxMessageBox(_T("CDKEY读取失败"));
			return;
		}
		Dialog_CDKey_Write(&st_AuthorizeCDKey);
		bOPenCDKey = true;
	}
}


void CDialog_CDKey::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	Dialog_CDKey_Init();
}


void CDialog_CDKey::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!bOPenCDKey)
	{
		AfxMessageBox(_T("没有打开要授权的文件"));
		return;
	}
	XENGINE_AUTHORIZE_LOCAL st_AuthorizeCDKey = {};
	Dialog_CDKey_Read(&st_AuthorizeCDKey);

	CString m_StrLeftTime;
	m_EditRegLeftTime.GetWindowText(m_StrLeftTime);

	if (3 == m_ComboRegSerial.GetCurSel())
	{
		Authorize_CDKey_BuildKeyTime(&st_AuthorizeCDKey, _ttoi64(m_StrLeftTime.GetBuffer()));
	}
	else if (4 == m_ComboRegSerial.GetCurSel())
	{
		XENGINE_LIBTIME st_LibTime = {};
		USES_CONVERSION;
		BaseLib_Time_StrToTime(W2A(m_StrLeftTime.GetBuffer()), &st_LibTime);
		Authorize_CDKey_BuildKeyTime(&st_AuthorizeCDKey, 0, &st_LibTime);
	}
	else
	{
		AfxMessageBox(_T("授权类型错误,无法继续"));
		return;
	}

	CString m_StrPass;
	m_EditKeyPass.GetWindowText(m_StrPass);
	if (m_StrPass.IsEmpty())
	{
		AfxMessageBox(_T("密码必须设置"));
		return;
	}
	CFileDialog m_FileDlg(false, _T(".key"), _T("CDKey"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CDKey文件(*.key)|*.key|"));
	if (IDOK == m_FileDlg.DoModal())
	{
		int nMSGLen = 0;
		XCHAR tszDEBuffer[4096] = {};
		XBYTE tszENBuffer[4096] = {};
		Authorize_CDKey_WriteMemory(tszDEBuffer, &nMSGLen, &st_AuthorizeCDKey);
		USES_CONVERSION;
		Cryption_XCrypto_Encoder(tszDEBuffer, &nMSGLen, tszENBuffer, W2A(m_StrPass.GetBuffer()));
		FILE* pSt_File = _tfopen(m_FileDlg.GetPathName(), _T("wb"));
		fwrite(tszENBuffer, 1, nMSGLen, pSt_File);
		fclose(pSt_File);
	}
	AfxMessageBox(_T("授权CDKEY成功"));
}
