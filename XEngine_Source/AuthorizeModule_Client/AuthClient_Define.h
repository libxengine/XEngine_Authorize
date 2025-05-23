﻿#pragma once
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
  意思：是否登录,如果没有登录将返回假,登录成功才需要判断是否通过验证
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
 参数.五：dwCryption
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入密码加密类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool AuthClient_Connector_Login(LPCXSTR lpszUser, LPCXSTR lpszPass, XSHOT nDYCode = 0, XNETHANDLE xhToken = 0, XLONG dwCryption = 0);
/********************************************************************
函数名称：AuthClient_Connector_Heart
函数功能：启用禁用客户端心跳
 参数.一：bEnable
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是启用还是禁用
返回值
  类型：逻辑型
  意思：是否成功
备注：服务器开启了心跳,那么客户端也需要开启
*********************************************************************/
extern "C" bool AuthClient_Connector_Heart(bool bEnable = true);
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
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入密码,如果服务端设置了密码客户端也必须使用加密通信
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool AuthClient_HTTPVer_TryRequest(LPCXSTR lpszURLAddr);
/********************************************************************
函数名称：AuthClient_HTTPVer_GetDCode
函数功能：获取动态码
 参数.一：lpszURLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器地址,完成的API地址,比如:http://127.0.0.1:5302/api?function=dcode&user=get
 参数.二：pInt_DYCode
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出动态码
 参数.三：pxhToken
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出绑定的句柄
 参数.四：pInt_Timeout
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出动态码超时时间
 参数.五：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入密码,如果服务端设置了密码客户端也必须使用加密通信
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool AuthClient_HTTPVer_GetDCode(LPCXSTR lpszURLAddr, int* pInt_DYCode, XNETHANDLE* pxhToken, int* pInt_Timeout = NULL);