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
#include <string.h>
#include <list>
#include <string>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Error.h>
#include "../XAuth_Protocol.h"
#include "../AuthorizeModule_CDKey/CDKey_Define.h"
#include "../AuthorizeModule_CDKey/CDKey_Error.h"
#include "Database_Define.h"
#include "Database_Error.h"
/********************************************************************
//    Created:     2022/05/26  11:00:58
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database\pch.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_Database
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     公有头文件
//    History:
*********************************************************************/
extern bool SQLPacket_IsErrorOccur;
extern XLONG SQLPacket_dwErrorCode;

#ifdef _UNICODE
typedef std::wstring xstring;
#else
typedef std::string xstring;
#endif

#ifdef _WINDOWS
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_DataBase.lib")
#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib,"../x64/Debug/AuthorizeModule_CDKey")
#else
#pragma comment(lib,"../x64/Release/AuthorizeModule_CDKey")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib,"../Debug/AuthorizeModule_CDKey")
#else
#pragma comment(lib,"../Release/AuthorizeModule_CDKey")
#endif
#endif
#endif