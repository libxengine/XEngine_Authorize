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