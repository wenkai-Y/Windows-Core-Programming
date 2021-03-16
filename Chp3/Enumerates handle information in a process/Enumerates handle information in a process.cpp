#include "Enumerates handle information in a process.h"


// STL 

/* ���庯��ָ�� */
vector<HANDLE_INFORMATION> __HandleInfo;
/* ����ָ�� ��ȡ������ַ */
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



    /* ͨ������ProcessImageName���ProcessIdentify */
    HANDLE ProcessIdentify = NULL;
    if (YWKGetProcessIdentify(&ProcessIdentify, sizeof(ProcessIdentify), 
        ProcessImageName, sizeof(ProcessImageName)) == FALSE){
        /* Failed to get ProcessIdentify */
        LastError = GetLastError();
        goto Exit;

    }
    /* ö�پ����Ϣ */
    if (YWKEnumHandleInfo(__HandleInfo, ProcessIdentify) == FALSE) {
        /* Fail */
        OutputDebugString(_T("YWKEnumHandleInfo: fail to get __HandleInfo\r\n"));
        goto Exit;
    }
    
    /* Show Information */
    for (v1 = __HandleInfo.begin(); v1 != __HandleInfo.end(); v1++) {
        _tprintf(_T("���ֵ��%d\r\n"), v1->HandleValue);
        _tprintf(_T("����0x%p\r\n"), v1->Object);
        _tprintf(_T("�������������%u\r\n"), v1->ObjectType);
    }
Exit:
    // ���ٶ���
    vector<HANDLE_INFORMATION>().swap(__HandleInfo);
    return;
}



// ��ý���ID
BOOL YWKGetProcessIdentify(_Out_ HANDLE* ProcessIdentify, _In_ ULONG_PTR ProcessIdentifyLength,
    _In_ const TCHAR* ProcessImageName, _In_ ULONG_PTR ProcessImageNameLength) {

    // ��ý���ID�ķ���
    // .exe
    // ProcessIdentify
    // ProcrssHandle -> _Object_Header EProcess(Body[ObjectTable])

    // ���� CreateToolhelp32Snapshot
    BOOL IsOk = FALSE;
    HANDLE SnapshotHandle = INVALID_HANDLE_VALUE;
    PROCESSENTRY32 ProcessEntry32;
    ProcessEntry32.dwSize = sizeof(PROCESSENTRY32);
    DWORD LastError(0);

    /* Determine if ProcessIdentify is empty */
    // �ڴ�������
    // IsBadWritePtr �쳣����д���

    if (IsBadWritePtr(ProcessIdentify, ProcessIdentifyLength) ||
        IsBadReadPtr(ProcessImageName, ProcessImageNameLength)) {
        LastError = ERROR_INVALID_PARAMETER;
        goto Exit;
    }

    // ��������
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

// ͨ������ID����ȡ�����Ϣ
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

    /* ���Ntdll��ģ���ַ */
    ModuleHandle = GetModuleHandle(_T("Ntdll.dll"));
    if (ModuleHandle == NULL) {
        OutputDebugString(_T("GetModuleHandle: Failed to get ModuleHandle\r\n"));
        goto Exit;
    }
    /* ��Ntdllģ��ĵ������л�ȡ��غ�����ַ */
    /* GetProcAddress����ֻ�е��� */
    if (__NtQuerySystemInformation == NULL) {
        __NtQuerySystemInformation = (LPFN_NTQUERYSYSTEMINFORMATION)GetProcAddress(ModuleHandle, "NtQuerySystemInformation");
    }
    if (__NtQuerySystemInformation == NULL) {
        OutputDebugString(_T("GetProcAddress: Failed to get __NtQuerySystemInformation\r\n"));
        LastError = GetLastError();
        goto Exit;
    }

    /* ����0x4000�ڴ� */
    SystemHandleInfo = (_OBJECT_HELPER_::PSYSTEM_HANDLE_INFORMATION)malloc(BufferLength);

    /* ö�����������Ϣ */
    while (TRUE) {

        Status = __NtQuerySystemInformation
        ((SYSTEM_INFORMATION_CLASS)_OBJECT_HELPER_::SystemHandleInformation,
            SystemHandleInfo,
            BufferLength,
            &ReturnLength);
        /* ���û�гɹ� */
        if (Status != STATUS_SUCCESS) {
            /* �ڴ治�� */
            if (Status == STATUS_INFO_LENGTH_MISMATCH) {
                /* ���������ڴ� */
                free(SystemHandleInfo);
                BufferLength *= 2;
                SystemHandleInfo = (_OBJECT_HELPER_::PSYSTEM_HANDLE_INFORMATION)malloc(BufferLength);
                /* ����ʧ�� */
                if (!SystemHandleInfo) {
                    OutputDebugString(_T("malloc�� fail to create SystemHandleInfo\r\n"));
                    goto Exit;
                }
            }
            else {
                goto Exit;
            }
        }
        /* ����ɹ� */
        else {
            break;
        }
    }
    /* �����Ϣ */
    if (SystemHandleInfo) {
        for (int i = 0; i < (int)SystemHandleInfo->NumberOfHandles; ++i) {
            /* �ж� */
            if (SystemHandleInfo->Handles[i].UniqueProcessId == (USHORT)ProcessIdentify) {
                HandleInfo.HandleValue = SystemHandleInfo->Handles[i].HandleValue;      // ���ֵ
                HandleInfo.Object      = SystemHandleInfo->Handles[i].Object;           // �������
                HandleInfo.ObjectType  = SystemHandleInfo->Handles[i].ObjectTypeIndex;  // �����������
                // ����vector
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