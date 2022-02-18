#pragma once

XHTHREAD CALLBACK XEngine_AuthService_ThreadClient(LPVOID lParam);
BOOL XEngine_Client_TaskHandle(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPVOID lParam, int nNetType);