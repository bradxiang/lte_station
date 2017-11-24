#pragma once
#include "afxwin.h"


// CPm_GSM_UpDate_PM �Ի���

class CPm_GSM_UpDate_PM : public CDialog
{
	DECLARE_DYNAMIC(CPm_GSM_UpDate_PM)

public:
	CPm_GSM_UpDate_PM(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPm_GSM_UpDate_PM();

// �Ի�������
	enum { IDD = IDD_PM_GSM_UPDATE_PM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ����
	afx_msg void OnCbnCloseupCombo4();
	afx_msg void OnBnClickedOk();

	//�Զ���
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����
	void		My_Get_PM_Table();										//��ȡ���ܱ�����

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CEdit		m_cType;
	CEdit		m_cFormula;
	CEdit		m_cPM_Name;												//��������
	CEdit		m_cObjType;
	CEdit		m_cUnit;
	CComboBox	m_cPM_Table;
	CButton		m_cOK;

	//�Զ���
	CString		v_sPM_Table;											//������
	CString		v_sType;												//����
	CString		v_sPM_Name;												//����
	CString		v_sUnit;												//��λ
	CString		v_sFormula;												//���㹫ʽ
	CString		v_sObjType;												//ObjType
	CString		v_sNum;													//Ψһ��ʶ
	bool		v_bFlag;												//��ǣ�0���޸ģ�1�����ӣ�
	bool		v_bBSC;													//��ǣ�0��Cell��1��BSC��

	mGSM_PM		v_mGSM_PM;												//�������ܱ�
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
