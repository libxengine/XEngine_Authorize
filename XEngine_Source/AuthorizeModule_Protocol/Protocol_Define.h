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