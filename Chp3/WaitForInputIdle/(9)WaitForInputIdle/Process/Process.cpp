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
	SecurityAttributes.bInheritHandle = TRUE;   //��ʼ��SD

	HANDLE EventHandle = CreateEvent(&SecurityAttributes, TRUE, FALSE, NULL);  //�����ں˶���̳�
	if (EventHandle != NULL)
	{

	}
	else
	{
		_tprintf(_T("CreateEvent() Error\r\n"));
		goto  Exit;
	}

	//����һ���ӽ���
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
			IsOk = WaitForInputIdle(ProcessInfo.hProcess, 1000);  //���ӽ���һ����ʼ����ʱ��
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
				//�ӽ��̳�ʼ�����
				//ͨ�����ڱ������ƻ�ô��ھ��
				HWND v1 = FindWindow(NULL, _T("Dialog"));

				if (v1 == NULL)
				{
					goto Exit;
				}
				//��Ŀ�괰�ڷ����Զ�����Ϣ
				SendMessage(v1, UM_SEND, NULL, (LPARAM)EventHandle);

				WaitForSingleObject(EventHandle, INFINITE);
				
				_tprintf(_T("Ϊ�������\r\n"));

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