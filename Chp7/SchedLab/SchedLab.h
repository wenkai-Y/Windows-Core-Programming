/******************************************************************************
Module:  SchedLab.cpp
Notices: Copyright (c) 2020 Wenkai Yus
******************************************************************************/

#pragma once
#pragma warning(disable : 28251)
#pragma warning(disable : 28183)


///////////////////////////////////////////////////////////////////////////////


#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <tchar.h>
#include <process.h>
#include <StrSafe.h>
#include "resource.h"
using namespace std;

/////////////////////////// chHANDLE_DLGMSG Macro /////////////////////////////


// The normal HANDLE_MSG macro in WindowsX.h does not work properly for dialog
// boxes because DlgProc returns a BOOL instead of an LRESULT (like
// WndProcs). This chHANDLE_DLGMSG macro corrects the problem:
#define chHANDLE_DLGMSG(hWnd, message, fn)                 \
   case (message): return (SetDlgMsgResult(hWnd, uMsg,     \
      HANDLE_##message((hWnd), (wParam), (lParam), (fn))))



typedef unsigned(__stdcall* PTHREAD_START) (void*);


#define chBEGINTHREADEX(psa, cbStackSize, pfnStartAddr, \
   pvParam, dwCreateFlags, pdwThreadId)                 \
      ((HANDLE)_beginthreadex(                          \
         (void *)        (psa),                         \
         (unsigned)      (cbStackSize),                 \
         (PTHREAD_START) (pfnStartAddr),                \
         (void *)        (pvParam),                     \
         (unsigned)      (dwCreateFlags),               \
         (unsigned *)    (pdwThreadId)))


// This macro returns TRUE if a number is between two others
#define chINRANGE(low, Num, High) (((low) <= (Num)) && ((Num) <= (High)))

////////////////////////////////// Funtion ////////////////////////////////////
INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam);

void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify);

// Sets the dialog box icons
inline void chSETDLGICONS(HWND hWnd, int idi);

DWORD WINAPI ThreadFunc(PVOID pvParam);

inline void chMB(PCSTR szMsg);


//////////////////////////////// End of File //////////////////////////////////
