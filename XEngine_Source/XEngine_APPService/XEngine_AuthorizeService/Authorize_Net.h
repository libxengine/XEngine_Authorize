#pragma once

BOOL __stdcall XEngine_Client_TCPAccept(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void __stdcall XEngine_Client_TCPRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam);
void __stdcall XEngine_Client_TCPClose(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);

BOOL __stdcall XEngine_Client_WSAccept(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void __stdcall XEngine_Client_WSRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam);
void __stdcall XEngine_Client_WSClose(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);

BOOL __stdcall XEngine_Client_HttpAccept(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void __stdcall XEngine_Client_HttpRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam);
void __stdcall XEngine_Client_HttpClose(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);

BOOL XEngine_CloseClient(LPCTSTR lpszClientAddr);

BOOL XEngine_Client_TaskSend(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, int nNetType);
BOOL XEngine_SendMsg(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, int nNetType, LPCTSTR lpszPass = NULL);