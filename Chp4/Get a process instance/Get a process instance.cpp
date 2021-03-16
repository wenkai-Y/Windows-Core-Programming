#include "Get a process instance.h"


/* 在连接器中修改子进程设置 */
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {
    TCHAR BufferData[0x1000] = { 0 };
    HMODULE ModuleHandle = (HMODULE)&__ImageBase;
    //_stprintf(BufferData, _T("0x%p"), __ImageBase);
    _stprintf_s(BufferData, _T("0x%p\r\n"), ModuleHandle);
    OutputDebugString(BufferData);
    
    Sub_1();
    return 0;
}

/*
INT WINAPI _tWinMain(
    _In_ HINSTANCE Instance,
    _In_opt_ HINSTANCE PrevInstance,
    _In_ PTSTR CmdLine,
    _In_ INT CmdShow
)
{
    TCHAR BufferData[0x1000] = { 0 };
    _stprintf(BufferData, _T("0x%p"), Instance);
    OutputDebugString(BufferData);
    Sub_1();
    return 0;
}*/


void Sub_1() {

    HMODULE ModuleBase = GetModuleHandle(NULL);
    TCHAR BufferData[0x1000] = { 0 };
    _stprintf_s(BufferData, _T("0x%p\r\n"), ModuleBase);
    OutputDebugString(BufferData);
    _tprintf(_T("%s\r\n"), BufferData);
}

void Sub_2() {
    HMODULE ModuleBase = NULL;
    TCHAR ModuleName[0x10000];
    //给定一个模块的基地值获得模块名称
    GetModuleFileName(NULL, ModuleName, 0x10000);
    TCHAR BufferData[0x1000] = { 0 };

    _stprintf_s(BufferData, _T("0x%p"), ModuleBase);

    OutputDebugString(BufferData);
}