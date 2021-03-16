#include "ProcessA.h"



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {

    setlocale(LC_ALL, "chs");
    _tprintf(_T("This is process A.\r\n"));
    Sub_1();     //��������
    _tprintf(_T("Input AnyKey To Exit\r\n"));
    _gettchar();
    return 0;
}

void Sub_1() {

    DWORD       LastError      = NULL;
    ULONG_PTR   VirtualAddress = NULL;
    HANDLE      MappingHandle  = INVALID_HANDLE_VALUE;

    // mapping  ӳ��
    // image    ����
    // pageSize 0x1000 4096byte

    /* �����ڴ�ӳ�� */
    if (YWKCreateMemroyMappingEx(ACCESS_WRITE, 0, 0x1000, _T("Yuwenkai"), 
        &MappingHandle, &VirtualAddress) == FALSE) {
        LastError = GetLastError();
    }
    else {
        __try {
            memcpy((LPVOID)VirtualAddress, _T("you are so beatuiful"),
                (sizeof(TCHAR) * _tcslen(_T("you are so beatuiful")) + 1));
            if (VirtualAddress != NULL) {
                _tprintf(_T("%s\r\n"), (TCHAR*)VirtualAddress);
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            OutputDebugString(_T("VirtualAddress assignment failed.\r\n"));
            LastError = GetExceptionCode();
        }
        _tprintf(_T("Input anykey to continue.\r\n"));
        _gettchar();
        if (VirtualAddress != NULL) {
            _tprintf(_T("%s\r\n"), (TCHAR*)VirtualAddress);
        }
    }
    /* ������Դ */
    YWKUnmapMemoryEx(MappingHandle, VirtualAddress);
    return;

}

// size_t
// [high][low]

BOOL YWKCreateMemroyMappingEx
(_In_ const OPERATION ReadOrWrite, _In_ const DWORD MaximumSizeHigh,
    _In_ const DWORD MaximumSizeLow, _In_ const LPCTSTR ObjectName,
    _Out_ LPHANDLE MappingHandle, _Out_ ULONG_PTR* VirtualAddress) {

    /* data */
    DWORD Protect       = 0;
    DWORD LastError     = 0;
    DWORD DesiredAccess = 0;

    /*handle */
    HANDLE v1 = INVALID_HANDLE_VALUE;
    HANDLE v2 = INVALID_HANDLE_VALUE;

    /* �жϣ��������� */
    if (MappingHandle == NULL || VirtualAddress == NULL) {
        OutputDebugString(_T("YWKCreateMemroyMappingEx: MappingHandle or VirtualAddress is NULL.\r\n"));
        LastError = ERROR_INVALID_PARAMETER;
        goto Exit;
    }

    /* �жϲ��� */
    switch (ReadOrWrite) {
    case ACCESS_WRITE:  /* д���� */
        Protect       = PAGE_READWRITE;
        DesiredAccess = SECTION_MAP_READ | SECTION_MAP_WRITE;
        break;
    case ACCESS_READ:   /* ������ */
        Protect       = PAGE_READONLY;
        DesiredAccess = SECTION_MAP_READ;
        break;
    default:
        OutputDebugString(_T("YWKCreateMemroyMappingEx: OPERATION is error.\r\n"));
        LastError = ERROR_INVALID_PARAMETER;
        goto Exit;
        break;
    }

    /* ����һ�������ڴ���� */
    __try {
        v1 = CreateFileMapping
        (INVALID_HANDLE_VALUE, NULL, Protect, MaximumSizeHigh, MaximumSizeLow, ObjectName);

        /* ���سɹ� */
        if (v1 != NULL && v1 != INVALID_HANDLE_VALUE) {
            *MappingHandle = v1;
            /* ͨ��������ӳ��������ڴ� */
            v2 = MapViewOfFile(v1, DesiredAccess, 0, 0, 0);
            /* ���سɹ� */
            if (v2 != NULL) {
                (*VirtualAddress) = (ULONG_PTR)v2;
                return TRUE;
            }
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER){
        LastError = GetExceptionCode();
        OutputDebugString(_T("CreateFileMapping fail.\r\n"));
        goto Exit;
    }
Exit:
    SetLastError(LastError);
    return FALSE;
}

void YWKUnmapMemoryEx(_In_ HANDLE MappingHandle, _In_ ULONG_PTR VirtualAddress) {

    __try
    {
        if (UnmapViewOfFile((void*)VirtualAddress))
        {
            if (YWKCloseHandle(MappingHandle)) {
                OutputDebugString(_T("Close Handle Success.\r\n"));
            }
        }

    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        SetLastError(GetExceptionCode());
    }

}

BOOL YWKCloseHandle(_In_ const HANDLE HandleValue) {
    DWORD HandleFlags(-1);
    if (GetHandleInformation(HandleValue, &HandleFlags)
        && (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
        return !!CloseHandle(HandleValue);
    return false;
}