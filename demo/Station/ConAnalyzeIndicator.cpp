// ConAnalyzeIndicator.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "ConAnalyzeIndicator.h"
#include "WebPage.h"													//��װ����CWebPage��������HTML��JSP����
#include "comdef.h" 
#include <atlbase.h>

IMPLEMENT_DYNCREATE(CConAnalyzeIndicator, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CConAnalyzeIndicator::CConAnalyzeIndicator()
	: CFormView(CConAnalyzeIndicator::IDD)
{
	v_bShow=false;
	v_bLaod=false;														//HTML�ļ����ñ�־

	m_pIHTMLDoc		= NULL;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CConAnalyzeIndicator::~CConAnalyzeIndicator()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_Station);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_CHECK1, m_cOnline);
	DDX_Control(pDX, IDC_CHECK3, m_cOK);
	DDX_Control(pDX, IDC_EXPLORER, m_webBrowser);
	DDX_Control(pDX, IDC_BUTTON2, m_cMarker);
	DDX_Control(pDX, IDC_QUARY3, m_cDetail);
	DDX_Control(pDX, IDC_EXCEL, m_cClear);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CConAnalyzeIndicator, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CConAnalyzeIndicator::OnBnClickedQuary)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CConAnalyzeIndicator::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CConAnalyzeIndicator::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_EXCEL, &CConAnalyzeIndicator::OnBnClickedExcel)
	ON_BN_CLICKED(IDC_BUTTON2, &CConAnalyzeIndicator::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_QUARY3, &CConAnalyzeIndicator::OnBnClickedQuary3)
	ON_BN_CLICKED(IDC_EXCEL2, &CConAnalyzeIndicator::OnBnClickedExcel2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CConAnalyzeIndicator ���

#ifdef _DEBUG
void CConAnalyzeIndicator::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CConAnalyzeIndicator::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CConAnalyzeIndicator ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="����ָ��";											//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	SetTimer(TIMER1, 1000, 0);
	time_control = true;
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

	//����ʾjs����
	m_webBrowser.SetSilent(TRUE);
	m_webBrowser.Navigate("http://192.168.0.39:9002", NULL, NULL, NULL, NULL);	//����htmlҳ��
	//��ʾ��־
	PostMessage(WM_SIZE);												//���
	v_bShow=true;
	SetTimer(1,1000,NULL);												//1s��ʱ   

	//��Ϣ��ʼ��
	m_cKey_Select.AddString("����/��Դ��");
	m_cKey_Select.AddString("����");
	m_cKey_Select.SetCurSel(0);											//Ĭ�ϣ�ѡ���һ��

	m_cOK.SetCheck(1);													//��Чվַ

	v_iList_Item_Room=-1;												//��Ŀѡ����Ч ��������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CConAnalyzeIndicator::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ���ʱ��
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!v_bLaod)
	{
		v_bLaod=true;													//��λ��HTML�����־
		PostMessage(WM_SIZE);											//���
	}
	switch (nIDEvent)
	{
	case TIMER1:
		{
			//if (time_control)
			//{
			//	time_control = false;
			//	//��ҳ���Dlgָ��
			//	CComQIPtr<IHTMLDocument2> document = m_webBrowser.GetDocument();
			//	CComDispatchDriver script;
			//	document->get_Script(&script);
			//	CComVariant var(static_cast<IDispatch*>(this));
			//	script.Invoke1(L"SaveCppObject", &var);
			//}
			break;
		}
	case TIMER2:
		{
			time_control = false;
			break;
		}
	default:
		break;
	}
	CFormView::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ӵ�����
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnDestroy()
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
void CConAnalyzeIndicator::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	CWnd	*pCellList;													//��ʱ����
	int		v_iY;														//��ʱ����
	pCellList = GetDlgItem(IDC_EXPLORER);
	v_iY = 50;															//��ֵ��Y��
	if(pCellList)
	{
		CRect v_cRect;
		GetClientRect( &v_cRect );
		pCellList->MoveWindow(v_cRect);
	}	
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б����� [վַ]
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PostMessage(WM_SIZE);												//���

	m_cList_Station.My_Sort(pNMHDR);									//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Marker
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	My_Marker();
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����Marker
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWebPage	web;													//����һ��ҳ��
	web.SetDocument(m_webBrowser.GetDocument());						//��ֵ����ҳָ��
	web.CallJScript("myMarker_Clear");									//����JS�����Marker
	m_webBrowser.UpdateData();											//��ҳ����
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�վַ����
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show_Name(&m_cList_Station,"վַ_ͳ��",true);				//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����б� [վַ]
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;						//��ã���Ŀָ��
	if (pNMListView->iItem>=0)												//�жϣ���Ŀ�����Ч������
	{
		CString str;
		if (v_iList_Item_Room != -1)
		{
			m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,255,255));		//�ָ�����ɫ
		} 		
		//���ȵõ������λ��
		POSITION pos = m_cList_Station.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			return;
		}
		//�õ��кţ�ͨ��POSITIONת��
		v_iList_Item_Room = (int)m_cList_Station.GetNextSelectedItem(pos);								//��ã�ѡ����Ŀ���
		m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,0,0));			//���ñ���ɫ����ɫ
		str = m_cList_Station.GetItemText(v_iList_Item_Room, 0);
		//��ʾվַλ��
		CString content(str);
		content.Trim();
		if (!content.IsEmpty())
		{
			CWebPage	web;														//����һ��ҳ��
			web.SetDocument(m_webBrowser.GetDocument());							//��ֵ����ҳָ��
			web.CallJScript("ShowStationSite", content);									//����JS������λ��
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ͼ��ȡ��վַ��
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnBnClickedQuary3()
{
	My_Map_Station();
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ��վַ��
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺲�ѯ
	//this->v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����	[��̬���޼���]
//------------------------------------------------------------------------------------------------------
UINT CConAnalyzeIndicator::My_Thread_Inoput(LPVOID lparam)
{
	CConAnalyzeIndicator *  lp_this = NULL ;
	lp_this = (CConAnalyzeIndicator *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(false);								//��������Ч
	lp_this->m_cDetail.EnableWindow(false);								//��ͼ��ȡ����Ч
	lp_this->m_cMarker.EnableWindow(false);								//��ͼ��ǣ���Ч
	lp_this->m_cClear.EnableWindow(false);								//����������Ч
	lp_this->m_cKey_Select.EnableWindow(false);							//����ѡ����Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(true);								//��������Ч
	lp_this->m_cDetail.EnableWindow(true);								//��ͼ��ȡ����Ч
	lp_this->m_cMarker.EnableWindow(true);								//��ͼ��ǣ���Ч
	lp_this->m_cClear.EnableWindow(true);								//����������Ч
	lp_this->m_cKey_Select.EnableWindow(true);							//����ѡ����Ч

	//lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_Query_Station();												//վַ��ѯ
		break;
	case 2:
		My_Map_Station();												//��ͼ��ȡ
		break;
	case 3:
		My_Marker();													//����Marker
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Marker
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::My_Marker()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWebPage	web;													//����һ��ҳ��
	web.SetDocument(m_webBrowser.GetDocument());						//��ֵ����ҳָ��
	for (int vi = 0; vi<m_cList_Station.GetItemCount(); vi++)				//�������б�
	{
		web.CallJScript("myMarker_Insert", m_cList_Station.GetItemText(vi, 9), m_cList_Station.GetItemText(vi, 10), m_cList_Station.GetItemText(vi, 1));	//����JS������Marker
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ������Ϣ
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::My_Map_Station()
{
	int				v_iID = 0;												//KPI����
	CString			v_sSql, v_sTemp;											//��ʱ����

	//��ȡ����ͼ��Ļ��γ��
	CComVariant		v_sw_lng, v_sw_lat, v_ne_lng, v_ne_lat;					//����ֵ���ĸ���γ��  
	CComVariant		v_varResult;											//��ŷ���ֵ
	CWebPage		v_cWeb;													//����һ��ҳ��
	v_cWeb.SetDocument(m_webBrowser.GetDocument());							//��ֵ����ҳָ��
	v_cWeb.CallJScript("myGet_Longitude", &v_varResult);						//����JS����ͼ��Ļ��γ��
	CComDispatchDriver spData = v_varResult.pdispVal;						//��ȡ������ 
	spData.GetPropertyByName(L"sw_lng", &v_sw_lng);
	spData.GetPropertyByName(L"sw_lat", &v_sw_lat);
	spData.GetPropertyByName(L"ne_lng", &v_ne_lng);
	spData.GetPropertyByName(L"ne_lat", &v_ne_lat);
	double	v_dsw_lng = v_sw_lng.dblVal;
	double	v_dsw_lat = v_sw_lat.dblVal;
	double	v_dne_lng = v_ne_lng.dblVal;
	double	v_dne_lat = v_ne_lat.dblVal;

	//??????????????????????????���׼ȷ����������������������������������������������
	double x_pi = 3.1415926;
	double bd_x = v_dsw_lng - 0.0065;
	double bd_y = v_dsw_lat - 0.006;
	double z = sqrt(bd_x * bd_x + bd_y * bd_y) - 0.00002 * sin(bd_y * x_pi);
	double theta = atan2(bd_y, bd_x) - 0.000003 * cos(bd_x * x_pi);
	v_dsw_lng = z * cos(theta) - 0.005;
	v_dsw_lat = z * sin(theta) - 0.001;

	bd_x = v_dne_lng - 0.0065;
	bd_y = v_dne_lat - 0.006;
	z = sqrt(bd_x * bd_x + bd_y * bd_y) - 0.00002 * sin(bd_y * x_pi);
	theta = atan2(bd_y, bd_x) - 0.000003 * cos(bd_x * x_pi);
	v_dne_lng = z * cos(theta) - 0.005;
	v_dne_lat = z * sin(theta) - 0.001;

	CString v_sTop, v_sLeft, v_sBottom, v_sRight;							//�ַ������ĸ���γ��
	v_sTop.Format("%8.6f", v_dsw_lng);
	v_sLeft.Format("%8.6f", v_dsw_lat);
	v_sBottom.Format("%8.6f", v_dne_lng);
	v_sRight.Format("%8.6f", v_dne_lat);

	v_sSql = "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT * FROM CON_Station";
	v_sSql += " where (zLongitude between " + v_sTop + " and " + v_sBottom + ") and (zLatitude between " + v_sLeft + " and " + v_sRight + ")";
	v_sSql += " order by zStation";
	My_LoadData_Station_Data(v_sSql);									//��ȡ��վַ��Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ������Ϣ
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::My_Query_Station()
{
	int				v_iID=0;											//KPI����
	CString			v_sSql,v_sTemp;										//��ʱ����
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//��ѯ����׼��
	m_cKey.GetWindowText(v_sLog_Key);										//��ã���ѯKEY
	v_sLog_Key.Trim();														//ȥ���ո�

	v_sSql += "SELECT * FROM CON_Station"; 

	if (!v_sLog_Key.IsEmpty())												//��ѯKEY����
	{
		switch (m_cKey_Select.GetCurSel())									//ѡ�񣿣���
		{
		case 0:																//վַ
			v_sSql = "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
			v_sSql += "SELECT * FROM CON_Station";
			v_sSql += " where (zStation like '%" + v_sLog_Key + "%')";
			break;
		case 1:		
			v_sSql = "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
			v_sSql += "SELECT * FROM CON_Station";
			v_sSql += " where (zRegion like '%" + v_sLog_Key + "%')";		//����
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
void CConAnalyzeIndicator::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem, v_iID = 0;									//KPI����
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����

	_variant_t	Var;
	CString			v_sStation, v_sRegion, v_sLongitude;					//��ʱ����
	CString			v_sLatitude, v_sOnline;								//��ʱ����
	double			v_dValue;											//��ʱ����
	int				v_iGSM_BTS, v_iGSM_Cell, v_iTD_NodeB, v_iTD_Cell, v_iLTE_NodeB, v_iLTE_Cell, v_iSum;	//��ʱ����
	int				v_iGSM_BTS_OUT, v_iGSM_Cell_OUT, v_iTD_NodeB_OUT, v_iTD_Cell_OUT, v_iLTE_NodeB_OUT, v_iLTE_Cell_OUT;	//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn		v_cAdoConn;											//���ݿ�ʵ��
	try
	{
		//�б�λ
		m_cList_Station.DeleteAllItems();										//���Items

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type, theApp.a_sDatabase_Ip, theApp.a_sDatabase_Name, theApp.a_sDatabase_User, theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while (!v_pRecordset->adoEOF)
		{
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_dValue = v_cAdoConn.GetField_Double("zLongitude");		//zLongitude
			v_sLongitude.Format("%10.6f", v_dValue);
			v_dValue = v_cAdoConn.GetField_Double("zLatitude");			//zLatitude
			v_sLatitude.Format("%10.6f", v_dValue);

			v_iGSM_BTS = 0;												//zGSM_BTS
			v_iGSM_Cell = 0;											//zGSM_Cell
			v_iTD_NodeB = 0;											//zTD_NodeB
			v_iTD_Cell = 0;												//zTD_Cell
			v_iLTE_NodeB = 0;											//zLTE_NodeB
			v_iLTE_Cell = 0;											//zLTE_Cell
			v_iGSM_BTS_OUT = 0;											//zGSM_BTS
			v_iGSM_Cell_OUT = 0;										//zGSM_Cell
			v_iTD_NodeB_OUT = 0;										//zTD_NodeB
			v_iTD_Cell_OUT = 0;											//zTD_Cell
			v_iLTE_NodeB_OUT = 0;										//zLTE_NodeB
			v_iLTE_Cell_OUT = 0;										//zLTE_Cell

			v_iGSM_BTS_OUT = v_cAdoConn.GetField_Int("zGSM_BTS_OUT");		//zGSM_BTS_OUT
			v_iGSM_Cell_OUT = v_cAdoConn.GetField_Int("zGSM_Cell_OUT");		//zGSM_Cell_OUT
			v_iTD_NodeB_OUT = v_cAdoConn.GetField_Int("zTD_NodeB_OUT");		//zTD_NodeB_OUT
			v_iTD_Cell_OUT = v_cAdoConn.GetField_Int("zTD_Cell_OUT");		//zTD_Cell_OUT
			v_iLTE_NodeB_OUT = v_cAdoConn.GetField_Int("zLTE_NodeB_OUT");	//zLTE_NodeB_OUT
			v_iLTE_Cell_OUT = v_cAdoConn.GetField_Int("zLTE_Cell_OUT");		//zLTE_Cell_OUT

			v_iGSM_BTS = v_cAdoConn.GetField_Int("zGSM_BTS");		//zGSM_BTS
			v_iGSM_Cell = v_cAdoConn.GetField_Int("zGSM_Cell");		//zGSM_Cell
			v_iTD_NodeB = v_cAdoConn.GetField_Int("zTD_NodeB");		//zTD_NodeB
			v_iTD_Cell = v_cAdoConn.GetField_Int("zTD_Cell");		//zTD_Cell
			v_iLTE_NodeB = v_cAdoConn.GetField_Int("zLTE_NodeB");	//zLTE_NodeB
			v_iLTE_Cell = v_cAdoConn.GetField_Int("zLTE_Cell");		//zLTE_Cell

																	//��ʾ
			if (true)
			{
				v_iSum = v_iGSM_BTS + v_iTD_NodeB + v_iLTE_NodeB + v_iGSM_BTS_OUT + v_iTD_NodeB_OUT + v_iLTE_NodeB_OUT;	//��վ�ܼ�
				if (v_iSum != 0)
				{
					v_sTemp.Format(" %04d", v_iID + 1);
					v_iItem = m_cList_Station.InsertItem(0xffff, v_sTemp);			//�����б�
					m_cList_Station.SetItemText(v_iItem, 1, v_sStation);			//����/��Դ������
					m_cList_Station.SetItemText(v_iItem, 2, v_sRegion);			//����
					v_sTemp.Format("%d", v_iGSM_BTS + v_iGSM_BTS_OUT);
					m_cList_Station.SetItemText(v_iItem, 3, v_sTemp);				//GSM_BTS
					v_sTemp.Format("%d", v_iGSM_Cell + v_iGSM_Cell_OUT);
					m_cList_Station.SetItemText(v_iItem, 4, v_sTemp);				//GSM_Cell
					v_sTemp.Format("%d", v_iTD_NodeB + v_iTD_NodeB_OUT);
					m_cList_Station.SetItemText(v_iItem, 5, v_sTemp);				//TD_NodeB
					v_sTemp.Format("%d", v_iTD_Cell + v_iTD_Cell_OUT);
					m_cList_Station.SetItemText(v_iItem, 6, v_sTemp);				//TD_Cell
					v_sTemp.Format("%d", v_iLTE_NodeB + v_iLTE_NodeB_OUT);
					m_cList_Station.SetItemText(v_iItem, 7, v_sTemp);				//LTE_NodeB
					v_sTemp.Format("%d", v_iLTE_Cell + v_iLTE_Cell_OUT);
					m_cList_Station.SetItemText(v_iItem, 8, v_sTemp);				//LTE_Cell
					m_cList_Station.SetItemText(v_iItem, 9, v_sLongitude);		//����
					m_cList_Station.SetItemText(v_iItem, 10, v_sLatitude);		//γ��

					v_iID++;											//�������澯����
				}
			}

			v_pRecordset->MoveNext();									//

			if (v_iID % 100 == 0)											//��ʾ��100������
			{
				v_sTemp.Format("%d", v_iID);								//
				m_cSum.SetWindowText(v_sTemp);							//��ʾ��վַ����
			}
		}
		v_sTemp.Format("%d", v_iID);										//
		m_cSum.SetWindowText(v_sTemp);									//��ʾ��վַ����

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch (_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}
}

//-------------------------------------------------------------------------------------------------
//	�������ܣ�JS������Ӧ�����ݣ���ͼ��Ļ��γ�ȣ�4��������
//	��    ����mStation_OutPut������ָ��
//-------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::My_Station_Select(CString v_sTop, CString v_sLeft,CString v_sBottom, CString v_sRight)
{
	CString		str;
	str.Format("��һ������:%s,�ڶ�������:%s,����������:%s,���ĸ�����:%s",v_sTop,v_sLeft,v_sBottom,v_sRight);
	MessageBox(str);
}

//=================================================================================================
//	�������ܣ��¼�ӳ�䡾������ؼ�--������
//=================================================================================================
BEGIN_EVENTSINK_MAP(CConAnalyzeIndicator, CFormView)
	//{{AFX_EVENTSINK_MAP(CMFCHtmlDlg)
	ON_EVENT(CConAnalyzeIndicator, IDC_EXPLORER, 259 /* DocumentComplete */, OnDocumentCompleteExplorerMain, VTS_DISPATCH VTS_PVARIANT)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

//-------------------------------------------------------------------------------------------------
//	�������ܣ�ʱ����Ӧ
//-------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnDocumentCompleteExplorerMain(LPDISPATCH pDisp, VARIANT FAR* URL) 
{
	// TODO: Add your control notification handler code here
	IDispatch*				pIDisp			= NULL;
	pIDisp = m_webBrowser.GetDocument();
	if(pIDisp)
	{
		if(m_pIHTMLDoc)
		{
			m_pIHTMLDoc->Release();
			m_pIHTMLDoc = NULL;
		}
		pIDisp->QueryInterface(IID_IHTMLDocument2, (void**)&m_pIHTMLDoc);
		pIDisp->Release();
		pIDisp = NULL;
	}
}

/***************************************************************************************************************
Description:�����ⶨ��ΨһID�����ID����ȡ0-16384֮���������
****************************************************************************************************************/
enum
{
	FUNCTION_ShowMessageBox = 1,
	FUNCTION_GetProcessID = 2,
	FUNCTION_GetStationBounds = 3,
};

/***************************************************************************************************************
Description: ����ʵ�֣�ֱ�ӷ���E_NOTIMPL
****************************************************************************************************************/
HRESULT STDMETHODCALLTYPE CConAnalyzeIndicator::GetTypeInfoCount(UINT *pctinfo)
{
	return E_NOTIMPL;
}

/***************************************************************************************************************
Description: ����ʵ�֣�ֱ�ӷ���E_NOTIMPL
****************************************************************************************************************/
HRESULT STDMETHODCALLTYPE CConAnalyzeIndicator::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	return E_NOTIMPL;
}

/***************************************************************************************************************
Description: JavaScript�����������ķ���ʱ����ѷ������ŵ�rgszNames�У�������Ҫ������������ⶨһ��Ψһ������ID����
			 rgDispId���ظ�����ͬ��JavaScript��ȡ������������ʱ������������ŵ�rgszNames�У�������Ҫ������������ⶨ
			 һ��Ψһ������ID����rgDispId���ظ�����������JavaScript�����Invoke���������ID��Ϊ�������ݽ���
****************************************************************************************************************/
HRESULT STDMETHODCALLTYPE CConAnalyzeIndicator::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
	//rgszNames�Ǹ��ַ������飬cNamesָ������������м����ַ������������1���ַ�����������
	if (cNames != 1)
		return E_NOTIMPL;
	//����ַ�����ShowMessageBox��˵��JavaScript�ڵ�������������ShowMessageBox�������ҾͰ����ⶨ��IDͨ��rgDispId������
	if (wcscmp(rgszNames[0], L"ShowMessageBox") == 0)
	{
		*rgDispId = FUNCTION_ShowMessageBox;
		return S_OK;
	}
	//ͬ������ַ�����GetProcessID��˵��JavaScript�ڵ�������������GetProcessID����
	else if (wcscmp(rgszNames[0], L"GetProcessID") == 0)
	{
		*rgDispId = FUNCTION_GetProcessID; FUNCTION_GetStationBounds;
		return S_OK;
	}
	//ͬ������ַ�����GetStationBounds��˵��JavaScript�ڵ�������������GetStationBounds����
	else if (wcscmp(rgszNames[0], L"GetStationBounds") == 0)
	{
		*rgDispId = FUNCTION_GetStationBounds;
		return S_OK;
	}
	else
		return E_NOTIMPL;
}

/***************************************************************************************************************
Description:  JavaScriptͨ��GetIDsOfNames�õ��ҵĶ���ķ�����ID�󣬻����Invoke��dispIdMember���Ǹղ��Ҹ����������Լ�
			  �ⶨ��ID��wFlagsָ��JavaScript���ҵĶ������ʲô���飡�����DISPATCH_METHOD��˵��JavaScript�ڵ��������
			  ��ķ���������cpp_object.ShowMessageBox();�����DISPATCH_PROPERTYGET��˵��JavaScript�ڻ�ȡ����������
			  �ԣ�����var n = cpp_object.num;�����DISPATCH_PROPERTYPUT��˵��JavaScript���޸������������ԣ�����
			  cpp_object.num = 10;�����DISPATCH_PROPERTYPUTREF��˵��JavaScript��ͨ�������޸�������󣬾�����Ҳ����
			  ʾ�����벢û���漰��wFlags�Ͷ������Ե�ʹ�ã���Ҫ���������о����÷���һ���ġ�/pDispParams����JavaScript����
			  �ҵĶ���ķ���ʱ���ݽ����Ĳ�����������һ�����鱣�������в�����pDispParams->cArgs�����������ж��ٸ�������
			  pDispParams->rgvarg���Ǳ����Ų��������飬��ʹ��[]�±������ʣ�ÿ����������VARIANT���ͣ����Ա���������͵�ֵ
			  ������ʲô������VARIANT::vt���жϣ���������ˣ�VARIANT�ⶫ����Ҷ�����pVarResult�������Ǹ�JavaScript�ķ���ֵ
****************************************************************************************************************/
HRESULT STDMETHODCALLTYPE CConAnalyzeIndicator::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
	WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	//ͨ��ID�Ҿ�֪��JavaScript������ĸ�����
	if (dispIdMember == FUNCTION_ShowMessageBox)
	{
		//����Ƿ�ֻ��һ������
		if (pDispParams->cArgs != 1)
			return E_NOTIMPL;
		//�����������Ƿ����ַ�������
		if (pDispParams->rgvarg[0].vt != VT_BSTR)
			return E_NOTIMPL;
		//���ĵ���
		ShowMessageBox(pDispParams->rgvarg[0].bstrVal);
		return S_OK;
	}
	else if (dispIdMember == FUNCTION_GetProcessID)
	{
		DWORD id = GetProcessID();
		*pVarResult = CComVariant(id);
		return S_OK;
	}
	else if (dispIdMember == FUNCTION_GetStationBounds)
	{
		GetStationBounds();
		return S_OK;
	}
	else
		return E_NOTIMPL;
}
/***************************************************************************************************************
Description: JavaScript�õ����Ǵ��ݸ�����ָ�����������������ǵĶ�����ʲô�����������QueryInterface��ѯ�����ǡ�����ʲ
			 ô������������ͨ��riid����������ʲô������ֻ�����ʵ������ǲ���IID_IDispatch�������ǲ���IID_IUnknownʱ����
			 �ǲ��ܿ϶��Ļش���S_OK��Ϊ���ǵĶ���̳���IDispatch����IDispatch�ּ̳���IUnknown������ֻʵ�����������ӿڣ�
			 ����ֻ���������ش�����ѯ��
****************************************************************************************************************/
HRESULT STDMETHODCALLTYPE CConAnalyzeIndicator::QueryInterface(REFIID riid, void **ppvObject)
{
	if (riid == IID_IDispatch || riid == IID_IUnknown)
	{
		//�Եģ�����һ��IDispatch�������Լ�(this)������
		*ppvObject = static_cast<IDispatch*>(this);
		return S_OK;
	}
	else
		return E_NOINTERFACE;
}

/***************************************************************************************************************
Description:  ����֪��COM����ʹ�����ü�������������������ڣ����ǵ�CJsCallCppDlg������������ھ��������������������
			  �ҵ����������Ҫ��JavaScript���ܣ����Լ���ܣ������Ҳ���ʵ��AddRef()��Release()��������дһЩ��
			  ��Ҫreturn 1;return 2;return 3;return 4;return 5;������
****************************************************************************************************************/
ULONG STDMETHODCALLTYPE CConAnalyzeIndicator::AddRef()
{
	return 1;
}

/***************************************************************************************************************
Description:  ����֪��COM����ʹ�����ü�������������������ڣ����ǵ�CJsCallCppDlg������������ھ��������������������
			  �ҵ����������Ҫ��JavaScript���ܣ����Լ���ܣ������Ҳ���ʵ��AddRef()��Release()��������дһЩ��
			  ��Ҫreturn 1;return 2;return 3;return 4;return 5������;��Ȼ�����Ҫnew��һ��c++���������Ӹ�JavaScript���ܣ�
			  �����Ҫʵ��AddRef()��Release()�������ü�������ʱdelete this;
****************************************************************************************************************/
ULONG STDMETHODCALLTYPE CConAnalyzeIndicator::Release()
{
	return 1;
}

DWORD CConAnalyzeIndicator::GetProcessID()
{
	return GetCurrentProcessId();
}

void CConAnalyzeIndicator::ShowMessageBox(const wchar_t *msg)
{
	CString v_sPara;
	CString content;
    //�����ַ���������szGBKΪ������ɲ�����
	int len = WideCharToMultiByte(CP_ACP, 0, msg, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, msg, -1, szGBK, len, NULL, NULL);

	content = (CString)szGBK + "hello c++!";
	v_sPara.Format("%s", content);
	MessageBox(v_sPara);

	delete[] szGBK;
}


void CConAnalyzeIndicator::GetStationBounds()
{
	int				v_iID = 0;												//KPI����
	CString			v_sSql, v_sTemp,content;								//��ʱ����
	//��ȡ����ͼ��Ļ��γ��
	CComVariant		v_sw_lng, v_sw_lat, v_ne_lng, v_ne_lat;					//����ֵ���ĸ���γ��  
	CComVariant		v_varResult;											//��ŷ���ֵ
	CWebPage		v_cWeb;													//����һ��ҳ��
	v_cWeb.SetDocument(m_webBrowser.GetDocument());							//��ֵ����ҳָ��
	v_cWeb.CallJScript("myGet_Longitude", &v_varResult);					//����JS����ͼ��Ļ��γ��
	CComDispatchDriver spData = v_varResult.pdispVal;						//��ȡ������ 
	spData.GetPropertyByName(L"sw_lng", &v_sw_lng);
	spData.GetPropertyByName(L"sw_lat", &v_sw_lat);
	spData.GetPropertyByName(L"ne_lng", &v_ne_lng);
	spData.GetPropertyByName(L"ne_lat", &v_ne_lat);
	double	v_dsw_lng = v_sw_lng.dblVal;
	double	v_dsw_lat = v_sw_lat.dblVal;
	double	v_dne_lng = v_ne_lng.dblVal;
	double	v_dne_lat = v_ne_lat.dblVal;

	//??????????????????????????���׼ȷ����������������������������������������������
	double x_pi = 3.1415926;
	double bd_x = v_dsw_lng - 0.0065;
	double bd_y = v_dsw_lat - 0.006;
	double z = sqrt(bd_x * bd_x + bd_y * bd_y) - 0.00002 * sin(bd_y * x_pi);
	double theta = atan2(bd_y, bd_x) - 0.000003 * cos(bd_x * x_pi);
	v_dsw_lng = z * cos(theta) - 0.005;
	v_dsw_lat = z * sin(theta) - 0.001;

	bd_x = v_dne_lng - 0.0065;
	bd_y = v_dne_lat - 0.006;
	z = sqrt(bd_x * bd_x + bd_y * bd_y) - 0.00002 * sin(bd_y * x_pi);
	theta = atan2(bd_y, bd_x) - 0.000003 * cos(bd_x * x_pi);
	v_dne_lng = z * cos(theta) - 0.005;
	v_dne_lat = z * sin(theta) - 0.001;

	CString v_sTop, v_sLeft, v_sBottom, v_sRight;							//�ַ������ĸ���γ��
	v_sTop.Format("%8.6f", v_dsw_lng);
	v_sLeft.Format("%8.6f", v_dsw_lat);
	v_sBottom.Format("%8.6f", v_dne_lng);
	v_sRight.Format("%8.6f", v_dne_lat);

	//��ʾGPS��γ��
	content = v_sBottom + "," + v_sLeft + "," + v_sTop + "," + v_sRight;
	content.Trim();
	if (!content.IsEmpty())
	{
		CWebPage	web;														//����һ��ҳ��
		web.SetDocument(m_webBrowser.GetDocument());							//��ֵ����ҳָ��
		web.CallJScript("ShowBounds", content);									//����JS������λ��
	}
}
//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
