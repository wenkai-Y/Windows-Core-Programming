#include "Queue.h"

// global
CQueue g_q(10);                       // The shared queue
volatile LONG g_fShutdown = FALSE;    // Signals client/server threads to die
HWND g_hwnd;                          // How client/server threads give status

// Handles to all client/server threads & number of client/server threads
HANDLE g_hThreads[MAXIMUM_WAIT_OBJECTS];
int    g_nNumThreads = 0;

int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {
    DialogBox(hinstExe, MAKEINTRESOURCE(IDD_QUEUE), NULL, Dlg_Proc);
    InterlockedExchange(&g_fShutdown, TRUE);
    // wiat for all the threads to tremisate & then cleanup
    WaitForMultipleObjects(g_nNumThreads, g_hThreads, TRUE, INFINITE);
    while (g_nNumThreads--)
        YekCloseHandle(g_hThreads[g_nNumThreads]);
    return 0;
}

INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        // 初始化函数
        chHANDLE_DLGMSG(hWnd, WM_INITDIALOG, Dlg_OnInitDialog);
        // 消息处理函数
        chHANDLE_DLGMSG(hWnd, WM_COMMAND, Dlg_OnCommand);
    }
    return FALSE;
}

BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {
    // 设置对话框图标
    chSETDLGICONS(hWnd, IDI_QUEUE);
    // 用来显示客户端和服务端的状态
    g_hwnd = hWnd;
    DWORD dwThreadID;
    // 创建客户端线程
    for (int x = 0; x < 4; ++x) {
        g_hThreads[g_nNumThreads++] =
            chBEGINTHREADEX(NULL, 0, ClientThread, (PVOID)(INT_PTR)x,
                0, &dwThreadID);
    }
    // 创建服务端线程
    for (int x = 0; x < 2; ++x) {
        g_hThreads[g_nNumThreads++] =
            chBEGINTHREADEX(NULL, 0, ServerThread, (PVOID)(INT_PTR)x,
                0, &dwThreadID);
    }
    return TRUE;
}

DWORD WINAPI ClientThread(PVOID pvParam) {
    int nThreadNum = PtrToUlong(pvParam);
    HWND hwndLB = GetDlgItem(g_hwnd, IDC_CLIENTS);
    int nRequestNum = 0;
    while (1 != InterlockedCompareExchange(&g_fShutdown, 0, 0)) {
        // keep track of the current processed element
        nRequestNum++;
        TCHAR sz[1024];
        CQueue::ELEMENT e = { nThreadNum, nRequestNum };
        // try to put an element on the queue
        if (g_q.Append(&e, 200)) {
            // Indicate which thread sent it and which request
            StringCchPrintf(sz, _countof(sz), TEXT("Sending %d:%d"),
                nThreadNum, nRequestNum);
        }
        else {
            // could't put an element on the queue
            StringCchPrintf(sz, _countof(sz), TEXT("Sending %d:%d(%s)"),
                nThreadNum, nRequestNum, (GetLastError() == ERROR_TIMEOUT)
                ? TEXT("timeout") : TEXT("full"));
        }
        // show result of appending element
        ListBox_SetCurSel(hwndLB, ListBox_AddString(hwndLB, sz));
        // wait befor appending another element
        Sleep(2500);
    }
    return 0;
}

DWORD WINAPI ServerThread(PVOID pvParam) {
    int nThreadNum = PtrToUlong(pvParam);
    HWND hwndLB = GetDlgItem(g_hwnd, IDC_SERVERS);
    while (1 != InterlockedCompareExchange(&g_fShutdown, 0, 0)) {
        TCHAR sz[1024];
        CQueue::ELEMENT e;
        // try to get an element from the queue
        if (g_q.Remove(&e, 5000)) {
            // Indicate which thread is processing it, which thread
            // sent it and which request we're processing
            StringCchPrintf(sz, _countof(sz), TEXT("%d: Processing %d:%d"),
                nThreadNum, e.m_nThreadNum, e.m_nRequestNum);
            // the server tasks some time to process the request
            Sleep(2000 * e.m_nThreadNum);
        }
        else {
            // Could'n get an element from the queue
            StringCchPrintf(sz, _countof(sz), TEXT("%d: (timeout)"), nThreadNum);
        }
        // show result of processing element
        ListBox_SetCurSel(hwndLB, ListBox_AddString(hwndLB, sz));
    }
    return 0;
}

void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {
    switch (id)
    {
    case IDC_BIN_STOP:
        EndDialog(hWnd, id);
    default:
        break;
    }
}

bool YekCloseHandle(HANDLE myHandle)
{
    DWORD HandleFlags;
    if (GetHandleInformation(myHandle, &HandleFlags) && (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
        return !!CloseHandle(myHandle);
    return false;
}