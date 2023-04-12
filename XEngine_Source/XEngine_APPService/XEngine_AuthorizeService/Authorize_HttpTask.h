#pragma once

XHTHREAD CALLBACK XEngine_AuthService_HttpThread(XPVOID lParam);
XBOOL XEngine_Client_HttpTask(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParament);