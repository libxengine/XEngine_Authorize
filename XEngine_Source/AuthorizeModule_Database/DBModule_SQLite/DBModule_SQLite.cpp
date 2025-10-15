#include "pch.h"
#include "DBModule_SQLite.h"
/********************************************************************
//    Created:     2022/05/26  11:01:13
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database\Database_SQLite\Database_SQLite.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database\Database_SQLite
//    File Base:   Database_SQLite
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     数据库管理器
//    History:
*********************************************************************/
CDBModule_SQLite::CDBModule_SQLite()
{
    m_bChange = false;
}
CDBModule_SQLite::~CDBModule_SQLite()
{

}
//////////////////////////////////////////////////////////////////////////
//                     公有函数
//////////////////////////////////////////////////////////////////////////
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
bool CDBModule_SQLite::DBModule_SQLite_Init(LPCXSTR lpszSQLFile, bool bIsChange /* = true */)
{
    SQLPacket_IsErrorOccur = false;
    m_bChange = bIsChange;
    //打开数据库
    if (!DataBase_SQLite_Open(&xhData, lpszSQLFile))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return false;
    }
    return true;
}
/********************************************************************
函数名称：DBModule_SQLite_Destroy
函数功能：销毁数据库服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
bool CDBModule_SQLite::DBModule_SQLite_Destroy()
{
    SQLPacket_IsErrorOccur = false;

    if (!DataBase_SQLite_Close(xhData))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return false;
    }
    return true;
}
/********************************************************************
函数名称：DBModule_SQLite_UserDelete
函数功能：删除一个用户从数据库中
 参数.一：pSt_UserInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入删除用户信息
返回值
  类型：逻辑型
  意思：是否删除成功
备注：
*********************************************************************/
bool CDBModule_SQLite::DBModule_SQLite_UserDelete(XENGINE_PROTOCOL_USERINFO* pSt_UserInfo)
{
    SQLPacket_IsErrorOccur = false;
    XCHAR tszSQLStatement[1024];    //SQL语句
    memset(tszSQLStatement, '\0', 1024);
    _xstprintf(tszSQLStatement, _X("DELETE FROM `Authorize_User` WHERE UserName = '%s'"), pSt_UserInfo->tszUserName);
    //执行
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_EXEC;
        return false;
    }
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_UserRegister(AUTHREG_USERTABLE* pSt_UserInfo)
{
    SQLPacket_IsErrorOccur = false;
    XCHAR tszSQLStatement[1024];    //SQL语句
    memset(tszSQLStatement, '\0', 1024);

    if (DBModule_SQLite_UserQuery(pSt_UserInfo->st_UserInfo.tszUserName))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_EXIST;
        return false;
    }
    _xstprintf(tszSQLStatement, _X("INSERT INTO Authorize_User(UserName, Password, LeftTime, EmailAddr, HardCode, CardSerialType, PhoneNumber, IDCard, nUserLevel, CreateTime) values('%s','%s','%s','%s','%s','%d',%lld,%lld,%d,datetime('now', 'localtime'))"), pSt_UserInfo->st_UserInfo.tszUserName, pSt_UserInfo->st_UserInfo.tszUserPass, pSt_UserInfo->tszLeftTime, pSt_UserInfo->st_UserInfo.tszEMailAddr, pSt_UserInfo->tszHardCode, pSt_UserInfo->enSerialType, pSt_UserInfo->st_UserInfo.nPhoneNumber, pSt_UserInfo->st_UserInfo.nIDNumber, pSt_UserInfo->st_UserInfo.nUserLevel);
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_INSERT;
        return false;
    }
    return true;
}
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
 参数.三：bUser
  In/Out：Out
  类型：逻辑型
  可空：Y
  意思：使用用户名查询还是硬件码
返回值
  类型：逻辑型
  意思：是否查询成功
备注：
*********************************************************************/
bool CDBModule_SQLite::DBModule_SQLite_UserQuery(LPCXSTR lpszUserName, AUTHREG_USERTABLE* pSt_UserInfo /* = NULL */, bool bUser /* = true */)
{
    SQLPacket_IsErrorOccur = false;

    XCHAR tszSQLStatement[1024];    //SQL语句
    char** ppszResult = NULL;
    int nRow = 0;
    int nColumn = 0;
    memset(tszSQLStatement, '\0', 1024);

    if (bUser)
    {
        _xstprintf(tszSQLStatement, _X("SELECT * FROM `Authorize_User` WHERE UserName = '%s'"), lpszUserName);
    }
    else
    {
        _xstprintf(tszSQLStatement, _X("SELECT * FROM `Authorize_User` WHERE HardCode = '%s'"), lpszUserName);
    }
    
    if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_GETTABLE;
        return false;
    }
    if ((0 == nRow) || (0 == nColumn))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_NOTUSER;
        return false;
    }
    //如果是NULL，表示不想知道结果
    if (NULL != pSt_UserInfo)
    {
        memset(pSt_UserInfo, '\0', sizeof(AUTHREG_USERTABLE));
        //ID
        int nFliedValue = nColumn;
        //用户名
        nFliedValue++;
        _tcsxcpy(pSt_UserInfo->st_UserInfo.tszUserName, ppszResult[nFliedValue]);
        //密码
        nFliedValue++;
        _tcsxcpy(pSt_UserInfo->st_UserInfo.tszUserPass, ppszResult[nFliedValue]);
        //过期时间
        nFliedValue++;
        _tcsxcpy(pSt_UserInfo->tszLeftTime, ppszResult[nFliedValue]);
        //电子邮件
        nFliedValue++;
        _tcsxcpy(pSt_UserInfo->st_UserInfo.tszEMailAddr, ppszResult[nFliedValue]);
        //硬件码
        nFliedValue++;
        _tcsxcpy(pSt_UserInfo->tszHardCode, ppszResult[nFliedValue]);
        //充值卡类型
        nFliedValue++;
        pSt_UserInfo->enSerialType = (ENUM_VERIFICATION_MODULE_SERIAL_TYPE)_ttxoi(ppszResult[nFliedValue]);
        //QQ号
        nFliedValue++;
        pSt_UserInfo->st_UserInfo.nPhoneNumber = _ttxoll(ppszResult[nFliedValue]);
        //身份证ID
        nFliedValue++;
        pSt_UserInfo->st_UserInfo.nIDNumber = _ttxoll(ppszResult[nFliedValue]);
        //用户级别 -1表示封禁
        nFliedValue++;
        pSt_UserInfo->st_UserInfo.nUserLevel = _ttxoi(ppszResult[nFliedValue]);
        //登录日期
        nFliedValue++;
        if (NULL != ppszResult[nFliedValue] && _tcsxlen(ppszResult[nFliedValue]) > 0)
        {
            _tcsxcpy(pSt_UserInfo->st_UserInfo.tszLoginTime, ppszResult[nFliedValue]);
        }
		//注册日期
		nFliedValue++;
		_tcsxcpy(pSt_UserInfo->st_UserInfo.tszCreateTime, ppszResult[nFliedValue]);
    }

    DataBase_SQLite_FreeTable(ppszResult);
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_UserPay(LPCXSTR lpszUserName, LPCXSTR lpszSerialName)
{
    SQLPacket_IsErrorOccur = false;

    AUTHREG_SERIALTABLE st_SerialTable;
    AUTHREG_USERTABLE st_UserTable;
    XCHAR tszSQLStatement[1024];    //SQL语句

    memset(tszSQLStatement, '\0', 1024);
    memset(&st_SerialTable, '\0', sizeof(st_SerialTable));
    memset(&st_UserTable, '\0', sizeof(st_UserTable));
    //参数检查
    if ((NULL == lpszUserName) || (NULL == lpszSerialName))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_PARAMENT;
        return false;
    }
    //查询充值卡是否存在
    if (!DBModule_SQLite_SerialQuery(lpszSerialName, &st_SerialTable))
    {
        return false;
    }
    //充值卡是否被使用
    if (st_SerialTable.bIsUsed)
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_ISUSED;
        return false;
    }
    //查询用户信息
    if (!DBModule_SQLite_UserQuery(lpszUserName, &st_UserTable))
    {
        return false;
    }
    //分析插入方式
    switch (st_SerialTable.enSerialType)
    {
    case ENUM_VERIFICATION_MODULE_SERIAL_TYPE_SECOND:
        if (!DBModule_SQLite_UserPayTime(lpszUserName, st_UserTable.tszLeftTime, st_SerialTable.tszMaxTime, st_SerialTable.enSerialType, st_UserTable.enSerialType))
        {
            return false;
        }
        break;
    case ENUM_VERIFICATION_MODULE_SERIAL_TYPE_DAY:
        if (!DBModule_SQLite_UserPayTime(lpszUserName, st_UserTable.tszLeftTime, st_SerialTable.tszMaxTime, st_SerialTable.enSerialType, st_UserTable.enSerialType))
        {
            return false;
        }
        break;
    case ENUM_VERIFICATION_MODULE_SERIAL_TYPE_TIME:
        if (!DBModule_SQLite_UserPayTime(lpszUserName, st_UserTable.tszLeftTime, st_SerialTable.tszMaxTime, st_SerialTable.enSerialType, st_UserTable.enSerialType))
        {
            return false;
        }
        break;
    case ENUM_VERIFICATION_MODULE_SERIAL_TYPE_CUSTOM:
        if (!DBModule_SQLite_UserPayTime(lpszUserName, st_UserTable.tszLeftTime, st_SerialTable.tszMaxTime, st_SerialTable.enSerialType, st_UserTable.enSerialType))
        {
            return false;
        }
        break;
    default:
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_NOTSUPPORT;
        return false;
    }
    _xstprintf(tszSQLStatement, _X("UPDATE Authorize_Serial SET UserName = '%s',bIsUsed = '1' WHERE SerialNumber = '%s'"), lpszUserName, lpszSerialName);
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_UPDATAUSEDNAME;
        return false;
    }
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_UserLeave(AUTHREG_PROTOCOL_TIME* pSt_TimeProtocol)
{
    SQLPacket_IsErrorOccur = false;

    XCHAR tszSQLStatement[1024];       //SQL语句
    memset(tszSQLStatement, '\0', 1024);

    if ((ENUM_VERIFICATION_MODULE_SERIAL_TYPE_DAY == pSt_TimeProtocol->enSerialType) || (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_CUSTOM == pSt_TimeProtocol->enSerialType))
    {
        //天数卡只有剩余时间没有的时候才需要做处理
        if (pSt_TimeProtocol->nTimeLeft <= 0)
        {
            _xstprintf(tszSQLStatement, _X("UPDATE Authorize_User SET LeftTime = '0' WHERE UserName = '%s'"), pSt_TimeProtocol->tszUserName);
        }
    }
    if (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_SECOND == pSt_TimeProtocol->enSerialType)
    {
        //分钟卡必须要有在线时间才能计算
        if (pSt_TimeProtocol->nTimeLeft <= 0)
        {
            pSt_TimeProtocol->nTimeLeft = 0;
        }
        _xstprintf(tszSQLStatement, _X("UPDATE Authorize_User SET LeftTime = '%lld' WHERE UserName = '%s'"), pSt_TimeProtocol->nTimeLeft, pSt_TimeProtocol->tszUserName);
    }
    else if (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_TIME == pSt_TimeProtocol->enSerialType)
    {
        //次数卡不需要在线时间,直接减去一次就可以了
        _xstprintf(tszSQLStatement, _X("UPDATE Authorize_User SET LeftTime = '%lld' WHERE UserName = '%s'"), _ttxoll(pSt_TimeProtocol->tszLeftTime), pSt_TimeProtocol->tszUserName);
    }
    else
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_UNKNOWTYPE;
        return false;
    }
    //更新用户剩余时间
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_UPDATA;
        return false;
    }
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_UserSet(AUTHREG_USERTABLE* pSt_UserTable)
{
    SQLPacket_IsErrorOccur = false;

    XCHAR tszSQLStatement[1024];       //SQL语句
    memset(tszSQLStatement, '\0', 1024);

    _xstprintf(tszSQLStatement, _X("UPDATE Authorize_User SET Password = '%s',LeftTime = '%s',EmailAddr = '%s',HardCode = '%s',CardSerialType = '%d',PhoneNumber = '%lld',IDCard = '%lld',nUserLevel = '%d',UPTime = '%s',CreateTime = '%s' WHERE UserName = '%s'"), pSt_UserTable->st_UserInfo.tszUserPass, pSt_UserTable->tszLeftTime, pSt_UserTable->st_UserInfo.tszEMailAddr, pSt_UserTable->tszHardCode, pSt_UserTable->enSerialType, pSt_UserTable->st_UserInfo.nPhoneNumber, pSt_UserTable->st_UserInfo.nIDNumber, pSt_UserTable->st_UserInfo.nUserLevel, pSt_UserTable->st_UserInfo.tszLoginTime, pSt_UserTable->st_UserInfo.tszCreateTime, pSt_UserTable->st_UserInfo.tszUserName);
    //更新用户剩余时间
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return false;
    }
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_UserList(AUTHREG_USERTABLE*** pppSt_UserInfo, int* pInt_ListCount, int nPosStart, int nPosEnd)
{
    SQLPacket_IsErrorOccur = false;

    int nRow = 0;
    int nColumn = 0;
    char** ppszResult = NULL;
    XCHAR tszSQLStatement[1024];    //SQL语句
    memset(tszSQLStatement, '\0', 1024);

    _xstprintf(tszSQLStatement, _X("SELECT * FROM Authorize_User LIMIT %d,%d"), nPosStart, nPosEnd - nPosStart);
    if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_GETTABLE;
        return false;
    }
    if ((0 == nRow) || (0 == nColumn))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_NOTUSER;
        return false;
    }
    *pInt_ListCount = nRow;
    BaseLib_Memory_Malloc((XPPPMEM)pppSt_UserInfo, nRow, sizeof(AUTHREG_USERTABLE));
    //ID
    int nFliedValue = nColumn;
    for (int i = 0; i < nRow; i++)
    {
        nFliedValue++;          //ID
        //用户名
        _tcsxcpy((*pppSt_UserInfo)[i]->st_UserInfo.tszUserName, ppszResult[nFliedValue]);
        //密码
        nFliedValue++;
        _tcsxcpy((*pppSt_UserInfo)[i]->st_UserInfo.tszUserPass, ppszResult[nFliedValue]);
        //过期时间
        nFliedValue++;
        _tcsxcpy((*pppSt_UserInfo)[i]->tszLeftTime, ppszResult[nFliedValue]);
        //电子邮件
        nFliedValue++;
        _tcsxcpy((*pppSt_UserInfo)[i]->st_UserInfo.tszEMailAddr, ppszResult[nFliedValue]);
        //硬件码
        nFliedValue++;
        _tcsxcpy((*pppSt_UserInfo)[i]->tszHardCode, ppszResult[nFliedValue]);
        //充值卡类型
        nFliedValue++;
        (*pppSt_UserInfo)[i]->enSerialType = (ENUM_VERIFICATION_MODULE_SERIAL_TYPE)_ttxoi(ppszResult[nFliedValue]);
        //QQ号
        nFliedValue++;
        (*pppSt_UserInfo)[i]->st_UserInfo.nPhoneNumber = _ttxoll(ppszResult[nFliedValue]);
        //身份证ID
        nFliedValue++;
        (*pppSt_UserInfo)[i]->st_UserInfo.nIDNumber = _ttxoll(ppszResult[nFliedValue]);
        //用户级别 -1表示封禁
        nFliedValue++;
        (*pppSt_UserInfo)[i]->st_UserInfo.nUserLevel = _ttxoi(ppszResult[nFliedValue]);
        //登录日期
        nFliedValue++;
		if (NULL != ppszResult[nFliedValue] && _tcsxlen(ppszResult[nFliedValue]) > 0)
		{
			_tcsxcpy((*pppSt_UserInfo)[i]->st_UserInfo.tszLoginTime, ppszResult[nFliedValue]);
		}
		//注册日期
		nFliedValue++;
		_tcsxcpy((*pppSt_UserInfo)[i]->st_UserInfo.tszCreateTime, ppszResult[nFliedValue]);
		nFliedValue++;
    }
    DataBase_SQLite_FreeTable(ppszResult);
    return true;
}
/********************************************************************
函数名称：DBModule_SQLite_UserLogin
函数功能：用户登录信息记录
 参数.一：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户名
 参数.二：lpszUserAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户IP地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CDBModule_SQLite::DBModule_SQLite_UserLogin(LPCXSTR lpszUserName, LPCXSTR lpszUserAddr)
{
	SQLPacket_IsErrorOccur = false;

    XCHAR tszSQLStatement[1024] = {};
	XCHAR tszTimeStr[128] = {};
	BaseLib_Time_TimeToStr(tszTimeStr, NULL, false);

    _xstprintf(tszSQLStatement, _X("INSERT INTO Authorize_Login(UserName, UserAddr, UserTime) values('%s','%s','%s')"), lpszUserName, lpszUserAddr, tszTimeStr);
	if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
	{
		SQLPacket_IsErrorOccur = true;
		SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_INSERT;
		return false;
	}
	return true;
}
/********************************************************************
函数名称：DBModule_SQLite_QueryLogin
函数功能：用户登录记录查询
 参数.一：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户名
 参数.二：lpszUserAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户IP地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CDBModule_SQLite::DBModule_SQLite_QueryLogin(LPCXSTR lpszUserName, LPCXSTR lpszUserAddr)
{
	SQLPacket_IsErrorOccur = false;

	XCHAR tszSQLStatement[1024];    //SQL语句
	char** ppszResult = NULL;
	int nRow = 0;
	int nColumn = 0;
	memset(tszSQLStatement, '\0', 1024);

    XCHAR tszTimeStr[128] = {};
    BaseLib_Time_TimeToStr(tszTimeStr, NULL, false);

    _xstprintf(tszSQLStatement, _X("SELECT * FROM `Authorize_Login` WHERE UserName = '%s' AND UserTime = '%s' AND UserAddr = '%s'"), lpszUserName, tszTimeStr, lpszUserAddr);
	if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
	{
		SQLPacket_IsErrorOccur = true;
		SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_GETTABLE;
		return false;
	}
	if ((0 == nRow) || (0 == nColumn))
	{
		SQLPacket_IsErrorOccur = true;
		SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_NOTUSER;
		return false;
	}
	DataBase_SQLite_FreeTable(ppszResult);
	return true;
}
/********************************************************************
函数名称：DBModule_SQLite_SerialInsert
函数功能：插入一个序列号到数据库
 参数.一：pSt_SerialTable
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要插入的序列号
返回值
  类型：逻辑型
  意思：是否插入成功
备注：
*********************************************************************/
bool CDBModule_SQLite::DBModule_SQLite_SerialInsert(AUTHREG_SERIALTABLE *pSt_SerialTable)
{
    SQLPacket_IsErrorOccur = false;

    XCHAR tszSQLStatement[1024];    //SQL语句
    memset(tszSQLStatement, '\0', 1024);

    if (DBModule_SQLite_SerialQuery(pSt_SerialTable->tszSerialNumber))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_EXIST;
        return false;
    }
    _xstprintf(tszSQLStatement, _X("INSERT INTO Authorize_Serial values(NULL,'NOT','%s','%s','%d',0,datetime('now', 'localtime'),'%s')"), pSt_SerialTable->tszSerialNumber, pSt_SerialTable->tszMaxTime, pSt_SerialTable->enSerialType, pSt_SerialTable->tszExpiredTime);

    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_ISFAILED;
        return false;
    }
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_SerialDelete(LPCXSTR lpszSerialNumber)
{
    SQLPacket_IsErrorOccur = false;

    XCHAR tszSQLStatement[1024];    //SQL语句
    memset(tszSQLStatement, '\0', 1024);
    _xstprintf(tszSQLStatement, _X("delete from Authorize_Serial where SerialNumber = '%s'"), lpszSerialNumber);
    //执行
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_ISFAILED;
        return false;
    }
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_SerialQuery(LPCXSTR lpszSerialNumber, LPAUTHREG_SERIALTABLE pSt_SerialTable /* = NULL */)
{
    SQLPacket_IsErrorOccur = false;
    XCHAR tszSQLStatement[1024];    //SQL语句
    char** ppszResult = NULL;
    int nRow = 0;
    int nColumn = 0;
    memset(tszSQLStatement, '\0', 1024);

    _xstprintf(tszSQLStatement, _X("select * from Authorize_Serial where SerialNumber = '%s'"), lpszSerialNumber);
    if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_GETTABLE;
        return false;
    }
    if ((0 == nRow) || (0 == nColumn))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_SERIAL;
        return false;
    }
    if (NULL != pSt_SerialTable)
    {
        memset(pSt_SerialTable, '\0', sizeof(AUTHREG_SERIALTABLE));
        //ID
        int nFliedValue = nColumn;
        //使用者
        nFliedValue++;
        _tcsxcpy(pSt_SerialTable->tszUserName, ppszResult[nFliedValue]);
        //序列号
        nFliedValue++;
        _tcsxcpy(pSt_SerialTable->tszSerialNumber, ppszResult[nFliedValue]);
        //超时时间
        nFliedValue++;
        _tcsxcpy(pSt_SerialTable->tszMaxTime, ppszResult[nFliedValue]);
        //序列卡类型
        nFliedValue++;
        pSt_SerialTable->enSerialType = (ENUM_VERIFICATION_MODULE_SERIAL_TYPE)_ttxoi(ppszResult[nFliedValue]);
        //是否已经使用
        nFliedValue++;
        pSt_SerialTable->bIsUsed = _ttxoi(ppszResult[nFliedValue]);
        //超时时间
        nFliedValue++;
        _tcsxcpy(pSt_SerialTable->tszCreateTime, ppszResult[nFliedValue]);
		//过期时间
		nFliedValue++;
		if (NULL != ppszResult[nFliedValue])
		{
            _tcsxcpy(pSt_SerialTable->tszExpiredTime, ppszResult[nFliedValue]);
		}
    }

    DataBase_SQLite_FreeTable(ppszResult);
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_SerialQueryAll(AUTHREG_SERIALTABLE*** pppSt_SerialTable, int* pInt_ListCount, int nPosStart, int nPosEnd)
{
    SQLPacket_IsErrorOccur = false;

    int nRow = 0;
    int nColumn = 0;
    char** ppszResult = NULL;
    XCHAR tszSQLStatement[1024];    //SQL语句

    memset(tszSQLStatement, '\0', 1024);

    _xstprintf(tszSQLStatement, _X("SELECT * FROM Authorize_Serial LIMIT %d,%d"), nPosStart, nPosEnd - nPosStart);
    if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_GETTABLE;
        return false;
    }
    if ((0 == nRow) || (0 == nColumn))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_NONE;
        return false;
    }
    BaseLib_Memory_Malloc((XPPPMEM)pppSt_SerialTable, nRow, sizeof(AUTHREG_SERIALTABLE));

    *pInt_ListCount = nRow;
    int nFliedValue = nColumn;
    for (int i = 0; i < nRow; i++)
    {
        //ID
        nFliedValue++;
        //使用者
        _tcsxcpy((*pppSt_SerialTable)[i]->tszUserName, ppszResult[nFliedValue]);
        nFliedValue++;
        //序列号
        _tcsxcpy((*pppSt_SerialTable)[i]->tszSerialNumber, ppszResult[nFliedValue]);
        nFliedValue++;
        //超时时间
        _tcsxcpy((*pppSt_SerialTable)[i]->tszMaxTime, ppszResult[nFliedValue]);
        nFliedValue++;
        //序列卡类型
        (*pppSt_SerialTable)[i]->enSerialType = (ENUM_VERIFICATION_MODULE_SERIAL_TYPE)_ttxoi(ppszResult[nFliedValue]);
        nFliedValue++;
        //是否已经使用
        (*pppSt_SerialTable)[i]->bIsUsed = _ttxoi(ppszResult[nFliedValue]);
        nFliedValue++;
        //创建时间
        _tcsxcpy((*pppSt_SerialTable)[i]->tszCreateTime, ppszResult[nFliedValue]);
        nFliedValue++;
		//过期时间
        if (NULL != ppszResult[nFliedValue])
        {
            _tcsxcpy((*pppSt_SerialTable)[i]->tszExpiredTime, ppszResult[nFliedValue]);
        }
        nFliedValue++;
    }
    DataBase_SQLite_FreeTable(ppszResult);
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_SerialPush(AUTHREG_SERIALTABLE* pSt_SerialTable)
{
    SQLPacket_IsErrorOccur = false;

    XCHAR tszSQLStatement[1024];    //SQL语句
    memset(tszSQLStatement, '\0', 1024);

    if (DBModule_SQLite_SerialQuery(pSt_SerialTable->tszSerialNumber))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_EXIST;
        return false;
    }
    _xstprintf(tszSQLStatement, _X("INSERT INTO Authorize_Serial values(NULL,'%s','%s','%s',%d,%d,'%s','%s')"), pSt_SerialTable->tszUserName, pSt_SerialTable->tszSerialNumber, pSt_SerialTable->tszMaxTime, pSt_SerialTable->enSerialType, pSt_SerialTable->bIsUsed, pSt_SerialTable->tszCreateTime, pSt_SerialTable->tszExpiredTime);

    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_ISFAILED;
        return false;
    }
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_TryInsert(AUTHREG_TEMPVER* pSt_AuthVer)
{
    SQLPacket_IsErrorOccur = false;

    if (NULL == pSt_AuthVer)
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_PARAMENT;
        return false;
    }
    XCHAR tszSQLStatement[1024];
    AUTHREG_TEMPVER st_AuthVer;

    memset(tszSQLStatement, '\0', 1024);
    memset(&st_AuthVer, '\0', sizeof(AUTHREG_TEMPVER));

    //验证是否存在
    _tcsxcpy(st_AuthVer.tszVSerial, pSt_AuthVer->tszVSerial);
    if (DBModule_SQLite_TryQuery(&st_AuthVer))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_EXIST;
        return false;
    }
    //插入数据库
    _xstprintf(tszSQLStatement, _X("INSERT INTO Authorize_TempVer(tszVSerial,nVMode,nVTime,nLTime,CreateTime) VALUES('%s',%d,%d,%d,datetime('now', 'localtime'))"), pSt_AuthVer->tszVSerial, pSt_AuthVer->enSerialType, pSt_AuthVer->nVTime, pSt_AuthVer->nVTime);
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return false;
    }
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_TryQuery(AUTHREG_TEMPVER* pSt_AuthVer)
{
    SQLPacket_IsErrorOccur = false;

    if (NULL == pSt_AuthVer)
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_PARAMENT;
        return false;
    }
    int nRow = 0;
    int nColumn = 0;
    XCHAR** ppszResult = NULL;
    XCHAR tszSQLStatement[1024];    //SQL语句

    memset(tszSQLStatement, '\0', 1024);
    _xstprintf(tszSQLStatement, _X("SELECT * FROM Authorize_TempVer WHERE tszVSerial = '%s'"), pSt_AuthVer->tszVSerial);

    if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return false;
    }
    if ((0 == nRow) || (0 == nColumn))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_NONE;
        return false;
    }
    //ID
    int nFliedValue = nColumn;
    nFliedValue++;
    //序列号
    nFliedValue++;
    //试用类型
    pSt_AuthVer->enSerialType = (ENUM_VERIFICATION_MODULE_SERIAL_TYPE)_ttxoi(ppszResult[nFliedValue]);
    nFliedValue++;
    //试用时间
    pSt_AuthVer->nVTime = _ttxoi(ppszResult[nFliedValue]);
    nFliedValue++;
	//剩余时间
	pSt_AuthVer->nLTime = _ttxoi(ppszResult[nFliedValue]);
	nFliedValue++;
    //注册时间
    _tcsxcpy(pSt_AuthVer->tszVDate, ppszResult[nFliedValue]);
    DataBase_SQLite_FreeTable(ppszResult);
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_TryDelete(LPCXSTR lpszSerial)
{
    SQLPacket_IsErrorOccur = false;

    if (NULL == lpszSerial)
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_PARAMENT;
        return false;
    }
    XCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', 1024);

    _xstprintf(tszSQLStatement, _X("DELETE FROM Authorize_TempVer WHERE tszVSerial = '%s'"), lpszSerial);

    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return false;
    }
    return true;
}
/********************************************************************
函数名称：DBModule_SQLite_TryClear
函数功能：清理函数,自动清理过期数据
 参数.一：nThanValue
  In/Out：In
  类型：整数型
  可空：N
  意思：清理用于判断需要大于此的值
 参数.二：enVMode
  In/Out：In
  类型：枚举型
  可空：Y
  意思：判断注册类型,默认不判断全部执行
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CDBModule_SQLite::DBModule_SQLite_TryClear(int nThanValue, ENUM_VERIFICATION_MODULE_SERIAL_TYPE enVMode /* = ENUM_VERIFICATION_MODULE_SERIAL_TYPE_UNKNOW */)
{
    SQLPacket_IsErrorOccur = false;

    int nRow = 0;
    int nColumn = 0;
    XCHAR** ppszResult = NULL;
    XCHAR tszSQLStatement[1024];    //SQL语句

    memset(tszSQLStatement, '\0', 1024);
    _xstprintf(tszSQLStatement, _X("SELECT * FROM Authorize_TempVer"));

    if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return false;
    }
    int nFliedValue = nColumn;
    list<AUTHREG_TEMPVER> stl_ListVer;
    //轮训所有内容
    for (int i = 0; i < nRow; i++)
    {
        AUTHREG_TEMPVER st_AuthVer;
        memset(&st_AuthVer, '\0', sizeof(AUTHREG_TEMPVER));
        //ID
        nFliedValue++;
        //序列号
        _tcsxcpy(st_AuthVer.tszVSerial, ppszResult[nFliedValue]);
        nFliedValue++;
        //模式
        st_AuthVer.enSerialType = (ENUM_VERIFICATION_MODULE_SERIAL_TYPE)_ttxoi(ppszResult[nFliedValue]);
        nFliedValue++;
        //测试时间
        st_AuthVer.nVTime = _ttxoi(ppszResult[nFliedValue]);
        nFliedValue++;
		//测试时间
		st_AuthVer.nLTime = _ttxoi(ppszResult[nFliedValue]);
		nFliedValue++;
        //注册时间
        _tcsxcpy(st_AuthVer.tszVDate, ppszResult[nFliedValue]);

        stl_ListVer.push_back(st_AuthVer);
    }
    DataBase_SQLite_FreeTable(ppszResult);
    //清理
    list<AUTHREG_TEMPVER>::const_iterator stl_ListIterator = stl_ListVer.begin();
    for (; stl_ListIterator != stl_ListVer.end(); stl_ListIterator++)
    {
        //判断是不是不关心注册的模式直接清理
        if (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_UNKNOW == enVMode)
        {
            if (nThanValue > stl_ListIterator->nVTime)
            {
                memset(tszSQLStatement, '\0', 1024);
                _xstprintf(tszSQLStatement, _X("DELETE FROM Authorize_TempVer WHERE tszVSerial = '%s'"), stl_ListIterator->tszVSerial);

                if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
                {
                    SQLPacket_IsErrorOccur = true;
                    SQLPacket_dwErrorCode = DataBase_GetLastError();
                    return false;
                }
            }
        }
        else
        {
            if (enVMode == stl_ListIterator->enSerialType)
            {
                memset(tszSQLStatement, '\0', 1024);
                _xstprintf(tszSQLStatement, _X("DELETE FROM Authorize_TempVer WHERE tszVSerial = '%s'"), stl_ListIterator->tszVSerial);

                if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
                {
                    SQLPacket_IsErrorOccur = true;
                    SQLPacket_dwErrorCode = DataBase_GetLastError();
                    return false;
                }
            }
        }
    }
    stl_ListVer.clear();
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_TrySet(AUTHREG_TEMPVER* pSt_AuthVer)
{
    SQLPacket_IsErrorOccur = false;

    XCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));

    _xstprintf(tszSQLStatement, _X("UPDATE Authorize_TempVer SET nVMode = '%d',nVTime = '%d',nLTime = '%d',CreateTime = '%s' WHERE tszVSerial = '%s'"), pSt_AuthVer->enSerialType, pSt_AuthVer->nVTime, pSt_AuthVer->nLTime, pSt_AuthVer->tszVDate, pSt_AuthVer->tszVSerial);
    //更新用户表
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return false;
    }
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_TryList(AUTHREG_TEMPVER*** pppSt_AuthVer, int* pInt_ListCount, int nPosStart, int nPosEnd)
{
	SQLPacket_IsErrorOccur = false;

	int nRow = 0;
	int nColumn = 0;
	XCHAR** ppszResult = NULL;
	XCHAR tszSQLStr[1024];    //SQL语句

	memset(tszSQLStr, '\0', 1024);
	_xstprintf(tszSQLStr, _X("SELECT * FROM Authorize_TempVer LIMIT %d,%d"), nPosStart, nPosEnd - nPosStart);

	if (!DataBase_SQLite_GetTable(xhData, tszSQLStr, &ppszResult, &nRow, &nColumn))
	{
		SQLPacket_IsErrorOccur = true;
		SQLPacket_dwErrorCode = DataBase_GetLastError();
		return false;
	}
    BaseLib_Memory_Malloc((XPPPMEM)pppSt_AuthVer, nRow, sizeof(AUTHREG_TEMPVER));
	int nFliedValue = nColumn;
	//轮训所有内容
	for (int i = 0; i < nRow; i++)
	{
		//ID
        (*pppSt_AuthVer)[i]->nID = _ttxoi(ppszResult[nFliedValue]);
		nFliedValue++;
		//是否启用
        _tcsxcpy((*pppSt_AuthVer)[i]->tszVSerial, ppszResult[nFliedValue]);
		nFliedValue++;
		//类型
        (*pppSt_AuthVer)[i]->enSerialType = (ENUM_VERIFICATION_MODULE_SERIAL_TYPE)_ttxoi(ppszResult[nFliedValue]);
		nFliedValue++;
		//时间
        (*pppSt_AuthVer)[i]->nVTime = _ttxoi(ppszResult[nFliedValue]);
		nFliedValue++;
		//时间
		(*pppSt_AuthVer)[i]->nLTime = _ttxoi(ppszResult[nFliedValue]);
		nFliedValue++;
		//注册时间
		_tcsxcpy((*pppSt_AuthVer)[i]->tszVDate, ppszResult[nFliedValue]);
		nFliedValue++;
	}
    *pInt_ListCount = nRow;
	DataBase_SQLite_FreeTable(ppszResult);
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_BannedInsert(AUTHREG_BANNED* pSt_Banned)
{
    SQLPacket_IsErrorOccur = false;

    if (NULL == pSt_Banned)
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_PARAMENT;
        return false;
    }
    //存在直接返回
    if (!DBModule_SQLite_BannedExist(pSt_Banned))
    {
        return true;
    }
    XCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //处理的类型
    if (_tcsxlen(pSt_Banned->tszUserName) > 0)
    {
        _xstprintf(tszSQLStatement, _X("INSERT INTO Authorize_BannedUser(bEnable,tszUserName,tszLeftTime,tszCreateTime) VALUES(%d,'%s','%s',datetime('now', 'localtime'))"), pSt_Banned->bEnable, pSt_Banned->tszUserName, pSt_Banned->tszLeftTime);
    }
    else
    {
        _xstprintf(tszSQLStatement, _X("INSERT INTO Authorize_BannedAddr(bEnable,tszIPAddr,tszLeftTime,tszCreateTime) VALUES(%d,'%s','%s',datetime('now', 'localtime'))"), pSt_Banned->bEnable, pSt_Banned->tszIPAddr, pSt_Banned->tszLeftTime);
    }
    //插入数据库
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return false;
    }
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_BannedDelete(AUTHREG_BANNED* pSt_Banned)
{
    SQLPacket_IsErrorOccur = false;

    if (NULL == pSt_Banned)
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_PARAMENT;
        return false;
    }
    XCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', 1024);
    //处理的类型
    if (_tcsxlen(pSt_Banned->tszUserName) > 0)
    {
        _xstprintf(tszSQLStatement, _X("DELETE FROM Authorize_BannedUser WHERE tszUserName = '%s'"), pSt_Banned->tszUserName);
    }
    else
    {
        _xstprintf(tszSQLStatement, _X("DELETE FROM Authorize_BannedAddr WHERE tszIPAddr = '%s'"), pSt_Banned->tszIPAddr);
    }
    //操作数据库
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return false;
    }
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_BannedList(AUTHREG_BANNED*** pppSt_BannedUser, int* pInt_UserCount, AUTHREG_BANNED*** pppSt_BannedAddr, int* pInt_AddrCount, int nPosStart, int nPosEnd)
{
    SQLPacket_IsErrorOccur = false;

    int nRow = 0;
    int nColumn = 0;
    XCHAR** ppszResult = NULL;
    XCHAR tszSQLStatement[1024];    //SQL语句

    memset(tszSQLStatement, '\0', 1024);
    _xstprintf(tszSQLStatement, _X("SELECT * FROM Authorize_BannedAddr LIMIT %d,%d"), nPosStart, nPosEnd - nPosStart);

    if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return false;
    }
    int nFliedValue = nColumn;
    list<AUTHREG_BANNED> stl_ListAddr;
    //轮训所有内容
    for (int i = 0; i < nRow; i++)
    {
        AUTHREG_BANNED st_Banned;
        memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));
        //ID
        st_Banned.nID = _ttxoll(ppszResult[nFliedValue]);
        nFliedValue++;
		//是否启用
		st_Banned.bEnable = _ttxoi(ppszResult[nFliedValue]);
		nFliedValue++;
        //地址
        _tcsxcpy(st_Banned.tszIPAddr, ppszResult[nFliedValue]);
        nFliedValue++;
        //过期时间
        _tcsxcpy(st_Banned.tszLeftTime, ppszResult[nFliedValue]);
        nFliedValue++;
        //注册时间
        _tcsxcpy(st_Banned.tszCreateTime, ppszResult[nFliedValue]);
        nFliedValue++;

        stl_ListAddr.push_back(st_Banned);
    }
    DataBase_SQLite_FreeTable(ppszResult);
    //用户列表
    nRow = 0;
    nColumn = 0;
    memset(tszSQLStatement, '\0', 1024);
    _xstprintf(tszSQLStatement, _X("SELECT * FROM Authorize_BannedUser LIMIT %d,%d"), nPosStart, nPosEnd - nPosStart);

    if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return false;
    }
    nFliedValue = nColumn;
    list<AUTHREG_BANNED> stl_ListUser;
    //轮训所有内容
    for (int i = 0; i < nRow; i++)
    {
        AUTHREG_BANNED st_Banned;
        memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));
        //ID
        st_Banned.nID = _ttxoll(ppszResult[nFliedValue]);
        nFliedValue++;
		//是否启用
		st_Banned.bEnable = _ttxoi(ppszResult[nFliedValue]);
		nFliedValue++;
        //用户名
        _tcsxcpy(st_Banned.tszUserName, ppszResult[nFliedValue]);
        nFliedValue++;
        //过期时间
        _tcsxcpy(st_Banned.tszLeftTime, ppszResult[nFliedValue]);
        nFliedValue++;
        //注册时间
        _tcsxcpy(st_Banned.tszCreateTime, ppszResult[nFliedValue]);
        nFliedValue++;

        stl_ListUser.push_back(st_Banned);
    }
    DataBase_SQLite_FreeTable(ppszResult);
    //导出
    *pInt_AddrCount = stl_ListAddr.size();
    *pInt_UserCount = stl_ListUser.size();
    BaseLib_Memory_Malloc((XPPPMEM)pppSt_BannedAddr, stl_ListAddr.size(), sizeof(AUTHREG_BANNED));
    BaseLib_Memory_Malloc((XPPPMEM)pppSt_BannedUser, stl_ListUser.size(), sizeof(AUTHREG_BANNED));

    list<AUTHREG_BANNED>::const_iterator stl_ListIterator = stl_ListAddr.begin();
    for (int i = 0; stl_ListIterator != stl_ListAddr.end(); stl_ListIterator++, i++)
    {
        (*pppSt_BannedAddr)[i]->nID = stl_ListIterator->nID;
        (*pppSt_BannedAddr)[i]->bEnable = stl_ListIterator->bEnable;
        _tcsxcpy((*pppSt_BannedAddr)[i]->tszIPAddr, stl_ListIterator->tszIPAddr);
        _tcsxcpy((*pppSt_BannedAddr)[i]->tszLeftTime, stl_ListIterator->tszLeftTime);
        _tcsxcpy((*pppSt_BannedAddr)[i]->tszCreateTime, stl_ListIterator->tszCreateTime);
    }
    stl_ListIterator = stl_ListUser.begin();
    for (int i = 0; stl_ListIterator != stl_ListUser.end(); stl_ListIterator++, i++)
    {
        (*pppSt_BannedUser)[i]->nID = stl_ListIterator->nID;
        (*pppSt_BannedUser)[i]->bEnable = stl_ListIterator->bEnable;
        _tcsxcpy((*pppSt_BannedUser)[i]->tszUserName, stl_ListIterator->tszUserName);
        _tcsxcpy((*pppSt_BannedUser)[i]->tszLeftTime, stl_ListIterator->tszLeftTime);
        _tcsxcpy((*pppSt_BannedUser)[i]->tszCreateTime, stl_ListIterator->tszCreateTime);
    }

    stl_ListAddr.clear();
    stl_ListUser.clear();
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_BannedExist(AUTHREG_BANNED* pSt_Banned)
{
    SQLPacket_IsErrorOccur = false;
    //判断用户名是否存在
    if (_tcsxlen(pSt_Banned->tszUserName) > 0)
    {
        int nRow = 0;
        int nColumn = 0;
        XCHAR** ppszResult = NULL;
        XCHAR tszSQLStatement[1024];
        memset(tszSQLStatement, '\0', 1024);

        _xstprintf(tszSQLStatement, _X("SELECT * FROM Authorize_BannedUser WHERE tszUserName = '%s'"), pSt_Banned->tszUserName);
        if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
        {
            SQLPacket_IsErrorOccur = true;
            SQLPacket_dwErrorCode = DataBase_GetLastError();
            return false;
        }
		if (nRow > 0)
		{
            int nFliedValue = nColumn;
            //ID
            pSt_Banned->nID = _ttxoll(ppszResult[nFliedValue]);
            nFliedValue++;
            //是否启用
            pSt_Banned->bEnable = _ttxoi(ppszResult[nFliedValue]);
            nFliedValue++;
            //地址
            nFliedValue++;
            //过期时间
            _tcsxcpy(pSt_Banned->tszLeftTime, ppszResult[nFliedValue]);
            nFliedValue++;
            //注册时间
            _tcsxcpy(pSt_Banned->tszCreateTime, ppszResult[nFliedValue]);
            //如果启用 且 tszLeftTime 大于 0
            if (pSt_Banned->bEnable)
            {
                if (_tcsxlen(pSt_Banned->tszLeftTime) > 0)
                {
					__int64x nTimer = 0;
					XCHAR tszStrTime[128];
					memset(tszStrTime, '\0', sizeof(tszStrTime));

					BaseLib_Time_TimeToStr(tszStrTime);
					BaseLib_TimeSpan_GetForStr(pSt_Banned->tszLeftTime, tszStrTime, &nTimer, 3);
					if (nTimer < 0)  
					{
						SQLPacket_IsErrorOccur = true;
						SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_BANNED;
						return false;
					}
                }
                else
                {
					SQLPacket_IsErrorOccur = true;
					SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_BANNED;
					return false;
                }
            }
		}
        DataBase_SQLite_FreeTable(ppszResult);
    }
    //判断IP地址是否存在
    if (_tcsxlen(pSt_Banned->tszIPAddr) > 0)
    {
        int nRow = 0;
        int nColumn = 0;
        XCHAR** ppszResult = NULL;
        XCHAR tszSQLStatement[1024];
        memset(tszSQLStatement, '\0', 1024);

       _xstprintf(tszSQLStatement, _X("SELECT * FROM Authorize_BannedAddr WHERE tszIPAddr = '%s'"), pSt_Banned->tszIPAddr);
        if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
        {
            SQLPacket_IsErrorOccur = true;
            SQLPacket_dwErrorCode = DataBase_GetLastError();
            return false;
        }
        if (nRow > 0)
        {
			int nFliedValue = nColumn;
			//ID
			pSt_Banned->nID = _ttxoll(ppszResult[nFliedValue]);
			nFliedValue++;
			//是否启用
			pSt_Banned->bEnable = _ttxoi(ppszResult[nFliedValue]);
			nFliedValue++;
			//地址
			nFliedValue++;
			//过期时间
			_tcsxcpy(pSt_Banned->tszLeftTime, ppszResult[nFliedValue]);
			nFliedValue++;
			//注册时间
			_tcsxcpy(pSt_Banned->tszCreateTime, ppszResult[nFliedValue]);

			//处理是否被禁用
			if (pSt_Banned->bEnable)
			{
				if (_tcsxlen(pSt_Banned->tszLeftTime) > 0)
				{
					__int64x nTimer = 0;
					XCHAR tszStrTime[128];
					memset(tszStrTime, '\0', sizeof(tszStrTime));

					BaseLib_Time_TimeToStr(tszStrTime);
					BaseLib_TimeSpan_GetForStr(pSt_Banned->tszLeftTime, tszStrTime, &nTimer, 3);
					if (nTimer < 0)
					{
						SQLPacket_IsErrorOccur = true;
						SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_TIMELEFT;
						return false;
					}
				}
			}
            else
            {
				SQLPacket_IsErrorOccur = true;
				SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_TIMELEFT;
                return false;
            }
        }
        DataBase_SQLite_FreeTable(ppszResult);
    }
    return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_BannedUPDate(AUTHREG_BANNED* pSt_Banned)
{
	SQLPacket_IsErrorOccur = false;

	if (NULL == pSt_Banned)
	{
		SQLPacket_IsErrorOccur = true;
		SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_PARAMENT;
		return false;
	}
	XCHAR tszSQLStatement[1024];
	memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
	//处理的类型
	if (_tcsxlen(pSt_Banned->tszUserName) > 0)
	{
		_xstprintf(tszSQLStatement, _X("UPDATE Authorize_BannedUser SET bEnable = %d,tszLeftTime = '%s' WHERE tszUserName = '%s'"), pSt_Banned->bEnable, pSt_Banned->tszLeftTime, pSt_Banned->tszUserName);
	}
	else
	{
		_xstprintf(tszSQLStatement, _X("UPDATE Authorize_BannedAddr SET bEnable = %d,tszLeftTime = '%s' WHERE tszIPAddr = '%s'"), pSt_Banned->bEnable, pSt_Banned->tszLeftTime, pSt_Banned->tszIPAddr);
	}
	//插入数据库
	if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
	{
		SQLPacket_IsErrorOccur = true;
		SQLPacket_dwErrorCode = DataBase_GetLastError();
		return false;
	}
	return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_AnnouncementInsert(AUTHREG_ANNOUNCEMENT* pSt_Announcement)
{
	SQLPacket_IsErrorOccur = false;

	if (NULL == pSt_Announcement)
	{
		SQLPacket_IsErrorOccur = true;
		SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_PARAMENT;
		return false;
	}
	XCHAR tszSQLStatement[8192];
	memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));

    _xstprintf(tszSQLStatement, _X("INSERT INTO Authorize_Announcement(tszContext,tszCreateTime) VALUES('%s',datetime('now', 'localtime'))"), pSt_Announcement->tszContext);
	//插入数据库
	if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
	{
		SQLPacket_IsErrorOccur = true;
		SQLPacket_dwErrorCode = DataBase_GetLastError();
		return false;
	}
	return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_AnnouncementDelete(AUTHREG_ANNOUNCEMENT* pSt_Announcement)
{
	SQLPacket_IsErrorOccur = false;

	if (NULL == pSt_Announcement)
	{
		SQLPacket_IsErrorOccur = true;
		SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_PARAMENT;
		return false;
	}
	XCHAR tszSQLStatement[1024];
	memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));

    _xstprintf(tszSQLStatement, _X("DELETE FROM Authorize_Announcement WHERE ID = %lld"), pSt_Announcement->nID);
	//插入数据库
	if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
	{
		SQLPacket_IsErrorOccur = true;
		SQLPacket_dwErrorCode = DataBase_GetLastError();
		return false;
	}
	return true;
}
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
bool CDBModule_SQLite::DBModule_SQLite_AnnouncementList(AUTHREG_ANNOUNCEMENT*** ppppSt_Announcement, int* pInt_ListCount)
{
	SQLPacket_IsErrorOccur = false;

	int nRow = 0;
	int nColumn = 0;
	XCHAR** ppszResult = NULL;
	XCHAR tszSQLStatement[1024];    //SQL语句

	memset(tszSQLStatement, '\0', 1024);
	_xstprintf(tszSQLStatement, _X("SELECT * FROM Authorize_Announcement"));

	if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
	{
		SQLPacket_IsErrorOccur = true;
		SQLPacket_dwErrorCode = DataBase_GetLastError();
		return false;
	}
    *pInt_ListCount = nRow;
    if (NULL == ppppSt_Announcement)
    {
        return true;
    }
   
    BaseLib_Memory_Malloc((XPPPMEM)ppppSt_Announcement, nRow, sizeof(AUTHREG_ANNOUNCEMENT));

	int nFliedValue = nColumn;
	//轮训所有内容
	for (int i = 0; i < nRow; i++)
	{
		(*ppppSt_Announcement)[i]->nID = _ttxoll(ppszResult[nFliedValue]);
        nFliedValue++;
		_tcsxcpy((*ppppSt_Announcement)[i]->tszContext, ppszResult[nFliedValue]);
        nFliedValue++;
		_tcsxcpy((*ppppSt_Announcement)[i]->tszCreateTime, ppszResult[nFliedValue]);
        nFliedValue++;
	}
	DataBase_SQLite_FreeTable(ppszResult);
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                       保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：DBModule_SQLite_UserPayTime
函数功能：日期方式充值方式
 参数.一：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：充值的用户
 参数.二：lpszUserTime
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户以前的超时时间
 参数.三：lpszCardTime
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：充值卡时间
 参数.四：en_AuthSerialType
  In/Out：In
  类型：枚举型
  可空：N
  意思：充值卡的充值类型
 参数.五：en_AuthUserType
  In/Out：In
  类型：枚举型
  可空：N
  意思：用户表中以前的充值卡类型
返回值
  类型：逻辑型
  意思：是否成功充值
备注：
*********************************************************************/
bool CDBModule_SQLite::DBModule_SQLite_UserPayTime(LPCXSTR lpszUserName, LPCXSTR lpszUserTime, LPCXSTR lpszCardTime, ENUM_VERIFICATION_MODULE_SERIAL_TYPE en_AuthSerialType, ENUM_VERIFICATION_MODULE_SERIAL_TYPE en_AuthUserType)
{
    SQLPacket_IsErrorOccur = false;

    XCHAR tszSQLStatement[1024];
	memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //判断和以前的充值卡是否匹配。
    if (en_AuthSerialType != en_AuthUserType)
    {
        //如果不等于,需要重写
        if (ENUM_VERIFICATION_MODULE_SERIAL_TYPE_UNKNOW != en_AuthUserType)
        {
            //判断是否允许改写。
            if (!m_bChange)
            {
                SQLPacket_IsErrorOccur = true;
                SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_NOTMATCH;
                return false;
            }
        }
        //更新用户表的充值卡类型
        memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
        _xstprintf(tszSQLStatement, _X("UPDATE Authorize_User SET CardSerialType = '%d' WHERE UserName = '%s'"), en_AuthSerialType, lpszUserName);
        //执行用户表更新，因为序列卡类型被改变，所以需要更新。
        if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
        {
            SQLPacket_IsErrorOccur = true;
            SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_UPDATATYPE;
            return false;
        }
        //处理卡类型
        switch (en_AuthSerialType)
        {
        case ENUM_VERIFICATION_MODULE_SERIAL_TYPE_SECOND:
        {
            //如果是分钟卡
            //如果当前的充值卡类型不匹配，那么他以前的充值内容全部都会被删除！
            _xstprintf(tszSQLStatement, _X("UPDATE Authorize_User SET LeftTime = '%d' WHERE UserName = '%s'"), _ttxoi(lpszCardTime), lpszUserName);      //更新用户表的过期时间
        }
        break;
        case ENUM_VERIFICATION_MODULE_SERIAL_TYPE_DAY:
        {
            //如果是天数卡
            //更新用户表的过期时间
            _xstprintf(tszSQLStatement, _X("UPDATE Authorize_User SET LeftTime = '%d' WHERE UserName = '%s'"), _ttxoi(lpszCardTime), lpszUserName);      //更新用户表的过期时间
        }
        break;
        case ENUM_VERIFICATION_MODULE_SERIAL_TYPE_TIME:
        {
            //如果是次数卡
            //更新用户表的过期时间
            _xstprintf(tszSQLStatement, _X("UPDATE Authorize_User SET LeftTime = '%d' WHERE UserName = '%s'"), _ttxoi(lpszCardTime), lpszUserName);
        }
        break;
        case ENUM_VERIFICATION_MODULE_SERIAL_TYPE_CUSTOM:
        {
            XCHAR tszTime[128] = {};
            //更新用户表的过期时间
            _xstprintf(tszSQLStatement, _X("UPDATE Authorize_User SET LeftTime = '%s' WHERE UserName = '%s'"), tszTime, lpszUserName);
        }
        break;
        default:
            break;
        }
    }
    else
    {
        switch (en_AuthSerialType)
        {
        case ENUM_VERIFICATION_MODULE_SERIAL_TYPE_SECOND:
        {
            int nCardTime = _ttxoi(lpszCardTime);
            nCardTime += _ttxoi(lpszUserTime);              //我们把用户以前的时间也加上
            _xstprintf(tszSQLStatement, _X("UPDATE Authorize_User SET LeftTime = '%d' WHERE UserName = '%s'"), nCardTime, lpszUserName);                    //更新用户表的过期时间
        }
        break;
        case ENUM_VERIFICATION_MODULE_SERIAL_TYPE_DAY:
        {
			int nCardTime = _ttxoi(lpszCardTime);
			nCardTime += _ttxoi(lpszUserTime);              //我们把用户以前的时间也加上
            //更新用户表的过期时间
            _xstprintf(tszSQLStatement, _X("UPDATE Authorize_User SET LeftTime = '%d' WHERE UserName = '%s'"), nCardTime, lpszUserName);
        }
        break;
        case ENUM_VERIFICATION_MODULE_SERIAL_TYPE_TIME:
        {
            //如果是次数卡
            int nCardTime = _ttxoi(lpszCardTime);
            nCardTime += _ttxoi(lpszUserTime);              //我们把用户以前的时间也加上
            _xstprintf(tszSQLStatement, _X("UPDATE Authorize_User SET LeftTime = '%d' WHERE UserName = '%s'"), nCardTime, lpszUserName);                    //更新用户表的过期时间
        }
        break;
        case ENUM_VERIFICATION_MODULE_SERIAL_TYPE_CUSTOM:
        {
            //自定义卡,无法相加
            XCHAR tszTime[128] = {};
            //更新用户表的过期时间
            _xstprintf(tszSQLStatement, _X("UPDATE Authorize_User SET LeftTime = '%s' WHERE UserName = '%s'"), tszTime, lpszUserName);
        }
        break;
        default:
            break;
        }
    }
    //更新用户表的过期时间
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = true;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_MODULE_DATABASE_UPDATA;
        return false;
    }
    return true;
}