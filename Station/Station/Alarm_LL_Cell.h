#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"



// CAlarm_LL_Cell ������ͼ

class CAlarm_LL_Cell : public CFormView
{
	DECLARE_DYNCREATE(CAlarm_LL_Cell)

protected:
	CAlarm_LL_Cell();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CAlarm_LL_Cell();

public:
	enum { IDD = IDD_ALARM_LL_CELL };
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
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);	//��Item�仯��Ӧ
	afx_msg void OnBnClickedExcel2();
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedQuary2();
	afx_msg void OnBnClickedQuary3();

	//�Զ���
	void		My_LoadData_Per();										//��ѯС��������Ϣ
	void		My_LoadData_PM_Data(int v_iTime,int v_iType,CString v_sTable,bool v_bCell);		//װ����������
	CString		My_Get_Table_Num(int v_iDay);							//��ȡ�����
	void		My_LoadData_Para();										//װ�����ܲ�����Ϣ
	COleDateTime My_LoadData_Time();									//��ȡ��������

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����
	void		My_Input_Main();										//����_������

	//------------------------------------------------------------------------------------------------------
	//	��������
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//Station��Ϣ
	CEdit			m_cSum;												//��Ԫ����
	CButton			m_cQuery;
	CButton			m_cExcel;
	CButton			m_cTime;

	//�Զ���
	CString			v_sFrame_Name;										//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item;										//ѡ�е��б�Item
	CString			v_sFilename;										//�ļ�����

	//��ѯ����
	Alarm_LL_Cell	v_dDate_Select[5];									//����ѡ��
	bool			v_bDown,v_bUp;										//��־��������Ч(���С�����)

	//��ȡС�����
	map_Alarm_LL_PM_DATA	v_mAlarm_LL_Cell;							//������С����������

	//��������
	set<CString>	v_mTable_Name;										//���ϣ����ܱ�����

	//С����Ϣ
	map_Cell_ID		v_mCell_ID;											//������С��ID -- С��

};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
