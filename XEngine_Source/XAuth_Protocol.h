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
//                         导出的协议定义
//////////////////////////////////////////////////////////////////////////
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQPAY 0x2001          //充值
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPPAY 0x2002
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETPASS 0x2003      //找回密码
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETPASS 0x2004
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETTIME 0x2005      //获取剩余时间
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETTIME 0x2006
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQDCODE 0x2007        //动态验证码请求
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPDCODE 0x2008     
//////////////////////////////////////////////////////////////////////////
//                            导出的错误码
//////////////////////////////////////////////////////////////////////////
#define ERROR_AUTHORIZE_PROTOCOL_USER 0x100                               //用户错误
#define ERROR_AUTHORIZE_PROTOCOL_PASS 0x101                               //密码错误
#define ERROR_AUTHORIZE_PROTOCOL_CLOSED 0x102                             //此功能关闭
#define ERROR_AUTHORIZE_PROTOCOL_DEVICE 0x103                             //设备类型未知
#define ERROR_AUTHORIZE_PROTOCOL_DYNAMICCODE 0x104                        //动态码错误
#define ERROR_AUTHORIZE_PROTOCOL_PASSVER 0x105                            //三方验证错误
#define ERROR_AUTHORIZE_PROTOCOL_BANNED 0x106                             //用户被禁用
#define ERROR_AUTHORIZE_PROTOCOL_HARDCODE 0x107                           //硬件码错误
#define ERROR_AUTHORIZE_PROTOCOL_LOGINED 0x108                            //已经登录
#define ERROR_AUTHORIZE_PROTOCOL_TIMELEFT 0x109                           //时间耗尽
#define ERROR_AUTHORIZE_PROTOCOL_REQUEST 0x110                            //请求内容错误
#define ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE 0x111                        //未授权
#define ERROR_AUTHORIZE_PROTOCOL_NOTFOUND 0x112                           //未找到请求的数据
#define ERROR_AUTHORIZE_PROTOCOL_NOTSUPPORT 0x113                         //不支持
#define ERROR_AUTHORIZE_PROTOCOL_NOTMATCH 0x114                           //数据不匹配
#define ERROR_AUTHORIZE_PROTOCOL_TIMEOUT 0x115                            //超时
#define ERROR_AUTHORIZE_PROTOCOL_PERMISSION 0x116                         //用户权限错误
#define ERROR_AUTHORIZE_PROTOCOL_EXPIRED 0x117                            //已经过期
#define ERROR_AUTHORIZE_PROTOCOL_CDKEY 0x118                              //CDKEY错误

#define ERROR_AUTHORIZE_PROTOCOL_SERVER 0x201                             //服务端内部错误
#define ERROR_AUTHORIZE_PROTOCOL_LIMIT 0x202                              //服务端内部限制,数据过大后者过小
//////////////////////////////////////////////////////////////////////////
//                            导出的枚举型
//////////////////////////////////////////////////////////////////////////
#ifndef _MSC_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif
static LPCXSTR lpszXSerialType[5] = { "UNKNOW","SECOND","DAY","TIME","CUSTOM" };
static LPCXSTR lpszXRegType[6] = { "UNKNOW","TEMP","TRY","OFFICIAL","UNLIMIT","EXPIRED" };
static LPCXSTR lpszXHDType[6] = { "UNKNOW","CPU","DISK","BOARD","MAC","BIOS" };
static LPCXSTR lpszXVerType[6] = { "UNKNOW","LOCAL","NETWORK" };
#ifndef _MSC_BUILD
#pragma GCC diagnostic pop
#endif
//////////////////////////////////////////////////////////////////////////
//                            导出的结构体
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
	ENUM_VERIFICATION_MODULE_SERIAL_TYPE enSerialType;
	ENUM_PROTOCOLDEVICE_TYPE enDeviceType;
}AUTHREG_PROTOCOL_TIME, * LPAUTHREG_PROTOCOL_TIME;
//用户表
typedef struct tag_AuthReg_UserTable
{
	XENGINE_PROTOCOL_USERINFO st_UserInfo;
	XCHAR tszADDInfo[2048];                                                //附加信息
	XCHAR tszLeftTime[64];                                                 //剩余日期
	XCHAR tszHardCode[32];                                                 //硬件码
	ENUM_VERIFICATION_MODULE_SERIAL_TYPE enSerialType;                        //充值卡类型
	ENUM_PROTOCOLDEVICE_TYPE enDeviceType;                                 //设备类型
}AUTHREG_USERTABLE, * LPAUTHREG_USERTABLE;
//注册序列号表
typedef struct tag_AuthReg_SerialTable
{
	XCHAR tszUserName[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];           //使用者是谁
	XCHAR tszSerialNumber[128];                                            //序列号
	XCHAR tszMaxTime[64];                                                  //使用时间
	ENUM_VERIFICATION_MODULE_SERIAL_TYPE enSerialType;                        //充值卡类型
	int bIsUsed;                                                           //是否已经使用
	XCHAR tszCreateTime[64];                                               //创建时间
	XCHAR tszExpiredTime[64];                                              //过期时间
}AUTHREG_SERIALTABLE, * LPAUTHREG_SERIALTABLE;
//网络临时验证表
typedef struct
{
	XCHAR tszVSerial[256];                                               //验证序列号
	XCHAR tszVDate[64];                                                  //验证时间
	__int64x nID;                                                        //ID
	int nVTime;                                                          //试用时间
	int nLTime;                                                          //剩余时间
	ENUM_VERIFICATION_MODULE_SERIAL_TYPE enSerialType;                   //验证方式
}AUTHREG_TEMPVER;
//禁止列表
typedef struct 
{
	__int64x nID;                                                         //ID
	XCHAR tszUserName[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];          //用户名
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