// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#ifdef _MSC_BUILD
#include "framework.h"
#include <WinSock2.h>
#endif
#endif //PCH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <memory>
#include <json/json.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include "../XAuth_BuildSwitch.h"
#if (1 == _XAUTH_BUILD_SWITCH_CLIENT_TCP)
#include <XEngine_Include/XEngine_Core/Cryption_Define.h>
#include <XEngine_Include/XEngine_Core/Cryption_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include "../XAuth_Protocol.h"
#endif
#if (1 == _XAUTH_BUILD_SWITCH_CLIENT_HTTP)
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#endif
#include "AuthClient_Define.h"
#include "AuthClient_Error.h"
using namespace std;
/********************************************************************
//    Created:     2023/11/15  10:37:19
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client\pch.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Client
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     公有头文件
//    History:
*********************************************************************/
extern bool AuthClient_IsErrorOccur;
extern XLONG AuthClient_dwErrorCode;

#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_Cryption")
#pragma comment(lib,"XEngine_Client/XClient_Socket")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi")
#ifdef _DEBUG
#ifdef _M_X64
#pragma comment(lib,"../x64/Debug/jsoncpp")
#elif _M_ARM64
#pragma comment(lib,"../ARM64/Debug/jsoncpp")
#elif _M_IX86
#pragma comment(lib,"../Debug/jsoncpp")
#endif
#else
#ifdef _M_X64
#pragma comment(lib,"../x64/Release/jsoncpp")
#elif _M_ARM64
#pragma comment(lib,"../ARM64/Release/jsoncpp")
#elif _M_IX86
#pragma comment(lib,"../Release/jsoncpp")
#endif
#endif
#endif