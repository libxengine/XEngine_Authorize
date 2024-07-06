﻿#pragma once
/********************************************************************
//    Created:     2022/05/26  11:00:43
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database\Database_Define.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database
//    File Base:   Database_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出定义和函数
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                         导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG DBModule_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         数据库服务导出函数                           */
/************************************************************************/
/********************************************************************
函数名称：DBModule_SQLite_Init
函数功能：初始化数据库服务
 参数.一：lpszSQLFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：数据库文件路径
 参数.二：bIsChange
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否允许更改用户充值类型
返回值
  类型：逻辑型
  意思：是否成功
备注：先初始化数据库服务，在初始化网络服务，才可以使用本验证服务器！
*********************************************************************/
extern "C" bool DBModule_SQLite_Init(LPCXSTR lpszSQLFile, bool bIsChange = true);
/********************************************************************
函数名称：DBModule_SQLite_Destroy
函数功能：销毁数据库服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_Destroy();
/********************************************************************
函数名称：DBModule_SQLite_UserDelete
函数功能：删除一个用户从数据库中
 参数.一：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要删除的用户
返回值
  类型：逻辑型
  意思：是否删除成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_UserDelete(LPCXSTR lpszUserName);
/********************************************************************
函数名称：DBModule_SQLite_UserRegister
函数功能：用户注册处理数据库语句函数
 参数.一：pSt_UserInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要插入的用户数据
返回值
  类型：逻辑型
  意思：是否插入成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_UserRegister(AUTHREG_USERTABLE * pSt_UserInfo);
/********************************************************************
函数名称：DBModule_SQLite_UserQuery
函数功能：查询用户相对应的值
 参数.一：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要查询的指定用户
 参数.二：pSt_UserInfo
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：如果为空NULL，那么将只判断此用户是否存在
返回值
  类型：逻辑型
  意思：是否查询成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_UserQuery(LPCXSTR lpszUserName,AUTHREG_USERTABLE *pSt_UserInfo);
/********************************************************************
函数名称：DBModule_SQLite_UserPay
函数功能：用户充值函数
 参数.一：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要充值的用户名
 参数.二：lpszSerialName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：充值使用的序列号
返回值
  类型：逻辑型
  意思：是否成功充值
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_UserPay(LPCXSTR lpszUserName,LPCXSTR lpszSerialName);
/********************************************************************
函数名称：DBModule_SQLite_UserLeave
函数功能：用户离开处理事件
 参数.一：pSt_TimeProtocol
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：离开的用户信息
返回值
  类型：逻辑型
  意思：是否处理成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_UserLeave(AUTHREG_PROTOCOL_TIME * pSt_TimeProtocol);
/********************************************************************
函数名称：DBModule_SQLite_UserSet
函数功能：设置用户信息
 参数.一：pSt_UserTable
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要设置的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_UserSet(AUTHREG_USERTABLE* pSt_UserTable);
/********************************************************************
函数名称：DBModule_SQLite_UserList
函数功能：获取用户列表
 参数.一：pppSt_UserInfo
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出用户列表,内存由用户释放
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型
  可空：N
  意思：输出列表个数
 参数.三：nPosStart
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入起始位置
 参数.四：nPosEnd
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入结束位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_UserList(AUTHREG_USERTABLE*** pppSt_UserInfo, int* pInt_ListCount, int nPosStart, int nPosEnd);
/********************************************************************
函数名称：DBModule_SQLite_SerialInsert
函数功能：插入一个序列号到数据库
 参数.一：lpszSerialNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要插入的序列号
返回值
  类型：逻辑型
  意思：是否插入成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_SerialInsert(LPCXSTR lpszSerialNumber);
/********************************************************************
函数名称：DBModule_SQLite_SerialDelete
函数功能：从数据库删除指定序列号
 参数.一：lpszSerialNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要删除的序列号
返回值
  类型：逻辑型
  意思：是否删除成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_SerialDelete(LPCXSTR lpszSerialNumber);
  /********************************************************************
函数名称：DBModule_SQLite_SerialQuery
函数功能：查询一个指定的序列号信息
 参数.一：lpszSerialNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要查询的序列号
 参数.二：pSt_SerialTable
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：导出查询到的信息，如果为NULL，此参数将不起作用
返回值
  类型：逻辑型
  意思：是否查询成功，如果第二个参数为NULL，那么将只返回是否有这个序列号
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_SerialQuery(LPCXSTR lpszSerialNumber,LPAUTHREG_SERIALTABLE pSt_SerialTable);
/********************************************************************
函数名称：DBModule_SQLite_SerialQueryAll
函数功能：查询序列卡表中的所有序列号
 参数.一：pppSt_SerialTable
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出序列卡列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出多少张卡
 参数.三：nPosStart
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入起始位置
 参数.四：nPosEnd
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入结束位置
返回值
  类型：逻辑型
  意思：是否查询成功
备注：参数一需要调用基础库的释放内存函数进行内存释放
*********************************************************************/
extern "C" bool DBModule_SQLite_SerialQueryAll(AUTHREG_SERIALTABLE * **pppSt_SerialTable, int* pInt_ListCount, int nPosStart, int nPosEnd);
/********************************************************************
函数名称：DBModule_SQLite_SerialPush
函数功能：插入一条指定的序列号信息到服务器
 参数.一：pSt_SerialTable
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要插入的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_SerialPush(AUTHREG_SERIALTABLE* pSt_SerialTable);
/********************************************************************
函数名称：DBModule_SQLite_TryInsert
函数功能：网络使用模式插入一条数据
 参数.一：pSt_AuthVer
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要插入的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_TryInsert(AUTHREG_TEMPVER* pSt_AuthVer);
/********************************************************************
函数名称：DBModule_SQLite_TryQuery
函数功能：试用序列号查询函数
 参数.一：pSt_AuthVer
  In/Out：In/Out
  类型：数据结构指针
  可空：N
  意思：输入序列号,输出获取到的内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_TryQuery(AUTHREG_TEMPVER* pSt_AuthVer);
/********************************************************************
函数名称：DBModule_SQLite_TryDelete
函数功能：删除一条指定的试用数据
 参数.一：lpszSerial
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要删除的序列号
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_TryDelete(LPCXSTR lpszSerial);
/********************************************************************
函数名称：DBModule_SQLite_TryClear
函数功能：清理函数,自动清理过期数据
 参数.一：nThanValue
  In/Out：In
  类型：整数型
  可空：N
  意思：清理用于判断需要大于此的值
 参数.二：enVerMode
  In/Out：In
  类型：枚举型
  可空：Y
  意思：判断注册类型,默认不判断全部执行
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_TryClear(int nThanValue, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode = ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW);
/********************************************************************
函数名称：DBModule_SQLite_TrySet
函数功能：设置用户信息函数
 参数.一：pSt_AuthVer
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要更新的用户信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_TrySet(AUTHREG_TEMPVER* pSt_AuthVer);
/********************************************************************
函数名称：DBModule_SQLite_TryList
函数功能：请求试用期列表
 参数.一：pppSt_AuthVer
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出获取到的列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
 参数.三：nPosStart
  In/Out：In
  类型：整数型
  可空：N
  意思：输入查询起始编号
 参数.四：nPosEnd
  In/Out：In
  类型：整数型
  可空：N
  意思：输入查询结束编号
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_TryList(AUTHREG_TEMPVER*** pppSt_AuthVer, int* pInt_ListCount, int nPosStart = 0, int nPosEnd = 1000);
/********************************************************************
函数名称：DBModule_SQLite_BannedInsert
函数功能：黑名单列表插入
 参数.一：pSt_Banned
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要操作的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_BannedInsert(AUTHREG_BANNED* pSt_Banned);
/********************************************************************
函数名称：DBModule_SQLite_BannedDelete
函数功能：黑名单列表删除
 参数.一：pSt_Banned
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要操作的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_BannedDelete(AUTHREG_BANNED* pSt_Banned);
/********************************************************************
函数名称：DBModule_SQLite_BannedList
函数功能：黑名单列表查询
 参数.一：pppSt_BannedUser
  In/Out：Out
  类型：三级指针
  可空：N
  意思：禁用的用户名列表
 参数.二：pInt_UserCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出用户禁用列表个数
 参数.三：pppSt_BannedAddr
  In/Out：Out
  类型：三级指针
  可空：N
  意思：禁用的IP地址列表
 参数.四：pInt_AddrCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出地址禁用列表个数
 参数.五：nPosStart
  In/Out：In
  类型：整数型
  可空：N
  意思：输入查找起始位置
 参数.六：nPosEnd
  In/Out：In
  类型：整数型
  可空：N
  意思：输入查找结束位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_BannedList(AUTHREG_BANNED*** pppSt_BannedUser, int* pInt_UserCount, AUTHREG_BANNED*** pppSt_BannedAddr, int* pInt_AddrCount, int nPosStart, int nPosEnd);
/********************************************************************
函数名称：DBModule_SQLite_BannedExist
函数功能：名单是否存在黑名单列表
 参数.一：pSt_Banned
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要操作的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_BannedExist(AUTHREG_BANNED* pSt_Banned);
/********************************************************************
函数名称：DBModule_SQLite_BannedUPDate
函数功能：更新名单列表信息
 参数.一：pSt_Banned
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要操作的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_BannedUPDate(AUTHREG_BANNED* pSt_Banned);
/********************************************************************
函数名称：DBModule_SQLite_AnnouncementInsert
函数功能：公告插入
 参数.一：pSt_Announcement
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要插入的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_AnnouncementInsert(AUTHREG_ANNOUNCEMENT* pSt_Announcement);
/********************************************************************
函数名称：DBModule_SQLite_AnnouncementDelete
函数功能：公告删除
 参数.一：pSt_Announcement
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要删除的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_AnnouncementDelete(AUTHREG_ANNOUNCEMENT* pSt_Announcement);
/********************************************************************
函数名称：DBModule_SQLite_AnnouncementList
函数功能：列举所有公告
 参数.一：ppppSt_Announcement
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出列举数据
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出数据个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_SQLite_AnnouncementList(AUTHREG_ANNOUNCEMENT*** ppppSt_Announcement, int* pInt_ListCount);

/************************************************************************/
/*                      MYSQL数据库服务导出函数                         */
/************************************************************************/
/********************************************************************
函数名称：DBModule_MySQL_Init
函数功能：初始化数据库管理器
 参数.一：pSt_DBConnector
  In/Out：In
  类型：数据结构指针
 参数.二：bIsChange
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否允许更改用户充值类型
返回值
  类型：逻辑型
  意思：是否成功
备注：先初始化数据库服务，在初始化网络服务，才可以使用本验证服务器！
*********************************************************************/
extern "C" bool DBModule_MySQL_Init(DATABASE_MYSQL_CONNECTINFO* pSt_DBConnector, bool bIsChange = true);
/********************************************************************
函数名称：DBModule_MySQL_Destroy
函数功能：销毁数据库服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_Destroy();
/********************************************************************
函数名称：DBModule_MySQL_UserDelete
函数功能：删除一个用户从数据库中
 参数.一：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要删除的用户
返回值
  类型：逻辑型
  意思：是否删除成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_UserDelete(LPCXSTR lpszUserName);
/********************************************************************
函数名称：DBModule_MySQL_UserRegister
函数功能：用户注册处理数据库语句函数
 参数.一：pSt_UserInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要插入的用户数据
返回值
  类型：逻辑型
  意思：是否插入成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_UserRegister(AUTHREG_USERTABLE* pSt_UserInfo);
/********************************************************************
函数名称：DBModule_MySQL_UserQuery
函数功能：查询用户相对应的值
 参数.一：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要查询的指定用户
 参数.二：pSt_UserInfo
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：如果为空NULL，那么将只判断此用户是否存在
返回值
  类型：逻辑型
  意思：是否查询成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_UserQuery(LPCXSTR lpszUserName, AUTHREG_USERTABLE* pSt_UserInfo);
/********************************************************************
函数名称：DBModule_MySQL_UserPay
函数功能：用户充值函数
 参数.一：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要充值的用户名
 参数.二：lpszSerialName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：充值使用的序列号
返回值
  类型：逻辑型
  意思：是否成功充值
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_UserPay(LPCXSTR lpszUserName, LPCXSTR lpszSerialName);
/********************************************************************
函数名称：DBModule_MySQL_UserLeave
函数功能：用户离开处理事件
 参数.一：pSt_TimeProtocol
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：离开的用户信息
返回值
  类型：逻辑型
  意思：是否处理成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_UserLeave(AUTHREG_PROTOCOL_TIME* pSt_TimeProtocol);
/********************************************************************
函数名称：DBModule_MySQL_UserSet
函数功能：设置用户信息
 参数.一：pSt_UserTable
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要设置的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_UserSet(AUTHREG_USERTABLE* pSt_UserTable);
/********************************************************************
函数名称：DBModule_MySQL_UserList
函数功能：获取用户列表
 参数.一：pppSt_UserInfo
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出用户列表,内存由用户释放
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型
  可空：N
  意思：输出列表个数
 参数.三：nPosStart
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入起始位置
 参数.四：nPosEnd
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入结束位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_UserList(AUTHREG_USERTABLE*** pppSt_UserInfo, int* pInt_ListCount, int nPosStart, int nPosEnd);
/********************************************************************
函数名称：DBModule_MySQL_SerialInsert
函数功能：插入一个序列号到数据库
 参数.一：lpszSerialNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要插入的序列号
返回值
  类型：逻辑型
  意思：是否插入成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_SerialInsert(LPCXSTR lpszSerialNumber);
/********************************************************************
函数名称：DBModule_MySQL_SerialDelete
函数功能：从数据库删除指定序列号
 参数.一：lpszSerialNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要删除的序列号
返回值
  类型：逻辑型
  意思：是否删除成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_SerialDelete(LPCXSTR lpszSerialNumber);
/********************************************************************
函数名称：DBModule_MySQL_SerialQuery
函数功能：查询一个指定的序列号信息
 参数.一：lpszSerialNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要查询的序列号
 参数.二：pSt_SerialTable
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：导出查询到的信息，如果为NULL，此参数将不起作用
返回值
  类型：逻辑型
  意思：是否查询成功，如果第二个参数为NULL，那么将只返回是否有这个序列号
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_SerialQuery(LPCXSTR lpszSerialNumber, LPAUTHREG_SERIALTABLE pSt_SerialTable);
/********************************************************************
函数名称：DBModule_MySQL_SerialQueryAll
函数功能：查询序列卡表中的所有序列号
 参数.一：pppSt_SerialTable
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出序列卡列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出多少张卡
 参数.三：nPosStart
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入起始位置
 参数.四：nPosEnd
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入结束位置
返回值
  类型：逻辑型
  意思：是否查询成功
备注：参数一需要调用基础库的释放内存函数进行内存释放
*********************************************************************/
extern "C" bool DBModule_MySQL_SerialQueryAll(AUTHREG_SERIALTABLE*** pppSt_SerialTable, int* pInt_ListCount, int nPosStart, int nPosEnd);
/********************************************************************
函数名称：DBModule_MySQL_SerialPush
函数功能：插入一条指定的序列号信息到服务器
 参数.一：pSt_SerialTable
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要插入的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_SerialPush(AUTHREG_SERIALTABLE* pSt_SerialTable);
/********************************************************************
函数名称：DBModule_MySQL_TryInsert
函数功能：网络使用模式插入一条数据
 参数.一：pSt_AuthVer
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要插入的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_TryInsert(AUTHREG_TEMPVER* pSt_AuthVer);
/********************************************************************
函数名称：DBModule_MySQL_TryQuery
函数功能：试用序列号查询函数
 参数.一：pSt_AuthVer
  In/Out：In/Out
  类型：数据结构指针
  可空：N
  意思：输入序列号,输出获取到的内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_TryQuery(AUTHREG_TEMPVER* pSt_AuthVer);
/********************************************************************
函数名称：DBModule_MySQL_TryDelete
函数功能：删除一条指定的试用数据
 参数.一：lpszSerial
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要删除的序列号
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_TryDelete(LPCXSTR lpszSerial);
/********************************************************************
函数名称：DBModule_MySQL_TryClear
函数功能：清理函数,自动清理过期数据
 参数.一：nThanValue
  In/Out：In
  类型：整数型
  可空：N
  意思：清理用于判断需要大于此的值
 参数.二：enVerMode
  In/Out：In
  类型：枚举型
  可空：Y
  意思：判断注册类型,默认不判断全部执行
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_TryClear(int nThanValue, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode = ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW);
/********************************************************************
函数名称：DBModule_MySQL_TrySet
函数功能：设置用户信息函数
 参数.一：pSt_AuthVer
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要更新的用户信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_TrySet(AUTHREG_TEMPVER* pSt_AuthVer);
/********************************************************************
函数名称：DBModule_MySQL_TryList
函数功能：请求试用期列表
 参数.一：pppSt_AuthVer
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出获取到的列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
 参数.三：nPosStart
  In/Out：In
  类型：整数型
  可空：N
  意思：输入查询起始编号
 参数.四：nPosEnd
  In/Out：In
  类型：整数型
  可空：N
  意思：输入查询结束编号
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_TryList(AUTHREG_TEMPVER*** pppSt_AuthVer, int* pInt_ListCount, int nPosStart = 0, int nPosEnd = 1000);
/********************************************************************
函数名称：DBModule_MySQL_BannedInsert
函数功能：黑名单列表插入
 参数.一：pSt_Banned
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要操作的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_BannedInsert(AUTHREG_BANNED* pSt_Banned);
/********************************************************************
函数名称：DBModule_MySQL_BannedDelete
函数功能：黑名单列表删除
 参数.一：pSt_Banned
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要操作的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_BannedDelete(AUTHREG_BANNED* pSt_Banned);
/********************************************************************
函数名称：DBModule_MySQL_BannedList
函数功能：黑名单列表查询
 参数.一：pppSt_BannedUser
  In/Out：Out
  类型：三级指针
  可空：N
  意思：禁用的用户名列表
 参数.二：pInt_UserCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出用户禁用列表个数
 参数.三：pppSt_BannedAddr
  In/Out：Out
  类型：三级指针
  可空：N
  意思：禁用的IP地址列表
 参数.四：pInt_AddrCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出地址禁用列表个数
 参数.五：nPosStart
  In/Out：In
  类型：整数型
  可空：N
  意思：输入查找起始位置
 参数.六：nPosEnd
  In/Out：In
  类型：整数型
  可空：N
  意思：输入查找结束位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_BannedList(AUTHREG_BANNED*** pppSt_BannedUser, int* pInt_UserCount, AUTHREG_BANNED*** pppSt_BannedAddr, int* pInt_AddrCount, int nPosStart, int nPosEnd);
/********************************************************************
函数名称：DBModule_MySQL_BannedExist
函数功能：名单是否存在黑名单列表
 参数.一：pSt_Banned
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要操作的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_BannedExist(AUTHREG_BANNED* pSt_Banned);
/********************************************************************
函数名称：DBModule_MySQL_BannedUPDate
函数功能：更新名单列表信息
 参数.一：pSt_Banned
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要操作的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_BannedUPDate(AUTHREG_BANNED* pSt_Banned);
/********************************************************************
函数名称：DBModule_MySQL_AnnouncementInsert
函数功能：公告插入
 参数.一：pSt_Announcement
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要插入的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_AnnouncementInsert(AUTHREG_ANNOUNCEMENT* pSt_Announcement);
/********************************************************************
函数名称：DBModule_MySQL_AnnouncementDelete
函数功能：公告删除
 参数.一：pSt_Announcement
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要删除的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_AnnouncementDelete(AUTHREG_ANNOUNCEMENT* pSt_Announcement);
/********************************************************************
函数名称：DBModule_MySQL_AnnouncementList
函数功能：列举所有公告
 参数.一：ppppSt_Announcement
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出列举数据
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出数据个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool DBModule_MySQL_AnnouncementList(AUTHREG_ANNOUNCEMENT*** ppppSt_Announcement, int* pInt_ListCount);
