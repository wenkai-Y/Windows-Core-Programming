#include "The registry.h"

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {
    setlocale(LC_ALL, "Chinese-simplified");

    vector<CString> KeyVector;
    vector<CString> KeyValueVector;
    vector<CString>::iterator v1;
    vector<CString>::iterator v2;
	try
	{
		if (argv[1] == NULL)
		{
			_tprintf(_T("请以CMD命令运行该程序并传入正确的参数\r\n"));
			system("PAUSE");
		}

		if (lstrcmpi(argv[1], _T("System")) == 0)
		{
			if (SeRegEnumKey(KeyVector, KeyValueVector,
				HKEY_LOCAL_MACHINE, SYSTEM_ENVIRONMENT,
				_tcslen(SYSTEM_ENVIRONMENT)))
			{
				_tprintf(_T("%x\r\n"), GetLastError());
			}

			for (v1 = KeyVector.begin(), v2 = KeyValueVector.begin();
				v1 != KeyVector.end() && v2 != KeyValueVector.end();
				v1++, v2++)
			{
				_tprintf(_T("%s = "), *v1);
				_tprintf(_T("%s\r\n"), *v2);
			}

			ReleaseSource(KeyVector);
			ReleaseSource(KeyValueVector);
		}
		// 用户环境变量
		else if (lstrcmpi(argv[1], _T("User")) == 0)
		{
			if (SeRegEnumKey(KeyVector, KeyValueVector,
				HKEY_CURRENT_USER, USER_ENVIRONMENT,
				_tcslen(USER_ENVIRONMENT)))
			{
				_tprintf(_T("%x\r\n"), GetLastError());
			}

			for (v1 = KeyVector.begin(), v2 = KeyValueVector.begin();
				v1 != KeyVector.end() && v2 != KeyValueVector.end();
				v1++, v2++)
			{
				_tprintf(_T("%s = "), *v1);
				_tprintf(_T("%s\r\n"), *v2);
			}

			ReleaseSource(KeyVector);
			ReleaseSource(KeyValueVector);
		}
		else
		{

		}



	}
	catch (...)
	{

	}
Exit:
	return 0;

	_tprintf(_T("Input AnyKey To Exit\r\n"));
	_gettchar();

	return 0;

    return 0;
}

VOID ReleaseSource(vector<CString>& ParameterData) {
	vector<CString>::iterator i;
	for (i = ParameterData.begin(); i != ParameterData.end(); i++)
	{
		try
		{
			(*(i)).ReleaseBuffer();
			(*(i)).Empty();
		}
		catch (...)
		{
			cout << "Throw Exception!" << endl;
		}
	}
}

BOOL SeRegEnumKey
(vector<CString>& KeyVector
    , vector<CString>& KeyValueVector
    , HKEY MainKey, const TCHAR* SubKey
    , SIZE_T SubKeyLength) {
	BOOL IsOk = TRUE;
	DWORD LastError = 0;

	HKEY KeyHandle = NULL;


	DWORD KeyIndex = 0;
	DWORD KeyCounts = 0;
	DWORD KeyLength = 0;
	DWORD KeyValueLength = 0;
	DWORD MaxKeyLength = 0;
	DWORD MaxKeyValueLength = 0;
	int Loop = 0;

	TCHAR* KeyName = NULL;
	TCHAR* KeyValue = NULL;

	if (MainKey == NULL || IsBadStringPtr(SubKey, SubKeyLength))
	{
		IsOk = FALSE;
		LastError = ERROR_INVALID_PARAMETER;
		goto Exit;
	}


	LastError = RegOpenKeyEx(MainKey, SubKey, 0, KEY_READ, &KeyHandle);

	if (LastError != ERROR_SUCCESS)
	{
		IsOk = FALSE;

		goto Exit;
	}
	LastError = RegQueryInfoKey(KeyHandle,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		&KeyCounts,
		&MaxKeyLength,
		&MaxKeyValueLength,
		NULL,
		NULL);


	if (LastError != ERROR_SUCCESS)
	{
		IsOk = FALSE;
		goto Exit;
	}

	if (KeyCounts > 0)
	{
		for (; KeyIndex <= KeyCounts; KeyIndex++)
		{
			KeyLength = MaxKeyLength + sizeof(TCHAR);
			KeyValueLength = MaxKeyValueLength + sizeof(TCHAR);

			CString Key;
			CString KeyValue;
			LastError = RegEnumValue(KeyHandle,
				KeyIndex,
				Key.GetBuffer(KeyLength),
				&KeyLength,
				NULL,
				NULL,
				(LPBYTE)KeyValue.GetBuffer(KeyValueLength),
				&KeyValueLength);

			if (LastError != ERROR_SUCCESS)
			{
				IsOk = FALSE;
				goto Exit;
			}

			KeyVector.push_back(Key);
			KeyValueVector.push_back(KeyValue);
		}
	}

Exit:
	if (KeyHandle)
	{
		CloseHandle(KeyHandle);
	}
	SetLastError(LastError);
	return IsOk;
}