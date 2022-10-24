#include "Authorize_Hdr.h"

XHTHREAD CALLBACK XEngine_AuthService_TCPThread(LPVOID lParam)
{
	int nPoolIndex = *(int*)lParam;
	int nThreadPos = nPoolIndex + 1;

	while (bIsRun)
	{
		if (!HelpComponents_Datas_WaitEventEx(xhTCPPacket, nThreadPos))
		{
			continue;
		}
		int nMsgLen = 2048;
		TCHAR tszMsgBuffer[2048];
		XENGINE_PROTOCOLHDR st_ProtocolHdr;

		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

		int nListCount = 0;
		HELPCOMPONENT_PACKET_CLIENT** ppSt_ListClient;
		HelpComponents_Datas_GetPoolEx(xhTCPPacket, nThreadPos, &ppSt_ListClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			if (!HelpComponents_Datas_GetEx(xhTCPPacket, ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
			{
				continue;
			}
			if (st_AuthConfig.st_XCrypto.bEnable && (ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_XCRYPT == st_ProtocolHdr.wCrypto))
			{
				TCHAR tszPassword[64];
				TCHAR tszDeBuffer[2048];

				memset(tszPassword, '\0', sizeof(tszPassword));
				memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

				_stprintf(tszPassword, _T("%d"), st_AuthConfig.st_XCrypto.nPassword);
				OPenSsl_XCrypto_Decoder(tszMsgBuffer, &nMsgLen, tszDeBuffer, tszPassword);
				XEngine_Client_TCPTask(ppSt_ListClient[i]->tszClientAddr, tszDeBuffer, nMsgLen, &st_ProtocolHdr, XENGINE_AUTH_APP_NETTYPE_TCP);
			}
			else
			{
				XEngine_Client_TCPTask(ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, nMsgLen, &st_ProtocolHdr, XENGINE_AUTH_APP_NETTYPE_TCP);
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	return 0;
}

BOOL XEngine_Client_TCPTask(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, int nNetType)
{
	int nSDLen = 0;
	TCHAR tszSDBuffer[2048];
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_USERTABLE st_UserTable;
		XENGINE_PROTOCOL_USERAUTH st_AuthProtocol;

		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_AuthProtocol, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));
		memcpy(&st_AuthProtocol, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERAUTH));

		pSt_ProtocolHdr->unPacketSize = 0;
		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN;

		if (ENUM_PROTOCOL_FOR_DEVICE_TYPE_UNKNOW == st_AuthProtocol.enDeviceType)
		{
			pSt_ProtocolHdr->wReserve = 250;
			Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，没有填写设备类型,无法继续"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		//是否使用了第三方验证
		if (st_AuthConfig.st_XLogin.bPassAuth)
		{
			//启用三方验证
			int nHTTPCode = 0;
			int nHTTPLen = 0;
			TCHAR* ptszMsgBuffer = NULL;
			Protocol_Packet_HttpUserPass(tszSDBuffer, &nSDLen, &st_AuthProtocol);
			APIHelp_HttpRequest_Custom(_T("POST"), st_AuthConfig.st_XLogin.st_PassUrl.tszPassLogin, tszSDBuffer, &nHTTPCode, &ptszMsgBuffer, &nHTTPLen);
			if (200 != nHTTPCode)
			{
				pSt_ProtocolHdr->wReserve = 251;
				Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，三方验证失败,错误码:%d"), lpszClientAddr, st_AuthProtocol.tszUserName, nHTTPCode);
				return FALSE;
			}
			Protocol_Parse_HttpParseTable(ptszMsgBuffer, nHTTPLen, &st_UserTable);
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		else
		{
			if (!Database_SQLite_UserQuery(st_AuthProtocol.tszUserName, &st_UserTable))
			{
				pSt_ProtocolHdr->wReserve = 251;
				Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，用户名不存在"), lpszClientAddr, st_AuthProtocol.tszUserName);
				return FALSE;
			}
			if ((_tcslen(st_AuthProtocol.tszUserPass) != _tcslen(st_UserTable.st_UserInfo.tszUserPass)) || (0 != _tcsncmp(st_AuthProtocol.tszUserPass, st_UserTable.st_UserInfo.tszUserPass, _tcslen(st_AuthProtocol.tszUserPass))))
			{
				pSt_ProtocolHdr->wReserve = 252;
				Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，密码错误"), lpszClientAddr, st_AuthProtocol.tszUserName);
				return FALSE;
			}
		}
		//是否已经登录
		TCHAR tszClientAddr[128];
		if (Session_Authorize_GetAddrForUser(st_AuthProtocol.tszUserName, tszClientAddr))
		{
			pSt_ProtocolHdr->wReserve = 253;
			Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，用户名已经登录"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		//是否被封禁
		if (-1 == st_UserTable.st_UserInfo.nUserLevel)
		{
			pSt_ProtocolHdr->wReserve = 254;
			Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，客户端已被封禁"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		//分析充值类型
		if ((ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_UserTable.enSerialType) || ('0' == st_UserTable.tszLeftTime[0]))
		{
			pSt_ProtocolHdr->wReserve = 255;
			Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，客户端类型错误"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		st_UserTable.enDeviceType = st_AuthProtocol.enDeviceType;
		if (!Session_Authorize_Insert(lpszClientAddr, &st_UserTable, nNetType))
		{
			pSt_ProtocolHdr->wReserve = 256;
			Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，插入会话管理失败,错误:%lX"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		pSt_ProtocolHdr->wReserve = 0;
		Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，登录成功,注册类型:%s,剩余时间:%s"), lpszClientAddr, st_AuthProtocol.tszUserName, lpszXSerialType[st_UserTable.enSerialType], st_UserTable.tszLeftTime);
	}
	return TRUE;
}