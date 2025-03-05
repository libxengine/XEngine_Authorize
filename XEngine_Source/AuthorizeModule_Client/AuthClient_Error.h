#pragma once
/********************************************************************
//    Created:     2023/11/15  11:32:27
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client\AuthClient_Error.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client
//    File Base:   AuthClient_Error
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     错误表
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                         导出的错误表
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*                         网络协议错误                                 */
/************************************************************************/
#define ERROR_AUTHORIZE_MODULE_CLIENT_PARAMENT 0x0060000                 //参数错误，无法继续
#define ERROR_AUTHORIZE_MODULE_CLIENT_SEND 0x0060001                     //发送数据失败
#define ERROR_AUTHORIZE_MODULE_CLIENT_RECV 0x0060002                     //接受数据失败
#define ERROR_AUTHORIZE_MODULE_CLIENT_LOGIN 0x0060003                    //登录失败，请查看错误码
#define ERROR_AUTHORIZE_MODULE_CLIENT_THREAD 0x0060004                   //创建线程失败
#define ERROR_AUTHORIZE_MODULE_CLIENT_NOTAUTH 0x0060005                  //没有验证无法启用心跳
/************************************************************************/
/*                          HTTP验证错误                                */
/************************************************************************/
#define ERROR_AUTHORIZE_MODULE_HTTPVER_PARAMENT 0x0060010
#define ERROR_AUTHORIZE_MODULE_HTTPVER_PARSE 0x0060011
#define ERROR_AUTHORIZE_MODULE_HTTPVER_VERFAILED 0x0060012