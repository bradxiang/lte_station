#pragma once
#include "afxwin.h"


// CCON_Para �Ի���

class CCON_Para : public CDialog
{
	DECLARE_DYNAMIC(CCON_Para)

public:
	CCON_Para(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCON_Para();

// �Ի�������
	enum { IDD = IDD_CON_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	//�Զ���
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����
	void		My_Get_Para();											//��ò���

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CComboBox	m_cHour_Month;
	CComboBox	m_cMin_Day;
	CComboBox	m_cFTP_Day;
	CComboBox	m_cCm_Month;
	CComboBox	m_cCm_FTP_Day;

	//�Զ���
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
