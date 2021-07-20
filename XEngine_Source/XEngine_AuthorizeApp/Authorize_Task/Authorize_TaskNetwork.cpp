#include "pch.h"

//////////////////////////////////////////////////////////////////////////
BOOL __stdcall XEngine_Client_Accept(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	CXEngineAuthorizeAppDlg* pClass_This = (CXEngineAuthorizeAppDlg*)lParam;
	HelpComponents_Datas_CreateEx(xhPacket, lpszClientAddr, 0);
	return TRUE;
}
void __stdcall XEngine_Client_Recv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	CXEngineAuthorizeAppDlg* pClass_This = (CXEngineAuthorizeAppDlg*)lParam;
	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, lpszRecvMsg, nMsgLen);
}
void __stdcall XEngine_Client_Close(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_CloseClient(lpszClientAddr, lParam);
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_CloseClient(LPCTSTR lpszClientAddr, LPVOID lParam)
{
	CXEngineAuthorizeAppDlg* pClass_This = (CXEngineAuthorizeAppDlg*)lParam;

	TCHAR tszClientUser[64];
	memset(tszClientUser, '\0', sizeof(tszClientUser));

	if (AuthService_Session_GetUserForAddr(lpszClientAddr, tszClientUser))
	{
		for (int i = 0; i < pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemCount(); i++)
		{
			CString m_StrAddr = pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemText(i, 2);
			if (0 == _tcsnicmp(lpszClientAddr, m_StrAddr.GetBuffer(), _tcslen(lpszClientAddr)))
			{
				pClass_This->m_DlgUser.m_ListCtrlOnlineClient.DeleteItem(i);
			}
		}
		AUTHREG_PROTOCOL_TIME st_TimeProtocol;
		memset(&st_TimeProtocol, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

		if (AuthService_Session_GetTimer(tszClientUser, &st_TimeProtocol))
		{
			AuthService_SQLPacket_UserLeave(&st_TimeProtocol);
		}
		AuthService_Session_CloseClient(tszClientUser);
	}
	HelpComponents_Datas_DeleteEx(xhPacket, lpszClientAddr);
	XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，离开服务器"), lpszClientAddr, tszClientUser);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_Client_TaskSend(LPCTSTR lpszClientAddr, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPVOID lParam, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	CXEngineAuthorizeAppDlg* pClass_This = (CXEngineAuthorizeAppDlg*)lParam;

	if (BST_CHECKED == pClass_This->m_DlgConfig.m_RadioKeyPass.GetCheck())
	{
		CString m_StrPass;
		pClass_This->m_DlgConfig.m_EditPass.GetWindowText(m_StrPass);
		XEngine_SendMsg(lpszClientAddr, pSt_ProtocolHdr, lpszMsgBuffer, nMsgLen, m_StrPass.GetBuffer());
	}
	else
	{
		XEngine_SendMsg(lpszClientAddr, pSt_ProtocolHdr, lpszMsgBuffer, nMsgLen);
	}
	return TRUE;
}
BOOL XEngine_SendMsg(LPCTSTR lpszClientAddr, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszMsgBuffer, int nMsgLen, LPCTSTR lpszPass)
{
	int nSDLen = 4096;
	TCHAR tszMsgBuffer[4096];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

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
			nMsgLen = nSDLen;
		}
	}
	NetCore_TCPXCore_SendEx(xhSocket, lpszClientAddr, tszMsgBuffer, nSDLen);
	return TRUE;
}