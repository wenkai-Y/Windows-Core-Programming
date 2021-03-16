#include "ProcessA.h"

int _tmain() {

    _tsetlocale(LC_ALL, _T("Chinese-simplified"));
    if (OpenClipboard(GetDesktopWindow())) {
        HANDLE ClipBoardHandle  = INVALID_HANDLE_VALUE;
        TCHAR* BufferData       = NULL;
        TCHAR* VirtualAddress   = NULL;
        UINT   ViewSize         = 0;
        // 获得剪贴板里的信息
        ClipBoardHandle = GetClipboardData(CF_TEXT);    // 获得内核对象的句柄
        // 通过句柄锁定内存
        VirtualAddress = (TCHAR*)GlobalLock(ClipBoardHandle);
        // 获取大小
        ViewSize = GlobalSize(ClipBoardHandle);
        BufferData = new TCHAR[ViewSize + 1];
        // 将剪贴板里面的数据拷贝出来
        if (VirtualAddress != NULL) {
            memcpy(BufferData, VirtualAddress, ViewSize);
        }
        // 解除锁定
        GlobalUnlock(ClipBoardHandle);
        _tprintf(_T("%s\r\n"), BufferData);
        if (BufferData != NULL) {
            delete[]BufferData;
            BufferData = NULL;
        }
        // 关闭剪贴板
        CloseClipboard();
        Sleep(2000);
    }
    return 0;
}