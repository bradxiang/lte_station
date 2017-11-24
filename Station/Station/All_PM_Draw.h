#pragma once
#include "afxcmn.h"
#include "afxcmn.h"

#include "MyChartViewer.h"
#include "afxdtctl.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"


// CAll_PM_Draw ������ͼ

class CAll_PM_Draw : public CFormView
{
	DECLARE_DYNCREATE(CAll_PM_Draw)

protected:
	CAll_PM_Draw();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CAll_PM_Draw();

public:
	enum { IDD = IDD_ALL_PM_DRAW };
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
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);	//��Item�仯��Ӧ
	afx_msg void OnDtnCloseupBegDate(NMHDR *pNMHDR, LRESULT *pResult);  //
	afx_msg void OnCbnSelchangeCombo1();                                //
	afx_msg void OnBnClickedRefresh();                                  //
	afx_msg void OnBnClickedDelete();                                   //
	afx_msg void OnBnClickedCkMuti();									//
	afx_msg void OnBnClickedQuary();									//
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);		//
	afx_msg void OnBnClickedInstanceDetail();							//

	//�Զ���
	void		My_Query_Station();										//��ѯ��վַ
	void		My_LoadData_Station_Data(CString v_sSql);				//װ��վַ��Ϣ
	void		My_LoadData_Tree(CString v_sRoom);						//װ�ػ�����Ϣ
	void		My_LoadData_Para();										//װ����������

	void		My_Tree_Chart_GetNode(HTREEITEM v_hNode);				//��ã��ڵ����Ϣ [���ڣ���ѯ��v_sItem_Instance��	��ʾ��v_sItem_Instance_Name]
    void		My_Chart_IP();											//װ��IP��Ϣ
	void		My_Chart_New();											//��ͼ��λ
	void		My_Load_Performance(CString v_sType,CString v_sFile_Key,CString v_sCell,CString v_sPerformance,CString v_sField,COleDateTime v_dDate_Start,int v_iDay_Sum);	//��ȡ���ݱ��е�IP����
    void		My_Tree_Chart(HTREEITEM v_hNode);						//��ͼ��ѡ�е�Tree Node
	void		My_Tree_Clear(HTREEITEM v_hNode);						//���Tree Nodeѡ��״̬
	void		My_Chart();												//��ͼ
	void		My_Chart_Muti();										//���߻��ơ����ߡ����ߡ�
	void		My_Show_Clear();										//�����Ļ��ʾ��Ϣ
	void		My_SendToFile();										//�����ݵ�����csv�ļ�
 

	//�ִ�
	CStatic			m_cDraw_AAA;										//�ִ�����
	CSSplitter		m_SplitterPane_Main;								//��ֱ��
	CSSplitter		m_SplitterPane_Left;								//ˮƽ��

	//m_cTree
	CImageList		v_cImage_Tree;										//��ͼ�꣺����
	CImageList		v_cImage_State;										//��ͼ�꣺״̬
	int				v_iItem_Tree;										//ѡ�е�Tree��Ŀ���

	//�̣߳�����
	CWinThread*		v_pThread_Input;									//�߳�ָ��
	static UINT		My_Thread_Inoput(LPVOID lparam);					//�̣߳�����
	void			My_Input_Main();									//����_������

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//
	CMyMutiTreeCtrl m_cTree;                                            //��
	CDateTimeCtrl	m_cDate;											//����
	CComboBox		m_cDay_Sum;                                         //����
	CButton			m_cDay;                                             //����
	CButton			m_cMuti_Line;                                       //����
	CButton			m_cKPI_S;                                           //��̬
	CButton			m_cLable;                                           //����
	CButton			m_cPoint;                                           //���ݵ�
	CEdit			m_cSum;												//
	CEdit			m_cKey;												//
	CComboBox		m_cOut_In;											//	
	                                
    //�Զ���
	CString			v_sFrame_Name;										//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item_Room;									//ѡ�е��б�Item [Room]
	
	mNode_Cell		v_mTree_All;										//�ڵ���Ϣ��Cell��
	HTREEITEM       v_hAll;                                             //ѡ�е�TREE�ڵ���

	//��������
	mLTE_PM_Type	v_mLTE_PM_Type;										//���������ܲ���
	set<CString>	v_mTable_Name;										//���ϣ����ܱ�����

	CString         v_sCell;											//ѡ����Ŀ��Cell����
	CString         v_sNetwork;											//ѡ����Ŀ������

	SYSTEMTIME		v_dChart_Date;										//��ѯ����
 	int				v_iChart_Day_Sum;									//��ѯ�������
 	HTREEITEM		v_hTree_Current;									//ѡ�е�Tree�ڵ�
	set<HTREEITEM>	m_set_Selected;										//�������洢ѡ�еĽڵ���Ϣ

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
	
	CComboBox m_cType;
	CButton m_cQuery;
	CButton m_cPM_Select;
	CButton m_cRefresh;
	CButton m_cClear;
	CButton m_cExcel;
	afx_msg void OnBnClickedButton1();
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
