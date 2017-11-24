// QUA_Element_TD.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "QUA_Element_TD.h"


// CQUA_Element_TD

IMPLEMENT_DYNCREATE(CQUA_Element_TD, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CQUA_Element_TD::CQUA_Element_TD()
	: CFormView(CQUA_Element_TD::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CQUA_Element_TD::~CQUA_Element_TD()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_COMBO1, m_cArea);
	DDX_Control(pDX, IDC_LIST3, m_cList_NodeB);
	DDX_Control(pDX, IDC_SUM_ALARM2, m_cSum_NodeB);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_CHECK1, m_cLocation);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CQUA_Element_TD, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CQUA_Element_TD::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL, &CQUA_Element_TD::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CQUA_Element_TD::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_EXCEL2, &CQUA_Element_TD::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CQUA_Element_TD::OnLvnColumnclickList3)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CQUA_Element_TD::OnNMClickList3)
	ON_BN_CLICKED(IDC_CHECK1, &CQUA_Element_TD::OnBnClickedCheck1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CQUA_Element_TD::OnCbnSelchangeCombo1)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CQUA_Element_TD::OnLvnGetdispinfoList1)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST3, &CQUA_Element_TD::OnLvnGetdispinfoList3)
END_MESSAGE_MAP()


// CQUA_Element_TD ���

#ifdef _DEBUG
void CQUA_Element_TD::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQUA_Element_TD::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQUA_Element_TD ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="TD-LTE��Ԫ��ѯ";										//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б� [NodeB]
	::SendMessage(m_cList_NodeB.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_NodeB.InsertColumn(0,"���",LVCFMT_CENTER,60);				//
	m_cList_NodeB.InsertColumn(1,"RNC����",LVCFMT_LEFT,100);			//
	m_cList_NodeB.InsertColumn(2,"NodeB",LVCFMT_LEFT,200);				//
	m_cList_NodeB.InsertColumn(3,"������",LVCFMT_LEFT,200);				//
	m_cList_NodeB.InsertColumn(4,"Ӳ��",LVCFMT_LEFT,100);				//
	m_cList_NodeB.InsertColumn(5,"����汾",LVCFMT_LEFT,200);			//
	m_cList_NodeB.InsertColumn(6,"����",LVCFMT_LEFT,80);				//
	m_cList_NodeB.InsertColumn(7,"��������/��Դ��",LVCFMT_LEFT,260);	//
	m_cList_NodeB.InsertColumn(8,"״̬",LVCFMT_LEFT,60);				//
	m_cList_NodeB.InsertColumn(9,"��������",LVCFMT_LEFT,60);			//
	m_cList_NodeB.InsertColumn(10,"��������[ʵ��]",LVCFMT_CENTER,120);	//
	m_cList_NodeB.InsertColumn(11,"��������[�ʹ�]",LVCFMT_CENTER,100);	//
	m_cList_NodeB.SetExtendedStyle(m_cList_NodeB.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��ʼ�����б� [Cell]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"RNC����",LVCFMT_LEFT,100);					//
	m_cList.InsertColumn(2,"����",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(3,"NODEBλ��",LVCFMT_LEFT,80);					//
	m_cList.InsertColumn(4,"UTRANCELLλ��",LVCFMT_LEFT,80);				//
	m_cList.InsertColumn(5,"NodeB",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(6,"NodeB�ʹ�״̬",LVCFMT_LEFT,80);				//
	m_cList.InsertColumn(7,"Cell",LVCFMT_LEFT,200);						//
	m_cList.InsertColumn(8,"������",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(9,"Cell�ʹ�״̬",LVCFMT_LEFT,80);				//
	m_cList.InsertColumn(10,"LAC",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(11,"��������",LVCFMT_LEFT,60);					//
	m_cList.InsertColumn(12,"��������[ʵ��]",LVCFMT_CENTER,120);		//
	m_cList.InsertColumn(13,"��������[�ʹ�]",LVCFMT_CENTER,100);		//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cArea.AddString("All");
	m_cArea.AddString("3G");
	m_cArea.AddString("4G");
	m_cArea.SetCurSel(0);												//Ĭ�ϣ�ȫ��

	//�ִ�
	CRect	v_cRect;													//����
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//��ã�ָ��
	pWnd->GetWindowRect(&v_cRect);										//��ã��ߴ�
	ScreenToClient(&v_cRect);											//����任
	pWnd->DestroyWindow();												//���٣�ָ��

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Main.Create(											//��ֱ��
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList_NodeB,			// Top pane
		&m_cList,				// Bottom pane
		IDC_QUA_Element_TD,		// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);

	PostMessage(WM_SIZE);												//���

	//��Ϣ��ʼ��
	v_iList_Item=-1;													//��Ŀѡ����Ч
	m_cLocation.SetCheck(true);											//ѡ�У�NodeB
	OnBnClickedCheck1();												//��ʾ��ѡ������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CQUA_Element_TD::PreTranslateMessage(MSG* pMsg)
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
void CQUA_Element_TD::OnDestroy()
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
void CQUA_Element_TD::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//�ִ��ߴ����
		CRect v_cRect;
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=45;
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�С������
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�NodeB����
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_NodeB);										//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б����� [Cell]
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort_Virtual(pNMHDR);									//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б����� [NodeB]
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList_NodeB.My_Sort_Virtual(pNMHDR);								//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������б���ʾ���ݻ�ȡ
//	��	  ���������б���Ҫ��Ԫ�ز���(�С���)�������������в�ѯ���ݣ���ֵ���б��С�
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnLvnGetdispinfoList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_cList.Get_List_Data(pDispInfo);									//�����б���ʾ���ݻ�ȡ

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������б���ʾ���ݻ�ȡ
//	��	  ���������б���Ҫ��Ԫ�ز���(�С���)�������������в�ѯ���ݣ���ֵ���б��С�
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_cList_NodeB.Get_List_Data(pDispInfo);								//�����б���ʾ���ݻ�ȡ

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������Ԫ����
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=2;														//��������װ��RNC����
	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����б� [NodeB]
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item>=0)											//��Ч��Ŀ��ţ�����
				m_cList_NodeB.SetItemBkColor(v_iList_Item,RGB(255,255,255));	//�ָ�����ɫ
			v_iList_Item=pNMListView->iItem;							//��ã�ѡ����Ŀ���
			m_cList_NodeB.SetItemBkColor(v_iList_Item,RGB(255,0,0));	//���ñ���ɫ����ɫ

			v_iWorking=3;												//�����������ڵ�����Ч��Ӧ
			v_pIterator_Frame->second.v_iThread_Exit=1;					//�̣߳�����
			v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);		//������ѯ���ݿ��߳�
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�NodeBѡ��
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cKey_Select.ResetContent();										//���ѡ������
	if (m_cLocation.GetCheck())											//BTSѡ�У�����
	{																	//ѡ��
		m_SplitterPane_Main.ShowTopPane();								//��ʾ��NodeB/Cell
		m_cKey_Select.AddString("NodeB");
		m_cKey_Select.AddString("������");
		m_cKey_Select.AddString("RNC");
		m_cKey_Select.AddString("����/��Դ��");
	}
	else
	{																	//ûѡ��
		m_SplitterPane_Main.HideTopPane();								//Hide��NodeB/Cell
		m_cKey_Select.AddString("Cell");
		m_cKey_Select.AddString("������");
		m_cKey_Select.AddString("NodeB");
		m_cKey_Select.AddString("����/��Դ��");
	}
	m_cKey_Select.SetCurSel(0);											//Ĭ�ϣ�ѡ���һ��
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//��������С����ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CQUA_Element_TD::My_Thread_Inoput(LPVOID lparam)
{
	CQUA_Element_TD *  lp_this = NULL ;
	lp_this = (CQUA_Element_TD *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(false);								//��������Ч
	lp_this->m_cExcel_NodeB.EnableWindow(false);						//NodeB��������Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(true);								//��������Ч
	lp_this->m_cExcel_NodeB.EnableWindow(true);							//NodeB��������Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		if (m_cLocation.GetCheck())										//վ��ѡ�У�����
			My_Query_NodeB();											//BTS��ѯ
		else
			My_Query_Cell();											//Cell��ѯ
		break;
	case 3:
		My_LoadData_NodeB_Cell();										//NodeB�б�����Ӧ
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�NodeB��ѯ
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::My_Query_NodeB()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int				v_iID=0;											//KPI����
	CString			v_sSql,v_sTemp;										//��ʱ����
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT LTE_NODEB.*  FROM LTE_NODEB";

	//��ѯ����׼��
	m_cKey.GetWindowText(v_sLog_Key);									//��ã���ѯKEY
	v_sLog_Key.Replace("[","[[]");										//���ƴ���
	v_sLog_Key.Replace("'","");											//���ƴ���
	v_sLog_Key.Trim();													//ȥ���ո�

	bool v_bFlag=false;													//���
	if (!v_sLog_Key.IsEmpty())											//��ѯKEY����
	{
		v_bFlag=true;													//��ǣ���Ч
		switch (m_cKey_Select.GetCurSel())								//ѡ�񣿣���
		{
		case 0:															//NodeB
			v_sSql+=" where zNODEB like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//������
			v_sSql+=" where zNODEB_Name like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//RNC
			v_sSql+=" where zRNC like '%"+v_sLog_Key+"%'";
			break;
		case 3:															//��������/��Դ��
			v_sSql+=" where LTE_NODEB.zStation like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	if (m_cArea.GetCurSel()==1)
	{
		if (v_bFlag)
			v_sSql+=" and zRNC like '%DAL-%'";
		else
			v_sSql+=" where zRNC like '%DAL-%'";
	}
	else if (m_cArea.GetCurSel()==2)
	{
		if (v_bFlag)
			v_sSql+=" and zRNC like '%dalian_%'";
		else
			v_sSql+=" where zRNC like '%dalian_%'";
	}

	v_sSql+=" order by zNODEB";

	My_LoadData_NodeB_Data(v_sSql);										//��ȡ��Cell��Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��NodeB��Ϣ
//------------------------------------------------------------------------------------------------------			
void CQUA_Element_TD::My_LoadData_NodeB_Data(CString v_sSql)
{
	int				v_iID=0;											//KPI����
	CString			v_sTemp,v_sCover_Type;								//��ʱ����
	CString			v_sDate,v_sHardware,v_sIP,v_sVer,v_sStation,v_sOnline,v_sDate_Update,v_sDate_Creat,v_sMaintence;	//��ʱ����
	CString			v_sRNC_Name,v_sNodeB_Name,v_sNodeB_ID,v_sArea,v_sAgency,v_sAgency_Phone,v_sAgency_Oper;	//��ʱ����
	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����

	//�����б�
	mInt_String		v_mLine_Data;										//�����б�������
	m_cList_NodeB.Put_Line_Begin();										//�б�������ݿ�ʼ

	_variant_t	Var;
	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		m_cList_NodeB.DeleteAllItems();									//�б�λ
		m_cList.DeleteAllItems();										//�б�λ
		v_iList_Item=-1;												//��Ŀѡ����Ч
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sRNC_Name = v_cAdoConn.GetField_String("zRNC");			//zRNC
			v_sNodeB_Name = v_cAdoConn.GetField_String("zNODEB");		//zNODEB
			v_sNodeB_ID = v_cAdoConn.GetField_String("zNODEB_Name");	//zNODEB_Name
			v_sHardware = v_cAdoConn.GetField_String("zHardware");		//zHardware
			v_sVer = v_cAdoConn.GetField_String("zNODEB_Version");		//zNODEB_Version
			v_sArea = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_sCover_Type = v_cAdoConn.GetField_String("zCover_Type");	//zCover_Type
			Var=v_pRecordset->GetCollect((_variant_t)"zDate");			//��������
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sDate = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sDate="";
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			Var=v_pRecordset->GetCollect((_variant_t)"zDate_Update");	//zDate_Update
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sDate_Update = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sDate_Update="";

			//���ݸ�λ
			v_mLine_Data.clear();										//�������

			//��ʾ
			v_sTemp.Format("%04d",v_iID+1);
			v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));			//���
			v_mLine_Data.insert(pair<int,CString>(1,v_sRNC_Name));		//��Ԫ����
			v_mLine_Data.insert(pair<int,CString>(2,v_sNodeB_Name));	//NodeB
			if (v_sNodeB_Name.Find("-EL")>=0)
				v_mLine_Data.insert(pair<int,CString>(3,v_sNodeB_ID));	//zNODEB_ID [����E_Nodeb����]
			v_mLine_Data.insert(pair<int,CString>(4,v_sHardware));		//v_sHardware
			v_mLine_Data.insert(pair<int,CString>(5,v_sVer));			//����汾
			v_mLine_Data.insert(pair<int,CString>(6,v_sArea));			//����
			v_mLine_Data.insert(pair<int,CString>(7,v_sStation));		//��������/��Դ��
			v_mLine_Data.insert(pair<int,CString>(8,v_sOnline));		//״̬
			v_mLine_Data.insert(pair<int,CString>(9,v_sCover_Type));	//��������
			v_mLine_Data.insert(pair<int,CString>(10,v_sDate));			//��������[ʵ��]
			v_mLine_Data.insert(pair<int,CString>(11,v_sDate_Update));	//��������[�ʹ�]

			//��ӣ������б�
			m_cList_NodeB.Put_Line_Data(v_mLine_Data);					//�б����������

			v_pRecordset->MoveNext();									//
			v_iID++;													//������NodeB����

			if (v_iID%100==0)
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum_NodeB.SetWindowText(v_sTemp);					//��ʾ��NodeB����
			}
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cSum_NodeB.SetWindowText(v_sTemp);							//��ʾ��NodeB����

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	//�����б�
	m_cList_NodeB.Put_Line_End();										//�б�������ݽ���
	v_mLine_Data.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�С����ѯ
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::My_Query_Cell()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int				v_iID=0;											//KPI����
	CString			v_sSql,v_sTemp;										//��ʱ����
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT LTE_Cell.*, LTE_NODEB.zRNC,LTE_NODEB.zStation as zStation_NodeB,\
			  LTE_NODEB.zOnline AS zOnline_NodeB,LTE_NODEB.zRegion AS zRegion_NodeB FROM LTE_Cell \
			  left JOIN LTE_NODEB ON LTE_Cell.zNODEB = LTE_NODEB.zNODEB";
	//��ѯ����׼��
	m_cKey.GetWindowText(v_sLog_Key);									//��ã���ѯKEY
	v_sLog_Key.Replace("[","[[]");										//���ƴ���
	v_sLog_Key.Replace("'","");											//���ƴ���
	v_sLog_Key.Trim();													//ȥ���ո�

	bool v_bFlag=false;													//���
	if (!v_sLog_Key.IsEmpty())											//��ѯKEY����
	{
		v_bFlag=true;													//��ǣ���Ч
		switch (m_cKey_Select.GetCurSel())								//ѡ�񣿣���
		{
		case 0:															//С������
			v_sSql+=" where zCell like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//������
			v_sSql+=" where zCell_Name like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//NodeB
			v_sSql+=" where LTE_Cell.zNODEB like '%"+v_sLog_Key+"%'";
			break;
		case 3:															//��������/��Դ��
			v_sSql+=" where LTE_Cell.zStation like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	if (m_cArea.GetCurSel()==1)
	{
		if (v_bFlag)
			v_sSql+=" and zRNC like '%DAL-%'";
		else
			v_sSql+=" where zRNC like '%DAL-%'";
	}
	else if (m_cArea.GetCurSel()==2)
	{
		if (v_bFlag)
			v_sSql+=" and zRNC like '%dalian_%'";
		else
			v_sSql+=" where zRNC like '%dalian_%'";
	}

	v_sSql+=" order by zCell";

	My_LoadData_Cell_Data(v_sSql);										//��ȡ��Cell��Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ�� NodeB_Cell ��Ϣ
//------------------------------------------------------------------------------------------------------			
void CQUA_Element_TD::My_LoadData_NodeB_Cell()
{
	CString v_sBTS=m_cList_NodeB.GetItemText(v_iList_Item,2);			//v_sBTS
	CString v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT LTE_Cell.*, LTE_NODEB.zRNC,LTE_NODEB.zStation as zStation_NodeB,\
			  LTE_NODEB.zOnline AS zOnline_NodeB,LTE_NODEB.zRegion AS zRegion_NodeB FROM LTE_Cell \
			  left JOIN LTE_NODEB ON LTE_Cell.zNODEB = LTE_NODEB.zNODEB";
	v_sSql+=" where LTE_Cell.zNODEB='"+v_sBTS+"' order by zCell";		//SQL
	My_LoadData_Cell_Data(v_sSql);										//ִ��SQL
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��С����Ϣ
//------------------------------------------------------------------------------------------------------			
void CQUA_Element_TD::My_LoadData_Cell_Data(CString v_sSql)
{
	int				v_iID=0;											//��ʱ����
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����
	mString_String::iterator v_pNodeB;									//������

	//�����б�
	mInt_String		v_mLine_Data;										//�����б�������
	m_cList.Put_Line_Begin();											//�б�������ݿ�ʼ

	_variant_t	Var;
	CString		v_sElementName,v_sNodeB_Name,v_sCell,v_sDate,v_sAgency_Oper,v_sStation_NodeB,v_sOnline_NodeB;	//
	CString		v_sLac,v_sArea,v_sCover,v_sStation,v_sOnline,v_sDate_Update,v_sCell_C;	//
	CString		v_sMaintence;											//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList.DeleteAllItems();										//���Items

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			Var=v_pRecordset->GetCollect((_variant_t)"zRNC");			//RNC
			if (Var.vt != VT_NULL)
				v_sElementName=Var.bstrVal;
			else
				v_sElementName="";
			Var=v_pRecordset->GetCollect((_variant_t)"zNodeB");			//NodeB
			if (Var.vt != VT_NULL)
				v_sNodeB_Name=Var.bstrVal;
			else
				v_sNodeB_Name="";
			Var=v_pRecordset->GetCollect((_variant_t)"zCell");			//С����
			if (Var.vt != VT_NULL)
				v_sCell=Var.bstrVal;
			else
				v_sCell="";
			Var=v_pRecordset->GetCollect((_variant_t)"zCell_Name");		//������
			if (Var.vt != VT_NULL)
				v_sCell_C=Var.bstrVal;
			else
				v_sCell_C="";
			Var=v_pRecordset->GetCollect((_variant_t)"zLAC");			//LAC
			if (Var.vt != VT_NULL)
				v_sLac=Var.bstrVal;
			else
				v_sLac="";
			Var=v_pRecordset->GetCollect((_variant_t)"zRegion_NodeB");	//����
			if (Var.vt != VT_NULL)
				v_sArea=Var.bstrVal;
			else
				v_sArea="";
			Var=v_pRecordset->GetCollect((_variant_t)"zCover_Type");	//��������
			if (Var.vt != VT_NULL)
				v_sCover=Var.bstrVal;
			else
				v_sCover="";
			Var=v_pRecordset->GetCollect((_variant_t)"zDate");			//��������
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sDate = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sDate="";
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_sStation_NodeB = v_cAdoConn.GetField_String("zStation_NodeB");	//zStation_NodeB
			v_sOnline_NodeB = v_cAdoConn.GetField_String("zOnline_NodeB");		//zOnline_NodeB
			Var=v_pRecordset->GetCollect((_variant_t)"zDate_Update");	//zDate_Update
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sDate_Update = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sDate_Update="";

			//���ݸ�λ
			v_mLine_Data.clear();										//�������

			//��ʾ
			v_sTemp.Format("%06d",v_iID+1);
			v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));			//���
			v_mLine_Data.insert(pair<int,CString>(1,v_sElementName));	//��Ԫ����
			v_mLine_Data.insert(pair<int,CString>(2,v_sArea));			//����
			v_mLine_Data.insert(pair<int,CString>(3,v_sStation_NodeB));	//NODEBλ��
			v_mLine_Data.insert(pair<int,CString>(4,v_sStation));		//UTRANCELLλ��
			v_mLine_Data.insert(pair<int,CString>(5,v_sNodeB_Name));	//NodeB
			v_mLine_Data.insert(pair<int,CString>(6,v_sOnline_NodeB));	//״̬
			v_mLine_Data.insert(pair<int,CString>(7,v_sCell));			//С����
			if (v_sNodeB_Name.Find("-EL")>=0)
				v_mLine_Data.insert(pair<int,CString>(8,v_sCell_C));	//������
			v_mLine_Data.insert(pair<int,CString>(9,v_sOnline));		//״̬
			v_mLine_Data.insert(pair<int,CString>(10,v_sLac));			//LAC
			v_mLine_Data.insert(pair<int,CString>(11,v_sCover));		//����
			v_mLine_Data.insert(pair<int,CString>(12,v_sDate));			//��������[ʵ��]
			v_mLine_Data.insert(pair<int,CString>(13,v_sDate_Update));	//��������[�ʹ�]

			//��ӣ������б�
			m_cList.Put_Line_Data(v_mLine_Data);						//�б����������

			v_pRecordset->MoveNext();									//
			v_iID++;													//�������澯����

			if (v_iID%100==0)											//��ʾ��100������
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum.SetWindowText(v_sTemp);							//��ʾ��С������
			}
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cSum.SetWindowText(v_sTemp);									//��ʾ��Alarm����

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	//�����б�
	m_cList.Put_Line_End();												//�б�������ݽ���
	v_mLine_Data.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
