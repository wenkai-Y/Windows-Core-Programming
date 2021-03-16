#pragma once
#pragma warning(disable : 6386)

#include <iostream>
#include <tchar.h>
#include <windows.h>
using namespace std;



VOID YekGetFullPathName(_In_ const TCHAR* argv, _Out_ TCHAR* BufferPath);