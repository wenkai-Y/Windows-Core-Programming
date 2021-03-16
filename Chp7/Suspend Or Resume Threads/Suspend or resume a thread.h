#pragma once
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <TlHelp32.h>

using namespace std;

/* get ImageName */
BOOL YekGetProcessImageName(OUT TCHAR* ProcessImageName, IN const UINT ImageNameLength = MAX_PATH);

/* get process identify */
BOOL YekGetProcessIdentifyFromImageName(IN const TCHAR* ImageName, OUT HANDLE* ProcessIdentify);

/* suspend or resume threads */
BOOL YeksuspendOrResumeThreads(IN DWORD ProcessIdentify, IN bool Flage = true);

/* close the handle */
bool YekCloseHandle(HANDLE myHandle);