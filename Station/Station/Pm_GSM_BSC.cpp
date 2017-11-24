// Pm_GSM_BSC.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Pm_GSM_BSC.h"
#include "Pm_GSM_UpDate.h"
#include "Pm_GSM_UpDate_PM.h"
#include "Pm_GSM_BSC_Add.h"


// CPm_GSM_BSC

IMPLEMENT_DYNCREATE(CPm_GSM_BSC, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CPm_GSM_BSC::CPm_GSM_BSC()
	: CFormView(CPm_GSM_BSC::IDD)
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CPm_GSM_BSC::~CPm_GSM_BSC()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_Station);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_COMBO4, m_cType);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPm_GSM_BSC, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CPm_GSM_BSC::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL2, &CPm_GSM_BSC::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CPm_GSM_BSC::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CPm_GSM_BSC::OnLvnColumnclickList3)
	ON_CBN_CLOSEUP(IDC_COMBO4, &CPm_GSM_BSC::OnCbnCloseupCombo4)
	ON_BN_CLICKED(IDC_EXCEL3, &CPm_GSM_BSC::OnBnClickedExcel3)
	ON_BN_CLICKED(IDC_EXCEL4, &CPm_GSM_BSC::OnBnClickedExcel4)
	ON_BN_CLICKED(IDC_EXCEL5, &CPm_GSM_BSC::OnBnClickedExcel5)
	ON_BN_CLICKED(IDC_EXCEL6, &CPm_GSM_BSC::OnBnClickedExcel6)
END_MESSAGE_MAP()


// CPm_GSM_BSC ���

#ifdef _DEBUG
void CPm_GSM_BSC::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPm_GSM_BSC::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPm_GSM_BSC ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="GSM_���ܼ���_BSC";									//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б�
	::SendMessage(m_cList_Station.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Station.SetExtendedStyle(m_cList_Station.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��Ϣ��ʼ��
	m_cType.AddString("������");
	m_cType.AddString("��������");
	m_cType.SetCurSel(0);												//Ĭ�ϣ�ѡ���һ��

	v_iList_Item=-1;													//��Ŀѡ����Ч

	//��ʾ
	OnCbnCloseupCombo4();												//����ѡ����Ӧ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CPm_GSM_BSC::PreTranslateMessage(MSG* pMsg)
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
void CPm_GSM_BSC::OnDestroy()
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
void CPm_GSM_BSC::OnSize(UINT nType, int cx, int cy)
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
void CPm_GSM_BSC::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Station);										//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ��
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnCbnCloseupCombo4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_cList_Station.DeleteAllItems();									//�б����
	while(m_cList_Station.DeleteColumn(0));								//ɾ��������

	//��ѯ����ѡ��
	m_cKey_Select.ResetContent();										//���
	if (m_cType.GetCurSel()==0)											//ObjType
	{
		m_cKey_Select.AddString("������");
		m_cKey_Select.AddString("����");
		m_cKey_Select.SetCurSel(0);										//Ĭ�ϣ�ѡ���һ��

		m_cList_Station.InsertColumn(0,"���",LVCFMT_CENTER,60);		//
		m_cList_Station.InsertColumn(1,"������",LVCFMT_LEFT,210);		//
		m_cList_Station.InsertColumn(2,"����",LVCFMT_LEFT,80);			//
		m_cList_Station.InsertColumn(3,"��������",LVCFMT_RIGHT,120);	//

		m_cList_Station.SetNumber(3);									//����������Ч
	}
	else																//Counter
	{
		m_cKey_Select.AddString("������");
		m_cKey_Select.AddString("����");
		m_cKey_Select.AddString("��������");
		m_cKey_Select.AddString("���㹫ʽ");
		m_cKey_Select.AddString("ObjType");
		m_cKey_Select.SetCurSel(0);										//Ĭ�ϣ�ѡ���һ��

		m_cList_Station.InsertColumn(0,"���",LVCFMT_CENTER,60);		//
		m_cList_Station.InsertColumn(1,"������",LVCFMT_LEFT,210);		//
		m_cList_Station.InsertColumn(2,"����",LVCFMT_LEFT,80);			//
		m_cList_Station.InsertColumn(3,"��������",LVCFMT_LEFT,300);		//
		m_cList_Station.InsertColumn(4,"���㹫ʽ",LVCFMT_LEFT,500);		//
		m_cList_Station.InsertColumn(5,"ObjType",LVCFMT_LEFT,200);		//
		m_cList_Station.InsertColumn(6,"��λ",LVCFMT_LEFT,60);			//
		m_cList_Station.InsertColumn(7,"���",LVCFMT_CENTER,90);		//
		m_cList_Station.InsertColumn(8,"Ψһ��ʶ",LVCFMT_CENTER,0);		//

		m_cList_Station.ClearNumber(3);									//����������Ч
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����Ӧ
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
//	�������ܣ��б�����
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList_Station.My_Sort(pNMHDR);									//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnBnClickedExcel3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_cType.GetCurSel()==1)											//��������
	{
		CPm_GSM_UpDate_PM	v_cDlg;										//ʵ����
		v_cDlg.v_sPM_Table=m_cList_Station.GetItemText(v_iList_Item,1);	//��ֵ��������
		v_cDlg.v_sType=m_cList_Station.GetItemText(v_iList_Item,2);		//��ֵ������
		v_cDlg.v_sPM_Name=m_cList_Station.GetItemText(v_iList_Item,3);	//��ֵ����������
		v_cDlg.v_sFormula=m_cList_Station.GetItemText(v_iList_Item,4);	//��ֵ�����㹫ʽ
		v_cDlg.v_sObjType=m_cList_Station.GetItemText(v_iList_Item,5);	//��ֵ��ObjType
		v_cDlg.v_sUnit=m_cList_Station.GetItemText(v_iList_Item,6);		//��ֵ����λ
		v_cDlg.v_sNum=m_cList_Station.GetItemText(v_iList_Item,8);		//��ֵ��Ψһ��ʶ
		v_cDlg.v_bFlag=true;											//����
		v_cDlg.v_bBSC=true;												//BSC
		if (v_cDlg.DoModal()==IDOK)
			OnBnClickedQuary();											//��ʾ�����½��
	}
				
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��޸�
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnBnClickedExcel4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (v_iList_Item==-1)
	{
		MessageBox("��ѡ��һ����Ч����Ϣ������","��Ϣ��ʾ",MB_OK);		//��ʾ
		return;
	}

	if (m_cType.GetCurSel()==0)											//������
	{
		CPm_GSM_UpDate	v_cDlg;											//ʵ����
		v_cDlg.v_sPM_Table=m_cList_Station.GetItemText(v_iList_Item,1);	//��ֵ��������
		v_cDlg.v_sType=m_cList_Station.GetItemText(v_iList_Item,2);		//��ֵ������
		v_cDlg.v_sDay=m_cList_Station.GetItemText(v_iList_Item,3);		//��ֵ����������
		if (v_cDlg.DoModal()==IDOK)
			m_cList_Station.SetItemText(v_iList_Item,3,v_cDlg.v_sDay);	//��ʾ����������
	}
	else
	{
		CPm_GSM_UpDate_PM	v_cDlg;										//ʵ����
		v_cDlg.v_sPM_Table=m_cList_Station.GetItemText(v_iList_Item,1);	//��ֵ��������
		v_cDlg.v_sType=m_cList_Station.GetItemText(v_iList_Item,2);		//��ֵ������
		v_cDlg.v_sPM_Name=m_cList_Station.GetItemText(v_iList_Item,3);	//��ֵ����������
		v_cDlg.v_sFormula=m_cList_Station.GetItemText(v_iList_Item,4);	//��ֵ�����㹫ʽ
		v_cDlg.v_sObjType=m_cList_Station.GetItemText(v_iList_Item,5);	//��ֵ��ObjType
		v_cDlg.v_sUnit=m_cList_Station.GetItemText(v_iList_Item,6);		//��ֵ����λ
		v_cDlg.v_sNum=m_cList_Station.GetItemText(v_iList_Item,8);		//��ֵ��Ψһ��ʶ
		if (v_cDlg.DoModal()==IDOK)
		{
			m_cList_Station.SetItemText(v_iList_Item,4,v_cDlg.v_sFormula);	//��ʾ�����㹫ʽ
			m_cList_Station.SetItemText(v_iList_Item,5,v_cDlg.v_sObjType);	//��ʾ��ObjType
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ɾ��
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnBnClickedExcel5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString v_sSql;														//��ʱ����

	if (v_iList_Item==-1)
	{
		MessageBox("��ѡ��һ����Ч����Ϣ������","��Ϣ��ʾ",MB_OK);		//��ʾ
		return;
	}
	if(MessageBox("�Ƿ�ɾ�����ܣ� "+m_cList_Station.GetItemText(v_iList_Item,3),"����ɾ��ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
	{
		v_sSql="delete from A_CAP_Info where znum='"+m_cList_Station.GetItemText(v_iList_Item,8)+"'";	//SQL
		My_ExecuteSQL(v_sSql);											//ִ��
		OnBnClickedQuary();												//��ʾ�����½��
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������ݹ���
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnBnClickedExcel6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPm_GSM_BSC_Add	v_cDlg;												//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CPm_GSM_BSC::My_Thread_Inoput(LPVOID lparam)
{
	CPm_GSM_BSC *  lp_this = NULL ;
	lp_this = (CPm_GSM_BSC *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel_NodeB.EnableWindow(false);						//NodeB��������Ч
	lp_this->m_cType.EnableWindow(false);								//���ͣ���Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel_NodeB.EnableWindow(true);							//NodeB��������Ч
	lp_this->m_cType.EnableWindow(true);								//���ͣ���Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_Query_Station();												//վַ��ѯ
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ��Ϣ
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::My_Query_Station()
{
	int				v_iID=0;											//��ʱ����
	CString			v_sSql,v_sTemp;										//��ʱ����
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//��ѯ����
	m_cKey.GetWindowText(v_sLog_Key);									//��ã���ѯKEY
	v_sLog_Key.Replace("[","[[]");										//���ƴ���
	v_sLog_Key.Replace("'","");											//���ƴ���
	v_sLog_Key.Trim();													//ȥ���ո�

	if (m_cType.GetCurSel()==0)											//������
	{
		v_sSql += "SELECT * FROM A_CAPGROUP_Info where ztypename like '%BSC%'"; 

		if (!v_sLog_Key.IsEmpty())										//��ѯKEY����
		{
			switch (m_cKey_Select.GetCurSel())							//ѡ�񣿣���
			{
			case 0:														//������
				v_sSql+=" where zgroupname like '%"+v_sLog_Key+"%'";
				break;
			case 1:														//����
				v_sSql+=" where ztypename like '%"+v_sLog_Key+"%'";
				break;
			default:
				break;
			}
		}
		v_sSql+=" order by zgroupname";
	}
	else
	{
		v_sSql += "SELECT znum,PM_G.zgroupname,PM_G.ztypename,zcap,zformula,zobjtypes,zunit,zmark FROM A_CAP_Info \
				  left join A_CAPGROUP_Info as PM_G on PM_G.zgroupnum=A_CAP_Info.zgroupnum where ztypename like '%BSC%'"; 

		if (!v_sLog_Key.IsEmpty())										//��ѯKEY����
		{
			switch (m_cKey_Select.GetCurSel())							//ѡ�񣿣���
			{
			case 0:														//������
				v_sSql+=" and zgroupname like '%"+v_sLog_Key+"%'";
				break;
			case 1:														//����
				v_sSql+=" and ztypename like '%"+v_sLog_Key+"%'";
				break;
			case 2:														//��������
				v_sSql+=" and zcap like '%"+v_sLog_Key+"%'";
				break;
			case 3:														//���㹫ʽ
				v_sSql+=" and zformula like '%"+v_sLog_Key+"%'";
				break;
			case 4:														//ObjType
				v_sSql+=" and zobjtypes like '%"+v_sLog_Key+"%'";
				break;
			default:
				break;
			}
		}
		v_sSql+=" order by zgroupname,zcap";
	}

	My_LoadData_Station_Data(v_sSql);									//��ȡ��վַ��Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ����Ϣ
//------------------------------------------------------------------------------------------------------			
void CPm_GSM_BSC::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem,v_iID=0;									//��ʱ����
	CString			v_sTemp;											//��ʱ����

	CString			v_sPM_Table,v_sType,v_sPM_Name,v_sFormula,v_sObjType,v_sUnit,v_sMark;	//��ʱ����
	int				v_iDay,v_iMark,v_iNum;								//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList_Station.DeleteAllItems();								//���Items
		v_iList_Item=-1;												//��Ŀѡ����Ч

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			if (m_cType.GetCurSel()==0)									//������
			{
				v_sPM_Table = v_cAdoConn.GetField_String("zgroupname");	//������
				v_sType = v_cAdoConn.GetField_String("ztypename");		//����
				v_iDay = v_cAdoConn.GetField_Int("zsavetime");			//��������

				//��ʾ
				v_sTemp.Format(" %04d",v_iID+1);
				v_iItem=m_cList_Station.InsertItem(0xffff,v_sTemp);		//�����б�
				m_cList_Station.SetItemText(v_iItem,1,v_sPM_Table);		//������
				m_cList_Station.SetItemText(v_iItem,2,v_sType);			//����
				v_sTemp.Format("%d",v_iDay);							//�ַ���
				m_cList_Station.SetItemText(v_iItem,3,v_sTemp);			//��������
			}
			else
			{
				v_sPM_Table = v_cAdoConn.GetField_String("zgroupname");	//������
				v_sType = v_cAdoConn.GetField_String("ztypename");		//����
				v_sPM_Name = v_cAdoConn.GetField_String("zcap");		//��������
				v_sFormula = v_cAdoConn.GetField_String("zformula");	//���㹫ʽ
				v_sObjType = v_cAdoConn.GetField_String("zobjtypes");	//ObjType
				v_sUnit = v_cAdoConn.GetField_String("zunit");			//��λ
				v_iMark = v_cAdoConn.GetField_Int("zmark");				//���
				v_iNum = v_cAdoConn.GetField_Int("znum");				//Ψһ��ʶ��

				//��ʾ
				v_sTemp.Format(" %04d",v_iID+1);
				v_iItem=m_cList_Station.InsertItem(0xffff,v_sTemp);		//�����б�
				m_cList_Station.SetItemText(v_iItem,1,v_sPM_Table);		//������
				m_cList_Station.SetItemText(v_iItem,2,v_sType);			//����
				m_cList_Station.SetItemText(v_iItem,3,v_sPM_Name);		//��������
				m_cList_Station.SetItemText(v_iItem,4,v_sFormula);		//���㹫ʽ
				m_cList_Station.SetItemText(v_iItem,5,v_sObjType);		//ObjType
				m_cList_Station.SetItemText(v_iItem,6,v_sUnit);			//��λ
				v_sTemp=v_iMark==1?"BSC���ܼ���":"";					//���
				m_cList_Station.SetItemText(v_iItem,7,v_sTemp);			//���
				v_sTemp.Format("%d",v_iNum);							//�ַ���
				m_cList_Station.SetItemText(v_iItem,8,v_sTemp);			//Ψһ��ʶ��
			}

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
bool CPm_GSM_BSC::My_ExecuteSQL(CString v_sSql)
{
	// TODO: Add your control notification handler code here
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);

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
