#include "Authorize_Hdr.h"
//////////////////////////////////////////////////////////////////////////
BOOL __stdcall XEngine_Client_TCPAccept(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	HelpComponents_Datas_CreateEx(xhTCPPacket, lpszClientAddr, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("TCP客户端：%s，进入服务器"), lpszClientAddr);
	return TRUE;
}
void __stdcall XEngine_Client_TCPRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (!HelpComponents_Datas_PostEx(xhTCPPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，投递数据包失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
	}
}
void __stdcall XEngine_Client_TCPClose(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr);
}
BOOL __stdcall XEngine_Client_WSAccept(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	RfcComponents_WSPacket_CreateEx(xhWSPacket, lpszClientAddr, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("WS客户端：%s，进入服务器"), lpszClientAddr);
	return TRUE;
}
void __stdcall XEngine_Client_WSRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	BOOL bLogin = FALSE;
	RfcComponents_WSPacket_GetLoginEx(xhWSPacket, lpszClientAddr, &bLogin);
	if (bLogin)
	{
		if (!RfcComponents_WSPacket_PostEx(xhWSPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，投递数据包失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
		}
	}
	else
	{
		int nSDLen = nMsgLen;
		TCHAR tszHandsBuffer[1024];
		memset(tszHandsBuffer, '\0', sizeof(tszHandsBuffer));

		RfcComponents_WSConnector_HandShake(lpszRecvMsg, &nSDLen, tszHandsBuffer);
		NetCore_TCPXCore_SendEx(xhWSSocket, lpszClientAddr, tszHandsBuffer, nSDLen);
		RfcComponents_WSPacket_SetLoginEx(xhWSPacket, lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("WS客户端：%s，握手成功"), lpszClientAddr);
	}
}
void __stdcall XEngine_Client_WSClose(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr);
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_CloseClient(LPCTSTR lpszClientAddr)
{
	TCHAR tszClientUser[64];
	AUTHREG_PROTOCOL_TIME st_TimeProtocol;

	memset(&st_TimeProtocol, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
	memset(tszClientUser, '\0', sizeof(tszClientUser));

	if (AuthService_Session_GetUserForAddr(lpszClientAddr, tszClientUser))
	{
		if (AuthService_Session_GetTimer(tszClientUser, &st_TimeProtocol))
		{
			AuthService_SQLPacket_UserLeave(&st_TimeProtocol);
		}
		AuthService_Session_CloseClient(tszClientUser);
	}
	HelpComponents_Datas_DeleteEx(xhTCPPacket, lpszClientAddr);
	RfcComponents_WSPacket_DeleteEx(xhWSPacket, lpszClientAddr);
	NetCore_TCPXCore_CloseForClientEx(xhTCPSocket, lpszClientAddr);
	NetCore_TCPXCore_CloseForClientEx(xhWSSocket, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，离开服务器,在线时长:%d"), lpszClientAddr, tszClientUser, st_TimeProtocol.nTimeONLine);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_Client_TaskSend(LPCTSTR lpszClientAddr, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, int nNetType, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	if (st_AuthConfig.st_Crypto.bEnable)
	{
		TCHAR tszPassword[64];
		memset(tszPassword, '\0', sizeof(tszPassword));

		_stprintf(tszPassword, _T("%d"), st_AuthConfig.st_Crypto.nPassword);
		XEngine_SendMsg(lpszClientAddr, pSt_ProtocolHdr, nNetType, lpszMsgBuffer, nMsgLen, tszPassword);
	}
	else
	{
		XEngine_SendMsg(lpszClientAddr, pSt_ProtocolHdr, nNetType, lpszMsgBuffer, nMsgLen);
	}
	return TRUE;
}
BOOL XEngine_SendMsg(LPCTSTR lpszClientAddr, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, int nNetType, LPCTSTR lpszMsgBuffer, int nMsgLen, LPCTSTR lpszPass)
{
	int nSDLen = 4096;
	TCHAR tszMsgBuffer[4096];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (XENGINE_AUTH_APP_NETTYPE_WS == nNetType)
	{
		TCHAR tszWSBuffer[4096];
		memset(tszWSBuffer, '\0', sizeof(tszWSBuffer));

		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETPASS == pSt_ProtocolHdr->unOperatorCode)
		{
			Protocol_Packet_WSPktAuth(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr, (XENGINE_PROTOCOL_USERAUTH *)lpszMsgBuffer);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETUSER == pSt_ProtocolHdr->unOperatorCode)
		{
			Protocol_Packet_WSPktInfo(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr, (XENGINE_PROTOCOL_USERINFO*)lpszMsgBuffer);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETTIME == pSt_ProtocolHdr->unOperatorCode)
		{
			Protocol_Packet_WSPktTime(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr, (AUTHREG_PROTOCOL_TIME*)lpszMsgBuffer);
		}
		else
		{
			Protocol_Packet_WSPkt(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr);
		}
		RfcComponents_WSCodec_EncodeMsg(tszMsgBuffer, tszWSBuffer, &nSDLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_TEXT);
		NetCore_TCPXCore_SendEx(xhWSSocket, lpszClientAddr, tszWSBuffer, nSDLen);
	}
	else
	{
		if (NULL == lpszPass)
		{
			Protocol_Packet_SendPkt(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr, lpszMsgBuffer, nMsgLen);
		}
		else
		{
			TCHAR tszEnBuffer[4096];
			memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));

			pSt_ProtocolHdr->wCrypto = ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_XCRYPT;
			if (NULL == lpszMsgBuffer)
			{
				Protocol_Packet_SendPkt(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr);
			}
			else
			{
				OPenSsl_XCrypto_Encoder(lpszMsgBuffer, &nMsgLen, (UCHAR*)tszEnBuffer, lpszPass);
				Protocol_Packet_SendPkt(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr, tszEnBuffer, nMsgLen);
			}
		}
		NetCore_TCPXCore_SendEx(xhTCPSocket, lpszClientAddr, tszMsgBuffer, nSDLen);
	}
	return TRUE;
}