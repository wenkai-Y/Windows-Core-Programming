#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
using namespace std;

void Sub_1();   // 何为内核对象
void Sub_2();   // 对象计数

int _tmain() {

    // 何为内核对象
    Sub_1();

    // 对象计数
    Sub_2();



    return 0;
}

void Sub_1() {

    //句柄的数据类型  4 / 8
    _tprintf(_T("%d"), sizeof(HANDLE));

    // 不合法句柄值为 -1
    HANDLE FileHandle = INVALID_HANDLE_VALUE;

    // 返回句柄表中的索引  4或8的倍数
    FileHandle = CreateFile(_T("Demo.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (FileHandle == INVALID_HANDLE_VALUE) {
        return ;
    }

    // 写数据
    TCHAR v1[] = _T("HelloWorld!");
    DWORD NumberOfbyteWritten(0);
    WriteFile(FileHandle, v1, _tcsclen(v1) * sizeof(TCHAR), &NumberOfbyteWritten, NULL);
    CloseHandle(FileHandle);
    FileHandle = INVALID_HANDLE_VALUE;

}

void Sub_2()
{

    // 

}