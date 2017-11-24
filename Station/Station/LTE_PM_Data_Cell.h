#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"


// CLTE_PM_Data_Cell ������ͼ

class CLTE_PM_Data_Cell : public CFormView
{
	DECLARE_DYNCREATE(CLTE_PM_Data_Cell)

protected:
	CLTE_PM_Data_Cell();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLTE_PM_Data_Cell();

public:
	enum { IDD = IDD_LTE_PM_DATA_CELL };
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
	afx_msg void OnBnClickedExcel2();
	afx_msg void OnBnClickedExcel3();
	afx_msg void OnBnClickedExcel4();
	afx_msg void OnBnClickedExcel5();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedExcel8();
	afx_msg void OnBnClickedExcel6();
	afx_msg void OnBnClickedExcel7();
	afx_msg void OnLvnColumnclickList9(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult);

	//�Զ���
	void		My_LoadData_Cell();										//װ��Cell��Ϣ
	void		My_LoadData_PM();										//װ��������Ϣ
	void		My_LoadData_PM_Tj();									//����ͳ��
	void		My_LoadData_PM_Table(CString v_sDate,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End,int v_iNum,int v_iSum);	//װ�����ܱ�����
	void		My_LoadData_PM_Data(mLTE_PM_Type::iterator v_pLTE_PM_Type,CString v_sTable,CString v_sField,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End);	//װ����������
	void		My_LoadData_PM_Data_Cal(CString v_sTable,CString v_sName_Cal,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End);	//װ����������
	bool		My_Get_Formula(CString v_sName,CString* v_sFormula,CString* v_sType);	//��ȡ���������͡��Զ��幫ʽ
	void		My_LoadData_Para();										//װ�����ܲ�����Ϣ
	void		My_Load_Columns(set<CString> *v_pTable_Columns,CString v_sTable);	//װ�ر�ṹ
	void		My_PM_Data_Delete();									//�����������������
	void		My_PM_Data_P_Delete();									//���������ͳ�Ƶ�ʱ����������
	void		My_PM_Show();											//ѡ�����ܣ������
	void		My_PM_Show_Data();										//��ʾ����ȡ��������ϸ
	void		My_PM_Show_Data_P();									//��ʾ��ͳ�Ƶ�ʱ��������ϸ
	void		My_PM_Show_Data_Tj(mString_Double* v_mPM_Tj_Date,CString v_sCell,CString v_sRegion,CString v_sOnline,CString v_sDate,int v_iSum_Time);		//��ʾ��ʱ��������ϸ
	CString		My_Get_Table_Num(int v_iDay);							//��ȡ�����

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����
	void		My_Input_Main();										//����_������

	//�ִ�
	CSSplitter		m_SplitterPane_Main;								//��ֱ��
	CSSplitter		m_SplitterPane_Left;								//��ֱ��

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//
	CMyListCtrl		m_cList_PM;
	CMyListCtrl		m_cList_Tj;
	CEdit			m_cLog_Sum;											//
	CDateTimeCtrl	m_cLog_Date;										//
	CComboBox		m_cLog_Day;											//
	CEdit			m_cLog_Key;											//
	CButton			m_cQuery;											//
	CButton			m_cExcel;											//
	CComboBox		m_cType;
	CEdit			m_cPM_Sum;
	CButton			m_cCheck;
	CButton			m_cPM_Cell;
	CButton			m_cCase_Cell;
	CButton			m_cCase_PM;
	CButton			m_cCase_Select;
	CButton			m_cTime_Check;
	CComboBox		m_cTj_Type;
	CButton			m_cSetup_Time;
	CButton			m_cTj;
	CButton			m_cExcel_Tj;
	CEdit			m_cSpan;
	CEdit			m_cQuary_Data;
	CStatic			m_cQuary_Name;

	//�Զ���
	CString			v_sFrame_Name;										//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item;										//ѡ�е��б�Item

	//��������
	mLTE_PM_Type	v_mLTE_PM_Type;										//��������������
	set<CString>	v_mTable_Name;										//���ϣ����ܱ�����

	//С����Ϣ
	mString_String	v_mCell;											//������С��   -- С��ID
	map_Cell_ID		v_mCell_ID;											//������С��ID -- С��

	mLTE_PM			v_mPM_Field_Num;									//��������������ӳ�������

	map_PM_Cell		v_mPM_Cell;											//�������������ݡ������ȡ���������ݡ�
	map_PM_Cell_P	v_mPM_Cell_P;										//�������������ݡ�����ͳ�Ƶ�ʱ���������ݡ�
	mInt_String		v_mTime;											//������ʱ����Ϣ��08��08--10ӳ�䡿

	Date_Select		v_dDate_Select[5];									//����ѡ��
	Time_Select		v_dTime_Select[5];									//ʱ��ѡ��

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

