#include "Gets the command-line arguments.h"

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {

    setlocale(LC_ALL, "Chinese-simplified");
    UNREFERENCED_PARAMETER(envp);
    _tprintf(_T("%s\r\n"), argv[0]);
    _tprintf(_T("命令行参数地址：%p\r\n"), argv);
    SIZE_T ReturnLength = sizeof(MEMORY_BASIC_INFORMATION);
    MEMORY_BASIC_INFORMATION MemoryBasicInfo = { 0 };
    VirtualQuery(argv[0], &MemoryBasicInfo, ReturnLength);
    _tprintf(_T("内存保护属性：%d\r\n"), MemoryBasicInfo.AllocationProtect);   // PAGE_READWRITE
    // 因为命令行参数所在内存属性为PAGE_READWRITE，所以我们尝试修改
    argv[0][0] = _T('H');
    _tprintf(_T("%s\r\n\n"), argv[0]);

    Sub_1();
    Sub_2();
    Sub_3();
    return 0;
}

/* 使用全局变量 */
void Sub_1() {
    _tprintf(_T("Sub_1()\r\n"));
#ifdef _UNICODE
    wprintf(L"%s\r\n", __wargv[0]);
#else
    printf("%s\r\n", __argv[0]);
    printf("命令行参数地址：%p\r\n", __argv);
#endif
}

void Sub_2() {
    _tprintf(_T("\nSub_2()\r\n"));
    TCHAR* CommandLine = GetCommandLine();  // 使用一份新的内存BaseUnicodeCommandLine全局变量
    _tprintf(_T("%s\r\n"), CommandLine);
    // 查看命令行参数所在内存的内存页属性
    SIZE_T ReturnLength = sizeof(MEMORY_BASIC_INFORMATION);
    MEMORY_BASIC_INFORMATION MemoryBasicInfo = { 0 };
    VirtualQuery(CommandLine, &MemoryBasicInfo, ReturnLength);
    printf("命令行参数地址：%p\r\n", CommandLine);
    _tprintf(_T("内存保护属性：%d\r\n"), MemoryBasicInfo.AllocationProtect);   // PAGE_READWRITE
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