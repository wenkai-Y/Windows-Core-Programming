// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:    // 当进程加载该模块时执行
    {
        //MessageBox(NULL, _T("Load"), _T("Prompt"), NULL);
        break;
    }    
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH: 
    {
        //MessageBox(NULL, _T("Unload"), _T("Prompt"), NULL);
        break;
    }
    }
    return TRUE;
}

