#include "SubProcess.h"

// 当前程序需要借助Windbg调试器

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {
    
    //Sub_1(argv);
    Sub_2();

}

void Sub_1(TCHAR* argv[]) {

    HANDLE EventHandle = 0;
    // 接收CommandLine的数据实际上是单字的
    _stscanf_s((TCHAR*)argv[0], _T("%d"), &EventHandle);

    _tprintf(_T("EventHandle:%d"), (int)EventHandle);
    //OutputDebugString((LPCWSTR)EventHandle);

    __try {
        if (EventHandle != NULL) {
            SetEvent(EventHandle);
        }
        Sleep(5000);

        if (EventHandle != NULL) {
            CloseHandle(EventHandle);
        }
        ExitProcess(0);
    }
    __except(EXCEPTION_EXECUTE_HANDLER){
        int lastError = GetExceptionCode();
    }
}


void Sub_2() {

	//获得进程环境变量
	const TCHAR* BufferData = _T("Shine");
	__try
	{
		int BufferLength = GetEnvironmentVariable(BufferData, NULL, 0);
		TCHAR* v5 = new TCHAR[BufferLength + 1];
		memset(v5, 0, (BufferLength + 1) * sizeof(TCHAR));
		GetEnvironmentVariable(BufferData, v5, BufferLength + 1);
		HANDLE  EventHandle = 0;
		//接收CommandLine的数据实际上是单字的
		_stscanf_s(v5, _T("%d"), &EventHandle);
		if (EventHandle != NULL)
		{
			if (SetEvent(EventHandle) == FALSE)
			{
				_tprintf(_T("%d"), GetLastError());
			}

		}
		Sleep(5000);
		if (EventHandle != NULL)
		{
			CloseHandle(EventHandle);
		}
		if (v5 != NULL)
		{
			delete[]v5;
		}
		ExitProcess(0);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		int LastError = GetExceptionCode();
	}
}