#include "pch.h"
#include "Protocol_Parse/Protocol_Parse.h"
#include "Protocol_Packet/Protocol_Packet.h"
/********************************************************************
//    Created:     2021/07/16  09:21:37
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\pch.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
BOOL Protocol_IsErrorOccur = FALSE;
DWORD Protocol_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CProtocol_Packet m_ProtocolPacket;
CProtocol_Parse m_ProtocolParse;
//////////////////////////////////////////////////////////////////////////
//                          导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD  Protocol_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Protocol_dwErrorCode;
}
/************************************************************************/
/*                          打包类协议                                  */
/************************************************************************/
extern "C" BOOL Protocol_Packet_SendPkt(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	return m_ProtocolPacket.Protocol_Packet_SendPkt(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolHdr, lpszMsgBuffer, nMsgLen);
}