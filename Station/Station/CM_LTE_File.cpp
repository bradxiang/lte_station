// CM_LTE_File.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "CM_LTE_File.h"


// CCM_LTE_File

IMPLEMENT_DYNCREATE(CCM_LTE_File, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CCM_LTE_File::CCM_LTE_File()
	: CFormView(CCM_LTE_File::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CCM_LTE_File::~CCM_LTE_File()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CCM_LTE_File::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_LIST3, m_cList);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_QUARY2, m_cQuery);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCM_LTE_File, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL2, &CCM_LTE_File::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CCM_LTE_File::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CCM_LTE_File::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_QUARY2, &CCM_LTE_File::OnBnClickedQuary2)
END_MESSAGE_MAP()


// CCM_LTE_File ���

#ifdef _DEBUG
void CCM_LTE_File::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCM_LTE_File::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCM_LTE_File ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CCM_LTE_File::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="LTE ���ù���_�ļ�";									//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б� [BTS]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"����",LVCFMT_CENTER,80);					//
	m_cList.InsertColumn(2,"�ļ�����",LVCFMT_LEFT,120);					//
	m_cList.InsertColumn(3,"���Ľ���",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(4,"�ӱ�����",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(5,"�ӱ����",LVCFMT_CENTER,80);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList.SetNumber(5);

	//��Ϣ��ʼ��
	m_cKey_Select.AddString("��Ϊ");
	m_cKey_Select.AddString("����");
	m_cKey_Select.SetCurSel(0);											//Ĭ�ϣ�ѡ���һ��

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
BOOL CCM_LTE_File::PreTranslateMessage(MSG* pMsg)
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
void CCM_LTE_File::OnDestroy()
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
void CCM_LTE_File::OnSize(UINT nType, int cx, int cy)
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
//	�������ܣ�����
//------------------------------------------------------------------------------------------------------
void CCM_LTE_File::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����
//------------------------------------------------------------------------------------------------------
void CCM_LTE_File::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����Ӧ
//------------------------------------------------------------------------------------------------------
void CCM_LTE_File::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
//	�������ܣ���ѯ
//------------------------------------------------------------------------------------------------------
void CCM_LTE_File::OnBnClickedQuary2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CCM_LTE_File::My_Thread_Inoput(LPVOID lparam)
{
	CCM_LTE_File *  lp_this = NULL ;
	lp_this = (CCM_LTE_File *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel_NodeB.EnableWindow(false);						//NodeB��������Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel_NodeB.EnableWindow(true);							//NodeB��������Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CCM_LTE_File::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Per();												//��ѯ
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ�����ܲ�����Ϣ
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_File::My_LoadData_Per()
{
	int				v_iItem,v_iID=0,v_iNum;								//��ʱ����
	CString			v_sTemp,v_sSql,v_sNum_Demo,v_sMaker;				//��ʱ����
	CString			v_sFile_Name,v_sFile_Demo,v_sNum;					//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList.DeleteAllItems();										//���Items
		v_iList_Item=-1;												//��Ŀѡ����Ч

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Cm_LTE_File ";
		//��ѯ����
		switch (m_cKey_Select.GetCurSel())								//ѡ�񣿣���
		{
		case 0:															//��Ϊ
			v_sSql+=" where zMaker='��Ϊ'";
			break;
		case 1:															//����
			v_sSql+=" where zMaker='����'";
			break;
		default:
			break;
		}
		v_sSql+=" order by zFile_Name,zNum";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sMaker = v_cAdoConn.GetField_String("zMaker");			//zMaker
			v_sNum_Demo = v_cAdoConn.GetField_String("zNum_Demo");		//zNum_Demo
			v_sFile_Name = v_cAdoConn.GetField_String("zFile_Name");	//zFile_Name
			v_sFile_Demo = v_cAdoConn.GetField_String("zFile_Demo");	//zFile_Demo
			v_iNum = v_cAdoConn.GetField_Int("zNum");					//zNum

			//��ʾ
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,v_sMaker);					//����
			m_cList.SetItemText(v_iItem,2,v_sFile_Name);				//�ļ�����
			m_cList.SetItemText(v_iItem,3,v_sFile_Demo);				//���Ľ���
			m_cList.SetItemText(v_iItem,4,v_sNum_Demo);					//�ӱ�����
			v_sNum.Format("%d",v_iNum);									//�ַ���
			m_cList.SetItemText(v_iItem,5,v_sNum);						//�ӱ����

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
