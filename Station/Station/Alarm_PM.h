#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"



// CAlarm_PM ������ͼ

class CAlarm_PM : public CFormView
{
	DECLARE_DYNCREATE(CAlarm_PM)

protected:
	CAlarm_PM();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CAlarm_PM();

public:
	enum { IDD = IDD_Alarm_PM };
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
	afx_msg void OnDestroy();											//���ٴ���
	afx_msg void OnSize(UINT nType, int cx, int cy);					//�����ߴ�
	afx_msg void OnBnClickedQuary();									//��ѯ
	afx_msg void OnBnClickedExcel();									//����
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);//����
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ���� [��ȡ������Ϣ������]
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedExcel2();
	afx_msg void OnLvnColumnclickList9(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAlarmPmCell();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedExcel3();

	//�Զ���
	void		My_LoadData();											//װ����־��Ϣ
	void		My_Begin();												//��ʼ��ѯ
	COleDateTime My_LoadData_Time();									//��ȡ��������
	void		My_LoadData_Para();										//װ�����ܲ�����Ϣ
	void		My_LoadData_Region();									//���ܲ�ѯ������
	void		My_LoadData_Cell();										//���ܲ�ѯ��С��

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
	CMyListCtrl		m_cList;
	CMyListCtrl		m_cList_PM;
	CEdit			m_cLog_Sum;
	CDateTimeCtrl	m_cLog_Date;
	CComboBox		m_cLog_Day;
	CEdit			m_cLog_Key;
	CButton			m_cQuery;
	CButton			m_cExcel;
	CComboBox		m_cType;
	CButton			m_cReal;
	CButton			m_cTime;

	//�Զ���
	CString		v_sFrame_Name;											//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������

	COleDateTime	v_dPM_Date;											//�ɼ�����

	COleDateTime	v_dAlarm_Date;										//Ԥ��ʱ��
	bool			v_bAlarm;											//�澯��־
	int				v_iList_Item;										//ѡ�е��б�Item

	//��������
	mLTE_PM_Type	v_mLTE_PM_Type;										//��������������
	set<CString>	v_mTable_Name;										//���ϣ����ܱ�����
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

