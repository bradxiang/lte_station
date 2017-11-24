// LTE_PM_Data_Cal.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Data_Cal.h"
#include "LTE_PM_DATA_CASE.h"											//ģ�塾С�������ܡ�
#include "LTE_PM_Data_Cal_Select.h"

// CLTE_PM_Data_Cal

IMPLEMENT_DYNCREATE(CLTE_PM_Data_Cal, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cal::CLTE_PM_Data_Cal()
	: CFormView(CLTE_PM_Data_Cal::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cal::~CLTE_PM_Data_Cal()
{
	v_mLTE_PM_Type.clear();												//�������
	v_mTable_Name.clear();												//�������
	v_mCell.clear();													//�������
	v_mCell_ID.clear();													//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_LOG_SUM3, m_cData_Sum);
	DDX_Control(pDX, IDC_CHECK2, m_cCheck);
	DDX_Control(pDX, IDC_BUTTON5, m_cCell_Case);
	DDX_Control(pDX, IDC_EXCEL2, m_cPM_Cal);
	DDX_Control(pDX, IDC_KEY3, m_cCal);
	DDX_Control(pDX, IDC_LOG_SUM4, m_cCase_Type);
	DDX_Control(pDX, IDC_BUTTON2, m_cCal_Select);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Data_Cal, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL, &CLTE_PM_Data_Cal::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CLTE_PM_Data_Cal::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Data_Cal::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Data_Cal::OnNMClickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CLTE_PM_Data_Cal::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_BUTTON5, &CLTE_PM_Data_Cal::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK2, &CLTE_PM_Data_Cal::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_EXCEL2, &CLTE_PM_Data_Cal::OnBnClickedExcel2)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST3, &CLTE_PM_Data_Cal::OnLvnGetdispinfoList3)
	ON_BN_CLICKED(IDC_BUTTON2, &CLTE_PM_Data_Cal::OnBnClickedButton2)
END_MESSAGE_MAP()


// CLTE_PM_Data_Cal ���

#ifdef _DEBUG
void CLTE_PM_Data_Cal::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLTE_PM_Data_Cal::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLTE_PM_Data_Cal ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	CString v_sTemp;													//��ʱ����
	//��������
	v_sFrame_Name="LTE �������ݼ���_С��";								//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б���С����
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);
	m_cList.InsertColumn(1,"С��",LVCFMT_LEFT,200);  
	m_cList.InsertColumn(2,"С��ID",LVCFMT_LEFT,0);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES );

	//��ʼ�����б������ܡ�
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"���",LVCFMT_CENTER,60);
	m_cList_PM.InsertColumn(1,"С��",LVCFMT_LEFT,140);					//��2�У�С��
	m_cList_PM.InsertColumn(2,"����",LVCFMT_CENTER,140);				//��3�У�����
	m_cList_PM.InsertColumn(3,"������",LVCFMT_RIGHT,80);				//��4�У���������
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
		IDC_LTE_PM_Data_Cal_Main,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);

	//ˢ�½���
	PostMessage(WM_SIZE);

	//������ʼ��
	m_cType.AddString("15m����");
	m_cType.AddString("1H����");
	m_cType.SetCurSel(1);												//Ĭ�ϣ�1H����

	//��ʼ��������ѡ��
	for (int vi=1;vi<=c_iSample_Day;vi++)								//�����
	{
		v_sTemp.Format("%d",vi);
		m_cLog_Day.AddString(v_sTemp);									//��ѡ������ѡť
	}
	m_cLog_Day.SetCurSel(0);											//Ĭ�ϲ�ѯ������1�죻

	m_cLog_Day.SetCurSel(4);											//Ĭ�ϣ���ѯ����
	v_iList_Item=-1;													//Ĭ�ϣ���ѡ�е��б�Item
	v_sCase_Name="������";											//Ĭ�ϣ���������

	//��ȡ�����ܲ���
	v_iWorking=3;														//�������񣺻�ȡ��ʼ����
	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ӵ�����
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnDestroy()
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
void CLTE_PM_Data_Cal::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//�ִ��ߴ����
		CRect v_cRect;
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=70;
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Data_Cal::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ�����Excel[��������]
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_PM);											//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����(�һ��б�������)��С����
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cal::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����(�һ��б�������)�����ܣ������б�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cal::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_cList_PM.My_Sort_Virtual(pNMHDR);									//�б����������б�

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������б���Ϣ��Ӧ����
//	��	  ���������б���Ҫ��Ԫ�ز���(�С���)�������������в�ѯ���ݣ���ֵ���б��С�
//------------------------------------------------------------------------------------------------------	
void CLTE_PM_Data_Cal::OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ�������б���ʾ���ݻ�ȡ
	m_cList_PM.Get_List_Data(pDispInfo);								//�����б���ʾ���ݻ�ȡ

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ȫѡ��С����
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//����
		m_cList.SetCheck(vi,m_cCheck.GetCheck());						//����״̬
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������ѯ��ť��С����ѯ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnBnClickedButton1()
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
void CLTE_PM_Data_Cal::OnBnClickedButton5()
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
//	�������ܣ����С����Ϣ�����ܲ�ѯ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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
//	�������ܣ�����ѡ���Զ���������ܡ�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cal::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp,v_sFormula,v_sType;								//��ʱ����

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	CLTE_PM_Data_Cal_Select v_Dlg;	

	//��1��������ѡ��Ի���
	v_Dlg.v_mLTE_PM_Type=&v_mLTE_PM_Type;								//����ָ��
	if (v_Dlg.DoModal()==IDOK)											//ȷ�Ϸ��أ�����
	{
		//��2���Ի���ȷ�Ϸ��أ���ȡѡ�е����ܲ���
		v_sFormula=v_Dlg.v_sFormula;									//��ֵ����������
		v_sCase_Name = v_Dlg.v_sName;									//��ֵ��ģ������
		v_sType=v_Dlg.v_sType;											//��ֵ����������
		m_cCal.SetWindowText(v_sFormula);								//��ֵ�����㹫ʽ
		m_cCase_Type.SetWindowText(v_sType);							//��ֵ����������

		//��3����ʽ���(��ʽ�е������Ƿ���ѡ���Ե����ܼ���)
		v_cFormula.v_mFormula.clear();									//�������
		v_cFormula.My_Get_Formula(v_sFormula);							//�ֽ⹫ʽ���ʽ(����ڶ�����)

		v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);					//���ң���������
		//�������ѡ��״̬
		for(v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();
			v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();
			v_pPM_Name++)
			v_pPM_Name->second.v_bOK=false;								//���ܣ���Ч
		//��������ѡ��״̬[���ݼ��㹫ʽ]
		for (v_cFormula.v_pFormula=v_cFormula.v_mFormula.begin();
			v_cFormula.v_pFormula!=v_cFormula.v_mFormula.end();
			v_cFormula.v_pFormula++)									//������ʽԪ��
		{
			if (v_cFormula.v_pFormula->v_bFormula==1)					//����ֵ������
			{
				v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.find(v_cFormula.v_pFormula->v_sPM_Formula);	//���ң���ʽԪ���Ƿ���ԭʼ���ܣ�����
				if (v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())	//�ҵ�������
					v_pPM_Name->second.v_bOK=true;						//���ܣ���Ч
			}
		}

		//��4��ɾ���б���Ϣ
		m_cList_PM.DeleteAllItems();									//ɾ��������Ŀ
		while(m_cList_PM.DeleteColumn(0));								//ɾ�������У�ObjType��Ϣ�б�

		//��5����ObjType��Ϣ�б������Ϣ
		m_cList_PM.InsertColumn(0,"���",LVCFMT_CENTER,50);				//��1�У����
		m_cList_PM.InsertColumn(1,"С��",LVCFMT_LEFT,140);				//��2�У�С��
		m_cList_PM.InsertColumn(2,"����",LVCFMT_CENTER,140);			//��3�У�����
		m_cList_PM.InsertColumn(3,v_sCase_Name,LVCFMT_RIGHT,80);		//��4�У�������������

		//��6��������������
		int v_iNum=4;
		m_cCase_Type.GetWindowText(v_sType);							//��ã���������
		v_sType.Trim();													//ȥ���ո�
		v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);					//���ң���������
		v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();			//��ָ��
		while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//��������
		{
			if (v_pPM_Name->second.v_bOK)								//����ѡ�У�����
			{
				m_cList_PM.InsertColumn(v_iNum,v_pPM_Name->second.v_sPM_Name,LVCFMT_RIGHT,60);	//��n�У�����
				m_cList_PM.SetNumber(v_iNum);							//�����ԣ�����
				v_iNum++;												//������
			}
			v_pPM_Name++;												//�ݼ�
		}

		//��7����ʾ����������
		v_sTemp.Format("%d",v_pLTE_PM_Type->second.v_mLTE_PM.size());	//��������
		m_cPM_Sum.SetWindowText(v_sTemp);								//��ʾ����������
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܼ���
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=2;														//�����������ܲ�ѯ��������
	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CLTE_PM_Data_Cal::My_Thread_Inoput(LPVOID lparam)
{
	CLTE_PM_Data_Cal *  lp_this = NULL ;
	lp_this = (CLTE_PM_Data_Cal *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cLog_Date.EnableWindow(false);							//���ڣ���Ч
	lp_this->m_cLog_Day.EnableWindow(false);							//��������Ч
	lp_this->m_cType.EnableWindow(false);								//��ѯ���ͣ���Ч
	lp_this->m_cExcel.EnableWindow(false);								//��������Ч
	lp_this->m_cCell_Case.EnableWindow(false);							//С��ģ�壺��Ч
	lp_this->m_cPM_Cal.EnableWindow(false);								//���ܼ��㣺��Ч
	lp_this->m_cCal_Select.EnableWindow(false);							//���ܼ���ѡ����Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(true);								//��������Ч
	lp_this->m_cLog_Date.EnableWindow(true);							//���ڣ���Ч
	lp_this->m_cLog_Day.EnableWindow(true);								//��������Ч
	lp_this->m_cType.EnableWindow(true);								//��ѯ���ͣ���Ч
	lp_this->m_cCell_Case.EnableWindow(true);							//С��ģ�壺��Ч
	lp_this->m_cPM_Cal.EnableWindow(true);								//���ܼ��㣺��Ч
	lp_this->m_cCal_Select.EnableWindow(true);							//���ܼ���ѡ����Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Cell();												//װ��Cell��Ϣ
		break;
	case 2:
		My_LoadData_PM();												//װ��������Ϣ
		break;
	case 3:
		My_LoadData_Para();												//��ȡ�����ܲ���
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��������Ϣ
//	��    �ݣ��������ƣ�15m/1H���ȣ��������ͣ������У�С�������ڡ����У��������͡�
//	�� �� ��15mins��ÿ��һ����  Hour��5��һ����һ��������
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cal::My_LoadData_PM()
{
	CString			v_sTemp;											//��ʱ����
	int				v_iDay_Sum;											//��ʱ����
	COleDateTime	v_dDate_Start,v_dDate_End,v_dDate;					//��ѯ��ʼ���ڡ���������
	CString			v_sCell="",v_sTime_Start,v_sTime_End,v_sTable_Num;	//��ʱ����

	//��1����ʾ���������б�������Ϣ
	m_cData_Sum.SetWindowText("");										//�����������������
	v_iPM_Sum=0;														//��λ����ȡ����������

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

	//��3����λ�������б�
	m_cList_PM.Put_Line_Begin();										//�б�������ݿ�ʼ

	//��4����ѯ����
	m_cLog_Date.GetTime(v_dDate_Start);									//��ã���ѯ��ʼ����
	v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//����ʱ�䣺00:00:00
	v_iDay_Sum=m_cLog_Day.GetCurSel()+1;								//��ã���ѯ����
	v_dDate_End =v_dDate_Start + COleDateTimeSpan(v_iDay_Sum,0,0,0);	//��ã���ѯ��������

	//��5�����ܲ�ѯ��15mins��ÿ��һ����  Hour��5��һ����һ��������
	if (m_cType.GetCurSel()==0)											//15m����
	{
		while(v_dDate_Start<v_dDate_End)								//��ʼ����<��������
		{
			v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//��ʼʱ��
			v_sTime_End=v_dDate_Start.Format("%Y-%m-%d")+" 23:59:59";	//����ʱ��
			v_sTemp=v_dDate_Start.Format("%Y%m%d");						//������
			v_sTemp=My_Get_Table()+"_"+v_sTemp;							//�ļ�����
			My_LoadData_PM_Data(v_sTemp,v_sCell,v_sTime_Start,v_sTime_End);		//װ����������
			v_dDate_Start+=COleDateTimeSpan(1,0,0,0);					//����һ��
		}
	}
	else
	{
		v_dDate=v_dDate_Start;											//��ֵ����ǰ����
		v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());			//��ȡ�����
		while(v_dDate<v_dDate_End)										//ʱ�䷶Χ�ڣ�����
		{
			v_dDate+=COleDateTimeSpan(1,0,0,0);							//����һ��
			if (My_Get_Table_Num(v_dDate.GetDay())!=v_sTable_Num || v_dDate>=v_dDate_End)
			{
				v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";		//��ʼʱ��
				v_sTime_End=v_dDate.Format("%Y-%m-%d")+" 00:00:00";		//����ʱ��
				v_sTemp=v_dDate_Start.Format("%Y%m");					//����
				v_sTemp=My_Get_Table()+"_"+v_sTemp+"_"+v_sTable_Num;	//�ļ�����
				My_LoadData_PM_Data(v_sTemp,v_sCell,v_sTime_Start,v_sTime_End);		//װ����������
				v_dDate_Start=v_dDate;									//�������ʼʱ��
				v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());	//��ȡ�����
			}
		}
	}

	//��6�����ã������б���Ч
	m_cList_PM.Put_Line_End();											//�б�������ݽ���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ������
//	��    �أ����ݱ����ơ�6����������֮һ��
//	��    �ݣ���������
//------------------------------------------------------------------------------------------------------			
CString CLTE_PM_Data_Cal::My_Get_Table()
{
	CString		v_sTemp;												//��ʱ����
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������

	//��ȡ����������
	m_cCase_Type.GetWindowText(v_sTemp);								//��ã���������
	v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sTemp);						//���ң���������

	v_sTemp = "Pm_LTE_"+v_pLTE_PM_Type->second.v_sTable_Key;			//������
	return v_sTemp;														//����
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ�����
//	��	  ����1��1-5��	2��6-10��	3��11-15��	4��16-20��	5��21-25��	6��26--
//------------------------------------------------------------------------------------------------------			
CString CLTE_PM_Data_Cal::My_Get_Table_Num(int v_iDay)
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
//	��	  ����v_sTable�������ƣ�	v_sCell��С����	v_sDate_Ben����ʼ���ڣ�		v_sDate_End����������
//	��    �ݣ��������ƣ�15m/1H���ȣ��������ͣ������У�С�������ڡ����У��������͡�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cal::My_LoadData_PM_Data(CString v_sTable,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End)
{
	CString			v_sSql;												//��ʱ����
	CString			v_sTemp,v_sTime,v_sCell_ID,v_sFormula;				//��ʱ����
	int				v_iID=0,v_iNum,v_iCell_ID;							//��ʱ����
	COleDateTime	v_tDate;											//����
	double			v_dValue;											//������ֵ

	mString_Double	v_mPM_Cell;											//����

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name;									//������
	map_Cell_ID::iterator	v_pIter_Cell;								//������
	mString_Double::iterator	v_pIter_Value;							//������

	//�����б�
	mInt_String		v_mLine_Data;										//�����б�������

	_variant_t		Var;
	_RecordsetPtr	v_pRecordset;										//��¼��
	CMyADOConn		v_cAdoConn;											//���ݿ�ʵ��
	try
	{
		//��1����ȡ������ָ��
		m_cCase_Type.GetWindowText(v_sTemp);							//��ã���������
		v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sTemp);					//���ң���������
		if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())						//û�ҵ�
			return;														//��Ч����

		//��2����飺�������Ƿ���Ч�����ݣ������Ƽ���v_mTable_Name��
		set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//���ң���
		if (v_pTable==v_mTable_Name.end())								//û�ҵ�
			return;														//��Ч����

		//��3���������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select * from "+v_sTable+" where zCell in("+v_sCell+") and zTime>='"+v_sDate_Ben
				+"' and zTime<'"+v_sDate_End+"' order by zCell,zTime";	//��ѯ���ݿ⣺ϵͳ��־

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		//��4����ȡ��������
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			//���ݸ�λ
			v_mLine_Data.clear();										//�������

			v_tDate=v_cAdoConn.GetField_DataTime("zTime");				//zTime
			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_iCell_ID=Var.intVal;									//��ȡ��Cell_ID
				//��ID==>Cell
				v_pIter_Cell=v_mCell_ID.find(v_iCell_ID);				//���ң�С��ID���ڣ�����
				if (v_pIter_Cell==v_mCell_ID.end())						//������
				{
					v_pRecordset->MoveNext();							//��һ��������
					continue;											//����
				}
				v_sCell_ID=v_pIter_Cell->second.v_sCell;				//С��
			}
			else
				v_sCell_ID="��Ч";										//��ЧС��


			//��5����ʾ��С��������
			v_sTemp.Format(" %06d",v_iPM_Sum+1);
			v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));			//���
			v_mLine_Data.insert(pair<int,CString>(1,v_sCell_ID));		//С��
			v_sTime=v_tDate.Format("%Y-%m-%d_%H:%M:%S");				//��ֵ������
			v_mLine_Data.insert(pair<int,CString>(2,v_sTime));			//����


			//��6����ʾ������
			v_mPM_Cell.clear();											//�������
			v_iNum=4;
			v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();		//��ָ��
			while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())	//��������
			{
				if (v_pPM_Name->second.v_bOK)							//����ѡ��???
				{
					v_dValue=v_cAdoConn.GetField_Double(v_pPM_Name->first);		//��ȡ������
					v_sTemp.Format("%10.4f",v_dValue);							//�ַ���
					v_mLine_Data.insert(pair<int,CString>(v_iNum,v_sTemp));		//ԭʼ����[׼���������б�]

					v_mPM_Cell.insert(pair<CString,double>(v_pPM_Name->first,v_dValue));	//�������ӣ����㹫ʽ
					v_iNum++;											//������
				}

				v_pPM_Name++;											//�ݼ�
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
					v_pIter_Value=v_mPM_Cell.find(v_cFormula.v_pFormula->v_sPM_Formula);	//���ң�Counter
					if (v_pIter_Value!=v_mPM_Cell.end())				//�ҵ�������
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
			//��ʾ
			v_sTemp.Format("%8.2f",v_fValue);							//�ַ���
			v_mLine_Data.insert(pair<int,CString>(3,v_sTemp));			//��������

			//�������ݷ����б�
			m_cList_PM.Put_Line_Data(v_mLine_Data);						//�б�������������

			v_pRecordset->MoveNext();									//��һ��

			v_iPM_Sum++;												//++
			if ((v_iPM_Sum)%100==0)
			{
				v_sTemp.Format("%d",v_iPM_Sum);							//
				m_cData_Sum.SetWindowText(v_sTemp);						//��ʾ��������������
			}
		}

		v_sTemp.Format("%d",v_iPM_Sum);									//
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
	v_mPM_Cell.clear();													//�������

	//�����б�
	v_mLine_Data.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��Cell��Ϣ
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cal::My_LoadData_Cell()
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
void CLTE_PM_Data_Cal::My_LoadData_Para()
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

		//��1����ȡ�����ܲ���
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
			v_cLTE_PM.v_sPM_Name=v_sPm_Name;							//���ܣ�������
			v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//���ӣ����ܲ���

			v_pRecordset->MoveNext();									//
		}

		//��2����ȡ���������ݱ����ơ����ڷ�����Ч�����ơ�
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and name like 'Pm_LTE_%' order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//������

			v_mTable_Name.insert(v_sTable);								//���Ӽ���

			v_pRecordset->MoveNext();									//ָ��++
		}	

		//��3����ȡ��С����Ϣ������С��ID����
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
//	END
//------------------------------------------------------------------------------------------------------
