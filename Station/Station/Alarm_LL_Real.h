#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"

#include "MyChartViewer.h"


// CAlarm_LL_Real ������ͼ

class CAlarm_LL_Real : public CFormView
{
	DECLARE_DYNCREATE(CAlarm_LL_Real)

protected:
	CAlarm_LL_Real();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CAlarm_LL_Real();

public:
	enum { IDD = IDD_ALARM_LL_REAL };
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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedExcel4();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo3();

	//�Զ���
	void		My_LoadData_Per();										//��ѯС��������Ϣ
	void		My_LoadData_PM_Data(int v_iTime,int v_iType,CString v_sTable,bool v_bCell,map_Alarm_LL_PM_DATA* v_mAlarm_LL_Real);		//װ����������
	CString		My_Get_Table_Num(int v_iDay);							//��ȡ�����
	void		My_LoadData_Para();										//װ�����ܲ�����Ϣ
	void		My_LoadData_Cell();										//װ��С����Ϣ
	COleDateTime My_LoadData_Time();									//��ȡ��������

	//��ͼ
	void		My_List_Chart(int v_iList_Item);						//��ͼ��ѡ�е�Item
	void		My_List_Chart_GetNode(int v_iList_Item);				//��ã��ڵ����Ϣ [���ڣ���ѯ��v_sItem_Instance��	��ʾ��v_sItem_Instance_Name]
	void		My_Chart_IP();											//װ��IP��Ϣ
	void		My_Chart_New();											//��ͼ��λ
	void		My_Load_Performance(CString v_sType,CString v_sFile_Key,CString v_sCell,CString v_sCell_ID,CString v_sPerformance,CString v_sField,COleDateTime v_dDate_Start,int v_iDay_Sum);	//��ȡ���ݱ��е�IP����
	void		My_Chart();												//���߻�ͼ
	void		My_Show_Clear();										//�����Ļ��ʾ��Ϣ

	//�ִ�
	CStatic		m_cDraw_AAA;											//��ͼ
	CSSplitter	m_SplitterPane_Main;									//��ֱ��
	CSSplitter	m_SplitterPane_Left;									//ˮƽ��

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����
	void		My_Input_Main();										//����_������

	//------------------------------------------------------------------------------------------------------
	//	��������
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//��������Ϣ
	CMyListCtrl		m_cList_Cell;										//С����Ϣ
	CEdit			m_cSum;												//��Ԫ����
	CButton			m_cQuery;
	CButton			m_cExcel;
	CButton			m_cTime;
	CEdit			m_cLog_Sum;
	CEdit			m_cLog_Key;
	CButton			m_cCell;
	CStatic			m_cTime_Real;
	CButton			m_cAuto;
	CButton			m_cOnline;
	CButton			m_cDay;
	CButton			m_cPoint;
	CComboBox		m_cDay_Sum;											//����
	CComboBox		m_cPara;											//����

	//�Զ���
	CString			v_sFrame_Name;										//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item;										//ѡ�е��б�Item[������]
	int				v_iList_Item_Cell;									//ѡ�е��б�Item[С��]
	bool			v_bSelect;											//��ǣ�true��С����false����������
	CString			v_sFilename;										//�ļ�����
	COleDateTime	v_dPM_Date;											//�ɼ�����

	//��ѯ����
	bool			v_bDown,v_bUp;										//��־��������Ч(���С�����)
	Alarm_LL_Cell	v_dDown_Select[4];									//����ѡ������
	Alarm_LL_Cell	v_dUp_Select[4];									//����ѡ������

	//��ȡС�����
	map_Alarm_LL_PM_DATA	v_mAlarm_LL_Real_Down;						//������С����������_����
	map_Alarm_LL_PM_DATA	v_mAlarm_LL_Real_Up;						//������С����������_����

	//��������
	mLTE_PM_Type	v_mLTE_PM_Type;										//���������ܲ���
	set<CString>	v_mTable_Name;										//���ϣ����ܱ�����

	//С����Ϣ
	mString_String	v_mCell;											//������С��   -- С��ID
	map_Cell_ID		v_mCell_ID;											//������С��ID -- С��

	CString         v_sCell;											//ѡ����Ŀ��Cell����
	CString         v_sCell_ID;											//ѡ����Ŀ��Cell_ID
	CString			v_sCell_All;										//ѡ���С��[С��ģ��]

	SYSTEMTIME		v_dChart_Date;										//��ѯ����
	int				v_iChart_Day_Sum;									//��ѯ�������

	//------------------------------------------------------------------------------------------------------
	//	Chart����
	//------------------------------------------------------------------------------------------------------
	//������Draw chart
	void			OnChart_Para();										//���ݣ���ͼ���� [����Ļ��Chart����]
	afx_msg BOOL	OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	//
	afx_msg void	OnViewPortChanged();								//
	afx_msg void	OnMouseMovePlotArea();								//
	afx_msg LRESULT Message_ChartView(WPARAM iParam1,LPARAM iParam2);	//����ChartView

	//ͼ�α�����Chart
	CMyChartViewer	m_cChartView;										//�ؼ�������Chart                                               
	bool			v_bShow;											//������Ļ�ߴ��־

};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
