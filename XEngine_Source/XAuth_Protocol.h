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
typedef enum
{
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_UNKNOW = 0,                 //无法识别的充值卡
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_SECOND = 1,                 //秒钟,本地使用在read和write的时候更新
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_DAY = 2,                    //天数,本地使用天数卡,不使用不减天数.
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_TIME = 3,                   //次数卡
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_CUSTOM = 4                  //自定义过期日期
}ENUM_AUTHORIZE_MODULE_SERIAL_TYPE, * LPENUM_AUTHORIZE_MODULE_SERIAL_TYPE;
typedef enum
{
	ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_UNKNOW = 0,                    //未注册,Authorize_CDKey_GetLeftTimer将返回失败
	ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_TEMP = 1,                      //临时,Authorize_CDKey_GetLeftTimer一次后过期,需要Write
	ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_TRY = 2,                       //试用
	ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_OFFICIAL = 3,                  //正式版
	ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_UNLIMIT = 4,                   //无限制版,永不过期.CDKEY不做任何验证
	ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_EXPIRED = 5                    //已过期的版本,Authorize_CDKey_GetLeftTimer将返回失败
}ENUM_AUTHORIZE_MODULE_CDKEY_TYPE, * LPENUM_AUTHORIZE_MODULE_CDKEY_TYPE;
typedef enum
{
	ENUM_AUTHORIZE_MODULE_HW_TYPE_UNKNOW = 0,                     //未指定
	ENUM_AUTHORIZE_MODULE_HW_TYPE_CPU = 1,                        //CPU序列号
	ENUM_AUTHORIZE_MODULE_HW_TYPE_DISK = 2,                       //硬盘序列号
	ENUM_AUTHORIZE_MODULE_HW_TYPE_BOARD = 3,                      //主板序列号
	ENUM_AUTHORIZE_MODULE_HW_TYPE_MAC = 4,                        //网卡MAC地址
	ENUM_AUTHORIZE_MODULE_HW_TYPE_BIOS = 5                        //BIOS序列号
}ENUM_AUTHORIZE_MODULE_HW_TYPE, * LPENUM_AUTHORIZE_MODULE_HW_TYPE;
typedef enum
{
	ENUM_AUTHORIZE_MODULE_VERMODE_TYPE_UNKNOW = 0,                 //未知
	ENUM_AUTHORIZE_MODULE_VERMODE_TYPE_LOCAL = 0x01,               //本地
	ENUM_AUTHORIZE_MODULE_VERMODE_TYPE_NETWORK = 0x02,             //网络
}ENUM_AUTHORIZE_MODULE_VERMODE_TYPE, * LPENUM_AUTHORIZE_MODULE_VERMODE_TYPE;
//////////////////////////////////////////////////////////////////////////
//                            导出的结构体
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	XCHAR tszAddr[XPATH_MAX];                                             //服务器或者域名地址
	int nPort;                                                           //端口号码,如果>0表示CDKEY验证失败后改为网络验证
	//版本信息
	struct
	{
		XCHAR tszAppName[128];                                            //应用程序名称
		XCHAR tszAppVer[128];                                             //应用程序版本号
		__int64x nExecTime;                                              //程序已经执行次数,调用Authorize_CDKey_GetLeftTimer会更新
		bool bInit;                                                      //是否初始化,由用户控制
	}st_AuthAppInfo;
	//CDKEY信息
	struct
	{
		XCHAR tszHardware[1024];                                          //硬件码
		XCHAR tszCreateTime[64];                                          //CDKEY创建日期，年/月/日-小时：分钟：秒
		XCHAR tszRegisterTime[64];                                        //注册时间，年/月/日-小时：分钟：秒
		XCHAR tszLeftTime[64];                                            //总的剩余时间,过期日期，根据nLeftType决定此值的意义
		XCHAR tszStartTime[64];                                           //当前启动时间,由系统读取CDKEY的时候自动更新,天数和分钟有效
		XCHAR tszExpiryTime[64];                                          //过期的时间,需要调用Authorize_CDKey_GetLeftTimer并且Write才生效
		__int64x nHasTime;                                               //当前还拥有时间，根据nLeftType决定此值的意义,调用Authorize_CDKey_GetLeftTimer会更新
		ENUM_AUTHORIZE_MODULE_SERIAL_TYPE enSerialType;          //过期类型，参考:ENUM_AUTHORIZE_MODULE_SERIAL_TYPE
		ENUM_AUTHORIZE_MODULE_CDKEY_TYPE enRegType;                //注册类型，参考:ENUM_AUTHORIZE_MODULE_CDKEY_TYPE
		ENUM_AUTHORIZE_MODULE_HW_TYPE enHWType;                  //硬件类型，参考:ENUM_AUTHORIZE_MODULE_HW_TYPE
		ENUM_AUTHORIZE_MODULE_VERMODE_TYPE enVModeType;          //验证方式，参考:ENUM_AUTHORIZE_MODULE_VERMODE_TYPE 
	}st_AuthRegInfo;
	//临时序列号
	struct
	{
		//次数限制
		struct
		{
			XCHAR tszTimeSerial[128];
			int nTimeCount;                                              //使用次数
		}st_TimeLimit;
		//时间限制
		struct
		{
			XCHAR tszDataTime[128];                                       //过期时间
			XCHAR tszDataSerial[128];                                     //序列号
			bool bTimeAdd;                                                //真,过期时间是设置的日期-创建日期.假过期日期就是设定的日期
		}st_DataLimit;
		//无限制
		struct
		{
			XCHAR tszUNLimitSerial[128];                                  //无限制序列号
		}st_UNLimit;
	}st_AuthSerial;
	//注册的用户信息，可以不填
	struct
	{
		XCHAR tszUserName[64];                                            //注册的用户
		XCHAR tszUserContact[64];                                         //联系方式，电子邮件或者手机等
		XCHAR tszCustom[1024];                                            //自定义数据
	}st_AuthUserInfo;
}XENGINE_AUTHORIZE_LOCAL, * LPXENGINE_AUTHORIZE_LOCAL;
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
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE enSerialType;
	ENUM_PROTOCOLDEVICE_TYPE enDeviceType;
}AUTHREG_PROTOCOL_TIME, * LPAUTHREG_PROTOCOL_TIME;
//用户表
typedef struct tag_AuthReg_UserTable
{
	XENGINE_PROTOCOL_USERINFO st_UserInfo;
	XCHAR tszADDInfo[2048];                                                //附加信息
	XCHAR tszLeftTime[64];                                                 //剩余日期
	XCHAR tszHardCode[32];                                                 //硬件码
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE enSerialType;                        //充值卡类型
	ENUM_PROTOCOLDEVICE_TYPE enDeviceType;                                 //设备类型
}AUTHREG_USERTABLE, * LPAUTHREG_USERTABLE;
//注册序列号表
typedef struct tag_AuthReg_SerialTable
{
	XCHAR tszUserName[XENGINE_AUTHREG_SERVICE_SQL_MAX_USERNAME];           //使用者是谁
	XCHAR tszSerialNumber[128];                                            //序列号
	XCHAR tszMaxTime[64];                                                  //使用时间
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE enSerialType;               //充值卡类型
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
	ENUM_AUTHORIZE_MODULE_SERIAL_TYPE enVMode;                   //验证方式
}AUTHREG_TEMPVER;
//禁止列表
typedef struct 
{
	__int64x nID;                                                         //ID
	XCHAR tszUserName[XPATH_MAX];                                           //用户名
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
//扩展登录协议
struct AUTHORIZE_PROTOCOL_USERAUTHEX : public XENGINE_PROTOCOL_USERAUTH {
	XCHAR tszHWCode[64];
};