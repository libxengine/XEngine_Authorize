﻿// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

#ifdef _WINDOWS
// 添加要在此处预编译的标头
#include "framework.h"
#include <tchar.h>
#endif
#endif //PCH_H
#include <string.h>
#include <errno.h>
#include <memory>
#include <list>
#include <json/json.h>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include "../XAuth_BuildSwitch.h"
#include "../XAuth_Protocol.h"
#include "../AuthorizeModule_CDKey/CDKey_Define.h"
#include "../AuthorizeModule_CDKey/CDKey_Error.h"
#include "../AuthorizeModule_Configure/Config_Define.h"
#include "../AuthorizeModule_Session/Session_Define.h"
#include "../AuthorizeModule_Session/Session_Error.h"
#include "Protocol_Define.h"
#include "Protocol_Error.h"
/********************************************************************
//    Created:     2021/07/16  09:20:47
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol\pch.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Protocol
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     公用头文件
//    History:
*********************************************************************/
extern bool Protocol_IsErrorOccur;
extern XLONG Protocol_dwErrorCode;

#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
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