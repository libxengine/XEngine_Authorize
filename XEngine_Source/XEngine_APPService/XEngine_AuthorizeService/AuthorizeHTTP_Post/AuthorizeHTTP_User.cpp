#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_User(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 8196;
	XCHAR tszSDBuffer[8196];
	LPCXSTR lpszAPIDelete = _X("delete");
	LPCXSTR lpszAPIRegister = _X("register");
	LPCXSTR lpszAPIPay = _X("pay");
	LPCXSTR lpszAPIPass = _X("pass");
	LPCXSTR lpszAPITime = _X("time");
	LPCXSTR lpszAPITry = _X("try");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsxnicmp(lpszAPIName, lpszAPIDelete, _tcsxlen(lpszAPIName)))
	{
		XENGINE_PROTOCOL_USERINFO st_UserInfo;
		memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));

		if (!st_FunSwitch.bSwitchDelete)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 503, "the function is closed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，删除失败，删除功能已经被服务器关闭!"), lpszClientAddr);
			return false;
		}
		Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, &st_UserInfo);
		//关闭链接
		int nListCount = 0;
		AUTHSESSION_NETCLIENT** ppSt_ListClient;
		Session_Authorize_GetClient(&ppSt_ListClient, &nListCount, st_UserInfo.tszUserName);
		for (int i = 0; i < nListCount; i++)
		{
			XEngine_CloseClient(ppSt_ListClient[i]->tszClientAddr, true);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);

		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_UserDelete(st_UserInfo.tszUserName);
		}
		else
		{
			DBModule_MySQL_UserDelete(st_UserInfo.tszUserName);
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求删除用户:%s 成功,在线用户数:%d"), lpszClientAddr, st_UserInfo.tszUserName, nListCount);
	}
	else if (0 == _tcsxnicmp(lpszAPIName, lpszAPIRegister, _tcsxlen(lpszAPIName)))
	{
		//用户注册
		AUTHREG_USERTABLE st_UserTable;
		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));

		if (!st_FunSwitch.bSwitchRegister)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 503, "the function is closed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，注册失败，注册功能已经被服务器关闭!"), lpszClientAddr);
			return false;
		}
		Protocol_Parse_HttpParseTable(lpszMsgBuffer, nMsgLen, &st_UserTable);
		//是否在黑名单
		AUTHREG_BANNED st_Banned;
		memset(&st_Banned, '\0', sizeof(AUTHREG_BANNED));

		_tcsxcpy(st_Banned.tszUserName, st_UserTable.st_UserInfo.tszUserName);
		bool bSuccess = false;
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			bSuccess = DBModule_SQLite_BannedExist(&st_Banned);
		}
		else 
		{
			bSuccess = DBModule_MySQL_BannedExist(&st_Banned);
		}
		if (bSuccess) 
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 423, "user name is banned");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，用户名：%s，注册失败，用户名已经被禁用!"), lpszClientAddr, st_Banned.tszUserName);
			return false;
		}
		//填充写入数据
		if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_UserTable.enSerialType)
		{
			_xstprintf(st_UserTable.tszLeftTime, _X("%d"), st_AuthConfig.st_XVerification.nTryTime);
			st_UserTable.enSerialType = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)st_AuthConfig.st_XVerification.nTryMode;
		}
		//禁止权限0和1注册
		if (st_UserTable.st_UserInfo.nUserLevel <= 1)
		{
			st_UserTable.st_UserInfo.nUserLevel = ENUM_XENGINE_PROTOCOLHDR_LEVEL_TYPE_USER;
		}
		if (_tcsxlen(st_UserTable.st_UserInfo.tszUserName) <= 0 || _tcsxlen(st_UserTable.st_UserInfo.tszUserPass) <= 0)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "user and pass does not set");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，注册失败，没有设置用户和密码"), lpszClientAddr);
			return false;
		}
		bSuccess = false;
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			bSuccess = DBModule_SQLite_UserRegister(&st_UserTable);
		}
		else 
		{
			bSuccess = DBModule_MySQL_UserRegister(&st_UserTable);
		}
		if (!bSuccess) 
		{
			XLONG dwRet = DBModule_GetLastError();
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, dwRet, "user register is failed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，用户名：%s，注册失败，无法继续，错误：%X"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName, dwRet);
			return false;
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端：%s，用户名：%s，注册成功"), lpszClientAddr, st_UserTable.st_UserInfo.tszUserName);
	}
	else if (0 == _tcsxnicmp(lpszAPIName, lpszAPIPay, _tcsxlen(lpszAPIName)))
	{
		AUTHREG_PROTOCOL_USERPAY st_UserPay;
		AUTHREG_USERTABLE st_UserInfo;

		memset(&st_UserInfo, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_UserPay, '\0', sizeof(AUTHREG_PROTOCOL_USERPAY));

		if (!st_FunSwitch.bSwitchPay)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 503, "the function is closed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，充值失败，充值功能已经被服务器关闭!"), lpszClientAddr);
			return false;
		}
		Protocol_Parse_HttpParsePay(lpszMsgBuffer, nMsgLen, &st_UserPay);
		bool bSuccess = false;
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			bSuccess = DBModule_SQLite_UserPay(st_UserPay.tszUserName, st_UserPay.tszSerialNumber);
		}
		else 
		{
			bSuccess = DBModule_MySQL_UserPay(st_UserPay.tszUserName, st_UserPay.tszSerialNumber);
		}
		if (!bSuccess) 
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "Serial number not available");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，用户名：%s，充值失败，无法继续，错误：%X"), lpszClientAddr, st_UserPay.tszUserName, DBModule_GetLastError());
			return false;
		}
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			DBModule_SQLite_UserQuery(st_UserPay.tszUserName, &st_UserInfo);
		}
		else
		{
			DBModule_MySQL_UserQuery(st_UserPay.tszUserName, &st_UserInfo);
		}
		Session_Authorize_SetUser(&st_UserInfo);
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端：%s，用户名：%s，充值成功，序列号：%s"), lpszClientAddr, st_UserPay.tszUserName, st_UserPay.tszSerialNumber);
	}
	else if (0 == _tcsxnicmp(lpszAPIName, lpszAPIPass, _tcsxlen(lpszAPIName)))
	{
		AUTHREG_USERTABLE st_UserTable;
		XENGINE_PROTOCOL_USERINFO st_UserInfo;
		XENGINE_PROTOCOL_USERAUTH st_AuthProtocol;

		memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
		memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));
		memset(&st_AuthProtocol, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

		if (!st_FunSwitch.bSwitchPass)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 503, "the function is closed");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，找回密码失败，密码找回功能已经被服务器关闭!"), lpszClientAddr);
			return false;
		}
		Protocol_Parse_HttpParseUser(lpszMsgBuffer, nMsgLen, &st_UserInfo);
		bool bSuccess = false;
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			bSuccess = DBModule_SQLite_UserQuery(st_UserInfo.tszUserName, &st_UserTable);
		}
		else 
		{
			bSuccess = DBModule_MySQL_UserQuery(st_UserInfo.tszUserName, &st_UserTable);
		}
		if (!bSuccess) 
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，用户名：%s，找回密码失败，用户不存在"), lpszClientAddr, st_UserInfo.tszUserName);
			return false;
		}
		//安全验证判断
		if ((0 != _tcsxnicmp(st_UserInfo.tszEMailAddr, st_UserTable.st_UserInfo.tszEMailAddr, _tcsxlen(st_UserInfo.tszEMailAddr))) || (st_UserTable.st_UserInfo.nIDNumber != st_UserInfo.nIDNumber))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "user information is incorrent");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，用户名：%s，找回密码失败，验证信息失败"), lpszClientAddr, st_UserInfo.tszUserName);
			return false;
		}
		_tcsxcpy(st_AuthProtocol.tszUserName, st_UserTable.st_UserInfo.tszUserName);
		_tcsxcpy(st_AuthProtocol.tszUserPass, st_UserTable.st_UserInfo.tszUserPass);

		Protocol_Packet_HttpUserPass(tszSDBuffer, &nSDLen, &st_AuthProtocol);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端：%s，用户名：%s，找回密码成功"), lpszClientAddr, st_UserInfo.tszUserName);
	}
	else if (0 == _tcsxnicmp(lpszAPIName, lpszAPITime, _tcsxlen(lpszAPIName)))
	{
		int nListCount = 0;
		AUTHSESSION_NETCLIENT** ppSt_ListClient;
		XENGINE_PROTOCOL_USERAUTH st_UserAuth;

		memset(&st_UserAuth, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

		Protocol_Parse_HttpParseAuth(lpszMsgBuffer, nMsgLen, &st_UserAuth);
		if (!Session_Authorize_GetClient(&ppSt_ListClient, &nListCount, st_UserAuth.tszUserName))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 404, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，用户名：%s，获取时间失败，无法继续，错误：%X"), lpszClientAddr, st_UserAuth.tszUserName, Session_GetLastError());
			return false;
		}
		Protocol_Packet_UserTime(tszSDBuffer, &nSDLen, &ppSt_ListClient, nListCount);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端：%s，用户名：%s，获取时间成功，用户同时在线数：%d"), lpszClientAddr, st_UserAuth.tszUserName, nListCount);
	}
	else if (0 == _tcsxnicmp(lpszAPIName, lpszAPITry, _tcsxlen(lpszAPIName)))
	{
		AUTHREG_TEMPVER st_VERTemp;
		memset(&st_VERTemp, '\0', sizeof(AUTHREG_TEMPVER));

		if (!st_FunSwitch.bSwitchTry)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 501, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，请求临时试用失败，服务器关闭了此服务"), lpszClientAddr);
			return false;
		}
		Protocol_Parse_HttpParseTry(lpszMsgBuffer, nMsgLen, &st_VERTemp);
		bool bSuccess = false;
		//判断是使用哪个数据库
		if (0 == st_AuthConfig.st_XSql.nDBType) 
		{
			bSuccess = DBModule_SQLite_TryQuery(&st_VERTemp);
		}
		else 
		{
			bSuccess = DBModule_MySQL_TryQuery(&st_VERTemp);
		}
		if (bSuccess) 
		{
			__int64x nTimeSpan = 0;
			//根据方式来计算剩余时间
			if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == st_VERTemp.enVMode)
			{
				//次数卡需要更新才可以
				st_VERTemp.nLTime--;
				nTimeSpan = st_VERTemp.nLTime;
			}
			else
			{
				XENGINE_LIBTIMER st_TimeStart = {};
				XENGINE_LIBTIMER st_TimeEnd = {};
				XCHAR tszTimeStart[128] = {};
				XCHAR tszTimeEnd[128] = {};
				//时间戳转换
				BaseLib_OperatorTime_StrToTime(st_VERTemp.tszVDate, &st_TimeStart);
				st_TimeEnd.wDay = st_VERTemp.nVTime;
				//得到超时时间
				BaseLib_OperatorTimeSpan_CalForStu(&st_TimeStart, &st_TimeEnd);
				BaseLib_OperatorTime_TimeToStr(tszTimeEnd, NULL, true, &st_TimeEnd);
				//计算时间差
				BaseLib_OperatorTime_TimeToStr(tszTimeStart);
				BaseLib_OperatorTimeSpan_GetForStr(tszTimeStart, tszTimeEnd, &nTimeSpan);

				st_VERTemp.nLTime = (int)nTimeSpan;
			}
			//是否超过
			if (nTimeSpan >= 0)
			{
				if (0 == st_AuthConfig.st_XSql.nDBType) 
				{
					DBModule_SQLite_TrySet(&st_VERTemp);
				}
				else
				{
					DBModule_MySQL_TrySet(&st_VERTemp);
				}
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端：%s，序列号：%s，类型:%s，临时验证成功，剩余时间:%lld"), lpszClientAddr, st_VERTemp.tszVSerial, lpszXSerialType[st_VERTemp.enVMode], nTimeSpan);
			}
			else
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "timeout");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，序列号：%s，临时验证失败，此序列号已经试用到期"), lpszClientAddr, st_VERTemp.tszVSerial);
			}
		}
		else
		{
			//填充写入数据
			st_VERTemp.nVTime = st_AuthConfig.st_XVerification.nVerTime;
			st_VERTemp.enVMode = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)st_AuthConfig.st_XVerification.nVerMode;
			//看下是否启用了此功能,不支持分钟,因为不登录
			if ((ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_VERTemp.enVMode) || (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_SECOND == st_VERTemp.enVMode) || (st_VERTemp.nVTime <= 0))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 501, "the function server unavailable");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端：%s，序列号：%s，临时验证插入失败，因为服务器关闭了此功能"), lpszClientAddr, st_VERTemp.tszVSerial);
				return false;
			}
			bool bSuccess = false;
			if (0 == st_AuthConfig.st_XSql.nDBType) 
			{
				bSuccess = DBModule_SQLite_TryInsert(&st_VERTemp);
			}
			else 
			{
				bSuccess = DBModule_MySQL_TryInsert(&st_VERTemp);
			}
			if (!bSuccess) 
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 500, "Internal Server Error");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，序列号：%s，临时验证插入失败，无法继续，错误：%X"), lpszClientAddr, st_VERTemp.tszVSerial, DBModule_GetLastError());
			}
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端：%s，序列号：%s，临时验证插入成功"), lpszClientAddr, st_VERTemp.tszVSerial);
		}
	}
	return true;
}