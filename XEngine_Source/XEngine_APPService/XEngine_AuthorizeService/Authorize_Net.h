#pragma once

XBOOL CALLBACK XEngine_Client_TCPAccept(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void CALLBACK XEngine_Client_TCPRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void CALLBACK XEngine_Client_TCPClose(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);

XBOOL CALLBACK XEngine_Client_WSAccept(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void CALLBACK XEngine_Client_WSRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void CALLBACK XEngine_Client_WSClose(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);

XBOOL CALLBACK XEngine_Client_HttpAccept(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void CALLBACK XEngine_Client_HttpRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void CALLBACK XEngine_Client_HttpClose(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);

XBOOL XEngine_CloseClient(LPCXSTR lpszClientAddr);

XBOOL XEngine_Client_TaskSend(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nNetType);
XBOOL XEngine_SendMsg(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nNetType, LPCXSTR lpszPass = NULL);