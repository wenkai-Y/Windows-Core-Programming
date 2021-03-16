#include "Process.h"


#define VARIABLE_NAME _T("WENKAI-Y")   //COMPUTERNAME   WENKAI-Y

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {
    _tsetlocale(LC_ALL, _T("Chinese-simplified"));
    Sub_1();
    return 0;
}


void Sub_1() {

    // 创建进程前的准备
    STARTUPINFO StartupInfo = { 0 };
    StartupInfo.cb = sizeof(STARTUPINFO);
    PROCESS_INFORMATION ProcessInfo = { 0 };
    ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));
    DWORD Flags = CREATE_NEW_CONSOLE;
#ifdef _UNICODE
    Flags |= CREATE_UNICODE_ENVIRONMENT;
#endif

    LPTSTR v1 = NULL;
    DWORD ReturnLength(0);
    DWORD LastError(0);

    // 创建子进程
    BOOL IsOk = CreateProcess(_T("D:\\Studying_Area\\Computer science and technology\\Programming code\\Reverse Engineering\\Windows Core Programming\\Chp4\\Basic operations for environment variables\\Debug\\SubProcess.exe")
        , NULL, NULL, NULL, FALSE, Flags, NULL, NULL, &StartupInfo, &ProcessInfo);
    // 命令行参数传递数据（父进程创建子进程）
    if (IsOk == NULL) {
        LastError = GetLastError();
        OutputDebugString(_T("子进程创建失败！\r\n"));
        goto Exit;
    }
    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);

   
    v1 = (LPTSTR)malloc(sizeof(TCHAR) * PAGE_SIZE);
    if (v1 == NULL) {
        goto Exit;
    }
    ReturnLength =  GetEnvironmentVariable(VARIABLE_NAME, v1, PAGE_SIZE);
    // 判断是否成功
    if (ReturnLength == 0) {
        LastError = GetLastError();
        IsOk = FALSE;
    }
    if (IsOk == TRUE) {
        // 删除环境变量名
        SetEnvironmentVariable(VARIABLE_NAME, NULL);
    }
    else {
        if (!SetEnvironmentVariable(VARIABLE_NAME, _T("昝博玮"))) /* WENKAI-Y = ABC*/ {
            OutputDebugString(_T("SetEnvironmentVariable() Error\r\n"));
            goto Exit;
        }
    }
    IsOk = CreateProcess(_T("D:\\Studying_Area\\Computer science and technology\\Programming code\\Reverse Engineering\\Windows Core Programming\\Chp4\\Basic operations for environment variables\\Debug\\SubProcess.exe")
        , NULL, NULL, NULL, FALSE, Flags, NULL, NULL,
        &StartupInfo, &ProcessInfo);
    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
    if (IsOk) {
        if (!SetEnvironmentVariable(VARIABLE_NAME, v1))
        {
            _tprintf(_T("SetEnvironmentVariable() Error\r\n"));
            goto Exit;
        }
    }
    else {
        SetEnvironmentVariable(VARIABLE_NAME, NULL);
    }

Exit:
    if (ProcessInfo.hProcess != NULL)
    {
        CloseHandle(ProcessInfo.hProcess);
    }
    if (ProcessInfo.hThread != NULL) {
        CloseHandle(ProcessInfo.hThread);
    }
    if (v1 != NULL)
    {
        free(v1);
        v1 = NULL;
    }
}