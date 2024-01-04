#pragma once
/********************************************************************
//    Created:     2023/11/15  11:29:49
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client\AuthClient_Define.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client
//    File Base:   AuthClient_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     为了方便使用，封装了验证客户端，可以通过c/c++ e 等语言使用
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                         导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG AuthClient_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         验证客户端导出函数                           */
/************************************************************************/
#if (1 == _XAUTH_BUILD_SWITCH_CLIENT_NETWORK)
/********************************************************************
函数名称：AuthClient_Connector_Connect
函数功能：链接到服务器
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器地址
 参数.二：nPort
  In/Out：In
  类型：整数型
  可空：N
  意思：服务器端口
 参数.三：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器密码，如果没有填空
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool AuthClient_Connector_Connect(LPCXSTR lpszClientAddr, int nPort, LPCXSTR lpszPass = NULL);
/********************************************************************
函数名称：AuthClient_Connector_Close
函数功能：销毁关闭客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool AuthClient_Connector_Close();
/********************************************************************
函数名称：AuthClient_Connector_GetAuth
函数功能：验证用户是否登录或者超时
 参数.一：pbAuth
  In/Out：Out
  类型：逻辑型指针
  可空：Y
  意思：输出是否验证,如果登录成功但是参数为假.说明没有剩余时间了
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool AuthClient_Connector_GetAuth(bool* pbAuth = NULL);
/********************************************************************
函数名称：AuthClient_Connector_Login
函数功能：登录到服务器
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
 参数.三：nDYCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入动态码
 参数.四：xhToken
  In/Out：In
  类型：句柄型
  可空：Y
  意思：输入动态码绑定的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool AuthClient_Connector_Login(LPCXSTR lpszUser, LPCXSTR lpszPass, int nDYCode = 0, XNETHANDLE xhToken = 0);
#endif
/************************************************************************/
/*                         临时验证函数                                 */
/************************************************************************/
/********************************************************************
函数名称：AuthClient_HTTPVer_TryRequest
函数功能：试用版请求
 参数.一：lpszURLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool AuthClient_HTTPVer_TryRequest(LPCXSTR lpszURLAddr);