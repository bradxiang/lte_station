// Alarm_LL_Cell.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Alarm_LL_Cell.h"
#include "Alarm_LL_Cell_Time.h"


// CAlarm_LL_Cell

IMPLEMENT_DYNCREATE(CAlarm_LL_Cell, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Cell::CAlarm_LL_Cell()
	: CFormView(CAlarm_LL_Cell::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Cell::~CAlarm_LL_Cell()
{
	v_mAlarm_LL_Cell.clear();											//�������
	v_mTable_Name.clear();												//�������
	v_mCell_ID.clear();													//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_LIST3, m_cList);
	DDX_Control(pDX, IDC_QUARY2, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel);
	DDX_Control(pDX, IDC_QUARY3, m_cTime);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAlarm_LL_Cell, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL2, &CAlarm_LL_Cell::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CAlarm_LL_Cell::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CAlarm_LL_Cell::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_QUARY2, &CAlarm_LL_Cell::OnBnClickedQuary2)
	ON_BN_CLICKED(IDC_QUARY3, &CAlarm_LL_Cell::OnBnClickedQuary3)
END_MESSAGE_MAP()


// CAlarm_LL_Cell ���

#ifdef _DEBUG
void CAlarm_LL_Cell::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAlarm_LL_Cell::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAlarm_LL_Cell ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="LTE ������С����ѯ";									//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б� [BTS]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"С��",LVCFMT_LEFT,200);						//��1�У�С��
	m_cList.InsertColumn(2,"Ӫҵ��",LVCFMT_LEFT,60);					//��2�У�Ӫҵ��
	m_cList.InsertColumn(3,"״̬",LVCFMT_LEFT,40);						//��3�У�״̬
	m_cList.InsertColumn(4,"����",LVCFMT_CENTER,120);					//��4�У�����
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��Ϣ��ʼ��
	v_iList_Item=-1;													//��Ŀѡ����Ч

	//����Ĭ�ϣ���ѯ����
	v_bDown=false;														//��Ч
	v_bUp=false;														//��Ч
	for(int vi=0;vi<5;vi++)
	{
		v_dDate_Select[vi].v_bFlag=false;								//��Ч
		v_dDate_Select[vi].v_iCal=0;									//<
		v_dDate_Select[vi].v_dDate=COleDateTime::GetCurrentTime();		//��ǰʱ��
		v_dDate_Select[vi].v_dValues=100;								//ֵ
		v_dDate_Select[vi].v_iHour=8;									//8��
	}

	//��ȡ�����ܲ���
	v_iWorking=2;														//�������񣺻�ȡ��ʼ����
	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CAlarm_LL_Cell::PreTranslateMessage(MSG* pMsg)
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
void CAlarm_LL_Cell::OnDestroy()
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
void CAlarm_LL_Cell::OnSize(UINT nType, int cx, int cy)
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
void CAlarm_LL_Cell::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ʱ���������
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell::OnBnClickedQuary3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString	v_sTemp,v_sTemp_1;											//��ʱ����
	int		v_iNum,v_iTime;												//��ʱ����

	CAlarm_LL_Cell_Time	v_cDlg;											//ʵ����

	//��ֵ����ѡ��Ĳ���
	v_cDlg.v_dDate_Select=&v_dDate_Select[0];							//����ѡ��
	v_cDlg.v_bDown=v_bDown;												//״̬������
	v_cDlg.v_bUp=v_bUp;													//״̬������
	
	if(v_cDlg.DoModal()!=IDOK)											//ȷ�Ϸ��أ�����
		return;															//��Ч������

	//��ȡ����ѡ��Ĳ���
	v_bDown=v_cDlg.v_bDown;												//״̬������
	v_bUp=v_cDlg.v_bUp;													//״̬������

	//����������Ϣ��ѡ������ȡ�
	m_cList.DeleteAllItems();											//ɾ���б���Ϣ
	while(m_cList.DeleteColumn(0));										//ɾ��������
	m_cSum.SetWindowText("");											//�������
	
	//Ĭ������Ϣ
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"С��",LVCFMT_LEFT,200);						//��1�У�С��
	m_cList.InsertColumn(2,"Ӫҵ��",LVCFMT_LEFT,60);					//��2�У�Ӫҵ��
	m_cList.InsertColumn(3,"״̬",LVCFMT_LEFT,40);						//��3�У�״̬

	v_iNum=4;															//�����
	//������Ч
	if (v_bDown)														//��Ч�����У�����
	{
		for (v_iTime=0;v_iTime<5;v_iTime++)								//����������
		{
			if (v_dDate_Select[v_iTime].v_bFlag)						//��Ч�����ȣ�����
			{
				v_sTemp=v_dDate_Select[v_iTime].v_dDate.Format("%Y-%m-%d");	//����
				v_sTemp_1.Format("%02d",v_dDate_Select[v_iTime].v_iHour);	//Сʱ
				v_sTemp="����_"+v_sTemp+"_"+v_sTemp_1;					//������
				m_cList.InsertColumn(v_iNum,v_sTemp,LVCFMT_RIGHT,130);	//���ã�������
				m_cList.SetNumber(v_iNum);								//���ã�������ʽ
				v_iNum++;												//������
			}
		}
	}
	//������Ч
	if (v_bUp)															//��Ч�����У�����
	{
		for (v_iTime=0;v_iTime<5;v_iTime++)								//����������
		{
			if (v_dDate_Select[v_iTime].v_bFlag)						//��Ч�����ȣ�����
			{
				v_sTemp=v_dDate_Select[v_iTime].v_dDate.Format("%Y-%m-%d");	//����
				v_sTemp_1.Format("%02d",v_dDate_Select[v_iTime].v_iHour);	//Сʱ
				v_sTemp="����_"+v_sTemp+"_"+v_sTemp_1;					//������
				m_cList.InsertColumn(v_iNum,v_sTemp,LVCFMT_RIGHT,130);	//���ã�������
				m_cList.SetNumber(v_iNum);								//���ã�������ʽ
				v_iNum++;												//������
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����Ӧ
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
void CAlarm_LL_Cell::OnBnClickedQuary2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CAlarm_LL_Cell::My_Thread_Inoput(LPVOID lparam)
{
	CAlarm_LL_Cell *  lp_this = NULL ;
	lp_this = (CAlarm_LL_Cell *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(false);								//��������Ч
	lp_this->m_cTime.EnableWindow(false);								//ʱ�����ã���Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(true);								//��������Ч
	lp_this->m_cTime.EnableWindow(true);								//ʱ�����ã���Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Per();												//��ѯ
		break;
	case 2:
		My_LoadData_Para();												//��ȡ�����ܲ���
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯС��������Ϣ
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Cell::My_LoadData_Per()
{
	bool		v_bOK=false,v_bCell=false;								//��ʱ����
	CString		v_sTable,v_sTemp;										//��ʱ����
	int			v_iTime,v_iNum,v_iID=0;									//��ʱ����

	//��1����ʼ��
	m_cList.DeleteAllItems();											//ɾ���б���Ϣ
	v_mAlarm_LL_Cell.clear();											//�������
	m_cSum.SetWindowText("");											//�������

	if (!v_bDown && !v_bUp)												//��Ч����ѡ�񣿣���
	{
		MessageBox("  ��ѡ����Ч������ !!! ");							//������ʾ
		return;
	}

	//��2����ȡ���������ݡ����ڸ��ֲ�ѯ���ǻ�Ĺ�ϵ�����Ե�һ�β�ѯ�����ӣ��Ժ��ѯ�����޳�����
	for (v_iTime=0;v_iTime<5;v_iTime++)									//����������
	{
		if (v_dDate_Select[v_iTime].v_bFlag)							//��Ч�����ȣ�����
		{
			v_bOK=true;													//������Ч������ѡ��

			//��3����ȡ��������
			v_sTable=v_dDate_Select[v_iTime].v_dDate.Format("%Y%m");	//����
			v_sTable="Pm_LTE_PDCP_"+v_sTable+"_"+My_Get_Table_Num(v_dDate_Select[v_iTime].v_dDate.GetDay());	//�ļ�����(�����)

			//��4����飺�������Ƿ���Ч�����ݣ������Ƽ���v_mTable_Name���������������Ч����鲻���������ݡ���
			set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//���ң���
			if (v_pTable==v_mTable_Name.end())							//û�ҵ������ݱ�
			{
				v_sTable=v_dDate_Select[v_iTime].v_dDate.Format("%Y%m%d");	//������
				MessageBox("  ��ѡ����Ч�����ȣ�"+v_sTable);			//������ʾ
				break;
			}
			else
			{
				if (v_bDown)											//������Ч������
				{
					My_LoadData_PM_Data(v_iTime,1,v_sTable,v_bCell);	//���ܻ�ȡ
					v_bCell=true;										//���ã���һ�λ�ȡ��Ч
				}
				if (v_bUp)												//������Ч������
				{
					My_LoadData_PM_Data(v_iTime,0,v_sTable,v_bCell);	//���ܻ�ȡ
					v_bCell=true;										//���ã���һ�λ�ȡ��Ч
				}
			}

			//��5��
		}
	}
	if (!v_bOK)															//��Ч����ѡ�񣿣���
	{
		MessageBox("  ��ѡ����Ч������ !!! ");							//������ʾ
		return;
	}

	//��3����ʾ����������
	map_Alarm_LL_PM_DATA::iterator	v_pPM_Cell;							//������
	for (v_pPM_Cell=v_mAlarm_LL_Cell.begin();v_pPM_Cell!=v_mAlarm_LL_Cell.end();v_pPM_Cell++)	//������С��
	{
		int v_iIndex = m_cList.InsertItem(0xffffff,_T("0"));
		v_sTemp.Format("%06d",v_iIndex+1);
		m_cList.SetItemText(v_iIndex,0,v_sTemp);						//���
		m_cList.SetItemText(v_iIndex,1,v_pPM_Cell->first);				//Cell
		m_cList.SetItemText(v_iIndex,2,v_pPM_Cell->second.v_sRegion);	//Ӫҵ��
		m_cList.SetItemText(v_iIndex,3,v_pPM_Cell->second.v_sOnline);	//״̬
		v_iNum=4;														//�����
		if (v_bDown)													//��Ч�����У�����
		{
			for (v_iTime=0;v_iTime<5;v_iTime++)							//����������
			{
				if (v_dDate_Select[v_iTime].v_bFlag)					//��Ч�����ȣ�����
				{
					v_sTemp.Format("%8.2f",v_pPM_Cell->second.v_dD_Values[v_iTime]);	//�ַ���
					m_cList.SetItemText(v_iIndex,v_iNum,v_sTemp);		//������ֵ
					v_iNum++;											//������
				}
			}
		}
		//������Ч
		if (v_bUp)														//��Ч�����У�����
		{
			for (v_iTime=0;v_iTime<5;v_iTime++)							//����������
			{
				if (v_dDate_Select[v_iTime].v_bFlag)					//��Ч�����ȣ�����
				{
					v_sTemp.Format("%8.2f",v_pPM_Cell->second.v_dU_Values[v_iTime]);	//�ַ���
					m_cList.SetItemText(v_iIndex,v_iNum,v_sTemp);		//������ֵ
					v_iNum++;											//������
				}
			}
		}
	
		v_iID++;														//������
		if (v_iID%100==0)
		{
			v_sTemp.Format("%d",v_iID);									//
			m_cSum.SetWindowText(v_sTemp);								//��ʾ������
		}
	}
	v_sTemp.Format("%d",v_iID);											//
	m_cSum.SetWindowText(v_sTemp);										//��ʾ��Log����
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ��������
//	��    ����v_iTime��������ţ�	v_iType��(1�����У�0�����У�)	v_sTable��������
//	��    �����������ݷ�������
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Cell::My_LoadData_PM_Data(int v_iTime,int v_iType,CString v_sTable,bool v_bCell)
{
	CString			v_sSql,v_sPM_Cell,v_sRegion,v_sOnline;				//��ʱ����
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_tDate;											//����
	double			v_dValue;											//������ֵ
	int				v_iCell_ID;											//��ʱ����

	_variant_t		Var;

	Alarm_LL_PM_DATA		v_cPM_DATA;									//�ṹ

	map_Cell_ID::iterator	v_pIter_Cell;								//������
	map_Alarm_LL_PM_DATA::iterator	v_pPM_Cell;							//������

	CString		v_sField,v_sValue,v_sTime,v_sCal;						//��ʱ����


	//��1����ʼ��
	v_sField=v_iType?"PDCP.UpOctDl":"PDCP.UpOctUl";						//�ֶ�����
	v_sValue.Format("%12.4f",v_dDate_Select[v_iTime].v_dValues*1024*1024/1000);	//��ֵ(�����㹫ʽ)
	v_sTime=v_dDate_Select[v_iTime].v_dDate.Format("%Y%m%d");			//����
	v_sTemp.Format("%02d:00:00",v_dDate_Select[v_iTime].v_iHour);		//Сʱ
	v_sTime+=" "+v_sTemp;												//����ʱ��
	v_sCal=(v_dDate_Select[v_iTime].v_iCal==0)?"<":((v_dDate_Select[v_iTime].v_iCal==1)?">":"=");	//�ȽϷ�

	//��2������� ���ӵڶ��β�ѯ��ɾ����ǰ�β�ƥ���С������
	if (v_bCell)														//����һ�β�ѯ
	{
		for (v_pPM_Cell=v_mAlarm_LL_Cell.begin();v_pPM_Cell!=v_mAlarm_LL_Cell.end();v_pPM_Cell++)	//������С��
			v_pPM_Cell->second.v_bFlag=false;							//��ʼ��
	}

	//��3���������ݻ�ȡ
	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(10*60);							//��ʱ����(10����)

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zTime,["+v_sField+"] from "+v_sTable+" where zTime='"+v_sTime+"' and ["+v_sField+"]"+v_sCal+v_sValue;		//SQL

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_iCell_ID=Var.intVal;									//��ȡ��Cell_ID
				v_pIter_Cell=v_mCell_ID.find(v_iCell_ID);				//���ң�С��ID���ڣ�����
				if (v_pIter_Cell==v_mCell_ID.end())						//������
				{
					v_pRecordset->MoveNext();							//��һС������
					continue;											//����
				}
				v_sPM_Cell=v_pIter_Cell->second.v_sCell;				//С��
				v_sOnline=v_pIter_Cell->second.v_sOnline;				//״̬
				v_sRegion=v_pIter_Cell->second.v_sRegion;				//Ӫҵ��
			}
			else
				v_sPM_Cell="";

			Var=v_pRecordset->GetCollect((_variant_t)v_sField);			//��ȡ��������ֵ
			if (Var.vt != VT_NULL)
			{
				v_dValue=Var.dblVal;
				v_dValue=v_dValue*1000/1024/1024;						//���������㹫ʽ
			}
			else
				v_dValue=0;

			if (!v_bCell)												//��һ�β�ѯ������
			{															//����
				v_pPM_Cell=v_mAlarm_LL_Cell.find(v_sPM_Cell);			//���ң�С�� ������
				if (v_pPM_Cell==v_mAlarm_LL_Cell.end())					//������
				{
					v_cPM_DATA.v_sOnline=v_sOnline;						//״̬
					v_cPM_DATA.v_sRegion=v_sRegion;						//����
					if (v_iType)										//����
						v_cPM_DATA.v_dD_Values[v_iTime]=v_dValue;		//����
					else												//����
						v_cPM_DATA.v_dU_Values[v_iTime]=v_dValue;		//����

					v_mAlarm_LL_Cell.insert(pair<CString,Alarm_LL_PM_DATA>(v_sPM_Cell,v_cPM_DATA));	//��������
				}
			}	
			else
			{
				v_pPM_Cell=v_mAlarm_LL_Cell.find(v_sPM_Cell);			//���ң�С�� ������
				if (v_pPM_Cell!=v_mAlarm_LL_Cell.end())					//����
				{
					v_pPM_Cell->second.v_bFlag=true;					//���ñ�־����ѯƥ��
					if (v_iType)										//����
						v_pPM_Cell->second.v_dD_Values[v_iTime]=v_dValue;		//����
					else												//����
						v_pPM_Cell->second.v_dU_Values[v_iTime]=v_dValue;		//����
				}
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

	//��4��ɾ����ƥ���С�� ���ӵڶ��β�ѯ��ɾ����ǰ�β�ƥ���С������
	if (v_bCell)														//����һ�β�ѯ
	{
		v_pPM_Cell=v_mAlarm_LL_Cell.begin();							//ͷָ��
		while(v_pPM_Cell!=v_mAlarm_LL_Cell.end())						//������С��
		{
			if (!v_pPM_Cell->second.v_bFlag)							//ûƥ�䣿����
				v_pPM_Cell=v_mAlarm_LL_Cell.erase(v_pPM_Cell);			//ɾ��С����������һָ��
			else
				v_pPM_Cell++;											//ָ��++
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ�����
//	��	  ����1��1-5��	2��6-10��	3��11-15��	4��16-20��	5��21-25��	6��26--
//------------------------------------------------------------------------------------------------------			
CString CAlarm_LL_Cell::My_Get_Table_Num(int v_iDay)
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
//	�������ܣ�װ�����ܲ�����Ϣ
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Cell::My_LoadData_Para()
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


		//��1����ȡ���������ݱ����ơ����ڷ�����Ч�����ơ�
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_LTE_%') order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//������

			v_mTable_Name.insert(v_sTable);								//���Ӽ���

			v_pRecordset->MoveNext();									//ָ��++
		}	

		//��2����ȡ��С����Ϣ������С��ID����
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zCell_ID,zCell_Name,LTE_Cell.zOnline,LTE_Cell.zRegion \
				from LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB";		//��ѯ���ݿ�
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sCell=v_cAdoConn.GetField_String("zCell");				//zCell
			v_iCell_ID=v_cAdoConn.GetField_Int("zCell_ID");				//zCell_ID
			v_sCell_ID.Format("%d",v_iCell_ID);							//�ַ���
			v_sCell_Name=v_cAdoConn.GetField_String("zCell_Name");		//zCell_Name
			v_sOnline=v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_sRegion=v_cAdoConn.GetField_String("zRegion");			//zRegion

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
