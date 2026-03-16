#include "Authorize_Hdr.h"

XHTHREAD XCALLBACK Authorize_MQTTTask_Thread(XPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;
	while (bIsRun)
	{
		if (!MQTTProtocol_Parse_WaitEvent(nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		XENGINE_MANAGEPOOL_TASKEVENT** ppSst_ListAddr;

		MQTTProtocol_Parse_GetPool(nThreadPos, &ppSst_ListAddr, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
			{
				int nMSGLen = 0;
				XCHAR *ptszMSGBuffer = NULL;
				MQTTPROTOCOL_FIXEDHEADER st_MQTTHdr = {};

				if (MQTTProtocol_Parse_Recv(ppSst_ListAddr[i]->tszClientAddr, &st_MQTTHdr, &ptszMSGBuffer, &nMSGLen))
				{
					if (st_AuthConfig.st_XCrypto.bEnable)
					{
						Cryption_Api_CryptDecodec(NULL, (XBYTE*)ptszMSGBuffer, &nMSGLen, st_AuthConfig.st_XCrypto.tszCryptoKey, (ENUM_XENGINE_CRYPTION_SYMMETRIC)st_AuthConfig.st_XCrypto.nCryptionType);
					}
					Authorize_MQTTTask_Handle(ppSst_ListAddr[i]->tszClientAddr, &st_MQTTHdr, ptszMSGBuffer, nMSGLen);
					BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);
				}
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
void Packet_Property(MQTTPROTOCOL_HDRPROPERTY*** pppSt_HDRProperty, int nListCount)
{
	BaseLib_Memory_Malloc((XPPPMEM)pppSt_HDRProperty, nListCount, sizeof(MQTTPROTOCOL_HDRPROPERTY));

	(*pppSt_HDRProperty)[0]->nProLen = 4;
	(*pppSt_HDRProperty)[0]->st_unValue.nValue = 1024000;
	(*pppSt_HDRProperty)[0]->byProFlag = XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_PROPERTY_PACKMAX;

	(*pppSt_HDRProperty)[1]->nProLen = 1;
	(*pppSt_HDRProperty)[1]->st_unValue.byValue = 1;
	(*pppSt_HDRProperty)[1]->byProFlag = XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_PROPERTY_REVERAVAI;

	(*pppSt_HDRProperty)[2]->nProLen = 1;
	(*pppSt_HDRProperty)[2]->st_unValue.byValue = 1;
	(*pppSt_HDRProperty)[2]->byProFlag = XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_PROPERTY_SHAREDSUBAVAI;

	(*pppSt_HDRProperty)[3]->nProLen = 1;
	(*pppSt_HDRProperty)[3]->st_unValue.byValue = 1;
	(*pppSt_HDRProperty)[3]->byProFlag = XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_PROPERTY_SUBIDAVAI;

	(*pppSt_HDRProperty)[4]->nProLen = 2;
	(*pppSt_HDRProperty)[4]->st_unValue.wValue = 65535;
	(*pppSt_HDRProperty)[4]->byProFlag = XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_PROPERTY_ALIASMAX;

	(*pppSt_HDRProperty)[5]->nProLen = 1;
	(*pppSt_HDRProperty)[5]->st_unValue.byValue = 1;
	(*pppSt_HDRProperty)[5]->byProFlag = XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_PROPERTY_WILDCARDSUBAVAI;
}
bool Authorize_MQTTTask_Handle(LPCXSTR lpszClientAddr, MQTTPROTOCOL_FIXEDHEADER* pSt_MQTTHdr, LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	int nSDLen = 0;
	int nRVLen = 0;
	XCHAR tszSDBuffer[1024];
	XCHAR tszRVBuffer[1024];

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	//是不是连接
	if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_CONNECT == pSt_MQTTHdr->byMsgType)
	{
		int nListCount = 0;
		MQTTPROTOCOL_HDRPROPERTY** ppSt_HDRProperty;
		MQTTPROTOCOL_HDRCONNNECT st_HDRConnect = {};
		MQTTPROTOCOL_USERINFO st_USerInfo = {};

		if (!MQTTProtocol_Parse_Connect(lpszMSGBuffer, nMSGLen, &st_HDRConnect, &st_USerInfo, &ppSt_HDRProperty, &nListCount))
		{
			//错误断开连接
			MQTTProtocol_Packet_DisConnect(tszRVBuffer, &nRVLen);
			MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_DISCONN, tszRVBuffer, nRVLen);
			//XEngine_MQXService_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_MQAPP_NETTYPE_MQTT);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("MQTT客户端:%s,请求链接失败,错误码:%lX"), lpszClientAddr, MQTTProtocol_GetLastError());
			return false;
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_HDRProperty, nListCount);

		XENGINE_PROTOCOLHDR st_ProtocolHdr = {};
		XENGINE_PROTOCOL_USERAUTH st_ProtocolAuth = {};
		st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
		st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
		st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN;
		st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOL_USERAUTH);
		st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

		st_ProtocolAuth.enClientType = ENUM_PROTOCOL_FOR_SERVICE_TYPE_USER;
		st_ProtocolAuth.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_MOBILE_EMBEDDED;
		_tcsxcpy(st_ProtocolAuth.tszUserName, st_USerInfo.tszClientUser);
		_tcsxcpy(st_ProtocolAuth.tszUserPass, st_USerInfo.tszClientPass);

		XEngine_Client_TCPTask(lpszClientAddr, (LPCXSTR)&st_ProtocolAuth, sizeof(XENGINE_PROTOCOL_USERAUTH), &st_ProtocolHdr, XENGINE_AUTH_APP_NETTYPE_MQTT);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("MQTT客户端:%s,请求链接成功,客户端ID:%s,用户名:%s"), lpszClientAddr, st_USerInfo.tszClientID, st_USerInfo.tszClientUser);
		return true;
	}
	else if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PINGREQ == pSt_MQTTHdr->byMsgType)
	{
		MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PINGREP);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_MQTT);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("MQTT客户端:%s,请求了心跳PING协议成功"), lpszClientAddr);
	}
	return true;
}