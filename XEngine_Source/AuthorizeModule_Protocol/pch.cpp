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
extern "C" BOOL Protocol_Packet_SendPkt(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	return m_ProtocolPacket.Protocol_Packet_SendPkt(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolHdr, lpszMsgBuffer, nMsgLen);
}
extern "C" BOOL Protocol_Packet_WSPkt(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr)
{
	return m_ProtocolPacket.Protocol_Packet_WSPkt(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolHdr);
}
extern "C" BOOL Protocol_Packet_WSPktAuth(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, XENGINE_PROTOCOL_USERAUTH * pSt_UserAuth)
{
	return m_ProtocolPacket.Protocol_Packet_WSPktAuth(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolHdr, pSt_UserAuth);
}
extern "C" BOOL Protocol_Packet_WSPktInfo(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, XENGINE_PROTOCOL_USERINFO * pSt_UserInfo)
{
	return m_ProtocolPacket.Protocol_Packet_WSPktInfo(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolHdr, pSt_UserInfo);
}
extern "C" BOOL Protocol_Packet_WSPktTime(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, AUTHREG_PROTOCOL_TIME * pSt_ProtocolTime)
{
	return m_ProtocolPacket.Protocol_Packet_WSPktTime(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolHdr, pSt_ProtocolTime);
}
extern "C" BOOL Protocol_Packet_WSPktNote(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	return m_ProtocolPacket.Protocol_Packet_WSPktNote(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolHdr, lpszMsgBuffer, nMsgLen);
}
extern "C" BOOL Protocol_Packet_HttpComm(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, int nCode, LPCTSTR lpszMsgBuffer)
{
	return m_ProtocolPacket.Protocol_Packet_HttpComm(ptszMsgBuffer, pInt_MsgLen, nCode, lpszMsgBuffer);
}
extern "C" BOOL Protocol_Packet_HttpClientInfo(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_USERTABLE * pSt_UserTable)
{
	return m_ProtocolPacket.Protocol_Packet_HttpClientInfo(ptszMsgBuffer, pInt_MsgLen, pSt_UserTable);
}
extern "C" BOOL Protocol_Packet_HttpClientList(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_USERTABLE * **pppSt_OnClient, int nOnCount, AUTHREG_USERTABLE * **pppSt_OffClient, int nOffCount)
{
	return m_ProtocolPacket.Protocol_Packet_HttpClientList(ptszMsgBuffer, pInt_MsgLen, pppSt_OnClient, nOnCount, pppSt_OffClient, nOffCount);
}
extern "C" BOOL Protocol_Packet_HttpSerialList(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_SERIALTABLE * **pppSt_SerialList, int nListCount)
{
	return m_ProtocolPacket.Protocol_Packet_HttpSerialList(ptszMsgBuffer, pInt_MsgLen, pppSt_SerialList, nListCount);
}
/************************************************************************/
/*                          解析类协议                                  */
/************************************************************************/
extern "C" BOOL Protocol_Parse_WSHdr(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr)
{
	return m_ProtocolParse.Protocol_Parse_WSHdr(lpszMsgBuffer, nMsgLen, pSt_ProtocolHdr);
}
extern "C" BOOL Protocol_Parse_WSUserInfo(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, XENGINE_PROTOCOL_USERINFO * pSt_UserInfo)
{
	return m_ProtocolParse.Protocol_Parse_WSUserInfo(lpszMsgBuffer, nMsgLen, pSt_ProtocolHdr, pSt_UserInfo);
}
extern "C" BOOL Protocol_Parse_WSUserAuth(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, XENGINE_PROTOCOL_USERAUTH * pSt_UserAuth)
{
	return m_ProtocolParse.Protocol_Parse_WSUserAuth(lpszMsgBuffer, nMsgLen, pSt_ProtocolHdr, pSt_UserAuth);
}
extern "C" BOOL Protocol_Parse_WSUserPay(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, AUTHREG_PROTOCOL_USERPAY * pSt_UserPay)
{
	return m_ProtocolParse.Protocol_Parse_WSUserPay(lpszMsgBuffer, nMsgLen, pSt_ProtocolHdr, pSt_UserPay);
}
extern "C" BOOL Protocol_Parse_WSUserNote(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, TCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_ProtocolParse.Protocol_Parse_WSUserNote(lpszMsgBuffer, nMsgLen, pSt_ProtocolHdr, ptszMsgBuffer, pInt_MsgLen);
}
extern "C" BOOL Protocol_Parse_HttpParseUser(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERINFO * pSt_UserInfo)
{
	return m_ProtocolParse.Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, pSt_UserInfo);
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