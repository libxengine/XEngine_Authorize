#include "pch.h"
#include "AuthClient_HTTPVer.h"
/********************************************************************
//    Created:     2024/01/04  09:44:04
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client\AuthClient_HTTPVer\AuthClient_HTTPVer.cpp
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client\AuthClient_HTTPVer
//    File Base:   AuthClient_HTTPVer
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     临时授权客户端
//    History:
*********************************************************************/
CAuthClient_HTTPVer::CAuthClient_HTTPVer()
{
}
CAuthClient_HTTPVer::~CAuthClient_HTTPVer()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：AuthClient_HTTPVer_TryRequest
函数功能：试用版请求
 参数.一：lpszURLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthClient_HTTPVer::AuthClient_HTTPVer_TryRequest(LPCXSTR lpszURLAddr)
{
	AuthClient_IsErrorOccur = true;

	if (NULL == lpszURLAddr)
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_HTTPVER_PARAMENT;
		return false;
	}
#if (1 == _XAUTH_BUILD_SWITCH_CLIENT_TRY)
	int nHTTPCode = 0;
	XCHAR tszJsonStr[MAX_PATH] = {};
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial = {};

	SystemApi_HardWare_GetSerial(&st_SDKSerial);

	_xstprintf(tszJsonStr, _X("%s"), st_SDKSerial.tszBoardSerial);

	st_JsonObject["tszVSerial"] = tszJsonStr;
	st_JsonRoot["st_VERTemp"] = st_JsonObject;
	//请求
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	APIClient_Http_Request(_X("POST"), lpszURLAddr, st_JsonRoot.toStyledString().c_str(), &nHTTPCode, &ptszMsgBuffer, &nMsgLen);
	st_JsonRoot.clear();
	st_JsonObject.clear();
	//解析回复
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_HTTPVER_PARSE;
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		return false;
	}
	if (0 != st_JsonRoot["code"].asInt())
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_HTTPVER_VERFAILED;
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		return false;
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
#endif
	return true;
}