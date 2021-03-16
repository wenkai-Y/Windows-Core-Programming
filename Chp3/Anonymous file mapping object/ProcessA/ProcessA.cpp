
#include "ProcessA.h"



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

	HANDLE		MappingHandle = NULL;
	ULONG_PTR	VirtualAddress = NULL;

	__try
	{

		//����һ�������ڴ�ӳ�����
		if (SeCreateMemroyMappingEx(ACCESS_WRITE, 0, 0x1000, NULL, &MappingHandle, &VirtualAddress) == FALSE)
		{
			goto Exit;
		}
		else
		{
			//��õ�ǰ����ID
			_tprintf(_T("��ǰ����Identify:%d\r\n"), GetCurrentProcessId());
			_tprintf(_T("��ǰ���̴���(SectionObject)���ں˶�������%d\r\n"), MappingHandle);
			_tprintf(_T("����Process(B)����\r\n"));
			memcpy((LPVOID)VirtualAddress, _T("�ҽ����ң���������"), sizeof(TCHAR) * (_tcslen(_T("�ҽ����ң���������")) + 1));

			_tprintf(_T("Input AnyKey To Continue\r\n"));
			_gettchar();
			_tprintf(_T("%s\r\n"), (TCHAR*)VirtualAddress);

		}


	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		SetLastError(GetExceptionCode());
	}


Exit:

	if (MappingHandle != NULL && VirtualAddress != NULL)
	{
		SeUnmapMemoryEx(MappingHandle, VirtualAddress);
	}


	return;
}
BOOL SeCreateMemroyMappingEx(DWORD ReadOrWrite, DWORD MaximumSizeHigh,
	DWORD MaximumSizeLow, LPCTSTR ObjectName, LPHANDLE MappingHandle, ULONG_PTR* VirtualAddress)
{
	DWORD DesiredAccess = 0;
	DWORD Protect = 0;
	HANDLE v1 = NULL;
	LPVOID v5 = NULL;
	int    LastError = 0;

	if (MappingHandle == NULL || VirtualAddress == NULL)
	{
		LastError = ERROR_INVALID_PARAMETER;

		goto Exit;
	}
	if (ReadOrWrite == ACCESS_READ)
	{
		Protect = PAGE_READONLY;
		DesiredAccess = SECTION_MAP_READ;

	}
	else if (ReadOrWrite == ACCESS_WRITE)
	{

		Protect = PAGE_READWRITE;
		DesiredAccess = SECTION_MAP_READ | SECTION_MAP_WRITE;
	}
	else
	{
		LastError = ERROR_INVALID_PARAMETER;

		goto Exit;
	}

	__try
	{

		v1 = CreateFileMapping(INVALID_HANDLE_VALUE,
			NULL, Protect, MaximumSizeHigh, MaximumSizeLow, ObjectName);
		if (v1 != NULL)
		{
			//�������óɹ�	
			*MappingHandle = v1;


			//ͨ�������ȡ�����ַ
			v5 = MapViewOfFile(v1, DesiredAccess, 0, 0, 0);

			if (v5 != NULL)
			{
				(*VirtualAddress) = (ULONG_PTR)v5;

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


