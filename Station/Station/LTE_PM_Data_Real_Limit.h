#pragma once
// #include "afxcmn.h"
// #include "afxwin.h"
#include "Resource.h"
#include "afxwin.h"

// CLTE_PM_Data_Real_Limit �Ի���

class CLTE_PM_Data_Real_Limit : public CDialog
{
	DECLARE_DYNAMIC(CLTE_PM_Data_Real_Limit)

public:
	CLTE_PM_Data_Real_Limit(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLTE_PM_Data_Real_Limit();

	// �Ի�������
	enum { IDD = IDD_LTE_PM_DATA_REAL_LIMIT  };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();										//�Ի�����ʼ��
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ����
	afx_msg void OnSize(UINT nType, int cx, int cy);					//��������ߴ�
	afx_msg void OnBnClickedExcel();									//����
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);		//���Pool�б�
	afx_msg void OnBnClickedButton1();

	//�Զ���

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl	m_cList;												//�б�����
	CEdit		m_cSum;													//�б�����Ŀ����
	CEdit		m_cPm_Value;											//Ԥ������
	CButton		m_cExcel;
	CButton		m_cPm_Flag;
	CComboBox	m_cPm_Type;
	CButton		m_cUpdate;
	CEdit		m_cPm_Name;

	//�Զ���
	mLTE_PM_Type*	v_mLTE_PM_Type;										//���ܲ���
	int				v_iItem;											//ѡ�е���Ŀ���
	CString			v_sItem_ID;											//ѡ����Ŀ��ID
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
