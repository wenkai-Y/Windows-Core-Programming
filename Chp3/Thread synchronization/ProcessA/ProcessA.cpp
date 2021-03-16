#include "ProcessA.h"

int _tmain() {

    _tsetlocale(LC_ALL, _T("Chinese-simplified"));//使当前程序支持中文

    //Sub_1();
    //Sub_2();
    //Sub_3();
    Sub_4();
    _tprintf(_T("Input anykey to exit.\r\n"));
    _gettchar();
    _gettchar();
    return 0;
}


/* 命名事件 */
void Sub_1() {

    DWORD LastError(-1);
    HANDLE EventHandle = CreateEvent(NULL, FALSE, FALSE, _T("Yuwenkai"));
    if (EventHandle == NULL) {
        LastError = GetLastError();
        goto Exit;
    }
    // wait for single object
    WaitForSingleObject(EventHandle, INFINITE);
    _tprintf(_T("Good for you!\r\n"));
Exit:
    if (EventHandle != NULL) {
        if (YekCloseHandle(EventHandle) == true) {
            EventHandle = INVALID_HANDLE_VALUE;
        }
        else {
            LastError = GetLastError();
        }
    }
}

/* 命名互斥体 */
void Sub_2() {
    DWORD LastError(-1);
    // 创建一个命名互斥体内核对象
    HANDLE MutexHandle = CreateMutex(NULL, FALSE, _T("Yuwenkai"));
    if (MutexHandle == NULL) {
        LastError = GetLastError();
        goto Exit;
    }
    {
        _tprintf(_T("进程ID：%d\r\n"), (int)GetCurrentProcess());
        _tprintf(_T("互斥体：%d\r\n"), (int)MutexHandle);
        WaitForSingleObject(MutexHandle, INFINITE); // 互斥体对象正在++
        _tprintf(_T("天王盖地虎\r\n"));
        TCHAR BufferData[0x1000] = { 0 };
        _tscanf(_T("%s"), BufferData);
        if (_tcscmp(BufferData, _T("宝塔镇河妖")) == 0) {
            ReleaseMutex(MutexHandle);
        }
    }
Exit:
    if (MutexHandle != NULL) {
        if (YekCloseHandle(MutexHandle) == true) {
            MutexHandle = INVALID_HANDLE_VALUE;
        }
        else {
            LastError = GetLastError();
        }
    }
}

/* 命名信号量 */
void Sub_3() {
    int LastError = -1;
    // 创建两个不授信的信号灯
    HANDLE SemaphoreHandle = CreateSemaphore(NULL, 0, 2, _T("Yuwenkai"));
    if (SemaphoreHandle == NULL) {
        LastError = GetLastError();
        goto Exit;
    }
    {
        TCHAR BufferData[0x1000] = { 0 };
        _tprintf(_T("口令："));
        _tscanf(_T("%s"), BufferData);
        if (_tcscmp(BufferData, _T("于文凯")) == 0) {
            // 授信两盏灯
            ReleaseSemaphore(SemaphoreHandle, 2, NULL);

            Sleep(10);
        }
    }
Exit:
    if (SemaphoreHandle != NULL) {
        if (YekCloseHandle(SemaphoreHandle) == true) {
            SemaphoreHandle = INVALID_HANDLE_VALUE;
        }
        else {
            LastError = GetLastError();
        }
    }
}

/* 匿名互斥体 */
void Sub_4() {
    DWORD LastError(-1);
    HANDLE MutexHandle = CreateMutex(NULL, FALSE, NULL);
    if (MutexHandle == NULL) {
        LastError = GetLastError();
        goto Exit;
    }

    _tprintf(_T("进程ID: %d\r\n"), (int)GetCurrentProcessId());
    _tprintf(_T("互斥体: %d\r\n"), (int)MutexHandle);
    _tprintf(_T("Please run ProcessB.exe!\r\n"));

    WaitForSingleObject(MutexHandle, INFINITE);
    {
        TCHAR BufferData[0x1000] = { 0 };
        _tprintf(_T("口令："));
        _tscanf(_T("%s"), BufferData);
        if (_tcscmp(BufferData, _T("昝博玮")) == 0) {
            ReleaseMutex(MutexHandle);
        }
        Sleep(1000);
    }
Exit:
    if (MutexHandle != NULL) {
        if (YekCloseHandle(MutexHandle) == true) {
            MutexHandle = INVALID_HANDLE_VALUE;
        }
        else {
            LastError = GetLastError();
        }
    }
}