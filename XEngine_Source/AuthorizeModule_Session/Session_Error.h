#pragma once
/********************************************************************
//    Created:     2022/05/26  10:53:29
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session\Session_Error.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Session
//    File Base:   Session_Error
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
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_INIT_PARAMENT 0x30D0000       //参数错误，无法继续
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_INIT_CREATETHREAD 0x30D0001   //创建线程失败
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_GETCLIENT_PARAMENT 0x30D0010  //参数错误
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_GETCLIENT_NOTFOUND 0x30D0011  //没有找到客户端
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_GETTIME_PARAMENT 0x30D0020    //参数错误，获取失败
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_GETTIME_NOTFOUND 0x30D0021    //没有找到制定的客户端
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_INSERT_PARAMENT 0x30D0050      //参数错误
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_INSERT_ISLOGIN 0x30D0051       //用户已经登陆，无法第二次登陆
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_GETPASS_PARAMENT 0x30D0070    //参数错误
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_GETPASS_MATCH 0x30D0071       //找回密码失败，匹配资料错误
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_GETADDR_PARAMENT 0x30D0080    //获取失败，参数错误
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_GETADDR_NOTFOUND 0x30D0081    //没有找到
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_GETUSER_PARAMENT 0x30D0090    //获取失败,参数错误
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_GETUSER_NOTFOUND 0x30D0091    //没有找到
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_SETUSER_PARAMENT 0x30D00A0    //参数错误
#define ERROR_AUTHORIZE_COMPONENTS_SESSION_SETUSER_NOTFOUND 0x30D00A1    //没有找到