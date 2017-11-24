// CON_GSM_Cell.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "CON_GSM_Cell.h"
#include "CON_GSM_Cell_Input.h"


// CCON_GSM_Cell

IMPLEMENT_DYNCREATE(CCON_GSM_Cell, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CCON_GSM_Cell::CCON_GSM_Cell()
	: CFormView(CCON_GSM_Cell::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CCON_GSM_Cell::~CCON_GSM_Cell()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CCON_GSM_Cell::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_Station);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_ELEMENT, m_cElement);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_GSM_Cell, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CCON_GSM_Cell::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_ELEMENT, &CCON_GSM_Cell::OnBnClickedElement)
	ON_BN_CLICKED(IDC_EXCEL2, &CCON_GSM_Cell::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_COMMAND(ID_CON_GSM_CELL_DEL, &CCON_GSM_Cell::OnConGsmCellDel)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CCON_GSM_Cell::OnNMRClickList3)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CCON_GSM_Cell::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CCON_GSM_Cell::OnLvnColumnclickList3)
END_MESSAGE_MAP()


// CCON_GSM_Cell ���

#ifdef _DEBUG
void CCON_GSM_Cell::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCON_GSM_Cell::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCON_GSM_Cell ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CCON_GSM_Cell::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="GSM Cell ����";										//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б� [վַ]
	::SendMessage(m_cList_Station.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Station.InsertColumn(0,"���",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(1,"BSC",LVCFMT_LEFT,80);				//
	m_cList_Station.InsertColumn(2,"Cell",LVCFMT_LEFT,80);				//
	m_cList_Station.InsertColumn(3,"BTS",LVCFMT_LEFT,80);				//
	m_cList_Station.InsertColumn(4,"������",LVCFMT_LEFT,200);			//
	m_cList_Station.InsertColumn(5,"LAC",LVCFMT_LEFT,120);				//
	m_cList_Station.InsertColumn(6,"����",LVCFMT_LEFT,80);				//
	m_cList_Station.InsertColumn(7,"��������/��Դ��",LVCFMT_LEFT,260);	//
	m_cList_Station.InsertColumn(8,"��������",LVCFMT_LEFT,70);			//
	m_cList_Station.InsertColumn(9,"����",LVCFMT_RIGHT,100);			//
	m_cList_Station.InsertColumn(10,"γ��",LVCFMT_RIGHT,100);			//
	m_cList_Station.InsertColumn(11,"���߷���",LVCFMT_RIGHT,100);		//
	m_cList_Station.InsertColumn(12,"���߸߶�",LVCFMT_RIGHT,100);		//
	m_cList_Station.InsertColumn(13,"�������",LVCFMT_RIGHT,100);		//
	m_cList_Station.InsertColumn(14,"״̬",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(15,"����ʱ��",LVCFMT_CENTER,100);		//
	m_cList_Station.SetExtendedStyle(m_cList_Station.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��Ϣ��ʼ��
	m_cKey_Select.AddString("Cell");
	m_cKey_Select.AddString("BTS");
	m_cKey_Select.AddString("������");
	m_cKey_Select.AddString("����/��Դ��");
	m_cKey_Select.SetCurSel(0);											//Ĭ�ϣ�ѡ���һ��

	v_iList_Item=-1;													//��Ŀѡ����Ч
	v_iSum=0;															//��������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CCON_GSM_Cell::PreTranslateMessage(MSG* pMsg)
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
void CCON_GSM_Cell::OnDestroy()
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
void CCON_GSM_Cell::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CWnd	*pCellList;													//��ʱ����
	int		v_iY;														//��ʱ����

	pCellList = GetDlgItem(IDC_LIST3);
	v_iY = 50;															//��ֵ��Y��
	if(pCellList)
		pCellList->MoveWindow(14,v_iY,cx-24,cy-v_iY-10);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�վַ����
//------------------------------------------------------------------------------------------------------
void CCON_GSM_Cell::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Station);										//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�վַ����
//------------------------------------------------------------------------------------------------------
void CCON_GSM_Cell::OnBnClickedElement()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCON_GSM_Cell_Input	v_cDlg;											//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����Ӧ
//------------------------------------------------------------------------------------------------------
void CCON_GSM_Cell::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
void CCON_GSM_Cell::OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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

	aMenu.LoadMenu(IDR_GSM_CELL);									//װ�أ�IDR_GSM_BTS�˵�
	CMenu *pPopup =aMenu.GetSubMenu(0);								//���ɸ����˵�
	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);		//��ʾ���������λ��[point]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ɾ��С��
//------------------------------------------------------------------------------------------------------
void CCON_GSM_Cell::OnConGsmCellDel()
{
	// TODO: �ڴ���������������
	int		v_iItem,vi;													//��ʱ����
	CString	v_sCell,v_sTemp,v_sSql;										//��ʱ����

	vi = m_cList_Station.GetSelectedCount();							//ѡ����Ŀ����
	v_sTemp.Format("%d",vi);											//�ַ���
	if(MessageBox("ɾ��ѡ�е� "+v_sTemp+" �� GSM Cell�� ɾ��ȷ�� ? ","����ɾ��ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)	//ɾ��ȷ�ϣ�����
	{
		//������ѡ�е�Item���в���
		while ((v_iItem = m_cList_Station.GetNextItem(-1, LVNI_SELECTED)) != -1)//��Ŀѡ�У�����
		{
			v_sCell=m_cList_Station.GetItemText(v_iItem,2);				//��ã�v_sCell [���ݱ�Ψһ��ʶ�������޸����ݱ�]

			//ɾ����Cell
			v_sSql = "delete from GSM_Cell where zCell='"+v_sCell+"'";	//SQLɾ��
			My_ExecuteSQL(v_sSql);										//д��

			//ɾ�����б���Ϣ
			m_cList_Station.DeleteItem(v_iItem);						//ɾ����ѡ�е�v_iItem

			//���棺Log
			COleDateTime v_tTime=COleDateTime::GetCurrentTime();
			CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");
			CString v_sSql="Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"+v_sCurrentTime+"','ɾ��Cell','Cell���ƣ�"
				+v_sCell+"������Ա��"+theApp.a_sUser_Name+"')";
			My_ExecuteSQL(v_sSql);										//���ݿ�ִ��
		}
		v_iSum-=vi;														//ʣ������
		v_sTemp.Format("%d",v_iSum);									//�ַ���
		m_cSum.SetWindowText(v_sTemp);									//��ʾ��վַ����
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б����� [վַ]
//------------------------------------------------------------------------------------------------------
void CCON_GSM_Cell::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList_Station.My_Sort(pNMHDR);										//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ
//------------------------------------------------------------------------------------------------------
void CCON_GSM_Cell::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CCON_GSM_Cell::My_Thread_Inoput(LPVOID lparam)
{
	CCON_GSM_Cell *  lp_this = NULL ;
	lp_this = (CCON_GSM_Cell *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel_NodeB.EnableWindow(false);						//NodeB��������Ч
	lp_this->m_cElement.EnableWindow(false);							//վַ���룺��Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel_NodeB.EnableWindow(true);							//NodeB��������Ч
	lp_this->m_cElement.EnableWindow(true);								//վַ���룺��Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CCON_GSM_Cell::My_Input_Main()
{
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
void CCON_GSM_Cell::My_Query_Station()
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

	v_sSql += "SELECT GSM_Cell.*, GSM_BTS.zBTS_NAME,GSM_BTS.zBSC FROM GSM_Cell \
			  left JOIN GSM_BTS ON GSM_Cell.zBTS = GSM_BTS.zBTS"; 

	if (!v_sLog_Key.IsEmpty())											//��ѯKEY����
	{
		switch (m_cKey_Select.GetCurSel())								//ѡ�񣿣���
		{
		case 0:															//Cell
			v_sSql+=" where zcell like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//BTS
			v_sSql+=" where GSM_Cell.zBTS like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//������
			v_sSql+=" where zBTS_NAME like '%"+v_sLog_Key+"%'";
			break;
		case 3:															//��������/��Դ��
			v_sSql+=" where GSM_Cell.zStation like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	v_sSql+=" order by zcell";

	My_LoadData_Station_Data(v_sSql);									//��ȡ��վַ��Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��վַ��Ϣ
//------------------------------------------------------------------------------------------------------			
void CCON_GSM_Cell::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem,v_iID=0;									//��ʱ����
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����

	_variant_t	Var;
	CString			v_sStation,v_sCell,v_sBTS,v_sRegion,v_sBTS_Name,v_sLongitude;		//��ʱ����
	CString			v_sLatitude,v_sOnline,v_sDate,v_sBSC,v_sType,v_sLAC;//��ʱ����
	double			v_dValue;											//��ʱ����
	int				v_iAnt_High,v_iAnt_Angle,v_iAnt_Dip;				//���߲���

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList_Station.DeleteAllItems();								//���Items
		v_iList_Item=-1;												//��Ŀѡ����Ч

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sBSC = v_cAdoConn.GetField_String("zBSC");				//zBSC
			v_sCell = v_cAdoConn.GetField_String("zcell");				//zcell
			v_sBTS = v_cAdoConn.GetField_String("zBTS");				//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sLAC = v_cAdoConn.GetField_String("zLAC");				//zLAC
			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_dValue = v_cAdoConn.GetField_Double("zLongitude");		//zLongitude
			v_sLongitude.Format("%10.6f",v_dValue);
			v_dValue = v_cAdoConn.GetField_Double("zLatitude");			//zLatitude
			v_sLatitude.Format("%10.6f",v_dValue);
			v_iAnt_Angle=v_cAdoConn.GetField_Int("zAnt_Angle");			//zAnt_Angle
			v_iAnt_High=v_cAdoConn.GetField_Int("zAnt_High");			//zAnt_High
			v_iAnt_Dip=v_cAdoConn.GetField_Int("zAnt_Dip");				//zAnt_Dip
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			Var=v_pRecordset->GetCollect((_variant_t)"zDate");			//zDate����ʱ��
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sDate = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sDate="";

			//��ʾ
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList_Station.InsertItem(0xffff,v_sTemp);			//�����б�
			m_cList_Station.SetItemText(v_iItem,1,v_sBSC);				//BSC
			m_cList_Station.SetItemText(v_iItem,2,v_sCell);				//Cell
			m_cList_Station.SetItemText(v_iItem,3,v_sBTS);				//BTS
			m_cList_Station.SetItemText(v_iItem,4,v_sBTS_Name);			//������
			m_cList_Station.SetItemText(v_iItem,5,v_sLAC);				//LAC
			m_cList_Station.SetItemText(v_iItem,6,v_sRegion);			//����
			m_cList_Station.SetItemText(v_iItem,7,v_sStation);			//��������/��Դ��
			m_cList_Station.SetItemText(v_iItem,8,v_sType);				//��������
			m_cList_Station.SetItemText(v_iItem,9,v_sLongitude);		//����
			m_cList_Station.SetItemText(v_iItem,10,v_sLatitude);		//γ��
			v_sTemp.Format("%d",v_iAnt_Angle);							//
			m_cList_Station.SetItemText(v_iItem,11,v_sTemp);			//���߷���
			v_sTemp.Format("%d",v_iAnt_High);							//
			m_cList_Station.SetItemText(v_iItem,12,v_sTemp);			//���߸߶�
			v_sTemp.Format("%d",v_iAnt_Dip);							//
			m_cList_Station.SetItemText(v_iItem,13,v_sTemp);			//�������
			m_cList_Station.SetItemText(v_iItem,14,v_sOnline);			//״̬
			m_cList_Station.SetItemText(v_iItem,15,v_sDate);			//��������

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

		v_iSum=v_iID;													//����

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
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CCON_GSM_Cell::My_ExecuteSQL(CString v_sSql)
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

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
