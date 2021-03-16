#include "_tchdir.h"

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {
    _tsetlocale(LC_ALL, _T("Chinese-simplified"));
    TCHAR BufferPath[MAX_PATH] = { 0 };
    YekGetFullPathName(argv[0], BufferPath);
    _tprintf(_T("%s\r\n"), BufferPath);


    return 0;
}

VOID YekGetFullPathName(_In_ const TCHAR* argv, _Out_ TCHAR* BufferPath) {
    GetFullPathName(argv, MAX_PATH, BufferPath, NULL);
}