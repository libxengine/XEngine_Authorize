﻿#include "Authorize_Hdr.h"
//////////////////////////////////////////////////////////////////////////
bool CALLBACK XEngine_Client_TCPAccept(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	HelpComponents_Datas_CreateEx(xhTCPPacket, lpszClientAddr, 0);
	SocketOpt_HeartBeat_InsertAddrEx(xhTCPHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("TCP客户端：%s，进入服务器"), lpszClientAddr);
	return true;
}
void CALLBACK XEngine_Client_TCPRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	if (!HelpComponents_Datas_PostEx(xhTCPPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，投递数据包失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhTCPHeart, lpszClientAddr);
}
void CALLBACK XEngine_Client_TCPClose(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr, 1);
}
void CALLBACK XEngine_Client_TCPHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr, 2);
}
//////////////////////////////////////////////////////////////////////////
bool CALLBACK XEngine_Client_WSAccept(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	RfcComponents_WSPacket_CreateEx(xhWSPacket, lpszClientAddr, 0);
	SocketOpt_HeartBeat_InsertAddrEx(xhWSHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WS客户端：%s，进入服务器"), lpszClientAddr);
	return true;
}
void CALLBACK XEngine_Client_WSRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	bool bLogin = false;
	RfcComponents_WSPacket_GetLoginEx(xhWSPacket, lpszClientAddr, &bLogin);
	if (bLogin)
	{
		if (!RfcComponents_WSPacket_InsertQueueEx(xhWSPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，投递数据包失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
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
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WS客户端：%s，握手成功"), lpszClientAddr);
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhWSHeart, lpszClientAddr);
}
void CALLBACK XEngine_Client_WSClose(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr, 1);
}
void CALLBACK XEngine_Client_WSHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr, 2);
}
//////////////////////////////////////////////////////////////////////////
bool CALLBACK XEngine_Client_HttpAccept(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	HttpProtocol_Server_CreateClientEx(xhHttpPacket, lpszClientAddr, 0);
	SocketOpt_HeartBeat_InsertAddrEx(xhHTTPHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端：%s，进入服务器"), lpszClientAddr);
	return true;
}
void CALLBACK XEngine_Client_HttpRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	if (!HttpProtocol_Server_InserQueueEx(xhHttpPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，投递数据包失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHTTPHeart, lpszClientAddr);
}
void CALLBACK XEngine_Client_HttpClose(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr, 1);
}
void CALLBACK XEngine_Client_HttpHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr, 2);
}
//////////////////////////////////////////////////////////////////////////
bool XEngine_CloseClient(LPCXSTR lpszClientAddr, int nLeaveType)
{
	xstring m_StrLeave;
	if (0 == nLeaveType)
	{
		NetCore_TCPXCore_CloseForClientEx(xhTCPSocket, lpszClientAddr);
		NetCore_TCPXCore_CloseForClientEx(xhWSSocket, lpszClientAddr);
		NetCore_TCPXCore_CloseForClientEx(xhHttpSocket, lpszClientAddr);

		SocketOpt_HeartBeat_DeleteAddrEx(xhTCPHeart, lpszClientAddr);
		SocketOpt_HeartBeat_DeleteAddrEx(xhWSHeart, lpszClientAddr);
		SocketOpt_HeartBeat_DeleteAddrEx(xhHTTPHeart, lpszClientAddr);
		m_StrLeave = _X("主动断开");
	}
	else if (1 == nLeaveType)
	{
		SocketOpt_HeartBeat_DeleteAddrEx(xhTCPHeart, lpszClientAddr);
		SocketOpt_HeartBeat_DeleteAddrEx(xhWSHeart, lpszClientAddr);
		SocketOpt_HeartBeat_DeleteAddrEx(xhHTTPHeart, lpszClientAddr);
		m_StrLeave = _X("正常断开");
	}
	else
	{
		NetCore_TCPXCore_CloseForClientEx(xhTCPSocket, lpszClientAddr);
		NetCore_TCPXCore_CloseForClientEx(xhWSSocket, lpszClientAddr);
		NetCore_TCPXCore_CloseForClientEx(xhHttpSocket, lpszClientAddr);
		m_StrLeave = _X("心跳断开");
	}
	HelpComponents_Datas_DeleteEx(xhTCPPacket, lpszClientAddr);
	RfcComponents_WSPacket_DeleteEx(xhWSPacket, lpszClientAddr);
	HttpProtocol_Server_CloseClinetEx(xhHttpPacket, lpszClientAddr);
	
	AUTHSESSION_NETCLIENT st_NETClient;
	memset(&st_NETClient, '\0', sizeof(AUTHSESSION_NETCLIENT));

	if (Session_Authorize_GetUserForAddr(lpszClientAddr, &st_NETClient))
	{
		AUTHREG_PROTOCOL_TIME st_AuthTime;
		memset(&st_AuthTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

		st_AuthTime.nTimeLeft = st_NETClient.nLeftTime;
		st_AuthTime.nTimeONLine = st_NETClient.nOnlineTime;
		st_AuthTime.enSerialType = st_NETClient.st_UserTable.enSerialType;
		_tcsxcpy(st_AuthTime.tszUserName, st_NETClient.st_UserTable.st_UserInfo.tszUserName);
		_tcsxcpy(st_AuthTime.tszLeftTime, st_NETClient.tszLeftTime);
		_tcsxcpy(st_AuthTime.tszUserAddr, st_NETClient.tszClientAddr);

		if (st_AuthConfig.st_XLogin.bPassAuth)
		{
			int nSDLen = 0;
			XCHAR tszSDBuffer[MAX_PATH];
			memset(tszSDBuffer, '\0', MAX_PATH);

			Protocol_Packet_HttpUserTime(tszSDBuffer, &nSDLen, &st_AuthTime);
			APIClient_Http_Request(_X("POST"), st_AuthConfig.st_XLogin.st_PassUrl.tszPassLogout, tszSDBuffer);
		}
		Session_Token_Delete(st_NETClient.xhToken);
		Session_Authorize_CloseAddr(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("客户端：%s，用户名：%s，Token:%llu，离开服务器,在线时长:%lld,离开方式:%s"), lpszClientAddr, st_NETClient.st_UserTable.st_UserInfo.tszUserName, st_NETClient.xhToken, st_AuthTime.nTimeONLine, m_StrLeave.c_str());
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("客户端：%s，离开服务器,离开方式:%s"), lpszClientAddr, m_StrLeave.c_str());
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
bool XEngine_Client_TaskSend(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nNetType)
{
	if (st_AuthConfig.st_XCrypto.bEnable)
	{
		XCHAR tszPassword[64];
		memset(tszPassword, '\0', sizeof(tszPassword));

		_xstprintf(tszPassword, _X("%d"), st_AuthConfig.st_XCrypto.nPassword);
		XEngine_SendMsg(lpszClientAddr, lpszMsgBuffer, nMsgLen, nNetType, tszPassword);
	}
	else
	{
		XEngine_SendMsg(lpszClientAddr, lpszMsgBuffer, nMsgLen, nNetType);
	}
	return true;
}
bool XEngine_SendMsg(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nNetType, LPCXSTR lpszPass)
{
	CHttpMemory_PoolEx m_HTTPMemory(XENGINE_MEMORY_SIZE_MAX);
	if (XENGINE_AUTH_APP_NETTYPE_WS == nNetType)
	{
		if (NULL == lpszPass)
		{
			RfcComponents_WSCodec_EncodeMsg(lpszMsgBuffer, m_HTTPMemory.get(), &nMsgLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_TEXT);
		}
		else
		{
			CHttpMemory_PoolEx m_CodecMemory(XENGINE_MEMORY_SIZE_MAX);
			Cryption_XCrypto_Encoder(lpszMsgBuffer, &nMsgLen, (XBYTE*)m_CodecMemory.get(), lpszPass);
			RfcComponents_WSCodec_EncodeMsg(m_CodecMemory.get(), m_HTTPMemory.get(), &nMsgLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_TEXT);
		}
		NetCore_TCPXCore_SendEx(xhWSSocket, lpszClientAddr, m_HTTPMemory.get(), nMsgLen);
		SocketOpt_HeartBeat_ActiveAddrEx(xhWSHeart, lpszClientAddr);
	}
	else if (XENGINE_AUTH_APP_NETTYPE_TCP == nNetType)
	{
		NetCore_TCPXCore_SendEx(xhTCPSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen);
		SocketOpt_HeartBeat_ActiveAddrEx(xhTCPHeart, lpszClientAddr);
	}
	else
	{
		int nSDSize = XENGINE_MEMORY_SIZE_MAX;
		RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;
		memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

		st_HDRParam.nHttpCode = 200;
		st_HDRParam.bIsClose = true;
		if (NULL == lpszPass)
		{
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, m_HTTPMemory.get(), &nSDSize, &st_HDRParam, lpszMsgBuffer, nMsgLen);
		}
		else
		{
			CHttpMemory_PoolEx m_CodecMemory(XENGINE_MEMORY_SIZE_MAX);
			Cryption_XCrypto_Encoder(lpszMsgBuffer, &nMsgLen, (XBYTE*)m_CodecMemory.get(), lpszPass);
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, m_HTTPMemory.get(), &nSDSize, &st_HDRParam, m_CodecMemory.get(), nMsgLen);
		}
		NetCore_TCPXCore_SendEx(xhHttpSocket, lpszClientAddr, m_HTTPMemory.get(), nSDSize);
		SocketOpt_HeartBeat_ActiveAddrEx(xhHTTPHeart, lpszClientAddr);
	}
	return true;
}