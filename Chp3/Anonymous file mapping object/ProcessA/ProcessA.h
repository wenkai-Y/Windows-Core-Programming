#pragma once
#ifndef __PROCESS_A_H__
#define __PROCESS_A_H__

#include <windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;



typedef enum _OPERATION_ {
    ACCESS_READ  = 0,
    ACCESS_WRITE = 1
}OPERATION, * POPERATION;


void  Sub_1();

BOOL SeCreateMemroyMappingEx(DWORD ReadOrWrite, DWORD MaximumSizeHigh, DWORD MaximumSizeLow, LPCTSTR ObjectName, LPHANDLE MappingHandle, ULONG_PTR* VirtualAddress);
void SeUnmapMemoryEx(HANDLE MappingHandle, ULONG_PTR VirtualAddress);
bool SeCloseHandle(HANDLE HandleValue);





#endif