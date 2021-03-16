#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include "resource.h"
using namespace std;


LRESULT CALLBACK CallBackRoutine(HWND Hwnd,
	UINT Message, WPARAM ParameterData1, LPARAM ParameterData2);

