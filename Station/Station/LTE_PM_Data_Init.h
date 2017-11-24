#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"


// CLTE_PM_Data_Init ������ͼ

class CLTE_PM_Data_Init : public CFormView
{
	DECLARE_DYNCREATE(CLTE_PM_Data_Init)

protected:
	CLTE_PM_Data_Init();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLTE_PM_Data_Init();

public:
	enum { IDD = IDD_LTE_PM_DATA_Init };
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
	afx_msg void OnCbnCloseupCombo3();
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);

	//�Զ���
	void		My_LoadData_Cell();										//װ��Cell��Ϣ
	void		My_LoadData_PM();										//װ��������Ϣ
	void		My_LoadData_PM_Data(CString v_sTable,CString v_sCell,CString v_sCell_ID,CString v_sDate_Ben,CString v_sDate_End);	//װ����������
	CString		My_Get_Table();											//��ȡ������
	void		My_LoadData_Para();										//װ�����ܲ�����Ϣ
	CString		My_Get_Table_Num(int v_iDay);							//��ȡ�����

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
	CComboBox		m_cPM_Type;
	CEdit			m_cLog_Sum;											//
	CDateTimeCtrl	m_cLog_Date;										//
	CComboBox		m_cLog_Day;											//
	CEdit			m_cLog_Key;											//
	CButton			m_cQuery;											//
	CButton			m_cExcel;											//
	CComboBox		m_cType;											//
	CEdit			m_cPM_Sum;											//
	CEdit			m_cData_Sum;										//

	//�Զ���
	CString		v_sFrame_Name;											//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item;										//ѡ�е��б�Item

	//��������
	mLTE_PM_Type	v_mLTE_PM_Type;										//��������������
	set<CString>	v_mTable_Name;										//���ϣ����ܱ�����

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

