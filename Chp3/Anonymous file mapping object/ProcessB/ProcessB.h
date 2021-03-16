#ifndef __PROCESS_B_H__
#define __PROCESS_B_H__

#include <windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;

#pragma warning(disable : 4996)


void Sub_1();
BOOL SeOpenMemroyMapping(DWORD ReadOrWrite, BOOL IsInheritHandle, LPCTSTR ObjectName, LPHANDLE MappingHandle, ULONG_PTR* VirtualAddress);
void SeUnmapMemoryEx(HANDLE MappingHandle, ULONG_PTR VirtualAddress);
bool SeCloseHandle(HANDLE HandleValue);
DWORD SeEnableSeDebugPrivilege(HANDLE ProcessHandle, BOOL IsEnable);
HANDLE SeOpenProcess(DWORD DesiredAccess, BOOL IsInheritHandle, HANDLE ProcessIdentify);

#endif