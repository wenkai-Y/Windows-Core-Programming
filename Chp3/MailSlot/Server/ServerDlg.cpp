
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


// CServerDlg 对话框

#pragma warning (disable:6031)

CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent),
	  m_MailSlotHandle(INVALID_HANDLE_VALUE)	// 初始化
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_CListBox);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON, &CServerDlg::OnBnClickedButton)
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
	this->StartServer();

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

/* 初始化油件槽 */
void CServerDlg::StartServer() {
	this->m_MailSlotHandle = CreateMailslot(MAIL_SLOT_NAME, 0, MAILSLOT_WAIT_FOREVER, NULL);
	if (this->m_MailSlotHandle == INVALID_HANDLE_VALUE) {
		TCHAR BufferData[0x1000] = { 0 };
		int LastError = GetLastError();
		_stprintf_s(BufferData, _T("无法创建油件槽：%d\r\n"), LastError);
		OutputDebugString(BufferData);
	}
}

/* 关闭句柄  */
bool CServerDlg::YwkCloseHandle(HANDLE &HandleValue)
{
	DWORD HandleFlags;
	if (GetHandleInformation(HandleValue, &HandleFlags)
		&& (HandleFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != HANDLE_FLAG_PROTECT_FROM_CLOSE) {
		int LastError = GetLastError();
		return !!CloseHandle(HandleValue);
	}
	return false;
}

/* 关闭消息 */
void CServerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (this->m_MailSlotHandle != INVALID_HANDLE_VALUE) {
		// 关闭
		if (this->YwkCloseHandle(this->m_MailSlotHandle)) {
			goto Exit;
		}
		
	}
	Sleep(5);
Exit:
	CDialogEx::OnClose();
}

/* 接收数据 */
void CServerDlg::OnBnClickedButton()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD NextSize(0);
	DWORD MessageCount(0);
	DWORD ReturnLength(0);

	/* 如果接收消息成功 */
	if (GetMailslotInfo(this->m_MailSlotHandle, NULL, &NextSize, &MessageCount, NULL)) {
		for (DWORD i = 0; i < MessageCount; i++)
		{
			LPBYTE BufferData = new BYTE[NextSize + 0x1000];
			memset(BufferData, 0, NextSize + 0x1000);
			ReadFile(this->m_MailSlotHandle, BufferData, NextSize, &ReturnLength, NULL);

			this->m_CListBox.AddString((LPCTSTR)BufferData);
			if (BufferData != NULL)
			{
				delete[]BufferData;
				BufferData = NULL;
			}
		}
	}
}