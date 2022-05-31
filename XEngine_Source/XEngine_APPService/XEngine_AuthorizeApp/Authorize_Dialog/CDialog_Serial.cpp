// CDialog_Serial.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "CDialog_Serial.h"
#include "afxdialogex.h"


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
	DDX_Control(pDX, IDC_COMBO3, m_ComboNumber);
	DDX_Control(pDX, IDC_EDIT1, m_EditHowCard);
	DDX_Control(pDX, IDC_EDIT11, m_EditCardTime);
}


BEGIN_MESSAGE_MAP(CDialog_Serial, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_Serial::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialog_Serial::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDialog_Serial::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDialog_Serial 消息处理程序


BOOL CDialog_Serial::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListSerial.InsertColumn(0, _T("使用者"), LVCFMT_LEFT, 80);
	m_ListSerial.InsertColumn(1, _T("序列号"), LVCFMT_LEFT, 180);
	m_ListSerial.InsertColumn(2, _T("使用时间/次数"), LVCFMT_LEFT, 100);
	m_ListSerial.InsertColumn(3, _T("卡类型"), LVCFMT_LEFT, 80);
	m_ListSerial.InsertColumn(4, _T("是否使用"), LVCFMT_LEFT, 60);
	m_ListSerial.InsertColumn(5, _T("创建日期"), LVCFMT_LEFT, 100);
	m_ListSerial.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_ComboSerialType.InsertString(0, _T("分钟卡"));
	m_ComboSerialType.InsertString(1, _T("天卡"));
	m_ComboSerialType.InsertString(2, _T("次数卡"));
	m_ComboSerialType.InsertString(3, _T("自定义"));

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

void CDialog_Serial::SerialManage_Flush()
{
	int nListCount = 0;
	AUTHREG_SERIALTABLE** ppSt_SerialTable;
	if (!Database_SQLite_SerialQueryAll(&ppSt_SerialTable, &nListCount))
	{
		if (ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_QUERYALL_NONE == DBModule_GetLastError())
		{
			return;
		}
		AfxMessageBox(_T("获取失败！未知错误！"));
		return;
	}
	m_ListSerial.DeleteAllItems();

	for (int i = 0; i < nListCount; i++)
	{
		m_ListSerial.InsertItem(i, _T(""));
		m_ListSerial.SetItemText(i, 0, ppSt_SerialTable[i]->tszUserName);
		m_ListSerial.SetItemText(i, 1, ppSt_SerialTable[i]->tszSerialNumber);
		m_ListSerial.SetItemText(i, 2, ppSt_SerialTable[i]->tszMaxTime);
		switch (ppSt_SerialTable[i]->en_AuthRegSerialType)
		{
		case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_MINUTE:
		{
			LPCTSTR lpszCardName = _T("分钟卡");
			m_ListSerial.SetItemText(i, 3, lpszCardName);
		}
		break;
		case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_DAY:
		{
			LPCTSTR lpszCardName = _T("天数卡");
			m_ListSerial.SetItemText(i, 3, lpszCardName);
		}
		break;
		case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME:
		{
			LPCTSTR lpszCardName = _T("次数卡");
			m_ListSerial.SetItemText(i, 3, lpszCardName);
		}
		break;
		case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_CUSTOM:
		{
			LPCTSTR lpszCardName = _T("自定义卡");
			m_ListSerial.SetItemText(i, 3, lpszCardName);
		}
		break;
		default:
			break;
		}
		if (ppSt_SerialTable[i]->bIsUsed)
		{
			m_ListSerial.SetItemText(i, 4, _T("是"));
		}
		else
		{
			m_ListSerial.SetItemText(i, 4, _T("否"));
		}
		m_ListSerial.SetItemText(i, 5, ppSt_SerialTable[i]->tszCreateTime);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_SerialTable, nListCount);
}
void CDialog_Serial::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	LPCTSTR lpszUserHdr = _T("XAUTH");
	CString m_StrHowCard;
	CString m_StrTimeCard;
	CString m_StrCardNumber;
	XENGINE_LIBTIMER st_AuthTimer;
	memset(&st_AuthTimer, '\0', sizeof(st_AuthTimer));

	m_ComboNumber.GetLBText(m_ComboNumber.GetCurSel(), m_StrCardNumber);
	m_EditHowCard.GetWindowText(m_StrHowCard);
	m_EditCardTime.GetWindowText(m_StrTimeCard);

	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)(m_ComboSerialType.GetCurSel() + 1);
	//生成序列卡
	if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_MINUTE == enSerialType)
	{
		st_AuthTimer.wMinute = _ttoi(m_StrTimeCard.GetBuffer());
	}
	else if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_DAY == enSerialType)
	{
		st_AuthTimer.wDay = _ttoi(m_StrTimeCard.GetBuffer());
	}
	else if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == enSerialType)
	{
		st_AuthTimer.wFlags = _ttoi(m_StrTimeCard.GetBuffer());
	}
	else if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_CUSTOM == enSerialType)
	{
		if (6 != _stscanf_s(m_StrTimeCard.GetBuffer(), _T("%04d-%02d-%02d %02d:%02d:%02d"), &st_AuthTimer.wYear, &st_AuthTimer.wMonth, &st_AuthTimer.wDay, &st_AuthTimer.wHour, &st_AuthTimer.wMinute, &st_AuthTimer.wSecond))
		{
			AfxMessageBox(_T("自定义日期格式错误"));
			return;
		}
	}
	else
	{
		AfxMessageBox(_T("生成失败！"));
		return;
	}
	TCHAR** pptszSerialNumber;
	if (!Authorize_Serial_Creator(&pptszSerialNumber, lpszUserHdr, _ttoi(m_StrHowCard.GetBuffer()), _ttoi(m_StrCardNumber.GetBuffer()), &st_AuthTimer, enSerialType))
	{
		AfxMessageBox(_T("生成失败！"));
		return;
	}
	//导入序列卡
	for (int i = 0; i < _ttoi(m_StrHowCard.GetBuffer()); i++)
	{
		TCHAR tszSerialCard[MAX_PATH];
		memset(tszSerialCard, '\0', MAX_PATH);
		Database_SQLite_SerialInsert(pptszSerialNumber[i]);
	}
	AfxMessageBox(_T("插入成功"));
	SerialManage_Flush();
}


void CDialog_Serial::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListSerial.GetFirstSelectedItemPosition();
	int nSelect = m_ListSerial.GetNextSelectedItem(pSt_Sition);
	if (nSelect < 0)
	{
		AfxMessageBox(_T("你没有选择序列号！"));
		return;
	}
	CString m_Str = m_ListSerial.GetItemText(nSelect, 1);
	if (!NetCore_PIPClipBoard_Set(m_Str.GetBuffer(), m_Str.GetLength()))
	{
		AfxMessageBox(_T("复制失败！"));
		return;
	}
	AfxMessageBox(_T("复制成功！"));
}


void CDialog_Serial::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pSt_Sition = m_ListSerial.GetFirstSelectedItemPosition();
	int nItemCount = m_ListSerial.GetNextSelectedItem(pSt_Sition);
	if (nItemCount < 0)
	{
		AfxMessageBox(_T("你没有选择任何充值卡！"));
		return;
	}
	CString m_StrSerial = m_ListSerial.GetItemText(nItemCount, 1);
	if (!Database_SQLite_SerialDelete(m_StrSerial.GetBuffer()))
	{
		AfxMessageBox(_T("删除失败！"));
		return;
	}
	m_ListSerial.DeleteItem(nItemCount);
}
