#pragma once

XHTHREAD CALLBACK XEngine_AuthService_TCPThread(XPVOID lParam);
XBOOL XEngine_Client_TCPTask(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, int nNetType);