#pragma once

XHTHREAD XCALLBACK XEngine_AuthService_HttpThread(XPVOID lParam);
bool XEngine_Client_HttpTask(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParament, XCHAR** pptszListHdr, int nHdrCount);