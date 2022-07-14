// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。
XLOG xhLog = NULL;
XHANDLE xhTCPSocket = 0;
XHANDLE xhWSSocket = 0;
XHANDLE xhTCPPacket = 0;
XHANDLE xhWSPacket = 0;
XNETHANDLE xhTCPPool = 0;
XNETHANDLE xhWSPool = 0;
AUTHORIZE_CONFIGURE st_AuthConfig;

HWND hUserWnd = NULL;
LPCSTR lpszKeyType[5] = { "未知类型","分钟卡","天数卡","次数卡","自定义卡" };