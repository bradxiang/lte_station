// LTE_PM_Data_Cell.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Data_Cell.h"
#include "LTE_PM_Draw_Znit_Select.h"									//����ѡ��
#include "LTE_PM_DATA_CASE.h"											//ģ�塾С�������ܡ�
#include "LTE_PM_Data_Cell_Time.h"										//ʱ��ѡ��

// CLTE_PM_Data_Cell

IMPLEMENT_DYNCREATE(CLTE_PM_Data_Cell, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cell::CLTE_PM_Data_Cell()
	: CFormView(CLTE_PM_Data_Cell::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cell::~CLTE_PM_Data_Cell()
{
	v_mLTE_PM_Type.clear();												//�������
	v_mTable_Name.clear();												//�������
	v_mTime.clear();													//�������
	v_mPM_Field_Num.clear();											//�������
	v_mCell.clear();													//�������
	v_mCell_ID.clear();													//�������
	My_PM_Data_Delete();												//�������
	My_PM_Data_P_Delete();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_LOG_SUM2, m_cPM_Sum);
	DDX_Control(pDX, IDC_CHECK2, m_cCheck);
	DDX_Control(pDX, IDC_EXCEL3, m_cPM_Cell);
	DDX_Control(pDX, IDC_EXCEL4, m_cCase_Cell);
	DDX_Control(pDX, IDC_EXCEL5, m_cCase_PM);
	DDX_Control(pDX, IDC_EXCEL2, m_cCase_Select);
	DDX_Control(pDX, IDC_LIST9, m_cList_Tj);
	DDX_Control(pDX, IDC_CHECK3, m_cTime_Check);
	DDX_Control(pDX, IDC_COMBO8, m_cTj_Type);
	DDX_Control(pDX, IDC_EXCEL6, m_cSetup_Time);
	DDX_Control(pDX, IDC_EXCEL7, m_cTj);
	DDX_Control(pDX, IDC_EXCEL8, m_cExcel_Tj);
	DDX_Control(pDX, IDC_KEY2, m_cSpan);
	DDX_Control(pDX, IDC_QUARY_DATA, m_cQuary_Data);
	DDX_Control(pDX, IDC_QUARY_NAME, m_cQuary_Name);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Data_Cell, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL, &CLTE_PM_Data_Cell::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CLTE_PM_Data_Cell::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Data_Cell::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Data_Cell::OnNMClickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CLTE_PM_Data_Cell::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_EXCEL2, &CLTE_PM_Data_Cell::OnBnClickedExcel2)
	ON_BN_CLICKED(IDC_EXCEL3, &CLTE_PM_Data_Cell::OnBnClickedExcel3)
	ON_BN_CLICKED(IDC_EXCEL4, &CLTE_PM_Data_Cell::OnBnClickedExcel4)
	ON_BN_CLICKED(IDC_EXCEL5, &CLTE_PM_Data_Cell::OnBnClickedExcel5)
	ON_BN_CLICKED(IDC_CHECK2, &CLTE_PM_Data_Cell::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_EXCEL8, &CLTE_PM_Data_Cell::OnBnClickedExcel8)
	ON_BN_CLICKED(IDC_EXCEL6, &CLTE_PM_Data_Cell::OnBnClickedExcel6)
	ON_BN_CLICKED(IDC_EXCEL7, &CLTE_PM_Data_Cell::OnBnClickedExcel7)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST9, &CLTE_PM_Data_Cell::OnLvnColumnclickList9)
	//��Ϣ��Ӧ���б���ʾ
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST3, &CLTE_PM_Data_Cell::OnLvnGetdispinfoList3)
END_MESSAGE_MAP()


// CLTE_PM_Data_Cell ���

#ifdef _DEBUG
void CLTE_PM_Data_Cell::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLTE_PM_Data_Cell::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLTE_PM_Data_Cell ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	CString v_sTemp;													//��ʱ����
	int		vi;															//��ʱ����

	//��������
	v_sFrame_Name="LTE �������ݲ�ѯ_С��";								//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б���С����
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);
	m_cList.InsertColumn(1,"С��",LVCFMT_LEFT,300);  
	m_cList.InsertColumn(2,"С��ID",LVCFMT_LEFT,0);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES );

	//��ʼ�����б������ܡ�
	m_cList_PM.ModifyStyle( 0, LVS_REPORT|LVS_SHOWSELALWAYS, TRUE );
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"���",LVCFMT_CENTER,60);
	m_cList_PM.InsertColumn(1,"С��",LVCFMT_LEFT,200);					//��1�У�С��
	m_cList_PM.InsertColumn(2,"Ӫҵ��",LVCFMT_LEFT,60);					//��2�У�Ӫҵ��
	m_cList_PM.InsertColumn(3,"״̬",LVCFMT_LEFT,40);					//��3�У�״̬
	m_cList_PM.InsertColumn(4,"����",LVCFMT_CENTER,120);				//��4�У�����
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��ʼ�����б���ͳ�ơ�
	::SendMessage(m_cList_Tj.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Tj.InsertColumn(0,"���",LVCFMT_CENTER,60);
	m_cList_Tj.InsertColumn(1,"С��",LVCFMT_LEFT,200);  
	m_cList_Tj.InsertColumn(2,"Ӫҵ��",LVCFMT_LEFT,60);					//��2�У�Ӫҵ��
	m_cList_Tj.InsertColumn(3,"״̬",LVCFMT_LEFT,40);					//��3�У�״̬
	m_cList_Tj.InsertColumn(4,"����",LVCFMT_CENTER,120);				//��4�У�����
	m_cList_Tj.SetExtendedStyle(m_cList_Tj.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//�ִ� [��������ߵ����¹�ϵ��ˮƽ�ָ��������������ҹ�ϵ�Ĵ�ֱ�ָ�������2*1]
	CRect	v_cRect;													//����
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//��ã�ָ��
	pWnd->GetWindowRect(&v_cRect);										//��ã��ߴ�
	ScreenToClient(&v_cRect);											//����任
	pWnd->DestroyWindow();												//���٣�ָ��

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Left.Create(											//��ֱ��
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList_PM,			// Left pane
		&m_cList_Tj,			// Right pane
		IDC_LTE_PM_Data_Cell_Main,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);
	m_SplitterPane_Left.m_bVertSplitter_Parent=true;					//��ֱ�ָ���Ϊ����
	m_SplitterPane_Main.Create(											//��ֱ��
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_cList,				// Left pane
		&m_SplitterPane_Left,	// Right pane
		IDC_LTE_PM_Data_Cell_Main_1,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);

	//������ʼ��
	m_cType.AddString("1H");
	m_cType.AddString("15m");
	m_cType.SetCurSel(0);												//Ĭ�ϣ�1H����

	m_cTj_Type.AddString("���");
	m_cTj_Type.AddString("ƽ��");
	m_cTj_Type.SetCurSel(0);											//Ĭ�ϣ����

	//��ʼ��������ѡ��
	for (vi=1;vi<=c_iSample_Day;vi++)									//�����
	{
		v_sTemp.Format("%d",vi);
		m_cLog_Day.AddString(v_sTemp);									//��ѡ������ѡť
	}
	m_cLog_Day.SetCurSel(0);											//Ĭ�ϲ�ѯ������1�죻

	v_iList_Item=-1;													//Ĭ�ϣ���ѡ�е��б�Item
	v_iWorking=0;														//��λ

	//����ѡ��ʱ��ѡ��
	for (vi=0;vi<5;vi++)												//��������
	{
		v_dDate_Select[vi].v_bFlag=false;								//��Ч
		v_dDate_Select[vi].v_dDate=COleDateTime::GetCurrentTime();		//��ǰ����
		v_dDate_Select[vi].v_iDay=0;									//1��

		v_dTime_Select[vi].v_bFlag=false;								//��Ч
		v_dTime_Select[vi].v_iHour_1=8;									//8��
		v_dTime_Select[vi].v_iHour_2=10;								//10��
	}
	v_dDate_Select[0].v_bFlag=true;										//��Ч������1

	//ˢ�½���
	PostMessage(WM_SIZE);

	//��ȡ�����ܲ���
	v_iWorking=4;														//�������񣺻�ȡ��ʼ����
	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ӵ�����
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnDestroy()
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
void CLTE_PM_Data_Cell::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//�ִ��ߴ����
		CRect v_cRect;
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=((v_iWorking==2)?80:40);							//���ܲ�ѯ��80(��ʾ��ѯ����)��������40
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Data_Cell::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ���ϸ����
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_PM);											//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ͳ�Ƶ���
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Tj);											//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Log�б�����(�һ��б�������)��С����
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����(�һ��б�������)�����ܣ������б�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_cList_PM.My_Sort_Virtual(pNMHDR);									//�б����������б�

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Log�б�����(�һ��б�������)��ͳ�ơ�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::OnLvnColumnclickList9(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList_Tj.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ȫѡ��С����
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//����
		m_cList.SetCheck(vi,m_cCheck.GetCheck());						//����״̬
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������ѯ��ť��С����ѯ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iList_Item=-1;													//С��ѡ����Ч

	v_iWorking=1;														//��������С����ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�С��ģ�塾��ȡ��С����Ϣ������m_cList��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp;												//��ʱ����
	int			v_iItem,v_iID=0;										//��ʱ����

	LTE_PM_DATA_CASE	v_Dlg;											//ʵ����
	v_Dlg.v_iSelect=0;													//��־��С��

	if (v_Dlg.DoModal()==IDOK)											//ȷ�Ϸ��أ�����
	{
		//ѡ�������С����Ϣ��������(v_Dlg.v_mCell);
		//��������1����ȡС������2������v_mList������Cell_ID��
		m_cList.DeleteAllItems();										//ɾ����ģ����Ϣ
		v_iList_Item=-1;												//��λ����Ŀ���

		mString_String::iterator	v_pIter_Cell;						//������
		set<CString>::iterator		v_pIter;							//������

		//��������
		for(v_pIter=v_Dlg.v_mCell.begin();v_pIter!=v_Dlg.v_mCell.end();v_pIter++)	//������ģ����Ϣ�б�
		{
			//��ЧС��
			v_pIter_Cell=v_mCell.find(*v_pIter);						//���ң�С�����ڣ�����
			if (v_pIter_Cell==v_mCell.end())							//������
				continue;												//��һС��
			
			//�����б�
			v_sTemp.Format(" %05d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,v_pIter_Cell->first);			//��ʾ��С��
			m_cList.SetItemText(v_iItem,2,v_pIter_Cell->second);		//��ʾ��С��ID
			m_cList.SetCheck(v_iID,TRUE); 
			v_iID++;
		}																//
		v_sTemp.Format("%d",v_iID);										//
		m_cLog_Sum.SetWindowText(v_sTemp);								//��ʾ��Log����

		m_cCheck.SetCheck(true);										//��Ч��ȫѡ
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ģ�塾��ȡ��������Ϣ������m_cList_PM ����Ϣ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LTE_PM_DATA_CASE	v_Dlg;											//ʵ����
	v_Dlg.v_iSelect=1;													//��־������

	v_Dlg.v_mLTE_PM_Type=&v_mLTE_PM_Type;								//ָ�룺���ܲ���
	if (v_Dlg.DoModal()==IDOK)											//ȷ�Ϸ��أ�����
		My_PM_Show();													//ѡ�����������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CLTE_PM_Draw_Znit_Select	v_cDlg;									//ʵ����

	v_cDlg.v_mLTE_PM_Type=&v_mLTE_PM_Type;								//ָ�룺���ܲ���

	if (v_cDlg.DoModal()==IDOK)											//ȷ�Ϸ��أ�����
		My_PM_Show();													//ѡ�����������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ѡ�����������
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::My_PM_Show()
{
	CString		v_sTemp;												//��ʱ����
	int			v_iNum;													//��ʱ����

	mInt_String					v_mPM_Field;							//��������
	mInt_String					v_mPM_Field_PM;							//��������(�ֶ�)
	mInt_String::iterator		v_pPM_Field;							//������
	mInt_String::iterator		v_pPM_Field_PM;							//������
	mLTE_PM::iterator			v_pPM_Field_Num;						//������

	mLTE_PM_Type::iterator		v_pSelect;								//������
	mLTE_PM::iterator			v_pPM_Name;								//������

	//��1�����������������v_mPM_Field��
	v_mPM_Field_Num.clear();											//�������
	for (v_pSelect=v_mLTE_PM_Type.begin();v_pSelect!=v_mLTE_PM_Type.end();v_pSelect++)
	{
		for (v_pPM_Name=v_pSelect->second.v_mLTE_PM.begin();v_pPM_Name!=v_pSelect->second.v_mLTE_PM.end();v_pPM_Name++)
		{
			if (v_pPM_Name->second.v_bOK)								//����ѡ�У�������������ţ�����������
			{
				v_mPM_Field.insert(pair<int,CString>(v_pPM_Name->second.v_iNum,v_pPM_Name->second.v_sPM_Name));	//�������ӣ������ӳ����������
				v_mPM_Field_PM.insert(pair<int,CString>(v_pPM_Name->second.v_iNum,v_pPM_Name->first));			//�������ӣ������ӳ����������(�ֶ�)
				v_mPM_Field_Num.insert(pair<CString,LTE_PM_Struct>(v_pPM_Name->first,v_pPM_Name->second));		//�������ӣ���������ӳ�������
			}
		}
	}

	//��2��������ϸ�б�
	//ɾ���б���Ϣ
	m_cList_PM.DeleteAllItems();										//ɾ��������Ŀ
	while(m_cList_PM.DeleteColumn(0));									//ɾ�������У�ObjType��Ϣ�б�

	//�������Ϣ����������
	m_cList_PM.InsertColumn(0,"���",LVCFMT_CENTER,50);					//��1�У����
	m_cList_PM.InsertColumn(1,"С��",LVCFMT_LEFT,200);					//��2�У�С��
	m_cList_PM.InsertColumn(2,"Ӫҵ��",LVCFMT_LEFT,60);					//��2�У�Ӫҵ��
	m_cList_PM.InsertColumn(3,"״̬",LVCFMT_LEFT,40);					//��3�У�״̬
	m_cList_PM.InsertColumn(4,"����",LVCFMT_CENTER,120);				//��4�У�����

	//�����У��������ơ�������ţ��������л�ȡ�������ơ�
	v_iNum=5;
	for (v_pPM_Field=v_mPM_Field.begin();v_pPM_Field!=v_mPM_Field.end();v_pPM_Field++)	//����				
	{
		//������
		m_cList_PM.InsertColumn(v_iNum,v_pPM_Field->second,LVCFMT_RIGHT,80);	//��n�У�����
		m_cList_PM.SetNumber(v_iNum);									//���ã���������

		//����������������(ȥ����ȱ�����)[����Ϣͬ��]
		v_pPM_Field_PM=v_mPM_Field_PM.find(v_pPM_Field->first);			//Ѱ�ң����
		if(v_pPM_Field_PM!=v_mPM_Field_PM.end())						//�ҵ�������
		{
			v_pPM_Field_Num=v_mPM_Field_Num.find(v_pPM_Field_PM->second);	//���ң�����
			if (v_pPM_Field_Num!=v_mPM_Field_Num.end())					//�ҵ�������
				v_pPM_Field_Num->second.v_iNum=v_iNum;					//��ȡ�������
		}

		v_iNum++;														//������
	}

	//��3��������ϸ�б�
	//ɾ���б���Ϣ
	m_cList_Tj.DeleteAllItems();										//ɾ��������Ŀ
	while(m_cList_Tj.DeleteColumn(0));									//ɾ�������У�ObjType��Ϣ�б�

	//�������Ϣ����������
	m_cList_Tj.InsertColumn(0,"���",LVCFMT_CENTER,50);					//��1�У����
	m_cList_Tj.InsertColumn(1,"С��",LVCFMT_LEFT,200);					//��2�У�С��
	m_cList_Tj.InsertColumn(2,"Ӫҵ��",LVCFMT_LEFT,60);					//��2�У�Ӫҵ��
	m_cList_Tj.InsertColumn(3,"״̬",LVCFMT_LEFT,40);					//��3�У�״̬
	m_cList_Tj.InsertColumn(4,"����",LVCFMT_CENTER,120);				//��4�У�����

	//�����У��������ơ�������ţ��������л�ȡ�������ơ�
	v_iNum=5;
	for (v_pPM_Field=v_mPM_Field.begin();v_pPM_Field!=v_mPM_Field.end();v_pPM_Field++)	//����				
	{
		//������
		m_cList_Tj.InsertColumn(v_iNum,v_pPM_Field->second,LVCFMT_RIGHT,80);	//��n�У�����
		m_cList_Tj.SetNumber(v_iNum);									//���ã���������

		v_iNum++;														//������
	}

	v_sTemp.Format("%d",v_iNum-5);										//��������
	m_cPM_Sum.SetWindowText(v_sTemp);									//��ʾ����������

	//��4����λ
	v_mPM_Field.clear();												//�������
	v_mPM_Field_PM.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܲ�ѯ
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(v_pIterator_Frame->second.v_iThread_Exit==0)
	{
		m_cTime_Check.SetCheck(false);									//��Ч��ʱ�Σ������ܲ�ѯ����������ʱ��ͳ�ơ���

		//��ȡ������1
		m_cLog_Date.GetTime(v_dDate_Select[0].v_dDate);					//��ȡ������
		v_dDate_Select[0].v_iDay=m_cLog_Day.GetCurSel();				//��ȡ������

		v_iWorking=2;													//�����������ܲ�ѯ
		v_pIterator_Frame->second.v_iThread_Exit=1;						//�̣߳�����
		v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);			//������ѯ���ݿ��߳�
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����С���б�
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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
//	�������ܣ�ʱ��ѡ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CLTE_PM_Data_Cell_Time	v_cDlg;										//ʵ����

	//��ȡ������1
	m_cLog_Date.GetTime(v_dDate_Select[0].v_dDate);						//��ȡ������
	v_dDate_Select[0].v_iDay=m_cLog_Day.GetCurSel();					//��ȡ������

	//ʱ��ѡ��
	v_cDlg.v_dDate_Select=&v_dDate_Select[0];							//����ѡ��
	v_cDlg.v_dTime_Select=&v_dTime_Select[0];							//ʱ��ѡ��
	v_cDlg.v_mTime=&v_mTime;											//ʱ������
	if (v_cDlg.DoModal()==IDOK)											//ȷ�Ϸ��أ�����
	{
		m_cLog_Date.SetTime(v_dDate_Select[0].v_dDate);					//����1������
		m_cLog_Day.SetCurSel(v_dDate_Select[0].v_iDay);					//����1������
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ʱ����������ͳ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=3;													//������������ͳ��
	v_pIterator_Frame->second.v_iThread_Exit=1;						//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);			//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CLTE_PM_Data_Cell::My_Thread_Inoput(LPVOID lparam)
{
	CLTE_PM_Data_Cell *  lp_this = NULL ;
	lp_this = (CLTE_PM_Data_Cell *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��������Ч
	lp_this->m_cExcel.EnableWindow(false);								//��������Ч
	lp_this->m_cLog_Date.EnableWindow(false);							//��������Ч
	lp_this->m_cLog_Day.EnableWindow(false);							//��������Ч
	lp_this->m_cType.EnableWindow(false);								//��������Ч
	lp_this->m_cCheck.EnableWindow(false);								//��������Ч
	lp_this->m_cPM_Cell.EnableWindow(false);							//��������Ч
	lp_this->m_cCase_Cell.EnableWindow(false);							//��������Ч
	lp_this->m_cCase_PM.EnableWindow(false);							//��������Ч
	lp_this->m_cCase_Select.EnableWindow(false);						//��������Ч
	lp_this->m_cTime_Check.EnableWindow(false);							//��������Ч
	lp_this->m_cTj_Type.EnableWindow(false);							//��������Ч
	lp_this->m_cSetup_Time.EnableWindow(false);							//��������Ч
	lp_this->m_cTj.EnableWindow(false);									//��������Ч
	lp_this->m_cExcel_Tj.EnableWindow(false);							//��������Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(true);								//��������Ч
	lp_this->m_cLog_Date.EnableWindow(true);							//��������Ч
	lp_this->m_cLog_Day.EnableWindow(true);								//��������Ч
	lp_this->m_cType.EnableWindow(true);								//��������Ч
	lp_this->m_cCheck.EnableWindow(true);								//��������Ч
	lp_this->m_cPM_Cell.EnableWindow(true);								//��������Ч
	lp_this->m_cCase_Cell.EnableWindow(true);							//��������Ч
	lp_this->m_cCase_PM.EnableWindow(true);								//��������Ч
	lp_this->m_cCase_Select.EnableWindow(true);							//��������Ч
	lp_this->m_cTime_Check.EnableWindow(true);							//��������Ч
	lp_this->m_cTj_Type.EnableWindow(true);								//��������Ч
	lp_this->m_cSetup_Time.EnableWindow(true);							//��������Ч
	lp_this->m_cTj.EnableWindow(true);									//��������Ч
	lp_this->m_cExcel_Tj.EnableWindow(true);							//��������Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Cell();												//Cell��ѯ
		break;
	case 2:
		m_cQuary_Data.ShowWindow(SW_SHOW);								//��ʾ
		m_cQuary_Name.ShowWindow(SW_SHOW);								//��ʾ
		PostMessage(WM_SIZE);											//ˢ�£�

		My_LoadData_PM();												//װ��������Ϣ

		v_iWorking=0;													//��λ
		m_cQuary_Data.ShowWindow(SW_HIDE);								//����
		m_cQuary_Name.ShowWindow(SW_HIDE);								//����
		PostMessage(WM_SIZE);											//ˢ�£�
		break;
	case 3:
		My_LoadData_PM_Tj();											//����ͳ��
		break;
	case 4:
		My_LoadData_Para();												//��ȡ�����ܲ���
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��������Ϣ
//	��    �ݣ��������ƣ�15m/1H���ȣ��������ͣ������У�С�������ڡ����У��������͡�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_LoadData_PM()
{
	CString			v_sTemp,v_sCell="",v_sTime;							//��ʱ����
	int				v_iDay_Sum;											//��ʱ����
	int				v_iNum;												//��ʱ����
	bool			v_bPM=false;										//��ʱ����
	COleDateTime	v_dDate_Start,v_dDate_End,v_dDate;					//��ѯ��ʼ���ڡ���������
	CString			v_sTime_Start,v_sTime_End,v_sTable_Num;				//����

	mDate_Int		v_mDate_Select;										//����
	COleDateTime	v_dBeg,v_dEnd;

	v_dBeg=COleDateTime::GetCurrentTime();

	//��1�����ݳ�ʼ��
	My_PM_Data_Delete();												//�������
	m_cList_PM.DeleteAllItems();										//����б�����
	m_cList_Tj.DeleteAllItems();										//����б�����

	//��ȡ��������1������
	for (int vi=0;vi<5;vi++)											//����5������
	{
		if (v_dDate_Select[vi].v_bFlag)									//ѡ�У�����
			v_mDate_Select.insert(pair<COleDateTime,int>(v_dDate_Select[vi].v_dDate,v_dDate_Select[vi].v_iDay+1));	//�������ӡ����ڡ�
	}

	//��2��С����ȡ��ѡ�е�С��ID (���ݲ�ѯ)
	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//�����б�
	{
		if(m_cList.GetCheck(vi))										//�жϣ�ѡ��״̬������
		{
			if (!v_sCell.IsEmpty())										//��Ϊ�գ�����
				v_sCell+=",";											//���ӣ��ָ���
			v_sCell+="'"+m_cList.GetItemText(vi,2)+"'";					//��ȡС��ID
		}
	}
	if(v_sCell.IsEmpty())												//Ϊ�գ�����
	{
		MessageBox("  ��ѡ����Ч��С�� !!! ");							//������ʾ
		return;
	}

	//��3�����ܴ�����ѡ�е����ܣ�����
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name_C;								//������
	for (v_pLTE_PM_Type=v_mLTE_PM_Type.begin();v_pLTE_PM_Type!=v_mLTE_PM_Type.end();v_pLTE_PM_Type++)
	{
		for (v_pPM_Name_C=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name_C!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name_C++)
		{
			if (v_pPM_Name_C->second.v_bOK)								//����ѡ�У�����	
			{
				v_bPM=true;												//������Ч
				break;													//�˳�
			}
		}
		if (v_bPM)														//������Ч����
			break;														//�˳�
	}
	if(!v_bPM)															//������Ч������
	{
		MessageBox("  ��ѡ����Ч������ !!! ");							//������ʾ
		return;
	}

	//��4����ȡ���ܱ�(15mins��ÿ��һ����  Hour��5��һ����һ��������)
	v_iNum=0;															//������
	mDate_Int::iterator		v_pDate;									//������
	v_pDate=v_mDate_Select.begin();										//��ָ��
	while(v_pDate!=v_mDate_Select.end())								//����������
	{
		//��ȡ�����ڷ�Χ
		v_dDate_Start=v_pDate->first;									//��ã���ѯ��ʼ����
		v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//����ʱ�䣺00:00:00
		v_iDay_Sum=v_pDate->second;										//��ã���ѯ����
		v_dDate_End =v_dDate_Start + COleDateTimeSpan(v_iDay_Sum,0,0,0);//��ã���ѯ��������

		//��ȡ����������
		v_iNum++;														//������
		if (m_cType.GetCurSel()==1)										//15m����
		{
			while(v_dDate_Start<v_dDate_End)							//��ʼ����<��������
			{
				v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//��ʼʱ��
				v_sTime_End=v_dDate_Start.Format("%Y-%m-%d")+" 23:59:59";	//����ʱ��
				v_sTemp=v_dDate_Start.Format("%Y%m%d");					//������
				My_LoadData_PM_Table(v_sTemp,v_sCell,v_sTime_Start,v_sTime_End,v_iNum,v_mDate_Select.size());	//װ����������
				v_dDate_Start+=COleDateTimeSpan(1,0,0,0);				//����һ��
			}
		}
		else
		{
			v_dDate=v_dDate_Start;										//��ֵ����ǰ����
			v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());		//��ȡ�����
			while(v_dDate<v_dDate_End)									//ʱ�䷶Χ�ڣ�����
			{
				if (v_pIterator_Frame->second.v_iThread_Exit==2)		//ǿ���˳�������
					break;												//�˳�

				v_dDate+=COleDateTimeSpan(1,0,0,0);						//����һ��
				if (My_Get_Table_Num(v_dDate.GetDay())!=v_sTable_Num || v_dDate>=v_dDate_End)
				{
					v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";		//��ʼʱ��
					v_sTime_End=v_dDate.Format("%Y-%m-%d")+" 00:00:00";	//����ʱ��
					v_sTemp=v_dDate_Start.Format("%Y%m");				//����
					v_sTemp=v_sTemp+"_"+v_sTable_Num;					//�ļ�����
					My_LoadData_PM_Table(v_sTemp,v_sCell,v_sTime_Start,v_sTime_End,v_iNum,v_mDate_Select.size());	//װ����������
					v_dDate_Start=v_dDate;									//�������ʼʱ��
					v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());	//��ȡ�����
				}
			}
		}

		v_pDate++;														//�ݼ�
	}

	v_dEnd=COleDateTime::GetCurrentTime();

	COleDateTimeSpan v_dSpan=v_dEnd-v_dBeg;								//ʱ��
	int  ss=v_dSpan.GetTotalSeconds();
	v_sTemp.Format("%d",ss);
	m_cSpan.SetWindowText(v_sTemp);										//��ʾ��ʱ��

	//��5����ʾ�����б���(������������)
	My_PM_Show_Data();													//��ʾ��õ���������
	
	//��6���������
	v_mDate_Select.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ�����
//	��	  ����1��1-5��	2��6-10��	3��11-15��	4��16-20��	5��21-25��	6��26--
//------------------------------------------------------------------------------------------------------			
CString CLTE_PM_Data_Cell::My_Get_Table_Num(int v_iDay)
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
//	�������ܣ������б���Ϣ��Ӧ����
//	��	  ���������б���Ҫ��Ԫ�ز���(�С���)�������������в�ѯ���ݣ���ֵ���б��С�
//------------------------------------------------------------------------------------------------------	
void CLTE_PM_Data_Cell::OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ�������б���ʾ���ݻ�ȡ
	m_cList_PM.Get_List_Data(pDispInfo);								//�����б���ʾ���ݻ�ȡ

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��������Ϣ������ͳ�ơ�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_LoadData_PM_Tj()
{
	CString			v_sTemp,v_sCell="",v_sTime,v_sDate;					//��ʱ����
	bool			v_bPM=false;										//��ʱ����
	COleDateTime	v_dDate_Start,v_dDate_End,v_dDate;					//��ѯ��ʼ���ڡ���������
	CString			v_sTime_Start,v_sTime_End;							//����
	int				v_iSum_Time,v_iSum_Cell;							//��ʱ����

	int				v_iHour,v_iDay=-1;									//Сʱ����
	map_PM_Time_P	v_mPM_Time_P;										//����
	Time_PM_Data	v_cTime_PM_Data;									//�ṹ
	mString_Double	v_mPM_Tj_Date;										//����ͳ�ƣ�����
	mString_Double	v_mPM_Tj_Cell;										//����ͳ�ƣ�Cell

	//��1�����ݳ�ʼ��
	My_PM_Data_P_Delete();												//�������
	m_cList_PM.DeleteAllItems();										//����б�����
	m_cList_Tj.DeleteAllItems();										//����б�����

	//��2�����ܴ�����ѡ�е����ܣ�����
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name_C;								//������
	for (v_pLTE_PM_Type=v_mLTE_PM_Type.begin();v_pLTE_PM_Type!=v_mLTE_PM_Type.end();v_pLTE_PM_Type++)
	{
		for (v_pPM_Name_C=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name_C!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name_C++)
		{
			if (v_pPM_Name_C->second.v_bOK)								//����ѡ�У�����	
			{
				v_bPM=true;												//������Ч
				break;													//�˳�
			}
		}
		if (v_bPM)														//������Ч����
			break;														//�˳�
	}
	if(!v_bPM)															//������Ч������
	{
		MessageBox("  ��ѡ����Ч������ !!! ");							//������ʾ
		return;
	}

	//��3��ʱ������ͳ��
	double					v_dValue;									//��ʱ����
	PM_Time_P_Struct		v_cPM_Time_P;								//�ṹ
	mInt_String::iterator	v_pTime;									//������

	map_PM_Cell::iterator	v_pPM_Cell;									//������
	map_PM_Time::iterator	v_pPM_Time;									//������
	mString_Double::iterator v_pPM_Name;								//������

	map_PM_Cell_P::iterator	v_pPM_Cell_P;								//������
	map_PM_Time_P::iterator	v_pPM_Time_P;								//������
	mString_Double::iterator v_pPM_Name_P;								//������

	m_cList_Tj.SetRedraw(false);										//��ֹ��˸
	for(v_pPM_Cell=v_mPM_Cell.begin();v_pPM_Cell!=v_mPM_Cell.end();v_pPM_Cell++)	//������С�� ������
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//ǿ���˳�������
			break;														//�˳�

		v_mPM_Tj_Cell.clear();											//�������
		v_iSum_Cell=0;													//��ʼ����С��ʱ������
		v_mPM_Tj_Date.clear();											//�������
		v_iSum_Time=0;													//��ʼ������������
		v_iDay=-1;														//��ʼ��������
		for(v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.begin();v_pPM_Time!=v_pPM_Cell->second.v_mTime_PM_Data.end();v_pPM_Time++)	//���������� ������
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			if(v_iDay!=v_pPM_Time->first.GetDay())						//��仯������
			{
				if(v_mPM_Tj_Date.size()!=0)								//����ͳ�ƣ�����
					My_PM_Show_Data_Tj(&v_mPM_Tj_Date,v_pPM_Cell->first,v_pPM_Cell->second.v_sRegion,v_pPM_Cell->second.v_sOnline,v_sDate,v_iSum_Time);	//��ʾ������ͳ��

				v_iDay=v_pPM_Time->first.GetDay();						//��ȡ����
				v_sDate=v_pPM_Time->first.Format("%Y-%m-%d");			//��ֵ������
				v_mPM_Tj_Date.clear();									//�������
				v_iSum_Time=0;											//��ʼ������������
			}
			if (m_cTime_Check.GetCheck())								//ʱ����Ч������
			{
				//��ȡ��Сʱ��������ʱ���ڣ���ͳ�ƣ�
				v_iHour=v_pPM_Time->first.GetHour();					//��ȡ������Сʱ
				v_sTime=v_pPM_Time->first.Format("%Y-%m-%d_");			//��ֵ������
				v_pTime=v_mTime.find(v_iHour);							//���ң�Сʱ��������
				if (v_pTime==v_mTime.end())								//���ȣ�����ʱ���ڣ�����
					continue;											//��һ����

				//��ʼͳ�ƣ�
				v_sTime+=v_pTime->second;								//��ȡ��ʱ��
				v_pPM_Cell_P=v_mPM_Cell_P.find(v_pPM_Cell->first);		//С�����ڣ�����
				if (v_pPM_Cell_P==v_mPM_Cell_P.end())					//�����ڣ�����
				{
					v_cPM_Time_P.v_sOnline=v_pPM_Cell->second.v_sOnline;//״̬
					v_cPM_Time_P.v_sRegion=v_pPM_Cell->second.v_sRegion;//Ӫҵ��
					v_mPM_Cell_P.insert(pair<CString,PM_Time_P_Struct>(v_pPM_Cell->first,v_cPM_Time_P));	//��������
				}
				v_pPM_Cell_P=v_mPM_Cell_P.find(v_pPM_Cell->first);

				v_pPM_Time_P=v_pPM_Cell_P->second.v_mTime_PM_Data.find(v_sTime);	//���ң�ʱ�� ������
				if (v_pPM_Time_P==v_pPM_Cell_P->second.v_mTime_PM_Data.end())		//������
				{
					v_cTime_PM_Data.v_iSum=0;							//��ʼ����ʱ������
					v_pPM_Cell_P->second.v_mTime_PM_Data.insert(pair<CString,Time_PM_Data>(v_sTime,v_cTime_PM_Data));	//��������
				}
				v_pPM_Time_P=v_pPM_Cell_P->second.v_mTime_PM_Data.find(v_sTime);	//
				v_pPM_Time_P->second.v_iSum++;							//ʱ������++
			}
			v_iSum_Cell++;												//��������С��ʱ������
			v_iSum_Time++;												//������������ʱ������
			for (v_pPM_Name=v_pPM_Time->second.begin();v_pPM_Name!=v_pPM_Time->second.end();v_pPM_Name++)	//���������ܣ�����
			{
				if (v_pPM_Name->second!=-1)								//�����쳣������
					v_dValue=v_pPM_Name->second;						//��ֵ����Ч����
				else
					v_dValue=0;											//�����쳣����ֵΪ0
				//(1) ʱ��ͳ��
				if (m_cTime_Check.GetCheck())							//ʱ����Ч������
				{
					v_pPM_Name_P=v_pPM_Time_P->second.v_mPM_Data.find(v_pPM_Name->first);	//���ң����ܣ�����
					if (v_pPM_Name_P==v_pPM_Time_P->second.v_mPM_Data.end())	//û�ҵ�������
						v_pPM_Time_P->second.v_mPM_Data.insert(pair<CString,double>(v_pPM_Name->first,v_dValue));	//��������
					else
						v_pPM_Name_P->second+=v_dValue;					//���
				}

				//(2) ����ͳ��
				v_pPM_Name_P=v_mPM_Tj_Date.find(v_pPM_Name->first);		//���ң����ܣ�����
				if (v_pPM_Name_P==v_mPM_Tj_Date.end())					//û�ҵ�������
					v_mPM_Tj_Date.insert(pair<CString,double>(v_pPM_Name->first,v_dValue));		//��������
				else
					v_pPM_Name_P->second+=v_dValue;						//���

				//(3) С��ͳ��
				v_pPM_Name_P=v_mPM_Tj_Cell.find(v_pPM_Name->first);		//���ң����ܣ�����
				if (v_pPM_Name_P==v_mPM_Tj_Cell.end())					//û�ҵ�������
					v_mPM_Tj_Cell.insert(pair<CString,double>(v_pPM_Name->first,v_dValue));		//��������
				else
					v_pPM_Name_P->second+=v_dValue;						//���
			}
		}
		if (v_mPM_Tj_Date.size()!=0)									//����ͳ�ƣ�����
			My_PM_Show_Data_Tj(&v_mPM_Tj_Date,v_pPM_Cell->first,v_pPM_Cell->second.v_sRegion,v_pPM_Cell->second.v_sOnline,v_sDate,v_iSum_Time);		//��ʾ������ͳ��

		if (v_mPM_Tj_Cell.size()!=0)									//С��ͳ�ƣ�����
			My_PM_Show_Data_Tj(&v_mPM_Tj_Cell,v_pPM_Cell->first,v_pPM_Cell->second.v_sRegion,v_pPM_Cell->second.v_sOnline,"С���ܼ�",v_iSum_Cell);	//��ʾ��С��ͳ��
	}
	m_cList_Tj.SetRedraw(true);											//�б���ʾ

	//��4����ʾ�����б���(������������)
	m_cQuary_Data.SetWindowText("  ��ʾ��ѯ���......");				//��ʾ����ѯ����

	My_PM_Show_Data();													//��ʾ��õ���������

	//��5���������
	v_mPM_Tj_Date.clear();												//�������
	v_mPM_Tj_Cell.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������ݼ��������б�����������v_mPM_Cell�С�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_PM_Show_Data()
{
	int		v_iNum3,v_iIndex=0;											//��ʱ����
	CString	v_sStr,v_sTime,v_sTemp,v_sPm_Value;							//��ʱ����

	mInt_Double	v_mInt_Double;											//��ʱ����

	mLTE_PM::iterator		v_pPM_Field_Num;							//������
	map_PM_Cell::iterator	v_pPM_Cell;									//������
	map_PM_Time::iterator	v_pPM_Time;									//������
	mString_Double::iterator v_pPM_Name;								//������

	//ʱ��ͳ�Ʊ���
	map_PM_Cell_P::iterator	v_pPM_Cell_P;								//������
	map_PM_Time_P::iterator	v_pPM_Time_P;								//������

	//��1����λ�������б�
	mInt_String		v_mLine_Data;										//�����б�������
	m_cList_PM.Put_Line_Begin();										//�б�������ݿ�ʼ

	//��2���жϣ���ϸ��ʱ�Σ�����
	if (!m_cTime_Check.GetCheck())										//ʱ����Ч������
	{																	//������ϸ��ʾ
		//��3��������ϸ
		for (v_pPM_Cell=v_mPM_Cell.begin();v_pPM_Cell!=v_mPM_Cell.end();v_pPM_Cell++)	//������С�� ������
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			//������ʱ��
			for(v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.begin();v_pPM_Time!=v_pPM_Cell->second.v_mTime_PM_Data.end();v_pPM_Time++)	//���������� ������
			{
				if (v_pIterator_Frame->second.v_iThread_Exit==2)		//ǿ���˳�������
					break;												//�˳�

				//���ݸ�λ
				v_mLine_Data.clear();									//�������

				v_iIndex++;												//������
				v_sTemp.Format(" %06d",v_iIndex);
				v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));							//���
				v_mLine_Data.insert(pair<int,CString>(1,v_pPM_Cell->first));				//Cell
				v_mLine_Data.insert(pair<int,CString>(2,v_pPM_Cell->second.v_sRegion));		//Ӫҵ��
				v_mLine_Data.insert(pair<int,CString>(3,v_pPM_Cell->second.v_sOnline));		//״̬
				v_sTime=v_pPM_Time->first.Format("%Y-%m-%d_%H:%M:%S");						//��ֵ������
				v_mLine_Data.insert(pair<int,CString>(4,v_sTime));							//����ʱ��


				//����������
				v_mInt_Double.clear();									//�������
				for (v_pPM_Name=v_pPM_Time->second.begin();v_pPM_Name!=v_pPM_Time->second.end();v_pPM_Name++)	//���������ܣ�����
				{
					v_pPM_Field_Num = v_mPM_Field_Num.find(v_pPM_Name->first);	//���ң���������
					if (v_pPM_Field_Num!=v_mPM_Field_Num.end())			//�ҵ�������
						v_mInt_Double.insert(pair<int,double>(v_pPM_Field_Num->second.v_iNum,v_pPM_Name->second));	//����������
				}

				//�������л�ȡ��������
				v_iNum3=5;												//��ʼ��
				mInt_Double::iterator v_pNum_Value;						//������
				for (v_pNum_Value=v_mInt_Double.begin();v_pNum_Value!=v_mInt_Double.end();v_pNum_Value++)
				{
					while(v_pNum_Value->first>v_iNum3)					//�п��У���
					{
						v_mLine_Data.insert(pair<int,CString>(v_iNum3,""));		//�ֶ����ݣ���
						v_iNum3++;										//������
					}
					v_sStr.Format("%8.2f",v_pNum_Value->second);		//�ַ���
					v_mLine_Data.insert(pair<int,CString>(v_iNum3,v_sStr));		//�ֶ�����
					v_iNum3++;											//������
				}

				//�������ݷ����б�
				m_cList_PM.Put_Line_Data(v_mLine_Data);					//�б�������������
			}
		}
	}
	else
	{
		//��3������ʱ��
		for (v_pPM_Cell_P=v_mPM_Cell_P.begin();v_pPM_Cell_P!=v_mPM_Cell_P.end();v_pPM_Cell_P++)	//������С�� ������
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			//������ʱ��
			for(v_pPM_Time_P=v_pPM_Cell_P->second.v_mTime_PM_Data.begin();v_pPM_Time_P!=v_pPM_Cell_P->second.v_mTime_PM_Data.end();v_pPM_Time_P++)	//���������� ������
			{
				if (v_pIterator_Frame->second.v_iThread_Exit==2)		//ǿ���˳�������
					break;												//�˳�

				//���ݸ�λ
				v_mLine_Data.clear();									//�������

				v_iIndex++;												//������
				v_sTemp.Format(" %06d",v_iIndex);
				v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));							//���
				v_mLine_Data.insert(pair<int,CString>(1,v_pPM_Cell_P->first));				//Cell
				v_mLine_Data.insert(pair<int,CString>(2,v_pPM_Cell_P->second.v_sRegion));	//Ӫҵ��
				v_mLine_Data.insert(pair<int,CString>(3,v_pPM_Cell_P->second.v_sOnline));	//״̬
				v_mLine_Data.insert(pair<int,CString>(4,v_pPM_Time_P->first));				//����ʱ��

				//����������
				v_mInt_Double.clear();									//�������
				for (v_pPM_Name=v_pPM_Time_P->second.v_mPM_Data.begin();v_pPM_Name!=v_pPM_Time_P->second.v_mPM_Data.end();v_pPM_Name++)	//���������ܣ�����
				{
					v_pPM_Field_Num = v_mPM_Field_Num.find(v_pPM_Name->first);	//���ң���������
					if (v_pPM_Field_Num!=v_mPM_Field_Num.end())			//�ҵ�������
					{
						if (m_cTj_Type.GetCurSel()==0)					//��ʾ���ͣ����
							v_mInt_Double.insert(pair<int,double>(v_pPM_Field_Num->second.v_iNum,v_pPM_Name->second));	//����������
						else											//��ʾ���ͣ�ƽ��
						{
							if (v_pPM_Time_P->second.v_iSum!=0)			//ͳ�ƴ�����Ϊ�㣿����
								v_mInt_Double.insert(pair<int,double>(v_pPM_Field_Num->second.v_iNum,v_pPM_Name->second/v_pPM_Time_P->second.v_iSum));	//����������ƽ��
						}
					}
				}

				//�������л�ȡ��������
				v_iNum3=5;												//��ʼ��
				mInt_Double::iterator v_pNum_Value;						//������
				for (v_pNum_Value=v_mInt_Double.begin();v_pNum_Value!=v_mInt_Double.end();v_pNum_Value++)
				{
					while(v_pNum_Value->first>v_iNum3)					//�п��У���
					{
						v_mLine_Data.insert(pair<int,CString>(v_iNum3,"-1"));		//�ֶ����ݣ���
						v_iNum3++;										//������
					}
					v_sStr.Format("%8.2f",v_pNum_Value->second);		//�ַ���
					v_mLine_Data.insert(pair<int,CString>(v_iNum3,v_sStr));		//�ֶ�����
					v_iNum3++;											//������
				}

				//�������ݷ����б�
				m_cList_PM.Put_Line_Data(v_mLine_Data);					//�б�������������
			}
		}
	}

	//��4����λ
	v_mInt_Double.clear();												//�������

	//��5�����ã������б���Ч
	v_mLine_Data.clear();												//�������
	m_cList_PM.Put_Line_End();											//�б�������ݽ���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ʾͳ�Ƶ�ʱ��������ϸ��������v_mPM_Cell_P�С�(���������б��滻)
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_PM_Show_Data_P()
{
	CString					v_sPm_Value,v_sTime,v_sTemp;				//��ʱ����
	int						v_iIndex;									//��ʱ����

	mLTE_PM::iterator		v_pPM_Field_Num;							//������
	map_PM_Cell_P::iterator	v_pPM_Cell_P;								//������
	map_PM_Time_P::iterator	v_pPM_Time_P;								//������
	mString_Double::iterator v_pPM_Name;								//������

	m_cList_PM.SetRedraw(false);										//��ֹ��˸
	//������С��
	for (v_pPM_Cell_P=v_mPM_Cell_P.begin();v_pPM_Cell_P!=v_mPM_Cell_P.end();v_pPM_Cell_P++)	//������С�� ������
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//ǿ���˳�������
			break;														//�˳�

		//������ʱ��
		for(v_pPM_Time_P=v_pPM_Cell_P->second.v_mTime_PM_Data.begin();v_pPM_Time_P!=v_pPM_Cell_P->second.v_mTime_PM_Data.end();v_pPM_Time_P++)	//���������� ������
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_iIndex = m_cList_PM.InsertItem(0xffffff,_T("0"));
			v_sTemp.Format("%06d",v_iIndex+1);
			m_cList_PM.SetItemText(v_iIndex,0,v_sTemp);					//���
			m_cList_PM.SetItemText(v_iIndex,1,v_pPM_Cell_P->first);		//Cell
			m_cList_PM.SetItemText(v_iIndex,2,v_pPM_Cell_P->second.v_sRegion);	//Ӫҵ��
			m_cList_PM.SetItemText(v_iIndex,3,v_pPM_Cell_P->second.v_sOnline);	//״̬
			m_cList_PM.SetItemText(v_iIndex,4,v_pPM_Time_P->first);		//

			//����������
			for (v_pPM_Name=v_pPM_Time_P->second.v_mPM_Data.begin();v_pPM_Name!=v_pPM_Time_P->second.v_mPM_Data.end();v_pPM_Name++)	//���������ܣ�����
			{
				v_pPM_Field_Num = v_mPM_Field_Num.find(v_pPM_Name->first);	//���ң���������
				if (v_pPM_Field_Num!=v_mPM_Field_Num.end())				//�ҵ�������
				{
					//��ʾ
					if (m_cTj_Type.GetCurSel()==0)						//��ʾ���ͣ����
						v_sTemp.Format("%8.2f",v_pPM_Name->second);		//�ַ��������
					else												//��ʾ���ͣ�ƽ��
					{
						if (v_pPM_Time_P->second.v_iSum!=0)				//ͳ�ƴ�����Ϊ�㣿����
							v_sTemp.Format("%8.2f",v_pPM_Name->second/v_pPM_Time_P->second.v_iSum);	//�ַ�����ƽ��
						else
							v_sTemp="-1";								//��Ч
					}
					m_cList_PM.SetItemText(v_iIndex,v_pPM_Field_Num->second.v_iNum,v_sTemp);	//����
				}
			}
		}
	}
	m_cList_PM.SetRedraw(true);											//�б���ʾ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ʾʱ����������
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_PM_Show_Data_Tj(mString_Double* v_mPM_Tj_Date,CString v_sCell,CString v_sRegion,CString v_sOnline,CString v_sDate,int v_iSum_Time)
{
	CString					v_sTemp;									//��ʱ����
	int						v_iIndex;									//��ʱ����

	mLTE_PM::iterator		v_pPM_Field_Num;							//������
	mString_Double::iterator v_pPM_Name;								//������

	v_iIndex = m_cList_Tj.InsertItem(0xffffff,_T("0"));
	v_sTemp.Format("%06d",v_iIndex+1);
	m_cList_Tj.SetItemText(v_iIndex,0,v_sTemp);							//���
	m_cList_Tj.SetItemText(v_iIndex,1,v_sCell);							//Cell
	m_cList_Tj.SetItemText(v_iIndex,2,v_sRegion);						//Ӫҵ��
	m_cList_Tj.SetItemText(v_iIndex,3,v_sOnline);						//״̬
	m_cList_Tj.SetItemText(v_iIndex,4,v_sDate);							//����

	for (v_pPM_Name=v_mPM_Tj_Date->begin();v_pPM_Name!=v_mPM_Tj_Date->end();v_pPM_Name++)	//���������ܣ�����
	{
		v_pPM_Field_Num = v_mPM_Field_Num.find(v_pPM_Name->first);		//���ң���������
		if (v_pPM_Field_Num!=v_mPM_Field_Num.end())						//�ҵ�������
		{
			if (m_cTj_Type.GetCurSel()==0)								//��ʾ���ͣ����
				v_sTemp.Format("%12.2f",v_pPM_Name->second);			//�ַ���
			else
			{
				if (v_iSum_Time!=0)										//ͳ�ƴ�����Ϊ�㣿����
					v_sTemp.Format("%8.2f",v_pPM_Name->second/v_iSum_Time);		//�ַ�����ƽ��
				else
					v_sTemp="-1";										//��Ч
			}
			m_cList_Tj.SetItemText(v_iIndex,v_pPM_Field_Num->second.v_iNum,v_sTemp);		//����
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ�����ܱ�ÿ�λ�ȡ������ܱ�
//	��	  ����v_sTable�����¡�201605����	v_sCell��С����		v_sDate_Ben����ʼ���ڣ�		v_sDate_End���������ڣ�	v_iNum��������ţ�v_iSum������������
//	��    �ݣ��������ƣ�15m/1H���ȣ��������ͣ������У�С�������ڡ����У��������͡�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_LoadData_PM_Table(CString v_sDate,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End,int v_iNum,int v_iSum)
{
	CString			v_sTemp,v_sTable,v_sField;							//��ʱ����
	int				v_iPM_Num,v_iPM_Sum;								//��ʱ����
	CString			v_sWorking_1,v_sWorking_2,v_sWorking_3;				//���ܻ�ȡ����

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	//��1������ͳ��(��ȡ���ܱ�����)
	v_iPM_Sum=0;														//����
	for (v_pLTE_PM_Type=v_mLTE_PM_Type.begin();v_pLTE_PM_Type!=v_mLTE_PM_Type.end();v_pLTE_PM_Type++)
	{
		for (v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
		{
			if (v_pPM_Name->second.v_bOK)								//����ѡ�У�����	
			{
				v_iPM_Sum++;											//����
				break;													//�˳�
			}
		}
	}

	//��2�����ܻ�ȡ(������������(ԭʼ��6�֣���������1�֣��Զ���1�֣���8��)��ÿ�����ܻ�ȡһ��)
	v_iPM_Num=0;														//������
	for (v_pLTE_PM_Type=v_mLTE_PM_Type.begin();v_pLTE_PM_Type!=v_mLTE_PM_Type.end();v_pLTE_PM_Type++)
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//ǿ���˳�������
			break;														//�˳�

		//��Ϊ���ࣺ(1)�Զ��壻(2)������ԭʼ�����㣻
		if (v_pLTE_PM_Type->first!="�Զ���")							//��1������
		{
			//��3����ȡ���ֶ���Ϣ
			v_sField="";												//��λ���ֶ�
			for (v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
			{
				if (v_pPM_Name->second.v_bOK)							//����ѡ�У�����	
				{
					if (!v_sField.IsEmpty())							//�ֶβ�Ϊ�գ�����
						v_sField+=",";									//�ָ���
					v_sField+=v_pPM_Name->first;						//��ȡ���ֶ���Ϣ
				}
			}

			//��4����ȡPM����(15mins��������_�����գ�1h��������_����_��ţ�)
			if (!v_sField.IsEmpty())									//�ֶ���Ч������
			{
				//��ȡ�����ܡ�1�����ܱ�
				v_iPM_Num++;											//������
				v_sTable = "Pm_LTE_"+v_pLTE_PM_Type->second.v_sTable_Key+"_"+v_sDate;					//������
				My_LoadData_PM_Data(v_pLTE_PM_Type,v_sTable,v_sField,v_sCell,v_sDate_Ben,v_sDate_End);	//װ����������
			}
		}
		else															//��2���Զ���
		{
			v_sField="";												//��λ���ֶ�
			for (v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
			{
				if (v_pPM_Name->second.v_bOK)							//����ѡ�У�����	
				{
					v_sTable = "Pm_LTE_"+v_pLTE_PM_Type->second.v_sTable_Key+"_"+v_sDate;				//������
					My_LoadData_PM_Data_Cal(v_sTable,v_pPM_Name->first,v_sCell,v_sDate_Ben,v_sDate_End);//װ����������
					v_sField="ִ��";									//���ã���Ч��־
				}
			}
			if (!v_sField.IsEmpty())									//�ֶ���Ч������
				v_iPM_Num++;											//������
		}
		
		//��ʾ����������
		v_sWorking_1.Format("  ��ѯ�����飺%d/%d��",v_iNum,v_iSum);		//���ܲ�ѯ���ȡ�����(���)��
		v_sWorking_2=v_sDate_Ben.Left(10);								//��ѯ����
		v_sWorking_3.Format("����ѯ�����飺%d/%d�����Ժ�......",v_iPM_Num,v_iPM_Sum);		//���ܲ�ѯ���ȡ�����(���)��
		m_cQuary_Data.SetWindowText(v_sWorking_1+"��ѯ���ڣ�"+v_sWorking_2+v_sWorking_3);	//��ʾ����ѯ����
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ����������
//	��    ����v_pLTE_PM_Type����������	v_sTable�������ƣ�	v_sField���ֶΣ�	v_sCell��С����		v_sDate_Ben����ʼ���ڣ�		v_sDate_End����������
//	��    �����������ݷ�������
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_LoadData_PM_Data(mLTE_PM_Type::iterator v_pLTE_PM_Type,CString v_sTable,CString v_sField,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End)
{
	CString			v_sSql,v_sPM_Cell,v_sRegion,v_sOnline,v_sField_New="";	//��ʱ����
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_tDate;											//����
	double			v_dValue;											//������ֵ
	int				v_iCell_ID=-1,v_iCell_ID_New=-1,v_iLen;				//��ʱ����
	PM_Time_Struct	v_cPM_Time;											//�ṹ

	set<CString>	v_mTable_Columns;									//��ṹ
	set<CString>::iterator	v_pTable_Columns;							//��ṹ

	map_PM_Time				v_mPM_Time;									//����
	mString_Double			v_mPM_Name;									//����

	mLTE_PM::iterator		v_pPM_Name;									//������
	map_PM_Cell::iterator	v_pPM_Cell;									//������
	map_PM_Time::iterator	v_pPM_Time;									//������
	mString_Double::iterator v_pPM_Value;								//������
	map_Cell_ID::iterator	v_pIter_Cell;								//������

	_variant_t		Var;
	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		//��1����飺�������Ƿ���Ч�����ݣ������Ƽ���v_mTable_Name���������������Ч����鲻���������ݡ���
		set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//���ң���
		if (v_pTable==v_mTable_Name.end())								//û�ҵ������ݱ�
			return;														//��Ч����

		//��2����ȡ���ݱ�ṹ��������ѯ�ֶ���Ϣ
		My_Load_Columns(&v_mTable_Columns,v_sTable);

		while(!v_sField.IsEmpty())										//�ֶαȽϣ�����ڣ�ok�� �����ڣ��޳���
		{
			v_iLen=v_sField.Find(",");									//
			if (v_iLen>=0)
			{
				v_sTemp=v_sField.Left(v_iLen);							//��ȡ���ֶ�
				v_sField=v_sField.Mid(v_iLen+1);						//�޳�
			}
			else
			{
				v_sTemp=v_sField;										//��ȡ���ֶ�
				v_sField="";											//��Ч
			}
			
			v_pTable_Columns=v_mTable_Columns.find(v_sTemp);			//�ֶδ��ڣ�����
			if (v_pTable_Columns!=v_mTable_Columns.end())				//���ڣ�����
			{
				if (!v_sField_New.IsEmpty())							//���ֶΣ�����
					v_sField_New+=",";									//�ӣ�����
				v_sField_New+="["+v_sTemp+"]";							//����
			}
		}

		//��3�� �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(10*60);							//��ʱ����(10����)

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zTime,"+v_sField_New+" from "+v_sTable+" where zCell in("+v_sCell+") and zTime>='"+v_sDate_Ben
				+"' and zTime<'"+v_sDate_End+"' order by zCell";	//��ѯ���ݿ⣺ϵͳ��־

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			//��4��С����ȡ��С��_ID ==> С������
			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_iCell_ID=Var.intVal;									//��ȡ��Cell_ID
				if (v_iCell_ID!=v_iCell_ID_New)							//�仯��v_iCell_ID������
				{
					v_pIter_Cell=v_mCell_ID.find(v_iCell_ID);			//���ң�С��ID���ڣ�����
					if (v_pIter_Cell==v_mCell_ID.end())					//������
					{
						v_pRecordset->MoveNext();						//��һ��������
						continue;										//����
					}
					v_iCell_ID_New=v_iCell_ID;							//
					v_sPM_Cell=v_pIter_Cell->second.v_sCell;			//С��
					v_sOnline=v_pIter_Cell->second.v_sOnline;			//״̬
					v_sRegion=v_pIter_Cell->second.v_sRegion;			//Ӫҵ��
				}
			}
			else
				v_sPM_Cell="��Ч";

			//��5�����Ȼ�ȡ
			Var=v_pRecordset->GetCollect((_variant_t)"zTime");
			if (Var.vt != VT_NULL)
				v_tDate=Var.date;
			else
				v_tDate=COleDateTime::GetCurrentTime();					//Ĭ�ϣ���ǰ����


			//��6����ȡ��С��״̬������
			v_pPM_Cell=v_mPM_Cell.find(v_sPM_Cell);						//���ң�С�� ������
			if (v_pPM_Cell==v_mPM_Cell.end())							//������
			{
				v_cPM_Time.v_sOnline=v_sOnline;							//״̬
				v_cPM_Time.v_sRegion=v_sRegion;							//����
				v_mPM_Cell.insert(pair<CString,PM_Time_Struct>(v_sPM_Cell,v_cPM_Time));	//��������
			}
			v_pPM_Cell=v_mPM_Cell.find(v_sPM_Cell);						//
			
			//��7����������
			v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.find(v_tDate);//���ң����� ������
			if (v_pPM_Time==v_pPM_Cell->second.v_mTime_PM_Data.end())	//������
				v_pPM_Cell->second.v_mTime_PM_Data.insert(pair<COleDateTime,mString_Double>(v_tDate,v_mPM_Name));	//��������
			v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.find(v_tDate);//

			//��8���ֶλ�ȡ������
			v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();		//��ָ��
			while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())	//��������
			{
				if (v_pPM_Name->second.v_bOK)							//ѡ�У�����
				{
					//��9���ֶδ���
					v_pTable_Columns=v_mTable_Columns.find(v_pPM_Name->first);
					if (v_pTable_Columns!=v_mTable_Columns.end())		//���ڣ�����
					{
						Var=v_pRecordset->GetCollect((_variant_t)v_pPM_Name->first);
						if (Var.vt != VT_NULL)
							v_dValue=Var.dblVal;
						else
							v_dValue=0;

						v_pPM_Value=v_pPM_Time->second.find(v_pPM_Name->first);		//���ң��������ƣ�����
						if (v_pPM_Value==v_pPM_Time->second.end())					//�����ڣ�����
							v_pPM_Time->second.insert(pair<CString,double>(v_pPM_Name->first,v_dValue));	//��������
					}
				}

				v_pPM_Name++;											//�ݼ�
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

	//��10���������
	v_mTable_Columns.clear();											//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ���Զ�����������
//	��	  ����v_sTable�������ƣ�	v_sName_Cal���������ƣ�	v_sCell��С����	v_sDate_Ben����ʼ���ڣ�		v_sDate_End����������
//	��    �ݣ��������ƣ�15m/1H���ȣ��������ͣ������У�С�������ڡ����У��������͡�
//	��	  ����������������(v_sName_Cal)��ȷ���������ͼ�����Ч���ܣ���ȡ�������ݣ������Զ��幫ʽ�������ܣ����棻
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_LoadData_PM_Data_Cal(CString v_sTable,CString v_sName_Cal,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End)
{
	CString			v_sSql;												//��ʱ����
	CString			v_sTemp,v_sTime,v_sFormula,v_sType,v_sKey,v_sField;	//��ʱ����
	CString			v_sPM_Cell,v_sRegion,v_sOnline;						//��ʱ����
	int				v_iID=0,v_iCell_ID;									//��ʱ����
	int				v_iCell_ID_New=-1;									//��ʱ����
	COleDateTime	v_tDate;											//����
	double			v_dValue;											//������ֵ
	PM_Time_Struct	v_cPM_Time;											//�ṹ

	//���㹫ʽ
	CMyFormula		v_cFormula;											//��ʽ����

	mString_Double	v_mLTE_PM;											//�Զ�����������(��Ч����)
	mString_Double	v_mPM_Name;											//����

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name;									//������
	map_Cell_ID::iterator	v_pIter_Cell;								//������
	mString_Double::iterator	v_pIter_Value;							//������
	map_PM_Cell::iterator	v_pPM_Cell;									//������
	map_PM_Time::iterator	v_pPM_Time;									//������

	//��1����ȡ���Զ���������Ϣ[�������͡��Զ��幫ʽ��]
	if(!My_Get_Formula(v_sName_Cal,&v_sFormula,&v_sType))				//��ȡ���������͡��Զ��幫ʽ����Key
		return;															//��ȡʧ��
	
	if (v_sType=="�����Ľ���")											//����==>������
		v_sKey="TXT";
	else if (v_sType=="��ͨ")
		v_sKey="RRC";
	else if (v_sType=="�л�")
		v_sKey="THO";
	else if (v_sType=="MAC��")
		v_sKey="MAC";
	else if (v_sType=="����")
		v_sKey="PDCP";
	else if (v_sType=="����")
		v_sKey="PHY";

	v_sTable.Replace("Def",v_sKey);										//����������[���Զ����Ϊ��������]

	//��2����飺�������Ƿ���Ч�����ݣ������Ƽ���v_mTable_Name���������������Ч����鲻���������ݡ���
	set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);		//���ң���
	if (v_pTable==v_mTable_Name.end())									//û�ҵ������ݱ�
		return;															//��Ч����

	//��3����ʽ��飬1[v_mLTE_PM]
	v_cFormula.v_mFormula.clear();										//�������
	v_cFormula.My_Get_Formula(v_sFormula);								//�ֽ⹫ʽ���ʽ(����ڶ�����)

	v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);						//���ң���������
	if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())							//û�ҵ�������
		return;															//����ʧ��

	//���ã���Ч������ѡ��״̬[���ݼ��㹫ʽ]
	v_sField="";														//Ĭ��
	for (v_cFormula.v_pFormula=v_cFormula.v_mFormula.begin();
		v_cFormula.v_pFormula!=v_cFormula.v_mFormula.end();
		v_cFormula.v_pFormula++)										//������ʽԪ��
	{
		if (v_cFormula.v_pFormula->v_bFormula==1)						//����ֵ������
		{
			v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.find(v_cFormula.v_pFormula->v_sPM_Formula);	//���ң���ʽԪ���Ƿ���ԭʼ���ܣ�����
			if (v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//�ҵ�������
			{
				v_mLTE_PM.insert(pair<CString,double>(v_pPM_Name->first,0));	//��������

				//���ݻ�ȡ�ֶ�
				if (!v_sField.IsEmpty())								//�ֶβ�Ϊ�գ�����
					v_sField+=",";										//�ָ���
				v_sField+="["+v_pPM_Name->first+"]";					//��ȡ���ֶ���Ϣ
			}
		}
	}

	//��4�����ݿ����
	_variant_t		Var;
	_RecordsetPtr	v_pRecordset;										//��¼��
	CMyADOConn		v_cAdoConn;											//���ݿ�ʵ��
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zTime,"+v_sField+" from "+v_sTable+" where zCell in("+v_sCell+") and zTime>='"+v_sDate_Ben
				+"' and zTime<'"+v_sDate_End+"' order by zCell";		//��ѯ���ݿ⣺ϵͳ��־

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		//��5����ȡ��������
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_tDate=v_cAdoConn.GetField_DataTime("zTime");				//zTime
			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_iCell_ID=Var.intVal;									//��ȡ��Cell_ID
				//��ID==>Cell
				if (v_iCell_ID!=v_iCell_ID_New)							//�仯��v_iCell_ID������
				{
					v_pIter_Cell=v_mCell_ID.find(v_iCell_ID);			//���ң�С��ID���ڣ�����
					if (v_pIter_Cell==v_mCell_ID.end())					//������
					{
						v_pRecordset->MoveNext();						//��һ��������
						continue;										//����
					}
					v_iCell_ID_New=v_iCell_ID;							//
					v_sPM_Cell=v_pIter_Cell->second.v_sCell;			//С��
					v_sOnline=v_pIter_Cell->second.v_sOnline;			//״̬
					v_sRegion=v_pIter_Cell->second.v_sRegion;			//Ӫҵ��
				}
			}
			else
				v_sPM_Cell="��Ч";										//��ЧС��

			//��5����������������С��������
			v_pPM_Cell=v_mPM_Cell.find(v_sPM_Cell);						//���ң�С�� ������
			if (v_pPM_Cell==v_mPM_Cell.end())							//������
			{
				v_cPM_Time.v_sOnline=v_sOnline;							//״̬
				v_cPM_Time.v_sRegion=v_sRegion;							//����
				v_mPM_Cell.insert(pair<CString,PM_Time_Struct>(v_sPM_Cell,v_cPM_Time));	//��������
			}
			v_pPM_Cell=v_mPM_Cell.find(v_sPM_Cell);						//
			v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.find(v_tDate);//���ң����� ������
			if (v_pPM_Time==v_pPM_Cell->second.v_mTime_PM_Data.end())	//������
				v_pPM_Cell->second.v_mTime_PM_Data.insert(pair<COleDateTime,mString_Double>(v_tDate,v_mPM_Name));	//��������
			v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.find(v_tDate);//

			//��6����ȡ��������ֵ
			v_pIter_Value=v_mLTE_PM.begin();							//��ָ��
			while(v_pIter_Value!=v_mLTE_PM.end())						//��������[�������ܣ���ȡ��ֵ]
			{
				v_dValue=v_cAdoConn.GetField_Double(v_pIter_Value->first);	//��ȡ��������ֵ
				v_pIter_Value->second=v_dValue;							//��ֵ����������

				v_pIter_Value++;										//�ݼ�
			}

			//��7�����ܼ���
			//�Ӽ��㹫ʽ���ȡ��Ҫ��ȡ��Counterֵ��v_mFormula��valueΪ1��Ϊ����ֵ����Counter���ơ�
			v_sFormula="";												//��λ�����㹫ʽ
			for (v_cFormula.v_pFormula=v_cFormula.v_mFormula.begin();
				v_cFormula.v_pFormula!=v_cFormula.v_mFormula.end();
				v_cFormula.v_pFormula++)								//��ʽ����
			{
				if (v_cFormula.v_pFormula->v_bFormula==1)				//����
				{
					v_pIter_Value=v_mLTE_PM.find(v_cFormula.v_pFormula->v_sPM_Formula);	//���ң�Counter
					if (v_pIter_Value!=v_mLTE_PM.end())					//�ҵ�������
					{
						v_sTemp.Format("%f",v_pIter_Value->second);		//�ַ���
						v_sFormula+=v_sTemp;							//����ֵ
					}
					else
						v_sFormula+="0";								//����ֵ
				}
				else
					v_sFormula+=v_cFormula.v_pFormula->v_sPM_Formula;	//�����
			}
			//����
			float v_fValue=v_cFormula.simcalc(v_sFormula.GetBuffer(0));	//��ʽ����
		
			//��8�����浽��������
			v_pPM_Time->second.insert(pair<CString,double>(v_sName_Cal,v_fValue));	//��������

			v_pRecordset->MoveNext();									//��һ��
		}
		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	//��������
	v_mLTE_PM.clear();													//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ�������͡��Զ��幫ʽ
//------------------------------------------------------------------------------------------------------			
bool CLTE_PM_Data_Cell::My_Get_Formula(CString v_sName,CString* v_sFormula,CString* v_sType)
{
	CString		v_sSql;													//��ʱ����
	CString		v_sTemp;												//��ʱ����
	bool		v_bOK=false;											//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select * from CON_Pm_LTE_Case_Cal where zAccount='"+theApp.a_sUser_Name+"' and zName='"+v_sName+"'";	//��ѯ���ݿ�

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		if(!v_pRecordset->adoEOF)
		{
			*v_sType=v_cAdoConn.GetField_String("zType");				//zType
			*v_sFormula=v_cAdoConn.GetField_String("zPm_Formula");		//zPm_Formula
			v_bOK=true;													//��λ
		}
		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	return	v_bOK;														//����
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������ɾ��
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_PM_Data_Delete()
{
	map_PM_Cell::iterator	v_pPM_Cell;									//������
	map_PM_Time::iterator	v_pPM_Time;									//������
	for (v_pPM_Cell=v_mPM_Cell.begin();v_pPM_Cell!=v_mPM_Cell.end();v_pPM_Cell++)
	{
		for(v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.begin();v_pPM_Time!=v_pPM_Cell->second.v_mTime_PM_Data.end();v_pPM_Time++)
			v_pPM_Time->second.clear();									//�������
		v_pPM_Cell->second.v_mTime_PM_Data.clear();						//�������
	}
	v_mPM_Cell.clear();													//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ͳ�Ƶ�ʱ����������ɾ��
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_PM_Data_P_Delete()
{
	map_PM_Cell_P::iterator	v_pPM_Cell_P;								//������
	map_PM_Time_P::iterator	v_pPM_Time_P;								//������
	for (v_pPM_Cell_P=v_mPM_Cell_P.begin();v_pPM_Cell_P!=v_mPM_Cell_P.end();v_pPM_Cell_P++)
	{
		for(v_pPM_Time_P=v_pPM_Cell_P->second.v_mTime_PM_Data.begin();v_pPM_Time_P!=v_pPM_Cell_P->second.v_mTime_PM_Data.end();v_pPM_Time_P++)
			v_pPM_Time_P->second.v_mPM_Data.clear();					//�������
		v_pPM_Cell_P->second.v_mTime_PM_Data.clear();					//�������
	}
	v_mPM_Cell_P.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��Cell��Ϣ
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_LoadData_Cell()
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
		v_iList_Item=-1;												//Ĭ�ϣ���ѡ�е��б�Item

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

			v_sTemp.Format(" %05d",v_iID+1);
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
void CLTE_PM_Data_Cell::My_LoadData_Para()
{
	int				v_iID=0,v_iCell_ID;									//��ʱ����
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

			//����
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//���ң���������
			if (v_pLTE_PM_Type!=v_mLTE_PM_Type.end())					//�ҵ�������
			{
				v_cLTE_PM.v_sPM_Name=v_sPm;								//���ܣ�������
				v_cLTE_PM.v_sTable_Key="Cal";							//���ܣ�������
				v_cLTE_PM.v_bOK=false;									//���ܣ�δѡ��
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//���ӣ����ܲ���
			}

			v_pRecordset->MoveNext();									//
		}

		//��3����ȡ�����ܲ������Զ��塿
		v_sType="�Զ���";												//��������								
		v_cLTE_PM_Type.v_sTable_Key = "Def";							//������-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//��������

		//��ȡ�����ܲ������������ܡ�
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Case_Cal where zAccount='"+theApp.a_sUser_Name+"' order by zName ";	//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sPm = v_cAdoConn.GetField_String("zName");				//zName

			//����
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//���ң���������
			if (v_pLTE_PM_Type!=v_mLTE_PM_Type.end())					//�ҵ�������
			{
				v_cLTE_PM.v_sPM_Name=v_sPm;								//���ܣ�������
				v_cLTE_PM.v_sTable_Key="Def";							//���ܣ�������
				v_cLTE_PM.v_bOK=false;									//���ܣ�δѡ��
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//���ӣ����ܲ���
			}

			v_pRecordset->MoveNext();									//
		}

		//��4����ȡ���������ݱ����ơ����ڷ�����Ч�����ơ�
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_LTE_%') order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//������

			v_mTable_Name.insert(v_sTable);								//���Ӽ���

			v_pRecordset->MoveNext();									//ָ��++
		}	

		//��5����ȡ��С����Ϣ������С��ID����
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zCell_ID,zCell_Name,LTE_Cell.zOnline,LTE_Cell.zRegion \
				from LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB \
				where LTE_NODEB.zRNC like '%dalian%'";					//��ѯ���ݿ�
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sCell=v_cAdoConn.GetField_String("zCell");				//zCell
			v_iCell_ID=v_cAdoConn.GetField_Int("zCell_ID");				//zCell_ID
			v_sCell_ID.Format("%d",v_iCell_ID);							//�ַ���
			v_sCell_Name=v_cAdoConn.GetField_String("zCell_Name");		//zCell_Name
			v_sOnline=v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_sRegion=v_cAdoConn.GetField_String("zRegion");			//zRegion

			v_mCell.insert(pair<CString,CString>(v_sCell,v_sCell_ID));	//��������
			v_cCell_ID.v_sCell=v_sCell;									//С��
			v_cCell_ID.v_sCell_Name=v_sCell_Name;						//С��������
			v_cCell_ID.v_sOnline=v_sOnline;								//״̬
			v_cCell_ID.v_sRegion=v_sRegion;								//Ӫҵ��
			v_mCell_ID.insert(pair<int,Cell_ID_Struct>(v_iCell_ID,v_cCell_ID));	//��������

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
void CLTE_PM_Data_Cell::My_Load_Columns(set<CString> *v_pTable_Columns,CString v_sTable)
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
