#include "pch.h"

LPCSTR lpszKeyType[] = { "未知类型","分钟卡","天数卡","次数卡","自定义卡" };
void __stdcall XEngine_TaskEvent_Client(LPCSTR lpszUserAddr, LPCSTR lpszUserName, __int64x nOnlineTimer, __int64x nLeftTimer, LPCSTR lpszLeftDate, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthRegSerialType, ENUM_PROTOCOLDEVICE_TYPE enDeviceType, LPVOID lParam)
{
	CXEngineAuthorizeAppDlg* pClass_This = (CXEngineAuthorizeAppDlg*)lParam;
	CString m_StrFormat;
	int nListCount = pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemCount();

	for (int i = 0; i < nListCount; i++)
	{
		CString m_StrUser = pClass_This->m_DlgUser.m_ListCtrlOnlineClient.GetItemText(i, 1);
		if (0 == _tcsncmp(lpszUserName, m_StrUser.GetBuffer(), _tcslen(lpszUserName)))
		{
			TCHAR tszTimeONLine[64];
			TCHAR tszTimeLeft[64];

			memset(tszTimeONLine, '\0', sizeof(tszTimeONLine));
			memset(tszTimeLeft, '\0', sizeof(tszTimeLeft));

			_i64tot(nOnlineTimer, tszTimeONLine, 10);
			_i64tot(nLeftTimer, tszTimeLeft, 10);

			pClass_This->m_DlgUser.m_ListCtrlOnlineClient.SetItemText(i, 3, tszTimeONLine);
			if ((ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_MINUTE == en_AuthRegSerialType) || (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == en_AuthRegSerialType))
			{
				pClass_This->m_DlgUser.m_ListCtrlOnlineClient.SetItemText(i, 4, tszTimeLeft);
			}
			else
			{
				pClass_This->m_DlgUser.m_ListCtrlOnlineClient.SetItemText(i, 4, lpszLeftDate);
			}
			pClass_This->m_DlgUser.m_ListCtrlOnlineClient.SetItemText(i, 5, lpszKeyType[en_AuthRegSerialType]);

			if (nLeftTimer <= 0)
			{
				CString m_StrFmtLog;
				CString m_StrPntLog;
				XENGINE_PROTOCOLHDR st_ProtocolHdr;
				memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

				st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
				st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
				st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_TIMEDOUT;
				st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

				pClass_This->m_EditLog.GetWindowText(m_StrPntLog);
				m_StrFmtLog.Format(_T("用户:%s,地址:%s,没有剩余时间,已经通知客户单超时\r\n"), lpszUserName, lpszUserAddr);
				m_StrPntLog += m_StrFmtLog;
				pClass_This->m_EditLog.SetWindowText(m_StrPntLog);

				XEngine_Client_TaskSend(lpszUserAddr, &st_ProtocolHdr, lParam, enDeviceType == ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC ? XENGINE_AUTH_APP_NETTYPE_TCP : XENGINE_AUTH_APP_NETTYPE_WS);
				if (!st_AuthConfig.bTimeNotify)
				{
					XEngine_CloseClient(lpszUserAddr, lParam);
					pClass_This->m_DlgUser.m_ListCtrlOnlineClient.DeleteItem(i);
				}
			}
		}
	}
}