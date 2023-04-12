#include "pch.h"
#include "AuthHelp_Windows.h"
/********************************************************************
//    Created:     2022/12/19  16:38:15
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_Windows\AuthHelp_Windows.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_Windows
//    File Base:   AuthHelp_Windows
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     窗口效果函数
//    History:
*********************************************************************/
CAuthHelp_Windows::CAuthHelp_Windows()
{
}
CAuthHelp_Windows::~CAuthHelp_Windows()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：AuthHelp_Windows_Dithering
函数功能：窗口抖动函数
 参数.一：hWnd
  In/Out：In
  类型：窗口句柄
  可空：N
  意思：要设置的窗口
 参数.二：nDitheringDegree
  In/Out：In
  类型：整数型
  可空：Y
  意思：抖动次数，默认4此
 参数.三：nDitheringCareer
  In/Out：In
  类型：整数型
  可空：Y
  意思：抖动间隔，默认30毫秒
返回值
  类型：逻辑型
  意思：是否成功抖动
备注：仿造QQ抖动，支持抖动后播放声音或者不播放，不播放传递NULL
*********************************************************************/
XBOOL CAuthHelp_Windows::AuthHelp_Windows_Dithering(HWND hWnd, int nDitheringDegree /* = 4 */, int nDitheringCareer /* = 30 */)
{
	RECT st_Rect = { '\0' };//定义一个矩形
	//获得窗口边框
	if (!GetWindowRect(hWnd, &st_Rect))
	{
		Help_IsErrorOccur = XTRUE;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_WINDOW_GETRECT;
		return XFALSE;
	}
	for (int i = 0; i < nDitheringDegree; i++)
	{
		Sleep(nDitheringCareer);//延迟。
		MoveWindow(hWnd, st_Rect.left - 1, st_Rect.top - 1, st_Rect.right - st_Rect.left, st_Rect.bottom - st_Rect.top, XTRUE);
		Sleep(nDitheringCareer);
		MoveWindow(hWnd, st_Rect.left - 1, st_Rect.top, st_Rect.right - st_Rect.left, st_Rect.bottom - st_Rect.top, XTRUE);
		Sleep(nDitheringCareer);
		MoveWindow(hWnd, st_Rect.left - 1, st_Rect.top + 1, st_Rect.right - st_Rect.left, st_Rect.bottom - st_Rect.top, XTRUE);
		Sleep(nDitheringCareer);
		MoveWindow(hWnd, st_Rect.left, st_Rect.top + 1, st_Rect.right - st_Rect.left, st_Rect.bottom - st_Rect.top, XTRUE);
		Sleep(nDitheringCareer);
		MoveWindow(hWnd, st_Rect.left + 1, st_Rect.top + 1, st_Rect.right - st_Rect.left, st_Rect.bottom - st_Rect.top, XTRUE);
		Sleep(nDitheringCareer);
		MoveWindow(hWnd, st_Rect.left + 1, st_Rect.top, st_Rect.right - st_Rect.left, st_Rect.bottom - st_Rect.top, XTRUE);
		Sleep(nDitheringCareer);
		MoveWindow(hWnd, st_Rect.left + 1, st_Rect.top - 1, st_Rect.right - st_Rect.left, st_Rect.bottom - st_Rect.top, XTRUE);
		Sleep(nDitheringCareer);
		MoveWindow(hWnd, st_Rect.left, st_Rect.top - 1, st_Rect.right - st_Rect.left, st_Rect.bottom - st_Rect.top, XTRUE);
	}
	MoveWindow(hWnd, st_Rect.left, st_Rect.top, st_Rect.right - st_Rect.left, st_Rect.bottom - st_Rect.top, 1);//归为
	return XTRUE;
}
/********************************************************************
函数名称：AuthHelp_Windows_CreateTooltip
函数功能：为控件句柄添加提示
 参数.一：hWnd
  In/Out：In
  类型：句柄
  可空：N
  意思：控件的句柄
 参数.二：lpszContextOfTip
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：是否添加成功
返回值
  类型：逻辑型
  意思：是否成功添加
备注：
*********************************************************************/
XBOOL CAuthHelp_Windows::AuthHelp_Windows_CreateTooltip(HWND hWnd, LPCXSTR lpszContextOfTip)
{
	Help_IsErrorOccur = XFALSE;

	TOOLINFO tInfo;
	RECT st_Rect = { '\0' };

	HWND hWndEx = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, 0, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, 0, GetModuleHandle(NULL), 0);
	if (NULL == hWndEx)
	{
		Help_IsErrorOccur = XTRUE;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_WINDOW_CREATEWINDOW;
		return XFALSE;
	}
	if (!SetWindowPos(hWndEx, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE))
	{
		Help_IsErrorOccur = XTRUE;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_WINDOW_SETWINDOW;
		return XFALSE;
	}
	GetClientRect(hWnd, &st_Rect);

	tInfo.cbSize = sizeof(TOOLINFO);
	tInfo.uFlags = TTF_SUBCLASS;
	tInfo.hwnd = hWnd;
	tInfo.hinst = GetModuleHandle(NULL);
	tInfo.uId = 0;
	tInfo.lpszText = (LPTSTR)lpszContextOfTip;
	tInfo.rect = st_Rect;
	SendMessage(hWndEx, TTM_ADDTOOL, 0, (LPARAM)&tInfo);
	return XTRUE;
}