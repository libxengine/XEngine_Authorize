// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。
HWND hUserWnd = NULL;
HWND hConfigWnd = NULL;
LPCSTR lpszStuType[2] = { "离线","在线" };
#if XENGINE_VERSION_BIT < 7390001001
LPCSTR lpszXSerialType[5] = { "未知类型","分钟卡","天数卡","次数卡","自定义卡" };
LPCSTR lpszXDevType[9] = { "未知","PC端","URFACE","安卓","IPAD","IOS","WEB","嵌入式","其他" };
LPCSTR lpszXLevelType[7] = { "封禁","管理员","维护员","2","3","4","5" };
#endif