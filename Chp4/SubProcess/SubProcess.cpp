#include "SubProcess.h"

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {

    setlocale(LC_ALL, "Chinese-simplified");
    CreateDirectory(_T("HelloWorld"), NULL);
    _tprintf(_T("Create Successed\r\n"));
    _gettchar();
    return 0;
}