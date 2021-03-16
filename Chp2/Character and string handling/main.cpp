#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable : 4996)
#pragma warning(disable : 6262)
#pragma warning(disable : 6031)
// ��Ҫʹ�� CString������� atlstr.h ��ͷ��
#include <atlstr.h>
using namespace std;

void Sub_1();
void Sub_2();
void Sub_3();
void Sub_4();
void Sub_5();
BOOL YwkMultiByteToWideChar(char*);
BOOL YwkWideCharToMultiByte(LPWSTR);


int _tmain() {
    //Sub_1();
    //Sub_2();
    //Sub_3();
    //Sub_4();
    _tsetlocale(LC_ALL, _T("Chinese-simplified"));
    //Sub_5();
    //char v1[] = "HelloWorld";
    //YwkMultiByteToWideChar(v1);
    WCHAR v1[] = _T("HelloWorld!");
    YwkWideCharToMultiByte(v1);
    return 0;
}

void Sub_1() {  

    TCHAR v1[20] = _T("HelloWorld!");
    TCHAR v2[20];
    
    // 1. number of characters
    // ���ַ�
    _tcscpy_s(v2, _countof(v2), v1);
    _tprintf(_T("%s\r\n"), v2);

    // 2��Count Bytes Of Source to Destination
    // ���ֽ�
    memcpy(v2, v1, sizeof(TCHAR) * (10 + 1));
    _tprintf(_T("%s\r\n"), v2);
    system("pause");

}

void Sub_2() {
    int v1[5] = { 1, 2, 3, 4, 5 };
    TCHAR v2[] = _T("HelloWorld");
    // sizeof()
    _tprintf(_T("%d\t%d\n"), sizeof(v1), _countof(v1));
    _tprintf(_T("%d\t%d\n"), sizeof(v2), _countof(v2));
    system("pause");
    //_tprintf(_T("%d\r\n"), sizeof(v1));
}

void Sub_3() {

#ifdef _UNICODE
#define _tstring wstring
#define _tcout wcout
#else
#define _tstirng string
#define _tcout cout
#endif

    // ���ֽ�
    string v1;

    // ˫�ֽ�
    wstring v2;

    // �Զ��жϵ�˫�ֽ�
    _tstring v3 = _T("HelloWorld");

    // Ĭ�ϵ�˫��֧�� ��MFC��
    // ���֧�� ��Ҫʹ�� CString������� atlstr.h ��ͷ��
    CString v4 = _T("HelloCString");

    _tcout << v3.c_str() << endl;
    _tcout << v4.GetString() << endl;
    // cout�������˫�ֽ�
    cout << v3.c_str() << endl;
    //_tprintf(_T("%s\n"), v3);
    system("pause");
}

void Sub_4() {

    // �ص�����
    TCHAR v1[] = _T("HelloWorld");
    TCHAR v2[20] = { 0 };
    // Դ����Ƶ�����

    // ǰ����
    _tcscpy_s(&v1[0], 10, &v1[1]);
    _tprintf(_T("%s\n"), v1);
    // �󿽱�
    _tcscpy_s(&v1[1], 10, &v1[0]);  // �������
    _tprintf(_T("%s\n"), v1);

    // memcpy֧���ص�����
    system("pause");

}

void Sub_5() {

    // ��ʽ��

#ifdef _UNICODE
#define _tstring wstring
#define _tcout wcout
#else
#define _tstirng string
#define _tcout cout
#endif


    //int v1 = 8;       // 0
    //                   // 0x30  ��0x30

    //_tprintf(_T("%d\r\n"), v1);     // 102

    //// ��ģ����ʽ��
    //_tprintf(_T("%c\r\n"), v1);     // 102  -> 1 0 2
    //_tprintf(_T("%c\r\n"), v1 + 0x30);     // 102  -> 1 0 2


    // ��ʽ������

    int v1 = 345;
    float v2 = 4.14f;
    TCHAR BufferData[0x1000] = { 0 };
    _stprintf(BufferData, _T("���Ƕ����й��ˣ� %d  %f"), v1, v2);
    _tprintf(_T("_tprintf: %s\r\n"), BufferData);     // �����ӡ

    int v3 = 0;
    float v4 = 0;
    TCHAR v5[0x1000];
    _stscanf(BufferData, _T("%s %d %f"), v5, &v3, &v4);
    _tprintf(_T("%d\r\n"), v3);
    _tprintf(_T("%f\r\n"), v4);
    _tprintf(_T("%s\r\n"), v5);

}

BOOL YwkMultiByteToWideChar(char *MultiByteString) {

    // ���ֽ�ת�������ֽ�
    LPWSTR      WideString;
    DWORD       WideStrLenth;

    // �жϵ��ֽ��Ƿ���Ч
    if (!MultiByteString)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }
    // 1����ȡ����
    WideStrLenth = MultiByteToWideChar(CP_ACP, 0, MultiByteString, -1, NULL, 0);
    // 2����̬�����ڴ�
    WideString = (WCHAR*)malloc(WideStrLenth * sizeof(WCHAR));
    if (!WideString) {
        SetLastError(SEC_E_INSUFFICIENT_MEMORY);
        return FALSE;
    }
    // 3��ת��
    MultiByteToWideChar(CP_ACP, 0, MultiByteString, -1, WideString, WideStrLenth);
    // 4�������ڴ�
    if (WideString)
    {
        free(WideString);
        WideString = NULL;
    }
    return TRUE;
}

BOOL YwkWideCharToMultiByte(LPWSTR WideString) {
    
    // ���ֽ�ת�������ֽ�
    char* ByteString;
    DWORD ByteStrLength;
    // �жϿ��ֽ��Ƿ���Ч
    if (!WideString) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }
    // 1����ȡ����
    ByteStrLength = WideCharToMultiByte(CP_ACP, 0, WideString, -1, NULL, 0, NULL, NULL);
    // 2����̬�����ڴ�
    ByteString = new char[ByteStrLength];
    if (!ByteString) {
        SetLastError(SEC_E_INSUFFICIENT_MEMORY);
        return FALSE;
    }
    // 3��ת��
    WideCharToMultiByte(CP_ACP, 0, WideString, -1, ByteString, ByteStrLength, NULL, NULL);
    // 4�������ڴ�
    if (ByteString) {
        delete[]ByteString;
        ByteString = NULL;
    }
    return TRUE;
}