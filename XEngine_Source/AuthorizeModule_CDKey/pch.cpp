#include "pch.h"
#include "Authorize_CDKey/Authorize_CDKey.h"
#include "Authorize_Serial/Authorize_Serial.h"
/********************************************************************
//    Created:     2021/07/20  13:11:54
//    File Name:   D:\XEngine\XEngine_SourceCode\XEngine_HelpComponents\HelpComponents_Authorize\pch.cpp
//    File Path:   D:\XEngine\XEngine_SourceCode\XEngine_HelpComponents\HelpComponents_Authorize
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool Authorize_IsErrorOccur = false;
XLONG Authorize_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CAuthorize_Serial m_Serial;
CAuthorize_CDKey m_Local;
//////////////////////////////////////////////////////////////////////////
//                             导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Authorize_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Authorize_dwErrorCode;
}
/************************************************************************/
/*                           卡导出函数                                 */
/************************************************************************/
extern "C" bool Authorize_Serial_Creator(XCHAR * **ppptszSerialNumber, LPCXSTR lpszUserHeader, int nCardNumber, int nFieldNumber, XENGINE_LIBTIME * pSt_CustomTimer, ENUM_AUTHORIZE_MODULE_SERIAL_TYPE enSerialType)
{
	return m_Serial.Authorize_Serial_Creator(ppptszSerialNumber, lpszUserHeader, nCardNumber, nFieldNumber, pSt_CustomTimer, enSerialType);
}
extern "C" bool Authorize_Serial_Create(XCHAR * **ppptszSerialNumber, LPCXSTR lpszUserHeader, int nCardNumber, int nFieldNumber)
{
	return m_Serial.Authorize_Serial_Create(ppptszSerialNumber, lpszUserHeader, nCardNumber, nFieldNumber);
}
extern "C" bool Authorize_Serial_GetType(LPCXSTR lpszSerialNumber, ENUM_AUTHORIZE_MODULE_SERIAL_TYPE * penSerialType, XENGINE_LIBTIME * pSt_CustomTimer)
{
	return m_Serial.Authorize_Serial_GetType(lpszSerialNumber, penSerialType, pSt_CustomTimer);
}
/************************************************************************/
/*                           本地授权导出函数                           */
/************************************************************************/
extern "C" bool Authorize_CDKey_WriteKey(LPCXSTR lpszFileKey, XENGINE_AUTHORIZE_LOCAL * pSt_AuthLocal)
{
	return m_Local.Authorize_CDKey_WriteKey(lpszFileKey, pSt_AuthLocal);
}
extern "C" bool Authorize_CDKey_ReadKey(LPCXSTR lpszFileKey, XENGINE_AUTHORIZE_LOCAL * pSt_AuthLocal)
{
	return m_Local.Authorize_CDKey_ReadKey(lpszFileKey, pSt_AuthLocal);
}
extern "C" bool Authorize_CDKey_WriteMemory(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_AUTHORIZE_LOCAL * pSt_AuthLocal)
{
	return m_Local.Authorize_CDKey_WriteMemory(ptszMsgBuffer, pInt_MsgLen, pSt_AuthLocal);
}
extern "C" bool Authorize_CDKey_ReadMemory(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_AUTHORIZE_LOCAL * pSt_AuthLocal)
{
	return m_Local.Authorize_CDKey_ReadMemory(lpszMsgBuffer, nMsgLen, pSt_AuthLocal);
}
extern "C" bool Authorize_CDKey_BuildKeyTime(XENGINE_AUTHORIZE_LOCAL * pSt_AuthLocal, __int64x nDayTimer, XENGINE_LIBTIME * pSt_DayTimer)
{
	return m_Local.Authorize_CDKey_BuildKeyTime(pSt_AuthLocal, nDayTimer, pSt_DayTimer);
}
extern "C" bool Authorize_CDKey_UserRegister(XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal, LPCXSTR lpszSerialStr)
{
	return m_Local.Authorize_CDKey_UserRegister(pSt_AuthLocal, lpszSerialStr);
}
extern "C" bool Authorize_CDKey_GetLeftTimer(XENGINE_AUTHORIZE_LOCAL * pSt_AuthLocal, LPCXSTR lpszSerialNumber)
{
	return m_Local.Authorize_CDKey_GetLeftTimer(pSt_AuthLocal, lpszSerialNumber);
}
extern "C" bool Authorize_CDKey_WriteTime(LPCXSTR lpszFileKey, int nCount)
{
	return m_Local.Authorize_CDKey_WriteTime(lpszFileKey, nCount);
}
extern "C" bool Authorize_CDKey_ReadTime(LPCXSTR lpszFileKey, XCHAR * **ppptszTimeList, int* pInt_ListCount)
{
	return m_Local.Authorize_CDKey_ReadTime(lpszFileKey, ppptszTimeList, pInt_ListCount);
}