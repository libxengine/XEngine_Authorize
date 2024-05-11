#include "Authorize_Hdr.h"

XHTHREAD CALLBACK XEngine_AuthService_TCPThread(XPVOID lParam)
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
		XCHAR tszMsgBuffer[2048];
		XENGINE_PROTOCOLHDR st_ProtocolHdr;

		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

		int nListCount = 0;
		XENGINE_MANAGEPOOL_TASKEVENT** ppSt_ListClient;
		HelpComponents_Datas_GetPoolEx(xhTCPPacket, nThreadPos, &ppSt_ListClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			if (!HelpComponents_Datas_GetEx(xhTCPPacket, ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
			{
				continue;
			}
			if (st_AuthConfig.st_XCrypto.bEnable && (ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_XCRYPT == st_ProtocolHdr.wCrypto))
			{
				XCHAR tszPassword[64];
				XCHAR tszDeBuffer[2048];

				memset(tszPassword, '\0', sizeof(tszPassword));
				memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

				_xstprintf(tszPassword, _X("%d"), st_AuthConfig.st_XCrypto.nPassword);
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

bool XEngine_Client_TCPTask(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, int nNetType)
{
	int nSDLen = 0;
	XCHAR tszSDBuffer[2048];
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	AUTHREG_BANNED st_Banned;
	memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));

	_tcsxcpy(st_Banned.tszIPAddr, lpszClientAddr);
	BaseLib_OperatorIPAddr_SegAddr(st_Banned.tszIPAddr);
	//是否在黑名单
	if (Database_SQLite_BannedExist(&st_Banned))
	{
		pSt_ProtocolHdr->wReserve = 423;
		Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，登录连接被阻止，IP地址被禁用!"), lpszClientAddr);
		return false;
	}

	if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_USERTABLE st_UserTable;
		XENGINE_PROTOCOL_USERAUTH st_AuthProtocol;

		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_AuthProtocol, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));
		memcpy(&st_AuthProtocol, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERAUTH));

		pSt_ProtocolHdr->unPacketSize = 0;
		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN;
		//是否允许登录
		if (!st_FunSwitch.bSwitchLogin)
		{
			pSt_ProtocolHdr->wReserve = 503;
			Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，登录失败，因为登录功能被服务器关闭!"), lpszClientAddr);
			return false;
		}
		if (ENUM_PROTOCOL_FOR_DEVICE_TYPE_UNKNOW == st_AuthProtocol.enDeviceType)
		{
			pSt_ProtocolHdr->wReserve = 250;
			Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，用户名：%s，登录失败，没有填写设备类型,无法继续"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return false;
		}
		//是否启用了动态码
		if (st_FunSwitch.bSwitchDCode)
		{
			if (!AuthHelp_DynamicCode_Get(pSt_ProtocolHdr->xhToken, _ttxoi(st_AuthProtocol.tszDCode)))
			{
				pSt_ProtocolHdr->wReserve = 257;
				Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，用户名：%s，登录失败，验证动态码失败,句柄:%llu,动态码;%s,错误码:%lX"), lpszClientAddr, st_AuthProtocol.tszUserName, pSt_ProtocolHdr->xhToken, st_AuthProtocol.tszDCode, AuthHelp_GetLastError());
				return false;
			}
		}
		//是否使用了第三方验证
		if (st_AuthConfig.st_XLogin.bPassAuth)
		{
			//启用三方验证
			int nHTTPCode = 0;
			int nHTTPLen = 0;
			XCHAR* ptszMsgBuffer = NULL;
			Protocol_Packet_HttpUserPass(tszSDBuffer, &nSDLen, &st_AuthProtocol);
			APIClient_Http_Request(_X("POST"), st_AuthConfig.st_XLogin.st_PassUrl.tszPassLogin, tszSDBuffer, &nHTTPCode, &ptszMsgBuffer, &nHTTPLen);
			if (200 != nHTTPCode)
			{
				pSt_ProtocolHdr->wReserve = 251;
				Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，用户名：%s，登录失败，三方验证失败,错误码:%d"), lpszClientAddr, st_AuthProtocol.tszUserName, nHTTPCode);
				return false;
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
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，用户名：%s，登录失败，用户名不存在"), lpszClientAddr, st_AuthProtocol.tszUserName);
				return false;
			}
			if ((_tcsxlen(st_AuthProtocol.tszUserPass) != _tcsxlen(st_UserTable.st_UserInfo.tszUserPass)) || (0 != _tcsxncmp(st_AuthProtocol.tszUserPass, st_UserTable.st_UserInfo.tszUserPass, _tcsxlen(st_AuthProtocol.tszUserPass))))
			{
				pSt_ProtocolHdr->wReserve = 252;
				Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，用户名：%s，登录失败，密码错误"), lpszClientAddr, st_AuthProtocol.tszUserName);
				return false;
			}
		}
		//是否已经登录
		if (st_FunSwitch.bSwitchMulti)
		{
			bool bLogin = false;
			int nListCount = 0;
			AUTHSESSION_NETCLIENT** ppSt_ListClient;

			Session_Authorize_GetClient(&ppSt_ListClient, &nListCount, st_AuthProtocol.tszUserName);
			for (int i = 0; i < nListCount; i++)
			{
				//找到客户端,支持的模式
				if (0 == st_AuthConfig.st_XLogin.nMultiMode)
				{
					int nSourceType = 0;
					int nDestType = 0;
					AuthHelp_MultiLogin_GetRange(ppSt_ListClient[i]->st_UserTable.enDeviceType, &nSourceType);
					AuthHelp_MultiLogin_GetRange(st_AuthProtocol.enDeviceType, &nDestType);

					if (nSourceType == nDestType)
					{
						bLogin = true;
						break;
					}
				}
				else if (1 == st_AuthConfig.st_XLogin.nMultiMode)
				{
					if (ppSt_ListClient[i]->st_UserTable.enDeviceType == st_AuthProtocol.enDeviceType)
					{
						bLogin = true;
						break;
					}
				}
				else
				{
					pSt_ProtocolHdr->wReserve = 257;
					Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
					XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，用户名：%s，登录失败，服务端设置不正确"), lpszClientAddr, st_AuthProtocol.tszUserName);
					return false;
				}
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
			//对多端登录的类型进行验证
			if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_SECOND == st_UserTable.enSerialType)
			{
				if (!st_AuthConfig.st_XLogin.st_MulitLogin.bSecond)
				{
					bLogin = true;
				}
			}
			if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == st_UserTable.enSerialType)
			{
				if (!st_AuthConfig.st_XLogin.st_MulitLogin.bTime)
				{
					bLogin = true;
				}
			}
			if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_DAY == st_UserTable.enSerialType)
			{
				if (!st_AuthConfig.st_XLogin.st_MulitLogin.bDay)
				{
					bLogin = true;
				}
			}
			if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_CUSTOM == st_UserTable.enSerialType)
			{
				if (!st_AuthConfig.st_XLogin.st_MulitLogin.bCustom)
				{
					bLogin = true;
				}
			}
			//判断这次登录是否允许
			if (bLogin)
			{
				pSt_ProtocolHdr->wReserve = 253;
				Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，用户名：%s，多端登录失败，用户已经登录,类型:%d"), lpszClientAddr, st_AuthProtocol.tszUserName, st_AuthProtocol.enDeviceType);
				return false;
			}
		}
		else
		{
			int nListCount = 0;
			AUTHSESSION_NETCLIENT** ppSt_ListClient;
			if (Session_Authorize_GetClient(&ppSt_ListClient, &nListCount, st_AuthProtocol.tszUserName))
			{
				pSt_ProtocolHdr->wReserve = 253;
				BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
				Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，用户名：%s，登录失败，用户名已经登录"), lpszClientAddr, st_AuthProtocol.tszUserName);
				return false;
			}
		}
		//是否被封禁
		if (-1 == st_UserTable.st_UserInfo.nUserLevel)
		{
			pSt_ProtocolHdr->wReserve = 254;
			Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，用户名：%s，登录失败，客户端已被封禁"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return false;
		}
		//分析充值类型
		if ((ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_UserTable.enSerialType) || ('0' == st_UserTable.tszLeftTime[0]))
		{
			pSt_ProtocolHdr->wReserve = 255;
			Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，用户名：%s，登录失败，客户端时间已经耗尽,需要充值才能使用"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return false;
		}
		//如果是次数卡,需要优先处理
		if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == st_UserTable.enSerialType)
		{
			__int64x nTime = _ttxoll(st_UserTable.tszLeftTime) - 1;
			_xtprintf(st_UserTable.tszLeftTime, _X("%lld"), nTime);

			Database_SQLite_UserSet(&st_UserTable);
		}
		st_UserTable.enDeviceType = st_AuthProtocol.enDeviceType;
		if (!Session_Authorize_Insert(lpszClientAddr, &st_UserTable, nNetType))
		{
			pSt_ProtocolHdr->wReserve = 256;
			Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，用户名：%s，登录失败，插入会话管理失败,错误:%lX"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return false;
		}
		pSt_ProtocolHdr->wReserve = 0;
		Protocol_Packet_HDRComm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, nNetType);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("客户端：%s，用户名：%s，登录成功,注册类型:%s,剩余时间:%s"), lpszClientAddr, st_AuthProtocol.tszUserName, lpszXSerialType[st_UserTable.enSerialType], st_UserTable.tszLeftTime);
	}
	return true;
}