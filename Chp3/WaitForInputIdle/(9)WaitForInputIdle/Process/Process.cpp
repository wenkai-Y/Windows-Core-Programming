#include "Process.h"

#define  UM_SEND  WM_USER + 0x1
void _tmain(int argc, TCHAR* argv[], TCHAR *envp[])
{
	setlocale(LC_ALL,"Chinese-simplified");
	Sub_1();


	_tprintf(_T("Input AnyKey To Exit\r\n"));

	_gettchar();
	return;
}

void Sub_1()
{

	SECURITY_ATTRIBUTES SecurityAttributes;
	SecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	SecurityAttributes.lpSecurityDescriptor = NULL;
	SecurityAttributes.bInheritHandle = TRUE;   //初始化SD

	HANDLE EventHandle = CreateEvent(&SecurityAttributes, TRUE, FALSE, NULL);  //允许内核对象继承
	if (EventHandle != NULL)
	{

	}
	else
	{
		_tprintf(_T("CreateEvent() Error\r\n"));
		goto  Exit;
	}

	//创建一个子进程
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION  ProcessInfo;

	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(STARTUPINFO);
	
	
	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));


	int IsOk = TRUE;

	IsOk = CreateProcess(_T("SubProcess.exe"),
		NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, 
		NULL, NULL, &StartupInfo, &ProcessInfo);

	if (IsOk != 0)
	{
		while (1)
		{
			IsOk = WaitForInputIdle(ProcessInfo.hProcess, 1000);  //给子进程一个初始化的时间
			if (IsOk == WAIT_FAILED)
			{
				goto Exit;
			}
			else if (IsOk == WAIT_TIMEOUT)
			{
				continue;
			}
			else
			{
				//子进程初始化完成
				//通过窗口标题名称获得窗口句柄
				HWND v1 = FindWindow(NULL, _T("Dialog"));

				if (v1 == NULL)
				{
					goto Exit;
				}
				//向目标窗口发送自定义消息
				SendMessage(v1, UM_SEND, NULL, (LPARAM)EventHandle);

				WaitForSingleObject(EventHandle, INFINITE);
				
				_tprintf(_T("为人民服务\r\n"));

				break;

			}

		}
		
		
	
	}

Exit:
	if (ProcessInfo.hThread)
	{
		CloseHandle(ProcessInfo.hThread);
	}

	if (ProcessInfo.hProcess)
	{
		CloseHandle(ProcessInfo.hProcess);
	}
	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));

	if (EventHandle)
	{
		CloseHandle(EventHandle);
		EventHandle = NULL;
	}
}