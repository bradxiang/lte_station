// Log_DataBase.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Log_DataBase.h"


// CLog_DataBase

IMPLEMENT_DYNCREATE(CLog_DataBase, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CLog_DataBase::CLog_DataBase()
	: CFormView(CLog_DataBase::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CLog_DataBase::~CLog_DataBase()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CLog_DataBase::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_cList);
	DDX_Control(pDX, IDC_STATION_SUM, m_cDataBase);
	DDX_Control(pDX, IDC_STATION_SUM3, m_cSize_DB);
	DDX_Control(pDX, IDC_STATION_SUM4, m_cSize_Log);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLog_DataBase, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, &CLog_DataBase::OnLvnColumnclickList2)
	ON_BN_CLICKED(IDC_EXCEL, &CLog_DataBase::OnBnClickedExcel)
END_MESSAGE_MAP()


// CLog_DataBase ���

#ifdef _DEBUG
void CLog_DataBase::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLog_DataBase::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLog_DataBase ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CLog_DataBase::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="���ݿ�����_LTE";										//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ����Ԥ���б�
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0," ���",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"������",LVCFMT_LEFT,300);  
	m_cList.InsertColumn(2,"����",LVCFMT_CENTER,60);  
	m_cList.InsertColumn(3,"��¼��",LVCFMT_RIGHT,70);  
	m_cList.InsertColumn(4,"ռ������",LVCFMT_RIGHT,70);  
	m_cList.InsertColumn(5,"��������",LVCFMT_RIGHT,70);  
	m_cList.InsertColumn(6,"��������",LVCFMT_RIGHT,70);  
	m_cList.InsertColumn(7,"��������",LVCFMT_RIGHT,60);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	for (int vi=3;vi<8;vi++)
		m_cList.SetNumber(vi);											//����������Ч����Ӧ����

	//��Ϣ��ʾ
	My_LoadData();														//���֣����ݿ�������Ϣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ӵ�����
//------------------------------------------------------------------------------------------------------
void CLog_DataBase::OnDestroy()
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
void CLog_DataBase::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CWnd	*v_pWnd;													//�����ؼ���ָ��

	int	v_iX0=11;														//X��λ��
	int	v_iY0=45;														//Y��λ��

	v_pWnd = GetDlgItem(IDC_LIST2);										//Ԥ���б�
	if(v_pWnd)
		v_pWnd->MoveWindow(v_iX0,v_iY0,cx-v_iX0-10,cy-v_iY0-10);		//���߶�̬����
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CLog_DataBase::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ�װ�����ݿ�������Ϣ
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase::My_LoadData()
{
	CString v_sSql;														//��ʱ����
	CString v_sTemp,v_sTable;											//��ʱ����

	//-----------------------------------------------------------------------------------------------------
	//����ʣ������
	My_LoadSize_Disk();													//����ʣ������

	//-----------------------------------------------------------------------------------------------------
	//���ݿ�������
	My_LoadSize_Table("",1);											//���ݿ�������ѯ����ʾ��1λ��
	v_sTemp.Format("%4.2f",(float)v_cData_Size.v_iReserved/1024);		//������λת����MB--��G
	m_cDataBase.SetWindowTextA(v_sTemp+" G");							//��ʾ�����ݿ�����

	//-----------------------------------------------------------------------------------------------------
	//վַ
	My_LoadSize("CON_Station","վַ��Ϣ��",0);							//��ȡ������������
	//GSM
	My_LoadSize("GSM_BTS","GSM ��վ��Ϣ��",0);							//��ȡ������������
	My_LoadSize("GSM_Cell","GSM Cell��Ϣ��",0);							//��ȡ������������
	//TD-LTE
	My_LoadSize("LTE_NODEB","TD-LTE ��վ��Ϣ��",0);						//��ȡ������������
	My_LoadSize("LTE_Cell","TD-LTE Cell��Ϣ��",0);						//��ȡ������������)
	//��������
	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_%' or name like 'Xm_%') order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//������

			if (v_sTable.Find("Pm")>=0)
				My_LoadSize(v_sTable,v_sTable+"_���ܱ�",2);				//��ȡ������������
			else
				My_LoadSize(v_sTable,v_sTable+"_���ñ�",2);				//��ȡ������������

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
//	�������ܣ�װ�ر�������Ϣ [������]
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase::My_LoadSize(CString v_sTable,CString v_sAlarm_Name,int v_iType)
{
	My_Data_Reset();													//������λ
	My_LoadSize_Table(v_sTable,0);										//��ȡ�������ƣ�����ţ�0 [��������ʾλ����0]
	My_LoadSize_Show(v_sAlarm_Name,v_iType,1);							//���ͣ�����/ʵʱ����������1
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ʣ������
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase::My_LoadSize_Disk()
{
	CString v_sSql;														//��ʱ����
	CString v_sTemp;													//��ʱ����
	double	v_dSize_DB,v_dSize_Log;										//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		//KPIʵʱ����̬����
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from CON_System";								//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		if(!v_pRecordset->adoEOF)
		{
			v_dSize_DB=v_cAdoConn.GetField_Double("zSize_DB");			//zSize_DB
			v_dSize_Log=v_cAdoConn.GetField_Double("zSize_Log");		//zSize_Log

			//��ʾ
			v_sTemp.Format("%8.2f G",v_dSize_DB);						//�ַ���
			m_cSize_DB.SetWindowText(v_sTemp);							//��ʾ
			v_sTemp.Format("%8.2f G",v_dSize_Log);						//�ַ���
			m_cSize_Log.SetWindowText(v_sTemp);							//��ʾ
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
//	�������ܣ���ѯ��������Ϣ
//	��	  ����v_sTable�������ƣ�[��ѯ�ı����ƣ���Ϊ�գ����ʾ��ѯ�������ݿ������]	
//			  v_iNum������� [��Ӧ���б�����ʾ��λ�ã���0--9��ͨ����������1λ�ã�ʵʱ��Ϊ0-9λ�ã���̬��Ϊ0-5λ��]
//	��	  �أ���ѯ���ݱ����ڽṹv_cData_Size�С�[�������������������������������������м�¼����]
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase::My_LoadSize_Table(CString v_sTable,int v_iNum)
{
	CString v_sSql;														//
	CString v_sTemp;													//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		//KPIʵʱ����̬����
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="sp_spaceused";											//SQL
		if (!v_sTable.IsEmpty())										//�����Ʋ�Ϊ�գ�
			v_sSql+=" '["+v_sTable+"]'";								//��ѯ������ 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		if(!v_pRecordset->adoEOF)
		{
			if (v_sTable.IsEmpty())										//������Ϊ�գ�
			{
				//�������ݿ�������ѯ
				v_sTemp=v_cAdoConn.GetField_String("database_size");	//database_size
				int vi=v_sTemp.Find("MB",0);
				v_cData_Size.v_iReserved=atoi(v_sTemp.Left(vi));		//��ֵת��
			}
			else
			{
				//������������ѯ
				v_sTemp=v_cAdoConn.GetField_String("rows");				//rows
				v_cData_Size.v_iRows[v_iNum]=atoi(v_sTemp);				//��ֵת�� [���ڱ���Ŷ�Ӧ��λ�ã��Ա���ʾ]
				v_sTemp=v_cAdoConn.GetField_String("reserved");			//reserved
				v_cData_Size.v_iReserved+=atoi(v_sTemp);				//��ֵת��
				v_sTemp=v_cAdoConn.GetField_String("data");				//data
				v_cData_Size.v_iData+=atoi(v_sTemp);					//��ֵת��
				v_sTemp=v_cAdoConn.GetField_String("index_size");		//index_size
				v_cData_Size.v_iIndex_size+=atoi(v_sTemp);				//��ֵת��
			}
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
//	�������ܣ���ʾ����������Ϣ��ӵ��б�  [�������¼����ʾ��1λ�ã���ʷ����1--12λ��]
//	��	  ����v_sTable�������ƣ�	
//			  v_iFlag��2��ʵʱ��	1����ʷ��	0��������	
//			  v_iSum����������[��ʷ��12�� ������1]
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase::My_LoadSize_Show(CString v_sTable,int v_iFlag,int v_iSum)
{
	CString v_sTemp;													//��ʱ����
	int		v_iItem;													//��ʱ����

	v_sTemp.Format(" %04d",m_cList.GetItemCount()+1);
	v_iItem=m_cList.InsertItem(0xffff,v_sTemp);							//�����б�
	m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sTable);					//
	m_cList.SetItemText(v_iItem,2,(_bstr_t)(v_iFlag==2?"ʵʱ":(v_iFlag==1?"��ʷ":"����")));	//ʵʱ����Ϣ
	for (int vi=0;vi<v_iSum;vi++)
	{
		v_sTemp.Format("%d",v_cData_Size.v_iRows[vi]);					//��¼��
		v_sTemp=My_Format(v_sTemp);										//���Ҹ�ʽ��
		m_cList.SetItemText(v_iItem,3+vi,(_bstr_t)v_sTemp);
	}
	v_sTemp.Format("%d",v_cData_Size.v_iReserved);						//ռ������
	v_sTemp=My_Format(v_sTemp);											//���Ҹ�ʽ��
	m_cList.SetItemText(v_iItem,4,(_bstr_t)v_sTemp);
	v_sTemp.Format("%d",v_cData_Size.v_iData);							//��������
	v_sTemp=My_Format(v_sTemp);											//���Ҹ�ʽ��
	m_cList.SetItemText(v_iItem,5,(_bstr_t)v_sTemp);
	v_sTemp.Format("%d",v_cData_Size.v_iIndex_size);					//��������
	v_sTemp=My_Format(v_sTemp);											//���Ҹ�ʽ��
	m_cList.SetItemText(v_iItem,6,(_bstr_t)v_sTemp);
	if ((v_cData_Size.v_iData+v_cData_Size.v_iIndex_size)!=0)
	{
		v_sTemp.Format("%4.2f",(float)v_cData_Size.v_iIndex_size*100/(float)(v_cData_Size.v_iData+v_cData_Size.v_iIndex_size));	//��������
		m_cList.SetItemText(v_iItem,7,(_bstr_t)v_sTemp);
	}
	m_cList.SetItemBkColor(v_iItem,v_iFlag==2?14805192:(v_iFlag==0?15647177:16647177));			//����ɫ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ݸ�λ
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase::My_Data_Reset()
{
	v_cData_Size.v_iReserved=0;											//������
	v_cData_Size.v_iData=0;												//��������
	v_cData_Size.v_iIndex_size=0;										//��������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����Ҹ�ʽ��
//------------------------------------------------------------------------------------------------------			
CString CLog_DataBase::My_Format(CString v_sTemp)
{
	int v_iLen=v_sTemp.GetLength();										//����
	while(v_iLen>3)														//����>3
	{
		v_sTemp.Insert(v_iLen-3,",");									//���룺","
		v_iLen-=3;														//
	}
	return v_sTemp;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����(�һ��б�������)
//------------------------------------------------------------------------------------------------------
void CLog_DataBase::OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Excel
//------------------------------------------------------------------------------------------------------
void CLog_DataBase::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
