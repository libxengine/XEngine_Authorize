#pragma once

XHTHREAD CALLBACK XEngine_AuthService_TCPThread(LPVOID lParam);
BOOL XEngine_Client_TCPTask(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, int nNetType);