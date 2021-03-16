#include "Gets the command-line arguments.h"

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {

    setlocale(LC_ALL, "Chinese-simplified");
    UNREFERENCED_PARAMETER(envp);
    _tprintf(_T("%s\r\n"), argv[0]);
    _tprintf(_T("�����в�����ַ��%p\r\n"), argv);
    SIZE_T ReturnLength = sizeof(MEMORY_BASIC_INFORMATION);
    MEMORY_BASIC_INFORMATION MemoryBasicInfo = { 0 };
    VirtualQuery(argv[0], &MemoryBasicInfo, ReturnLength);
    _tprintf(_T("�ڴ汣�����ԣ�%d\r\n"), MemoryBasicInfo.AllocationProtect);   // PAGE_READWRITE
    // ��Ϊ�����в��������ڴ�����ΪPAGE_READWRITE���������ǳ����޸�
    argv[0][0] = _T('H');
    _tprintf(_T("%s\r\n\n"), argv[0]);

    Sub_1();
    Sub_2();
    Sub_3();
    return 0;
}

/* ʹ��ȫ�ֱ��� */
void Sub_1() {
    _tprintf(_T("Sub_1()\r\n"));
#ifdef _UNICODE
    wprintf(L"%s\r\n", __wargv[0]);
#else
    printf("%s\r\n", __argv[0]);
    printf("�����в�����ַ��%p\r\n", __argv);
#endif
}

void Sub_2() {
    _tprintf(_T("\nSub_2()\r\n"));
    TCHAR* CommandLine = GetCommandLine();  // ʹ��һ���µ��ڴ�BaseUnicodeCommandLineȫ�ֱ���
    _tprintf(_T("%s\r\n"), CommandLine);
    // �鿴�����в��������ڴ���ڴ�ҳ����
    SIZE_T ReturnLength = sizeof(MEMORY_BASIC_INFORMATION);
    MEMORY_BASIC_INFORMATION MemoryBasicInfo = { 0 };
    VirtualQuery(CommandLine, &MemoryBasicInfo, ReturnLength);
    printf("�����в�����ַ��%p\r\n", CommandLine);
    _tprintf(_T("�ڴ汣�����ԣ�%d\r\n"), MemoryBasicInfo.AllocationProtect);   // PAGE_READWRITE
}

void Sub_3() {
    _tprintf(_T("\nSub_2()\r\n"));
    LPWSTR* v1 = NULL;
    int v2;
    int i;
    v1 = CommandLineToArgvW(GetCommandLineW(), &v2);
    if (v1 == NULL) {
        return;
    }
    else {
        for (i = 0; i < v2; ++i) {
            printf("%d: %ws\r\n", i, v1[i]);
            wprintf(L"%d: %s\r\n", v1[i]);
        }
    }
    int IsOk = HeapFree(GetProcessHeap(), 0, v1);
    int LastError = GetLastError();
}