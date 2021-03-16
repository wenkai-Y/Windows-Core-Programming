#pragma once
#include <Windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;


/////////////////////////////////////////////

VOID Sub_1();
VOID Sub_2();
VOID Sub_3();

////////////////////////////////////////////
DWORD WINAPI ThreadProc1(LPVOID lpParameter);
DWORD WINAPI ThreadProc2(LPVOID lpParameter);
DWORD WINAPI ThreadProc3(LPVOID lpParameter);
DWORD WINAPI ThreadProc4(LPVOID lpParameter);
DWORD WINAPI ThreadProc5(LPVOID lpParameter);
DWORD WINAPI ThreadProc6(LPVOID lpParameter);
////////////////////////////////////////////
bool YekCloseHandle(HANDLE myHandle)
{
    DWORD HandleFlags;
    if (GetHandleInformation(myHandle, &HandleFlags) && (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
        return !!CloseHandle(myHandle);
    _tprintf(_T("Fail.\r\n"));
    return false;
}