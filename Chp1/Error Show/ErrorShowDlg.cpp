
// ErrorShowDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ErrorShow.h"
#include "ErrorShowDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CErrorShowDlg 对话框



CErrorShowDlg::CErrorShowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ERRORSHOW_DIALOG, pParent)
	, ErrorNumber(0)
	, SystemLocale(-1)
	, LocalHandle(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CErrorShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VALUE, ErrorNumber);
	DDX_Control(pDX, IDC_EDIT_VALUE, EditError);
}

BEGIN_MESSAGE_MAP(CErrorShowDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CErrorShowDlg::OnBnClickedButtonFind)
END_MESSAGE_MAP()


// CErrorShowDlg 消息处理程序

BOOL CErrorShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 初始化信息查找
	// Use the default system locale since we look for Windows messages.
	// Note: this MAKELANGID combination has 0 as value
	this->SystemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CErrorShowDlg::OnPaint()
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
HCURSOR CErrorShowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CErrorShowDlg::OnBnClickedButtonFind()
{
	// TODO: 在此添加控件通知处理程序代码
	// 刷新数据
	this->UpdateData(TRUE);
	ReturnErrorMessage(ErrorNumber);
	EditError.SetSel(0,-1);
}

void CErrorShowDlg::ReturnErrorMessage(const DWORD ErrorCode)
{
	// Get the error code's textual description
	BOOL IsOk = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, ErrorCode, this->SystemLocale,
		(PTSTR)&this->LocalHandle, 0, NULL);

	if (!IsOk) {
		// Is it a network-related error?
		HMODULE Module = LoadLibraryEx(_T("netmsg.dll"), NULL,
			DONT_RESOLVE_DLL_REFERENCES);

		if (Module != NULL) {
			IsOk = FormatMessage(
				FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_ALLOCATE_BUFFER,
				Module, ErrorCode, this->SystemLocale,
				(PTSTR)&this->LocalHandle, 0, NULL);
			FreeLibrary(Module);
		}
	}
	if (IsOk && (LocalHandle != NULL)) {
		this->SetDlgItemText(IDC_EDIT_ERROR, (PCTSTR)LocalLock(LocalHandle));
		LocalFree(LocalHandle);
	}
	else {
		this->SetDlgItemText(IDC_EDIT_ERROR, _T("没有找到对应的错误码."));
	}
}