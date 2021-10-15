#include "Authorize_Hdr.h"

LPCSTR lpszKeyType[] = { "未知类型","分钟卡","天数卡","次数卡","自定义卡" };
void __stdcall XEngine_TaskEvent_Client(LPCSTR lpszUserAddr, LPCSTR lpszUserName, __int64x nOnlineTimer, __int64x nLeftTimer, LPCSTR lpszLeftDate, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE en_AuthRegSerialType, ENUM_PROTOCOLDEVICE_TYPE enDeviceType, LPVOID lParam)
{
	if (nLeftTimer <= 0)
	{
		XENGINE_PROTOCOLHDR st_ProtocolHdr;
		memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

		st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
		st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
		st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_TIMEDOUT;
		st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("用户:%s,地址:%s,没有剩余时间,已经通知客户单超时\r\n"), lpszUserName, lpszUserAddr);
		XEngine_Client_TaskSend(lpszUserAddr, &st_ProtocolHdr, enDeviceType == ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC ? XENGINE_AUTH_APP_NETTYPE_TCP : XENGINE_AUTH_APP_NETTYPE_WS);
	}
}