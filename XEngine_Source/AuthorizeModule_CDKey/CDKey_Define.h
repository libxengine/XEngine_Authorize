#pragma once
/********************************************************************
//    Created:     2024/07/08  17:30:14
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey\Authorize_Define.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey
//    File Base:   Authorize_Define
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     CDKEY授权验证服务
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                            导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Authorize_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                           序列卡导出函数                             */
/************************************************************************/
/********************************************************************
函数名称：Authorize_Serial_Creator
函数功能：开始生成指定的序列号
 参数.一：ppptszSerialNumber
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出序列卡列表,根据nCardNumber 来确定列表个数
 参数.二：lpszUserHeader
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：自定义头，长度为6，填充5 比如 XHEAD
 参数.三：nCardNumber
  In/Out：In
  类型：整数型
  可空：N
  意思：要生成多少张卡，生成多了，此函数会阻塞程序，那么请放置线程中运行
 参数.四：nFieldNumber
  In/Out：In
  类型：整数型
  可空：N
  意思：卡的段数，AAAAA-BBBBB-CCCCC-DDDDD-EEEEE,不包含三段自定义头,总字段:3 + nFieldNumber
 参数.五：pSt_CustomTimer
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：如果第五个参数为自定义日期，那么这个参数必须赋值。如果第五个参数为天数卡，这个结构体的nDay 必须赋值，表明拥有几天。
		如果是分钟卡,那么这个值的nMinute必须有值,表示拥有的分钟.如果是次数卡,那么这个值的wFlags必须有值,表示拥有的次数,其他类型可以为NULL
 参数.六：enSerialType
  In/Out：In
  类型：枚举型
  可空：Y
  意思：要生成的卡类型
返回值
  类型：逻辑型
  意思：是否生成成功
备注：输入的时间不能超过99999,多天卡现在由用户直接使用pSt_CustomTimer参数指定,不在内部指定了
*********************************************************************/
extern "C" bool Authorize_Serial_Creator(XCHAR * **ppptszSerialNumber, LPCXSTR lpszUserHeader, int nCardNumber, int nFieldNumber, XENGINE_LIBTIME * pSt_CustomTimer, ENUM_AUTHORIZE_MODULE_SERIAL_TYPE enSerialType = ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_DAY);
/********************************************************************
函数名称：Authorize_Serial_Create
函数功能：创建自定义无类型序列号
 参数.一：ppptszSerialNumber
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出序列卡列表,根据nCardNumber 来确定列表个数
 参数.二：lpszUserHeader
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：自定义头，长度为6，填充5 比如 XHEAD
 参数.三：nCardNumber
  In/Out：In
  类型：整数型
  可空：N
  意思：要生成多少张卡，生成多了，此函数会阻塞程序，那么请放置线程中运行
 参数.四：nFieldNumber
  In/Out：In
  类型：整数型
  可空：N
  意思：卡的段数，AAAAA-BBBBB-CCCCC-DDDDD-EEEEE
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Authorize_Serial_Create(XCHAR*** ppptszSerialNumber, LPCXSTR lpszUserHeader, int nCardNumber, int nFieldNumber);
/********************************************************************
函数名称：Authorize_Serial_GetType
函数功能：获取一个序列号的类型
 参数.一：lpszSerialNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要获取的缓冲区
 参数.二：penSerialType
  In/Out：Out
  类型：枚举型
  可空：Y
  意思：导出此卡的类型
 参数.三：pSt_CustomTimer
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：如果不是自定义类型或者天数卡,或者次数卡，这个参数将不起作用
返回值
  类型：逻辑型
  意思：是否获取成功
备注：
*********************************************************************/
extern "C" bool Authorize_Serial_GetType(LPCXSTR lpszSerialNumber, ENUM_AUTHORIZE_MODULE_SERIAL_TYPE* penSerialType, XENGINE_LIBTIME* pSt_CustomTimer = NULL);
/************************************************************************/
/*                           本地授权导出函数                           */
/************************************************************************/
/********************************************************************
函数名称：Authorize_CDKey_WriteKey
函数功能：写一个CDKey文件
 参数.一：lpszFileKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要操作的文件路径
 参数.二：pSt_AuthLocal
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要写入的文件信息
返回值
  类型：逻辑型
  意思：是否成功
备注：写入读取必须是明文,建议你加密处理CDKEY,通过OPENSSL模块,来加解密,在读写
*********************************************************************/
extern "C" bool Authorize_CDKey_WriteKey(LPCXSTR lpszFileKey, XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal);
/********************************************************************
函数名称：Authorize_CDKey_ReadKey
函数功能：读一个数据文件
 参数.一：lpszFileKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要操作的文件路径
 参数.二：pSt_AuthLocal
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：导出获取到的文件信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Authorize_CDKey_ReadKey(LPCXSTR lpszFileKey, XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal);
/********************************************************************
函数名称：Authorize_CDKey_WriteMemory
函数功能：写配置信息到内存
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：写到的内存
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：写到的内存大小
 参数.三：pSt_AuthLocal
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要写的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Authorize_CDKey_WriteMemory(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal);
/********************************************************************
函数名称：Authorize_CDKey_ReadMemory
函数功能：内存配置文件读取
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要读取配置的内存
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入读取内存大小
 参数.三：pSt_AuthLocal
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出读取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Authorize_CDKey_ReadMemory(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal);
/********************************************************************
函数名称：Authorize_CDKey_BuildKeyTime
函数功能：构造注册的时间结构体信息
 参数.一：pSt_AuthLocal
  In/Out：In
  类型：结构体指针
  可空：N
  意思：要构造的结构体
 参数.二：nDayTimer
  In/Out：In
  类型：整数型
  可空：Y
  意思：可用时间或者次数.非自定义时间需要设置此值
 参数.三：pSt_DayTimer
  In/Out：In
  类型：结构体指针
  可空：Y
  意思：如果是自定义时间,这个参数需要设置,其他类型请设置参数二
返回值
  类型：逻辑型
  意思：是否构造成功
备注：此函数会修改st_AuthRegInfo的时间信息成员
*********************************************************************/
extern "C" bool Authorize_CDKey_BuildKeyTime(XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal, __int64x nDayTimer = 0, XENGINE_LIBTIME* pSt_DayTimer = NULL);
/********************************************************************
函数名称：Authorize_CDKey_UserRegister
函数功能：用户注册CDKEY函数
 参数.一：pSt_AuthLocal
  In/Out：In
  类型：结构体指针
  可空：N
  意思：要构造的结构体
 参数.二：lpszSerialStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入注册验证的序列号
返回值
  类型：逻辑型
  意思：是否构造成功
备注：此函数会修改st_AuthRegInfo的时间信息成员,必须重写CDKEY
*********************************************************************/
extern "C" bool Authorize_CDKey_UserRegister(XENGINE_AUTHORIZE_LOCAL* pSt_AuthLocal, LPCXSTR lpszSerialStr);
/********************************************************************
函数名称：Authorize_CDKey_GetLeftTimer
函数功能：验证CDKey
 参数.一：pSt_AuthLocal
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入Authorize_CDKey_ReadKey获取到的值
 参数.二：lpszSerialNumber
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：如果CDKEY带序列号,可以输入序列号进行验证(优先序列号验证,失败进行常规CDKEY验证)
返回值
  类型：逻辑型
  意思：是否成功
备注：无限制版本不做验证
	  其他验证nHasTime将被设置还拥有时间
	  此函数在程序启动的时候必须调用,可以设置更新CDKEY信息也可以验证CDKEY
*********************************************************************/
extern "C" bool Authorize_CDKey_GetLeftTimer(XENGINE_AUTHORIZE_LOCAL * pSt_AuthLocal, LPCXSTR lpszSerialNumber = NULL);
/********************************************************************
函数名称：Authorize_CDKey_WriteTime
函数功能：记录一次执行时间
 参数.一：lpszFileKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的文件
 参数.二：nCount
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入最大允许记录个数,0不限制
返回值
  类型：逻辑型
  意思：是否成功
备注：记录次数越多,文件越大.读取需要的内存就越多
*********************************************************************/
extern "C" bool Authorize_CDKey_WriteTime(LPCXSTR lpszFileKey, int nCount = 0);
/********************************************************************
函数名称：Authorize_CDKey_ReadTime
函数功能：读取记录的时间列表信息
 参数.一：lpszFileKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要读取的文件
 参数.二：ppptszTimeList
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出时间信息列表
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Authorize_CDKey_ReadTime(LPCXSTR lpszFileKey, XCHAR*** ppptszTimeList, int* pInt_ListCount);