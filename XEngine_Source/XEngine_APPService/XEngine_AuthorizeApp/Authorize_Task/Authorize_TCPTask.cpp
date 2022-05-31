#include "pch.h"

XHTHREAD CALLBACK XEngine_AuthService_ThreadClient(LPVOID lParam)
{
	XENGINE_THREADINFO* pSt_ThreadInfo = (XENGINE_THREADINFO*)lParam;
	CXEngineAuthorizeAppDlg* pClass_This = (CXEngineAuthorizeAppDlg*)pSt_ThreadInfo->lPClass;
	int nThreadPos = pSt_ThreadInfo->nPoolIndex + 1;

	while (pClass_This->bThread)
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
			if ((BST_CHECKED == pClass_This->m_DlgConfig.m_RadioKeyPass.GetCheck()) && (ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_XCRYPT == st_ProtocolHdr.wCrypto))
			{
				CString m_StrPass;
				TCHAR tszDeBuffer[2048];
				memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

				pClass_This->m_DlgConfig.m_EditPass.GetWindowText(m_StrPass);
				OPenSsl_XCrypto_Decoder(tszMsgBuffer, &nMsgLen, tszDeBuffer, m_StrPass.GetBuffer());
				XEngine_Client_TaskHandle(ppSt_ListClient[i]->tszClientAddr, tszDeBuffer, nMsgLen, &st_ProtocolHdr, pSt_ThreadInfo->lPClass, XENGINE_AUTH_APP_NETTYPE_TCP);
			}
			else
			{
				XEngine_Client_TaskHandle(ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, nMsgLen, &st_ProtocolHdr, pSt_ThreadInfo->lPClass, XENGINE_AUTH_APP_NETTYPE_TCP);
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	return 0;
}

BOOL XEngine_Client_TaskHandle(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPVOID lParam, int nNetType)
{
	CXEngineAuthorizeAppDlg* pClass_This = (CXEngineAuthorizeAppDlg*)lParam;
	//判断协议头和尾部
	if ((XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER != pSt_ProtocolHdr->wHeader) || (XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL != pSt_ProtocolHdr->wTail) || (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH != pSt_ProtocolHdr->unOperatorType))
	{
		XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，协议错误"), lpszClientAddr);
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
		if (!Database_SQLite_UserQuery(st_UserInfo.tszUserName, &st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 211;
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，用户不存在"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			return FALSE;
		}
		if ((st_UserTable.st_UserInfo.nIDNumber != st_UserInfo.nIDNumber) || (0 != _tcsncmp(st_UserTable.st_UserInfo.tszEMailAddr, st_UserInfo.tszEMailAddr, _tcslen(st_UserTable.st_UserInfo.tszEMailAddr))) || (0 != _tcsncmp(st_UserTable.st_UserInfo.tszUserPass, st_UserInfo.tszUserPass, _tcslen(st_UserTable.st_UserInfo.tszUserPass))))
		{
			pSt_ProtocolHdr->wReserve = 212;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，注销失败,验证信息错误"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
			return FALSE;
		}
		//从数据库删除文件
		if (!Database_SQLite_UserDelete(st_UserInfo.tszUserName))
		{
			pSt_ProtocolHdr->wReserve = 213;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，注销失败,删除信息失败"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
			return FALSE;
		}
		//注销需要删除登录用户
		Session_Authorize_CloseClient(st_UserInfo.tszUserName);
		for (int i = 0; i < pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemCount(); i++)
		{
			CString m_StrAddr = pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemText(i, 2);
			if (0 == _tcsnicmp(lpszClientAddr, m_StrAddr.GetBuffer(), _tcslen(lpszClientAddr)))
			{
				pClass_This->m_DlgUser.m_ListCtrlOnlineClient.DeleteItem(i);
			}
		}

		pSt_ProtocolHdr->unPacketSize = 0;
		XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，注销成功"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQREGISTER == pSt_ProtocolHdr->unOperatorCode)
	{
		//用户注册
		AUTHREG_USERTABLE st_UserTable;
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		memcpy(&st_UserTable.st_UserInfo, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERINFO));
		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPREGISTER;
		//写试用期
		CString m_StrTryTime;
		pClass_This->m_DlgConfig.m_EditTryTime.GetWindowText(m_StrTryTime);
		//填充写入数据
		_tcscpy(st_UserTable.tszLeftTime, m_StrTryTime.GetBuffer());
		st_UserTable.en_AuthRegSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)pClass_This->m_DlgConfig.m_ComboRegTry.GetCurSel();
		if (Database_SQLite_UserRegister(&st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 0;
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，注册成功"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
		}
		else
		{
			pSt_ProtocolHdr->wReserve = 231;
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，注册失败，无法继续，错误：%X"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName, DBModule_GetLastError());
		}
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_USERTABLE st_UserTable;
		XENGINE_PROTOCOL_USERAUTH st_AuthProtocol;

		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_AuthProtocol, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));
		memcpy(&st_AuthProtocol, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERAUTH));

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN;
		if (!Database_SQLite_UserQuery(st_AuthProtocol.tszUserName, &st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 251;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，登录失败，用户名不存在"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}

		if (_tcslen(st_AuthProtocol.tszUserPass) != _tcslen(st_UserTable.st_UserInfo.tszUserPass))
		{
			pSt_ProtocolHdr->wReserve = 252;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，登录失败，密码错误"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		if (0 != _tcsncmp(st_AuthProtocol.tszUserPass, st_UserTable.st_UserInfo.tszUserPass, _tcslen(st_AuthProtocol.tszUserPass)))
		{
			pSt_ProtocolHdr->wReserve = 252;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，登录失败，密码错误"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		//是否被封禁
		if (-1 == st_UserTable.st_UserInfo.nUserLevel)
		{
			pSt_ProtocolHdr->wReserve = 256;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，登录失败，客户端已被封禁"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		//分析充值类型
		if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_UserTable.en_AuthRegSerialType)
		{
			pSt_ProtocolHdr->wReserve = 253;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，登录失败，客户端类型错误"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		if ('0' == st_UserTable.tszLeftTime[0])
		{
			pSt_ProtocolHdr->wReserve = 254;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，登录失败，没有剩余时间了"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		st_UserTable.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB == st_AuthProtocol.enDeviceType ? ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB : ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC;
		if (!Session_Authorize_Insert(lpszClientAddr, &st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 255;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，登录失败，插入会话管理失败,错误:%lX"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		//是否勾选了所有用户
		BOOL bFound = FALSE;
		if (BST_CHECKED == pClass_This->m_DlgUser.m_CheckAllUser.GetCheck())
		{
			for (int i = 0; i < pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemCount(); i++)
			{
				CString m_StrUser = pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemText(i, 1);
				if (0 == _tcsncmp(m_StrUser.GetBuffer(), st_AuthProtocol.tszUserName, m_StrUser.GetLength()))
				{
					bFound = TRUE;
					CString m_StrDeviceType;
					if (ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC == st_UserTable.enDeviceType)
					{
						m_StrDeviceType.Format(_T("TCP"));
					}
					else
					{
						m_StrDeviceType.Format(_T("WEB"));
					}

					pClass_This->m_DlgUser.m_ListCtrlOnlineClient.SetItemText(i, 2, lpszClientAddr);
					pClass_This->m_DlgUser.m_ListCtrlOnlineClient.SetItemText(i, 6, m_StrDeviceType.GetBuffer());
					pClass_This->m_DlgUser.m_ListCtrlOnlineClient.SetItemText(i, 7, _T("在线"));
					break;
				}
			}
		}
		if (BST_CHECKED != pClass_This->m_DlgUser.m_CheckAllUser.GetCheck() || !bFound)
		{
			TCHAR tszUserId[64];
			memset(tszUserId, '\0', sizeof(tszUserId));
			int nItemCount = pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemCount();
			_stprintf(tszUserId, _T("%d"), nItemCount);
			CString m_StrDeviceType;
			if (ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC == st_UserTable.enDeviceType)
			{
				m_StrDeviceType.Format(_T("TCP"));
			}
			else
			{
				m_StrDeviceType.Format(_T("WEB"));
			}
			pClass_This->m_DlgUser.m_ListCtrlOnlineClient.InsertItem(nItemCount, tszUserId);
			pClass_This->m_DlgUser.m_ListCtrlOnlineClient.SetItemText(nItemCount, 1, st_AuthProtocol.tszUserName);
			pClass_This->m_DlgUser.m_ListCtrlOnlineClient.SetItemText(nItemCount, 2, lpszClientAddr);
			pClass_This->m_DlgUser.m_ListCtrlOnlineClient.SetItemText(nItemCount, 6, m_StrDeviceType.GetBuffer());
			pClass_This->m_DlgUser.m_ListCtrlOnlineClient.SetItemText(nItemCount, 7, _T("在线"));
		}
		
		pSt_ProtocolHdr->wReserve = 0;
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
		XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，登录成功"), lpszClientAddr, st_AuthProtocol.tszUserName);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQPAY == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_PROTOCOL_USERPAY st_UserPay;
		memset(&st_UserPay, '\0', sizeof(AUTHREG_PROTOCOL_USERPAY));
		memcpy(&st_UserPay, lpszMsgBuffer, sizeof(AUTHREG_PROTOCOL_USERPAY));

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPPAY;
		if (Database_SQLite_UserPay(st_UserPay.tszUserName, st_UserPay.tszSerialNumber))
		{
			AUTHREG_USERTABLE st_UserInfo;
			memset(&st_UserInfo, '\0', sizeof(AUTHREG_USERTABLE));
			if (Database_SQLite_UserQuery(st_UserPay.tszUserName, &st_UserInfo))
			{
				Session_Authorize_SetUser(&st_UserInfo);
			}
			pClass_This->m_DlgSerial.SerialManage_Flush();
			pSt_ProtocolHdr->wReserve = 0;
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，充值成功，序列号：%s"), lpszClientAddr, st_UserPay.tszUserName, st_UserPay.tszSerialNumber);
		}
		else
		{
			pSt_ProtocolHdr->wReserve = 271;
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，充值失败，无法继续，错误：%X"), lpszClientAddr, st_UserPay.tszUserName, DBModule_GetLastError());
		}
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
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
		if (!Database_SQLite_UserQuery(st_UserInfo.tszUserName, &st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 291;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，找回密码失败，用户不存在"), lpszClientAddr, st_UserInfo.tszUserName);
			return FALSE;
		}
		//安全验证判断
		if ((0 != _tcsnicmp(st_UserInfo.tszEMailAddr, st_UserTable.st_UserInfo.tszEMailAddr, _tcslen(st_UserInfo.tszEMailAddr))) || (st_UserTable.st_UserInfo.nIDNumber != st_UserInfo.nIDNumber))
		{
			pSt_ProtocolHdr->wReserve = 292;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，找回密码失败，验证信息失败"), lpszClientAddr, st_UserInfo.tszUserName);
			return FALSE;
		}

		if (st_AuthConfig.st_EMail.bSmtpEnable)
		{
			XNETHANDLE xhEMail;
			TCHAR tszEMailBuffer[1024];
			RFCCOMPONENTS_EMAILSMTP st_EmailInfo;

			memset(tszEMailBuffer, '\0', sizeof(tszEMailBuffer));
			memset(&st_EmailInfo, '\0', sizeof(RFCCOMPONENTS_EMAILSMTP));

			_tcscpy(st_EmailInfo.tszServiceAddr, st_AuthConfig.st_EMail.tszSmtpAddr);
			_tcscpy(st_EmailInfo.tszUserName, st_AuthConfig.st_EMail.tszSmtpUser);
			_tcscpy(st_EmailInfo.tszPassWord, st_AuthConfig.st_EMail.tszSmtpPass);
			_tcscpy(st_EmailInfo.tszFromAddr, st_AuthConfig.st_EMail.tszSmtpFrom);

			_stprintf(tszEMailBuffer, _T("你的用户名:%s\r\n你的密码:%s\r\n"), st_UserTable.st_UserInfo.tszUserName, st_UserTable.st_UserInfo.tszUserPass);
			RfcComponents_EMailClient_SmtpInit(&xhEMail, &st_EmailInfo);
			RfcComponents_EMailClient_SmtpSend(xhEMail, st_UserTable.st_UserInfo.tszEMailAddr, _T("XEngine授权验证服务-密码找回"), tszEMailBuffer);
			RfcComponents_EMailClient_SmtpClose(xhEMail);
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
		}
		else
		{
			_tcscpy(st_AuthProtocol.tszUserName, st_UserTable.st_UserInfo.tszUserName);
			_tcscpy(st_AuthProtocol.tszUserPass, st_UserTable.st_UserInfo.tszUserPass);

			pSt_ProtocolHdr->wReserve = 0;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType, (LPCTSTR)&st_AuthProtocol, sizeof(XENGINE_PROTOCOL_USERAUTH));
		}
		XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，找回密码成功"), lpszClientAddr, st_UserInfo.tszUserName);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETTIME == pSt_ProtocolHdr->unOperatorCode)
	{
		TCHAR tszUserName[128];
		AUTHREG_PROTOCOL_TIME st_AuthTime;

		memset(tszUserName, '\0', sizeof(tszUserName));
		memset(&st_AuthTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETTIME;
		Session_Authorize_GetUserForAddr(lpszClientAddr, tszUserName);
		if (Session_Authorize_GetTimer(tszUserName, &st_AuthTime))
		{
			pSt_ProtocolHdr->wReserve = 0;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType, (LPCTSTR)&st_AuthTime, sizeof(AUTHREG_PROTOCOL_TIME));
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，获取时间成功，类型：%d，在线时间：%lld，剩余时间：%lld"), lpszClientAddr, st_AuthTime.tszUserName, st_AuthTime.enSerialType, st_AuthTime.nTimeONLine, st_AuthTime.nTimeLeft);
		}
		else
		{
			pSt_ProtocolHdr->wReserve = 0x2B1;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，获取时间失败，无法继续，错误：%X"), lpszClientAddr, st_AuthTime.tszUserName, Session_GetLastError());
		}
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_NOTIFYMSG == pSt_ProtocolHdr->unOperatorCode)
	{
		XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，发送数据，大小：%d，内容：%s"), lpszClientAddr, pSt_ProtocolHdr->unPacketSize, lpszMsgBuffer);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQTRYVER == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_NETVER st_AuthVer;
		memset(&st_AuthVer, '\0', sizeof(AUTHREG_NETVER));

		memcpy(st_AuthVer.tszVerSerial, lpszMsgBuffer, pSt_ProtocolHdr->unPacketSize);

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPTRYVER;
		if (Database_SQLite_TryQuery(&st_AuthVer))
		{
			TCHAR tszEndTime[64];
			__int64 nTimeSpan = 0;
			memset(tszEndTime, '\0', sizeof(tszEndTime));
			//根据方式来计算剩余时间
			if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == st_AuthVer.enVerMode)
			{
				nTimeSpan = st_AuthVer.nTryTime;
			}
			else
			{
				//计算时间差
				BaseLib_OperatorTime_TimeToStr(tszEndTime);
				BaseLib_OperatorTimeSpan_GetForStr(st_AuthVer.tszVerData, tszEndTime, &nTimeSpan, 2);
			}

			CString m_StrAuthTime;
			pClass_This->m_DlgConfig.m_EditAuthTime.GetWindowText(m_StrAuthTime);
			//是否超过
			if (nTimeSpan > _ttoi(m_StrAuthTime.GetBuffer()))
			{
				pSt_ProtocolHdr->wReserve = 0x2D1;
				pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_TIMEDOUT;
				XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，序列号：%s，临时验证失败，此序列号已经试用到期"), lpszClientAddr, st_AuthVer.tszVerSerial);
			}
			else
			{
				pSt_ProtocolHdr->wReserve = 0;
				XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，序列号：%s，临时验证成功，已用时间:%lld"), lpszClientAddr, st_AuthVer.tszVerSerial, nTimeSpan);

				if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == st_AuthVer.enVerMode)
				{
					//次数卡需要更新才可以
					st_AuthVer.nTryTime++;
					Database_SQLite_TrySet(&st_AuthVer);
				}
			}
		}
		else
		{
			CString m_StrAuthTime;
			pClass_This->m_DlgConfig.m_EditAuthTime.GetWindowText(m_StrAuthTime);
			//填充写入数据
			st_AuthVer.nTryTime = _ttoi(m_StrAuthTime.GetBuffer());
			st_AuthVer.enVerMode = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)pClass_This->m_DlgConfig.m_ComboListAuth.GetCurSel();
			//看下是否启用了此功能
			if ((ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_AuthVer.enVerMode) || (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_MINUTE == st_AuthVer.enVerMode) || (st_AuthVer.nTryTime <= 0))
			{
				pSt_ProtocolHdr->wReserve = 0x2D2;
				XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，序列号：%s，临时验证插入失败，因为服务器关闭了此功能"), lpszClientAddr, st_AuthVer.tszVerSerial);
			}
			else
			{
				if (Database_SQLite_TryInsert(&st_AuthVer))
				{
					XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，序列号：%s，临时验证插入成功"), lpszClientAddr, st_AuthVer.tszVerSerial);
					pSt_ProtocolHdr->unPacketSize = 0;
				}
				else
				{
					pSt_ProtocolHdr->wReserve = 0x2D3;
					XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，序列号：%s，临时验证插入失败，无法继续，错误：%X"), lpszClientAddr, st_AuthVer.tszVerSerial, DBModule_GetLastError());
				}
			}
		}
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETUSER == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_USERTABLE st_UserTable;
		XENGINE_PROTOCOL_USERINFO st_UserInfo;
		XENGINE_PROTOCOL_USERAUTH st_UserAuth;

		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));
		memset(&st_UserAuth, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

		memcpy(&st_UserAuth, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERAUTH));

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETUSER;

		int nListCount = 0;
		AUTHREG_USERTABLE** ppSt_ListClient;
		if (!Session_Authorize_GetClient(&ppSt_ListClient, &nListCount, lpszClientAddr))
		{
			pSt_ProtocolHdr->wReserve = 2100;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，获取客户端信息失败,客户端没有登录,无法继续，错误：%X"), lpszClientAddr, Session_GetLastError());
			return FALSE;
		}
		
		if (!Database_SQLite_UserQuery(st_UserAuth.tszUserName, &st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 2101;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，请求查询用户信息失败,帐户不存在"), lpszClientAddr, st_UserInfo.tszUserName);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
			return FALSE;
		}
		if (ppSt_ListClient[0]->st_UserInfo.nUserLevel <= st_UserTable.st_UserInfo.nUserLevel)
		{
			pSt_ProtocolHdr->wReserve = 2102;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，获取客户端信息失败,当前用户权限不足"), lpszClientAddr, ppSt_ListClient[0]->st_UserInfo.tszUserName);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
			return FALSE;
		}
		XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，请求查询用户信息成功"), lpszClientAddr, st_UserInfo.tszUserName);
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType, (LPCTSTR)&st_UserTable, sizeof(AUTHREG_USERTABLE));
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQSETUSER == pSt_ProtocolHdr->unOperatorCode)
	{
		AUTHREG_USERTABLE st_UserTable;
		XENGINE_PROTOCOL_USERINFO st_UserInfo;

		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));

		memcpy(&st_UserInfo, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERINFO));

		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPSETUSER;

		int nListCount = 0;
		AUTHREG_USERTABLE** ppSt_ListClient;
		if (!Session_Authorize_GetClient(&ppSt_ListClient, &nListCount, lpszClientAddr))
		{
			pSt_ProtocolHdr->wReserve = 2100;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，设置客户端信息失败,客户端没有登录,无法继续，错误：%X"), lpszClientAddr, Session_GetLastError());
			return FALSE;
		}
		
		if (!Database_SQLite_UserQuery(st_UserInfo.tszUserName, &st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 2101;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，请求设置用户信息失败,帐户不存在"), lpszClientAddr, ppSt_ListClient[0]->st_UserInfo.tszUserName);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
			return FALSE;
		}
		if (ppSt_ListClient[0]->st_UserInfo.nUserLevel <= st_UserTable.st_UserInfo.nUserLevel)
		{
			pSt_ProtocolHdr->wReserve = 2102;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，设置客户端信息失败,当前用户权限不足"), lpszClientAddr, ppSt_ListClient[0]->st_UserInfo.tszUserName);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
			return FALSE;
		}

		memcpy(&st_UserTable.st_UserInfo, &st_UserInfo, sizeof(XENGINE_PROTOCOL_USERINFO));
		if (!Database_SQLite_UserSet(&st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 2103;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，请求设置用户信息失败,错误:%lX"), lpszClientAddr, ppSt_ListClient[0]->st_UserInfo.tszUserName, DBModule_GetLastError());
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
			return FALSE;
		}
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
		XEngine_Authorize_LogPrint(lParam, _T("客户端：%s，用户名：%s，请求设置用户信息成功"), lpszClientAddr, ppSt_ListClient[0]->st_UserInfo.tszUserName);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	return TRUE;
}