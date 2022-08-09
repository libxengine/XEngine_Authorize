#include "Authorize_Hdr.h"

XHTHREAD CALLBACK XEngine_AuthService_HttpThread(LPVOID lParam)
{
	int nPoolIndex = *(int*)lParam;
	int nThreadPos = nPoolIndex + 1;

	while (bIsRun)
	{
		if (!RfcComponents_HttpServer_EventWaitEx(xhHttpPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_ListClient;
		RfcComponents_HttpServer_GetPoolEx(xhHttpPacket, nThreadPos, &ppSt_ListClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			int nMsgLen = 0;
			CHAR* ptszMsgBuffer = NULL;
			RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParament;

			memset(&st_HTTPParament, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));

			if (RfcComponents_HttpServer_GetMemoryEx(xhHttpPacket, ppSt_ListClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPParament))
			{
				XEngine_Client_HttpTask(ppSt_ListClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &st_HTTPParament);
			}
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	return 0;
}

BOOL XEngine_Client_HttpTask(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM *pSt_HTTPParament)
{
	LPCTSTR lpszMethodGet = _T("POST");
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	if (0 != _tcsncmp(lpszMethodGet, pSt_HTTPParament->tszHttpMethod, _tcslen(lpszMethodGet)))
	{
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,发送的方法不支持"), lpszClientAddr);
		return FALSE;
	}

	if (!Protocol_Parse_WSHdr(lpszMsgBuffer, nMsgLen, &st_ProtocolHdr))
	{
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("WS客户端：%s，协议错误"), lpszClientAddr);
		return FALSE;
	}

	return TRUE;
}