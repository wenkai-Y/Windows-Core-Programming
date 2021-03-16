
// ServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/* 定义一个实例 */
CServerDlg* __ServerDlg = NULL;

// CServerDlg 对话框



CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
	, m_MutexHandle(INVALID_HANDLE_VALUE)
	, m_v1(0)
	, m_v2(0)
	, m_v3(0)
	, m_CEditMaxConnectCount(0)
	, m_IsServerStart(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_UserData, 0, sizeof(USER_DATA) * MAX_CONNECT_COUNT);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_v1);
	DDX_Text(pDX, IDC_EDIT2, m_v2);
	DDX_Text(pDX, IDC_EDIT3, m_v3);
	DDX_Text(pDX, IDC_EDIT_MAX_CONNECT_COUNT, m_CEditMaxConnectCount);
	DDV_MinMaxLong(pDX, m_CEditMaxConnectCount, 1, 100);	// 设置值的大小
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK_START, &CServerDlg::OnBnClickedButtonStartServer)
	ON_BN_CLICKED(IDCANCEL_CLOSE, &CServerDlg::OnBnClickedButtonStopServer)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	/* 服务器只能运行一个实例*/
	__ServerDlg = this;
	/* 创建命名互斥体，防止程序多开 */
	m_MutexHandle = OpenMutex(MUTEX_ALL_ACCESS, true, _T("NamedPipeMutex")); // 打开一个有名称的互斥体
	if (m_MutexHandle == NULL) {
		// 创建一个互斥体
		m_MutexHandle = CreateMutex(NULL, true, _T("NamedPipeMutex"));
	}
	else {
		// 程序已经打开，关闭程序
		this->SetWindowText(_T("服务端启动失败"));
		this->MessageBox(_T("服务器只能运行一个实例"));	// MFC
		TerminateProcess(GetCurrentProcess(), 0);		// 自杀程序
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::OnBnClickedButtonStartServer()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString PipeName = _T("\\\\.\\Pipe\\Yek");
	if (m_CEditMaxConnectCount > 0 && m_CEditMaxConnectCount < 100) {
		for (UINT i = 0; i < (UINT)m_CEditMaxConnectCount; ++i) {
			// 创建管道实例
			m_UserData[i].PipeHandle = CreateNamedPipe(PipeName, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, \
				PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, m_CEditMaxConnectCount, 0, 0, 1000, NULL);
			if (m_UserData[i].PipeHandle == INVALID_HANDLE_VALUE) {
				DWORD ErrorCode = GetLastError();
				this->MessageBox(_T("创建管道错误"));
				// 防止内存泄漏
				for (UINT j = 0; j < i; ++j) {
					// 关闭句柄
					YekCloseHandle(m_UserData[j].PipeHandle);
				}
				Sleep(500);
				return;
			}
			// 为每一个管道实例创建一个事件对象，用于实现重叠IO
			m_UserData[i].EventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
			// 为每个管道分配一个线程，用于响应客户端的请求
			m_UserData[i].ThreadHandle = CreateThread(NULL, 0,
				(LPTHREAD_START_ROUTINE)ThreadProcedure, &m_UserData[i], 0, NULL);
		}
		{
			this->SetWindowText(_T("命名管道—服务器(运行)"));
			this->MessageBox(_T("服务启动成功"));
		}
	}
}


void CServerDlg::OnBnClickedButtonStopServer()
{
	// TODO: 在此添加控件通知处理程序代码
	for (UINT i = 0; i < (UINT)m_CEditMaxConnectCount; ++i) {
		// 设置重叠I/O的事件，使得工作线程安全结束
		SetEvent(m_UserData[i].EventHandle);
		Sleep(2);
		if (m_UserData[i].PipeHandle != INVALID_HANDLE_VALUE) {
			YekCloseHandle(m_UserData[i].PipeHandle);
			m_UserData[i].PipeHandle = INVALID_HANDLE_VALUE;
		}
		if (m_UserData[i].EventHandle != INVALID_HANDLE_VALUE) {
			YekCloseHandle(m_UserData[i].EventHandle);
			m_UserData[i].EventHandle = INVALID_HANDLE_VALUE;
		}
		if (m_UserData[i].ThreadHandle != INVALID_HANDLE_VALUE) {
			YekCloseHandle(m_UserData[i].ThreadHandle);
			m_UserData[i].ThreadHandle = INVALID_HANDLE_VALUE;
		}
	}
	if (m_MutexHandle != INVALID_HANDLE_VALUE) {
		YekCloseHandle(m_MutexHandle);
		m_MutexHandle = INVALID_HANDLE_VALUE;
	}
	__ServerDlg->SetDlgItemInt(IDC_EDIT1, 0, true);
	__ServerDlg->SetDlgItemInt(IDC_EDIT2, 0, true);
	__ServerDlg->SetDlgItemInt(IDC_EDIT3, 0, true);
	__ServerDlg->SetDlgItemInt(IDC_EDIT_MAX_CONNECT_COUNT, 0, true);

	this->SetWindowText(_T("命名管道—服务器(停止)"));
	this->MessageBox(_T("停止启动成功"));
}


UINT WINAPI ThreadProcedure(LPVOID ParameterData) {
	DWORD ReturnLength = 0;
	TCHAR BufferData[0x1000] = { 0 };
	USER_DATA UserData = *(USER_DATA*)ParameterData;
	OVERLAPPED Overlapped = { 0, 0, 0,0, UserData.EventHandle };	// 使用异步IO
	while (TRUE) {
		memset(BufferData, 0, 0x1000 * sizeof(TCHAR));
		// 命名管道的连接函数，等待客户端的连接（只针对NT）
		ConnectNamedPipe(UserData.PipeHandle, &Overlapped);
		// 实现重叠I/O，等待OVERLAPPED结构的事件对象
		WaitForSingleObject(UserData.EventHandle, INFINITE);
		// 检测I/O是否已经完成，如果未完成，意味着该事件对象是人工设置，则服务需要停止
		if (!GetOverlappedResult(UserData.PipeHandle, &Overlapped, &ReturnLength, true)) {
			DisconnectNamedPipe(UserData.PipeHandle);
			break;
		}
		// 从管道中读取客户端的请求信息
		if (!ReadFile(UserData.PipeHandle, BufferData, 0x1000 * sizeof(TCHAR), &ReturnLength, NULL)) {
			MessageBox(0, _T("读取管道错误"), 0, 0);
		}
		int v1;	memset(&v1, 0, sizeof(int));
		int v2;	memset(&v2, 0, sizeof(int));
		_stscanf_s(BufferData, _T("%d %d"), &v1, &v2);	// 字符串转换为%d
		__ServerDlg->m_v1 = v1;
		__ServerDlg->m_v2 = v2;
		__ServerDlg->m_v3 = v1 + v2;
		memset(BufferData, 0, 0x1000 * sizeof(TCHAR));
		_stprintf_s(BufferData, _T("%d"), __ServerDlg->m_v3);
		// 把反馈信息写入管道
		WriteFile(UserData.PipeHandle, BufferData, _tcslen(BufferData) * sizeof(TCHAR), &ReturnLength, NULL);
		__ServerDlg->SetDlgItemInt(IDC_EDIT1, v1, true);
		__ServerDlg->SetDlgItemInt(IDC_EDIT2, v2, true);
		__ServerDlg->SetDlgItemInt(IDC_EDIT3, __ServerDlg->m_v3, true);
		//
		//   __ServerDlg->UpdateData(FALSE);
		//
		// 断开客户端的连接，以便等待下一个客户的到来
		DisconnectNamedPipe(UserData.PipeHandle);
	}
	return 0;
}

bool YekCloseHandle(HANDLE myHandle)
{
	DWORD HandleFlags;
	if (GetHandleInformation(myHandle, &HandleFlags) && (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
		return !!CloseHandle(myHandle);
	return false;
}


void CServerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->OnBnClickedButtonStopServer();
	Sleep(500);
	CDialogEx::OnClose();
}
