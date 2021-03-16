#include "Suspend or resume a thread.h"

int _tmain() {

    // data
    TCHAR* ProcessImageName = NULL;
    HANDLE ProcessHandle = INVALID_HANDLE_VALUE;
    // get the process imagename
    ProcessImageName = new TCHAR[MAX_PATH];
    if (!YekGetProcessImageName(ProcessImageName)) {
        _tprintf(_T("YekGetProcessImageName Fail.\r\n"));
        goto Exit;
    }
    // get the process identify by imagename
    if (!YekGetProcessIdentifyFromImageName(ProcessImageName, &ProcessHandle)) {
        _tprintf(_T("YekGetProcessIdentifyFromImageName Fail.\r\n"));
        goto Exit;
    }
    // suspend or resume
    if (!YeksuspendOrResumeThreads((DWORD)ProcessHandle, true)) {
        _tprintf(_T("YeksuspendOrResumeThreads Fail.\r\n"));
        goto Exit;
    }
    system("pause");
    YeksuspendOrResumeThreads((DWORD)ProcessHandle, false);
    // exit
Exit:
    if (ProcessImageName != NULL) {
        delete[]ProcessImageName;
        ProcessImageName = NULL;
    }
    
    return 0;
}

/* »ñÈ¡ImageName */
BOOL YekGetProcessImageName(OUT TCHAR* ProcessImageName, IN const UINT ImageNameLength) {
    if (ProcessImageName == NULL || ImageNameLength > MAX_PATH) {
        return FALSE;
    }
    UINT i = 0;
    TCHAR BufferData = _gettchar();
    while (BufferData != '\n' && i < ImageNameLength) {
        ProcessImageName[i++] = BufferData;
        BufferData = _gettchar();
    }
    ProcessImageName[i] = '\0';
    return TRUE;
}

/* get process identify */
BOOL YekGetProcessIdentifyFromImageName(IN const TCHAR* ImageName, OUT HANDLE* ProcessIdentify) {
    PROCESSENTRY32 ProcessEntry32 = { sizeof(PROCESSENTRY32) };
    HANDLE SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (SnapshotHandle == INVALID_HANDLE_VALUE) {
        _tprintf(_T("CreateToolhelp32Snapshot Fail.\r\n"));
        return FALSE;
    }
    BOOL Flage = FALSE;
    BOOL IsOk = Process32First(SnapshotHandle, &ProcessEntry32);
    while (IsOk) {
        if (_memicmp(ProcessEntry32.szExeFile, ImageName, _tcslen(ProcessEntry32.szExeFile) * sizeof(TCHAR)) == 0) {
            *ProcessIdentify = (HANDLE)ProcessEntry32.th32ParentProcessID;
            Flage = TRUE;
            break;
        }
        IsOk = Process32Next(SnapshotHandle, &ProcessEntry32);
    }
    if (SnapshotHandle == NULL) {
        _tprintf(_T("fffff\r\n"));
    }
    if (YekCloseHandle(SnapshotHandle)) {
        return Flage;
    }
    return FALSE;
}

/* suspend or resume threads */
BOOL YeksuspendOrResumeThreads(IN DWORD ProcessIdentify, IN bool Flage) {
    // get the list of threads in the system
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, ProcessIdentify);
    HANDLE hThread = INVALID_HANDLE_VALUE;
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        _tprintf(_T("CreateToolhelp32Snapshot Fail.\r\n"));
        return FALSE;
    }
    // walk the list of thread
    THREADENTRY32 ThreadEntry32 = { sizeof(THREADENTRY32) };
    BOOL IsOk = Thread32First(hSnapshot, &ThreadEntry32);
    while (IsOk) {
        // is this thread in the desired process?
        if (ThreadEntry32.th32OwnerProcessID == ProcessIdentify) {
            // attemp to convert the thread id into a handle
            hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, ThreadEntry32.th32ThreadID);
            if (hThread != NULL) {
                // suspend
                if (Flage) {
                    SuspendThread(hThread);
                    _tprintf(_T("SuspendThread:%d.\r\n"), (int)hThread);
                }
                // resume
                else {
                    ResumeThread(hThread);
                    _tprintf(_T("ResumeThread:%d.\r\n"), (int)hThread);
                }
            }
            YekCloseHandle(hThread);
        }
        IsOk = Thread32Next(hSnapshot, &ThreadEntry32);
    }
    YekCloseHandle(hSnapshot);
    return TRUE;
}

/* close the handle */
bool YekCloseHandle(HANDLE myHandle)
{
    DWORD HandleFlags;
    if (GetHandleInformation(myHandle, &HandleFlags) && (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
        return !!CloseHandle(myHandle);
    return false;
}