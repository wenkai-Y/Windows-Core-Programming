#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
using namespace std;

void Sub_1();   // ��Ϊ�ں˶���
void Sub_2();   // �������

int _tmain() {

    // ��Ϊ�ں˶���
    Sub_1();

    // �������
    Sub_2();



    return 0;
}

void Sub_1() {

    //�������������  4 / 8
    _tprintf(_T("%d"), sizeof(HANDLE));

    // ���Ϸ����ֵΪ -1
    HANDLE FileHandle = INVALID_HANDLE_VALUE;

    // ���ؾ�����е�����  4��8�ı���
    FileHandle = CreateFile(_T("Demo.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (FileHandle == INVALID_HANDLE_VALUE) {
        return ;
    }

    // д����
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