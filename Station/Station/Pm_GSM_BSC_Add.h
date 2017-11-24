#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPm_GSM_BSC_Add �Ի���

class CPm_GSM_BSC_Add : public CDialog
{
	DECLARE_DYNAMIC(CPm_GSM_BSC_Add)

public:
	CPm_GSM_BSC_Add(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPm_GSM_BSC_Add();

// �Ի�������
	enum { IDD = IDD_PM_GSM_BSC_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ����
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedExcel();
	afx_msg void OnBnClickedAdd2();
	afx_msg void OnBnClickedButton1();


	//�Զ���
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����
	void		My_LoadData();											//���֣����ݱ���Ϣ
	bool		My_New_Name(CString v_sName);							//�����ظ����

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//�б����û�
	CEdit			m_cSum;												//�û�����
	CComboBox		m_cRole;											//��������Role
	CButton			m_cAdd;
	CButton			m_cModify;
	CButton			m_cDelete;
	CButton			m_cExcel;
	CEdit			m_cName;
	CEdit			m_cObjType;
	CEdit			m_cText;
	
	//�Զ���
	int				v_iList_State;										//��־��0����ѯ��	1�����ӣ�	2���޸ģ�
	int				v_iList_Item;										//ѡ�е���Ŀ���

	afx_msg void OnBnClickedButton2();
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
