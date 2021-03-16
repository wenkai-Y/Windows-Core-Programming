#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;

#pragma warning(disable : 6031)
#pragma warning(disable : 4996)


/* 命名事件 */
void Sub_1();
/* 命名互斥体 */
void Sub_2();
/* 命名信号量 */
void Sub_3();
/* 匿名互斥体 */
void Sub_4();   // Duplicate


/* 关闭事件 */
bool YekCloseHandle(const HANDLE& Handle) {
    DWORD HandleFlags;
    if (GetHandleInformation(Handle, &HandleFlags) && (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
        return !!CloseHandle(Handle);
    return false;
}