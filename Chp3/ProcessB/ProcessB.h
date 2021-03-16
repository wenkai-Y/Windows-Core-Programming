#ifndef __PROCESSA_H__
#define __PROCESSA_H__

/* disable warning */
#pragma warning(disable : 6031)
#pragma warning(disable : 6101)
#pragma warning(disable : 26812)
//#pragma warning(disable : 28251)
//#pragma warning(disable : 26812)

#include <windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;

typedef enum _OPERATION_
{
	ACCESS_READ = 1,
	ACCESS_WRITE = 2,
}OPERATION, * POPERATION;
void Sub_1();
BOOL SeOpenMemoryMappingEx(OPERATION ReadOrWrite, BOOL IsInheritHandle, LPCTSTR ObjectName, _Out_ LPHANDLE MappingHandle, _Out_ ULONG_PTR* VirtualAddress);
void SeUnmapMemoryEx(_In_ HANDLE MappingHandle, _In_ ULONG_PTR VirtualAddress);
bool SeCloseHandle(HANDLE HandleValue);


#endif