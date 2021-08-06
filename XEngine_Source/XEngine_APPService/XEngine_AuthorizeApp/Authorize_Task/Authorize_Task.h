#pragma once

typedef struct
{
	int nPoolIndex;
	LPVOID lPClass;
}XENGINE_THREADINFO;

XHTHREAD CALLBACK XEngine_AuthService_ThreadClient(LPVOID lParam);
BOOL XEngine_Client_TaskHandle(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPVOID lParam);