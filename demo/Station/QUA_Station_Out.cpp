// QUA_Station_Out.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "QUA_Station_Out.h"

#include "comdef.h" 

// CQUA_Station_Out

IMPLEMENT_DYNCREATE(CQUA_Station_Out, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CQUA_Station_Out::CQUA_Station_Out()
	: CFormView(CQUA_Station_Out::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CQUA_Station_Out::~CQUA_Station_Out()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_Station);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_TREE1, m_cTree_Station);
	DDX_Control(pDX, IDC_CHECK1, m_cOnline);
	DDX_Control(pDX, IDC_CHECK3, m_cOK);
	DDX_Control(pDX, IDC_BUTTON2, m_cDetail);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CQUA_Station_Out, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CQUA_Station_Out::OnBnClickedQuary)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CQUA_Station_Out::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CQUA_Station_Out::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_EXCEL, &CQUA_Station_Out::OnBnClickedExcel)
	ON_BN_CLICKED(IDC_BUTTON2, &CQUA_Station_Out::OnBnClickedButton2)
END_MESSAGE_MAP()


// CQUA_Station_Out ���

#ifdef _DEBUG
void CQUA_Station_Out::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQUA_Station_Out::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQUA_Station_Out ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="���⹲ַ��ѯ";										//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б� [վַ]
	::SendMessage(m_cList_Station.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Station.InsertColumn(0,"���",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(1,"����/��Դ������",LVCFMT_LEFT,260);	//
	m_cList_Station.InsertColumn(2,"����",LVCFMT_LEFT,70);				//
	m_cList_Station.InsertColumn(3,"BTS",LVCFMT_RIGHT,60);				//
	m_cList_Station.InsertColumn(4,"Cell",LVCFMT_RIGHT,60);				//
	m_cList_Station.InsertColumn(5,"NodeB",LVCFMT_RIGHT,60);			//
	m_cList_Station.InsertColumn(6,"Cell",LVCFMT_RIGHT,60);				//
	m_cList_Station.InsertColumn(7,"E_NodeB",LVCFMT_RIGHT,60);			//
	m_cList_Station.InsertColumn(8,"Cell",LVCFMT_RIGHT,60);				//
	m_cList_Station.InsertColumn(9,"����",LVCFMT_RIGHT,80);				//
	m_cList_Station.InsertColumn(10,"γ��",LVCFMT_RIGHT,80);			//
	m_cList_Station.SetExtendedStyle(m_cList_Station.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList_Station.SetNumber(3);										//���ֱȽ�
	m_cList_Station.SetNumber(4);
	m_cList_Station.SetNumber(5);
	m_cList_Station.SetNumber(6);
	m_cList_Station.SetNumber(7);
	m_cList_Station.SetNumber(8);
	m_cList_Station.SetNumber(9);
	m_cList_Station.SetNumber(10);

	//����ʼ����m_cTree
	v_cImage_Tree.Create(IDB_TREE_STATION,16,1,RGB(255,255,255));		//����ͼ���б�����
	//	v_cImage_State.Create(IDB_TREE_CHECK,13, 1, RGB(255,255,255));
	m_cTree_Station.SetImageList(&v_cImage_Tree,TVSIL_NORMAL);			//����ʼ��
	m_cTree_Station.SetImageList(&v_cImage_State,TVSIL_STATE);
	m_cTree_Station.SetBkColor(RGB(200,200,255));						//�������οؼ��ı���ɫ
	m_cTree_Station.SetTextColor(RGB(127,0,0));							//�����ı�����ɫ

	//�ִ�
	CRect	v_cRect;													//����
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//��ã�ָ��
	pWnd->GetWindowRect(&v_cRect);										//��ã��ߴ�
	ScreenToClient(&v_cRect);											//����任
	pWnd->DestroyWindow();												//���٣�ָ��

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Main.Create(											//��ֱ��
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_cList_Station,		// Top pane
		&m_cTree_Station,		// Bottom pane
		IDC_QUA_Station_Out_Tree,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);
	m_SplitterPane_Bottom.m_bVertSplitter_Parent=true;					//��ֱ�ָ���Ϊ����

	PostMessage(WM_SIZE);												//���

	//��Ϣ��ʼ��
	m_cKey_Select.AddString("����/��Դ��");
	m_cKey_Select.AddString("����");
	m_cKey_Select.SetCurSel(0);											//Ĭ�ϣ�ѡ���һ��

//	m_cOnline.SetCheck(1);												//������Ч
	m_cOK.SetCheck(1);													//��Чվַ

	v_iList_Item_Room=-1;												//��Ŀѡ����Ч ��������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CQUA_Station_Out::PreTranslateMessage(MSG* pMsg)
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
void CQUA_Station_Out::OnDestroy()
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
void CQUA_Station_Out::OnSize(UINT nType, int cx, int cy)
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
//	�������ܣ�ͳ�Ƶ���
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show_Name(&m_cList_Station,"���⹲ַ_ͳ��",true);			//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϸ����
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=2;														//����������ϸ����

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б����� [վַ]
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList_Station.My_Sort(pNMHDR);									//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����б� [վַ]
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item_Room>=0)									//��Ч��Ŀ��ţ�����
				m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,255,255));	//�ָ�����ɫ
			v_iList_Item_Room=pNMListView->iItem;						//��ã�ѡ����Ŀ���
			m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,0,0));			//���ñ���ɫ����ɫ
			
			My_LoadData_Tree(m_cList_Station.GetItemText(v_iList_Item_Room,1));		//װ�ػ�����Ϣ ������Tree��
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ��վַ��
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����	[��̬���޼���]
//------------------------------------------------------------------------------------------------------
UINT CQUA_Station_Out::My_Thread_Inoput(LPVOID lparam)
{
	CQUA_Station_Out *  lp_this = NULL ;
	lp_this = (CQUA_Station_Out *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(false);								//��������Ч
	lp_this->m_cDetail.EnableWindow(false);								//��������Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(true);								//��������Ч
	lp_this->m_cDetail.EnableWindow(true);								//��������Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_Query_Station();												//վַ��ѯ
		break;
	case 2:
		My_OutPut();													//��ϸ����
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ������Ϣ
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::My_Query_Station()
{
	int				v_iID=0;											//KPI����
	CString			v_sSql,v_sTemp;										//��ʱ����
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//��ѯ����׼��
	m_cKey.GetWindowText(v_sLog_Key);									//��ã���ѯKEY
	v_sLog_Key.Trim();													//ȥ���ո�

	v_sSql += "SELECT * FROM CON_Station"; 

	if (!v_sLog_Key.IsEmpty())											//��ѯKEY����
	{
		switch (m_cKey_Select.GetCurSel())								//ѡ�񣿣���
		{
		case 0:															//վַ
			v_sSql+=" where zStation like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//����
			v_sSql+=" where zRegion like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	v_sSql+=" order by zStation";

	My_LoadData_Station_Data(v_sSql);									//��ȡ��վַ��Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��վַ��Ϣ
//------------------------------------------------------------------------------------------------------			
void CQUA_Station_Out::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem,v_iID=0;									//KPI����
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����

	_variant_t	Var;
	CString			v_sStation,v_sRegion,v_sLongitude;					//��ʱ����
	CString			v_sLatitude,v_sOnline;								//��ʱ����
	double			v_dValue;											//��ʱ����
	int				v_iGSM_BTS,v_iGSM_Cell,v_iTD_NodeB,v_iTD_Cell,v_iLTE_NodeB,v_iLTE_Cell,v_iSum;	//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		//����λ
		m_cTree_Station.DeleteAllItems();								//�����
		//�б�λ
		m_cList_Station.DeleteAllItems();								//���Items
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
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_dValue = v_cAdoConn.GetField_Double("zLongitude");		//zLongitude
			v_sLongitude.Format("%10.6f",v_dValue);
			v_dValue = v_cAdoConn.GetField_Double("zLatitude");			//zLatitude
			v_sLatitude.Format("%10.6f",v_dValue);
			v_iGSM_BTS = v_cAdoConn.GetField_Int("zGSM_BTS_OUT");		//zGSM_BTS_OUT
			v_iGSM_Cell = v_cAdoConn.GetField_Int("zGSM_Cell_OUT");		//zGSM_Cell_OUT
			v_iTD_NodeB = v_cAdoConn.GetField_Int("zTD_NodeB_OUT");		//zTD_NodeB_OUT
			v_iTD_Cell = v_cAdoConn.GetField_Int("zTD_Cell_OUT");		//zTD_Cell_OUT
			v_iLTE_NodeB = v_cAdoConn.GetField_Int("zLTE_NodeB_OUT");	//zLTE_NodeB_OUT
			v_iLTE_Cell = v_cAdoConn.GetField_Int("zLTE_Cell_OUT");		//zLTE_Cell_OUT

			//��ʾ
			if (!m_cOnline.GetCheck() || (m_cOnline.GetCheck() && v_sOnline == "����"))
			{
				v_iSum=v_iGSM_BTS+v_iTD_NodeB+v_iLTE_NodeB;				//��վ�ܼ�
				if (!m_cOK.GetCheck() || (m_cOK.GetCheck() && v_iSum != 0))
				{
					v_sTemp.Format(" %04d",v_iID+1);
					v_iItem=m_cList_Station.InsertItem(0xffff,v_sTemp);	//�����б�
					m_cList_Station.SetItemText(v_iItem,1,v_sStation);	//����/��Դ������
					m_cList_Station.SetItemText(v_iItem,2,v_sRegion);	//����
					v_sTemp.Format("%d",v_iGSM_BTS);
					m_cList_Station.SetItemText(v_iItem,3,v_sTemp);		//GSM_BTS
					v_sTemp.Format("%d",v_iGSM_Cell);
					m_cList_Station.SetItemText(v_iItem,4,v_sTemp);		//GSM_Cell
					v_sTemp.Format("%d",v_iTD_NodeB);
					m_cList_Station.SetItemText(v_iItem,5,v_sTemp);		//TD_NodeB
					v_sTemp.Format("%d",v_iTD_Cell);
					m_cList_Station.SetItemText(v_iItem,6,v_sTemp);		//TD_Cell
					v_sTemp.Format("%d",v_iLTE_NodeB);
					m_cList_Station.SetItemText(v_iItem,7,v_sTemp);		//LTE_NodeB
					v_sTemp.Format("%d",v_iLTE_Cell);
					m_cList_Station.SetItemText(v_iItem,8,v_sTemp);		//LTE_Cell
					m_cList_Station.SetItemText(v_iItem,9,v_sLongitude);//����
					m_cList_Station.SetItemText(v_iItem,10,v_sLatitude);//γ��

					v_iID++;											//�������澯����
				}
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
void CQUA_Station_Out::My_LoadData_Tree(CString v_sRoom)
{
	CString		v_sSql,v_sTemp;											//��ʱ����
	CString		v_sBTS,v_sBTS_Name,v_sNodeB_Name,v_sSation,v_sCell,v_sSation_Cell;	//��ʱ����
	bool		v_bRoot=false;											//��ʱ����

	mStation_OutPut_BTS				v_mNodeb;							//Nodeb/BTS ��Tree��¼��value=1��ΪBTS;value=2��ΪNodeb;��
	mStation_OutPut_BTS::iterator	v_pNodeb;							//������
	Station_OutPut_BTS_Struct		v_cNodeb;							//�ṹ
	mString_String::iterator		v_pCell;							//������

		//����λ
	m_cTree_Station.DeleteAllItems();									//�����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//С����ַ��ѯ ����ѯС��վַΪv_sRoom��Ӧ�Ļ�վ��Ϣ����������v_mNodeb��
		//��GSMС����
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select GSM_Cell.zCell,GSM_Cell.zBTS as zBTS_BTS,GSM_Cell.zStation as zStation_Cell, \
				  GSM_BTS.zBTS_NAME,GSM_BTS.zStation as zStation_BTS \
				  FROM GSM_Cell left join GSM_BTS on GSM_Cell.zBTS=GSM_BTS.zBTS \
				  where GSM_BTS.zType='����' and GSM_Cell.zStation='"+v_sRoom+"' order by GSM_BTS.zStation,GSM_Cell.zCell"; 
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
				  LTE_NODEB.zNODEB_Name,LTE_NODEB.zStation as zStation_BTS \
				  FROM LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB \
				  where LTE_NODEB.zType='����' and LTE_Cell.zStation='"+v_sRoom+"' order by LTE_NODEB.zStation,LTE_Cell.zCell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zNODEB
			v_sBTS_Name = v_cAdoConn.GetField_String("zNODEB_Name");	//zNODEB_Name
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell

			v_pNodeb=v_mNodeb.find(v_sBTS);								//���ң�BTS
			if (v_pNodeb==v_mNodeb.end())
			{
				//���ӣ�E_NodeB
				v_cNodeb.v_sName=v_sBTS_Name;							//������
				v_cNodeb.v_sNetwork="3/4";								//�������ͣ�3/4G
				v_cNodeb.v_sStation=v_sSation;							//վַ
				v_mNodeb.insert(pair<CString,Station_OutPut_BTS_Struct>(v_sBTS,v_cNodeb));	//��������
			}
			v_pNodeb=v_mNodeb.find(v_sBTS);								//���ң�BTS
			//���ӣ�Cell
			v_pNodeb->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sSation_Cell));	//��������

			v_pRecordset->MoveNext();									//
		}

		//���������ӵĻ�վ������Tree��Ϣ ������v_mNodeb��ValueΪ1��
		v_hBTS_Root=m_cTree_Station.InsertItem(v_sRoom,6,7,TVI_ROOT);	//�������Ӹ��ڵ� [����/��Դ��]		
		for (v_pNodeb=v_mNodeb.begin();v_pNodeb!=v_mNodeb.end();v_pNodeb++)		//����������
		{
			if(v_pNodeb->second.v_sNetwork=="2")						//BTS
			{
				//GSM��BSC��ȡ
				v_hBTS_Node=m_cTree_Station.InsertItem(v_pNodeb->first+"_"+v_pNodeb->second.v_sName
					+"     ��"+v_pNodeb->second.v_sStation+"��",4,5,v_hBTS_Root);	//���������ӽڵ� [BTS]
				for (v_pCell=v_pNodeb->second.v_mCell.begin();v_pCell!=v_pNodeb->second.v_mCell.end();v_pCell++)	//����������
					m_cTree_Station.InsertItem(v_pCell->first+"     ��"+v_pCell->second+"��",0,1,v_hBTS_Node);		//���������ӽڵ� [Cell]
			}
			else														//Nodeb
			{
				//TD_LTE��Nodeb��ȡ
				v_sTemp=v_pNodeb->first;								//NodeB
				if (v_pNodeb->first.Find("-EL")>=0)						//������4G ������
					v_sTemp+="_"+v_pNodeb->second.v_sName;				//+������
				v_hBTS_Node=m_cTree_Station.InsertItem(v_sTemp+"     ��"
					+v_pNodeb->second.v_sStation+"��",4,5,v_hBTS_Root);				//���������ӽڵ� [NodeB]
				for (v_pCell=v_pNodeb->second.v_mCell.begin();v_pCell!=v_pNodeb->second.v_mCell.end();v_pCell++)	//����������
					m_cTree_Station.InsertItem(v_pCell->first+"     ��"+v_pCell->second+"��",0,1,v_hBTS_Node);		//���������ӽڵ� [Cell]
			}
		}

		//��չ��
		v_hBTS_Node=m_cTree_Station.GetRootItem();
		while (v_hBTS_Node!=NULL)
		{
			m_cTree_Station.ExpandTree(v_hBTS_Node);					//����չ��
			v_hBTS_Node=m_cTree_Station.GetNextSiblingItem(v_hBTS_Node);//��ã��ھӽڵ�
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

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϸ����
//------------------------------------------------------------------------------------------------------			
void CQUA_Station_Out::My_OutPut()
{
	CString		v_sSql,v_sTemp,v_sRegion,v_sRNC,v_sOnline;				//��ʱ����
	CString		v_sBTS,v_sCell,v_sBTS_Name,v_sNodeB_Name,v_sSation,v_sSation_Cell;

	//վַ����
	mStation_OutPut				v_mStation_OutPut;						//վַ����
	mStation_OutPut::iterator	v_pStation_OutPut;						//������
	Station_OutPut_Struct		v_cStation_OutPut;						//�ṹ
	mStation_OutPut_BTS::iterator	v_pmStation_OutPut_BTS;				//������
	Station_OutPut_BTS_Struct	v_cStation_OutPut_BTS;					//�ṹ

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��GSM��
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select GSM_Cell.zCell,GSM_Cell.zBTS as zBTS_BTS,GSM_Cell.zStation as zStation_Cell, \
				  GSM_BTS.zBTS_NAME,GSM_BTS.zStation as zStation_BTS,GSM_BTS.zRegion,GSM_Cell.zOnline as zOnline_Cell \
				  FROM GSM_Cell left join GSM_BTS on GSM_Cell.zBTS=GSM_BTS.zBTS \
				  where GSM_BTS.zType='����' order by GSM_BTS.zStation,GSM_Cell.zCell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell
			v_sOnline = v_cAdoConn.GetField_String("zOnline_Cell");		//zOnline_Cell

			v_pStation_OutPut=v_mStation_OutPut.find(v_sSation_Cell);	//���ң�վַ
			if (v_pStation_OutPut==v_mStation_OutPut.end())
			{
				//���ӣ�վַ
				v_cStation_OutPut.v_sRegion=v_sRegion;					//����
				v_cStation_OutPut.v_bNet_GSM=1;							//��ַ����
				v_cStation_OutPut.v_bNet_TD=0;							//��ַ����
				v_cStation_OutPut.v_bNet_LTE=0;							//��ַ����
				v_mStation_OutPut.insert(pair<CString,Station_OutPut_Struct>(v_sSation_Cell,v_cStation_OutPut));	//��������
			}
			v_pStation_OutPut=v_mStation_OutPut.find(v_sSation_Cell);	//���ң�վַ
			v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mGSM.find(v_sBTS);	//���ң�BTS
			if (v_pmStation_OutPut_BTS==v_pStation_OutPut->second.v_mGSM.end())
			{
				//���ӣ�BTS
				v_cStation_OutPut_BTS.v_sName=v_sBTS_Name;				//������
				v_cStation_OutPut_BTS.v_sNetwork="2";					//��������
				v_pStation_OutPut->second.v_mGSM.insert(pair<CString,Station_OutPut_BTS_Struct>(v_sBTS,v_cStation_OutPut_BTS));	//��������
			}
			v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mGSM.find(v_sBTS);	//���ң�BTS
			//���ӣ�Cell
			v_pmStation_OutPut_BTS->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sOnline));	//��������

			v_pRecordset->MoveNext();									//��һ��
		}

		//��TD-LTE��
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select LTE_Cell.zCell,LTE_Cell.zNODEB as zBTS_BTS,LTE_Cell.zStation as zStation_Cell, \
					LTE_NODEB.zStation as zStation_BTS,LTE_NODEB.zRegion,LTE_NODEB.zRNC,LTE_Cell.zOnline as zOnline_Cell \
					FROM LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB \
					where LTE_NODEB.zType='����' order by LTE_NODEB.zStation,LTE_Cell.zCell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zBTS
			v_sRNC = v_cAdoConn.GetField_String("zRNC");				//zRNC
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell
			v_sOnline = v_cAdoConn.GetField_String("zOnline_Cell");		//zOnline_Cell

			v_pStation_OutPut=v_mStation_OutPut.find(v_sSation_Cell);	//���ң�վַ
			if (v_pStation_OutPut==v_mStation_OutPut.end())
			{
				//���ӣ�վַ
				v_cStation_OutPut.v_sRegion=v_sRegion;					//����
				v_cStation_OutPut.v_bNet_GSM=0;							//��ַ����
				v_cStation_OutPut.v_bNet_TD=0;							//��ַ����
				v_cStation_OutPut.v_bNet_LTE=0;							//��ַ����
				v_mStation_OutPut.insert(pair<CString,Station_OutPut_Struct>(v_sSation_Cell,v_cStation_OutPut));	//��������
			}
			v_pStation_OutPut=v_mStation_OutPut.find(v_sSation_Cell);	//���ң�վַ
			if (v_sRNC.Find("dalian_")>=0)								//4G������
			{
				v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mLTE.find(v_sBTS);	//���ң�BTS
				if (v_pmStation_OutPut_BTS==v_pStation_OutPut->second.v_mLTE.end())
				{
					//�޸ģ���ַ����
					v_pStation_OutPut->second.v_bNet_LTE=1;				//��ַ����
					//���ӣ�E_NodeB
					v_cStation_OutPut_BTS.v_sName=v_sBTS;				//������
					v_cStation_OutPut_BTS.v_sNetwork="4";				//�������ͣ�4G
					v_pStation_OutPut->second.v_mLTE.insert(pair<CString,Station_OutPut_BTS_Struct>(v_sBTS,v_cStation_OutPut_BTS));	//��������
				}
				v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mLTE.find(v_sBTS);	//���ң�BTS
				//���ӣ�Cell
				v_pmStation_OutPut_BTS->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sOnline));	//��������
			}
			else
			{
				v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mTD.find(v_sBTS);	//���ң�BTS
				if (v_pmStation_OutPut_BTS==v_pStation_OutPut->second.v_mTD.end())
				{
					//�޸ģ���ַ����
					v_pStation_OutPut->second.v_bNet_TD=1;				//��ַ����
					//���ӣ�NodeB
					v_cStation_OutPut_BTS.v_sName=v_sBTS;				//������
					v_cStation_OutPut_BTS.v_sNetwork="3";				//�������ͣ�3G
					v_pStation_OutPut->second.v_mTD.insert(pair<CString,Station_OutPut_BTS_Struct>(v_sBTS,v_cStation_OutPut_BTS));	//��������
				}
				v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mTD.find(v_sBTS);	//���ң�BTS
				//���ӣ�Cell
				v_pmStation_OutPut_BTS->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sOnline));	//��������
			}

			v_pRecordset->MoveNext();									//
		}

		v_cAdoConn.ExitConnect();										//�Ͽ�����

		//���ɵ����ļ�
		MyOutPut_Station(&v_mStation_OutPut,"���⹲ַ_��ϸ",true);		//����Excel
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	v_mStation_OutPut.clear();											//�������
}

//-------------------------------------------------------------------------------------------------
// ����Excel����д��CSV�ļ���
// ������mStation_OutPut������ָ��
//		 v_sFile_Name��������ļ�����
//		 v_bFile_Open���ļ��򿪱�־��[1���򿪣�0�����򿪣�]
//-------------------------------------------------------------------------------------------------
void CQUA_Station_Out::MyOutPut_Station(mStation_OutPut *v_mStation_OutPut,CString v_sFile_Name,bool v_bFile_Open) 
{
	int		v_iNum1=0;													//��ʱ����
	CString	v_sStr,v_sCSV,v_sPath_Data,v_sCSV_File,v_sStation;			//��ʱ����
	CString	v_sRegion,v_sBTS,v_sBTS_Name,v_sHead,v_sNetwork;			//��ʱ����
	CString	v_sLong,v_sLan;												//��ʱ����
	CFile	v_fWriter;													//ʵ����
	CFileFind	finder;

	mStation_OutPut::iterator		v_pStation_OutPut;					//������
	mStation_OutPut_BTS::iterator	v_pmStation_OutPut_BTS;				//������
	mString_String::iterator		v_pCell;							//������

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
		//����CSV�ļ����ļ�����v_sCSV_File
		if (v_sFile_Name.IsEmpty())
			v_sCSV_File = theApp.m_sPath+"\\DATA\\Excel.csv";			//���ã�CSVĬ���ļ�����Excel.csv
		else
			v_sCSV_File = theApp.m_sPath+"\\DATA\\"+v_sFile_Name+".csv";//���ã�CSVָ�����ļ�����
		if(v_fWriter.Open(v_sCSV_File, CFile::modeCreate | CFile::modeWrite))		//���ļ����粻���������ɣ������������ļ���
		{
			//��ͷ����ȡ�б�Ŀ������ƣ�����д��CSV
			v_sCSV = "���,��������/��Դ��,����,γ��,������ʽ-վ��,BTS/NODEB/ENODEB,CELL,����,��վ����,С����,������ʽ-С����,״̬";												//�ڶ��С������С�...����n��
			v_sCSV +="\r\n";											//�س�����
			v_fWriter.Write(v_sCSV,v_sCSV.GetLength());					//д���ַ���

			//�����ݴ���ȡ�������ݣ�д��CSV
			for (int vi=0;vi<m_cList_Station.GetItemCount();vi++)
			{
				v_sStation=m_cList_Station.GetItemText(vi,1);			//��ã�վַ
				v_sLong=m_cList_Station.GetItemText(vi,9);				//��ã�����
				v_sLan=m_cList_Station.GetItemText(vi,10);				//��ã�γ��
				v_pStation_OutPut=v_mStation_OutPut->find(v_sStation);	//���ң�վַ
				if(v_pStation_OutPut==v_mStation_OutPut->end())			//û�ҵ�������
					continue;											//û�ҵ�����һ��

				v_iNum1++;												//վַ������
					
				v_sHead.Format("%d",v_iNum1);							//���
				v_sHead+=","+v_pStation_OutPut->first+",";				//վַ
				v_sHead+=v_sLong+",";									//����
				v_sHead+=v_sLan+",";									//γ��
				v_sRegion=v_pStation_OutPut->second.v_sRegion;			//����
				v_sNetwork="";											//��ַ����
				if(v_pStation_OutPut->second.v_bNet_GSM)
					v_sNetwork="2";										//��ַ����
				if(v_pStation_OutPut->second.v_bNet_TD)
				{
					if(!v_sNetwork.IsEmpty())
						v_sNetwork+="&";								//��ַ����
					v_sNetwork+="3";									//��ַ����
				}
				if(v_pStation_OutPut->second.v_bNet_LTE)
				{
					if(!v_sNetwork.IsEmpty())
						v_sNetwork+="&";								//��ַ����
					v_sNetwork+="4";									//��ַ����
				}
				//GSM BTS
				if (v_pStation_OutPut->second.v_mGSM.size()>0)			//GSM BTS
				{
					v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mGSM.begin();		//ͷ������
					while(v_pmStation_OutPut_BTS!=v_pStation_OutPut->second.v_mGSM.end())	//β������
					{
						v_sBTS=v_pmStation_OutPut_BTS->first;			//BTS
						v_sBTS_Name=v_pmStation_OutPut_BTS->second.v_sName;		//BTS_Name

						v_pCell=v_pmStation_OutPut_BTS->second.v_mCell.begin();	//ͷָ��
						while(v_pCell!=v_pmStation_OutPut_BTS->second.v_mCell.end())
						{
							//BTS��Ϣ
							v_sCSV =v_sHead+"GSM-"+v_pmStation_OutPut_BTS->second.v_sName+",";	//������ʽ-վ��
							v_sCSV +=v_pmStation_OutPut_BTS->first+",";	//BTS
							//Cell��Ϣ
							v_sCSV +=v_pCell->first+",";				//Cell
							v_sCSV +=v_sRegion+",";						//����
							v_sCSV +=v_sNetwork+",";					//��ַ����
							v_sCSV +=v_pCell->first.Right(1)+",";		//С����
							v_sCSV +="GSM-"+v_sBTS_Name+v_pCell->first.Right(1)+",";//������ʽ-С����
							v_sCSV +=v_pCell->second;					//״̬
							v_sCSV +="\r\n";							//�س�����
							v_fWriter.Write(v_sCSV,v_sCSV.GetLength());	//д���ַ���

							v_pCell++;									//������++
						}

						v_pmStation_OutPut_BTS++;						//������++
					}
				}
				//TD NodeB
				if (v_pStation_OutPut->second.v_mTD.size()>0)			//TD NodeB
				{
					v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mTD.begin();			//ͷ������
					while(v_pmStation_OutPut_BTS!=v_pStation_OutPut->second.v_mTD.end())	//β������
					{
						v_sBTS=v_pmStation_OutPut_BTS->first;			//BTS

						v_pCell=v_pmStation_OutPut_BTS->second.v_mCell.begin();	//ͷָ��
						while(v_pCell!=v_pmStation_OutPut_BTS->second.v_mCell.end())
						{
							//BTS��Ϣ
							v_sCSV =v_sHead+"TD-"+v_pmStation_OutPut_BTS->second.v_sName+",";	//������ʽ-վ��
							v_sCSV +=v_pmStation_OutPut_BTS->first+",";	//BTS
							//Cell��Ϣ
							v_sCSV +=v_pCell->first+",";				//Cell
							v_sCSV +=v_sRegion+",";						//����
							v_sCSV +=v_sNetwork+",";					//��ַ����
							v_sCSV +=v_pCell->first.Right(1)+",";		//С����
							v_sCSV +="TD-"+v_pCell->first+",";			//������ʽ-С����
							v_sCSV +=v_pCell->second;					//״̬
							v_sCSV +="\r\n";							//�س�����
							v_fWriter.Write(v_sCSV,v_sCSV.GetLength());	//д���ַ���

							v_pCell++;									//������++
						}

						v_pmStation_OutPut_BTS++;						//������++
					}
				}
				//LTE E_NodeB
				if (v_pStation_OutPut->second.v_mLTE.size()>0)			//LTE E_NodeB
				{
					v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mLTE.begin();		//ͷ������
					while(v_pmStation_OutPut_BTS!=v_pStation_OutPut->second.v_mLTE.end())	//β������
					{
						v_sBTS=v_pmStation_OutPut_BTS->first;			//BTS

						v_pCell=v_pmStation_OutPut_BTS->second.v_mCell.begin();	//ͷָ��
						while(v_pCell!=v_pmStation_OutPut_BTS->second.v_mCell.end())
						{
							//BTS��Ϣ
							v_sCSV =v_sHead+"LTE-"+v_pmStation_OutPut_BTS->second.v_sName+",";	//������ʽ-վ��
							v_sCSV +=v_pmStation_OutPut_BTS->first+",";	//BTS
							//Cell��Ϣ
							v_sCSV +=v_pCell->first+",";				//Cell
							v_sCSV +=v_sRegion+",";						//����
							v_sCSV +=v_sNetwork+",";					//��ַ����
							v_sCSV +=v_pCell->first.Right(1)+",";		//С����
							v_sCSV +="LTE-"+v_pCell->first+",";			//������ʽ-С����
							v_sCSV +=v_pCell->second;					//״̬
							v_sCSV +="\r\n";							//�س�����
							v_fWriter.Write(v_sCSV,v_sCSV.GetLength());	//д���ַ���

							v_pCell++;									//������++
						}

						v_pmStation_OutPut_BTS++;						//������++
					}
				}
			}
			
			v_fWriter.Close();											//�ļ��ر�

			//���ļ�
			if (v_bFile_Open)
				ShellExecute(NULL,"open", v_sCSV_File, NULL, NULL, SW_SHOWNORMAL);		//���ļ���Excel.csv
		}
		else
			MessageBox("��ر��Ѿ��򿪵ĵ����ļ��������µ���������");	//��ʾ��Ϣ
	}	
	catch(_com_error e)
	{
		CString v_sError = (char *)e.Description();						//��ã�������Ϣ
		MessageBox("�쳣"+v_sError);									//��ʾ��Ϣ
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡС����Ϣ��Nodeb��
//------------------------------------------------------------------------------------------------------			
void CQUA_Station_Out::My_LoadData_Tree_Node_Cell(CString v_sNodeb,HTREEITEM v_hBTS_Node)
{
	CString		v_sSql,v_sTemp;											//��ʱ����
	CString		v_sCell,v_sStation;

	_variant_t	Var;

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//GSM��BSC��ȡ
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT zCell,zStation FROM LTE_Cell where zNODEB='"+v_sNodeb+"' order by zCell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation

			m_cTree_Station.InsertItem(v_sCell+"     ��"+v_sStation+"��",0,1,v_hBTS_Node);	//���������ӽڵ� [Nodeb]

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
//	END
//------------------------------------------------------------------------------------------------------
