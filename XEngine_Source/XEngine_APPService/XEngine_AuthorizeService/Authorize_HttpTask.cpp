﻿#include "Authorize_Hdr.h"

XHTHREAD CALLBACK XEngine_AuthService_HttpThread(LPVOID lParam)
{
	int nPoolIndex = *(int*)lParam;
	int nThreadPos = nPoolIndex + 1;

	while (bIsRun)
	{
		if (!RfcComponents_HttpServer_EventWaitEx(xhHttpPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_ListClient;
		RfcComponents_HttpServer_GetPoolEx(xhHttpPacket, nThreadPos, &ppSt_ListClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			int nMsgLen = 0;
			CHAR* ptszMsgBuffer = NULL;
			RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParament;

			memset(&st_HTTPParament, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));

			if (RfcComponents_HttpServer_GetMemoryEx(xhHttpPacket, ppSt_ListClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPParament))
			{
				XEngine_Client_HttpTask(ppSt_ListClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &st_HTTPParament);
			}
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	return 0;
}

BOOL XEngine_Client_HttpTask(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM *pSt_HTTPParament)
{
	int nSDLen = 4096;
	TCHAR tszSDBuffer[4096];
	LPCTSTR lpszMethodPost = _T("POST");
	LPCTSTR lpszMethodGet = _T("GET");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsnicmp(lpszMethodPost, pSt_HTTPParament->tszHttpMethod, _tcslen(lpszMethodPost)))
	{
		TCHAR tszAPIType[64];
		TCHAR tszAPIVer[64];
		TCHAR tszAPIName[64];
		XNETHANDLE xhToken = 0;
		LPCTSTR lpszAPIType = _T("auth");
		LPCTSTR lpszAPIVerClient = _T("client");
		LPCTSTR lpszAPIVerSerial = _T("serial");
		LPCTSTR lpszAPIVerUser = _T("user");

		memset(tszAPIType, '\0', sizeof(tszAPIType));
		memset(tszAPIVer, '\0', sizeof(tszAPIVer));
		memset(tszAPIName, '\0', sizeof(tszAPIName));

		RfcComponents_HttpHelp_GetUrlApi(pSt_HTTPParament->tszHttpUri, tszAPIType, tszAPIVer, tszAPIName);
		if (0 != _tcsnicmp(lpszAPIType, tszAPIType, _tcslen(lpszAPIType)))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "request url is incorrent");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 不支持"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
			return FALSE;
		}

		if (0 == _tcsnicmp(lpszAPIVerClient, tszAPIVer, _tcslen(lpszAPIVerClient)))
		{
			//验证权限
			Protocol_Parse_HttpParseToken(lpszMsgBuffer, nMsgLen, &xhToken);
			if (!Session_Token_Get(xhToken))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "Unauthorized");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 失败,因为没有经过验证"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return FALSE;
			}
			XEngine_AuthorizeHTTP_Client(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
		}
		else if (0 == _tcsnicmp(lpszAPIVerSerial, tszAPIVer, _tcslen(lpszAPIVerSerial)))
		{
			//验证权限
			Protocol_Parse_HttpParseToken(lpszMsgBuffer, nMsgLen, &xhToken);
			if (!Session_Token_Get(xhToken))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 401, "Unauthorized");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 失败,因为没有经过验证"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
				return FALSE;
			}
			XEngine_AuthorizeHTTP_Serial(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
		}
		else if (0 == _tcsnicmp(lpszAPIVerUser, tszAPIVer, _tcslen(lpszAPIVerUser)))
		{
			XEngine_AuthorizeHTTP_User(lpszClientAddr, tszAPIName, lpszMsgBuffer, nMsgLen);
		}
	}
	else if (0 == _tcsnicmp(lpszMethodGet, pSt_HTTPParament->tszHttpMethod, _tcslen(lpszMethodGet)))
	{
		int nListCount = 0;
		TCHAR** pptszList;
		TCHAR tszUrlName[128];
		LPCTSTR lpszFuncName = _T("api");

		memset(tszUrlName, '\0', sizeof(tszUrlName));
		RfcComponents_HttpHelp_GetParament(pSt_HTTPParament->tszHttpUri, &pptszList, &nListCount, tszUrlName);
		if ((nListCount < 1) || (0 != _tcsnicmp(lpszFuncName, tszUrlName, _tcslen(lpszFuncName))))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "request is failed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
			return FALSE;
		}
		XEngine_AuthorizeHTTP_Token(lpszClientAddr, lpszMsgBuffer, nMsgLen, pptszList, nListCount);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
	}
	else
	{
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 405, "method not allow");
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,发送的方法:%s 不支持"), lpszClientAddr, pSt_HTTPParament->tszHttpMethod);
		return FALSE;
	}
	
	return TRUE;
}