#pragma once
#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <vector>
#include <atlstr.h>


#define SYSTEM_ENVIRONMENT	_T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment")
#define USER_ENVIRONMENT	_T("Environment")

VOID ReleaseSource(vector<CString>& ParameterData);
BOOL SeRegEnumKey
(   vector<CString>& KeyVector
  , vector<CString>& KeyValueVector
  , HKEY MainKey, const TCHAR* SubKey
  , SIZE_T SubKeyLength);

using namespace std;