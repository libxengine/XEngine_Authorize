#include "Authorize_Hdr.h"

XHTHREAD CALLBACK XEngine_AuthService_TCPThread(LPVOID lParam)
{
	XENGINE_THREADINFO* pSt_ThreadInfo = (XENGINE_THREADINFO*)lParam;
	int nThreadPos = pSt_ThreadInfo->nPoolIndex + 1;

	while (bIsRun)
	{
		if (!HelpComponents_Datas_WaitEventEx(xhPacket, nThreadPos))
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
		HelpComponents_Datas_GetPoolEx(xhPacket, nThreadPos, &ppSt_ListClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			if (!HelpComponents_Datas_GetEx(xhPacket, ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
			{
				continue;
			}
			if (st_AuthConfig.st_Crypto.bEnable && (ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_XCRYPT == st_ProtocolHdr.wCrypto))
			{
				TCHAR tszPassword[64];
				TCHAR tszDeBuffer[2048];

				memset(tszPassword, '\0', sizeof(tszPassword));
				memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

				_stprintf(tszPassword, _T("%d"), st_AuthConfig.st_Crypto.nPassword);
				OPenSsl_XCrypto_Decoder(tszMsgBuffer, &nMsgLen, tszDeBuffer, tszPassword);
				XEngine_Client_TCPTask(ppSt_ListClient[i]->tszClientAddr, tszDeBuffer, nMsgLen, &st_ProtocolHdr);
			}
			else
			{
				XEngine_Client_TCPTask(ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, nMsgLen, &st_ProtocolHdr);
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	return 0;
}

BOOL XEngine_Client_TCPTask(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr)
{
	//判断协议头和尾部
	if ((XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER != pSt_ProtocolHdr->wHeader) || (XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL != pSt_ProtocolHdr->wTail) || (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH != pSt_ProtocolHdr->unOperatorType))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，协议错误"), lpszClientAddr);
		return FALSE;
	}
	//用户删除
	if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQDEL == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_USERTABLE st_UserTable;
		XENGINE_PROTOCOL_USERINFO st_UserInfo;

		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));

		memcpy(&st_UserInfo, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERINFO));

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPDEL;
		if (!AuthService_SQLPacket_UserQuery(st_UserInfo.tszUserName, &st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 211;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，用户不存在"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
			return FALSE;
		}
		if ((st_UserTable.st_UserInfo.nIDNumber != st_UserInfo.nIDNumber) || (0 != _tcsncmp(st_UserTable.st_UserInfo.tszEMailAddr, st_UserInfo.tszEMailAddr, _tcslen(st_UserTable.st_UserInfo.tszEMailAddr))) || (0 != _tcsncmp(st_UserTable.st_UserInfo.tszUserPass, st_UserInfo.tszUserPass, _tcslen(st_UserTable.st_UserInfo.tszUserPass))))
		{
			pSt_ProtocolHdr->wReserve = 212;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，注销失败,验证信息错误"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
			return FALSE;
		}
		//从数据库删除文件
		if (!AuthService_SQLPacket_UserDelete(st_UserInfo.tszUserName))
		{
			pSt_ProtocolHdr->wReserve = 213;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，注销失败,删除信息失败"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
			return FALSE;
		}
		pSt_ProtocolHdr->unPacketSize = 0;
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，注销成功"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQREGISTER == pSt_ProtocolHdr->unOperatorCode)
	{
		//用户注册
		AUTHREG_USERTABLE st_UserTable;
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		memcpy(&st_UserTable.st_UserInfo, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERINFO));
		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPREGISTER;
		//填充写入数据
		_stprintf(st_UserTable.tszLeftTime, _T("%d"), st_AuthConfig.st_Verification.nTryTime);
		st_UserTable.en_AuthRegSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)st_AuthConfig.st_Verification.nTryMode;
		if (AuthService_SQLPacket_UserRegister(&st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 0;
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，注册成功"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
		}
		else
		{
			pSt_ProtocolHdr->wReserve = 231;
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，注册失败，无法继续，错误：%X"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName, SQLPacket_GetLastError());
		}
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_USERTABLE st_UserTable;
		XENGINE_PROTOCOL_USERAUTH st_AuthProtocol;

		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_AuthProtocol, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));
		memcpy(&st_AuthProtocol, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERAUTH));

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN;
		if (!AuthService_SQLPacket_UserQuery(st_AuthProtocol.tszUserName, &st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 251;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，用户名不存在"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		if (0 != _tcsncmp(st_AuthProtocol.tszUserPass, st_UserTable.st_UserInfo.tszUserPass, _tcslen(st_AuthProtocol.tszUserPass)))
		{
			pSt_ProtocolHdr->wReserve = 252;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，密码错误"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		//分析充值类型
		if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_UserTable.en_AuthRegSerialType)
		{
			pSt_ProtocolHdr->wReserve = 253;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，客户端类型错误"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		if ('0' == st_UserTable.tszLeftTime[0])
		{
			pSt_ProtocolHdr->wReserve = 254;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，没有剩余时间了"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		if (!AuthService_Session_Insert(lpszClientAddr, &st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 255;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，插入会话管理失败,错误:%lX"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		pSt_ProtocolHdr->wReserve = 0;
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录成功"), lpszClientAddr, st_AuthProtocol.tszUserName);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQPAY == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_PROTOCOL_USERPAY st_UserPay;
		memset(&st_UserPay, '\0', sizeof(AUTHREG_PROTOCOL_USERPAY));
		memcpy(&st_UserPay, lpszMsgBuffer, sizeof(AUTHREG_PROTOCOL_USERPAY));

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPPAY;
		if (AuthService_SQLPacket_UserPay(st_UserPay.tszUserName, st_UserPay.tszSerialNumber))
		{
			AUTHREG_USERTABLE st_UserInfo;
			memset(&st_UserInfo, '\0', sizeof(AUTHREG_USERTABLE));
			if (AuthService_SQLPacket_UserQuery(st_UserPay.tszUserName, &st_UserInfo))
			{
				AuthService_Session_SetUser(&st_UserInfo);
			}
			pSt_ProtocolHdr->wReserve = 0;
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，充值成功，序列号：%s"), lpszClientAddr, st_UserPay.tszUserName, st_UserPay.tszSerialNumber);
		}
		else
		{
			pSt_ProtocolHdr->wReserve = 271;
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，充值失败，无法继续，错误：%X"), lpszClientAddr, st_UserPay.tszUserName, SQLPacket_GetLastError());
		}
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETPASS == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_USERTABLE st_UserTable;
		XENGINE_PROTOCOL_USERINFO st_UserInfo;
		XENGINE_PROTOCOL_USERAUTH st_AuthProtocol;

		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));
		memset(&st_AuthProtocol, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

		memcpy(&st_UserInfo, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERINFO));

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETPASS;
		if (!AuthService_SQLPacket_UserQuery(st_UserInfo.tszUserName, &st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 291;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，找回密码失败，用户不存在"), lpszClientAddr, st_UserInfo.tszUserName);
			return FALSE;
		}
		//安全验证判断
		if ((0 != _tcsnicmp(st_UserInfo.tszEMailAddr, st_UserTable.st_UserInfo.tszEMailAddr, _tcslen(st_UserInfo.tszEMailAddr))) || (st_UserTable.st_UserInfo.nIDNumber != st_UserInfo.nIDNumber))
		{
			pSt_ProtocolHdr->wReserve = 292;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，找回密码失败，验证信息失败"), lpszClientAddr, st_UserInfo.tszUserName);
			return FALSE;
		}
		_tcscpy(st_AuthProtocol.tszUserName, st_UserTable.st_UserInfo.tszUserName);
		_tcscpy(st_AuthProtocol.tszUserPass, st_UserTable.st_UserInfo.tszUserPass);

		pSt_ProtocolHdr->wReserve = 0;
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, (LPCTSTR)&st_AuthProtocol, sizeof(XENGINE_PROTOCOL_USERAUTH));
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，找回密码成功"), lpszClientAddr, st_UserInfo.tszUserName);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETTIME == pSt_ProtocolHdr->unOperatorCode)
	{
		TCHAR tszUserName[128];
		AUTHREG_PROTOCOL_TIME st_AuthTime;

		memset(tszUserName, '\0', sizeof(tszUserName));
		memset(&st_AuthTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETTIME;
		AuthService_Session_GetUserForAddr(lpszClientAddr, tszUserName);
		if (AuthService_Session_GetTimer(tszUserName, &st_AuthTime))
		{
			pSt_ProtocolHdr->wReserve = 0;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, (LPCTSTR)&st_AuthTime, sizeof(AUTHREG_PROTOCOL_TIME));
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，获取时间成功，类型：%d，在线时间：%ld，剩余时间：%ld"), lpszClientAddr, st_AuthTime.tszUserName, st_AuthTime.enSerialType, st_AuthTime.nTimeONLine, st_AuthTime.nTimeLeft);
		}
		else
		{
			pSt_ProtocolHdr->wReserve = 0x2B1;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，获取时间失败，无法继续，错误：%X"), lpszClientAddr, st_AuthTime.tszUserName, Session_GetLastError());
		}
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_NOTIFYMSG == pSt_ProtocolHdr->unOperatorCode)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，发送数据，大小：%d，内容：%s"), lpszClientAddr, pSt_ProtocolHdr->unPacketSize, lpszMsgBuffer);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQTRYVER == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_NETVER st_AuthVer;
		memset(&st_AuthVer, '\0', sizeof(AUTHREG_NETVER));

		memcpy(st_AuthVer.tszVerSerial, lpszMsgBuffer, pSt_ProtocolHdr->unPacketSize);

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPTRYVER;
		if (AuthService_SQLPacket_TryQuery(&st_AuthVer))
		{
			TCHAR tszEndTime[64];
			__int64x nTimeSpan = 0;
			memset(tszEndTime, '\0', sizeof(tszEndTime));
			//根据方式来计算剩余时间
			if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == st_AuthVer.enVerMode)
			{
				nTimeSpan = st_AuthVer.nTryTime;
			}
			else
			{
				//计算时间差
				BaseLib_OperatorTime_ToStringTimer(tszEndTime);
				BaseLib_OperatorTimeSpan_GetForStr(st_AuthVer.tszVerData, tszEndTime, &nTimeSpan, 2);
			}
			//是否超过
			if (nTimeSpan > st_AuthConfig.st_Verification.nVerTime)
			{
				pSt_ProtocolHdr->wReserve = 0x2D1;
				pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_TIMEDOUT;
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，序列号：%s，临时验证失败，此序列号已经试用到期"), lpszClientAddr, st_AuthVer.tszVerSerial);
			}
			else
			{
				pSt_ProtocolHdr->wReserve = 0;
				if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == st_AuthVer.enVerMode)
				{
					//次数卡需要更新才可以
					st_AuthVer.nTryTime++;
					AuthService_SQLPacket_TrySet(&st_AuthVer);
				}
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，序列号：%s，临时验证成功，已用时间:%lld"), lpszClientAddr, st_AuthVer.tszVerSerial, nTimeSpan);
			}
		}
		else
		{
			//填充写入数据
			st_AuthVer.nTryTime = st_AuthConfig.st_Verification.nVerTime;
			st_AuthVer.enVerMode = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)st_AuthConfig.st_Verification.nVerMode;
			//看下是否启用了此功能
			if ((ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_AuthVer.enVerMode) || (st_AuthVer.nTryTime <= 0))
			{
				pSt_ProtocolHdr->wReserve = 0x2D2;
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("客户端：%s，序列号：%s，临时验证插入失败，因为服务器关闭了此功能"), lpszClientAddr, st_AuthVer.tszVerSerial);
			}
			else
			{
				if (AuthService_SQLPacket_TryInsert(&st_AuthVer))
				{
					pSt_ProtocolHdr->unPacketSize = 0;
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，序列号：%s，临时验证插入成功"), lpszClientAddr, st_AuthVer.tszVerSerial);
				}
				else
				{
					pSt_ProtocolHdr->wReserve = 0x2D3;
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，序列号：%s，临时验证插入失败，无法继续，错误：%X"), lpszClientAddr, st_AuthVer.tszVerSerial, SQLPacket_GetLastError());
				}
			}
		}
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETUSER == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_USERTABLE st_UserTable;
		XENGINE_PROTOCOL_USERINFO st_UserInfo;

		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));

		memcpy(&st_UserInfo, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERINFO));

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETUSER;
		if (AuthService_SQLPacket_UserQuery(st_UserInfo.tszUserName, &st_UserTable))
		{
			if ((0 == _tcsncmp(st_UserInfo.tszUserPass, st_UserTable.st_UserInfo.tszUserPass, _tcslen(st_UserInfo.tszUserPass))) && (0 != _tcsnicmp(st_UserInfo.tszEMailAddr, st_UserTable.st_UserInfo.tszEMailAddr, _tcslen(st_UserInfo.tszEMailAddr))) && (st_UserInfo.nIDNumber == st_UserTable.st_UserInfo.nIDNumber))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，请求查询用户信息成功"), lpszClientAddr, st_UserInfo.tszUserName);
			}
			else
			{
				pSt_ProtocolHdr->wReserve = 2100;
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，请求查询用户信息失败,密码错误"), lpszClientAddr, st_UserInfo.tszUserName);
			}
		}
		else
		{
			pSt_ProtocolHdr->wReserve = 2101;
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，请求查询用户信息失败,帐户不存在"), lpszClientAddr, st_UserInfo.tszUserName);
		}
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQSETUSER == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_USERTABLE st_SQLTable;
		XENGINE_PROTOCOL_USERINFO st_UserInfo;

		memset(&st_SQLTable, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));

		memcpy(&st_UserInfo, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERINFO));

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPSETUSER;
		if (AuthService_SQLPacket_UserQuery(st_UserInfo.tszUserName, &st_SQLTable))
		{
			if ((0 == _tcsncmp(st_UserInfo.tszUserPass, st_SQLTable.st_UserInfo.tszUserPass, _tcslen(st_UserInfo.tszUserPass))) && (0 != _tcsnicmp(st_UserInfo.tszEMailAddr, st_SQLTable.st_UserInfo.tszEMailAddr, _tcslen(st_UserInfo.tszEMailAddr))) && (st_UserInfo.nIDNumber == st_SQLTable.st_UserInfo.nIDNumber))
			{
				memcpy(&st_SQLTable.st_UserInfo, &st_UserInfo, sizeof(XENGINE_PROTOCOL_USERINFO));
				if (AuthService_SQLPacket_UserSet(&st_SQLTable))
				{
					pSt_ProtocolHdr->wReserve = 0;
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，请求设置用户信息成功\r\n"), lpszClientAddr, st_UserInfo.tszUserName);

				}
				else
				{
					pSt_ProtocolHdr->wReserve = 2120;
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，请求设置用户信息失败,错误:%lX\r\n"), lpszClientAddr, st_UserInfo.tszUserName, SQLPacket_GetLastError());
				}
			}
			else
			{
				pSt_ProtocolHdr->wReserve = 2121;
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，请求设置用户信息失败,信息不匹配\r\n"), lpszClientAddr, st_UserInfo.tszUserName);
			}
		}
		else
		{
			pSt_ProtocolHdr->wReserve = 2122;
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，请求设置用户信息失败,错误:%lX\r\n"), lpszClientAddr, st_UserInfo.tszUserName, SQLPacket_GetLastError());
		}
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr);
	}
	return TRUE;
}