#include "Authorize_Hdr.h"
//////////////////////////////////////////////////////////////////////////
BOOL __stdcall XEngine_Client_Accept(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	HelpComponents_Datas_CreateEx(xhPacket, lpszClientAddr, 0);
	return TRUE;
}
void __stdcall XEngine_Client_Recv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (!HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，投递数据包失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
	}
}
void __stdcall XEngine_Client_Close(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr);
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_CloseClient(LPCTSTR lpszClientAddr)
{
	TCHAR tszClientUser[64];
	memset(tszClientUser, '\0', sizeof(tszClientUser));

	if (AuthService_Session_GetUserForAddr(lpszClientAddr, tszClientUser))
	{
		AUTHREG_PROTOCOL_TIME st_TimeProtocol;
		memset(&st_TimeProtocol, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

		if (AuthService_Session_GetTimer(tszClientUser, &st_TimeProtocol))
		{
			AuthService_SQLPacket_UserLeave(&st_TimeProtocol);
		}
		AuthService_Session_CloseClient(tszClientUser);
	}
	HelpComponents_Datas_DeleteEx(xhPacket, lpszClientAddr);
	NetCore_TCPXCore_CloseForClientEx(xhSocket, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，离开服务器"), lpszClientAddr, tszClientUser);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_Client_TaskSend(LPCTSTR lpszClientAddr, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	if (st_AuthConfig.st_Crypto.bEnable)
	{
		TCHAR tszPassword[64];
		memset(tszPassword, '\0', sizeof(tszPassword));

		_stprintf(tszPassword, _T("%d"), st_AuthConfig.st_Crypto.nPassword);
		XEngine_SendMsg(lpszClientAddr, pSt_ProtocolHdr, lpszMsgBuffer, nMsgLen, tszPassword);
	}
	else
	{
		XEngine_SendMsg(lpszClientAddr, pSt_ProtocolHdr, lpszMsgBuffer, nMsgLen);
	}
	return TRUE;
}
BOOL XEngine_SendMsg(LPCTSTR lpszClientAddr, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer, int nMsgLen, LPCTSTR lpszPass)
{
	int nSDLen = 4096;
	TCHAR tszMsgBuffer[4096];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

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
			nMsgLen = nSDLen;
		}
	}
	NetCore_TCPXCore_SendEx(xhSocket, lpszClientAddr, tszMsgBuffer, nSDLen);
	return TRUE;
}