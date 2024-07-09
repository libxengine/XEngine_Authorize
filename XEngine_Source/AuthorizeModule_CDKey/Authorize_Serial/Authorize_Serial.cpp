#include "pch.h"
#include "Authorize_Serial.h"
/********************************************************************
//    Created:     2024/07/08  17:30:53
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey\Authorize_Serial\Authorize_Serial.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey\Authorize_Serial
//    File Base:   Authorize_Serial
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     序列卡生成器
//    History:
*********************************************************************/
CAuthorize_Serial::CAuthorize_Serial()
{
    memset(tszTimerSecond, '\0', sizeof(tszTimerSecond));
    memset(tszTimerDay, '\0', sizeof(tszTimerDay));
    memset(tszTimerTime, '\0', sizeof(tszTimerTime));
    memset(tszTimerCustom, '\0', sizeof(tszTimerCustom));

    _tcsxcpy(tszTimerSecond, _X("XYRYS"));
    _tcsxcpy(tszTimerDay, _X("XYRYD"));
    _tcsxcpy(tszTimerTime, _X("XYRYT"));
    _tcsxcpy(tszTimerCustom, _X("XYRYC"));
}
CAuthorize_Serial::~CAuthorize_Serial()
{

}
//////////////////////////////////////////////////////////////////////////
//                        公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Authorize_Serial_Creator
函数功能：开始生成指定的序列号
 参数.一：ppptszSerialNumber
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出序列卡列表,根据nCardNumber 来确定列表个数
 参数.二：lpszUserHeader
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：自定义头，长度为6，填充5 比如 XHEAD
 参数.三：nCardNumber
  In/Out：In
  类型：整数型
  可空：N
  意思：要生成多少张卡，生成多了，此函数会阻塞程序，那么请放置线程中运行
 参数.四：nFieldNumber
  In/Out：In
  类型：整数型
  可空：N
  意思：卡的段数，AAAAA-BBBBB-CCCCC-DDDDD-EEEEE,不包含三段自定义头,总字段:3 + nFieldNumber
 参数.五：pSt_CustomTimer
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：如果第五个参数为自定义日期，那么这个参数必须赋值。如果第五个参数为天数卡，这个结构体的nDay 必须赋值，表明拥有几天。
        如果是分钟卡,那么这个值的nMinute必须有值,表示拥有的分钟.如果是次数卡,那么这个值的wFlags必须有值,表示拥有的次数,其他类型可以为NULL
 参数.六：enSerialType
  In/Out：In
  类型：枚举型
  可空：Y
  意思：要生成的卡类型
返回值
  类型：逻辑型
  意思：是否生成成功
备注：输入的时间不能超过99999,多天卡现在由用户直接使用pSt_CustomTimer参数指定,不在内部指定了
*********************************************************************/
bool CAuthorize_Serial::Authorize_Serial_Creator(XCHAR*** ppptszSerialNumber, LPCXSTR lpszUserHeader, int nCardNumber, int nFieldNumber, XENGINE_LIBTIMER* pSt_CustomTimer, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType /* = ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_DAY */)
{
    Authorize_IsErrorOccur = false;
    //参数检查
    if ((NULL == ppptszSerialNumber) || (NULL == lpszUserHeader))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
        return false;
    }
    BaseLib_OperatorMemory_Malloc((XPPPMEM)ppptszSerialNumber, nCardNumber, MAX_PATH);
    srand((unsigned int)time(NULL));                //取随机因子

    for (int i = 0; i < nCardNumber; i++)
    {
        XCHAR tszSerialNumber[MAX_PATH];
        memset(tszSerialNumber, '\0', MAX_PATH);
        //检查生成类型
        switch (enSerialType)
        {
        case ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_SECOND:
        {
            if (NULL == pSt_CustomTimer)
            {
                Authorize_IsErrorOccur = true;
                Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
                return false;
            }
            XCHAR tszFillBuffer[10];
            XCHAR tszDayBuffer[10];
            memset(tszFillBuffer, '\0', sizeof(tszFillBuffer));
            memset(tszDayBuffer, '\0', sizeof(tszDayBuffer));

            Authorize_Serial_RandomWord(tszFillBuffer);
            _xstprintf(tszDayBuffer, _X("%d"), pSt_CustomTimer->wSecond);
            memcpy(tszFillBuffer + (_tcsxlen(tszFillBuffer) - _tcsxlen(tszDayBuffer)), tszDayBuffer, _tcsxlen(tszDayBuffer));

            _xstprintf(tszSerialNumber, _X("%s-%s-%s"), lpszUserHeader, tszTimerSecond, tszFillBuffer);
            if (!Authorize_Serial_Fill(tszSerialNumber, nFieldNumber))
            {
                return false;
            }
        }
        break;
        case ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_DAY:
        {
            if (NULL == pSt_CustomTimer)
            {
                Authorize_IsErrorOccur = true;
                Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
                return false;
            }
            XCHAR tszFillBuffer[10];
            XCHAR tszDayBuffer[10];
            memset(tszFillBuffer, '\0', sizeof(tszFillBuffer));
            memset(tszDayBuffer, '\0', sizeof(tszDayBuffer));

            Authorize_Serial_RandomWord(tszFillBuffer);
            _xstprintf(tszDayBuffer, _X("%d"), pSt_CustomTimer->wDay);
            memcpy(tszFillBuffer + (_tcsxlen(tszFillBuffer) - _tcsxlen(tszDayBuffer)), tszDayBuffer, _tcsxlen(tszDayBuffer));

            _xstprintf(tszSerialNumber, _X("%s-%s-%s"), lpszUserHeader, tszTimerDay, tszFillBuffer);
            if (!Authorize_Serial_Fill(tszSerialNumber, nFieldNumber))
            {
                return false;
            }
        }
        break;
        case ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_TIME:
            XCHAR tszFillBuffer[10];
            XCHAR tszTimeBuffer[10];
            memset(tszFillBuffer, '\0', sizeof(tszFillBuffer));
            memset(tszTimeBuffer, '\0', sizeof(tszTimeBuffer));

            Authorize_Serial_RandomWord(tszFillBuffer);
            _xstprintf(tszTimeBuffer, _X("%d"), pSt_CustomTimer->wFlags);
            memcpy(tszFillBuffer + (_tcsxlen(tszFillBuffer) - _tcsxlen(tszTimeBuffer)), tszTimeBuffer, _tcsxlen(tszTimeBuffer));

            _xstprintf(tszSerialNumber, _X("%s-%s-%s"), lpszUserHeader, tszTimerTime, tszFillBuffer);
            if (!Authorize_Serial_Fill(tszSerialNumber, nFieldNumber))
            {
                return false;
            }
            break;
        case ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_CUSTOM:
            if (NULL == pSt_CustomTimer)
            {
                Authorize_IsErrorOccur = true;
                Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
                return false;
            }
            XCHAR tszTimer[128];
            memset(tszTimer, '\0', 128);

            if ((pSt_CustomTimer->wHour > 0) || (pSt_CustomTimer->wMinute > 0) || (pSt_CustomTimer->wSecond > 0))
            {
                _xstprintf(tszTimer, _X("%04d%02d%02d-%02d%02d%02d"), pSt_CustomTimer->wYear, pSt_CustomTimer->wMonth, pSt_CustomTimer->wDay, pSt_CustomTimer->wHour, pSt_CustomTimer->wMinute, pSt_CustomTimer->wSecond);
                nFieldNumber--;
            }
            else
            {
                _xstprintf(tszTimer, _X("%04d%02d%02d"), pSt_CustomTimer->wYear, pSt_CustomTimer->wMonth, pSt_CustomTimer->wDay);
            }
            _xstprintf(tszSerialNumber, _X("%s-%s-%s"), lpszUserHeader, tszTimerCustom, tszTimer);
            if (!Authorize_Serial_Fill(tszSerialNumber, nFieldNumber))
            {
                return false;
            }
            break;
        default:
            Authorize_IsErrorOccur = true;
            Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_NOTSUPPORT;
            return false;
        }
        _tcsxcpy((*ppptszSerialNumber)[i], tszSerialNumber);
    }
    return true;
}
/********************************************************************
函数名称：Authorize_Serial_Creator
函数功能：创建自定义无类型序列号
 参数.一：ppptszSerialNumber
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出序列卡列表,根据nCardNumber 来确定列表个数
 参数.二：lpszUserHeader
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：自定义头，长度为6，填充5 比如 XHEAD
 参数.三：nCardNumber
  In/Out：In
  类型：整数型
  可空：N
  意思：要生成多少张卡，生成多了，此函数会阻塞程序，那么请放置线程中运行
 参数.四：nFieldNumber
  In/Out：In
  类型：整数型
  可空：N
  意思：卡的段数，AAAAA-BBBBB-CCCCC-DDDDD-EEEEE
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthorize_Serial::Authorize_Serial_Create(XCHAR*** ppptszSerialNumber, LPCXSTR lpszUserHeader, int nCardNumber, int nFieldNumber)
{
	Authorize_IsErrorOccur = false;
	//参数检查
	if ((NULL == ppptszSerialNumber) || (NULL == lpszUserHeader))
	{
		Authorize_IsErrorOccur = true;
		Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
		return false;
	}
	BaseLib_OperatorMemory_Malloc((XPPPMEM)ppptszSerialNumber, nCardNumber, MAX_PATH);
	srand((unsigned int)time(NULL));                //取随机因子

	for (int i = 0; i < nCardNumber; i++)
	{
        XCHAR tszSerialNumber[MAX_PATH];
		memset(tszSerialNumber, '\0', MAX_PATH);;

		_xstprintf(tszSerialNumber, _X("%s"), lpszUserHeader);
		if (!Authorize_Serial_Fill(tszSerialNumber, nFieldNumber))
		{
			return false;
		}
		_tcsxcpy((*ppptszSerialNumber)[i], tszSerialNumber);
	}
	return true;
}
/********************************************************************
函数名称：Authorize_Serial_GetType
函数功能：获取一个序列号的类型
 参数.一：lpszSerialNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要获取的缓冲区
 参数.二：penSerialType
  In/Out：Out
  类型：枚举型
  可空：Y
  意思：导出此卡的类型
 参数.三：pSt_CustomTimer
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：如果不是自定义类型或者天数卡,或者次数卡，这个参数将不起作用
返回值
  类型：逻辑型
  意思：是否获取成功
备注：
*********************************************************************/
bool CAuthorize_Serial::Authorize_Serial_GetType(LPCXSTR lpszSerialNumber, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE* penSerialType, XENGINE_LIBTIMER* pSt_CustomTimer /* = NULL */)
{
    Authorize_IsErrorOccur = false;

    if (NULL == penSerialType)
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
        return false;
    }

    XCHAR tszBuffer[6];
    XCHAR tszBuffertemp[6];
    XCHAR tszTimeData[128];
    XCHAR tszTimeTime[128];

    memset(tszBuffer, '\0', 6);
    memset(tszBuffertemp, '\0', 6);
    memset(tszTimeData, '\0', sizeof(tszTimeData));
    memset(tszTimeTime, '\0', sizeof(tszTimeTime));
    //获取指定内容
    if (4 != _stxscanf(lpszSerialNumber, _X("%[^-]-%[^-]-%[^-]-%[^-]"), tszBuffertemp, tszBuffer, tszTimeData, tszTimeTime))
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_BUFFER;
        return false;
    }
    //匹配类型
    if (0 == _tcsxncmp(tszTimerSecond, tszBuffer, 5))
    {
        if (NULL == pSt_CustomTimer)
        {
            Authorize_IsErrorOccur = true;
            Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
            return false;
        }
        unsigned int i = 0;
        for (i = 0; i < _tcsxlen(tszTimeData); i++)
        {
            if (0 != isdigit(tszTimeData[i]))
            {
                break;
            }
        }
        XCHAR tszIntBuffer[64];
        memset(tszIntBuffer, '\0', sizeof(tszIntBuffer));
        memcpy(tszIntBuffer, tszTimeData + i, _tcsxlen(tszTimeData) - i);
        pSt_CustomTimer->wSecond = _ttxoi(tszIntBuffer);

        *penSerialType = ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_SECOND;
    }
    else if (0 == _tcsxncmp(tszTimerDay, tszBuffer, 5))
    {
        if (NULL == pSt_CustomTimer)
        {
            Authorize_IsErrorOccur = true;
            Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
            return false;
        }
        unsigned int i = 0;
        for (i = 0; i < _tcsxlen(tszTimeData); i++)
        {
            if (0 != isdigit(tszTimeData[i]))
            {
                break;
            }
        }
        XCHAR tszIntBuffer[64];
        memset(tszIntBuffer, '\0', sizeof(tszIntBuffer));
        memcpy(tszIntBuffer, tszTimeData + i, _tcsxlen(tszTimeData) - i);
        pSt_CustomTimer->wDay = _ttxoi(tszIntBuffer);

        *penSerialType = ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_DAY;
    }
    else if (0 == _tcsxncmp(tszTimerTime, tszBuffer, 5))
    {
        if (NULL == pSt_CustomTimer)
        {
            Authorize_IsErrorOccur = true;
            Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
            return false;
        }
        unsigned int i = 0;
        for (i = 0; i < _tcsxlen(tszTimeData); i++)
        {
            if (0 != isdigit(tszTimeData[i]))
            {
                break;
            }
        }
        XCHAR tszIntBuffer[64];
        memset(tszIntBuffer, '\0', sizeof(tszIntBuffer));
        memcpy(tszIntBuffer, tszTimeData + i, _tcsxlen(tszTimeData) - i);
        pSt_CustomTimer->wFlags = _ttxoi(tszIntBuffer);

        *penSerialType = ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_TIME;
    }
    else if (0 == _tcsxncmp(tszTimerCustom, tszBuffer, 5))
    {
        *penSerialType = ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_CUSTOM;
        if (NULL == pSt_CustomTimer)
        {
            Authorize_IsErrorOccur = true;
            Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT;
            return false;
        }
        if (3 != _stxscanf(tszTimeData, _X("%04d%02d%02d"), &pSt_CustomTimer->wYear, &pSt_CustomTimer->wMonth, &pSt_CustomTimer->wDay))
        {
            Authorize_IsErrorOccur = true;
            Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_CUSTOM;
            return false;
        }
		if (3 != _stxscanf(tszTimeTime, _X("%02d%02d%02d"), &pSt_CustomTimer->wHour, &pSt_CustomTimer->wMinute, &pSt_CustomTimer->wSecond))
		{
            pSt_CustomTimer->wHour = 0; 
            pSt_CustomTimer->wMinute = 0;
            pSt_CustomTimer->wSecond = 0;
		}
    }
    else
    {
        Authorize_IsErrorOccur = true;
        Authorize_dwErrorCode = ERROR_AUTHORIZE_MODULE_CDKEY_UNKNOW;
        return false;
    }
    return true;
}
//////////////////////////////////////////////////////////////////////////
//                        保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Authorize_Serial_Fill
函数功能：填充缓冲区
 参数.一：ptszFillBuffer
  In/Out：In/Out
  类型：字符指针
  可空：N
  意思：输入填充了头和序列号类型的缓冲区 HEARD-XYRYD
 参数.二：nCardNumer
  In/Out：In
  类型：整数型
  可空：N
  意思：要生成的卡的段数
返回值
  类型：逻辑型
  意思：是否生成成功
备注：
*********************************************************************/
bool CAuthorize_Serial::Authorize_Serial_Fill(XCHAR *ptszFillBuffer, int nCardNumer)
{
    Authorize_IsErrorOccur = false;

    int nPosType = 0;
    
    for (int i = 0; i < nCardNumer; i++)
    {
        XCHAR tszRandom[6];
        memset(tszRandom, '\0', 6);
        if (0 == nPosType)
        {
			Authorize_Serial_Random(tszRandom);
			_tcsxcat(ptszFillBuffer, _X("-"));
			_tcsxcat(ptszFillBuffer, tszRandom);
            nPosType = 1;
        }
        else if (1 == nPosType)
        {
			Authorize_Serial_RandomWord(tszRandom);
			_tcsxcat(ptszFillBuffer, _X("-"));
			_tcsxcat(ptszFillBuffer, tszRandom);
            nPosType = 2;
        }
        else
        {
			Authorize_Serial_RandomNumber(tszRandom);
			_tcsxcat(ptszFillBuffer, _X("-"));
			_tcsxcat(ptszFillBuffer, tszRandom);
            nPosType = 0;
        }
    }
    return true;
}
/********************************************************************
函数名称：Authorize_Serial_Random
函数功能：取随机字母数字
 参数.一：ptszFillBuffer
  In/Out：In/Out
  类型：字符指针
  可空：N
  意思：输入已经初始化为0的字符串，内存空间为6，导出生成成功的字符串
返回值
  类型：逻辑型
  意思：是否生成成功
备注：
*********************************************************************/
bool CAuthorize_Serial::Authorize_Serial_Random(XCHAR *ptszFillBuffer)
{
    Authorize_IsErrorOccur = false;
    
    BaseLib_OperatorHandle_CreateStr(ptszFillBuffer, 6, 0, 1);
    return true;
}
/********************************************************************
函数名称：Authorize_Serial_RandomWord
函数功能：取随机字母，大写
 参数.一：ptszFillBuffer
  In/Out：In/Out
  类型：字符指针
  可空：N
  意思：输入已经初始化为0的字符串，内存空间为6，导出生成成功的字符串
返回值
  类型：逻辑型
  意思：是否生成成功
备注：
*********************************************************************/
bool CAuthorize_Serial::Authorize_Serial_RandomWord(XCHAR *ptszFillBuffer)
{
    Authorize_IsErrorOccur = false;

    BaseLib_OperatorHandle_CreateStr(ptszFillBuffer, 1, 2, 1);
    return true;
}
/********************************************************************
函数名称：Authorize_Serial_RandomNumber
函数功能：取随机数
 参数.一：ptszFillBuffer
  In/Out：In/Out
  类型：字符指针
  可空：N
  意思：输入已经初始化为0的字符串，内存空间为6，导出生成成功的字符串
返回值
  类型：逻辑型
  意思：是否生成成功
备注：
*********************************************************************/
bool CAuthorize_Serial::Authorize_Serial_RandomNumber(XCHAR *ptszFilleBuffer)
{
    Authorize_IsErrorOccur = false;

    BaseLib_OperatorHandle_CreateStr(ptszFilleBuffer, 1, 1);
    return true;
}
