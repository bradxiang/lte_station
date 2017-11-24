// PM_LTE_Case_PM.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "PM_LTE_Case_PM.h"
#include "PM_LTE_Case_PM_Input.h"


// CPM_LTE_Case_PM

IMPLEMENT_DYNCREATE(CPM_LTE_Case_PM, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CPM_LTE_Case_PM::CPM_LTE_Case_PM()
	: CFormView(CPM_LTE_Case_PM::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CPM_LTE_Case_PM::~CPM_LTE_Case_PM()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_LIST3, m_cList);
	DDX_Control(pDX, IDC_QUARY2, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel);
	DDX_Control(pDX, IDC_SUM_ALARM2, m_cCase);
	DDX_Control(pDX, IDC_LIST5, m_cList_Case);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPM_LTE_Case_PM, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL2, &CPM_LTE_Case_PM::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CPM_LTE_Case_PM::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CPM_LTE_Case_PM::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_QUARY2, &CPM_LTE_Case_PM::OnBnClickedQuary2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CPM_LTE_Case_PM::OnNMRClickList3)
	ON_COMMAND(ID_PM_LTE_CASE_PM_DEL, &CPM_LTE_Case_PM::OnPmLteCaseDel)
	ON_COMMAND(ID_PM_LTE_CASE_PM_M_DEL, &CPM_LTE_Case_PM::OnPmLteCaseWDel)
	ON_BN_CLICKED(IDC_EXCEL3, &CPM_LTE_Case_PM::OnBnClickedExcel3)
	ON_NOTIFY(NM_RCLICK, IDC_LIST5, &CPM_LTE_Case_PM::OnNMRClickList5)
END_MESSAGE_MAP()


// CPM_LTE_Case_PM ���

#ifdef _DEBUG
void CPM_LTE_Case_PM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPM_LTE_Case_PM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPM_LTE_Case_PM ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="LTE_���ܲ�ѯģ��_����";								//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б� [ģ��]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"�û��˺�",LVCFMT_LEFT,100);					//
	m_cList.InsertColumn(2,"ģ������",LVCFMT_LEFT,120);					//
	m_cList.InsertColumn(3,"��������",LVCFMT_CENTER,120);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	//��ʼ�����б� [С��]
	::SendMessage(m_cList_Case.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Case.InsertColumn(0,"���",LVCFMT_CENTER,60);				//
	m_cList_Case.InsertColumn(1,"�û��˺�",LVCFMT_LEFT,100);			//
	m_cList_Case.InsertColumn(2,"ģ������",LVCFMT_LEFT,120);			//
	m_cList_Case.InsertColumn(3,"����",LVCFMT_LEFT,100);				//
	m_cList_Case.InsertColumn(4,"����",LVCFMT_LEFT,280);				//
	m_cList_Case.InsertColumn(5,"Ԥ��",LVCFMT_CENTER,80);				//
	m_cList_Case.InsertColumn(6,"Ԥ������",LVCFMT_RIGHT,80);			//
	m_cList_Case.InsertColumn(7,"Ԥ������",LVCFMT_CENTER,80);			//
	m_cList_Case.InsertColumn(8,"��������",LVCFMT_CENTER,120);			//
	m_cList_Case.SetExtendedStyle(m_cList_Case.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

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
		&m_cList,				// Top pane
		&m_cList_Case,			// Bottom pane
		IDC_PM_LTE_Case_PM,		// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);
	m_SplitterPane_Bottom.m_bVertSplitter_Parent=true;					//��ֱ�ָ���Ϊ����

	PostMessage(WM_SIZE);												//���

	//��Ϣ��ʼ��
	v_iList_Item=-1;													//��Ŀѡ����Ч

	//��ѯ
	v_iWorking=1;														//�������񣺲�ѯ
	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CPM_LTE_Case_PM::PreTranslateMessage(MSG* pMsg)
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
void CPM_LTE_Case_PM::OnDestroy()
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
void CPM_LTE_Case_PM::OnSize(UINT nType, int cx, int cy)
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
//	�������ܣ�����
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Case);										//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����Ӧ
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
			v_sName_Account=m_cList.GetItemText(v_iList_Item,1);		//��ȡ��ѡ����ʺ�
			v_sName_Case=m_cList.GetItemText(v_iList_Item,2);			//��ȡ��ģ������
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));			//���ñ���ɫ����ɫ

			//��ѯ
			v_iWorking=2;												//�������񣺲�ѯ
			v_pIterator_Frame->second.v_iThread_Exit=1;					//�̣߳�����
			v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);		//������ѯ���ݿ��߳�
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б��һ���Ӧ��ģ�塿
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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

	aMenu.LoadMenu(IDR_PM_LTE_CASE_PM_DEL);								//װ��
	CMenu *pPopup =aMenu.GetSubMenu(0);									//���ɸ����˵�
	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);			//��ʾ���������λ��[point]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ɾ�����ܲ�ѯģ��
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnPmLteCaseDel()
{
	// TODO: �ڴ���������������
	int				v_iItem,vi;											//��ʱ����
	CString			v_sTemp,v_sSql,v_sName,v_sAccount;					//��ʱ����
	COleDateTime	v_tTime=COleDateTime::GetCurrentTime();
	CString			v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");

	vi = m_cList.GetSelectedCount();									//ѡ����Ŀ����
	v_sTemp.Format("%d",vi);											//�ַ���
	if(MessageBox("ɾ��ѡ�е� "+v_sTemp+" �� ���ܲ�ѯģ��_���ܣ� ɾ��ȷ�� ? ","����ɾ��ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)	//ɾ��ȷ�ϣ�����
	{
		//������ѡ�е�Item���в���
		while ((v_iItem = m_cList.GetNextItem(-1, LVNI_SELECTED)) != -1)//��Ŀѡ�У�����
		{
			v_sAccount=m_cList.GetItemText(v_iItem,1);					//��ã��ʺ�
			v_sName=m_cList.GetItemText(v_iItem,2);						//��ã�ģ������

			//ɾ����Cell
			v_sSql = "delete from CON_PM_LTE_Case_PM where zAccount='"
						+v_sAccount+"' and zName='"+v_sName+"'\r\n";	//SQLɾ��
			v_sSql += "delete from CON_PM_LTE_Case_PM_Data where zAccount='"
						+v_sAccount+"' and zName='"+v_sName+"'";		//SQLɾ��
			My_ExecuteSQL(v_sSql);										//д��

			//ɾ�����б���Ϣ
			m_cList.DeleteItem(v_iItem);								//ɾ����ѡ�е�v_iItem

			//���棺Log
			v_sSql = "Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"
						+v_sCurrentTime+"','���ܲ�ѯģ��_����_ɾ��','ģ�����ƣ�"
						+v_sName+" ������Ա��"+theApp.a_sUser_Name+"��')";
			My_ExecuteSQL(v_sSql);										//���ݿ�ִ��
		}
		OnBnClickedQuary2();											//��ȡģ��
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б��һ���Ӧ�����ܡ�
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnNMRClickList5(NMHDR *pNMHDR, LRESULT *pResult)
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

	aMenu.LoadMenu(IDR_PM_LTE_CASE_PM_M_DEL);							//װ��
	CMenu *pPopup =aMenu.GetSubMenu(0);									//���ɸ����˵�
	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);			//��ʾ���������λ��[point]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ɾ�����ܲ�ѯģ�塾���ܡ�
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnPmLteCaseWDel()
{
	// TODO: �ڴ���������������
	int				v_iItem,vi;											//��ʱ����
	CString			v_sTemp,v_sSql,v_sName,v_sType,v_sPM_Name,v_sAccount;	//��ʱ����
	COleDateTime	v_tTime=COleDateTime::GetCurrentTime();
	CString			v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");

	vi = m_cList_Case.GetSelectedCount();								//ѡ����Ŀ����
	v_sTemp.Format("%d",vi);											//�ַ���
	if(MessageBox("ɾ��ѡ�е� "+v_sTemp+" �� ���ܲ�ѯģ��_���ܣ� ɾ��ȷ�� ? ","����ɾ��ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)	//ɾ��ȷ�ϣ�����
	{
		//������ѡ�е�Item���в���
		while ((v_iItem = m_cList_Case.GetNextItem(-1, LVNI_SELECTED)) != -1)//��Ŀѡ�У�����
		{
			v_sAccount=m_cList_Case.GetItemText(v_iItem,1);				//��ã��ʺ�
			v_sName=m_cList_Case.GetItemText(v_iItem,2);				//��ã�ģ������
			v_sType=m_cList_Case.GetItemText(v_iItem,3);				//��ã�����
			v_sPM_Name=m_cList_Case.GetItemText(v_iItem,4);				//��ã�����

			//ɾ����Cell
			v_sSql = "delete from CON_PM_LTE_Case_PM_Data where zAccount='"+v_sAccount
						+"' and zName='"+v_sName+"' and zType='"+v_sType+"' and zPM_Name='"+v_sPM_Name+"'";		//SQLɾ��
			My_ExecuteSQL(v_sSql);										//д��

			//ɾ�����б���Ϣ
			m_cList_Case.DeleteItem(v_iItem);							//ɾ����ѡ�е�v_iItem

			//���棺Log
			v_sSql = "Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"
				+v_sCurrentTime+"','���ܲ�ѯģ��_����_ɾ��','ģ�����ƣ�"
				+v_sName+"���������ƣ�"+v_sPM_Name+" ������Ա��"+theApp.a_sUser_Name+"��')";
			My_ExecuteSQL(v_sSql);										//���ݿ�ִ��
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnBnClickedQuary2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnBnClickedExcel3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPM_LTE_Case_PM_Input	v_cDlg;										//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
	OnBnClickedQuary2();												//��ȡģ��
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CPM_LTE_Case_PM::My_Thread_Inoput(LPVOID lparam)
{
	CPM_LTE_Case_PM *  lp_this = NULL ;
	lp_this = (CPM_LTE_Case_PM *)lparam;

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
void CPM_LTE_Case_PM::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Per();												//��ѯ
		break;
	case 2:
		My_LoadData_Case();												//��ѯ
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ����Ϣ��ģ�塿
//------------------------------------------------------------------------------------------------------			
void CPM_LTE_Case_PM::My_LoadData_Per()
{
	int				v_iItem,v_iID=0;									//��ʱ����
	CString			v_sTemp,v_sSql;										//��ʱ����
	CString			v_sUser,v_sName,v_sDate;							//��ʱ����
	COleDateTime	v_dDate;											//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList.DeleteAllItems();										//���Items
		m_cList_Case.DeleteAllItems();									//���Items
		v_iList_Item=-1;												//��Ŀѡ����Ч

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_PM_LTE_Case_PM where zAccount='����' or zAccount='"
					+theApp.a_sUser_Account+"' order by zAccount,zName";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sUser = v_cAdoConn.GetField_String("zAccount");			//zAccount
			v_sName = v_cAdoConn.GetField_String("zName");				//zName
			v_dDate = v_cAdoConn.GetField_DataTime("zDate");			//zDate
			v_sDate = v_dDate.Format(_T("%Y-%m-%d"));					//�ַ���

			//��ʾ
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,v_sUser);						//v_sUser
			m_cList.SetItemText(v_iItem,2,v_sName);						//v_sName
			m_cList.SetItemText(v_iItem,3,v_sDate);						//v_sDate

			v_pRecordset->MoveNext();									//
			v_iID++;													//�������澯����

			if (v_iID%100==0)											//��ʾ��100������
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum.SetWindowText(v_sTemp);							//��ʾ������
			}
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cSum.SetWindowText(v_sTemp);									//��ʾ������

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
//	�������ܣ�װ����Ϣ�����ܡ�
//------------------------------------------------------------------------------------------------------			
void CPM_LTE_Case_PM::My_LoadData_Case()
{
	int				v_iItem,v_iID=0,v_iTemp;							//��ʱ����
	double			v_dTemp;											//��ʱ����
	CString			v_sTemp,v_sSql;										//��ʱ����
	CString			v_sUser,v_sName,v_sType,v_sPM_Name,v_sDate;			//��ʱ����
	COleDateTime	v_dDate;											//��ʱ����
	CString			v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;			//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList_Case.DeleteAllItems();									//���Items

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_PM_LTE_Case_PM_Data where zAccount='"+v_sName_Account+"' and zName='"+v_sName_Case+"' order by zNum";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sUser = v_cAdoConn.GetField_String("zAccount");			//zAccount
			v_sName = v_cAdoConn.GetField_String("zName");				//zName
			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sPM_Name = v_cAdoConn.GetField_String("zPM_Name");		//zPM_Name
			v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Flag");				//zAlarm_Flag
			v_sAlarm_Flag=(v_iTemp==1?"Ԥ��":"��Ԥ��");						
			v_dTemp=v_cAdoConn.GetField_Double("zAlarm_Value");			//zAlarm_Value
			v_sAlarm_Value.Format("%6.2f",v_dTemp);
			v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Type");				//zAlarm_Type
			v_sAlarm_Type=(v_iTemp==1?"��":"��");						
			v_dDate = v_cAdoConn.GetField_DataTime("zDate");			//zDate
			v_sDate = v_dDate.Format(_T("%Y-%m-%d"));					//�ַ���

			//��ʾ
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList_Case.InsertItem(0xffff,v_sTemp);			//�����б�
			m_cList_Case.SetItemText(v_iItem,1,v_sUser);				//v_sUser
			m_cList_Case.SetItemText(v_iItem,2,v_sName);				//v_sName
			m_cList_Case.SetItemText(v_iItem,3,v_sType);				//v_sType
			m_cList_Case.SetItemText(v_iItem,4,v_sPM_Name);				//v_sPM_Name
			m_cList_Case.SetItemText(v_iItem,5,v_sAlarm_Flag);			//Ԥ����־
			m_cList_Case.SetItemText(v_iItem,6,v_sAlarm_Value);			//Ԥ��ֵ
			m_cList_Case.SetItemText(v_iItem,7,v_sAlarm_Type);			//Ԥ������
			m_cList_Case.SetItemText(v_iItem,8,v_sDate);				//v_sDate

			v_pRecordset->MoveNext();									//
			v_iID++;													//�������澯����

			if (v_iID%100==0)											//��ʾ��100������
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cCase.SetWindowText(v_sTemp);							//��ʾ������
			}
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cCase.SetWindowText(v_sTemp);									//��ʾ������

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
bool CPM_LTE_Case_PM::My_ExecuteSQL(CString v_sSql)
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
