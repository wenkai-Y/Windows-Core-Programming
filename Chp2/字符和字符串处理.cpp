#include <windows.h>
#include <iostream>
#include <tchar.h>    
#include <atlstr.h>   

using namespace std;

void Sub_1();
void Sub_2();
void Sub_3();
void Sub_4();
void Sub_5();


void _tmain()
{
	_tsetlocale(LC_ALL, _T("Chinese-simplified"));


	//Sub_1();
	//Sub_2();
	//Sub_3();
	//Sub_4();

	//Sub_5();
}


void Sub_1()
{
	TCHAR v1[20] = _T("HelloWorld");   //20  20 20  40  
	TCHAR v2[20] = {0};

	//NumberOfCharacters
	//_tcscpy_s(v2, 20, v1);
	//_tprintf(_T("%s\r\n"), v2);

	//Count Bytes Of Source to Destination

	//memcpy(v2, v1, sizeof(TCHAR)*(10+1)); 
	//_tprintf(_T("%s\r\n"), v2);

}

void Sub_2()
{
	int v1[5] = { 1,2,3,4,5 };
	TCHAR v2[] = _T("HellWorld");
	//#define _countof(array) (sizeof(array)/sizeof(array[0]))   sizeof(v1)/sizeof(v1[0])
	_tprintf(_T("%d		%d\r\n"), sizeof(v1), _countof(v1));
	_tprintf(_T("%d		%d\r\n"), sizeof(v2), _countof(v2));
}

void Sub_3()
{
#ifdef _UNICODE
#define _tstring wstring
#define _tcout   wcout
#else
#define _tstring string
#define _tcout   cout
#endif
	//_tstring v1 = _T("HelloString");
	//_tcout << v1.c_str() << endl;
	//_tprintf(_T("%s\r\n"), v1.c_str());

	CString v2 = _T("HelloCString");
	_tcout << v2.GetString() << endl;
}

void Sub_4()
{
	//重叠拷贝  

	TCHAR  v1[20] = _T("HelloWorld"); 

	//_tcscpy_s(&v1[0], 10, &v1[1]);
	//_tcscpy_s(&v1[1], 10, &v1[0]);   //程序崩溃
	//_tcscpy(&v1[1], &v1[0]);

	//memcpy(&v1[1], &v1[0],(_tcslen(v1)+1)*sizeof(TCHAR));
	_tprintf(_T("%s\r\n"), v1);
}
void Sub_5()
{
	int v1 = 345;
	float v2 = 4.14;
	TCHAR BufferData[0x1000] = { 0 };
	_stprintf(BufferData, _T("我们都是中国人 %d %f"), v1,v2);
	_tprintf(_T("_tprintf:%s\r\n"), BufferData);   //输出打印  

	int v3 = 0;
	float v4 = 0;
	TCHAR v5[0x1000];
	_stscanf(BufferData, _T("%s %d %f"),v5,&v3,&v4);
	_tprintf(_T("%d\r\n"),v3);
	_tprintf(_T("%f\r\n"), v4);
	_tprintf(_T("%s\r\n"), v5);

}
