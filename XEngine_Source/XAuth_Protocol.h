#pragma once
/********************************************************************
//    Created:     2021/07/14  13:27:45
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XAuth_Protocol.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source
//    File Base:   XAuth_Protocol
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     验证协议头
//    History:
*********************************************************************/
#define XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME 32
//////////////////////////////////////////////////////////////////////////
//                         导出的数据结构
//////////////////////////////////////////////////////////////////////////
//充值协议
typedef struct
{
	CHAR tszUserName[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];          //用户
	CHAR tszSerialNumber[128];                                           //序列号
}AUTHREG_PROTOCOL_USERPAY, * LPAUTHREG_PROTOCOL_USERPAY;
//时间信息协议
typedef struct
{
	CHAR tszUserAddr[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];           //地址
	CHAR tszUserName[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];           //用户
	CHAR tszLeftTime[64];                                                 //过期日期
	__int64x nTimeLeft;                                                   //剩余时间
	__int64x nTimeONLine;                                                 //在线时间
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType;
}AUTHREG_PROTOCOL_TIME, * LPAUTHREG_PROTOCOL_TIME;
//用户表
typedef struct tag_AuthReg_UserTable
{
	XENGINE_PROTOCOL_USERREG st_UserInfo;
	CHAR tszLeftTime[64];                                                 //剩余日期
	CHAR tszHardCode[32];                                                 //硬件码
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthRegSerialType;                 //充值卡类型
}AUTHREG_USERTABLE, * LPAUTHREG_USERTABLE;
//注册序列号表
typedef struct tag_AuthReg_SerialTable
{
	CHAR tszUserName[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];        //使用者是谁
	CHAR tszSerialNumber[128];                                           //序列号
	CHAR tszMaxTime[64];                                                 //使用时间
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthRegSerialType;                 //充值卡类型
	BOOL bIsUsed;                                                         //是否已经使用
}AUTHREG_SERIALTABLE, * LPAUTHREG_SERIALTABLE;
//网络临时验证表
typedef struct tag_AuthReg_NetVer
{
	CHAR tszVerSerial[256];                                               //验证序列号
	CHAR tszVerData[64];                                                  //验证时间
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode;                            //验证方式
	int nTryTime;                                                         //试用时间
}AUTHREG_NETVER, * LPAUTHREG_NETVER;