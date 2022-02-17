#include "pch.h"
#include "Protocol_Packet.h"
/********************************************************************
//    Created:     2021/07/16  09:10:52
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Packet\Protocol_Packet.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Packet
//    File Base:   Protocol_Packet
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议打包类
//    History:
*********************************************************************/
CProtocol_Packet::CProtocol_Packet()
{

}
CProtocol_Packet::~CProtocol_Packet()
{

}
//////////////////////////////////////////////////////////////////////////
//                              公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Protocol_Packet_SendPkt
函数功能：发送打包函数
 参数.一：ptszMsgBuffer
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出要打包的缓冲区
 参数.二：pInt_MsgLen
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出打包大小
 参数.三：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入协议头
 参数.四：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要发送的数据
 参数.五：nMsgLen
  In/Out：In
  类型：整数型
  可空：Y
  意思：发送数据大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_SendPkt(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer /* = NULL */, int nMsgLen /* = 0 */)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	pSt_ProtocolHdr->unPacketSize = nMsgLen;

	memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	if (nMsgLen > 0)
	{
		*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + nMsgLen;
		memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), lpszMsgBuffer, nMsgLen);
	}
	else
	{
		*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR);
	}
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Packet_WSPkt
函数功能：WEBSOCKET通过打包函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的数据
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出数据大小
 参数.三：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_WSPkt(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	st_JsonRoot["wHeader"] = pSt_ProtocolHdr->wHeader;
	st_JsonRoot["wTail"] = pSt_ProtocolHdr->wTail;
	st_JsonRoot["unOperatorType"] = pSt_ProtocolHdr->unOperatorType;
	st_JsonRoot["unOperatorCode"] = pSt_ProtocolHdr->unOperatorCode;
	st_JsonRoot["wCrypto"] = pSt_ProtocolHdr->wCrypto;
	st_JsonRoot["wReserve"] = pSt_ProtocolHdr->wReserve;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Packet_WSPktAuth
函数功能：WEBSOCKET验证打包协议
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的数据
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出数据大小
 参数.三：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的数据
 参数.四：pSt_UserAuth
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_WSPktAuth(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonRoot["wHeader"] = pSt_ProtocolHdr->wHeader;
	st_JsonRoot["wTail"] = pSt_ProtocolHdr->wTail;
	st_JsonRoot["unOperatorType"] = pSt_ProtocolHdr->unOperatorType;
	st_JsonRoot["unOperatorCode"] = pSt_ProtocolHdr->unOperatorCode;
	st_JsonRoot["wCrypto"] = pSt_ProtocolHdr->wCrypto;
	st_JsonRoot["wReserve"] = pSt_ProtocolHdr->wReserve;

	st_JsonObject["tszUserName"] = pSt_UserAuth->tszUserName;
	st_JsonObject["tszUserPass"] = pSt_UserAuth->tszUserPass;
	st_JsonObject["enClientType"] = pSt_UserAuth->enClientType;
	st_JsonObject["enDeviceType"] = pSt_UserAuth->enDeviceType;

	st_JsonRoot["st_UserAuth"] = st_JsonObject;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Packet_WSPktInfo
函数功能：WEBSOCKET用户信息打包协议
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的数据
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出数据大小
 参数.三：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的数据
 参数.四：pSt_UserInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_WSPktInfo(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonRoot["wHeader"] = pSt_ProtocolHdr->wHeader;
	st_JsonRoot["wTail"] = pSt_ProtocolHdr->wTail;
	st_JsonRoot["unOperatorType"] = pSt_ProtocolHdr->unOperatorType;
	st_JsonRoot["unOperatorCode"] = pSt_ProtocolHdr->unOperatorCode;
	st_JsonRoot["wCrypto"] = pSt_ProtocolHdr->wCrypto;
	st_JsonRoot["wReserve"] = pSt_ProtocolHdr->wReserve;

	st_JsonObject["tszUserName"] = pSt_UserInfo->tszUserName;
	st_JsonObject["tszUserPass"] = pSt_UserInfo->tszUserPass;
	st_JsonObject["tszCreateTime"] = pSt_UserInfo->tszCreateTime;
	st_JsonObject["tszEMailAddr"] = pSt_UserInfo->tszEMailAddr;
	st_JsonObject["tszLoginTime"] = pSt_UserInfo->tszLoginTime;
	st_JsonObject["nIDNumber"] = (Json::Value::Int64)pSt_UserInfo->nIDNumber;
	st_JsonObject["nPhoneNumber"] = (Json::Value::Int64)pSt_UserInfo->nPhoneNumber;
	st_JsonObject["nUserLevel"] = pSt_UserInfo->nUserLevel;
	st_JsonObject["nUserState"] = pSt_UserInfo->nUserState;

	st_JsonRoot["st_UserInfo"] = st_JsonObject;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Packet_WSPktTime
函数功能：WEBSOCKET时间打包协议
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的数据
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出数据大小
 参数.三：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的数据
 参数.四：pSt_ProtocolTime
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的附加数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_WSPktTime(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, AUTHREG_PROTOCOL_TIME* pSt_ProtocolTime)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonRoot["wHeader"] = pSt_ProtocolHdr->wHeader;
	st_JsonRoot["wTail"] = pSt_ProtocolHdr->wTail;
	st_JsonRoot["unOperatorType"] = pSt_ProtocolHdr->unOperatorType;
	st_JsonRoot["unOperatorCode"] = pSt_ProtocolHdr->unOperatorCode;
	st_JsonRoot["wCrypto"] = pSt_ProtocolHdr->wCrypto;
	st_JsonRoot["wReserve"] = pSt_ProtocolHdr->wReserve;

	st_JsonObject["tszUserName"] = pSt_ProtocolTime->tszUserName;
	st_JsonObject["tszUserAddr"] = pSt_ProtocolTime->tszUserAddr;
	st_JsonObject["tszLeftTime"] = pSt_ProtocolTime->tszLeftTime;
	st_JsonObject["nTimeLeft"] = (Json::Value::Int64)pSt_ProtocolTime->nTimeLeft;
	st_JsonObject["nTimeONLine"] = (Json::Value::Int64)pSt_ProtocolTime->nTimeONLine;
	st_JsonObject["enDeviceType"] = pSt_ProtocolTime->enDeviceType;
	st_JsonObject["enSerialType"] = pSt_ProtocolTime->enSerialType;

	st_JsonRoot["st_UserTime"] = st_JsonObject;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Packet_WSPktNote
函数功能：WEBSOCKET公告打包协议
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的数据
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出数据大小
 参数.三：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的数据
 参数.四：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要打包的附加数据
 参数.五：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入公告信息大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_WSPktNote(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonRoot["wHeader"] = pSt_ProtocolHdr->wHeader;
	st_JsonRoot["wTail"] = pSt_ProtocolHdr->wTail;
	st_JsonRoot["unOperatorType"] = pSt_ProtocolHdr->unOperatorType;
	st_JsonRoot["unOperatorCode"] = pSt_ProtocolHdr->unOperatorCode;
	st_JsonRoot["wCrypto"] = pSt_ProtocolHdr->wCrypto;
	st_JsonRoot["wReserve"] = pSt_ProtocolHdr->wReserve;

	st_JsonObject["lpszMsgBuffer"] = lpszMsgBuffer;
	st_JsonObject["nMsgLen"] = nMsgLen;

	st_JsonRoot["st_UserNote"] = st_JsonObject;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}