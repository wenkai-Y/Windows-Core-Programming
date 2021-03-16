
// ClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, m_v1(0)
	, m_v2(0)
	, m_v3(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_v1);
	DDX_Text(pDX, IDC_EDIT2, m_v2);
	DDX_Text(pDX, IDC_EDIT3, m_v3);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CClientDlg::OnBnClickedButtonSubmitClient)
	ON_BN_CLICKED(IDCANCEL, &CClientDlg::OnBnClickedButtonEmptyClient)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnBnClickedButtonSubmitClient()
{
	// TODO: 在此添加控件通知处理程序代码

	HANDLE NamedPipeHandle = CreateFile(_T("\\\\.\\Pipe\\Yek"), GENERIC_READ | GENERIC_WRITE, \
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (NamedPipeHandle == INVALID_HANDLE_VALUE) {
		this->MessageBox(_T("打开管道失败，服务器尚未启动，或者客户端数量过多"));
		return;
	}
	DWORD ReturnLength = 0;
	TCHAR BufferData[0x1000] = { 0 };
	UpdateData(TRUE);
	_stprintf_s(BufferData, _T("%d %d"), this->m_v1, this->m_v2);
	// 将数据写入管道
	WriteFile(NamedPipeHandle, BufferData, _tcslen(BufferData) * sizeof(TCHAR), &ReturnLength, NULL);
	memset(BufferData, 0, sizeof(TCHAR) * 0x1000);
	// 读取服务器的反馈信息
	if (!ReadFile(NamedPipeHandle, BufferData, 0x1000 * sizeof(TCHAR), &ReturnLength, NULL)) {
		this->MessageBox(_T("读取管道错误"));
	}
	_stscanf_s(BufferData, _T("%d"), &(this->m_v3));
	UpdateData(FALSE);
	if (NamedPipeHandle != INVALID_HANDLE_VALUE)
	{
		YekCloseHandle(NamedPipeHandle);
		NamedPipeHandle = INVALID_HANDLE_VALUE;
	}

}


void CClientDlg::OnBnClickedButtonEmptyClient()
{
	// TODO: 在此添加控件通知处理程序代码
	m_v1 = 0;
	m_v2 = 0;
	m_v3 = 0;
	UpdateData(FALSE);
}

bool YekCloseHandle(HANDLE myHandle)
{
	DWORD HandleFlags;
	if (GetHandleInformation(myHandle, &HandleFlags) && (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE)
		return !!CloseHandle(myHandle);
	return false;
}

void CClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->MessageBox(_T("正在关闭中"));
	Sleep(500);
	TerminateProcess(GetCurrentProcess(), 0);		// 自杀程序
	CDialogEx::OnClose();
}
