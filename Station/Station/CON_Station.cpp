// CON_Station.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "CON_Station.h"
#include "CON_Station_Input.h"
#include "CON_Station_Input_R.h"
#include "CON_Station_Input_L.h"
#include "CON_Station_Region.h"


// CCON_Station

IMPLEMENT_DYNCREATE(CCON_Station, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CCON_Station::CCON_Station()
	: CFormView(CCON_Station::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CCON_Station::~CCON_Station()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CCON_Station::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_Station);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_ELEMENT, m_cElement);
	DDX_Control(pDX, IDC_ELEMENT2, m_cRemote);
	DDX_Control(pDX, IDC_ELEMENT3, m_cStation);
	DDX_Control(pDX, IDC_BUTTON2, m_cCell);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_Station, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CCON_Station::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_ELEMENT, &CCON_Station::OnBnClickedElement)
	ON_BN_CLICKED(IDC_EXCEL2, &CCON_Station::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CCON_Station::OnNMClickList3)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CCON_Station::OnNMRClickList3)
	ON_COMMAND(ID_CON_STATION_DEL, &CCON_Station::OnConStationDel)
	ON_BN_CLICKED(IDC_ELEMENT2, &CCON_Station::OnBnClickedElement2)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CCON_Station::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_ELEMENT3, &CCON_Station::OnBnClickedElement3)
	ON_BN_CLICKED(IDC_BUTTON2, &CCON_Station::OnBnClickedButton2)
END_MESSAGE_MAP()


// CCON_Station ���

#ifdef _DEBUG
void CCON_Station::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCON_Station::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCON_Station ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="վַ����";											//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б� [վַ]
	::SendMessage(m_cList_Station.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Station.InsertColumn(0,"���",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(1,"����/��Դ������",LVCFMT_LEFT,300);	//
	m_cList_Station.InsertColumn(2,"����",LVCFMT_LEFT,120);				//
	m_cList_Station.InsertColumn(3,"����",LVCFMT_LEFT,80);				//
	m_cList_Station.InsertColumn(4,"��ַ",LVCFMT_LEFT,300);				//
	m_cList_Station.InsertColumn(5,"����",LVCFMT_RIGHT,100);			//
	m_cList_Station.InsertColumn(6,"γ��",LVCFMT_RIGHT,100);			//
	m_cList_Station.InsertColumn(7,"״̬",LVCFMT_CENTER,80);			//
	m_cList_Station.InsertColumn(8,"��������",LVCFMT_CENTER,100);		//
	m_cList_Station.SetExtendedStyle(m_cList_Station.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��Ϣ��ʼ��
	m_cKey_Select.AddString("����/��Դ��");
	m_cKey_Select.AddString("����");
	m_cKey_Select.AddString("����");
	m_cKey_Select.AddString("��ַ");
	m_cKey_Select.SetCurSel(0);											//Ĭ�ϣ�ѡ���һ��

	v_iList_Item=-1;													//��Ŀѡ����Ч
	v_iSum=0;															//��������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CCON_Station::PreTranslateMessage(MSG* pMsg)
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
void CCON_Station::OnDestroy()
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
void CCON_Station::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CWnd	*pCellList;													//��ʱ����
	int		v_iY;														//��ʱ����

	pCellList = GetDlgItem(IDC_LIST3);
	v_iY = 75;															//��ֵ��Y��
	if(pCellList)
		pCellList->MoveWindow(14,v_iY,cx-24,cy-v_iY-10);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�վַ����
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Station);										//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnBnClickedElement()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCON_Station_Input	v_cDlg;											//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Դ�㵼��
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnBnClickedElement2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCON_Station_Input_R	v_cDlg;										//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�վ�㵼��
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnBnClickedElement3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCON_Station_Input_L	v_cDlg;										//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б����� [վַ]
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList_Station.My_Sort(pNMHDR);										//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����Ӧ
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item>=0)											//��Ч��Ŀ��ţ�����
				m_cList_Station.SetItemBkColor(v_iList_Item,RGB(255,255,255));		//�ָ�����ɫ
			v_iList_Item=pNMListView->iItem;							//��ã�ѡ����Ŀ���
			m_cList_Station.SetItemBkColor(v_iList_Item,RGB(255,0,0));	//���ñ���ɫ����ɫ
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б��һ���Ӧ
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu	aMenu;														//�˵�
	CPoint	point;														//λ�õ�

	GetCursorPos(&point);											//��ã����λ��
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;				//��ã�������б�λ��
	int v_iSelect = pNMListView->iItem;								//��ȡ������б���Item
	if( v_iSelect == -1 )											//��ЧItem������
		return;														//��Ч������
	if(v_iList_Item>=0)												//��Ч��Ŀ��ţ�����
		m_cList_Station.SetItemBkColor(v_iList_Item,RGB(255,255,255));		//�ָ�����ɫ
	v_iList_Item=v_iSelect;											//��ã�ѡ����Ŀ���
	m_cList_Station.SetItemBkColor(v_iList_Item,RGB(255,0,0));		//���ñ���ɫ����ɫ

	aMenu.LoadMenu(IDR_STATION_DEL);								//װ�أ�IDR_GSM_BTS�˵�
	CMenu *pPopup =aMenu.GetSubMenu(0);								//���ɸ����˵�
	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);		//��ʾ���������λ��[point]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ɾ��վַ
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnConStationDel()
{
	// TODO: �ڴ���������������
	int		v_iItem,vi;													//��ʱ����
	CString	v_sCell,v_sTemp,v_sSql;										//��ʱ����

	vi = m_cList_Station.GetSelectedCount();							//ѡ����Ŀ����
	v_sTemp.Format("%d",vi);											//�ַ���
	if(MessageBox("ɾ��ѡ�е� "+v_sTemp+" ��վַ�� ɾ��ȷ�� ? ","����ɾ��ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)	//ɾ��ȷ�ϣ�����
	{
		//������ѡ�е�Item���в���
		while ((v_iItem = m_cList_Station.GetNextItem(-1, LVNI_SELECTED)) != -1)//��Ŀѡ�У�����
		{
			v_sCell=m_cList_Station.GetItemText(v_iItem,1);				//��ã�v_sCell [���ݱ�Ψһ��ʶ�������޸����ݱ�]

			//ɾ����Cell
			v_sSql = "delete from CON_Station where zStation='"+v_sCell+"'";	//SQLɾ��
			My_ExecuteSQL(v_sSql);										//д��

			//ɾ�����б���Ϣ
			m_cList_Station.DeleteItem(v_iItem);						//ɾ����ѡ�е�v_iItem

			//���棺Log
			COleDateTime v_tTime=COleDateTime::GetCurrentTime();
			CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");
			CString v_sSql="Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"+v_sCurrentTime+"','ɾ��վַ','վַ���ƣ�"
				+v_sCell+"������Ա��"+theApp.a_sUser_Name+"')";
			My_ExecuteSQL(v_sSql);										//���ݿ�ִ��
		}
		v_iSum-=vi;														//ʣ������
		v_sTemp.Format("%d",v_iSum);									//�ַ���
		m_cSum.SetWindowText(v_sTemp);									//��ʾ��վַ����
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�����ͬ��
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCON_Station_Region	v_cDlg;											//
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CCON_Station::My_Thread_Inoput(LPVOID lparam)
{
	CCON_Station *  lp_this = NULL ;
	lp_this = (CCON_Station *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel_NodeB.EnableWindow(false);						//NodeB��������Ч
	lp_this->m_cElement.EnableWindow(false);							//վַ���룺��Ч
	lp_this->m_cRemote.EnableWindow(false);								//��Դ�㵼�룺��Ч
	lp_this->m_cStation.EnableWindow(false);							//վ�㵼�룺��Ч
	lp_this->m_cCell.EnableWindow(false);								//����˲飺��Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel_NodeB.EnableWindow(true);							//NodeB��������Ч
	lp_this->m_cElement.EnableWindow(true);								//վַ���룺��Ч
	lp_this->m_cRemote.EnableWindow(true);								//��Դ�㵼�룺��Ч
	lp_this->m_cStation.EnableWindow(true);								//վ�㵼�룺��Ч
	lp_this->m_cCell.EnableWindow(true);								//����˲飺��Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CCON_Station::My_Input_Main()
{
	m_cList_Station.DeleteAllItems();									//���Items
	v_iList_Item=-1;													//��Ŀѡ����Ч

	switch(v_iWorking)
	{
	case 1:
		My_Query_Station();												//վַ��ѯ
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯվַ��Ϣ
//------------------------------------------------------------------------------------------------------
void CCON_Station::My_Query_Station()
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
		switch (m_cKey_Select.GetCurSel())								//ѡ�񣿣���
		{
		case 0:															//վַ
			v_sSql+=" where zStation like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//����
			v_sSql+=" where zType like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//����
			v_sSql+=" where zRegion like '%"+v_sLog_Key+"%'";
			break;
		case 3:															//��ַ
			v_sSql+=" where zAddress like '%"+v_sLog_Key+"%'";
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
void CCON_Station::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem;											//��ʱ����
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����

	_variant_t	Var;
	CString			v_sStation,v_sType,v_sRegion,v_sAddress,v_sLongitude;	//��ʱ����
	CString			v_sLatitude,v_sOnline,v_sDate;						//��ʱ����
	double			v_dValue;											//��ʱ����

	v_iSum=0;															//��ʼֵ

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
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

			//��ʾ
			v_sTemp.Format(" %04d",v_iSum+1);
			v_iItem=m_cList_Station.InsertItem(0xffff,v_sTemp);			//�����б�
			m_cList_Station.SetItemText(v_iItem,1,v_sStation);			//����/��Դ������
			m_cList_Station.SetItemText(v_iItem,2,v_sType);				//����
			m_cList_Station.SetItemText(v_iItem,3,v_sRegion);			//����
			m_cList_Station.SetItemText(v_iItem,4,v_sAddress);			//��ַ
			m_cList_Station.SetItemText(v_iItem,5,v_sLongitude);		//����
			m_cList_Station.SetItemText(v_iItem,6,v_sLatitude);			//γ��
			m_cList_Station.SetItemText(v_iItem,7,v_sOnline);			//״̬
			m_cList_Station.SetItemText(v_iItem,8,v_sDate);				//��������

			v_pRecordset->MoveNext();									//
			v_iSum++;													//�������澯����

			if (v_iSum%100==0)											//��ʾ��100������
			{
				v_sTemp.Format("%d",v_iSum);							//
				m_cSum.SetWindowText(v_sTemp);							//��ʾ��վַ����
			}
		}
		v_sTemp.Format("%d",v_iSum);									//
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
//	�������ܣ�����˲�
//------------------------------------------------------------------------------------------------------			
void CCON_Station::My_Query_Region()
{
	CString		v_sSql,v_sTemp,v_sRegion,v_sRNC,v_sRegion_Station;		//��ʱ����
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
				  GSM_BTS.zBTS_NAME,GSM_BTS.zStation as zStation_BTS,GSM_Cell.zRegion,CON_Station.zRegion as zRegion_Station \
				  FROM GSM_Cell left join GSM_BTS on GSM_Cell.zBTS=GSM_BTS.zBTS left join CON_Station on GSM_Cell.zStation=CON_Station.zStation"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sRegion_Station = v_cAdoConn.GetField_String("zRegion_Station");	//zRegion_Station
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell
			v_sRegion = v_cAdoConn.GetField_String("zRegion");		//zRegion

			v_pStation_OutPut=v_mStation_OutPut.find(v_sSation_Cell);	//���ң�վַ
			if (v_pStation_OutPut==v_mStation_OutPut.end())
			{
				//���ӣ�վַ
				v_cStation_OutPut.v_sRegion=v_sRegion_Station;			//����
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
			v_pmStation_OutPut_BTS->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sRegion));	//��������

			v_pRecordset->MoveNext();									//��һ��
		}

		//��TD-LTE��
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select LTE_Cell.zCell,LTE_Cell.zNODEB as zBTS_BTS,LTE_Cell.zStation as zStation_Cell, \
				  LTE_NODEB.zStation as zStation_BTS,LTE_NODEB.zRNC,LTE_Cell.zRegion,CON_Station.zRegion as zRegion_Station \
				  FROM LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB left join CON_Station on LTE_Cell.zStation=CON_Station.zStation"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zBTS
			v_sRNC = v_cAdoConn.GetField_String("zRNC");				//zRNC
			v_sRegion_Station = v_cAdoConn.GetField_String("zRegion_Station");	//zRegion_Station
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell
			v_sRegion = v_cAdoConn.GetField_String("zRegion");		//zRegion

			v_pStation_OutPut=v_mStation_OutPut.find(v_sSation_Cell);	//���ң�վַ
			if (v_pStation_OutPut==v_mStation_OutPut.end())
			{
				//���ӣ�վַ
				v_cStation_OutPut.v_sRegion=v_sRegion_Station;			//����
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
				v_pmStation_OutPut_BTS->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sRegion));	//��������
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
				v_pmStation_OutPut_BTS->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sRegion));	//��������
			}

			v_pRecordset->MoveNext();									//
		}

		v_cAdoConn.ExitConnect();										//�Ͽ�����

		//���ɵ����ļ�
		My_Show_Region(&v_mStation_OutPut);								//��ʾ����˲���
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
// ����Excel���������б�
// ��         ����mStation_OutPut������ָ��
//-------------------------------------------------------------------------------------------------
void CCON_Station::My_Show_Region(mStation_OutPut *v_mStation_OutPut) 
{
	CString		v_sTemp,v_sNetwork,v_sBTS,v_sBTS_Name;
	int			v_iItem,v_iID=0;

	mStation_OutPut::iterator		v_pStation_OutPut;					//������
	mStation_OutPut_BTS::iterator	v_pmStation_OutPut_BTS;				//������
	mString_String::iterator		v_pCell;							//������

	for(v_pStation_OutPut=v_mStation_OutPut->begin();v_pStation_OutPut!=v_mStation_OutPut->end();v_pStation_OutPut++)
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//ǿ���˳�������
			break;														//�˳�

		if ((v_pStation_OutPut->second.v_mGSM.size()+v_pStation_OutPut->second.v_mTD.size()+v_pStation_OutPut->second.v_mLTE.size())==0)
			continue;													//��һվַ

		if(v_pStation_OutPut->second.v_bNet_GSM)
			v_sNetwork="2";												//��ַ����
		if(v_pStation_OutPut->second.v_bNet_TD)
		{
			if(!v_sNetwork.IsEmpty())
				v_sNetwork+="_";										//��ַ����
			v_sNetwork+="3";											//��ַ����
		}
		if(v_pStation_OutPut->second.v_bNet_LTE)
		{
			if(!v_sNetwork.IsEmpty())
				v_sNetwork+="_";										//��ַ����
			v_sNetwork+="4";											//��ַ����
		}
		//GSM BTS
		if (v_pStation_OutPut->second.v_mGSM.size()>0)					//GSM BTS
		{
			v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mGSM.begin();		//ͷ������
			while(v_pmStation_OutPut_BTS!=v_pStation_OutPut->second.v_mGSM.end())	//β������
			{
				v_sBTS=v_pmStation_OutPut_BTS->first;			//BTS
				v_sBTS_Name=v_pmStation_OutPut_BTS->second.v_sName;		//BTS_Name

				v_pCell=v_pmStation_OutPut_BTS->second.v_mCell.begin();	//ͷָ��
				while(v_pCell!=v_pmStation_OutPut_BTS->second.v_mCell.end())
				{
					//��ʾ
					v_sTemp.Format(" %04d",v_iID+1);
					v_iItem=m_cList_Station.InsertItem(0xffffff,v_sTemp);			//�����б�
					m_cList_Station.SetItemText(v_iItem,1,v_pStation_OutPut->first);//����/��Դ������
					m_cList_Station.SetItemText(v_iItem,2,v_sNetwork);				//����
					m_cList_Station.SetItemText(v_iItem,3,v_pStation_OutPut->second.v_sRegion);		//����
					m_cList_Station.SetItemText(v_iItem,4,v_sBTS);					//BTS
					m_cList_Station.SetItemText(v_iItem,5,v_pCell->first);			//Cell
					m_cList_Station.SetItemText(v_iItem,6,v_pCell->second);			//����
					if (!v_pCell->second.IsEmpty() && v_pStation_OutPut->second.v_sRegion==v_pCell->second)		//����ͬ������
						m_cList_Station.SetItemText(v_iItem,7,"��");				//����˲�

					v_pCell++;											//������++
				}

				v_pmStation_OutPut_BTS++;								//������++
			}
		}
		//TD NodeB
		if (v_pStation_OutPut->second.v_mTD.size()>0)					//TD NodeB
		{
			v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mTD.begin();			//ͷ������
			while(v_pmStation_OutPut_BTS!=v_pStation_OutPut->second.v_mTD.end())	//β������
			{
				v_sBTS=v_pmStation_OutPut_BTS->first;					//BTS

				v_pCell=v_pmStation_OutPut_BTS->second.v_mCell.begin();	//ͷָ��
				while(v_pCell!=v_pmStation_OutPut_BTS->second.v_mCell.end())
				{
					//��ʾ
					v_sTemp.Format(" %04d",v_iID+1);
					v_iItem=m_cList_Station.InsertItem(0xffffff,v_sTemp);			//�����б�
					m_cList_Station.SetItemText(v_iItem,1,v_pStation_OutPut->first);//����/��Դ������
					m_cList_Station.SetItemText(v_iItem,2,v_sNetwork);				//����
					m_cList_Station.SetItemText(v_iItem,3,v_pStation_OutPut->second.v_sRegion);		//����
					m_cList_Station.SetItemText(v_iItem,4,v_sBTS);					//BTS
					m_cList_Station.SetItemText(v_iItem,5,v_pCell->first);			//Cell
					m_cList_Station.SetItemText(v_iItem,6,v_pCell->second);			//����
					if (!v_pCell->second.IsEmpty() && v_pStation_OutPut->second.v_sRegion==v_pCell->second)		//����ͬ������
						m_cList_Station.SetItemText(v_iItem,7,"��");				//����˲�

					v_pCell++;											//������++
				}

				v_pmStation_OutPut_BTS++;								//������++
			}
		}
		//LTE E_NodeB
		if (v_pStation_OutPut->second.v_mLTE.size()>0)					//LTE E_NodeB
		{
			v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mLTE.begin();		//ͷ������
			while(v_pmStation_OutPut_BTS!=v_pStation_OutPut->second.v_mLTE.end())	//β������
			{
				v_sBTS=v_pmStation_OutPut_BTS->first;					//BTS

				v_pCell=v_pmStation_OutPut_BTS->second.v_mCell.begin();	//ͷָ��
				while(v_pCell!=v_pmStation_OutPut_BTS->second.v_mCell.end())
				{
					//��ʾ
					v_sTemp.Format(" %04d",v_iID+1);
					v_iItem=m_cList_Station.InsertItem(0xffffff,v_sTemp);			//�����б�
					m_cList_Station.SetItemText(v_iItem,1,v_pStation_OutPut->first);//����/��Դ������
					m_cList_Station.SetItemText(v_iItem,2,v_sNetwork);				//����
					m_cList_Station.SetItemText(v_iItem,3,v_pStation_OutPut->second.v_sRegion);		//����
					m_cList_Station.SetItemText(v_iItem,4,v_sBTS);					//BTS
					m_cList_Station.SetItemText(v_iItem,5,v_pCell->first);			//Cell
					m_cList_Station.SetItemText(v_iItem,6,v_pCell->second);			//����
					if (!v_pCell->second.IsEmpty() && v_pStation_OutPut->second.v_sRegion==v_pCell->second)		//����ͬ������
						m_cList_Station.SetItemText(v_iItem,7,"��");				//����˲�

					v_pCell++;											//������++
				}
				v_pmStation_OutPut_BTS++;								//������++
			}
		}
		v_iID++;														//������
		if (v_iID%100==0)												//��ʾ��100������
		{
			v_sTemp.Format("%d",v_iID);									//
			m_cSum.SetWindowText(v_sTemp);								//��ʾ��վַ����
		}
	}
	v_sTemp.Format("%d",v_iID);											//
	m_cSum.SetWindowText(v_sTemp);										//��ʾ��վַ����
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CCON_Station::My_ExecuteSQL(CString v_sSql)
{
	// TODO: Add your control notification handler code here
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);

		v_cAdoConn.ExecuteSQL((_bstr_t)v_sSql);							//ִ�����ݿ����

		v_cAdoConn.ExitConnect();										//�Ͽ�����
		return true;
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�ر����ݿ�

		CString v_sError = (char *)e.Description();						//��ã�������Ϣ
		MessageBox(v_sError);											//ʧ��
		return false;
	}
}



//	�������	
// 	mStation_Long		v_mStation_Long;								//��ʱ����
// 	Station_Long_Struct	v_cStation_Long;								//�ṹ
// 	CString				v_sLongitude,v_sLatitude,v_sStation;
// 	int					v_Sum=0;
// 
// 	CEarthPoint        pt1, pt2;
// 	double            dDistance;
// 
// 	//��ȡվַ��Ϣ
// 	for (int vi=0;vi<m_cList_Station.GetItemCount();vi++)				//����
// 	{
// 		v_sStation=m_cList_Station.GetItemText(vi,1);					//վַ
// 		v_sLongitude=m_cList_Station.GetItemText(vi,5);					//����
// 		v_sLatitude=m_cList_Station.GetItemText(vi,6);					//γ��
// 		v_cStation_Long.v_dLongitude=atof(v_sLongitude);				//����
// 		v_cStation_Long.v_dLatitude=atof(v_sLatitude);					//γ��
// 		v_mStation_Long.insert(pair<CString,Station_Long_Struct>(v_sStation,v_cStation_Long));	//��������
// 	}
// 
// 	//������㡾ѭ��������ÿ����վַ�ľ��롿
// 	mStation_Long::iterator		v_pStation;								//������
// 	while(v_mStation_Long.size()>0)
// 	{
// 		v_pStation=v_mStation_Long.begin();								//ͷָ��
// 		pt1.SetPos(v_pStation->second.v_dLongitude,v_pStation->second.v_dLatitude);		//���
// 		v_pStation++;													//������++
// 		while(v_pStation!=v_mStation_Long.end())						//����������
// 		{
// 			pt2.SetPos(v_pStation->second.v_dLongitude,v_pStation->second.v_dLatitude);	//�յ�
// 			dDistance = CEarthPoint::Distance(pt1, pt2);
// 
// 			if (dDistance<0.12)
// 			{
// 				v_Sum++;
// 			}
// 
// 			v_pStation++;												//������++
// 		}
// 		v_mStation_Long.erase(v_mStation_Long.begin());					//ɾ��ͷָ��
// 	}
// 
// 	v_mStation_Long.clear();											//�������

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
