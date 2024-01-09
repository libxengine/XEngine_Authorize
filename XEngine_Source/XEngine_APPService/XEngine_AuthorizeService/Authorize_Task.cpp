#include "Authorize_Hdr.h"

void CALLBACK XEngine_TaskEvent_Client(LPCXSTR lpszUserAddr, LPCXSTR lpszUserName, __int64x nOnlineTimer, __int64x nLeftTimer, LPCXSTR lpszLeftDate, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType, ENUM_PROTOCOLDEVICE_TYPE enDeviceType, int nNetType, XPVOID lParam)
{
	if (nLeftTimer <= 0)
	{
		int nMsgLen = 0;
		XCHAR tszMsgBuffer[1024];
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
			_tcsxcpy(st_AuthTime.tszUserName, lpszUserName);
			_tcsxcpy(st_AuthTime.tszLeftTime, lpszLeftDate);
			_tcsxcpy(st_AuthTime.tszUserAddr, lpszUserAddr);

			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
			Protocol_Packet_HttpUserTime(tszMsgBuffer, &nMsgLen, &st_AuthTime);
			APIClient_Http_Request(_X("POST"), st_AuthConfig.st_XLogin.st_PassUrl.tszPassTimeout, tszMsgBuffer);
		}
		//HTTP不能通知
		Protocol_Packet_HDRComm(tszMsgBuffer, &nMsgLen, &st_ProtocolHdr, nNetType);
		XEngine_Client_TaskSend(lpszUserAddr, tszMsgBuffer, nMsgLen, nNetType);
		if (!st_AuthConfig.bTimeNotify)
		{
			XEngine_CloseClient(lpszUserAddr);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("地址:%s,用户:%s,网络类型:%d,没有剩余时间,已经通知客户单超时,三方通知设置:%d"), lpszUserName, lpszUserAddr, nNetType, st_AuthConfig.st_XLogin.bPassAuth);
	}
	AUTHREG_PROTOCOL_TIME st_AuthTime;
	memset(&st_AuthTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));

	st_AuthTime.nTimeLeft = nLeftTimer;
	st_AuthTime.nTimeONLine = nOnlineTimer;
	st_AuthTime.enSerialType = enSerialType;
	_tcsxcpy(st_AuthTime.tszLeftTime, lpszLeftDate);
	_tcsxcpy(st_AuthTime.tszUserName, lpszUserName);
	_tcsxcpy(st_AuthTime.tszUserAddr, lpszUserAddr);

	Database_SQLite_UserLeave(&st_AuthTime);
}
void CALLBACK XEngine_TaskEvent_Token(XNETHANDLE xhToken, XPVOID lParam)
{
	AUTHREG_USERTABLE st_UserTable;
	memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

	Session_Token_Get(xhToken, &st_UserTable);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Token:%lld,用户名：%s，已经超时,被移除服务器"), xhToken, st_UserTable.st_UserInfo.tszUserName);
	Session_Token_Delete(xhToken);
}