#include "ProcessB.h"


int _tmain() {
    _tsetlocale(LC_ALL, _T("Chinese-simplified"));

    if (OpenClipboard(GetDesktopWindow())) {
        // ���������
        HANDLE ClipboardHandle = INVALID_HANDLE_VALUE;
        TCHAR* VirtualAddress  = NULL;
        TCHAR* BufferData      = NULL;
        // ��ʼ����ռ�����
        EmptyClipboard();
        const TCHAR* CommandLine = _T("�ҽ����ң���������");
        // ����Ҫ�洢�ַ�����С�Ļ��������Ӷ��з���ָ�����ֽ�����
        #ifdef _UNICODE
        // �����ڴ�
            ClipboardHandle = GlobalAlloc(GMEM_MOVEABLE, (wcslen(CommandLine) + 1) * sizeof(WCHAR));
        #else
            ClipboardHandle = GlobalAlloc(GMEM_MOVEABLE, (wcslen(CommandLine) + 1));
        #endif
        if (ClipboardHandle != NULL) {
            VirtualAddress = (TCHAR*)GlobalLock(ClipboardHandle);
        }
        #ifdef _UNICODE
            if (VirtualAddress != NULL) {
                memcpy(VirtualAddress, CommandLine, (wcslen(CommandLine) + 1) * sizeof(WCHAR));
            }
        #else
            memcpy(BufferData, CommandLine, strlen(CommandLine) + 1);
        #endif
        if (ClipboardHandle != NULL) {
            GlobalUnlock(ClipboardHandle);
        }
        // ���ü����������
        SetClipboardData(CF_TEXT, ClipboardHandle);
        // ���رռ�����
        CloseClipboard();
        GlobalFree(ClipboardHandle);
    }

    return 0;
}