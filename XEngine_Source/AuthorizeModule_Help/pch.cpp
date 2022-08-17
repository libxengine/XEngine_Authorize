﻿#include "pch.h"
#include "AuthHelp_ClipBoard/AuthHelp_ClipBoard.h"
/********************************************************************
//    Created:     2022/08/12  14:14:11
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\pch.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
BOOL Help_IsErrorOccur = FALSE;
DWORD Help_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CAuthHelp_ClipBoard m_HelpClipBoard;
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD AuthHelp_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Help_dwErrorCode;
}
/************************************************************************/
/*                    剪贴板导出定义                                    */
/************************************************************************/
#ifdef _MSC_BUILD
extern "C" BOOL AuthHelp_ClipBoard_Set(LPCTSTR lpszMsgBuffer, int nMsgLen, DWORD dwFormat)
{
	return m_HelpClipBoard.AuthHelp_ClipBoard_Set(lpszMsgBuffer, nMsgLen, dwFormat);
}
extern "C" BOOL AuthHelp_ClipBoard_Get(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, DWORD dwFormat)
{
	return m_HelpClipBoard.AuthHelp_ClipBoard_Get(ptszMsgBuffer, pInt_MsgLen, dwFormat);
}
extern "C" BOOL AuthHelp_ClipBoard_Clear()
{
	return m_HelpClipBoard.AuthHelp_ClipBoard_Clear();
}
#endif