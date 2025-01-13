// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。
HWND hMainWnd = NULL;
HWND hUserWnd = NULL;
HWND hConfigWnd = NULL;
bool bCrypto = false;
LPCTSTR lpszStuType[2] = { _T("离线"),_T("在线") };