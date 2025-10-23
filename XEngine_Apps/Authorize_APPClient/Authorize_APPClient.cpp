#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_Cryption")
#pragma comment(lib,"XEngine_Client/XClient_Socket")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp")
#ifdef _WIN64
#pragma comment(lib,"../../XEngine_Source/x64/Debug/jsoncpp")
#else
#pragma comment(lib,"../../XEngine_Source/Debug/jsoncpp")
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <json/json.h>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/Cryption_Define.h>
#include <XEngine_Include/XEngine_Core/Cryption_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
#include "../../XEngine_Source/XEngine_DependLibrary/XEngine_Module/XEngine_Verification/Verification_Define.h"
#include "../../XEngine_Source/XAuth_Protocol.h"

//Windows::vs2022 x86 debug 编译即可
//Linux::g++ -std=c++17 -Wall -g Authorize_APPClient.cpp -o Authorize_APPClient.exe -I ../../XEngine_Source/XEngine_DependLibrary/XEngine_Module/jsoncpp -L ../../XEngine_Release -lXEngine_Cryption -lXClient_Socket  -lXEngine_BaseLib -lXClient_APIHelp -lpthread -ljsoncpp -Wl,-rpath=../../XEngine_Release

//#define _DYNAMIC_CODE
#define _PASS_ENCRYPT
bool bRun = true;
bool bLogin = true;
bool bTimeOut = true;
bool bEncrypto = false;
bool bHeart = true;
XNETHANDLE xhToken = 0;
int nDYCode = 0;

XSOCKET m_Socket = 0;
LPCXSTR lpszUser = _X("123123aa");
LPCXSTR lpszPass = _X("123123");
LPCXSTR lpszHWCode = _X("2FDWAD02JD2091");
LPCXSTR lpszSerial = _X("Y5V0Y-03O2L-E7G8Y-NX1DR-3EGVO-UDIRQ-GBBKO-FGX0C");
LPCXSTR lpszEmail = _X("486179@qq.com");
__int64x nPhoneNumber = 1366666666;
__int64x nIDNumber = 511025111111111111;

XHTHREAD AuthClient_Thread()
{
	time_t nTimeStart = time(NULL);
	while (bRun)
	{
		int nMsgLen = 0;
		XCHAR *ptszMsgBuffer;
		XENGINE_PROTOCOLHDR st_ProtocolHdr;

		memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
		if (XClient_TCPSelect_RecvPkt(m_Socket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
		{
			XCHAR tszMsgBuffer[4096];
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

			if (nMsgLen > 0 && bEncrypto)
			{
				//只有有后续数据的情况才需要解密
				Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, lpszPass);
			}
			else
			{
				memcpy(tszMsgBuffer, ptszMsgBuffer, nMsgLen);
			}
			
			if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN == st_ProtocolHdr.unOperatorCode)
			{
				bLogin = false;
				if (0 == st_ProtocolHdr.wReserve)
				{
					printf(_X("登录成功\n"));
					xhToken = st_ProtocolHdr.xhToken;
				}
				else
				{
					printf(_X("登录失败,错误:%d\n"), st_ProtocolHdr.wReserve);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_EXPIRED == st_ProtocolHdr.unOperatorCode)
			{
				bTimeOut = false;
				printf(_X("用户过期\n"));
			}
			else
			{
				printf(_X("不明白的类型\n"));
			}
		}
		if (bHeart)
		{
			time_t nTimeEnd = time(NULL);
			if ((nTimeEnd - nTimeStart) > 2)
			{
				XENGINE_PROTOCOLHDR st_ProtocolHdr = {};
				st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
				st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_HEARTBEAT;
				st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_HB_SYN;
				st_ProtocolHdr.byVersion = 1;
				st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

				nTimeStart = nTimeEnd;
				XClient_TCPSelect_SendMsg(m_Socket, (LPCXSTR)&st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return 0;
}

int AuthClient_Register()
{
	Json::Value st_JsonRoot;
	Json::Value st_JsonUserInfo;
	Json::Value st_JsonUserTable;
	LPCXSTR lpszUrl = _X("http://127.0.0.1:5302/auth/user/register");

#ifdef _PASS_ENCRYPT
	XCHAR tszPassCodec[128] = {};
	int nPLen = _tcsxlen(lpszPass);
	XBYTE byMD5Buffer[XPATH_MAX] = {};
	Cryption_Api_Digest(lpszPass, byMD5Buffer, &nPLen);
	BaseLib_String_StrToHex((LPCXSTR)byMD5Buffer, nPLen, tszPassCodec);
	st_JsonUserInfo["tszUserPass"] = tszPassCodec;
#else
	st_JsonUserInfo["tszUserPass"] = lpszPass;
#endif

	st_JsonUserInfo["tszUserName"] = lpszUser;
	st_JsonUserInfo["tszEMailAddr"] = lpszEmail;
	st_JsonUserInfo["nPhoneNumber"] = (Json::Value::Int64)nPhoneNumber;
	st_JsonUserInfo["nIDNumber"] = (Json::Value::Int64)nIDNumber;
	st_JsonUserInfo["nUserLevel"] = ENUM_XENGINE_PROTOCOLHDR_LEVEL_TYPE_USER;

	st_JsonUserTable["tszHardCode"] = lpszHWCode;
	st_JsonUserTable["st_UserInfo"] = st_JsonUserInfo;

	st_JsonRoot["st_UserTable"] = st_JsonUserTable;

	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;

	if (bEncrypto)
	{
		XCHAR tszENBuffer[2048] = {};
		XCHAR tszDEBuffer[2048] = {};

		nMsgLen = st_JsonRoot.toStyledString().length();
		Cryption_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (XBYTE*)tszENBuffer, lpszPass);
		APIClient_Http_Request(_X("POST"), lpszUrl, tszENBuffer, NULL, &ptszMsgBuffer, &nMsgLen);

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszDEBuffer, lpszPass);
		printf("AuthClient_Register:\n%s\n", tszDEBuffer);
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	else
	{
		APIClient_Http_Request(_X("POST"), lpszUrl, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);

		printf("AuthClient_Register:\n%s\n", ptszMsgBuffer);
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	return 0;
}
int AuthClient_Pay()
{
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	LPCXSTR lpszUrl = _X("http://127.0.0.1:5302/auth/user/pay");

	st_JsonObject["tszSerialNumber"] = lpszSerial;
	st_JsonObject["tszUserName"] = lpszUser;

	st_JsonRoot["st_UserPay"] = st_JsonObject;

	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;

	if (bEncrypto)
	{
		XCHAR tszENBuffer[2048] = {};
		XCHAR tszDEBuffer[2048] = {};

		nMsgLen = st_JsonRoot.toStyledString().length();
		Cryption_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (XBYTE*)tszENBuffer, lpszPass);
		APIClient_Http_Request(_X("POST"), lpszUrl, tszENBuffer, NULL, &ptszMsgBuffer, &nMsgLen);

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszDEBuffer, lpszPass);
		printf("AuthClient_Pay:\n%s\n", tszDEBuffer);
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	else
	{
		APIClient_Http_Request(_X("POST"), lpszUrl, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
		printf("AuthClient_Pay:\n%s\n", ptszMsgBuffer);
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	return 0;
}
int AuthClient_DynamicCode()
{
	LPCXSTR lpszUrl = _X("http://127.0.0.1:5302/api?function=dcode");

	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	APIClient_Http_Request(_X("GET"), lpszUrl, NULL, NULL, &ptszMsgBuffer, &nMsgLen);

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (bEncrypto)
	{
		XCHAR tszDEBuffer[2048] = {};

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszDEBuffer, lpszPass);
		if (!pSt_JsonReader->parse(tszDEBuffer, tszDEBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			return 0;
		}
		printf("AuthClient_DynamicCode:\n%s\n", tszDEBuffer);
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			return 0;
		}
		printf("AuthClient_DynamicCode:\n%s\n", ptszMsgBuffer);
	}
	XCHAR tszDCodeStr[64];
	XCHAR tszTokenStr[64];

	memset(tszDCodeStr, '\0', sizeof(tszDCodeStr));
	memset(tszTokenStr, '\0', sizeof(tszTokenStr));

	nDYCode = st_JsonRoot["nDynamicCode"].asUInt();
	xhToken = st_JsonRoot["xhToken"].asUInt64();
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	return 0;
}
int AuthClient_Login()
{
	XCHAR tszMsgBuffer[2048];
	XENGINE_PROTOCOLHDR st_ProtocolHdr;                    //协议头
	XENGINE_PROTOCOL_USERAUTHEX st_AuthUser;
	
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	memset(&st_AuthUser, '\0', sizeof(XENGINE_PROTOCOL_USERAUTHEX));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN;
	st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOL_USERAUTHEX);
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	st_AuthUser.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC_WINDOWS;
	strcpy(st_AuthUser.tszUserName, lpszUser);
	strcpy(st_AuthUser.tszUserPass, lpszPass);
	strcpy(st_AuthUser.tszHWCode, lpszHWCode);

#ifdef _PASS_ENCRYPT
	int nPLen = _tcsxlen(st_AuthUser.tszUserPass);
	XBYTE byMD5Buffer[XPATH_MAX] = {};
	Cryption_Api_Digest(st_AuthUser.tszUserPass, byMD5Buffer, &nPLen);
	memset(st_AuthUser.tszUserPass, '\0', sizeof(st_AuthUser.tszUserPass));
	BaseLib_String_StrToHex((LPCXSTR)byMD5Buffer, nPLen, st_AuthUser.tszUserPass);
#endif

	if (nDYCode > 0)
	{
		st_ProtocolHdr.xhToken = xhToken;
		_xstprintf(st_AuthUser.tszDCode, _X("%d"), nDYCode);
	}

	int nMsgLen = 0;
	if (bEncrypto)
	{
		XCHAR tszCodecBuffer[2048];
		memset(tszCodecBuffer, '\0', sizeof(tszCodecBuffer));

		st_ProtocolHdr.wCrypto = ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_XCRYPT;
		Cryption_XCrypto_Encoder((LPCXSTR)&st_AuthUser, (int*)&st_ProtocolHdr.unPacketSize, (XBYTE*)tszCodecBuffer, lpszPass);

		memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), tszCodecBuffer, st_ProtocolHdr.unPacketSize);

		nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	}
	else
	{
		memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_AuthUser, st_ProtocolHdr.unPacketSize);

		nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOL_USERAUTHEX);
	}

	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_X("登录协议发送失败！\n"));
		return -1;
	}
	while (bLogin)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int AuthClient_Notice()
{
	XCHAR tszURLStr[XPATH_MAX] = {};
	_xstprintf(tszURLStr, _X("http://127.0.0.1:5302/api?function=notice&token=%lld"), xhToken);

	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());

	APIClient_Http_Request(_X("GET"), tszURLStr, NULL, NULL, &ptszMsgBuffer, &nMsgLen);
	if (bEncrypto)
	{
		XCHAR tszDEBuffer[2048] = {};

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszDEBuffer, lpszPass);
		if (!pSt_JsonReader->parse(tszDEBuffer, tszDEBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			return 0;
		}
	}
	else
	{
		if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
		{
			return 0;
		}
	}
	
	for (unsigned int i = 0; i < st_JsonRoot["Array"].size(); i++)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		Json::Value st_JsonArray = st_JsonRoot["Array"][i];

		int nMsgLen = st_JsonArray["tszContext"].asString().length();
		BaseLib_Charset_UTFToAnsi(st_JsonArray["tszContext"].asCString(), tszMsgBuffer, &nMsgLen);

#ifdef _MSC_BUILD
		printf("AuthClient_Notice:\nID:%lld:%s\n", st_JsonArray["nID"].asInt64(), tszMsgBuffer);
#else
		printf("AuthClient_Notice:\nID:%ld:%s\n", st_JsonArray["nID"].asInt64(), tszMsgBuffer);
#endif
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	return 0;
}
int AuthClient_GetPass()
{
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	LPCXSTR lpszUrl = _X("http://127.0.0.1:5302/auth/user/pass");

#ifdef _PASS_ENCRYPT
	XCHAR tszPASSCodec[128] = {};
	int nPLen = _tcsxlen(lpszPass);
	XBYTE byMD5Buffer[XPATH_MAX] = {};
	Cryption_Api_Digest(lpszPass, byMD5Buffer, &nPLen);
	BaseLib_String_StrToHex((LPCXSTR)byMD5Buffer, nPLen, tszPASSCodec);
	st_JsonObject["tszUserPass"] = tszPASSCodec;
#else
	st_JsonObject["tszUserPass"] = lpszPass;
#endif

	st_JsonObject["tszUserName"] = lpszUser;
	st_JsonObject["tszEMailAddr"] = lpszEmail;
	st_JsonObject["nPhoneNumber"] = (Json::Value::Int64)nPhoneNumber;
	st_JsonObject["nIDNumber"] = (Json::Value::Int64)nIDNumber;

	st_JsonRoot["st_UserInfo"] = st_JsonObject;

	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;

	if (bEncrypto)
	{
		XCHAR tszENBuffer[2048] = {};
		XCHAR tszDEBuffer[2048] = {};

		nMsgLen = st_JsonRoot.toStyledString().length();
		Cryption_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (XBYTE*)tszENBuffer, lpszPass);
		APIClient_Http_Request(_X("POST"), lpszUrl, tszENBuffer, NULL, &ptszMsgBuffer, &nMsgLen);

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszDEBuffer, lpszPass);
		printf("AuthClient_GetPass:\n%s\n", tszDEBuffer);
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	else
	{
		APIClient_Http_Request(_X("POST"), lpszUrl, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
		printf("AuthClient_GetPass:\n%s\n", ptszMsgBuffer);
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}

	return 0;
}
int AuthClient_GetTime()
{
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	XCHAR tszURLStr[XPATH_MAX] = {};
	_xstprintf(tszURLStr, _X("http://127.0.0.1:5302/api?function=time&token=%lld"),xhToken);

	st_JsonObject["tszUserName"] = lpszUser;

#ifdef _PASS_ENCRYPT
	XCHAR tszPASSCodec[128] = {};
	int nPLen = _tcsxlen(lpszPass);
	XBYTE byMD5Buffer[XPATH_MAX] = {};
	Cryption_Api_Digest(lpszPass, byMD5Buffer, &nPLen);
	BaseLib_String_StrToHex((LPCXSTR)byMD5Buffer, nPLen, tszPASSCodec);
	st_JsonObject["tszUserPass"] = tszPASSCodec;
#else
	st_JsonObject["tszUserPass"] = lpszPass;
#endif
	
	st_JsonRoot["st_UserAuth"] = st_JsonObject;

	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;

	if (bEncrypto)
	{
		XCHAR tszENBuffer[2048] = {};
		XCHAR tszDEBuffer[2048] = {};

		nMsgLen = st_JsonRoot.toStyledString().length();
		Cryption_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (XBYTE*)tszENBuffer, lpszPass);
		APIClient_Http_Request(_X("GET"), tszURLStr, tszENBuffer, NULL, &ptszMsgBuffer, &nMsgLen);

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszDEBuffer, lpszPass);
		printf("AuthClient_GetTime:\n%s\n", tszDEBuffer);
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	else
	{
		APIClient_Http_Request(_X("GET"), tszURLStr, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
		printf("AuthClient_GetTime:\n%s\n", ptszMsgBuffer);
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}

	return 0;
}
int AuthClient_Delete()
{
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	LPCXSTR lpszUrl = _X("http://127.0.0.1:5302/auth/user/delete");

#ifdef _PASS_ENCRYPT
	XCHAR tszPASSCodec[128] = {};
	int nPLen = _tcsxlen(lpszPass);
	XBYTE byMD5Buffer[XPATH_MAX] = {};
	Cryption_Api_Digest(lpszPass, byMD5Buffer, &nPLen);
	BaseLib_String_StrToHex((LPCXSTR)byMD5Buffer, nPLen, tszPASSCodec);
	st_JsonObject["tszUserPass"] = tszPASSCodec;
#else
	st_JsonObject["tszUserPass"] = lpszPass;
#endif

	st_JsonObject["tszUserName"] = lpszUser;
	st_JsonObject["tszEMailAddr"] = lpszEmail;
	st_JsonObject["nPhoneNumber"] = (Json::Value::Int64)nPhoneNumber;
	st_JsonObject["nIDNumber"] = (Json::Value::Int64)nIDNumber;

	st_JsonRoot["st_UserInfo"] = st_JsonObject;

	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;

	if (bEncrypto)
	{
		XCHAR tszENBuffer[2048] = {};
		XCHAR tszDEBuffer[2048] = {};

		nMsgLen = st_JsonRoot.toStyledString().length();
		Cryption_XCrypto_Encoder(st_JsonRoot.toStyledString().c_str(), &nMsgLen, (XBYTE*)tszENBuffer, lpszPass);
		APIClient_Http_Request(_X("POST"), lpszUrl, tszENBuffer, NULL, &ptszMsgBuffer, &nMsgLen);

		Cryption_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszDEBuffer, lpszPass);
		printf("AuthClient_Delete:\n%s\n", tszDEBuffer);
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	else
	{
		APIClient_Http_Request(_X("POST"), lpszUrl, st_JsonRoot.toStyledString().c_str(), NULL, &ptszMsgBuffer, &nMsgLen);
		printf("AuthClient_Delete:\n%s\n", ptszMsgBuffer);
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	return 0;
}
int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	int nPort = 5300;
	LPCXSTR lpszAddr = _X("127.0.0.1");

	if (!XClient_TCPSelect_Create(&m_Socket, lpszAddr, nPort))
	{
		printf("XClient_TCPSelect_Create:%lX", XClient_GetLastError());
		return -1;
	}
	std::thread pSTDThread(AuthClient_Thread);

	AuthClient_Register();
	AuthClient_Pay();
#ifdef _DYNAMIC_CODE
	AuthClient_DynamicCode();
#endif
	AuthClient_Login();
	AuthClient_Notice();
	AuthClient_GetPass();
	AuthClient_GetTime();

	std::this_thread::sleep_for(std::chrono::seconds(10));
	AuthClient_Delete();

	bRun = false;
	XClient_TCPSelect_Close(m_Socket);
	pSTDThread.join();

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}