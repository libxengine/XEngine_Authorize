#include "../Authorize_Hdr.h"

BOOL XEngine_AuthorizeHTTP_Token(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR** pptszList, int nListCount)
{
	int nSDLen = 4096;
	TCHAR tszSDBuffer[4096];
	TCHAR tszURLKey[128];
	TCHAR tszURLValue[128];
	LPCTSTR lpszAPILogin = _T("login");
	LPCTSTR lpszAPIUPDate = _T("update");
	LPCTSTR lpszAPIClose = _T("close");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszURLKey, '\0', sizeof(tszURLKey));
	memset(tszURLValue, '\0', sizeof(tszURLValue));
	BaseLib_OperatorString_GetKeyValue(pptszList[0], "=", tszURLKey, tszURLValue);

	if (0 == _tcsnicmp(lpszAPILogin, tszURLValue, _tcslen(lpszAPILogin)))
	{
		//http://app.xyry.org:5302/api?function=login&user=123123aa&pass=123123
		TCHAR tszUserName[128];
		TCHAR tszUserPass[128];
		XNETHANDLE xhToken = 0;
		AUTHREG_USERTABLE st_UserTable;

		memset(tszUserName, '\0', sizeof(tszUserName));
		memset(tszUserPass, '\0', sizeof(tszUserPass));
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszURLKey, tszUserName);
		BaseLib_OperatorString_GetKeyValue(pptszList[2], "=", tszURLKey, tszUserPass);

		//是否使用了第三方验证
		if (st_AuthConfig.st_XLogin.bPassAuth)
		{
			//启用三方验证
			int nHTTPCode = 0;
			int nHTTPLen = 0;
			TCHAR* ptszMsgBuffer = NULL;
			XENGINE_PROTOCOL_USERAUTH st_AuthProtocol;
			memset(&st_AuthProtocol, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

			_tcscpy(st_AuthProtocol.tszUserName, tszUserName);
			_tcscpy(st_AuthProtocol.tszUserPass, tszUserPass);
			st_AuthProtocol.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB;

			Protocol_Packet_HttpUserPass(tszSDBuffer, &nSDLen, &st_AuthProtocol);
			APIHelp_HttpRequest_Post(st_AuthConfig.st_XLogin.st_PassUrl.tszPassLogin, tszSDBuffer, &nHTTPCode, &ptszMsgBuffer, &nHTTPLen);
			if (200 != nHTTPCode)
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，三方验证失败,错误码:%d"), lpszClientAddr, st_AuthProtocol.tszUserName, nHTTPCode);
				return FALSE;
			}
			Protocol_Parse_HttpParseTable(ptszMsgBuffer, nHTTPLen, &st_UserTable);
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		else
		{
			if (!Database_SQLite_UserQuery(tszUserName, &st_UserTable))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，登录失败，用户名不存在"), lpszClientAddr);
				return FALSE;
			}
			if ((_tcslen(tszUserPass) != _tcslen(st_UserTable.st_UserInfo.tszUserPass)) || (0 != _tcsncmp(tszUserPass, st_UserTable.st_UserInfo.tszUserPass, _tcslen(tszUserPass))))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "password is incorrent");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，登录失败，密码错误"), lpszClientAddr);
				return FALSE;
			}
		}
		//用户是否存在会话,存在就返回
		if (Session_Token_GetUser(tszUserName, tszUserPass, &xhToken))
		{
			if (st_AuthConfig.st_XLogin.bMultiLogin)
			{
				Protocol_Packet_HttpToken(tszSDBuffer, &nSDLen, xhToken, st_AuthConfig.st_XVerification.nTokenTimeout);
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求登录发现会话已经存在,直接返回TOKEN:%lld 成功"), lpszClientAddr, xhToken);
				return TRUE;
			}
			else
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "user was login");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求登录发现会话已经存在,服务器不允许重读登录"), lpszClientAddr);
				return FALSE;
			}
		}
		//是否被封禁
		if (-1 == st_UserTable.st_UserInfo.nUserLevel)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "User was banned");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，用户名：%s，登录失败，客户端已被封禁"), lpszClientAddr, tszUserName);
			return FALSE;
		}
		//处理权限
		if (st_UserTable.st_UserInfo.nUserLevel > 0)
		{
			//普通用户
			if (!st_AuthConfig.st_XLogin.bHTTPAuth)
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "User permission error");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，登录失败，客户端权限不足"), lpszClientAddr);
				return FALSE;
			}
			//是否已经登录
			TCHAR tszClientAddr[128];
			if (Session_Authorize_GetAddrForUser(tszUserName, tszClientAddr))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "User was login");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，用户名已经登录"), lpszClientAddr, tszUserName);
				return FALSE;
			}
			//分析充值类型
			if ((ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_UserTable.enSerialType) || ('0' == st_UserTable.tszLeftTime[0]))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "User not time");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，用户名：%s，登录失败，客户端类型错误"), lpszClientAddr, tszUserName);
				return FALSE;
			}
			st_UserTable.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_WEB;
			if (!Session_Authorize_Insert(lpszClientAddr, &st_UserTable, XENGINE_AUTH_APP_NETTYPE_HTTP))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 500, "server is error");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，用户名：%s，登录失败，插入会话管理失败,错误:%lX"), lpszClientAddr, tszUserName);
				return FALSE;
			}
		}
		BaseLib_OperatorHandle_Create(&xhToken);
		Session_Token_Insert(xhToken, &st_UserTable, st_UserTable.st_UserInfo.nUserLevel > 0 ? st_AuthConfig.st_XLogin.nHTTPAuthTime : 0);
		Protocol_Packet_HttpToken(tszSDBuffer, &nSDLen, xhToken, st_AuthConfig.st_XVerification.nTokenTimeout);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求登录获得TOKEN:%lld 成功,用户级别:%d"), lpszClientAddr, xhToken, st_UserTable.st_UserInfo.nUserLevel);
	}
	else if (0 == _tcsnicmp(lpszAPIUPDate, tszURLValue, _tcslen(lpszAPIUPDate)))
	{
		//http://app.xyry.org:5302/api?function=update&token=1000112345
		TCHAR tszUserToken[128];
		memset(tszUserToken, '\0', sizeof(tszUserToken));

		BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszURLKey, tszUserToken);

		if (!Session_Token_UPDate(_ttoi64(tszUserToken)))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端：%s，更新TOKEN失败，不存在的Token:%s"), lpszClientAddr, tszUserToken);
			return FALSE;
		}
		Protocol_Packet_HttpToken(tszSDBuffer, &nSDLen, _ttoi64(tszUserToken), st_AuthConfig.st_XVerification.nTokenTimeout);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求更新TOKEN:%s 成功"), lpszClientAddr, tszUserToken);
	}
	else if (0 == _tcsnicmp(lpszAPIClose, tszURLValue, _tcslen(lpszAPIClose)))
	{
		//http://app.xyry.org:5302/api?function=close&token=1000112345
		TCHAR tszUserToken[128];
		memset(tszUserToken, '\0', sizeof(tszUserToken));

		BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszURLKey, tszUserToken);
		Session_Token_Delete(_ttoi64(tszUserToken));
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求关闭TOKEN:%s 成功"), lpszClientAddr, tszUserToken);
	}
	return TRUE;
}