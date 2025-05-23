﻿#pragma once
/********************************************************************
//    Created:     2022/05/26  10:53:14
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_Define.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session
//    File Base:   Session_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出定义和函数
//    History:
*********************************************************************/
typedef struct
{
	AUTHREG_USERTABLE st_UserTable;                                       //用户表
	XENGINE_LIBTIME st_LibTimer;                                         //登录时间结构
	XCHAR tszClientAddr[64];
	XCHAR tszLeftTime[64];                                                //过期日期
	XNETHANDLE xhToken;
	__int64x nOnlineTime;                                                 //在线时间
	__int64x nLeftTime;                                                   //剩余时间
	int nNetType;                                                         //连接类型
}AUTHSESSION_NETCLIENT, * LPAUTHSESSION_NETCLIENT;
//////////////////////////////////////////////////////////////////////////
//                         导出的回调函数
//////////////////////////////////////////////////////////////////////////
//用户在线时间事件回调处理函数，用户名 在线时间 剩余时间（分,天） 注册的卡类型 自定义参数
typedef void(CALLBACK* CALLBACK_XENGIEN_AUTHORIZE_SESSION_CLIENT_EVENTS)(LPCXSTR lpszUserAddr, LPCXSTR lpszUserName, __int64x nOnlineTimer, __int64x nLeftTimer, LPCXSTR lpszLeftDate, ENUM_AUTHORIZE_MODULE_SERIAL_TYPE enSerialType, ENUM_PROTOCOLDEVICE_TYPE enDeviceType, int nNetType, XPVOID lParam);
typedef void(CALLBACK* CALLBACK_XENGIEN_AUTHORIZE_SESSION_TOKEN_EVENTS)(XNETHANDLE xhToken, XPVOID lParam);
//////////////////////////////////////////////////////////////////////////
//                         导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Session_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         网络验证导出函数                             */
/************************************************************************/
/********************************************************************
函数名称：Session_Authorize_Init
函数功能：初始化会话
 参数.一：fpCall_AuthEvent
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：验证服务器事件时间回调函数，通过此来通知上层用户时间更新
 参数.二：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数自定义参数
返回值
  类型：逻辑型
  意思：是否初始化成功
备注：
*********************************************************************/
extern "C" bool Session_Authorize_Init(CALLBACK_XENGIEN_AUTHORIZE_SESSION_CLIENT_EVENTS fpCall_AuthEvent,XPVOID lParam = NULL);
/********************************************************************
函数名称：Session_Authorize_GetClient
函数功能：获取客户端信息
 参数.一：pppSt_ListClient
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出获取到的客户端信息
 参数.二：pInt_ListCount
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输出列表个数
 参数.三：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要获取的客户端地址，为空表示获取所有
返回值
  类型：逻辑型
  意思：是否获取成功
备注：参数一必须通过基础库的内存释放函数BaseLib_Memory_Free进行释放内存
*********************************************************************/
extern "C" bool Session_Authorize_GetClient(AUTHSESSION_NETCLIENT * **pppSt_ListClient, int* pInt_ListCount, LPCXSTR lpszClientUser = NULL);
/********************************************************************
函数名称：Session_Authorize_GetUserForAddr
函数功能：通过IP地址获取对应用户名
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入IP地址端口
 参数.二：pSt_Client
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：导出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Authorize_GetUserForAddr(LPCXSTR lpszClientAddr, AUTHSESSION_NETCLIENT * pSt_Client = NULL);
/********************************************************************
函数名称：Session_Authorize_CloseAddr
函数功能：移除一个客户端
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要移除的地址
返回值
  类型：逻辑型
  意思：是否移除成功
备注：
*********************************************************************/
extern "C" bool Session_Authorize_CloseAddr(LPCXSTR lpszClientAddr);
/********************************************************************
函数名称：Session_Authorize_Destroy
函数功能：销毁网络服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
extern "C" bool Session_Authorize_Destroy();
/********************************************************************
函数名称：Session_Authorize_Insert
函数功能：用户登陆协议分析
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户的IP地址加端口
 参数.二：pSt_AuthProtocol
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：用户传递的协议数据
 参数.三：xhToken
  In/Out：In
  类型：句柄
  可空：Y
  意思：所属TOKEN
 参数.四：nNetType
  In/Out：In
  类型：整数型
  可空：Y
  意思：用户连接的类型
返回值
  类型：逻辑型
  意思：是否允许登陆
备注：如果成功，服务器会自动进行计时
*********************************************************************/
extern "C" bool Session_Authorize_Insert(LPCXSTR lpszClientAddr, AUTHREG_USERTABLE * pSt_UserTable, XNETHANDLE xhToken, int nNetType = 0);
/********************************************************************
函数名称：Session_Authorize_SetUser
函数功能：设置用户信息
 参数.一：pSt_UserTable
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要设置的用户信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Authorize_SetUser(AUTHREG_USERTABLE* pSt_UserTable);
/************************************************************************/
/*                         TOKEN会话导出函数                            */
/************************************************************************/
/********************************************************************
函数名称：Session_Token_Init
函数功能：初始化会话
 参数.一：nTimeout
  In/Out：In
  类型：整数型
  可空：N
  意思：超时时间,单位秒
 参数.二：fpCall_AuthEvent
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：TOKEN登录的客户端连接超时回调
 参数.三：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数自定义参数
返回值
  类型：逻辑型
  意思：是否初始化成功
备注：
*********************************************************************/
extern "C" bool Session_Token_Init(int nTimeout, CALLBACK_XENGIEN_AUTHORIZE_SESSION_TOKEN_EVENTS fpCall_AuthEvent, XPVOID lParam = NULL);
/********************************************************************
函数名称：Session_Token_Destroy
函数功能：销毁服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
extern "C" bool Session_Token_Destroy();
/********************************************************************
函数名称：Session_Authorize_Insert
函数功能：用户登陆协议分析
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
 参数.二：pSt_UserTable
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：用户信息表
返回值
  类型：逻辑型
  意思：是否允许登陆
备注：
*********************************************************************/
extern "C" bool Session_Token_Insert(XNETHANDLE xhToken, AUTHREG_USERTABLE* pSt_UserTable);
/********************************************************************
函数名称：Session_Token_Delete
函数功能：移除一个客户端
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
返回值
  类型：逻辑型
  意思：是否移除成功
备注：
*********************************************************************/
extern "C" bool Session_Token_Delete(XNETHANDLE xhToken);
/********************************************************************
函数名称：Session_Token_UPDate
函数功能：更新一个客户端的TOKEN时间
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Token_UPDate(XNETHANDLE xhToken);
/********************************************************************
函数名称：Session_Token_Get
函数功能：获取客户端信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的客户端
 参数.二：pSt_UserTable
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：输出TOKEN对应的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Token_Get(XNETHANDLE xhToken, AUTHREG_USERTABLE* pSt_UserTable = NULL);
/********************************************************************
函数名称：Session_Token_GetUser
函数功能：获取用户是否存在
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：密码
 参数.三：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：导出获取到的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Token_GetUser(LPCXSTR lpszUser, LPCXSTR lpszPass, XNETHANDLE * pxhToken);
/********************************************************************
函数名称：Session_Token_RenewalTime
函数功能：续期时间
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的TOKEN
 参数.二：pInt_RenewalTime
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出续期的次数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Token_RenewalTime(XNETHANDLE xhToken, int* pInt_RenewalTime);