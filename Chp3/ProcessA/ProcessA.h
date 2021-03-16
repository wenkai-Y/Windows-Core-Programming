#ifndef __PROCESSA_H__
#define __PROCESSA_H__

/* disable warning */
#pragma warning(disable : 6031)
#pragma warning(disable : 6101)
#pragma warning(disable : 26812)
//#pragma warning(disable : 26812)


#include <windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;

/* data */
typedef enum _OPERATION_
{
	ACCESS_READ  = 1,
	ACCESS_WRITE = 2,
	//ACCESS_ALL	 = 3,
}OPERATION, * POPERATION;



/* funtion */
void Sub_1();

// �����ڴ�ӳ��
BOOL YWKCreateMemroyMappingEx
	(_In_ const OPERATION ReadOrWrite, _In_ const DWORD MaximumSizeHigh,
	 _In_ const DWORD MaximumSizeLow, _In_ const LPCTSTR ObjectName,
	 _Out_ LPHANDLE MappingHandle, _Out_ ULONG_PTR* VirtualAddress);

// ɾ���ڴ�ӳ��
void YWKUnmapMemoryEx(_In_ HANDLE MappingHandle, _In_ ULONG_PTR VirtualAddress);

// ɾ�����
BOOL YWKCloseHandle(_In_ const HANDLE HandleValue);



#endif