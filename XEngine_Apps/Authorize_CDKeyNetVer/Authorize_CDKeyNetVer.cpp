#ifdef _MSC_BUILD
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_Cryption")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemConfig")
#ifdef _WIN64
#pragma comment(lib,"../../XEngine_Source/x64/Debug/jsoncpp")
#pragma comment(lib,"../../XEngine_Source/x64/Debug/XEngine_Verification")
#else
#pragma comment(lib,"../../XEngine_Source/Debug/jsoncpp")
#pragma comment(lib,"../../XEngine_Source/Debug/XEngine_Verification")
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
#include <XEngine_Include/XEngine_Core/Cryption_Define.h>
#include <XEngine_Include/XEngine_Core/Cryption_Error.h>
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemConfig_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemConfig_Error.h>
#include "../../XEngine_Source/XEngine_DependLibrary/XEngine_Module/XEngine_Verification/Verification_Define.h"
#include "../../XEngine_Source/XEngine_DependLibrary/XEngine_Module/XEngine_Verification/Verification_Error.h"
#include "../../XEngine_Source/XAuth_Protocol.h"

//需要优先配置XEngine
//WINDOWS支持VS2022 x64 debug 编译调试
//g++ -std=c++17 -Wall -g Authorize_CDKeyNetVer.cpp -o Authorize_CDKeyNetVer.exe -I ../../XEngine_Source/XEngine_DependLibrary/XEngine_Module/jsoncpp -lXEngine_BaseLib -L ../../XEngine_Release -lXEngine_Cryption -lXClient_APIHelp -lXEngine_SystemConfig -lXEngine_Verification -ljsoncpp -Wl,-rpath=../../XEngine_Release

LPCXSTR lpszPasswd = _X("123123");

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	int nMSGLen = 0;
	XCHAR tszMSGBuffer[2048] = {};
	VERIFICATION_XAUTHKEY st_AuthLocal = {};

	XCHAR* ptszCreateBuffer = NULL;
	LPCXSTR lpszCreateUrl = _X("http://127.0.0.1:5302/auth/cdkey/create");
	//1. 创建CDKEY
	if (!APIClient_Http_Request(_X("POST"), lpszCreateUrl, NULL, NULL, &ptszCreateBuffer, &nMSGLen))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	Verification_XAuthKey_ReadMemory(ptszCreateBuffer, nMSGLen, &st_AuthLocal);
	sprintf(st_AuthLocal.st_AuthUserInfo.tszUserName, _X("qyt"));
	sprintf(st_AuthLocal.st_AuthUserInfo.tszUserContact, _X("486179@qq.com"));
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszCreateBuffer);

	//2. 授权CDKEY
	XCHAR* ptszAuthBuffer = NULL;
	LPCXSTR lpszAuthUrl = _X("http://127.0.0.1:5302/auth/cdkey/auth");
	Verification_XAuthKey_WriteMemory(tszMSGBuffer, &nMSGLen, &st_AuthLocal);
	if (!APIClient_Http_Request(_X("POST"), lpszAuthUrl, tszMSGBuffer, NULL, &ptszAuthBuffer, &nMSGLen))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	printf("接受到数据,大小:%d,内容:\n%s\n", nMSGLen, ptszAuthBuffer);
	memset(tszMSGBuffer, 0, sizeof(tszMSGBuffer));
	Verification_XAuthKey_WriteMemory(tszMSGBuffer, &nMSGLen, &st_AuthLocal);
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszAuthBuffer);
	//3. 验证CDKEY
	XCHAR* ptszVerBuffer = NULL;
	LPCXSTR lpszVerUrl = _X("http://127.0.0.1:5302/auth/cdkey/ver");

	VERIFICATION_XAUTHKEY st_Authorize = {};
	if (!APIClient_Http_Request(_X("POST"), lpszVerUrl, tszMSGBuffer, NULL, &ptszVerBuffer, &nMSGLen))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszVerBuffer);

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}