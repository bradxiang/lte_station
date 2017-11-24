#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CUser_Role �Ի���

class CUser_Role : public CDialog
{
	DECLARE_DYNAMIC(CUser_Role)

public:
	CUser_Role(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUser_Role();

// �Ի�������
	enum { IDD = IDD_USER_ROLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();										//
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ����
	afx_msg void OnSize(UINT nType, int cx, int cy);					//
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);		//
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);//
	afx_msg void OnBnClickedMscKpiExcel();								//
	afx_msg void OnBnClickedAddMscKpi2();								//
	afx_msg void OnBnClickedButton23();									//
	afx_msg void OnBnClickedButton25();									//
	afx_msg void OnBnClickedCheck1();									//
	afx_msg void OnBnClickedAll();										//
	afx_msg void OnBnClickedNull();										//
	afx_msg void OnBnClickedSave();										//


	//�Զ���
	void		My_LoadData();											//���֣����ݱ���Ϣ
	void		My_LoadData_Power();									//���֣�Ȩ����Ϣ
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����
	bool		My_New_Name(CString v_sName);							//�����ظ����
	void		My_GetPower();											//��ã�Ȩ��
	void		My_GetPower_OK(CString v_sRolie_ID);					//��ã���ȨȨ��

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//�б�
	CMyListCtrl		m_cList_Power;										//�б�
	CButton			m_cRole_Flag;										//��־����չ��ʾ
	CEdit			m_cAdd_Role_Name;									//���ӣ���ɫ����
	CEdit			m_cAdd_Role_Marker;									//���ӣ���ɫ��ע
	CEdit			m_cMod_Role_Marker;									//�޸ģ���ɫ��ע
	CEdit			m_cMod_Role_Name;									//�޸ģ���ɫ����
	CEdit			m_cDel_Role_Name;									//ɾ������ɫ����
	CEdit			m_cSum;												//Role�ܺ�

	//�Զ���
	int				v_iList_State;										//��־��0����ѯ��	1�����ӣ�	2���޸ģ�
	int				v_iList_Item;										//ѡ�е���Ŀ���
	CString			v_sList_Item_ID;									//ѡ����Ŀ��ID

	//Ȩ�ޣ�����
	mUSER_Power		v_mUSER_Power;										//�û�Ȩ������

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
