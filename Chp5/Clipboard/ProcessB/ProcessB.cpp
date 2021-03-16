#include "ProcessB.h"


int _tmain() {
    _tsetlocale(LC_ALL, _T("Chinese-simplified"));

    if (OpenClipboard(GetDesktopWindow())) {
        // 剪贴板对象
        HANDLE ClipboardHandle = INVALID_HANDLE_VALUE;
        TCHAR* VirtualAddress  = NULL;
        TCHAR* BufferData      = NULL;
        // 初始化清空剪贴板
        EmptyClipboard();
        const TCHAR* CommandLine = _T("我将无我，不负人民");
        // 申请要存储字符串大小的缓存区（从堆中分配指定的字节数）
        #ifdef _UNICODE
        // 调试内存
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
        // 设置剪贴板的内容
        SetClipboardData(CF_TEXT, ClipboardHandle);
        // 最后关闭剪贴板
        CloseClipboard();
        GlobalFree(ClipboardHandle);
    }

    return 0;
}