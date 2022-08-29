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
//////////////////////////////////////////////////////////////////////////
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
BOOL __stdcall XEngine_Client_HttpAccept(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	RfcComponents_HttpServer_CreateClientEx(xhHttpPacket, lpszClientAddr, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端：%s，进入服务器"), lpszClientAddr);
	return TRUE;
}
void __stdcall XEngine_Client_HttpRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (!RfcComponents_HttpServer_InserQueueEx(xhHttpPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，投递数据包失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
	}
}
void __stdcall XEngine_Client_HttpClose(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr);
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_CloseClient(LPCTSTR lpszClientAddr)
{
	HelpComponents_Datas_DeleteEx(xhTCPPacket, lpszClientAddr);
	RfcComponents_WSPacket_DeleteEx(xhWSPacket, lpszClientAddr);
	RfcComponents_HttpServer_CloseClinetEx(xhHttpPacket, lpszClientAddr);

	NetCore_TCPXCore_CloseForClientEx(xhTCPSocket, lpszClientAddr);
	NetCore_TCPXCore_CloseForClientEx(xhWSSocket, lpszClientAddr);
	NetCore_TCPXCore_CloseForClientEx(xhHttpSocket, lpszClientAddr);

	TCHAR tszClientUser[64];
	AUTHREG_PROTOCOL_TIME st_TimeProtocol;

	memset(&st_TimeProtocol, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
	memset(tszClientUser, '\0', sizeof(tszClientUser));

	if (Session_Authorize_GetUserForAddr(lpszClientAddr, tszClientUser))
	{
		if (Session_Authorize_GetTimer(tszClientUser, &st_TimeProtocol))
		{
			Database_SQLite_UserLeave(&st_TimeProtocol);
		}
		Session_Authorize_CloseClient(tszClientUser);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，离开服务器,在线时长:%d"), lpszClientAddr, tszClientUser, st_TimeProtocol.nTimeONLine);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，离开服务器"), lpszClientAddr);
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_Client_TaskSend(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, int nNetType)
{
	if (st_AuthConfig.st_XCrypto.bEnable)
	{
		TCHAR tszPassword[64];
		memset(tszPassword, '\0', sizeof(tszPassword));

		_stprintf(tszPassword, _T("%d"), st_AuthConfig.st_XCrypto.nPassword);
		XEngine_SendMsg(lpszClientAddr, lpszMsgBuffer, nMsgLen, nNetType, tszPassword);
	}
	else
	{
		XEngine_SendMsg(lpszClientAddr, lpszMsgBuffer, nMsgLen, nNetType);
	}
	return TRUE;
}
BOOL XEngine_SendMsg(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, int nNetType, LPCTSTR lpszPass)
{
	TCHAR* ptszMsgBuffer = (TCHAR*)malloc(XENGINE_AUTH_MAX_BUFFER);
	if (NULL == ptszMsgBuffer)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，网络类型:%d 发送数据失败,内存申请失败,错误码:%d"), lpszClientAddr, nNetType, errno);
		return FALSE;
	}
	memset(ptszMsgBuffer, '\0', XENGINE_AUTH_MAX_BUFFER);

	if (XENGINE_AUTH_APP_NETTYPE_WS == nNetType)
	{
		if (NULL == lpszPass)
		{
			RfcComponents_WSCodec_EncodeMsg(lpszMsgBuffer, ptszMsgBuffer, &nMsgLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_TEXT);
		}
		else
		{
			TCHAR* ptszCodecBuffer = (TCHAR*)malloc(XENGINE_AUTH_MAX_BUFFER);
			if (NULL == ptszCodecBuffer)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，网络类型:%d 发送数据失败,内存申请失败,错误码:%d"), lpszClientAddr, nNetType, errno);
				return FALSE;
			}
			memset(ptszCodecBuffer, '\0', XENGINE_AUTH_MAX_BUFFER);
			OPenSsl_XCrypto_Encoder(lpszMsgBuffer, &nMsgLen, (UCHAR*)ptszCodecBuffer, lpszPass);
			RfcComponents_WSCodec_EncodeMsg(ptszCodecBuffer, ptszMsgBuffer, &nMsgLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_TEXT);
			free(ptszCodecBuffer);
			ptszCodecBuffer = NULL;
		}
		NetCore_TCPXCore_SendEx(xhWSSocket, lpszClientAddr, ptszMsgBuffer, nMsgLen);
	}
	else if (XENGINE_AUTH_APP_NETTYPE_TCP == nNetType)
	{
		if (NULL == lpszPass)
		{
			NetCore_TCPXCore_SendEx(xhTCPSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen);
		}
		else
		{
			OPenSsl_XCrypto_Encoder(lpszMsgBuffer, &nMsgLen, (UCHAR*)ptszMsgBuffer, lpszPass);
			NetCore_TCPXCore_SendEx(xhTCPSocket, lpszClientAddr, ptszMsgBuffer, nMsgLen);
		}
	}
	else
	{
		RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;
		memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

		st_HDRParam.nHttpCode = 200;
		st_HDRParam.bIsClose = TRUE;
		if (NULL == lpszPass)
		{
			RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, ptszMsgBuffer, &nMsgLen, &st_HDRParam, lpszMsgBuffer, nMsgLen);
		}
		else
		{
			TCHAR* ptszCodecBuffer = (TCHAR*)malloc(XENGINE_AUTH_MAX_BUFFER);
			if (NULL == ptszCodecBuffer)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，网络类型:%d 发送数据失败,内存申请失败,错误码:%d"), lpszClientAddr, nNetType, errno);
				return FALSE;
			}
			memset(ptszCodecBuffer, '\0', XENGINE_AUTH_MAX_BUFFER);

			OPenSsl_XCrypto_Encoder(lpszMsgBuffer, &nMsgLen, (UCHAR*)ptszCodecBuffer, lpszPass);
			RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, ptszMsgBuffer, &nMsgLen, &st_HDRParam, ptszCodecBuffer, nMsgLen);
			free(ptszCodecBuffer);
			ptszCodecBuffer = NULL;
		}
		NetCore_TCPXCore_SendEx(xhHttpSocket, lpszClientAddr, ptszMsgBuffer, nMsgLen);
	}
	free(ptszMsgBuffer);
	ptszMsgBuffer = NULL;
	return TRUE;
}