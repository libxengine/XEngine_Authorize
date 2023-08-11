#pragma once
/********************************************************************
//    Created:     2022/08/12  14:15:37
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help\AuthHelp_Error.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Help
//    File Base:   AuthHelp_Error
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
/*                     剪贴板                                           */
/************************************************************************/
#define ERROR_AUTHORIZE_MODULE_HELP_CLIPBOARD_PARRAMENT 0x0050001         //设置失败,参数错误
#define ERROR_AUTHORIZE_MODULE_HELP_CLIPBOARD_OPEN 0x0050002              //打开剪贴板失败
#define ERROR_AUTHORIZE_MODULE_HELP_CLIPBOARD_OWNER 0x0050003             //设置所有者失败
#define ERROR_AUTHORIZE_MODULE_HELP_CLIPBOARD_MALLOC 0x0050004            //申请内存失败
#define ERROR_AUTHORIZE_MODULE_HELP_CLIPBOARD_GETDATA 0x0050005           //获取内容失败
/************************************************************************/
/*                     窗口                                             */
/************************************************************************/
#define ERROR_AUTHORIZE_MODULE_HELP_WINDOW_GETRECT 0x0050010              //获取失败
#define ERROR_AUTHORIZE_MODULE_HELP_WINDOW_INIT 0x0050011                 //初始化窗口失败
#define ERROR_AUTHORIZE_MODULE_HELP_WINDOW_CREATEWINDOW 0x0050012         //创建窗口失败
#define ERROR_AUTHORIZE_MODULE_HELP_WINDOW_SETWINDOW 0x0050013            //设置窗口失败
/************************************************************************/
/*                     动态码                                           */
/************************************************************************/
#define ERROR_AUTHORIZE_MODULE_HELP_DYNAMIC_THREAD 0x0050020              //线程创建失败
#define ERROR_AUTHORIZE_MODULE_HELP_DYNAMIC_PARAMENT 0x0050021            //参数错误
#define ERROR_AUTHORIZE_MODULE_HELP_DYNAMIC_NOTFOUND 0x0050022            //没有找到
#define ERROR_AUTHORIZE_MODULE_HELP_DYNAMIC_NOTMATCH 0x0050023            //动态码不匹配
/************************************************************************/
/*                     多端登录                                         */
/************************************************************************/
#define ERROR_AUTHORIZE_MODULE_HELP_MULTI_NOTSUPPORT 0x0050030            //不支持