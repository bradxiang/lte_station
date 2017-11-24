// CM_LTE_Quary.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "CM_LTE_Quary.h"

// CCM_LTE_Quary

IMPLEMENT_DYNCREATE(CCM_LTE_Quary, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CCM_LTE_Quary::CCM_LTE_Quary()
	: CFormView(CCM_LTE_Quary::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CCM_LTE_Quary::~CCM_LTE_Quary()
{
	v_mTable_Name.clear();												//�������
	v_mField_Main.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Quary::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_BEG_DATE, m_cLog_Date);
	DDX_Control(pDX, IDC_COMBO1, m_cLog_Day);
	DDX_Control(pDX, IDC_KEY, m_cLog_Key);
	DDX_Control(pDX, IDC_BUTTON1, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_LIST3, m_cList_PM);
	DDX_Control(pDX, IDC_COMBO3, m_cType_2);
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_LOG_SUM3, m_cData_Sum);
	DDX_Control(pDX, IDC_KEY3, m_cLog_Key1);
	DDX_Control(pDX, IDC_COMBO8, m_cRelation);
	DDX_Control(pDX, IDC_CHECK2, m_cDay);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCM_LTE_Quary, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL, &CCM_LTE_Quary::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CCM_LTE_Quary::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CCM_LTE_Quary::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CCM_LTE_Quary::OnNMClickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CCM_LTE_Quary::OnLvnColumnclickList3)
	ON_NOTIFY(DTN_CLOSEUP, IDC_BEG_DATE, &CCM_LTE_Quary::OnDtnCloseupBegDate)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST3, &CCM_LTE_Quary::OnLvnGetdispinfoList3)
END_MESSAGE_MAP()


// CCM_LTE_Quary ���

#ifdef _DEBUG
void CCM_LTE_Quary::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCM_LTE_Quary::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCM_LTE_Quary ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Quary::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	CString v_sTemp;													//��ʱ����
	//��������
	v_sFrame_Name="LTE ���ò�ѯ";										//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б���С����
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,40);					//
	m_cList.InsertColumn(1,"����",LVCFMT_CENTER,40);					//
	m_cList.InsertColumn(2,"������",LVCFMT_LEFT,160);					//
	m_cList.InsertColumn(3,"������",LVCFMT_CENTER,0);					//
	m_cList.InsertColumn(4,"�ӱ����",LVCFMT_CENTER,0);					//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��ʼ�����б������á�
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"���",LVCFMT_CENTER,60);
	m_cList_PM.InsertColumn(1,"����",LVCFMT_CENTER,140);  
	m_cList_PM.InsertColumn(2,"ʱ��",LVCFMT_CENTER,80);  
	m_cList_PM.InsertColumn(3,"DN",LVCFMT_CENTER,120);  
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
		IDC_CM_LTE_Quary_Main,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);

	//ˢ�½���
	PostMessage(WM_SIZE);

	//��ʼ��������ѡ��
	for (int vi=1;vi<=100;vi++)											//�����
	{
		v_sTemp.Format("%d",vi);
		m_cLog_Day.AddString(v_sTemp);									//��ѡ������ѡť
	}
	m_cLog_Day.SetCurSel(0);											//Ĭ�ϲ�ѯ������1�죻

	m_cRelation.AddString("��");
	m_cRelation.AddString("��");
	m_cRelation.SetCurSel(0);											//Ĭ��

	m_cDay.SetCheck(1);													//Ĭ�ϣ���ѡ��
	m_cDay.EnableWindow(false);											//��Ч

	v_sTable_Pro="";													//��ʼ�����ļ�ǰ׺
	v_iList_Item=-1;													//Ĭ�ϣ���ѡ�е��б�Item

	m_cLog_Date.GetTime(v_dDate);										//��ã���ѯ��ʼ����

	//��ȡ�����ܲ���
	My_LoadData_Para();													//��ȡ�����ܲ���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ӵ�����
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Quary::OnDestroy()
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
void CCM_LTE_Quary::OnSize(UINT nType, int cx, int cy)
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
BOOL CCM_LTE_Quary::PreTranslateMessage(MSG* pMsg)
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
void CCM_LTE_Quary::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_PM);											//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Log�б�����(�һ��б�������)���ļ���
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Quary::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (v_iList_Item!=-1)
	{
		m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));			//�ָ�����ɫ
		v_iList_Item=-1;												//Ĭ�ϣ���ѡ�е��б�Item
	}
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Log�б�����(�һ��б�������)�����á�
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Quary::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	m_cList_PM.My_Sort_Virtual(pNMHDR);									//�б����������б�

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ��
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Quary::OnDtnCloseupBegDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString v_sTemp,v_sTemp1;											//��ʱ����
	COleDateTime v_dSelect;												//��ʱ����

	m_cLog_Date.GetTime(v_dSelect);										//��ã���ѯ��ʼ����
	v_sTemp=v_dSelect.Format("%Y-%m");									//��ȡ��ѡ�������
	v_sTemp1=v_dDate.Format("%Y-%m");									//��ȡ����ѡ������

	if (v_sTemp!=v_sTemp1)												//�·ݷ����仯
	{
		v_dDate=v_dSelect;												//��ȡ��ѡ�������

		if(v_iList_Item!=-1)											//ѡ�����ļ�������
			My_Get_Cm_Field("����");									//��ȡ���ò���(����ʾ)
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ļ��б�
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Quary::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			//���ѡ����Ŀ
			if(v_iList_Item>=0)											//��Ч��Ŀ��ţ�����
				m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));	//�ָ�����ɫ
			v_iList_Item=pNMListView->iItem;							//��ã�ѡ����Ŀ���
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));			//���ñ���ɫ����ɫ

			//��ȡ�����ò���(���ݣ����ڡ��ļ�)
			My_Get_Cm_Field("�ļ�");									//��ȡ���ò���(����ʾ)

			if (m_cList.GetItemText(v_iList_Item,2).Find("LTE-Tdd���ڹ�ϵ")>=0)	//�ǣ�LTE-Tdd���ڹ�ϵ��Ϣ������[�����ѡ��]
				m_cDay.EnableWindow(true);								//��Ч
			else
				m_cDay.EnableWindow(false);								//��Ч
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ���ò���(����ʾ)
//	��	  ����v_sType���ļ�������
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Quary::My_Get_Cm_Field(CString v_sType)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp,v_sTable,v_sMaker,v_sNum;						//��ʱ����
	int			v_iNum;													//��ʱ����

	//��0����ʼ��
	m_cList_PM.DeleteAllItems();										//ɾ��������Ŀ
	while(m_cList_PM.DeleteColumn(0));									//ɾ�������У�ObjType��Ϣ�б�
	m_cType.ResetContent();												//ɾ��
	m_cType_2.ResetContent();											//ɾ��
	m_cData_Sum.SetWindowText("");										//�����������

	//��1����ȡ���ļ����� (�ļ�����+ѡ������)
	v_sMaker=m_cList.GetItemText(v_iList_Item,1);						//����
	if (v_sMaker=="��Ϊ")
		v_sMaker="HW";													//��Ϊ
	else
		v_sMaker="ZTE";													//����
	v_sTable=m_cList.GetItemText(v_iList_Item,3);						//������
	v_sNum=m_cList.GetItemText(v_iList_Item,4);							//�ӱ����
	v_sNum.Trim();														//ȥ���ո�

	v_sTable_Pro="Xm_LTE_"+v_sMaker+"_"+v_sTable;						//�ļ�ǰ׺
	if (v_sNum!="0")
		v_sTable_Pro+="_"+v_sNum;										//�ļ�ǰ׺

	//��2���жϣ��ļ��Ƿ���ڣ�����
	set<CString>::iterator v_pFile;										//������
	v_sTable_All=v_dDate.Format("%Y%m");								//����
	v_sTable_All=v_sTable_Pro+"_"+v_sTable_All;							//�ļ�ȫ��
	v_pFile=v_mTable_Name.find(v_sTable_All);							//�����ļ�
	if (v_pFile==v_mTable_Name.end())									//�����ڣ�����
	{
		MessageBox("��Чѡ��!!!  ������ѡ�� "+v_sType);					//��ʾ
		return;															//����
	}

	//��3����ȡ���ļ��ֶ�(��ʾ)
	v_mField_Main.clear();												//�������
	My_Get_File_Field(&v_mField_Main,v_sTable_All);						//��ȡ���ļ��ֶ�

	//��4����ʾ���ļ��ֶ���Ϣ(����Ϣ)

	//��ObjType��Ϣ�б������Ϣ
	m_cList_PM.InsertColumn(0,"���",LVCFMT_CENTER,60);					//
	m_cList_PM.InsertColumn(1,"����",LVCFMT_LEFT,140);					//
	m_cList_PM.InsertColumn(2,"ʱ��",LVCFMT_CENTER,80);					//
	m_cList_PM.InsertColumn(3,"DN",LVCFMT_LEFT,120);					//

	//�����С������б�
	m_cType.AddString("����");											//
	m_cType_2.AddString("����");										//
	mString_Int::iterator v_pField;										//������
	v_iNum=4;
	for(v_pField=v_mField_Main.begin();v_pField!=v_mField_Main.end();v_pField++)	//����
	{
		//������
		m_cList_PM.InsertColumn(v_iNum,v_pField->first,LVCFMT_LEFT,120);//��n�У�����
		v_iNum++;														//������
		
		//��������
		m_cType.AddString(v_pField->first);								//
		m_cType_2.AddString(v_pField->first);							//
	}
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	m_cType.SetCurSel(0);												//Ĭ��
	m_cType_2.SetCurSel(0);												//Ĭ��
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������б���Ϣ��Ӧ����
//	��	  ���������б���Ҫ��Ԫ�ز���(�С���)�������������в�ѯ���ݣ���ֵ���б��С�
//------------------------------------------------------------------------------------------------------	
void CCM_LTE_Quary::OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ�������б���ʾ���ݻ�ȡ
	m_cList_PM.Get_List_Data(pDispInfo);								//�����б���ʾ���ݻ�ȡ

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������ѯ��ť�����ò�ѯ��
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Quary::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//��������С����ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CCM_LTE_Quary::My_Thread_Inoput(LPVOID lparam)
{
	CCM_LTE_Quary *  lp_this = NULL ;
	lp_this = (CCM_LTE_Quary *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(false);								//��������Ч
	lp_this->m_cType_2.EnableWindow(false);								//�������ͣ���Ч
	lp_this->m_cType.EnableWindow(false);								//��ѯ���ͣ���Ч
	lp_this->m_cLog_Date.EnableWindow(false);							//���ڣ���Ч
	lp_this->m_cLog_Day.EnableWindow(false);							//��������Ч
	lp_this->m_cLog_Key.EnableWindow(false);							//���룺��Ч
	lp_this->m_cLog_Key1.EnableWindow(false);							//���룺��Ч
	lp_this->m_cRelation.EnableWindow(false);							//����ϵ����Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(true);								//��������Ч
	lp_this->m_cType_2.EnableWindow(true);								//�������ͣ���Ч
	lp_this->m_cType.EnableWindow(true);								//��ѯ���ͣ���Ч
	lp_this->m_cLog_Date.EnableWindow(true);							//���ڣ���Ч
	lp_this->m_cLog_Day.EnableWindow(true);								//��������Ч
	lp_this->m_cLog_Key.EnableWindow(true);								//���룺��Ч
	lp_this->m_cLog_Key1.EnableWindow(true);							//���룺��Ч
	lp_this->m_cRelation.EnableWindow(true);							//����ϵ����Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Quary::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_CM();												//װ��������Ϣ
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ����������
//	��    �ݣ�
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Quary::My_LoadData_CM()
{
	CString			v_sTemp;											//��ʱ����
	int				v_iDay_Sum,v_iMonth;								//��ʱ����
	COleDateTime	v_dDate_Start,v_dDate_End,v_dDate;					//��ѯ��ʼ���ڡ���������
	CString			v_sTime_Start,v_sTime_End,v_sTable_Num;				//����

	//��1����ʾ���������б�������Ϣ
	m_cData_Sum.SetWindowText("");										//�����������������
	v_iCM_Sum=0;														//��ȡ����������

	//��2����λ�������б�
	m_cList_PM.Put_Line_Begin();										//�б�������ݿ�ʼ

	//��3����ȡ��ѯ���ڣ�����Ĳ�ѯ��Ϣ
	m_cLog_Date.GetTime(v_dDate_Start);									//��ã���ѯ��ʼ����
	v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//����ʱ�䣺00:00:00
	v_iDay_Sum=m_cLog_Day.GetCurSel()+1;								//��ã���ѯ����
	v_dDate_End =v_dDate_Start + COleDateTimeSpan(v_iDay_Sum,0,0,0);	//��ã���ѯ��������

	//��4�����ò�ѯ���±� (һ�²�һ��) 
	v_dDate=v_dDate_Start;												//��ֵ����ǰ����
	while(v_dDate<v_dDate_End)											//ʱ�䷶Χ�ڣ�����
	{
		v_iMonth=v_dDate.GetMonth();									//�·�
		v_dDate += COleDateTimeSpan(1,0,0,0);							//����һ��
		if (v_dDate<v_dDate_End && v_dDate.GetMonth()==v_iMonth)		//�仯���±���
			continue;

		//��ѯ������ȷ��������
		v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";		//��ʼʱ��
		v_sTime_End=v_dDate.Format("%Y-%m-%d")+" 00:00:00";				//����ʱ��
		v_sTemp=v_dDate_Start.Format("%Y%m");							//����
		v_sTemp = v_sTable_Pro+"_"+v_sTemp;								//�ļ�����
		My_LoadData_CM_Data(v_sTemp,v_sTime_Start,v_sTime_End);			//��ȡ��������
		v_dDate_Start=v_dDate;											//�������ʼʱ��
	}

	//��5�����ã������б���Ч
	m_cList_PM.Put_Line_End();											//�б�������ݽ���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ����������
//	��	  ����v_sTable�������ƣ�	v_sDate_Ben����ʼ���ڣ�		v_sDate_End����������
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Quary::My_LoadData_CM_Data(CString v_sTable,CString v_sDate_Ben,CString v_sDate_End)
{
	CString			v_sSql;												//��ʱ����
	CString			v_sTemp,v_sTime,v_sLog_Key,v_sLog_Key1;				//��ʱ����
	int				v_iItem;											//��ʱ����
	CString			v_sObject,v_sDN,v_sValues,v_sSelect,v_sSelect1;		//��ʱ����
	CString			v_sRelation="",v_sRelation1="";						//��ʱ����
	COleDateTime	v_dTime;											//����

	mString_Int		v_mField_Table;										//��ȡ����ֶ�����
	mString_Int::iterator v_pField_Table;								//������
	mString_Int::iterator v_pField_Main;								//������

	//�����б�
	mInt_String		v_mLine_Data;										//�����б�������

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		//��1����飺�������Ƿ���Ч�����ݣ������Ƽ���v_mTable_Name��
		set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//���ң���
		if (v_pTable==v_mTable_Name.end())								//û�ҵ�
			return;														//��Ч����

		My_Get_File_Field(&v_mField_Table,v_sTable);					//��ȡ���ļ��ֶ�

		//��2���������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		if (v_sTable.Find("EutranRelationTdd")>=0 && m_cDay.GetCheck())	//EutranRelationTdd���������
			v_sTable=v_sTable.Left(v_sTable.GetLength()-6)+"Day";		//�����

		v_sSql+="select * from "+v_sTable+" where zTime>='"+v_sDate_Ben+"' and zTime<'"+v_sDate_End+"' ";
		
		m_cLog_Key.GetWindowText(v_sLog_Key);							//��ã���ѯKEY
		v_sLog_Key.Replace("[","[[]");									//���ƴ���
		v_sLog_Key.Replace("'","");										//���ƴ���
		v_sLog_Key.Trim();												//ȥ���ո�
		m_cLog_Key1.GetWindowText(v_sLog_Key1);							//��ã���ѯKEY1
		v_sLog_Key1.Replace("[","[[]");									//���ƴ���
		v_sLog_Key1.Replace("'","");									//���ƴ���
		v_sLog_Key1.Trim();												//ȥ���ո�

		if (!v_sLog_Key.IsEmpty())										//��ѯKEY����
		{
			if(m_cType.GetCurSel()==0)									//ѡ�񣺶���
				v_sRelation="zObject like '%"+v_sLog_Key+"%'";			//����
			else
			{
				m_cType.GetLBText(m_cType.GetCurSel(),v_sSelect);
				v_pField_Main=v_mField_Main.find(v_sSelect);			//���ң��ֶ�
				if (v_pField_Main!=v_mField_Main.end())					//�ҵ�����
					v_sRelation="["+v_sSelect+"] like '%"+v_sLog_Key+"%'";		//ѡ�������
			}
		}
		if (!v_sLog_Key1.IsEmpty())										//��ѯKEY1����
		{
			if(m_cType_2.GetCurSel()==0)								//ѡ�񣺶���
				v_sRelation1="zObject like '%"+v_sLog_Key1+"%'";		//����
			else
			{
				m_cType_2.GetLBText(m_cType_2.GetCurSel(),v_sSelect1);
				v_pField_Main=v_mField_Main.find(v_sSelect1);			//���ң��ֶ�
				if (v_pField_Main!=v_mField_Main.end())					//�ҵ�����
					v_sRelation1="["+v_sSelect1+"] like '%"+v_sLog_Key1+"%'";	//ѡ�������
			}
		}
		if (!v_sRelation.IsEmpty() && !v_sRelation1.IsEmpty())			//������
		{
			if (m_cRelation.GetCurSel()==0)								//��
				v_sSql+=" and ("+v_sRelation+" and "+v_sRelation1+")";	//��
			else														//��
				v_sSql+=" and ("+v_sRelation+" or "+v_sRelation1+")";	//��
		}
		else if (!v_sRelation.IsEmpty())								//����
			v_sSql+=" and "+v_sRelation;
		else if (!v_sRelation1.IsEmpty())								//����1
			v_sSql+=" and "+v_sRelation1;

		v_sSql+=" order by zTime";										//��ѯ���ݿ�

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		//��3����ȡ��������
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			//���ݸ�λ
			v_mLine_Data.clear();											//�������

			//��4����ȡ����
			v_sObject=v_cAdoConn.GetField_String("zObject");			//zObject
			v_dTime=v_cAdoConn.GetField_DataTime("zTime");				//zTime
			v_sTime=v_dTime.Format("%Y-%m-%d");							//�ַ���
			v_sDN=v_cAdoConn.GetField_String("zDN");					//zDN

			v_sTemp.Format(" %06d",v_iCM_Sum+1);
			v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));			//���
			v_mLine_Data.insert(pair<int,CString>(1,v_sObject));		//����
			v_mLine_Data.insert(pair<int,CString>(2,v_sTime));			//ʱ��
			v_mLine_Data.insert(pair<int,CString>(3,v_sDN));			//DN

			//��5����ʾ������
			v_iItem=4;													//���
			v_pField_Table=v_mField_Table.begin();						//�ֶ���ָ��
			while(v_pField_Table!=v_mField_Table.end())					//�ֶν�������
			{
				//�Ƿ���Ч�ֶΣ������������ֶα�v_mField_Main��
				v_pField_Main=v_mField_Main.find(v_pField_Table->first);//���ң��ֶ�
				if (v_pField_Main!=v_mField_Main.end())					//�ҵ�����
				{
					v_sValues=v_cAdoConn.GetField_String(v_pField_Table->first);	//��ȡ����������
					v_mLine_Data.insert(pair<int,CString>(v_iItem,v_sValues));		//��������
				}
		
				v_iItem++;												//�ݼ�
				v_pField_Table++;										//�ݼ�
			}
			
			//�������ݷ����б�
			m_cList_PM.Put_Line_Data(v_mLine_Data);						//�б�������������

			v_pRecordset->MoveNext();									//��һ��

			v_iCM_Sum++;												//++
			if ((v_iCM_Sum)%500==0)
			{
				v_sTemp.Format("%d",v_iCM_Sum);							//
				m_cData_Sum.SetWindowText(v_sTemp);						//��ʾ��������������
			}
		}
		
		v_sTemp.Format("%d",v_iCM_Sum);									//
		m_cData_Sum.SetWindowText(v_sTemp);								//��ʾ��������������

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	//�ֶ�
	v_mField_Table.clear();												//�������

	//�����б�
	v_mLine_Data.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ�����ܲ�����Ϣ
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Quary::My_LoadData_Para()
{
	int				v_iID=0,v_iNum,v_iItem;								//��ʱ����
	CString			v_sTemp,v_sSql,v_sLog_Key,v_sTable;					//��ʱ����
	CString			v_sType,v_sPm,v_sNum;								//��ʱ����
	CString			v_sMaker,v_sNum_Demo,v_sFile_Name;					//��ʱ����

	LTE_PM_Struct	v_cLTE_PM;											//�ṹ
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	LTE_PM_Type_Struct		v_cLTE_PM_Type;								//�ṹ

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��1����ȡ����ʾ�������ļ���Ϣ
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Cm_LTE_File order by zMaker,zFile_Demo";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sMaker = v_cAdoConn.GetField_String("zMaker");			//zMaker
			v_sNum_Demo = v_cAdoConn.GetField_String("zNum_Demo");		//zNum_Demo
			v_sFile_Name = v_cAdoConn.GetField_String("zFile_Name");	//zFile_Name
			v_iNum = v_cAdoConn.GetField_Int("zNum");					//zNum

			//��ʾ
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,v_sMaker);					//����
			m_cList.SetItemText(v_iItem,2,v_sNum_Demo);					//�ӱ�����
			m_cList.SetItemText(v_iItem,3,v_sFile_Name);				//�ļ�����
			v_sNum.Format("%d",v_iNum);									//�ַ���
			m_cList.SetItemText(v_iItem,4,v_sNum);						//�ӱ����

			v_iID++;													//������
			v_pRecordset->MoveNext();									//
		}

		//��3����ȡ���������ݱ����ơ����ڷ�����Ч�����ơ�
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and name like 'Xm_LTE_%' order by [name]";		//SQL
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
//	�������ܣ���ȡ�ļ��ֶ�(��ʾ)
//	��	  ����v_mField_Main���ֶ�������
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Quary::My_Get_File_Field(mString_Int* v_mField,CString v_sTable)
{
	CString			v_sSql;												//��ʱ����
	CString			v_sTemp,v_sName;									//��ʱ����

	//��ȡ��������
	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��1����ȡ�ֶ�����
		v_sSql="select name from syscolumns where id=OBJECT_ID('"+v_sTable+"')";	//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sName = v_cAdoConn.GetField_String("name");				//������

			if (v_sName!="zObject" && v_sName!="zTime" && v_sName!="zDN")
				v_mField->insert(pair<CString,int>(v_sName,0));			//����������v_iNum������š�

			v_pRecordset->MoveNext();									//ָ��++
		}

		v_cAdoConn.ExitConnect();										//�Ͽ�����

		//��2���ֶ���������
		mString_Int::iterator	v_pIter;								//������
		int						v_iNum=4;								//��ʱ����
		for (v_pIter=v_mField->begin();v_pIter!=v_mField->end();v_pIter++)		//����
		{
			v_pIter->second=v_iNum;										//�ֶ����
			v_iNum++;													//������
		}
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
