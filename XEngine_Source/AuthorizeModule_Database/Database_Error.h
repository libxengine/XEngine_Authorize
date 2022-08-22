#pragma once
/********************************************************************
//    Created:     2022/05/26  11:00:50
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database\Database_Error.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database
//    File Base:   Database_Error
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
/*                         数据库错误                                    */
/************************************************************************/
#define ERROR_AUTHORIZE_MODULE_DATABASE_CREATETABLE 0x0030001            //创建表失败
#define ERROR_AUTHORIZE_MODULE_DATABASE_EXIST 0x0030002                  //用户已经存在
#define ERROR_AUTHORIZE_MODULE_DATABASE_INSERT 0x0030003                 //插入内容失败
#define ERROR_AUTHORIZE_MODULE_DATABASE_NOTUSER 0x0030004                //用户不存在
#define ERROR_AUTHORIZE_MODULE_DATABASE_GETTABLE 0x0030005               //获取表失败
#define ERROR_AUTHORIZE_MODULE_DATABASE_SERIAL 0x0030006                 //不存在的序列号
#define ERROR_AUTHORIZE_MODULE_DATABASE_EXEC 0x0030007                   //删除执行失败
#define ERROR_AUTHORIZE_MODULE_DATABASE_ISFAILED 0x0030008               //序列号插入失败
#define ERROR_AUTHORIZE_MODULE_DATABASE_PARAMENT 0x0030009               //充值失败，参数错误
#define ERROR_AUTHORIZE_MODULE_DATABASE_NOTSUPPORT 0x0030010             //不支持的充值卡
#define ERROR_AUTHORIZE_MODULE_DATABASE_UPDATAUSEDNAME 0x0030011         //更新序列卡使用者失败
#define ERROR_AUTHORIZE_MODULE_DATABASE_ISUSED 0x0030012                 //充值卡已经被使用
#define ERROR_AUTHORIZE_MODULE_DATABASE_NOTMATCH 0x0030013               //充值方式不匹配
#define ERROR_AUTHORIZE_MODULE_DATABASE_UPDATATYPE 0x0030014             //更新用户充值卡类型失败
#define ERROR_AUTHORIZE_MODULE_DATABASE_GETTIME 0x0030015                //获取时间结构失败
#define ERROR_AUTHORIZE_MODULE_DATABASE_UPDATA 0x0030016                 //更新数据库失败
#define ERROR_AUTHORIZE_MODULE_DATABASE_UNKNOWTYPE 0x0030017             //无法识别的卡的类型，无法正确处理离开消息
#define ERROR_AUTHORIZE_MODULE_DATABASE_NONE 0x0030018                   //获取失败，没有任何记录