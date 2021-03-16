#include "ProcessB.h"


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	setlocale(LC_ALL, "chs");

	_tprintf(_T("This is process B.\r\n"));
	Sub_1();   //命名对象
	_tprintf(_T("Input AnyKey To Exit\r\n"));
	_gettchar();
	return 0;
}

void Sub_1()
{
	HANDLE		MappingHandle = NULL;
	ULONG_PTR	VirtualAddress = NULL;
	DWORD       LastError = 0;

	if (SeOpenMemoryMappingEx(ACCESS_WRITE, FALSE, _T("Yuwenkai"), &MappingHandle, &VirtualAddress) == FALSE)
	{
		GetLastError();
	}
	else
	{

		__try
		{

			_tprintf(_T("%s\r\n"), (TCHAR*)VirtualAddress);
			memcpy((LPVOID)VirtualAddress, _T("为人民服务"), sizeof(TCHAR) * (_tcslen(_T("为人民服务")) + 1));
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LastError = GetExceptionCode();
		}

	}
	SeUnmapMemoryEx(MappingHandle, VirtualAddress);
	return;
}

BOOL SeOpenMemoryMappingEx(OPERATION ReadOrWrite, BOOL IsInheritHandle,
	LPCTSTR ObjectName, _Out_ LPHANDLE MappingHandle, _Out_ ULONG_PTR* VirtualAddress)
{
	DWORD DesiredAccess = 0;
	HANDLE v1 = NULL;
	LPVOID v5 = NULL;
	int    LastError = 0;


	if (ObjectName == NULL || MappingHandle == NULL || VirtualAddress == NULL)
	{
		LastError = ERROR_INVALID_PARAMETER;

		goto Exit;
	}
	if (ReadOrWrite == ACCESS_READ)
	{

		DesiredAccess = SECTION_MAP_READ;

	}
	else if (ReadOrWrite == ACCESS_WRITE)
	{


		DesiredAccess = SECTION_MAP_READ | SECTION_MAP_WRITE;
	}
	else
	{
		LastError = ERROR_INVALID_PARAMETER;

		goto Exit;
	}



	__try
	{
		v1 = OpenFileMapping(DesiredAccess, IsInheritHandle, ObjectName);
		if (v1 != NULL)
		{

			*MappingHandle = v1;

			v5 = MapViewOfFile(v1, DesiredAccess, 0, 0, 0);

			if (v5 != NULL)
			{
				(*VirtualAddress) = (ULONG_PTR)v5;
				_tprintf(_T("%s"), (TCHAR*)VirtualAddress);
				return TRUE;
			}
		}

	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LastError = GetExceptionCode();

		goto Exit;
	}

Exit:
	SetLastError(LastError);
	return FALSE;
}


void SeUnmapMemoryEx(_In_ HANDLE MappingHandle, _In_ ULONG_PTR VirtualAddress)
{
	__try
	{
		if (UnmapViewOfFile((void*)VirtualAddress))
		{
			if (SeCloseHandle(MappingHandle)) {
				OutputDebugString(_T("Close Handle Success.\r\n"));
			}
		}

	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		SetLastError(GetExceptionCode());
	}

}

bool SeCloseHandle(HANDLE HandleValue)
{
	DWORD HandleFlags(-1);
	if (GetHandleInformation(HandleValue, &HandleFlags)
		&& (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
		return !!CloseHandle(HandleValue);
	return false;
}
