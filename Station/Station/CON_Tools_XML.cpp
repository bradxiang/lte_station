// CON_Tools_XML.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "Station.h"
#include "CON_Tools_XML.h"

#define _CRTDBG_MAP_ALLOC 
#include<stdlib.h> 
#include<crtdbg.h> 

// CCON_Tools_XML

IMPLEMENT_DYNCREATE(CCON_Tools_XML, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CCON_Tools_XML::CCON_Tools_XML()
	: CFormView(CCON_Tools_XML::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CCON_Tools_XML::~CCON_Tools_XML()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST6, m_cList_ObjType);
	DDX_Control(pDX, IDC_LIST7, m_cList_Child);
	DDX_Control(pDX, IDC_EDIT1, m_cFile_Name);
	DDX_Control(pDX, IDC_Open_File, m_cOpen_File);
	DDX_Control(pDX, IDC_COMBO3, m_cType);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_BUTTON4, m_cFile);
	DDX_Control(pDX, IDC_LIST8, m_cList_Object);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_Tools_XML, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON4, &CCON_Tools_XML::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CCON_Tools_XML::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCON_Tools_XML::OnBnClickedButton2)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST6, &CCON_Tools_XML::OnLvnColumnclickList6)
	ON_NOTIFY(NM_CLICK, IDC_LIST6, &CCON_Tools_XML::OnNMClickList6)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST4, &CCON_Tools_XML::OnLvnColumnclickList4)
	ON_NOTIFY(NM_CLICK, IDC_LIST7, &CCON_Tools_XML::OnNMClickList7)
	ON_BN_CLICKED(IDC_BUTTON5, &CCON_Tools_XML::OnBnClickedButton5)
END_MESSAGE_MAP()


// CCON_Tools_XML ���

#ifdef _DEBUG
void CCON_Tools_XML::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCON_Tools_XML::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCON_Tools_XML ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="XML�ļ�����";										//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��

	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б�ObjType
	::SendMessage(m_cList_ObjType.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_ObjType.InsertColumn(0,"���",LVCFMT_CENTER,40);
	m_cList_ObjType.InsertColumn(1,"ObjType",LVCFMT_LEFT,280);  
	m_cList_ObjType.SetExtendedStyle(m_cList_ObjType.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	//��ʼ�����б�Child
	::SendMessage(m_cList_Child.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Child.InsertColumn(0,"���",LVCFMT_CENTER,40);
	m_cList_Child.InsertColumn(1,"Counter",LVCFMT_LEFT,280);  
	m_cList_Child.SetExtendedStyle(m_cList_Child.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	//��ʼ�����б�Object
	::SendMessage(m_cList_Object.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Object.InsertColumn(0,"���",LVCFMT_CENTER,40);
	m_cList_Object.InsertColumn(1,"Counter",LVCFMT_LEFT,280);  
	m_cList_Object.SetExtendedStyle(m_cList_Object.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//�ִ�
	CRect	v_cRect;													//����
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//��ã�ָ��
	pWnd->GetWindowRect(&v_cRect);										//��ã��ߴ�
	ScreenToClient(&v_cRect);											//����任
	pWnd->DestroyWindow();												//���٣�ָ��

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Right.Create(											//ˮƽ��
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList_ObjType,		// Top pane
		&m_cList_Child,			// Bottom pane
		IDC_Tools_XML_2,		// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);
	m_SplitterPane_Right.m_bVertSplitter_Parent=true;					//��ֱ�ָ���Ϊ����
	m_SplitterPane_Main.Create(											//��ֱ��
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_SplitterPane_Right,	// Top pane
		&m_cList_Object,		// Bottom pane
		IDC_Tools_XML_1,		// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);

	PostMessage(WM_SIZE);												//���

	//��������
	m_cOpen_File.SetCheck(true);										//�����򿪸�ѡť����Ч
	m_cType.AddString("����");											//�ļ����ͣ�����
	m_cType.AddString("����");											//�ļ����ͣ�����
	m_cType.AddString("����_ֱ��");										//�ļ����ͣ����ܡ�ֱ�Ӷ�ȡ��
	m_cType.SetCurSel(0);												//Ĭ�����ͣ�����

	v_bRead=false;														//�������ͣ�0�����á����ܣ�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CCON_Tools_XML::PreTranslateMessage(MSG* pMsg)
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
void CCON_Tools_XML::OnDestroy()
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
void CCON_Tools_XML::OnSize(UINT nType, int cx, int cy)
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
//	�������ܣ���ȡXML�ļ���Ϣ
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int			v_iVi;													//��ʱ����
	CString		v_sTemp;												//��ʱ����
	map_ObjType::iterator		v_pObjType;								//��������v_mObjType
	map_ObjType_Child::iterator v_pObjType_Child;						//��������v_mObjType_Child

	CString		v_sFilename;											//�ļ�����
	CFileDialog *filedialog;											//ѡ���ļ��Ի���ָ��

	filedialog = new CFileDialog(true,(LPCTSTR)"bak");					//ʵ����
	if(filedialog->DoModal()==IDOK)										//��ѡ���ļ��Ի���
	{
		v_sFilename = filedialog->GetPathName();						//��ã�ѡ����ļ�����
		m_cFile_Name.SetWindowText(v_sFilename);						//���֣�ѡ�е��ļ�����
		//��ȡXML�ļ����ݣ�[ObjType/Object]
		switch(m_cType.GetCurSel())
		{
		case 0:															//�����ļ�
			if (!v_cFile_XML.Read_XML(v_sFilename.GetBuffer(0)))		//��ȡ��XML�ļ���Ϣ[ObjType/Object]
				MessageBox("����Ч��XML�ļ���������ѡ�� ������");		//������ʾ	
			break;
		case 1:															//�����ļ�
			if (!v_cFile_XML.Read_XML_PM(v_sFilename.GetBuffer(0)))		//��ȡ��XML�ļ���Ϣ[ObjType/Object]
				MessageBox("����Ч��XML�ļ���������ѡ�� ������");		//������ʾ	
			break;
		case 2:															//�����ļ���ֱ�Ӷ�ȡ��
			if (!v_cFile_XML.Read_XML_PM_Dir(v_sFilename))				//��ȡ��XML�ļ���Ϣ
				MessageBox("����Ч��XML�ļ���������ѡ�� ������");		//������ʾ	
			break;
		}

		//�����ʾ�б���Ϣ
		m_cList_ObjType.DeleteAllItems();								//ɾ�������б���Ŀ
		m_cList_Child.DeleteAllItems();									//ɾ�������б���Ŀ

		switch(m_cType.GetCurSel())
		{
		case 0:															//�����ļ�
		case 1:															//�����ļ�
			//��ʾ��ObjType
			for(v_pObjType=v_cFile_XML.v_mObjType.begin();v_pObjType!=v_cFile_XML.v_mObjType.end();v_pObjType++)
			{
				v_iVi = m_cList_ObjType.InsertItem(0xffffff,_T("0"));	//���һ��
				v_sTemp.Format("%04d",v_iVi+1);							//��ã��ַ���
				m_cList_ObjType.SetItemText(v_iVi,0,v_sTemp);			//��0�У����
				m_cList_ObjType.SetItemText(v_iVi,1,(_bstr_t)v_pObjType->first);	//ObjType
			}
			v_bRead=false;												//�������ͣ�0�����á����ܣ�
			break;
		case 2:															//�����ļ���ֱ�Ӷ�ȡ��
			//��ʾ��ObjType_Child
			for(v_pObjType_Child=v_cFile_XML.v_mObjType_Child.begin();v_pObjType_Child!=v_cFile_XML.v_mObjType_Child.end();v_pObjType_Child++)
			{
				//��ʾ��ObjType
				v_iVi = m_cList_ObjType.InsertItem(0xffffff,_T("0"));	//���һ��
				v_sTemp.Format("%04d",v_iVi+1);							//��ã��ַ���
				m_cList_ObjType.SetItemText(v_iVi,0,v_sTemp);			//��0�У����
				m_cList_ObjType.SetItemText(v_iVi,1,(_bstr_t)v_pObjType_Child->first);	//ObjType
			}
			v_bRead=true;												//�������ͣ�1������ֱ����
			break;
		}

		//��λ
		v_iItem_ObjType=-1;												//ObjType�б���Ŀѡ����Ч[-1]
		v_iItem_ObjType_Child=-1;										//ObjType_Child�б���Ŀѡ����Ч[-1]
		m_cList_Object.DeleteAllItems();								//ɾ��������Ŀ
	}
	delete(filedialog);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ObjType����
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show_Name(&m_cList_ObjType,"ObjType",m_cOpen_File.GetCheck());	//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Object����
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	v_sName_ObjType.Replace("/","_");									//�Ƿ��ļ������ַ��滻
	MyExcel.Show_Name(&m_cList_Object,v_sName_ObjType,m_cOpen_File.GetCheck());	//����Excel��ָ���ļ����ƣ�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ObjType�б�����(�һ��б�������)
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnLvnColumnclickList6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList_ObjType.My_Sort(pNMHDR);									//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Object�б�����(�һ��б�������)
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnLvnColumnclickList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList_Object.My_Sort(pNMHDR);										//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ObjType�б�
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnNMClickList6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if(v_iItem_ObjType>=0)											//��Ч��Ŀ��ţ�����
			m_cList_ObjType.SetItemBkColor(v_iItem_ObjType,RGB(255,255,255));		//�ָ�����ɫ
		v_iItem_ObjType=pNMListView->iItem;								//��ã�ѡ����Ŀ���
		m_cList_ObjType.SetItemBkColor(v_iItem_ObjType,RGB(255,0,0));	//���ñ���ɫ
		v_sName_ObjType=m_cList_ObjType.GetItemText(v_iItem_ObjType,1);	//��ã�ѡ����Ŀ����

		v_iWorking=0;													//ObjType
		v_pIterator_Frame->second.v_iThread_Exit=1;						//�̣߳�����
		v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);			//������ѯ���ݿ��߳�
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ObjType_Child�б�
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnNMClickList7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if(v_iItem_ObjType_Child>=0)									//��Ч��Ŀ��ţ�����
			m_cList_Child.SetItemBkColor(v_iItem_ObjType_Child,RGB(255,255,255));		//�ָ�����ɫ
		v_iItem_ObjType_Child=pNMListView->iItem;						//��ã�ѡ����Ŀ���
		m_cList_Child.SetItemBkColor(v_iItem_ObjType_Child,RGB(255,0,0));	//���ñ���ɫ
		v_sName_Child=m_cList_Child.GetItemText(v_iItem_ObjType_Child,1);//��ã�ѡ����Ŀ����

		v_iWorking=1;													//ObjType_Child
		v_pIterator_Frame->second.v_iThread_Exit=1;						//�̣߳�����
		v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);			//������ѯ���ݿ��߳�
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ʾ�����б�
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int	v_iVi=0,v_iNum;													//��ʱ����
	CString	v_sTemp;													//��ʱ����
	map_ObjType_Child::iterator		v_pObjType;							//��������map_ObjType_Child
	map_Child::iterator				v_pChild;							//��������map_Child
	map_C_Data::iterator			v_pChild_Data;						//��������map_Child_Data
	mString_String::iterator		v_pCounter;							//��������map_Counter

	//��1����ʼ��������Ϣ
	m_cList_Object.DeleteAllItems();									//ɾ��������Ŀ
	while(m_cList_Object.DeleteColumn(0));								//ɾ�������У�ObjType��Ϣ�б�

	//��2����ObjType��Ϣ�б������Ϣ
	m_cList_Object.InsertColumn(0,"���",LVCFMT_CENTER,50);				//��0�У����
	m_cList_Object.InsertColumn(1,"Counter",LVCFMT_LEFT,100);			//��1�У�Counter
	//����б�������
	for (unsigned int vi=1;vi<=v_iChild_Col_Max;vi++)
	{
		v_sTemp.Format("��_%03d",vi);									//�ַ���		
		m_cList_Object.InsertColumn(vi+1,v_sTemp,LVCFMT_RIGHT,60);		//��������ơ�����š�
	}
	m_cList_Object.SetRedraw(false);									//��ֹ��˸

	//��3����ʾ��������Ϣ������б�����Ϣ��
	m_cList_Object.SetRedraw(false);									//��ֹ��˸

	//��ã�ObjType
	v_pObjType=v_cFile_XML.v_mObjType_Child.find(v_sName_ObjType.GetBuffer(0));		//��ѯ��v_sObjType
	if (v_pObjType != v_cFile_XML.v_mObjType_Child.end())
	{
		v_pChild=v_pObjType->second.v_mChild.begin();					//��ȡ���׵�ַ
		while(v_pChild!=v_pObjType->second.v_mChild.end())				//����
		{
			v_iVi = m_cList_Object.InsertItem(0xffffff,_T("0"));		//���һ��
			v_sTemp.Format("%06d",v_iVi+1);								//��ã��ַ���
			m_cList_Object.SetItemText(v_iVi,0,v_sTemp);				//��0�У����
			m_cList_Object.SetItemText(v_iVi,1,v_pChild->first);		//��1�У�Counter

			//���������Ϣ
			v_iNum=2;													//�ӵ�2�������
			v_pChild_Data=v_pChild->second.begin();						//��ã�map_Child_Data��������ʼλ��
			v_pCounter=v_pChild_Data->second.begin();					//��ã�Counter��������ʼλ��
			while(v_pCounter != v_pChild_Data->second.end())			//������Counter��������ֹλ��
			{       
				m_cList_Object.SetItemText(v_iVi,v_iNum,v_pCounter->first);	//��1�У�v_sObject
				v_pCounter++;													//������++
				v_iNum++;												//�к�++
			}

			v_pChild++;													//������++
		}
	}
	m_cList_Object.SetRedraw(true);										//�б���ʾ

	v_sTemp.Format("%d",v_iVi+1);										//
	m_cSum.SetWindowText(v_sTemp);										//��ʾ������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CCON_Tools_XML::My_Thread_Inoput(LPVOID lparam)
{
	CCON_Tools_XML *  lp_this = NULL ;
	lp_this = (CCON_Tools_XML *)lparam;

	lp_this->m_cFile.EnableWindow(false);								//�ļ�ѡ����Ч
	lp_this->m_cList_ObjType.EnableWindow(false);						//�б���Ч

	if (lp_this->v_iWorking==0)
		lp_this->My_Show_Object();										//����_������Object��
	else if (lp_this->v_iWorking==1)
		lp_this->My_Show_Object_Child();								//����_������Object_Child��

	lp_this->m_cFile.EnableWindow(true);								//�ļ�ѡ����Ч
	lp_this->m_cList_ObjType.EnableWindow(true);						//�б���Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ʾObject��Ϣ
//------------------------------------------------------------------------------------------------------			
void CCON_Tools_XML::My_Show_Object()
{
	int			v_iVi;													//��ʱ����
	CString		v_sTemp;												//��ʱ����

	map_ObjType::iterator		v_pObjType;								//��������ObjType
	map_ObjType_Child::iterator	v_pObjType_Child;						//��������ObjType_Child
	map_Child::iterator			v_pChild;								//��������map_Child
	map_C_Data::iterator		v_pChild_Data;							//��������map_Child_Data

	if (!v_bRead)														//��־�����á�����
	{
		//��ѯ��ObjType
		v_pObjType=v_cFile_XML.v_mObjType.find(v_sName_ObjType.GetBuffer(0));		//��ѯ��v_sObjType
		if (v_pObjType != v_cFile_XML.v_mObjType.end())
		{
			My_Show_Object_Data(&v_pObjType->second);					//��ʾ��Object_Date��Ϣ
		}
	}
	else																//��־������ֱ��
	{
		//��ѯ��ObjType
		v_pObjType_Child=v_cFile_XML.v_mObjType_Child.find(v_sName_ObjType.GetBuffer(0));		//��ѯ��v_sObjType
		if (v_pObjType_Child != v_cFile_XML.v_mObjType_Child.end())
		{
			v_iChild_Col_Max=0;											//������������
			//��ʾ��Child
			m_cList_Child.DeleteAllItems();								//ɾ�������б���Ŀ
			v_iItem_ObjType_Child=-1;									//ObjType_Child�б���Ŀѡ����Ч[-1]
			for(v_pChild=v_pObjType_Child->second.v_mChild.begin();v_pChild!=v_pObjType_Child->second.v_mChild.end();v_pChild++)
			{
				v_iVi = m_cList_Child.InsertItem(0xffffff,_T("0"));		//���һ��
				v_sTemp.Format("%04d",v_iVi+1);							//��ã��ַ���
				m_cList_Child.SetItemText(v_iVi,0,v_sTemp);				//��0�У����
				m_cList_Child.SetItemText(v_iVi,1,(_bstr_t)v_pChild->first);		//ObjType_Child

				//��ȡ��������������
				v_pChild_Data=v_pChild->second.begin();					//��ָ��
				if (v_pChild_Data->second.size()>v_iChild_Col_Max)
					v_iChild_Col_Max=v_pChild_Data->second.size();		//��ȡ�����ֵ
			}

			//��ʾ��Object
			My_Show_Object_Data(&v_pObjType_Child->second.v_lObject);	//��ʾ��Object_Date��Ϣ
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ʾObject_Date��Ϣ
//------------------------------------------------------------------------------------------------------			
void CCON_Tools_XML::My_Show_Object_Data(list_Object* v_pObject)
{
	int	v_iVi,v_iV_Num;													//��ʱ����
	CString	v_sTemp;													//��ʱ����

	list_Object::iterator		v_pIterator_Object;						//list_Object������
	mString_String::iterator	v_pIterator_Counter;					//map_Counter������

	m_cList_Object.DeleteAllItems();									//ɾ��������Ŀ
	while(m_cList_Object.DeleteColumn(0));								//ɾ�������У�ObjType��Ϣ�б�
	//��ObjType��Ϣ�б������Ϣ
	m_cList_Object.InsertColumn(0,"���",LVCFMT_CENTER,50);				//��0�У����
	m_cList_Object.InsertColumn(1,"Object",LVCFMT_LEFT,100);			//��1�У�Object
	m_cList_Object.InsertColumn(2,"Dn",LVCFMT_LEFT,240);				//��2�У�Object
	//�����Ϣ
	if(!v_pObject->empty())												//ObjType����Object��[����Object����Ϊ�գ�����]
	{
		//����б������ƣ���Counter���ƣ�
		v_iVi=3;														//�ӵ�3�������
		v_pIterator_Counter=v_pObject->begin()->v_mCounter.begin();		//��ã�Counter��������ʼλ��
		while(v_pIterator_Counter != v_pObject->begin()->v_mCounter.end())	//������Counter��������ֹλ��
		{       
			m_cList_Object.InsertColumn(v_iVi,v_pIterator_Counter->first,LVCFMT_RIGHT,60);	//��������ƣ���Counter����
			v_pIterator_Counter++;										//������++
			v_iVi++;													//�к�++
		}
		m_cList_Object.SetRedraw(false);								//��ֹ��˸
		//����б�����Ϣ����Object��Ϣ
		v_pIterator_Object=v_pObject->begin();							//��ã�Object��������ʼλ��
		while(v_pIterator_Object != v_pObject->end())					//������Object��������ֹλ��
		{       
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_iVi = m_cList_Object.InsertItem(0xffffff,_T("0"));		//���һ��
			v_sTemp.Format("%06d",v_iVi+1);								//��ã��ַ���
			m_cList_Object.SetItemText(v_iVi,0,v_sTemp);				//��1�У����
			m_cList_Object.SetItemText(v_iVi,1,v_pIterator_Object->v_sObject_ID);	//��2�У�v_sObject
			m_cList_Object.SetItemText(v_iVi,2,v_pIterator_Object->v_sObject_DN);	//��3�У�Dn
			v_iV_Num=3;													//Counter���ݴӵ�2����
			v_pIterator_Counter=v_pIterator_Object->v_mCounter.begin();				//��ã�Counter��������ʼλ��
			while(v_pIterator_Counter != v_pIterator_Object->v_mCounter.end())		//������Counter��������ֹλ��
			{  
				v_sTemp=v_pIterator_Counter->second;					//�滻
				v_sTemp.Replace(",",";");								//","��Ϊ";"
				v_sTemp.Replace("\n","");								//ȥ������
				m_cList_Object.SetItemText(v_iVi,v_iV_Num,v_sTemp);		//��ӣ�Object�б�
				v_pIterator_Counter++;									//������++
				v_iV_Num++;												//�к�++
			}
			v_pIterator_Object++;

			if ((v_iVi+1)%1000==0)										//��ʾ��100������
			{
				v_sTemp.Format("%d",v_iVi+1);							//
				m_cSum.SetWindowText(v_sTemp);							//��ʾ������
			}
		}
		m_cList_Object.SetRedraw(true);									//�б���ʾ

		v_sTemp.Format("%d",v_iVi+1);									//
		m_cSum.SetWindowText(v_sTemp);									//��ʾ������
	}	
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ʾObject_Child��Ϣ
//------------------------------------------------------------------------------------------------------			
void CCON_Tools_XML::My_Show_Object_Child()
{
	int	v_iVi,v_iV_Num;													//��ʱ����
	CString	v_sTemp;													//��ʱ����
	map_ObjType_Child::iterator		v_pObjType;							//��������map_ObjType_Child
	map_Child::iterator				v_pChild;							//��������map_Child
	map_C_Data::iterator			v_pIterator_Object;					//��������map_Object
	mString_String::iterator		v_pIterator_Counter;				//��������map_Counter

	//��ã�ObjType
	v_pObjType=v_cFile_XML.v_mObjType_Child.find(v_sName_ObjType.GetBuffer(0));		//��ѯ��v_sObjType
	if (v_pObjType == v_cFile_XML.v_mObjType_Child.end())
		return;															//��Ч����
	v_pChild=v_pObjType->second.v_mChild.find(v_sName_Child.GetBuffer(0));			//��ѯ��v_sName_Child
	if (v_pChild != v_pObjType->second.v_mChild.end())
	{
		m_cList_Object.DeleteAllItems();								//ɾ��������Ŀ
		while(m_cList_Object.DeleteColumn(0));							//ɾ�������У�ObjType��Ϣ�б�
		//��ObjType��Ϣ�б������Ϣ
		m_cList_Object.InsertColumn(0,"���",LVCFMT_CENTER,50);			//��1�У����
		m_cList_Object.InsertColumn(1,"Object",LVCFMT_LEFT,100);		//��2�У�Object
		//�����Ϣ
		if(v_pChild->second.size()>0)									//ObjType����Object��[����Object����Ϊ�գ�����]
		{
			//����б������ƣ���Counter���ƣ�
			v_iVi=2;													//�ӵ�2�������
			v_pIterator_Object=v_pChild->second.begin();				//����������һ��С��
			v_pIterator_Counter=v_pIterator_Object->second.begin();			//��ã�Counter��������ʼλ��
			while(v_pIterator_Counter != v_pIterator_Object->second.end())	//������Counter��������ֹλ��
			{       
				m_cList_Object.InsertColumn(v_iVi,v_pIterator_Counter->first,LVCFMT_RIGHT,60);		//��������ƣ���Counter����
				v_pIterator_Counter++;																//������++
				v_iVi++;												//�к�++
			}
			m_cList_Object.SetRedraw(false);							//��ֹ��˸
			//����б�����Ϣ����Object��Ϣ
			v_pIterator_Object=v_pChild->second.begin();				//��ã�Object��������ʼλ��
			while(v_pIterator_Object != v_pChild->second.end())			//������Object��������ֹλ��
			{       
				if (v_pIterator_Frame->second.v_iThread_Exit==2)		//ǿ���˳�������
					break;												//�˳�

				v_iVi = m_cList_Object.InsertItem(0xffffff,_T("0"));	//���һ��
				v_sTemp.Format("%06d",v_iVi+1);							//��ã��ַ���
				m_cList_Object.SetItemText(v_iVi,0,v_sTemp);			//��0�У����
				m_cList_Object.SetItemText(v_iVi,1,v_pIterator_Object->first);				//��1�У�v_sObject
				v_iV_Num=2;												//Counter���ݴӵ�2����
				v_pIterator_Counter=v_pIterator_Object->second.begin();				//��ã�Counter��������ʼλ��
				while(v_pIterator_Counter != v_pIterator_Object->second.end())		//������Counter��������ֹλ��
				{  
					v_sTemp=v_pIterator_Counter->second;				//�滻
					v_sTemp.Replace(",",";");							//","��Ϊ";"
					v_sTemp.Replace("\n","");							//ȥ������
					m_cList_Object.SetItemText(v_iVi,v_iV_Num,v_sTemp);	//��ӣ�Object�б�
					v_pIterator_Counter++;								//������++
					v_iV_Num++;											//�к�++
				}
				v_pIterator_Object++;

				if ((v_iVi+1)%1000==0)									//��ʾ��100������
				{
					v_sTemp.Format("%d",v_iVi+1);						//
					m_cSum.SetWindowText(v_sTemp);						//��ʾ������
				}
			}
			m_cList_Object.SetRedraw(true);								//�б���ʾ

			v_sTemp.Format("%d",v_iVi+1);								//
			m_cSum.SetWindowText(v_sTemp);								//��ʾ������
		}	
	}
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
