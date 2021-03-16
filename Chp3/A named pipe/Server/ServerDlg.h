
// ServerDlg.h: 头文件
//

#pragma once

#define MAX_CONNECT_COUNT 128

typedef struct _USER_DATA_  //多线程异步IO
{
	HANDLE	ThreadHandle;
	HANDLE	PipeHandle;
	HANDLE	EventHandle;
}USER_DATA;

// CServerDlg 对话框
class CServerDlg : public CDialogEx
{
// 构造
public:
	CServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	// 互斥体
	HANDLE m_MutexHandle;
	BOOL m_IsServerStart;
	USER_DATA m_UserData[MAX_CONNECT_COUNT];

public:
	// Edit Control变量
	int m_v1;
	int m_v2;
	int m_v3;
	// 最大连接数
	long m_CEditMaxConnectCount;

public:
	afx_msg void OnBnClickedButtonStartServer();
	afx_msg void OnBnClickedButtonStopServer();
	afx_msg void OnClose();
};

UINT WINAPI ThreadProcedure(LPVOID ParameterData);
bool YekCloseHandle(HANDLE myHandle);
