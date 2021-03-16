#include "SubProcess.h"

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {

    LPTCH v1;
    TCHAR* EnvironmentVariable = NULL;
    v1 = GetEnvironmentStrings();
    if (v1 == NULL) {
        _tprintf(_T("Error\r\n"));
        return -1;
    }
    else {
        EnvironmentVariable = v1;
        while (*EnvironmentVariable) {
            _tprintf(_T("%s\r\n"), EnvironmentVariable);
            // 注意这里是指针加减
            EnvironmentVariable += _tcslen(EnvironmentVariable) + 1;
        }
        FreeEnvironmentStrings(v1);
        _tprintf(_T("\r\n"));
        _gettchar();
        _gettchar();
    }
    Sleep(5000);
    return 0;
}
