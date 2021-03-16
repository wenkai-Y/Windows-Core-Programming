#include "Inside Threads.h"

int _tmain() {
    setlocale(LC_ALL, "Chinese-simplified");
    InsideThreads();
    return 0;
}

VOID InsideThreads() {
    DWORD LastError = 0;
    HANDLE ThreadHandle = CreateThread( NULL
                                      , 0x1000
                                      , (LPTHREAD_START_ROUTINE)ThreadFuntion
                                      , NULL
                                      , 0
                                      , NULL);
    if (ThreadHandle == NULL) {
        LastError = GetLastError();
        _tprintf(_T("CreateThread Fail: %d.\r\n"), LastError);
        return;
    }
    Sleep(1000);
    SuspendThread(ThreadHandle);    // 将线程挂起
    CONTEXT Context;                // 上下文
    memset(&Context, 0, sizeof(CONTEXT));
    Context.ContextFlags = CONTEXT_CONTROL | CONTEXT_FULL;
    GetThreadContext(ThreadHandle, &Context);
    #ifdef _WIN64
        Context.Rip = (DWORD)ShowData;
    #else
        Context.Eip = (DWORD)ShowData;
    #endif
    SetThreadContext(ThreadHandle, &Context);
    ResumeThread(ThreadHandle);
    WaitForSingleObject(ThreadHandle, INFINITE);
    if (ThreadHandle != NULL) {
        YekCloseHandle(ThreadHandle);
    }
    system("pause");
}

VOID ShowData() {
    int i = 0;
    for (i; i < 1000; ++i) {
        _tprintf(_T("ShowData()\r\n"));
    }
}

DWORD WINAPI ThreadFuntion(LPVOID ParameterData) {
    int i = 0;
    for (i; i < 1000; ++i) {
        _tprintf(_T("ThreadFuntion()\r\n"));
    }
    return 0;
}

/* 关闭句柄 */
bool YekCloseHandle(HANDLE myHandle)
{
    DWORD HandleFlags;
    if (GetHandleInformation(myHandle, &HandleFlags) && (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
        return !!CloseHandle(myHandle);
    return false;
}