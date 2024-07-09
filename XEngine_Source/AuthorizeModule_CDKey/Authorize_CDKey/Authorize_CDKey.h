#pragma once
/********************************************************************
//    Created:     2024/07/08  17:27:26
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey\Authorize_CDKey\Authorize_CDKey.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey\Authorize_CDKey
//    File Base:   Authorize_CDKey
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     本地授权服务
//    History:
*********************************************************************/

class CAuthorize_CDKey
{
public:
	CAuthorize_CDKey();
	~CAuthorize_CDKey();
public:
	bool Authorize_CDKey_WriteKey(LPCXSTR lpszFileKey, XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal);
	bool Authorize_CDKey_ReadKey(LPCXSTR lpszFileKey, XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal);
	bool Authorize_CDKey_WriteMemory(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal);
	bool Authorize_CDKey_ReadMemory(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal);
public:
	bool Authorize_CDKey_BuildKeyTime(XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal, __int64x nDayTimer = 0, XENGINE_LIBTIMER* pSt_DayTimer = NULL);
	bool Authorize_CDKey_GetLeftTimer(XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal, LPCXSTR lpszSerialNumber = NULL);
	bool Authorize_CDKey_WriteTime(LPCXSTR lpszFileKey, int nCount = 0);
	bool Authorize_CDKey_ReadTime(LPCXSTR lpszFileKey, XCHAR*** ppptszTimeList, int* pInt_ListCount);
private:
};
