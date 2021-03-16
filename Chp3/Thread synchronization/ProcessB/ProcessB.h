#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;

#pragma warning(disable : 6031)
#pragma warning(disable : 4996)


/* 命名事件 */
void Sub_1();
/* 命名互斥体 */
void Sub_2();
/* 命名信号量 */
void Sub_3();
/* 匿名互斥体 */
void Sub_4();




/* 关闭事件 */
bool YekCloseHandle(const HANDLE& Handle) {
    DWORD HandleFlags;
    if (GetHandleInformation(Handle, &HandleFlags) && (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
        return !!CloseHandle(Handle);
    return false;
}

BOOL __EnableDebugPrivilege = TRUE;

BOOL SeEnableSeDebugPrivilege(HANDLE ProcessHandle, BOOL IsEnable)
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

/* 打开进程 */
HANDLE YekOpenProcess(DWORD DesiredAccess, BOOL IsInheritHandle, HANDLE ProcessIdentify)
{

	if (__EnableDebugPrivilege)
	{
		SeEnableSeDebugPrivilege(GetCurrentProcess(), TRUE);
	}
	HANDLE ProcessHandle =
		OpenProcess(DesiredAccess,
			IsInheritHandle,
			(DWORD)ProcessIdentify);

	DWORD LastError = GetLastError();
	if (__EnableDebugPrivilege)
	{
		SeEnableSeDebugPrivilege(GetCurrentProcess(), FALSE);
	}
	SetLastError(LastError);
	return ProcessHandle;
}