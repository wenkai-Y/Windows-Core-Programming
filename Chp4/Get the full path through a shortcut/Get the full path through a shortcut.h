#pragma once

#include <Shobjidl.h>
#include <shlguid.h>
#include <windows.h>
#include <string.h>
#include <iostream>
#include <tchar.h>
using namespace std;
#pragma warning(disable : 6101)
BOOL ExpandShortcut(_In_ TCHAR* ShortcutPath, _In_ ULONG ShortcutPathLength, _Out_ TCHAR** FileFullPath);

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
NTSTATUS __ExceptionCode = STATUS_SUCCESS;
