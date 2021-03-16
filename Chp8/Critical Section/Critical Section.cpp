#include "Critical Section.h"

const int COUNT = 100000;
int g_nSum = 0;
CRITICAL_SECTION g_cs;

// slim
RTL_SRWLOCK g_rsl;


int _tmain() {

    Sub_1();
    g_nSum = 0;
    Sub_2();
    g_nSum = 0;
    Sub_3();
    int sum = 0;
    for (int n = 1; n < COUNT; ++n) {
        sum += n;
    }
    for (int n = 1; n < COUNT; ++n) {
        sum += n;
    }
    _tprintf(_T("sum = %d\r\n"), sum);
    return 0;
}

void Sub_1() {
    HANDLE hThreads[2] = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };
    hThreads[0] = CreateThread(NULL, 0, ThreadProc1, NULL, 0, NULL);
    hThreads[1] = CreateThread(NULL, 0, ThreadProc2, NULL, 0, NULL);
    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
    YekCloseHandle(hThreads[0]);
    YekCloseHandle(hThreads[1]);
    hThreads[0] = INVALID_HANDLE_VALUE;
    hThreads[1] = INVALID_HANDLE_VALUE;
    _tprintf(_T("g_nSum = %d\r\n"), g_nSum);
}

void Sub_2() {
    // 初始化
    InitializeCriticalSection(&g_cs);
    HANDLE hThreads[2] = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };
    hThreads[0] = CreateThread(NULL, 0, ThreadProc3, NULL, 0, NULL);
    hThreads[1] = CreateThread(NULL, 0, ThreadProc4, NULL, 0, NULL);
    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
    YekCloseHandle(hThreads[0]);
    YekCloseHandle(hThreads[1]);
    hThreads[0] = INVALID_HANDLE_VALUE;
    hThreads[1] = INVALID_HANDLE_VALUE;
    _tprintf(_T("g_nSum = %d\r\n"), g_nSum);
    // 清除
    DeleteCriticalSection(&g_cs);
}

void Sub_3() {
    // 初始化
    InitializeSRWLock(&g_rsl);
    HANDLE hThreads[2] = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };
    hThreads[0] = CreateThread(NULL, 0, ThreadProc5, NULL, 0, NULL);
    hThreads[1] = CreateThread(NULL, 0, ThreadProc6, NULL, 0, NULL);
    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
    YekCloseHandle(hThreads[0]);
    YekCloseHandle(hThreads[1]);
    hThreads[0] = INVALID_HANDLE_VALUE;
    hThreads[1] = INVALID_HANDLE_VALUE;
    _tprintf(_T("g_nSum = %d\r\n"), g_nSum);
    // 清除
}

DWORD WINAPI ThreadProc1(LPVOID lpParameter) {
    for (int n = 1; n < COUNT; ++n) {
        g_nSum += n;
    }
    return 0;
}
DWORD WINAPI ThreadProc2(LPVOID lpParameter) {
    for (int n = 1; n < COUNT; ++n) {
        g_nSum += n;
    }
    return 0;
}
DWORD WINAPI ThreadProc3(LPVOID lpParameter) {
    EnterCriticalSection(&g_cs);
    for (int n = 1; n < COUNT; ++n) {
        g_nSum += n;
    }
    LeaveCriticalSection(&g_cs);
    return 0;
}
DWORD WINAPI ThreadProc4(LPVOID lpParameter) {
    EnterCriticalSection(&g_cs);
    for (int n = 1; n < COUNT; ++n) {
        g_nSum += n;
    }
    LeaveCriticalSection(&g_cs);
    return 0;
}
DWORD WINAPI ThreadProc5(LPVOID lpParameter) {
    AcquireSRWLockExclusive(&g_rsl);
    for (int n = 1; n < COUNT; ++n) {
        g_nSum += n;
    }
    ReleaseSRWLockExclusive(&g_rsl);
    return 0;
}
DWORD WINAPI ThreadProc6(LPVOID lpParameter) {
    AcquireSRWLockExclusive(&g_rsl);
    for (int n = 1; n < COUNT; ++n) {
        g_nSum += n;
    }
    ReleaseSRWLockExclusive(&g_rsl);
    return 0;
}

bool YekCloseHandle(HANDLE myHandle)
{
    DWORD HandleFlags;
    if (GetHandleInformation(myHandle, &HandleFlags) && (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
        return !!CloseHandle(myHandle);
    return false;
}