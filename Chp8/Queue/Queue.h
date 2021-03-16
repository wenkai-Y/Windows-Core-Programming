#pragma once
#pragma warning(disable : 28251)


#include "../CommonFiles/CmnHdr.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <StrSafe.h>
#include <windowsx.h>
#include "resource.h"
#include "CQueue.h"
using namespace std;


// Funtion
INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam);
void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify);

// Threads
DWORD WINAPI ClientThread(PVOID pvParam);
DWORD WINAPI ServerThread(PVOID pvParam);

// close handle
bool YekCloseHandle(HANDLE myHandle);