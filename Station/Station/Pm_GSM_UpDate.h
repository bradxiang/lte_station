#pragma once
#include "afxwin.h"


// CPm_GSM_UpDate �Ի���

class CPm_GSM_UpDate : public CDialog
{
	DECLARE_DYNAMIC(CPm_GSM_UpDate)

public:
	CPm_GSM_UpDate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPm_GSM_UpDate();

// �Ի�������
	enum { IDD = IDD_PM_GSM_UPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ����
	afx_msg void OnBnClickedOk();

	//�Զ���
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CEdit	m_cPM_Table;
	CEdit	m_cType;
	CEdit	m_cDay;

	//�Զ���
	CString v_sPM_Table;												//������
	CString	v_sType;													//����
	CString v_sDay;														//��������

protected:
public:
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
