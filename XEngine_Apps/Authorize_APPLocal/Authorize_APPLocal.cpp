#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp")
#ifdef _WIN64
#pragma comment(lib,"../../XEngine_Source/x64/Debug/jsoncpp")
#pragma comment(lib,"../../XEngine_Source/x64/Debug/AuthorizeModule_CDKey")
#else
#pragma comment(lib,"../../XEngine_Source/Debug/jsoncpp")
#pragma comment(lib,"../../XEngine_Source/Debug/AuthorizeModule_CDKey")
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <json/json.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
#include "../../XEngine_Source/XAuth_Protocol.h"
#include "../../XEngine_Source/AuthorizeModule_CDKey/CDKey_Define.h"
#include "../../XEngine_Source/AuthorizeModule_CDKey/CDKey_Error.h"

//需要优先配置XEngine
//WINDOWS支持VS2022 x64 debug 编译调试
//g++ -std=c++17 -Wall -g Authorize_APPLocal.cpp -o Authorize_APPLocal.exe -I ../../XEngine_Source/XEngine_Depend/XEngine_Module/jsoncpp -lXEngine_BaseLib -L ../../XEngine_Release -lXEngine_OPenSsl -lXClient_APIHelp -lAuthorizeModule_CDKey -ljsoncpp -Wl,-rpath=../../XEngine_Release

//#define XENGINE_AUTHORIZE_CDKEY_CRYPTO

LPCXSTR lpszPasswd = _X("123123");

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	int nLen = 0;
	int nCode = 0;
	Json::Value st_JsonRoot;
	Json::Value st_JsonAPPInfo;
	Json::Value st_JsonREGInfo;
	Json::Value st_JsonUserInfo;
	st_JsonRoot["tszAddr"] = "http://app.xyry.org";
	st_JsonRoot["nPort"] = 5501;

	st_JsonAPPInfo["tszAppName"] = "XEngine";
	st_JsonAPPInfo["tszAppVer"] = "1.0.0.1001";

	st_JsonREGInfo["tszHardware"] = "5501012NE21N";
	st_JsonREGInfo["enSerialType"] = ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_TIME;
	st_JsonREGInfo["enRegType"] = ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_TRY;
	st_JsonREGInfo["enHWType"] = ENUM_AUTHORIZE_MODULE_HW_TYPE_CPU;

	st_JsonUserInfo["tszUserName"] = "qyt";
	st_JsonUserInfo["tszUserContact"] = "486179@qq.com";

	st_JsonRoot["st_AuthAppInfo"] = st_JsonAPPInfo;
	st_JsonRoot["st_AuthRegInfo"] = st_JsonREGInfo;
	st_JsonRoot["st_AuthUserInfo"] = st_JsonUserInfo;

	XCHAR* ptszCreateBuffer = NULL;
	LPCXSTR lpszCreateUrl = _X("http://192.168.1.10:5302/auth/cdkey/create");
	//1. 创建CDKEY
#ifdef XENGINE_AUTHORIZE_CDKEY_CRYPTO
	//加密
	CHAR tszCodecBuffer[4096];
	memset(tszCodecBuffer, '\0', sizeof(tszCodecBuffer));

	nLen = st_JsonRoot.toStyledString().length();
	OPenSsl_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nLen, (XBYTE *)tszCodecBuffer, lpszPasswd);
	if (!APIClient_Http_Request(_X("POST"), lpszCreateUrl, tszCodecBuffer, &nCode, &ptszCreateBuffer, &nLen))
#else
	if (!APIClient_Http_Request(_X("POST"), lpszCreateUrl, st_JsonRoot.toStyledString().c_str(), &nCode, &ptszCreateBuffer, &nLen))
#endif
	{
		printf("发送投递失败！\n");
		return 0;
	}
	
	XCHAR tszMsgBuffer[4096];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
#ifdef XENGINE_AUTHORIZE_CDKEY_CRYPTO
	//解密
	memset(tszCodecBuffer, '\0', sizeof(tszCodecBuffer));

	OPenSsl_XCrypto_Decoder(ptszCreateBuffer, &nLen, tszCodecBuffer, lpszPasswd);
	printf("接受到数据,大小:%d,内容:\n%s\n", nLen, tszCodecBuffer);
	//你也可以通过授权模块的API函数来读内存,都一回事,这里为了方便直接写了,请求分钟卡,拥有10分钟,也可以写自定义时间格式
	BaseLib_OperatorFile_WriteProfileFromMemory(tszCodecBuffer, nLen, "AuthReg", "tszLeftTime", "10", tszMsgBuffer, &nLen);
#else
	BaseLib_OperatorFile_WriteProfileFromMemory(ptszCreateBuffer, nLen, "AuthReg", "tszLeftTime", "10", tszMsgBuffer, &nLen);
	printf("接受到数据,大小:%d,内容:\n%s\n", nLen, ptszCreateBuffer);
#endif
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszCreateBuffer);
	//2. 授权CDKEY
	XCHAR* ptszAuthBuffer = NULL;
	LPCXSTR lpszAuthUrl = _X("http://192.168.1.10:5302/auth/cdkey/auth");
#ifdef XENGINE_AUTHORIZE_CDKEY_CRYPTO
	//加密
	memset(tszCodecBuffer, '\0', sizeof(tszCodecBuffer));
	nLen = nLen;
	OPenSsl_XCrypto_Encoder(tszMsgBuffer, &nLen, (XBYTE*)tszCodecBuffer, lpszPasswd);
	if (!APIClient_Http_Request(_X("POST"), lpszAuthUrl, tszCodecBuffer, &nCode, &ptszAuthBuffer, &nLen))
#else
	if (!APIClient_Http_Request(_X("POST"), lpszAuthUrl, tszMsgBuffer, &nCode, &ptszAuthBuffer, &nLen))
#endif
	{
		printf("发送投递失败！\n");
		return 0;
	}
#ifdef XENGINE_AUTHORIZE_CDKEY_CRYPTO
	//解密
	memset(tszCodecBuffer, '\0', sizeof(tszCodecBuffer));

	OPenSsl_XCrypto_Decoder(ptszAuthBuffer, &nLen, tszCodecBuffer, lpszPasswd);
	printf("接受到数据,大小:%d,内容:\n%s\n", nLen, tszCodecBuffer);
	//你也可以通过授权模块的API函数来读内存,都一回事,这里为了方便直接写了,请求分钟卡,拥有10分钟,也可以写自定义时间格式
	BaseLib_OperatorFile_WriteProfileFromMemory(tszCodecBuffer, nLen, "AuthReg", "tszLeftTime", "10", tszMsgBuffer, &nLen);
#else
	printf("接受到数据,大小:%d,内容:\n%s\n", nLen, ptszAuthBuffer);
	BaseLib_OperatorFile_WriteProfileFromMemory(ptszAuthBuffer, nLen, "AuthReg", "tszLeftTime", "10", tszMsgBuffer, &nLen);
#endif
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszAuthBuffer);
	//3. 验证CDKEY
	XCHAR* ptszVerBuffer = NULL;
	LPCXSTR lpszVerUrl = _X("http://192.168.1.10:5302/auth/cdkey/ver");

	XENGINE_AUTHORIZE_LOCAL st_Authorize;
	memset(&st_Authorize, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));

#ifdef XENGINE_AUTHORIZE_CDKEY_CRYPTO
//加密
	memset(tszCodecBuffer, '\0', sizeof(tszCodecBuffer));
	OPenSsl_XCrypto_Encoder(tszMsgBuffer, &nLen, (XBYTE*)tszCodecBuffer, lpszPasswd);
	if (!APIClient_Http_Request(_X("POST"), lpszVerUrl, tszCodecBuffer, &nCode, &ptszVerBuffer, &nLen))
#else
	if (!APIClient_Http_Request(_X("POST"), lpszVerUrl, tszMsgBuffer, &nCode, &ptszVerBuffer, &nLen))
#endif
	{
		printf("发送投递失败！\n");
		return 0;
	}
#ifdef XENGINE_AUTHORIZE_CDKEY_CRYPTO
	//解密
	memset(tszCodecBuffer, '\0', sizeof(tszCodecBuffer));

	OPenSsl_XCrypto_Decoder(ptszVerBuffer, &nLen, tszCodecBuffer, lpszPasswd);
	printf("接受到数据,大小:%d,内容:\n%s\n", nLen, tszCodecBuffer);
	Authorize_CDKey_ReadMemory(tszCodecBuffer, nLen, &st_Authorize);
#else
	printf("接受到数据,大小:%d,内容:\n%s\n", nLen, ptszVerBuffer);
	Authorize_CDKey_ReadMemory(ptszVerBuffer, nLen, &st_Authorize);
#endif
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszVerBuffer);

	//4. 也可以本地验证
	if (Authorize_CDKey_GetLeftTimer(&st_Authorize))
	{
		printf("ok\n");
	}
	else
	{
		printf("timeout\n");
	}
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}