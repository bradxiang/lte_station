// Alarm_PM.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Alarm_PM.h"

// CAlarm_PM

IMPLEMENT_DYNCREATE(CAlarm_PM, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CAlarm_PM::CAlarm_PM()
	: CFormView(CAlarm_PM::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CAlarm_PM::~CAlarm_PM()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_LOG_SUM, m_cLog_Sum);
	DDX_Control(pDX, IDC_BEG_DATE, m_cLog_Date);
	DDX_Control(pDX, IDC_COMBO1, m_cLog_Day);
	DDX_Control(pDX, IDC_KEY, m_cLog_Key);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_COMBO2, m_cType);
	DDX_Control(pDX, IDC_CHECK1, m_cReal);
	DDX_Control(pDX, IDC_CHECK4, m_cTime);
	DDX_Control(pDX, IDC_LIST9, m_cList_PM);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAlarm_PM, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CAlarm_PM::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL, &CAlarm_PM::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CAlarm_PM::OnLvnColumnclickList1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_EXCEL2, &CAlarm_PM::OnBnClickedExcel2)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST9, &CAlarm_PM::OnLvnColumnclickList9)
	ON_COMMAND(ID_ALARM_PM_CELL, &CAlarm_PM::OnAlarmPmCell)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CAlarm_PM::OnNMClickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CAlarm_PM::OnNMRClickList1)
	ON_BN_CLICKED(IDC_EXCEL3, &CAlarm_PM::OnBnClickedExcel3)
END_MESSAGE_MAP()


// CAlarm_PM ���

#ifdef _DEBUG
void CAlarm_PM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAlarm_PM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAlarm_PM ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="����Ԥ����ѯ";										//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б���Ԥ����
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"��    ��",LVCFMT_CENTER,120);  
	m_cList.InsertColumn(2,"����",LVCFMT_LEFT,140);  
	m_cList.InsertColumn(3,"Ԥ������",LVCFMT_LEFT,140);  
	m_cList.InsertColumn(4,"Ԥ������",LVCFMT_LEFT,80);  
	m_cList.InsertColumn(5,"���ܲ���",LVCFMT_RIGHT,80);  
	m_cList.InsertColumn(6,"Ԥ������",LVCFMT_RIGHT,80);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList.SetNumber(5);												//��������
	m_cList.SetNumber(6);												//��������

	//��ʼ�����б�������Top20��
	m_cList_PM.ModifyStyle( 0, LVS_REPORT|LVS_SHOWSELALWAYS, TRUE );
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"���",LVCFMT_CENTER,60);
	m_cList_PM.InsertColumn(1,"С��",LVCFMT_LEFT,200);					//��1�У�С��
	m_cList_PM.InsertColumn(2,"Ӫҵ��",LVCFMT_LEFT,60);					//��2�У�Ӫҵ��
	m_cList_PM.InsertColumn(3,"״̬",LVCFMT_LEFT,40);					//��3�У�״̬
	m_cList_PM.InsertColumn(4,"����",LVCFMT_CENTER,120);				//��4�У�����
	m_cList_PM.InsertColumn(5,"����",LVCFMT_LEFT,100);					//��5�У�����
	m_cList_PM.InsertColumn(6,"����",LVCFMT_RIGHT,80);					//��6�У�����
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList_PM.SetNumber(6);											//��������

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
		&m_cList_PM,			// Right pane
		IDC_Alarm_PM,			// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);
	//������ʼ��
	m_cLog_Day.SetCurSel(4);											//Ĭ�ϣ���ѯ����

	SetTimer(1,60000,NULL);												//1 mins��ʱ 

	v_dAlarm_Date=COleDateTime::GetCurrentTime();						//��ȡ���澯ʱ��

	//ѡ��
	m_cType.AddString("����");
	m_cType.AddString("Ԥ������");
	m_cType.SetCurSel(0);												//Ĭ��

	v_iList_Item=-1;													//��Ŀѡ����Ч

	//ˢ�½���
	PostMessage(WM_SIZE);

	//��ȡ����ʼ����
	My_LoadData_Para();													//��ȡ����ʼ����
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ӵ�����
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnDestroy()
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
void CAlarm_PM::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//�ִ��ߴ����
		CRect v_cRect;
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=40;												//���ܲ�ѯ��80(��ʾ��ѯ����)��������40
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CAlarm_PM::PreTranslateMessage(MSG* pMsg)
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
void CAlarm_PM::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_dTime;											//��ʱ����

	if (m_cReal.GetCheck())												//��Ч��ʵʱ������
	{
		v_dTime=My_LoadData_Time();										//��ȡ����������
		if (v_dTime!=v_dPM_Date)										//�������ȱ仯������
		{
			v_bAlarm=true;												//��Ч���¸澯��ʾ
			v_dPM_Date=v_dTime;											//��ȡ����ʱ��
			My_Begin();													//��ʼ��ѯ
		}
	}

	CFormView::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������ѯ��ť
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	v_bAlarm=false;														//��Ч���¸澯��ʾ
	My_Begin();															//��ʼ��ѯ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ʼ��ѯ
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::My_Begin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��1����ȡ����ѯ����
	v_dPM_Date=My_LoadData_Time();										//��ȡ����������

	//��2����ʼ��ѯ(�߳�)
	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Excel
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Excel
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnBnClickedExcel3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_PM);											//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����(�һ��б�������)��Ԥ����Ϣ��
//------------------------------------------------------------------------------------------------------			
void CAlarm_PM::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����(�һ��б�������)��������ϸ��
//------------------------------------------------------------------------------------------------------			
void CAlarm_PM::OnLvnColumnclickList9(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList_PM.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����Ӧ
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б��һ���Ӧ
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu	aMenu;														//�˵�
	CPoint	point;														//λ�õ�

	GetCursorPos(&point);												//��ã����λ��
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã�������б�λ��
	int v_iSelect = pNMListView->iItem;									//��ȡ������б���Item
	if( v_iSelect == -1 )												//��ЧItem������
		return;															//��Ч������
	if(v_iList_Item>=0)													//��Ч��Ŀ��ţ�����
		m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));			//�ָ�����ɫ
	v_iList_Item=v_iSelect;												//��ã�ѡ����Ŀ���
	m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));					//���ñ���ɫ����ɫ

	aMenu.LoadMenu(IDR_ALARM_PM);										//װ�أ��Ҽ��˵�
	CMenu *pPopup =aMenu.GetSubMenu(0);									//���ɸ����˵�
	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);			//��ʾ���������λ��[point]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܲ�ѯ
//	��    �ݣ������Ҽ�ѡ���ж�Ԥ�����ͣ�����==��My_LoadData_Region��С��==��My_LoadData_Cell
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnAlarmPmCell()
{
	CString v_sType=m_cList.GetItemText(v_iList_Item,4);				//Ԥ������

	if (v_sType=="����")
		My_LoadData_Region();											//���ܲ�ѯ������
	else if (v_sType=="С������" || v_sType=="С��Ԥ��")
		My_LoadData_Cell();												//���ܲ�ѯ��С��
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܲ�ѯ������Top 30��
//	��    �ݣ��������ƣ���������15m���ȣ��������ͣ�����Ԥ������
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::My_LoadData_Region()
{
	CString			v_sTemp,v_sCell,v_sTime,v_sPm_Name,v_sRegion,v_sOnline;		//��ʱ����
	bool			v_bPM=false;										//��ʱ����
	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����
	CString			v_sTable,v_sSql,v_sAlarm_Type;						//����
	double			v_dValue;											//��ʱ����

	//��1�����ݳ�ʼ��
	m_cList_PM.DeleteAllItems();										//����б�����

	v_sTime=m_cList.GetItemText(v_iList_Item,1);						//ʱ��
	v_sTime.Replace("_"," ");											//�滻
	v_sPm_Name=m_cList.GetItemText(v_iList_Item,2);						//����
	v_sRegion=m_cList.GetItemText(v_iList_Item,3);						//Ԥ����������

	//��2����ȡ���ܱ�����(15m���ȣ�ÿ��һ����)
	v_dDate_Start.SetDateTime(atoi(v_sTime.Left(4)),atoi(v_sTime.Mid(5,2)),atoi(v_sTime.Mid(8,2)),atoi(v_sTime.Mid(11,2)),atoi(v_sTime.Mid(14,2)),0);	//����ʱ��
	v_sTemp=v_dDate_Start.Format("%Y%m%d");								//������
	v_sTable = "Pm_LTE_Cal_"+v_sTemp;									//�����ơ��������ܱ�

	//��3����飺�������Ƿ���Ч�����ݣ������Ƽ���v_mTable_Name���������������Ч����鲻���������ݡ���
	set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);		//���ң���
	if (v_pTable==v_mTable_Name.end())									//û�ҵ������ݱ�
		return;															//��Ч����

	//��4����飺�����Ƿ���Ч�����ݣ����ܼ���v_mLTE_PM_Type��
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pLTE_PM_Name;								//������

	v_pLTE_PM_Type=v_mLTE_PM_Type.find("��������");						//���ң���������
	if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())							//û�ҵ�������
		return;															//��Ч����
	v_pLTE_PM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.find(v_sPm_Name);	//���ң���������
	if (v_pLTE_PM_Name==v_pLTE_PM_Type->second.v_mLTE_PM.end())			//û�ҵ�������
		return;															//��Ч����
	v_sAlarm_Type=v_pLTE_PM_Name->second.v_sAlarm_Type;					//��ȡ���澯����

	//��5���������ݻ�ȡ
	_variant_t		Var;
	_RecordsetPtr	v_pRecordset;										//��¼��
	CMyADOConn		v_cAdoConn;											//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select LTE_Cell.zCell,LTE_Cell.zRegion,LTE_Cell.zOnline,"+v_sPm_Name+" from "+v_sTable+" left join LTE_Cell ON "+v_sTable
				+".zCell= LTE_Cell.zCell_ID where zTime='"+v_sTime+"' and LTE_Cell.zRegion='"+v_sRegion
				+"' order by "+v_sPm_Name;								//SQL
		if (v_sAlarm_Type=="1")											//����Ч������
			v_sSql+=" asc";												//SQL
		else															//����Ч
			v_sSql+=" desc";											//SQL

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_sCell=Var.bstrVal;									//��ȡ��Cell
				v_dValue=v_cAdoConn.GetField_Double(v_sPm_Name);		//��ȡ������
				v_sRegion=v_cAdoConn.GetField_String("zRegion");		//��ȡ��Ӫҵ��
				v_sOnline=v_cAdoConn.GetField_String("zOnline");		//��ȡ��״̬

				if (v_dValue!=-1)
				{
					//��6����ʾ
					int	v_iItem = m_cList_PM.InsertItem(0xffff,_T("0"));
					v_sTemp.Format("%04d",v_iItem+1);
					m_cList_PM.SetItemText(v_iItem,0," "+v_sTemp);		//���
					m_cList_PM.SetItemText(v_iItem,1,v_sCell);			//С��
					m_cList_PM.SetItemText(v_iItem,2,v_sRegion);		//��������
					m_cList_PM.SetItemText(v_iItem,3,v_sOnline);		//Ԥ������
					m_cList_PM.SetItemText(v_iItem,4,v_sTime);			//ʱ��
					m_cList_PM.SetItemText(v_iItem,5,v_sPm_Name);		//����
					v_sTemp.Format("%8.2f",v_dValue);					//�ַ���
					m_cList_PM.SetItemText(v_iItem,6,v_sTemp);			//���ܲ���
				}
				if (m_cList_PM.GetItemCount()>=30)						//Top 30 ������
					break;												//�˳�
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
//	�������ܣ����ܲ�ѯ��С������ѯС����Ԥ��������������ܡ�
//	��    �ݣ��������ƣ���������15m���ȣ��������ͣ�С�����㡢С��Ԥ������
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::My_LoadData_Cell()
{
	CString			v_sTemp,v_sCell,v_sTime,v_sPm_Name,v_sRegion,v_sOnline;		//��ʱ����
	bool			v_bPM=false;										//��ʱ����
	COleDateTime	v_dDate_Start;										//��ѯ��ʼ����
	CString			v_sTable,v_sSql,v_sAlarm_Type,v_sType;				//����
	double			v_dValue;											//��ʱ����

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pLTE_PM_Name;								//������

	//��1�����ݳ�ʼ��
	m_cList_PM.DeleteAllItems();										//����б�����

	v_sTime=m_cList.GetItemText(v_iList_Item,1);						//ʱ��
	v_sTime.Replace("_"," ");											//�滻
	v_sPm_Name=m_cList.GetItemText(v_iList_Item,2);						//����
	v_sCell=m_cList.GetItemText(v_iList_Item,3);						//Ԥ������С��
	v_sType=m_cList.GetItemText(v_iList_Item,4);						//Ԥ�����ͣ�С�����㡢С��Ԥ��

	//��2����ȡ���ܱ�����(15m���ȣ�ÿ��һ����)
	v_dDate_Start.SetDateTime(atoi(v_sTime.Left(4)),atoi(v_sTime.Mid(5,2)),atoi(v_sTime.Mid(8,2)),atoi(v_sTime.Mid(11,2)),atoi(v_sTime.Mid(14,2)),0);	//����ʱ��
	v_sTemp=v_dDate_Start.Format("%Y%m%d");								//������
	if (v_sType=="С������")											//�������ܱ�
	{
		v_sTable = "Pm_LTE_Cal_"+v_sTemp;								//�����ơ��������ܱ�
		//��3����飺�����Ƿ���Ч�����ݣ����ܼ���v_mLTE_PM_Type��
		v_pLTE_PM_Type=v_mLTE_PM_Type.find("��������");					//���ң���������
		if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())						//û�ҵ�������
			return;														//��Ч����
		v_pLTE_PM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.find(v_sPm_Name);	//���ң���������
		if (v_pLTE_PM_Name==v_pLTE_PM_Type->second.v_mLTE_PM.end())		//û�ҵ�������
			return;														//��Ч����
		v_sAlarm_Type=v_pLTE_PM_Name->second.v_sAlarm_Type;				//��ȡ���澯����
	}
	else																//ԭʼ���ܱ�
	{
		//��3����飺�����Ƿ���Ч�����ݣ����ܼ���v_mLTE_PM_Type��
		for(v_pLTE_PM_Type=v_mLTE_PM_Type.begin();v_pLTE_PM_Type!=v_mLTE_PM_Type.end();v_pLTE_PM_Type++)	//����
		{
			v_pLTE_PM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.find(v_sPm_Name);	//���ң���������
			if (v_pLTE_PM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())	//�ҵ�������
				break;
		}
		if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())						//û�ҵ�������
			return;														//��Ч����
		
		//��ȡ������Ϣ
		v_sTable = "Pm_LTE_"+v_pLTE_PM_Name->second.v_sTable_Key+"_"+v_sTemp;	//�����ơ�ԭʼ���ܱ�
		v_sAlarm_Type=v_pLTE_PM_Name->second.v_sAlarm_Type;				//��ȡ���澯����
	}

	//��4����飺�������Ƿ���Ч�����ݣ������Ƽ���v_mTable_Name���������������Ч����鲻���������ݡ���
	set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);		//���ң���
	if (v_pTable==v_mTable_Name.end())									//û�ҵ������ݱ�
		return;															//��Ч����
	
	//��5��Cell_ID ==> Cell


	//��6���������ݻ�ȡ
	_variant_t		Var;
	_RecordsetPtr	v_pRecordset;										//��¼��
	CMyADOConn		v_cAdoConn;											//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select LTE_Cell.zCell,LTE_Cell.zRegion,LTE_Cell.zOnline,"+v_sTable+".zTime,["+v_sPm_Name
				+"] from "+v_sTable+" left join LTE_Cell ON "+v_sTable
				+".zCell= LTE_Cell.zCell_ID where LTE_Cell.zCell='"+v_sCell+"' order by zTime asc";	//SQL

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_sCell=Var.bstrVal;									//��ȡ��Cell
				v_dValue=v_cAdoConn.GetField_Double(v_sPm_Name);		//��ȡ������
				v_sRegion=v_cAdoConn.GetField_String("zRegion");		//��ȡ��Ӫҵ��
				v_sOnline=v_cAdoConn.GetField_String("zOnline");		//��ȡ��״̬
				v_dDate_Start=v_cAdoConn.GetField_DataTime("zTime");	//��ȡ��ʱ��
				v_sTime=v_dDate_Start.Format("%Y-%m-%d_%H:%M:%S");		//�ַ���
		
				if (v_dValue!=-1)
				{
					//��6����ʾ
					int	v_iItem = m_cList_PM.InsertItem(0xffff,_T("0"));
					v_sTemp.Format("%04d",v_iItem+1);
					m_cList_PM.SetItemText(v_iItem,0," "+v_sTemp);		//���
					m_cList_PM.SetItemText(v_iItem,1,v_sCell);			//С��
					m_cList_PM.SetItemText(v_iItem,2,v_sRegion);		//��������
					m_cList_PM.SetItemText(v_iItem,3,v_sOnline);		//Ԥ������
					m_cList_PM.SetItemText(v_iItem,4,v_sTime);			//ʱ��
					m_cList_PM.SetItemText(v_iItem,5,v_sPm_Name);		//����
					v_sTemp.Format("%8.2f",v_dValue);					//�ַ���
					m_cList_PM.SetItemText(v_iItem,6,v_sTemp);			//���ܲ���
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
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Ԥ��ȷ��
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_dAlarm_Date=COleDateTime::GetCurrentTime();						//��ȡ���澯ʱ��
	OnBnClickedQuary();													//Ԥ����ѯ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CAlarm_PM::My_Thread_Inoput(LPVOID lparam)
{
	CAlarm_PM *  lp_this = NULL ;
	lp_this = (CAlarm_PM *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(false);								//��������Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(true);								//��������Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::My_Input_Main()
{
	m_cList_PM.DeleteAllItems();										//����б�����
	m_cList.DeleteAllItems();											//����б�����
	v_iList_Item=-1;													//��Ŀѡ����Ч
	My_LoadData();														//װ��Bug��Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��Ԥ����Ϣ
//------------------------------------------------------------------------------------------------------			
void CAlarm_PM::My_LoadData()
{
	CString			v_sSql,v_sTemp,v_sLog_Key,v_sAlarm_Type;			//��ʱ����
	int				v_iItem,v_iID=0;									//��ʱ����
	int				v_iDay_Sum;											//��ѯ����
	CString			v_sTime_Start,v_sTime_End;							//����
	CString			v_sAlarm_PM,v_sAlarm_Object,v_sAlarm_Value,v_sAlarm_Level,v_sAlarm_Content;	//Ԥ������
	double			v_dValue;											//��ֵ
	COleDateTime	v_dDate_Start,v_dDate_End;							//��ѯ��ʼ���ڡ���������
	bool			v_bMessage=false;									//��ʱ����
	int				v_iAlarm_Sum=0;										//Ԥ������
	

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		//��ѯ����׼��
		m_cLog_Date.GetTime(v_dDate_Start);								//��ã���ѯ��ʼ����
		v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//����ʱ�䣺00:00:00
		v_iDay_Sum=m_cLog_Day.GetCurSel()+1;							//��ã���ѯ����
		v_dDate_End =v_dDate_Start + COleDateTimeSpan(v_iDay_Sum,0,0,0);//��ã���ѯ��������
		v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";		//��ʼʱ��
		v_sTime_End=v_dDate_End.Format("%Y-%m-%d")+" 00:00:00";			//����ʱ��
		m_cLog_Key.GetWindowText(v_sLog_Key);							//��ã���ѯKEY
		v_sLog_Key.Replace("[","[[]");									//���ƴ���
		v_sLog_Key.Replace("'","");										//���ƴ���
		v_sLog_Key.Trim();												//ȥ���ո�

		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql = "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select * from PM_Alarm where zTime>='"+v_sTime_Start+"' and zTime<='"+v_sTime_End+"'";	//��ѯ
		if (!v_sLog_Key.IsEmpty())										//�йؼ��ֲ�ѯ������
		{
			switch(m_cType.GetCurSel())
			{
			case 0:
				v_sSql += " and zAlarm_PM like '%"+v_sLog_Key+"%'";		//����
				break;
			case 1:
				v_sSql += " and zAlarm_Object like '%"+v_sLog_Key+"%'";		//����
				break;
			}
		}

		v_sSql += " order by zTime";									//����ʱ��
		if (m_cTime.GetCheck())											//��Ч�����򣿣���
			v_sSql += " desc";											//����

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_dDate_Start=v_cAdoConn.GetField_DataTime("zTime");		//zTime
			v_sAlarm_PM=v_cAdoConn.GetField_String("zAlarm_PM");		//zAlarm_PM
			v_sAlarm_Object=v_cAdoConn.GetField_String("zAlarm_Object");//zAlarm_Object
			v_sAlarm_Type=v_cAdoConn.GetField_String("zAlarm_Type");	//zAlarm_Type
			v_dValue=v_cAdoConn.GetField_Double("zAlarm_Value");		//zAlarm_Value
			v_sAlarm_Value.Format("%8.2f",v_dValue);
			v_dValue=v_cAdoConn.GetField_Double("zAlarm_Level");		//zAlarm_Level
			v_sAlarm_Level.Format("%8.2f",v_dValue);
			v_sAlarm_Content=v_cAdoConn.GetField_String("zAlarm_Content");	//zAlarm_Content

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d_%H:%M:%S");	//��ֵ������
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sTime_Start);		//
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sAlarm_PM);		//
			m_cList.SetItemText(v_iItem,3,(_bstr_t)v_sAlarm_Object);	//
			m_cList.SetItemText(v_iItem,4,(_bstr_t)v_sAlarm_Type);		//
			m_cList.SetItemText(v_iItem,5,(_bstr_t)v_sAlarm_Value);		//
			m_cList.SetItemText(v_iItem,6,(_bstr_t)v_sAlarm_Level);		//
			m_cList.SetItemText(v_iItem,7,(_bstr_t)v_sAlarm_Content);	//

			if (v_dAlarm_Date<(v_dDate_Start+COleDateTimeSpan(0,0,30,0)))	//Ԥ�� && Ԥ��ʱ��<����ʱ��
			{
				m_cList.SetItemBkColor(v_iItem,RGB(255,0,0));			//
				v_bMessage=true;										//��Ч����Ϣ
				v_iAlarm_Sum++;											//Ԥ������++
			}

			v_pRecordset->MoveNext();									//
			v_iID++;													//

			if (v_iID%100==0)
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cLog_Sum.SetWindowText(v_sTemp);						//��ʾ��Bug����
			}

		}
		v_sTemp.Format("%d",v_iID);										//
		m_cLog_Sum.SetWindowText(v_sTemp);								//��ʾ��Bug����

		if (v_bAlarm && v_bMessage)
		{
			v_sTemp.Format("%d",v_iAlarm_Sum);							//Ԥ������
			theApp.a_pDlg_Alarm_Message->m_cSum.SetWindowText("���� "+v_sTemp+" ��Ԥ��");	//Ԥ����Ϣ
			theApp.a_pDlg_Alarm_Message->ShowWindow(SW_SHOW);			//��ʾԤ������
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
//	�������ܣ���ȡ��������
//------------------------------------------------------------------------------------------------------			
COleDateTime CAlarm_PM::My_LoadData_Time()
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
			v_sTime = v_cAdoConn.GetField_DataTime("zPm_LTE_Time_Data");//zPm_LTE_Time_Data [��������]
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

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ�����ܲ�����Ϣ
//------------------------------------------------------------------------------------------------------			
void CAlarm_PM::My_LoadData_Para()
{
	int				v_iID=0,v_iAlarm_Type;								//��ʱ����
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

		//��1����ȡ�����ܲ��������桿
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE order by ztype,ztype_1,zPm";//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sType_1 = v_cAdoConn.GetField_String("ztype_1");			//ztype_1
			v_sPm = v_cAdoConn.GetField_String("zPm");					//zPm
			v_sPm_Name = v_cAdoConn.GetField_String("zPm_Name");		//zPm_Name
			v_sTable_Key = v_cAdoConn.GetField_String("zTable_Key");	//zTable_Key

			//����
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//���ң���������
			if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())					//û�ҵ�������
			{
				v_cLTE_PM_Type.v_sTable_Key=v_sTable_Key;				//������-Key
				v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//��������
			}
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//���ң���������
			v_cLTE_PM.v_sTable_Key=v_sTable_Key;						//������-Key
			v_cLTE_PM.v_sPM_Name=v_sPm_Name;							//���ܣ�������
			v_cLTE_PM.v_bOK=false;										//���ܣ�δѡ��
			v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//���ӣ����ܲ���

			v_pRecordset->MoveNext();									//
		}

		//��2����ȡ�����ܲ������������ܡ�
		v_sType="��������";												//��������								
		v_cLTE_PM_Type.v_sTable_Key = "Cal";							//������-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//��������

		//��ȡ�����ܲ������������ܡ�
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Cal order by zPm";			//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sPm = v_cAdoConn.GetField_String("zPm");					//zPm
			v_iAlarm_Type = v_cAdoConn.GetField_Int("zAlarm_Type");		//zAlarm_Type

			//����
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//���ң���������
			if (v_pLTE_PM_Type!=v_mLTE_PM_Type.end())					//�ҵ�������
			{
				v_cLTE_PM.v_sPM_Name=v_sPm;								//���ܣ�������
				v_cLTE_PM.v_sTable_Key="Cal";							//���ܣ�������
				v_cLTE_PM.v_bOK=false;									//���ܣ�δѡ��
				v_sTemp.Format("%d",v_iAlarm_Type);						//�ַ���
				v_cLTE_PM.v_sAlarm_Type=v_sTemp;						//���ܣ�Ԥ������
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//���ӣ����ܲ���
			}

			v_pRecordset->MoveNext();									//
		}

		//��3����ȡ���������ݱ����ơ����ڷ�����Ч�����ơ�
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_LTE_%') order by [name]";		//SQL
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

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
