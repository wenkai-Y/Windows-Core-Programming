#include "ProcessA.h"

int _tmain() {

    _tsetlocale(LC_ALL, _T("Chinese-simplified"));//ʹ��ǰ����֧������

    //Sub_1();
    //Sub_2();
    //Sub_3();
    Sub_4();
    _tprintf(_T("Input anykey to exit.\r\n"));
    _gettchar();
    _gettchar();
    return 0;
}


/* �����¼� */
void Sub_1() {

    DWORD LastError(-1);
    HANDLE EventHandle = CreateEvent(NULL, FALSE, FALSE, _T("Yuwenkai"));
    if (EventHandle == NULL) {
        LastError = GetLastError();
        goto Exit;
    }
    // wait for single object
    WaitForSingleObject(EventHandle, INFINITE);
    _tprintf(_T("Good for you!\r\n"));
Exit:
    if (EventHandle != NULL) {
        if (YekCloseHandle(EventHandle) == true) {
            EventHandle = INVALID_HANDLE_VALUE;
        }
        else {
            LastError = GetLastError();
        }
    }
}

/* ���������� */
void Sub_2() {
    DWORD LastError(-1);
    // ����һ�������������ں˶���
    HANDLE MutexHandle = CreateMutex(NULL, FALSE, _T("Yuwenkai"));
    if (MutexHandle == NULL) {
        LastError = GetLastError();
        goto Exit;
    }
    {
        _tprintf(_T("����ID��%d\r\n"), (int)GetCurrentProcess());
        _tprintf(_T("�����壺%d\r\n"), (int)MutexHandle);
        WaitForSingleObject(MutexHandle, INFINITE); // �������������++
        _tprintf(_T("�����ǵػ�\r\n"));
        TCHAR BufferData[0x1000] = { 0 };
        _tscanf(_T("%s"), BufferData);
        if (_tcscmp(BufferData, _T("���������")) == 0) {
            ReleaseMutex(MutexHandle);
        }
    }
Exit:
    if (MutexHandle != NULL) {
        if (YekCloseHandle(MutexHandle) == true) {
            MutexHandle = INVALID_HANDLE_VALUE;
        }
        else {
            LastError = GetLastError();
        }
    }
}

/* �����ź��� */
void Sub_3() {
    int LastError = -1;
    // �������������ŵ��źŵ�
    HANDLE SemaphoreHandle = CreateSemaphore(NULL, 0, 2, _T("Yuwenkai"));
    if (SemaphoreHandle == NULL) {
        LastError = GetLastError();
        goto Exit;
    }
    {
        TCHAR BufferData[0x1000] = { 0 };
        _tprintf(_T("���"));
        _tscanf(_T("%s"), BufferData);
        if (_tcscmp(BufferData, _T("���Ŀ�")) == 0) {
            // ������յ��
            ReleaseSemaphore(SemaphoreHandle, 2, NULL);

            Sleep(10);
        }
    }
Exit:
    if (SemaphoreHandle != NULL) {
        if (YekCloseHandle(SemaphoreHandle) == true) {
            SemaphoreHandle = INVALID_HANDLE_VALUE;
        }
        else {
            LastError = GetLastError();
        }
    }
}

/* ���������� */
void Sub_4() {
    DWORD LastError(-1);
    HANDLE MutexHandle = CreateMutex(NULL, FALSE, NULL);
    if (MutexHandle == NULL) {
        LastError = GetLastError();
        goto Exit;
    }

    _tprintf(_T("����ID: %d\r\n"), (int)GetCurrentProcessId());
    _tprintf(_T("������: %d\r\n"), (int)MutexHandle);
    _tprintf(_T("Please run ProcessB.exe!\r\n"));

    WaitForSingleObject(MutexHandle, INFINITE);
    {
        TCHAR BufferData[0x1000] = { 0 };
        _tprintf(_T("���"));
        _tscanf(_T("%s"), BufferData);
        if (_tcscmp(BufferData, _T("�ò���")) == 0) {
            ReleaseMutex(MutexHandle);
        }
        Sleep(1000);
    }
Exit:
    if (MutexHandle != NULL) {
        if (YekCloseHandle(MutexHandle) == true) {
            MutexHandle = INVALID_HANDLE_VALUE;
        }
        else {
            LastError = GetLastError();
        }
    }
}