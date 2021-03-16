#pragma once

#pragma warning(disable : 4473)
#pragma warning(disable : 6064)

#include <Windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;
#define  JOB_OBJECT_COMPLETE_KEY  100
#define  EXIT_THREAD_COMPLETE_KEY 101

DWORD WINAPI ThreadProcedure(LPVOID ParameterData);
BOOL __IsLoop = TRUE;
HANDLE __CompletionPortHandle = INVALID_HANDLE_VALUE;
