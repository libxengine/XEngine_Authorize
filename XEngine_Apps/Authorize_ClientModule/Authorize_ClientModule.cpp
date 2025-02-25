#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#ifdef _WIN64
#pragma comment(lib,"../../XEngine_Source/x64/Debug/AuthorizeModule_Client")
#else
#pragma comment(lib,"../../XEngine_Source/Debug/AuthorizeModule_Client")
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include "../../XEngine_Source/XAuth_BuildSwitch.h"
#include "../../XEngine_Source/AuthorizeModule_Client/AuthClient_Define.h"
#include "../../XEngine_Source/AuthorizeModule_Client/AuthClient_Error.h"

//Windows::vs2022 x86 debug 编译即可
//Linux MACOS::g++ -std=c++17 -Wall -g Authorize_ClientModule.cpp -o Authorize_ClientModule.exe -L ../../XEngine_Release/ -lAuthorizeModule_Client -ljsoncpp -Wl,-rpath=../../XEngine_Release

int TestClient_HardLogin()
{
	if (!AuthClient_Connector_Connect(_X("127.0.0.1"), 5300))
	{
		printf("AuthClient_Connector_Connect:%lX\n", AuthClient_GetLastError());
		return -1;
	}
	if (!AuthClient_Connector_Login("2FDWAD02JD2091", ""))
	{
		printf("AuthClient_Connector_Login:%lX\n", AuthClient_GetLastError());
		return -1;
	}

	AuthClient_Connector_Close();
	return 0;
}

int TestClient_TryLogin()
{
	//插入查询
	LPCXSTR lpszTryInsert = _X("http://127.0.0.1:5302/auth/user/try");

	if (AuthClient_HTTPVer_TryRequest(lpszTryInsert))
	{
		printf("ok\n");
	}
	else
	{
		printf("failed\n");
	}
	return 0;
}
int main()
{
	TestClient_TryLogin();
	return 0;
}