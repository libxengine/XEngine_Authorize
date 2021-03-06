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
	if (!st_JsonProtocol["nUserLeave"].isNull())
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