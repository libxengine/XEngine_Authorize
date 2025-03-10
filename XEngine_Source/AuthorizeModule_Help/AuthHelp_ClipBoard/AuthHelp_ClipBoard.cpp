﻿#include "pch.h"
#include "AuthHelp_ClipBoard.h"
/********************************************************************
//    Created:     2022/08/12  14:13:21
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_ClipBoard\AuthHelp_ClipBoard.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_ClipBoard
//    File Base:   AuthHelp_ClipBoard
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     剪贴板通信实现
//    History:
*********************************************************************/
CAuthHelp_ClipBoard::CAuthHelp_ClipBoard()
{
}
CAuthHelp_ClipBoard::~CAuthHelp_ClipBoard()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：AuthHelp_ClipBoard_Set
函数功能：设置剪贴板内容
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要拷贝到剪贴板的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.三：dwFormat
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入格式,格式需要参考:https://docs.microsoft.com/en-us/windows/win32/dataxchg/standard-clipboard-formats
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthHelp_ClipBoard::AuthHelp_ClipBoard_Set(LPCTSTR lpszMsgBuffer, int nMsgLen, XLONG dwFormat)
{
	Help_IsErrorOccur = true;

	if (NULL == lpszMsgBuffer)
	{
		Help_IsErrorOccur = true;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_CLIPBOARD_PARRAMENT;
		return false;
	}
	//判断先我们可否打开剪贴板，如果可以
	if (!OpenClipboard(NULL))
	{
		Help_IsErrorOccur = true;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_CLIPBOARD_OPEN;
		return false;
	}
	if (!EmptyClipboard())
	{
		Help_IsErrorOccur = true;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_CLIPBOARD_OWNER;
		return false;
	}
	HANDLE hGlobal = INVALID_HANDLE_VALUE;
	//GlobalAlloc 是分配指定的内存空间 单位为字节
	hGlobal = GlobalAlloc(GMEM_MOVEABLE, nMsgLen * sizeof(TCHAR) + 1);
	if (NULL == hGlobal)
	{
		Help_IsErrorOccur = true;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_CLIPBOARD_MALLOC;
		return false;
	}
	TCHAR* ptszBuffer = (TCHAR*)GlobalLock(hGlobal); //锁定一个全局内存对象 并且返回一个指向其第一个内存地址的指针 返回类型为 XPVOID
	memcpy(ptszBuffer, lpszMsgBuffer, nMsgLen * sizeof(TCHAR));

#ifdef _UNICODE
	SetClipboardData(CF_UNICODETEXT, hGlobal);       
#else
	SetClipboardData(CF_TEXT, hGlobal);    
#endif

	GlobalUnlock(hGlobal);                     //解锁。这样 其他程序才能操作这款内存！
	GlobalFree(hGlobal);                       //释放这个申请的空间
	CloseClipboard();                          //关闭这个剪贴板 其他进程才能操作！

	return true;
}
/********************************************************************
函数名称：AuthHelp_ClipBoard_Get
函数功能：获取剪贴板内容
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获取到的内容
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出获取的内容大小
 参数.二：dwFormat
  In/Out：In
  类型：整数型
  可空：Y
  意思：要获取的内容格式
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthHelp_ClipBoard::AuthHelp_ClipBoard_Get(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XLONG dwFormat /* = 1 */)
{
	Help_IsErrorOccur = false;

	if (NULL == ptszMsgBuffer)
	{
		Help_IsErrorOccur = true;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_CLIPBOARD_PARRAMENT;
		return false;
	}
	if (!OpenClipboard(NULL))
	{
		Help_IsErrorOccur = true;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_CLIPBOARD_OPEN;
		return false;
	}

	HANDLE hCliBd = INVALID_HANDLE_VALUE;
	if (NULL == (hCliBd = GetClipboardData(dwFormat)))
	{
		Help_IsErrorOccur = true;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_CLIPBOARD_GETDATA;
		return false;
	}

	TCHAR* ptszBuffer = (TCHAR*)GlobalLock(hCliBd);        //将句柄转化为地址
	*pInt_MsgLen = (int)GlobalSize(hCliBd) - 1;                 //剪贴板内容大小

	memcpy(ptszMsgBuffer, ptszBuffer, *pInt_MsgLen);

	GlobalUnlock(hCliBd);
	CloseClipboard();

	return true;
}
/********************************************************************
函数名称：AuthHelp_ClipBoard_Clear
函数功能：清空剪贴板
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthHelp_ClipBoard::AuthHelp_ClipBoard_Clear()
{
	Help_IsErrorOccur = false;

	OpenClipboard(NULL);
	EmptyClipboard();
	CloseClipboard();
	return true;
}