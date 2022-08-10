#include "Authorize_Hdr.h"

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
	int nSDLen = 2048;
	TCHAR tszSDBuffer[2048];
	LPCTSTR lpszMethodGet = _T("GET");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 != _tcsnicmp(lpszMethodGet, pSt_HTTPParament->tszHttpMethod, _tcslen(lpszMethodGet)))
	{
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,发送的方法:%s 不支持"), lpszClientAddr, pSt_HTTPParament->tszHttpMethod);
		return FALSE;
	}
	TCHAR tszAPIType[64];
	TCHAR tszAPIVer[64];
	TCHAR tszAPIName[64];
	LPCTSTR lpszAPIType = _T("auth");
	LPCTSTR lpszAPIVer = _T("client");

	memset(tszAPIType, '\0', sizeof(tszAPIType));
	memset(tszAPIVer, '\0', sizeof(tszAPIVer));
	memset(tszAPIName, '\0', sizeof(tszAPIName));

	RfcComponents_HttpHelp_GetUrlApi(pSt_HTTPParament->tszHttpUri, tszAPIType, tszAPIVer, tszAPIName);
	if (0 != _tcsnicmp(lpszAPIType, tszAPIType, _tcslen(lpszAPIType)))
	{
		XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求的API:%s 不支持"), lpszClientAddr, pSt_HTTPParament->tszHttpUri);
		return FALSE;
	}
	if (0 == _tcsnicmp(lpszAPIVer, tszAPIVer, _tcslen(lpszAPIVer)))
	{
		LPCTSTR lpszAPIList = _T("list");
		LPCTSTR lpszAPIClose = _T("close");
		LPCTSTR lpszAPIModify = _T("modify");
		//枚举文件
		if (0 == _tcsnicmp(lpszAPIList, tszAPIName, _tcslen(lpszAPIList)))
		{
			//得到在线用户
			int nOnCount = 0;
			AUTHREG_USERTABLE** ppSt_ListClient;
			Session_Authorize_GetClient(&ppSt_ListClient, &nOnCount);
			//得到所有用户
			int nOffCount = 0;
			AUTHREG_USERTABLE** ppSt_UserInfo;
			Database_SQLite_UserList(&ppSt_UserInfo, &nOffCount);

			Protocol_Packet_HttpClientList(tszSDBuffer, &nSDLen, &ppSt_ListClient, nOnCount, &ppSt_UserInfo, nOffCount);

			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nOnCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_UserInfo, nOffCount);
			XEngine_Client_TaskSend(lpszClientAddr, NULL, XENGINE_AUTH_APP_NETTYPE_HTTP, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求客户端列表成功,在线用户:%d,离线用户:%d"), lpszClientAddr, nOnCount, nOffCount);
		}
	}
	return TRUE;
}