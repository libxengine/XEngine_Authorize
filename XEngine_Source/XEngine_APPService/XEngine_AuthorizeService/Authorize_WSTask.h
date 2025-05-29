#pragma once

XHTHREAD XCALLBACK XEngine_AuthService_WSThread(XPVOID lParam);
bool XEngine_Client_WSTask(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE enOPCode);