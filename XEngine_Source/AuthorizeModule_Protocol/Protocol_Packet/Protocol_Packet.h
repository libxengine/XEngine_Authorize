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
	XBOOL Protocol_Packet_HDRComm(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, int enDeviceType);
public:
	XBOOL Protocol_Packet_HttpComm(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode = 0, LPCXSTR lpszMsgBuffer = NULL);
	XBOOL Protocol_Packet_HttpUserPass(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth);
	XBOOL Protocol_Packet_HttpUserTime(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_PROTOCOL_TIME* pSt_ProtocolTime);
	XBOOL Protocol_Packet_HttpClientInfo(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_USERTABLE* pSt_UserTable);
	XBOOL Protocol_Packet_HttpClientList(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHSESSION_NETCLIENT*** pppSt_OnClient, int nOnCount, AUTHREG_USERTABLE*** pppSt_OffClient, int nOffCount);
	XBOOL Protocol_Packet_HttpSerialList(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_SERIALTABLE*** pppSt_SerialList, int nListCount);
	XBOOL Protocol_Packet_HttpToken(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, int nTimeout);
	XBOOL Protocol_Packet_HttpSwitch(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_FUNCTIONSWITCH* pSt_FunSwitch);
	XBOOL Protocol_Packet_HttpBanned(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_BANNED*** pppSt_BannedUser, int nUserCount, AUTHREG_BANNED*** pppSt_BannedAddr, int nAddrCount);
	XBOOL Protocol_Packet_HttpAnnouncement(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, AUTHREG_ANNOUNCEMENT*** pppSt_Announcement, int nListCount);
protected:
private:
};