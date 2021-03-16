#include "Gets the process environment variables.h"

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {

    setlocale(LC_ALL, "Chinese-simplified");
    
    Sub_1(envp);
    Sub_2();



    return 0;
}

/* 继承环境变量 */
void Sub_1(TCHAR** EnvironmentVariable) {
    TCHAR** v1 = EnvironmentVariable;
    TCHAR* v2 = NULL;
    while (v1 != NULL) {
        v2 = *v1;
        if (v2 == NULL) {
            break;
        }
        else {
            _tprintf(_T("%s\r\n"), v2);
            v1++;
        }
    }
    _tprintf(_T("\n\n"));
}

/* GetEnvironmentStrings */
void Sub_2() {
    LPTCH v1;
    TCHAR* EnvironmentVariable = NULL;
    v1 = GetEnvironmentStrings();
    if (v1 == NULL) {
        _tprintf(_T("%d"), (int)GetLastError());
    }
    else {
        EnvironmentVariable = v1;
        while (*EnvironmentVariable) {
            _tprintf(_T("%s\r\n"), EnvironmentVariable);
            EnvironmentVariable += lstrlen(EnvironmentVariable) + 1;
        }
        FreeEnvironmentStrings(v1);
    }
    _tprintf(_T("\n\n"));
}