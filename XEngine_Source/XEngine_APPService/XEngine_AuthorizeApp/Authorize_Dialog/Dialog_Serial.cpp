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
}


BEGIN_MESSAGE_MAP(CDialog_Serial, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_Serial::OnBnClickedButton1)
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

	for (int i = 0; i < 4; i++)
	{
		m_ComboSerialType.InsertString(i, lpszKeyType[i]);
	}

	m_ComboNumber.InsertString(0, _T("4"));
	m_ComboNumber.InsertString(1, _T("5"));
	m_ComboNumber.InsertString(2, _T("6"));
	m_ComboNumber.InsertString(3, _T("7"));
	m_ComboNumber.InsertString(4, _T("8"));
	m_ComboNumber.InsertString(5, _T("9"));
	m_ComboNumber.InsertString(6, _T("10"));

	m_ComboNumber.SetCurSel(0);
	m_ComboSerialType.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDialog_Serial::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListSerial.DeleteAllItems();
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);

	TCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);

	_stprintf(tszUrlAddr, _T("http://%s:%s/auth/serial/list"), m_StrIPAddr.GetBuffer(), m_StrIPPort.GetBuffer());
	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	APIHelp_HttpRequest_Post(tszUrlAddr, NULL, NULL, &ptszMsgBuffer, &nMsgLen);

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		return;
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
		m_ListSerial.SetItemText(i, 3, lpszKeyType[st_JsonArray["enSerialType"].asInt()]);
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
