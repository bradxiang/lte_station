// QUA_Station.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "QUA_Station.h"
#include "WebPage.h"													//��װ����CWebPage��������HTML��JSP����


// CQUA_Station

IMPLEMENT_DYNCREATE(CQUA_Station, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CQUA_Station::CQUA_Station()
	: CFormView(CQUA_Station::IDD)
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CQUA_Station::~CQUA_Station()
{
	My_Clear();															//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CQUA_Station::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_Station);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_BUTTON8, m_cMap_Get);
	DDX_Control(pDX, IDC_LIST5, m_cList_BTS);
	DDX_Control(pDX, IDC_CHECK2, m_cBTS);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CQUA_Station, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CQUA_Station::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL2, &CQUA_Station::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CQUA_Station::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CQUA_Station::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_BUTTON8, &CQUA_Station::OnBnClickedButton7)

	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST3, &CQUA_Station::OnLvnGetdispinfoList3)
END_MESSAGE_MAP()


// CQUA_Station ���

#ifdef _DEBUG
void CQUA_Station::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQUA_Station::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQUA_Station ��Ϣ��������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CQUA_Station::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ�����ר�ô����/����û���
	//��������
	v_sFrame_Name="վַ��ѯ";											//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б��� [վַ]
	::SendMessage(m_cList_Station.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Station.InsertColumn(0,"���",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(1,"����/��Դ������",LVCFMT_LEFT,300);	//
	m_cList_Station.InsertColumn(2,"����",LVCFMT_LEFT,80);				//
	m_cList_Station.InsertColumn(3,"վ��",LVCFMT_CENTER,50);			//
	m_cList_Station.InsertColumn(4,"����",LVCFMT_CENTER,50);			//
	m_cList_Station.InsertColumn(5,"��_BTS",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(6,"��_NodeB",LVCFMT_CENTER,80);		//
	m_cList_Station.InsertColumn(7,"��_E_NodeB",LVCFMT_CENTER,90);		//
	m_cList_Station.InsertColumn(8,"��Զվ",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(9,"��_BTS",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(10,"��_NodeB",LVCFMT_CENTER,80);		//
	m_cList_Station.InsertColumn(11,"��_E_NodeB",LVCFMT_CENTER,90);		//
	m_cList_Station.InsertColumn(12,"��վ",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(13,"��_BTS",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(14,"��_NodeB",LVCFMT_CENTER,80);		//
	m_cList_Station.InsertColumn(15,"��_E_NodeB",LVCFMT_CENTER,90);		//
	m_cList_Station.InsertColumn(16,"״̬",LVCFMT_LEFT,60);				//
	m_cList_Station.InsertColumn(17,"����",LVCFMT_RIGHT,80);			//
	m_cList_Station.InsertColumn(18,"γ��",LVCFMT_RIGHT,80);			//
	m_cList_Station.InsertColumn(19,"����",LVCFMT_LEFT,120);			//
	m_cList_Station.InsertColumn(20,"��ַ",LVCFMT_LEFT,300);			//
	m_cList_Station.InsertColumn(21,"��������",LVCFMT_CENTER,100);		//
	m_cList_Station.InsertColumn(22,"��������",LVCFMT_CENTER,100);		//
	m_cList_Station.SetExtendedStyle(m_cList_Station.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList_Station.SetNumber(5);										//���ã�����ģʽ
	m_cList_Station.SetNumber(6);										//���ã�����ģʽ
	m_cList_Station.SetNumber(7);										//���ã�����ģʽ
	m_cList_Station.SetNumber(9);										//���ã�����ģʽ
	m_cList_Station.SetNumber(10);										//���ã�����ģʽ
	m_cList_Station.SetNumber(11);										//���ã�����ģʽ
	m_cList_Station.SetNumber(12);										//���ã�����ģʽ
	m_cList_Station.SetNumber(13);										//���ã�����ģʽ
	m_cList_Station.SetNumber(14);										//���ã�����ģʽ
	m_cList_Station.SetNumber(15);										//���ã�����ģʽ

	m_cList_Station.SetNumber(17);										//���ã�����ģʽ
	m_cList_Station.SetNumber(18);										//���ã�����ģʽ
	
	v_iCol_Lon=17;														//�����к�
	v_iCol_Lat=18;														//γ���к�

	//��ʼ�����б��� [��վ]
	::SendMessage(m_cList_BTS.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_BTS.InsertColumn(0,"���",LVCFMT_CENTER,60);				//
	m_cList_BTS.InsertColumn(1,"����",LVCFMT_CENTER,50);				//
	m_cList_BTS.InsertColumn(2,"վַ-��վ",LVCFMT_LEFT,240);			//
	m_cList_BTS.InsertColumn(3,"վַ-С��",LVCFMT_LEFT,240);			//
	m_cList_BTS.InsertColumn(4,"��ͬ",LVCFMT_LEFT,60);					//
	m_cList_BTS.InsertColumn(5,"��վ",LVCFMT_LEFT,240);					//
	m_cList_BTS.InsertColumn(6,"С��",LVCFMT_LEFT,240);					//
	m_cList_BTS.SetExtendedStyle(m_cList_BTS.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

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
		&m_cList_Station,		// Top pane
		&m_cList_BTS,			// Bottom pane
		IDC_QUA_Station,		// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);

	//��ѯ����
	m_cKey_Select.AddString("����/��Դ��");
	m_cKey_Select.AddString("����");
	m_cKey_Select.AddString("��ַ");
	m_cKey_Select.SetCurSel(0);											//Ĭ�ϣ�ѡ���һ��

	PostMessage(WM_SIZE);												//���

	v_iList_Item_Room=-1;												//��Ŀѡ����Ч ��������
	m_cBTS.SetCheck(true);												//��λ���ں���վ

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CQUA_Station::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ�����ר�ô����/����û���
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
void CQUA_Station::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: �ڴ˴�������Ϣ�����������
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//�����Ӵ�����
	if(v_pIterator_Frame != theApp.v_mFrame.end())						//�Ӵ����ڣ�
		theApp.v_mFrame.erase(v_pIterator_Frame);						//������������Ӵ�����
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����������С
//------------------------------------------------------------------------------------------------------
void CQUA_Station::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴�������Ϣ�����������
	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//�ִ��ߴ����
		CRect v_cRect;
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=45;
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�վַ����
//------------------------------------------------------------------------------------------------------
void CQUA_Station::OnBnClickedExcel2()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	if(v_iList_Item_Room>=0)											//��Ч��Ŀ��ţ�����
	{
		m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,255,255));	//�ָ�����ɫ
		v_iList_Item_Room=-1;											//��Ŀѡ����Ч ��������
	}

	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Station);										//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���վ����
//------------------------------------------------------------------------------------------------------
void CQUA_Station::OnBnClickedButton7()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_BTS);											//����Excel
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б������� [����][�����б�]
//------------------------------------------------------------------------------------------------------
void CQUA_Station::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	if(v_iList_Item_Room>=0)											//��Ч��Ŀ��ţ�����
	{
		m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,255,255));	//�ָ�����ɫ
		v_iList_Item_Room=-1;											//��Ŀѡ����Ч ��������
	}

	m_cList_Station.My_Sort_Virtual(pNMHDR);							//�б�����[�����б�]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������б���ʾ���ݻ�ȡ
//	��	  ���������б���Ҫ��Ԫ�ز���(�С���)�������������в�ѯ���ݣ���ֵ���б��С�
//------------------------------------------------------------------------------------------------------
void CQUA_Station::OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ����ӿؼ�֪ͨ�����������

	m_cList_Station.Get_List_Data(pDispInfo);							//�����б���ʾ���ݻ�ȡ

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����б��� [����]
//------------------------------------------------------------------------------------------------------
void CQUA_Station::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	CString		v_sRoom,v_sCovet_Type;									//��ʱ����
	double		v_dRoom_Lon,v_dRoom_Lat;								//��ʱ����

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item_Room>=0)									//��Ч��Ŀ��ţ�����
				m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,255,255));	//�ָ�����ɫ
			v_iList_Item_Room=pNMListView->iItem;						//��ã�ѡ����Ŀ���
			m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,0,0));			//���ñ���ɫ����ɫ
			
			My_Clear();													//�������
			v_sRoom=m_cList_Station.GetItemText(v_iList_Item_Room,1);	//��ȡ������
			v_sCovet_Type=m_cList_Station.GetItemText(v_iList_Item_Room,4);			//��ȡ������
			v_dRoom_Lon=atof(m_cList_Station.GetItemText(v_iList_Item_Room,v_iCol_Lon));	//��ȡ������
			v_dRoom_Lat=atof(m_cList_Station.GetItemText(v_iList_Item_Room,v_iCol_Lat));	//��ȡ��γ��
			My_LoadData_Tree(v_sRoom,v_sCovet_Type,v_dRoom_Lon,v_dRoom_Lat);				//װ�ػ�����Ϣ
			My_Show_Tree(v_sRoom);										//��ʾ����վ��С����Ϣ
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ
//------------------------------------------------------------------------------------------------------
void CQUA_Station::OnBnClickedQuary()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CQUA_Station::My_Thread_Inoput(LPVOID lparam)
{
	CQUA_Station *  lp_this = NULL ;
	lp_this = (CQUA_Station *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel_NodeB.EnableWindow(false);						//��������Ч
	lp_this->m_cMap_Get.EnableWindow(false);							//��ͼ��ȡ����Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel_NodeB.EnableWindow(true);							//��������Ч
	lp_this->m_cMap_Get.EnableWindow(true);								//��ͼ��ȡ����Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CQUA_Station::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_Query_Station();												//վַ��ѯ
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ������Ϣ
//------------------------------------------------------------------------------------------------------
void CQUA_Station::My_Query_Station()
{
	int				v_iID=0;											//KPI����
	CString			v_sSql,v_sTemp;										//��ʱ����
	CString			v_sLog_Key;
	bool			v_bOK=false;										//

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//��ѯ����׼��
	m_cKey.GetWindowText(v_sLog_Key);									//��ã���ѯKEY
	v_sLog_Key.Replace("[","[[]");										//���ƴ���
	v_sLog_Key.Replace("'","");											//���ƴ���
	v_sLog_Key.Trim();													//ȥ���ո�

	v_sSql += "SELECT * FROM CON_Station"; 

	if (!v_sLog_Key.IsEmpty())											//��ѯKEY����
	{
		v_bOK=true;														//
		switch (m_cKey_Select.GetCurSel())								//ѡ�񣿣���
		{
		case 0:															//վַ
			v_sSql+=" where zStation like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//����
			v_sSql+=" where zRegion like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//��ַ
			v_sSql+=" where zAddress like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	if (m_cBTS.GetCheck())
	{
		if (v_bOK)
			v_sSql+=" and zOK='1'";
		else
			v_sSql+=" where zOK='1'";
	}
	v_sSql+=" order by zStation";

	My_LoadData_Station_Data(v_sSql);									//��ȡ��վַ��Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��վַ��Ϣ
//------------------------------------------------------------------------------------------------------			
void CQUA_Station::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iID=0;											//��ʱ����
	int				v_iLoc_BTS,v_iLoc_NodeB,v_iLoc_E_NodeB;				//��ʱ����
	int				v_iRem_BTS,v_iRem_NodeB,v_iRem_E_NodeB,v_iRemote;	//��ʱ����
	int				v_iSla_BTS,v_iSla_NodeB,v_iSla_E_NodeB,v_iSlave;	//��ʱ����
	int				v_iStation;											//
	CString			v_sTemp,v_sCover_Type,v_sRemote;					//��ʱ����
	bool			v_bBTS=true;										//��ʱ����
	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����

	_variant_t	Var;
	CString			v_sStation,v_sType,v_sRegion,v_sAddress,v_sLongitude;		//��ʱ����
	CString			v_sLatitude,v_sOnline,v_sDate,v_sCurrentTime;		//��ʱ����
	double			v_dValue;											//��ʱ����

	//�����б�
	mInt_String		v_mLine_Data;										//�����б�������
	m_cList_Station.Put_Line_Begin();									//�б��������ݿ�ʼ

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList_Station.DeleteAllItems();								//���Items
		v_iList_Item_Room=-1;											//��Чѡ��
		m_cList_BTS.DeleteAllItems();									//�������

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_sCover_Type = v_cAdoConn.GetField_String("zCover_Type");	//zCover_Type
			v_sRemote = v_cAdoConn.GetField_String("zRemote");			//zRemote
			v_iLoc_BTS = v_cAdoConn.GetField_Int("zLoc_BTS");			//zLoc_BTS
			v_iLoc_NodeB = v_cAdoConn.GetField_Int("zLoc_NodeB");		//zLoc_NodeB
			v_iLoc_E_NodeB = v_cAdoConn.GetField_Int("zLoc_E_NodeB");	//zLoc_E_NodeB
			v_iRem_BTS = v_cAdoConn.GetField_Int("zRem_BTS");			//zRem_BTS
			v_iRem_NodeB = v_cAdoConn.GetField_Int("zRem_NodeB");		//zRem_NodeB
			v_iRem_E_NodeB = v_cAdoConn.GetField_Int("zRem_E_NodeB");	//zRem_E_NodeB
			v_iSla_BTS = v_cAdoConn.GetField_Int("zSla_BTS");			//zSla_BTS
			v_iSla_NodeB = v_cAdoConn.GetField_Int("zSla_NodeB");		//zSla_NodeB
			v_iSla_E_NodeB = v_cAdoConn.GetField_Int("zSla_E_NodeB");	//zSla_E_NodeB
			v_iRemote = v_cAdoConn.GetField_Int("zRem_Sum");			//zRem_Sum
			v_iSlave = v_cAdoConn.GetField_Int("zSla_Sum");				//zSla_Sum
			v_dValue = v_cAdoConn.GetField_Double("zLongitude");		//zLongitude
			v_sLongitude.Format("%10.6f",v_dValue);
			v_dValue = v_cAdoConn.GetField_Double("zLatitude");			//zLatitude
			v_sLatitude.Format("%10.6f",v_dValue);
			v_sAddress = v_cAdoConn.GetField_String("zAddress");		//zAddress
			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			Var=v_pRecordset->GetCollect((_variant_t)"zDate");			//zDate
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sDate = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sDate="";
			Var=v_pRecordset->GetCollect((_variant_t)"zDate_Update");	//��������
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sCurrentTime = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sCurrentTime="";

			//���ݸ�λ
			v_mLine_Data.clear();										//�������
			//��ʾ����ֵ
			v_iStation=v_iLoc_BTS+v_iLoc_NodeB+v_iLoc_E_NodeB;			//��վBTS����
			v_sTemp.Format("%06d",v_iID+1);
			v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));			//���
			v_mLine_Data.insert(pair<int,CString>(1,v_sStation));		//����/��Դ������
			v_mLine_Data.insert(pair<int,CString>(2,v_sRegion));		//����
			v_mLine_Data.insert(pair<int,CString>(3,v_sRemote));		//վ��[��վ����Զ]
			v_mLine_Data.insert(pair<int,CString>(4,v_sCover_Type));	//����
			if (v_iLoc_BTS!=0)
				v_sTemp.Format("%d",v_iLoc_BTS);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(5,v_sTemp));			//��_BTS����
			if (v_iLoc_NodeB!=0)
				v_sTemp.Format("%d",v_iLoc_NodeB);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(6,v_sTemp));			//��_NodeB����
			if (v_iLoc_E_NodeB!=0)
				v_sTemp.Format("%d",v_iLoc_E_NodeB);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(7,v_sTemp));			//��_E_NodeB����
			if (v_iRemote!=0)
				v_sTemp.Format("%d",v_iRemote);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(8,v_sTemp));			//��Զվ����
			if (v_iRem_BTS!=0)
				v_sTemp.Format("%d",v_iRem_BTS);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(9,v_sTemp));			//��_BTS����
			if (v_iRem_NodeB!=0)
				v_sTemp.Format("%d",v_iRem_NodeB);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(10,v_sTemp));			//��_NodeB����
			if (v_iRem_E_NodeB!=0)
				v_sTemp.Format("%d",v_iRem_E_NodeB);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(11,v_sTemp));			//��_E_NodeB����
			if (v_iSlave!=0)
				v_sTemp.Format("%d",v_iSlave);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(12,v_sTemp));			//��վ����
			if (v_iSla_BTS!=0)
				v_sTemp.Format("%d",v_iSla_BTS);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(13,v_sTemp));			//��_BTS����
			if (v_iSla_NodeB!=0)
				v_sTemp.Format("%d",v_iSla_NodeB);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(14,v_sTemp));			//��_NodeB����
			if (v_iSla_E_NodeB!=0)
				v_sTemp.Format("%d",v_iSla_E_NodeB);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(15,v_sTemp));			//��_E_NodeB����
			v_mLine_Data.insert(pair<int,CString>(16,v_sOnline));		//״̬
			v_mLine_Data.insert(pair<int,CString>(17,v_sLongitude));	//����
			v_mLine_Data.insert(pair<int,CString>(18,v_sLatitude));		//γ��
			v_mLine_Data.insert(pair<int,CString>(19,v_sType));			//����
			v_mLine_Data.insert(pair<int,CString>(20,v_sAddress));		//��ַ
			v_mLine_Data.insert(pair<int,CString>(21,v_sDate));			//��������
			v_mLine_Data.insert(pair<int,CString>(22,v_sCurrentTime));	//��������

			//���ӣ������б�
			m_cList_Station.Put_Line_Data(v_mLine_Data);				//�б�����������

			v_iID++;													//�������澯����

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

	//�����б�
	m_cList_Station.Put_Line_End();										//�б��������ݽ���
	v_mLine_Data.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ�ػ�����Ϣ
//	��	  �������ȣ���ȡ��վ(��վ)����Σ���ȡ��Զ(С��)��
//------------------------------------------------------------------------------------------------------			
void CQUA_Station::My_LoadData_Tree(CString v_sRoom,CString v_sStation_Cover,double v_dLongitude,double v_dLatitude)
{
	CString		v_sSql,v_sTemp,v_sRNC,v_sCover;							//��ʱ����
	CString		v_sBTS,v_sBTS_Name,v_sNodeB_Name,v_sStation;			//��ʱ����
	CString		v_sCell,v_sStation_BTS;									//��ʱ����

	Tree_Room_Struct	v_cTree_Room;									//�ṹ
	Tree_BTS_Struct		v_cTree_BTS;									//�ṹ

	mTree_Room::iterator	v_pRoom;									//������

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��վ����
		//��1��GSM��BTS��ȡ
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT zBTS,zBTS_NAME,zCover_Type FROM GSM_BTS where zStation='"+v_sRoom+"' order by zBTS"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sBTS = v_cAdoConn.GetField_String("zBTS");				//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sCover= v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type

			v_pRoom=v_mTree_Room.find(v_sRoom);							//��ѯ������
			if (v_pRoom==v_mTree_Room.end())							//û�ҵ�������
			{
				v_cTree_Room.v_iType=0;									//��վ
				v_cTree_Room.v_sCover_Type=v_sStation_Cover;			//��������
				v_cTree_Room.v_dLongitude=v_dLongitude;					//����
				v_cTree_Room.v_dLatitude=v_dLatitude;					//γ��
				v_mTree_Room.insert(pair<CString,Tree_Room_Struct>(v_sRoom,v_cTree_Room));	//�������ӣ�����
				v_pRoom=v_mTree_Room.find(v_sRoom);						//��ѯ������
			}
			v_cTree_BTS.v_bBreak=false;									//��־������
			v_cTree_BTS.v_sBTS_Name=v_sBTS_Name;						//��ȡ��������
			v_cTree_BTS.v_sCover=v_sCover;								//��ȡ����������
			My_LoadData_Tree_Cell(v_sRoom,v_sBTS,0,&v_cTree_BTS);		//��ȡ��С����Ϣ��BTS��

			v_pRecordset->MoveNext();									//
		}

		//��2��TD_LTE��NodeB��ȡ
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT zNODEB,zRNC,zCover_Type FROM LTE_NODEB where zStation='"+v_sRoom+"' order by zNODEB"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sNodeB_Name = v_cAdoConn.GetField_String("zNODEB");		//zNODEB
			v_sRNC= v_cAdoConn.GetField_String("zRNC");					//zRNC
			v_sCover= v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type

			v_pRoom=v_mTree_Room.find(v_sRoom);							//��ѯ������
			if (v_pRoom==v_mTree_Room.end())							//û�ҵ�������
			{
				v_cTree_Room.v_iType=0;									//��վ
				v_cTree_Room.v_sCover_Type=v_sCover;					//��������
				v_cTree_Room.v_dLongitude=v_dLongitude;					//����
				v_cTree_Room.v_dLatitude=v_dLatitude;					//γ��
				v_mTree_Room.insert(pair<CString,Tree_Room_Struct>(v_sRoom,v_cTree_Room));	//�������ӣ�����
				v_pRoom=v_mTree_Room.find(v_sRoom);						//��ѯ������
			}
			v_cTree_BTS.v_bBreak=false;									//��־������
			v_cTree_BTS.v_sCover=v_sCover;								//��ȡ����������
			My_LoadData_Tree_Cell(v_sRoom,v_sNodeB_Name,1,&v_cTree_BTS);//��ȡ��С����Ϣ��NodeB��

			v_pRecordset->MoveNext();									//
		}

		//��3��GSM GT��BTS��ȡ(����)
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT zBTS,zBSC,zCover_Type FROM GT_BTS where zStation='"+v_sRoom+"' order by zBTS"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sBTS = v_cAdoConn.GetField_String("zBTS");				//zBTS
			v_sCover= v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type

			v_pRoom=v_mTree_Room.find(v_sRoom);							//��ѯ������
			if (v_pRoom==v_mTree_Room.end())							//û�ҵ�������
			{
				v_cTree_Room.v_iType=0;									//��վ
				v_cTree_Room.v_sCover_Type=v_sCover;					//��������
				v_cTree_Room.v_dLongitude=v_dLongitude;					//����
				v_cTree_Room.v_dLatitude=v_dLatitude;					//γ��
				v_mTree_Room.insert(pair<CString,Tree_Room_Struct>(v_sRoom,v_cTree_Room));	//�������ӣ�����
				v_pRoom=v_mTree_Room.find(v_sRoom);						//��ѯ������
			}
			v_cTree_BTS.v_bBreak=false;									//��־������
			v_cTree_BTS.v_sCover=v_sCover;								//��ȡ����������
			My_LoadData_Tree_Cell(v_sRoom,v_sBTS,2,&v_cTree_BTS);		//��ȡ��С����Ϣ��GT BTS��

			v_pRecordset->MoveNext();									//
		}

		//��Զվ����(�����Ǻ�վ)
		//��4��GSM Cell
		My_LoadData_Tree_Cell_Remote(v_sRoom,0);						//��ȡ��С����Ϣ��GSM Cell��
		//��5��LD-LTE Cell
		My_LoadData_Tree_Cell_Remote(v_sRoom,1);						//��ȡ��С����Ϣ��LTE Cell��
		//��6��GSM GT Cell
		My_LoadData_Tree_Cell_Remote(v_sRoom,2);						//��ȡ��С����Ϣ��GSM GT Cell��

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
//	�������ܣ���ȡС����Ϣ��BTS\LTE\GT��
//	��	  ����v_sRoom��������	v_sBTS����վ���ƣ�	v_iType��0��GSM��1��LTE��2��GSM GT��
//------------------------------------------------------------------------------------------------------			
void CQUA_Station::My_LoadData_Tree_Cell(CString v_sRoom,CString v_sBTS,int v_iType,Tree_BTS_Struct* v_cTree_BTS_Data)
{
	CString		v_sSql,v_sTemp;											//��ʱ����
	CString		v_sCell,v_sStation="",v_sCover;							//��ʱ����
	double		v_dLongitude,v_dLatitude;								//��ʱ����

	Tree_Room_Struct		v_cTree_Room;								//�ṹ
	Tree_BTS_Struct			v_cTree_BTS;								//�ṹ
	Tree_Cell_Struct		v_cTree_Cell;								//�ṹ

	mTree_Room::iterator	v_pRoom;									//������
	mTree_BTS::iterator		v_pBTS;										//������

	//��1����λ��������BTS
	v_pRoom=v_mTree_Room.find(v_sRoom);									//��ѯ����վ����
	if (v_pRoom==v_mTree_Room.end())									//û�ҵ�������
		return;															//��Ч����										

	_variant_t	Var;

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��2����ȡ��SQL
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		if (v_iType==0)													//GSM
			v_sSql += "SELECT zCell,GSM_Cell.zStation,CON_Station.zCover_Type,CON_Station.zLongitude,CON_Station.zLatitude FROM GSM_Cell \
					  left join CON_Station on GSM_Cell.zStation=CON_Station.zStation where zBTS='"+v_sBTS+"'";
		else if (v_iType==1)											//LTE
			v_sSql += "SELECT zCell,LTE_Cell.zStation,CON_Station.zCover_Type,CON_Station.zLongitude,CON_Station.zLatitude FROM LTE_Cell \
					  left join CON_Station on LTE_Cell.zStation=CON_Station.zStation where zNODEB='"+v_sBTS+"'"; 
		else															//GSM GT
			v_sSql += "SELECT zCell,GT_Cell.zStation,CON_Station.zCover_Type,zLongitude,zLatitude FROM GT_Cell \
					  left join CON_Station on GT_Cell.zStation=CON_Station.zStation where zBTS='"+v_sBTS+"'"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			//��3����ȡ��С����Ϣ
			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sCover = v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type
			v_dLongitude = v_cAdoConn.GetField_Double("zLongitude");	//zLongitude
			v_dLatitude = v_cAdoConn.GetField_Double("zLatitude");		//zLatitude
			v_sStation.Trim();											//ȥ���ո�

			if (v_sStation==v_sRoom || v_sStation.IsEmpty())
			{
				//��4����վ����
				if (v_sStation==v_sRoom)								//��վ������
					v_sStation="��վ";									//С����վ==����
				v_pRoom->second.v_mTree_BTS.insert(pair<CString,Tree_BTS_Struct>(v_sBTS,*v_cTree_BTS_Data));	//��������
				v_pBTS=v_pRoom->second.v_mTree_BTS.find(v_sBTS);		//���ң�BTS
			}
			else
			{
				//��4����Զ��������
				v_pRoom=v_mTree_Room.find(v_sStation);					//��ѯ����Զ
				if (v_pRoom==v_mTree_Room.end())						//û�ҵ�������
				{
					v_cTree_Room.v_iType=1;								//��Զ����
					v_cTree_Room.v_sCover_Type=v_sCover;				//��������
					v_cTree_Room.v_dLongitude=v_dLongitude;				//����
					v_cTree_Room.v_dLatitude=v_dLatitude;				//γ��
					v_mTree_Room.insert(pair<CString,Tree_Room_Struct>(v_sStation,v_cTree_Room));	//�������ӣ���Զ
					v_pRoom=v_mTree_Room.find(v_sStation);				//��ѯ������
				}
				v_pBTS=v_pRoom->second.v_mTree_BTS.find(v_sBTS);		//���ң�BTS
				if (v_pBTS==v_pRoom->second.v_mTree_BTS.end())			//û�ҵ�������
				{
					v_cTree_BTS.v_bBreak=false;							//��־������
					v_cTree_BTS.v_sCover=v_cTree_BTS_Data->v_sCover;	//��ȡ����������
					v_cTree_BTS.v_sBTS_Name=v_cTree_BTS_Data->v_sBTS_Name;	//��ȡ����ά
					v_pRoom->second.v_mTree_BTS.insert(pair<CString,Tree_BTS_Struct>(v_sBTS,v_cTree_BTS));	//�������ӣ���վ
					v_pBTS=v_pRoom->second.v_mTree_BTS.find(v_sBTS);	//���ң�BTS
				}
			}
			//��5�����ӣ�С����Ϣ
			v_cTree_Cell.v_bStop=false;									//����
			v_cTree_Cell.v_sStation=v_sStation;							//����
			v_pBTS->second.v_mTree_Cell.insert(pair<CString,Tree_Cell_Struct>(v_sCell,v_cTree_Cell));		//�������ӣ�С��

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
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡС����Ϣ_��Զ��BTS\LTE\GT��
//	��	  ����v_sRoom��������	v_iType��0��GSM��1��LTE��2��GSM GT��
//------------------------------------------------------------------------------------------------------			
void CQUA_Station::My_LoadData_Tree_Cell_Remote(CString v_sRoom,int v_iType)
{
	CString		v_sSql,v_sTemp,v_sStation_BTS,v_sCover_Type="",v_sCover_Type_Cell;		//��ʱ����
	CString		v_sCell,v_sStation="",v_sCover,v_sBTS;					//��ʱ����
	double		v_dLongitude,v_dLatitude;								//��ʱ����
	double		v_dLongitude_BTS=0,v_dLatitude_BTS=0;					//��ʱ����

	Tree_Room_Struct		v_cTree_Room;								//�ṹ
	Tree_BTS_Struct			v_cTree_BTS;								//�ṹ
	Tree_Cell_Struct		v_cTree_Cell;								//�ṹ

	mTree_Room::iterator	v_pRoom;									//������
	mTree_BTS::iterator		v_pBTS;										//������

	_variant_t	Var;

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��2����ȡ��SQL
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		if (v_iType==0)													//GSM
			v_sSql += "SELECT zCell,zBTS,GSM_Cell.zStation,CON_Station.zCover_Type,CON_Station.zLongitude,CON_Station.zLatitude FROM GSM_Cell \
					  left join CON_Station on GSM_Cell.zStation=CON_Station.zStation where GSM_Cell.zStation='"+v_sRoom+"'";
		else if (v_iType==1)											//LTE
			v_sSql += "SELECT zCell,zNODEB as zBTS,LTE_Cell.zStation,CON_Station.zCover_Type,CON_Station.zLongitude,CON_Station.zLatitude FROM LTE_Cell \
					  left join CON_Station on LTE_Cell.zStation=CON_Station.zStation where LTE_Cell.zStation='"+v_sRoom+"'"; 
		else															//GSM GT
			v_sSql += "SELECT zCell,zBTS,GT_Cell.zStation,CON_Station.zCover_Type,zLongitude,zLatitude FROM GT_Cell \
					  left join CON_Station on GT_Cell.zStation=CON_Station.zStation where GT_Cell.zStation='"+v_sRoom+"'"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			//��3����ȡ��С����Ϣ
			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sBTS = v_cAdoConn.GetField_String("zBTS");				//zBTS
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sCover_Type_Cell = v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type
			v_dLongitude = v_cAdoConn.GetField_Double("zLongitude");	//zLongitude
			v_dLatitude = v_cAdoConn.GetField_Double("zLatitude");		//zLatitude
			v_sStation.Trim();											//ȥ���ո�

			v_sStation_BTS=My_Get_Station_BTS(v_sBTS,v_iType,&v_cTree_BTS,&v_sCover_Type,&v_dLongitude_BTS,&v_dLatitude_BTS);		//��ȡ��BTS����
			if (v_sStation_BTS!=v_sStation && !v_sStation_BTS.IsEmpty())		//BTS������С��������ͬ������
			{
				//��4����Զ��������
				v_pRoom=v_mTree_Room.find(v_sStation_BTS);				//��ѯ����Զ
				if (v_pRoom==v_mTree_Room.end())						//û�ҵ�������
				{
					v_cTree_Room.v_iType=2;								//��վ(��Զ)����
					v_cTree_Room.v_sRoom_Cell=v_sRoom;					//����(С��)
					v_cTree_Room.v_sCover_Type_Cell=v_sCover_Type_Cell;	//��������(С��)
					v_cTree_Room.v_dLongitude_Cell=v_dLongitude;		//����(С��)
					v_cTree_Room.v_dLatitude_Cell=v_dLatitude;			//γ��(С��)
					v_cTree_Room.v_dLongitude=v_dLongitude_BTS;			//����_BTS(��վ)
					v_cTree_Room.v_dLatitude=v_dLatitude_BTS;			//γ��_BTS(��վ)
					v_cTree_Room.v_sCover_Type=v_sCover_Type;			//��������_BTS(��վ)
					v_mTree_Room.insert(pair<CString,Tree_Room_Struct>(v_sStation_BTS,v_cTree_Room));		//�������ӣ���Զ
					v_pRoom=v_mTree_Room.find(v_sStation_BTS);			//��ѯ������
				}
				v_pBTS=v_pRoom->second.v_mTree_BTS.find(v_sBTS);		//���ң�BTS
				if (v_pBTS==v_pRoom->second.v_mTree_BTS.end())			//û�ҵ�������
				{
					v_pRoom->second.v_mTree_BTS.insert(pair<CString,Tree_BTS_Struct>(v_sBTS,v_cTree_BTS));	//�������ӣ���վ
					v_pBTS=v_pRoom->second.v_mTree_BTS.find(v_sBTS);	//���ң�BTS
				}

				//��5�����ӣ�С����Ϣ
				v_cTree_Cell.v_bStop=false;									//����
				v_cTree_Cell.v_sStation=v_sStation;							//����
				v_pBTS->second.v_mTree_Cell.insert(pair<CString,Tree_Cell_Struct>(v_sCell,v_cTree_Cell));	//�������ӣ�С��
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
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ��վ������BTS\LTE\GT��
//	��	  ����v_sBTS����վ���ƣ�	v_iType��0��GSM��1��LTE��2��GSM GT��
//------------------------------------------------------------------------------------------------------			
CString CQUA_Station::My_Get_Station_BTS(CString v_sBTS,int v_iType,Tree_BTS_Struct* v_cTree_BTS,CString* v_sCover_Type,double* v_dLongitude,double* v_dLatitude)
{
	CString		v_sSql,v_sTemp;											//��ʱ����
	CString		v_sStation="",v_sCover;									//��ʱ����

	_variant_t	Var;

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��1����ȡ��SQL
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		if (v_iType==0)													//GSM
			v_sSql += "SELECT zBTS_NAME,GSM_BTS.zCover_Type,GSM_BTS.zStation,CON_Station.zLongitude,CON_Station.zLatitude \
					  ,CON_Station.zCover_Type as zCover_Station FROM GSM_BTS \
					  left join CON_Station on GSM_BTS.zStation=CON_Station.zStation where zBTS='"+v_sBTS+"'"; 
		else if (v_iType==1)											//LTE
			v_sSql += "SELECT LTE_NODEB.zCover_Type,LTE_NODEB.zStation,CON_Station.zLongitude,CON_Station.zLatitude \
					  ,CON_Station.zCover_Type as zCover_Station FROM LTE_NODEB \
					  left join CON_Station on LTE_NODEB.zStation=CON_Station.zStation where zNODEB='"+v_sBTS+"'"; 
		else															//GSM GT
			v_sSql += "SELECT GT_BTS.zCover_Type,GT_BTS.zStation,CON_Station.zLongitude,CON_Station.zLatitude \
					  ,CON_Station.zCover_Type as zCover_Station FROM GT_BTS \
					  left join CON_Station on GT_BTS.zStation=CON_Station.zStation where zBTS='"+v_sBTS+"'"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			//��2����ȡ��BTS��Ϣ
			if (v_iType==0)												//GSM BTS
				v_cTree_BTS->v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");	//zBTS_NAME
			else
				v_cTree_BTS->v_sBTS_Name = "";							//
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			*v_sCover_Type = v_cAdoConn.GetField_String("zCover_Station");	//zCover_Station
			*v_dLongitude = v_cAdoConn.GetField_Double("zLongitude");	//zLongitude
			*v_dLatitude = v_cAdoConn.GetField_Double("zLatitude");		//zLatitude
			v_cTree_BTS->v_bBreak=false;								//
			v_cTree_BTS->v_sCover=v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type
			v_sStation.Trim();											//ȥ���ո�

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
	return	v_sStation;													//����
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ʾ������Ϣ
//------------------------------------------------------------------------------------------------------			
void CQUA_Station::My_Show_Tree(CString v_sRoom)
{
	CString			v_sTemp,v_sRoom_BTS,v_sBTS,v_sCover;				//��ʱ����
	bool			v_bOK=false;										//��ʱ����

	mTree_Room::iterator	v_pRoom;									//������
	mTree_BTS::iterator		v_pBTS;										//������
	mTree_Cell::iterator	v_pCell;									//������

	//��0������λ
	m_cList_BTS.DeleteAllItems();										//�������
	if (v_mTree_Room.size()==0)											//�л�����Ϣ������
		return;															//���أ��޻���

	//��1����վ����(��վ)
	for (v_pRoom=v_mTree_Room.begin();v_pRoom!=v_mTree_Room.end();v_pRoom++)	//����
	{
		if(v_pRoom->second.v_iType==0)									//��վ������
		{
			//(1)��վ����Ŀ¼
			v_bOK=true;													//��λ
			v_sRoom_BTS=v_pRoom->first;									//��������վ

			//(3)��ʾ����վ��Ϣ��״̬
			v_pBTS=v_pRoom->second.v_mTree_BTS.begin();					//ͷָ��
			while(v_pBTS!=v_pRoom->second.v_mTree_BTS.end())			//����
			{
				//(4)���ӣ���վ��Ϣ
				if (v_pBTS->first.GetLength()==6)						//GSM ???
					v_sBTS=v_pBTS->first+"_"+v_pBTS->second.v_sBTS_Name;	//BTS+������
				else
					v_sBTS=v_pBTS->first;								//BTS
				v_sCover=v_pBTS->second.v_sCover;						//����

				//(5)���ӣ�С����Ϣ
				v_pCell=v_pBTS->second.v_mTree_Cell.begin();			//ͷָ��
				while(v_pCell!=v_pBTS->second.v_mTree_Cell.end())		//����
				{
					My_ShowList_BTS(v_sCover,v_sRoom_BTS,v_pCell->second.v_sStation,"��ͬ",v_sBTS,v_pCell->first);	//�б���ʾ
					v_pCell++;											//ָ��++
				}
				v_pBTS++;												//ָ��++
			}
		}
	}

	//��2����վ����(��Զ)
	if (v_bOK)															//��վ������
	{
		for (v_pRoom=v_mTree_Room.begin();v_pRoom!=v_mTree_Room.end();v_pRoom++)	//����
		{
			if(v_pRoom->second.v_iType==1)								//��Զ������
			{
				//��ʾ������Ϣ��״̬
				for(v_pBTS=v_pRoom->second.v_mTree_BTS.begin();v_pBTS!=v_pRoom->second.v_mTree_BTS.end();v_pBTS++)	//����
				{
					if (v_pBTS->first.GetLength()==6)					//GSM ???
						v_sBTS=v_pBTS->first+"_"+v_pBTS->second.v_sBTS_Name;	//BTS+������
					else
						v_sBTS=v_pBTS->first;							//BTS
					v_sCover=v_pBTS->second.v_sCover;					//����

					//���ӣ�С����Ϣ
					v_pCell=v_pBTS->second.v_mTree_Cell.begin();		//ͷָ��
					while(v_pCell!=v_pBTS->second.v_mTree_Cell.end())	//����
					{
						My_ShowList_BTS(v_sCover,v_sRoom_BTS,v_pCell->second.v_sStation,"",v_sBTS,v_pCell->first);	//�б���ʾ
						v_pCell++;										//ָ��++
					}
				}
			}
		}
	}

	//��3����վ����(��������Զ)
	for (v_pRoom=v_mTree_Room.begin();v_pRoom!=v_mTree_Room.end();v_pRoom++)	//����
	{
		if(v_pRoom->second.v_iType==2)									//��վ������
		{
			//(1)��վ����Ŀ¼
			v_sRoom_BTS=v_pRoom->first;									//�������Ӹ��ڵ�

			//(2)��ʾ����վ��Ϣ��״̬
			v_pBTS=v_pRoom->second.v_mTree_BTS.begin();					//ͷָ��
			while(v_pBTS!=v_pRoom->second.v_mTree_BTS.end())			//����
			{
				//(3)���ӣ���վ��Ϣ
				if (v_pBTS->first.GetLength()==6)						//GSM ???
					v_sBTS=v_pBTS->first+"_"+v_pBTS->second.v_sBTS_Name;//BTS+������
				else
					v_sBTS=v_pBTS->first;								//BTS
				v_sCover=v_pBTS->second.v_sCover;						//����

				//(4)���ӣ�С����Ϣ
				v_pCell=v_pBTS->second.v_mTree_Cell.begin();			//ͷָ��
				while(v_pCell!=v_pBTS->second.v_mTree_Cell.end())		//����
				{
					v_sTemp="";
					if (v_bOK)
						v_sTemp=(v_sRoom==v_pCell->second.v_sStation)?"��վ":"";	//
					My_ShowList_BTS(v_sCover,v_sRoom_BTS,v_pCell->second.v_sStation,v_sTemp,v_sBTS,v_pCell->first);	//�б���ʾ
					v_pCell++;											//ָ��++
				}
				v_pBTS++;												//ָ��++
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������
//------------------------------------------------------------------------------------------------------
void CQUA_Station::My_Clear()
{
	mTree_Room::iterator	v_pRoom;									//������
	mTree_BTS::iterator		v_pBTS;										//������
	for(v_pRoom=v_mTree_Room.begin();v_pRoom!=v_mTree_Room.end();v_pRoom++)	//����
	{
		for(v_pBTS=v_pRoom->second.v_mTree_BTS.begin();v_pBTS!=v_pRoom->second.v_mTree_BTS.end();v_pBTS++)	//����
			v_pBTS->second.v_mTree_Cell.clear();						//�������
		v_pRoom->second.v_mTree_BTS.clear();							//�������
	}
	v_mTree_Room.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б���������һ��ͨ����Ϣ
//	��    ����v_tpTimespan����ʱ��	v_sType�����ͣ�	v_sLog��Log��Ϣ��
//------------------------------------------------------------------------------------------------------
void CQUA_Station::My_ShowList_BTS(CString v_s1,CString v_s2,CString v_s3,CString v_s4,CString v_s5,CString v_s6)
{
	int		vi,v_iIndex;												//��ʱ����
	CString v_sTemp;													//��ʱ����
	CString v_sCurTime;													//��ʱ����
	CTime	v_CurTime=CTime::GetCurrentTime();

	v_sCurTime=v_CurTime.Format("%y.%m.%d_%H:%M:%S");
	v_iIndex = m_cList_BTS.InsertItem(0xffff,_T("0"));
	v_sTemp.Format("%04d",v_iIndex+1);
	m_cList_BTS.SetItemText(v_iIndex,0," "+v_sTemp);					//���
	m_cList_BTS.SetItemText(v_iIndex,1,v_s1);							//
	m_cList_BTS.SetItemText(v_iIndex,2,v_s2);							//
	m_cList_BTS.SetItemText(v_iIndex,3,v_s3);							//
	m_cList_BTS.SetItemText(v_iIndex,4,v_s4);							//
	m_cList_BTS.SetItemText(v_iIndex,5,v_s5);							//
	m_cList_BTS.SetItemText(v_iIndex,6,v_s6);							//

	//�б����������ƣ�����1000����ɾ��300����
	m_cList_BTS.SetRedraw(FALSE);										//�����½���
	if (m_cList_BTS.GetItemCount()>=1000)								//�����¼������1000
	{
		for(vi=0;vi<300;vi++)											//ɾ��ǰ300��
			m_cList_BTS.DeleteItem(0);									//ɾ����һ�����ݼ�¼
		for(vi=0;vi<m_cList_BTS.GetItemCount();vi++)					//���·������
		{
			v_sTemp.Format("%04d",vi+1);
			m_cList_BTS.SetItemText(vi,0," "+v_sTemp);
		}
	}
	m_cList_BTS.SetRedraw(true);										//���½���
	m_cList_BTS.EnsureVisible(m_cList_BTS.GetItemCount()-1,false);		//��ʾ����һ����¼
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------