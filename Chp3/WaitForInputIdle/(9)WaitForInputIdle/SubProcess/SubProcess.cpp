#include "SubProcess.h"


HANDLE __EventHandle = NULL;
#define  UM_SEND  WM_USER + 0x1
INT WINAPI _tWinMain(
	_In_ HINSTANCE Instance,
	_In_opt_ HINSTANCE PrevInstance,
	_In_ PTSTR CmdLine,
	_In_ INT CmdShow
)
{
	DialogBoxParam(Instance, 
		MAKEINTRESOURCE(IDD_DIALOG), NULL, (DLGPROC)CallBackRoutine, NULL);
	return 0;
}

LRESULT CALLBACK CallBackRoutine(HWND Hwnd,
	UINT Message, WPARAM ParameterData1, LPARAM ParameterData2)
{
	switch (Message)
	{
	case WM_COMMAND:   
	{
		DWORD v1 = LOWORD(ParameterData1);     //low word  high wrod

		switch (v1)
		{
		case IDOK:
		{
			if (__EventHandle!=NULL)
			{
				SetEvent(__EventHandle);
			}		
			break;
		}
		case IDCANCEL:
		{

			if (__EventHandle)
			{
				CloseHandle(__EventHandle);
				__EventHandle = NULL;
			}
			EndDialog(Hwnd, 0);
			break;
		}
		}
		
		break;
	}
	case WM_CLOSE:
	{
		if (__EventHandle)
		{
			CloseHandle(__EventHandle);
			__EventHandle = NULL;
		}
		EndDialog(Hwnd, 0);
		break;
	}
	case UM_SEND:   
	{
		__EventHandle = (HANDLE)ParameterData2;


		//格式化该数据
		TCHAR BufferData[20] = { 0 };
		_stprintf(BufferData, _T("0x%p"), ParameterData2);


		//显示该数据
		SetDlgItemText(Hwnd, IDC_STATIC_SHOW, BufferData);

		break;

	}


	}

	return 0;
}