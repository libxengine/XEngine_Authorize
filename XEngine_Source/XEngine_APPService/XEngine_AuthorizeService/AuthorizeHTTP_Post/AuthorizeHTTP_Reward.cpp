#include "../Authorize_Hdr.h"

bool XEngine_AuthorizeHTTP_Reward(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	int nSDLen = 4096;
	XCHAR tszSDBuffer[4096] = {};
	LPCXSTR lpszAPIInsert = _X("insert");   
	LPCXSTR lpszAPIDelete = _X("delete");  
	LPCXSTR lpszAPIList = _X("list");  

	if (0 == _tcsxncmp(lpszAPIInsert, lpszAPIName, _tcsxlen(lpszAPIInsert)))
	{
		//http://127.0.0.1:5302/api/reward/insert
		AUTHREG_REWARD st_AuthReward = {};
		if (!Protocol_Parse_HttpParseReward(lpszMSGBuffer, nMSGLen, &st_AuthReward))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "request failure,json parse incorrect");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求插入奖励系统数据失败,解析失败:%s"), lpszClientAddr, lpszMSGBuffer);
			return false;
		}

		bool bRet = false;
		if (0 == st_AuthConfig.st_XSql.nDBType)
		{
			bRet = DBModule_SQLite_RewardInsert(&st_AuthReward);
		}
		else
		{
			bRet = DBModule_MySQL_RewardInsert(&st_AuthReward);
		}
		if (!bRet)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "service failure,insert info to db failure");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求插入奖励系统失败,失败：%lX"), lpszClientAddr, DBModule_GetLastError());
			return false;
		}
		Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求插入奖励系统:%s->%s 成功"), lpszClientAddr, st_AuthReward.tszUserName, st_AuthReward.tszReferrer);
	}
	else if (0 == _tcsxncmp(lpszAPIDelete, lpszAPIName, _tcsxlen(lpszAPIDelete)))
	{
		//http://127.0.0.1:5302/api/reward/delete
		AUTHREG_REWARD st_AuthReward = {};
		if (!Protocol_Parse_HttpParseReward(lpszMSGBuffer, nMSGLen, &st_AuthReward))
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "request failure,json parse incorrect");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求删除奖励系统数据失败,解析失败:%s"), lpszClientAddr, lpszMSGBuffer);
			return false;
		}
		bool bRet = false;
		if (0 == st_AuthConfig.st_XSql.nDBType)
		{
			bRet = DBModule_SQLite_RewardDelete(&st_AuthReward);
		}
		else
		{
			bRet = DBModule_MySQL_RewardDelete(&st_AuthReward);
		}
		if (!bRet)
		{
			Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "request info is correct,delete db failure");
			XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求删除奖励系统:%s->%s 失败, 错误：%lX"), lpszClientAddr, st_AuthReward.tszUserName, st_AuthReward.tszReferrer, DBModule_GetLastError());
			return false;
		}
		
		XEngine_Client_TaskSend(lpszClientAddr, NULL, 0, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求删除奖励系统:%s->%s 成功"), lpszClientAddr, st_AuthReward.tszUserName, st_AuthReward.tszReferrer);
	}
	else if (0 == _tcsxncmp(lpszAPIList, lpszAPIName, _tcsxlen(lpszAPIList)))
	{
		//http://127.0.0.1:5302/api/reward/list

		int nListCount = 0;
		AUTHREG_REWARD** ppSt_Reward;
		if (0 == nMSGLen)
		{
			if (0 == st_AuthConfig.st_XSql.nDBType)
			{
				DBModule_SQLite_RewardList(&ppSt_Reward, &nListCount);
			}
			else
			{
				DBModule_MySQL_RewardList(&ppSt_Reward, &nListCount);
			}
		}
		else
		{
			AUTHREG_REWARD st_AuthReward = {};
			if (!Protocol_Parse_HttpParseReward(lpszMSGBuffer, nMSGLen, &st_AuthReward))
			{
				Protocol_Packet_HttpComm(tszSDBuffer, &nSDLen, ERROR_AUTHORIZE_PROTOCOL_REQUEST, "request failure,json parse incorrect");
				XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求列举奖励系统数据失败,解析失败:%s"), lpszClientAddr, lpszMSGBuffer);
				return false;
			}
			if (0 == st_AuthConfig.st_XSql.nDBType)
			{
				DBModule_SQLite_RewardList(&ppSt_Reward, &nListCount, &st_AuthReward);
			}
			else
			{
				DBModule_MySQL_RewardList(&ppSt_Reward, &nListCount, &st_AuthReward);
			}
		}
		Protocol_Packet_HttpParseReward(tszSDBuffer, &nSDLen, &ppSt_Reward, nListCount);
		BaseLib_Memory_Free((XPPPMEM)&ppSt_Reward, nListCount);
		XEngine_Client_TaskSend(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_AUTH_APP_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求列举奖励系统成功,列表个数:%d"), lpszClientAddr, nListCount);
	}
	return true;
}