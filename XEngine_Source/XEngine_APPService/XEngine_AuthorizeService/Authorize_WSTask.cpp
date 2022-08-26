#include "Authorize_Hdr.h"

XHTHREAD CALLBACK XEngine_AuthService_WSThread(LPVOID lParam)
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
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		int nListCount = 0;
		RFCCOMPONENTS_WSPKT_CLIENT** ppSt_ListClient;
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
				TCHAR tszPassword[64];
				TCHAR tszDeBuffer[2048];

				memset(tszPassword, '\0', sizeof(tszPassword));
				memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

				_stprintf(tszPassword, _T("%d"), st_AuthConfig.st_XCrypto.nPassword);
				OPenSsl_XCrypto_Decoder(tszMsgBuffer, &nMsgLen, tszDeBuffer, tszPassword);
				XEngine_Client_WSTask(ppSt_ListClient[i]->tszClientAddr, tszDeBuffer, nMsgLen, enOPCode);
			}
			else
			{
				XEngine_Client_WSTask(ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, nMsgLen, enOPCode);
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	return 0;
}

BOOL XEngine_Client_WSTask(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE enOPCode)
{
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	if (ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_CLOSE == enOPCode)
	{
		XEngine_CloseClient(lpszClientAddr);
		return TRUE;
	}
	if (!Protocol_Parse_WSHdr(lpszMsgBuffer, nMsgLen, &st_ProtocolHdr))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("WS客户端：%s，协议错误"), lpszClientAddr);
		return FALSE;
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN == st_ProtocolHdr.unOperatorCode)
	{
		XENGINE_PROTOCOL_USERAUTH st_UserAuth;
		memset(&st_UserAuth, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

		Protocol_Parse_HttpParseAuth(lpszMsgBuffer, nMsgLen, &st_UserAuth);
		XEngine_Client_TCPTask(lpszClientAddr, (LPCTSTR)&st_UserAuth, sizeof(XENGINE_PROTOCOL_USERAUTH), &st_ProtocolHdr, XENGINE_AUTH_APP_NETTYPE_WS);
	}
	return TRUE;
}