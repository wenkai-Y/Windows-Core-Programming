#include "HOOK instruction pointer register.h"

DWORD gLASTERROR = 0;
LPFN_NTQUERYSYSTEMINFORMATION __NtQuerySystemInformation = NULL;
vector<THREAD_INFO> __ThreadInfo;
THREAD_ENTRY Thread_Entry;
int _tmain() {

    // Initialization Data
    HMODULE ModuleHandle    = NULL;
    TCHAR* ImageName        = NULL;
    HANDLE ProcessIdentify  = INVALID_HANDLE_VALUE;

    // Load module into this process
    ModuleHandle = LoadLibrary(_T("Dll.dll"));
    if (ModuleHandle == NULL) {
        gLASTERROR = GetLastError();
        _tprintf(_T("LoadLibrary Fail: %d\r\n"), gLASTERROR);
        goto FREE;
    }
    _tprintf(_T("The DLL has been loaded.\r\n"));

    // get the ImageName
    ImageName = new TCHAR[MAX_PATH];
    _tprintf(_T("Please enter the image name: "));
    YekInutData(ImageName);

    // Get the Identify from the ImageName
    if (!YekGetIdentifyFromImageName(ImageName, &ProcessIdentify)) {
        _tprintf(_T("YekGetIdentifyFromImageName Fail.\r\n"));
        goto FREE;
    }
    _tprintf(_T("%s : %d\r\n"), ImageName, (int)ProcessIdentify);

    YekGetMainThreadIdentify(&ProcessIdentify);
    for (int i = __ThreadInfo.size() - 1; i >= 0; --i) {
        _tprintf(_T("%d\r\n"), (int)__ThreadInfo[i].ThreadIdentify);
    }


    // Free
FREE:
    if (ImageName != NULL) {
        delete[]ImageName;
        ImageName = NULL;
    }
    if (ModuleHandle != NULL) {
        if (!FreeLibrary(ModuleHandle)) {
            gLASTERROR = GetLastError();
            _tprintf(_T("FreeLibrary Fail: %d\r\n"), gLASTERROR);
        }
        else {
            _tprintf(_T("The DLL has been uninstalled.\r\n"));
            ModuleHandle = NULL;
        }
        
    }
   

    system("pause");
    return 0;
}

/* Hook指令指针寄存器 */
VOID HOOKInstructionPointerRegister() {

}

/* 获取ImageName */
inline BOOL YekInutData(TCHAR* ProcessImageName, ULONG_PTR ProcessImageNameLength) {
    if (ProcessImageName == NULL || ProcessImageNameLength > MAX_PATH) {
        return FALSE;
    }
    UINT i = 0;
    TCHAR BufferData = _gettchar();
    while (BufferData != '\n' && i < ProcessImageNameLength) {
        ProcessImageName[i++] = BufferData;
        BufferData = _gettchar();
    }
    ProcessImageName[i] = '\0';
    return TRUE;
}

/* 获取进程Identify */
BOOL YekGetIdentifyFromImageName(const TCHAR* ImageName, HANDLE* ProcessIdentify) {
    PROCESSENTRY32 ProcessEntry32 = { 0 };
    ProcessEntry32.dwSize = sizeof(PROCESSENTRY32);
    HANDLE SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    BOOL Flage = FALSE;
    if (SnapshotHandle == INVALID_HANDLE_VALUE)
        return FALSE;
    BOOL IsOk = Process32First(SnapshotHandle, &ProcessEntry32);
    while (IsOk)
    {
        if (_memicmp(ProcessEntry32.szExeFile, ImageName, _tcslen(ProcessEntry32.szExeFile)*sizeof(TCHAR)) == 0) {
            *ProcessIdentify = (HANDLE)ProcessEntry32.th32ProcessID;
            Flage = TRUE;
            break;
        }
        IsOk = Process32Next(SnapshotHandle, &ProcessEntry32);
    }
    if (SnapshotHandle == NULL)
        return FALSE;
    YekCloseHandle(SnapshotHandle);
    return Flage;
}

/* 获得主线程Identify */
BOOL YekGetMainThreadIdentify(const HANDLE* ProcessIdentify) {
    int ThreadCount = 0;
    DWORD LastError = 0;
    HANDLE SnapshotHandle = INVALID_HANDLE_VALUE;
    THREADENTRY32 ThreadEntry;

    //memset(&ThreadEntry, 0, sizeof(THREADENTRY32));
    SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    // Returns an open handle to the specified snapshot if successful or INVALID_HANDLE_VALUE otherwise. 
    if (SnapshotHandle == INVALID_HANDLE_VALUE) {
        LastError = GetLastError();
        _tprintf(_T("CreateToolhelp32Snapshot Fail: %d.\r\n"), (int)LastError);
        throw _tstring(_T("CreateToolhelp32Snapshot Fail.\r\n"));
    }
    // check that this current peocess is enumerated
    ThreadEntry.dwSize = sizeof(ThreadEntry);
    if (Thread32First(SnapshotHandle, &ThreadEntry)) {
        do {
            if (ThreadEntry.th32OwnerProcessID == (DWORD)ProcessIdentify) {
                // get the thread information
                Thread_Entry.ThreadIdentify = (HANDLE)ThreadEntry.th32ThreadID;
                __ThreadEntry.push_back(Thread_Entry);
                ++ThreadCount;
            }
        } while (Thread32Next(SnapshotHandle, &ThreadEntry));
    }
    // To destroy the snapshot, use the CloseHandle function.
    if (SnapshotHandle != INVALID_HANDLE_VALUE && SnapshotHandle != NULL) {
        YekCloseHandle(SnapshotHandle);
    }
    else {
        throw _tstring(_T("Process snapshot missing.\r\n"));
    }
}

/* 关闭句柄 */
bool YekCloseHandle(HANDLE myHandle)
{
    DWORD HandleFlags;
    if (GetHandleInformation(myHandle, &HandleFlags) && (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
        return !!CloseHandle(myHandle);
    return false;
}