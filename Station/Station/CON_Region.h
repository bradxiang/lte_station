#pragma once
// #include "afxcmn.h"
// #include "afxwin.h"
#include "Resource.h"
#include "afxwin.h"

// CCON_Region �Ի���

class CCON_Region : public CDialog
{
	DECLARE_DYNAMIC(CCON_Region)

public:
	CCON_Region(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCON_Region();

	// �Ի�������
	enum { IDD = IDD_CON_REGION  };

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
	afx_msg void OnBnClickedAdd();										//����
	afx_msg void OnBnClickedDelete();									//ɾ��
	afx_msg void OnBnClickedExcel();									//����
	afx_msg void OnBnClickedAdd2();										//����ȷ��
	afx_msg void OnBnClickedButton2();									//����ȡ��
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);		//���Pool�б�

	//�Զ���
	void		My_LoadData();											//���֣����ݱ���Ϣ
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����
	bool		My_New_Name(CString v_sName);							//�����ظ����

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl	m_cList;												//�б�����
	CEdit		m_cSum;													//�б�����Ŀ����
	CEdit		m_cName;												//��������
	CButton		m_cAdd;
	CButton		m_cDelete;
	CButton		m_cExcel;

	//�Զ���
	bool		v_bAdd;													//���ӱ�־
	int			v_iItem;												//ѡ�е���Ŀ���
	CString		v_sItem_ID;												//ѡ����Ŀ��ID
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
