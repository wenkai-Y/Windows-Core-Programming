#include "Process.h"

// ���̶���
// �¼�����
// ���̼�ͨ�ţ�IPC, InterProcess Communication����ָ�ڲ�ͬ����֮�䴫���򽻻���Ϣ
// Handle Inherit




// �������
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {
    

    Sub_1();   // �����в���
    Sub_2();   // �������� 
    
    
    return 0;
}


void Sub_1()   // �����в���
{
    // �����ӽ�����Ҫ�Ĳ���
    STARTUPINFO StartupInfo = { 0 };    //����һ���ӽ���   CreateProcess
    StartupInfo.cb = sizeof(STARTUPINFO);
    PROCESS_INFORMATION ProcessInfo = { 0 };

    // ����һ����ȫ�����ṹ�岢�������������̳�
    SECURITY_ATTRIBUTES SecurityAttributes;
    SecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    SecurityAttributes.bInheritHandle = TRUE;   //�������̳�
    SecurityAttributes.lpSecurityDescriptor = NULL;

    // ����һ���¼��������̳У�
    // �¼��ں˶��������
    HANDLE EventHandle = CreateEvent(&SecurityAttributes, TRUE, FALSE, NULL);// ����һ���¼����ں˶���
    if (EventHandle == NULL) {
        // �Ժ�ʹ�õ��������
        OutputDebugString(_T("�¼�����ʧ��\r\n"));
        goto Exit;
    }
    else {

        // ��ʽ�����
        // ˵���¼������ɹ�������ӡ
        TCHAR BufferData[0x1000] = { 0 };
        _stprintf(BufferData, _T("EventCreate:%d\r\n"), (int)EventHandle);
        _tprintf(_T("EventHandle:%d\r\n"), (int)EventHandle);
        OutputDebugString(BufferData);

        // �����¼��ľ��ֵ���ݵ�Ŀ������ӽ���
        // �����ֵ��ʽ�����ַ�����
        memset(BufferData, 0, sizeof(BufferData));
        _stprintf(BufferData, _T("%d"), (int)EventHandle);  // ��ʽ�����ַ���
     /*   OutputDebugString(BufferData);*/

        // ����һ���ӽ��̲�����ǰ�����е�һЩ�����������в�������ʽ���ݵ��ӽ���
        BOOL IsOk = CreateProcess(_T("D:\\Studying_Area\\Computer science and technology\\Programming code\\Reverse Engineering\\Windows Core Programming\\Chp3\\Handle to the parent-child process\\Debug\\SubProcess.exe"),
            (LPTSTR)BufferData, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL,
            &StartupInfo, &ProcessInfo);

        // �����в����������ݣ������̽��ӽ��̣�
        if (IsOk == FALSE) {
            _tprintf(_T("�ӽ��̴���ʧ��\r\n"));
            OutputDebugString(_T("�ӽ��̴���ʧ��\r\n"));
            goto Exit;
        }
        // ��Ҫ��ռ���ӽ�����Դ
        if (ProcessInfo.hProcess != NULL) {
            CloseHandle(ProcessInfo.hProcess);
        }
        if (ProcessInfo.hThread != NULL) {
            CloseHandle(ProcessInfo.hThread);
        }
        // �ȴ��¼�����
        _tprintf(_T("�ȴ��ӽ����¼�֪ͨ\r\n"));
        WaitForSingleObject(EventHandle, INFINITE);
        MessageBox(NULL, _T("�ӽ��̴����¼����̳гɹ�"), _T("Success"), 1);

        // ��������˳�
        _tprintf(_T("Inputer Anykey To Exit\r\n"));
        _gettchar();
    }
Exit:
    if (EventHandle != NULL) {
        CloseHandle(EventHandle);
        EventHandle = NULL;
    }
}

void Sub_2()    // ��������
{
    DWORD CreateFlags = 0;
    TCHAR BufferData[MAX_PATH] = { 0 };
    STARTUPINFO StartupInfo = { 0 };
    StartupInfo.cb = sizeof(STARTUPINFO);
    PROCESS_INFORMATION ProcessInfo = { 0 };
    ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));

    //����һ����ȫ�����ṹ�岢�������������̳�
    SECURITY_ATTRIBUTES SecurityAttributes{ sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };
    // ����һ���¼��������̳У�
    // �¼��ں˶��������
    HANDLE EventHandle = CreateEvent(&SecurityAttributes, TRUE, FALSE, NULL);// ����һ���¼����ں˶���
    if (EventHandle == NULL) {
        // �Ժ�ʹ�õ��������
        OutputDebugString(_T("�¼�����ʧ��\r\n"));
        goto Exit;
    }
    else {
        _tprintf(_T("EventHandle:%d\r\n"), (int)EventHandle);
        // ���¼��ľ��ֵ���ݵ�Ŀ���ӽ���
        // �����ֵ��ʽ�����ַ�����
        int BufferDataLength = _stprintf_s(BufferData, _T("Yuwenkai=%d"), (int)EventHandle);
        BufferData[BufferDataLength] = '\0';
        BufferData[BufferDataLength + 1] = '\0';
        _tprintf(_T("BufferData:%s\r\n"), BufferData);
        // ����һ���ӽ��̲�����ǰ�����е�һЩ�����Ի�����������ʽ���ݵ��ӽ���
        #ifdef UNICODE
            CreateFlags = CREATE_UNICODE_ENVIRONMENT;
        #endif
        BOOL IsOk = CreateProcess(_T("SubProcess.exe"), NULL, NULL, NULL, TRUE,
            CreateFlags | CREATE_NEW_CONSOLE,  //��ʾ���������ĸ�ʽ����������˴˱�־��lpEnvironment��ָ��Ļ����齫ʹ��Unicode�ַ������򣬻�����ʹ��ANSI�ַ�
            (LPVOID)BufferData,		           //�µĻ��������ڴ��
            NULL, &StartupInfo, &ProcessInfo);
            //�����в�����������(�����̴����ӽ���)
        if (IsOk == FALSE)
        {
            int LastError = GetLastError();
            OutputDebugString(_T("�ӽ��̴���ʧ��\r\n"));
            goto Exit;
        }
        //��Ҫ��ռ�ж��ӽ�����Դ
        if (ProcessInfo.hProcess != NULL || ProcessInfo.hThread != NULL)
        {
            CloseHandle(ProcessInfo.hProcess);
            CloseHandle(ProcessInfo.hThread);
        }
        //�ȴ��¼�����
        WaitForSingleObject(EventHandle, INFINITE);
        //���ӽ�������������¼� ���� ����MessageBox
        MessageBox(NULL, _T("�ӽ��̴����¼����̳гɹ�"), _T("Success"), 1);

        printf("Input AnyKey To Exit\r\n");
        _gettchar();
    }
Exit:
    if (EventHandle != NULL)
    {
        CloseHandle(EventHandle);
        EventHandle = NULL;
    }
}