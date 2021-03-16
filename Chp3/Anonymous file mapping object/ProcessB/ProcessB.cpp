
#include "ProcessB.h"

BOOL __EnableDebugPrivilege = TRUE;
void _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	setlocale(LC_ALL, "chs");

	Sub_1();
	_tprintf(_T("Input AnyKey To Exit\r\n"));
	_gettchar();
	_gettchar();
	return;
}

void Sub_1()
{
	HANDLE      ProcessIdentify = 0;
	HANDLE		MappingHandle = NULL;
	ULONG_PTR	VirtualAddress = NULL;
	HANDLE      v1 = NULL;
	HANDLE      ProcessHandle = NULL;

	_tprintf(_T("输入Process(A)进程Identify与内核对象的句柄值\r\n"));

	__try
	{

		//如何知道进程
		_tscanf(_T("%d"), &ProcessIdentify);
		_tscanf(_T("%d"), &MappingHandle);


		//根据目标进程的ID打开目标进程获取进程句柄
		ProcessHandle = SeOpenProcess(PROCESS_DUP_HANDLE, FALSE, ProcessIdentify);
		if (ProcessHandle == NULL)
		{
			goto Exit;
		}

		//Anti策略
		BOOL IsOk = DuplicateHandle(ProcessHandle, MappingHandle,
			GetCurrentProcess(), &v1, 0, FALSE, DUPLICATE_SAME_ACCESS);

		if ((VirtualAddress = (ULONG_PTR)MapViewOfFile(v1, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0)) == NULL)
		{
			int LastError = GetLastError();
			goto Exit;
		}
		else
		{
			_tprintf(_T("%s\r\n"), (TCHAR*)VirtualAddress);
			memcpy((LPVOID)VirtualAddress, _T("为人民服务"), sizeof(TCHAR) * (_tcslen(_T("为人民服务")) + 1));

		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		SetLastError(GetExceptionCode());
	}
Exit:
	if (ProcessHandle != NULL)
	{
		SeCloseHandle(ProcessHandle);
	}
	if (v1 != NULL)
	{
		SeUnmapMemoryEx(v1, VirtualAddress);
	}

	return;
}

void SeUnmapMemoryEx(HANDLE MappingHandle, ULONG_PTR VirtualAddress)
{
	__try
	{
		if (UnmapViewOfFile((void*)VirtualAddress))
		{
			SeCloseHandle(MappingHandle);
		}

	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		SetLastError(GetExceptionCode());
	}

}
bool SeCloseHandle(HANDLE HandleValue)
{
	DWORD HandleFlags;
	if (GetHandleInformation(HandleValue, &HandleFlags)
		&& (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
		return !!CloseHandle(HandleValue);
	return false;
}

DWORD SeEnableSeDebugPrivilege(HANDLE ProcessHandle, BOOL IsEnable)
{
	DWORD  LastError;
	HANDLE TokenHandle = 0;

	if (!OpenProcessToken(ProcessHandle, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &TokenHandle))
	{
		LastError = GetLastError();
		if (TokenHandle)
			CloseHandle(TokenHandle);
		return LastError;
	}
	TOKEN_PRIVILEGES TokenPrivileges;
	memset(&TokenPrivileges, 0, sizeof(TOKEN_PRIVILEGES));
	LUID v1;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &v1))
	{
		LastError = GetLastError();
		CloseHandle(TokenHandle);
		return LastError;
	}
	TokenPrivileges.PrivilegeCount = 1;
	TokenPrivileges.Privileges[0].Luid = v1;
	if (IsEnable)
		TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		TokenPrivileges.Privileges[0].Attributes = 0;
	AdjustTokenPrivileges(TokenHandle, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	LastError = GetLastError();
	CloseHandle(TokenHandle);
	return LastError;
}
HANDLE SeOpenProcess(DWORD DesiredAccess, BOOL IsInheritHandle, HANDLE ProcessIdentify)
{
	if (__EnableDebugPrivilege)
	{
		SeEnableSeDebugPrivilege(GetCurrentProcess(), TRUE);
	}


	HANDLE ProcessHandle = OpenProcess(DesiredAccess, IsInheritHandle, (DWORD)ProcessIdentify);

	DWORD LastError = GetLastError();



	if (__EnableDebugPrivilege)
	{
		SeEnableSeDebugPrivilege(GetCurrentProcess(), FALSE);
	}
	SetLastError(LastError);
	return ProcessHandle;
}
