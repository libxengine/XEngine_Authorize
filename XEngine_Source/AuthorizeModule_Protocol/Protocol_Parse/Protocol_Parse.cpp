#include "pch.h"
#include "Protocol_Parse.h"
/********************************************************************
//    Created:     2021/07/14  14:27:27
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Parse\Protocol_Parse.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Parse
//    File Base:   Protocol_Parse
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议解析类
//    History:
*********************************************************************/
CProtocol_Parse::CProtocol_Parse()
{

}
CProtocol_Parse::~CProtocol_Parse()
{

}
//////////////////////////////////////////////////////////////////////////
//                              公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Protocol_Parse_WSHdr
函数功能：获取WEBSOCKET协议信息头
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_ProtocolHdr
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析好的协议头
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_WSHdr(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolHdr))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	pSt_ProtocolHdr->wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	pSt_ProtocolHdr->wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	pSt_ProtocolHdr->unOperatorType = st_JsonRoot["unOperatorType"].asInt();
	pSt_ProtocolHdr->unOperatorCode = st_JsonRoot["unOperatorCode"].asInt();
	pSt_ProtocolHdr->byIsReply = st_JsonRoot["byIsReply"].asInt();
	pSt_ProtocolHdr->wCrypto = st_JsonRoot["wCrypto"].asInt();
	if (!st_JsonRoot["xhToken"].isNull())
	{
		pSt_ProtocolHdr->xhToken = st_JsonRoot["xhToken"].asUInt64();
	}
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseToken
函数功能：解析TOKEN
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出解析到的TOKEN值
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseToken(LPCXSTR lpszMsgBuffer, int nMsgLen, XNETHANDLE* pxhToken)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pxhToken))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	if (st_JsonRoot["xhToken"].isNull())
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	*pxhToken = st_JsonRoot["xhToken"].asUInt64();
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseAuth
函数功能：用户验证解析协议
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_UserAuth
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseAuth(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHORIZE_PROTOCOL_USERAUTHEX* pSt_UserAuth)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_UserAuth))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	Json::Value st_JsonProtocol = st_JsonRoot["st_UserAuth"];

	if (!st_JsonProtocol["tszUserName"].isNull())
	{
		_tcsxcpy(pSt_UserAuth->tszUserName, st_JsonProtocol["tszUserName"].asCString());
	}
	if (!st_JsonProtocol["tszUserPass"].isNull())
	{
		_tcsxcpy(pSt_UserAuth->tszUserPass, st_JsonProtocol["tszUserPass"].asCString());
	}
	if (!st_JsonProtocol["tszDCode"].isNull())
	{
		_tcsxcpy(pSt_UserAuth->tszDCode, st_JsonProtocol["tszDCode"].asCString());
	}
	if (!st_JsonProtocol["enClientType"].isNull())
	{
		pSt_UserAuth->enClientType = (ENUM_PROTOCOLCLIENT_TYPE)st_JsonProtocol["enClientType"].asInt();
	}
	if (!st_JsonProtocol["enDeviceType"].isNull())
	{
		pSt_UserAuth->enDeviceType = (ENUM_PROTOCOLDEVICE_TYPE)st_JsonProtocol["enDeviceType"].asInt();
	}
	if (!st_JsonProtocol["tszHWCode"].isNull())
	{
		_tcsxcpy(pSt_UserAuth->tszHWCode, st_JsonProtocol["tszHWCode"].asCString());
	}
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseUser
函数功能：解析用户信息
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_UserInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseUser(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_UserInfo))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	Json::Value st_JsonProtocol = st_JsonRoot["st_UserInfo"];
	if (!st_JsonProtocol["tszUserName"].isNull())
	{
		_tcsxcpy(pSt_UserInfo->tszUserName, st_JsonProtocol["tszUserName"].asCString());
	}
	if (!st_JsonProtocol["tszUserPass"].isNull())
	{
		_tcsxcpy(pSt_UserInfo->tszUserPass, st_JsonProtocol["tszUserPass"].asCString());
	}
	if (!st_JsonProtocol["tszEMailAddr"].isNull())
	{
		_tcsxcpy(pSt_UserInfo->tszEMailAddr, st_JsonProtocol["tszEMailAddr"].asCString());
	}
	if (!st_JsonProtocol["tszLoginTime"].isNull())
	{
		_tcsxcpy(pSt_UserInfo->tszLoginTime, st_JsonProtocol["tszLoginTime"].asCString());
	}
	if (!st_JsonProtocol["tszCreateTime"].isNull())
	{
		_tcsxcpy(pSt_UserInfo->tszCreateTime, st_JsonProtocol["tszCreateTime"].asCString());
	}
	if (!st_JsonProtocol["nPhoneNumber"].isNull())
	{
		pSt_UserInfo->nPhoneNumber = st_JsonProtocol["nPhoneNumber"].asInt64();
	}
	if (!st_JsonProtocol["nIDNumber"].isNull())
	{
		pSt_UserInfo->nIDNumber = st_JsonProtocol["nIDNumber"].asInt64();
	}
	if (!st_JsonProtocol["nUserLevel"].isNull())
	{
		pSt_UserInfo->nUserLevel = st_JsonProtocol["nUserLevel"].asInt();
	}
	if (!st_JsonProtocol["nUserState"].isNull())
	{
		pSt_UserInfo->nUserState = st_JsonProtocol["nUserState"].asInt();
	}
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParsePay
函数功能：解析用户表信息
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_UserPay
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParsePay(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_PROTOCOL_USERPAY* pSt_UserPay)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_UserPay))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	Json::Value st_JsonProtocol = st_JsonRoot["st_UserPay"];

	if (!st_JsonProtocol["tszSerialNumber"].isNull())
	{
		_tcsxcpy(pSt_UserPay->tszSerialNumber, st_JsonProtocol["tszSerialNumber"].asCString());
	}
	if (!st_JsonProtocol["tszUserName"].isNull())
	{
		_tcsxcpy(pSt_UserPay->tszUserName, st_JsonProtocol["tszUserName"].asCString());
	}
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseTry
函数功能：解析用户表信息
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_NETTry
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseTry(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_TEMPVER* pSt_NETTry)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_NETTry))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	Json::Value st_JsonProtocol = st_JsonRoot["st_VERTemp"];

	if (!st_JsonProtocol["enVMode"].isNull())
	{
		pSt_NETTry->enVMode = (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE)st_JsonProtocol["enVMode"].asInt();
	}
	if (!st_JsonProtocol["nID"].isNull())
	{
		pSt_NETTry->nID = st_JsonProtocol["nID"].asInt64();
	}
	if (!st_JsonProtocol["nVTime"].isNull())
	{
		pSt_NETTry->nVTime = st_JsonProtocol["nVTime"].asInt();
	}
	if (!st_JsonProtocol["nLTime"].isNull())
	{
		pSt_NETTry->nLTime = st_JsonProtocol["nLTime"].asInt();
	}
	if (!st_JsonProtocol["tszVSerial"].isNull())
	{
		_tcsxcpy(pSt_NETTry->tszVSerial, st_JsonProtocol["tszVSerial"].asCString());
	}
	if (!st_JsonProtocol["tszVDate"].isNull())
	{
		_tcsxcpy(pSt_NETTry->tszVDate, st_JsonProtocol["tszVDate"].asCString());
	}
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseTable
函数功能：解析用户表信息
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_UserTable
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseTable(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_USERTABLE* pSt_UserTable)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_UserTable))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	Json::Value st_UserTable = st_JsonRoot["st_UserTable"];
	Json::Value st_UserInfo = st_UserTable["st_UserInfo"];

	if (!st_UserTable["enSerialType"].isNull())
	{
		pSt_UserTable->enSerialType = (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE)st_UserTable["enSerialType"].asInt();
	}
	if (!st_UserTable["enDeviceType"].isNull())
	{
		pSt_UserTable->enDeviceType = (ENUM_PROTOCOLDEVICE_TYPE)st_UserTable["enDeviceType"].asInt();
	}
	if (!st_UserTable["tszHardCode"].isNull())
	{
		_tcsxcpy(pSt_UserTable->tszHardCode, st_UserTable["tszHardCode"].asCString());
	}
	if (!st_UserTable["tszLeftTime"].isNull())
	{
		_tcsxcpy(pSt_UserTable->tszLeftTime, st_UserTable["tszLeftTime"].asCString());
	}
	
	if (!st_UserInfo["tszUserName"].isNull())
	{
		_tcsxcpy(pSt_UserTable->st_UserInfo.tszUserName, st_UserInfo["tszUserName"].asCString());
	}
	if (!st_UserInfo["tszUserPass"].isNull())
	{
		_tcsxcpy(pSt_UserTable->st_UserInfo.tszUserPass, st_UserInfo["tszUserPass"].asCString());
	}
	if (!st_UserInfo["tszEMailAddr"].isNull())
	{
		_tcsxcpy(pSt_UserTable->st_UserInfo.tszEMailAddr, st_UserInfo["tszEMailAddr"].asCString());
	}
	if (!st_UserInfo["tszLoginTime"].isNull())
	{
		_tcsxcpy(pSt_UserTable->st_UserInfo.tszLoginTime, st_UserInfo["tszLoginTime"].asCString());
	}
	if (!st_UserInfo["tszCreateTime"].isNull())
	{
		_tcsxcpy(pSt_UserTable->st_UserInfo.tszCreateTime, st_UserInfo["tszCreateTime"].asCString());
	}
	if (!st_UserInfo["nPhoneNumber"].isNull())
	{
		pSt_UserTable->st_UserInfo.nPhoneNumber = st_UserInfo["nPhoneNumber"].asInt64();
	}
	if (!st_UserInfo["nIDNumber"].isNull())
	{
		pSt_UserTable->st_UserInfo.nIDNumber = st_UserInfo["nIDNumber"].asInt64();
	}
	if (!st_UserInfo["nUserLevel"].isNull())
	{
		pSt_UserTable->st_UserInfo.nUserLevel = st_UserInfo["nUserLevel"].asInt();
	}
	if (!st_UserInfo["nUserState"].isNull())
	{
		pSt_UserTable->st_UserInfo.nUserState = st_UserInfo["nUserState"].asInt();
	}
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseSerial
函数功能：解析HTTP序列号信息
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pppSt_SerialTable
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出数据列表
 参数.四：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseSerial(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_SERIALTABLE*** pppSt_SerialTable, int* pInt_ListCount)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pppSt_SerialTable))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	*pInt_ListCount = st_JsonRoot["Array"].size();
	BaseLib_Memory_Malloc((XPPPMEM)pppSt_SerialTable, st_JsonRoot["Array"].size(), sizeof(AUTHREG_SERIALTABLE));
	Json::Value st_JsonArray = st_JsonRoot["Array"];
	for (unsigned int i = 0; i < st_JsonArray.size(); i++)
	{
		if (!st_JsonArray[i]["bIsUsed"].isNull())
		{
			(*pppSt_SerialTable)[i]->bIsUsed = st_JsonArray[i]["bIsUsed"].asBool();
		}
		if (!st_JsonArray[i]["enSerialType"].isNull())
		{
			(*pppSt_SerialTable)[i]->enSerialType = (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE)st_JsonArray[i]["enSerialType"].asInt();
		}
		if (!st_JsonArray[i]["tszCreateTime"].isNull())
		{
			_tcsxcpy((*pppSt_SerialTable)[i]->tszCreateTime, st_JsonArray[i]["tszCreateTime"].asCString());
		}
		if (!st_JsonArray[i]["tszMaxTime"].isNull())
		{
			_tcsxcpy((*pppSt_SerialTable)[i]->tszMaxTime, st_JsonArray[i]["tszMaxTime"].asCString());
		}
		if (!st_JsonArray[i]["tszSerialNumber"].isNull())
		{
			_tcsxcpy((*pppSt_SerialTable)[i]->tszSerialNumber, st_JsonArray[i]["tszSerialNumber"].asCString());
		}
		if (!st_JsonArray[i]["tszUserName"].isNull())
		{
			_tcsxcpy((*pppSt_SerialTable)[i]->tszUserName, st_JsonArray[i]["tszUserName"].asCString());
		}
	}
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseSerial2
函数功能：解析HTTP序列号
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：penSerialType
  In/Out：Out
  类型：枚举型指针
  可空：N
  意思：导出序列卡类型
 参数.四：pInt_NumberCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出序列卡长度
 参数.五：pInt_SerialCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出序列卡个数
 参数.六：ptszHasTime
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出拥有时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseSerial2(LPCXSTR lpszMsgBuffer, int nMsgLen, ENUM_AUTHORIZE_MODULE_SERIAL_TYPE* penSerialType, int* pInt_NumberCount, int* pInt_SerialCount, XCHAR* ptszHasTime)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == penSerialType))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	Json::Value st_JsonObject = st_JsonRoot["st_SerialInfo"];

	*penSerialType = (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE)st_JsonObject["enSerialType"].asInt();
	*pInt_NumberCount = st_JsonObject["nNumberCount"].asInt();
	*pInt_SerialCount = st_JsonObject["nSerialCount"].asInt();
	_tcsxcpy(ptszHasTime, st_JsonObject["tszHasTime"].asCString());
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseOnline
函数功能：解析在线列表
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pbOnline
  In/Out：Out
  类型：逻辑型
  可空：N
  意思：导出是否只解析在线列表
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseOnline(LPCXSTR lpszMsgBuffer, int nMsgLen, bool* pbOnline)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pbOnline))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}

	*pbOnline = st_JsonRoot["Online"].asBool();
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseTime
函数功能：解析时间信息结构
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_ProtocolTime
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：导出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseTime(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_PROTOCOL_TIME* pSt_ProtocolTime)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolTime))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	Json::Value st_JsonProtocol = st_JsonRoot["st_UserTime"];

	if (!st_JsonProtocol["enSerialType"].isNull())
	{
		pSt_ProtocolTime->enSerialType = (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE)st_JsonProtocol["enSerialType"].asInt();
	}
	if (!st_JsonProtocol["enDeviceType"].isNull())
	{
		pSt_ProtocolTime->enDeviceType = (ENUM_PROTOCOLDEVICE_TYPE)st_JsonProtocol["enDeviceType"].asInt();
	}
	if (!st_JsonProtocol["nNetType"].isNull())
	{
		pSt_ProtocolTime->nNetType = st_JsonProtocol["nNetType"].asInt();
	}
	if (!st_JsonProtocol["nTimeLeft"].isNull())
	{
		pSt_ProtocolTime->nTimeLeft = (ENUM_PROTOCOLCLIENT_TYPE)st_JsonProtocol["nTimeLeft"].asInt64();
	}
	if (!st_JsonProtocol["nTimeONLine"].isNull())
	{
		pSt_ProtocolTime->nTimeONLine = (ENUM_PROTOCOLCLIENT_TYPE)st_JsonProtocol["nTimeONLine"].asInt64();
	}
	if (!st_JsonProtocol["tszLeftTime"].isNull())
	{
		_tcsxcpy(pSt_ProtocolTime->tszLeftTime, st_JsonProtocol["tszLeftTime"].asCString());
	}
	if (!st_JsonProtocol["tszUserAddr"].isNull())
	{
		_tcsxcpy(pSt_ProtocolTime->tszUserAddr, st_JsonProtocol["tszUserAddr"].asCString());
	}
	if (!st_JsonProtocol["tszUserName"].isNull())
	{
		_tcsxcpy(pSt_ProtocolTime->tszUserName, st_JsonProtocol["tszUserName"].asCString());
	}
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseSwitch
函数功能：开关选项解析函数
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_FunSwitch
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：导出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseSwitch(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_FUNCTIONSWITCH* pSt_FunSwitch)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_FunSwitch))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	Json::Value st_JsonObject = st_JsonRoot["st_SwitchInfo"];

	pSt_FunSwitch->bSwitchDelete = st_JsonObject["bSwitchDelete"].asBool();
	pSt_FunSwitch->bSwitchRegister = st_JsonObject["bSwitchRegister"].asBool();
	pSt_FunSwitch->bSwitchLogin = st_JsonObject["bSwitchLogin"].asBool();
	pSt_FunSwitch->bSwitchPay = st_JsonObject["bSwitchPay"].asBool();
	pSt_FunSwitch->bSwitchPass = st_JsonObject["bSwitchPass"].asBool();
	pSt_FunSwitch->bSwitchCDKey = st_JsonObject["bSwitchCDKey"].asBool();
	pSt_FunSwitch->bSwitchNotice = st_JsonObject["bSwitchNotice"].asBool();
	pSt_FunSwitch->bSwitchDCode = st_JsonObject["bSwitchDCode"].asBool();
	pSt_FunSwitch->bSwitchMulti = st_JsonObject["bSwitchMulti"].asBool();
	pSt_FunSwitch->bSwitchTry = st_JsonObject["bSwitchTry"].asBool();
	pSt_FunSwitch->bSwitchBanned = st_JsonObject["bSwitchBanned"].asBool();
	pSt_FunSwitch->bSwitchTokenLogin = st_JsonObject["bSwitchTokenLogin"].asBool();
	pSt_FunSwitch->bSwitchHCLogin = st_JsonObject["bSwitchHCLogin"].asBool();
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParsePos
函数功能：解析开始结束字段
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pInt_PosStart
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出获取到的开始位置
 参数.四：pInt_PosEnd
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出获取到的结束位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParsePos(LPCXSTR lpszMsgBuffer, int nMsgLen, int* pInt_PosStart, int* pInt_PosEnd)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pInt_PosStart))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}

	*pInt_PosStart = st_JsonRoot["PosStart"].asInt();
	*pInt_PosEnd = st_JsonRoot["PosEnd"].asInt();
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseBanned
函数功能：解析黑名单协议
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_AuthBanned
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：导出解析的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseBanned(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_BANNED* pSt_AuthBanned)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_AuthBanned))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	Json::Value st_JsonObject = st_JsonRoot["st_Banned"];

	if (!st_JsonObject["nID"].isNull())
	{
		pSt_AuthBanned->nID = st_JsonObject["nID"].asInt64();
	}
	if (!st_JsonObject["bEnable"].isNull())
	{
		pSt_AuthBanned->bEnable = st_JsonObject["bEnable"].asBool();
	}
	if (!st_JsonObject["tszIPAddr"].isNull())
	{
		_tcsxcpy(pSt_AuthBanned->tszIPAddr, st_JsonObject["tszIPAddr"].asCString());
	}
	if (!st_JsonObject["tszLeftTime"].isNull())
	{
		_tcsxcpy(pSt_AuthBanned->tszLeftTime, st_JsonObject["tszLeftTime"].asCString());
	}
	if (!st_JsonObject["tszCreateTime"].isNull())
	{
		_tcsxcpy(pSt_AuthBanned->tszCreateTime, st_JsonObject["tszCreateTime"].asCString());
	}
	if (!st_JsonObject["tszUserName"].isNull())
	{
		_tcsxcpy(pSt_AuthBanned->tszUserName, st_JsonObject["tszUserName"].asCString());
	}
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseBanned2
函数功能：解析HTTP的禁用协议
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pInt_POSStart
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出解析到的起始位置
 参数.四：pInt_POSEnd
  In/Out：In
  类型：整数型
  可空：N
  意思：输出解析到的结束位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseBanned2(LPCXSTR lpszMsgBuffer, int nMsgLen, int* pInt_POSStart, int* pInt_POSEnd)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pInt_POSStart))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	Json::Value st_JsonObject = st_JsonRoot["st_Banned"];

	if (!st_JsonObject["nPosStart"].isNull())
	{
		*pInt_POSStart = st_JsonObject["nPosStart"].asInt();
	}
	if (!st_JsonObject["nPosEnd"].isNull())
	{
		*pInt_POSEnd = st_JsonObject["nPosEnd"].asInt();
	}
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseCDKey
函数功能：解析CDKEY
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.三：pSt_Authorize
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析后的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseCDKey(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_AUTHORIZE_LOCAL* pSt_Authorize)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_Authorize))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	//解析JSON
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}

	if (!st_JsonRoot["tszAddr"].isNull())
	{
		_tcsxcpy(pSt_Authorize->tszAddr, st_JsonRoot["tszAddr"].asCString());
	}
	if (!st_JsonRoot["nPort"].isNull())
	{
		pSt_Authorize->nPort = st_JsonRoot["nPort"].asInt();
	}
	//应用信息
	Json::Value st_JsonAPPInfo = st_JsonRoot["st_AuthAppInfo"];
	if (!st_JsonAPPInfo["nExecTime"].isNull())
	{
		pSt_Authorize->st_AuthAppInfo.nExecTime = st_JsonAPPInfo["nExecTime"].asInt();
	}
	if (!st_JsonAPPInfo["bInit"].isNull())
	{
		pSt_Authorize->st_AuthAppInfo.bInit = st_JsonAPPInfo["bInit"].asBool();
	}
	if (!st_JsonAPPInfo["tszAppName"].isNull())
	{
		_tcsxcpy(pSt_Authorize->st_AuthAppInfo.tszAppName, st_JsonAPPInfo["tszAppName"].asCString());
	}
	if (!st_JsonAPPInfo["tszAppVer"].isNull())
	{
		_tcsxcpy(pSt_Authorize->st_AuthAppInfo.tszAppVer, st_JsonAPPInfo["tszAppVer"].asCString());
	}
	//注册信息
	Json::Value st_JsonREGInfo = st_JsonRoot["st_AuthRegInfo"];
	if (!st_JsonREGInfo["tszHardware"].isNull())
	{
		_tcsxcpy(pSt_Authorize->st_AuthRegInfo.tszHardware, st_JsonREGInfo["tszHardware"].asCString());
	}
	if (!st_JsonREGInfo["tszCreateTime"].isNull())
	{
		_tcsxcpy(pSt_Authorize->st_AuthRegInfo.tszCreateTime, st_JsonREGInfo["tszCreateTime"].asCString());
	}
	if (!st_JsonREGInfo["tszRegisterTime"].isNull())
	{
		_tcsxcpy(pSt_Authorize->st_AuthRegInfo.tszRegisterTime, st_JsonREGInfo["tszRegisterTime"].asCString());
	}
	if (!st_JsonREGInfo["tszLeftTime"].isNull())
	{
		_tcsxcpy(pSt_Authorize->st_AuthRegInfo.tszLeftTime, st_JsonREGInfo["tszLeftTime"].asCString());
	}
	if (!st_JsonREGInfo["tszStartTime"].isNull())
	{
		_tcsxcpy(pSt_Authorize->st_AuthRegInfo.tszStartTime, st_JsonREGInfo["tszStartTime"].asCString());
	}
	if (!st_JsonREGInfo["tszExpiryTime"].isNull())
	{
		_tcsxcpy(pSt_Authorize->st_AuthRegInfo.tszExpiryTime, st_JsonREGInfo["tszExpiryTime"].asCString());
	}
	if (!st_JsonREGInfo["nHasTime"].isNull())
	{
		pSt_Authorize->st_AuthRegInfo.nHasTime = st_JsonREGInfo["nHasTime"].asInt();
	}
	if (!st_JsonREGInfo["enSerialType"].isNull())
	{
		pSt_Authorize->st_AuthRegInfo.enSerialType = (ENUM_AUTHORIZE_MODULE_SERIAL_TYPE)st_JsonREGInfo["enSerialType"].asInt();
	}
	if (!st_JsonREGInfo["enRegType"].isNull())
	{
		pSt_Authorize->st_AuthRegInfo.enRegType = (ENUM_AUTHORIZE_MODULE_CDKEY_TYPE)st_JsonREGInfo["enRegType"].asInt();
	}
	if (!st_JsonREGInfo["enHWType"].isNull())
	{
		pSt_Authorize->st_AuthRegInfo.enHWType = (ENUM_AUTHORIZE_MODULE_HW_TYPE)st_JsonREGInfo["enHWType"].asInt();
	}
	if (!st_JsonREGInfo["enVModeType"].isNull())
	{
		pSt_Authorize->st_AuthRegInfo.enVModeType = (ENUM_AUTHORIZE_MODULE_VERMODE_TYPE)st_JsonREGInfo["enVModeType"].asInt();
	}
	//临时序列号
	Json::Value st_JsonSerialInfo = st_JsonRoot["st_AuthSerial"];
	if (!st_JsonSerialInfo.isNull())
	{
		if (!st_JsonSerialInfo["tszTimeSerial"].isNull())
		{
			_tcsxcpy(pSt_Authorize->st_AuthSerial.st_TimeLimit.tszTimeSerial, st_JsonSerialInfo["tszTimeSerial"].asCString());
		}
		if (!st_JsonSerialInfo["nTimeCount"].isNull())
		{
			pSt_Authorize->st_AuthSerial.st_TimeLimit.nTimeCount = st_JsonSerialInfo["nTimeCount"].asInt();
		}
		if (!st_JsonSerialInfo["tszTimeSerial"].isNull())
		{
			_tcsxcpy(pSt_Authorize->st_AuthSerial.st_DataLimit.tszDataSerial, st_JsonSerialInfo["tszTimeSerial"].asCString());
		}
		if (!st_JsonSerialInfo["tszDataTime"].isNull())
		{
			_tcsxcpy(pSt_Authorize->st_AuthSerial.st_DataLimit.tszDataTime, st_JsonSerialInfo["tszDataTime"].asCString());
		}
		if (!st_JsonSerialInfo["bTimeAdd"].isNull())
		{
			pSt_Authorize->st_AuthSerial.st_DataLimit.bTimeAdd = st_JsonSerialInfo["bTimeAdd"].asBool();
		}
		if (!st_JsonSerialInfo["tszUNLimitSerial"].isNull())
		{
			_tcsxcpy(pSt_Authorize->st_AuthSerial.st_UNLimit.tszUNLimitSerial, st_JsonSerialInfo["tszUNLimitSerial"].asCString());
		}
	}
	//CDKEY用户信息
	Json::Value st_JsonUserInfo = st_JsonRoot["st_AuthUserInfo"];
	if (!st_JsonUserInfo["tszUserName"].isNull())
	{
		_tcsxcpy(pSt_Authorize->st_AuthUserInfo.tszUserName, st_JsonUserInfo["tszUserName"].asCString());
	}
	if (!st_JsonUserInfo["tszUserContact"].isNull())
	{
		_tcsxcpy(pSt_Authorize->st_AuthUserInfo.tszUserContact, st_JsonUserInfo["tszUserContact"].asCString());
	}
	if (!st_JsonUserInfo["tszCustom"].isNull())
	{
		_tcsxcpy(pSt_Authorize->st_AuthUserInfo.tszCustom, st_JsonUserInfo["tszCustom"].asCString());
	}
	return true;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseAnnouncement
函数功能：解析公告信息
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.三：pSt_Announcement
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析后的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_Parse::Protocol_Parse_HttpParseAnnouncement(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_ANNOUNCEMENT* pSt_Announcement)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_Announcement))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	//解析JSON
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARSE;
		return false;
	}
	Json::Value st_JsonObject = st_JsonRoot["st_Notice"];

	if (!st_JsonObject["tszContext"].isNull())
	{
		_tcsxcpy(pSt_Announcement->tszContext, st_JsonObject["tszContext"].asCString());
	}
	if (!st_JsonObject["tszCreateTime"].isNull())
	{
		_tcsxcpy(pSt_Announcement->tszCreateTime, st_JsonObject["tszCreateTime"].asCString());
	}
	if (!st_JsonObject["nID"].isNull())
	{
		pSt_Announcement->nID = st_JsonObject["nID"].asInt64();
	}
	return true;
}