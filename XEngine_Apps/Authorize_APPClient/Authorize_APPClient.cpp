#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"x86//XEngine_Core/XEngine_OPenSsl.lib")
#pragma comment(lib,"x86//XEngine_Client/XClient_Socket.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Define.h>
#include "../../XEngine_Source/XAuth_Protocol.h"
//g++ -std=c++17 -Wall -g Auth_APPClient.cpp -o Auth_APPClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AuthorizeReg -lXEngine_BaseLib -lXClient_Socket -lXEngine_AuthRegClient -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AuthorizeReg,--disable-new-dtags

BOOL bRun = TRUE;
BOOL bRegiser = TRUE;
BOOL bPay = TRUE;
BOOL bLogin = TRUE;
BOOL bPass = TRUE;
BOOL bLeftTime = TRUE;
BOOL bReadTxt = TRUE;
BOOL bDelete = TRUE;
BOOL bTry = TRUE;
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

			if (nMsgLen > 0)
			{
				//只有有后续数据的情况才需要解密
				OPenSsl_XCrypto_Decoder(ptszMsgBuffer, &nMsgLen, tszMsgBuffer, lpszPass);
			}
			if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPDEL == st_ProtocolHdr.unOperatorCode)
			{
				bDelete = FALSE;
				if (0 == st_ProtocolHdr.wReserve)
				{
					printf(_T("删除用户成功\n"));
				}
				else
				{
					printf(_T("删除用户失败,错误:%d\n"), st_ProtocolHdr.wReserve);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPREGISTER == st_ProtocolHdr.unOperatorCode)
			{
				bRegiser = FALSE;
				if (0 == st_ProtocolHdr.wReserve)
				{
					printf(_T("注册成功\n"));
				}
				else
				{
					printf(_T("注册失败,错误:%d\n"), st_ProtocolHdr.wReserve);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN == st_ProtocolHdr.unOperatorCode)
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
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPPAY == st_ProtocolHdr.unOperatorCode)
			{
				bPay = FALSE;
				if (0 == st_ProtocolHdr.wReserve)
				{
					printf(_T("充值成功\n"));
				}
				else
				{
					printf(_T("充值失败,错误:%d\n"), st_ProtocolHdr.wReserve);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETPASS == st_ProtocolHdr.unOperatorCode)
			{
				bPass = FALSE;
				if (0 == st_ProtocolHdr.wReserve)
				{
					if (0 == st_ProtocolHdr.unPacketSize)
					{
						printf(_T("找回密码成功,账号密码已发送到你的邮箱\n"));
					}
					else
					{
						XENGINE_PROTOCOL_USERAUTH st_AuthProtocol;
						memset(&st_AuthProtocol, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));
						memcpy(&st_AuthProtocol, tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), sizeof(XENGINE_PROTOCOL_USERAUTH));

						printf(_T("找回密码成功,账号:%s,密码:%s\n"), st_AuthProtocol.tszUserName, st_AuthProtocol.tszUserPass);
					}
				}
				else
				{
					printf(_T("找回密码失败,错误:%d\n"), st_ProtocolHdr.wReserve);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETTIME == st_ProtocolHdr.unOperatorCode)
			{
				bLeftTime = FALSE;
				if (0 == st_ProtocolHdr.wReserve)
				{
					AUTHREG_PROTOCOL_TIME st_AuthTime;
					memset(&st_AuthTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
					memcpy(&st_AuthTime, tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), sizeof(AUTHREG_PROTOCOL_TIME));

					printf(_T("获取剩余时间成功,账号:%s,地址:%s,剩余时间:%lld 分钟,在线时间:%lld,过期日期:%s,类型:%d\n"), st_AuthTime.tszUserName, st_AuthTime.tszUserAddr, st_AuthTime.nTimeLeft, st_AuthTime.nTimeONLine, st_AuthTime.tszLeftTime, st_AuthTime.enSerialType);
				}
				else
				{
					printf(_T("获取时间失败,错误:%d\n"), st_ProtocolHdr.wReserve);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_NOTIFYMSG == st_ProtocolHdr.unOperatorCode)
			{
				bReadTxt = FALSE;
				if (0 == st_ProtocolHdr.wReserve)
				{
					TCHAR tszMsgInfo[2048];
					memset(tszMsgInfo, '\0', sizeof(tszMsgInfo));
					memcpy(tszMsgInfo, tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_ProtocolHdr.unPacketSize);

					printf(_T("读取到公告:%s\n"), tszMsgInfo);
				}
				else
				{
					printf(_T("获取数据失败,错误:%d\n"), st_ProtocolHdr.wReserve);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPTRYVER == st_ProtocolHdr.unOperatorCode)
			{
				bTry = FALSE;
				if (0 == st_ProtocolHdr.wReserve)
				{
					printf(_T("临时验证成功\n"));
				}
				else
				{
					printf(_T("临时验证失败,错误:%d\n"), st_ProtocolHdr.wReserve);
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

BOOL AuthRegClient_Protocol_Send(LPCTSTR lpszMsgBuffer, TCHAR* ptszMsgBuffer, int* pInt_MsgLen, UINT en_AuthProtocol)
{
	//填充协议
	TCHAR tszMsgBuffer[4096];
	XENGINE_PROTOCOLHDR st_ProtocolHdr;                    //协议头

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
	st_ProtocolHdr.unOperatorCode = en_AuthProtocol;
	st_ProtocolHdr.unPacketSize = *pInt_MsgLen;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	if (bEncrypto)
	{
		st_ProtocolHdr.wCrypto = ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_XCRYPT;
		if (*pInt_MsgLen > 0)
		{
			OPenSsl_XCrypto_Encoder(lpszMsgBuffer, (int*)&st_ProtocolHdr.unPacketSize, (UCHAR*)tszMsgBuffer, lpszPass);
			memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
			memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), tszMsgBuffer, st_ProtocolHdr.unPacketSize);
		}
		else
		{
			memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		}
		*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	}
	else
	{
		memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		if (*pInt_MsgLen > 0)
		{
			memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), lpszMsgBuffer, st_ProtocolHdr.unPacketSize);
		}
		*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	}
	return TRUE;
}
int AuthClient_Register()
{
	AUTHREG_USERTABLE st_UserTable;
	TCHAR tszMsgBuffer[2048];

	memset(&st_UserTable, '\0', sizeof(st_UserTable));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	strcpy(st_UserTable.st_UserInfo.tszUserName, lpszUser);
	strcpy(st_UserTable.st_UserInfo.tszUserPass, lpszPass);
	strcpy(st_UserTable.st_UserInfo.tszEMailAddr, lpszEmail);
	st_UserTable.st_UserInfo.nPhoneNumber = nPhoneNumber;
	st_UserTable.st_UserInfo.nIDNumber = nIDNumber;
	strcpy(st_UserTable.tszHardCode, "2FDWAD02JD2091");

	int nMsgLen = sizeof(AUTHREG_USERTABLE);
	AuthRegClient_Protocol_Send((LPCTSTR)&st_UserTable, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQREGISTER);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("注册协议发送失败！\n"));
		return -1;
	}
	while (bRegiser)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int AuthClient_Pay()
{
	AUTHREG_PROTOCOL_USERPAY st_SerialTable;
	TCHAR tszMsgBuffer[2048];

	memset(&st_SerialTable, '\0', sizeof(AUTHREG_PROTOCOL_USERPAY));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	strcpy(st_SerialTable.tszSerialNumber, lpszSerial);
	strcpy(st_SerialTable.tszUserName, lpszUser);

	int nMsgLen = sizeof(AUTHREG_PROTOCOL_USERPAY);
	AuthRegClient_Protocol_Send((LPCTSTR)&st_SerialTable, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQPAY);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("充值协议发送失败！\n"));
		return -1;
	}
	while (bPay)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int AuthClient_Login()
{
	XENGINE_PROTOCOL_USERAUTH st_AuthUser;
	TCHAR tszMsgBuffer[2048];

	memset(&st_AuthUser, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	strcpy(st_AuthUser.tszUserName, lpszUser);
	strcpy(st_AuthUser.tszUserPass, lpszPass);

	int nMsgLen = sizeof(XENGINE_PROTOCOL_USERAUTH);
	AuthRegClient_Protocol_Send((LPCTSTR)&st_AuthUser, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN);
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
	XENGINE_PROTOCOL_USERINFO st_UserTable;
	TCHAR tszMsgBuffer[2048];

	memset(&st_UserTable, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	strcpy(st_UserTable.tszUserName, lpszUser);
	strcpy(st_UserTable.tszEMailAddr, lpszEmail);
	st_UserTable.nIDNumber = nIDNumber;

	int nMsgLen = sizeof(XENGINE_PROTOCOL_USERINFO);
	AuthRegClient_Protocol_Send((LPCTSTR)&st_UserTable, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETPASS);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("找回密码协议发送失败！\n"));
		return -1;
	}
	while (bPass)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int AuthClient_GetTime()
{
	AUTHREG_PROTOCOL_TIME st_AuthTime;
	TCHAR tszMsgBuffer[2048];

	memset(&st_AuthTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	strcpy(st_AuthTime.tszUserName, lpszUser);
	int nMsgLen = sizeof(AUTHREG_PROTOCOL_TIME);
	AuthRegClient_Protocol_Send((LPCTSTR)&st_AuthTime, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETTIME);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("获取剩余时间协议发送失败！\n"));
		return -1;
	}
	while (bLeftTime)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int AuthClient_Delete()
{
	XENGINE_PROTOCOL_USERINFO st_UserInfo;
	TCHAR tszMsgBuffer[2048];

	memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERINFO));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	strcpy(st_UserInfo.tszUserName, lpszUser);
	strcpy(st_UserInfo.tszUserPass, lpszPass);
	strcpy(st_UserInfo.tszEMailAddr, lpszEmail);
	st_UserInfo.nIDNumber = nIDNumber;

	int nMsgLen = sizeof(XENGINE_PROTOCOL_USERINFO);
	AuthRegClient_Protocol_Send((LPCTSTR)&st_UserInfo, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQDEL);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("注销协议发送失败！\n"));
		return -1;
	}
	while (bDelete)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}

int AuthClient_ReadTxt()
{
	while (bReadTxt)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}

int AuthClient_Timeout()
{
	while (bTimeOut)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}

int AuthClient_Try()
{
	TCHAR tszMsgBuffer[2048];
	LPCTSTR lpszSerialNet = _T("cpuid:112233"); //通过此可以做临时网络验证,安全性比本地临时验证高

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	int nMsgLen = strlen(lpszSerialNet);

	AuthRegClient_Protocol_Send(lpszSerialNet, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQTRYVER);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("登录协议发送失败！\n"));
		return -1;
	}
	while (bTry)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	int nPort = 5500;
	LPCTSTR lpszAddr = _T("127.0.0.1");

	if (!XClient_TCPSelect_Create(&m_Socket, lpszAddr, nPort))
	{
		printf("XClient_TCPSelect_Create:%lX", XClient_GetLastError());
		return -1;
	}
	std::thread pSTDThread(AuthClient_Thread);

	printf("AuthClient_Register\n");
	AuthClient_Register();

	printf("AuthClient_Pay\n");
	AuthClient_Pay();

	printf("AuthClient_Login\n");
	AuthClient_Login();

	printf("AuthClient_GetPass\n");
	AuthClient_GetPass();

	printf("AuthClient_GetTime\n");
	AuthClient_GetTime();

	printf("AuthClient_ReadTxt\n");
	AuthClient_ReadTxt();

	printf("AuthClient_Timeout\n");
	AuthClient_Timeout();

	printf("AuthClient_Delete\n");
	AuthClient_Delete();

	printf("AuthClient_Try\n");
	AuthClient_Try();

	bRun = FALSE;
	XClient_TCPSelect_Close(m_Socket);
	pSTDThread.join();

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}