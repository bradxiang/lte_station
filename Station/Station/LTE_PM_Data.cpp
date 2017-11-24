// LTE_PM_Data.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Data.h"

// CLTE_PM_Data

IMPLEMENT_DYNCREATE(CLTE_PM_Data, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data::CLTE_PM_Data()
	: CFormView(CLTE_PM_Data::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data::~CLTE_PM_Data()
{
	v_mLTE_PM_Type.clear();												//�������
	v_mTable_Name.clear();												//�������
	v_mRegion.clear();													//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_LOG_SUM, m_cLog_Sum);
	DDX_Control(pDX, IDC_BEG_DATE, m_cLog_Date);
	DDX_Control(pDX, IDC_COMBO1, m_cLog_Day);
	DDX_Control(pDX, IDC_KEY, m_cLog_Key);
	DDX_Control(pDX, IDC_BUTTON1, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_LIST3, m_cList_PM);
	DDX_Control(pDX, IDC_COMBO3, m_cPM_Type);
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_LOG_SUM2, m_cPM_Sum);
	DDX_Control(pDX, IDC_LOG_SUM3, m_cData_Sum);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Data, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL, &CLTE_PM_Data::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CLTE_PM_Data::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Data::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Data::OnNMClickList1)
	ON_CBN_CLOSEUP(IDC_COMBO3, &CLTE_PM_Data::OnCbnCloseupCombo3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CLTE_PM_Data::OnLvnColumnclickList3)
END_MESSAGE_MAP()


// CLTE_PM_Data ���

#ifdef _DEBUG
void CLTE_PM_Data::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLTE_PM_Data::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLTE_PM_Data ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	CString v_sTemp;													//��ʱ����
	//��������
	v_sFrame_Name="LTE �������ݲ�ѯ";									//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б���С����
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);
	m_cList.InsertColumn(1,"С��",LVCFMT_LEFT,200);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	m_cList.InsertColumn(2,"С��ID",LVCFMT_LEFT,0);  

	//��ʼ�����б������ܡ�
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"���",LVCFMT_CENTER,60);
	m_cList_PM.InsertColumn(1,"ʱ��",LVCFMT_CENTER,140);  
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

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
		IDC_LTE_PM_Data_Main,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);

	//ˢ�½���
	PostMessage(WM_SIZE);

	//������ʼ��
	m_cType.AddString("15m����");
	m_cType.AddString("1H����");
	m_cType.SetCurSel(0);												//Ĭ�ϣ�15m����

	//��ʼ��������ѡ��
	for (int vi=1;vi<=c_iSample_Day;vi++)								//�����
	{
		v_sTemp.Format("%d",vi);
		m_cLog_Day.AddString(v_sTemp);									//��ѡ������ѡť
	}
	m_cLog_Day.SetCurSel(0);											//Ĭ�ϲ�ѯ������1�죻

	m_cLog_Day.SetCurSel(4);											//Ĭ�ϣ���ѯ����
	v_iList_Item=-1;													//Ĭ�ϣ���ѡ�е��б�Item

	//��ȡ�����ܲ���
	My_LoadData_Para();													//��ȡ�����ܲ���
	OnCbnCloseupCombo3();												//��������ѡ���޸��б��⡿
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ӵ�����
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::OnDestroy()
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
void CLTE_PM_Data::OnSize(UINT nType, int cx, int cy)
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
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Data::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ�����Excel
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_PM);											//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Log�б�����(�һ��б�������)��С����
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Log�б�����(�һ��б�������)�����ܡ�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList_PM.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������ѡ���޸��б��⡿
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::OnCbnCloseupCombo3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp;												//��ʱ����
	int			v_iNum,v_iItem;											//��ʱ����
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	//��1��ɾ���б���Ϣ
	m_cList_PM.DeleteAllItems();										//ɾ��������Ŀ
	while(m_cList_PM.DeleteColumn(0));									//ɾ�������У�ObjType��Ϣ�б�
	//��2����ObjType��Ϣ�б������Ϣ
	m_cList_PM.InsertColumn(0,"���",LVCFMT_CENTER,50);					//��1�У����
	m_cList_PM.InsertColumn(1,"С��",LVCFMT_LEFT,140);					//��2�У�С��
	m_cList_PM.InsertColumn(2,"����",LVCFMT_CENTER,140);				//��3�У�����
	//������������
	v_iNum=3;
	m_cPM_Type.GetWindowText(v_sTemp);									//��ã���������
	v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sTemp);						//���ң���������
	v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();				//��ָ��
	while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())			//��������
	{
		m_cList_PM.InsertColumn(v_iNum,v_pPM_Name->second.v_sPM_Name,LVCFMT_RIGHT,60);	//��n�У�����
		v_pPM_Name++;													//�ݼ�
		v_iNum++;														//������
	}
	//��3����ʾ����������
	v_sTemp.Format("%d",v_pLTE_PM_Type->second.v_mLTE_PM.size());		//��������
	m_cPM_Sum.SetWindowText(v_sTemp);									//��ʾ����������
	//��4��ѡ��Χ��С��  or ����
	m_cList.DeleteAllItems();											//ɾ��������Ŀ
	v_iList_Item=-1;													//��λ
	while(m_cList.DeleteColumn(0));										//ɾ�������У�С���������б�
	if (m_cPM_Type.GetCurSel()==0)										//��������
	{
		m_cType.EnableWindow(true);										//��Ч
		m_cQuery.EnableWindow(true);									//��Ч
		m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);
		m_cList.InsertColumn(1,"С��",LVCFMT_LEFT,200); 
		m_cList.InsertColumn(2,"С��ID",LVCFMT_LEFT,0);  
	}
	else																//��������
	{
		m_cType.SetCurSel(0);											//ѡ��15mins
		m_cType.EnableWindow(false);									//��Ч
		m_cQuery.EnableWindow(false);									//��Ч
		m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);
		m_cList.InsertColumn(1,"����",LVCFMT_LEFT,200);  
		m_cList.InsertColumn(2,"С��ID",LVCFMT_LEFT,0);  

		v_iItem = m_cList.InsertItem(0xffff,_T("0"));
		v_sTemp.Format(" %04d",v_iItem+1);
		m_cList.SetItemText(v_iItem,0," "+v_sTemp);						//���
		m_cList.SetItemText(v_iItem,1,(_bstr_t)"ȫ��");					//ȫ��
		m_cList.SetItemText(v_iItem,2,(_bstr_t)"ȫ��");					//ȫ��
		v_iItem = m_cList.InsertItem(0xffff,_T("0"));
		v_sTemp.Format(" %04d",v_iItem+1);
		m_cList.SetItemText(v_iItem,0," "+v_sTemp);						//���
		m_cList.SetItemText(v_iItem,1,(_bstr_t)"��Ϊ");					//��Ϊ
		m_cList.SetItemText(v_iItem,2,(_bstr_t)"��Ϊ");					//��Ϊ
		v_iItem = m_cList.InsertItem(0xffff,_T("0"));
		v_sTemp.Format(" %04d",v_iItem+1);
		m_cList.SetItemText(v_iItem,0," "+v_sTemp);						//���
		m_cList.SetItemText(v_iItem,1,(_bstr_t)"����");					//����
		m_cList.SetItemText(v_iItem,2,(_bstr_t)"����");					//����
		set<CString>::iterator v_pTable=v_mRegion.begin();				//��ʼ
		while (v_pTable!=v_mRegion.end())								//����������
		{
			v_iItem = m_cList.InsertItem(0xffff,_T("0"));
			v_sTemp.Format(" %04d",v_iItem+1);
			m_cList.SetItemText(v_iItem,0," "+v_sTemp);					//���
			m_cList.SetItemText(v_iItem,1,*v_pTable);					//����
			m_cList.SetItemText(v_iItem,2,*v_pTable);					//����

			v_pTable++;
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������ѯ��ť��С����ѯ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//��������С����ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����С���б�С����Ӧ�����ܲ�ѯ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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

		if(v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			v_iWorking=2;												//�����������ܲ�ѯ
			v_pIterator_Frame->second.v_iThread_Exit=1;					//�̣߳�����
			v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);		//������ѯ���ݿ��߳�
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CLTE_PM_Data::My_Thread_Inoput(LPVOID lparam)
{
	CLTE_PM_Data *  lp_this = NULL ;
	lp_this = (CLTE_PM_Data *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(false);								//��������Ч
	lp_this->m_cPM_Type.EnableWindow(false);							//�������ͣ���Ч
	lp_this->m_cLog_Date.EnableWindow(false);							//���ڣ���Ч
	lp_this->m_cLog_Day.EnableWindow(false);							//��������Ч
	lp_this->m_cType.EnableWindow(false);								//��ѯ���ͣ���Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(true);								//��������Ч
	lp_this->m_cPM_Type.EnableWindow(true);								//�������ͣ���Ч
	lp_this->m_cLog_Date.EnableWindow(true);							//���ڣ���Ч
	lp_this->m_cLog_Day.EnableWindow(true);								//��������Ч
	if (lp_this->m_cPM_Type.GetCurSel()==0)								//��������
		lp_this->m_cType.EnableWindow(true);							//��ѯ���ͣ���Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Cell();												//װ��Cell��Ϣ
		break;
	case 2:
		My_LoadData_PM();												//װ��������Ϣ
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��������Ϣ
//	��    �ݣ��������ƣ�15m/1H���ȣ��������ͣ������У�С�������ڡ����У��������͡�
//	�� �� ��15mins��ÿ��һ����  Hour��5��һ����һ��������
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::My_LoadData_PM()
{
	CString			v_sTemp;											//��ʱ����
	int				v_iDay_Sum;											//��ʱ����
	COleDateTime	v_dDate_Start,v_dDate_End,v_dDate;					//��ѯ��ʼ���ڡ���������
	CString			v_sTime_Start,v_sTime_End,v_sTable_Num;				//����

	//��1����ʾ���������б�������Ϣ
	m_cList_PM.DeleteAllItems();										//������б�����
	m_cData_Sum.SetWindowText("");										//�����������������

	//��2����ȡ��ѯ���ڣ�����Ĳ�ѯ��Ϣ
	m_cLog_Date.GetTime(v_dDate_Start);									//��ã���ѯ��ʼ����
	v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//����ʱ�䣺00:00:00
	v_iDay_Sum=m_cLog_Day.GetCurSel()+1;								//��ã���ѯ����
	v_dDate_End =v_dDate_Start + COleDateTimeSpan(v_iDay_Sum,0,0,0);	//��ã���ѯ��������

	//��3�����ܲ�ѯ��(�������ܣ�15mins��ÿ��һ����  Hour��5��һ����һ��������)  
	//				 (�������ܣ�����15mins����һ����)
	if (m_cPM_Type.GetCurSel()==0)										//��������
	{
		if (m_cType.GetCurSel()==0)										//15m���ȡ�ÿ��һ����Pm_LTE_Cal_20160701��
		{
			while(v_dDate_Start<v_dDate_End)							//��ʼ����<��������
			{
				v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";		//��ʼʱ��
				v_sTime_End=v_dDate_Start.Format("%Y-%m-%d")+" 23:59:59";		//����ʱ��
				v_sTemp=v_dDate_Start.Format("%Y%m%d");					//������
				v_sTemp="Pm_LTE_Cal_"+v_sTemp;							//�ļ�����
				My_LoadData_PM_Data(v_sTemp,m_cList.GetItemText(v_iList_Item,1),m_cList.GetItemText(v_iList_Item,2),v_sTime_Start,v_sTime_End);		//װ����������
				v_dDate_Start+=COleDateTimeSpan(1,0,0,0);				//����һ��
			}
		}
		else
		{																//Hour
			v_dDate=v_dDate_Start;										//��ֵ����ǰ����
			v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());		//��ȡ�����
			while(v_dDate<v_dDate_End)									//ʱ�䷶Χ�ڣ�����
			{
				v_dDate+=COleDateTimeSpan(1,0,0,0);						//����һ��
				if (My_Get_Table_Num(v_dDate.GetDay())!=v_sTable_Num || v_dDate>=v_dDate_End)
				{
					v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//��ʼʱ��
					v_sTime_End=v_dDate.Format("%Y-%m-%d")+" 00:00:00";			//����ʱ��
					v_sTemp=v_dDate_Start.Format("%Y%m");				//����
					v_sTemp="Pm_LTE_Cal_"+v_sTemp+"_"+v_sTable_Num;		//�ļ�����
					My_LoadData_PM_Data(v_sTemp,m_cList.GetItemText(v_iList_Item,1),m_cList.GetItemText(v_iList_Item,2),v_sTime_Start,v_sTime_End);	//װ����������
					v_dDate_Start=v_dDate;								//�������ʼʱ��
					v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());		//��ȡ�����
				}
			}
		}
	}
	else																//�������ܡ���һ����Pm_LTE_Region��
	{
		v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";		//��ʼʱ��
		v_sTime_End=v_dDate_End.Format("%Y-%m-%d")+" 00:00:00";			//����ʱ��
		My_LoadData_PM_Data("Pm_LTE_Region",m_cList.GetItemText(v_iList_Item,1),m_cList.GetItemText(v_iList_Item,2),v_sTime_Start,v_sTime_End);		//װ����������
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ�����
//	��	  ����1��1-5��	2��6-10��	3��11-15��	4��16-20��	5��21-25��	6��26--
//------------------------------------------------------------------------------------------------------			
CString CLTE_PM_Data::My_Get_Table_Num(int v_iDay)
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
//	�������ܣ�װ����������
//	��	  ����v_sTable�������ƣ�	v_sCell��С����		v_sCell_ID��С��ID;		v_sDate_Ben����ʼ���ڣ�		v_sDate_End����������
//	��    �ݣ��������ƣ�15m/1H���ȣ��������ͣ������У�С�������ڡ����У��������͡�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::My_LoadData_PM_Data(CString v_sTable,CString v_sCell,CString v_sCell_ID,CString v_sDate_Ben,CString v_sDate_End)
{
	CString			v_sSql;												//��ʱ����
	CString			v_sTemp,v_sTime;									//��ʱ����
	int				v_iItem=0,v_iNum;									//��ʱ����
	COleDateTime	v_tDate;											//����
	double			v_dValue;											//������ֵ

	set<CString>	v_mTable_Columns;									//��ṹ
	set<CString>::iterator	v_pTable_Columns;							//��ṹ

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		//��1����ȡ������ָ��
		m_cPM_Type.GetWindowText(v_sTemp);								//��ã���������
		v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sTemp);					//���ң���������
		if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())						//û�ҵ�
			return;														//��Ч����

		//��2����飺�������Ƿ���Ч�����ݣ������Ƽ���v_mTable_Name��
		set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//���ң���
		if (v_pTable==v_mTable_Name.end())								//û�ҵ�
			return;														//��Ч����

		//��3����ȡ���ݱ�ṹ
		My_Load_Columns(&v_mTable_Columns,v_sTable);

		//��4���������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select * from "+v_sTable+" where zCell='"+v_sCell_ID+"' and zTime>='"+v_sDate_Ben
				+"' and zTime<'"+v_sDate_End+"' order by zTime";		//��ѯ���ݿ⣺ϵͳ��־

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		//��5����ȡ��������
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_tDate=v_cAdoConn.GetField_DataTime("zTime");				//zTime

			//��6����ʾ��С��������
			v_iItem = m_cList_PM.InsertItem(0xffff,_T("0"));
			v_sTemp.Format(" %04d",v_iItem+1);
			m_cList_PM.SetItemText(v_iItem,0,(_bstr_t)" "+v_sTemp);		//���
			m_cList_PM.SetItemText(v_iItem,1,(_bstr_t)v_sCell);			//С��
			v_sTime=v_tDate.Format("%Y-%m-%d_%H:%M:%S");				//�ַ���
			m_cList_PM.SetItemText(v_iItem,2,(_bstr_t)v_sTime);			//����

			//��7����ʾ������
			v_iNum=3;
			v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();		//��ָ��
			while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())	//��������
			{
				//��8���ֶδ��ڣ�����
				v_pTable_Columns=v_mTable_Columns.find(v_pPM_Name->first);
				if (v_pTable_Columns!=v_mTable_Columns.end())			//���ڣ�����
				{
					v_dValue=v_cAdoConn.GetField_Double(v_pPM_Name->first);		//��ȡ������
					v_sTemp.Format("%8.2f",v_dValue);							//�ַ���
					m_cList_PM.SetItemText(v_iItem,v_iNum,(_bstr_t)v_sTemp);	//����
				}

				v_pPM_Name++;											//�ݼ�
				v_iNum++;												//������
			}
			
			v_pRecordset->MoveNext();									//��һ��

			v_iItem++;													//++
			if ((v_iItem)%100==0)
			{
				v_sTemp.Format("%d",v_iItem);							//
				m_cData_Sum.SetWindowText(v_sTemp);						//��ʾ��������������
			}
		}
		
		v_iItem=m_cList_PM.GetItemCount();								//��ȡ���б���Ŀ����
		v_sTemp.Format("%d",v_iItem);									//
		m_cData_Sum.SetWindowText(v_sTemp);								//��ʾ��������������

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	//��9���������
	v_mTable_Columns.clear();											//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��Cell��Ϣ
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::My_LoadData_Cell()
{
	CString		v_sSql;													//��ʱ����
	int			v_iItem,v_iID=0,v_iCell_ID;								//��ʱ����
	CString		v_sTemp,v_sCell,v_sCell_ID;								//��ʱ����
	CString		v_sLog_Key;												//��־��ѯ��KEY

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList.DeleteAllItems();										//����б�����

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

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sCell=v_cAdoConn.GetField_String("zCell");				//zCell
			v_iCell_ID=v_cAdoConn.GetField_Int("zCell_ID");				//zCell_ID

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sCell);			//
			v_sCell_ID.Format("%d",v_iCell_ID);							//�ַ���
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sCell_ID);			//Cell_ID

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
//	�������ܣ�װ�����ܲ�����Ϣ
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::My_LoadData_Para()
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

		//��1���������ͣ���������
		v_sType="��������";												//��������								
		v_cLTE_PM_Type.v_sTable_Key=v_sType;							//������-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//��������
		//��ʾ����������
		m_cPM_Type.AddString(v_sType);									//��������

		//��ȡ�����ܲ���
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
				v_cLTE_PM.v_sPM_Name=v_sPm;								//��������
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//���ӣ����ܲ���
			}

			v_pRecordset->MoveNext();									//
		}
		m_cPM_Type.SetCurSel(0);										//Ĭ�ϣ���һ��

		//��2���������ͣ���������
		v_sType="��������";												//��������								
		v_cLTE_PM_Type.v_sTable_Key=v_sType;							//������-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//��������
		//��ʾ����������
		m_cPM_Type.AddString(v_sType);									//��������

		//��ȡ�����ܲ���
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Region order by zPm";		//Sql

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
				v_cLTE_PM.v_sPM_Name=v_sPm;								//��������
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//���ӣ����ܲ���
			}

			v_pRecordset->MoveNext();									//
		}
		m_cPM_Type.SetCurSel(0);										//Ĭ�ϣ���һ��

		//��3����ȡ���������ݱ����ơ����ڷ�����Ч�����ơ�
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_LTE_Cal%' or name like 'Pm_LTE_Region%') order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//������

			v_mTable_Name.insert(v_sTable);								//���Ӽ���

			v_pRecordset->MoveNext();									//ָ��++
		}	

		//��4����ȡ��������Ϣ�����������ѯ��
		v_sSql="select * from CON_Region";								//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("zRegion");			//zRegion

			v_mRegion.insert(v_sTable);									//����

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
//	�������ܣ�װ�ر�ṹ
//	��	  ����v_pTable_Columns������ָ��
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::My_Load_Columns(set<CString> *v_pTable_Columns,CString v_sTable)
{
	CString			v_sTemp,v_sSql,v_sColumns;							//��ʱ����


	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from sys.columns where object_id=object_id('"+v_sTable+"')";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sColumns = v_cAdoConn.GetField_String("name");			//�ṹ����

			v_pTable_Columns->insert(v_sColumns);						//���Ӽ���

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
