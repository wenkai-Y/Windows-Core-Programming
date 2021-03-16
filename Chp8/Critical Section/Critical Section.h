#pragma once
#include <Windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;


void Sub_1();
void Sub_2();
void Sub_3();

DWORD WINAPI ThreadProc1(LPVOID lpParameter = NULL);
DWORD WINAPI ThreadProc2(LPVOID lpParameter = NULL);
DWORD WINAPI ThreadProc3(LPVOID lpParameter = NULL);
DWORD WINAPI ThreadProc4(LPVOID lpParameter = NULL);
DWORD WINAPI ThreadProc5(LPVOID lpParameter = NULL);
DWORD WINAPI ThreadProc6(LPVOID lpParameter = NULL);

bool YekCloseHandle(HANDLE myHandle);