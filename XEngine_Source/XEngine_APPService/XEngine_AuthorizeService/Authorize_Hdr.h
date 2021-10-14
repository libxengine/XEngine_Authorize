#pragma once
#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
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
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Authorize_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/WSProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/WSProtocol_Error.h>
#include "../../XAuth_Protocol.h"
#include "../../XEngine_AuthComponents/AuthComponents_SQLPacket/SQLPacket_Define.h"
#include "../../XEngine_AuthComponents/AuthComponents_SQLPacket/SQLPacket_Error.h"
#include "../../XEngine_AuthComponents/AuthComponents_Session/Session_Define.h"
#include "../../XEngine_AuthComponents/AuthComponents_Session/Session_Error.h"
#include "../../AuthorizeModule_Protocol/Protocol_Define.h"
#include "../../AuthorizeModule_Protocol/Protocol_Error.h"
#include "../../AuthorizeModule_Configure/Config_Define.h"
#include "../../AuthorizeModule_Configure/Config_Error.h"

extern BOOL bIsRun;
extern XLOG xhLog;
extern XNETHANDLE xhTCPSocket;
extern XNETHANDLE xhWSSocket;
extern XNETHANDLE xhTCPPacket;
extern XHANDLE xhWSPacket;
extern XNETHANDLE xhTCPPool;
extern XNETHANDLE xhWSPool;
extern AUTHORIZE_CONFIGURE st_AuthConfig;

#define XENGINE_AUTH_APP_NETTYPE_TCP 0
#define XENGINE_AUTH_APP_NETTYPE_WS 1

typedef struct
{
	int nPoolIndex;
}XENGINE_THREADINFO;

#include "Authorize_Config.h"
#include "Authorize_Net.h"
#include "Authorize_Task.h"
#include "Authorize_TCPTask.h"
#include "Authorize_WSTask.h"

#ifdef _WINDOWS
#ifdef _DEBUG
#pragma comment(lib,"../../Debug/AuthorizeModule_Configure.lib")
#pragma comment(lib,"../../Debug/AuthorizeModule_Protocol.lib")
#pragma comment(lib,"../../Debug/AuthComponents_Session.lib")
#pragma comment(lib,"../../Debug/AuthComponents_SQLPacket.lib")
#else
#ifdef WIN64
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
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Core/XEngine_ManagePool.lib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Packets.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Authorize.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_WSProtocol.lib")
#endif