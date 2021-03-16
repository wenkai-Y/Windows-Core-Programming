#include "Process.h"

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {
    setlocale(LC_ALL, "Chinese-simplified");
    YekGreateProcess();
    _tprintf(_T("Waitting...\r\n"));
    _gettchar();
    return 0;
}

void YekGreateProcess() {

    STARTUPINFO StartupInfo;
    memset(&StartupInfo, 0, sizeof(STARTUPINFO));

    PROCESS_INFORMATION ProcessInfo;
    memset(&ProcessInfo, 0, sizeof(PROCESS_INFORMATION));
    BOOL IsOk = CreateProcess(_T("D:\\Studying_Area\\Computer science and technology\\Programming code\\Reverse Engineering\\Windows Core Programming\\Chp4\\Process sets current directory for SubProcess\\Debug\\SubProcess.exe"),
        NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, _T("D:"), &StartupInfo, &ProcessInfo);

    if (IsOk == FALSE)
    {
        return;
    }
    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
    CloseHandle(ProcessInfo.hProcess);
    CloseHandle(ProcessInfo.hThread);
    return;
}