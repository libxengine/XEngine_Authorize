﻿// Dialog_Announcement.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "afxdialogex.h"
#include "Dialog_Announcement.h"


// CDialog_Announcement 对话框

IMPLEMENT_DYNAMIC(CDialog_Announcement, CDialogEx)

CDialog_Announcement::CDialog_Announcement(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ANNOUNCEMENT, pParent)
{

}

CDialog_Announcement::~CDialog_Announcement()
{
}

void CDialog_Announcement::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListAnnouncement);
	DDX_Control(pDX, IDC_EDIT1, m_EditNoticeStr);
}


BEGIN_MESSAGE_MAP(CDialog_Announcement, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_Announcement::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_Announcement::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialog_Announcement::OnBnClickedButton3)
END_MESSAGE_MAP()


// CDialog_Announcement 消息处理程序


BOOL CDialog_Announcement::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListAnnouncement.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 60);
	m_ListAnnouncement.InsertColumn(1, _T("公告内容"), LVCFMT_LEFT, 500);
	m_ListAnnouncement.InsertColumn(2, _T("创建时间"), LVCFMT_LEFT, 150);
	m_ListAnnouncement.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	return TRUE;  // return true unless you set the focus to a control
	// 异常: OCX 属性页应返回 false
}


void CDialog_Announcement::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	CString m_StrNotice;

	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);
	m_EditNoticeStr.GetWindowText(m_StrNotice);

	if (m_StrNotice.GetLength() < 1)
	{
		AfxMessageBox(_T("内容不能小于1个字节"));
		return;
	}
	XCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);

	USES_CONVERSION;
	_xstprintf(tszUrlAddr, _X("http://%s:%s/auth/notice/insert"), W2A(m_StrIPAddr.GetBuffer()), W2A(m_StrIPPort.GetBuffer()));
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	Json::StreamWriterBuilder st_JsonBuilder;
	st_JsonBuilder["emitUTF8"] = true;

	st_JsonRoot["xhToken"] = _ttoll(m_StrToken.GetBuffer());
	
	st_JsonObject["tszContext"] = W2A(m_StrNotice.GetBuffer());
	st_JsonRoot["st_Notice"] = st_JsonObject;
	//是否加密
	XCHAR tszCodecBuffer[2048];
	TCHAR tszPassBuffer[64];

	memset(tszCodecBuffer, '\0', sizeof(tszCodecBuffer));
	memset(tszPassBuffer, '\0', sizeof(tszPassBuffer));
	::GetDlgItemText(hConfigWnd, IDC_EDIT6, tszPassBuffer, sizeof(tszPassBuffer));

	nMsgLen = Json::writeString(st_JsonBuilder, st_JsonRoot).length();
	BaseLib_Charset_AnsiToUTF(Json::writeString(st_JsonBuilder, st_JsonRoot).c_str(), tszCodecBuffer, &nMsgLen);
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		Cryption_XCrypto_Encoder(tszCodecBuffer, &nMsgLen, (UCHAR*)tszMsgBuffer, W2A(tszPassBuffer));
		APIClient_Http_Request(_X("POST"), tszUrlAddr, tszMsgBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	else
	{
		APIClient_Http_Request(_X("POST"), tszUrlAddr, tszCodecBuffer, NULL, &ptszMsgBuffer, &nMsgLen);
	}
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	st_JsonRoot.clear();
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, W2A(tszPassBuffer));
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
		Authorize_Help_LogPrint(_T("插入公告信息成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("插入公告信息成功"));
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	OnBnClickedButton3();
}

void CDialog_Announcement::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;
	CString m_StrNotice;

	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	POSITION pSt_Sition = m_ListAnnouncement.GetFirstSelectedItemPosition();
	int nSelect = m_ListAnnouncement.GetNextSelectedItem(pSt_Sition);
	if (nSelect < 0)
	{
		Authorize_Help_LogPrint(_T("没有选择公告,无法继续"));
		return;
	}
	m_StrNotice = m_ListAnnouncement.GetItemText(nSelect, 0);

	XCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);
	USES_CONVERSION;
	_xstprintf(tszUrlAddr, _X("http://%s:%s/auth/notice/delete"), W2A(m_StrIPAddr.GetBuffer()), W2A(m_StrIPPort.GetBuffer()));
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	st_JsonRoot["xhToken"] = _ttoll(m_StrToken.GetBuffer());

	st_JsonObject["nID"] = _ttoll(m_StrNotice.GetBuffer());
	st_JsonRoot["st_Notice"] = st_JsonObject;
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
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	st_JsonRoot.clear();
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, W2A(tszPassBuffer));
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
		Authorize_Help_LogPrint(_T("删除公告信息成功"));
	}
	else
	{
		Authorize_Help_LogPrint(_T("删除公告信息成功"));
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	OnBnClickedButton3();
}

void CDialog_Announcement::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListAnnouncement.DeleteAllItems();

	CString m_StrIPAddr;
	CString m_StrIPPort;
	CString m_StrToken;

	CDialog_Config* pWnd = (CDialog_Config*)CDialog_Config::FromHandle(hConfigWnd);
	pWnd->m_EditIPAddr.GetWindowText(m_StrIPAddr);
	pWnd->m_EditIPPort.GetWindowText(m_StrIPPort);
	pWnd->m_EditToken.GetWindowText(m_StrToken);

	XCHAR tszUrlAddr[MAX_PATH];
	memset(tszUrlAddr, '\0', MAX_PATH);

	USES_CONVERSION;
	_xstprintf(tszUrlAddr, _X("http://%s:%s/auth/notice/list"), W2A(m_StrIPAddr.GetBuffer()), W2A(m_StrIPPort.GetBuffer()));
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
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
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	st_JsonRoot.clear();
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bCrypto)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, W2A(tszPassBuffer));
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
		Json::Value st_JsonArray = st_JsonRoot["Array"][i];

		TCHAR tszMsgBuffer[2048];
		TCHAR tszIndex[64];

		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		memset(tszIndex, '\0', 64);

		_i64tot(st_JsonArray["nID"].asInt64(), tszIndex, 10);
		int nMsgLen = st_JsonArray["tszContext"].asString().length();
		BaseLib_Charset_UTFToUnicode(st_JsonArray["tszContext"].asCString(), tszMsgBuffer, &nMsgLen);

		m_ListAnnouncement.InsertItem(i, _T(""));
		m_ListAnnouncement.SetItemText(i, 0, tszIndex);
		m_ListAnnouncement.SetItemText(i, 1, tszMsgBuffer);
		m_ListAnnouncement.SetItemText(i, 2, A2W(st_JsonArray["tszCreateTime"].asCString()));
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}
