#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"


// CLTE_PM_Data_Real ������ͼ

class CLTE_PM_Data_Real : public CFormView
{
	DECLARE_DYNCREATE(CLTE_PM_Data_Real)

protected:
	CLTE_PM_Data_Real();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLTE_PM_Data_Real();

public:
	enum { IDD = IDD_LTE_PM_DATA_REAL };
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedExcel6();
	afx_msg void OnLvnColumnclickList9(NMHDR *pNMHDR, LRESULT *pResult);

	//�Զ���
	void		My_LoadData_Cell();										//װ��Cell��Ϣ
	void		My_LoadData_PM();										//װ��������Ϣ
	void		My_LoadData_PM_Table(CString v_sDate,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End);	//װ�����ܱ�����
	void		My_LoadData_PM_Data(mLTE_PM_Type::iterator v_pLTE_PM_Type,CString v_sTable,CString v_sField,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End);	//װ����������
	void		My_LoadData_Para();										//װ�����ܲ�����Ϣ
	void		My_PM_Data_Delete();									//�����������������
	void		My_PM_Show();											//ѡ�����ܣ������
	COleDateTime My_LoadData_Time();									//��ȡ��������
	void		My_Alarm_Insert(COleDateTime v_dTime,CString v_sPM_Name,CString v_sObject,CString v_sPm_Value,CString v_sPm_Limite);	//���ӣ�Ԥ����Ϣ
	void		My_Alarm_Show(CString v_sTime,CString v_sPM_Name,CString v_sObject,CString v_sPm_Value,CString v_sPm_Limite,int v_iSum,int v_iSum_Cell);	//��ʾ��Ԥ����Ϣ
	void		My_DeleteFiles(CString v_sDirName);						//ɾ���ļ�[*.xml]��

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
	CButton			m_cExcel_Tj;
	CButton			m_cAuto;											//�Զ�ˢ��
	CStatic			m_cTime;											//ʱ��
	CButton			m_cAlarm;											//
	CButton			m_cAlarm_All;										//������Ԥ��

	//�Զ���
	CString			v_sFrame_Name;										//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item;										//ѡ�е��б�Item
	CString			v_sPM_Working;										//���ܻ�ȡ����
	COleDateTime	v_dPM_Date;											//�ɼ�����

	//��������
	mLTE_PM_Type	v_mLTE_PM_Type;										//��������������
	set<CString>	v_mTable_Name;										//���ϣ����ܱ�����

	//Ԥ����Ϣ������Ԥ����ͳ����Ϣ����ÿ������+С����������������Ԥ����ͳ������(����������Ԥ������)��
	struct  PM_Cell_Real_Alarm_Struct
	{
		CString				v_sPM_Name;									//�澯��������
		CString				v_sObject;									//�澯����
		mDate_Int			v_dDate_Real;								//�������澯����ʱ��
	};  
	typedef map<CString,PM_Cell_Real_Alarm_Struct> mPM_Cell_Real_Alarm;	//������mPM_Cell_Real_Alarm

	mDate_Int				v_dDate_Real;								//����������ʱ��
	mPM_Cell_Real_Alarm		v_mCell_Real_Alarm;							//������С��Ԥ��

	//С����Ϣ
	mString_String	v_mCell;											//������С��   -- С��ID
	map_Cell_ID		v_mCell_ID;											//������С��ID -- С��

	mLTE_PM			v_mPM_Field_Num;									//��������������ӳ�������

	map_PM_Cell		v_mPM_Cell;											//�������������ݡ������ȡ���������ݡ�
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

