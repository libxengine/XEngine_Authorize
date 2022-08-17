#include "../Authorize_Hdr.h"

BOOL XEngine_AuthorizeHTTP_User(LPCTSTR lpszClientAddr, LPCTSTR lpszAPIName, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 4096;
	TCHAR tszSDBuffer[4096];
	LPCTSTR lpszAPIRegister = _T("register");
	LPCTSTR lpszAPILogin = _T("login");
	LPCTSTR lpszAPIPay = _T("pay");
	LPCTSTR lpszAPIPass = _T("pass");
	LPCTSTR lpszAPITime = _T("time");
	LPCTSTR lpszAPITry = _T("try");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	if (0 == _tcsnicmp(lpszAPIName, lpszAPIRegister, _tcslen(lpszAPIName)))
	{
		//用户注册
		AUTHREG_USERTABLE st_UserTable;
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, &st_UserTable.st_UserInfo);
		//填充写入数据
		_stprintf(st_UserTable.tszLeftTime, _T("%d"), st_AuthConfig.st_XVerification.nTryTime);
		st_UserTable.enSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)st_AuthConfig.st_XVerification.nTryMode;
		if (!Database_SQLite_UserRegister(&st_UserTable))
		{
			DWORD dwRet = DBModule_GetLastError();
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, dwRet, "user register is failed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，注册失败，无法继续，错误：%X"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName, dwRet);
			return FALSE;
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，注册成功"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
	}
	else if (0 == _tcsnicmp(lpszAPIName, lpszAPILogin, _tcslen(lpszAPIName)))
	{
		AUTHREG_USERTABLE st_UserTable;
		XENGINE_PROTOCOL_USERAUTH st_AuthProtocol;

		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_AuthProtocol, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

		Protocol_Parse_HttpParseAuth(lpszMsgBuffer, nMsgLen, &st_AuthProtocol);

		st_UserTable.enDeviceType = st_AuthProtocol.enDeviceType;
		if (!Database_SQLite_UserQuery(st_AuthProtocol.tszUserName, &st_UserTable))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，用户名不存在"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		if ((_tcslen(st_AuthProtocol.tszUserPass) != _tcslen(st_UserTable.st_UserInfo.tszUserPass)) || (0 != _tcsncmp(st_AuthProtocol.tszUserPass, st_UserTable.st_UserInfo.tszUserPass, _tcslen(st_AuthProtocol.tszUserPass))))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "password is incorrent");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，密码错误"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		//是否被封禁
		if (-1 == st_UserTable.st_UserInfo.nUserLevel)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "User has been banned");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，客户端已被封禁"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		//分析充值类型
		if ((ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_UserTable.enSerialType) || ('0' == st_UserTable.tszLeftTime[0]))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "user pay is timeout");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，没有剩余时间了"), lpszClientAddr, st_AuthProtocol.tszUserName);
			return FALSE;
		}
		if (!Session_Authorize_Insert(lpszClientAddr, &st_UserTable))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 500, "service internal error");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录失败，插入会话管理失败,错误:%lX"), lpszClientAddr, st_AuthProtocol.tszUserName, Session_GetLastError());
			return FALSE;
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，登录成功,注册类型:%d,剩余时间:%s"), lpszClientAddr, st_AuthProtocol.tszUserName, st_UserTable.enSerialType, st_UserTable.tszLeftTime);
	}
	else if (0 == _tcsnicmp(lpszAPIName, lpszAPIPay, _tcslen(lpszAPIName)))
	{
		AUTHREG_PROTOCOL_USERPAY st_UserPay;
		AUTHREG_USERTABLE st_UserInfo;

		memset(&st_UserInfo, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_UserPay, '\0', sizeof(AUTHREG_PROTOCOL_USERPAY));

		Protocol_Parse_HttpParsePay(lpszMsgBuffer, nMsgLen, &st_UserPay);
		if (!Database_SQLite_UserPay(st_UserPay.tszUserName, st_UserPay.tszSerialNumber))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "Serial number not available");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，充值失败，无法继续，错误：%X"), lpszClientAddr, st_UserPay.tszUserName, DBModule_GetLastError());
			return FALSE;
		}
		Database_SQLite_UserQuery(st_UserPay.tszUserName, &st_UserInfo);
		Session_Authorize_SetUser(&st_UserInfo);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，充值成功，序列号：%s"), lpszClientAddr, st_UserPay.tszUserName, st_UserPay.tszSerialNumber);
	}
	else if (0 == _tcsnicmp(lpszAPIName, lpszAPIPass, _tcslen(lpszAPIName)))
	{
		AUTHREG_USERTABLE st_UserTable;
		XENGINE_PROTOCOL_USERINFO st_UserInfo;
		XENGINE_PROTOCOL_USERAUTH st_AuthProtocol;

		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));
		memset(&st_AuthProtocol, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

		Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, &st_UserInfo);

		if (!Database_SQLite_UserQuery(st_UserInfo.tszUserName, &st_UserTable))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，找回密码失败，用户不存在"), lpszClientAddr, st_UserInfo.tszUserName);
			return FALSE;
		}
		//安全验证判断
		if ((0 != _tcsnicmp(st_UserInfo.tszEMailAddr, st_UserTable.st_UserInfo.tszEMailAddr, _tcslen(st_UserInfo.tszEMailAddr))) || (st_UserTable.st_UserInfo.nIDNumber != st_UserInfo.nIDNumber))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "user information is incorrent");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，找回密码失败，验证信息失败"), lpszClientAddr, st_UserInfo.tszUserName);
			return FALSE;
		}
		_tcscpy(st_AuthProtocol.tszUserName, st_UserTable.st_UserInfo.tszUserName);
		_tcscpy(st_AuthProtocol.tszUserPass, st_UserTable.st_UserInfo.tszUserPass);

		Protocol_Packet_HttpUserPass(tszSDBuffer, &nSDLen, &st_AuthProtocol);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，找回密码成功"), lpszClientAddr, st_UserInfo.tszUserName);
	}
	else if (0 == _tcsnicmp(lpszAPIName, lpszAPITime, _tcslen(lpszAPIName)))
	{
		AUTHREG_PROTOCOL_TIME st_AuthTime;
		XENGINE_PROTOCOL_USERAUTH st_UserAuth;

		memset(&st_AuthTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
		memset(&st_UserAuth, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

		Protocol_Parse_HttpParseAuth(lpszMsgBuffer, nMsgLen, &st_UserAuth);
		_tcscpy(st_AuthTime.tszUserName, st_UserAuth.tszUserName);
		if (!Session_Authorize_GetTimer(st_AuthTime.tszUserName, &st_AuthTime))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，用户名：%s，获取时间失败，无法继续，错误：%X"), lpszClientAddr, st_AuthTime.tszUserName, Session_GetLastError());
			return FALSE;
		}
		Protocol_Packet_HttpUserTime(tszSDBuffer, &nSDLen, &st_AuthTime);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，用户名：%s，获取时间成功，类型：%d，在线时间：%lld，剩余时间：%lld"), lpszClientAddr, st_AuthTime.tszUserName, st_AuthTime.enSerialType, st_AuthTime.nTimeONLine, st_AuthTime.nTimeLeft);
	}
	else if (0 == _tcsnicmp(lpszAPIName, lpszAPITry, _tcslen(lpszAPIName)))
	{
		AUTHREG_NETVER st_AuthVer;
		memset(&st_AuthVer, '\0', sizeof(AUTHREG_NETVER));

		memcpy(st_AuthVer.tszVerSerial, lpszMsgBuffer, nMsgLen);
		if (Database_SQLite_TryQuery(&st_AuthVer))
		{
			TCHAR tszEndTime[64];
			__int64x nTimeSpan = 0;
			memset(tszEndTime, '\0', sizeof(tszEndTime));
			//根据方式来计算剩余时间
			if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == st_AuthVer.enVerMode)
			{
				nTimeSpan = st_AuthVer.nTryTime;
			}
			else
			{
				//计算时间差
				BaseLib_OperatorTime_TimeToStr(tszEndTime);
				BaseLib_OperatorTimeSpan_GetForStr(st_AuthVer.tszVerData, tszEndTime, &nTimeSpan, 2);
			}
			//是否超过
			if (nTimeSpan > st_AuthConfig.st_XVerification.nVerTime)
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "timeout");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，序列号：%s，临时验证失败，此序列号已经试用到期"), lpszClientAddr, st_AuthVer.tszVerSerial);
			}
			else
			{
				if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == st_AuthVer.enVerMode)
				{
					//次数卡需要更新才可以
					st_AuthVer.nTryTime++;
					Database_SQLite_TrySet(&st_AuthVer);
				}
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "timeout");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，序列号：%s，临时验证成功，已用时间:%lld"), lpszClientAddr, st_AuthVer.tszVerSerial, nTimeSpan);
			}
		}
		else
		{
			//填充写入数据
			st_AuthVer.nTryTime = st_AuthConfig.st_XVerification.nVerTime;
			st_AuthVer.enVerMode = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)st_AuthConfig.st_XVerification.nVerMode;
			//看下是否启用了此功能
			if ((ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_AuthVer.enVerMode) || (st_AuthVer.nTryTime <= 0))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 501, "the function server unavailable");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("客户端：%s，序列号：%s，临时验证插入失败，因为服务器关闭了此功能"), lpszClientAddr, st_AuthVer.tszVerSerial);
				return FALSE;
			}
			if (!Database_SQLite_TryInsert(&st_AuthVer))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 500, "Internal Server Error");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，序列号：%s，临时验证插入失败，无法继续，错误：%X"), lpszClientAddr, st_AuthVer.tszVerSerial, DBModule_GetLastError());
			}
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，序列号：%s，临时验证插入成功"), lpszClientAddr, st_AuthVer.tszVerSerial);
		}
	}
	return TRUE;
}