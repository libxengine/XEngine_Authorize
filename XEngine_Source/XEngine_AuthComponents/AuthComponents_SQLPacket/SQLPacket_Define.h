#pragma once
/********************************************************************
//    Created:     2021/07/14  13:14:43
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_SQLPacket\SQLPacket_Define.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_SQLPacket
//    File Base:   SQLPacket_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出定义和函数
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                         导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD SQLPacket_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         数据库服务导出函数                           */
/************************************************************************/
/********************************************************************
函数名称：AuthService_SQLPacket_Init
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
extern "C" BOOL AuthService_SQLPacket_Init(LPCSTR lpszSQLFile, BOOL bIsChange = TRUE);
/********************************************************************
函数名称：AuthService_SQLPacket_Destroy
函数功能：销毁数据库服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
extern "C" BOOL AuthService_SQLPacket_Destroy();
/********************************************************************
函数名称：AuthService_SQLPacket_UserDelete
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
extern "C" BOOL AuthService_SQLPacket_UserDelete(LPCSTR lpszUserName);
/********************************************************************
函数名称：AuthService_SQLPacket_UserRegister
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
extern "C" BOOL AuthService_SQLPacket_UserRegister(AUTHREG_USERTABLE * pSt_UserInfo);
/********************************************************************
函数名称：AuthService_SQLPacket_UserQuery
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
extern "C" BOOL AuthService_SQLPacket_UserQuery(LPCSTR lpszUserName,AUTHREG_USERTABLE *pSt_UserInfo);
/********************************************************************
函数名称：AuthService_SQLPacket_UserPay
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
extern "C" BOOL AuthService_SQLPacket_UserPay(LPCSTR lpszUserName,LPCSTR lpszSerialName);
/********************************************************************
函数名称：AuthService_SQLPacket_UserLeave
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
extern "C" BOOL AuthService_SQLPacket_UserLeave(AUTHREG_PROTOCOL_TIME * pSt_TimeProtocol);
/********************************************************************
函数名称：AuthService_SQLPacket_UserSet
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
extern "C" BOOL AuthService_SQLPacket_UserSet(AUTHREG_USERTABLE* pSt_UserTable);
/********************************************************************
函数名称：AuthService_SQLPacket_SerialInsert
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
extern "C" BOOL AuthService_SQLPacket_SerialInsert(LPCSTR lpszSerialNumber);
/********************************************************************
函数名称：AuthService_SQLPacket_SerialDelete
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
extern "C" BOOL AuthService_SQLPacket_SerialDelete(LPCSTR lpszSerialNumber);
  /********************************************************************
函数名称：AuthService_SQLPacket_SerialQuery
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
extern "C" BOOL AuthService_SQLPacket_SerialQuery(LPCSTR lpszSerialNumber,LPAUTHREG_SERIALTABLE pSt_SerialTable);
/********************************************************************
函数名称：AuthService_SQLPacket_SerialQueryAll
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
返回值
  类型：逻辑型
  意思：是否查询成功
备注：参数一需要调用基础库的释放内存函数进行内存释放
*********************************************************************/
extern "C" BOOL AuthService_SQLPacket_SerialQueryAll(AUTHREG_SERIALTABLE * **pppSt_SerialTable, int* pInt_ListCount);
/********************************************************************
函数名称：AuthService_SQLPacket_TryInsert
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
extern "C" BOOL AuthService_SQLPacket_TryInsert(AUTHREG_NETVER* pSt_AuthVer);
/********************************************************************
函数名称：AuthService_SQLPacket_TryQuery
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
extern "C" BOOL AuthService_SQLPacket_TryQuery(AUTHREG_NETVER* pSt_AuthVer);
/********************************************************************
函数名称：AuthService_SQLPacket_TryDelete
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
extern "C" BOOL AuthService_SQLPacket_TryDelete(LPCSTR lpszSerial);
/********************************************************************
函数名称：AuthService_SQLPacket_TryClear
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
extern "C" BOOL AuthService_SQLPacket_TryClear(int nThanValue, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode = ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW);
/********************************************************************
函数名称：AuthService_SQLPacket_TrySet
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
extern "C" BOOL AuthService_SQLPacket_TrySet(AUTHREG_NETVER* pSt_AuthVer);