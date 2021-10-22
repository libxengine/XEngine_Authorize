#include "pch.h"
#include "Protocol_Parse.h"
/********************************************************************
//    Created:     2021/07/14  14:27:27
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Parse\Protocol_Parse.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Parse
//    File Base:   Protocol_Parse
//    File Ext:    cpp
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     Э�������
//    History:
*********************************************************************/
CProtocol_Parse::CProtocol_Parse()
{

}
CProtocol_Parse::~CProtocol_Parse()
{

}
//////////////////////////////////////////////////////////////////////////
//                              ���к���
//////////////////////////////////////////////////////////////////////////
/********************************************************************
�������ƣ�Protocol_Parse_WSHdr
�������ܣ���ȡWEBSOCKETЭ����Ϣͷ
 ����.һ��lpszMsgBuffer
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ�����Ļ�����
 ����.����nMsgLen
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼������Ҫ�����Ĵ�С
 ����.����pSt_ProtocolHdr
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼����������õ�Э��ͷ
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CProtocol_Parse::Protocol_Parse_WSHdr(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolHdr))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	pSt_ProtocolHdr->wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	pSt_ProtocolHdr->wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	pSt_ProtocolHdr->unOperatorType = st_JsonRoot["unOperatorType"].asInt();
	pSt_ProtocolHdr->unOperatorCode = st_JsonRoot["unOperatorCode"].asInt();
	pSt_ProtocolHdr->byIsReply = st_JsonRoot["byIsReply"].asInt();
	pSt_ProtocolHdr->wCrypto = st_JsonRoot["wCrypto"].asInt();
	return TRUE;
}
/********************************************************************
�������ƣ�Protocol_Parse_WSUserInfo
�������ܣ���ȡ��Ϣ�ṹ���Э������
 ����.һ��lpszMsgBuffer
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ�����Ļ�����
 ����.����nMsgLen
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼������Ҫ�����Ĵ�С
 ����.����pSt_ProtocolHdr
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼����������õ�Э��ͷ
 ����.�ģ�pSt_UserInfo
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼����������õ�Э������
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CProtocol_Parse::Protocol_Parse_WSUserInfo(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolHdr))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	Json::Value st_JsonProtocol = st_JsonRoot["st_UserInfo"];

	pSt_ProtocolHdr->wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	pSt_ProtocolHdr->wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	pSt_ProtocolHdr->unOperatorType = st_JsonRoot["unOperatorType"].asInt();
	pSt_ProtocolHdr->unOperatorCode = st_JsonRoot["unOperatorCode"].asInt();
	pSt_ProtocolHdr->byIsReply = st_JsonRoot["byIsReply"].asInt();
	pSt_ProtocolHdr->wCrypto = st_JsonRoot["wCrypto"].asInt();
	if (!st_JsonProtocol["tszUserName"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszUserName, st_JsonProtocol["tszUserName"].asCString());
	}
	if (!st_JsonProtocol["tszUserPass"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszUserPass, st_JsonProtocol["tszUserPass"].asCString());
	}
	if (!st_JsonProtocol["tszEMailAddr"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszEMailAddr, st_JsonProtocol["tszEMailAddr"].asCString());
	}
	if (!st_JsonProtocol["tszLoginTime"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszLoginTime, st_JsonProtocol["tszLoginTime"].asCString());
	}
	if (!st_JsonProtocol["tszCreateTime"].isNull())
	{
		_tcscpy(pSt_UserInfo->tszCreateTime, st_JsonProtocol["tszCreateTime"].asCString());
	}
	if (!st_JsonProtocol["nPhoneNumber"].isNull())
	{
		pSt_UserInfo->nPhoneNumber = st_JsonProtocol["nPhoneNumber"].asInt64();
	}
	if (!st_JsonProtocol["nIDNumber"].isNull())
	{
		pSt_UserInfo->nIDNumber = st_JsonProtocol["nIDNumber"].asInt64();
	}
	if (!st_JsonProtocol["nUserLeave"].isNull())
	{
		pSt_UserInfo->nUserLevel = st_JsonProtocol["nUserLevel"].asInt();
	}
	if (!st_JsonProtocol["nUserState"].isNull())
	{
		pSt_UserInfo->nUserState = st_JsonProtocol["nUserState"].asInt();
	}
	return TRUE;
}
/********************************************************************
�������ƣ�Protocol_Parse_WSUserAuth
�������ܣ���ȡ��֤Э���������
 ����.һ��lpszMsgBuffer
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ�����Ļ�����
 ����.����nMsgLen
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼������Ҫ�����Ĵ�С
 ����.����pSt_ProtocolHdr
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼����������õ�Э��ͷ
 ����.�ģ�pSt_UserAuth
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼����������õ�Э������
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CProtocol_Parse::Protocol_Parse_WSUserAuth(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolHdr))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	Json::Value st_JsonProtocol = st_JsonRoot["st_UserAuth"];

	pSt_ProtocolHdr->wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	pSt_ProtocolHdr->wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	pSt_ProtocolHdr->unOperatorType = st_JsonRoot["unOperatorType"].asInt();
	pSt_ProtocolHdr->unOperatorCode = st_JsonRoot["unOperatorCode"].asInt();
	pSt_ProtocolHdr->byIsReply = st_JsonRoot["byIsReply"].asInt();
	pSt_ProtocolHdr->wCrypto = st_JsonRoot["wCrypto"].asInt();
	if (!st_JsonProtocol["tszUserName"].isNull())
	{
		_tcscpy(pSt_UserAuth->tszUserName, st_JsonProtocol["tszUserName"].asCString());
	}
	if (!st_JsonProtocol["tszUserPass"].isNull())
	{
		_tcscpy(pSt_UserAuth->tszUserPass, st_JsonProtocol["tszUserPass"].asCString());
	}
	if (!st_JsonProtocol["enClientType"].isNull())
	{
		pSt_UserAuth->enClientType = (ENUM_PROTOCOLCLIENT_TYPE)st_JsonProtocol["enClientType"].asInt();
	}
	if (!st_JsonProtocol["enDeviceType"].isNull())
	{
		pSt_UserAuth->enDeviceType = (ENUM_PROTOCOLDEVICE_TYPE)st_JsonProtocol["enDeviceType"].asInt();
	}
	return TRUE;
}
/********************************************************************
�������ƣ�Protocol_Parse_WSUserPay
�������ܣ���ȡ�û�֧�����Э������
 ����.һ��lpszMsgBuffer
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ�����Ļ�����
 ����.����nMsgLen
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼������Ҫ�����Ĵ�С
 ����.����pSt_ProtocolHdr
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼����������õ�Э��ͷ
 ����.�ģ�pSt_UserAuth
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼����������õ�Э������
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CProtocol_Parse::Protocol_Parse_WSUserPay(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, AUTHREG_PROTOCOL_USERPAY* pSt_UserPay)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolHdr))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	Json::Value st_JsonProtocol = st_JsonRoot["st_UserPay"];

	pSt_ProtocolHdr->wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	pSt_ProtocolHdr->wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	pSt_ProtocolHdr->unOperatorType = st_JsonRoot["unOperatorType"].asInt();
	pSt_ProtocolHdr->unOperatorCode = st_JsonRoot["unOperatorCode"].asInt();
	pSt_ProtocolHdr->byIsReply = st_JsonRoot["byIsReply"].asInt();
	pSt_ProtocolHdr->wCrypto = st_JsonRoot["wCrypto"].asInt();
	if (!st_JsonProtocol["tszSerialNumber"].isNull())
	{
		_tcscpy(pSt_UserPay->tszSerialNumber, st_JsonProtocol["tszSerialNumber"].asCString());
	}
	if (!st_JsonProtocol["tszUserName"].isNull())
	{
		_tcscpy(pSt_UserPay->tszUserName, st_JsonProtocol["tszUserName"].asCString());
	}
	return TRUE;
}
/********************************************************************
�������ƣ�Protocol_Parse_WSUserNote
�������ܣ������û�ͨ��Ϳ�����֤Э��
 ����.һ��lpszMsgBuffer
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ�����Ļ�����
 ����.����nMsgLen
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼������Ҫ�����Ĵ�С
 ����.����pSt_ProtocolHdr
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼����������õ�Э��ͷ
 ����.�ģ�ptszMsgBuffer
  In/Out��Out
  ���ͣ��ַ�ָ��
  �ɿգ�N
  ��˼����������õ�Э������
 ����.�壺pInt_MsgLen
  In/Out��Out
  ���ͣ�������ָ��
  �ɿգ�N
  ��˼�����ݴ�С
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CProtocol_Parse::Protocol_Parse_WSUserNote(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, TCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolHdr))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARSE;
		return FALSE;
	}
	pSt_ProtocolHdr->wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	pSt_ProtocolHdr->wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	pSt_ProtocolHdr->unOperatorType = st_JsonRoot["unOperatorType"].asInt();
	pSt_ProtocolHdr->unOperatorCode = st_JsonRoot["unOperatorCode"].asInt();
	pSt_ProtocolHdr->byIsReply = st_JsonRoot["byIsReply"].asInt();
	pSt_ProtocolHdr->wCrypto = st_JsonRoot["wCrypto"].asInt();

	if (!st_JsonRoot["lpszPayload"].isNull())
	{
		*pInt_MsgLen = st_JsonRoot["lpszPayload"].asString().length();
		_tcscpy(ptszMsgBuffer, st_JsonRoot["lpszPayload"].asCString());
	}
	return TRUE;
}