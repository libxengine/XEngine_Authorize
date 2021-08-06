#pragma once
/********************************************************************
//    Created:     2021/07/14  13:14:57
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_SQLPacket\SQLPacket_Error.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_SQLPacket
//    File Base:   SQLPacket_Error
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
/*                         数据库错误                                     */
/************************************************************************/
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_INIT_CREATETABLE 0x30D1001   //创建表失败
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_REGISERT_EXIST 0x30D1010     //用户已经存在
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_REGISERT_INSERT 0x30D1011    //插入内容失败
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_QUERY_NOTUSER 0x30D1020      //用户不存在
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_QUERY_GETTABLE 0x30D1021     //获取表失败
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_QUERY_SERIAL 0x30DAA22       //不存在的序列号
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_DELETE_EXEC 0x30D1030        //删除执行失败
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_SERIALINSERT_ISFAILED 0x30D1040 //序列号插入失败
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_SERIALINSERT_EXIST 0x30D1041 //序列卡已经存在
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_SERIALDEL_ISFAILED 0x30D1042 //序列号删除失败
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAY_PARAMENT 0x30D1050       //充值失败，参数错误
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAY_NOTSUPPORT 0x30D1051     //不支持的充值卡
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAY_UPDATAUSEDNAME 0x30D1052 //更新序列卡使用者失败
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAY_ISUSED 0x30D1053         //充值卡已经被使用
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAYTIME_NOTMATCH 0x30D1060   //充值方式不匹配
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAYTIME_UPDATATYPE 0x30D1061 //更新用户充值卡类型失败
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAYTIME_GETTIME 0x30D1062    //获取时间结构失败
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAYTIME_UPDATA 0x30D1063     //更新数据库失败
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_LEAVE_UPDATA 0x30D10A0       //离开处理失败，更新数据库失败
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_LEAVE_UNKNOWTYPE 0x30D10A1   //无法识别的卡的类型，无法正确处理离开消息
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_QUERYALL_GETTABLE 0x30D10B0  //获取表失败，内部数据库错误
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_QUERYALL_NONE 0x30D10B1      //获取失败，没有任何记录
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_TRYINSERT_PARAMENT 0x30D10C0 //试用插入失败,参数错误
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_TRYINSERT_EXIST 0x30D10C1    //此序列号存在,无法继续
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_TRYQUERY_PARAMENT 0x30D10D0  //查询试用序列号失败,参数错误
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_TRYQUERY_NONE 0x30D10D1      //查询的内容为空
#define ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_TRYDELETE_PARAMENT 0x30D10E0 //删除失败,参数错误
