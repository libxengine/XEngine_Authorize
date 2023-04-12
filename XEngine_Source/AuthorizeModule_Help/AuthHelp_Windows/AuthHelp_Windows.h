#pragma once
/********************************************************************
//    Created:     2022/12/19  16:37:34
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_Windows\AuthHelp_Windows.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_Windows
//    File Base:   AuthHelp_Windows
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     窗口效果函数
//    History:
*********************************************************************/

class CAuthHelp_Windows
{
public:
	CAuthHelp_Windows();
	~CAuthHelp_Windows();
public:
	XBOOL AuthHelp_Windows_Dithering(HWND hWnd, int nDitheringDegree = 4, int nDitheringCareer = 30);
	XBOOL AuthHelp_Windows_CreateTooltip(HWND hWnd, LPCXSTR lpszContextOfTip);
private:
};