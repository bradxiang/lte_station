#pragma once
// #include "afxcmn.h"
// #include "afxwin.h"
#include "Resource.h"
#include "afxwin.h"

// CLTE_PM_Data_Cal_Select �Ի���

class CLTE_PM_Data_Cal_Select : public CDialog
{
	DECLARE_DYNAMIC(CLTE_PM_Data_Cal_Select)

public:
	CLTE_PM_Data_Cal_Select(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLTE_PM_Data_Cal_Select();

	// �Ի�������
	enum { IDD = IDD_LTE_PM_DATA_CAL_SELECT  };

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
	afx_msg void OnBnClickedButton4();

	//�Զ���
	void	My_Load_Cal();												//��ȡ���Զ����������
	bool	My_ExecuteSQL(CString v_sSql);								//Sqlִ��
	bool	My_PM_Cal(CString v_sAccount,CString v_sName);				//�˺š�ģ����ڣ�����

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl	m_cList;												//�б�����
	CEdit		m_cSum;													//�б�����Ŀ����
	CEdit		m_cPm_Value;											//���㹫ʽ
	CButton		m_cExcel;
	CComboBox	m_cPm_Type;
	CButton		m_cSave;
	CEdit		m_cPm_Name;
	CButton		m_cOK;

	//�Զ���
	mLTE_PM_Type*	v_mLTE_PM_Type;										//���ܲ���
	int				v_iItem;											//ѡ�е���Ŀ���
	CString			v_sType;											//��������
	CString			v_sFormula;											//���㹫ʽ
	CString			v_sName;											//ģ������
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
