#pragma once

XHTHREAD CALLBACK XEngine_AuthService_HttpThread(LPVOID lParam);
BOOL XEngine_Client_HttpTask(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParament);