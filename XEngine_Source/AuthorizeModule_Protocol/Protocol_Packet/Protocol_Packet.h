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
	BOOL Protocol_Packet_SendPkt(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer = NULL, int nMsgLen = 0);
	BOOL Protocol_Packet_WSPkt(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr);
protected:
private:
};