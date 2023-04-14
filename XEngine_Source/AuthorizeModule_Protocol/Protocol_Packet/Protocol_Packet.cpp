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
函数名称：Protocol_Packet_HDRComm
函数功能：通用头打包函数
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
 参数.四：enDeviceType
  In/Out：In
  类型：整数型
  可空：N
  意思：输入设备类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Packet::Protocol_Packet_HDRComm(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, int enDeviceType)
{
	Protocol_IsErrorOccur = XFALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = XTRUE;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return XFALSE;
	}
	if (0 == enDeviceType)
	{
		*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR);
		memcpy(ptszMsgBuffer, pSt_ProtocolHdr, *pInt_MsgLen);
	}
	else
	{
		Json::Value st_JsonRoot;
		st_JsonRoot["wHeader"] = pSt_ProtocolHdr->wHeader;
		st_JsonRoot["wTail"] = pSt_ProtocolHdr->wTail;
		st_JsonRoot["unOperatorType"] = pSt_ProtocolHdr->unOperatorType;
		st_JsonRoot["unOperatorCode"] = pSt_ProtocolHdr->unOperatorCode;
		st_JsonRoot["wCrypto"] = pSt_ProtocolHdr->wCrypto;
		st_JsonRoot["wReserve"] = pSt_ProtocolHdr->wReserve;

		*pInt_MsgLen = st_JsonRoot.toStyledString().length();
		memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	}
	return XTRUE;
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
XBOOL CProtocol_Packet::Protocol_Packet_HttpComm(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode /* = 0 */, LPCXSTR lpszMsgBuffer /* = NULL */)
{
	Protocol_IsErrorOccur = XFALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = XTRUE;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return XFALSE;
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
	return XTRUE;
}
/********************************************************************
函数名称：Protocol_Packet_HttpUserPass
函数功能：HTTP客户端验证数据打包函数
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
 参数.三：pSt_UserAuth
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Packet::Protocol_Packet_HttpUserPass(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth)
{
	Protocol_IsErrorOccur = XFALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = XTRUE;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return XFALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonUser;

	st_JsonUser["tszUserName"] = pSt_UserAuth->tszUserName;
	st_JsonUser["tszUserPass"] = pSt_UserAuth->tszUserPass;
	st_JsonUser["enDeviceType"] = pSt_UserAuth->enDeviceType;
	st_JsonUser["enClientType"] = pSt_UserAuth->enClientType;

	st_JsonRoot["msg"] = "success";
	st_JsonRoot["code"] = 0;
	st_JsonRoot["st_UserAuth"] = st_JsonUser;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return XTRUE;
}
/********************************************************************
函数名称：Protocol_Packet_HttpUserTime
函数功能：用户时间打包函数
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
 参数.三：pSt_ProtocolTime
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的附加数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Packet::Protocol_Packet_HttpUserTime(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_PROTOCOL_TIME* pSt_ProtocolTime)
{
	Protocol_IsErrorOccur = XFALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = XTRUE;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return XFALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonObject["tszUserName"] = pSt_ProtocolTime->tszUserName;
	st_JsonObject["tszUserAddr"] = pSt_ProtocolTime->tszUserAddr;
	st_JsonObject["tszLeftTime"] = pSt_ProtocolTime->tszLeftTime;
	st_JsonObject["nTimeLeft"] = (Json::Value::Int64)pSt_ProtocolTime->nTimeLeft;
	st_JsonObject["nTimeONLine"] = (Json::Value::Int64)pSt_ProtocolTime->nTimeONLine;
	st_JsonObject["enDeviceType"] = pSt_ProtocolTime->enDeviceType;
	st_JsonObject["enSerialType"] = pSt_ProtocolTime->enSerialType;
	st_JsonObject["nNetType"] = pSt_ProtocolTime->nNetType;

	st_JsonRoot["st_UserTime"] = st_JsonObject;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return XTRUE;
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
XBOOL CProtocol_Packet::Protocol_Packet_HttpClientInfo(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_USERTABLE* pSt_UserTable)
{
	Protocol_IsErrorOccur = XFALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = XTRUE;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return XFALSE;
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
	return XTRUE;
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
XBOOL CProtocol_Packet::Protocol_Packet_HttpClientList(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHSESSION_NETCLIENT*** pppSt_OnClient, int nOnCount, AUTHREG_USERTABLE*** pppSt_OffClient, int nOffCount)
{
	Protocol_IsErrorOccur = XFALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = XTRUE;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return XFALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;
	
	for (int i = 0; i < nOnCount; i++)
	{
		Json::Value st_JsonUser;
		Json::Value st_JsonObject;
		
		st_JsonUser["tszUserName"] = (*pppSt_OnClient)[i]->st_UserTable.st_UserInfo.tszUserName;
		st_JsonUser["tszUserPass"] = (*pppSt_OnClient)[i]->st_UserTable.st_UserInfo.tszUserPass;
		st_JsonUser["tszEMailAddr"] = (*pppSt_OnClient)[i]->st_UserTable.st_UserInfo.tszEMailAddr;
		st_JsonUser["tszLoginTime"] = (*pppSt_OnClient)[i]->st_UserTable.st_UserInfo.tszLoginTime;
		st_JsonUser["tszCreateTime"] = (*pppSt_OnClient)[i]->st_UserTable.st_UserInfo.tszCreateTime;
		st_JsonUser["nPhoneNumber"] = (Json::Value::UInt64)(*pppSt_OnClient)[i]->st_UserTable.st_UserInfo.nPhoneNumber;
		st_JsonUser["nIDNumber"] = (Json::Value::UInt64)(*pppSt_OnClient)[i]->st_UserTable.st_UserInfo.nIDNumber;
		st_JsonUser["nUserLevel"] = (*pppSt_OnClient)[i]->st_UserTable.st_UserInfo.nUserLevel;
		st_JsonUser["nUserState"] = 1;

		st_JsonObject["tszClientAddr"] = (*pppSt_OnClient)[i]->tszClientAddr;
		st_JsonObject["tszLeftTime"] = (*pppSt_OnClient)[i]->tszLeftTime;
		st_JsonObject["nLeftTime"] = (Json::Value::UInt64)(*pppSt_OnClient)[i]->nLeftTime;
		st_JsonObject["nOnlineTime"] = (Json::Value::UInt64)(*pppSt_OnClient)[i]->nOnlineTime;

		st_JsonObject["tszHardCode"] = (*pppSt_OnClient)[i]->st_UserTable.tszHardCode;
		st_JsonObject["enSerialType"] = (*pppSt_OnClient)[i]->st_UserTable.enSerialType;
		st_JsonObject["enDeviceType"] = (*pppSt_OnClient)[i]->st_UserTable.enDeviceType;
		st_JsonObject["st_UserInfo"] = st_JsonUser;
		st_JsonArray.append(st_JsonObject);
	}
	for (int i = 0; i < nOffCount; i++)
	{
		XBOOL bFound = XFALSE;
		//查找是否在线,在线就跳过
		for (int j = 0; j < nOnCount; j++)
		{
			if (0 == _tcsncmp((*pppSt_OnClient)[j]->st_UserTable.st_UserInfo.tszUserName, (*pppSt_OffClient)[i]->st_UserInfo.tszUserName, _tcslen((*pppSt_OnClient)[j]->st_UserTable.st_UserInfo.tszUserName)))
			{
				bFound = XTRUE;
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
	return XTRUE;
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
XBOOL CProtocol_Packet::Protocol_Packet_HttpSerialList(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_SERIALTABLE*** pppSt_SerialList, int nListCount)
{
	Protocol_IsErrorOccur = XFALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = XTRUE;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return XFALSE;
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
	return XTRUE;
}
/********************************************************************
函数名称：Protocol_Packet_HttpToken
函数功能：TOKEN创建打包函数
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
 参数.三：xhToken
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入创建的TOKEN
 参数.四：nTimeout
  In/Out：In
  类型：整数型
  可空：N
  意思：输入超时时间
 参数.五：nDCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入动态验证码
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Packet::Protocol_Packet_HttpToken(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, int nTimeout, int nDCode /* = 0 */)
{
	Protocol_IsErrorOccur = XFALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = XTRUE;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return XFALSE;
	}
	XCHAR tszTimeStart[128];
	XCHAR tszTimeEnd[128];
	XENGINE_LIBTIMER st_TimeOut;
	Json::Value st_JsonRoot;

	memset(tszTimeStart, '\0', sizeof(tszTimeStart));
	memset(tszTimeEnd, '\0', sizeof(tszTimeEnd));
	memset(&st_TimeOut, '\0', sizeof(XENGINE_LIBTIMER));

	BaseLib_OperatorTime_TimeToStr(tszTimeStart);
	_stprintf(tszTimeEnd, _T("0-0-0 0:0:%d"), nTimeout);
	BaseLib_OperatorTimeSpan_CalForStr(tszTimeStart, tszTimeEnd, &st_TimeOut);
	BaseLib_OperatorTime_TimeToStr(tszTimeEnd, NULL, XTRUE, &st_TimeOut);

	st_JsonRoot["msg"] = "success";
	st_JsonRoot["code"] = 0;
	st_JsonRoot["xhToken"] = (Json::Value::UInt64)xhToken;
	st_JsonRoot["nTimeout"] = nTimeout;
	st_JsonRoot["tszTimeStart"] = tszTimeStart;
	st_JsonRoot["tszTimeEnd"] = tszTimeEnd;
	if (nDCode > 0)
	{
		st_JsonRoot["nDynamicCode"] = nDCode;
	}

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return XTRUE;
}
/********************************************************************
函数名称：Protocol_Packet_HttpSwitch
函数功能：打包开关功能选项
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
 参数.三：pSt_FunSwitch
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Packet::Protocol_Packet_HttpSwitch(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_FUNCTIONSWITCH* pSt_FunSwitch)
{
	Protocol_IsErrorOccur = XFALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = XTRUE;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return XFALSE;
	}
	Json::Value st_JsonRoot;

	st_JsonRoot["msg"] = "success";
	st_JsonRoot["code"] = 0;
	st_JsonRoot["bSwitchDelete"] = pSt_FunSwitch->bSwitchDelete;
	st_JsonRoot["bSwitchLogin"] = pSt_FunSwitch->bSwitchLogin;
	st_JsonRoot["bSwitchPass"] = pSt_FunSwitch->bSwitchPass;
	st_JsonRoot["bSwitchPay"] = pSt_FunSwitch->bSwitchPay;
	st_JsonRoot["bSwitchRegister"] = pSt_FunSwitch->bSwitchRegister;
	st_JsonRoot["bSwitchCDKey"] = pSt_FunSwitch->bSwitchCDKey;
	st_JsonRoot["bSwitchNotice"] = pSt_FunSwitch->bSwitchNotice;
	st_JsonRoot["bSwitchDCode"] = pSt_FunSwitch->bSwitchDCode;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return XTRUE;
}
/********************************************************************
函数名称：Protocol_Packet_HttpBanned
函数功能：HTTP客户端禁用名单包装函数
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
 参数.三：pppSt_BannedUser
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要处理的用户列表
 参数.四：nUserCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入用户列表个数
 参数.五：pppSt_BannedAddr
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要处理的地址列表
 参数.六：nAddrCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入地址列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Packet::Protocol_Packet_HttpBanned(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_BANNED*** pppSt_BannedUser, int nUserCount, AUTHREG_BANNED*** pppSt_BannedAddr, int nAddrCount)
{
	Protocol_IsErrorOccur = XFALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = XTRUE;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return XFALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonUser;
	Json::Value st_JsonAddr;

	for (int i = 0; i < nUserCount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["nID"] = (Json::Value::Int64)(*pppSt_BannedUser)[i]->nID;
		st_JsonObject["bEnable"] = (*pppSt_BannedUser)[i]->bEnable;
		st_JsonObject["tszLeftTime"] = (*pppSt_BannedUser)[i]->tszLeftTime;
		st_JsonObject["tszCreateTime"] = (*pppSt_BannedUser)[i]->tszCreateTime;
		st_JsonObject["tszUserName"] = (*pppSt_BannedUser)[i]->tszUserName;
		st_JsonUser.append(st_JsonObject);
	}
	for (int i = 0; i < nAddrCount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["nID"] = (Json::Value::Int64)(*pppSt_BannedAddr)[i]->nID;
		st_JsonObject["bEnable"] = (*pppSt_BannedAddr)[i]->bEnable;
		st_JsonObject["tszIPAddr"] = (*pppSt_BannedAddr)[i]->tszIPAddr;
		st_JsonObject["tszLeftTime"] = (*pppSt_BannedAddr)[i]->tszLeftTime;
		st_JsonObject["tszCreateTime"] = (*pppSt_BannedAddr)[i]->tszCreateTime;
		st_JsonAddr.append(st_JsonObject);
	}
	st_JsonRoot["msg"] = "success";
	st_JsonRoot["code"] = 0;
	st_JsonRoot["CountAddr"] = st_JsonAddr.size();
	st_JsonRoot["CountUser"] = st_JsonUser.size();
	st_JsonRoot["ArrayUser"] = st_JsonUser;
	st_JsonRoot["ArrayAddr"] = st_JsonAddr;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return XTRUE;
}
/********************************************************************
函数名称：Protocol_Packet_HttpAnnouncement
函数功能：打包公告列表
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
 参数.三：pppSt_Announcement
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要处理的列表
 参数.四：nListCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XBOOL CProtocol_Packet::Protocol_Packet_HttpAnnouncement(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_ANNOUNCEMENT*** pppSt_Announcement, int nListCount)
{
	Protocol_IsErrorOccur = XFALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = XTRUE;
		Protocol_dwErrorCode = ERROR_AUTHORIZE_MODULE_PROTOCOL_PARAMENT;
		return XFALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;

	for (int i = 0; i < nListCount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["nID"] = (Json::Value::Int64)(*pppSt_Announcement)[i]->nID;
		st_JsonObject["tszContext"] = (*pppSt_Announcement)[i]->tszContext;
		st_JsonObject["tszCreateTime"] = (*pppSt_Announcement)[i]->tszCreateTime;
		st_JsonArray.append(st_JsonObject);
	}
	st_JsonRoot["msg"] = "success";
	st_JsonRoot["code"] = 0;
	st_JsonRoot["Count"] = st_JsonArray.size();
	st_JsonRoot["Array"] = st_JsonArray;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return XTRUE;
}