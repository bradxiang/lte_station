#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"



// CPm_GSM_Cell_HO ������ͼ

class CPm_GSM_Cell_HO : public CFormView
{
	DECLARE_DYNCREATE(CPm_GSM_Cell_HO)

protected:
	CPm_GSM_Cell_HO();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CPm_GSM_Cell_HO();

public:
	enum { IDD = IDD_PM_GSM_CELL_HO };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual void OnInitialUpdate();										//��ʼ��
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ����
	afx_msg void OnDestroy();											//
	afx_msg void OnSize(UINT nType, int cx, int cy);					//
	afx_msg void OnBnClickedQuary();									//
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);	//��Item�仯��Ӧ
	afx_msg void OnBnClickedExcel2();
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnCloseupCombo4();
	afx_msg void OnBnClickedExcel3();
	afx_msg void OnBnClickedExcel4();

	//�Զ���
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����
	void		My_Query_Station();										//��ѯ��վַ
	void		My_LoadData_Station_Data(CString v_sSql);				//װ��վַ��Ϣ

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����
	void		My_Input_Main();										//����_������

	//------------------------------------------------------------------------------------------------------
	//	��������
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList_Station;									//Station��Ϣ
	CEdit			m_cSum;												//��Ԫ����
	CEdit			m_cKey;												//Key
	CComboBox		m_cKey_Select;
	CButton			m_cQuery;
	CButton			m_cExcel;
	CButton			m_cExcel_NodeB;
	CComboBox		m_cType;

	//�Զ���
	CString			v_sFrame_Name;										//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item;										//ѡ�е��б�Item
	afx_msg void OnBnClickedExcel5();
};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
