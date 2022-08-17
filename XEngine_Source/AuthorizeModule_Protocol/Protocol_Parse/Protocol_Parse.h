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
	BOOL Protocol_Parse_WSHdr(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr);
	BOOL Protocol_Parse_WSUserInfo(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo);
	BOOL Protocol_Parse_WSUserAuth(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth);
	BOOL Protocol_Parse_WSUserPay(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, AUTHREG_PROTOCOL_USERPAY* pSt_UserPay);
	BOOL Protocol_Parse_WSUserNote(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, TCHAR* ptszMsgBuffer, int* pInt_MsgLen);
public:
	BOOL Protocol_Parse_HttpParseAuth(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth);
	BOOL Protocol_Parse_HttpParseUser(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo);
	BOOL Protocol_Parse_HttpParsePay(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_PROTOCOL_USERPAY* pSt_UserPay);
	BOOL Protocol_Parse_HttpParseTable(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_USERTABLE* pSt_UserTable);
	BOOL Protocol_Parse_HttpParseSerial(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_SERIALTABLE*** pppSt_SerialTable, int* pInt_ListCount);
	BOOL Protocol_Parse_HttpParseSerial2(LPCTSTR lpszMsgBuffer, int nMsgLen, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE* penSerialType, int* pInt_NumberCount, int* pInt_SerialCount, TCHAR* ptszHasTime);
protected:
private:
};