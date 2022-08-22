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

		Protocol_Packet_HDRComm(tszMsgBuffer, &nMsgLen, &st_ProtocolHdr, enDeviceType);
		XEngine_Client_TaskSend(lpszUserAddr, tszMsgBuffer, nMsgLen, enDeviceType == ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC ? XENGINE_AUTH_APP_NETTYPE_TCP : XENGINE_AUTH_APP_NETTYPE_WS);

		if (!st_AuthConfig.bTimeNotify)
		{
			XEngine_CloseClient(lpszUserAddr);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("地址:%s,用户:%s,没有剩余时间,已经通知客户单超时"), lpszUserName, lpszUserAddr);
	}
}
void __stdcall XEngine_TaskEvent_Token(XNETHANDLE xhToken, LPVOID lParam)
{
	Session_Token_Delete(xhToken);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("Token:%lld,已经超时,被移除服务器"), xhToken);
}