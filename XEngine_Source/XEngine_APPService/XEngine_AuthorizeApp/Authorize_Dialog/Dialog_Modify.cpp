// Dialog_Modify.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "afxdialogex.h"
#include "Dialog_Modify.h"


// CDialog_Modify 对话框

IMPLEMENT_DYNAMIC(CDialog_Modify, CDialogEx)

CDialog_Modify::CDialog_Modify(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MODIFY, pParent)
{

}

CDialog_Modify::~CDialog_Modify()
{
}

void CDialog_Modify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditUser);
	DDX_Control(pDX, IDC_EDIT2, m_EditPass);
	DDX_Control(pDX, IDC_EDIT3, m_EditEMail);
	DDX_Control(pDX, IDC_EDIT5, m_EditCardID);
	DDX_Control(pDX, IDC_EDIT4, m_EditPhone);
	DDX_Control(pDX, IDC_EDIT6, m_EditLeftTime);
	DDX_Control(pDX, IDC_COMBO1, m_ComboSerial);
	DDX_Control(pDX, IDC_COMBO2, m_ComboLeave);
	DDX_Control(pDX, IDC_EDIT7, m_EditHardCode);
	DDX_Control(pDX, IDC_BUTTON2, m_BtnModify);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateTimeRegister);
}


BEGIN_MESSAGE_MAP(CDialog_Modify, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_Modify::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialog_Modify::OnBnClickedButton3)
END_MESSAGE_MAP()


// CDialog_Modify 消息处理程序


BOOL CDialog_Modify::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDialog_Config* pConfigWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	CDialog_User* pUserWnd = (CDialog_User*)CDialog_User::FromHandle(hUserWnd);

	for (int i = 0; i < 5; i++)
	{
		USES_CONVERSION;
		m_ComboSerial.InsertString(i, A2W(lpszXSerialType[i]));
	}
	for (int i = 0; i < 21; i++)
	{
		USES_CONVERSION;
		m_ComboLeave.InsertString(i, A2W(lpszXLevelType[i]));
	}
	m_DateTimeRegister.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	POSITION pSt_Sition = pUserWnd->m_ListCtrlClient.GetFirstSelectedItemPosition();
	if (NULL == pSt_Sition)
	{
		//没有选择,表示添加
		m_BtnModify.SetWindowText(_T("添加"));
		
		m_ComboSerial.SetCurSel(1);
		m_ComboLeave.SetCurSel(11);
	}
	else
	{
		int nItemCount = pUserWnd->m_ListCtrlClient.GetNextSelectedItem(pSt_Sition);
		CString m_StrUser = pUserWnd->m_ListCtrlClient.GetItemText(nItemCount, 1);

		CString m_StrIPAddr;
		CString m_StrIPPort;
		CString m_StrToken;
		XCHAR tszUrlAddr[MAX_PATH];
		memset(tszUrlAddr, '\0', MAX_PATH);
		//组合请求URL
		pConfigWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
		pConfigWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
		pConfigWnd->m_EditToken.GetWindowText(m_StrToken);
		USES_CONVERSION;
		_xstprintf(tszUrlAddr, _X("http://%s:%s/auth/client/get"), W2A(m_StrIPAddr.GetBuffer()), W2A(m_StrIPPort.GetBuffer()));
		//请求用户信息
		int nMsgLen = 0;
		XCHAR* ptszMsgBuffer = NULL;
		Json::Value st_JsonRoot;
		Json::Value st_JsonObject;

		st_JsonObject["tszUserName"] = W2A(m_StrUser.GetBuffer());
		st_JsonRoot["st_UserInfo"] = st_JsonObject;
		st_JsonRoot["xhToken"] = _ttoll(m_StrToken.GetBuffer());
		
		//是否加密
		TCHAR tszPassBuffer[64];
		memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
		::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));
		if (bCrypto)
		{
			XCHAR tszMsgBuffer[2048];
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

			nMsgLen = st_JsonRoot.toStyledString().length();
			Cryption_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (UCHAR*)tszMsgBuffer, W2A(tszPassBuffer));
			APIClient_Http_Request(_X("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
		}
		else
		{
			APIClient_Http_Request(_X("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
		}
		st_JsonObject.clear();
		st_JsonRoot.clear();
		JSONCPP_STRING st_JsonError;
		Json::CharReaderBuilder st_ReaderBuilder;
		std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
		if (bCrypto)
		{
			XCHAR tszMsgBuffer[2048];
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

			Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, W2A(tszPassBuffer));
			if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
			{
				Authorize_Help_LogPrint(_T("解析客户接口数据错误,无法继续"));
				return false;
			}
		}
		else
		{
			if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
			{
				Authorize_Help_LogPrint(_T("解析客户接口数据错误,无法继续"));
				return false;
			}
		}

		st_JsonObject = st_JsonRoot["st_UserTable"];
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

		m_EditUser.SetWindowText(A2W(st_JsonObject["st_UserInfo"]["tszUserName"].asCString()));
		m_EditPass.SetWindowText(A2W(st_JsonObject["st_UserInfo"]["tszUserPass"].asCString()));
		m_EditEMail.SetWindowText(A2W(st_JsonObject["st_UserInfo"]["tszEMailAddr"].asCString()));
		COleDateTime m_OleDTime;
		if (m_OleDTime.ParseDateTime(A2W(st_JsonObject["st_UserInfo"]["tszCreateTime"].asCString())))
		{
			m_DateTimeRegister.SetTime(m_OleDTime);
		}

		CString m_StrNumber;
		m_StrNumber.Format(_T("%lld"), st_JsonObject["st_UserInfo"]["nIDNumber"].asUInt64());
		m_EditCardID.SetWindowText(m_StrNumber);

		m_StrNumber.ReleaseBuffer();
		m_StrNumber.Format(_T("%lld"), st_JsonObject["st_UserInfo"]["nPhoneNumber"].asUInt64());
		m_EditPhone.SetWindowText(m_StrNumber);

		m_EditHardCode.SetWindowText(A2W(st_JsonObject["tszHardCode"].asCString()));
		m_EditLeftTime.SetWindowText(A2W(st_JsonObject["tszLeftTime"].asCString()));

		m_ComboSerial.SetCurSel(st_JsonObject["enSerialType"].asInt());
		m_ComboLeave.SetCurSel(st_JsonObject["st_UserInfo"]["nUserLevel"].asInt() + 1);

		if (ENUM_XENGINE_PROTOCOLHDR_LEVEL_TYPE_ROOT == st_JsonObject["st_UserInfo"]["nUserLevel"].asInt())
		{
			m_ComboLeave.EnableWindow(false);
		}
	}
	return TRUE;  // return true unless you set the focus to a control
	// 异常: OCX 属性页应返回 false
}


void CDialog_Modify::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	AUTHREG_USERTABLE st_UserTable;
	memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

	CString m_StrUserInfo;
	USES_CONVERSION;
	m_EditUser.GetWindowText(m_StrUserInfo);
	strcpy(st_UserTable.st_UserInfo.tszUserName, W2A(m_StrUserInfo.GetBuffer()));
	m_StrUserInfo.ReleaseBuffer();
	m_EditPass.GetWindowText(m_StrUserInfo);
	strcpy(st_UserTable.st_UserInfo.tszUserPass, W2A(m_StrUserInfo.GetBuffer()));
	m_StrUserInfo.ReleaseBuffer();
	m_EditEMail.GetWindowText(m_StrUserInfo);
	strcpy(st_UserTable.st_UserInfo.tszEMailAddr, W2A(m_StrUserInfo.GetBuffer()));
	m_StrUserInfo.ReleaseBuffer();
	m_DateTimeRegister.GetWindowText(m_StrUserInfo);
	strcpy(st_UserTable.st_UserInfo.tszCreateTime, W2A(m_StrUserInfo.GetBuffer()));
	m_StrUserInfo.ReleaseBuffer();
	m_EditHardCode.GetWindowText(m_StrUserInfo);
	strcpy(st_UserTable.tszHardCode, W2A(m_StrUserInfo.GetBuffer()));

	CString m_StrNumber;
	m_EditPhone.GetWindowText(m_StrNumber);
	st_UserTable.st_UserInfo.nPhoneNumber = _ttoll(m_StrNumber.GetBuffer());
	m_StrNumber.ReleaseBuffer();
	m_EditCardID.GetWindowText(m_StrNumber);
	st_UserTable.st_UserInfo.nIDNumber = _ttoll(m_StrNumber.GetBuffer());

	m_StrUserInfo.ReleaseBuffer();
	m_EditLeftTime.GetWindowText(m_StrUserInfo);
	strcpy(st_UserTable.tszLeftTime, W2A(m_StrUserInfo.GetBuffer()));

	st_UserTable.enSerialType = (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE)m_ComboSerial.GetCurSel();
	st_UserTable.st_UserInfo.nUserLevel = m_ComboLeave.GetCurSel() - 1;

	Json::Value st_JsonRoot;
	Json::Value st_JsonTable;
	Json::Value st_JsonUser;
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	CString m_StrBtnModify;

	XCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);
	m_BtnModify.GetWindowText(m_StrBtnModify);
	//组合请求URL
	CDialog_Config* pConfigWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	pConfigWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pConfigWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pConfigWnd->m_EditToken.GetWindowText(m_StrToken);
	if (0 == _tcscmp(_T("添加"), m_StrBtnModify.GetBuffer()))
	{
		_xstprintf(tszUrlAddr, _X("http://%s:%s/auth/user/register"), W2A(m_StrIPAddr.GetBuffer()), W2A(m_StrIPPort.GetBuffer()));
	}
	else
	{
		st_JsonRoot["xhToken"] = _ttoll(m_StrToken.GetBuffer());
		_xstprintf(tszUrlAddr, _X("http://%s:%s/auth/client/modify"), W2A(m_StrIPAddr.GetBuffer()), W2A(m_StrIPPort.GetBuffer()));
	}

	st_JsonUser["tszUserName"] = st_UserTable.st_UserInfo.tszUserName;
	st_JsonUser["tszUserPass"] = st_UserTable.st_UserInfo.tszUserPass;
	st_JsonUser["tszEMailAddr"] = st_UserTable.st_UserInfo.tszEMailAddr;
	st_JsonUser["tszCreateTime"] = st_UserTable.st_UserInfo.tszCreateTime;
	st_JsonUser["nPhoneNumber"] = st_UserTable.st_UserInfo.nPhoneNumber;
	st_JsonUser["nIDNumber"] = st_UserTable.st_UserInfo.nIDNumber;
	st_JsonUser["nUserLevel"] = st_UserTable.st_UserInfo.nUserLevel;
	st_JsonTable["tszLeftTime"] = st_UserTable.tszLeftTime;
	st_JsonTable["tszHardCode"] = st_UserTable.tszHardCode;
	st_JsonTable["enSerialType"] = st_UserTable.enSerialType;
	st_JsonTable["st_UserInfo"] = st_JsonUser;
	st_JsonRoot["st_UserTable"] = st_JsonTable;
	
	//是否加密
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	TCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		nMsgLen = st_JsonRoot.toStyledString().length();
		Cryption_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (UCHAR*)tszMsgBuffer, W2A(tszPassBuffer));
		APIClient_Http_Request(_X("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIClient_Http_Request(_X("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
	}
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, W2A(tszPassBuffer));
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析客户接口数据错误,无法继续"));
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析客户接口数据错误,无法继续"));
			return;
		}
	}

	if (0 == st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("修改客户端成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("修改客户端失败"));
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	OnOK();
}


void CDialog_Modify::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
