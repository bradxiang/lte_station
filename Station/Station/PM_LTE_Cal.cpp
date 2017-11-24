// PM_LTE_Cal.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "PM_LTE_Cal.h"
#include "PM_LTE_Cal_Input.h"


// CPM_LTE_Cal

IMPLEMENT_DYNCREATE(CPM_LTE_Cal, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CPM_LTE_Cal::CPM_LTE_Cal()
	: CFormView(CPM_LTE_Cal::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CPM_LTE_Cal::~CPM_LTE_Cal()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Cal::DoDataExchange(CDataExchange* pDX)
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
BEGIN_MESSAGE_MAP(CPM_LTE_Cal, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CPM_LTE_Cal::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_ELEMENT, &CPM_LTE_Cal::OnBnClickedElement)
	ON_BN_CLICKED(IDC_EXCEL2, &CPM_LTE_Cal::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_COMMAND(ID_PM_LTE_CAL_DEL, &CPM_LTE_Cal::OnConLteCellDel)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CPM_LTE_Cal::OnNMRClickList3)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CPM_LTE_Cal::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CPM_LTE_Cal::OnLvnColumnclickList3)
END_MESSAGE_MAP()


// CPM_LTE_Cal ���

#ifdef _DEBUG
void CPM_LTE_Cal::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPM_LTE_Cal::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPM_LTE_Cal ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Cal::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="LTE_���ܹ���_����";									//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б� [վַ]
	::SendMessage(m_cList_Station.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Station.InsertColumn(0,"���",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(1,"LTE��������",LVCFMT_LEFT,140);		//
	m_cList_Station.InsertColumn(2,"LTE���Ľ���",LVCFMT_LEFT,200);		//
	m_cList_Station.InsertColumn(3,"���㹫ʽ",LVCFMT_LEFT,400);			//
	m_cList_Station.InsertColumn(4,"����",LVCFMT_CENTER,80);			//
	m_cList_Station.InsertColumn(5,"Ԥ��",LVCFMT_CENTER,80);			//
	m_cList_Station.InsertColumn(6,"Ԥ������",LVCFMT_RIGHT,80);			//
	m_cList_Station.InsertColumn(7,"Ԥ������",LVCFMT_CENTER,80);		//
	m_cList_Station.InsertColumn(8,"��ע",LVCFMT_LEFT,200);				//
	m_cList_Station.SetExtendedStyle(m_cList_Station.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��Ϣ��ʼ��
	m_cKey_Select.AddString("LTE��������");
	m_cKey_Select.AddString("LTE���Ľ���");
	m_cKey_Select.AddString("���㹫ʽ");
	m_cKey_Select.SetCurSel(0);											//Ĭ�ϣ�ѡ���һ��

	v_iList_Item=-1;													//��Ŀѡ����Ч
	v_iSum=0;															//��������

	//��ʼ��ʾ
	OnBnClickedQuary();													//��ʾ������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CPM_LTE_Cal::PreTranslateMessage(MSG* pMsg)
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
void CPM_LTE_Cal::OnDestroy()
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
void CPM_LTE_Cal::OnSize(UINT nType, int cx, int cy)
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
void CPM_LTE_Cal::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Station);										//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�վַ����
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Cal::OnBnClickedElement()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPM_LTE_Cal_Input	v_cDlg;											//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����Ӧ
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Cal::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
void CPM_LTE_Cal::OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
		m_cList_Station.SetItemBkColor(v_iList_Item,RGB(255,255,255));	//�ָ�����ɫ
	v_iList_Item=v_iSelect;												//��ã�ѡ����Ŀ���
	m_cList_Station.SetItemBkColor(v_iList_Item,RGB(255,0,0));			//���ñ���ɫ����ɫ

	aMenu.LoadMenu(IDR_PM_LTE_CAL_DEL);									//װ�أ��Ҽ��˵�
	CMenu *pPopup =aMenu.GetSubMenu(0);									//���ɸ����˵�
	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);			//��ʾ���������λ��[point]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ɾ������
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Cal::OnConLteCellDel()
{
	// TODO: �ڴ���������������
	int		v_iItem,vi;													//��ʱ����
	CString	v_sCell,v_sTemp,v_sSql;										//��ʱ����

	vi = m_cList_Station.GetSelectedCount();							//ѡ����Ŀ����
	v_sTemp.Format("%d",vi);											//�ַ���
	if(MessageBox("ɾ��ѡ�е� "+v_sTemp+" �� ���ܣ� ɾ��ȷ�� ? ","����ɾ��ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)	//ɾ��ȷ�ϣ�����
	{
		//������ѡ�е�Item���в���
		while ((v_iItem = m_cList_Station.GetNextItem(-1, LVNI_SELECTED)) != -1)//��Ŀѡ�У�����
		{
			v_sCell=m_cList_Station.GetItemText(v_iItem,1);				//��ã�v_sCell [���ݱ�Ψһ��ʶ�������޸����ݱ�]

			//ɾ����Cell
			v_sSql = "delete from CON_Pm_LTE_Cal where zPm='"+v_sCell+"'";	//SQLɾ��
			My_ExecuteSQL(v_sSql);										//д��

			//ɾ�����б���Ϣ
			m_cList_Station.DeleteItem(v_iItem);						//ɾ����ѡ�е�v_iItem
			v_iList_Item=-1;											//��Ŀѡ����Ч

			//���棺Log
			COleDateTime v_tTime=COleDateTime::GetCurrentTime();
			CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");
			CString v_sSql="Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"+v_sCurrentTime+"','ɾ������(����)','�������ƣ�"
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
void CPM_LTE_Cal::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList_Station.My_Sort(pNMHDR);										//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Cal::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CPM_LTE_Cal::My_Thread_Inoput(LPVOID lparam)
{
	CPM_LTE_Cal *  lp_this = NULL ;
	lp_this = (CPM_LTE_Cal *)lparam;

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
void CPM_LTE_Cal::My_Input_Main()
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
void CPM_LTE_Cal::My_Query_Station()
{
	int				v_iID=0;											//��ʱ����
	CString			v_sSql,v_sTemp;										//��ʱ����
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT * FROM CON_Pm_LTE_Cal"; 

	//��ѯ����׼��
	m_cKey.GetWindowText(v_sLog_Key);									//��ã���ѯKEY
	v_sLog_Key.Replace("[","[[]");										//���ƴ���
	v_sLog_Key.Replace("'","");											//���ƴ���
	v_sLog_Key.Trim();													//ȥ���ո�

	if (!v_sLog_Key.IsEmpty())											//��ѯKEY����
	{
		switch (m_cKey_Select.GetCurSel())								//ѡ�񣿣���
		{
		case 0:															//��������
			v_sSql+=" where zPm like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//���Ľ���
			v_sSql+=" where zPm_Name like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//���㹫ʽ
			v_sSql+=" where zPm_Formula like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	v_sSql+=" order by zPm";

	My_LoadData_Station_Data(v_sSql);									//��ȡ��վַ��Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��վַ��Ϣ
//------------------------------------------------------------------------------------------------------			
void CPM_LTE_Cal::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem,v_iID=0,v_iTemp;							//��ʱ����
	double			v_dTemp;											//��ʱ����
	CString			v_sTemp;											//��ʱ����
	CString			v_sPM,v_sPM_Name,v_sFormula,v_sSave,v_sAlarm_Flag;	//��ʱ����
	CString			v_sAlarm_Value,v_sAlarm_Type,v_sDemo;				//��ʱ����

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

			v_sPM=v_cAdoConn.GetField_String("zPm");					//zPm
			v_sPM_Name=v_cAdoConn.GetField_String("zPm_Name");			//���Ľ���
			v_sFormula=v_cAdoConn.GetField_String("zPm_Formula");		//���㹫ʽ
			v_iTemp=v_cAdoConn.GetField_Int("zSave_Flag");				//zSave_Flag
			v_sSave=(v_iTemp==1?"����":"������");						
			v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Flag");				//zAlarm_Flag
			v_sAlarm_Flag=(v_iTemp==1?"Ԥ��":"��Ԥ��");						
			v_dTemp=v_cAdoConn.GetField_Double("zAlarm_Value");			//zAlarm_Value
			v_sAlarm_Value.Format("%6.2f",v_dTemp);
			v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Type");				//zAlarm_Type
			v_sAlarm_Type=(v_iTemp==1?"��":"��");						
			v_sDemo=v_cAdoConn.GetField_String("zDemo");				//��ע

			//��ʾ
			v_sTemp.Format(" %06d",v_iID+1);
			v_iItem=m_cList_Station.InsertItem(0xffff,v_sTemp);			//�����б�
			m_cList_Station.SetItemText(v_iItem,1,v_sPM);				//����
			m_cList_Station.SetItemText(v_iItem,2,v_sPM_Name);			//��������
			m_cList_Station.SetItemText(v_iItem,3,v_sFormula);			//���㹫ʽ
			m_cList_Station.SetItemText(v_iItem,4,v_sSave);				//�����־
			m_cList_Station.SetItemText(v_iItem,5,v_sAlarm_Flag);		//Ԥ����־
			m_cList_Station.SetItemText(v_iItem,6,v_sAlarm_Value);		//Ԥ��ֵ
			m_cList_Station.SetItemText(v_iItem,7,v_sAlarm_Type);		//Ԥ������
			m_cList_Station.SetItemText(v_iItem,8,v_sDemo);				//������

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
bool CPM_LTE_Cal::My_ExecuteSQL(CString v_sSql)
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
