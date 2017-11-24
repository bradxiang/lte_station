// Alarm_LL_Real.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Alarm_LL_Real.h"
#include "Alarm_LL_Real_Time.h"
#include "LTE_PM_DATA_CASE.h"											//ģ�塾С�������ܡ�


// CAlarm_LL_Real

IMPLEMENT_DYNCREATE(CAlarm_LL_Real, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Real::CAlarm_LL_Real()
	: CFormView(CAlarm_LL_Real::IDD)
{
	v_bShow=false;														//
	v_sCell_All="";														//ѡ���С��[С��ģ��]
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Real::~CAlarm_LL_Real()
{
	v_mAlarm_LL_Real_Down.clear();										//�������
	v_mAlarm_LL_Real_Up.clear();										//�������
	v_mTable_Name.clear();												//�������
	v_mCell.clear();													//�������
	v_mCell_ID.clear();													//�������
	v_mLTE_PM_Type.clear();												//�������

	delete m_cChartView.getChart();										//�ͷ���Դ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_LIST3, m_cList);
	DDX_Control(pDX, IDC_QUARY2, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel);
	DDX_Control(pDX, IDC_QUARY3, m_cTime);
	DDX_Control(pDX, IDC_LOG_SUM, m_cLog_Sum);
	DDX_Control(pDX, IDC_KEY3, m_cLog_Key);
	DDX_Control(pDX, IDC_BUTTON1, m_cCell);
	DDX_Control(pDX, IDC_TIME, m_cTime_Real);
	DDX_Control(pDX, IDC_LIST1, m_cList_Cell);
	DDX_Control(pDX, IDC_DRAW_AAA, m_cDraw_AAA);
	DDX_Control(pDX, IDC_CHARTVIEW2, m_cChartView);
	DDX_Control(pDX, IDC_CHECK3, m_cAuto);
	DDX_Control(pDX, IDC_CHECK14, m_cOnline);
	DDX_Control(pDX, IDC_CK_DAY, m_cDay);
	DDX_Control(pDX, IDC_YSCALE2, m_cPoint);
	DDX_Control(pDX, IDC_COMBO1, m_cDay_Sum);
	DDX_Control(pDX, IDC_COMBO3, m_cPara);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAlarm_LL_Real, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()

	//Chart
	ON_WM_MOUSEWHEEL()
	ON_CONTROL(CVN_ViewPortChanged, IDC_CHARTVIEW2, OnViewPortChanged)
	ON_CONTROL(CVN_MouseMovePlotArea, IDC_CHARTVIEW2, OnMouseMovePlotArea)
	ON_MESSAGE(123,&CAlarm_LL_Real::Message_ChartView)					//�����Ϣӳ�� 

	ON_BN_CLICKED(IDC_EXCEL2, &CAlarm_LL_Real::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CAlarm_LL_Real::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CAlarm_LL_Real::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_QUARY2, &CAlarm_LL_Real::OnBnClickedQuary2)
	ON_BN_CLICKED(IDC_QUARY3, &CAlarm_LL_Real::OnBnClickedQuary3)
	ON_BN_CLICKED(IDC_BUTTON1, &CAlarm_LL_Real::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_EXCEL4, &CAlarm_LL_Real::OnBnClickedExcel4)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CAlarm_LL_Real::OnNMClickList1)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAlarm_LL_Real::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CAlarm_LL_Real::OnCbnSelchangeCombo3)
END_MESSAGE_MAP()


// CAlarm_LL_Real ���

#ifdef _DEBUG
void CAlarm_LL_Real::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAlarm_LL_Real::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAlarm_LL_Real ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	CString v_sTemp;													//��ʱ����

	//��������
	v_sFrame_Name="LTE ������С��ʵʱ";									//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б���С����
	::SendMessage(m_cList_Cell.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Cell.InsertColumn(0,"���",LVCFMT_CENTER,50);
	m_cList_Cell.InsertColumn(1,"С��",LVCFMT_LEFT,300);  
	m_cList_Cell.InsertColumn(2,"С��ID",LVCFMT_LEFT,0);  
	m_cList_Cell.SetExtendedStyle(m_cList_Cell.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT);

	//��ʼ�����б� [������]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"С��",LVCFMT_LEFT,200);						//��1�У�С��
	m_cList.InsertColumn(2,"С��ID",LVCFMT_LEFT,0);						//��2�У�С��ID
	m_cList.InsertColumn(3,"Ӫҵ��",LVCFMT_LEFT,60);					//��3�У�Ӫҵ��
	m_cList.InsertColumn(4,"״̬",LVCFMT_CENTER,40);					//��4�У�״̬
	m_cList.InsertColumn(5,"����",LVCFMT_CENTER,60);					//��5�У�����
	m_cList.InsertColumn(6,"����",LVCFMT_CENTER,120);					//��6�У�����
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//�ִ� [��������ߵ����¹�ϵ��ˮƽ�ָ��������������ҹ�ϵ�Ĵ�ֱ�ָ�������2*1]
	CRect	v_cRect;													//����
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//��ã�ָ��
	pWnd->GetWindowRect(&v_cRect);										//��ã��ߴ�
	ScreenToClient(&v_cRect);											//����任
	pWnd->DestroyWindow();												//���٣�ָ��

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Left.Create(											//��ֱ��
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList,				// Left pane
		&m_cDraw_AAA,			// Right pane
		IDC_ALARM_LL_REAL_1,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);
	m_SplitterPane_Left.m_bHorizSplitter_Parent=true;					//��ֱ�ָ���Ϊ����
	m_SplitterPane_Main.Create(											//��ֱ��
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_cList_Cell,			// Left pane
		&m_SplitterPane_Left,	// Right pane
		IDC_ALARM_LL_REAL_0,	// this ID is used for saving/restoring splitter position and it must be unique !
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
	m_cDay_Sum.SetCurSel(7);											//Ĭ�ϲ�ѯ������8�죻[һ��]

	m_cPara.AddString("1��");											//��������
	m_cPara.AddString("2��");
	m_cPara.AddString("3��");
	m_cPara.AddString("4��");
	m_cPara.AddString("5��");
	m_cPara.SetCurSel(0);												//Ĭ��

	//Chart��ʼ��
	v_bShow=true;
	GetLocalTime(&v_dChart_Date);										//��ã���ѯ����
	v_iChart_Day_Sum=m_cDay_Sum.GetCurSel()+1;							//��ã���ѯ�������
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

	//��ʱ����
	SetTimer(1,60000,NULL);												//��ʱ��1 mins 
	m_cAuto.SetCheck(true);												//��Ч���Զ�ˢ��

	//��Ϣ��ʼ��
	v_iList_Item=-1;													//��Ŀѡ����Ч
	v_iList_Item_Cell=-1;												//��Ŀѡ����Ч
	v_dPM_Date=COleDateTime::GetCurrentTime();							//Ĭ�ϣ���ǰʱ��

	//����Ĭ�ϣ���ѯ����
	v_bDown=false;														//��Ч
	v_bUp=false;														//��Ч
	for(int vi=0;vi<4;vi++)
	{
		//����
		v_dDown_Select[vi].v_bFlag=false;								//��Ч
		v_dDown_Select[vi].v_iCal=0;									//<
		v_dDown_Select[vi].v_dDate=COleDateTime::GetCurrentTime();		//��ǰʱ��
		v_dDown_Select[vi].v_dValues=100;								//ֵ
		v_dDown_Select[vi].v_iHour=8;									//8��
		//����
		v_dUp_Select[vi].v_bFlag=false;									//��Ч
		v_dUp_Select[vi].v_iCal=0;										//<
		v_dUp_Select[vi].v_dDate=COleDateTime::GetCurrentTime();		//��ǰʱ��
		v_dUp_Select[vi].v_dValues=100;									//ֵ
		v_dUp_Select[vi].v_iHour=8;										//8��
	}

	//��ȡ����������
	v_dPM_Date=My_LoadData_Time();										//��ȡ������ʱ��
	v_sTemp=v_dPM_Date.Format("%Y-%m-%d_%H:%S:%S");						//�ַ���[Сʱ����]
	m_cTime_Real.SetWindowText(v_sTemp);								//��ʾ������ʱ��

	//��ȡ�����ܲ���
	v_iWorking=2;														//�������񣺻�ȡ��ʼ����
	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CAlarm_LL_Real::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)	//������Ϣ������
	{
		// Translate dialog key if applicable
		if(::IsDialogMessage(m_hWnd, pMsg))								//ת����Ϣ [����Ϣ����m_hWnd]
			return TRUE;												//��Ϣ���� [����Ѹ���Ϣ�ַ������ں�������]
	}

	return CFormView::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ӵ�����
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnDestroy()
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
void CAlarm_LL_Real::OnSize(UINT nType, int cx, int cy)
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
		v_pWnd = GetDlgItem(IDC_CHARTVIEW2);							//��ͼ
		if(v_pWnd)
			v_pWnd->MoveWindow(lpRec.left,lpRec.top,10,10);				//��������

		GetClientRect(&lpRec); 
		m_cChartView.v_cUI_Data.v_iChart_X=lpRec.right;					//���ڿ�
		m_cChartView.v_cUI_Data.v_iChart_Y=lpRec.bottom;				//���ڸ�

		m_cChartView.updateViewPort(true, false);						//��ͼChart
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ʱ��
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_dTime;											//��ʱ����

	if (m_cAuto.GetCheck())												//�Զ�ˢ�£�����
	{
		v_dTime=My_LoadData_Time();										//��ȡ����������
		if (v_dTime!=v_dPM_Date)										//�������ȱ仯������
		{
			v_dPM_Date=v_dTime;											//��ȡ����ʱ��
			OnBnClickedQuary2();										//���ܲ�ѯ
		}
	}

	CFormView::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ChartView
//	��    ������(1)���̣߳��������ݻ�ȡ��(2)������Ϣ��(3)��Ϣ��Ӧ�����û�����������ߣ���
//------------------------------------------------------------------------------------------------------
LRESULT CAlarm_LL_Real::Message_ChartView(WPARAM iParam1,LPARAM iParam2)
{
	//��ͼ��Chart
	m_cChartView.updateViewPort(true, false);							//ͼ�λ���

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�������ѡ��
//	��	  ��������ѡ��Ĳ�����ţ���ȡ������������
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnCbnSelchangeCombo3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql,v_sTemp_1;										//��ʱ����
	int			v_iPara,v_iTime,v_iNum;									//��ʱ����
	CString		v_sTemp;												//��ʱ����
	COleDateTime v_tDate;												//��ʱ����

	int			v_iDown=0,v_iDown_Time[4];								//����
	int			v_iUp=0,v_iUp_Time[4];									//����
	double		v_dDown_Value[4],v_dUp_Value[4];						//��ֵ

	//��ʼ��
	for (v_iTime=0;v_iTime<4;v_iTime++)									//����������
	{
		//��ȡ�������ñ�
		v_iDown_Time[v_iTime]=0;										//��ʼ��
		v_iUp_Time[v_iTime]=0;											//��ʼ��
		v_dDown_Value[v_iTime]=0;										//��ʼ��
		v_dUp_Value[v_iTime]=0;											//��ʼ��

		//��ѯ�������ݱ�
		v_dDown_Select[v_iTime].v_bFlag=false;							//ѡ����Ч
		v_dDown_Select[v_iTime].v_dValues=0;							//��ֵ����λ
		v_dUp_Select[v_iTime].v_bFlag=false;							//ѡ����Ч
		v_dUp_Select[v_iTime].v_dValues=0;								//��ֵ����λ
	}

	//��ȡ��������
	v_iPara=m_cPara.GetCurSel()+1;										//��ȡ��������

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		//�������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��ѯ�����������ݵ�½�ʺ�+������ţ�
		v_sTemp.Format("%d",v_iPara);									//�ַ���
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select * from CON_Pm_LTE_LL where zAccount='"+theApp.a_sUser_Account+"' and zPara="+v_sTemp;	//��ѯ���ݿ�

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		if(!v_pRecordset->adoEOF)
		{
			//��ȡ��������������
			v_iDown=v_cAdoConn.GetField_Int("zDown");					//zDown
			v_iDown_Time[0]=v_cAdoConn.GetField_Int("zDown_Time_1");	//zDown_Time_1
			v_iDown_Time[1]=v_cAdoConn.GetField_Int("zDown_Time_2");	//zDown_Time_2
			v_iDown_Time[2]=v_cAdoConn.GetField_Int("zDown_Time_3");	//zDown_Time_3
			v_iDown_Time[3]=v_cAdoConn.GetField_Int("zDown_Time_4");	//zDown_Time_4
			v_dDown_Value[0]=v_cAdoConn.GetField_Double("zDown_Value_1");//zDown_Value_1
			v_dDown_Value[1]=v_cAdoConn.GetField_Double("zDown_Value_2");//zDown_Value_2
			v_dDown_Value[2]=v_cAdoConn.GetField_Double("zDown_Value_3");//zDown_Value_3
			v_dDown_Value[3]=v_cAdoConn.GetField_Double("zDown_Value_4");//zDown_Value_4

			//��ȡ��������������
			v_iUp=v_cAdoConn.GetField_Int("zUp");						//zUp
			v_iUp_Time[0]=v_cAdoConn.GetField_Int("zUp_Time_1");		//zUp_Time_1
			v_iUp_Time[1]=v_cAdoConn.GetField_Int("zUp_Time_2");		//zUp_Time_2
			v_iUp_Time[2]=v_cAdoConn.GetField_Int("zUp_Time_3");		//zUp_Time_3
			v_iUp_Time[3]=v_cAdoConn.GetField_Int("zUp_Time_4");		//zUp_Time_4
			v_dUp_Value[0]=v_cAdoConn.GetField_Double("zUp_Value_1");	//zUp_Value_1
			v_dUp_Value[1]=v_cAdoConn.GetField_Double("zUp_Value_2");	//zUp_Value_2
			v_dUp_Value[2]=v_cAdoConn.GetField_Double("zUp_Value_3");	//zUp_Value_3
			v_dUp_Value[3]=v_cAdoConn.GetField_Double("zUp_Value_4");	//zUp_Value_4
		}
		//�Ͽ����ݿ�
		v_cAdoConn.ExitConnect();										//�Ͽ�����

		//��������
		v_bDown=false;													//������Ч
		v_bUp=false;													//������Ч
		for (v_iTime=0;v_iTime<4;v_iTime++)								//����������
		{
			//����
			if (v_iDown_Time[v_iTime])									//��Ч�����ȣ�����
			{
				v_bDown=true;											//������Ч

				v_dDown_Select[v_iTime].v_bFlag=true;					//ѡ��
				v_dDown_Select[v_iTime].v_dValues=v_dDown_Value[v_iTime];	//��ֵ
				
				//���ã�����������ȣ�
				if(v_iTime==0)											//��ǰ����
				{
					v_tDate=v_dPM_Date;									//��ȡ���ɼ�����
					v_dDown_Select[v_iTime].v_iCal=0;					//С��
				}
				else if(v_iTime==1)										//��������
				{
					v_tDate=v_dPM_Date-COleDateTimeSpan(7,0,0,0);		//��ȡ����������
					v_dDown_Select[v_iTime].v_iCal=1;					//����
				}
				else if(v_iTime==2)										//��������
				{
					v_tDate=v_dPM_Date-COleDateTimeSpan(1,0,0,0);		//��ȡ����������
					v_dDown_Select[v_iTime].v_iCal=1;					//����
				}
				else													//ǰһ����
				{
					v_tDate=v_dPM_Date-COleDateTimeSpan(0,1,0,0);		//��ȡ��ǰһ����
					v_dDown_Select[v_iTime].v_iCal=1;					//����
				}
				v_dDown_Select[v_iTime].v_dDate.SetDateTime(v_tDate.GetYear(),v_tDate.GetMonth(),v_tDate.GetDay(),0,0,0);	//����
				v_dDown_Select[v_iTime].v_iHour=v_tDate.GetHour();		//Сʱ
			}
			//����
			if (v_iUp_Time[v_iTime])									//��Ч�����ȣ�����
			{
				v_bUp=true;												//������Ч

				v_dUp_Select[v_iTime].v_bFlag=true;						//ѡ��
				v_dUp_Select[v_iTime].v_dValues=v_dUp_Value[v_iTime];	//��ֵ

				//���ã�����������ȣ�
				if(v_iTime==0)											//��ǰ����
				{
					v_tDate=v_dPM_Date;									//��ȡ���ɼ�����
					v_dUp_Select[v_iTime].v_iCal=0;						//С��
				}
				else if(v_iTime==1)										//��������
				{
					v_tDate=v_dPM_Date-COleDateTimeSpan(7,0,0,0);		//��ȡ����������
					v_dUp_Select[v_iTime].v_iCal=1;						//����
				}
				else if(v_iTime==2)										//��������
				{
					v_tDate=v_dPM_Date-COleDateTimeSpan(1,0,0,0);		//��ȡ����������
					v_dUp_Select[v_iTime].v_iCal=1;						//����
				}
				else													//ǰһ����
				{
					v_tDate=v_dPM_Date-COleDateTimeSpan(0,1,0,0);		//��ȡ��ǰһ����
					v_dUp_Select[v_iTime].v_iCal=1;						//����
				}
				v_dUp_Select[v_iTime].v_dDate.SetDateTime(v_tDate.GetYear(),v_tDate.GetMonth(),v_tDate.GetDay(),0,0,0);	//����
				v_dUp_Select[v_iTime].v_iHour=v_tDate.GetHour();		//Сʱ
			}
		}

		
		//����������Ϣ��ѡ������ȡ�
		m_cList.DeleteAllItems();										//ɾ���б���Ϣ
		while(m_cList.DeleteColumn(0));									//ɾ��������
		m_cSum.SetWindowText("");										//�������

		//Ĭ������Ϣ
		m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);				//
		m_cList.InsertColumn(1,"С��",LVCFMT_LEFT,200);					//��1�У�С��
		m_cList.InsertColumn(2,"С��ID",LVCFMT_LEFT,0);					//��2�У�С��ID
		m_cList.InsertColumn(3,"Ӫҵ��",LVCFMT_LEFT,60);				//��3�У�Ӫҵ��
		m_cList.InsertColumn(4,"״̬",LVCFMT_CENTER,40);				//��4�У�״̬
		m_cList.InsertColumn(5,"����",LVCFMT_CENTER,60);				//��5�У�����

		v_iNum=6;														//�����
		//������Ч
		if (v_bDown)													//��Ч�����У�����
		{
			for (v_iTime=0;v_iTime<4;v_iTime++)							//����������
			{
				if (v_dDown_Select[v_iTime].v_bFlag)					//��Ч�����ȣ�����
				{
					v_sTemp=v_dDown_Select[v_iTime].v_dDate.Format("%Y-%m-%d");	//����
					v_sTemp_1.Format("%02d",v_dDown_Select[v_iTime].v_iHour);	//Сʱ
					v_sTemp="����_"+v_sTemp+"_"+v_sTemp_1;						//������
					m_cList.InsertColumn(v_iNum,v_sTemp,LVCFMT_RIGHT,130);		//���ã�������
					m_cList.SetNumber(v_iNum);							//���ã�������ʽ
					v_iNum++;											//������
				}
			}
		}

		//������Ч
		if (v_bUp)														//��Ч�����У�����
		{
			for (v_iTime=0;v_iTime<4;v_iTime++)							//����������
			{
				if (v_dUp_Select[v_iTime].v_bFlag)						//��Ч�����ȣ�����
				{
					v_sTemp=v_dUp_Select[v_iTime].v_dDate.Format("%Y-%m-%d");	//����
					v_sTemp_1.Format("%02d",v_dUp_Select[v_iTime].v_iHour);		//Сʱ
					v_sTemp="����_"+v_sTemp+"_"+v_sTemp_1;						//������
					m_cList.InsertColumn(v_iNum,v_sTemp,LVCFMT_RIGHT,130);		//���ã�������
					m_cList.SetNumber(v_iNum);							//���ã�������ʽ
					v_iNum++;											//������
				}
			}
		}
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ʱ���������
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnBnClickedQuary3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAlarm_LL_Real_Time	v_cDlg;											//ʵ����

	if(v_cDlg.DoModal()==IDOK)											//ȷ�Ϸ��أ�����
	{
		OnCbnSelchangeCombo3();											//������ѡ��
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����Ӧ[С��]
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item_Cell>=0)									//��Ч��Ŀ��ţ�����
				m_cList_Cell.SetItemBkColor(v_iList_Item_Cell,RGB(255,255,255));	//�ָ�����ɫ
			v_iList_Item_Cell=pNMListView->iItem;						//��ã�ѡ����Ŀ���
			m_cList_Cell.SetItemBkColor(v_iList_Item_Cell,RGB(255,0,0));//���ñ���ɫ����ɫ

			//����С����������
			v_iWorking=4;												//�������񣺵��߻���
			v_pIterator_Frame->second.v_iThread_Exit=1;					//�̣߳�����
			v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);		//������ѯ���ݿ��߳�
			v_bSelect=true;												//��ǣ�true��С����false����������
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����Ӧ[������С��]
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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

			//����С����������
			v_iWorking=4;												//�������񣺵��߻���
			v_pIterator_Frame->second.v_iThread_Exit=1;					//�̣߳�����
			v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);		//������ѯ���ݿ��߳�
			v_bSelect=false;											//��ǣ�true��С����false����������
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�С����ѯ
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=3;														//��������С����ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�С��ģ�塾��ȡ��С����Ϣ������m_cList_Cell��
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnBnClickedExcel4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp;												//��ʱ����
	int			v_iItem,v_iID=0;										//��ʱ����

	LTE_PM_DATA_CASE	v_Dlg;											//ʵ����
	v_Dlg.v_iSelect=0;													//��־��С��

	if (v_Dlg.DoModal()==IDOK)											//ȷ�Ϸ��أ�����
	{
		//ѡ�������С����Ϣ��������(v_Dlg.v_mCell);
		//��������1����ȡС������2������v_mList������Cell_ID��
		m_cList_Cell.DeleteAllItems();									//ɾ����ģ����Ϣ
		My_Chart_New();													//��ͼ��λ
		v_iList_Item_Cell=-1;											//��λ����Ŀ���

		mString_String::iterator	v_pIter_Cell;						//������
		set<CString>::iterator		v_pIter;							//������

		v_sCell_All="";													//��λ��ѡ���С��
		//��������
		for(v_pIter=v_Dlg.v_mCell.begin();v_pIter!=v_Dlg.v_mCell.end();v_pIter++)	//������ģ����Ϣ�б�
		{
			//��ЧС��
			v_pIter_Cell=v_mCell.find(*v_pIter);						//���ң�С�����ڣ�����
			if (v_pIter_Cell==v_mCell.end())							//������
				continue;												//��һС��
	
			//ѡ���С��
			if (!v_sCell_All.IsEmpty())
				v_sCell_All+=",";										//�ָ���
			v_sCell_All+=v_pIter_Cell->second;							//����С��

			//�����б�
			v_sTemp.Format(" %05d",v_iID+1);
			v_iItem=m_cList_Cell.InsertItem(0xffff,v_sTemp);			//�����б�
			m_cList_Cell.SetItemText(v_iItem,1,v_pIter_Cell->first);	//��ʾ��С��
			m_cList_Cell.SetItemText(v_iItem,2,v_pIter_Cell->second);	//��ʾ��С��ID
			v_iID++;
		}																//
		v_sTemp.Format("%d",v_iID);										//
		m_cLog_Sum.SetWindowText(v_sTemp);								//��ʾ��Log����
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnBnClickedQuary2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_dTime;											//��ʱ����

	//��1����ȡ����ѯ����
	v_dPM_Date=My_LoadData_Time();										//��ȡ����������
	v_sTemp=v_dPM_Date.Format("%Y-%m-%d_%H:%S:%S");						//�ַ���[Сʱ����]
	m_cTime_Real.SetWindowText(v_sTemp);								//��ʾ������ʱ��

	//��2����ѯ��������
	v_iWorking=1;														//�������񣺲�ѯ
	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CAlarm_LL_Real::My_Thread_Inoput(LPVOID lparam)
{
	CAlarm_LL_Real *  lp_this = NULL ;
	lp_this = (CAlarm_LL_Real *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(false);								//��������Ч
	lp_this->m_cTime.EnableWindow(false);								//ʱ�����ã���Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(true);								//��������Ч
	lp_this->m_cTime.EnableWindow(true);								//ʱ�����ã���Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Per();												//������С����ѯ
		break;
	case 2:
		My_LoadData_Para();												//��ȡ�����ܲ���
		break;
	case 3:
		My_LoadData_Cell();												//��ȡ��С����Ϣ
		break;
	case 4:
		My_Chart();														//���߻���
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯС��������Ϣ
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_LoadData_Per()
{
	bool		v_bOK=false,v_bCell_Down=false,v_bCell_Up=false;		//��ʱ����
	CString		v_sTable,v_sTemp;										//��ʱ����
	int			v_iTime,v_iNum,v_iID=0;									//��ʱ����

	//��1����ʼ��
	My_Chart_New();														//��ͼ��λ
	v_iList_Item=-1;													//��ѡ����
	m_cList.DeleteAllItems();											//ɾ���б���Ϣ
	v_mAlarm_LL_Real_Down.clear();										//�������
	v_mAlarm_LL_Real_Up.clear();										//�������
	m_cSum.SetWindowText("");											//�������

	if (!v_bDown && !v_bUp)												//��Ч����ѡ�񣿣���
	{
		MessageBox("  ��ѡ����Ч������ !!! ");							//������ʾ
		return;
	}

	//��2����ȡ���������ݡ����ڸ��ֲ�ѯ���ǻ�Ĺ�ϵ�����Ե�һ�β�ѯ�����ӣ��Ժ��ѯ�����޳�����
	for (v_iTime=0;v_iTime<4;v_iTime++)									//����������
	{
		//����
		if (v_dDown_Select[v_iTime].v_bFlag)							//��Ч�����ȣ�����
		{
			v_bOK=true;													//������Ч������ѡ��

			//��3����ȡ��������
			v_sTable=v_dDown_Select[v_iTime].v_dDate.Format("%Y%m");	//����
			v_sTable="Pm_LTE_Cal_"+v_sTable+"_"+My_Get_Table_Num(v_dDown_Select[v_iTime].v_dDate.GetDay());	//�ļ�����(�����)

			//��4����飺�������Ƿ���Ч�����ݣ������Ƽ���v_mTable_Name���������������Ч����鲻���������ݡ���
			set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//���ң���
			if (v_pTable==v_mTable_Name.end())							//û�ҵ������ݱ�
			{
				v_sTable=v_dDown_Select[v_iTime].v_dDate.Format("%Y%m%d");	//������
				MessageBox("  ��ѡ����Ч�����ȣ�"+v_sTable);			//������ʾ
				break;
			}
			else
			{
				My_LoadData_PM_Data(v_iTime,1,v_sTable,v_bCell_Down,&v_mAlarm_LL_Real_Down);	//���ܻ�ȡ
				v_bCell_Down=true;										//���ã���һ�λ�ȡ��Ч
			}
		}
		//����
		if (v_dUp_Select[v_iTime].v_bFlag)								//��Ч�����ȣ�����
		{
			v_bOK=true;													//������Ч������ѡ��

			//��3����ȡ��������
			v_sTable=v_dUp_Select[v_iTime].v_dDate.Format("%Y%m");		//����
			v_sTable="Pm_LTE_Cal_"+v_sTable+"_"+My_Get_Table_Num(v_dUp_Select[v_iTime].v_dDate.GetDay());	//�ļ�����(�����)

			//��4����飺�������Ƿ���Ч�����ݣ������Ƽ���v_mTable_Name���������������Ч����鲻���������ݡ���
			set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//���ң���
			if (v_pTable==v_mTable_Name.end())							//û�ҵ������ݱ�
			{
				v_sTable=v_dUp_Select[v_iTime].v_dDate.Format("%Y%m%d");	//������
				MessageBox("  ��ѡ����Ч�����ȣ�"+v_sTable);			//������ʾ
				break;
			}
			else
			{
				My_LoadData_PM_Data(v_iTime,0,v_sTable,v_bCell_Up,&v_mAlarm_LL_Real_Up);	//���ܻ�ȡ
				v_bCell_Up=true;										//���ã���һ�λ�ȡ��Ч
			}
		}
	}
	if (!v_bOK)															//��Ч����ѡ�񣿣���
	{
		MessageBox("  ��ѡ����Ч������ !!! ");							//������ʾ
		return;
	}

	//��4�����С��������ݺϲ�������[�����������С���������һ������ʾ][���������ݺϲ�������������]
	Alarm_LL_PM_DATA		v_cPM_DATA;									//�ṹ
	map_Alarm_LL_PM_DATA::iterator	v_pPM_Cell,v_pPM_Cell_Up;			//������
	for (v_pPM_Cell_Up=v_mAlarm_LL_Real_Up.begin();v_pPM_Cell_Up!=v_mAlarm_LL_Real_Up.end();v_pPM_Cell_Up++)	//����������С��
	{
		//���У�����С�������ڣ������ӣ�
		v_pPM_Cell=v_mAlarm_LL_Real_Down.find(v_pPM_Cell_Up->first);	//��ѯ������С�������ڣ�����
		if(v_pPM_Cell==v_mAlarm_LL_Real_Down.end())						//����
		{
			v_cPM_DATA.v_sOnline=v_pPM_Cell_Up->second.v_sOnline;		//״̬
			v_cPM_DATA.v_sRegion=v_pPM_Cell_Up->second.v_sRegion;		//����
			v_cPM_DATA.v_sCell_ID=v_pPM_Cell_Up->second.v_sCell_ID;		//С��ID
			v_cPM_DATA.v_sType="����";									//����
			for(int vi=0;vi<4;vi++)										//���ݳ�ʼ��
			{
				v_cPM_DATA.v_dD_Values[vi]=-1;							//����
				v_cPM_DATA.v_dU_Values[vi]=-1;							//����
			}
			v_mAlarm_LL_Real_Down.insert(pair<CString,Alarm_LL_PM_DATA>(v_pPM_Cell_Up->first,v_cPM_DATA));		//��������
			v_pPM_Cell=v_mAlarm_LL_Real_Down.find(v_pPM_Cell_Up->first);//��ѯ������С��
		}
		else
			v_pPM_Cell->second.v_sType="�ϡ�����";						//����

		//���У�����С�����ڣ��򲹳��������ݣ�
		for (v_iTime=0;v_iTime<4;v_iTime++)								//����������
		{
			if (v_dUp_Select[v_iTime].v_bFlag)							//��Ч�����ȣ�����
				v_pPM_Cell->second.v_dU_Values[v_iTime]=v_pPM_Cell_Up->second.v_dU_Values[v_iTime];		//���ݸ���
		}
		
	}

	//��5����ʾ����������[�����ϡ����кϲ�������]
	for (v_pPM_Cell=v_mAlarm_LL_Real_Down.begin();v_pPM_Cell!=v_mAlarm_LL_Real_Down.end();v_pPM_Cell++)	//������С��
	{
		if (!m_cOnline.GetCheck())										//������Ч������
		{
			if (v_pPM_Cell->second.v_sOnline != "����")					//���̣����� [�����]
				continue;
		}
		int v_iIndex = m_cList.InsertItem(0xffffff,_T("0"));
		v_sTemp.Format("%06d",v_iIndex+1);
		m_cList.SetItemText(v_iIndex,0,v_sTemp);						//���
		m_cList.SetItemText(v_iIndex,1,v_pPM_Cell->first);				//Cell
		m_cList.SetItemText(v_iIndex,2,v_pPM_Cell->second.v_sCell_ID);	//Cell_ID
		m_cList.SetItemText(v_iIndex,3,v_pPM_Cell->second.v_sRegion);	//Ӫҵ��
		m_cList.SetItemText(v_iIndex,4,v_pPM_Cell->second.v_sOnline);	//״̬
		m_cList.SetItemText(v_iIndex,5,v_pPM_Cell->second.v_sType);		//����
		v_iNum=6;														//�����
		if (v_bDown)													//��Ч�����У�����
		{
			for (v_iTime=0;v_iTime<4;v_iTime++)							//����������
			{
				if (v_dDown_Select[v_iTime].v_bFlag)					//��Ч�����ȣ�����
				{
					v_sTemp.Format("%8.2f",v_pPM_Cell->second.v_dD_Values[v_iTime]);	//�ַ���
					v_sTemp.Trim();
					if (v_sTemp=="-1.00") 
						v_sTemp="";
					m_cList.SetItemText(v_iIndex,v_iNum,v_sTemp);		//������ֵ
					v_iNum++;											//������
				}
			}
		}
		//������Ч
		if (v_bUp)														//��Ч�����У�����
		{
			for (v_iTime=0;v_iTime<4;v_iTime++)							//����������
			{
				if (v_dUp_Select[v_iTime].v_bFlag)						//��Ч�����ȣ�����
				{
					v_sTemp.Format("%8.2f",v_pPM_Cell->second.v_dU_Values[v_iTime]);	//�ַ���
					v_sTemp.Trim();
					if (v_sTemp=="-1.00") 
						v_sTemp="";
					m_cList.SetItemText(v_iIndex,v_iNum,v_sTemp);		//������ֵ
					v_iNum++;											//������
				}
			}
		}
	
		v_iID++;														//������
		if (v_iID%100==0)
		{
			v_sTemp.Format("%d",v_iID);									//
			m_cSum.SetWindowText(v_sTemp);								//��ʾ������
		}
	}
	v_sTemp.Format("%d",v_iID);											//
	m_cSum.SetWindowText(v_sTemp);										//��ʾ��Log����
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ��������
//	��    ����v_iTime��������ţ�	v_iType��(1�����У�0�����У�)	v_sTable�������ƣ�	v_bCell����Ч/��Ч��	v_mAlarm_LL_Real������ָ�룻
//	��    �����������ݷ�������
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_LoadData_PM_Data(int v_iTime,int v_iType,CString v_sTable,bool v_bCell,map_Alarm_LL_PM_DATA* v_mAlarm_LL_Real)
{
	CString			v_sSql,v_sPM_Cell,v_sRegion,v_sOnline;				//��ʱ����
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_tDate;											//����
	double			v_dValue;											//������ֵ
	int				v_iCell_ID;											//��ʱ����

	_variant_t		Var;

	Alarm_LL_PM_DATA		v_cPM_DATA;									//�ṹ

	map_Cell_ID::iterator	v_pIter_Cell;								//������
	map_Alarm_LL_PM_DATA::iterator	v_pPM_Cell;							//������

	CString		v_sField,v_sValue,v_sTime,v_sCal;						//��ʱ����


	//��1����ʼ��
	if(v_iType)															//����
	{
		v_sField = "��������(M)";										//�ֶ�����
		v_sValue.Format("%12.4f",v_dDown_Select[v_iTime].v_dValues);	//��ֵ(�����㹫ʽ)
		v_sTime=v_dDown_Select[v_iTime].v_dDate.Format("%Y%m%d");		//����
		v_sTemp.Format("%02d:00:00",v_dDown_Select[v_iTime].v_iHour);	//Сʱ
		v_sTime+=" "+v_sTemp;											//����ʱ��
		v_sCal=(v_dDown_Select[v_iTime].v_iCal==0)?"<":((v_dDown_Select[v_iTime].v_iCal==1)?">":"=");	//�ȽϷ�
	}
	else
	{
		v_sField = "��������(M)";										//�ֶ�����
		v_sValue.Format("%12.4f",v_dUp_Select[v_iTime].v_dValues);		//��ֵ(�����㹫ʽ)
		v_sTime=v_dUp_Select[v_iTime].v_dDate.Format("%Y%m%d");			//����
		v_sTemp.Format("%02d:00:00",v_dUp_Select[v_iTime].v_iHour);		//Сʱ
		v_sTime+=" "+v_sTemp;											//����ʱ��
		v_sCal=(v_dUp_Select[v_iTime].v_iCal==0)?"<":((v_dUp_Select[v_iTime].v_iCal==1)?">":"=");	//�ȽϷ�
	}
	v_sValue.Trim();													//ȥ���ո�

	//��2������� ���ӵڶ��β�ѯ��ɾ����ǰ�β�ƥ���С������
	if (v_bCell)														//����һ�β�ѯ
	{
		for (v_pPM_Cell=v_mAlarm_LL_Real->begin();v_pPM_Cell!=v_mAlarm_LL_Real->end();v_pPM_Cell++)	//������С��
			v_pPM_Cell->second.v_bFlag=false;							//��ʼ��
	}

	//��3���������ݻ�ȡ
	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(10*60);							//��ʱ����(10����)

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zTime,["+v_sField+"] from "+v_sTable;		//SQL
			
		if (v_sCell_All.IsEmpty())										//����С��������
			v_sSql+=" where ";											//SQL
		else															//ѡ���С��
			v_sSql+=" where zCell in("+v_sCell_All+") and ";			//SQL
		
		v_sSql+="zTime='"+v_sTime+"' and ["+v_sField+"]"+v_sCal+v_sValue;	//SQL

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_iCell_ID=Var.intVal;									//��ȡ��Cell_ID
				v_pIter_Cell=v_mCell_ID.find(v_iCell_ID);				//���ң�С��ID���ڣ�����
				if (v_pIter_Cell==v_mCell_ID.end())						//������
				{
					v_pRecordset->MoveNext();							//��һС������
					continue;											//����
				}
				v_sPM_Cell=v_pIter_Cell->second.v_sCell;				//С��
				v_sOnline=v_pIter_Cell->second.v_sOnline;				//״̬
				v_sRegion=v_pIter_Cell->second.v_sRegion;				//Ӫҵ��
			}
			else
				v_sPM_Cell="";

			Var=v_pRecordset->GetCollect((_variant_t)v_sField);			//��ȡ��������ֵ
			if (Var.vt != VT_NULL)
				v_dValue=Var.dblVal;
			else
				v_dValue=0;

			if (!v_bCell)												//��һ�β�ѯ������
			{															//����
				v_pPM_Cell=v_mAlarm_LL_Real->find(v_sPM_Cell);			//���ң�С�� ������
				if (v_pPM_Cell==v_mAlarm_LL_Real->end())				//������
				{
					v_cPM_DATA.v_sOnline=v_sOnline;						//״̬
					v_cPM_DATA.v_sRegion=v_sRegion;						//����
					v_cPM_DATA.v_sType="����";							//����
					for(int vi=0;vi<4;vi++)								//���ݳ�ʼ��
					{
						v_cPM_DATA.v_dD_Values[vi]=-1;					//����
						v_cPM_DATA.v_dU_Values[vi]=-1;					//����
					}
					v_sTemp.Format("%d",v_iCell_ID);					//�ַ���
					v_cPM_DATA.v_sCell_ID=v_sTemp;						//С��ID
					if (v_iType)										//����
						v_cPM_DATA.v_dD_Values[v_iTime]=v_dValue;		//����
					else												//����
						v_cPM_DATA.v_dU_Values[v_iTime]=v_dValue;		//����

					v_mAlarm_LL_Real->insert(pair<CString,Alarm_LL_PM_DATA>(v_sPM_Cell,v_cPM_DATA));	//��������
				}
			}	
			else
			{
				v_pPM_Cell=v_mAlarm_LL_Real->find(v_sPM_Cell);			//���ң�С�� ������
				if (v_pPM_Cell!=v_mAlarm_LL_Real->end())				//����
				{
					v_pPM_Cell->second.v_bFlag=true;					//���ñ�־����ѯƥ��
					if (v_iType)										//����
						v_pPM_Cell->second.v_dD_Values[v_iTime]=v_dValue;		//����
					else												//����
						v_pPM_Cell->second.v_dU_Values[v_iTime]=v_dValue;		//����
				}
			}

			v_pRecordset->MoveNext();									//
		}
		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	//��4��ɾ����ƥ���С�� ���ӵڶ��β�ѯ��ɾ����ǰ�β�ƥ���С������
	if (v_bCell)														//����һ�β�ѯ
	{
		v_pPM_Cell=v_mAlarm_LL_Real->begin();							//ͷָ��
		while(v_pPM_Cell!=v_mAlarm_LL_Real->end())						//������С��
		{
			if (!v_pPM_Cell->second.v_bFlag)							//ûƥ�䣿����
				v_pPM_Cell=v_mAlarm_LL_Real->erase(v_pPM_Cell);			//ɾ��С����������һָ��
			else
				v_pPM_Cell++;											//ָ��++
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ�����
//	��	  ����1��1-5��	2��6-10��	3��11-15��	4��16-20��	5��21-25��	6��26--
//------------------------------------------------------------------------------------------------------			
CString CAlarm_LL_Real::My_Get_Table_Num(int v_iDay)
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
void CAlarm_LL_Real::My_LoadData_Para()
{
	int				v_iID=0,v_iCell_ID;									//��ʱ����
	CString			v_sTemp,v_sSql,v_sLog_Key,v_sTable;					//��ʱ����
	CString			v_sType,v_sType_1,v_sPm,v_sPm_Name,v_sTable_Key;	//��ʱ����
	CString			v_sCell,v_sCell_ID,v_sCell_Name,v_sRegion,v_sOnline;//��ʱ����
	Cell_ID_Struct	v_cCell_ID;											//�ṹ

	LTE_PM_Struct	v_cLTE_PM;											//�ṹ
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	LTE_PM_Type_Struct		v_cLTE_PM_Type;								//�ṹ

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);


		//��1����ȡ���������ݱ����ơ����ڷ�����Ч�����ơ�
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_LTE_%') order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//������

			v_mTable_Name.insert(v_sTable);								//���Ӽ���

			v_pRecordset->MoveNext();									//ָ��++
		}	

		//��2����ȡ��С����Ϣ������С��ID����
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zCell_ID,zCell_Name,LTE_Cell.zOnline,LTE_Cell.zRegion \
				from LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB";		//��ѯ���ݿ�
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sCell=v_cAdoConn.GetField_String("zCell");				//zCell
			v_iCell_ID=v_cAdoConn.GetField_Int("zCell_ID");				//zCell_ID
			v_sCell_ID.Format("%d",v_iCell_ID);							//�ַ���
			v_sCell_Name=v_cAdoConn.GetField_String("zCell_Name");		//zCell_Name
			v_sOnline=v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_sRegion=v_cAdoConn.GetField_String("zRegion");			//zRegion

			v_mCell.insert(pair<CString,CString>(v_sCell,v_sCell_ID));	//��������
			v_cCell_ID.v_sCell=v_sCell;									//С��
			v_cCell_ID.v_sCell_Name=v_sCell_Name;						//С��������
			v_cCell_ID.v_sOnline=v_sOnline;								//״̬
			v_cCell_ID.v_sRegion=v_sRegion;								//Ӫҵ��
			v_mCell_ID.insert(pair<int,Cell_ID_Struct>(v_iCell_ID,v_cCell_ID));	//��������

			v_pRecordset->MoveNext();									//ָ��++
		}	

		//��3���������ͣ���������
		v_sType="��������";												//��������								
		v_cLTE_PM_Type.v_sTable_Key=v_sType;							//������-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//��������

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
				if (v_sPm.Find("����")>=0)								//����������
					v_cLTE_PM.v_bOK=true;								//���ܣ�ѡ��(��Ҫ����)
				else
					v_cLTE_PM.v_bOK=false;								//���ܣ�δѡ��(����Ҫ����)

				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//���ӣ����ܲ���
			}

			v_pRecordset->MoveNext();									//
		}

		v_cAdoConn.ExitConnect();										//�Ͽ�����

		//����ѡ��
		OnCbnSelchangeCombo3();											//����ѡ��
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��Cell��Ϣ
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_LoadData_Cell()
{
	CString		v_sSql;													//��ʱ����
	int			v_iItem,v_iID=0,v_iCell_ID;								//��ʱ����
	CString		v_sTemp,v_sCell,v_sCell_ID;								//��ʱ����
	CString		v_sLog_Key;												//��־��ѯ��KEY

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList_Cell.DeleteAllItems();									//����б�����
		v_iList_Item_Cell=-1;											//Ĭ�ϣ���ѡ�е��б�Item
		My_Chart_New();													//��ͼ��λ

		//��ѯ����׼��
		m_cLog_Key.GetWindowText(v_sLog_Key);							//��ã���ѯKEY
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

		v_sCell_All="";													//��λ��ѡ���С��

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sCell=v_cAdoConn.GetField_String("zCell");				//zCell
			v_iCell_ID=v_cAdoConn.GetField_Int("zCell_ID");				//zCell_ID

			//ѡ���С��
			if (!v_sCell_All.IsEmpty())
				v_sCell_All+=",";										//�ָ���
			v_sCell_All+=v_sCell_ID;									//����С��

			//�б�
			v_sTemp.Format(" %05d",v_iID+1);
			v_iItem=m_cList_Cell.InsertItem(0xffff,v_sTemp);			//�����б�
			m_cList_Cell.SetItemText(v_iItem,1,(_bstr_t)v_sCell);		//
			v_sCell_ID.Format("%d",v_iCell_ID);							//�ַ���
			m_cList_Cell.SetItemText(v_iItem,2,(_bstr_t)v_sCell_ID);	//Cell_ID

			v_pRecordset->MoveNext();									//
			v_iID++;													//

			if (v_iID%100==0)
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cLog_Sum.SetWindowText(v_sTemp);						//��ʾ��Log����
			}
		}

		v_sTemp.Format("%d",v_iID);										//
		m_cLog_Sum.SetWindowText(v_sTemp);								//��ʾ��Log����

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ��������
//------------------------------------------------------------------------------------------------------			
COleDateTime CAlarm_LL_Real::My_LoadData_Time()
{
	CString			v_sTemp,v_sSql;										//��ʱ����
	COleDateTime	v_sTime=COleDateTime::GetCurrentTime();				//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT top 1 * FROM CON_System";						//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		if(!v_pRecordset->adoEOF)
		{
			v_sTime = v_cAdoConn.GetField_DataTime("zPm_LTE_Time_Data");	//zPm_LTE_Time_Data [��������]
		}

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	return	v_sTime;													//����
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Chart
////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------------
//	�������ܣ�Draw track cursor when mouse is moving over plotarea
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnMouseMovePlotArea()
{
	// Get the focus to ensure being able to receive mouse wheel events
	m_cChartView.SetFocus();

//	m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),m_cLable.GetCheck()); 
	m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),true); 
	m_cChartView.updateDisplay();
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�����ֻ�����Ϣ [��;���Ŵ���С������ƽ��]
//------------------------------------------------------------------------------------------------------
BOOL CAlarm_LL_Real::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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
void CAlarm_LL_Real::OnViewPortChanged()
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
//		m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),m_cLable.GetCheck());
		m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),true);
		m_cChartView.updateDisplay();
	}
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ����ݻ�ͼ����  [����Ļ��Chart����]
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnChart_Para()
{
	CString		v_sTemp;												//��ʱ����

	m_cChartView.v_cUI_Data.v_dDate_Start=COleDateTime::GetCurrentTime()-COleDateTimeSpan(v_iChart_Day_Sum-1,0,0,0);	//��ã���ѯ��ʼ����
 	m_cChartView.v_cUI_Data.v_iChart_Day_Sum=v_iChart_Day_Sum;			//��ѯ����
 	m_cChartView.v_cUI_Data.v_iMuti_Line=false;							//������Ч
 	m_cChartView.v_cUI_Data.v_iKPI_S=false;								//��̬��Ч
	m_cChartView.v_cUI_Data.v_iDay=m_cDay.GetCheck();					//������Ч
	m_cChartView.v_cUI_Data.v_iPoint=m_cPoint.GetCheck();				//���ݵ���Ч

	m_cChartView.v_cUI_Data.v_sKPI_Name = "С������������";				//��Ŀ����

	m_cChartView.v_cUI_Data.v_iY_scale=0;								//y��̶���Ч

	m_cChartView.v_cUI_Data.v_iTime=1;									//������Ϣ��ʱ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������Ļ��ʾ��Ϣ
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_Show_Clear()									//�����Ļ��ʾ��Ϣ
{
	v_iList_Item_Cell=-1;												//ѡ�е�Item��Ч
	My_Chart_New();														//��ͼ��λ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ��
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iChart_Day_Sum=m_cDay_Sum.GetCurSel()+1;							//��ã���ѯ�������

	My_Chart_New();														//��ͼ��λ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ͼ��λ
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_Chart_New()
{
	//��ͼ��ʼ��
	OnChart_Para();														//���ݣ���ͼ����
	m_cChartView.initChartViewer(&m_cChartView);						//��ʼ��Chart
	//��ͼ��Chart
	m_cChartView.updateViewPort(true, false);							//��ͼ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ͼ��λȻ���ҵ�ǰ�ڵ�
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_Chart()
{
	//��ͼ��λ
	My_Chart_New();														//��ͼ��λ

	//��ͼ������Tree���ҵ�ѡ�еĽڵ�	
	if (v_bSelect)														//��ǣ�true��С����false����������
		My_List_Chart(v_iList_Item_Cell);								//��ͼ��ѡ�е�Item
	else
		My_List_Chart(v_iList_Item);									//��ͼ��ѡ�е�Item
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ���ͼ��ѡ�е�Tree Node
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::My_List_Chart(int v_iList_Item)
{
	My_List_Chart_GetNode(v_iList_Item);								//��ȡ���ڵ���Ϣ��������public�����С�
	My_Chart_IP();														//���ƣ�������Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ�ڵ���Ϣ 
//	��    ����v_iList_Item��ѡ�е�Item
//			 �����ڣ���ͼ����ʾ�ڵ���Ϣ��������v_sCell�����С�
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_List_Chart_GetNode(int v_iList_Item)
{
	if (v_bSelect)														//��ǣ�true��С����false����������
	{
		v_sCell=m_cList_Cell.GetItemText(v_iList_Item,1);				//Cell����
		v_sCell_ID=m_cList_Cell.GetItemText(v_iList_Item,2);			//Cell_ID
	}
	else
	{
		v_sCell=m_cList.GetItemText(v_iList_Item,1);					//Cell����
		v_sCell_ID=m_cList.GetItemText(v_iList_Item,2);					//Cell_ID
	}
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�Chart��һ����
//	��    ����ѡ�е� IP [v_sItem_IP]
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::My_Chart_IP()
{
	CString					v_sTemp;									//��ʱ����
	bool					v_bOK=false;								//��ʱ����
	mLTE_PM_Type::iterator	v_pSelect;									//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	COleDateTime	v_dDate_Start=COleDateTime::GetCurrentTime()-COleDateTimeSpan(v_iChart_Day_Sum-1,0,0,0);	//��ѯ��ʼ���ڣ���ǰʱ��-��ѯ���� [Ĭ��һ��]
	v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//����ʱ�䣺00:00:00

	//�������������͡��������ܡ��������ܡ�
	for (v_pSelect=v_mLTE_PM_Type.begin();v_pSelect!=v_mLTE_PM_Type.end();v_pSelect++)
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//ǿ���˳�������
			break;														//�˳�

		if (v_pSelect->first=="��������")								//��ѡ���һ�£�����
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
						v_pPM_Name->second.v_sTable_Key,				//�����ļ�����key
						v_sCell,										//С������
						v_sCell_ID,										//С��ID
						v_pPM_Name->second.v_sPM_Name,					//��������
						v_pPM_Name->first,								//�����ֶ�
						v_dDate_Start,									//��ʼ����
						v_iChart_Day_Sum);								//��ѯ����

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
//	�������ܣ�	��ȡ���ݱ��е���������
//	������		v_sCell_ID��Cell_ID��	v_dDate_Start����ѯ����ʼ���ڣ�	v_iDay_Sum����ѯ����
//	��ȡ���̣�	�Ӷ�Ӧ���л�ȡ���ݣ�����v_mIP_Instance�����У�[��Ÿ������ȵ�IP����]
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::My_Load_Performance(CString v_sType,CString v_sFile_Key,CString v_sCell,CString v_sCell_ID,CString v_sPerformance,CString v_sField,COleDateTime v_dDate_Start,int v_iDay_Sum)
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

			//��������
			v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//��ʼʱ��
			//Hour��5��1����(ÿ��6����)
			while (true)
			{
				v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());	//��ȡ�����
				v_dTime=v_dDate_Start;									//1�����ѯ��������
				//�����
				v_dDate_Start += COleDateTimeSpan(1,0,0,0);				//����һ��
				v_iDay++;												//
				if (v_iDay>=v_iDay_Sum)
					break;
				v_sTable_Num_New=My_Get_Table_Num(v_dDate_Start.GetDay());	//��ȡ�����
				if (v_sTable_Num!=v_sTable_Num_New)
					break;
			}
			v_sTime_End=v_dTime.Format("%Y-%m-%d")+" 23:59:59";			//����ʱ��
			v_sNet="Pm_LTE_"+v_sFile_Key;								//�ļ�����
			v_sDate=v_dTime.Format("%Y%m");								//����
			v_sNet+="_"+v_sDate+"_"+v_sTable_Num;						//���������ӣ�����_���

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
//	END
//------------------------------------------------------------------------------------------------------
