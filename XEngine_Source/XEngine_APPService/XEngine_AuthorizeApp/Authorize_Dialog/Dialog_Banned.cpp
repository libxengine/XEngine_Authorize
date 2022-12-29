// Dialog_Banned.cpp: 实现文件
//

#include "pch.h"
#include "../XEngine_AuthorizeApp.h"
#include "afxdialogex.h"
#include "Dialog_Banned.h"


// CDialog_Banned 对话框

IMPLEMENT_DYNAMIC(CDialog_Banned, CDialogEx)

CDialog_Banned::CDialog_Banned(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BANNED, pParent)
{

}

CDialog_Banned::~CDialog_Banned()
{
}

void CDialog_Banned::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialog_Banned, CDialogEx)
END_MESSAGE_MAP()


// CDialog_Banned 消息处理程序
