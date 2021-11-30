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
	//�ж�Э��ͷ��β��
	if ((XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER != pSt_ProtocolHdr->wHeader) || (XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL != pSt_ProtocolHdr->wTail) || (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH != pSt_ProtocolHdr->unOperatorType))
	{
		XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s��Э�����"), lpszClientAddr);
		return FALSE;
	}
	//�û�ɾ��
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
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s���û�������"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			return FALSE;
		}
		if ((st_UserTable.st_UserInfo.nIDNumber != st_UserInfo.nIDNumber) || (0 != _tcsncmp(st_UserTable.st_UserInfo.tszEMailAddr, st_UserInfo.tszEMailAddr, _tcslen(st_UserTable.st_UserInfo.tszEMailAddr))) || (0 != _tcsncmp(st_UserTable.st_UserInfo.tszUserPass, st_UserInfo.tszUserPass, _tcslen(st_UserTable.st_UserInfo.tszUserPass))))
		{
			pSt_ProtocolHdr->wReserve = 212;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s��ע��ʧ��,��֤��Ϣ����"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
			return FALSE;
		}
		//�����ݿ�ɾ���ļ�
		if (!AuthService_SQLPacket_UserDelete(st_UserInfo.tszUserName))
		{
			pSt_ProtocolHdr->wReserve = 213;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s��ע��ʧ��,ɾ����Ϣʧ��"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
			return FALSE;
		}
		//ע����Ҫɾ����¼�û�
		AuthService_Session_CloseClient(st_UserInfo.tszUserName);
		for (int i = 0; i < pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemCount(); i++)
		{
			CString m_StrAddr = pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemText(i, 2);
			if (0 == _tcsnicmp(lpszClientAddr, m_StrAddr.GetBuffer(), _tcslen(lpszClientAddr)))
			{
				pClass_This->m_DlgUser.m_ListCtrlOnlineClient.DeleteItem(i);
			}
		}

		pSt_ProtocolHdr->unPacketSize = 0;
		XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s��ע���ɹ�"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQREGISTER == pSt_ProtocolHdr->unOperatorCode)
	{
		//�û�ע��
		AUTHREG_USERTABLE st_UserTable;
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		memcpy(&st_UserTable.st_UserInfo, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERINFO));
		pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPREGISTER;
		//д������
		CString m_StrTryTime;
		pClass_This->m_DlgConfig.m_EditTryTime.GetWindowText(m_StrTryTime);
		//���д������
		_tcscpy(st_UserTable.tszLeftTime, m_StrTryTime.GetBuffer());
		st_UserTable.en_AuthRegSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)pClass_This->m_DlgConfig.m_ComboRegTry.GetCurSel();
		if (AuthService_SQLPacket_UserRegister(&st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 0;
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s��ע��ɹ�"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
		}
		else
		{
			pSt_ProtocolHdr->wReserve = 231;
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s��ע��ʧ�ܣ��޷�����������%X"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName, SQLPacket_GetLastError());
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
		if (!AuthService_SQLPacket_UserQuery(st_AuthProtocol.tszUserName, &st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 251;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s����¼ʧ�ܣ��û���������"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}

		if (_tcslen(st_AuthProtocol.tszUserPass) != _tcslen(st_UserTable.st_UserInfo.tszUserPass))
		{
			pSt_ProtocolHdr->wReserve = 252;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s����¼ʧ�ܣ��������"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		if (0 != _tcsncmp(st_AuthProtocol.tszUserPass, st_UserTable.st_UserInfo.tszUserPass, _tcslen(st_AuthProtocol.tszUserPass)))
		{
			pSt_ProtocolHdr->wReserve = 252;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s����¼ʧ�ܣ��������"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		//������ֵ����
		if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_UserTable.en_AuthRegSerialType)
		{
			pSt_ProtocolHdr->wReserve = 253;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s����¼ʧ�ܣ��ͻ������ʹ���"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		if ('0' == st_UserTable.tszLeftTime[0])
		{
			pSt_ProtocolHdr->wReserve = 254;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s����¼ʧ�ܣ�û��ʣ��ʱ����"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		st_UserTable.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB == st_AuthProtocol.enDeviceType ? ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB : ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC;
		if (!AuthService_Session_Insert(lpszClientAddr, &st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 255;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s����¼ʧ�ܣ�����Ự����ʧ��,����:%lX"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}

		TCHAR tszUserId[64];
		memset(tszUserId, '\0', sizeof(tszUserId));
		int nItemCount = pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemCount();
		if (0 == nItemCount)
		{
			_stprintf(tszUserId, _T("%d"), nItemCount);
		}
		else
		{
			_stprintf(tszUserId, _T("%d"), nItemCount + 1);
		}
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

		pSt_ProtocolHdr->wReserve = 0;
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
		XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s����¼�ɹ�"), lpszClientAddr, st_AuthProtocol.tszUserName);
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
			pClass_This->m_DlgSerial.SerialManage_Flush();
			pSt_ProtocolHdr->wReserve = 0;
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s����ֵ�ɹ������кţ�%s"), lpszClientAddr, st_UserPay.tszUserName, st_UserPay.tszSerialNumber);
		}
		else
		{
			pSt_ProtocolHdr->wReserve = 271;
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s����ֵʧ�ܣ��޷�����������%X"), lpszClientAddr, st_UserPay.tszUserName, SQLPacket_GetLastError());
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
		if (!AuthService_SQLPacket_UserQuery(st_UserInfo.tszUserName, &st_UserTable))
		{
			pSt_ProtocolHdr->wReserve = 291;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s���һ�����ʧ�ܣ��û�������"), lpszClientAddr, st_UserInfo.tszUserName);
			return FALSE;
		}
		//��ȫ��֤�ж�
		if ((0 != _tcsnicmp(st_UserInfo.tszEMailAddr, st_UserTable.st_UserInfo.tszEMailAddr, _tcslen(st_UserInfo.tszEMailAddr))) || (st_UserTable.st_UserInfo.nIDNumber != st_UserInfo.nIDNumber))
		{
			pSt_ProtocolHdr->wReserve = 292;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s���һ�����ʧ�ܣ���֤��Ϣʧ��"), lpszClientAddr, st_UserInfo.tszUserName);
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

			_stprintf(tszEMailBuffer, _T("����û���:%s\r\n�������:%s\r\n"), st_UserTable.st_UserInfo.tszUserName, st_UserTable.st_UserInfo.tszUserPass);
			RfcComponents_EMailClient_SmtpInit(&xhEMail, &st_EmailInfo);
			RfcComponents_EMailClient_SmtpSend(xhEMail, st_UserTable.st_UserInfo.tszEMailAddr, _T("XEngine��Ȩ��֤����-�����һ�"), tszEMailBuffer);
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
		XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s���һ�����ɹ�"), lpszClientAddr, st_UserInfo.tszUserName);
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
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType, (LPCTSTR)&st_AuthTime, sizeof(AUTHREG_PROTOCOL_TIME));
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s����ȡʱ��ɹ������ͣ�%d������ʱ�䣺%ld��ʣ��ʱ�䣺%ld"), lpszClientAddr, st_AuthTime.tszUserName, st_AuthTime.enSerialType, st_AuthTime.nTimeONLine, st_AuthTime.nTimeLeft);
		}
		else
		{
			pSt_ProtocolHdr->wReserve = 0x2B1;
			XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s����ȡʱ��ʧ�ܣ��޷�����������%X"), lpszClientAddr, st_AuthTime.tszUserName, Session_GetLastError());
		}
	}
	else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_NOTIFYMSG == pSt_ProtocolHdr->unOperatorCode)
	{
		XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���������ݣ���С��%d�����ݣ�%s"), lpszClientAddr, pSt_ProtocolHdr->unPacketSize, lpszMsgBuffer);
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
			__int64 nTimeSpan = 0;
			memset(tszEndTime, '\0', sizeof(tszEndTime));
			//���ݷ�ʽ������ʣ��ʱ��
			if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == st_AuthVer.enVerMode)
			{
				nTimeSpan = st_AuthVer.nTryTime;
			}
			else
			{
				//����ʱ���
				BaseLib_OperatorTime_ToStringTimer(tszEndTime);
				BaseLib_OperatorTimeSpan_GetForStr(st_AuthVer.tszVerData, tszEndTime, &nTimeSpan, 2);
			}

			CString m_StrAuthTime;
			pClass_This->m_DlgConfig.m_EditAuthTime.GetWindowText(m_StrAuthTime);
			//�Ƿ񳬹�
			if (nTimeSpan > _ttoi(m_StrAuthTime.GetBuffer()))
			{
				pSt_ProtocolHdr->wReserve = 0x2D1;
				pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_TIMEDOUT;
				XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s�����кţ�%s����ʱ��֤ʧ�ܣ������к��Ѿ����õ���"), lpszClientAddr, st_AuthVer.tszVerSerial);
			}
			else
			{
				pSt_ProtocolHdr->wReserve = 0;
				XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s�����кţ�%s����ʱ��֤�ɹ�������ʱ��:%lld"), lpszClientAddr, st_AuthVer.tszVerSerial, nTimeSpan);

				if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == st_AuthVer.enVerMode)
				{
					//��������Ҫ���²ſ���
					st_AuthVer.nTryTime++;
					AuthService_SQLPacket_TrySet(&st_AuthVer);
				}
			}
		}
		else
		{
			CString m_StrAuthTime;
			pClass_This->m_DlgConfig.m_EditAuthTime.GetWindowText(m_StrAuthTime);
			//���д������
			st_AuthVer.nTryTime = _ttoi(m_StrAuthTime.GetBuffer());
			st_AuthVer.enVerMode = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)pClass_This->m_DlgConfig.m_ComboListAuth.GetCurSel();
			//�����Ƿ������˴˹���
			if ((ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_AuthVer.enVerMode) || (st_AuthVer.nTryTime <= 0))
			{
				pSt_ProtocolHdr->wReserve = 0x2D2;
				XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s�����кţ�%s����ʱ��֤����ʧ�ܣ���Ϊ�������ر��˴˹���"), lpszClientAddr, st_AuthVer.tszVerSerial);
			}
			else
			{
				if (AuthService_SQLPacket_TryInsert(&st_AuthVer))
				{
					XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s�����кţ�%s����ʱ��֤����ɹ�"), lpszClientAddr, st_AuthVer.tszVerSerial);
					pSt_ProtocolHdr->unPacketSize = 0;
				}
				else
				{
					pSt_ProtocolHdr->wReserve = 0x2D3;
					XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s�����кţ�%s����ʱ��֤����ʧ�ܣ��޷�����������%X"), lpszClientAddr, st_AuthVer.tszVerSerial, SQLPacket_GetLastError());
				}
			}
		}
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
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
				XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s�������ѯ�û���Ϣ�ɹ�"), lpszClientAddr, st_UserInfo.tszUserName);
			}
			else
			{
				pSt_ProtocolHdr->wReserve = 2100;
				XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s�������ѯ�û���Ϣʧ��,�������"), lpszClientAddr, st_UserInfo.tszUserName);
			}
		}
		else
		{
			pSt_ProtocolHdr->wReserve = 2101;
			XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s�������ѯ�û���Ϣʧ��,�ʻ�������"), lpszClientAddr, st_UserInfo.tszUserName);
		}
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
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
					XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s�����������û���Ϣ�ɹ�\r\n"), lpszClientAddr, st_UserInfo.tszUserName);
					
				}
				else
				{
					pSt_ProtocolHdr->wReserve = 2120;
					XEngine_Authorize_LogPrint(lParam, _T("�ͻ��ˣ�%s���û�����%s�����������û���Ϣʧ��,����:%lX\r\n"), lpszClientAddr, st_UserInfo.tszUserName, SQLPacket_GetLastError());
				}
			}
			else
			{
				pSt_ProtocolHdr->wReserve = 2121;
				XEngine_Authorize_LogPrint(_T("�ͻ��ˣ�%s���û�����%s�����������û���Ϣʧ��,��Ϣ��ƥ��\r\n"), lpszClientAddr, st_UserInfo.tszUserName);
			}
		}
		else
		{
			pSt_ProtocolHdr->wReserve = 2122;
			XEngine_Authorize_LogPrint(_T("�ͻ��ˣ�%s���û�����%s�����������û���Ϣʧ��,����:%lX\r\n"), lpszClientAddr, st_UserInfo.tszUserName, SQLPacket_GetLastError());
		}
		XEngine_Client_TaskSend(lpszClientAddr, pSt_ProtocolHdr, lParam, nNetType);
	}
	return TRUE;
}