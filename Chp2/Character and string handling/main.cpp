#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable : 4996)
#pragma warning(disable : 6262)
#pragma warning(disable : 6031)
// 若要使用 CString，请包含 atlstr.h 标头。
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
    // 按字符
    _tcscpy_s(v2, _countof(v2), v1);
    _tprintf(_T("%s\r\n"), v2);

    // 2、Count Bytes Of Source to Destination
    // 按字节
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

    // 单字节
    string v1;

    // 双字节
    wstring v2;

    // 自动判断单双字节
    _tstring v3 = _T("HelloWorld");

    // 默认单双字支持 在MFC下
    // 如何支持 若要使用 CString，请包含 atlstr.h 标头。
    CString v4 = _T("HelloCString");

    _tcout << v3.c_str() << endl;
    _tcout << v4.GetString() << endl;
    // cout不能输出双字节
    cout << v3.c_str() << endl;
    //_tprintf(_T("%s\n"), v3);
    system("pause");
}

void Sub_4() {

    // 重叠拷贝
    TCHAR v1[] = _T("HelloWorld");
    TCHAR v2[20] = { 0 };
    // 源码设计的问题

    // 前拷贝
    _tcscpy_s(&v1[0], 10, &v1[1]);
    _tprintf(_T("%s\n"), v1);
    // 后拷贝
    _tcscpy_s(&v1[1], 10, &v1[0]);  // 程序崩溃
    _tprintf(_T("%s\n"), v1);

    // memcpy支持重叠拷贝
    system("pause");

}

void Sub_5() {

    // 格式化

#ifdef _UNICODE
#define _tstring wstring
#define _tcout wcout
#else
#define _tstirng string
#define _tcout cout
#endif


    //int v1 = 8;       // 0
    //                   // 0x30  差0x30

    //_tprintf(_T("%d\r\n"), v1);     // 102

    //// 除模，格式化
    //_tprintf(_T("%c\r\n"), v1);     // 102  -> 1 0 2
    //_tprintf(_T("%c\r\n"), v1 + 0x30);     // 102  -> 1 0 2


    // 格式化函数

    int v1 = 345;
    float v2 = 4.14f;
    TCHAR BufferData[0x1000] = { 0 };
    _stprintf(BufferData, _T("我们都是中国人！ %d  %f"), v1, v2);
    _tprintf(_T("_tprintf: %s\r\n"), BufferData);     // 输出打印

    int v3 = 0;
    float v4 = 0;
    TCHAR v5[0x1000];
    _stscanf(BufferData, _T("%s %d %f"), v5, &v3, &v4);
    _tprintf(_T("%d\r\n"), v3);
    _tprintf(_T("%f\r\n"), v4);
    _tprintf(_T("%s\r\n"), v5);

}

BOOL YwkMultiByteToWideChar(char *MultiByteString) {

    // 多字节转换到多字节
    LPWSTR      WideString;
    DWORD       WideStrLenth;

    // 判断单字节是否无效
    if (!MultiByteString)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }
    // 1、获取长度
    WideStrLenth = MultiByteToWideChar(CP_ACP, 0, MultiByteString, -1, NULL, 0);
    // 2、动态申请内存
    WideString = (WCHAR*)malloc(WideStrLenth * sizeof(WCHAR));
    if (!WideString) {
        SetLastError(SEC_E_INSUFFICIENT_MEMORY);
        return FALSE;
    }
    // 3、转换
    MultiByteToWideChar(CP_ACP, 0, MultiByteString, -1, WideString, WideStrLenth);
    // 4、销毁内存
    if (WideString)
    {
        free(WideString);
        WideString = NULL;
    }
    return TRUE;
}

BOOL YwkWideCharToMultiByte(LPWSTR WideString) {
    
    // 宽字节转换到多字节
    char* ByteString;
    DWORD ByteStrLength;
    // 判断宽字节是否有效
    if (!WideString) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }
    // 1、获取长度
    ByteStrLength = WideCharToMultiByte(CP_ACP, 0, WideString, -1, NULL, 0, NULL, NULL);
    // 2、动态申请内存
    ByteString = new char[ByteStrLength];
    if (!ByteString) {
        SetLastError(SEC_E_INSUFFICIENT_MEMORY);
        return FALSE;
    }
    // 3、转换
    WideCharToMultiByte(CP_ACP, 0, WideString, -1, ByteString, ByteStrLength, NULL, NULL);
    // 4、销毁内存
    if (ByteString) {
        delete[]ByteString;
        ByteString = NULL;
    }
    return TRUE;
}