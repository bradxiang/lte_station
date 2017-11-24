#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CUser_Setup �Ի���

class CUser_Setup : public CDialog
{
	DECLARE_DYNAMIC(CUser_Setup)

public:
	CUser_Setup(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUser_Setup();

// �Ի�������
	enum { IDD = IDD_USER_SETUP };

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
	afx_msg void OnBnClickedPassword();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedExcel();
	afx_msg void OnBnClickedAdd2();
	afx_msg void OnBnClickedButton1();


	//�Զ���
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����
	void		My_GetRole();											//��ã�Role����
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
	CButton			m_cClear;
	CEdit			m_cName;
	CEdit			m_cAccount;
	CButton			m_cFlag;
	CEdit			m_cMarker;

	//�Զ���
	int				v_iList_State;										//��־��0����ѯ��	1�����ӣ�	2���޸ģ�
	int				v_iList_Item;										//ѡ�е���Ŀ���
	CString			v_sList_Item_ID;									//ѡ����Ŀ��ID

	//Role������
	mUser_Role		v_mUser_Role;										//User_Role
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
