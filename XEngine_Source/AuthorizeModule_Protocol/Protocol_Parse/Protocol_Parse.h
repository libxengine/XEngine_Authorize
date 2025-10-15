#pragma once
/********************************************************************
//    Created:     2021/07/14  14:26:26
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Parse\Protocol_Parse.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Parse
//    File Base:   Protocol_Parse
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议解析类
//    History:
*********************************************************************/

class CProtocol_Parse
{
public:
	CProtocol_Parse();
	~CProtocol_Parse();
public:
	bool Protocol_Parse_WSHdr(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr);
public:
	bool Protocol_Parse_HttpParseToken(LPCXSTR lpszMsgBuffer, int nMsgLen, XNETHANDLE* pxhToken);
	bool Protocol_Parse_HttpParseAuth(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERAUTHEX* pSt_UserAuth);
	bool Protocol_Parse_HttpParseUser(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo);
	bool Protocol_Parse_HttpParsePay(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_PROTOCOL_USERPAY* pSt_UserPay);
	bool Protocol_Parse_HttpParseTry(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_TEMPVER* pSt_NETTry);
	bool Protocol_Parse_HttpParseTable(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_USERTABLE* pSt_UserTable);
	bool Protocol_Parse_HttpParseSerial(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_SERIALTABLE*** pppSt_SerialTable, int* pInt_ListCount);
	bool Protocol_Parse_HttpParseOnline(LPCXSTR lpszMsgBuffer, int nMsgLen, bool* pbOnline);
	bool Protocol_Parse_HttpParseTime(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_PROTOCOL_TIME* pSt_ProtocolTime);
	bool Protocol_Parse_HttpParseSwitch(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_FUNCTIONSWITCH* pSt_FunSwitch);
	bool Protocol_Parse_HttpParsePos(LPCXSTR lpszMsgBuffer, int nMsgLen, int* pInt_PosStart, int* pInt_PosEnd);
	bool Protocol_Parse_HttpParseBanned(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_BANNED* pSt_AuthBanned);
	bool Protocol_Parse_HttpParseBanned2(LPCXSTR lpszMsgBuffer, int nMsgLen, int* pInt_POSStart, int* pInt_POSEnd);
	bool Protocol_Parse_HttpParseCDKey(LPCXSTR lpszMsgBuffer, int nMsgLen, VERIFICATION_XAUTHKEY* pSt_Authorize);
	bool Protocol_Parse_HttpParseAnnouncement(LPCXSTR lpszMsgBuffer, int nMsgLen, AUTHREG_ANNOUNCEMENT* pSt_Announcement);
protected:
private:
};