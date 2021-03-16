#include "InterLocked Funtioins.h"

// global
UINT g_x = 0;
long g_fResourceInUse = FALSE;


int _tmain() {

    ////
    ////
    Sub_1();
    memset(&g_x, 0, sizeof(UINT));

    Sub_2();
    memset(&g_x, 0, sizeof(UINT));

    Sub_3();
    memset(&g_x, 0, sizeof(UINT));
    return 0;
}

VOID Sub_1() {
    HANDLE hThreads[2] = { INVALID_HANDLE_VALUE , INVALID_HANDLE_VALUE };
    DWORD ThreadId1 = NULL;
    DWORD ThreadId2 = NULL;
    hThreads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc1,
        NULL, 0, &ThreadId1);
    hThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc2,
        NULL, 0, &ThreadId2);
    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
    YekCloseHandle(hThreads[0]);
    YekCloseHandle(hThreads[1]);
    hThreads[0] = hThreads[1] = INVALID_HANDLE_VALUE;
    _tprintf(_T("g_x = %d\n"), g_x);
}

VOID Sub_2() {
    HANDLE hThreads[2] = { INVALID_HANDLE_VALUE , INVALID_HANDLE_VALUE };
    DWORD ThreadId1 = NULL;
    DWORD ThreadId2 = NULL;
    hThreads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc3,
        NULL, 0, &ThreadId1);
    hThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc4,
        NULL, 0, &ThreadId2);
    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
    YekCloseHandle(hThreads[0]);
    YekCloseHandle(hThreads[1]);
    hThreads[0] = hThreads[1] = INVALID_HANDLE_VALUE;
    _tprintf(_T("g_x = %d\n"), g_x);
}

VOID Sub_3() {
    HANDLE hThreads[2] = { INVALID_HANDLE_VALUE , INVALID_HANDLE_VALUE };
    DWORD ThreadId1 = NULL;
    DWORD ThreadId2 = NULL;
    hThreads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc5,
        NULL, 0, &ThreadId1);
    hThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc6,
        NULL, 0, &ThreadId2);
    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
    YekCloseHandle(hThreads[0]);
    YekCloseHandle(hThreads[1]);
    hThreads[0] = hThreads[1] = INVALID_HANDLE_VALUE;
    _tprintf(_T("g_x = %d\n"), g_x);
}
///////////////////////////////////////////////////////
DWORD WINAPI ThreadProc1(LPVOID lpParameter) {
    int i = 0;
    while (i < 10000000) {
        g_x++;
        i++;
    }
    return 0;
}


DWORD WINAPI ThreadProc2(LPVOID lpParameter) {
    int i = 0;
    while (i < 10000000) {
        g_x++;
        i++;
    }
    return 0;
}


DWORD WINAPI ThreadProc3(LPVOID lpParameter) {
    int i = 0;
    while (i < 10000000) {
        InterlockedExchangeAdd(&g_x, 1);
        i++;
    }
    return 0;
}


DWORD WINAPI ThreadProc4(LPVOID lpParameter) {
    int i = 0;
    while (i < 10000000) {
        InterlockedExchangeAdd(&g_x, 1);
        i++;
    }
    return 0;
}


DWORD WINAPI ThreadProc5(LPVOID lpParameter) {
    int i = 0;
    while (i < 10000000) {
        while (InterlockedExchange(&g_fResourceInUse, TRUE) == TRUE) {
            SwitchToThread();
        }
        g_x++;
        i++;
        InterlockedExchange(&g_fResourceInUse, FALSE);
    }
    return 0;
}


DWORD WINAPI ThreadProc6(LPVOID lpParameter) {
    int i = 0;
    while (i < 10000000) {
        while (InterlockedExchange(&g_fResourceInUse, TRUE) == TRUE) {
            SwitchToThread();
        }
        g_x++;
        i++;
        InterlockedExchange(&g_fResourceInUse, FALSE);
    }
    return 0;
}