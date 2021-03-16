#include "Enumerates handle information in a process.h"


// STL 

/* 定义函数指针 */
vector<HANDLE_INFORMATION> __HandleInfo;
/* 函数指针 获取函数地址 */
LPFN_NTQUERYSYSTEMINFORMATION  __NtQuerySystemInformation = NULL;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {

    _tsetlocale(LC_ALL, _T("Chinese-simplified"));
    setlocale(LC_ALL, "chs");
    Sub_1();
    _tprintf(_T("Input AnyKey to Exit\r\n"));
    _gettchar();
    return 0;
}


void Sub_1() {

    vector<HANDLE_INFORMATION>::iterator v1;
    vector<HANDLE> ThreadIdentify{};
    TCHAR           ProcessImageName[MAX_PATH] = { 0 };
    DWORD           LastError = 0;
    int             i = 0;

    TCHAR v2 = _gettchar();
    while (v2 != '\n'){
        ProcessImageName[i++] = v2;
        v2 = _gettchar();
    }
    _tprintf(_T("ProcessImageName: %s\r\n"), ProcessImageName);



    /* 通过进程ProcessImageName获得ProcessIdentify */
    HANDLE ProcessIdentify = NULL;
    if (YWKGetProcessIdentify(&ProcessIdentify, sizeof(ProcessIdentify), 
        ProcessImageName, sizeof(ProcessImageName)) == FALSE){
        /* Failed to get ProcessIdentify */
        LastError = GetLastError();
        goto Exit;

    }
    /* 枚举句柄信息 */
    if (YWKEnumHandleInfo(__HandleInfo, ProcessIdentify) == FALSE) {
        /* Fail */
        OutputDebugString(_T("YWKEnumHandleInfo: fail to get __HandleInfo\r\n"));
        goto Exit;
    }
    
    /* Show Information */
    for (v1 = __HandleInfo.begin(); v1 != __HandleInfo.end(); v1++) {
        _tprintf(_T("句柄值：%d\r\n"), v1->HandleValue);
        _tprintf(_T("对象：0x%p\r\n"), v1->Object);
        _tprintf(_T("句柄类型索引：%u\r\n"), v1->ObjectType);
    }
Exit:
    // 销毁对象
    vector<HANDLE_INFORMATION>().swap(__HandleInfo);
    return;
}



// 获得进程ID
BOOL YWKGetProcessIdentify(_Out_ HANDLE* ProcessIdentify, _In_ ULONG_PTR ProcessIdentifyLength,
    _In_ const TCHAR* ProcessImageName, _In_ ULONG_PTR ProcessImageNameLength) {

    // 获得进程ID的方法
    // .exe
    // ProcessIdentify
    // ProcrssHandle -> _Object_Header EProcess(Body[ObjectTable])

    // 快照 CreateToolhelp32Snapshot
    BOOL IsOk = FALSE;
    HANDLE SnapshotHandle = INVALID_HANDLE_VALUE;
    PROCESSENTRY32 ProcessEntry32;
    ProcessEntry32.dwSize = sizeof(PROCESSENTRY32);
    DWORD LastError(0);

    /* Determine if ProcessIdentify is empty */
    // 内存管理代码
    // IsBadWritePtr 异常处理，写或读

    if (IsBadWritePtr(ProcessIdentify, ProcessIdentifyLength) ||
        IsBadReadPtr(ProcessImageName, ProcessImageNameLength)) {
        LastError = ERROR_INVALID_PARAMETER;
        goto Exit;
    }

    // 创建快照
    SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (SnapshotHandle == INVALID_HANDLE_VALUE) {
        OutputDebugString(_T("CreateToolhelp32Snapshot: Snapshothandle creation failed.\r\n"));
        LastError = GetLastError();
        goto Exit;
    }
    if (!Process32First(SnapshotHandle, &ProcessEntry32)) {
        OutputDebugString(_T("Process32First: Snapshothandle creation failed.\r\n"));
        LastError = GetLastError();
        goto Exit;
    }
    /* Get the Identiy from the imageName */
    do {
        if (_tcsicmp(ProcessEntry32.szExeFile, ProcessImageName) == 0) {
            *ProcessIdentify = (HANDLE)ProcessEntry32.th32ProcessID;
            IsOk = TRUE;
            goto Exit;
        }
        /* Take the next snapshot */
    } while (Process32Next(SnapshotHandle, &ProcessEntry32));
    /* The specified module could not be found. */
    LastError = ERROR_MOD_NOT_FOUND;
Exit:
    if (SnapshotHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(SnapshotHandle);
    }
    SnapshotHandle = INVALID_HANDLE_VALUE;
    SetLastError(LastError);
    return IsOk;
}

// 通过进程ID来获取句柄信息
BOOL YWKEnumHandleInfo(
    _Out_ vector<HANDLE_INFORMATION>& HandleInfoVector, _In_ HANDLE ProcessIdentify
) {

    _OBJECT_HELPER_::PSYSTEM_HANDLE_INFORMATION 
                       SystemHandleInfo = NULL;
    HANDLE_INFORMATION HandleInfo = { 0 };
    ULONG              BufferLength = 0x4000;   //0x1000 MemoryPage
    ULONG              ReturnLength = 0;
    DWORD              LastError = 0;
    HMODULE            ModuleHandle = NULL;
    NTSTATUS           Status;

    /* 获得Ntdll的模块地址 */
    ModuleHandle = GetModuleHandle(_T("Ntdll.dll"));
    if (ModuleHandle == NULL) {
        OutputDebugString(_T("GetModuleHandle: Failed to get ModuleHandle\r\n"));
        goto Exit;
    }
    /* 从Ntdll模块的导出表中获取相关函数地址 */
    /* GetProcAddress函数只有单字 */
    if (__NtQuerySystemInformation == NULL) {
        __NtQuerySystemInformation = (LPFN_NTQUERYSYSTEMINFORMATION)GetProcAddress(ModuleHandle, "NtQuerySystemInformation");
    }
    if (__NtQuerySystemInformation == NULL) {
        OutputDebugString(_T("GetProcAddress: Failed to get __NtQuerySystemInformation\r\n"));
        LastError = GetLastError();
        goto Exit;
    }

    /* 申请0x4000内存 */
    SystemHandleInfo = (_OBJECT_HELPER_::PSYSTEM_HANDLE_INFORMATION)malloc(BufferLength);

    /* 枚举搜索句柄信息 */
    while (TRUE) {

        Status = __NtQuerySystemInformation
        ((SYSTEM_INFORMATION_CLASS)_OBJECT_HELPER_::SystemHandleInformation,
            SystemHandleInfo,
            BufferLength,
            &ReturnLength);
        /* 如果没有成功 */
        if (Status != STATUS_SUCCESS) {
            /* 内存不够 */
            if (Status == STATUS_INFO_LENGTH_MISMATCH) {
                /* 重新申请内存 */
                free(SystemHandleInfo);
                BufferLength *= 2;
                SystemHandleInfo = (_OBJECT_HELPER_::PSYSTEM_HANDLE_INFORMATION)malloc(BufferLength);
                /* 申请失败 */
                if (!SystemHandleInfo) {
                    OutputDebugString(_T("malloc； fail to create SystemHandleInfo\r\n"));
                    goto Exit;
                }
            }
            else {
                goto Exit;
            }
        }
        /* 如果成功 */
        else {
            break;
        }
    }
    /* 句柄信息 */
    if (SystemHandleInfo) {
        for (int i = 0; i < (int)SystemHandleInfo->NumberOfHandles; ++i) {
            /* 判断 */
            if (SystemHandleInfo->Handles[i].UniqueProcessId == (USHORT)ProcessIdentify) {
                HandleInfo.HandleValue = SystemHandleInfo->Handles[i].HandleValue;      // 句柄值
                HandleInfo.Object      = SystemHandleInfo->Handles[i].Object;           // 句柄对象
                HandleInfo.ObjectType  = SystemHandleInfo->Handles[i].ObjectTypeIndex;  // 句柄对象索引
                // 添入vector
                HandleInfoVector.push_back(HandleInfo);
            }
        }
    }


Exit:
    if (SystemHandleInfo != NULL) {
        free(SystemHandleInfo);
        SystemHandleInfo = NULL;
    }
    return (HandleInfoVector.size() > 0);
}