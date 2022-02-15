#include "pch.h"
#include "Protocol_Packet.h"
/********************************************************************
//    Created:     2021/07/16  09:10:52
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Packet\Protocol_Packet.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Packet
//    File Base:   Protocol_Packet
//    File Ext:    cpp
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     Э������
//    History:
*********************************************************************/
CProtocol_Packet::CProtocol_Packet()
{

}
CProtocol_Packet::~CProtocol_Packet()
{

}
//////////////////////////////////////////////////////////////////////////
//                              ���к���
//////////////////////////////////////////////////////////////////////////
/********************************************************************
�������ƣ�Protocol_Packet_SendPkt
�������ܣ����ʹ������
 ����.һ��ptszMsgBuffer
  In/Out��In
  ���ͣ��ַ�ָ��
  �ɿգ�N
  ��˼�����Ҫ����Ļ�����
 ����.����pInt_MsgLen
  In/Out��In
  ���ͣ�������ָ��
  �ɿգ�N
  ��˼����������С
 ����.����pSt_ProtocolHdr
  In/Out��In
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼������Э��ͷ
 ����.�ģ�lpszMsgBuffer
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼������Ҫ���͵�����
 ����.�壺nMsgLen
  In/Out��In
  ���ͣ�������
  �ɿգ�Y
  ��˼���������ݴ�С
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_SendPkt(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer /* = NULL */, int nMsgLen /* = 0 */)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	pSt_ProtocolHdr->unPacketSize = nMsgLen;

	memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	if (nMsgLen > 0)
	{
		*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + nMsgLen;
		memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), lpszMsgBuffer, nMsgLen);
	}
	else
	{
		*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR);
	}
	return TRUE;
}
/********************************************************************
�������ƣ�Protocol_Packet_WSPkt
�������ܣ�WEBSOCKETͨ���������
 ����.һ��ptszMsgBuffer
  In/Out��Out
  ���ͣ��ַ�ָ��
  �ɿգ�N
  ��˼�������ð�������
 ����.����pInt_MsgLen
  In/Out��Out
  ���ͣ�������ָ��
  �ɿգ�N
  ��˼��������ݴ�С
 ����.����pSt_ProtocolHdr
  In/Out��In
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼������Ҫ���������
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_WSPkt(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	st_JsonRoot["wHeader"] = pSt_ProtocolHdr->wHeader;
	st_JsonRoot["wTail"] = pSt_ProtocolHdr->wTail;
	st_JsonRoot["unOperatorType"] = pSt_ProtocolHdr->unOperatorType;
	st_JsonRoot["unOperatorCode"] = pSt_ProtocolHdr->unOperatorCode;
	st_JsonRoot["wCrypto"] = pSt_ProtocolHdr->wCrypto;
	st_JsonRoot["wReserve"] = pSt_ProtocolHdr->wReserve;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}
/********************************************************************
�������ƣ�Protocol_Packet_WSPktAuth
�������ܣ�WEBSOCKET��֤���Э��
 ����.һ��ptszMsgBuffer
  In/Out��Out
  ���ͣ��ַ�ָ��
  �ɿգ�N
  ��˼�������ð�������
 ����.����pInt_MsgLen
  In/Out��Out
  ���ͣ�������ָ��
  �ɿգ�N
  ��˼��������ݴ�С
 ����.����pSt_ProtocolHdr
  In/Out��In
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼������Ҫ���������
 ����.�ģ�pSt_UserAuth
  In/Out��In
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼������Ҫ���������
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_WSPktAuth(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonRoot["wHeader"] = pSt_ProtocolHdr->wHeader;
	st_JsonRoot["wTail"] = pSt_ProtocolHdr->wTail;
	st_JsonRoot["unOperatorType"] = pSt_ProtocolHdr->unOperatorType;
	st_JsonRoot["unOperatorCode"] = pSt_ProtocolHdr->unOperatorCode;
	st_JsonRoot["wCrypto"] = pSt_ProtocolHdr->wCrypto;
	st_JsonRoot["wReserve"] = pSt_ProtocolHdr->wReserve;

	st_JsonObject["tszUserName"] = pSt_UserAuth->tszUserName;
	st_JsonObject["tszUserPass"] = pSt_UserAuth->tszUserPass;
	st_JsonObject["enClientType"] = pSt_UserAuth->enClientType;
	st_JsonObject["enDeviceType"] = pSt_UserAuth->enDeviceType;

	st_JsonRoot["st_UserAuth"] = st_JsonObject;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}
/********************************************************************
�������ƣ�Protocol_Packet_WSPktInfo
�������ܣ�WEBSOCKET�û���Ϣ���Э��
 ����.һ��ptszMsgBuffer
  In/Out��Out
  ���ͣ��ַ�ָ��
  �ɿգ�N
  ��˼�������ð�������
 ����.����pInt_MsgLen
  In/Out��Out
  ���ͣ�������ָ��
  �ɿգ�N
  ��˼��������ݴ�С
 ����.����pSt_ProtocolHdr
  In/Out��In
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼������Ҫ���������
 ����.�ģ�pSt_UserInfo
  In/Out��In
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼������Ҫ���������
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_WSPktInfo(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonRoot["wHeader"] = pSt_ProtocolHdr->wHeader;
	st_JsonRoot["wTail"] = pSt_ProtocolHdr->wTail;
	st_JsonRoot["unOperatorType"] = pSt_ProtocolHdr->unOperatorType;
	st_JsonRoot["unOperatorCode"] = pSt_ProtocolHdr->unOperatorCode;
	st_JsonRoot["wCrypto"] = pSt_ProtocolHdr->wCrypto;
	st_JsonRoot["wReserve"] = pSt_ProtocolHdr->wReserve;

	st_JsonObject["tszUserName"] = pSt_UserInfo->tszUserName;
	st_JsonObject["tszUserPass"] = pSt_UserInfo->tszUserPass;
	st_JsonObject["tszCreateTime"] = pSt_UserInfo->tszCreateTime;
	st_JsonObject["tszEMailAddr"] = pSt_UserInfo->tszEMailAddr;
	st_JsonObject["tszLoginTime"] = pSt_UserInfo->tszLoginTime;
	st_JsonObject["nIDNumber"] = (Json::Value::Int64)pSt_UserInfo->nIDNumber;
	st_JsonObject["nPhoneNumber"] = (Json::Value::Int64)pSt_UserInfo->nPhoneNumber;
	st_JsonObject["nUserLevel"] = pSt_UserInfo->nUserLevel;
	st_JsonObject["nUserState"] = pSt_UserInfo->nUserState;

	st_JsonRoot["st_UserInfo"] = st_JsonObject;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}
/********************************************************************
�������ƣ�Protocol_Packet_WSPktTime
�������ܣ�WEBSOCKETʱ����Э��
 ����.һ��ptszMsgBuffer
  In/Out��Out
  ���ͣ��ַ�ָ��
  �ɿգ�N
  ��˼�������ð�������
 ����.����pInt_MsgLen
  In/Out��Out
  ���ͣ�������ָ��
  �ɿգ�N
  ��˼��������ݴ�С
 ����.����pSt_ProtocolHdr
  In/Out��In
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼������Ҫ���������
 ����.�ģ�pSt_ProtocolTime
  In/Out��In
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼������Ҫ����ĸ�������
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_WSPktTime(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, AUTHREG_PROTOCOL_TIME* pSt_ProtocolTime)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonRoot["wHeader"] = pSt_ProtocolHdr->wHeader;
	st_JsonRoot["wTail"] = pSt_ProtocolHdr->wTail;
	st_JsonRoot["unOperatorType"] = pSt_ProtocolHdr->unOperatorType;
	st_JsonRoot["unOperatorCode"] = pSt_ProtocolHdr->unOperatorCode;
	st_JsonRoot["wCrypto"] = pSt_ProtocolHdr->wCrypto;
	st_JsonRoot["wReserve"] = pSt_ProtocolHdr->wReserve;

	st_JsonObject["tszUserName"] = pSt_ProtocolTime->tszUserName;
	st_JsonObject["tszUserAddr"] = pSt_ProtocolTime->tszUserAddr;
	st_JsonObject["tszLeftTime"] = pSt_ProtocolTime->tszLeftTime;
	st_JsonObject["nTimeLeft"] = (Json::Value::Int64)pSt_ProtocolTime->nTimeLeft;
	st_JsonObject["nTimeONLine"] = (Json::Value::Int64)pSt_ProtocolTime->nTimeONLine;
	st_JsonObject["enDeviceType"] = pSt_ProtocolTime->enDeviceType;
	st_JsonObject["enSerialType"] = pSt_ProtocolTime->enSerialType;

	st_JsonRoot["st_UserTime"] = st_JsonObject;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}
/********************************************************************
�������ƣ�Protocol_Packet_WSPktNote
�������ܣ�WEBSOCKET������Э��
 ����.һ��ptszMsgBuffer
  In/Out��Out
  ���ͣ��ַ�ָ��
  �ɿգ�N
  ��˼�������ð�������
 ����.����pInt_MsgLen
  In/Out��Out
  ���ͣ�������ָ��
  �ɿգ�N
  ��˼��������ݴ�С
 ����.����pSt_ProtocolHdr
  In/Out��In
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼������Ҫ���������
 ����.�ģ�lpszMsgBuffer
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ����ĸ�������
 ����.�壺nMsgLen
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼�����빫����Ϣ��С
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CProtocol_Packet::Protocol_Packet_WSPktNote(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = XENGINE_AUTHORIZE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;

	st_JsonRoot["wHeader"] = pSt_ProtocolHdr->wHeader;
	st_JsonRoot["wTail"] = pSt_ProtocolHdr->wTail;
	st_JsonRoot["unOperatorType"] = pSt_ProtocolHdr->unOperatorType;
	st_JsonRoot["unOperatorCode"] = pSt_ProtocolHdr->unOperatorCode;
	st_JsonRoot["wCrypto"] = pSt_ProtocolHdr->wCrypto;
	st_JsonRoot["wReserve"] = pSt_ProtocolHdr->wReserve;

	st_JsonObject["lpszMsgBuffer"] = lpszMsgBuffer;
	st_JsonObject["nMsgLen"] = nMsgLen;

	st_JsonRoot["st_UserNote"] = st_JsonObject;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}