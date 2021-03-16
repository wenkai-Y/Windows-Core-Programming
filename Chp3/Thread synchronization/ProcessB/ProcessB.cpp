#include "ProcessB.h"

int _tmain() {

    _tsetlocale(LC_ALL, _T("Chinese-simplified"));//ʹ��ǰ����֧������
    //Sub_1();
    //Sub_2();
    //Sub_3();
    Sub_4();

    _tprintf(_T("Input anykey to exit.\r\n"));
    _gettchar();
    return 0;
}



/* �����¼� */
void Sub_1() {

    DWORD LastError(-1);
    HANDLE EventHandle = OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("Yuwenkai"));
    if (EventHandle == NULL) {
        LastError = GetLastError();
        goto Exit;
    }
    _tprintf(_T("Good for me.\r\n"));
    SetEvent(EventHandle);  // �����¼��ں˶���

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

    DWORD IsOk(-1);
    // ��ռ������
    HANDLE MutexHandle = OpenMutex(MUTEX_ALL_ACCESS, FALSE, _T("Yuwenkai"));
    if (MutexHandle != NULL) {
        IsOk = WaitForSingleObject(MutexHandle, 10000L);
    }
    switch (IsOk)
    {
    case WAIT_FAILED: {
        DWORD LastError = GetLastError();
        _tprintf(_T("Error number: %d\r\n"), (int)LastError);
        return;
    }
    case WAIT_OBJECT_0:
        __try {
            _tprintf(_T("�ò���\r\n"));
        }
        __finally {
            // Release ownership of the mutex object
            if (!ReleaseMutex(MutexHandle)) {
                // deal with error
                _tprintf(_T("�ͷŴ���\r\n"));
            }
            //break;
        }
        break;
    // cannot get mutex ownership due to time-out
    case WAIT_TIMEOUT:{
        _tprintf(_T("����ʱ\r\n"));
        return;
        }
    default: {
        _tprintf(_T("δ֪����\r\n"));
        break;
    }
    }
    if (MutexHandle != NULL) {
        if (YekCloseHandle(MutexHandle) == true) {
            MutexHandle = INVALID_HANDLE_VALUE;
        }
    }
}

/* �����ź��� */
void Sub_3() {
    HANDLE SemaphoreHandle = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("Yuwenkai"));
    if (SemaphoreHandle == NULL) {
        goto Exit;
    }

    // �ȴ�һ�����ŵ��źŵ��е��ź�
    WaitForSingleObject(SemaphoreHandle, INFINITE);
    _tprintf(_T("�ò���\r\n"));

Exit:
    if (SemaphoreHandle != NULL) {
        if (YekCloseHandle(SemaphoreHandle) == true) {
            SemaphoreHandle = INVALID_HANDLE_VALUE;
        }
    }
}

/* ���������� */
void Sub_4() {
    HANDLE ProcessIdentify = 0;
    HANDLE v1 = 0;
    HANDLE MutexHandle = NULL;
    _tprintf(_T("Input Process Identify:\r\n"));
    _tscanf(_T("%d"), &ProcessIdentify);       //��������


    HANDLE ProcessHandle = YekOpenProcess(PROCESS_DUP_HANDLE, FALSE, ProcessIdentify);

    if (ProcessHandle == NULL)
    {
        goto Exit;
    }

    _tprintf(_T("Input Handle Value:\r\n"));   //��������
    _tscanf(_T("%d"), &v1);
    {
        BOOL IsOk = DuplicateHandle(ProcessHandle, v1, GetCurrentProcess(), &MutexHandle, 0,
            FALSE, DUPLICATE_SAME_ACCESS);

        if (MutexHandle == NULL)
        {
            int LastError = GetLastError();
            goto Exit;

        }
        IsOk = WaitForSingleObject(MutexHandle, INFINITE);
        switch (IsOk)
        {
        case WAIT_OBJECT_0:
        {
            _tprintf(_T("�ҽ�����,��������\r\n"));
            break;
        }
        case WAIT_ABANDONED:
        {
            _tprintf(_T("�����屻����\r\n"));
            break;
        }
        }
    }
Exit:
    if (ProcessHandle != NULL)
    {
        if (YekCloseHandle(ProcessHandle) == TRUE)
        {
            ProcessHandle = NULL;
        }
    }
    if (MutexHandle != NULL)
    {
        if (YekCloseHandle(MutexHandle) == TRUE)
        {
            MutexHandle = NULL;
        }
    }
}