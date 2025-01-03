// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

#ifdef _MSC_BUILD
// 添加要在此处预编译的标头
#include "framework.h"
#include <tchar.h>
#include <io.h>
#else
#include <unistd.h>
#endif
#endif //PCH_H
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <list>
#include <string>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemConfig_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemConfig_Error.h>
#include "../XAuth_BuildSwitch.h"
#include "../XAuth_Protocol.h"
#include "CDKey_Define.h"
#include "CDKey_Error.h"
/********************************************************************
//    Created:     2024/07/08  17:29:57
//    File Name:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey\pch.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source\AuthorizeModule_CDKey
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     公有头文件
//    History:
*********************************************************************/
extern bool Authorize_IsErrorOccur;
extern XLONG Authorize_dwErrorCode;

typedef std::string xstring;

#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemConfig.lib")
#endif