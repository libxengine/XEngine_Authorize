#include "pch.h"
#include "AuthHelp_DynamicCode/AuthHelp_DynamicCode.h"
#include "AuthHelp_ClipBoard/AuthHelp_ClipBoard.h"
#include "AuthHelp_Windows/AuthHelp_Windows.h"
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
bool Help_IsErrorOccur = false;
XLONG Help_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CAuthHelp_DynamicCode m_HelpDynamic;
CAuthHelp_ClipBoard m_HelpClipBoard;
CAuthHelp_Windows m_HelpWindow;
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG AuthHelp_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Help_dwErrorCode;
}
/************************************************************************/
/*                    动态码导出定义                                    */
/************************************************************************/
extern "C" bool AuthHelp_DynamicCode_Init(int nSecond)
{
	return m_HelpDynamic.AuthHelp_DynamicCode_Init(nSecond);
}
extern "C" bool AuthHelp_DynamicCode_Destory()
{
	return m_HelpDynamic.AuthHelp_DynamicCode_Destory();
}
extern "C" bool AuthHelp_DynamicCode_Create(XNETHANDLE * pxhToken, int* pInt_DynamicCode)
{
	return m_HelpDynamic.AuthHelp_DynamicCode_Create(pxhToken, pInt_DynamicCode);
}
extern "C" bool AuthHelp_DynamicCode_Get(XNETHANDLE xhToken, int nDynamicCode)
{
	return m_HelpDynamic.AuthHelp_DynamicCode_Get(xhToken, nDynamicCode);
}
/************************************************************************/
/*                    剪贴板导出定义                                    */
/************************************************************************/
#ifdef _MSC_BUILD
extern "C" bool AuthHelp_ClipBoard_Set(LPCXSTR lpszMsgBuffer, int nMsgLen, XLONG dwFormat)
{
	return m_HelpClipBoard.AuthHelp_ClipBoard_Set(lpszMsgBuffer, nMsgLen, dwFormat);
}
extern "C" bool AuthHelp_ClipBoard_Get(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, XLONG dwFormat)
{
	return m_HelpClipBoard.AuthHelp_ClipBoard_Get(ptszMsgBuffer, pInt_MsgLen, dwFormat);
}
extern "C" bool AuthHelp_ClipBoard_Clear()
{
	return m_HelpClipBoard.AuthHelp_ClipBoard_Clear();
}
/************************************************************************/
/*                    窗口类导出定义                                    */
/************************************************************************/
extern "C" bool AuthHelp_Windows_Dithering(HWND hWnd, int nDitheringDegree, int nDitheringCareer)
{
	return m_HelpWindow.AuthHelp_Windows_Dithering(hWnd, nDitheringDegree, nDitheringCareer);
}
extern "C" bool AuthHelp_Windows_CreateTooltip(HWND hWnd, LPCXSTR lpszContextOfTip)
{
	return m_HelpWindow.AuthHelp_Windows_CreateTooltip(hWnd, lpszContextOfTip);
}
#endif