#pragma once

XHTHREAD CALLBACK XEngine_AuthService_WSThread(LPVOID lParam);
BOOL XEngine_Client_WSTask(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen);