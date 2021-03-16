#include "Process.h"

// 进程对象
// 事件对象
// 进程间通信（IPC, InterProcess Communication）是指在不同进程之间传播或交换信息
// Handle Inherit




// 定义参数
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {
    

    Sub_1();   // 命令行参数
    Sub_2();   // 环境变量 
    
    
    return 0;
}


void Sub_1()   // 命令行参数
{
    // 启动子进程需要的参数
    STARTUPINFO StartupInfo = { 0 };    //启动一个子进程   CreateProcess
    StartupInfo.cb = sizeof(STARTUPINFO);
    PROCESS_INFORMATION ProcessInfo = { 0 };

    // 创建一个安全描述结构体并设置允许句柄被继承
    SECURITY_ATTRIBUTES SecurityAttributes;
    SecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    SecurityAttributes.bInheritHandle = TRUE;   //允许句柄继承
    SecurityAttributes.lpSecurityDescriptor = NULL;

    // 创建一个事件（允许被继承）
    // 事件内核对象的作用
    HANDLE EventHandle = CreateEvent(&SecurityAttributes, TRUE, FALSE, NULL);// 创建一个事件的内核对象
    if (EventHandle == NULL) {
        // 以后使用的输出函数
        OutputDebugString(_T("事件创建失败\r\n"));
        goto Exit;
    }
    else {

        // 格式化输出
        // 说明事件创建成功，并打印
        TCHAR BufferData[0x1000] = { 0 };
        _stprintf(BufferData, _T("EventCreate:%d\r\n"), (int)EventHandle);
        _tprintf(_T("EventHandle:%d\r\n"), (int)EventHandle);
        OutputDebugString(BufferData);

        // 将该事件的句柄值传递到目标进程子进程
        // 将句柄值格式化到字符串中
        memset(BufferData, 0, sizeof(BufferData));
        _stprintf(BufferData, _T("%d"), (int)EventHandle);  // 格式化成字符串
     /*   OutputDebugString(BufferData);*/

        // 创建一个子进程并将当前进程中的一些数据以命令行参数的形式传递到子进程
        BOOL IsOk = CreateProcess(_T("D:\\Studying_Area\\Computer science and technology\\Programming code\\Reverse Engineering\\Windows Core Programming\\Chp3\\Handle to the parent-child process\\Debug\\SubProcess.exe"),
            (LPTSTR)BufferData, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL,
            &StartupInfo, &ProcessInfo);

        // 命令行参数传递数据（父进程建子进程）
        if (IsOk == FALSE) {
            _tprintf(_T("子进程创建失败\r\n"));
            OutputDebugString(_T("子进程创建失败\r\n"));
            goto Exit;
        }
        // 不要再占有子进程资源
        if (ProcessInfo.hProcess != NULL) {
            CloseHandle(ProcessInfo.hProcess);
        }
        if (ProcessInfo.hThread != NULL) {
            CloseHandle(ProcessInfo.hThread);
        }
        // 等待事件触信
        _tprintf(_T("等待子进程事件通知\r\n"));
        WaitForSingleObject(EventHandle, INFINITE);
        MessageBox(NULL, _T("子进程触发事件，继承成功"), _T("Success"), 1);

        // 按任意键退出
        _tprintf(_T("Inputer Anykey To Exit\r\n"));
        _gettchar();
    }
Exit:
    if (EventHandle != NULL) {
        CloseHandle(EventHandle);
        EventHandle = NULL;
    }
}

void Sub_2()    // 环境变量
{
    DWORD CreateFlags = 0;
    TCHAR BufferData[MAX_PATH] = { 0 };
    STARTUPINFO StartupInfo = { 0 };
    StartupInfo.cb = sizeof(STARTUPINFO);
    PROCESS_INFORMATION ProcessInfo = { 0 };
    ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));

    //创建一个安全描述结构体并设置允许句柄被继承
    SECURITY_ATTRIBUTES SecurityAttributes{ sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };
    // 创建一个事件（允许被继承）
    // 事件内核对象的作用
    HANDLE EventHandle = CreateEvent(&SecurityAttributes, TRUE, FALSE, NULL);// 创建一个事件的内核对象
    if (EventHandle == NULL) {
        // 以后使用的输出函数
        OutputDebugString(_T("事件创建失败\r\n"));
        goto Exit;
    }
    else {
        _tprintf(_T("EventHandle:%d\r\n"), (int)EventHandle);
        // 将事件的句柄值传递到目标子进程
        // 将句柄值格式化到字符串中
        int BufferDataLength = _stprintf_s(BufferData, _T("Yuwenkai=%d"), (int)EventHandle);
        BufferData[BufferDataLength] = '\0';
        BufferData[BufferDataLength + 1] = '\0';
        _tprintf(_T("BufferData:%s\r\n"), BufferData);
        // 创建一个子进程并将当前进程中的一些参数以环境变量的形式传递到子进程
        #ifdef UNICODE
            CreateFlags = CREATE_UNICODE_ENVIRONMENT;
        #endif
        BOOL IsOk = CreateProcess(_T("SubProcess.exe"), NULL, NULL, NULL, TRUE,
            CreateFlags | CREATE_NEW_CONSOLE,  //表示环境参数的格式。如果设置了此标志，lpEnvironment所指向的环境块将使用Unicode字符。否则，环境块使用ANSI字符
            (LPVOID)BufferData,		           //新的环境变量内存块
            NULL, &StartupInfo, &ProcessInfo);
            //命令行参数传递数据(父进程创建子进程)
        if (IsOk == FALSE)
        {
            int LastError = GetLastError();
            OutputDebugString(_T("子进程创建失败\r\n"));
            goto Exit;
        }
        //不要再占有对子进程资源
        if (ProcessInfo.hProcess != NULL || ProcessInfo.hThread != NULL)
        {
            CloseHandle(ProcessInfo.hProcess);
            CloseHandle(ProcessInfo.hThread);
        }
        //等待事件触信
        WaitForSingleObject(EventHandle, INFINITE);
        //当子进程设置了这个事件 则触信 弹出MessageBox
        MessageBox(NULL, _T("子进程触发事件，继承成功"), _T("Success"), 1);

        printf("Input AnyKey To Exit\r\n");
        _gettchar();
    }
Exit:
    if (EventHandle != NULL)
    {
        CloseHandle(EventHandle);
        EventHandle = NULL;
    }
}