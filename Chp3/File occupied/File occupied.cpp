#include <windows.h>
#include <iostream>
#include <tchar.h>
using namespace std;

BOOL __EnableDebugPrivilege = TRUE;
DWORD wkEnableSeDebugPrivilege(HANDLE ProcessHandle, BOOL IsEnable);
BOOL wkCloseHandle(HANDLE HandleValue);
HANDLE wkOpenProcess(
    DWORD dwDesiredAccess,  // access flag
    BOOL bInheritHandle,    // handle inheritance option
    DWORD dwProcessId       // process identifier
);


int _tmain() {

    HANDLE ProcessHandle = INVALID_HANDLE_VALUE;

    // 打开一个进程
    int process(0);
    cin >> process;
    ProcessHandle = wkOpenProcess(PROCESS_DUP_HANDLE, NULL, (DWORD)process); // explore.exe进程
    if (ProcessHandle == NULL) {
        if (ProcessHandle != NULL) {
            wkCloseHandle(ProcessHandle);
            ProcessHandle = NULL;
        }
        return -1;
    }

    HANDLE FileHandle = INVALID_HANDLE_VALUE;
    // 打开xxx文件（文件对象）
    FileHandle = CreateFile(_T("Readme.txt"), GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (FileHandle == INVALID_HANDLE_VALUE) {
        if (ProcessHandle != NULL) {
            wkCloseHandle(ProcessHandle);
            ProcessHandle = NULL;
        }
        return -1;
    }

    HANDLE v1 = NULL;
    // 拷贝当前进程的句柄到目标进程中
    BOOL IsOk = DuplicateHandle(GetCurrentProcess(), FileHandle, ProcessHandle, &v1, 0,
        FALSE, DUPLICATE_SAME_ACCESS);
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        wkCloseHandle(FileHandle);
        FileHandle = INVALID_HANDLE_VALUE;
    }

    return 0;
}

HANDLE wkOpenProcess(
    DWORD dwDesiredAccess,  // access flag
    BOOL bInheritHandle,    // handle inheritance option
    DWORD dwProcessId       // process identifier
) {

    // 提高权限
    if (__EnableDebugPrivilege) {
        wkEnableSeDebugPrivilege(GetCurrentProcess(), TRUE);
    }

    HANDLE ProcessHandle = OpenProcess(dwDesiredAccess, bInheritHandle, (DWORD)dwProcessId);

    DWORD LastError = GetLastError();
    if (__EnableDebugPrivilege)
    {
        wkEnableSeDebugPrivilege(GetCurrentProcess(), FALSE);
    }
    SetLastError(LastError);
    return ProcessHandle;
}

DWORD wkEnableSeDebugPrivilege(HANDLE ProcessHandle, BOOL IsEnable) {
    DWORD  LastError;
    HANDLE TokenHandle = 0;

    if (!OpenProcessToken(ProcessHandle, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &TokenHandle))
    {
        LastError = GetLastError();
        if (TokenHandle)
            wkCloseHandle(TokenHandle);
        return LastError;
    }
    TOKEN_PRIVILEGES TokenPrivileges;
    memset(&TokenPrivileges, 0, sizeof(TOKEN_PRIVILEGES));
    LUID v1;
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &v1))
    {
        LastError = GetLastError();
        wkCloseHandle(TokenHandle);
        return LastError;
    }
    TokenPrivileges.PrivilegeCount = 1;
    TokenPrivileges.Privileges[0].Luid = v1;
    if (IsEnable)
        TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        TokenPrivileges.Privileges[0].Attributes = 0;
    AdjustTokenPrivileges(TokenHandle, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
    LastError = GetLastError();
    wkCloseHandle(TokenHandle);
    return LastError;
}

BOOL wkCloseHandle(HANDLE HandleValue) {
    DWORD HandleFlags;
    if (GetHandleInformation(HandleValue, &HandleFlags)
        && (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
        return !!CloseHandle(HandleValue);
    return FALSE;
}