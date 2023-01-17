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
public:
	BOOL Protocol_Parse_HttpParseToken(LPCTSTR lpszMsgBuffer, int nMsgLen, XNETHANDLE* pxhToken);
	BOOL Protocol_Parse_HttpParseAuth(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth);
	BOOL Protocol_Parse_HttpParseUser(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo);
	BOOL Protocol_Parse_HttpParsePay(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_PROTOCOL_USERPAY* pSt_UserPay);
	BOOL Protocol_Parse_HttpParseTry(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszSerial);
	BOOL Protocol_Parse_HttpParseTable(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_USERTABLE* pSt_UserTable);
	BOOL Protocol_Parse_HttpParseSerial(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_SERIALTABLE*** pppSt_SerialTable, int* pInt_ListCount);
	BOOL Protocol_Parse_HttpParseSerial2(LPCTSTR lpszMsgBuffer, int nMsgLen, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE* penSerialType, int* pInt_NumberCount, int* pInt_SerialCount, TCHAR* ptszHasTime);
	BOOL Protocol_Parse_HttpParseOnline(LPCTSTR lpszMsgBuffer, int nMsgLen, BOOL* pbOnline);
	BOOL Protocol_Parse_HttpParseTime(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_PROTOCOL_TIME* pSt_ProtocolTime);
	BOOL Protocol_Parse_HttpParseSwitch(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_FUNCTIONSWITCH* pSt_FunSwitch);
	BOOL Protocol_Parse_HttpParsePos(LPCTSTR lpszMsgBuffer, int nMsgLen, int* pInt_PosStart, int* pInt_PosEnd);
	BOOL Protocol_Parse_HttpParseBanned(LPCTSTR lpszMsgBuffer, int nMsgLen, AUTHREG_BANNED* pSt_AuthBanned);
	BOOL Protocol_Parse_HttpParseCDKey(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_AUTHORIZE_LOCAL* pSt_Authorize);
protected:
private:
};