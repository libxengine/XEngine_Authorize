﻿// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"

#endif //PCH_H
#include <json/json.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Error.h>
#include "../../XAuth_Protocol.h"
#include "../../AuthorizeModule_Help/AuthHelp_Define.h"
#include "../../AuthorizeModule_Help/AuthHelp_Error.h"
#include "Authorize_Dialog/Dialog_Config.h"
#include "Authorize_Dialog/Dialog_User.h"
#include "Authorize_Dialog/Dialog_Serial.h"
#include "Authorize_Dialog/Dialog_Modify.h"

extern HWND hUserWnd;
extern HWND hConfigWnd;
extern LPCSTR lpszStuType[2];
#if XENGINE_VERSION_BIT < 7390001001
extern LPCSTR lpszXSerialType[5];
extern LPCSTR lpszXDevType[9];
extern LPCSTR lpszXLevelType[7];
#endif

#ifdef _DEBUG
#ifdef _WIN64
#pragma comment(lib,"../../x64/Debug/jsoncpp")
#pragma comment(lib,"../../x64/Debug/AuthorizeModule_Help")
#else
#pragma comment(lib,"../../Debug/jsoncpp")
#pragma comment(lib,"../../Debug/AuthorizeModule_Help")
#endif
#else
#ifdef _WIN64
#pragma comment(lib,"../../x64/Release/jsoncpp")
#pragma comment(lib,"../../x64/Release/AuthorizeModule_Help")
#else
#pragma comment(lib,"../../Release/jsoncpp")
#pragma comment(lib,"../../Release/AuthorizeModule_Help")
#endif
#endif

#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIHelp.lib")