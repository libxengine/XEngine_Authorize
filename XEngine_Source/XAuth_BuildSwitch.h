#pragma once
/********************************************************************
//    Created:     2024/01/04  10:01:07
//    File Name:   D:\XEngine_Authorize\XEngine_Source\XAuth_BuildSwitch.h
//    File Path:   D:\XEngine_Authorize\XEngine_Source
//    File Base:   XAuth_BuildSwitch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     编译选项
//    History:
*********************************************************************/
#define _XAUTH_BUILD_SWITCH_CLIENT_TCP 1
#define _XAUTH_BUILD_SWITCH_CLIENT_HTTP 1

#if (XENGINE_VERSION_KERNEL <= 9) && (XENGINE_VERSION_MAIN < 10)
typedef XENGINE_LIBTIMER XENGINE_LIBTIME;
#endif