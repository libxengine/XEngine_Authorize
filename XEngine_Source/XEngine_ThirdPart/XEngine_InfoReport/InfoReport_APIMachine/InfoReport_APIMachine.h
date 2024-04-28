#pragma once
/********************************************************************
//    Created:     2024/04/28  15:16:03
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XEngine_ThirdPart\XEngine_InfoReport\InfoReport_APIMachine\InfoReport_APIMachine.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\XEngine_ThirdPart\XEngine_InfoReport\InfoReport_APIMachine
//    File Base:   InfoReport_APIMachine
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     信息收集报告类
//    History:
*********************************************************************/

class CInfoReport_APIMachine
{
public:
    CInfoReport_APIMachine();
    ~CInfoReport_APIMachine();
public:
    bool InfoReport_APIMachine_Send(LPCXSTR lpszAPIUrl);
    bool InfoReport_APIMachine_GetTime(LPCXSTR lpszAPIUrl, __int64x* pInt_TimeNumber);
protected:
    bool InfoReport_APIMachine_GetText(XCHAR *ptszMSGBuffer);
};
