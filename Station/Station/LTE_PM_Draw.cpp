// LTE_PM_Draw.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Draw.h"
#include <math.h>
#include <vector>
#include <sstream>
#include "LTE_PM_Draw_Select.h"


// CLTE_PM_Draw

IMPLEMENT_DYNCREATE(CLTE_PM_Draw, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CLTE_PM_Draw::CLTE_PM_Draw()
	: CFormView(CLTE_PM_Draw::IDD)
{
	v_bShow=false;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CLTE_PM_Draw::~CLTE_PM_Draw()
{
	v_mLTE_PM_Type.clear();												//�������
	v_mTable_Name.clear();												//�������
	v_mRegion.clear();													//�������
	delete m_cChartView.getChart();										//�ͷ���Դ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BEG_DATE, m_cDate);
	DDX_Control(pDX, IDC_COMBO1, m_cDay_Sum);
	DDX_Control(pDX, IDC_CK_DAY, m_cDay);
	DDX_Control(pDX, IDC_CK_MUTI, m_cMuti_Line);
	DDX_Control(pDX, IDC_CK_S, m_cKPI_S);
	DDX_Control(pDX, IDC_CK_VALUE, m_cLable);
	DDX_Control(pDX, IDC_YSCALE2, m_cPoint);
	DDX_Control(pDX, IDC_CHARTVIEW, m_cChartView);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_DRAW_AAA, m_cDraw_AAA);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_BUTTON1, m_cExcel);
	DDX_Control(pDX, IDC_INSTANCE_DETAIL, m_cPM_Select);
	DDX_Control(pDX, IDC_REFRESH, m_cRefresh);
	DDX_Control(pDX, IDC_DELETE, m_cClear);
	DDX_Control(pDX, IDC_COMBO3, m_cPM_Type);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Draw, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//Chart
	ON_WM_MOUSEWHEEL()
	ON_CONTROL(CVN_ViewPortChanged, IDC_CHARTVIEW, OnViewPortChanged)
	ON_CONTROL(CVN_MouseMovePlotArea, IDC_CHARTVIEW, OnMouseMovePlotArea)
	ON_MESSAGE(123,&CLTE_PM_Draw::Message_ChartView)					//�����Ϣӳ�� 

	ON_NOTIFY(DTN_CLOSEUP, IDC_BEG_DATE, &CLTE_PM_Draw::OnDtnCloseupBegDate)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CLTE_PM_Draw::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_REFRESH, &CLTE_PM_Draw::OnBnClickedRefresh)
	ON_BN_CLICKED(IDC_DELETE, &CLTE_PM_Draw::OnBnClickedDelete)
	//MutiTreCtrl����Ϣ [�����State]
	ON_BN_CLICKED(IDC_CK_MUTI, &CLTE_PM_Draw::OnBnClickedCkMuti)
	ON_BN_CLICKED(IDC_QUARY, &CLTE_PM_Draw::OnBnClickedQuary)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Draw::OnNMClickList1)
	ON_BN_CLICKED(IDC_INSTANCE_DETAIL, &CLTE_PM_Draw::OnBnClickedInstanceDetail)
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Draw::OnBnClickedButton1)
	ON_CBN_CLOSEUP(IDC_COMBO3, &CLTE_PM_Draw::OnCbnCloseupCombo3)
END_MESSAGE_MAP()


// CLTE_PM_Draw ���

#ifdef _DEBUG
void CLTE_PM_Draw::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLTE_PM_Draw::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLTE_PM_Draw ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CString v_sTemp;													//��ʱ����
	//��������
	v_sFrame_Name="LTE ����ͼ�β�ѯ";									//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б� [С��]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);
	m_cList.InsertColumn(1,"С��",LVCFMT_LEFT,200);  
	m_cList.InsertColumn(2,"С��ID",LVCFMT_LEFT,0);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES );

	//�ִ� [��������ߵ����¹�ϵ��ˮƽ�ָ��������������ҹ�ϵ�Ĵ�ֱ�ָ�������2*1]
	CRect	v_cRect;													//����
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//��ã�ָ��
	pWnd->GetWindowRect(&v_cRect);										//��ã��ߴ�
	ScreenToClient(&v_cRect);											//����任
	pWnd->DestroyWindow();												//���٣�ָ��

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Main.Create(											//��ֱ��
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_cList,				// Left pane
		&m_cDraw_AAA,			// Right pane
		IDC_LTE_PM_Draw_Main,		// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);

	//��ʼ��������ѡ��
	for (int vi=1;vi<=c_iSample_Day;vi++)								//�����
	{
		v_sTemp.Format("%d",vi);
		m_cDay_Sum.AddString(v_sTemp);									//��ѡ������ѡť
	}
	m_cDay_Sum.SetCurSel(0);											//Ĭ�ϲ�ѯ������1�죻

	//Chart��ʼ��
	v_bShow=true;
    GetLocalTime(&v_dChart_Date);										//��ã���ѯ����
	v_iChart_Day_Sum=1;													//��ã���ѯ���������1��
	CRect v_pRec; 
	m_cChartView.GetWindowRect(&v_pRec); 
	ScreenToClient(&v_pRec); 
	m_cChartView.v_cUI_Data.v_iChart_X0=v_pRec.left;
	m_cChartView.v_cUI_Data.v_iChart_Y0=v_pRec.top;						//���
	// Initialize the CChartViewer
	m_cChartView.getDefaultBgColor();									//����Ĭ�ϱ���ɫ
	OnChart_Para();														//���ݣ���ͼ����
	m_cChartView.initChartViewer(&m_cChartView);						//��ʼ��Chart
	// Trigger the ViewPortChanged event to draw the chart
	m_cChartView.updateViewPort(true, true);							//��ͼ��Chart

	//ˢ�½���
	PostMessage(WM_SIZE);

	//��Ϣ��ʾ
	v_iList_Item=-1;													//��Чѡ��
	My_Show_Clear();													//ɾ��Tree�ؼ���Ϣ
												
	//������ʼ��
	m_cType.AddString("15m����");
	m_cType.AddString("1H����");
	m_cType.SetCurSel(0);												//Ĭ�ϣ�15m����

	// TODO: �ڴ����ר�ô����/����û���
	My_LoadData_Para();													//��ȡ�����ܲ���
	OnCbnCloseupCombo3();												//��������ѡ��

	OnBnClickedInstanceDetail();										//����ѡ��
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Draw::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)	//������Ϣ������
	{
		// Translate dialog key if applicable
		if(::IsDialogMessage(m_hWnd, pMsg))								//ת����Ϣ [����Ϣ����m_hWnd]
			return TRUE;												    //��Ϣ���� [����Ѹ���Ϣ�ַ������ں�������]
	}

	return CFormView::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ӵ�����
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//�����Ӵ�����
	if(v_pIterator_Frame != theApp.v_mFrame.end())						//�Ӵ����ڣ�
		theApp.v_mFrame.erase(v_pIterator_Frame);						//������������Ӵ�����
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����������С
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
	CWnd	*v_pWnd;													//�����ؼ���ָ��

	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//�ִ��ߴ����
		CRect v_cRect;
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=45;
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}

	//Chart
	if (v_bShow)														//Chart����
	{
		CRect lpRec; 
		m_cDraw_AAA.GetWindowRect(&lpRec); 
		ScreenToClient(&lpRec); 
		m_cChartView.v_cUI_Data.v_iChart_X0=lpRec.left;
		m_cChartView.v_cUI_Data.v_iChart_Y0=lpRec.top;					//���
		v_pWnd = GetDlgItem(IDC_CHARTVIEW);								//��ͼ
		if(v_pWnd)
			v_pWnd->MoveWindow(lpRec.left,lpRec.top,10,10);				//��������

		GetClientRect(&lpRec); 
		m_cChartView.v_cUI_Data.v_iChart_X=lpRec.right;					//���ڿ�
		m_cChartView.v_cUI_Data.v_iChart_Y=lpRec.bottom;				//���ڸ�

		m_cChartView.updateViewPort(true, false);						//��ͼChart
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ChartView
//	��    ������(1)���̣߳��������ݻ�ȡ��(2)������Ϣ��(3)��Ϣ��Ӧ�����û�����������ߣ���
//------------------------------------------------------------------------------------------------------
LRESULT CLTE_PM_Draw::Message_ChartView(WPARAM iParam1,LPARAM iParam2)
{
	//��ͼ��Chart
	m_cChartView.updateViewPort(true, false);							//ͼ�λ���

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������ѡ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnCbnCloseupCombo3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp;												//��ʱ����
	int			v_iItem;												//��ʱ����
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	//��1��ѡ��Χ��С��  or ����
	m_cList.DeleteAllItems();											//ɾ��������Ŀ
	v_iList_Item=-1;													//��λ
	while(m_cList.DeleteColumn(0));										//ɾ�������У�С���������б�
	if (m_cPM_Type.GetCurSel()==0)										//��������
	{
		m_cType.EnableWindow(true);										//��Ч
		m_cQuery.EnableWindow(true);									//��Ч
		m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);
		m_cList.InsertColumn(1,"С��",LVCFMT_LEFT,200); 
		m_cList.InsertColumn(2,"С��ID",LVCFMT_LEFT,0);  
	}
	else																//��������
	{
		m_cType.SetCurSel(0);											//ѡ��15mins
		m_cType.EnableWindow(false);									//��Ч
		m_cQuery.EnableWindow(false);									//��Ч
		m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);
		m_cList.InsertColumn(1,"����",LVCFMT_LEFT,200);  
		m_cList.InsertColumn(2,"����",LVCFMT_LEFT,0);  

		v_iItem = m_cList.InsertItem(0xffff,_T("0"));
		v_sTemp.Format(" %04d",v_iItem+1);
		m_cList.SetItemText(v_iItem,0," "+v_sTemp);						//���
		m_cList.SetItemText(v_iItem,1,(_bstr_t)"ȫ��");					//ȫ��
		m_cList.SetItemText(v_iItem,2,(_bstr_t)"ȫ��");					//ȫ��
		v_iItem = m_cList.InsertItem(0xffff,_T("0"));
		v_sTemp.Format(" %04d",v_iItem+1);
		m_cList.SetItemText(v_iItem,0," "+v_sTemp);						//���
		m_cList.SetItemText(v_iItem,1,(_bstr_t)"��Ϊ");					//��Ϊ
		m_cList.SetItemText(v_iItem,2,(_bstr_t)"��Ϊ");					//��Ϊ
		v_iItem = m_cList.InsertItem(0xffff,_T("0"));
		v_sTemp.Format(" %04d",v_iItem+1);
		m_cList.SetItemText(v_iItem,0," "+v_sTemp);						//���
		m_cList.SetItemText(v_iItem,1,(_bstr_t)"����");					//����
		m_cList.SetItemText(v_iItem,2,(_bstr_t)"����");					//����
		set<CString>::iterator v_pTable=v_mRegion.begin();				//��ʼ
		while (v_pTable!=v_mRegion.end())								//����������
		{
			v_iItem = m_cList.InsertItem(0xffff,_T("0"));
			v_sTemp.Format(" %04d",v_iItem+1);
			m_cList.SetItemText(v_iItem,0," "+v_sTemp);					//���
			m_cList.SetItemText(v_iItem,1,*v_pTable);					//����
			m_cList.SetItemText(v_iItem,2,*v_pTable);					//����

			v_pTable++;
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����б� [С��]
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item>=0)											//��Ч��Ŀ��ţ�����
				m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));	//�ָ�����ɫ
			v_iList_Item=pNMListView->iItem;							//��ã�ѡ����Ŀ���
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));			//���ñ���ɫ����ɫ

			if(!m_cMuti_Line.GetCheck())								//����
			{
				v_iWorking=2;											//�������񣺵��߻���
				v_pIterator_Frame->second.v_iThread_Exit=1;				//�̣߳�����
				v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);	//������ѯ���ݿ��߳�
			}
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ˢ�¡����»������ߡ�
//------------------------------------------------------------------------------------------------------	
void CLTE_PM_Draw::OnBnClickedRefresh()
{
 	v_iWorking=3;														//�����������߻��ơ����ߡ����ߡ�
 	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
 	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�С����ѯ
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (v_pIterator_Frame->second.v_iThread_Exit==0)
	{
		v_iWorking=1;													//�������񣺲�ѯ

		v_pIterator_Frame->second.v_iThread_Exit=1;						//�̣߳�����
		v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);			//������ѯ���ݿ��߳�
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnBnClickedInstanceDetail()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CLTE_PM_Draw_Select	v_cDlg;											//ʵ����

 	v_cDlg.v_mLTE_PM_Type=&v_mLTE_PM_Type;								//ָ�룺���ܲ���
	v_cDlg.v_iSelect_PM_Type=m_cPM_Type.GetCurSel();					//ѡ��������������

	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܵ���
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	My_SendToFile();													//������CSV�ļ�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CLTE_PM_Draw::My_Thread_Inoput(LPVOID lparam)
{
	CLTE_PM_Draw *  lp_this = NULL ;
	lp_this = (CLTE_PM_Draw *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cPM_Type.EnableWindow(false);							//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(false);								//��������Ч
	lp_this->m_cPM_Select.EnableWindow(false);							//��������Ч
	lp_this->m_cRefresh.EnableWindow(false);							//��������Ч
	lp_this->m_cClear.EnableWindow(false);								//��������Ч

	lp_this->My_Input_Main();											//����_������

	if (lp_this->m_cPM_Type.GetCurSel()==0)
	{
		lp_this->m_cQuery.EnableWindow(true);							//��ѯ����Ч
	}
	lp_this->m_cPM_Type.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(true);								//��������Ч
	lp_this->m_cPM_Select.EnableWindow(true);							//��������Ч
	lp_this->m_cRefresh.EnableWindow(true);								//��������Ч
	lp_this->m_cClear.EnableWindow(true);								//��������Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Cell();												//װ��Cell��Ϣ
		break;
	case 2:
		My_Chart();														//���߻���
		break;
	case 3:
		My_Chart_Muti();												//���߻��ơ����ߡ����ߡ�
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��Cell��Ϣ
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::My_LoadData_Cell()
{
	CString		v_sSql;													//��ʱ����
	int			v_iItem,v_iID=0,v_iCell_ID;								//��ʱ����
	CString		v_sTemp,v_sCell,v_sCell_ID;								//��ʱ����
	CString		v_sLog_Key;												//��־��ѯ��KEY

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList.DeleteAllItems();										//����б�����
		v_iList_Item=-1;												//��Чѡ��

		//��ѯ����׼��
		m_cKey.GetWindowText(v_sLog_Key);								//��ã���ѯKEY
		v_sLog_Key.Replace("[","[[]");									//���ƴ���
		v_sLog_Key.Replace("'","");										//���ƴ���
		v_sLog_Key.Trim();												//ȥ���ո�

		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zCell_ID from LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB where LTE_NODEB.zRNC like '%dalian%'";							//��ѯ���ݿ�
		if (!v_sLog_Key.IsEmpty())										//�йؼ��ֲ�ѯ������
			v_sSql+=" and zCell like '%"+v_sLog_Key+"%'";				//��ѯ���ݿ�
		v_sSql+=" order by zCell";										//��ѯ���ݿ�

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sCell=v_cAdoConn.GetField_String("zCell");				//zCell
			v_iCell_ID=v_cAdoConn.GetField_Int("zCell_ID");				//zCell_ID

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sCell);			//Cell
			v_sCell_ID.Format("%d",v_iCell_ID);							//�ַ���
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sCell_ID);			//Cell_ID

			v_pRecordset->MoveNext();									//
			v_iID++;													//

			if (v_iID%100==0)
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum.SetWindowText(v_sTemp);							//��ʾ��Log����
			}
		}

		v_sTemp.Format("%d",v_iID);										//
		m_cSum.SetWindowText(v_sTemp);									//��ʾ��Log����

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Chart
////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------------
//	�������ܣ�Draw track cursor when mouse is moving over plotarea
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnMouseMovePlotArea()
{
	// Get the focus to ensure being able to receive mouse wheel events
	m_cChartView.SetFocus();

	m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),m_cLable.GetCheck()); 
	m_cChartView.updateDisplay();
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�����ֻ�����Ϣ [��;���Ŵ���С������ƽ��]
//------------------------------------------------------------------------------------------------------
BOOL CLTE_PM_Draw::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// Process the mouse wheel only if the mouse is over the plot area
	if (!m_cChartView.isMouseOnPlotArea())
		return FALSE;

	// We zoom in or out by 10% depending on the mouse wheel direction.
	double newVpWidth = m_cChartView.getViewPortWidth() * (zDelta > 0 ? 0.7 : 1 / 0.7);
	double newVpHeight = m_cChartView.getViewPortHeight() * (zDelta > 0 ? 0.7 : 1 / 0.7);

	// We do not zoom beyond the zoom width or height limits.
	newVpWidth = max(m_cChartView.getZoomInWidthLimit(), min(newVpWidth,
		m_cChartView.getZoomOutWidthLimit()));
	newVpHeight = max(m_cChartView.getZoomInHeightLimit(), min(newVpWidth,
		m_cChartView.getZoomOutHeightLimit()));

	if ((newVpWidth != m_cChartView.getViewPortWidth()) || 
		(newVpHeight != m_cChartView.getViewPortHeight()))
	{
		// Set the view port position and size so that the point under the mouse remains under
		// the mouse after zooming.

		double deltaX = (m_cChartView.getPlotAreaMouseX() - m_cChartView.getPlotAreaLeft()) * 
			(m_cChartView.getViewPortWidth() - newVpWidth) / m_cChartView.getPlotAreaWidth();
		m_cChartView.setViewPortLeft(m_cChartView.getViewPortLeft() + deltaX);
		m_cChartView.setViewPortWidth(newVpWidth);

		double deltaY = (m_cChartView.getPlotAreaMouseY() - m_cChartView.getPlotAreaTop()) *
			(m_cChartView.getViewPortHeight() - newVpHeight) / m_cChartView.getPlotAreaHeight();
		m_cChartView.setViewPortTop(m_cChartView.getViewPortTop() + deltaY);
		m_cChartView.setViewPortHeight(newVpHeight);

		m_cChartView.updateViewPort(true, false);
	}

	return true;
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�View port changed event
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnViewPortChanged()
{
	// Update the chart if necessary
	if (m_cChartView.needUpdateChart())
	{
		OnChart_Para();													//��ã���ͼ����
		m_cChartView.drawChart(&m_cChartView);							//��ͼ
	}

	// We need to update the track line too. If the mouse is moving on the chart (eg. if 
	// the user drags the mouse on the chart to scroll it), the track line will be updated
	// in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
	if ((!m_cChartView.isInMouseMoveEvent()) && m_cChartView.isMouseOnPlotArea())
	{
		m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),m_cLable.GetCheck());
		m_cChartView.updateDisplay();
	}
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ����ݻ�ͼ����  [����Ļ��Chart����]
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnChart_Para()
{
	CString		v_sTemp;												//��ʱ����

	m_cDate.GetTime(m_cChartView.v_cUI_Data.v_dDate_Start);				//��ã���ѯ��ʼ����
	m_cChartView.v_cUI_Data.v_iChart_Day_Sum=m_cDay_Sum.GetCurSel()+1;	//��ѯ����
	m_cChartView.v_cUI_Data.v_iDay=m_cDay.GetCheck();					//������Ч
	m_cChartView.v_cUI_Data.v_iMuti_Line=m_cMuti_Line.GetCheck();		//������Ч
	m_cChartView.v_cUI_Data.v_iKPI_S=m_cKPI_S.GetCheck();				//��̬��Ч
	m_cChartView.v_cUI_Data.v_iPoint=m_cPoint.GetCheck();				//���ݵ���Ч

	m_cChartView.v_cUI_Data.v_sKPI_Name = "��������";					//��Ŀ����
	
	m_cChartView.v_cUI_Data.v_iY_scale=0;								//y��̶���Ч

	if (m_cType.GetCurSel()==0)											//15m ???
		m_cChartView.v_cUI_Data.v_iTime=0;								//������Ϣ��15min
	else
		m_cChartView.v_cUI_Data.v_iTime=1;								//������Ϣ��ʱ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������Ļ��ʾ��Ϣ
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::My_Show_Clear()									    //�����Ļ��ʾ��Ϣ
{
	v_iList_Item=-1;													//ѡ�е�Item��Ч
	My_Chart_New();														//��ͼ��λ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::OnBnClickedCkMuti()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_cMuti_Line.GetCheck())										//��������Ч��
	{
 		m_cDay.SetCheck(false);                                         //��Ч�����죻
 		m_cKPI_S.SetCheck(false);										//��Ч����̬���ޣ�
	}

	OnBnClickedDelete();												//���Chart
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ͼ�� [��ͼ��λ]
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//����б��ѡ��״̬
	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//�����б�
		m_cList.SetCheck(vi,0);											//���ѡ��״̬

	//��ͼ��ʼ��
	My_Chart_New();														//��ͼ��λ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ͼ��λ
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::My_Chart_New()
{
	//��ͼ��ʼ��
	OnChart_Para();														//���ݣ���ͼ����
	m_cChartView.initChartViewer(&m_cChartView);						//��ʼ��Chart
	//��ͼ��Chart
	m_cChartView.updateViewPort(true, false);							//��ͼ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����߻��ơ����ߡ����ߡ�
//------------------------------------------------------------------------------------------------------	
void CLTE_PM_Draw::My_Chart_Muti()
{
	if(v_iList_Item!=-1)												//��Ч��Ŀ��ţ�����
	{

		if(!m_cMuti_Line.GetCheck())									//����
		{
            My_Chart();													//���߻���
        }
		if (m_cMuti_Line.GetCheck())									//����
		{
			My_Chart_New();												//��ͼ��λ
	
			for (int vi=0;vi<m_cList.GetItemCount();vi++)				//�����б�
			{
				if(m_cList.GetCheck(vi))								//�жϣ�ѡ��״̬������
					My_List_Chart(vi);									//�������ߡ����ߡ�
			}
      	}
	
     }
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ͼ��λȻ���ҵ�ǰ�ڵ�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::My_Chart()
{
	//��ͼ��λ
	My_Chart_New();														//��ͼ��λ

	//��ͼ������Tree���ҵ�ѡ�еĽڵ�	
	My_List_Chart(v_iList_Item);										//��ͼ��ѡ�е�Item
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ���ͼ��ѡ�е�Tree Node
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::My_List_Chart(int v_iList_Item)
{
	My_List_Chart_GetNode(v_iList_Item);								//��ȡ���ڵ���Ϣ��������public�����С�
	My_Chart_IP();														//���ƣ�������Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ�ڵ���Ϣ 
//	��    ����v_iList_Item��ѡ�е�Item
//			 �����ڣ���ͼ����ʾ�ڵ���Ϣ��������v_sCell�����С�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::My_List_Chart_GetNode(int v_iList_Item)
{
	v_sCell=m_cList.GetItemText(v_iList_Item,1);						//Cell����
	v_sCell_ID=m_cList.GetItemText(v_iList_Item,2);						//Cell_ID
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�Chart��һ����
//	��    ����ѡ�е� IP [v_sItem_IP]
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::My_Chart_IP()
{
	CString					v_sTemp;									//��ʱ����
	bool					v_bOK=false;								//��ʱ����
	mLTE_PM_Type::iterator	v_pSelect;									//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����
	m_cDate.GetTime(v_dDate_Start);										//��ã�ѡ����������
	v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//����ʱ�䣺00:00:00

	//�������������͡��������ܡ��������ܡ�
	for (v_pSelect=v_mLTE_PM_Type.begin();v_pSelect!=v_mLTE_PM_Type.end();v_pSelect++)
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//ǿ���˳�������
			break;														//�˳�

		m_cPM_Type.GetWindowText(v_sTemp);
		if (v_pSelect->first==v_sTemp)									//��ѡ���һ�£�����
		{
			//����ѡ����������ͣ�����ѡ������ܣ����л�ͼ��
			for (v_pPM_Name=v_pSelect->second.v_mLTE_PM.begin();v_pPM_Name!=v_pSelect->second.v_mLTE_PM.end();v_pPM_Name++)
			{
				if (v_pIterator_Frame->second.v_iThread_Exit==2)		//ǿ���˳�������
					break;												//�˳�
				
				//��ȡ�������ݿ⣬����ͼ
				if (v_pPM_Name->second.v_bOK)							//����ѡ�У�����
				{
					My_Load_Performance(v_pSelect->first,				//��������
					v_pPM_Name->second.v_sTable_Key,					//�����ļ�����key
					v_sCell,											//С������
					v_sCell_ID,											//С��ID
					v_pPM_Name->second.v_sPM_Name,						//��������
					v_pPM_Name->first,									//�����ֶ�
					v_dDate_Start,										//��ʼ����
					v_iChart_Day_Sum);									//��ѯ����
					
					v_bOK=true;											//��־����ͼ��Ч
				}
			}
		}
	}
	if (!v_bOK)
		MessageBox("  ��ѡ����Ч������ !!! ");							//������ʾ
	else
		::PostMessage(m_hWnd, 123, (WPARAM) 0, (LPARAM) 0);				//������Ϣ������ChartView	
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ��
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::OnDtnCloseupBegDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cDate.GetTime(&v_dChart_Date);									//��ã���ѯ��ʼ����

	My_Chart_New();														//��ͼ��λ

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ��
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iChart_Day_Sum=m_cDay_Sum.GetCurSel()+1;							//��ã���ѯ�������

	My_Chart_New();														//��ͼ��λ
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�	��ȡ���ݱ��е���������
//	������		v_sCell_ID��Cell_ID��	v_dDate_Start����ѯ����ʼ���ڣ�	v_iDay_Sum����ѯ����
//	��ȡ���̣�	�Ӷ�Ӧ���л�ȡ���ݣ�����v_mIP_Instance�����У�[��Ÿ������ȵ�IP����]
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::My_Load_Performance(CString v_sType,CString v_sFile_Key,CString v_sCell,CString v_sCell_ID,CString v_sPerformance,CString v_sField,COleDateTime v_dDate_Start,int v_iDay_Sum)
{
	CString			v_sSql,v_sTemp,v_sTime;								//��ʱ����
	CString			v_sTime_Start,v_sTime_End;							//����
	int				v_iNum_Old=-1;										//��¼�����
	CString			v_sTable_Num,v_sTable_Num_New;						//ʵʱ�����
	double			v_dValue;											//��ʱ����
	CString			v_sInstance;										//Instance_ID
	COleDateTime	v_dTime;											//��ѯ����
	Chart_Data		v_cChart_Data;										//��������
	_variant_t		Var;												//��ʱ����
	CString			v_sNet,v_sDate;										//��ʱ����

	//��1�����ݳ�ʼ�����������������ڴ�
	v_cChart_Data.v_dChart_Value = new double[m_cChartView.v_iTime_Sum];//��������ռ�
	for(int vi=0;vi<m_cChartView.v_iTime_Sum;vi++)
		v_cChart_Data.v_dChart_Value[vi]=Chart::NoValue;				//��ʼ��������

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//�� 2�������ݱ��ж�ȡ��������
		int v_iDay=0;													//��ȡ����
		while(v_iDay<v_iDay_Sum)										//ѭ������ѯ����
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			if (m_cPM_Type.GetCurSel()==0)								//�������ܣ�����
			{
				//ȷ�������ƣ�Сʱ��5��1����(ÿ��6����)��15min��1��1����
				v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//��ʼʱ��
				if (m_cType.GetCurSel()==1)								//1H���ȣ�����
				{
					//Hour��5��1����(ÿ��6����)
					while (true)
					{
						v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());		//��ȡ�����
						v_dTime=v_dDate_Start;							//1�����ѯ��������
						//�����
						v_dDate_Start += COleDateTimeSpan(1,0,0,0);		//����һ��
						v_iDay++;										//
						if (v_iDay>=v_iDay_Sum)
							break;
						v_sTable_Num_New=My_Get_Table_Num(v_dDate_Start.GetDay());	//��ȡ�����
						if (v_sTable_Num!=v_sTable_Num_New)
							break;
					}
					v_sTime_End=v_dTime.Format("%Y-%m-%d")+" 23:59:59";	//����ʱ��
					v_sNet="Pm_LTE_"+v_sFile_Key;						//�ļ�����
					v_sDate=v_dTime.Format("%Y%m");						//����
					v_sNet+="_"+v_sDate+"_"+v_sTable_Num;				//���������ӣ�����_���
				}
				else													//15mins
				{
					//15mins��1��1����
					//��ȡ��������
					v_sTime_End=v_dDate_Start.Format("%Y-%m-%d")+" 23:59:59";	//����ʱ��
					v_sNet="Pm_LTE_"+v_sFile_Key;						//�ļ�����
					v_sDate=v_dDate_Start.Format("%Y%m%d");				//������
					v_sNet+="_"+v_sDate;								//���������ӣ�������
					//��+1
					v_dDate_Start += COleDateTimeSpan(1,0,0,0);			//����һ��
					v_iDay++;											//
				}
			}
			else
			{															//��������
				//�������ܣ��ܹ�һ����(Pm_LTE_Region)
				v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//��ʼʱ��
				//��+1
				v_dDate_Start += COleDateTimeSpan(v_iDay_Sum-1,0,0,0);	//���ӣ���ѯ����
				v_iDay+=v_iDay_Sum;										//��ѯ����
				v_sTime_End=v_dDate_Start.Format("%Y-%m-%d")+" 23:59:59";	//����ʱ��
				v_sNet="Pm_LTE_"+v_sFile_Key;							//�ļ�����
			}

			//��飺�������Ƿ���Ч�����ݣ������Ƽ���v_mTable_Name��
			set<CString>::iterator v_pTable=v_mTable_Name.find(v_sNet);	//���ң���
			if (v_pTable==v_mTable_Name.end())							//û�ҵ�
				continue;												//��Ч����

			//�ֶδ���һ���ֶΡ�
			v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
			v_sSql+="select zTime,["+v_sField+"]";

			//SQL�������֯
			v_sSql+=" from "+v_sNet+" where zCell='"+ v_sCell_ID+"' and zTime>='"
				+ v_sTime_Start +"' and  zTime<='"+ v_sTime_End +"' order by zTime";	//��ѯ���ݿ⣺���ܱ�
			v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);	//ִ��SELETE���										

			//���ݻ�ȡ
			while(!v_pRecordset->adoEOF)
			{
				if (v_pIterator_Frame->second.v_iThread_Exit==2)		//ǿ���˳�������
					break;												//�˳�

				//ֱ�Ӷ�ȡ�ֶΣ�Ŀ������߶�ȡ�ٶȣ�[�����ֶζ�ȡ����ȡֵЧ�ʵͣ�����]
				Var=v_pRecordset->GetCollect((_variant_t)"zTime");		//����
				if (Var.vt != VT_NULL)
					v_dTime=Var.date;									//
				else
					v_dTime=COleDateTime::GetCurrentTime();				//Ĭ�ϣ���ǰ����

				//�ֶζ�ȡ
				v_dValue=v_cAdoConn.GetField_Double(v_sField);			//����
				if(v_dValue==-1)										//-1����ĸΪ�㣬��Ч����
					v_dValue=Chart::NoValue;							//��Ч���ݣ������ߣ�

				//���ݱ��棺�ѻ�ȡ�ĸ��������ݱ��浽v_cChart_Data.v_dChart_Value��ǰ���Ǹ���������Ч�ķ�Χ�ڣ�
				double	m_nextDataTime = Chart::chartTime(v_dTime.GetYear(),v_dTime.GetMonth(),v_dTime.GetDay(),v_dTime.GetHour(),v_dTime.GetMinute(),0);		//��ȷ����
				mTime_Array::iterator v_pIterator_Time=m_cChartView.v_mTime_Array.find(m_nextDataTime);		//��������[����m_nextDataTime]
				if(v_pIterator_Time != m_cChartView.v_mTime_Array.end())//�ҵ���
				{
					int v_iNum=v_pIterator_Time->second;				//��ã��������
					v_cChart_Data.v_dChart_Value[v_iNum]=v_dValue;		//��ֵ������
				}

				v_pRecordset->MoveNext();								//
			}
		}
		v_cAdoConn.ExitConnect();										//�Ͽ�����

		//��3������Ǩ��(�������������������ͼ������)
		v_cChart_Data.v_sName = v_sCell+"_"+v_sPerformance;				//��ʾ������
		m_cChartView.v_mChart_Data.insert(pair<CString,Chart_Data>(v_sCell+"_"+v_sPerformance,v_cChart_Data));	//��ӣ�map������IP_Instance
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ�����
//	��	  ����1��1-5��	2��6-10��	3��11-15��	4��16-20��	5��21-25��	6��26--
//------------------------------------------------------------------------------------------------------			
CString CLTE_PM_Draw::My_Get_Table_Num(int v_iDay)
{
	CString	v_sNum;

	if (v_iDay<=5)
		v_sNum="1";
	else if(v_iDay<=10)
		v_sNum="2";
	else if(v_iDay<=15)
		v_sNum="3";
	else if(v_iDay<=20)
		v_sNum="4";
	else if(v_iDay<=25)
		v_sNum="5";
	else 
		v_sNum="6";

	return v_sNum;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ�����ܲ�����Ϣ
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::My_LoadData_Para()
{
	int				v_iID=0;											//��ʱ����
	CString			v_sTemp,v_sSql,v_sLog_Key,v_sTable;					//��ʱ����
	CString			v_sType,v_sPm;										//��ʱ����

	LTE_PM_Struct	v_cLTE_PM;											//�ṹ
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	LTE_PM_Type_Struct		v_cLTE_PM_Type;								//�ṹ

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��1���������ͣ���������
		v_sType="��������";												//��������								
		v_cLTE_PM_Type.v_sTable_Key=v_sType;							//������-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//��������
		//��ʾ����������
		m_cPM_Type.AddString(v_sType);									//��������

		//��ȡ�����ܲ��������桿
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Cal order by zPm";			//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sPm = v_cAdoConn.GetField_String("zPm");					//zPm

			//����
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//���ң���������
			if (v_pLTE_PM_Type!=v_mLTE_PM_Type.end())					//�ҵ�������
			{
				v_cLTE_PM.v_sPM_Name=v_sPm;								//���ܣ�������
				v_cLTE_PM.v_sTable_Key="Cal";							//���ܣ�������
				v_cLTE_PM.v_bOK=false;									//���ܣ�δѡ��
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//���ӣ����ܲ���
			}

			v_pRecordset->MoveNext();									//
		}
		m_cPM_Type.SetCurSel(0);										//Ĭ�ϣ���һ��
		m_cType.SetCurSel(0);											//ѡ��15mins
		m_cType.EnableWindow(false);									//��Ч

		//��2���������ͣ���������
		v_sType="��������";												//��������								
		v_cLTE_PM_Type.v_sTable_Key=v_sType;							//������-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//��������
		//��ʾ����������
		m_cPM_Type.AddString(v_sType);									//��������

		//��ȡ�����ܲ���
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Region order by zPm";		//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sPm = v_cAdoConn.GetField_String("zPm");					//zPm

			//����
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//���ң���������
			if (v_pLTE_PM_Type!=v_mLTE_PM_Type.end())					//�ҵ�������
			{
				v_cLTE_PM.v_sPM_Name=v_sPm;								//��������
				v_cLTE_PM.v_sTable_Key="Region";						//���ܣ�������
				v_cLTE_PM.v_bOK=false;									//���ܣ�δѡ��
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//���ӣ����ܲ���
			}

			v_pRecordset->MoveNext();									//
		}
		m_cPM_Type.SetCurSel(1);										//Ĭ�ϣ���һ��

		//��3����ȡ���������ݱ����ơ����ڷ�����Ч�����ơ�
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_LTE_Cal%' or name like 'Pm_LTE_Region%') order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//������

			v_mTable_Name.insert(v_sTable);								//���Ӽ���

			v_pRecordset->MoveNext();									//ָ��++
		}	

		//��4����ȡ��������Ϣ�����������ѯ��
		v_sSql="select * from CON_Region";								//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("zRegion");			//zRegion

			v_mRegion.insert(v_sTable);									//����

			v_pRecordset->MoveNext();									//ָ��++
		}	

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}
}

//-------------------------------------------------------------------------------------------------
// ��������д��CSV�ļ�������Excel�򿪣�
//-------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::My_SendToFile() 
{
	CString	v_sStr,v_sCSV,v_sPath_Data,v_sCSV_File;						//��ʱ����
	CFile	v_fWriter;													//ʵ����
	CFileFind	finder;
	double		v_dKPI;													//KPIֵ
	mChart_Data::iterator v_pIterator_KPI;								//KPI_Instance��map����������

	if (m_cChartView.v_mChart_Data.size()==0)							//�������ݣ�
		return;															//û�У�����

	//Ŀ¼����
	v_sPath_Data=theApp.m_sPath+"\\DATA";								//�ļ�Ŀ¼
	if (!finder.FindFile(v_sPath_Data))									//���ڣ�
	{
		if (!::CreateDirectory(v_sPath_Data, NULL))						//�����ڣ�����Ŀ¼
		{
			MessageBox("�쳣��"+v_sPath_Data+" Ŀ¼����ʧ�ܡ�");		//ʧ��
			return;
		}
	}

	try
	{
		//��ʼ���ڼ���
		COleDateTime	v_dDate_Start;									//��ѯ��ʼ����
		v_dDate_Start = m_cChartView.v_cUI_Data.v_dDate_Start;			//��ȡ����ͼ��ѯ��ʼ����
		v_dDate_Start=COleDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);

		//����CSV�ļ����ļ�����v_sCSV_File
		v_sCSV_File = theApp.m_sPath+"\\DATA\\Excel.csv";				//���ã�CSV�ļ�����Excel.csv
		if(v_fWriter.Open(v_sCSV_File, CFile::modeCreate | CFile::modeWrite))		//���ļ����粻���������ɣ������������ļ���
		{
			if (m_cChartView.v_mChart_Data.size()>1)
			{
				v_sCSV="���,ʱ��";										//������
				v_pIterator_KPI=m_cChartView.v_mChart_Data.begin();		//��ͷ��ʼ
				while(v_pIterator_KPI != m_cChartView.v_mChart_Data.end())			//����������
				{
					v_sCSV += "," + v_pIterator_KPI->second.v_sName;	//�ڶ��С������С�...����n��
					v_pIterator_KPI++;									//��������һ
				}
				v_sCSV +="\r\n";										//�س�����
				v_fWriter.Write(v_sCSV,v_sCSV.GetLength());				//д���ַ���

				//�����ݴ���ȡ�б����ݣ�д��CSV
				for (int vi=0;vi<m_cChartView.v_iTime_Sum;vi++)
				{
					v_sCSV.Format("%d",vi+1);
					v_sCSV+=","+v_dDate_Start.Format("%Y-%m-%d_%H:%M");
					v_pIterator_KPI=m_cChartView.v_mChart_Data.begin();	//��ͷ��ʼ
					while(v_pIterator_KPI != m_cChartView.v_mChart_Data.end())	//����������
					{
						v_dKPI=v_pIterator_KPI->second.v_dChart_Value[vi];	//��ã�KPIֵ
						if (v_dKPI==Chart::NoValue)						//Ϊ��ֵ������
							v_dKPI=-1;									//��ֵ����ֵ
						v_sStr.Format("%6.2f",v_dKPI);					//���ݻ�ȡ
						v_sCSV += "," + v_sStr;							//�ڶ��С������С�...����n��
						v_pIterator_KPI++;								//��������һ
					}
					v_sCSV +="\r\n";									//�س�����
					v_fWriter.Write(v_sCSV,v_sCSV.GetLength());			//д���ַ���

					if (m_cType.GetCurSel()==0)							//15m
						v_dDate_Start += COleDateTimeSpan(0,0,15,0);	//���ȼ���
					else
						v_dDate_Start += COleDateTimeSpan(0,1,0,0);		//���ȼ���
				}
			}
			else
			{
				v_sCSV="���,ʱ��,��ֵ";								//������
				v_pIterator_KPI=m_cChartView.v_mChart_Data.begin();		//��ͷ��ʼ
				while(v_pIterator_KPI != m_cChartView.v_mChart_Data.end())			//����������
				{
					v_sCSV += "," + v_pIterator_KPI->second.v_sName;	//�ڶ��С������С�...����n��
					v_pIterator_KPI++;									//��������һ
				}
				v_sCSV +="\r\n";										//�س�����
				v_fWriter.Write(v_sCSV,v_sCSV.GetLength());				//д���ַ���

				//�����ݴ���ȡ�б����ݣ�д��CSV
				for (int vi=0;vi<m_cChartView.v_iTime_Sum;vi++)
				{
					v_sCSV.Format("%d",vi+1);
					v_sCSV+=","+v_dDate_Start.Format("%Y-%m-%d_%H:%M");
					v_pIterator_KPI=m_cChartView.v_mChart_Data.begin();	//��ͷ��ʼ
					v_dKPI=v_pIterator_KPI->second.v_dChart_Value[vi];		//��ã�KPIֵ
					if (v_dKPI==Chart::NoValue)							//Ϊ��ֵ������
						v_dKPI=-1;										//��ֵ����ֵ
					v_sStr.Format("%6.2f",v_dKPI);						//���ݻ�ȡ
					v_sCSV += "," + v_sStr;								//�ڶ��С������С�...����n��
					v_sCSV +="\r\n";									//�س�����
					v_fWriter.Write(v_sCSV,v_sCSV.GetLength());			//д���ַ���

					if (m_cType.GetCurSel()==0)							//15m
						v_dDate_Start += COleDateTimeSpan(0,0,15,0);	//���ȼ���
					else
						v_dDate_Start += COleDateTimeSpan(0,1,0,0);		//���ȼ���
				}
			}
			v_fWriter.Close();											//�ļ��ر�

			//���ļ�
			ShellExecute(NULL,"open", v_sCSV_File, NULL, NULL, SW_SHOWNORMAL);		//���ļ���Excel.csv

		}
		else
			MessageBox("��ر��Ѿ��򿪵ĵ����ļ��������µ���������");	//������ʾ
	}	
	catch(_com_error e)
	{
		CString v_sError = (char *)e.Description();						//��ã�������Ϣ
		MessageBox("�쳣"+v_sError);									//��ʾ��Ϣ
	}
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
