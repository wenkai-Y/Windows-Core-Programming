#include "Get the full path through a shortcut.h"

int _tmain() {

	_tsetlocale(LC_ALL, _T("Chinese-simplified"));//使当前程序支持中文
    TCHAR ShortCutPath[] = _T("D:\\Users\\86187\\Desktop\\ErrorShow.exe.lnk");
    TCHAR* FileFullPath = NULL;
    if (_tcsstr(ShortCutPath, _T(".lnk")) != NULL) {
        if (ExpandShortcut(ShortCutPath, _tcslen(ShortCutPath), &FileFullPath)) {
            _tprintf(_T("%s\r\n"), FileFullPath);
        }
        else {
            _tprintf(_T("ExpandShortcut():: Error\r\n"));
        }
    }
    if (FileFullPath) {
        free(FileFullPath);
        FileFullPath = NULL;
    }
    return 0;
}

BOOL ExpandShortcut(_In_ TCHAR* ShortcutPath, _In_ ULONG ShortcutPathLength, _Out_ TCHAR** FileFullPath) {

	IShellLink* v1 = NULL;		//用来接收指向Com对象接口地址的指针变量
	IPersistFile* v2 = NULL;
	HRESULT   ResultHandle = S_FALSE;
	WCHAR* v5 = NULL;
	ULONG_PTR v7 = 0;

	if (!SUCCEEDED(CoInitialize(NULL)))
	{
		return FALSE;
	}
	//用指定的类标识符创建一个未初始化的对象
	ResultHandle = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&v1);
	if (SUCCEEDED(ResultHandle))
	{
		ResultHandle = v1->QueryInterface(IID_IPersistFile, (void**)&v2);
		if (SUCCEEDED(ResultHandle))
		{

#ifdef _UNICODE	

			v5 = ShortcutPath;
			v7 = ShortcutPathLength;

#else		

			//转换char成为wchar
			__try
			{

				v7 = (size_t)MultiByteToWideChar(CP_ACP, 0,
					ShortcutPath, int(ShortcutPathLength), NULL, 0);

				if (v7 == 0)
				{
					goto Exit;
				}
				v5 = (WCHAR*)malloc(sizeof(WCHAR) * (v7 + 1));


				if (v5 == NULL)
				{
					goto Exit;
				}
				ZeroMemory(v5, sizeof(WCHAR) * (v7 + 1));

				v7 = (size_t)MultiByteToWideChar(CP_ACP, 0, ShortcutPath,
					ShortcutPathLength, v5, sizeof(WCHAR) * v7);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				__ExceptionCode = GetExceptionCode();
				SetLastError(__ExceptionCode);

				goto Exit;
			}

#endif
			ResultHandle = v2->Load(v5, STGM_READ);
			/*Load()函数的参数一必须为双字:
			typedef LPCWSTR LPCOLESTR; 一个32位的指向unicode字符串常量的指针*/
			if (SUCCEEDED(ResultHandle))
			{
				TCHAR  v3[MAX_PATH] = { 0 };
				TCHAR  v4[MAX_PATH] = { 0 };
				BOOL   IsOk = FALSE;
				ResultHandle = v1->GetPath(v3, ARRAYSIZE(v3), NULL, SLGP_RAWPATH);	//这个参数通常通过调用ARRAYSIZE(pszFile)来设置
				IsOk = !!ExpandEnvironmentStrings(v3, v4, ARRAYSIZE(v4));

				if (SUCCEEDED(ResultHandle) && IsOk)
				{
					v7 = _tcslen(v4);
					//申请内存
					*FileFullPath = (TCHAR*)malloc(sizeof(TCHAR) * (v7 + 1));

					if (*FileFullPath == NULL)
					{
						goto Exit;
					}
					else {

						ZeroMemory(*FileFullPath, sizeof(TCHAR) * (v7 + 1));
						memcpy(*FileFullPath, v4, v7 * sizeof(TCHAR));
					}
				}
			}
			if (v2)
			{
				v2->Release();
			}
		}
		//Release the pointer to the IShellLink interface.
		if (v1)
		{
			v1->Release();
		}
	}
Exit:
#ifndef _UNICODE
	if (v5)
	{
		free(v5);
		v5 = NULL;
	}
#endif // !_UNICODE
	//Uninitialize COM stuff
	CoUninitialize();
	return SUCCEEDED(ResultHandle);
}