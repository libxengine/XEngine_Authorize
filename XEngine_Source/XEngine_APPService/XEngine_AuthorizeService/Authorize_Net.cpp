#include "Authorize_Hdr.h"
//////////////////////////////////////////////////////////////////////////
XBOOL CALLBACK XEngine_Client_TCPAccept(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	HelpComponents_Datas_CreateEx(xhTCPPacket, lpszClientAddr, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("TCP客户端：%s，进入服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK XEngine_Client_TCPRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	if (!HelpComponents_Datas_PostEx(xhTCPPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，投递数据包失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
	}
}
void CALLBACK XEngine_Client_TCPClose(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr);
}
//////////////////////////////////////////////////////////////////////////
XBOOL CALLBACK XEngine_Client_WSAccept(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	RfcComponents_WSPacket_CreateEx(xhWSPacket, lpszClientAddr, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("WS客户端：%s，进入服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK XEngine_Client_WSRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	XBOOL bLogin = FALSE;
	RfcComponents_WSPacket_GetLoginEx(xhWSPacket, lpszClientAddr, &bLogin);
	if (bLogin)
	{
		if (!RfcComponents_WSPacket_InsertQueueEx(xhWSPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，投递数据包失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
		}
	}
	else
	{
		int nSDLen = nMsgLen;
		XCHAR tszHandsBuffer[1024];
		memset(tszHandsBuffer, '\0', sizeof(tszHandsBuffer));

		RfcComponents_WSConnector_HandShake(lpszRecvMsg, &nSDLen, tszHandsBuffer);
		NetCore_TCPXCore_SendEx(xhWSSocket, lpszClientAddr, tszHandsBuffer, nSDLen);
		RfcComponents_WSPacket_SetLoginEx(xhWSPacket, lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("WS客户端：%s，握手成功"), lpszClientAddr);
	}
}
void CALLBACK XEngine_Client_WSClose(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr);
}
//////////////////////////////////////////////////////////////////////////
XBOOL CALLBACK XEngine_Client_HttpAccept(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	HttpProtocol_Server_CreateClientEx(xhHttpPacket, lpszClientAddr, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端：%s，进入服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK XEngine_Client_HttpRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	if (!HttpProtocol_Server_InserQueueEx(xhHttpPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，投递数据包失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
	}
}
void CALLBACK XEngine_Client_HttpClose(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr);
}
//////////////////////////////////////////////////////////////////////////
XBOOL XEngine_CloseClient(LPCXSTR lpszClientAddr)
{
	HelpComponents_Datas_DeleteEx(xhTCPPacket, lpszClientAddr);
	RfcComponents_WSPacket_DeleteEx(xhWSPacket, lpszClientAddr);
	HttpProtocol_Server_CloseClinetEx(xhHttpPacket, lpszClientAddr);

	NetCore_TCPXCore_CloseForClientEx(xhTCPSocket, lpszClientAddr);
	NetCore_TCPXCore_CloseForClientEx(xhWSSocket, lpszClientAddr);
	NetCore_TCPXCore_CloseForClientEx(xhHttpSocket, lpszClientAddr);

	XCHAR tszClientUser[64];
	AUTHREG_PROTOCOL_TIME st_AuthTime;
	AUTHSESSION_NETCLIENT st_NETClient;

	memset(tszClientUser, '\0', sizeof(tszClientUser));
	memset(&st_AuthTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
	memset(&st_NETClient, '\0', sizeof(AUTHSESSION_NETCLIENT));

	if (Session_Authorize_GetUserForAddr(lpszClientAddr, tszClientUser))
	{
		if (Session_Authorize_GetClientForUser(tszClientUser, &st_NETClient))
		{
			st_AuthTime.nTimeLeft = st_NETClient.nLeftTime;
			st_AuthTime.nTimeONLine = st_NETClient.nOnlineTime;
			st_AuthTime.enSerialType = st_NETClient.st_UserTable.enSerialType;
			_tcscpy(st_AuthTime.tszUserName, tszClientUser);
			_tcscpy(st_AuthTime.tszLeftTime, st_NETClient.tszLeftTime);
			_tcscpy(st_AuthTime.tszUserAddr, st_NETClient.tszClientAddr);
		}
		//只有登录的用户才通知
		if (st_AuthConfig.st_XLogin.bPassAuth)
		{
			int nSDLen = 0;
			XCHAR tszSDBuffer[MAX_PATH];
			memset(tszSDBuffer, '\0', MAX_PATH);

			Protocol_Packet_HttpUserTime(tszSDBuffer, &nSDLen, &st_AuthTime);
			APIClient_Http_Request(_T("POST"), st_AuthConfig.st_XLogin.st_PassUrl.tszPassLogout, tszSDBuffer);
		}
		else
		{
			Database_SQLite_UserLeave(&st_AuthTime);
		}
		Session_Authorize_CloseClient(tszClientUser);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，离开服务器,在线时长:%d"), lpszClientAddr, tszClientUser, st_AuthTime.nTimeONLine);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，离开服务器"), lpszClientAddr);
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
XBOOL XEngine_Client_TaskSend(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nNetType)
{
	if (st_AuthConfig.st_XCrypto.bEnable)
	{
		XCHAR tszPassword[64];
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
XBOOL XEngine_SendMsg(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nNetType, LPCXSTR lpszPass)
{
	XCHAR* ptszMsgBuffer = (XCHAR*)ManagePool_Memory_Alloc(xhMemPool, XENGINE_AUTH_MAX_BUFFER);
	if (NULL == ptszMsgBuffer)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，网络类型:%d 发送数据失败,内存申请失败,错误码:%d"), lpszClientAddr, nNetType, errno);
		return FALSE;
	}

	if (XENGINE_AUTH_APP_NETTYPE_WS == nNetType)
	{
		if (NULL == lpszPass)
		{
			RfcComponents_WSCodec_EncodeMsg(lpszMsgBuffer, ptszMsgBuffer, &nMsgLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_TEXT);
		}
		else
		{
			XCHAR* ptszCodecBuffer = (XCHAR*)malloc(XENGINE_AUTH_MAX_BUFFER);
			if (NULL == ptszCodecBuffer)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，网络类型:%d 发送数据失败,内存申请失败,错误码:%d"), lpszClientAddr, nNetType, errno);
				return FALSE;
			}
			OPenSsl_XCrypto_Encoder(lpszMsgBuffer, &nMsgLen, (UCHAR*)ptszCodecBuffer, lpszPass);
			RfcComponents_WSCodec_EncodeMsg(ptszCodecBuffer, ptszMsgBuffer, &nMsgLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_TEXT);
			ManagePool_Memory_Free(xhMemPool, ptszCodecBuffer);
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
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, ptszMsgBuffer, &nMsgLen, &st_HDRParam, lpszMsgBuffer, nMsgLen);
		}
		else
		{
			XCHAR* ptszCodecBuffer = (XCHAR*)ManagePool_Memory_Alloc(xhMemPool, XENGINE_AUTH_MAX_BUFFER);
			if (NULL == ptszCodecBuffer)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，网络类型:%d 发送数据失败,内存申请失败,错误码:%d"), lpszClientAddr, nNetType, errno);
				return FALSE;
			}

			OPenSsl_XCrypto_Encoder(lpszMsgBuffer, &nMsgLen, (UCHAR*)ptszCodecBuffer, lpszPass);
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, ptszMsgBuffer, &nMsgLen, &st_HDRParam, ptszCodecBuffer, nMsgLen);
			ManagePool_Memory_Free(xhMemPool, ptszCodecBuffer);
		}
		NetCore_TCPXCore_SendEx(xhHttpSocket, lpszClientAddr, ptszMsgBuffer, nMsgLen);
	}
	ManagePool_Memory_Free(xhMemPool, ptszMsgBuffer);
	return TRUE;
}