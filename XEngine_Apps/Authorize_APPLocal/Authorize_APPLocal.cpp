#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_Cryption")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi")
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
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/Cryption_Define.h>
#include <XEngine_Include/XEngine_Core/Cryption_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include "../../XEngine_Source/XAuth_Protocol.h"
#include "../../XEngine_Source/AuthorizeModule_CDKey/CDKey_Define.h"
#include "../../XEngine_Source/AuthorizeModule_CDKey/CDKey_Error.h"

//需要优先配置XEngine
//WINDOWS支持VS2022 x86 debug 编译调试
//g++ -std=c++17 -Wall -g Authorize_APPLocal.cpp -o Authorize_APPLocal.exe -I ../../XEngine_Source/XEngine_DependLibrary/XEngine_Module/jsoncpp -L ../../XEngine_Release -lXEngine_BaseLib -lXEngine_Cryption -lXEngine_SystemApi -lAuthorizeModule_CDKey -ljsoncpp -Wl,-rpath=../../XEngine_Release

XCHAR tszSerialStr[XPATH_MAX] = {};
//1.创建CDKEY.或者由管理员创建.
bool Authorize_APPLocal_Create(LPCXSTR lpszKeyFile, LPCXSTR lpszPasswd)
{
	int nRet = 0;
	XCHAR tszENCodecBuffer[4096] = {};
	XCHAR tszDECodecBuffer[4096] = {};
	XENGINE_AUTHORIZE_LOCAL st_AuthLocal = {};
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial = {};

	SystemApi_HardWare_GetSerial(&st_SDKSerial);
	//网络信息
	st_AuthLocal.nPort = 5302;
	_tcsxcpy(st_AuthLocal.tszAddr, _X("http://app.xyry.org"));
	//软件信息
	st_AuthLocal.st_AuthAppInfo.bInit = false;
	st_AuthLocal.st_AuthAppInfo.nExecTime = 0;
	_xstprintf(st_AuthLocal.st_AuthAppInfo.tszAppName, _X("XEngine_Authorize"));
	_xstprintf(st_AuthLocal.st_AuthAppInfo.tszAppVer, _X("V1.0.0.1"));
	//注册信息
	st_AuthLocal.st_AuthRegInfo.enHWType = ENUM_AUTHORIZE_MODULE_HW_TYPE_BIOS;
	st_AuthLocal.st_AuthRegInfo.enRegType = ENUM_AUTHORIZE_MODULE_CDKEY_TYPE_TRY;
	st_AuthLocal.st_AuthRegInfo.enSerialType = ENUM_AUTHORIZE_MODULE_SERIAL_TYPE_TIME;
	st_AuthLocal.st_AuthRegInfo.enVModeType = ENUM_AUTHORIZE_MODULE_VERMODE_TYPE_LOCAL;
	st_AuthLocal.st_AuthRegInfo.nHasTime = 0;
	BaseLib_Time_TimeToStr(st_AuthLocal.st_AuthRegInfo.tszCreateTime);
	_tcsxcpy(st_AuthLocal.st_AuthRegInfo.tszHardware, st_SDKSerial.tszBoardSerial);
	_xstprintf(st_AuthLocal.st_AuthRegInfo.tszLeftTime, _X("0"));           //0次试用
	//序列号信息.可以不写,如果不想启用用户自己注册.或者交给管理员填充
	int nSerialCount = 3;
	XCHAR** pptszSerialList;
	Authorize_Serial_Create(&pptszSerialList, _X("XAUTH"), nSerialCount, 9);
	_tcsxcpy(tszSerialStr, pptszSerialList[0]);

	st_AuthLocal.st_AuthSerial.st_TimeLimit.nTimeCount = 5;
	_tcsxcpy(st_AuthLocal.st_AuthSerial.st_TimeLimit.tszTimeSerial, pptszSerialList[0]);

	st_AuthLocal.st_AuthSerial.st_DataLimit.bTimeAdd = false;
	_tcsxcpy(st_AuthLocal.st_AuthSerial.st_DataLimit.tszDataSerial, pptszSerialList[1]);
	XCHAR tszTimeStr[128] = {};
	XENGINE_LIBTIME st_LibTime = {};
	BaseLib_Time_GetSysTime(&st_LibTime);
	st_LibTime.wYear += 1; //一年后过期
	BaseLib_Time_TimeToStr(tszTimeStr, NULL, true, &st_LibTime);

	_tcsxcpy(st_AuthLocal.st_AuthSerial.st_DataLimit.tszDataTime, tszTimeStr);

	_tcsxcpy(st_AuthLocal.st_AuthSerial.st_UNLimit.tszUNLimitSerial, pptszSerialList[2]);
	//用户信息
	_xstprintf(st_AuthLocal.st_AuthUserInfo.tszUserName, _X("qyt"));
	_xstprintf(st_AuthLocal.st_AuthUserInfo.tszUserContact, _X("486179@qq.com"));

	Authorize_CDKey_WriteMemory(tszDECodecBuffer, &nRet, &st_AuthLocal);
	Cryption_XCrypto_Encoder(tszDECodecBuffer, &nRet, (XBYTE*)tszENCodecBuffer, lpszPasswd);
	FILE* pSt_File = _xtfopen(lpszKeyFile, _X("wb"));
	if (NULL == pSt_File)
	{
		printf("create key file is failed\n");
		return false;
	}
	fwrite(tszENCodecBuffer, 1, nRet, pSt_File);
	fclose(pSt_File);

	return true;
}
//2.打开CDKEY并且授权(授权只能通过自我授权或者由管理员授权,用户不应该可以自己随意授权)
bool Authorize_APPLocal_Auth(LPCXSTR lpszKeyFile, LPCXSTR lpszPasswd, LPCXSTR lpszSerialStr)
{
	XCHAR tszENCodecBuffer[4096] = {};
	XCHAR tszDECodecBuffer[4096] = {};
	XENGINE_AUTHORIZE_LOCAL st_AuthLocal = {};

	FILE* pSt_File = _xtfopen(lpszKeyFile, _X("rb"));
	if (NULL == pSt_File)
	{
		printf("open key file is failed\n");
		return false;
	}
	int nRet = fread(tszENCodecBuffer, 1, sizeof(tszENCodecBuffer), pSt_File);
	fclose(pSt_File);

	Cryption_XCrypto_Decoder(tszENCodecBuffer, &nRet, tszDECodecBuffer, lpszPasswd);
	//printf("大小:%d,内容:\n%s\n", nRet, tszDECodecBuffer);

	Authorize_CDKey_ReadMemory(tszDECodecBuffer, nRet, &st_AuthLocal);
	if (!Authorize_CDKey_UserRegister(&st_AuthLocal, lpszSerialStr))
	{
		printf("serila verifacation is failed,error code:%lX\n", Authorize_GetLastError());
		return false;
	}

	memset(tszENCodecBuffer, '\0', sizeof(tszENCodecBuffer));
	memset(tszDECodecBuffer, '\0', sizeof(tszDECodecBuffer));
	Authorize_CDKey_WriteMemory(tszDECodecBuffer, &nRet, &st_AuthLocal);
	Cryption_XCrypto_Encoder(tszDECodecBuffer, &nRet, (XBYTE*)tszENCodecBuffer, lpszPasswd);
	pSt_File = _xtfopen(lpszKeyFile, _X("wb"));
	fwrite(tszENCodecBuffer, 1, nRet, pSt_File);
	fclose(pSt_File);
	return true;
}
//3.验证CDkey
bool Authorize_APPLocal_Auth(LPCXSTR lpszKeyFile, LPCXSTR lpszPasswd)
{
	XCHAR tszENCodecBuffer[4096] = {};
	XCHAR tszDECodecBuffer[4096] = {};
	XENGINE_AUTHORIZE_LOCAL st_AuthLocal = {};

	FILE* pSt_File = _xtfopen(lpszKeyFile, _X("rb"));
	if (NULL == pSt_File)
	{
		printf("open key file is failed\n");
		return false;
	}
	int nRet = fread(tszENCodecBuffer, 1, sizeof(tszENCodecBuffer), pSt_File);
	fclose(pSt_File);

	Cryption_XCrypto_Decoder(tszENCodecBuffer, &nRet, tszDECodecBuffer, lpszPasswd);
	//printf("大小:%d,内容:\n%s\n", nRet, tszDECodecBuffer);
	Authorize_CDKey_ReadMemory(tszDECodecBuffer, nRet, &st_AuthLocal);
	if (!Authorize_CDKey_GetLeftTimer(&st_AuthLocal))
	{
		//失败也需要重写CDKEY
		memset(tszENCodecBuffer, '\0', sizeof(tszENCodecBuffer));
		memset(tszDECodecBuffer, '\0', sizeof(tszDECodecBuffer));
		Authorize_CDKey_WriteMemory(tszDECodecBuffer, &nRet, &st_AuthLocal);
		Cryption_XCrypto_Encoder(tszDECodecBuffer, &nRet, (XBYTE*)tszENCodecBuffer, lpszPasswd);
		pSt_File = _xtfopen(lpszKeyFile, _X("wb"));
		fwrite(tszENCodecBuffer, 1, nRet, pSt_File);
		fclose(pSt_File);

		printf("verifaction is failed\n");
		return false;
	}
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial = {};
	SystemApi_HardWare_GetSerial(&st_SDKSerial);

	if (ENUM_AUTHORIZE_MODULE_VERMODE_TYPE_LOCAL != st_AuthLocal.st_AuthRegInfo.enVModeType)
	{
		printf("cdkey does not support local verification\n");
		return false;
	}
	if (ENUM_AUTHORIZE_MODULE_HW_TYPE_BIOS != st_AuthLocal.st_AuthRegInfo.enHWType)
	{
		printf("cdkey serial type is incorrect\n");
		return false;
	}
	if (0 != _tcsxnicmp(st_SDKSerial.tszBoardSerial, st_AuthLocal.st_AuthRegInfo.tszHardware, _tcsxlen(st_SDKSerial.tszBoardSerial)))
	{
		printf("cdkey serial verification is failed\n");
		return false;
	}

	memset(tszENCodecBuffer, '\0', sizeof(tszENCodecBuffer));
	memset(tszDECodecBuffer, '\0', sizeof(tszDECodecBuffer));
	Authorize_CDKey_WriteMemory(tszDECodecBuffer, &nRet, &st_AuthLocal);
	Cryption_XCrypto_Encoder(tszDECodecBuffer, &nRet, (XBYTE*)tszENCodecBuffer, lpszPasswd);
	pSt_File = _xtfopen(lpszKeyFile, _X("wb"));
	fwrite(tszENCodecBuffer, 1, nRet, pSt_File);
	fclose(pSt_File);
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

	if (!Authorize_APPLocal_Auth(lpszFileStr, lpszPasswd, tszSerialStr))
	{
		return -1;
	}

	for (int i = 0; i < 10; i++)
	{
		if (!Authorize_APPLocal_Auth(lpszFileStr, lpszPasswd))
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