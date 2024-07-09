#pragma once
/********************************************************************
//    Created:     2024/07/08  17:30:06
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey\Authorize_Serial\Authorize_Serial.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey\Authorize_Serial
//    File Base:   Authorize_Serial
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     序列卡生成器
//    History:
*********************************************************************/


class CAuthorize_Serial
{
public:
    CAuthorize_Serial();
    ~CAuthorize_Serial();
public:
    bool Authorize_Serial_Creator(XCHAR*** ppptszSerialNumber, LPCXSTR lpszUserHeader, int nCardNumber, int nFieldNumber, XENGINE_LIBTIMER* pSt_CustomTimer, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType = ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_DAY);
    bool Authorize_Serial_Create(XCHAR*** ppptszSerialNumber, LPCXSTR lpszUserHeader, int nCardNumber, int nFieldNumber);
    bool Authorize_Serial_GetType(LPCXSTR lpszSerialNumber, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE* penSerialType, XENGINE_LIBTIMER* pSt_CustomTimer = NULL);
protected:
    bool Authorize_Serial_Fill(XCHAR *ptszFillBuffer, int nCardNumer);            //填充后面的字段
protected:
    bool Authorize_Serial_Random(XCHAR *ptszFillBuffer);                         //取随机字母数字
    bool Authorize_Serial_RandomWord(XCHAR *ptszFillBuffer);                     //随机取单词
    bool Authorize_Serial_RandomNumber(XCHAR *ptszFilleBuffer);                  //取随机数
private:
    XCHAR tszTimerSecond[6];               //按照秒钟时间格式
    XCHAR tszTimerDay[6];                  //按照天的时间格式
    XCHAR tszTimerTime[6];                 //次数卡
    XCHAR tszTimerCustom[32];              //自定义到期时间
};
