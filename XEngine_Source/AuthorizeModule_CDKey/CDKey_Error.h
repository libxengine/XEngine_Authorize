#pragma once
/********************************************************************
//    Created:     2024/07/08  17:30:28
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey\Authorize_Error.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey
//    File Base:   Authorize_Error
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     错误导出表
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                    序列卡授权导出错误表
//////////////////////////////////////////////////////////////////////////
#define ERROR_AUTHORIZE_MODULE_CDKEY_PARAMENT 0x50A0001                   //参数不合适
#define ERROR_AUTHORIZE_MODULE_CDKEY_NOTSUPPORT 0x50A0002                 //不支持此生成类型
#define ERROR_AUTHORIZE_MODULE_CDKEY_BUFFER 0x50A0003                     //这个缓冲区有问题，获取卡类型失败
#define ERROR_AUTHORIZE_MODULE_CDKEY_CUSTOM 0x50A0004                     //获取自定义时间序列号的过期时间失败。
#define ERROR_AUTHORIZE_MODULE_CDKEY_UNKNOW 0x50A0005                     //获取类型失败，这个类型未知
#define ERROR_AUTHORIZE_MODULE_CDKEY_CONNECT 0x50A1001   //连接信息写入失败
#define ERROR_AUTHORIZE_MODULE_CDKEY_APPINFO 0x50A1002   //程序信息写入失败
#define ERROR_AUTHORIZE_MODULE_CDKEY_AUTHREG 0x50A1003   //添加注册信息失败
#define ERROR_AUTHORIZE_MODULE_CDKEY_SERIAL 0x50A1004    //写入序列号失败
#define ERROR_AUTHORIZE_MODULE_CDKEY_USER 0x50A1005      //写入用户信息失败
#define ERROR_AUTHORIZE_MODULE_CDKEY_NOTREG 0x50A1021     //没有注册
#define ERROR_AUTHORIZE_MODULE_CDKEY_NOTHASTIME 0x50A1022 //没有可用时间
#define ERROR_AUTHORIZE_MODULE_CDKEY_TIMEOUT 0x50A1024    //CDKEY已经超时,没有可用时间
#define ERROR_AUTHORIZE_MODULE_CDKEY_TEMP 0x50A1026       //临时版本已不可用
#define ERROR_AUTHORIZE_MODULE_CDKEY_EXPIRED 0x50A1027    //已经过期的CDKEY,无法继续使用
#define ERROR_AUTHORIZE_MODULE_CDKEY_OPENFILE 0x50A1031 //文件不存在
#define ERROR_AUTHORIZE_MODULE_CDKEY_SETFILE 0x50A1032  //设置文件失败
#define ERROR_AUTHORIZE_MODULE_CDKEY_TIMELIST 0x50A1041  //不存在时间列表
#define ERROR_AUTHORIZE_MODULE_CDKEY_MALLOC 0x50A1042    //申请内存失败
#define ERROR_AUTHORIZE_MODULE_CDKEY_READ 0x50A1043      //读取文件失败
#define ERROR_AUTHORIZE_MODULE_CDKEY_FAILED 0x50A1044    //序列号验证失败,输入的序列号错误