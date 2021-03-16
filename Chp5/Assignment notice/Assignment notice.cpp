#include "Assignment notice.h"

// ��ҵ֪ͨ

int _tmain() {

    JOBOBJECT_ASSOCIATE_COMPLETION_PORT CompleteWindow;
    JOBOBJECT_BASIC_UI_RESTRICTIONS     JobObject;
    HANDLE ThreadHandle = INVALID_HANDLE_VALUE;
    HANDLE JobHandle    = INVALID_HANDLE_VALUE;
	TCHAR  Choose		= NULL;

    // ����һ����ɴ���
    __CompletionPortHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, NULL);
    if (__CompletionPortHandle == NULL) {
        goto Exit;
    }
    // ������ҵ
    JobHandle = CreateJobObject(NULL, NULL);
    if (JobHandle == NULL) {
        goto Exit;
    }
    // ��ҵ����ɴ��ڹ���
    CompleteWindow.CompletionKey  = (PVOID)JOB_OBJECT_COMPLETE_KEY;
    CompleteWindow.CompletionPort = __CompletionPortHandle;
    if (SetInformationJobObject(JobHandle, JobObjectAssociateCompletionPortInformation,
        &CompleteWindow, sizeof(JOBOBJECT_ASSOCIATE_COMPLETION_PORT)) == FALSE) {
        goto Exit;
    }
    // ����һ���غ�����ɶ˿��ϵ��߳�
	ThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProcedure, NULL, 0, NULL);
	if (ThreadHandle == NULL) {
		goto Exit;
	}
	// ����
	JobObject.UIRestrictionsClass = JOB_OBJECT_UILIMIT_NONE;  //��ʼ��
	JobObject.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_EXITWINDOWS;
	JobObject.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_HANDLES;
	SetInformationJobObject(JobHandle, JobObjectBasicUIRestrictions,
		&JobObject, sizeof(JOBOBJECT_BASIC_UI_RESTRICTIONS));

	// ѡ��
	do {
		_tprintf(_T("1..Create Process In Job\r\n"));
		_tprintf(_T("2..Remove Process From Job\r\n"));
		_tprintf(_T("3..Query Restrictions\r\n"));
		_tprintf(_T("4..Exit\r\n"));
		_tprintf(_T("Please Enter Your Choice: "));
		_tscanf_s(_T(" %c"), &Choose);
		// �ж�
		switch (Choose) {
		case '1':
		{
			STARTUPINFO StartupInfo;
			memset(&StartupInfo, 0, sizeof(STARTUPINFO));
			StartupInfo.cb = sizeof(STARTUPINFO);
			PROCESS_INFORMATION ProcessInfo;
			memset(&ProcessInfo, 0, sizeof(PROCESS_INFORMATION));
			TCHAR CommandLine[] = _T("Notepad.exe");
			BOOL IsOk = CreateProcess(NULL, CommandLine, NULL, NULL,
				FALSE, CREATE_SUSPENDED | CREATE_NEW_CONSOLE | CREATE_BREAKAWAY_FROM_JOB,
				NULL, NULL, &StartupInfo, &ProcessInfo);
			AssignProcessToJobObject(JobHandle, ProcessInfo.hProcess);
			ResumeThread(ProcessInfo.hThread);
			CloseHandle(ProcessInfo.hThread);
			CloseHandle(ProcessInfo.hProcess);
		break;
		}
		case '2':
			TerminateJobObject(JobHandle, 0);   //������ҵ
			break;
		case '3':
			QueryInformationJobObject(JobHandle, JobObjectBasicUIRestrictions,
				&JobObject, sizeof(JobObject), NULL);
			_tprintf(_T("%x\n"), JobObject.UIRestrictionsClass);
			break;
		case '4':
			PostQueuedCompletionStatus(__CompletionPortHandle,
				0, EXIT_THREAD_COMPLETE_KEY, NULL);  //�þ����������� ���Կ���
			goto Exit;
		}
		_tprintf(_T("Continue\n"));
		_tscanf_s(_T(" %c"), &Choose);
		_tprintf(_T("********************************\n"));
	} while (Choose == 'y' || Choose == 'Y');

Exit:
	if (__CompletionPortHandle != NULL)
	{
		CloseHandle(__CompletionPortHandle);
		__CompletionPortHandle = NULL;
	}
	if (JobHandle != NULL)
	{
		TerminateJobObject(JobHandle, 0);   //������ҵ
		CloseHandle(JobHandle);
		JobHandle = NULL;
	}
	if (ThreadHandle != NULL)
	{
		WaitForSingleObject(ThreadHandle, INFINITE);
		CloseHandle(ThreadHandle);
		ThreadHandle = NULL;
	}
	_tprintf(_T("Input AnyKey To Exit\r\n"));
	Choose = _gettchar();
    return 0;
}

DWORD WINAPI ThreadProcedure(LPVOID ParameterData) {
	DWORD  TransferredDataLength = 0;
	ULONG_PTR CompletionKey = 0;
	LPOVERLAPPED Overlapped;
	BOOL IsOk = FALSE;
	while (__IsLoop)
	{
		IsOk = GetQueuedCompletionStatus(__CompletionPortHandle, &TransferredDataLength,
			&CompletionKey, &Overlapped, INFINITE);
		if (IsOk == TRUE)
		{
			if (CompletionKey == JOB_OBJECT_COMPLETE_KEY)
			{
				switch (TransferredDataLength)
				{
					//��ҵ֪ͨ
				case JOB_OBJECT_MSG_NEW_PROCESS:
				{
					_tprintf(_T("New Process Identify:%d In Job\r\n"), (int)Overlapped);
					break;
				}
				case JOB_OBJECT_MSG_EXIT_PROCESS:
				{
					_tprintf(_T("New Process Identify:%d Out Job\r\n"), (int)Overlapped);   //�����Ѿ�����
					break;
				}
				case JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO:
				{
					_tprintf(_T("Active Procss Is Zero\r\n"));
				}
				default:
				{
				}
				}
			}
			else if (CompletionKey == EXIT_THREAD_COMPLETE_KEY)
			{
				_tprintf(_T("EXIT_THREAD_COMPLETE_KEY\r\n"));
				break;
			}
		}
		else
		{
			_tprintf(_T("CloseHandle(__CompletionPortHandle)\r\n"));
			break;
		}
	}
	_tprintf(_T("ThreadProcedure() Exit\r\n"));
	return 0;
}