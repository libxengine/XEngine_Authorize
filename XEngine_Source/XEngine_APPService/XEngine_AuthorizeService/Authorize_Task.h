#pragma once

void CALLBACK XEngine_TaskEvent_Client(LPCSTR lpszUserAddr, LPCSTR lpszUserName, __int64x nOnlineTimer, __int64x nLeftTimer, LPCSTR lpszLeftDate, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType, ENUM_PROTOCOLDEVICE_TYPE enDeviceType, int nNetType, LPVOID lParam);
void CALLBACK XEngine_TaskEvent_Token(XNETHANDLE xhToken, LPVOID lParam);