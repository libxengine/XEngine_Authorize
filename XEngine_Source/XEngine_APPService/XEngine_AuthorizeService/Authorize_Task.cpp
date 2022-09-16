#include "Authorize_Hdr.h"

void __stdcall XEngine_TaskEvent_Client(LPCSTR lpszUserAddr, LPCSTR lpszUserName, __int64x nOnlineTimer, __int64x nLeftTimer, LPCSTR lpszLeftDate, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType, ENUM_PROTOCOLDEVICE_TYPE enDeviceType, LPVOID lParam)
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

		int nNetType = enDeviceType == ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB ? XENGINE_AUTH_APP_NETTYPE_WS : XENGINE_AUTH_APP_NETTYPE_TCP;
		Protocol_Packet_HDRComm(tszMsgBuffer, &nMsgLen, &st_ProtocolHdr, nNetType);
		XEngine_Client_TaskSend(lpszUserAddr, tszMsgBuffer, nMsgLen, nNetType);

		if (!st_AuthConfig.bTimeNotify)
		{
			XEngine_CloseClient(lpszUserAddr);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("地址:%s,用户:%s,没有剩余时间,已经通知客户单超时"), lpszUserName, lpszUserAddr);
	}
}
void __stdcall XEngine_TaskEvent_Token(XNETHANDLE xhToken, LPVOID lParam)
{
	AUTHREG_USERTABLE st_UserTable;
	memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

	Session_Token_Get(xhToken, &st_UserTable);
	if (st_UserTable.st_UserInfo.nUserLevel > 0)
	{
		//如果权限是普通用户
		AUTHREG_PROTOCOL_TIME st_TimeProtocol;
		memset(&st_TimeProtocol, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
		//需要设置时间并且关闭会话
		if (Session_Authorize_GetTimer(st_UserTable.st_UserInfo.tszUserName, &st_TimeProtocol))
		{
			Database_SQLite_UserLeave(&st_TimeProtocol);
		}
		Session_Authorize_CloseClient(st_UserTable.st_UserInfo.tszUserName);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("Token：%lld，用户名：%s，离开服务器,在线时长:%d"), xhToken, st_UserTable.st_UserInfo.tszUserName, st_TimeProtocol.nTimeONLine);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("Token:%lld,已经超时,被移除服务器"), xhToken);
	}
	Session_Token_Delete(xhToken);
}