// pch.h: 这是预编译标头文件。
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
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <list>
#include <string>
#include <json/json.h>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include "Config_Define.h"
#include "Config_Error.h"
/********************************************************************
//    Created:     2021/07/26  10:00:36
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Configure\pch.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Configure
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     公用头文件
//    History:
*********************************************************************/
extern bool Config_IsErrorOccur;
extern XLONG Config_dwErrorCode;

#ifdef _MSC_BUILD
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