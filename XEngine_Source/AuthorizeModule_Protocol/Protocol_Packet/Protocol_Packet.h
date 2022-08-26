#pragma once
/********************************************************************
//    Created:     2021/07/14  14:24:11
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Packet\Protocol_Packet.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Packet
//    File Base:   Protocol_Packet
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议打包类
//    History:
*********************************************************************/

class CProtocol_Packet
{
public:
	CProtocol_Packet();
	~CProtocol_Packet();
public:
	BOOL Protocol_Packet_HDRComm(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, int enDeviceType);
public:
	BOOL Protocol_Packet_HttpComm(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode = 0, LPCTSTR lpszMsgBuffer = NULL);
	BOOL Protocol_Packet_HttpUserPass(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth);
	BOOL Protocol_Packet_HttpUserTime(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_PROTOCOL_TIME* pSt_ProtocolTime);
	BOOL Protocol_Packet_HttpClientInfo(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_USERTABLE* pSt_UserTable);
	BOOL Protocol_Packet_HttpClientList(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHSESSION_NETCLIENT*** pppSt_OnClient, int nOnCount, AUTHREG_USERTABLE*** pppSt_OffClient, int nOffCount);
	BOOL Protocol_Packet_HttpSerialList(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_SERIALTABLE*** pppSt_SerialList, int nListCount);
	BOOL Protocol_Packet_HttpToken(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, int nTimeout);
protected:
private:
};