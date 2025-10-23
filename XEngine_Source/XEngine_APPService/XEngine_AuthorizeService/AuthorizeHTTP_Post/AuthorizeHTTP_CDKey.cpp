#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_CDKey(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 0;
	LPCXSTR lpszAPICreate = _X("create");
	LPCXSTR lpszAPIAuth = _X("auth");
	LPCXSTR lpszAPIVer = _X("ver");

	CHttpMemory_PoolEx m_MemoryPoolSend(XENGINE_MEMORY_SIZE_MAX);
	if (!st_FunSwitch.bSwitchCDKey)
	{
		Protocol_Packet_HttpComm(m_MemoryPoolSend.get(), &nSDLen, ERROR_AUTHORIZE_PROTOCOL_CLOSED, "the function is closed");
		XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPoolSend.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，CDKey验证授权失败，功能已经被服务器关闭!"), lpszClientAddr);
		return false;
	}
	if (0 == _tcsxnicmp(lpszAPICreate, lpszAPIName, _tcsxlen(lpszAPICreate)))
	{
		VERIFICATION_XAUTHKEY st_Authorize = {};

		Verification_XAuthKey_KeyInit(&st_Authorize);
		if (!Verification_XAuthKey_MemoryWrite(&st_Authorize, m_MemoryPoolSend.get(), &nSDLen))
		{
			Protocol_Packet_HttpComm(m_MemoryPoolSend.get(), &nSDLen, ERROR_AUTHORIZE_PROTOCOL_SERVER, "Not Acceptable,write key failed");
			XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPoolSend.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求创建CDKEY协议失败,读取CDKEY失败：%lX"), lpszClientAddr, Verification_GetLastError());
			return false;
		}
		XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPoolSend.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,请求创建CDKEY成功,APP名:%s,APP版本:%s"), lpszClientAddr, st_Authorize.st_AuthAppInfo.tszAppName, st_Authorize.st_AuthAppInfo.tszAppVer);
	}
	else if (0 == _tcsxnicmp(lpszAPIAuth, lpszAPIName, _tcsxlen(lpszAPIAuth)))
	{
		VERIFICATION_XAUTHKEY st_Authorize = {};
		if (!Verification_XAuthKey_MemoryRead(&st_Authorize, lpszMsgBuffer, nMsgLen))
		{
			Protocol_Packet_HttpComm(m_MemoryPoolSend.get(), &nSDLen, ERROR_AUTHORIZE_PROTOCOL_CDKEY, "Not Acceptable,read key failed");
			XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPoolSend.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求创建授权CDKEY协议失败,读取CDKEY失败：%lX"), lpszClientAddr, Verification_GetLastError());
			return false;
		}
		//授权
		if (!Verification_XAuthKey_UserRegister(&st_Authorize, st_Authorize.st_AuthSerial.st_TimeLimit.tszTimeSerial))
		{
			Protocol_Packet_HttpComm(m_MemoryPoolSend.get(), &nSDLen, ERROR_AUTHORIZE_PROTOCOL_SERVER, "Not Acceptable,read key failed");
			XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPoolSend.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求创建授权CDKEY协议失败,读取CDKEY失败：%lX"), lpszClientAddr, Verification_GetLastError());
			return false;
		}
		Verification_XAuthKey_MemoryWrite(&st_Authorize, m_MemoryPoolSend.get(), &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPoolSend.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,请求授权CDKEY成功,APP名:%s,APP版本:%s,授权期限:%s"), lpszClientAddr, st_Authorize.st_AuthAppInfo.tszAppName, st_Authorize.st_AuthAppInfo.tszAppVer, st_Authorize.st_AuthRegInfo.tszLeftTime);
	}
	else if (0 == _tcsxnicmp(lpszAPIVer, lpszAPIName, _tcsxlen(lpszAPIVer)))
	{
		VERIFICATION_XAUTHKEY st_Authorize = {};
		if (!Verification_XAuthKey_MemoryRead(&st_Authorize, lpszMsgBuffer, nMsgLen))
		{
			Protocol_Packet_HttpComm(m_MemoryPoolSend.get(), &nSDLen, ERROR_AUTHORIZE_PROTOCOL_CDKEY, "Not Acceptable,read key failed");
			XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPoolSend.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求创建授权CDKEY协议失败,读取CDKEY失败：%lX"), lpszClientAddr, Verification_GetLastError());
			return false;
		}
		if (ENUM_VERIFICATION_MODULE_VERMODE_TYPE_NETWORK != st_Authorize.st_AuthRegInfo.enVModeType)
		{
			Protocol_Packet_HttpComm(m_MemoryPoolSend.get(), &nSDLen, ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE, "unsupport,cdkey is not authorized");
			XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPoolSend.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求验证CDKEY失败,cdkey未授权或者已超时,错误：%lX"), lpszClientAddr, Verification_GetLastError());
			return false;
		}
		if (!Verification_XAuthKey_KeyParse(&st_Authorize))
		{
			Protocol_Packet_HttpComm(m_MemoryPoolSend.get(), &nSDLen, ERROR_AUTHORIZE_PROTOCOL_UNAUTHORIZE, "Unauthorized,cdkey is not authorized");
			XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPoolSend.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求验证CDKEY失败,cdkey未授权或者已超时,错误：%lX"), lpszClientAddr, Verification_GetLastError());
			return false;
		}
		Verification_XAuthKey_MemoryWrite(&st_Authorize, m_MemoryPoolSend.get(), &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPoolSend.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,请求验证CDKEY成功,APP名:%s,APP版本:%s,授权期限:%s"), lpszClientAddr, st_Authorize.st_AuthAppInfo.tszAppName, st_Authorize.st_AuthAppInfo.tszAppVer, st_Authorize.st_AuthRegInfo.tszLeftTime);
	}
	else
	{
		Protocol_Packet_HttpComm(m_MemoryPoolSend.get(), &nSDLen, ERROR_AUTHORIZE_PROTOCOL_NOTSUPPORT, "Not support protocol");
		XEngine_Client_TaskSend(lpszClientAddr, m_MemoryPoolSend.get(), nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,请求了一条未知的子协议：%s"), lpszClientAddr, lpszAPIName);
	}
	return true;
}