#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_Cryption")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi")
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
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/Cryption_Define.h>
#include <XEngine_Include/XEngine_Core/Cryption_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include "../../XEngine_Source/XEngine_DependLibrary/XEngine_Module/XEngine_Verification/Verification_Define.h"
#include "../../XEngine_Source/XEngine_DependLibrary/XEngine_Module/XEngine_Verification/Verification_Error.h"
#include "../../XEngine_Source/XAuth_Protocol.h"

//需要优先配置XEngine
//WINDOWS支持VS2022 x86 debug 编译调试
//g++ -std=c++17 -Wall -g Authorize_APPLocal.cpp -o Authorize_APPLocal.exe -I ../../XEngine_Source/XEngine_DependLibrary/XEngine_Module/jsoncpp -L ../../XEngine_Release -lXEngine_BaseLib -lXEngine_Cryption -lXEngine_SystemApi -lXEngine_Verification -ljsoncpp -Wl,-rpath=../../XEngine_Release

XCHAR tszSerialStr[XPATH_MAX] = {};
//1.创建CDKEY.或者由管理员创建.
bool Authorize_APPLocal_Create(LPCXSTR lpszKeyFile, LPCXSTR lpszPasswd)
{
	VERIFICATION_XAUTHKEY st_AuthLocal = {};

	if (!Verification_XAuthKey_KeyInit(&st_AuthLocal))
	{
		printf("Verification_XAuthKey_KeyInit is failed\n");
		return false;
	}
	_tcsxcpy(tszSerialStr, st_AuthLocal.st_AuthSerial.st_TimeLimit.tszTimeSerial);
	//用户信息
	_xstprintf(st_AuthLocal.st_AuthUserInfo.tszUserName, _X("qyt"));
	_xstprintf(st_AuthLocal.st_AuthUserInfo.tszUserContact, _X("486179@qq.com"));

	Verification_XAuthKey_FileWrite(&st_AuthLocal, lpszKeyFile, lpszPasswd);
	return true;
}
//2.打开CDKEY并且授权(授权只能通过自我授权或者由管理员授权,用户不应该可以自己随意授权)
bool Authorize_APPLocal_Auth(LPCXSTR lpszKeyFile, LPCXSTR lpszPasswd)
{
	VERIFICATION_XAUTHKEY st_AuthLocal = {};

	if (!Verification_XAuthKey_FileRead(&st_AuthLocal, lpszKeyFile, lpszPasswd))
	{
		printf("Verification_XAuthKey_FileRead is failed\n");
		return false;
	}
	if (!Verification_XAuthKey_UserRegister(&st_AuthLocal, tszSerialStr))
	{
		printf("serila verifacation is failed,error code:%lX\n", Verification_GetLastError());
		return false;
	}
	Verification_XAuthKey_FileWrite(&st_AuthLocal, lpszKeyFile, lpszPasswd);
	return true;
}
//3.验证CDkey
bool Authorize_APPLocal_Ver(LPCXSTR lpszKeyFile, LPCXSTR lpszPasswd)
{
	VERIFICATION_XAUTHKEY st_AuthLocal = {};
	if (!Verification_XAuthKey_FileRead(&st_AuthLocal, lpszKeyFile, lpszPasswd))
	{
		printf("Verification_XAuthKey_FileRead is failed\n");
		return false;
	}
	if (!Verification_XAuthKey_KeyParse(&st_AuthLocal))
	{
		printf("verifaction is failed\n");
	}
	Verification_XAuthKey_FileWrite(&st_AuthLocal, lpszKeyFile, lpszPasswd);
	return true;
}
int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
	LPCXSTR lpszFileStr = _X("D:\\XEngine_Authorize\\XEngine_Apps\\Debug\\cd.key");
#else
	LPCXSTR lpszFileStr = _X("cd.key");
#endif
	LPCXSTR lpszPasswd = _X("123123");
	
	if (!Authorize_APPLocal_Create(lpszFileStr, lpszPasswd))
	{
		return -1;
	}
	Authorize_APPLocal_Auth(lpszFileStr, lpszPasswd);

	if (!Authorize_APPLocal_Ver(lpszFileStr, lpszPasswd))
	{
		return -1;
	}

	for (int i = 0; i < 10; i++)
	{
		if (!Authorize_APPLocal_Ver(lpszFileStr, lpszPasswd))
		{
			break;
		}
		printf("ok\n");
	}
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}