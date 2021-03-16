#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;

#pragma warning(disable : 6031)
#pragma warning(disable : 4996)


/* �����¼� */
void Sub_1();
/* ���������� */
void Sub_2();
/* �����ź��� */
void Sub_3();
/* ���������� */
void Sub_4();   // Duplicate


/* �ر��¼� */
bool YekCloseHandle(const HANDLE& Handle) {
    DWORD HandleFlags;
    if (GetHandleInformation(Handle, &HandleFlags) && (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
        return !!CloseHandle(Handle);
    return false;
}