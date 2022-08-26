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
#define ERROR_AUTHORIZE_MODULE_SESSION_PARAMENT 0x0020000                //参数错误，无法继续
#define ERROR_AUTHORIZE_MODULE_SESSION_CREATETHREAD 0x0020001            //创建线程失败
#define ERROR_AUTHORIZE_MODULE_SESSION_NOTFOUND 0x0020002                //没有找到客户端
#define ERROR_AUTHORIZE_MODULE_SESSION_ISLOGIN 0x0020003                 //用户已经登陆，无法第二次登陆
#define ERROR_AUTHORIZE_MODULE_SESSION_MATCH 0x0020004                   //找回密码失败，匹配资料错误
#define ERROR_AUTHORIZE_MODULE_SESSION_VER 0x0020005                     //验证失败
#define ERROR_AUTHORIZE_MODULE_SESSION_EXIST 0x0020006                   //已存在