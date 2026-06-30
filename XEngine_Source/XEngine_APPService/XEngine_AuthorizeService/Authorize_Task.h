#pragma once

void XCALLBACK XEngine_TaskEvent_Client(LPCXSTR lpszUserAddr, LPCXSTR lpszUserName, __int64x nOnlineTimer, __int64x nLeftTimer, LPCXSTR lpszLeftDate, ENUM_VERIFICATION_MODULE_SERIAL_TYPE enSerialType, ENUM_PROTOCOLDEVICE_TYPE enDeviceType, int nNetType, XPVOID lParam);
void XCALLBACK XEngine_TaskEvent_Token(LPCXSTR lpszTokenStr, int nTimeout, int nTimerenewal, XENGINE_LIBTIME* pSt_LibTime, XENGINE_PROTOCOL_USERINFO* pSt_UserInfo, XPVOID lParam);