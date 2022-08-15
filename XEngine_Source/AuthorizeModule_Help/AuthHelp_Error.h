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
#define ERROR_AUTH_HELP_CLIPBOARD_SET_PARRAMENT 0x00E0001              //设置失败,参数错误
#define ERROR_AUTH_HELP_CLIPBOARD_SET_OPEN 0x00E0002                   //打开剪贴板失败
#define ERROR_AUTH_HELP_CLIPBOARD_SET_OWNER 0x00E0003                  //设置所有者失败
#define ERROR_AUTH_HELP_CLIPBOARD_SET_MALLOC 0x00E0004                 //申请内存失败
#define ERROR_AUTH_HELP_CLIPBOARD_GET_PARAMENT 0x00E0010               //获取失败,参数错误
#define ERROR_AUTH_HELP_CLIPBOARD_GET_OPEN 0x00E0011                   //打开剪贴板失败
#define ERROR_AUTH_HELP_CLIPBOARD_GET_GETDATA 0x00E0012                //获取内容失败