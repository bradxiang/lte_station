#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"


// CCM_LTE_Quary ������ͼ

class CCM_LTE_Quary : public CFormView
{
	DECLARE_DYNCREATE(CCM_LTE_Quary)

protected:
	CCM_LTE_Quary();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CCM_LTE_Quary();

public:
	enum { IDD = IDD_CM_LTE_QUARY };
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
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);//
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnCloseupBegDate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult);

	//�Զ���
	void		My_LoadData_CM();										//װ����������
	void		My_LoadData_CM_Data(CString v_sTable,CString v_sDate_Ben,CString v_sDate_End);	//��ȡ��������

	void		My_LoadData_Para();										//װ�����ܲ�����Ϣ
	void		My_Get_Cm_Field(CString v_sType);						//��ȡ���ò���(����ʾ)
	void		My_Get_File_Field(mString_Int* v_mField,CString v_sTable);		//��ȡ�ļ��ֶ�

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����
	void		My_Input_Main();										//����_������

	//�ִ�
	CSSplitter		m_SplitterPane_Main;								//��ֱ��

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//
	CMyListCtrl		m_cList_PM;
	CComboBox		m_cType_2;
	CDateTimeCtrl	m_cLog_Date;										//
	CComboBox		m_cLog_Day;											//
	CEdit			m_cLog_Key;											//
	CEdit			m_cLog_Key1;
	CButton			m_cQuery;											//
	CButton			m_cExcel;											//
	CComboBox		m_cType;
	CEdit			m_cData_Sum;
	CComboBox		m_cRelation;
	CButton			m_cDay;

	//�Զ���
	CString			v_sFrame_Name;										//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item;										//ѡ�е��б�Item
	COleDateTime	v_dDate;											//ѡ�������
	CString			v_sTable_Pro;										//�ļ�ǰ׺��Cm_LTE_HW_AAA_1��(�Ͳ��·�)
	CString			v_sTable_All;										//�ļ�ȫ�̡�Cm_LTE_HW_AAA_1_201608��
	int				v_iCM_Sum;											//��ȡ����������

	//��������
	set<CString>	v_mTable_Name;										//���ϣ����ܱ�����
	mString_Int		v_mField_Main;										//ѡ����ֶ�����

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

