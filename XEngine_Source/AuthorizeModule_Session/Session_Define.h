#pragma once
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
//////////////////////////////////////////////////////////////////////////
//                         导出的回调函数
//////////////////////////////////////////////////////////////////////////
//用户在线时间事件回调处理函数，用户名 在线时间 剩余时间（分,天） 注册的卡类型 自定义参数
typedef void(CALLBACK* CALLBACK_XENGIEN_AUTHREG_SERVICE_EVENTS)(LPCSTR lpszUserAddr, LPCSTR lpszUserName, __int64x nOnlineTimer, __int64x nLeftTimer, LPCSTR lpszLeftDate, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthRegSerialType, ENUM_PROTOCOLDEVICE_TYPE enDeviceType, LPVOID lParam);
//////////////////////////////////////////////////////////////////////////
//                         导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Session_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         网络服务导出函数                             */
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
extern "C" BOOL Session_Authorize_Init(CALLBACK_XENGIEN_AUTHREG_SERVICE_EVENTS fpCall_AuthEvent,LPVOID lParam = NULL);
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
备注：参数一必须通过基础库的内存释放函数BaseLib_OperatorMemory_Free进行释放内存
*********************************************************************/
extern "C" BOOL Session_Authorize_GetClient(AUTHREG_USERTABLE * **pppSt_ListClient, int* pInt_ListCount, LPCSTR lpszClientAddr = NULL);
/********************************************************************
函数名称：Session_Authorize_GetTimer
函数功能：获取客户端时间信息
 参数.一：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要查找用户名
 参数.二：pSt_AuthTime
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：用户时间信息结构体
返回值
  类型：逻辑型
  意思：是否获取成功
备注：通过卡类型来判断导出的时间是分钟还是天
*********************************************************************/
extern "C" BOOL Session_Authorize_GetTimer(LPCSTR lpszUserName,AUTHREG_PROTOCOL_TIME *pSt_AuthTime);
/********************************************************************
函数名称：Session_Authorize_GetAddrForUser
函数功能：通过用户名获取对应地址
 参数.一：lpszClientUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.二：ptszClientAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出获取到的用户套接字地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_Authorize_GetAddrForUser(LPCSTR lpszClientUser,CHAR *ptszClientAddr);
/********************************************************************
函数名称：Session_Authorize_GetUserForAddr
函数功能：通过IP地址获取对应用户名
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入IP地址端口
 参数.二：ptszClientUser
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出获取到的用户名
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_Authorize_GetUserForAddr(LPCSTR lpszClientAddr, CHAR *ptszClientUser);
/********************************************************************
函数名称：Session_Authorize_CloseClient
函数功能：移除一个客户端
 参数.一：lpszClientUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要移除的用户名
返回值
  类型：逻辑型
  意思：是否移除成功
备注：此函数会自动调用AuthRegService_Sql_UserLeave来处理离开时间
*********************************************************************/
extern "C" BOOL Session_Authorize_CloseClient(LPCSTR lpszClientUser);
/********************************************************************
函数名称：Session_Authorize_Destroy
函数功能：销毁网络服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
extern "C" BOOL Session_Authorize_Destroy();
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
返回值
  类型：逻辑型
  意思：是否允许登陆
备注：如果成功，服务器会自动进行计时
*********************************************************************/
extern "C" BOOL Session_Authorize_Insert(LPCSTR lpszClientAddr, AUTHREG_USERTABLE * pSt_UserTable);
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
extern "C" BOOL Session_Authorize_SetUser(AUTHREG_USERTABLE* pSt_UserTable);
