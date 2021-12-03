// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"

#endif //PCH_H


#define WINSDK_SKIN_USERMSG_TRAY (WM_USER + 10012)
#include <io.h>
#include <list>

using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/WSProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/WSProtocol_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/EmailClient_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/EmailClient_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include "../../XAuth_Protocol.h"
#include "../../XEngine_AuthComponents/AuthComponents_SQLPacket/SQLPacket_Define.h"
#include "../../XEngine_AuthComponents/AuthComponents_SQLPacket/SQLPacket_Error.h"
#include "../../XEngine_AuthComponents/AuthComponents_Session/Session_Define.h"
#include "../../XEngine_AuthComponents/AuthComponents_Session/Session_Error.h"
#include "../../AuthorizeModule_Protocol/Protocol_Define.h"
#include "../../AuthorizeModule_Protocol/Protocol_Error.h"
#include "../../AuthorizeModule_Configure/Config_Define.h"
#include "../../AuthorizeModule_Configure/Config_Error.h"
#include "Authorize_Dialog/CDialog_Configure.h"
#include "Authorize_Dialog/CDialog_Serial.h"
#include "Authorize_Dialog/CDialog_User.h"
#include "Authorize_Dialog/CDialog_Local.h"
#include "Authorize_Dialog/CDialog_Info.h"
#include "XEngine_AuthorizeAppDlg.h"
#include "Authorize_Task/Authorize_TCPTask.h"
#include "Authorize_Task/Authorize_WSTask.h"
#include "Authorize_Task/Authorize_TaskLog.h"
#include "Authorize_Task/Authorize_TaskNetwork.h"
#include "Authorize_Task/Authorize_TaskEvent.h"

#define XENGINE_AUTH_APP_NETTYPE_TCP 0
#define XENGINE_AUTH_APP_NETTYPE_WS 1

extern XNETHANDLE xhTCPSocket;
extern XNETHANDLE xhWSSocket;
extern XHANDLE xhTCPPacket;
extern XHANDLE xhWSPacket;
extern XNETHANDLE xhTCPPool;
extern XNETHANDLE xhWSPool;
extern AUTHORIZE_CONFIGURE st_AuthConfig;

extern HWND hUserWnd;

typedef struct
{
	int nPoolIndex;
	LPVOID lPClass;
}XENGINE_THREADINFO;

#ifdef _DEBUG
#pragma comment(lib,"../../Debug/AuthComponents_SQLPacket")
#pragma comment(lib,"../../Debug/AuthComponents_Session")
#pragma comment(lib,"../../Debug/AuthorizeModule_Configure")
#pragma comment(lib,"../../Debug/AuthorizeModule_Protocol")
#else
#ifdef _WIN64
#pragma comment(lib,"../../x64/Release/AuthorizeModule_Configure.lib")
#pragma comment(lib,"../../x64/Release/AuthorizeModule_Protocol.lib")
#pragma comment(lib,"../../x64/Release/AuthComponents_Session.lib")
#pragma comment(lib,"../../x64/Release/AuthComponents_SQLPacket.lib")
#else
#pragma comment(lib,"../../Release/AuthorizeModule_Configure.lib")
#pragma comment(lib,"../../Release/AuthorizeModule_Protocol.lib")
#pragma comment(lib,"../../Release/AuthComponents_Session.lib")
#pragma comment(lib,"../../Release/AuthComponents_SQLPacket.lib")
#endif
#endif

#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Core/XEngine_ManagePool.lib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Packets.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Authorize.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_WSProtocol.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_EmailClient.lib")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi.lib")