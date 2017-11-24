#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"


// CCM_LTE_Relation ������ͼ

class CCM_LTE_Relation : public CFormView
{
	DECLARE_DYNCREATE(CCM_LTE_Relation)

protected:
	CCM_LTE_Relation();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CCM_LTE_Relation();

public:
	enum { IDD = IDD_CM_LTE_RELATION };
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
	//��Ļ
	virtual void OnInitialUpdate();										//��ʼ��
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ���� [��ȡ������Ϣ������]
	afx_msg void OnDestroy();											//���ٴ���
	afx_msg void OnSize(UINT nType, int cx, int cy);					//�����ߴ�
	afx_msg void OnBnClickedExcel();									//
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnCloseupCombo1();

	//�Զ���
	void		My_LoadData_CM();										//װ����������
	void		My_LoadData_CM_Data(CString v_sTable);					//��ȡ��������

	void		My_LoadData_Para();										//װ�����ܲ�����Ϣ
	void		My_LoadData_Cell();										//װ��С����Ϣ
	void		My_Get_Cm_Field();										//��ȡ���ò���(����ʾ)
	void		My_Get_File_Field(mString_Int* v_mField,CString v_sTable);		//��ȡ�ļ��ֶ�

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����
	void		My_Input_Main();										//����_������

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//
	CMyListCtrl		m_cList_PM;
	CComboBox		m_cType_2;
	CComboBox		m_cLog_Day;											//
	CEdit			m_cLog_Key;											//
	CEdit			m_cLog_Key1;
	CButton			m_cQuery;											//
	CButton			m_cExcel;											//
	CComboBox		m_cType;
	CEdit			m_cData_Sum;
	CComboBox		m_cRelation;
	CButton			m_cSave;

	//�Զ���
	CString			v_sFrame_Name;										//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item;										//ѡ�е��б�Item
	COleDateTime	v_dDate;											//ѡ�������
	CString			v_sTable_Pro;										//�ļ�ǰ׺��Cm_LTE_HW_AAA_1��(�Ͳ��·�)
	CString			v_sTable_All;										//�ļ�ȫ�̡�Cm_LTE_HW_AAA_1_201608��
	int				v_iCM_Sum;											//��ȡ����������
	CString			v_sOutput;											//�ļ����

	//��������
	set<CString>	v_mTable_Name;										//���ϣ����ܱ�����
	mString_Int		v_mField_Main;										//ѡ����ֶ�����

	mString_String	v_mCell;											//С������[���� E_NodeB_ID+Cell_ID Ӱ�� С������]
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

