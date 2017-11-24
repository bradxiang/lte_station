#pragma once
// #include "afxcmn.h"
// #include "afxwin.h"
#include "Resource.h"
#include "afxwin.h"

// CPM_LTE_Alarm �Ի���

class CPM_LTE_Alarm : public CDialog
{
	DECLARE_DYNAMIC(CPM_LTE_Alarm)

public:
	CPM_LTE_Alarm(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPM_LTE_Alarm();

	// �Ի�������
	enum { IDD = IDD_PM_LTE_ALARM  };

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
	afx_msg void OnBnClickedQuary2();

	//�Զ���
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����
	void		My_LoadData_Per();										//װ����Ϣ

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
	CEdit		m_cKey;
	CComboBox	m_cKey_Select;

	//�Զ���
	int				v_iItem;											//ѡ�е���Ŀ���
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
