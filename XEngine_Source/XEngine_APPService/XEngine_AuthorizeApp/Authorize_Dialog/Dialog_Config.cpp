// Dialog_Config.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "afxdialogex.h"
#include "Dialog_Config.h"


// CDialog_Config 对话框

IMPLEMENT_DYNAMIC(CDialog_Config, CDialogEx)

CDialog_Config::CDialog_Config(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CONFIG, pParent)
{

}

CDialog_Config::~CDialog_Config()
{
}

void CDialog_Config::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditIPAddr);
	DDX_Control(pDX, IDC_EDIT2, m_EditIPPort);
}


BEGIN_MESSAGE_MAP(CDialog_Config, CDialogEx)
END_MESSAGE_MAP()


// CDialog_Config 消息处理程序


BOOL CDialog_Config::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_EditIPAddr.SetWindowText(_T("127.0.0.1"));
	m_EditIPPort.SetWindowText(_T("5302"));

	hConfigWnd = m_hWnd;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
