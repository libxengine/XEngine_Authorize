#pragma once

XHTHREAD XCALLBACK Authorize_MQTTTask_Thread(XPVOID lParam);
bool Authorize_MQTTTask_Handle(LPCXSTR lpszClientAddr, MQTTPROTOCOL_FIXEDHEADER* pSt_MQTTHdr, LPCXSTR lpszMSGBuffer, int nMSGLen);