#include "Authorize_Hdr.h"

void __stdcall XEngine_TaskEvent_Client(LPCSTR lpszUserAddr, LPCSTR lpszUserName, __int64x nOnlineTimer, __int64x nLeftTimer, LPCSTR lpszLeftDate, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType, ENUM_PROTOCOLDEVICE_TYPE enDeviceType, int nNetType, LPVOID lParam)
{
	if (nLeftTimer <= 0)
	{
		int nMsgLen = 0;
		TCHAR tszMsgBuffer[1024];
		XENGINE_PROTOCOLHDR st_ProtocolHdr;

		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

		st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
		st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
		st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_TIMEDOUT;
		st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

		if (st_AuthConfig.st_XLogin.bPassAuth)
		{
			AUTHREG_PROTOCOL_TIME st_AuthTime;
			memset(&st_AuthTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

			st_AuthTime.nNetType = nNetType;
			st_AuthTime.nTimeLeft = nLeftTimer;
			st_AuthTime.nTimeONLine = nOnlineTimer;
			st_AuthTime.enSerialType = enSerialType;
			_tcscpy(st_AuthTime.tszUserName, lpszUserName);
			_tcscpy(st_AuthTime.tszLeftTime, lpszLeftDate);
			_tcscpy(st_AuthTime.tszUserAddr, lpszUserAddr);

			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
			Protocol_Packet_HttpUserTime(tszMsgBuffer, &nMsgLen, &st_AuthTime);
			APIHelp_HttpRequest_Post(st_AuthConfig.st_XLogin.st_PassUrl.tszPassTimeout, tszMsgBuffer);
		}
		if (XENGINE_AUTH_APP_NETTYPE_HTTP != nNetType)
		{
			//HTTP不能通知
			Protocol_Packet_HDRComm(tszMsgBuffer, &nMsgLen, &st_ProtocolHdr, nNetType);
			XEngine_Client_TaskSend(lpszUserAddr, tszMsgBuffer, nMsgLen, nNetType);
		}
		if (!st_AuthConfig.bTimeNotify)
		{
			XEngine_CloseClient(lpszUserAddr);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("地址:%s,用户:%s,网络类型:%d,没有剩余时间,已经通知客户单超时,三方通知设置:%d"), lpszUserName, lpszUserAddr, nNetType, st_AuthConfig.st_XLogin.bPassAuth);
	}
}
void __stdcall XEngine_TaskEvent_Token(XNETHANDLE xhToken, LPVOID lParam)
{
	AUTHREG_USERTABLE st_UserTable;
	memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

	Session_Token_Get(xhToken, &st_UserTable);
	if (st_UserTable.st_UserInfo.nUserLevel > 1)
	{
		//如果权限是普通用户
		AUTHREG_PROTOCOL_TIME st_AuthTime;
		AUTHSESSION_NETCLIENT st_NETClient;
		memset(&st_AuthTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
		memset(&st_NETClient, '\0', sizeof(AUTHSESSION_NETCLIENT));
		//需要设置时间并且关闭会话
		if (Session_Authorize_GetClientForUser(st_UserTable.st_UserInfo.tszUserName, &st_NETClient))
		{
			st_AuthTime.nTimeLeft = st_NETClient.nLeftTime;
			st_AuthTime.nTimeONLine = st_NETClient.nOnlineTime;
			st_AuthTime.enSerialType = st_NETClient.st_UserTable.enSerialType;
			_tcscpy(st_AuthTime.tszUserName, st_UserTable.st_UserInfo.tszUserName);
			_tcscpy(st_AuthTime.tszLeftTime, st_NETClient.tszLeftTime);
			_tcscpy(st_AuthTime.tszUserAddr, st_NETClient.tszClientAddr);

			if (st_AuthConfig.st_XLogin.bHTTPAuth)
			{
				int nSDLen = 0;
				TCHAR tszSDBuffer[MAX_PATH];
				memset(tszSDBuffer, '\0', MAX_PATH);

				Protocol_Packet_HttpUserTime(tszSDBuffer, &nSDLen, &st_AuthTime);
				APIHelp_HttpRequest_Post(st_AuthConfig.st_XLogin.st_PassUrl.tszPassLogout, tszSDBuffer);
			}
			Database_SQLite_UserLeave(&st_AuthTime);
		}
		Session_Authorize_CloseClient(st_UserTable.st_UserInfo.tszUserName);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("Token：%lld，用户名：%s，离开服务器,在线时长:%d"), xhToken, st_UserTable.st_UserInfo.tszUserName, st_AuthTime.nTimeONLine);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("Token:%lld,已经超时,被移除服务器"), xhToken);
	}
	Session_Token_Delete(xhToken);
}