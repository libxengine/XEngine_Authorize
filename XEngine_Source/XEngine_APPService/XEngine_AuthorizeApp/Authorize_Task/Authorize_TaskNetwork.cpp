#include "pch.h"
//////////////////////////////////////////////////////////////////////////
BOOL __stdcall XEngine_Client_TCPAccept(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	CXEngineAuthorizeAppDlg* pClass_This = (CXEngineAuthorizeAppDlg*)lParam;
	HelpComponents_Datas_CreateEx(xhTCPPacket, lpszClientAddr, 0);
	return TRUE;
}
void __stdcall XEngine_Client_TCPRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	CXEngineAuthorizeAppDlg* pClass_This = (CXEngineAuthorizeAppDlg*)lParam;
	HelpComponents_Datas_PostEx(xhTCPPacket, lpszClientAddr, lpszRecvMsg, nMsgLen);
}
void __stdcall XEngine_Client_TCPClose(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr, lParam);
}
BOOL __stdcall XEngine_Client_WSAccept(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	RfcComponents_WSPacket_CreateEx(xhWSPacket, lpszClientAddr, 0);
	return TRUE;
}
void __stdcall XEngine_Client_WSRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	BOOL bLogin = FALSE;
	RfcComponents_WSPacket_GetLoginEx(xhWSPacket, lpszClientAddr, &bLogin);
	if (bLogin)
	{
		RfcComponents_WSPacket_PostEx(xhWSPacket, lpszClientAddr, lpszRecvMsg, nMsgLen);
	}
	else
	{
		int nSDLen = nMsgLen;
		TCHAR tszHandsBuffer[1024];
		memset(tszHandsBuffer, '\0', sizeof(tszHandsBuffer));

		RfcComponents_WSConnector_HandShake(lpszRecvMsg, &nSDLen, tszHandsBuffer);
		NetCore_TCPXCore_SendEx(xhWSSocket, lpszClientAddr, tszHandsBuffer, nSDLen);
		RfcComponents_WSPacket_SetLoginEx(xhWSPacket, lpszClientAddr);
	}
}
void __stdcall XEngine_Client_WSClose(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr, lParam);
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_CloseClient(LPCTSTR lpszClientAddr, LPVOID lParam)
{
	CXEngineAuthorizeAppDlg* pClass_This = (CXEngineAuthorizeAppDlg*)lParam;

	TCHAR tszClientUser[64];
	memset(tszClientUser, '\0', sizeof(tszClientUser));

	if (Session_Authorize_GetUserForAddr(lpszClientAddr, tszClientUser))
	{
		for (int i = 0; i < pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemCount(); i++)
		{
			CString m_StrAddr = pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemText(i, 2);
			if (0 == _tcsnicmp(lpszClientAddr, m_StrAddr.GetBuffer(), _tcslen(lpszClientAddr)))
			{
				if (BST_CHECKED == pClass_This->m_DlgUser.m_CheckAllUser.GetCheck())
				{
					pClass_This->m_DlgUser.m_ListCtrlOnlineClient.SetItemText(i, 7, _T("离线"));
				}
				else
				{
					pClass_This->m_DlgUser.m_ListCtrlOnlineClient.DeleteItem(i);
				}
			}
		}
		AUTHREG_PROTOCOL_TIME st_TimeProtocol;
		memset(&st_TimeProtocol, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

		if (Session_Authorize_GetTimer(tszClientUser, &st_TimeProtocol))
		{
			Database_SQLite_UserLeave(&st_TimeProtocol);
		}
		Session_Authorize_CloseClient(tszClientUser);
	}
	HelpComponents_Datas_DeleteEx(xhTCPPacket, lpszClientAddr);
	RfcComponents_WSPacket_DeleteEx(xhWSPacket, lpszClientAddr);
	NetCore_TCPXCore_CloseForClientEx(xhTCPSocket, lpszClientAddr);
	NetCore_TCPXCore_CloseForClientEx(xhWSSocket, lpszClientAddr);
	XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，离开服务器"), lpszClientAddr, tszClientUser);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_Client_TaskSend(LPCTSTR lpszClientAddr, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPVOID lParam, int nNetType, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	CXEngineAuthorizeAppDlg* pClass_This = (CXEngineAuthorizeAppDlg*)lParam;

	if (BST_CHECKED == pClass_This->m_DlgConfig.m_RadioKeyPass.GetCheck())
	{
		CString m_StrPass;
		pClass_This->m_DlgConfig.m_EditPass.GetWindowText(m_StrPass);
		XEngine_SendMsg(lpszClientAddr, pSt_ProtocolHdr, nNetType, lpszMsgBuffer, nMsgLen, m_StrPass.GetBuffer());
	}
	else
	{
		XEngine_SendMsg(lpszClientAddr, pSt_ProtocolHdr, nNetType, lpszMsgBuffer, nMsgLen);
	}
	return TRUE;
}
BOOL XEngine_SendMsg(LPCTSTR lpszClientAddr, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, int nNetType, LPCTSTR lpszMsgBuffer, int nMsgLen, LPCTSTR lpszPass)
{
	int nSDLen = 4096;
	TCHAR tszMsgBuffer[4096];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (XENGINE_AUTH_APP_NETTYPE_WS == nNetType)
	{
		TCHAR tszWSBuffer[4096];
		memset(tszWSBuffer, '\0', sizeof(tszWSBuffer));

		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETPASS == pSt_ProtocolHdr->unOperatorCode)
		{
			Protocol_Packet_WSPktAuth(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr, (XENGINE_PROTOCOL_USERAUTH*)lpszMsgBuffer);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETUSER == pSt_ProtocolHdr->unOperatorCode)
		{
			Protocol_Packet_WSPktInfo(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr, (XENGINE_PROTOCOL_USERINFO*)lpszMsgBuffer);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETTIME == pSt_ProtocolHdr->unOperatorCode)
		{
			Protocol_Packet_WSPktTime(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr, (AUTHREG_PROTOCOL_TIME*)lpszMsgBuffer);
		}
		else
		{
			Protocol_Packet_WSPkt(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr);
		}
		RfcComponents_WSCodec_EncodeMsg(tszMsgBuffer, tszWSBuffer, &nSDLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_TEXT);
		NetCore_TCPXCore_SendEx(xhWSSocket, lpszClientAddr, tszWSBuffer, nSDLen);
	}
	else
	{
		if (NULL == lpszPass)
		{
			Protocol_Packet_SendPkt(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr, lpszMsgBuffer, nMsgLen);
		}
		else
		{
			TCHAR tszEnBuffer[4096];
			memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));

			pSt_ProtocolHdr->wCrypto = ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_XCRYPT;
			if (NULL == lpszMsgBuffer)
			{
				Protocol_Packet_SendPkt(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr);
			}
			else
			{
				OPenSsl_XCrypto_Encoder(lpszMsgBuffer, &nMsgLen, (UCHAR*)tszEnBuffer, lpszPass);
				Protocol_Packet_SendPkt(tszMsgBuffer, &nSDLen, pSt_ProtocolHdr, tszEnBuffer, nMsgLen);
			}
		}
		NetCore_TCPXCore_SendEx(xhTCPSocket, lpszClientAddr, tszMsgBuffer, nSDLen);
	}
	return TRUE;
}