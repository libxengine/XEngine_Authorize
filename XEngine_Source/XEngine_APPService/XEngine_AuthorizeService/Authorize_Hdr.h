#pragma once
#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#include <minidumpapiset.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <signal.h>
#include <errno.h>
#include <thread>
#include <list>
#include <string>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#include <XEngine_Include/XEngine_Core/Cryption_Define.h>
#include <XEngine_Include/XEngine_Core/Cryption_Error.h>
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIAddr_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIAddr_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/WSProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/WSProtocol_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Error.h>
#include "../../XAuth_BuildSwitch.h"
#include "../../XAuth_Protocol.h"
#include "../../AuthorizeModule_Configure/Config_Define.h"
#include "../../AuthorizeModule_Configure/Config_Error.h"
#include "../../AuthorizeModule_Database/Database_Define.h"
#include "../../AuthorizeModule_Database/Database_Error.h"
#include "../../AuthorizeModule_Session/Session_Define.h"
#include "../../AuthorizeModule_Session/Session_Error.h"
#include "../../AuthorizeModule_Protocol/Protocol_Define.h"
#include "../../AuthorizeModule_Protocol/Protocol_Error.h"
#include "../../AuthorizeModule_Help/AuthHelp_Define.h"
#include "../../AuthorizeModule_Help/AuthHelp_Error.h"
#include "../../AuthorizeModule_CDKey/CDKey_Define.h"
#include "../../AuthorizeModule_CDKey/CDKey_Error.h"
#include "../../XEngine_Depend/XEngine_Module/XEngine_InfoReport/InfoReport_Define.h"
#include "../../XEngine_Depend/XEngine_Module/XEngine_InfoReport/InfoReport_Error.h"

extern bool bIsRun;
extern bool bIsTest;
extern XHANDLE xhLog;
extern XHANDLE xhTCPSocket;
extern XHANDLE xhWSSocket;
extern XHANDLE xhHttpSocket;

extern XHANDLE xhTCPPacket;
extern XHANDLE xhWSPacket;
extern XHANDLE xhHttpPacket;

extern XHANDLE xhTCPHeart;
extern XHANDLE xhWSHeart;
extern XHANDLE xhHTTPHeart;

extern XHANDLE xhMemPool;
extern XHANDLE xhTCPPool;
extern XHANDLE xhWSPool;
extern XHANDLE xhHttpPool;

extern XENGINE_SERVICECONFIG st_AuthConfig;
extern XENGINE_FUNCTIONSWITCH st_FunSwitch;

#define XENGINE_AUTH_APP_NETTYPE_TCP 0
#define XENGINE_AUTH_APP_NETTYPE_WS 1
#define XENGINE_AUTH_APP_NETTYPE_HTTP 2
#define XENGINE_AUTH_MAX_BUFFER (1024 * 1024 * 10)

#include "Authorize_Config.h"
#include "Authorize_Net.h"
#include "Authorize_Task.h"
#include "Authorize_TCPTask.h"
#include "Authorize_WSTask.h"
#include "Authorize_HttpTask.h"
#include "AuthorizeHTTP_Post/AuthorizeHTTP_User.h"
#include "AuthorizeHTTP_Post/AuthorizeHTTP_Client.h"
#include "AuthorizeHTTP_Post/AuthorizeHTTP_Serial.h"
#include "AuthorizeHTTP_Post/AuthorizeHTTP_Pass.h"
#include "AuthorizeHTTP_Post/AuthorizeHTTP_Switch.h"
#include "AuthorizeHTTP_Post/AuthorizeHTTP_Banned.h"
#include "AuthorizeHTTP_Post/AuthorizeHTTP_CDKey.h"
#include "AuthorizeHTTP_Post/AuthorizeHTTP_Announcement.h"
#include "AuthorizeHTTP_Post/AuthorizeHTTP_Try.h"
#include "AuthorizeHTTP_Get/AuthorizeHTTP_Token.h"
#include "AuthorizeHTTP_Get/AuthorizeHTTP_GetTask.h"

#ifdef _MSC_BUILD
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Dbghelp.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Core/XEngine_ManagePool.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Cryption.lib")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIAddr.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Packets.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_WSProtocol.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_HttpProtocol.lib")
#ifdef _DEBUG
#ifdef _M_X64
#pragma comment(lib,"../../x64/Debug/AuthorizeModule_Configure.lib")
#pragma comment(lib,"../../x64/Debug/AuthorizeModule_Protocol.lib")
#pragma comment(lib,"../../x64/Debug/AuthorizeModule_Session.lib")
#pragma comment(lib,"../../x64/Debug/AuthorizeModule_Database.lib")
#pragma comment(lib,"../../x64/Debug/AuthorizeModule_Help.lib")
#pragma comment(lib,"../../x64/Debug/AuthorizeModule_CDKey.lib")
#pragma comment(lib,"../../x64/Debug/XEngine_InfoReport.lib")
#elif _M_ARM64
#pragma comment(lib,"../../ARM64/Debug/AuthorizeModule_Configure.lib")
#pragma comment(lib,"../../ARM64/Debug/AuthorizeModule_Protocol.lib")
#pragma comment(lib,"../../ARM64/Debug/AuthorizeModule_Session.lib")
#pragma comment(lib,"../../ARM64/Debug/AuthorizeModule_Database.lib")
#pragma comment(lib,"../../ARM64/Debug/AuthorizeModule_Help.lib")
#pragma comment(lib,"../../ARM64/Debug/AuthorizeModule_CDKey.lib")
#pragma comment(lib,"../../ARM64/Debug/XEngine_InfoReport.lib")
#elif _M_IX86
#pragma comment(lib,"../../Debug/AuthorizeModule_Configure.lib")
#pragma comment(lib,"../../Debug/AuthorizeModule_Protocol.lib")
#pragma comment(lib,"../../Debug/AuthorizeModule_Session.lib")
#pragma comment(lib,"../../Debug/AuthorizeModule_Database.lib")
#pragma comment(lib,"../../Debug/AuthorizeModule_Help.lib")
#pragma comment(lib,"../../Debug/AuthorizeModule_CDKey.lib")
#pragma comment(lib,"../../Debug/XEngine_InfoReport.lib")
#endif
#else
#ifdef _M_X64
#pragma comment(lib,"../../x64/Release/AuthorizeModule_Configure.lib")
#pragma comment(lib,"../../x64/Release/AuthorizeModule_Protocol.lib")
#pragma comment(lib,"../../x64/Release/AuthorizeModule_Session.lib")
#pragma comment(lib,"../../x64/Release/AuthorizeModule_Database.lib")
#pragma comment(lib,"../../x64/Release/AuthorizeModule_Help.lib")
#pragma comment(lib,"../../x64/Release/AuthorizeModule_CDKey.lib")
#pragma comment(lib,"../../x64/Release/XEngine_InfoReport.lib")
#elif _M_ARM64
#pragma comment(lib,"../../ARM64/Release/AuthorizeModule_Configure.lib")
#pragma comment(lib,"../../ARM64/Release/AuthorizeModule_Protocol.lib")
#pragma comment(lib,"../../ARM64/Release/AuthorizeModule_Session.lib")
#pragma comment(lib,"../../ARM64/Release/AuthorizeModule_Database.lib")
#pragma comment(lib,"../../ARM64/Release/AuthorizeModule_Help.lib")
#pragma comment(lib,"../../ARM64/Release/AuthorizeModule_CDKey.lib")
#pragma comment(lib,"../../ARM64/Release/XEngine_InfoReport.lib")
#elif _M_IX86
#pragma comment(lib,"../../Release/AuthorizeModule_Configure.lib")
#pragma comment(lib,"../../Release/AuthorizeModule_Protocol.lib")
#pragma comment(lib,"../../Release/AuthorizeModule_Session.lib")
#pragma comment(lib,"../../Release/AuthorizeModule_Database.lib")
#pragma comment(lib,"../../Release/AuthorizeModule_Help.lib")
#pragma comment(lib,"../../Release/AuthorizeModule_CDKey.lib")
#pragma comment(lib,"../../Release/XEngine_InfoReport.lib")
#endif
#endif
#endif