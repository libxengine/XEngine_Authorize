// Dialog_Serial.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "afxdialogex.h"
#include "Dialog_Serial.h"


// CDialog_Serial 对话框

IMPLEMENT_DYNAMIC(CDialog_Serial, CDialogEx)

CDialog_Serial::CDialog_Serial(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SERIAL, pParent)
{

}

CDialog_Serial::~CDialog_Serial()
{
}

void CDialog_Serial::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListSerial);
	DDX_Control(pDX, IDC_COMBO1, m_ComboSerialType);
	DDX_Control(pDX, IDC_COMBO2, m_ComboNumber);
	DDX_Control(pDX, IDC_EDIT1, m_EditSerialCount);
	DDX_Control(pDX, IDC_EDIT2, m_EditHasTime);
	DDX_Control(pDX, IDC_EDIT3, m_EditPosStart);
	DDX_Control(pDX, IDC_EDIT7, m_EditPosEnd);
}


BEGIN_MESSAGE_MAP(CDialog_Serial, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_Serial::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_Serial::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CDialog_Serial::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialog_Serial::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, &CDialog_Serial::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CDialog_Serial::OnBnClickedButton7)
END_MESSAGE_MAP()


// CDialog_Serial 消息处理程序


BOOL CDialog_Serial::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListSerial.InsertColumn(0, _T("使用者"), LVCFMT_LEFT, 80);
	m_ListSerial.InsertColumn(1, _T("序列号"), LVCFMT_LEFT, 180);
	m_ListSerial.InsertColumn(2, _T("时间/次数"), LVCFMT_LEFT, 90);
	m_ListSerial.InsertColumn(3, _T("卡类型"), LVCFMT_LEFT, 80);
	m_ListSerial.InsertColumn(4, _T("是否使用"), LVCFMT_LEFT, 60);
	m_ListSerial.InsertColumn(5, _T("创建日期"), LVCFMT_LEFT, 120);
	m_ListSerial.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	for (int i = 0; i < 5; i++)
	{
		m_ComboSerialType.InsertString(i, lpszXSerialType[i]);
	}

	m_ComboNumber.InsertString(0, _T("4"));
	m_ComboNumber.InsertString(1, _T("5"));
	m_ComboNumber.InsertString(2, _T("6"));
	m_ComboNumber.InsertString(3, _T("7"));
	m_ComboNumber.InsertString(4, _T("8"));
	m_ComboNumber.InsertString(5, _T("9"));
	m_ComboNumber.InsertString(6, _T("10"));

	m_EditPosStart.SetWindowText("0");
	m_EditPosEnd.SetWindowText("50");

	m_ComboNumber.SetCurSel(0);
	m_ComboSerialType.SetCurSel(0);
	return TRUE;  // return true unless you set the focus to a control
	// 异常: OCX 属性页应返回 false
}


void CDialog_Serial::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListSerial.DeleteAllItems();
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	CString m_StrPosStart;
	CString m_StrPosEnd;
	
	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);
	m_EditPosStart.GetWindowText(m_StrPosStart);
	m_EditPosEnd.GetWindowText(m_StrPosEnd);

	TCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);

	_xstprintf(tszUrlAddr, _T("http://%s:%s/auth/serial/list"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());
	int nMsgLen = 0;
	TCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	st_JsonRoot["xhToken"] = _ttxoll(m_StrToken.GetBuffer());
	st_JsonRoot["PosStart"] = _ttxoi(m_StrPosStart.GetBuffer());
	st_JsonRoot["PosEnd"] = _ttxoi(m_StrPosEnd.GetBuffer());
	//是否加密
	TCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));
	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		nMsgLen = st_JsonRoot.toStyledString().length();
		OPenSsl_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (UCHAR*)tszMsgBuffer, tszPassBuffer);
		APIClient_Http_Request(_T("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIClient_Http_Request(_T("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
	}
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	st_JsonRoot.clear();
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		OPenSsl_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, tszPassBuffer);
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析客户列表接口数据错误,无法继续"));
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析客户列表接口数据错误,无法继续"));
			return;
		}
	}

	for (unsigned int i = 0; i < st_JsonRoot["Array"].size(); i++)
	{
		TCHAR tszIndex[10];
		memset(tszIndex, '\0', 10);
		_itot_s(i, tszIndex, 10);

		Json::Value st_JsonArray = st_JsonRoot["Array"][i];

		m_ListSerial.InsertItem(i, _T(""));
		m_ListSerial.SetItemText(i, 0, st_JsonArray["tszUserName"].asCString());
		m_ListSerial.SetItemText(i, 1, st_JsonArray["tszSerialNumber"].asCString());
		m_ListSerial.SetItemText(i, 2, st_JsonArray["tszMaxTime"].asCString());
		m_ListSerial.SetItemText(i, 3, lpszXSerialType[st_JsonArray["enSerialType"].asInt()]);
		if (st_JsonArray["bIsUsed"].asBool())
		{
			m_ListSerial.SetItemText(i, 4, "已使用");
		}
		else
		{
			m_ListSerial.SetItemText(i, 4, "未使用");
		}
		m_ListSerial.SetItemText(i, 5, st_JsonArray["tszCreateTime"].asCString());
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}


void CDialog_Serial::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	TCHAR tszUrlAddr[MAX_PATH];
	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);

	memset(tszUrlAddr, '\0', MAX_PATH);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	CString m_StrHasTime;
	CString m_StrSerialCount;
	CString m_StrNumberCount;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	m_EditHasTime.GetWindowText(m_StrHasTime);
	m_EditSerialCount.GetWindowText(m_StrSerialCount);
	m_ComboNumber.GetLBText(m_ComboNumber.GetCurSel(), m_StrNumberCount);

	st_JsonObject["enSerialType"] = m_ComboSerialType.GetCurSel();
	st_JsonObject["nNumberCount"] = _ttxoi(m_StrNumberCount.GetBuffer());
	st_JsonObject["nSerialCount"] = _ttxoi(m_StrSerialCount.GetBuffer());
	st_JsonObject["tszHasTime"] = m_StrHasTime.GetBuffer();
	st_JsonRoot["st_SerialInfo"] = st_JsonObject;
	st_JsonRoot["xhToken"] = _ttxoll(m_StrToken.GetBuffer());

	int nMsgLen = 0;
	TCHAR* ptszMsgBuffer = NULL;
	_xstprintf(tszUrlAddr, _T("http://%s:%s/auth/serial/insert"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());
	//是否加密
	TCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));

	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		nMsgLen = st_JsonRoot.toStyledString().length();
		OPenSsl_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (UCHAR*)tszMsgBuffer, tszPassBuffer);
		APIClient_Http_Request(_T("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIClient_Http_Request(_T("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
	}
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		OPenSsl_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, tszPassBuffer);
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析客户列表接口数据错误,无法继续"));
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析客户列表接口数据错误,无法继续"));
			return;
		}
	}

	if (0 == st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("插入序列号成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("插入序列号失败"));
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	//刷新
	OnBnClickedButton1();
}


void CDialog_Serial::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListSerial.GetFirstSelectedItemPosition();
	int nSelect = m_ListSerial.GetNextSelectedItem(pSt_Sition);
	if (nSelect < 0)
	{
		Authorize_Help_LogPrint(_T("你没有选择任何客户！"));
		return;
	}
	CString m_StrSerial = m_ListSerial.GetItemText(nSelect, 1);

	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	TCHAR tszUrlAddr[MAX_PATH];
	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);

	memset(tszUrlAddr, '\0', MAX_PATH);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	CString m_StrHasTime;
	CString m_StrSerialCount;
	CString m_StrNumberCount;
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;
	Json::Value st_JsonObject;

	m_EditHasTime.GetWindowText(m_StrHasTime);
	m_EditSerialCount.GetWindowText(m_StrSerialCount);
	m_ComboNumber.GetLBText(m_ComboNumber.GetCurSel(), m_StrNumberCount);

	st_JsonObject["tszSerialNumber"] = m_StrSerial.GetBuffer();
	st_JsonArray.append(st_JsonObject);
	st_JsonRoot["Array"] = st_JsonArray;
	st_JsonRoot["xhToken"] = _ttxoll(m_StrToken.GetBuffer());

	int nMsgLen = 0;
	TCHAR* ptszMsgBuffer = NULL;
	_xstprintf(tszUrlAddr, _T("http://%s:%s/auth/serial/delete"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());
	//是否加密
	TCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));

	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		nMsgLen = st_JsonRoot.toStyledString().length();
		OPenSsl_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (UCHAR*)tszMsgBuffer, tszPassBuffer);
		APIClient_Http_Request(_T("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIClient_Http_Request(_T("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
	}
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		OPenSsl_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, tszPassBuffer);
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析客户列表接口数据错误,无法继续"));
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析客户列表接口数据错误,无法继续"));
			return;
		}
	}

	if (0 == st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("删除序列号成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("删除序列号失败"));
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	//刷新
	OnBnClickedButton1();
}


void CDialog_Serial::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListSerial.GetFirstSelectedItemPosition();
	int nSelect = m_ListSerial.GetNextSelectedItem(pSt_Sition);
	if (nSelect < 0)
	{
		Authorize_Help_LogPrint(_T("你没有选择序列号！"));
		return;
	}
	CString m_Str = m_ListSerial.GetItemText(nSelect, 1);
	if (!AuthHelp_ClipBoard_Set(m_Str.GetBuffer(), m_Str.GetLength()))
	{
		Authorize_Help_LogPrint(_T("复制失败！"));
		return;
	}
	Authorize_Help_LogPrint(_T("复制成功！"));
}


void CDialog_Serial::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR tszFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");
	CFileDialog m_FileDlg(true, _T("txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, tszFilter, this);
	// 显示保存文件对话框   
	if (IDOK != m_FileDlg.DoModal())
	{
		return;
	}
	FILE* pSt_File = _tfopen(m_FileDlg.GetPathName(), _T("rb"));

	TCHAR tszMsgBuffer[MAX_PATH];
	//跳过第一行
	if (NULL == fgets(tszMsgBuffer, MAX_PATH, pSt_File))
	{
		//没有数据
		AfxMessageBox(_T("没有数据"));
		return;
	}
	list<AUTHREG_SERIALTABLE> stl_ListSerial;
	while (true)
	{
		memset(tszMsgBuffer, '\0', MAX_PATH);
		//一行一行读取
		if (NULL == fgets(tszMsgBuffer, MAX_PATH, pSt_File))
		{
			break;
		}
		//解析数据
		int nID = 0;
		AUTHREG_SERIALTABLE st_SerialTable;
		memset(&st_SerialTable, '\0', sizeof(AUTHREG_SERIALTABLE));

		int nRet = _stscanf(tszMsgBuffer, _T("%d %s %s %s %d %d %s %s"), &nID, st_SerialTable.tszUserName, st_SerialTable.tszSerialNumber, st_SerialTable.tszMaxTime, &st_SerialTable.enSerialType, &st_SerialTable.bIsUsed, st_SerialTable.tszCreateTime, (st_SerialTable.tszCreateTime + 11));
		st_SerialTable.tszCreateTime[10] = ' ';
		stl_ListSerial.push_back(st_SerialTable);
	}
	fclose(pSt_File);

	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	TCHAR tszUrlAddr[MAX_PATH];
	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);

	memset(tszUrlAddr, '\0', MAX_PATH);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	CString m_StrHasTime;
	CString m_StrSerialCount;
	CString m_StrNumberCount;
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;

	m_EditHasTime.GetWindowText(m_StrHasTime);
	m_EditSerialCount.GetWindowText(m_StrSerialCount);
	m_ComboNumber.GetLBText(m_ComboNumber.GetCurSel(), m_StrNumberCount);

	for (auto stl_ListIterator = stl_ListSerial.begin(); stl_ListIterator != stl_ListSerial.end(); stl_ListIterator++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["bIsUsed"] = stl_ListIterator->bIsUsed;
		st_JsonObject["enSerialType"] = stl_ListIterator->enSerialType;
		st_JsonObject["tszCreateTime"] = stl_ListIterator->tszCreateTime;
		st_JsonObject["tszMaxTime"] = stl_ListIterator->tszMaxTime;
		st_JsonObject["tszSerialNumber"] = stl_ListIterator->tszSerialNumber;
		st_JsonObject["tszUserName"] = stl_ListIterator->tszUserName;
		st_JsonArray.append(st_JsonObject);
	}
	st_JsonRoot["Array"] = st_JsonArray;
	st_JsonRoot["xhToken"] = _ttxoll(m_StrToken.GetBuffer());

	int nMsgLen = 0;
	TCHAR* ptszMsgBuffer = NULL;
	_xstprintf(tszUrlAddr, _T("http://%s:%s/auth/serial/push"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());
	//是否加密
	TCHAR tszPassBuffer[64];
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));

	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		nMsgLen = st_JsonRoot.toStyledString().length();
		OPenSsl_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (UCHAR*)tszMsgBuffer, tszPassBuffer);
		APIClient_Http_Request(_T("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIClient_Http_Request(_T("POST"), tszUrlAddr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
	}
	st_JsonRoot.clear();
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		OPenSsl_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, tszPassBuffer);
		if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析序列号推送接口数据错误,无法继续"));
			return;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			Authorize_Help_LogPrint(_T("解析序列号推送接口数据错误,无法继续"));
			return;
		}
	}

	if (0 == st_JsonRoot["code"].asInt())
	{
		Authorize_Help_LogPrint(_T("导入序列号成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("导入序列号失败"));
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	//刷新
	OnBnClickedButton1();
}


void CDialog_Serial::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR tszFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");
	CFileDialog m_FileDlg(false, _T("txt"), _T("serial"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, tszFilter, this);
	// 显示保存文件对话框   
	if (IDOK != m_FileDlg.DoModal())
	{
		return;
	}
	FILE* pSt_File = _tfopen(m_FileDlg.GetPathName(), _T("wb"));

	TCHAR tszMsgBuffer[MAX_PATH];
	memset(tszMsgBuffer, '\0', MAX_PATH);
	//写字段头
	int nRet = _xstprintf(tszMsgBuffer, _T("ID UserName SerialNumber MaxTime CardSerialType bIsUsed CreateTime\r\n"));
	fwrite(tszMsgBuffer, 1, nRet, pSt_File);

	for (int i = 0; i < m_ListSerial.GetItemCount(); i++)
	{
		int nSerialType = 0;
		int nUsedType = 0;
		memset(tszMsgBuffer, '\0', MAX_PATH);

		for (int j = 0; j < sizeof(lpszXSerialType) - 1; j++)
		{
			if (0 == _tcsxnicmp(lpszXSerialType[j], m_ListSerial.GetItemText(i, 3).GetBuffer(), _tcsxlen(lpszXSerialType[j])))
			{
				nSerialType = j;
				break;
			}
		}
		if (0 == _tcsxnicmp(m_ListSerial.GetItemText(i, 4).GetBuffer(), "未使用", m_ListSerial.GetItemText(i, 4).GetLength()))
		{
			nUsedType = 0;
		}
		nRet = _xstprintf(tszMsgBuffer, _T("%d %s %s %s %d %d %s %s\r\n"), i, m_ListSerial.GetItemText(i, 0).GetBuffer(), m_ListSerial.GetItemText(i, 1).GetBuffer(), m_ListSerial.GetItemText(i, 2).GetBuffer(), nSerialType, nUsedType, m_ListSerial.GetItemText(i, 5).GetBuffer(), m_ListSerial.GetItemText(i, 6).GetBuffer());
		fwrite(tszMsgBuffer, 1, nRet, pSt_File);
	}
	fclose(pSt_File);

	Authorize_Help_LogPrint(_T("导出序列号成功"));
}
