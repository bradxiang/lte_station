// Pm_LTE.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Pm_LTE.h"


// CPm_LTE

IMPLEMENT_DYNCREATE(CPm_LTE, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CPm_LTE::CPm_LTE()
	: CFormView(CPm_LTE::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CPm_LTE::~CPm_LTE()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CPm_LTE::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_LIST3, m_cList);
	DDX_Control(pDX, IDC_QUARY, m_cInput);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_EDIT1, m_cKey);
	DDX_Control(pDX, IDC_QUARY2, m_cQuery);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPm_LTE, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CPm_LTE::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL2, &CPm_LTE::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CPm_LTE::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CPm_LTE::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_QUARY2, &CPm_LTE::OnBnClickedQuary2)
END_MESSAGE_MAP()


// CPm_LTE ���

#ifdef _DEBUG
void CPm_LTE::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPm_LTE::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPm_LTE ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CPm_LTE::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="LTE_���ܹ���_ԭʼ";									//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б� [BTS]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"����",LVCFMT_CENTER,80);					//
	m_cList.InsertColumn(2,"����",LVCFMT_CENTER,80);					//
	m_cList.InsertColumn(3,"LTE����",LVCFMT_LEFT,300);					//
	m_cList.InsertColumn(4,"���Ľ���",LVCFMT_LEFT,300);					//
	m_cList.InsertColumn(5,"������",LVCFMT_CENTER,80);					//
	m_cList.InsertColumn(6,"Сʱͳ������",LVCFMT_CENTER,100);			//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��Ϣ��ʼ��
	m_cKey_Select.AddString("LTE����");
	m_cKey_Select.AddString("���Ľ���");
	m_cKey_Select.SetCurSel(0);											//Ĭ�ϣ�ѡ���һ��

	//��Ϣ��ʼ��
	v_iList_Item=-1;													//��Ŀѡ����Ч

	m_cInput.EnableWindow(false);										//�����ɣ���Ч

	//��ѯ
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CPm_LTE::PreTranslateMessage(MSG* pMsg)
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
void CPm_LTE::OnDestroy()
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
void CPm_LTE::OnSize(UINT nType, int cx, int cy)
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
void CPm_LTE::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����
//------------------------------------------------------------------------------------------------------
void CPm_LTE::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����Ӧ
//------------------------------------------------------------------------------------------------------
void CPm_LTE::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
void CPm_LTE::OnBnClickedQuary2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܱ�����
//------------------------------------------------------------------------------------------------------
void CPm_LTE::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString	v_sFile_Key="";												//��ʱ����
	CString	v_sSql;														//��ʱ����

	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//�����б�
	{
		if (v_sFile_Key!=m_cList.GetItemText(vi,5))						//�����Ʊ仯������
		{
			if (!v_sFile_Key.IsEmpty())									//�����±�
			{
				v_sSql+=")\r\n";										//����
				v_sSql+="CREATE NONCLUSTERED INDEX PK_Pm_LTE_"+v_sFile_Key+" ON Pm_LTE_"+v_sFile_Key+"(zTime,zCell)";	//������
				My_ExecuteSQL(v_sSql);									//ִ��
			}
			v_sFile_Key=m_cList.GetItemText(vi,5);						//������
			v_sSql="CREATE TABLE Pm_LTE_"+v_sFile_Key+"([zTime] [datetime] NOT NULL,[zCell] [varchar](100) NOT NULL";	//ʱ�䡢С��
		}
		v_sSql+=",["+m_cList.GetItemText(vi,3)+"] [float] NULL";		//�����ֶ�
	}
	if (!v_sFile_Key.IsEmpty())											//�����±�
	{
		v_sSql+=")\r\n";												//����
		v_sSql+="CREATE NONCLUSTERED INDEX PK_Pm_LTE_"+v_sFile_Key+" ON Pm_LTE_"+v_sFile_Key+"(zTime,zCell)";	//������
		My_ExecuteSQL(v_sSql);											//ִ��
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CPm_LTE::My_Thread_Inoput(LPVOID lparam)
{
	CPm_LTE *  lp_this = NULL ;
	lp_this = (CPm_LTE *)lparam;

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
void CPm_LTE::My_Input_Main()
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
void CPm_LTE::My_LoadData_Per()
{
	int				v_iItem,v_iID=0,v_iHour;							//��ʱ����
	CString			v_sTemp,v_sSql,v_sLog_Key,v_sHour;					//��ʱ����
	CString			v_sType,v_sType_1,v_sPm,v_sPm_Name,v_sTable_Key;	//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList.DeleteAllItems();										//���Items
		v_iList_Item=-1;												//��Ŀѡ����Ч

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE ";

		//��ѯ����
		m_cKey.GetWindowText(v_sLog_Key);								//��ã���ѯKEY
		v_sLog_Key.Replace("[","[[]");									//���ƴ���
		v_sLog_Key.Replace("'","");										//���ƴ���
		v_sLog_Key.Trim();												//ȥ���ո�

		if (!v_sLog_Key.IsEmpty())										//��ѯKEY����
		{
			switch (m_cKey_Select.GetCurSel())							//ѡ�񣿣���
			{
			case 0:														//LTE����
				v_sSql+=" where zPm like '%"+v_sLog_Key+"%'";
				break;
			case 1:														//���Ľ���
				v_sSql+=" where  zPm_Name like '%"+v_sLog_Key+"%'";
				break;
			default:
				break;
			}
		}
		v_sSql+=" order by ztype,ztype_1,zPm";

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
			v_iHour = v_cAdoConn.GetField_Int("zHour");					//zHour

			//��ʾ
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,v_sType);						//����
			m_cList.SetItemText(v_iItem,2,v_sType_1);					//����
			m_cList.SetItemText(v_iItem,3,v_sPm);						//LTE����
			m_cList.SetItemText(v_iItem,4,v_sPm_Name);					//���Ľ���
			m_cList.SetItemText(v_iItem,5,v_sTable_Key);				//������
			v_sHour=(v_iHour==0?"�ϼ�":(v_iHour==1?"ƽ��":(v_iHour==2?"���":"��С")));
			m_cList.SetItemText(v_iItem,6,v_sHour);						//Сʱͳ������

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
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CPm_LTE::My_ExecuteSQL(CString v_sSql)
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
