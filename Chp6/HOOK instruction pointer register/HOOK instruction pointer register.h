#pragma once

#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <string>
#include <vector>
using namespace std;

#define NT_SUCCESS(x) ((x) >= 0)

#ifdef _UNICODE
#define _tstring ::std::wstring
#define _tcout ::std::wcout
#define _tcin ::std:wcin
#else
#define _tstring ::std::string
#define _tcout ::std::cout
#define _tcin ::std:cin
#endif // _UNICODE


/* Data */
typedef struct _THREAD_INFO_
{
	HANDLE ThreadIdentify;

}THREAD_INFO, * PTHREAD_INFO;

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING* PUNICODE_STRING;

typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID, * PCLIENT_ID;

typedef enum _SYSTEM_INFORMATION_CLASS {

	SystemProcessInformation = 5
} SYSTEM_INFORMATION_CLASS;

typedef LONG KPRIORITY;

typedef struct _SYSTEM_THREAD_INFORMATION
{
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG WaitTime;
	PVOID StartAddress;
	CLIENT_ID ClientId;
	KPRIORITY Priority;
	LONG BasePriority;
	ULONG ContextSwitches;
	ULONG ThreadState;
	ULONG WaitReason;
	ULONG PadPadAlignment;
} SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;

typedef struct _SYSTEM_PROCESS_INFORMATION
{
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER WorkingSetPrivateSize;
	ULONG HardFaultCount;
	ULONG NumberOfThreadsHighWatermark;
	ULONGLONG CycleTime;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ImageName;
	KPRIORITY BasePriority;
	HANDLE UniqueProcessId;
	HANDLE InheritedFromUniqueProcessId;
	ULONG HandleCount;
	ULONG SessionId;
	ULONG_PTR UniqueProcessKey;
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	ULONG PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
	SIZE_T PrivatePageCount;
	LARGE_INTEGER ReadOperationCount;
	LARGE_INTEGER WriteOperationCount;
	LARGE_INTEGER OtherOperationCount;
	LARGE_INTEGER ReadTransferCount;
	LARGE_INTEGER WriteTransferCount;
	LARGE_INTEGER OtherTransferCount;
	SYSTEM_THREAD_INFORMATION Threads[1];
}SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;

typedef
NTSTATUS(_stdcall* LPFN_NTQUERYSYSTEMINFORMATION)(
	IN SYSTEM_INFORMATION_CLASS SystemInfoClass,
	OUT PVOID SystemInfoBuffer,
	IN ULONG SystemInfoBufferSize,
	OUT PULONG BytesReturned OPTIONAL);


/* Funtion */

/* Hook指令指针寄存器 */
VOID HOOKInstructionPointerRegister();

/* 获取ImageName */
inline BOOL YekInutData(TCHAR* ProcessImageName, ULONG_PTR ProcessImageNameLength = MAX_PATH);

/* 获取进程Identify */
BOOL YekGetIdentifyFromImageName(const TCHAR* ImageName, HANDLE* ProcessIdentify);

/* 获得主线程Identify */
BOOL YekGetMainThreadIdentify(const HANDLE* ProcessIdentify);

/* 关闭句柄 */
bool YekCloseHandle(HANDLE myHandle);