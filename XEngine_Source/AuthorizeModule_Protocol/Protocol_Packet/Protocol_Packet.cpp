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
/********************************************************************
函数名称：Protocol_Packet_HttpComm
函数功能：HTTP通用消息打包
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出包装好的缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出包装大小
 参数.三：nCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入返回的code
 参数.四：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入返回的消息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_HttpComm(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode /* = 0 */, LPCTSTR lpszMsgBuffer /* = NULL */)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;

	st_JsonRoot["code"] = nCode;
	if (NULL == lpszMsgBuffer)
	{
		st_JsonRoot["msg"] = "success";
	}
	else
	{
		st_JsonRoot["msg"] = lpszMsgBuffer;
	}

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Packet_HttpClientInfo
函数功能：HTTP客户端包装函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出包装好的缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出包装大小
 参数.三：pSt_UserTable
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_HttpClientInfo(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_USERTABLE* pSt_UserTable)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;

	Json::Value st_JsonUser;
	Json::Value st_JsonObject;

	st_JsonUser["tszUserName"] = pSt_UserTable->st_UserInfo.tszUserName;
	st_JsonUser["tszUserPass"] = pSt_UserTable->st_UserInfo.tszUserPass;
	st_JsonUser["tszEMailAddr"] = pSt_UserTable->st_UserInfo.tszEMailAddr;
	st_JsonUser["tszLoginTime"] = pSt_UserTable->st_UserInfo.tszLoginTime;
	st_JsonUser["tszCreateTime"] = pSt_UserTable->st_UserInfo.tszCreateTime;
	st_JsonUser["nPhoneNumber"] = (Json::Value::UInt64)pSt_UserTable->st_UserInfo.nPhoneNumber;
	st_JsonUser["nIDNumber"] = (Json::Value::UInt64)pSt_UserTable->st_UserInfo.nIDNumber;
	st_JsonUser["nUserLevel"] = pSt_UserTable->st_UserInfo.nUserLevel;
	st_JsonUser["nUserState"] = 0;

	st_JsonObject["tszLeftTime"] = pSt_UserTable->tszLeftTime;
	st_JsonObject["tszHardCode"] = pSt_UserTable->tszHardCode;
	st_JsonObject["enSerialType"] = pSt_UserTable->enSerialType;
	st_JsonObject["enDeviceType"] = pSt_UserTable->enDeviceType;
	st_JsonObject["st_UserInfo"] = st_JsonUser;

	st_JsonRoot["msg"] = "success";
	st_JsonRoot["code"] = 0;
	st_JsonRoot["st_UserTable"] = st_JsonObject;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Packet_HttpClientList
函数功能：HTTP客户端列表包装函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出包装好的缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出包装大小
 参数.三：pppSt_OnClient
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要处理的在线列表
 参数.四：nOnCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入在线列表个数
 参数.五：pppSt_OffClient
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要处理的离线列表
 参数.六：nOffCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入离线列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_HttpClientList(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_USERTABLE*** pppSt_OnClient, int nOnCount, AUTHREG_USERTABLE*** pppSt_OffClient, int nOffCount)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;
	
	for (int i = 0; i < nOnCount; i++)
	{
		Json::Value st_JsonUser;
		Json::Value st_JsonObject;
		
		st_JsonUser["tszUserName"] = (*pppSt_OnClient)[i]->st_UserInfo.tszUserName;
		st_JsonUser["tszUserPass"] = (*pppSt_OnClient)[i]->st_UserInfo.tszUserPass;
		st_JsonUser["tszEMailAddr"] = (*pppSt_OnClient)[i]->st_UserInfo.tszEMailAddr;
		st_JsonUser["tszLoginTime"] = (*pppSt_OnClient)[i]->st_UserInfo.tszLoginTime;
		st_JsonUser["tszCreateTime"] = (*pppSt_OnClient)[i]->st_UserInfo.tszCreateTime;
		st_JsonUser["nPhoneNumber"] = (Json::Value::UInt64)(*pppSt_OnClient)[i]->st_UserInfo.nPhoneNumber;
		st_JsonUser["nIDNumber"] = (Json::Value::UInt64)(*pppSt_OnClient)[i]->st_UserInfo.nIDNumber;
		st_JsonUser["nUserLevel"] = (*pppSt_OnClient)[i]->st_UserInfo.nUserLevel;
		st_JsonUser["nUserState"] = 1;

		st_JsonObject["tszLeftTime"] = (*pppSt_OnClient)[i]->tszLeftTime;
		st_JsonObject["tszHardCode"] = (*pppSt_OnClient)[i]->tszHardCode;
		st_JsonObject["enSerialType"] = (*pppSt_OnClient)[i]->enSerialType;
		st_JsonObject["enDeviceType"] = (*pppSt_OnClient)[i]->enDeviceType;
		st_JsonObject["st_UserInfo"] = st_JsonUser;
		st_JsonArray.append(st_JsonObject);
	}
	for (int i = 0; i < nOffCount; i++)
	{
		BOOL bFound = FALSE;
		//查找是否在线,在线就跳过
		for (int j = 0; j < nOnCount; j++)
		{
			if (0 == _tcsncmp((*pppSt_OnClient)[i]->st_UserInfo.tszUserName, (*pppSt_OffClient)[i]->st_UserInfo.tszUserName, _tcslen((*pppSt_OnClient)[i]->st_UserInfo.tszUserName)))
			{
				bFound = TRUE;
				break;
			}
		}
		if (bFound)
		{
			continue;
		}
		Json::Value st_JsonUser;
		Json::Value st_JsonObject;

		st_JsonUser["tszUserName"] = (*pppSt_OffClient)[i]->st_UserInfo.tszUserName;
		st_JsonUser["tszUserPass"] = (*pppSt_OffClient)[i]->st_UserInfo.tszUserPass;
		st_JsonUser["tszEMailAddr"] = (*pppSt_OffClient)[i]->st_UserInfo.tszEMailAddr;
		st_JsonUser["tszLoginTime"] = (*pppSt_OffClient)[i]->st_UserInfo.tszLoginTime;
		st_JsonUser["tszCreateTime"] = (*pppSt_OffClient)[i]->st_UserInfo.tszCreateTime;
		st_JsonUser["nPhoneNumber"] = (Json::Value::UInt64)(*pppSt_OffClient)[i]->st_UserInfo.nPhoneNumber;
		st_JsonUser["nIDNumber"] = (Json::Value::UInt64)(*pppSt_OffClient)[i]->st_UserInfo.nIDNumber;
		st_JsonUser["nUserLevel"] = (*pppSt_OffClient)[i]->st_UserInfo.nUserLevel;
		st_JsonUser["nUserState"] = 0;

		st_JsonObject["tszLeftTime"] = (*pppSt_OffClient)[i]->tszLeftTime;
		st_JsonObject["tszHardCode"] = (*pppSt_OffClient)[i]->tszHardCode;
		st_JsonObject["enSerialType"] = (*pppSt_OffClient)[i]->enSerialType;
		st_JsonObject["enDeviceType"] = (*pppSt_OffClient)[i]->enDeviceType;
		st_JsonObject["st_UserInfo"] = st_JsonUser;
		st_JsonArray.append(st_JsonObject);
	}

	st_JsonRoot["msg"] = "success";
	st_JsonRoot["code"] = 0;
	st_JsonRoot["Array"] = st_JsonArray;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}
/********************************************************************
函数名称：Protocol_Packet_HttpSerialList
函数功能：HTTP序列号打包函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出包装好的缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出包装大小
 参数.三：pppSt_SerialList
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要处理的序列号列表
 参数.四：nListCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_HttpSerialList(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_SERIALTABLE*** pppSt_SerialList, int nListCount)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;

	for (int i = 0; i < nListCount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["tszUserName"] = (*pppSt_SerialList)[i]->tszUserName;
		st_JsonObject["tszSerialNumber"] = (*pppSt_SerialList)[i]->tszSerialNumber;
		st_JsonObject["tszMaxTime"] = (*pppSt_SerialList)[i]->tszMaxTime;
		st_JsonObject["enSerialType"] = (*pppSt_SerialList)[i]->enSerialType;
		st_JsonObject["bIsUsed"] = (*pppSt_SerialList)[i]->bIsUsed;
		st_JsonObject["tszCreateTime"] = (*pppSt_SerialList)[i]->tszCreateTime;
		st_JsonArray.append(st_JsonObject);
	}
	st_JsonRoot["msg"] = "success";
	st_JsonRoot["code"] = 0;
	st_JsonRoot["Array"] = st_JsonArray;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}