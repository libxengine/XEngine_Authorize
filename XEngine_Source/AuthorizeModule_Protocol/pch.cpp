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
BOOL Protocol_IsErrorOccur = FALSE;
DWORD Protocol_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CProtocol_Packet m_ProtocolPacket;
CProtocol_Parse m_ProtocolParse;
//////////////////////////////////////////////////////////////////////////
//                          导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD  Protocol_GetLastError(int* pInt_SysError)
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
extern "C" BOOL Protocol_Packet_HDRComm(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, int enDeviceType)
{
	return m_ProtocolPacket.Protocol_Packet_HDRComm(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolHdr, enDeviceType);
}
extern "C" BOOL Protocol_Packet_HttpComm(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, int nCode, LPCTSTR lpszMsgBuffer)
{
	return m_ProtocolPacket.Protocol_Packet_HttpComm(ptszMsgBuffer, pInt_MsgLen, nCode, lpszMsgBuffer);
}
extern "C" BOOL Protocol_Packet_HttpUserPass(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOL_USERAUTH * pSt_UserAuth)
{
	return m_ProtocolPacket.Protocol_Packet_HttpUserPass(ptszMsgBuffer, pInt_MsgLen, pSt_UserAuth);
}
extern "C" BOOL Protocol_Packet_HttpUserTime(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_PROTOCOL_TIME * pSt_ProtocolTime)
{
	return m_ProtocolPacket.Protocol_Packet_HttpUserTime(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolTime);
}
extern "C" BOOL Protocol_Packet_HttpClientInfo(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_ProtocolPacket.Protocol_Packet_HttpClientInfo(ptszMsgBuffer, pInt_MsgLen, pSt_UserTable);
}
extern "C" BOOL Protocol_Packet_HttpClientList(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHSESSION_NETCLIENT * **pppSt_OnClient, int nOnCount, AUTHREG_USERTABLE * **pppSt_OffClient, int nOffCount)
{
	return m_ProtocolPacket.Protocol_Packet_HttpClientList(ptszMsgBuffer, pInt_MsgLen, pppSt_OnClient, nOnCount, pppSt_OffClient, nOffCount);
}
extern "C" BOOL Protocol_Packet_HttpSerialList(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_SERIALTABLE * **pppSt_SerialList, int nListCount)
{
	return m_ProtocolPacket.Protocol_Packet_HttpSerialList(ptszMsgBuffer, pInt_MsgLen, pppSt_SerialList, nListCount);
}
extern "C" BOOL Protocol_Packet_HttpToken(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, int nTimeout)
{
	return m_ProtocolPacket.Protocol_Packet_HttpToken(ptszMsgBuffer, pInt_MsgLen, xhToken, nTimeout);
}
extern "C" BOOL Protocol_Packet_HttpSwitch(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_FUNCTIONSWITCH * pSt_FunSwitch)
{
	return m_ProtocolPacket.Protocol_Packet_HttpSwitch(ptszMsgBuffer, pInt_MsgLen, pSt_FunSwitch);
}
extern "C" BOOL Protocol_Packet_HttpBanned(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_BANNED * **pppSt_BannedUser, int nUserCount, AUTHREG_BANNED * **pppSt_BannedAddr, int nAddrCount)
{
	return m_ProtocolPacket.Protocol_Packet_HttpBanned(ptszMsgBuffer, pInt_MsgLen, pppSt_BannedUser, nUserCount, pppSt_BannedAddr, nAddrCount);
}
extern "C" BOOL Protocol_Packet_HttpAnnouncement(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_ANNOUNCEMENT * **pppSt_Announcement, int nListCount)
{
	return m_ProtocolPacket.Protocol_Packet_HttpAnnouncement(ptszMsgBuffer, pInt_MsgLen, pppSt_Announcement, nListCount);
}
/************************************************************************/
/*                          解析类协议                                  */
/************************************************************************/
extern "C" BOOL Protocol_Parse_WSHdr(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr)
{
	return m_ProtocolParse.Protocol_Parse_WSHdr(lpszMsgBuffer, nMsgLen, pSt_ProtocolHdr);
}
extern "C" BOOL Protocol_Parse_HttpParseToken(LPCTSTR lpszMsgBuffer, int nMsgLen, XNETHANDLE * pxhToken)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseToken(lpszMsgBuffer, nMsgLen, pxhToken);
}
extern "C" BOOL Protocol_Parse_HttpParseAuth(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERAUTH * pSt_UserAuth)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseAuth(lpszMsgBuffer, nMsgLen, pSt_UserAuth);
}
extern "C" BOOL Protocol_Parse_HttpParseUser(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERINFO * pSt_UserInfo)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, pSt_UserInfo);
}
extern "C" BOOL Protocol_Parse_HttpParsePay(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_PROTOCOL_USERPAY * pSt_UserPay)
{
	return m_ProtocolParse.Protocol_Parse_HttpParsePay(lpszMsgBuffer, nMsgLen, pSt_UserPay);
}
extern "C" BOOL Protocol_Parse_HttpParseTry(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR * ptszSerial)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseTry(lpszMsgBuffer, nMsgLen, ptszSerial);
}
extern "C" BOOL Protocol_Parse_HttpParseTable(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseTable(lpszMsgBuffer, nMsgLen, pSt_UserTable);
}
extern "C" BOOL Protocol_Parse_HttpParseSerial(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_SERIALTABLE * **pppSt_SerialTable, int* pInt_ListCount)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseSerial(lpszMsgBuffer, nMsgLen, pppSt_SerialTable, pInt_ListCount);
}
extern "C" BOOL Protocol_Parse_HttpParseSerial2(LPCTSTR lpszMsgBuffer, int nMsgLen, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE * penSerialType, int* pInt_NumberCount, int* pInt_SerialCount, TCHAR * ptszHasTime)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseSerial2(lpszMsgBuffer, nMsgLen, penSerialType, pInt_NumberCount, pInt_SerialCount, ptszHasTime);
}
extern "C" BOOL Protocol_Parse_HttpParseOnline(LPCTSTR lpszMsgBuffer, int nMsgLen, BOOL * pbOnline)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseOnline(lpszMsgBuffer, nMsgLen, pbOnline);
}
extern "C" BOOL Protocol_Parse_HttpParseTime(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_PROTOCOL_TIME * pSt_ProtocolTime)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseTime(lpszMsgBuffer, nMsgLen, pSt_ProtocolTime);
}
extern "C" BOOL Protocol_Parse_HttpParseSwitch(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_FUNCTIONSWITCH * pSt_FunSwitch)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseSwitch(lpszMsgBuffer, nMsgLen, pSt_FunSwitch);
}
extern "C" BOOL Protocol_Parse_HttpParsePos(LPCTSTR lpszMsgBuffer, int nMsgLen, int* pInt_PosStart, int* pInt_PosEnd)
{
	return m_ProtocolParse.Protocol_Parse_HttpParsePos(lpszMsgBuffer, nMsgLen, pInt_PosStart, pInt_PosEnd);
}
extern "C" BOOL Protocol_Parse_HttpParseBanned(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_BANNED * pSt_AuthBanned)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseBanned(lpszMsgBuffer, nMsgLen, pSt_AuthBanned);
}
extern "C" BOOL Protocol_Parse_HttpParseCDKey(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_AUTHORIZE_LOCAL * pSt_Authorize)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseCDKey(lpszMsgBuffer, nMsgLen, pSt_Authorize);
}
extern "C" BOOL Protocol_Parse_HttpParseAnnouncement(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_ANNOUNCEMENT * pSt_Announcement)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseAnnouncement(lpszMsgBuffer, nMsgLen, pSt_Announcement);
}