// QUA_Element_GSM.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "QUA_Element_GSM.h"


// CQUA_Element_GSM

IMPLEMENT_DYNCREATE(CQUA_Element_GSM, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CQUA_Element_GSM::CQUA_Element_GSM()
	: CFormView(CQUA_Element_GSM::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CQUA_Element_GSM::~CQUA_Element_GSM()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_NodeB);
	DDX_Control(pDX, IDC_SUM_ALARM2, m_cSum_NodeB);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_CHECK1, m_cBTS);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CQUA_Element_GSM, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CQUA_Element_GSM::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL, &CQUA_Element_GSM::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CQUA_Element_GSM::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_EXCEL2, &CQUA_Element_GSM::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CQUA_Element_GSM::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_CHECK1, &CQUA_Element_GSM::OnBnClickedCheck1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CQUA_Element_GSM::OnNMClickList1)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CQUA_Element_GSM::OnLvnGetdispinfoList1)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST3, &CQUA_Element_GSM::OnLvnGetdispinfoList3)
END_MESSAGE_MAP()


// CQUA_Element_GSM ���

#ifdef _DEBUG
void CQUA_Element_GSM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQUA_Element_GSM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQUA_Element_GSM ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="GSM��Ԫ��ѯ";										//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б� [BTS]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"BTS",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(2,"������",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(3,"BSC",LVCFMT_LEFT,70);						//
	m_cList.InsertColumn(4,"����",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(5,"��������/��Դ��",LVCFMT_LEFT,200);			//
	m_cList.InsertColumn(6,"Ӳ��",LVCFMT_LEFT,100);						//
	m_cList.InsertColumn(7,"���",LVCFMT_LEFT,100);						//
	m_cList.InsertColumn(8,"վ��",LVCFMT_LEFT,100);						//
	m_cList.InsertColumn(9,"VVIP",LVCFMT_LEFT,60);						//
	m_cList.InsertColumn(10,"״̬",LVCFMT_LEFT,60);						//
	m_cList.InsertColumn(11,"��������",LVCFMT_LEFT,60);					//
	m_cList.InsertColumn(12,"����ʱ��[ʵ��]",LVCFMT_CENTER,140);		//
	m_cList.InsertColumn(13,"����ʱ��[�ʹ�]",LVCFMT_CENTER,100);		//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��ʼ�����б� [Cell]
	::SendMessage(m_cList_NodeB.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_NodeB.InsertColumn(0,"���",LVCFMT_CENTER,60);				//
	m_cList_NodeB.InsertColumn(1,"BTS",LVCFMT_LEFT,60);
	m_cList_NodeB.InsertColumn(2,"С �� ��",LVCFMT_LEFT,60);
	m_cList_NodeB.InsertColumn(3,"��    ��    ��",LVCFMT_LEFT,140);
	m_cList_NodeB.InsertColumn(4,"��    ��",LVCFMT_LEFT,80);
	m_cList_NodeB.InsertColumn(5,"��������/��Դ��",LVCFMT_LEFT,200);	//
	m_cList_NodeB.InsertColumn(6,"״̬",LVCFMT_LEFT,60);				//
	m_cList_NodeB.InsertColumn(7,"��������",LVCFMT_LEFT,60);			//
	m_cList_NodeB.InsertColumn(8,"��������[ʵ��]",LVCFMT_CENTER,120);
	m_cList_NodeB.InsertColumn(9,"��������[�ʹ�]",LVCFMT_CENTER,120);
	m_cList_NodeB.SetExtendedStyle(m_cList_NodeB.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

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
		&m_cList,				// Top pane
		&m_cList_NodeB,			// Bottom pane
		IDC_QUA_Element_GSM,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);

	PostMessage(WM_SIZE);												//���

	//��Ϣ��ʼ��
	m_cBTS.SetCheck(true);												//վ�㣺��Ч
	OnBnClickedCheck1();												//��ѯ����

	v_iList_Item=-1;													//��Ŀѡ����Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CQUA_Element_GSM::PreTranslateMessage(MSG* pMsg)
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
void CQUA_Element_GSM::OnDestroy()
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
void CQUA_Element_GSM::OnSize(UINT nType, int cx, int cy)
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
//	�������ܣ�BTS����
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Cell����
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_NodeB);										//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б����� [BTS]
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort_Virtual(pNMHDR);									//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б����� [Cell]
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
void CQUA_Element_GSM::OnLvnGetdispinfoList1(NMHDR *pNMHDR, LRESULT *pResult)
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
void CQUA_Element_GSM::OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_cList_NodeB.Get_List_Data(pDispInfo);								//�����б���ʾ���ݻ�ȡ

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�BTSѡ��
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cKey_Select.ResetContent();										//���ѡ������
	if (m_cBTS.GetCheck())												//BTSѡ�У�����
	{																	//ѡ��
		m_SplitterPane_Main.ShowTopPane();								//��ʾ��BTS
		m_cKey_Select.AddString("BTS");
		m_cKey_Select.AddString("������");
		m_cKey_Select.AddString("BSC");
		m_cKey_Select.AddString("����/��Դ��");
	}
	else
	{																	//ûѡ��
		m_SplitterPane_Main.HideTopPane();								//���أ�BTS
		m_cKey_Select.AddString("Cell");
		m_cKey_Select.AddString("BTS");
		m_cKey_Select.AddString("������");
		m_cKey_Select.AddString("����/��Դ��");
	}
	m_cKey_Select.SetCurSel(0);											//Ĭ�ϣ�ѡ���һ��
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����б� [BTS]
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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

			v_iWorking=3;												//��������վ������Ч��Ӧ

			v_pIterator_Frame->second.v_iThread_Exit=1;					//�̣߳�����
			v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);		//������ѯ���ݿ��߳�
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//��������С����ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����	[��̬���޼���]
//------------------------------------------------------------------------------------------------------
UINT CQUA_Element_GSM::My_Thread_Inoput(LPVOID lparam)
{
	CQUA_Element_GSM *  lp_this = NULL ;
	lp_this = (CQUA_Element_GSM *)lparam;

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
void CQUA_Element_GSM::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		if (m_cBTS.GetCheck())											//BSCѡ�У�����
			My_Query_BTS();												//BTS��ѯ
		else
			My_Query_Cell();											//Cell��ѯ
		break;
	case 3:
		My_Query_BTS_Cell();											//BTS�б�����Ӧ
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�BTS��ѯ
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::My_Query_BTS()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int				v_iID=0;											//KPI����
	CString			v_sSql,v_sTemp;										//��ʱ����
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//��ѯ����׼��
	m_cKey.GetWindowText(v_sLog_Key);									//��ã���ѯKEY
	v_sLog_Key.Replace("[","[[]");										//���ƴ���
	v_sLog_Key.Replace("'","");											//���ƴ���
	v_sLog_Key.Trim();													//ȥ���ո�

	v_sSql += "SELECT * FROM GSM_BTS";

	if (!v_sLog_Key.IsEmpty())											//��ѯKEY����
	{
		switch (m_cKey_Select.GetCurSel())								//ѡ�񣿣���
		{
		case 0:															//BTS
			v_sSql+=" where zBTS like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//������
			v_sSql+=" where zBTS_Name like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//BSC
			v_sSql+=" where zBSC like '%"+v_sLog_Key+"%'";
			break;
		case 3:															//��������/��Դ��
			v_sSql+=" where zStation like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	v_sSql+=" order by zBTS";

	My_LoadData_BTS_Data(v_sSql);										//��ȡ��BTS��Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��BTS��Ϣ
//------------------------------------------------------------------------------------------------------			
void CQUA_Element_GSM::My_LoadData_BTS_Data(CString v_sSql)
{
	int				v_iID=0;											//KPI����
	CString			v_sTemp,v_sDate_Creat;								//��ʱ����
	CString			v_sBTS,v_sBTS_Name,v_sBSC,v_sRegion,v_sStation,v_sHardware,v_sOper,v_sTel;		//��ʱ����
	CString			v_sSoftware,v_sVVIP,v_sRF,v_sAgency,v_sOnline,v_sDate,v_sCurrentTime,v_sDevice;	//��ʱ����
	CString			v_sMaintence,v_sCover_Type;							//��ʱ����
	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����

	//�����б�
	mInt_String		v_mLine_Data;										//�����б�������
	m_cList.Put_Line_Begin();											//�б�������ݿ�ʼ

	_variant_t	Var;
	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		m_cList.DeleteAllItems();										//�б�λ
		v_iList_Item=-1;												//��Ŀѡ����Ч
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sBTS = v_cAdoConn.GetField_String("zBTS");				//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sBSC = v_cAdoConn.GetField_String("zBSC");				//zBSC
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sHardware = v_cAdoConn.GetField_String("zHardware");		//zHardware
			v_sSoftware = v_cAdoConn.GetField_String("zBTS_Version");	//zBTS_Version
			v_sDevice = v_cAdoConn.GetField_String("zDevice");			//zDevice
			v_sVVIP = v_cAdoConn.GetField_String("zVVIP");				//zVVIP
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_sCover_Type = v_cAdoConn.GetField_String("zCover_Type");	//zCover_Type
			v_dDate_Start = v_cAdoConn.GetField_DataTime("zDate");		//zDate
			if (v_dDate_Start!=theApp.a_tData_Default)					//������Ч������
				v_sDate = v_dDate_Start.Format(_T("%Y-%m-%d"));
			else
				v_sDate="";												//��Ч����
			v_dDate_Start = v_cAdoConn.GetField_DataTime("zDate_Update");	//zDate_Update
			if (v_dDate_Start!=theApp.a_tData_Default)					//�������ڣ�����
				v_sCurrentTime = v_dDate_Start.Format(_T("%Y-%m-%d"));
			else
				v_sCurrentTime="";										//��Ч����

			//���ݸ�λ
			v_mLine_Data.clear();										//�������

			//��ʾ
			v_sTemp.Format(" %04d",v_iID+1);
			v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));			//���
			v_mLine_Data.insert(pair<int,CString>(1,v_sBTS));			//BTS
			v_mLine_Data.insert(pair<int,CString>(2,v_sBTS_Name));		//������
			v_mLine_Data.insert(pair<int,CString>(3,v_sBSC));			//BSC
			v_mLine_Data.insert(pair<int,CString>(4,v_sRegion));		//����
			v_mLine_Data.insert(pair<int,CString>(5,v_sStation));		//��������/��Դ��
			v_mLine_Data.insert(pair<int,CString>(6,v_sHardware));		//Ӳ��
			v_mLine_Data.insert(pair<int,CString>(7,v_sSoftware));		//���
			v_mLine_Data.insert(pair<int,CString>(8,v_sDevice));		//վ��
			v_mLine_Data.insert(pair<int,CString>(9,v_sVVIP));			//VVIP
			v_mLine_Data.insert(pair<int,CString>(10,v_sOnline));		//״̬
			v_mLine_Data.insert(pair<int,CString>(11,v_sCover_Type));	//��������
			v_mLine_Data.insert(pair<int,CString>(12,v_sDate));			//��������
			v_mLine_Data.insert(pair<int,CString>(13,v_sCurrentTime));	//��������

			v_mLine_Data.insert(pair<int,CString>(17,v_sDate_Creat));	//��������

			//��ӣ������б�
			m_cList.Put_Line_Data(v_mLine_Data);						//�б����������

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
	m_cList.Put_Line_End();												//�б�������ݽ���
	v_mLine_Data.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯCell��Ϣ
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::My_Query_Cell()
{
	int				v_iID=0;											//KPI����
	CString			v_sSql,v_sTemp;										//��ʱ����
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//��ѯ����׼��
	m_cKey.GetWindowText(v_sLog_Key);									//��ã���ѯKEY
	v_sLog_Key.Replace("[","[[]");										//���ƴ���
	v_sLog_Key.Replace("'","");											//���ƴ���
	v_sLog_Key.Trim();													//ȥ���ո�

	v_sSql += "SELECT GSM_Cell.*, GSM_BTS.zBTS_NAME, GSM_BTS.zRegion as zRegion_BTS FROM GSM_Cell \
			  left JOIN GSM_BTS ON GSM_Cell.zBTS = GSM_BTS.zBTS";
	if (!v_sLog_Key.IsEmpty())											//��ѯKEY����
	{
		switch (m_cKey_Select.GetCurSel())								//ѡ�񣿣���
		{
		case 0:															//Cell
			v_sSql+=" where zCell like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//BTS
			v_sSql+=" where zBTS like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//������
			v_sSql+=" where zBTS_NAME like '%"+v_sLog_Key+"%'";
			break;
		case 3:															//��������/��Դ��
			v_sSql+=" where GSM_BTS.zStation like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	v_sSql+=" order by zCell";

	My_LoadData_Cell_Data(v_sSql);										//��ȡ��Cell��Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ�� BTS_Cell ��Ϣ
//------------------------------------------------------------------------------------------------------			
void CQUA_Element_GSM::My_Query_BTS_Cell()
{
	CString v_sBTS=m_cList.GetItemText(v_iList_Item,1);					//v_sBTS
	CString v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT GSM_Cell.*, GSM_BTS.zBTS_NAME, GSM_BTS.zRegion as zRegion_BTS FROM GSM_Cell \
			  left JOIN GSM_BTS ON GSM_Cell.zBTS = GSM_BTS.zBTS";
	v_sSql+=" where GSM_Cell.zBTS='"+v_sBTS+"' order by zCell";			//SQL
	My_LoadData_Cell_Data(v_sSql);										//ִ��SQL
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��Cell��Ϣ
//------------------------------------------------------------------------------------------------------			
void CQUA_Element_GSM::My_LoadData_Cell_Data(CString v_sSql)
{
	int				v_iID=0;											//KPI����
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����

	_variant_t	Var;
	CString		v_sCover,v_sBTS,v_sBTS_Name,v_sCell,v_sRegion,v_sStation;	//��ʱ����
	CString		v_sAgency,v_sOnline,v_sDate,v_sCurrentTime,v_sStie_Type;//
	CString		v_sEquip,v_sCGI,v_sTG,v_sTrx,v_sDip,v_sDip_Num,v_sTrx_Type;	//��ʱ����
	CString		v_sMaintianer,v_sTel,v_sDXU,v_sCDU,v_sDate_Update,v_sOper;

	//�����б�
	mInt_String		v_mLine_Data;										//�����б�������
	m_cList_NodeB.Put_Line_Begin();										//�б�������ݿ�ʼ

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList_NodeB.DeleteAllItems();									//���Items

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sBTS = v_cAdoConn.GetField_String("zBTS");				//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sRegion = v_cAdoConn.GetField_String("zRegion_BTS");		//zRegion_BTS
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_sCover = v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type

			v_dDate_Start = v_cAdoConn.GetField_DataTime("zDate");		//zDate ����ʱ��[ʵ��]
			if (v_dDate_Start!=theApp.a_tData_Default)					//�������ڣ�����
				v_sDate = v_dDate_Start.Format(_T("%Y-%m-%d"));
			else
				v_sDate="";												//��Ч����
			v_dDate_Start = v_cAdoConn.GetField_DataTime("zDate_Update");	//zDate_Update ����ʱ��[�ʹ�]
			if (v_dDate_Start!=theApp.a_tData_Default)					//�������ڣ�����
				v_sDate_Update = v_dDate_Start.Format(_T("%Y-%m-%d"));
			else
				v_sDate_Update="";										//��Ч����

			//���ݸ�λ
			v_mLine_Data.clear();										//�������

			//��ʾ
			v_sTemp.Format("%06d",v_iID+1);
			v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));			//���
			v_mLine_Data.insert(pair<int,CString>(1,v_sBTS));			//BTS
			v_mLine_Data.insert(pair<int,CString>(2,v_sCell));			//Cell
			v_mLine_Data.insert(pair<int,CString>(3,v_sBTS_Name));		//������
			v_mLine_Data.insert(pair<int,CString>(4,v_sRegion));		//����
			v_mLine_Data.insert(pair<int,CString>(5,v_sStation));		//��������/��Դ��
			v_mLine_Data.insert(pair<int,CString>(6,v_sOnline));		//״̬
			v_mLine_Data.insert(pair<int,CString>(7,v_sCover));			//��������
			v_mLine_Data.insert(pair<int,CString>(8,v_sDate));			//��������[ʵ��]
			v_mLine_Data.insert(pair<int,CString>(9,v_sDate_Update));	//��������[�ʹ�]

			//��ӣ������б�
			m_cList_NodeB.Put_Line_Data(v_mLine_Data);					//�б����������

			v_pRecordset->MoveNext();									//
			v_iID++;													//�������澯����

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
	m_cList_NodeB.Put_Line_End();										//�б�������ݽ���
	v_mLine_Data.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
