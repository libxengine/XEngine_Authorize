#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"x86/XEngine_Core/XEngine_OPenSsl")
#pragma comment(lib,"x86/XEngine_SystemSdk/XEngine_SystemApi")
#pragma comment(lib,"x86/XEngine_HelpComponents/HelpComponents_Authorize")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Error.h>
#include "../../XEngine_Source/XAuth_Protocol.h"

#ifdef _WINDOWS
LPCTSTR lpszFile = _T("D:\\XEngine_Authorize\\XEngine_Apps\\Debug\\XEngine_Authorize.key");
#else
LPCTSTR lpszFile = _T("XEngine_Authorize.key");
#endif

#define _ENCRYPTO

int main()
{
	//////////////////////////////////////////////////////////////////////////生成CDKEY
	__int64x nLeftTime = 0;
	UCHAR tszEnBuffer[2048];
	TCHAR tszDeBuffer[2048];
	XENGINE_AUTHORIZE_LOCAL st_AuthLocal;
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial;

	memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));
	memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
	memset(&st_AuthLocal, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));
	memset(&st_SDKSerial, '\0', sizeof(SYSTEMAPI_SERIAL_INFOMATION));

	strcpy(st_AuthLocal.tszAddr, _T("127.0.0.1"));
	st_AuthLocal.nPort = 5000;

	strcpy(st_AuthLocal.st_AuthAppInfo.tszAppName, "XEngine");
	strcpy(st_AuthLocal.st_AuthAppInfo.tszAppVer, "7.29.0.1001");

	st_AuthLocal.st_AuthRegInfo.enSerialType = ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME;
	st_AuthLocal.st_AuthRegInfo.enRegType = ENUM_HELPCOMPONENTS_AUTHORIZE_REG_TYPE_TRY;
	st_AuthLocal.st_AuthRegInfo.enHWType = ENUM_HELPCOMPONENTS_AUTHORIZE_HW_TYPE_CPU;
	SystemApi_HardWare_GetSerial(&st_SDKSerial);
	strcpy(st_AuthLocal.st_AuthRegInfo.tszHardware, st_SDKSerial.tszCpuSerial);

	strcpy(st_AuthLocal.st_AuthUserInfo.tszUserName, "XEngine");
	strcpy(st_AuthLocal.st_AuthUserInfo.tszUserContact, "486179@qq.com");
	//生成完毕后可以交给服务端来对key本地验证进行授权
	if (!Authorize_Local_WriteKey(lpszFile, &st_AuthLocal))
	{
		return -1;
	}
	//需要加密?
#ifdef _ENCRYPTO
	FILE* pSt_File = fopen(lpszFile, "rb");
	int nRet = fread(tszDeBuffer, 1, sizeof(tszDeBuffer), pSt_File);
	fclose(pSt_File);
	if (!OPenSsl_Api_CryptEncodec(tszDeBuffer, tszEnBuffer, &nRet, "123123", XENGINE_OPENSSL_API_CRYPT_3DES))
	{
		return -1;
	}
	pSt_File = fopen(lpszFile, "wb");
	fwrite(tszEnBuffer, 1, nRet, pSt_File);
	fclose(pSt_File);
#endif

	return 0;
}