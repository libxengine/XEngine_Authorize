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
#include <list>
#include <vector>
#include <memory>
#include <thread>
#include <shared_mutex>
#include <unordered_map>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Error.h>
#include "../../XAuth_Protocol.h"
#include "Session_Define.h"
#include "Session_Error.h"
/********************************************************************
//    Created:     2021/07/14  13:22:08
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_Session\pch.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\XEngine_AuthComponents\AuthComponents_Session
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     公有头文件
//    History:
*********************************************************************/
extern BOOL Session_IsErrorOccur;
extern DWORD Session_dwErrorCode;

#ifdef _WINDOWS
#ifdef _WIN64
#pragma comment(lib,"x64/XEngine_BaseLib/XEngine_BaseLib.lib")
#else
#pragma comment(lib,"x86/XEngine_BaseLib/XEngine_BaseLib.lib")
#endif // _WIN64
#else

#endif