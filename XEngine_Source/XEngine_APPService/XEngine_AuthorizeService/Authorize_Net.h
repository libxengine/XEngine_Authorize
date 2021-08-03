#pragma once

BOOL __stdcall XEngine_Client_Accept(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void __stdcall XEngine_Client_Recv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam);
void __stdcall XEngine_Client_Close(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);

BOOL XEngine_CloseClient(LPCTSTR lpszClientAddr);

BOOL XEngine_Client_TaskSend(LPCTSTR lpszClientAddr, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer = NULL, int nMsgLen = 0);
BOOL XEngine_SendMsg(LPCTSTR lpszClientAddr, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer = NULL, int nMsgLen = 0, LPCTSTR lpszPass = NULL);