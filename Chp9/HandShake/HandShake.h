#pragma once

#pragma warning(disable : 28251)
#pragma warning(disable : 6387)

#include "../CommonFiles/CmnHdr.h"
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <windowsx.h>
#include "resource.h"



DWORD WINAPI ServerThread(PVOID ParaterData);
INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);


/* ¹Ø±Õ¾ä±ú */
bool YekCloseHandle(HANDLE myHandle);



using namespace std;