#pragma once

XHTHREAD CALLBACK XEngine_AuthService_WSThread(LPVOID lParam);
BOOL XEngine_Client_WSTask(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE enOPCode, LPVOID lParam);