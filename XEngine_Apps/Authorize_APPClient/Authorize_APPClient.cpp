#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl")
#pragma comment(lib,"XEngine_Client/XClient_Socket")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIHelp")
#pragma comment(lib,"../../XEngine_Source/Debug/jsoncpp")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
#include <json/json.h>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Define.h>
#include "../../XEngine_Source/XAuth_Protocol.h"

//Windows::vs2022 x86 debug 编译即可
//Linux::g++ -std=c++17 -Wall -g Auth_APPClient.cpp -o Auth_APPClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lXEngine_OPenSsl -lXClient_Socket -lNetHelp_APIHelp -lpthread -ljsoncpp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g Auth_APPClient.cpp -o Auth_APPClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lXEngine_OPenSsl -lXClient_Socket -lNetHelp_APIHelp -lpthread -ljsoncpp

BOOL bRun = TRUE;
BOOL bLogin = TRUE;
BOOL bTimeOut = TRUE;
BOOL bEncrypto = FALSE;

SOCKET m_Socket = 0;
LPCTSTR lpszUser = _T("123123aa");
LPCTSTR lpszPass = _T("123123");
LPCTSTR lpszSerial = _T("XAUTH-XYRYM-ZAA10-59NKW-KOFLP-35838-ZXC36-ZOVTH");
LPCTSTR lpszEmail = _T("486179@qq.com");
__int64x nPhoneNumber = 1366666666;
__int64x nIDNumber = 511025111111111111;

XHTHREAD AuthClient_Thread()
{
	while (bRun)
	{
		int nMsgLen = 0;
		TCHAR *ptszMsgBuffer;
		XENGINE_PROTOCOLHDR st_ProtocolHdr;

		memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
		if (XClient_TCPSelect_RecvPkt(m_Socket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
		{
			TCHAR tszMsgBuffer[4096];
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

			if (nMsgLen > 0 && bEncrypto)
			{
				//只有有后续数据的情况才需要解密
				OPenSsl_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, lpszPass);
			}
			else
			{
				memcpy(tszMsgBuffer, ptszMsgBuffer, nMsgLen);
			}
			
			if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN == st_ProtocolHdr.unOperatorCode)
			{
				bLogin = FALSE;
				if (0 == st_ProtocolHdr.wReserve)
				{
					printf(_T("登录成功\n"));
				}
				else
				{
					printf(_T("登录失败,错误:%d\n"), st_ProtocolHdr.wReserve);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_TIMEDOUT == st_ProtocolHdr.unOperatorCode)
			{
				bTimeOut = FALSE;
				printf(_T("用户过期\n"));
			}
			else
			{
				printf(_T("不明白的类型\n"));
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return 0;
}

int AuthClient_Register()
{
	int nHTTPCode = 0;
	Json::Value st_JsonRoot;
	Json::Value st_JsonUserInfo;
	Json::Value st_JsonUserTable;
	LPCTSTR lpszUrl = _T("http://127.0.0.1:5302/auth/user/register");

	st_JsonUserInfo["tszUserName"] = lpszUser;
	st_JsonUserInfo["tszUserPass"] = lpszPass;
	st_JsonUserInfo["tszEMailAddr"] = lpszEmail;
	st_JsonUserInfo["nPhoneNumber"] = nPhoneNumber;
	st_JsonUserInfo["nIDNumber"] = nIDNumber;
	st_JsonUserInfo["nUserLevel"] = 5;

	st_JsonUserTable["tszHardCode"] = "2FDWAD02JD2091";
	st_JsonUserTable["st_UserInfo"] = st_JsonUserInfo;

	st_JsonRoot["st_UserTable"] = st_JsonUserTable;

	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	APIHelp_HttpRequest_Post(lpszUrl, st_JsonRoot.toStyledString().c_str(), &nHTTPCode, &ptszMsgBuffer, &nMsgLen);
	printf("AuthClient_Register:\n%s\n", ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	return 0;
}
int AuthClient_Pay()
{
	int nHTTPCode = 0;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	LPCTSTR lpszUrl = _T("http://127.0.0.1:5302/auth/user/pay");

	st_JsonObject["tszSerialNumber"] = lpszSerial;
	st_JsonObject["tszUserName"] = lpszUser;

	st_JsonRoot["st_UserPay"] = st_JsonObject;

	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	APIHelp_HttpRequest_Post(lpszUrl, st_JsonRoot.toStyledString().c_str(), &nHTTPCode, &ptszMsgBuffer, &nMsgLen);
	printf("AuthClient_Pay:\n%s\n", ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	return 0;
}
int AuthClient_Login()
{
	TCHAR tszMsgBuffer[2048];
	XENGINE_PROTOCOLHDR st_ProtocolHdr;                    //协议头
	XENGINE_PROTOCOL_USERAUTH st_AuthUser;
	
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	memset(&st_AuthUser, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN;
	st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOL_USERAUTH);
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	st_AuthUser.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC;
	strcpy(st_AuthUser.tszUserName, lpszUser);
	strcpy(st_AuthUser.tszUserPass, lpszPass);

	int nMsgLen = 0;
	if (bEncrypto)
	{
		TCHAR tszCodecBuffer[2048];
		memset(tszCodecBuffer, '\0', sizeof(tszCodecBuffer));

		st_ProtocolHdr.wCrypto = ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_XCRYPT;
		OPenSsl_XCrypto_Encoder((LPCTSTR)&st_AuthUser, (int*)&st_ProtocolHdr.unPacketSize, (UCHAR*)tszCodecBuffer, lpszPass);

		memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), tszCodecBuffer, st_ProtocolHdr.unPacketSize);

		nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	}
	else
	{
		memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_AuthUser, st_ProtocolHdr.unPacketSize);

		nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOL_USERAUTH);
	}

	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("登录协议发送失败！\n"));
		return -1;
	}
	while (bLogin)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int AuthClient_GetPass()
{
	int nHTTPCode = 0;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	LPCTSTR lpszUrl = _T("http://127.0.0.1:5302/auth/user/pass");

	st_JsonObject["tszUserName"] = lpszUser;
	st_JsonObject["tszEMailAddr"] = lpszEmail;
	st_JsonObject["nPhoneNumber"] = nPhoneNumber;
	st_JsonObject["nIDNumber"] = nIDNumber;

	st_JsonRoot["st_UserInfo"] = st_JsonObject;

	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	APIHelp_HttpRequest_Post(lpszUrl, st_JsonRoot.toStyledString().c_str(), &nHTTPCode, &ptszMsgBuffer, &nMsgLen);
	printf("AuthClient_GetPass:\n%s\n", ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	return 0;
}
int AuthClient_GetTime()
{
	int nHTTPCode = 0;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	LPCTSTR lpszUrl = _T("http://127.0.0.1:5302/auth/user/time");

	st_JsonObject["tszUserName"] = lpszUser;
	st_JsonObject["tszUserPass"] = lpszPass;
	st_JsonRoot["st_UserAuth"] = st_JsonObject;

	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	APIHelp_HttpRequest_Post(lpszUrl, st_JsonRoot.toStyledString().c_str(), &nHTTPCode, &ptszMsgBuffer, &nMsgLen);
	printf("AuthClient_GetTime:\n%s\n", ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	return 0;
}
int AuthClient_Delete()
{
	int nHTTPCode = 0;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	LPCTSTR lpszUrl = _T("http://127.0.0.1:5302/auth/user/delete");

	st_JsonObject["tszUserName"] = lpszUser;
	st_JsonObject["tszUserPass"] = lpszPass;
	st_JsonObject["tszEMailAddr"] = lpszEmail;
	st_JsonObject["nPhoneNumber"] = nPhoneNumber;
	st_JsonObject["nIDNumber"] = nIDNumber;

	st_JsonRoot["st_UserInfo"] = st_JsonObject;

	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	APIHelp_HttpRequest_Post(lpszUrl, st_JsonRoot.toStyledString().c_str(), &nHTTPCode, &ptszMsgBuffer, &nMsgLen);
	printf("AuthClient_Delete:\n%s\n", ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	return 0;
}
int AuthClient_Try()
{
	int nHTTPCode = 0;
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	LPCTSTR lpszSerialNet = _T("cpuid:112233"); //通过此可以做临时网络验证,安全性比本地临时验证高
	LPCTSTR lpszUrl = _T("http://127.0.0.1:5302/auth/user/try");

	st_JsonObject["tszSerial"] = lpszSerialNet;
	st_JsonRoot["st_UserTry"] = st_JsonObject;

	int nMsgLen = 0;
	CHAR* ptszMsgBuffer = NULL;
	APIHelp_HttpRequest_Post(lpszUrl, st_JsonRoot.toStyledString().c_str(), &nHTTPCode, &ptszMsgBuffer, &nMsgLen);
	printf("AuthClient_Try:\n%s\n", ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	return 0;
}
int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	int nPort = 5300;
	LPCTSTR lpszAddr = _T("127.0.0.1");

	if (!XClient_TCPSelect_Create(&m_Socket, lpszAddr, nPort))
	{
		printf("XClient_TCPSelect_Create:%lX", XClient_GetLastError());
		return -1;
	}
	std::thread pSTDThread(AuthClient_Thread);

	AuthClient_Register();
	AuthClient_Pay();
	AuthClient_Login();
	AuthClient_GetPass();
	AuthClient_GetTime();
	AuthClient_Delete();
	AuthClient_Try();

	bRun = FALSE;
	XClient_TCPSelect_Close(m_Socket);
	pSTDThread.join();

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}