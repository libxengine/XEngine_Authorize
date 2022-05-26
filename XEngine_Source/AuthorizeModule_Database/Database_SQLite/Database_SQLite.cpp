#include "pch.h"
#include "Database_SQLite.h"
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
CDatabase_SQLite::CDatabase_SQLite()
{
    m_bChange = FALSE;
}
CDatabase_SQLite::~CDatabase_SQLite()
{

}
//////////////////////////////////////////////////////////////////////////
//                     公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Database_SQLite_Init
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
BOOL CDatabase_SQLite::Database_SQLite_Init(LPCTSTR lpszSQLFile, BOOL bIsChange /* = TRUE */)
{
    SQLPacket_IsErrorOccur = FALSE;
    BOOL bIsCreate = FALSE;
    //创建数据库
    if (!DataBase_SQLite_Create(lpszSQLFile))
    {
        //如果不是这个错误，说明创建数据库失败了
        if (ERROR_HELPCOMPONENTS_DATABASE_SQLITE_CREATE_ISEXIST != DataBase_GetLastError())
        {
            SQLPacket_IsErrorOccur = TRUE;
            SQLPacket_dwErrorCode = DataBase_GetLastError();
            return FALSE;
        }
    }
    else
    {
        bIsCreate = TRUE;
    }
    m_bChange = bIsChange;
    //打开数据库
    if (!DataBase_SQLite_Open(&xhData,lpszSQLFile))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    //如果是否创建数据库为真，那么我们需要创建数据库
    if (bIsCreate)
    {
        //如果创建成功了，说明需要创建表
        if (!DataBase_SQLite_Exec(xhData,_T("CREATE TABLE AuthReg_User(ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,UserName TEXT,Password TEXT,LeftTime TEXT,EmailAddr TEXT,HardCode TEXT,CardSerialType integer,PhoneNumber integer,IDCard integer,nUserLevel integer,CreateTime TEXT NOT NULL)")))
        {
            SQLPacket_IsErrorOccur = TRUE;
            SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_INIT_CREATETABLE;
            return FALSE;
        }
        if (!DataBase_SQLite_Exec(xhData,_T("CREATE TABLE AuthReg_Serial(ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,UserName TEXT,SerialNumber TEXT,MaxTime TEXT,CardSerialType integer,bIsUsed boolean,CreateTime TEXT NOT NULL)")))
        {
            SQLPacket_IsErrorOccur = TRUE;
            SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_INIT_CREATETABLE;
            return FALSE;
        }
        if (!DataBase_SQLite_Exec(xhData, _T("CREATE TABLE AuthReg_NetVer(ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,VerSerial TEXT NOT NULL,VerMode integer NOT NULL,TryTime integer NOT NULL,CreateTime TEXT NOT NULL)")))
        {
            SQLPacket_IsErrorOccur = TRUE;
            SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_INIT_CREATETABLE;
            return FALSE;
        }
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_Destroy
函数功能：销毁数据库服务
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
*********************************************************************/
BOOL CDatabase_SQLite::Database_SQLite_Destroy()
{
    SQLPacket_IsErrorOccur = FALSE;

    if (!DataBase_SQLite_Close(xhData))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_UserDelete
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
BOOL CDatabase_SQLite::Database_SQLite_UserDelete(LPCTSTR lpszUserName)
{
    SQLPacket_IsErrorOccur = FALSE;
    TCHAR tszSQLStatement[1024];    //SQL语句
    memset(tszSQLStatement,'\0',1024);
    _stprintf_s(tszSQLStatement,_T("delete from AuthReg_User where UserName = '%s'"),lpszUserName);
    //执行
    if (!DataBase_SQLite_Exec(xhData,tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_DELETE_EXEC;
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_UserRegister
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
BOOL CDatabase_SQLite::Database_SQLite_UserRegister(AUTHREG_USERTABLE* pSt_UserInfo)
{
    SQLPacket_IsErrorOccur = FALSE;
    TCHAR tszSQLStatement[1024];    //SQL语句
    memset(tszSQLStatement,'\0',1024);

    if (Database_SQLite_UserQuery(pSt_UserInfo->st_UserInfo.tszUserName))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_REGISERT_EXIST;
        return FALSE;
    }
    _stprintf_s(tszSQLStatement, _T("INSERT INTO AuthReg_User(UserName, Password, LeftTime, EmailAddr, HardCode, CardSerialType, PhoneNumber, IDCard, nUserLevel, CreateTime) values('%s','%s','%s','%s','%s','%d',%lld,%d,%lld,datetime('now', 'localtime'))"), pSt_UserInfo->st_UserInfo.tszUserName, pSt_UserInfo->st_UserInfo.tszUserPass, pSt_UserInfo->tszLeftTime, pSt_UserInfo->st_UserInfo.tszEMailAddr, pSt_UserInfo->tszHardCode, pSt_UserInfo->en_AuthRegSerialType, pSt_UserInfo->st_UserInfo.nPhoneNumber, pSt_UserInfo->st_UserInfo.nUserLevel, pSt_UserInfo->st_UserInfo.nIDNumber);
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_REGISERT_INSERT;
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_UserQuery
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
BOOL CDatabase_SQLite::Database_SQLite_UserQuery(LPCTSTR lpszUserName, AUTHREG_USERTABLE* pSt_UserInfo /* = NULL */)
{
    SQLPacket_IsErrorOccur = FALSE;
    TCHAR tszSQLStatement[1024];    //SQL语句
    char **ppszResult = NULL;
    int nRow = 0;
    int nColumn = 0;
    memset(tszSQLStatement,'\0',1024);

    _stprintf_s(tszSQLStatement,_T("select * from AuthReg_User where UserName = '%s'"),lpszUserName);
    if (!DataBase_SQLite_GetTable(xhData,tszSQLStatement,&ppszResult,&nRow,&nColumn))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_QUERY_GETTABLE;
        return FALSE;
    }
    if ((0 == nRow) || (0 == nColumn))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_QUERY_NOTUSER;
        return FALSE;
    }
    //如果是NULL，表示不想知道结果
    if (NULL != pSt_UserInfo)
    {
        memset(pSt_UserInfo, '\0', sizeof(AUTHREG_USERTABLE));
        //ID
        int nFliedValue = nColumn;
        //用户名
        nFliedValue++;
        _tcscpy(pSt_UserInfo->st_UserInfo.tszUserName, ppszResult[nFliedValue]);
        //密码
        nFliedValue++;
        _tcscpy(pSt_UserInfo->st_UserInfo.tszUserPass, ppszResult[nFliedValue]);
        //过期时间
        nFliedValue++;
        _tcscpy(pSt_UserInfo->tszLeftTime, ppszResult[nFliedValue]);
        //电子邮件
        nFliedValue++;
        _tcscpy(pSt_UserInfo->st_UserInfo.tszEMailAddr, ppszResult[nFliedValue]);
        //硬件码
        nFliedValue++;
        _tcscpy(pSt_UserInfo->tszHardCode, ppszResult[nFliedValue]);
        //充值卡类型
        nFliedValue++;
        pSt_UserInfo->en_AuthRegSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)_ttoi(ppszResult[nFliedValue]);
        //QQ号
        nFliedValue++;
        pSt_UserInfo->st_UserInfo.nPhoneNumber = _tcstoi64(ppszResult[nFliedValue], NULL, 10);
        //身份证ID
        nFliedValue++;
        pSt_UserInfo->st_UserInfo.nIDNumber = _tcstoi64(ppszResult[nFliedValue], NULL, 10);
		//用户级别 -1表示封禁
		nFliedValue++;
		pSt_UserInfo->st_UserInfo.nUserLevel = _tcstoi64(ppszResult[nFliedValue], NULL, 10);
		//注册日期
		nFliedValue++;
		_tcscpy(pSt_UserInfo->st_UserInfo.tszCreateTime, ppszResult[nFliedValue]);
    }

    DataBase_SQLite_FreeTable(ppszResult);
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_UserPay
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
BOOL CDatabase_SQLite::Database_SQLite_UserPay(LPCTSTR lpszUserName,LPCTSTR lpszSerialName)
{
    SQLPacket_IsErrorOccur = FALSE;

    AUTHREG_SERIALTABLE st_SerialTable;
    AUTHREG_USERTABLE st_UserTable;
    TCHAR tszSQLStatement[1024];    //SQL语句

    memset(tszSQLStatement,'\0',1024);
    memset(&st_SerialTable,'\0',sizeof(st_SerialTable));
    memset(&st_UserTable,'\0',sizeof(st_UserTable));
    //参数检查
    if ((NULL == lpszUserName) || (NULL == lpszSerialName))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAY_PARAMENT;
        return FALSE;
    }
    //查询充值卡是否存在
    if (!Database_SQLite_SerialQuery(lpszSerialName,&st_SerialTable))
    {
        return FALSE;
    }
    //充值卡是否被使用
    if (st_SerialTable.bIsUsed)
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAY_ISUSED;
        return FALSE;
    }
    //查询用户信息
    if (!Database_SQLite_UserQuery(lpszUserName,&st_UserTable))
    {
        return FALSE;
    }
    //分析插入方式
    switch (st_SerialTable.en_AuthRegSerialType)
    {
    case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_MINUTE:
        if (!Database_SQLite_UserPayTime(lpszUserName, st_UserTable.tszLeftTime, st_SerialTable.tszMaxTime, st_SerialTable.en_AuthRegSerialType, st_UserTable.en_AuthRegSerialType))
        {
            return FALSE;
        }
        break;
    case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_DAY:
        if (!Database_SQLite_UserPayTime(lpszUserName, st_UserTable.tszLeftTime, st_SerialTable.tszMaxTime, st_SerialTable.en_AuthRegSerialType, st_UserTable.en_AuthRegSerialType))
        {
            return FALSE;
        }
        break;
    case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME:
        if (!Database_SQLite_UserPayTime(lpszUserName, st_UserTable.tszLeftTime, st_SerialTable.tszMaxTime, st_SerialTable.en_AuthRegSerialType, st_UserTable.en_AuthRegSerialType))
        {
            return FALSE;
        }
        break;
    case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_CUSTOM:
        if (!Database_SQLite_UserPayTime(lpszUserName, st_UserTable.tszLeftTime, st_SerialTable.tszMaxTime, st_SerialTable.en_AuthRegSerialType, st_UserTable.en_AuthRegSerialType))
        {
            return FALSE;
        }
        break;
    default:
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAY_NOTSUPPORT;
        return FALSE;
    }
    _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_Serial SET UserName = '%s',bIsUsed = '1' WHERE SerialNumber = '%s'"), lpszUserName, lpszSerialName);
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAY_UPDATAUSEDNAME;
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_UserLeave
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
BOOL CDatabase_SQLite::Database_SQLite_UserLeave(AUTHREG_PROTOCOL_TIME* pSt_TimeProtocol)
{
    SQLPacket_IsErrorOccur = FALSE;

    TCHAR tszSQLStatement[1024];       //SQL语句
    memset(tszSQLStatement, '\0', 1024);

    if ((ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_DAY == pSt_TimeProtocol->enSerialType) || (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_CUSTOM == pSt_TimeProtocol->enSerialType))
    {
        //天数卡只有剩余时间没有的时候才需要做处理
        if (pSt_TimeProtocol->nTimeLeft <= 0)
        {
            _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_User SET LeftTime = '0' WHERE UserName = '%s'"), pSt_TimeProtocol->tszUserName);
        }
    }
    if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_MINUTE == pSt_TimeProtocol->enSerialType)
    {
        //分钟卡必须要有在线时间才能计算
        if (pSt_TimeProtocol->nTimeLeft <= 0)
        {
            pSt_TimeProtocol->nTimeLeft = 0;
        }
        _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_User SET LeftTime = '%lld' WHERE UserName = '%s'"), pSt_TimeProtocol->nTimeLeft, pSt_TimeProtocol->tszUserName);
    }
    else if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == pSt_TimeProtocol->enSerialType)
    {
        //次数卡不需要在线时间,直接减去一次就可以了
        _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_User SET LeftTime = '%lld' WHERE UserName = '%s'"), _ttoi64(pSt_TimeProtocol->tszLeftTime) - 1, pSt_TimeProtocol->tszUserName);
    }
    else
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_LEAVE_UNKNOWTYPE;
        return FALSE;
    }
    //更新用户剩余时间
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_LEAVE_UPDATA;
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_UserSet
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
BOOL CDatabase_SQLite::Database_SQLite_UserSet(AUTHREG_USERTABLE* pSt_UserTable)
{
    SQLPacket_IsErrorOccur = FALSE;

    TCHAR tszSQLStatement[1024];       //SQL语句
    memset(tszSQLStatement, '\0', 1024);

    _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_User SET Password = '%s',EmailAddr = '%s',PhoneNumber = '%lld',IDCard = '%lld',LeftTime = '%s',CardSerialType = '%d',nUserLevel = '%d' WHERE UserName = '%s'"), pSt_UserTable->st_UserInfo.tszUserPass, pSt_UserTable->st_UserInfo.tszEMailAddr, pSt_UserTable->st_UserInfo.nPhoneNumber, pSt_UserTable->st_UserInfo.nIDNumber, pSt_UserTable->tszLeftTime, pSt_UserTable->en_AuthRegSerialType, pSt_UserTable->st_UserInfo.nUserLevel, pSt_UserTable->st_UserInfo.tszUserName);
    //更新用户剩余时间
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_SerialInsert
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
BOOL CDatabase_SQLite::Database_SQLite_SerialInsert(LPCTSTR lpszSerialNumber)
{
    SQLPacket_IsErrorOccur = FALSE;

    TCHAR tszSQLStatement[1024];    //SQL语句
    memset(tszSQLStatement,'\0',1024);

    if (Database_SQLite_SerialQuery(lpszSerialNumber))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_SERIALINSERT_EXIST;
        return FALSE;
    }
    ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enAuthSerialType;
    XENGINE_LIBTIMER st_AuthTimer;
    memset(&st_AuthTimer,'\0',sizeof(st_AuthTimer));

    if (!Authorize_Serial_GetType(lpszSerialNumber, &enAuthSerialType, &st_AuthTimer))
    {
        return FALSE;
    }
    if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_MINUTE == enAuthSerialType)
    {
        _stprintf_s(tszSQLStatement, _T("INSERT INTO AuthReg_Serial values(NULL,'NOT','%s','%d','%d',0,datetime('now', 'localtime'))"), lpszSerialNumber, st_AuthTimer.wMinute, enAuthSerialType);
    }
    else if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_DAY == enAuthSerialType)
    {
        _stprintf_s(tszSQLStatement, _T("INSERT INTO AuthReg_Serial values(NULL,'NOT','%s','%d','%d',0,datetime('now', 'localtime'))"), lpszSerialNumber, st_AuthTimer.wDay, enAuthSerialType);
    }
    else if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == enAuthSerialType)
    {
        _stprintf_s(tszSQLStatement, _T("INSERT INTO AuthReg_Serial values(NULL,'NOT','%s','%d','%d',0,datetime('now', 'localtime'))"), lpszSerialNumber, st_AuthTimer.wFlags, enAuthSerialType);
    }
    else if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_CUSTOM == enAuthSerialType)
    {
        TCHAR tszLeftTime[MAX_PATH];
        memset(tszLeftTime, '\0', MAX_PATH);
        _stprintf_s(tszLeftTime, _T("%04d-%02d-%02d %02d:%02d:%02d"), st_AuthTimer.wYear, st_AuthTimer.wMonth, st_AuthTimer.wDay, st_AuthTimer.wHour, st_AuthTimer.wMinute, st_AuthTimer.wSecond);
        _stprintf_s(tszSQLStatement, _T("INSERT INTO AuthReg_Serial values(NULL,'NOT','%s','%s','%d',0,datetime('now', 'localtime'))"), lpszSerialNumber, tszLeftTime, enAuthSerialType);
    }
    else
    {
        _stprintf_s(tszSQLStatement, _T("INSERT INTO AuthReg_Serial values(NULL,'NOT','%s',0,'%d',0,datetime('now', 'localtime'))"), lpszSerialNumber, enAuthSerialType);
    }

    if (!DataBase_SQLite_Exec(xhData,tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_SERIALINSERT_ISFAILED;
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_SerialDelete
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
BOOL CDatabase_SQLite::Database_SQLite_SerialDelete(LPCTSTR lpszSerialNumber)
{
    SQLPacket_IsErrorOccur = FALSE;

    TCHAR tszSQLStatement[1024];    //SQL语句
    memset(tszSQLStatement,'\0',1024);
    _stprintf_s(tszSQLStatement,_T("delete from AuthReg_Serial where SerialNumber = '%s'"),lpszSerialNumber);
    //执行
    if (!DataBase_SQLite_Exec(xhData,tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_SERIALDEL_ISFAILED;
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_SerialQuery
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
BOOL CDatabase_SQLite::Database_SQLite_SerialQuery(LPCTSTR lpszSerialNumber,LPAUTHREG_SERIALTABLE pSt_SerialTable /* = NULL */)
{
    SQLPacket_IsErrorOccur = FALSE;
    TCHAR tszSQLStatement[1024];    //SQL语句
    char **ppszResult = NULL;
    int nRow = 0;
    int nColumn = 0;
    memset(tszSQLStatement,'\0',1024);

    _stprintf_s(tszSQLStatement,_T("select * from AuthReg_Serial where SerialNumber = '%s'"),lpszSerialNumber);
    if (!DataBase_SQLite_GetTable(xhData,tszSQLStatement,&ppszResult,&nRow,&nColumn))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_QUERY_GETTABLE;
        return FALSE;
    }
    if ((0 == nRow) || (0 == nColumn))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_QUERY_SERIAL;
        return FALSE;
    }
    if (NULL != pSt_SerialTable)
    {
        memset(pSt_SerialTable, '\0', sizeof(AUTHREG_SERIALTABLE));
        //ID
        int nFliedValue = nColumn;
        //使用者
        nFliedValue++;
        _tcscpy(pSt_SerialTable->tszUserName,ppszResult[nFliedValue]);
        //序列号
        nFliedValue++;
        _tcscpy(pSt_SerialTable->tszSerialNumber,ppszResult[nFliedValue]);
        //超时时间
        nFliedValue++;
        _tcscpy(pSt_SerialTable->tszMaxTime,ppszResult[nFliedValue]);
        //序列卡类型
        nFliedValue++;
        pSt_SerialTable->en_AuthRegSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)_ttoi(ppszResult[nFliedValue]);
        //是否已经使用
        nFliedValue++;
        pSt_SerialTable->bIsUsed = _ttoi(ppszResult[nFliedValue]);
		//超时时间
		nFliedValue++;
		_tcscpy(pSt_SerialTable->tszCreateTime, ppszResult[nFliedValue]);
    }

    DataBase_SQLite_FreeTable(ppszResult);
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_SerialQueryAll
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
BOOL CDatabase_SQLite::Database_SQLite_SerialQueryAll(AUTHREG_SERIALTABLE ***pppSt_SerialTable,int *pInt_ListCount)
{
    SQLPacket_IsErrorOccur = FALSE;
    
	int nRow = 0;
	int nColumn = 0;
    char** ppszResult = NULL;
    TCHAR tszSQLStatement[1024];    //SQL语句
   
    memset(tszSQLStatement,'\0',1024);
    
    _stprintf_s(tszSQLStatement,_T("select * from AuthReg_Serial"));
    if (!DataBase_SQLite_GetTable(xhData,tszSQLStatement,&ppszResult,&nRow,&nColumn))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_QUERYALL_GETTABLE;
        return FALSE;
    }
    if ((0 == nRow) || (0 == nColumn))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_QUERYALL_NONE;
        return FALSE;
    }
    BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_SerialTable, nRow, sizeof(AUTHREG_SERIALTABLE));

    *pInt_ListCount = nRow;
    int nFliedValue = nColumn;
    for (int i = 0;i < nRow;i++)
    {
        //ID
        nFliedValue++;
        //使用者
        _tcscpy((*pppSt_SerialTable)[i]->tszUserName,ppszResult[nFliedValue]);
        nFliedValue++;
        //序列号
        _tcscpy((*pppSt_SerialTable)[i]->tszSerialNumber,ppszResult[nFliedValue]);
        nFliedValue++;
        //超时时间
        _tcscpy((*pppSt_SerialTable)[i]->tszMaxTime,ppszResult[nFliedValue]);
        nFliedValue++;
        //序列卡类型
        (*pppSt_SerialTable)[i]->en_AuthRegSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)_ttoi(ppszResult[nFliedValue]);
        nFliedValue++;
        //是否已经使用
        (*pppSt_SerialTable)[i]->bIsUsed = _ttoi(ppszResult[nFliedValue]);
        nFliedValue++;
		//创建时间
        _tcscpy((*pppSt_SerialTable)[i]->tszCreateTime, ppszResult[nFliedValue]);
		nFliedValue++;
    }
    DataBase_SQLite_FreeTable(ppszResult);
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_TryInsert
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
BOOL CDatabase_SQLite::Database_SQLite_TryInsert(AUTHREG_NETVER* pSt_AuthVer)
{
    SQLPacket_IsErrorOccur = FALSE;

    if (NULL == pSt_AuthVer)
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_TRYINSERT_PARAMENT;
        return FALSE;
    }
    TCHAR tszSQLStatement[1024];
    AUTHREG_NETVER st_AuthVer;

    memset(tszSQLStatement, '\0', 1024);
    memset(&st_AuthVer, '\0', sizeof(AUTHREG_NETVER));

    //验证是否存在
    _tcscpy(st_AuthVer.tszVerSerial, pSt_AuthVer->tszVerSerial);
    if (Database_SQLite_TryQuery(&st_AuthVer))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_TRYINSERT_EXIST;
        return FALSE;
    }
    //插入数据库
    _stprintf_s(tszSQLStatement, _T("INSERT INTO AuthReg_NetVer(VerSerial,VerMode,TryTime,CreateTime) VALUES('%s',%d,%d,datetime('now', 'localtime'))"), pSt_AuthVer->tszVerSerial, pSt_AuthVer->enVerMode, pSt_AuthVer->nTryTime);
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_TryQuery
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
BOOL CDatabase_SQLite::Database_SQLite_TryQuery(AUTHREG_NETVER* pSt_AuthVer)
{
    SQLPacket_IsErrorOccur = FALSE;

    if (NULL == pSt_AuthVer)
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_TRYQUERY_PARAMENT;
        return FALSE;
    }
    int nRow = 0;
    int nColumn = 0;
    CHAR** ppszResult = NULL;
    TCHAR tszSQLStatement[1024];    //SQL语句

    memset(tszSQLStatement, '\0', 1024);
    _stprintf_s(tszSQLStatement, _T("SELECT * FROM AuthReg_NetVer WHERE VerSerial = '%s'"), pSt_AuthVer->tszVerSerial);

    if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    if ((0 == nRow) || (0 == nColumn))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_TRYQUERY_NONE;
        return FALSE;
    }
    //ID
    int nFliedValue = nColumn;
    nFliedValue++;
    //序列号
    nFliedValue++;
    //试用类型
    pSt_AuthVer->enVerMode = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)_ttoi(ppszResult[nFliedValue]);
    nFliedValue++;
    //试用时间
    pSt_AuthVer->nTryTime = _ttoi(ppszResult[nFliedValue]);
    nFliedValue++;
    //注册时间
    _tcscpy(pSt_AuthVer->tszVerData, ppszResult[nFliedValue]);
    DataBase_SQLite_FreeTable(ppszResult);
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_TryDelete
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
BOOL CDatabase_SQLite::Database_SQLite_TryDelete(LPCTSTR lpszSerial)
{
    SQLPacket_IsErrorOccur = FALSE;

    if (NULL == lpszSerial)
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_TRYDELETE_PARAMENT;
        return FALSE;
    }
    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', 1024);

    _stprintf_s(tszSQLStatement, _T("DELETE * FROM AuthReg_NetVer WHERE VerSerial = '%s'"), lpszSerial);

    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_TryClear
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
BOOL CDatabase_SQLite::Database_SQLite_TryClear(int nThanValue, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enVerMode /* = ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW */)
{
    SQLPacket_IsErrorOccur = FALSE;

    int nRow = 0;
    int nColumn = 0;
    CHAR** ppszResult = NULL;
    TCHAR tszSQLStatement[1024];    //SQL语句

    memset(tszSQLStatement, '\0', 1024);
    _stprintf_s(tszSQLStatement, _T("SELECT * FROM AuthReg_NetVer"));

    if (!DataBase_SQLite_GetTable(xhData, tszSQLStatement, &ppszResult, &nRow, &nColumn))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    int nFliedValue = nColumn;
    list<AUTHREG_NETVER> stl_ListVer;
    //轮训所有内容
    for (int i = 0; i < nRow; i++)
    {
        AUTHREG_NETVER st_AuthVer;
        memset(&st_AuthVer, '\0', sizeof(AUTHREG_NETVER));
        //ID
        nFliedValue++;
        //序列号
        _tcscpy(st_AuthVer.tszVerSerial, ppszResult[nFliedValue]);
        nFliedValue++;
        //模式
        st_AuthVer.enVerMode = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)_ttoi(ppszResult[nFliedValue]);
        nFliedValue++;
        //测试时间
        st_AuthVer.nTryTime = _ttoi(ppszResult[nFliedValue]);
        nFliedValue++;
        //注册时间
        _tcscpy(st_AuthVer.tszVerData, ppszResult[nFliedValue]);

        stl_ListVer.push_back(st_AuthVer);
    }
    DataBase_SQLite_FreeTable(ppszResult);
    //清理
    list<AUTHREG_NETVER>::const_iterator stl_ListIterator = stl_ListVer.begin();
    for (; stl_ListIterator != stl_ListVer.end(); stl_ListIterator++)
    {
        //判断是不是不关心注册的模式直接清理
        if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == enVerMode)
        {
            if (nThanValue > stl_ListIterator->nTryTime)
            {
                memset(tszSQLStatement, '\0', 1024);
                _stprintf_s(tszSQLStatement, _T("DELETE * FROM AuthReg_NetVer WHERE VerSerial = '%s'"), stl_ListIterator->tszVerSerial);

                if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
                {
                    SQLPacket_IsErrorOccur = TRUE;
                    SQLPacket_dwErrorCode = DataBase_GetLastError();
                    return FALSE;
                }
            }
        }
        else
        {
            if (enVerMode == stl_ListIterator->enVerMode)
            {
                memset(tszSQLStatement, '\0', 1024);
                _stprintf_s(tszSQLStatement, _T("DELETE * FROM AuthReg_NetVer WHERE VerSerial = '%s'"), stl_ListIterator->tszVerSerial);

                if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
                {
                    SQLPacket_IsErrorOccur = TRUE;
                    SQLPacket_dwErrorCode = DataBase_GetLastError();
                    return FALSE;
                }
            }
        }
    }
    stl_ListVer.clear();
    return TRUE;
}
/********************************************************************
函数名称：Database_SQLite_TrySet
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
BOOL CDatabase_SQLite::Database_SQLite_TrySet(AUTHREG_NETVER* pSt_AuthVer)
{
    SQLPacket_IsErrorOccur = FALSE;

    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));

    _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_NetVer SET VerMode = '%d',TryTime = '%d',CreateTime = '%s' WHERE VerSerial = '%s'"), pSt_AuthVer->enVerMode, pSt_AuthVer->nTryTime, pSt_AuthVer->tszVerData, pSt_AuthVer->tszVerSerial);
    //更新用户表
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                       保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Database_SQLite_UserPayTime
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
BOOL CDatabase_SQLite::Database_SQLite_UserPayTime(LPCTSTR lpszUserName, LPCTSTR lpszUserTime, LPCTSTR lpszCardTime, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthSerialType, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthUserType)
{
    SQLPacket_IsErrorOccur = FALSE;

    TCHAR tszSQLStatement[1024];
    TCHAR tszTimer[128];

    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    memset(tszTimer, '\0', sizeof(tszTimer));
    //判断和以前的充值卡是否匹配。
    if (en_AuthSerialType != en_AuthUserType)
    {
        //如果不等于,需要重写
        if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW != en_AuthUserType)
        {
            //判断是否允许改写。
            if (!m_bChange)
            {
                SQLPacket_IsErrorOccur = TRUE;
                SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAYTIME_NOTMATCH;
                return FALSE;
            }
        }
        //更新用户表的充值卡类型
        memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
        _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_User SET CardSerialType = '%d' WHERE UserName = '%s'"), en_AuthSerialType, lpszUserName);
        //执行用户表更新，因为序列卡类型被改变，所以需要更新。
        if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
        {
            SQLPacket_IsErrorOccur = TRUE;
            SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAYTIME_UPDATATYPE;
            return FALSE;
        }
        //处理卡类型
        switch (en_AuthSerialType)
        {
        case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_MINUTE:
        {
            //如果是分钟卡
            //如果当前的充值卡类型不匹配，那么他以前的充值内容全部都会被删除！
            _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_User SET LeftTime = '%d' WHERE UserName = '%s'"), _ttoi(lpszCardTime), lpszUserName);      //更新用户表的过期时间
        }
        break;
        case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_DAY:
        {
            //如果是天数卡
            XENGINE_LIBTIMER st_StartTimer;
            XENGINE_LIBTIMER st_EndTimer;

            memset(&st_StartTimer, '\0', sizeof(XENGINE_LIBTIMER));
            memset(&st_EndTimer, '\0', sizeof(XENGINE_LIBTIMER));

            BaseLib_OperatorTime_GetSysTime(&st_StartTimer);

            st_EndTimer.wDay += _ttoi(lpszCardTime);
            BaseLib_OperatorTimeSpan_CalForStu(&st_StartTimer, &st_EndTimer);
            //格式化时间，到超时的时间
            _stprintf_s(tszTimer, _T("%04d-%02d-%02d %02d:%02d:%02d"), st_EndTimer.wYear, st_EndTimer.wMonth, st_EndTimer.wDay, st_EndTimer.wHour, st_EndTimer.wMinute, st_EndTimer.wSecond);
            //更新用户表的过期时间
            _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_User SET LeftTime = '%s' WHERE UserName = '%s'"), tszTimer, lpszUserName);      //更新用户表的过期时间
        }
        break;
        case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME:
        {
            //如果是次数卡
            //更新用户表的过期时间
            _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_User SET LeftTime = '%d' WHERE UserName = '%s'"), _ttoi(lpszCardTime), lpszUserName);
        }
        break;
        case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_CUSTOM:
        {
            TCHAR tszTime[128];
            XENGINE_LIBTIMER st_AuthTime;
            ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_GeneraterSerialType;

            memset(tszTime, '\0', sizeof(tszTime));
            memset(&st_AuthTime, '\0', sizeof(XENGINE_LIBTIMER));
            //获取重置卡类型和时间
            if (!Authorize_Serial_GetType(lpszCardTime, &en_GeneraterSerialType, &st_AuthTime))
            {
                return FALSE;
            }
            _stprintf_s(tszTime, _T("%04d-%02d-%02d %02d:%02d:%02d"), st_AuthTime.wYear, st_AuthTime.wMonth, st_AuthTime.wDay, st_AuthTime.wHour, st_AuthTime.wMinute, st_AuthTime.wSecond);
            //更新用户表的过期时间
            _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_User SET LeftTime = '%s' WHERE UserName = '%s'"), tszTimer, lpszUserName);
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
        case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_MINUTE:
        {
            int nCardTime = _ttoi(lpszCardTime);
            nCardTime += _ttoi(lpszUserTime);              //我们把用户以前的时间也加上
            _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_User SET LeftTime = '%d' WHERE UserName = '%s'"), nCardTime, lpszUserName);                    //更新用户表的过期时间
        }
        break;
        case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_DAY:
        {
            XENGINE_LIBTIMER st_EndTimer;
            XENGINE_LIBTIMER st_AuthTimer;

            memset(&st_EndTimer, '\0', sizeof(XENGINE_LIBTIMER));
            memset(&st_AuthTimer, '\0', sizeof(st_AuthTimer));
            //获取用户拥有的时间
            if (6 != _stscanf_s(lpszUserTime, _T("%04d-%02d-%02d %02d:%02d:%02d"), &st_AuthTimer.wYear, &st_AuthTimer.wMonth, &st_AuthTimer.wDay, &st_AuthTimer.wHour, &st_AuthTimer.wMinute, &st_AuthTimer.wSecond))
            {
                SQLPacket_IsErrorOccur = TRUE;
                SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAYTIME_GETTIME;
                return FALSE;
            }
            st_EndTimer.wDay += _ttoi(lpszCardTime);

            BaseLib_OperatorTimeSpan_CalForStu(&st_AuthTimer, &st_EndTimer);
            //格式化时间，到超时的时间
            _stprintf_s(tszTimer, _T("%04d-%02d-%02d %02d:%02d:%02d"), st_EndTimer.wYear, st_EndTimer.wMonth, st_EndTimer.wDay, st_EndTimer.wHour, st_EndTimer.wMinute, st_EndTimer.wSecond);
            //更新用户表的过期时间
            _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_User SET LeftTime = '%s' WHERE UserName = '%s'"), tszTimer, lpszUserName);
        }
        break;
        case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME:
        {
            //如果是次数卡
            int nCardTime = _ttoi(lpszCardTime);
            nCardTime += _ttoi(lpszUserTime);              //我们把用户以前的时间也加上
            _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_User SET LeftTime = '%d' WHERE UserName = '%s'"), nCardTime, lpszUserName);                    //更新用户表的过期时间
        }
        break;
        case ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_CUSTOM:
        {
            //自定义卡,无法相加
            TCHAR tszTime[128];
            XENGINE_LIBTIMER st_AuthTime;
            ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_GeneraterSerialType;

            memset(tszTime, '\0', sizeof(tszTime));
            memset(&st_AuthTime, '\0', sizeof(XENGINE_LIBTIMER));
            //获取重置卡类型和时间
            if (!Authorize_Serial_GetType(lpszCardTime, &en_GeneraterSerialType, &st_AuthTime))
            {
                return FALSE;
            }
            _stprintf_s(tszTime, _T("%04d-%02d-%02d %02d:%02d:%02d"), st_AuthTime.wYear, st_AuthTime.wMonth, st_AuthTime.wDay, st_AuthTime.wHour, st_AuthTime.wMinute, st_AuthTime.wSecond);
            //更新用户表的过期时间
            _stprintf_s(tszSQLStatement, _T("UPDATE AuthReg_User SET LeftTime = '%s' WHERE UserName = '%s'"), tszTimer, lpszUserName);
        }
        break;
        default:
            break;
        }
    }
    //更新用户表的过期时间
    if (!DataBase_SQLite_Exec(xhData, tszSQLStatement))
    {
        SQLPacket_IsErrorOccur = TRUE;
        SQLPacket_dwErrorCode = ERROR_AUTHORIZE_COMPONENTS_SQLPACKET_PAYTIME_UPDATA;
        return FALSE;
    }
    return TRUE;
}