#include "pch.h"
#include "Protocol_Parse/Protocol_Parse.h"
#include "Protocol_Packet/Protocol_Packet.h"
/********************************************************************
//    Created:     2021/07/16  09:21:37
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\pch.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool Protocol_IsErrorOccur = false;
XLONG Protocol_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CProtocol_Packet m_ProtocolPacket;
CProtocol_Parse m_ProtocolParse;
//////////////////////////////////////////////////////////////////////////
//                          导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG  Protocol_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Protocol_dwErrorCode;
}
/************************************************************************/
/*                          打包类协议                                  */
/************************************************************************/
extern "C" bool Protocol_Packet_HDRComm(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, int enDeviceType)
{
	return m_ProtocolPacket.Protocol_Packet_HDRComm(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolHdr, enDeviceType);
}
extern "C" bool Protocol_Packet_HttpComm(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, int nCode, LPCXSTR lpszMsgBuffer)
{
	return m_ProtocolPacket.Protocol_Packet_HttpComm(ptszMsgBuffer, pInt_MsgLen, nCode, lpszMsgBuffer);
}
extern "C" bool Protocol_Packet_HttpUserPass(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOL_USERAUTHEX* pSt_UserAuth)
{
	return m_ProtocolPacket.Protocol_Packet_HttpUserPass(ptszMsgBuffer, pInt_MsgLen, pSt_UserAuth);
}
extern "C" bool Protocol_Packet_UserTime(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHSESSION_NETCLIENT * **pppSt_ListClient, int nListCount)
{
	return m_ProtocolPacket.Protocol_Packet_UserTime(ptszMsgBuffer, pInt_MsgLen, pppSt_ListClient, nListCount);
}
extern "C" bool Protocol_Packet_HttpUserTime(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_PROTOCOL_TIME* pSt_ProtocolTime)
{
	return m_ProtocolPacket.Protocol_Packet_HttpUserTime(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolTime);
}
extern "C" bool Protocol_Packet_HttpClientInfo(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_ProtocolPacket.Protocol_Packet_HttpClientInfo(ptszMsgBuffer, pInt_MsgLen, pSt_UserTable);
}
extern "C" bool Protocol_Packet_HttpClientList(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHSESSION_NETCLIENT * **pppSt_OnClient, int nOnCount, AUTHREG_USERTABLE * **pppSt_OffClient, int nOffCount)
{
	return m_ProtocolPacket.Protocol_Packet_HttpClientList(ptszMsgBuffer, pInt_MsgLen, pppSt_OnClient, nOnCount, pppSt_OffClient, nOffCount);
}
extern "C" bool Protocol_Packet_HttpSerialList(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_SERIALTABLE * **pppSt_SerialList, int nListCount)
{
	return m_ProtocolPacket.Protocol_Packet_HttpSerialList(ptszMsgBuffer, pInt_MsgLen, pppSt_SerialList, nListCount);
}
extern "C" bool Protocol_Packet_HttpToken(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, int nTimeout, XSHOT nDCode)
{
	return m_ProtocolPacket.Protocol_Packet_HttpToken(ptszMsgBuffer, pInt_MsgLen, xhToken, nTimeout, nDCode);
}
extern "C" bool Protocol_Packet_HttpSwitch(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_FUNCTIONSWITCH * pSt_FunSwitch)
{
	return m_ProtocolPacket.Protocol_Packet_HttpSwitch(ptszMsgBuffer, pInt_MsgLen, pSt_FunSwitch);
}
extern "C" bool Protocol_Packet_HttpBanned(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_BANNED * **pppSt_BannedUser, int nUserCount, AUTHREG_BANNED * **pppSt_BannedAddr, int nAddrCount)
{
	return m_ProtocolPacket.Protocol_Packet_HttpBanned(ptszMsgBuffer, pInt_MsgLen, pppSt_BannedUser, nUserCount, pppSt_BannedAddr, nAddrCount);
}
extern "C" bool Protocol_Packet_HttpAnnouncement(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_ANNOUNCEMENT * **pppSt_Announcement, int nListCount)
{
	return m_ProtocolPacket.Protocol_Packet_HttpAnnouncement(ptszMsgBuffer, pInt_MsgLen, pppSt_Announcement, nListCount);
}
extern "C" bool Protocol_Packet_HttpTryList(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_TEMPVER * **pppSt_TryList, int nListCount)
{
	return m_ProtocolPacket.Protocol_Packet_HttpTryList(ptszMsgBuffer, pInt_MsgLen, pppSt_TryList, nListCount);
}
/************************************************************************/
/*                          解析类协议                                  */
/************************************************************************/
extern "C" bool Protocol_Parse_WSHdr(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr)
{
	return m_ProtocolParse.Protocol_Parse_WSHdr(lpszMsgBuffer, nMsgLen, pSt_ProtocolHdr);
}
extern "C" bool Protocol_Parse_HttpParseToken(LPCXSTR lpszMsgBuffer, int nMsgLen, XNETHANDLE * pxhToken)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseToken(lpszMsgBuffer, nMsgLen, pxhToken);
}
extern "C" bool Protocol_Parse_HttpParseAuth(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERAUTHEX* pSt_UserAuth)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseAuth(lpszMsgBuffer, nMsgLen, pSt_UserAuth);
}
extern "C" bool Protocol_Parse_HttpParseUser(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERINFO * pSt_UserInfo)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, pSt_UserInfo);
}
extern "C" bool Protocol_Parse_HttpParsePay(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_PROTOCOL_USERPAY * pSt_UserPay)
{
	return m_ProtocolParse.Protocol_Parse_HttpParsePay(lpszMsgBuffer, nMsgLen, pSt_UserPay);
}
extern "C" bool Protocol_Parse_HttpParseTry(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_TEMPVER * pSt_NETTry)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseTry(lpszMsgBuffer, nMsgLen, pSt_NETTry);
}
extern "C" bool Protocol_Parse_HttpParseTable(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseTable(lpszMsgBuffer, nMsgLen, pSt_UserTable);
}
extern "C" bool Protocol_Parse_HttpParseSerial(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_SERIALTABLE * **pppSt_SerialTable, int* pInt_ListCount)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseSerial(lpszMsgBuffer, nMsgLen, pppSt_SerialTable, pInt_ListCount);
}
extern "C" bool Protocol_Parse_HttpParseSerial2(LPCXSTR lpszMsgBuffer, int nMsgLen, ENUM_VERIFICATION_MODULE_SERIAL_TYPE * penSerialType, int* pInt_NumberCount, int* pInt_SerialCount, XCHAR * ptszHasTime, XCHAR* ptszExpiredTime)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseSerial2(lpszMsgBuffer, nMsgLen, penSerialType, pInt_NumberCount, pInt_SerialCount, ptszHasTime, ptszExpiredTime);
}
extern "C" bool Protocol_Parse_HttpParseOnline(LPCXSTR lpszMsgBuffer, int nMsgLen, bool * pbOnline)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseOnline(lpszMsgBuffer, nMsgLen, pbOnline);
}
extern "C" bool Protocol_Parse_HttpParseTime(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_PROTOCOL_TIME * pSt_ProtocolTime)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseTime(lpszMsgBuffer, nMsgLen, pSt_ProtocolTime);
}
extern "C" bool Protocol_Parse_HttpParseSwitch(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_FUNCTIONSWITCH * pSt_FunSwitch)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseSwitch(lpszMsgBuffer, nMsgLen, pSt_FunSwitch);
}
extern "C" bool Protocol_Parse_HttpParsePos(LPCXSTR lpszMsgBuffer, int nMsgLen, int* pInt_PosStart, int* pInt_PosEnd)
{
	return m_ProtocolParse.Protocol_Parse_HttpParsePos(lpszMsgBuffer, nMsgLen, pInt_PosStart, pInt_PosEnd);
}
extern "C" bool Protocol_Parse_HttpParseBanned(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_BANNED * pSt_AuthBanned)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseBanned(lpszMsgBuffer, nMsgLen, pSt_AuthBanned);
}
extern "C" bool Protocol_Parse_HttpParseBanned2(LPCXSTR lpszMsgBuffer, int nMsgLen, int* pInt_POSStart, int* pInt_POSEnd)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseBanned2(lpszMsgBuffer, nMsgLen, pInt_POSStart, pInt_POSEnd);
}
extern "C" bool Protocol_Parse_HttpParseCDKey(LPCXSTR lpszMsgBuffer, int nMsgLen, VERIFICATION_XAUTHKEY * pSt_Authorize)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseCDKey(lpszMsgBuffer, nMsgLen, pSt_Authorize);
}
extern "C" bool Protocol_Parse_HttpParseAnnouncement(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_ANNOUNCEMENT * pSt_Announcement)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseAnnouncement(lpszMsgBuffer, nMsgLen, pSt_Announcement);
}