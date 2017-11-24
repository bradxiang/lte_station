#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CAlarm_Message 对话框

class CAlarm_Message : public CDialog
{
	DECLARE_DYNAMIC(CAlarm_Message)

public:
	CAlarm_Message(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAlarm_Message();

// 对话框数据
	enum { IDD = IDD_ALARM_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_cSum;

	afx_msg void	OnClose();
	virtual BOOL	OnInitDialog();
	afx_msg void	OnPaint();
	afx_msg HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void	OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
