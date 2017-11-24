// All_PM_Draw.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "All_PM_Draw.h"
#include <math.h>
#include <vector>
#include <sstream>
#include "LTE_PM_Draw_Select.h"


// CAll_PM_Draw

IMPLEMENT_DYNCREATE(CAll_PM_Draw, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CAll_PM_Draw::CAll_PM_Draw()
	: CFormView(CAll_PM_Draw::IDD)
{
	v_bShow=false;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CAll_PM_Draw::~CAll_PM_Draw()
{
	v_mTree_All.clear();												//�������
	v_mLTE_PM_Type.clear();												//�������
	v_mTable_Name.clear();												//�������
	delete m_cChartView.getChart();										//�ͷ���Դ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE1, m_cTree);
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
	DDX_Control(pDX, IDC_COMBO3, m_cOut_In);
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_INSTANCE_DETAIL, m_cPM_Select);
	DDX_Control(pDX, IDC_REFRESH, m_cRefresh);
	DDX_Control(pDX, IDC_DELETE, m_cClear);
	DDX_Control(pDX, IDC_BUTTON1, m_cExcel);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAll_PM_Draw, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CAll_PM_Draw::OnTvnSelchangedTree1)
	//Chart
	ON_WM_MOUSEWHEEL()
	ON_CONTROL(CVN_ViewPortChanged, IDC_CHARTVIEW, OnViewPortChanged)
	ON_CONTROL(CVN_MouseMovePlotArea, IDC_CHARTVIEW, OnMouseMovePlotArea)
	ON_MESSAGE(123,&CAll_PM_Draw::Message_ChartView)					//�����Ϣӳ�� 

	ON_NOTIFY(DTN_CLOSEUP, IDC_BEG_DATE, &CAll_PM_Draw::OnDtnCloseupBegDate)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAll_PM_Draw::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_REFRESH, &CAll_PM_Draw::OnBnClickedRefresh)
	ON_BN_CLICKED(IDC_DELETE, &CAll_PM_Draw::OnBnClickedDelete)
	//MutiTreCtrl����Ϣ [�����State]
	ON_BN_CLICKED(IDC_CK_MUTI, &CAll_PM_Draw::OnBnClickedCkMuti)
	ON_BN_CLICKED(IDC_QUARY, &CAll_PM_Draw::OnBnClickedQuary)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CAll_PM_Draw::OnNMClickList1)
	ON_BN_CLICKED(IDC_INSTANCE_DETAIL, &CAll_PM_Draw::OnBnClickedInstanceDetail)
	ON_BN_CLICKED(IDC_BUTTON1, &CAll_PM_Draw::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAll_PM_Draw ���

#ifdef _DEBUG
void CAll_PM_Draw::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAll_PM_Draw::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAll_PM_Draw ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CString v_sTemp;													//��ʱ����
	//��������
	v_sFrame_Name="��ַ���ܲ�ѯ";										//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б� [վַ]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,40);					//
	m_cList.InsertColumn(1,"����/��Դ������",LVCFMT_LEFT,240);			//
	m_cList.InsertColumn(2,"����",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(3,"2G-3G-4G",LVCFMT_CENTER,60);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//����ʼ����m_cTree
	m_cTree.ModifyStyle(0,TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES);
	v_cImage_Tree.Create(IDB_TREE_STATION,16,1,RGB(255,255,255));		//����ͼ���б�����
	m_cTree.SetImageList(&v_cImage_Tree,TVSIL_NORMAL);					//����ʼ��
	v_cImage_State.Create(IDB_TREE_CHECK,13, 1, RGB(255,255,255));
	m_cTree.SetImageList(&v_cImage_State,TVSIL_STATE);
	m_cTree.SetBkColor(RGB(200,200,255));								//�������οؼ��ı���ɫ
	m_cTree.SetTextColor(RGB(127,0,0));									//�����ı�����ɫ

	//���⡢���ڹ�ַѡ��
	m_cOut_In.AddString("����");										//
	m_cOut_In.AddString("����");										//
	m_cOut_In.SetCurSel(0);												//Ĭ��

	//�ִ� [��������ߵ����¹�ϵ��ˮƽ�ָ��������������ҹ�ϵ�Ĵ�ֱ�ָ�������2*1]
	CRect	v_cRect;													//����
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//��ã�ָ��
	pWnd->GetWindowRect(&v_cRect);										//��ã��ߴ�
	ScreenToClient(&v_cRect);											//����任
	pWnd->DestroyWindow();												//���٣�ָ��

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Left.Create(											//ˮƽ��
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList,				// Top pane
		&m_cTree,				// Bottom nested pane
		IDC_All_PM_Draw_HORIZ,	// this ID is used for saving/restoring splitter position and therefore it must be unique !
		v_cRect					// dimensions of the splitter pane
		);
	m_SplitterPane_Left.m_bHorizSplitter_Parent=true;					//ˮƽ�ָ���Ϊ����
	m_SplitterPane_Main.Create(											//��ֱ��
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_SplitterPane_Left,	// Left pane
		&m_cDraw_AAA,			// Right pane
		IDC_All_PM_Draw_Main,		// this ID is used for saving/restoring splitter position and it must be unique !
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

	//������ʼ��
    v_hTree_Current=NULL;                                               //ѡ�е�Tree�ڵ�

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

	//������ʼ��
	m_cType.AddString("15m����");
	m_cType.AddString("1H����");
	m_cType.SetCurSel(0);												//Ĭ�ϣ�15m����

	//��Ϣ��ʾ
	My_Show_Clear();													//ɾ��Tree�ؼ���Ϣ
												
	// TODO: �ڴ����ר�ô����/����û���
	My_LoadData_Para();													//��ȡ����������

//	OnBnClickedInstanceDetail();										//����ѡ��
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CAll_PM_Draw::PreTranslateMessage(MSG* pMsg)
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
void CAll_PM_Draw::OnDestroy()
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
void CAll_PM_Draw::OnSize(UINT nType, int cx, int cy)
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
LRESULT CAll_PM_Draw::Message_ChartView(WPARAM iParam1,LPARAM iParam2)
{
	//��ͼ��Chart
	m_cChartView.updateViewPort(true, false);							//ͼ�λ���

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����б� [վַ]
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item_Room>=0)									//��Ч��Ŀ��ţ�����
				m_cList.SetItemBkColor(v_iList_Item_Room,RGB(255,255,255));		//�ָ�����ɫ
			v_iList_Item_Room=pNMListView->iItem;						//��ã�ѡ����Ŀ���
			m_cList.SetItemBkColor(v_iList_Item_Room,RGB(255,0,0));		//���ñ���ɫ����ɫ

			My_LoadData_Tree(m_cList.GetItemText(v_iList_Item_Room,1));	//��ȡ������Ϣ ������Tree��
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�վַ��ѯ
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ��
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::OnBnClickedInstanceDetail()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CLTE_PM_Draw_Select	v_cDlg;											//ʵ����

	v_cDlg.v_mLTE_PM_Type=&v_mLTE_PM_Type;								//ָ�룺���ܲ���

	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܵ���
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	My_SendToFile();													//������CSV�ļ�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CAll_PM_Draw::My_Thread_Inoput(LPVOID lparam)
{
	CAll_PM_Draw *  lp_this = NULL ;
	lp_this = (CAll_PM_Draw *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(false);								//��������Ч
	lp_this->m_cPM_Select.EnableWindow(false);							//��������Ч
	lp_this->m_cRefresh.EnableWindow(false);							//��������Ч
	lp_this->m_cClear.EnableWindow(false);								//��������Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
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
void CAll_PM_Draw::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_Query_Station();												//վַ��ѯ
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
//	�������ܣ���ѯ������Ϣ
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::My_Query_Station()
{
	int				v_iID=0;											//��ʱ����
	CString			v_sSql,v_sTemp;										//��ʱ����
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//��ѯ����׼��
	m_cKey.GetWindowText(v_sLog_Key);									//��ã���ѯKEY
	v_sLog_Key.Replace("[","[[]");										//���ƴ���
	v_sLog_Key.Replace("'","");											//���ƴ���
	v_sLog_Key.Trim();													//ȥ���ո�

	v_sSql += "SELECT * FROM CON_Station"; 

	if (!v_sLog_Key.IsEmpty())											//��ѯKEY����
	{
		v_sSql+=" where zStation like '%"+v_sLog_Key+"%'";
	}
	v_sSql+=" order by zStation";

	My_LoadData_Station_Data(v_sSql);									//��ȡ��վַ��Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��վַ��Ϣ
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem,v_iID=0;									//��ʱ����
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����
	int				v_iGSM_BTS,v_iTD_NodeB,v_iLTE_NodeB,v_iSum;			//��ʱ����

	_variant_t	Var;
	CString			v_sStation,v_sRegion,v_sLongitude;					//��ʱ����
	CString			v_sLatitude,v_sOnline;								//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		//����λ
		m_cTree.DeleteAllItems();										//�����
		//�б�λ
		m_cList.DeleteAllItems();										//���Items
		v_iList_Item_Room=-1;											//��Чѡ��

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			if (m_cOut_In.GetCurSel()==0)								
			{															//����
				v_iGSM_BTS = v_cAdoConn.GetField_Int("zGSM_BTS_OUT");	//zGSM_BTS_OUT
				v_iTD_NodeB = v_cAdoConn.GetField_Int("zTD_NodeB_OUT");	//zTD_NodeB_OUT
				v_iLTE_NodeB=v_cAdoConn.GetField_Int("zLTE_NodeB_OUT");	//zLTE_NodeB_OUT
			}
			else
			{															//����
				v_iGSM_BTS = v_cAdoConn.GetField_Int("zGSM_BTS");		//zGSM_BTS
				v_iTD_NodeB = v_cAdoConn.GetField_Int("zTD_NodeB");		//zTD_NodeB
				v_iLTE_NodeB = v_cAdoConn.GetField_Int("zLTE_NodeB");	//zLTE_NodeB
			}
			//��ʾ
			v_iSum=v_iGSM_BTS+v_iTD_NodeB+v_iLTE_NodeB;					//��վ�ܼ�
			if (v_iSum != 0)
			{
				v_sTemp.Format(" %04d",v_iID+1);
				v_iItem=m_cList.InsertItem(0xffff,v_sTemp);				//�����б�
				m_cList.SetItemText(v_iItem,1,v_sStation);				//����/��Դ������
				m_cList.SetItemText(v_iItem,2,v_sRegion);				//����
				v_sTemp.Format("%d-%d-%d",v_iGSM_BTS,v_iTD_NodeB,v_iLTE_NodeB);
				m_cList.SetItemText(v_iItem,3,v_sTemp);					//GSM_BTS

				v_iID++;												//�������澯����
			}
			v_pRecordset->MoveNext();									//

			if (v_iID%100==0)											//��ʾ��100������
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum.SetWindowText(v_sTemp);							//��ʾ��վַ����
			}
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cSum.SetWindowText(v_sTemp);									//��ʾ��վַ����

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
//	�������ܣ�װ��վַ��Ϣ��Tree��
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_LoadData_Tree(CString v_sRoom)
{
	CString		v_sSql,v_sTemp,v_sRNC;									//��ʱ����
	CString		v_sBTS,v_sBTS_Name,v_sNodeB_Name,v_sSation,v_sCell,v_sSation_Cell;	//��ʱ����

	mStation_OutPut_BTS				v_mNodeb;							//Nodeb/BTS ��Tree��¼��value=1��ΪBTS;value=2��ΪNodeb;��
	mStation_OutPut_BTS::iterator	v_pNodeb;							//������
	Station_OutPut_BTS_Struct		v_cNodeb;							//�ṹ
	mString_String::iterator		v_pCell;							//������
	Node_Cell_Struct				v_cNode_Cell;						//�ṹ

	//����λ
	m_cTree.DeleteAllItems();											//�����
	v_mTree_All.clear();												//�������

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//
		m_cOut_In.GetWindowText(v_sRNC);								//ѡ���������⡢���ڣ�
		//С����ַ��ѯ ����ѯС��վַΪv_sRoom��Ӧ�Ļ�վ��Ϣ����������v_mNodeb��
		//��GSMС����
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select GSM_Cell.zCell,GSM_Cell.zBTS as zBTS_BTS,GSM_Cell.zStation as zStation_Cell, \
				  GSM_BTS.zBTS_NAME,GSM_BTS.zStation as zStation_BTS \
				  FROM GSM_Cell left join GSM_BTS on GSM_Cell.zBTS=GSM_BTS.zBTS \
				  where GSM_BTS.zType='"+v_sRNC+"' and GSM_Cell.zStation='"+v_sRoom+"' order by GSM_BTS.zStation,GSM_Cell.zCell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell

			v_pNodeb=v_mNodeb.find(v_sBTS);								//���ң�BTS
			if (v_pNodeb==v_mNodeb.end())
			{
				//���ӣ�BTS
				v_cNodeb.v_sName=v_sBTS_Name;							//������
				v_cNodeb.v_sNetwork="2";								//��������
				v_cNodeb.v_sStation=v_sSation;							//վַ
				v_mNodeb.insert(pair<CString,Station_OutPut_BTS_Struct>(v_sBTS,v_cNodeb));	//��������
			}
			v_pNodeb=v_mNodeb.find(v_sBTS);								//���ң�BTS
			//���ӣ�Cell
			v_pNodeb->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sSation_Cell));	//��������

			v_pRecordset->MoveNext();									//��һ��
		}

		//��TD-LTE��
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select LTE_Cell.zCell,LTE_Cell.zNODEB as zBTS_BTS,LTE_Cell.zStation as zStation_Cell, \
				  LTE_NODEB.zStation as zStation_BTS,LTE_NODEB.zRNC \
				  FROM LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB \
				  where LTE_NODEB.zType='"+v_sRNC+"' and LTE_Cell.zStation='"+v_sRoom+"' order by LTE_NODEB.zStation,LTE_Cell.zCell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sRNC = v_cAdoConn.GetField_String("zRNC");				//zRNC
			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zBTS
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell

			v_pNodeb=v_mNodeb.find(v_sBTS);								//���ң�BTS
			if (v_pNodeb==v_mNodeb.end())
			{
				//���ӣ�E_NodeB
				v_cNodeb.v_sName=v_sBTS;								//������
				if (v_sRNC.Find("dalian_")>=0)
					v_cNodeb.v_sNetwork="4";							//�������ͣ�4G
				else
					v_cNodeb.v_sNetwork="3";							//�������ͣ�3G
				v_cNodeb.v_sStation=v_sSation;							//վַ
				v_mNodeb.insert(pair<CString,Station_OutPut_BTS_Struct>(v_sBTS,v_cNodeb));	//��������
			}
			v_pNodeb=v_mNodeb.find(v_sBTS);								//���ң�BTS
			//���ӣ�Cell
			v_pNodeb->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sSation_Cell));	//��������

			v_pRecordset->MoveNext();									//
		}

		HTREEITEM	v_hRoot,v_hNode_BTS,v_hNode_Cell;					//��Ŀ¼������ڵ�����
		//���������ӵĻ�վ������Tree��Ϣ
		v_hRoot=m_cTree.InsertItem(v_sRoom,6,7,TVI_ROOT);			//�������Ӹ��ڵ� [����/��Դ��]		
		for (v_pNodeb=v_mNodeb.begin();v_pNodeb!=v_mNodeb.end();v_pNodeb++)		//����������
		{
			if(v_pNodeb->second.v_sNetwork=="2")						//BTS
			{
				//GSM��BSC��ȡ
				v_hNode_BTS=m_cTree.InsertItem(v_pNodeb->first+"_"+v_pNodeb->second.v_sName
					+"     ��"+v_pNodeb->second.v_sStation+"��",4,5,v_hRoot);	//���������ӽڵ� [BTS]
				for (v_pCell=v_pNodeb->second.v_mCell.begin();v_pCell!=v_pNodeb->second.v_mCell.end();v_pCell++)	//����������
				{
					v_hNode_Cell=m_cTree.InsertItem(v_pCell->first+"     ��"+v_pCell->second+"��",0,1,v_hNode_BTS);	//���������ӽڵ� [Cell]
					v_cNode_Cell.v_sCell=v_pCell->first;				//Cell
					v_cNode_Cell.v_sNetwork=v_pNodeb->second.v_sNetwork;//Network
					v_mTree_All.insert(pair<HTREEITEM,Node_Cell_Struct>(v_hNode_Cell,v_cNode_Cell));				//��������
				}
			}
			else														//Nodeb
			{
				//TD_LTE��Nodeb��ȡ
				v_hNode_BTS=m_cTree.InsertItem(v_pNodeb->first+"     ��"
					+v_pNodeb->second.v_sStation+"��",4,5,v_hRoot);		//���������ӽڵ� [NodeB]
				for (v_pCell=v_pNodeb->second.v_mCell.begin();v_pCell!=v_pNodeb->second.v_mCell.end();v_pCell++)	//����������
				{
					v_hNode_Cell=m_cTree.InsertItem(v_pCell->first+"     ��"+v_pCell->second+"��",0,1,v_hNode_BTS);	//���������ӽڵ� [Cell]
					v_cNode_Cell.v_sCell=v_pCell->first;				//Cell
					v_cNode_Cell.v_sNetwork=v_pNodeb->second.v_sNetwork;//Network
					v_mTree_All.insert(pair<HTREEITEM,Node_Cell_Struct>(v_hNode_Cell,v_cNode_Cell));				//��������
				}
			}
		}

		//��չ��
		v_hNode_BTS=m_cTree.GetRootItem();
		while (v_hNode_BTS!=NULL)
		{
			m_cTree.ExpandTree(v_hNode_BTS);							//����չ��
			v_hNode_BTS=m_cTree.GetNextSiblingItem(v_hNode_BTS);		//��ã��ھӽڵ�
		}

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	v_mNodeb.clear();													//�������
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Chart
////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------------
//	�������ܣ�Draw track cursor when mouse is moving over plotarea
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::OnMouseMovePlotArea()
{
	// Get the focus to ensure being able to receive mouse wheel events
	m_cChartView.SetFocus();

	m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),m_cLable.GetCheck()); 
	m_cChartView.updateDisplay();
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�����ֻ�����Ϣ [��;���Ŵ���С������ƽ��]
//------------------------------------------------------------------------------------------------------
BOOL CAll_PM_Draw::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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
void CAll_PM_Draw::OnViewPortChanged()
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
void CAll_PM_Draw::OnChart_Para()
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
//	�������ܣ�Tree���οؼ���Ŀ�仯��Ӧ
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		HTREEITEM v_hCurrent = m_cTree.GetSelectedItem();
		if(v_hCurrent!= NULL && !m_cTree.ItemHasChildren(v_hCurrent))	//ѡ��NodeΪҶ�ӣ���
		{
			if (!m_cMuti_Line.GetCheck())								//���ߣ�����
			{
				if (v_hTree_Current != NULL && v_hTree_Current != v_hCurrent)							//ȥ���ϴε�ѡ��
					m_cTree.SetItemState(v_hTree_Current,INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);//��ѡ��Ч
				v_hTree_Current=v_hCurrent;								//��ȡ��Tree�ڵ�
				m_cTree.SetItemState(v_hTree_Current,INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK);	//��ѡ��Ч
				v_iWorking=2;											//�������񣺵��߻���
				v_pIterator_Frame->second.v_iThread_Exit=1;				//�̣߳�����
				v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);	//������ѯ���ݿ��߳�
			}
			else
			{
				v_hTree_Current=v_hCurrent;								//��ȡ��Tree�ڵ�
				if (m_cTree.GetItemState(v_hTree_Current,TVIS_STATEIMAGEMASK)>>12 == 1)
				{
					m_cTree.SetItemState(v_hTree_Current,INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK);//��ѡ��Ч
					m_set_Selected.insert(m_cTree.GetSelectedItem());	//�����д洢��ѡ�ڵ����Ϣ
				}
				else
				{
					m_cTree.SetItemState(v_hTree_Current,INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);//��ѡ��Ч
					set<HTREEITEM>::iterator v_it_item = m_set_Selected.find(v_hTree_Current);          //����Ƿ�����ѡ�нڵ㱻����
					if(v_it_item != m_set_Selected.end())				//
						m_set_Selected.erase(v_it_item);				//�������ĳһ��ԭ��ѡ�еĽڵ����ڳ����ˣ����ǾͶ������Ĵ˽ڵ����
				}
			}
		}
	}
	
	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������Ļ��ʾ��Ϣ
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_Show_Clear()									    //�����Ļ��ʾ��Ϣ
{
	m_cTree.DeleteAllItems();											//ɾ��Tree�ؼ���Ϣ
	
	v_hTree_Current=NULL;												//ѡ�е�Tree�ڵ�
	My_Chart_New();														//��ͼ��λ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::OnBnClickedCkMuti()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_cMuti_Line.GetCheck())										//��������Ч��
	{
		m_set_Selected.clear();											//����ʱ��������գ�
		m_cDay.SetCheck(false);                                         //��Ч�����죻
		m_cKPI_S.SetCheck(false);										//��Ч����̬���ޣ�
		if(v_hTree_Current != NULL)										//��Ч�ڵ㣿����
			v_hTree_Current = NULL;										//ѡ�еĽڵ㣺��Ч
	}
	OnBnClickedDelete();												//���Chart
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ͼ�� [��ͼ��λ]
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//���Tree Nodeѡ��״̬
	HTREEITEM item;
	for(item = m_cTree.GetRootItem();item != NULL;item = m_cTree.GetNextItem(item,TVGN_NEXT))
		My_Tree_Clear(item);								//���Tree Nodeѡ��״̬

	//��ͼ��ʼ��
	My_Chart_New();														//��ͼ��λ
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ����Tree Nodeѡ��״̬
//-----------------------------------------------------------------------------------------------------
void CAll_PM_Draw::My_Tree_Clear(HTREEITEM v_hNode)
{
	if(!m_cTree.ItemHasChildren(v_hNode))
	{
		m_cTree.SetItemState(v_hNode,INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);	//��ѡ��Ч
		return;
	}
	HTREEITEM hNextItem = m_cTree.GetChildItem(v_hNode);
	while (hNextItem != NULL)
	{
		My_Tree_Clear(hNextItem);
		hNextItem = m_cTree.GetNextItem(hNextItem, TVGN_NEXT);
	}
	if(!m_cTree.ItemHasChildren(v_hNode))
		m_cTree.SetItemState(v_hNode,INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);	//��ѡ��Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ͼ��λ
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_Chart_New()
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
void CAll_PM_Draw::My_Chart_Muti()
{
	if(v_hTree_Current!=NULL)											//��Ч��Ŀ��ţ�����
	{

		if(!m_cMuti_Line.GetCheck())									//����
		{
			m_cMuti_Line.SetCheck(false);								//������Ч
			My_Chart();													//����
		}
		if (m_cMuti_Line.GetCheck())									//����
		{
			My_Chart_New();												//��ͼ��λ
			for(set<HTREEITEM>::iterator v_itSel = m_set_Selected.begin();v_itSel != m_set_Selected.end();++v_itSel)
				My_Tree_Chart(*v_itSel);								//������һ�飬��ѡ�еĽڵ���Ϣ���л���
		}

	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ˢ�¡����»������ߡ�
//------------------------------------------------------------------------------------------------------	
void CAll_PM_Draw::OnBnClickedRefresh()
{
	v_iWorking=3;														//�����������߻��ơ����ߡ����ߡ�
	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ͼ��λȻ���ҵ�ǰ�ڵ�
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_Chart()
{
	//��ͼ��λ
	My_Chart_New();														//��ͼ��λ

	//��ͼ������Tree���ҵ�ѡ�еĽڵ�	
	My_Tree_Chart(m_cTree.GetSelectedItem());							//��ͼ��ѡ�еĽڵ�

}

//------------------------------------------------------------------------------------------------------
//	�������ܣ���ͼ��ѡ�е�Tree Node
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::My_Tree_Chart(HTREEITEM v_hNode)
{
	if(!m_cTree.ItemHasChildren(v_hNode))								//�޺��ӽڵ㣺������ ����Cell�ڵ㣬�����������ߡ�
	{																	//�޺��ӽڵ�
		if (m_cTree.GetItemState(v_hNode,TVIS_STATEIMAGEMASK)>>12 == 3)	//ѡ�У�����
		{
			My_Tree_Chart_GetNode(v_hNode);								//��ȡ���ڵ���Ϣ��������public�����С�
			My_Chart_IP();												//���ƣ�������Ϣ
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ�ڵ���Ϣ 
//	��    ����v_hNode���ڵ���
//			 �����ڣ���ͼ����ʾ�ڵ���Ϣ��������v_sNetwork��v_sCell�����С�
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_Tree_Chart_GetNode(HTREEITEM v_hNode)
{
	mNode_Cell::iterator v_pNode=v_mTree_All.find(v_hNode);	            //���ң��ڵ���Ϣ	
	if (v_pNode != v_mTree_All.end())
	{
		v_sCell=v_pNode->second.v_sCell;								//Cell
		v_sNetwork=v_pNode->second.v_sNetwork;							//Netowrk
		v_hAll=v_hNode;                                                 //��Ȼ�ڵ���
	}
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�Chart��һ����
//	��    ����ѡ�е� IP [v_sItem_IP]
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::My_Chart_IP()
{
	mLTE_PM_Type::iterator	v_pSelect;									//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����
	m_cDate.GetTime(v_dDate_Start);										//��ã�ѡ����������
	v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//����ʱ�䣺00:00:00

	for (v_pSelect=v_mLTE_PM_Type.begin();v_pSelect!=v_mLTE_PM_Type.end();v_pSelect++)
	{
		for (v_pPM_Name=v_pSelect->second.v_mLTE_PM.begin();v_pPM_Name!=v_pSelect->second.v_mLTE_PM.end();v_pPM_Name++)
		{
			//��ȡ�������ݿ⣬����ͼ
			if (v_pPM_Name->second.v_bOK)								//����ѡ�У�����
				My_Load_Performance(v_pSelect->first,					//��������
				v_pPM_Name->second.v_sTable_Key,						//�����ļ�����key
				v_sCell,												//С��
				v_pPM_Name->second.v_sPM_Name,							//��������
				v_pPM_Name->first,										//�����ֶ�
				v_dDate_Start,											//��ʼ����
				v_iChart_Day_Sum);										//��ѯ����	
		}
	}

	//��ͼ��Chart
	::PostMessage(m_hWnd, 123, (WPARAM) 0, (LPARAM) 0);					//������Ϣ������ChartView	
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ��
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::OnDtnCloseupBegDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cDate.GetTime(&v_dChart_Date);									//��ã���ѯ��ʼ����

	My_Chart_New();														//��ͼ��λ

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ��
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iChart_Day_Sum=m_cDay_Sum.GetCurSel()+1;							//��ã���ѯ�������

	My_Chart_New();														//��ͼ��λ
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�	��ȡ���ݱ��е�KPI_Instance����
//	������		v_sIP��IP��	v_dDate_Start����ѯ����ʼ���ڣ�	v_iDay_Sum����ѯ����
//	��ȡ���̣�	�Ӷ�Ӧ���л�ȡ���ݣ�����v_mIP_Instance�����У�[��Ÿ������ȵ�IP����]
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::My_Load_Performance(CString v_sType,CString v_sFile_Key,CString v_sCell,CString v_sPerformance,CString v_sField,COleDateTime v_dDate_Start,int v_iDay_Sum)
{
	CString			v_sSql,v_sTemp,v_sTime;								//��ʱ����
	CString			v_sTime_Start,v_sTime_End;							//����
	int				v_iNum_Old=-1;										//��¼�����
	int				v_iTable_Num,v_iTable_Num_New;						//ʵʱ�����
	CString			v_sInstance;										//Instance_ID
	COleDateTime	v_dTime;											//��ѯ����
	Chart_Data		v_cChart_Data;										//KPI����
	double			v_dValue;											//ȡֵ
	_variant_t Var;														//��ʱ����
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

		//�����ݱ������ݶ�ȡ��1�ζ�1��������� [һ����Ϊ1������]
		int v_iDay=0;													//��ȡ����
		while(v_iDay<v_iDay_Sum)										//ѭ������ѯ����
		{
			//��ȡ��1���������
			v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//��ʼʱ��
			while (true)
			{
				v_iTable_Num=v_dDate_Start.GetMonth();					//ʵʱ����ţ��·�
				v_dTime=v_dDate_Start;									//1�����ѯ��������
				//�����
				v_dDate_Start += COleDateTimeSpan(1,0,0,0);				//����һ��
				v_iDay++;												//
				if (v_iDay>=v_iDay_Sum)
					break;
				v_iTable_Num_New=v_dDate_Start.GetMonth();				//ʵʱ����ţ��·�
				if (v_iTable_Num!=v_iTable_Num_New)
					break;
			}
			v_sTime_End=v_dTime.Format("%Y-%m-%d")+" 23:59:59";			//����ʱ��
			v_sNet="Pm_LTE_"+v_sFile_Key;								//�ļ�����
			if (m_cType.GetCurSel()==1)									//1H���ȣ�����
			{
				v_sDate=v_dTime.Format("%Y%m");							//����
				v_sNet+="_"+v_sDate;									//���ӣ�����
				//��飺�������Ƿ���Ч�����ݣ������Ƽ���v_mTable_Name��
				set<CString>::iterator v_pTable=v_mTable_Name.find(v_sNet);		//���ң���
				if (v_pTable==v_mTable_Name.end())						//û�ҵ�
					continue;											//��Ч����
			}

			//�ֶδ����������ܣ�һ�������ֶΣ��������ܣ���������ֶΡ�
			v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
			v_sSql+="select zTime,["+v_sField+"]";

			v_sSql+=" from "+v_sNet+" where zCell='"+ v_sCell+"' and zTime>='"
				+ v_sTime_Start +"' and  zTime<='"+ v_sTime_End +"' order by zTime";	//��ѯ���ݿ⣺���ܱ�
			v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);	//ִ��SELETE���										

			while(!v_pRecordset->adoEOF)
			{
				//ֱ�Ӷ�ȡ�ֶΣ�Ŀ������߶�ȡ�ٶȣ�[�����ֶζ�ȡ����ȡֵЧ�ʵͣ�����]
				Var=v_pRecordset->GetCollect((_variant_t)"zTime");		//����
				if (Var.vt != VT_NULL)
					v_dTime=Var.date;									//
				else
					v_dTime=COleDateTime::GetCurrentTime();				//Ĭ�ϣ���ǰ����

				//�ֶζ�ȡ���������ܣ�һ�������ֶΣ��������ܣ���������ֶΣ������㡿
				v_dValue=v_cAdoConn.GetField_Double(v_sField);			//����

				double	m_nextDataTime = Chart::chartTime(v_dTime.GetYear(),v_dTime.GetMonth(),v_dTime.GetDay(),v_dTime.GetHour(),v_dTime.GetMinute(),0);		//��ȷ����
				mTime_Array::iterator v_pIterator_Time=m_cChartView.v_mTime_Array.find(m_nextDataTime);					//��������[����m_nextDataTime]
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
//	�������ܣ�װ�����ܲ�����Ϣ
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_LoadData_Para()
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

		//��1����������
		v_sType="��������";												//��������								
		v_cLTE_PM_Type.v_sTable_Key=v_sType;							//������-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//��������

		//��2����ȡ�����ܲ��������桿
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
				v_cLTE_PM.v_sTable_Key="Formula_Performance";			//���ܣ�������
				v_cLTE_PM.v_bOK=false;									//���ܣ�δѡ��
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//���ӣ����ܲ���
			}

			v_pRecordset->MoveNext();									//
		}

		//��3����ȡ���������ݱ����ơ����ڷ�����Ч�����ơ�
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and name like 'Pm_LTE_Formula_Performance%' order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//������

			v_mTable_Name.insert(v_sTable);								//���Ӽ���

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
void CAll_PM_Draw::My_SendToFile() 
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
					v_pIterator_KPI=m_cChartView.v_mChart_Data.begin();//��ͷ��ʼ
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
					v_dKPI=v_pIterator_KPI->second.v_dChart_Value[vi];	//��ã�KPIֵ
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
