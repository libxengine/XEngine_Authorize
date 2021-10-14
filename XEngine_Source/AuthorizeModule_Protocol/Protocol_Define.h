#pragma once
/********************************************************************
//    Created:     2021/07/16  09:18:44
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\Protocol_Define.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol
//    File Base:   Protocol_Define
//    File Ext:    h
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     �����Ķ���
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                          ��������
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD  Protocol_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                          �����Э��                                  */
/************************************************************************/
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
extern "C" BOOL Protocol_Packet_SendPkt(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer = NULL, int nMsgLen = 0);
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
extern "C" BOOL Protocol_Packet_WSPkt(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr);
/************************************************************************/
/*                          ������Э��                                  */
/************************************************************************/
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
extern "C" BOOL Protocol_Parse_WSHdr(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr);
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
extern "C" BOOL Protocol_Parse_WSUserInfo(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo);
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
extern "C" BOOL Protocol_Parse_WSUserAuth(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth);
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
extern "C" BOOL Protocol_Parse_WSUserPay(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, AUTHREG_PROTOCOL_USERPAY* pSt_UserPay);