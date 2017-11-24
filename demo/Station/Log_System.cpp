// Log_System.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Log_System.h"

// CLog_System

IMPLEMENT_DYNCREATE(CLog_System, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CLog_System::CLog_System()
	: CFormView(CLog_System::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CLog_System::~CLog_System()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CLog_System::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_LOG_SUM, m_cLog_Sum);
	DDX_Control(pDX, IDC_BEG_DATE, m_cLog_Date);
	DDX_Control(pDX, IDC_COMBO1, m_cLog_Day);
	DDX_Control(pDX, IDC_KEY, m_cLog_Key);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLog_System, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CLog_System::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL, &CLog_System::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CLog_System::OnLvnColumnclickList1)
END_MESSAGE_MAP()


// CLog_System ���

#ifdef _DEBUG
void CLog_System::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLog_System::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLog_System ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CLog_System::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="ϵͳ��־��ѯ";										//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText("ϵͳ��־��ѯ");					//�Ӵ���������

	//��ʼ�����б�KPI
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"��    ��",LVCFMT_CENTER,140);  
	m_cList.InsertColumn(2,"����",LVCFMT_LEFT,160);  
	m_cList.InsertColumn(3,"��־����",LVCFMT_LEFT,1000);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//������ʼ��
	m_cLog_Day.SetCurSel(4);											//Ĭ�ϣ���ѯ����
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ӵ�����
//------------------------------------------------------------------------------------------------------
void CLog_System::OnDestroy()
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
void CLog_System::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CWnd	*pCellList;													//��ʱ����
	int		v_iY;														//��ʱ����

	pCellList = GetDlgItem(IDC_LIST1);
	v_iY = 50;															//��ֵ��Y��
	if(pCellList)
		pCellList->MoveWindow(14,v_iY,cx-24,cy-v_iY-10);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CLog_System::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ������ѯ��ť
//------------------------------------------------------------------------------------------------------
void CLog_System::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);													//��ȡ��Ļ��Ϣ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Excel
//------------------------------------------------------------------------------------------------------
void CLog_System::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Log�б�����(�һ��б�������)
//------------------------------------------------------------------------------------------------------			
void CLog_System::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CLog_System::My_Thread_Inoput(LPVOID lparam)
{
	CLog_System *  lp_this = NULL ;
	lp_this = (CLog_System *)lparam;

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
void CLog_System::My_Input_Main()
{
	m_cList.DeleteAllItems();											//����б�����
	My_LoadData();														//װ��Log��Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��Log��Ϣ
//------------------------------------------------------------------------------------------------------			
void CLog_System::My_LoadData()
{
	CString		v_sSql;													//��ʱ����
	int			v_iItem,v_iID=0;										//KPI����
	CString		v_sKPI_Name;											//
	CString		v_sTemp;												//��ʱ����
	COleDateTime	v_dDate_Start,v_dDate_End;							//��ѯ��ʼ���ڡ���������
	int				v_iDay_Sum;											//��ѯ����
	CString		v_sTime_Start,v_sTime_End;								//����
	CString		v_sLog_Content,v_sLog_Type;								//��־����
	CString		v_sLog_Key;												//��־��ѯ��KEY

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		//��ѯ����׼��
		m_cLog_Date.GetTime(v_dDate_Start);								//��ã���ѯ��ʼ����
		v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//����ʱ�䣺00:00:00
		v_iDay_Sum=m_cLog_Day.GetCurSel()+1;							//��ã���ѯ����
		v_dDate_End =v_dDate_Start + COleDateTimeSpan(v_iDay_Sum,0,0,0);//��ã���ѯ��������
		v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";		//��ʼʱ��
		v_sTime_End=v_dDate_End.Format("%Y-%m-%d")+" 00:00:00";			//����ʱ��
		m_cLog_Key.GetWindowText(v_sLog_Key);							//��ã���ѯKEY
		v_sLog_Key.Trim();												//ȥ���ո�

		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		if (v_sLog_Key.IsEmpty())										//�йؼ��ֲ�ѯ������
			v_sSql="select * from BAS_Log where zTime>='"+v_sTime_Start+"' and zTime<='"+v_sTime_End+"' order by zTime";	//��ѯ���ݿ⣺ϵͳ��־
		else
			v_sSql="select * from BAS_Log where zTime>='"+v_sTime_Start+"' and zTime<='"+v_sTime_End+"' and zLog_Content like '%"+v_sLog_Key+"%' order by zTime";	//��ѯ���ݿ⣺ϵͳ��־

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_dDate_Start=v_cAdoConn.GetField_DataTime("zTime");		//zTime
			v_sLog_Type=v_cAdoConn.GetField_String("zLog_Type");		//zLog_Type
			v_sLog_Content=v_cAdoConn.GetField_String("zLog_Content");	//zLog_Content

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d_%H:%M:%S");	//��ֵ������
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sTime_Start);		//
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sLog_Type);		//
			m_cList.SetItemText(v_iItem,3,(_bstr_t)v_sLog_Content);		//

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
//	END
//------------------------------------------------------------------------------------------------------
