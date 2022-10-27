﻿#pragma once
/********************************************************************
//    Created:     2021/07/16  09:18:44
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Define.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol
//    File Base:   Protocol_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出的定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                          导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD  Protocol_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                          打包类协议                                  */
/************************************************************************/
/********************************************************************
函数名称：Protocol_Packet_HDRComm
函数功能：通用头打包函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的数据
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出数据大小
 参数.三：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的数据
 参数.四：enDeviceType
  In/Out：In
  类型：整数型
  可空：N
  意思：输入设备类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Packet_HDRComm(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, int enDeviceType);
/********************************************************************
函数名称：Protocol_Packet_HttpComm
函数功能：HTTP通用消息打包
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出包装好的缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出包装大小
 参数.三：nCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入返回的code
 参数.四：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入返回的消息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Packet_HttpComm(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode = 0, LPCTSTR lpszMsgBuffer = NULL);
/********************************************************************
函数名称：Protocol_Packet_HttpUserPass
函数功能：HTTP客户端验证数据打包函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出包装好的缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出包装大小
 参数.三：pSt_UserAuth
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Packet_HttpUserPass(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth);
/********************************************************************
函数名称：Protocol_Packet_HttpUserTime
函数功能：用户时间打包函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的数据
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出数据大小
 参数.三：pSt_ProtocolTime
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的附加数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Packet_HttpUserTime(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_PROTOCOL_TIME* pSt_ProtocolTime);
/********************************************************************
函数名称：Protocol_Packet_HttpClientInfo
函数功能：HTTP客户端包装函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出包装好的缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出包装大小
 参数.三：pSt_UserTable
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Packet_HttpClientInfo(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_USERTABLE* pSt_UserTable);
/********************************************************************
函数名称：Protocol_Packet_HttpClientList
函数功能：HTTP客户端列表包装函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出包装好的缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出包装大小
 参数.三：pppSt_OnClient
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要处理的在线列表
 参数.四：nOnCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入在线列表个数
 参数.五：pppSt_OffClient
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要处理的离线列表
 参数.六：nOffCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入离线列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Packet_HttpClientList(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHSESSION_NETCLIENT *** pppSt_OnClient, int nOnCount, AUTHREG_USERTABLE*** pppSt_OffClient, int nOffCount);
/********************************************************************
函数名称：Protocol_Packet_HttpSerialList
函数功能：HTTP序列号打包函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出包装好的缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出包装大小
 参数.三：pppSt_SerialList
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要处理的序列号列表
 参数.四：nListCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Packet_HttpSerialList(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_SERIALTABLE*** pppSt_SerialList, int nListCount);
/********************************************************************
函数名称：Protocol_Packet_HttpToken
函数功能：TOKEN创建打包函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出包装好的缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出包装大小
 参数.三：xhToken
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入创建的TOKEN
 参数.四：nTimeout
  In/Out：In
  类型：整数型
  可空：N
  意思：输入超时时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Packet_HttpToken(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, int nTimeout);
/************************************************************************/
/*                          解析类协议                                  */
/************************************************************************/
/********************************************************************
函数名称：Protocol_Parse_WSHdr
函数功能：获取WEBSOCKET协议信息头
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_ProtocolHdr
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析好的协议头
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Parse_WSHdr(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr);
/********************************************************************
函数名称：Protocol_Parse_HttpParseToken
函数功能：解析TOKEN
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出解析到的TOKEN值
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Parse_HttpParseToken(LPCTSTR lpszMsgBuffer, int nMsgLen, XNETHANDLE* pxhToken);
/********************************************************************
函数名称：Protocol_Parse_HttpParseAuth
函数功能：用户验证解析协议
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_UserAuth
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Parse_HttpParseAuth(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERAUTH * pSt_UserAuth);
/********************************************************************
函数名称：Protocol_Parse_HttpParseUser
函数功能：解析用户信息
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_UserInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Parse_HttpParseUser(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo);
/********************************************************************
函数名称：Protocol_Parse_HttpParsePay
函数功能：解析用户表信息
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_UserPay
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Parse_HttpParsePay(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_PROTOCOL_USERPAY* pSt_UserPay);
/********************************************************************
函数名称：Protocol_Parse_HttpParseTry
函数功能：解析用户表信息
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_UserPay
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Parse_HttpParseTry(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszSerial);
/********************************************************************
函数名称：Protocol_Parse_HttpParseUser
函数功能：解析用户表信息
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_UserTable
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Parse_HttpParseTable(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_USERTABLE* pSt_UserTable);
/********************************************************************
函数名称：Protocol_Parse_HttpParseSerial
函数功能：解析HTTP序列号信息
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pppSt_SerialTable
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出数据列表
 参数.四：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Parse_HttpParseSerial(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_SERIALTABLE*** pppSt_SerialTable, int* pInt_ListCount);
/********************************************************************
函数名称：Protocol_Parse_HttpParseSerial2
函数功能：解析HTTP序列号
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：penSerialType
  In/Out：Out
  类型：枚举型指针
  可空：N
  意思：导出序列卡类型
 参数.四：pInt_NumberCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出序列卡长度
 参数.五：pInt_SerialCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出序列卡个数
 参数.六：ptszHasTime
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出拥有时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Parse_HttpParseSerial2(LPCTSTR lpszMsgBuffer, int nMsgLen, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE * penSerialType, int* pInt_NumberCount, int* pInt_SerialCount, TCHAR * ptszHasTime);
/********************************************************************
函数名称：Protocol_Parse_HttpParseOnline
函数功能：解析在线列表
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pbOnline
  In/Out：Out
  类型：逻辑型
  可空：N
  意思：导出是否只解析在线列表
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Parse_HttpParseOnline(LPCTSTR lpszMsgBuffer, int nMsgLen, BOOL* pbOnline);
/********************************************************************
函数名称：Protocol_Parse_HttpParseTime
函数功能：解析时间信息结构
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_ProtocolTime
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：导出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Parse_HttpParseTime(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_PROTOCOL_TIME* pSt_ProtocolTime);
/********************************************************************
函数名称：Protocol_Parse_HttpParseSwitch
函数功能：开关选项解析函数
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_FunSwitch
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：导出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Parse_HttpParseSwitch(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_FUNCTIONSWITCH * pSt_FunSwitch);