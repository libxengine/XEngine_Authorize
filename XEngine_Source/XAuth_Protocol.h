﻿#pragma once
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
//                         导出的协议定义
//////////////////////////////////////////////////////////////////////////
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQDEL 0x2001          //删除用户
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPDEL 0x2002
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQREGISTER 0x2003     //注册
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPREGISTER 0x2004
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN 0x2005        //登陆
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN 0x2006
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQPAY 0x2007          //充值
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPPAY 0x2008
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETPASS 0x2009      //找回密码
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETPASS 0x200A
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETTIME 0x200B      //获取剩余时间
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETTIME 0x200C
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQTRYVER 0x200D       //临时验证协议请求
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPTRYVER 0x200E
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQDCODE 0x2010        //动态验证码请求
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPDCODE 0x2011     
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_TIMEDOUT 0x2FFF        //通知客户端时间到期
//////////////////////////////////////////////////////////////////////////
//                         导出的数据结构
//////////////////////////////////////////////////////////////////////////
//充值协议
typedef struct
{
	XCHAR tszUserName[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];          //用户
	XCHAR tszSerialNumber[128];                                           //序列号
}AUTHREG_PROTOCOL_USERPAY, * LPAUTHREG_PROTOCOL_USERPAY;
//时间信息协议
typedef struct
{
	XCHAR tszUserAddr[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];           //地址
	XCHAR tszUserName[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];           //用户
	XCHAR tszLeftTime[64];                                                 //过期日期
	__int64x nTimeLeft;                                                   //剩余时间
	__int64x nTimeONLine;                                                 //在线时间
	int nNetType;                                                         //连接类型
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType;
	ENUM_PROTOCOLDEVICE_TYPE enDeviceType;
}AUTHREG_PROTOCOL_TIME, * LPAUTHREG_PROTOCOL_TIME;
//用户表
typedef struct tag_AuthReg_UserTable
{
	XENGINE_PROTOCOL_USERINFO st_UserInfo;
	XCHAR tszLeftTime[64];                                                 //剩余日期
	XCHAR tszHardCode[32];                                                 //硬件码
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType;               //充值卡类型
	ENUM_PROTOCOLDEVICE_TYPE enDeviceType;                                //设备类型
}AUTHREG_USERTABLE, * LPAUTHREG_USERTABLE;
//注册序列号表
typedef struct tag_AuthReg_SerialTable
{
	XCHAR tszUserName[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];           //使用者是谁
	XCHAR tszSerialNumber[128];                                            //序列号
	XCHAR tszMaxTime[64];                                                  //使用时间
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType;               //充值卡类型
	int bIsUsed;                                                         //是否已经使用
	XCHAR tszCreateTime[64];                                               //创建时间
}AUTHREG_SERIALTABLE, * LPAUTHREG_SERIALTABLE;
//网络临时验证表
typedef struct
{
	XCHAR tszVSerial[256];                                               //验证序列号
	XCHAR tszVDate[64];                                                  //验证时间
	__int64x nID;                                                        //ID
	int nVTime;                                                          //试用时间
	int nLTime;                                                          //剩余时间
	ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVMode;                   //验证方式
}AUTHREG_TEMPVER;
//禁止列表
typedef struct 
{
	__int64x nID;                                                         //ID
	XCHAR tszUserName[MAX_PATH];                                           //用户名
	XCHAR tszIPAddr[128];                                                  //IP
	XCHAR tszLeftTime[64];                                                 //过期时间
	XCHAR tszCreateTime[64];                                               //创建时间
	bool bEnable;                                                         //是否启用
}AUTHREG_BANNED, * LPAUTHREG_BANNED;
//公告信息
typedef struct
{
	XCHAR tszContext[4096];                                                //信息
	XCHAR tszCreateTime[64];                                               //创建时间
	__int64x nID;                                                         //ID
}AUTHREG_ANNOUNCEMENT, * LPAUTHREG_ANNOUNCEMENT;