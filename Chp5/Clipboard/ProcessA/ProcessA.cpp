#include "ProcessA.h"

int _tmain() {

    _tsetlocale(LC_ALL, _T("Chinese-simplified"));
    if (OpenClipboard(GetDesktopWindow())) {
        HANDLE ClipBoardHandle  = INVALID_HANDLE_VALUE;
        TCHAR* BufferData       = NULL;
        TCHAR* VirtualAddress   = NULL;
        UINT   ViewSize         = 0;
        // ��ü����������Ϣ
        ClipBoardHandle = GetClipboardData(CF_TEXT);    // ����ں˶���ľ��
        // ͨ����������ڴ�
        VirtualAddress = (TCHAR*)GlobalLock(ClipBoardHandle);
        // ��ȡ��С
        ViewSize = GlobalSize(ClipBoardHandle);
        BufferData = new TCHAR[ViewSize + 1];
        // ����������������ݿ�������
        if (VirtualAddress != NULL) {
            memcpy(BufferData, VirtualAddress, ViewSize);
        }
        // �������
        GlobalUnlock(ClipBoardHandle);
        _tprintf(_T("%s\r\n"), BufferData);
        if (BufferData != NULL) {
            delete[]BufferData;
            BufferData = NULL;
        }
        // �رռ�����
        CloseClipboard();
        Sleep(2000);
    }
    return 0;
}