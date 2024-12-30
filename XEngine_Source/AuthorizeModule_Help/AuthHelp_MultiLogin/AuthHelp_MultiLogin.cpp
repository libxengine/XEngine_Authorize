#include "pch.h"
#include "AuthHelp_MultiLogin.h"
/********************************************************************
//    Created:     2023/08/09  15:10:52
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_MultiLogin\AuthHelp_MultiLogin.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_MultiLogin
//    File Base:   AuthHelp_MultiLogin
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     多端登录处理代码
//    History:
*********************************************************************/
CAuthHelp_MultiLogin::CAuthHelp_MultiLogin()
{
}
CAuthHelp_MultiLogin::~CAuthHelp_MultiLogin()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：AuthHelp_MultiLogin_GetRange
函数功能：获取多端登录范围
 参数.一：nClientDevice
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要判断的设备类型
 参数.二：pInt_IDType
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出设备范围类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthHelp_MultiLogin::AuthHelp_MultiLogin_GetRange(int nClientDevice, int* pInt_IDType)
{
	Help_IsErrorOccur = true;

	//在范围
	switch (nClientDevice)
	{
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC_WINDOWS:
		*pInt_IDType = 1;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC_LINUX:
		*pInt_IDType = 1;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC_UNIX:
		*pInt_IDType = 1;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC_MACOS:
		*pInt_IDType = 1;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_PAD_SURFACE:
		*pInt_IDType = 2;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_PAD_ANDROID:
		*pInt_IDType = 2;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_PAD_IPAD:
		*pInt_IDType = 2;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_MOBILE_ANDROID:
		*pInt_IDType = 3;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_MOBILE_IOS:
		*pInt_IDType = 3;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_MOBILE_EMBEDDED:
		*pInt_IDType = 3;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB_IE:
		*pInt_IDType = 4;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB_EDGE:
		*pInt_IDType = 4;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB_CHROME:
		*pInt_IDType = 4;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB_FIREFOX:
		*pInt_IDType = 4;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB_SAFARI:
		*pInt_IDType = 4;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB_OPERA:
		*pInt_IDType = 4;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB_RESTFUL:
		*pInt_IDType = 4;
		break;
	case ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB_LIKEGG:
		*pInt_IDType = 4;
		break;
	default:
		Help_IsErrorOccur = true;
		Help_dwErrorCode = ERROR_AUTHORIZE_MODULE_HELP_MULTI_NOTSUPPORT;
		return false;
	}

	return true;
}
/********************************************************************
函数名称：AuthHelp_MultiLogin_TimeMatch
函数功能：判断登录时间是否匹配当前系统时间
 参数.一：lpszLoginTime
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要判断的时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthHelp_MultiLogin::AuthHelp_MultiLogin_TimeMatch(LPCXSTR lpszLoginTime)
{
	Help_IsErrorOccur = true;

	XENGINE_LIBTIMER st_TimeLogin = {};
	XENGINE_LIBTIMER st_TimeLocal = {};
	BaseLib_Time_GetSysTime(&st_TimeLocal);
	BaseLib_Time_StrToTime(lpszLoginTime, &st_TimeLogin);
	
	bool bRet = false;
	if ((st_TimeLogin.wYear == st_TimeLocal.wYear) && (st_TimeLogin.wMonth == st_TimeLocal.wMonth) && (st_TimeLogin.wDay == st_TimeLocal.wDay))
	{
		bRet = true;
	}
	return bRet;
}