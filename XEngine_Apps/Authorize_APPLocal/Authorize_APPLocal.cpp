#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"jsoncpp")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIClient")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Authorize")
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
#include <XEngine_Include/XEngine_NetHelp/APIClient_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Error.h>

//需要优先配置XEngine
//WINDOWS支持VS2022 x64 debug 编译调试
//linux::g++ -std=c++17 -Wall -g Authorize_APPLocal.cpp -o Authorize_APPLocal.exe -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -lXEngine_BaseLib -lXEngine_OPenSsl -lNetHelp_APIClient -lHelpComponents_Authorize
//macos::g++ -std=c++17 -Wall -g Authorize_APPLocal.cpp -o Authorize_APPLocal.exe -lXEngine_BaseLib -lXEngine_OPenSsl -lNetHelp_APIClient -lHelpComponents_Authorize

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	int nLen = 0;
	int nCode = 0;
	LPCTSTR lpszCreateUrl = _T("http://127.0.0.1:5302/auth/cdkey/create");

	Json::Value st_JsonRoot;
	Json::Value st_JsonAPPInfo;
	Json::Value st_JsonREGInfo;
	Json::Value st_JsonUserInfo;
	st_JsonRoot["tszAddr"] = "http://app.xyry.org";
	st_JsonRoot["nPort"] = 5501;

	st_JsonAPPInfo["tszAppName"] = "XEngine";
	st_JsonAPPInfo["tszAppVer"] = "1.0.0.1001";

	st_JsonREGInfo["tszHardware"] = "5501012NE21N";
	st_JsonREGInfo["enSerialType"] = ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME;
	st_JsonREGInfo["enRegType"] = ENUM_HELPCOMPONENTS_AUTHORIZE_REG_TYPE_TRY;
	st_JsonREGInfo["enHWType"] = ENUM_HELPCOMPONENTS_AUTHORIZE_HW_TYPE_CPU;

	st_JsonUserInfo["tszUserName"] = "qyt";
	st_JsonUserInfo["tszUserContact"] = "486179@qq.com";

	st_JsonRoot["st_AuthAppInfo"] = st_JsonAPPInfo;
	st_JsonRoot["st_AuthRegInfo"] = st_JsonREGInfo;
	st_JsonRoot["st_AuthUserInfo"] = st_JsonUserInfo;

	TCHAR* ptszCreateBuffer = NULL;
	TCHAR* ptszAuthBuffer = NULL;
	if (!APIClient_Http_Request(_T("POST"), lpszCreateUrl, st_JsonRoot.toStyledString().c_str(), &nCode, &ptszCreateBuffer, &nLen))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	printf("接受到数据,大小:%d,内容:%s\n", nLen, ptszCreateBuffer);

	TCHAR tszMsgBuffer[4096];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	//你也可以通过授权模块的API函数来读内存,都一回事,这里为了方便直接写了,请求分钟卡,拥有10分钟,也可以写自定义时间格式
	BaseLib_OperatorFile_WriteProfileFromMemory(ptszCreateBuffer, nLen, "AuthReg", "tszLeftTime", "10", tszMsgBuffer, &nLen);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszCreateBuffer);

	LPCTSTR lpszAuthUrl = _T("http://127.0.0.1:5302/auth/cdkey/auth?pass=123123");
	if (!APIClient_Http_Request(_T("POST"), lpszAuthUrl, tszMsgBuffer, &nCode, &ptszAuthBuffer, &nLen))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	//ptszAuthBuffer 写文件,然后验证即可.如下所示
	//step 1.解密
	CHAR tszCodecBuffer[4096];
	memset(tszCodecBuffer, '\0', sizeof(tszCodecBuffer));
	OPenSsl_XCrypto_Decoder(ptszAuthBuffer, &nLen, tszCodecBuffer, "123123");
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszAuthBuffer);
	printf("接受到数据,大小:%d,内容:\n%s\n", nLen, tszCodecBuffer);
	//step 2.转换格式
	XENGINE_AUTHORIZE_LOCAL st_Authorize;
	memset(&st_Authorize, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));

	Authorize_Local_ReadMemory(tszCodecBuffer, nLen, &st_Authorize);
	//step 3.判断您是否超时
	if (Authorize_Local_GetLeftTimer(&st_Authorize))
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