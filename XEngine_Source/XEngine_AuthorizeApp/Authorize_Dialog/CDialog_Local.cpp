// CDialog_Local.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "CDialog_Local.h"
#include "afxdialogex.h"


// CDialog_Local 对话框

IMPLEMENT_DYNAMIC(CDialog_Local, CDialogEx)

CDialog_Local::CDialog_Local(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOCAL, pParent)
{

}

CDialog_Local::~CDialog_Local()
{
}

void CDialog_Local::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_EditKeyFile);
	DDX_Control(pDX, IDC_EDIT3, m_EditPass);
	DDX_Control(pDX, IDC_EDIT1, m_EditAPPName);
	DDX_Control(pDX, IDC_EDIT11, m_EditAPPVersion);
	DDX_Control(pDX, IDC_COMBO1, m_ComboHWType);
	DDX_Control(pDX, IDC_COMBO2, m_ComboRegType);
	DDX_Control(pDX, IDC_COMBO4, m_ComboSerialType);
	DDX_Control(pDX, IDC_EDIT4, m_EditMachineCode);
	DDX_Control(pDX, IDC_EDIT5, m_EditRegTime);
	DDX_Control(pDX, IDC_EDIT12, m_EditHaveTime);
	DDX_Control(pDX, IDC_EDIT13, m_EditLeftTime);
	DDX_Control(pDX, IDC_EDIT7, m_EditUser);
	DDX_Control(pDX, IDC_EDIT8, m_EditContact);
	DDX_Control(pDX, IDC_EDIT9, m_EditCustomInfo);
	DDX_Control(pDX, IDC_EDIT10, m_EditPort);
	DDX_Control(pDX, IDC_IPADDRESS1, m_EditIPAddr);
	DDX_Control(pDX, IDC_EDIT14, m_EditCreateTime);
}


BEGIN_MESSAGE_MAP(CDialog_Local, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_Local::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT4, &CDialog_Local::OnEnChangeEdit4)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialog_Local::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_EDIT12, &CDialog_Local::OnEnChangeEdit12)
END_MESSAGE_MAP()


// CDialog_Local 消息处理程序

BOOL CDialog_Local::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboHWType.InsertString(0, _T("未指定"));
	m_ComboHWType.InsertString(1, _T("CPU"));
	m_ComboHWType.InsertString(2, _T("硬盘"));
	m_ComboHWType.InsertString(3, _T("主板"));
	m_ComboHWType.InsertString(4, _T("网卡"));
	m_ComboHWType.SetCurSel(0);

	m_ComboSerialType.InsertString(0, _T("不支持"));
	m_ComboSerialType.InsertString(1, _T("分钟"));
	m_ComboSerialType.InsertString(2, _T("天数"));
	m_ComboSerialType.InsertString(3, _T("次数"));
	m_ComboSerialType.InsertString(4, _T("自定义"));
	m_ComboSerialType.SetCurSel(0);

	m_ComboRegType.InsertString(0, _T("未注册"));
	m_ComboRegType.InsertString(1, _T("临时"));
	m_ComboRegType.InsertString(2, _T("试用"));
	m_ComboRegType.InsertString(3, _T("正式版"));
	m_ComboRegType.InsertString(4, _T("无限制版"));
	m_ComboRegType.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDialog_Local::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrPass;
	UCHAR tszEnBuffer[2048];
	TCHAR tszDeBuffer[2048];
	XENGINE_AUTHORIZE_LOCAL st_AuthLocal;

	m_EditPass.GetWindowText(m_StrPass);

	CFileDialog m_FileDlg(TRUE, _T(".CDKey"), NULL, NULL, _T("CDKey文件(*.key)|*.key|所有文件(*.*)|*.*||"));
	if (IDCANCEL == m_FileDlg.DoModal())
	{
		return;
	}
	CString m_StrFile = m_FileDlg.GetPathName();
	m_EditKeyFile.SetWindowText(m_StrFile);

	memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));
	memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
	memset(&st_AuthLocal, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));

	if (!m_StrPass.IsEmpty())
	{
		FILE* pSt_File = fopen(m_StrFile.GetBuffer(), "rb");
		if (NULL == pSt_File)
		{
			AfxMessageBox(_T("文件打开失败"));
			return;
		}
		int nRet = fread(tszEnBuffer, 1, sizeof(tszEnBuffer), pSt_File);
		fclose(pSt_File);
		if (!OPenSsl_Api_CryptDecodec(tszEnBuffer, tszDeBuffer, &nRet, m_StrPass.GetBuffer(), XENGINE_OPENSSL_API_CRYPT_3DES))
		{
			AfxMessageBox(_T("解密失败"));
			return;
		}
		pSt_File = fopen(m_StrFile.GetBuffer(), "wb");
		fwrite(tszDeBuffer, 1, nRet, pSt_File);
		fclose(pSt_File);
	}
	if (!Authorize_Local_ReadKey(m_StrFile.GetBuffer(), &st_AuthLocal))
	{
		AfxMessageBox(_T("读取CDKEY失败"));
		return;
	}
	TCHAR tszTmp[64];
	memset(tszTmp, '\0', sizeof(tszTmp));

	_stprintf(tszTmp, _T("%d"), st_AuthLocal.nPort);
	m_EditPort.SetWindowText(tszTmp);
	m_EditIPAddr.SetWindowText(st_AuthLocal.tszAddr);

	m_EditAPPName.SetWindowText(st_AuthLocal.st_AuthAppInfo.tszAppName);
	m_EditAPPVersion.SetWindowText(st_AuthLocal.st_AuthAppInfo.tszAppVer);

	m_ComboHWType.SetCurSel(st_AuthLocal.st_AuthRegInfo.enHWType);
	m_ComboRegType.SetCurSel(st_AuthLocal.st_AuthRegInfo.enRegType);
	m_ComboSerialType.SetCurSel(st_AuthLocal.st_AuthRegInfo.enSerialType);
	m_EditMachineCode.SetWindowText(st_AuthLocal.st_AuthRegInfo.tszHardware);
	m_EditRegTime.SetWindowText(st_AuthLocal.st_AuthRegInfo.tszRegisterTime);
	m_EditCreateTime.SetWindowText(st_AuthLocal.st_AuthRegInfo.tszCreateTime);
	m_EditLeftTime.SetWindowText(st_AuthLocal.st_AuthRegInfo.tszLeftTime);
	memset(tszTmp, '\0', sizeof(tszTmp));
	_stprintf(tszTmp, _T("%lld"), st_AuthLocal.st_AuthRegInfo.nHasTime);
	m_EditHaveTime.SetWindowText(tszTmp);

	m_EditUser.SetWindowText(st_AuthLocal.st_AuthUserInfo.tszUserName);
	m_EditContact.SetWindowText(st_AuthLocal.st_AuthUserInfo.tszUserContact);
	m_EditCustomInfo.SetWindowText(st_AuthLocal.st_AuthUserInfo.tszCustom);
}



void CDialog_Local::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CDialog_Local::Dialog_Local_GetInfo(XENGINE_AUTHORIZE_LOCAL *pSt_AuthLocal)
{
	CString m_StrPort;
	CString m_StrHaveTime;
	XENGINE_AUTHORIZE_LOCAL st_AuthLocal;

	memset(&st_AuthLocal, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));

	st_AuthLocal.st_AuthAppInfo.bInit = TRUE;
	m_EditAPPName.GetWindowText(st_AuthLocal.st_AuthAppInfo.tszAppName, sizeof(st_AuthLocal.st_AuthAppInfo.tszAppName));
	m_EditAPPVersion.GetWindowText(st_AuthLocal.st_AuthAppInfo.tszAppVer, sizeof(st_AuthLocal.st_AuthAppInfo.tszAppVer));

	m_EditIPAddr.GetWindowText(st_AuthLocal.tszAddr, sizeof(st_AuthLocal.tszAddr));
	m_EditPort.GetWindowText(m_StrPort);
	st_AuthLocal.nPort = _ttoi(m_StrPort.GetBuffer());

	st_AuthLocal.st_AuthRegInfo.enHWType = (ENUM_HELPCOMPONENTS_AUTHORIZE_HW_TYPE)m_ComboHWType.GetCurSel();
	st_AuthLocal.st_AuthRegInfo.enRegType = (ENUM_HELPCOMPONENTS_AUTHORIZE_REG_TYPE)m_ComboRegType.GetCurSel();
	st_AuthLocal.st_AuthRegInfo.enSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)m_ComboSerialType.GetCurSel();
	m_EditMachineCode.GetWindowText(st_AuthLocal.st_AuthRegInfo.tszHardware, sizeof(st_AuthLocal.st_AuthRegInfo.tszHardware));
	m_EditRegTime.GetWindowText(st_AuthLocal.st_AuthRegInfo.tszRegisterTime, sizeof(st_AuthLocal.st_AuthRegInfo.tszRegisterTime));
	m_EditCreateTime.GetWindowText(st_AuthLocal.st_AuthRegInfo.tszCreateTime, sizeof(st_AuthLocal.st_AuthRegInfo.tszCreateTime));
	m_EditLeftTime.GetWindowText(st_AuthLocal.st_AuthRegInfo.tszLeftTime, sizeof(st_AuthLocal.st_AuthRegInfo.tszLeftTime));
	m_EditHaveTime.GetWindowText(m_StrHaveTime);
	st_AuthLocal.st_AuthRegInfo.nHasTime = _ttoi(m_StrHaveTime.GetBuffer());

	m_EditUser.GetWindowText(st_AuthLocal.st_AuthUserInfo.tszUserName, sizeof(st_AuthLocal.st_AuthUserInfo.tszUserName));
	m_EditContact.GetWindowText(st_AuthLocal.st_AuthUserInfo.tszUserContact, sizeof(st_AuthLocal.st_AuthUserInfo.tszUserContact));
	m_EditCustomInfo.GetWindowText(st_AuthLocal.st_AuthUserInfo.tszCustom, sizeof(st_AuthLocal.st_AuthUserInfo.tszCustom));

	memcpy(pSt_AuthLocal, &st_AuthLocal, sizeof(XENGINE_AUTHORIZE_LOCAL));
}

void CDialog_Local::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog m_FileDlg(FALSE, _T(".CDKey"), NULL, NULL, _T("CDKey文件(*.key)|*.key|所有文件(*.*)|*.*||"));
	if (IDCANCEL == m_FileDlg.DoModal())
	{
		return;
	}
	CString m_StrFile = m_FileDlg.GetPathName();
	m_EditKeyFile.SetWindowText(m_StrFile);

	CString m_StrPass;
	UCHAR tszEnBuffer[2048];
	TCHAR tszDeBuffer[2048];
	XENGINE_AUTHORIZE_LOCAL st_AuthLocal;

	memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));
	memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
	memset(&st_AuthLocal, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));

	m_EditPass.GetWindowText(m_StrPass);
	
	Dialog_Local_GetInfo(&st_AuthLocal);
	if (!Authorize_Local_WriteKey(m_StrFile.GetBuffer(), &st_AuthLocal))
	{
		AfxMessageBox(_T("写入CDKEY失败"));
		return;
	}
	if (!m_StrPass.IsEmpty())
	{
		FILE* pSt_File = fopen(m_StrFile.GetBuffer(), "rb");
		int nRet = fread(tszDeBuffer, 1, sizeof(tszDeBuffer), pSt_File);
		fclose(pSt_File);
		if (!OPenSsl_Api_CryptEncodec(tszDeBuffer, tszEnBuffer, &nRet, m_StrPass.GetBuffer(), XENGINE_OPENSSL_API_CRYPT_3DES))
		{
			AfxMessageBox(_T("加密失败"));
			return;
		}
		pSt_File = fopen(m_StrFile.GetBuffer(), "wb");
		fwrite(tszEnBuffer, 1, nRet, pSt_File);
		fclose(pSt_File);
	}
}

void CDialog_Local::OnEnChangeEdit12()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	XENGINE_AUTHORIZE_LOCAL st_AuthLocal;
	memset(&st_AuthLocal, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));

	Dialog_Local_GetInfo(&st_AuthLocal);
	if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_CUSTOM == st_AuthLocal.st_AuthRegInfo.enSerialType)
	{
		CString m_StrHaveTime;
		XENGINE_LIBTIMER st_LibTime;

		memset(&st_LibTime, '\0', sizeof(XENGINE_LIBTIMER));
		m_EditHaveTime.GetWindowText(m_StrHaveTime);

		BaseLib_OperatorTime_StrToStuTime(m_StrHaveTime.GetBuffer(), &st_LibTime);
		Authorize_Local_BuildKeyTime(&st_AuthLocal, NULL, &st_LibTime);
	}
	else
	{
		Authorize_Local_BuildKeyTime(&st_AuthLocal, st_AuthLocal.st_AuthRegInfo.nHasTime);
	}
	m_EditRegTime.SetWindowText(st_AuthLocal.st_AuthRegInfo.tszRegisterTime);
	m_EditLeftTime.SetWindowText(st_AuthLocal.st_AuthRegInfo.tszLeftTime);
}
