#pragma once
/********************************************************************
//    Created:     2022/08/12  14:13:29
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_ClipBoard\AuthHelp_ClipBoard.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_ClipBoard
//    File Base:   AuthHelp_ClipBoard
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     剪贴板通信实现
//    History:
*********************************************************************/

class CAuthHelp_ClipBoard
{
public:
	CAuthHelp_ClipBoard();
	~CAuthHelp_ClipBoard();
public:
	BOOL AuthHelp_ClipBoard_Set(LPCTSTR lpszMsgBuffer, int nMsgLen, DWORD dwFormat = 1);
	BOOL AuthHelp_ClipBoard_Get(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, DWORD dwFormat = 1);
	BOOL AuthHelp_ClipBoard_Clear();
private:
};