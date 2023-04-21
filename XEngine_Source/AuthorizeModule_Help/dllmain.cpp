// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

bool APIENTRY DllMain( HMODULE hModule,
                       XLONG  ul_reason_for_call,
                       XPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return true;
}

