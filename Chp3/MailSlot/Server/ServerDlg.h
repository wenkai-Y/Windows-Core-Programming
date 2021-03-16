
// ServerDlg.h: 头文件
//

#pragma once

#define MAIL_SLOT_NAME _T("\\\\.\\mailslot\\YUWENKAI")

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
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


private:
	HANDLE m_MailSlotHandle;
	CListBox m_CListBox;

public:
	bool CServerDlg::YwkCloseHandle(HANDLE& HandleValue);
	void StartServer();
};
