#ifndef __EnumeratesHandleInformationInAProcess__
#define __EnumeratesHandleInformationInAProcess__

#pragma warning(disable : 4302)
#pragma warning(disable : 6001)
#pragma warning(disable : 6031)
#pragma warning(disable : 6387)

#define STATUS_SUCCESS                  (NTSTATUS)0x00000000
#define STATUS_INFO_LENGTH_MISMATCH     ((NTSTATUS)0xC0000004L)

#include <tchar.h>
#include <windows.h>
#include <Tlhelp32.h>
#include <Winternl.h>
#include <vector>
#include <iostream>
//#include <ntstatus.h>
using namespace std;

// Data
typedef struct _HANDLE_INFORMATION_
{
    USHORT HandleValue;
    PVOID  Object;
    UCHAR  ObjectType;
}HANDLE_INFORMATION, * PHANDLE_INFORMATION;

namespace _OBJECT_HELPER_
{

	typedef enum _SYSTEM_INFORMATION_CLASS {
		SystemBasicInformation,
		SystemProcessorInformation,
		SystemPerformanceInformation,
		SystemTimeOfDayInformation,
		SystemPathInformation,
		SystemProcessInformation,
		SystemCallCountInformation,
		SystemDeviceInformation,
		SystemProcessorPerformanceInformation,
		SystemFlagsInformation,
		SystemCallTimeInformation,
		SystemModuleInformation,
		SystemLocksInformation,
		SystemStackTraceInformation,
		SystemPagedPoolInformation,
		SystemNonPagedPoolInformation,
		SystemHandleInformation,	//16
		SystemObjectInformation,
		SystemPageFileInformation,
		SystemVdmInstemulInformation,
		SystemVdmBopInformation,
		SystemFileCacheInformation,
		SystemPoolTagInformation,
		SystemInterruptInformation,
		SystemDpcBehaviorInformation,
		SystemFullMemoryInformation,
		SystemLoadGdiDriverInformation,
		SystemUnloadGdiDriverInformation,
		SystemTimeAdjustmentInformation,
		SystemSummaryMemoryInformation,
		SystemNextEventIdInformation,
		SystemEventIdsInformation,
		SystemCrashDumpInformation,
		SystemExceptionInformation,
		SystemCrashDumpStateInformation,
		SystemKernelDebuggerInformation,
		SystemContextSwitchInformation,
		SystemRegistryQuotaInformation,
		SystemExtendServiceTableInformation,
		SystemPrioritySeperation,
		SystemPlugPlayBusInformation,
		SystemDockInformation,
#if !defined PO_CB_SYSTEM_POWER_POLICY
		SystemPowerInformation,
#else
		_SystemPowerInformation,
#endif
		SystemProcessorSpeedInformation,
		SystemCurrentTimeZoneInformation,
		SystemLookasideInformation
	} SYSTEM_INFORMATION_CLASS;

	typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO
	{
		USHORT UniqueProcessId;
		USHORT CreatorBackTraceIndex;
		UCHAR ObjectTypeIndex;
		UCHAR HandleAttributes;
		USHORT HandleValue;
		PVOID Object;
		ULONG GrantedAccess;
	} SYSTEM_HANDLE_TABLE_ENTRY_INFO, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

	typedef struct _SYSTEM_HANDLE_INFORMATION
	{
		ULONG NumberOfHandles;
		SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
	} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;


};

// function

void Sub_1();

// 获取进程ID
BOOL YWKGetProcessIdentify(_Out_ HANDLE* ProcessIdentify, _In_ ULONG_PTR ProcessIdentifyLength, 
    _In_ const TCHAR* ProcessImageName, _In_ ULONG_PTR ProcessImageNameLength);

// From Id to 获得当前进程句柄信息
// Ntdll.dll
BOOL YWKEnumHandleInfo(
	_Out_ vector<HANDLE_INFORMATION>& HandleInfoVector, _In_ HANDLE ProcessIdentify
);

//typedef _Return_type_success_(return >= 0) LONG NTSTATUS;
//#define NTAPI __stdcall
typedef NTSTATUS (NTAPI* LPFN_NTQUERYSYSTEMINFORMATION)
(
    _In_  SYSTEM_INFORMATION_CLASS SystemInformationClass,
    _Out_ PVOID SystemInformation,
    _In_  ULONG Length,
    _Out_ PULONG UnsafeResultLength
);

// //typedef decltype(&NtQuerySystemInformation) LPFN_NTQUERYSYSTEMINFORMATION;


#endif
