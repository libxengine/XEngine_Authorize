#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Try(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 8196;
	XCHAR tszSDBuffer[8196];

	LPCXSTR lpszAPIInsert = _X("insert");
	LPCXSTR lpszAPIDelete = _X("delete");
	LPCXSTR lpszAPIModify = _X("modify");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (0 == _tcsxnicmp(lpszAPIName, lpszAPIInsert, _tcsxlen(lpszAPIName)))
	{
		AUTHREG_NETVER st_AuthVer;
		memset(&st_AuthVer, '\0', sizeof(AUTHREG_NETVER));

		if (!st_FunSwitch.bSwitchTry)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 501, "user not found");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，请求临时试用失败，服务器关闭了此服务"), lpszClientAddr);
			return false;
		}
		Protocol_Parse_HttpParseTry(lpszMsgBuffer, nMsgLen, st_AuthVer.tszVerSerial);
		if (Database_SQLite_TryQuery(&st_AuthVer))
		{
			__int64x nTimeSpan = 0;
			//根据方式来计算剩余时间
			if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == st_AuthVer.enVerMode)
			{
				//次数卡需要更新才可以
				st_AuthVer.nTryTime--;
				Database_SQLite_TrySet(&st_AuthVer);
				nTimeSpan = st_AuthVer.nTryTime;
			}
			else
			{
				XENGINE_LIBTIMER st_TimeStart = {};
				XENGINE_LIBTIMER st_TimeEnd = {};
				XCHAR tszTimeStart[128] = {};
				XCHAR tszTimeEnd[128] = {};
				//时间戳转换
				BaseLib_OperatorTime_StrToTime(st_AuthVer.tszVerData, &st_TimeStart);
				st_TimeEnd.wDay = st_AuthVer.nTryTime;
				//得到超时时间
				BaseLib_OperatorTimeSpan_CalForStu(&st_TimeStart, &st_TimeEnd);
				BaseLib_OperatorTime_TimeToStr(tszTimeEnd, NULL, true, &st_TimeEnd);
				//计算时间差
				BaseLib_OperatorTime_TimeToStr(tszTimeStart);
				BaseLib_OperatorTimeSpan_GetForStr(tszTimeStart, tszTimeEnd, &nTimeSpan);
			}
			//是否超过
			if (nTimeSpan > 0)
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端：%s，序列号：%s，类型:%s，临时验证成功，剩余时间:%lld"), lpszClientAddr, st_AuthVer.tszVerSerial, lpszXSerialType[st_AuthVer.enVerMode], nTimeSpan);
			}
			else
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 400, "timeout");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，序列号：%s，临时验证失败，此序列号已经试用到期"), lpszClientAddr, st_AuthVer.tszVerSerial);
			}
		}
		else
		{
			//填充写入数据
			st_AuthVer.nTryTime = st_AuthConfig.st_XVerification.nVerTime;
			st_AuthVer.enVerMode = (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE)st_AuthConfig.st_XVerification.nVerMode;
			//看下是否启用了此功能,不支持分钟,因为不登录
			if ((ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_UNKNOW == st_AuthVer.enVerMode) || (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_SECOND == st_AuthVer.enVerMode) || (st_AuthVer.nTryTime <= 0))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 501, "the function server unavailable");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端：%s，序列号：%s，临时验证插入失败，因为服务器关闭了此功能"), lpszClientAddr, st_AuthVer.tszVerSerial);
				return false;
			}
			if (!Database_SQLite_TryInsert(&st_AuthVer))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, 500, "Internal Server Error");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端：%s，序列号：%s，临时验证插入失败，无法继续，错误：%X"), lpszClientAddr, st_AuthVer.tszVerSerial, DBModule_GetLastError());
			}
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端：%s，序列号：%s，临时验证插入成功"), lpszClientAddr, st_AuthVer.tszVerSerial);
		}
	}
	return true;
}