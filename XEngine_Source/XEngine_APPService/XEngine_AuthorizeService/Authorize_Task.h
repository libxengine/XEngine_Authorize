#pragma once

void CALLBACK XEngine_TaskEvent_Client(LPCXSTR lpszUserAddr, LPCXSTR lpszUserName, __int64x nOnlineTimer, __int64x nLeftTimer, LPCXSTR lpszLeftDate, ENUM_AUTHORIZE_MODULE_SERIAL_TYPE enSerialType, ENUM_PROTOCOLDEVICE_TYPE enDeviceType, int nNetType, XPVOID lParam);
void CALLBACK XEngine_TaskEvent_Token(XNETHANDLE xhToken, XPVOID lParam);