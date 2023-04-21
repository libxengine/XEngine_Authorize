#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_DynamicCode(LPCXSTR lpszClientAddr, XCHAR** pptszList, int nListCount)
{
	int nMsgLen = 0;
	XCHAR tszMsgBuffer[4096];
	XCHAR tszURLKey[128];
	XCHAR tszURLValue[128];
	LPCXSTR lpszAPIGet = _X("get");

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(tszURLKey, '\0', sizeof(tszURLKey));
	memset(tszURLValue, '\0', sizeof(tszURLValue));
	BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszURLKey, tszURLValue);

	if (0 == _tcsxnicmp(lpszAPIGet, tszURLValue, _tcsxlen(lpszAPIGet)))
	{
		//http://app.xyry.org:5302/api?function=dcode&user=get
		int nDCode = 0;
		XNETHANDLE xhToken = 0;
		AuthHelp_DynamicCode_Create(&xhToken, &nDCode);
		Protocol_Packet_HttpToken(tszMsgBuffer, &nMsgLen, xhToken, st_AuthConfig.st_XVerification.nDynamicTimeout, nDCode);
		XEngine_Client_TaskSend(lpszClientAddr, tszMsgBuffer, nMsgLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求获得动态验证码成功,TOKEN:%lld,验证码:%d"), lpszClientAddr, xhToken, nDCode);
	}
	return true;
}