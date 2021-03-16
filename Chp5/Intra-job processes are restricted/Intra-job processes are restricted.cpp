#include "Intra-job processes are restricted.h"

int _tmain() {

    // Data
    JOBOBJECT_BASIC_UI_RESTRICTIONS OperationProcess;   // 作业进程
    PROCESS_INFORMATION             ProcessInfo;        // 进程消息
    STARTUPINFO                     StartupInfo;
    TCHAR                           CommandLine[] = _T("Notepad.exe");
    BOOL                            IsOk          = FALSE;
    DWORD                           LastError     = 0;
    // 创建作业
    HANDLE JobHandle = CreateJobObject(NULL, NULL);
    if (JobHandle == NULL) {
        LastError = GetLastError();
        OutputDebugString(LPCWSTR(LastError));
        goto Exit;
    }
    // 限制作业行为
    OperationProcess.UIRestrictionsClass = JOB_OBJECT_UILIMIT_NONE;
    OperationProcess.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_WRITECLIPBOARD;
    OperationProcess.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_READCLIPBOARD;
    SetInformationJobObject(JobHandle, JobObjectBasicUIRestrictions,
        &OperationProcess, sizeof(JOBOBJECT_BASIC_UI_RESTRICTIONS));

    memset(&StartupInfo, 0, sizeof(STARTUPINFO));
    StartupInfo.cb = sizeof(STARTUPINFO);
    memset(&ProcessInfo, 0, sizeof(PROCESS_INFORMATION));

    IsOk = CreateProcess(NULL, CommandLine, NULL, NULL,
        FALSE, CREATE_SUSPENDED | CREATE_NEW_CONSOLE | CREATE_BREAKAWAY_FROM_JOB,
        NULL, NULL, &StartupInfo, &ProcessInfo);
    AssignProcessToJobObject(JobHandle, ProcessInfo.hProcess);
    ResumeThread(ProcessInfo.hThread);

    CloseHandle(ProcessInfo.hProcess);
    CloseHandle(ProcessInfo.hThread);

Exit:
    if (JobHandle != NULL) {
        CloseHandle(JobHandle);
        JobHandle = NULL;
    }
    _tprintf(_T("Input AnyKey To Exit.\r\n"));
    _gettchar();

    return 0;
}