#include "Manually raise process permissions.h"

int _tmain() {
    setlocale(LC_ALL, "Chinese-simplified");
    Sub_1();
    _tprintf(_T("Input AnyKey To Exit\r\n"));
    _gettchar();
    return 0;
}

void Sub_1() {
    SHELLEXECUTEINFO ShellExecuteInfo;
    memset(&ShellExecuteInfo, 0, sizeof(SHELLEXECUTEINFO));
    ShellExecuteInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShellExecuteInfo.lpVerb = _T("runas");
    ShellExecuteInfo.lpFile = _T("Cmd.exe");
    if (!ShellExecuteEx(&ShellExecuteInfo)) {
        _tprintf(_T("ShellExecuteEx()  Error\r\n"));
    }
    STARTUPINFO StartupInfo;
    memset(&StartupInfo, 0, sizeof(STARTUPINFO));
    StartupInfo.cb = sizeof(STARTUPINFO);
    PROCESS_INFORMATION ProcessInfo;
    memset(&ProcessInfo, 0, sizeof(PROCESS_INFORMATION));
    TCHAR v1[] = _T("Cmd");
    CreateProcess(NULL, v1,
        NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &StartupInfo, &ProcessInfo);
    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
    CloseHandle(ProcessInfo.hThread);
    CloseHandle(ProcessInfo.hProcess);
    return;
}

void Sub_2() {

}