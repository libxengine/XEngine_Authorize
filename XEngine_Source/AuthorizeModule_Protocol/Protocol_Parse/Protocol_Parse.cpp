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
BOOL CProtocol_Parse::Protocol_Parse_WSHdr(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolHdr))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	pSt_ProtocolHdr->wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	pSt_ProtocolHdr->wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	pSt_ProtocolHdr->unOperatorType = st_JsonRoot["unOperatorType"].asInt();
	pSt_ProtocolHdr->unOperatorCode = st_JsonRoot["unOperatorCode"].asInt();
	pSt_ProtocolHdr->byIsReply = st_JsonRoot["byIsReply"].asInt();
	pSt_ProtocolHdr->wCrypto = st_JsonRoot["wCrypto"].asInt();
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_WSUserInfo
函数功能：获取信息结构相关协议内容
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
 参数.四：pSt_UserInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析好的协议内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Parse::Protocol_Parse_WSUserInfo(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolHdr))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	Json::Value st_JsonProtocol = st_JsonRoot["st_UserInfo"];

	pSt_ProtocolHdr->wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	pSt_ProtocolHdr->wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	pSt_ProtocolHdr->unOperatorType = st_JsonRoot["unOperatorType"].asInt();
	pSt_ProtocolHdr->unOperatorCode = st_JsonRoot["unOperatorCode"].asInt();
	pSt_ProtocolHdr->byIsReply = st_JsonRoot["byIsReply"].asInt();
	pSt_ProtocolHdr->wCrypto = st_JsonRoot["wCrypto"].asInt();
	if (!st_JsonProtocol["tszUserName"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszUserName, st_JsonProtocol["tszUserName"].asCString());
	}
	if (!st_JsonProtocol["tszUserPass"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszUserPass, st_JsonProtocol["tszUserPass"].asCString());
	}
	if (!st_JsonProtocol["tszEMailAddr"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszEMailAddr, st_JsonProtocol["tszEMailAddr"].asCString());
	}
	if (!st_JsonProtocol["tszLoginTime"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszLoginTime, st_JsonProtocol["tszLoginTime"].asCString());
	}
	if (!st_JsonProtocol["tszCreateTime"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszCreateTime, st_JsonProtocol["tszCreateTime"].asCString());
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
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_WSUserAuth
函数功能：获取验证协议相关内容
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
 参数.四：pSt_UserAuth
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析好的协议内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Parse::Protocol_Parse_WSUserAuth(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolHdr))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	Json::Value st_JsonProtocol = st_JsonRoot["st_UserAuth"];

	pSt_ProtocolHdr->wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	pSt_ProtocolHdr->wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	pSt_ProtocolHdr->unOperatorType = st_JsonRoot["unOperatorType"].asInt();
	pSt_ProtocolHdr->unOperatorCode = st_JsonRoot["unOperatorCode"].asInt();
	pSt_ProtocolHdr->byIsReply = st_JsonRoot["byIsReply"].asInt();
	pSt_ProtocolHdr->wCrypto = st_JsonRoot["wCrypto"].asInt();
	if (!st_JsonProtocol["tszUserName"].isNull())
	{
		_tcscpy(pSt_UserAuth->tszUserName, st_JsonProtocol["tszUserName"].asCString());
	}
	if (!st_JsonProtocol["tszUserPass"].isNull())
	{
		_tcscpy(pSt_UserAuth->tszUserPass, st_JsonProtocol["tszUserPass"].asCString());
	}
	if (!st_JsonProtocol["enClientType"].isNull())
	{
		pSt_UserAuth->enClientType = (ENUM_PROTOCOLCLIENT_TYPE)st_JsonProtocol["enClientType"].asInt();
	}
	if (!st_JsonProtocol["enDeviceType"].isNull())
	{
		pSt_UserAuth->enDeviceType = (ENUM_PROTOCOLDEVICE_TYPE)st_JsonProtocol["enDeviceType"].asInt();
	}
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_WSUserPay
函数功能：获取用户支持相关协议内容
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
 参数.四：pSt_UserAuth
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析好的协议内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Parse::Protocol_Parse_WSUserPay(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, AUTHREG_PROTOCOL_USERPAY* pSt_UserPay)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolHdr))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	Json::Value st_JsonProtocol = st_JsonRoot["st_UserPay"];

	pSt_ProtocolHdr->wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	pSt_ProtocolHdr->wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	pSt_ProtocolHdr->unOperatorType = st_JsonRoot["unOperatorType"].asInt();
	pSt_ProtocolHdr->unOperatorCode = st_JsonRoot["unOperatorCode"].asInt();
	pSt_ProtocolHdr->byIsReply = st_JsonRoot["byIsReply"].asInt();
	pSt_ProtocolHdr->wCrypto = st_JsonRoot["wCrypto"].asInt();
	if (!st_JsonProtocol["tszSerialNumber"].isNull())
	{
		_tcscpy(pSt_UserPay->tszSerialNumber, st_JsonProtocol["tszSerialNumber"].asCString());
	}
	if (!st_JsonProtocol["tszUserName"].isNull())
	{
		_tcscpy(pSt_UserPay->tszUserName, st_JsonProtocol["tszUserName"].asCString());
	}
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_WSUserNote
函数功能：解析用户通告和快速验证协议
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
 参数.四：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出解析好的协议内容
 参数.五：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：内容大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Parse::Protocol_Parse_WSUserNote(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, TCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolHdr))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	pSt_ProtocolHdr->wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	pSt_ProtocolHdr->wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	pSt_ProtocolHdr->unOperatorType = st_JsonRoot["unOperatorType"].asInt();
	pSt_ProtocolHdr->unOperatorCode = st_JsonRoot["unOperatorCode"].asInt();
	pSt_ProtocolHdr->byIsReply = st_JsonRoot["byIsReply"].asInt();
	pSt_ProtocolHdr->wCrypto = st_JsonRoot["wCrypto"].asInt();

	if (!st_JsonRoot["lpszPayload"].isNull())
	{
		*pInt_MsgLen = st_JsonRoot["lpszPayload"].asString().length();
		_tcscpy(ptszMsgBuffer, st_JsonRoot["lpszPayload"].asCString());
	}
	return TRUE;
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
BOOL CProtocol_Parse::Protocol_Parse_HttpParseUser(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_UserInfo))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	Json::Value st_JsonProtocol = st_JsonRoot["st_UserInfo"];
	if (!st_JsonProtocol["tszUserName"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszUserName, st_JsonProtocol["tszUserName"].asCString());
	}
	if (!st_JsonProtocol["tszUserPass"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszUserPass, st_JsonProtocol["tszUserPass"].asCString());
	}
	if (!st_JsonProtocol["tszEMailAddr"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszEMailAddr, st_JsonProtocol["tszEMailAddr"].asCString());
	}
	if (!st_JsonProtocol["tszLoginTime"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszLoginTime, st_JsonProtocol["tszLoginTime"].asCString());
	}
	if (!st_JsonProtocol["tszCreateTime"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszCreateTime, st_JsonProtocol["tszCreateTime"].asCString());
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
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Parse_HttpParseUser
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
BOOL CProtocol_Parse::Protocol_Parse_HttpParseTable(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_USERTABLE* pSt_UserTable)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_UserTable))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	Json::Value st_UserTable = st_JsonRoot["st_UserTable"];
	Json::Value st_UserInfo = st_UserTable["st_UserInfo"];

	pSt_UserTable->enSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)st_UserTable["enSerialType"].asInt();
	pSt_UserTable->enDeviceType = (ENUM_PROTOCOLDEVICE_TYPE)st_UserTable["enDeviceType"].asInt();
	_tcscpy(pSt_UserTable->tszHardCode, st_UserTable["tszHardCode"].asCString());
	_tcscpy(pSt_UserTable->tszLeftTime, st_UserTable["tszLeftTime"].asCString());
	if (!st_UserInfo["tszUserName"].isNull())
	{
		_tcscpy(pSt_UserTable->st_UserInfo.tszUserName, st_UserInfo["tszUserName"].asCString());
	}
	if (!st_UserInfo["tszUserPass"].isNull())
	{
		_tcscpy(pSt_UserTable->st_UserInfo.tszUserPass, st_UserInfo["tszUserPass"].asCString());
	}
	if (!st_UserInfo["tszEMailAddr"].isNull())
	{
		_tcscpy(pSt_UserTable->st_UserInfo.tszEMailAddr, st_UserInfo["tszEMailAddr"].asCString());
	}
	if (!st_UserInfo["tszLoginTime"].isNull())
	{
		_tcscpy(pSt_UserTable->st_UserInfo.tszLoginTime, st_UserInfo["tszLoginTime"].asCString());
	}
	if (!st_UserInfo["tszCreateTime"].isNull())
	{
		_tcscpy(pSt_UserTable->st_UserInfo.tszCreateTime, st_UserInfo["tszCreateTime"].asCString());
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
	return TRUE;
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
BOOL CProtocol_Parse::Protocol_Parse_HttpParseSerial(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_SERIALTABLE*** pppSt_SerialTable, int* pInt_ListCount)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pppSt_SerialTable))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	*pInt_ListCount = st_JsonRoot["Array"].size();
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_SerialTable, st_JsonRoot["Array"].size(), sizeof(AUTHREG_SERIALTABLE));
	Json::Value st_JsonArray = st_JsonRoot["Array"];
	for (unsigned int i = 0; i < st_JsonArray.size(); i++)
	{
		if (!st_JsonArray[i]["bIsUsed"].isNull())
		{
			(*pppSt_SerialTable)[i]->bIsUsed = st_JsonArray[i]["bIsUsed"].asBool();
		}
		if (!st_JsonArray[i]["enSerialType"].isNull())
		{
			(*pppSt_SerialTable)[i]->enSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)st_JsonArray[i]["enSerialType"].asInt();
		}
		if (!st_JsonArray[i]["tszCreateTime"].isNull())
		{
			_tcscpy((*pppSt_SerialTable)[i]->tszCreateTime, st_JsonArray[i]["tszCreateTime"].asCString());
		}
		if (!st_JsonArray[i]["tszMaxTime"].isNull())
		{
			_tcscpy((*pppSt_SerialTable)[i]->tszMaxTime, st_JsonArray[i]["tszMaxTime"].asCString());
		}
		if (!st_JsonArray[i]["tszSerialNumber"].isNull())
		{
			_tcscpy((*pppSt_SerialTable)[i]->tszSerialNumber, st_JsonArray[i]["tszSerialNumber"].asCString());
		}
		if (!st_JsonArray[i]["tszUserName"].isNull())
		{
			_tcscpy((*pppSt_SerialTable)[i]->tszUserName, st_JsonArray[i]["tszUserName"].asCString());
		}
	}
	return TRUE;
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
BOOL CProtocol_Parse::Protocol_Parse_HttpParseSerial2(LPCTSTR lpszMsgBuffer, int nMsgLen, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE* penSerialType, int* pInt_NumberCount, int* pInt_SerialCount, TCHAR* ptszHasTime)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == penSerialType))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	Json::Value st_JsonObject = st_JsonRoot["st_SerialInfo"];

	*penSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)st_JsonObject["enSerialType"].asInt();
	*pInt_NumberCount = st_JsonObject["nNumberCount"].asInt();
	*pInt_SerialCount = st_JsonObject["nSerialCount"].asInt();
	_tcscpy(ptszHasTime, st_JsonObject["tszHasTime"].asCString());
	return TRUE;
}