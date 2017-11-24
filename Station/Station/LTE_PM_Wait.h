#pragma once
#include "afxwin.h"

#include "LTE_PM_Data_Cell.h"											//С�����ܲ�ѯ
#include "LTE_PM_Data_Real.h"											//ʵʱ���ܲ�ѯ


// CLTE_PM_Wait �Ի���

class CLTE_PM_Wait : public CDialog
{
	DECLARE_DYNAMIC(CLTE_PM_Wait)

public:
	CLTE_PM_Wait(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLTE_PM_Wait();

// �Ի�������
	enum { IDD = IDD_LTE_PM_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();										//
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//
	afx_msg void OnTimer(UINT_PTR nIDEvent);							//��ʱ��


	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CEdit		m_cName;												//��������
	int			v_iParent;												//�������־��  0��CIP_Flow��	1��CKPI_Flow��

	CLTE_PM_Data_Cell		*v_pLTE_PM_Data_Cell;						//ָ�����ָ��
	CLTE_PM_Data_Real		*v_pLTE_PM_Data_Real;						//ָ�����ָ��
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
