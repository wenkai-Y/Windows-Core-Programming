#pragma once
#include <Windows.h>
#include <iostream>
#include <tchar.h>
using namespace std;

/* �߳���Ļ */
VOID InsideThreads();
VOID ShowData();
DWORD WINAPI ThreadFuntion(LPVOID ParameterData);
/* �رվ�� */
bool YekCloseHandle(HANDLE myHandle);