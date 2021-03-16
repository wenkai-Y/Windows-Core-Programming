#pragma once
#include <Windows.h>
#include <iostream>
#include <tchar.h>
using namespace std;

/* 线程内幕 */
VOID InsideThreads();
VOID ShowData();
DWORD WINAPI ThreadFuntion(LPVOID ParameterData);
/* 关闭句柄 */
bool YekCloseHandle(HANDLE myHandle);