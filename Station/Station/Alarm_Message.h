#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CAlarm_Message �Ի���

class CAlarm_Message : public CDialog
{
	DECLARE_DYNAMIC(CAlarm_Message)

public:
	CAlarm_Message(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAlarm_Message();

// �Ի�������
	enum { IDD = IDD_ALARM_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CButton m_cSum;

	afx_msg void	OnClose();
	virtual BOOL	OnInitDialog();
	afx_msg void	OnPaint();
	afx_msg HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void	OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
