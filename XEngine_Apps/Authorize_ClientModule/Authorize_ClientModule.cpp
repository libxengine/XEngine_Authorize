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
//Linux MACOS::g++ -std=c++17 -Wall -g Authorize_ClientModule.cpp -o Authorize_ClientModule.exe -L ../../XEngine_Release/ -lAuthorizeModule_Client -ljsoncpp

int main()
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