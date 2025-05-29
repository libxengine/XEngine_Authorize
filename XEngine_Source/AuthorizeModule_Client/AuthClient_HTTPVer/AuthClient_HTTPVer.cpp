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
  意思：函数的API地址,需要输入完整的URL
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入密码,如果服务端设置了密码客户端也必须使用加密通信
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthClient_HTTPVer::AuthClient_HTTPVer_TryRequest(LPCXSTR lpszURLAddr, LPCXSTR lpszPass /* = NULL */)
{
	AuthClient_IsErrorOccur = true;

	if (NULL == lpszURLAddr)
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_HTTPVER_PARAMENT;
		return false;
	}
#if (1 == _XAUTH_BUILD_SWITCH_CLIENT_HTTP)
	int nHTTPCode = 0;
	XCHAR tszJsonStr[XPATH_MAX] = {};
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

	if (NULL != lpszPass)
	{
		XCHAR tszENCodec[2048] = {};
		XCHAR tszDECodec[2048] = {};

		nMsgLen = st_JsonRoot.toStyledString().length();
		Cryption_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (XBYTE*)tszENCodec, lpszPass);
		APIClient_Http_Request(_X("POST"), lpszURLAddr, tszENCodec, &nHTTPCode, &ptszMsgBuffer, &nMsgLen);

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszDECodec, lpszPass);
		st_JsonRoot.clear();
		st_JsonObject.clear();
		//解析回复
		std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
		if (!pSt_JsonReader->parse(tszDECodec, tszDECodec + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			AuthClient_IsErrorOccur = true;
			AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_HTTPVER_PARSE;
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			return false;
		}
	}
	else
	{
		APIClient_Http_Request(_X("POST"), lpszURLAddr, st_JsonRoot.toStyledString().c_str(), &nHTTPCode, &ptszMsgBuffer, &nMsgLen);
		st_JsonRoot.clear();
		st_JsonObject.clear();
		//解析回复
		std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			AuthClient_IsErrorOccur = true;
			AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_HTTPVER_PARSE;
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			return false;
		}
	}
	
	if (0 != st_JsonRoot["code"].asInt())
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_HTTPVER_VERFAILED;
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		return false;
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
#endif
	return true;
}
/********************************************************************
函数名称：AuthClient_HTTPVer_GetDCode
函数功能：获取动态码
 参数.一：lpszURLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器地址,完成的API地址,比如:http://127.0.0.1:5302/api?function=dcode&user=get
 参数.二：pInt_DYCode
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出动态码
 参数.三：pxhToken
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出绑定的句柄
 参数.四：pInt_Timeout
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出动态码超时时间
 参数.五：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入密码,如果服务端设置了密码客户端也必须使用加密通信
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAuthClient_HTTPVer::AuthClient_HTTPVer_GetDCode(LPCXSTR lpszURLAddr, int* pInt_DYCode, XNETHANDLE* pxhToken, int* pInt_Timeout /* = NULL */, LPCXSTR lpszPass /* = NULL */)
{
	AuthClient_IsErrorOccur = true;

	if (NULL == lpszURLAddr)
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_HTTPVER_PARAMENT;
		return false;
	}
#if (1 == _XAUTH_BUILD_SWITCH_CLIENT_HTTP)
	int nHTTPCode = 0;
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	//请求
	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;

	APIClient_Http_Request(_X("GET"), lpszURLAddr, NULL, &nHTTPCode, &ptszMsgBuffer, &nMsgLen);
	if (NULL != lpszPass)
	{
		XCHAR tszDECodec[2048] = {};
		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszDECodec, lpszPass);

		std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
		if (!pSt_JsonReader->parse(tszDECodec, tszDECodec + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			AuthClient_IsErrorOccur = true;
			AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_HTTPVER_PARSE;
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			return false;
		}
	}
	else
	{
		//解析回复
		std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			AuthClient_IsErrorOccur = true;
			AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_HTTPVER_PARSE;
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			return false;
		}
	}

	if (0 != st_JsonRoot["code"].asInt())
	{
		AuthClient_IsErrorOccur = true;
		AuthClient_dwErrorCode = ERROR_AUTHORIZE_MODULE_HTTPVER_VERFAILED;
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		return false;
	}

	*pInt_DYCode = st_JsonRoot["nDynamicCode"].asInt();
	*pxhToken = st_JsonRoot["xhToken"].asUInt64();
	if (NULL != pInt_Timeout)
	{
		*pInt_Timeout = st_JsonRoot["nTimeout"].asInt();
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
#endif
	return true;
}