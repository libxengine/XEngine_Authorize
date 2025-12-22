#include "Authorize_Hdr.h"

XHTHREAD XCALLBACK XEngine_AuthService_WSThread(XPVOID lParam)
{
	int nPoolIndex = *(int*)lParam;
	int nThreadPos = nPoolIndex + 1;

	while (bIsRun)
	{
		if (!RfcComponents_WSPacket_WaitEventEx(xhWSPacket, nThreadPos))
		{
			continue;
		}
		int nMsgLen = 2048;
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		int nListCount = 0;
		XENGINE_MANAGEPOOL_TASKEVENT** ppSt_ListClient;
		RfcComponents_WSPacket_GetPoolEx(xhWSPacket, nThreadPos, &ppSt_ListClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE enOPCode;
			if (!RfcComponents_WSPacket_GetEx(xhWSPacket, ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &enOPCode))
			{
				continue;
			}
			if (st_AuthConfig.st_XCrypto.bEnable)
			{
				XCHAR tszDeBuffer[2048] = {};
				Cryption_XCrypto_Decoder(tszMsgBuffer, &nMsgLen, tszDeBuffer, st_AuthConfig.st_XCrypto.tszCryptoKey);
				XEngine_Client_WSTask(ppSt_ListClient[i]->tszClientAddr, tszDeBuffer, nMsgLen, enOPCode);
			}
			else
			{
				XEngine_Client_WSTask(ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, nMsgLen, enOPCode);
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	return 0;
}

bool XEngine_Client_WSTask(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE enOPCode)
{
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	if (ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_CLOSE == enOPCode)
	{
		int nSDLen = 0;
		XCHAR tszMSGBuffer[XPATH_MAX] = {};
		RfcComponents_WSCodec_EncodeMsg(NULL, tszMSGBuffer, &nSDLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_CLOSE);
		NetCore_TCPXCore_SendEx(xhWSSocket, lpszClientAddr, tszMSGBuffer, nSDLen);
	}
	else if (ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_PING == enOPCode)
	{
		int nSDLen = 0;
		XCHAR tszMSGBuffer[XPATH_MAX] = {};
		RfcComponents_WSCodec_EncodeMsg(NULL, tszMSGBuffer, &nSDLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_PONG);
		NetCore_TCPXCore_SendEx(xhWSSocket, lpszClientAddr, tszMSGBuffer, nSDLen);
	}
	else
	{
		if (!Protocol_Parse_WSHdr(lpszMsgBuffer, nMsgLen, &st_ProtocolHdr))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WS客户端：%s，协议错误"), lpszClientAddr);
			return false;
		}

		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN == st_ProtocolHdr.unOperatorCode)
		{
			XENGINE_PROTOCOL_USERAUTHEX st_UserAuth = {};
			Protocol_Parse_HttpParseAuth(lpszMsgBuffer, nMsgLen, &st_UserAuth);
			XEngine_Client_TCPTask(lpszClientAddr, (LPCXSTR)&st_UserAuth, sizeof(XENGINE_PROTOCOL_USERAUTHEX), &st_ProtocolHdr, XENGINE_AUTH_APP_NETTYPE_WS);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETTIME == st_ProtocolHdr.unOperatorCode)
		{
			XEngine_Client_TCPTask(lpszClientAddr, NULL, 0, &st_ProtocolHdr, XENGINE_AUTH_APP_NETTYPE_WS);
		}
		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_HB_SYN == st_ProtocolHdr.unOperatorCode)
		{
			XEngine_Client_TCPTask(lpszClientAddr, NULL, 0, &st_ProtocolHdr, XENGINE_AUTH_APP_NETTYPE_WS);
		}
	}
	
	return true;
}