// Log_DataBase_GSM.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Log_DataBase_GSM.h"


// CLog_DataBase_GSM

IMPLEMENT_DYNCREATE(CLog_DataBase_GSM, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CLog_DataBase_GSM::CLog_DataBase_GSM()
	: CFormView(CLog_DataBase_GSM::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CLog_DataBase_GSM::~CLog_DataBase_GSM()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CLog_DataBase_GSM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_cList);
	DDX_Control(pDX, IDC_STATION_SUM, m_cDataBase);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLog_DataBase_GSM, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, &CLog_DataBase_GSM::OnLvnColumnclickList2)
	ON_BN_CLICKED(IDC_EXCEL, &CLog_DataBase_GSM::OnBnClickedExcel)
END_MESSAGE_MAP()


// CLog_DataBase_GSM ���

#ifdef _DEBUG
void CLog_DataBase_GSM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLog_DataBase_GSM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLog_DataBase_GSM ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CLog_DataBase_GSM::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="���ݿ�����_GSM";										//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ����Ԥ���б�
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0," ���",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"������",LVCFMT_LEFT,180);  
	m_cList.InsertColumn(2,"����",LVCFMT_CENTER,80);  
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
void CLog_DataBase_GSM::OnDestroy()
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
void CLog_DataBase_GSM::OnSize(UINT nType, int cx, int cy)
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
BOOL CLog_DataBase_GSM::PreTranslateMessage(MSG* pMsg)
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
void CLog_DataBase_GSM::My_LoadData()
{
	CString v_sSql;														//��ʱ����
	CString v_sTemp,v_sTable,v_sType;									//��ʱ����
	int		v_iType;													//��ʱ����

	mString_String	v_mObjType;											//������ObjType
	mString_String	v_mPM_Table;										//������ObjType
	mString_String::iterator	v_pIter;								//������

	//-----------------------------------------------------------------------------------------------------
	//���ݿ�������
	My_LoadSize_Table("",1);											//���ݿ�������ѯ����ʾ��1λ��
	v_sTemp.Format("%4.2f",(float)v_cData_Size.v_iReserved/1024);		//������λת����MB--��G
	m_cDataBase.SetWindowTextA(v_sTemp+" G");							//��ʾ�����ݿ�����

	//-----------------------------------------------------------------------------------------------------
	//GSM
	My_LoadSize("A_BSC_Info","BSC��Ϣ��","����",0);						//��ȡ������������
	My_LoadSize("A_CELL_Info","Cell��Ϣ��","����",0);					//��ȡ������������

	//��������
	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��ȡ��ObjType������
		v_sSql="select * from A_OBJTYPE_Info";							//����Objtype��Ϣ
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("zobjtype");			//��ȡ��zobjtype
			v_iType=v_cAdoConn.GetField_Int("ztype");					//��ȡ������
			v_sType=(v_iType==1?"BSC":(v_iType==2?"CELL":v_iType==3?"HANGOVER":"Others"));

			v_mObjType.insert(pair<CString,CString>(v_sTable,v_sType));	//��������

			v_pRecordset->MoveNext();									//ָ������ƶ�
		}

		//��ȡ���������ܱ�����
		v_sSql="select * from A_CAPGROUP_Info";							//���ؼ������ܱ���Ϣ
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("zgroupname");		//��ȡ�����ܱ�����
			v_sType = v_cAdoConn.GetField_String("ztypename");			//��ȡ����������

			v_mPM_Table.insert(pair<CString,CString>(v_sTable,v_sType));//��������

			v_pRecordset->MoveNext();									//ָ������ƶ�
		}

		v_cAdoConn.ExitConnect();										//�Ͽ�����

		//������
		for(v_pIter=v_mObjType.begin();v_pIter!=v_mObjType.end();v_pIter++)
		{
			My_LoadSize(v_pIter->first,v_pIter->first,v_pIter->second,2);	//��ȡ������������		
		}
		for(v_pIter=v_mPM_Table.begin();v_pIter!=v_mPM_Table.end();v_pIter++)
		{
			My_LoadSize(v_pIter->first,v_pIter->first,v_pIter->second,2);	//��ȡ������������		
		}
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	v_mObjType.clear();													//�������
	v_mPM_Table.clear();												//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ�ر�������Ϣ [������]
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase_GSM::My_LoadSize(CString v_sTable,CString v_sAlarm_Name,CString v_sType,int v_iType)
{
	My_Data_Reset();													//������λ
	My_LoadSize_Table(v_sTable,0);										//��ȡ�������ƣ�����ţ�0 [��������ʾλ����0]
	My_LoadSize_Show(v_sAlarm_Name,v_sType,v_iType,1);					//���ͣ�����/ʵʱ����������1
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ��������Ϣ
//	��	  ����v_sTable�������ƣ�[��ѯ�ı����ƣ���Ϊ�գ����ʾ��ѯ�������ݿ������]	
//			  v_iNum������� [��Ӧ���б�����ʾ��λ�ã���0--9��ͨ����������1λ�ã�ʵʱ��Ϊ0-9λ�ã���̬��Ϊ0-5λ��]
//	��	  �أ���ѯ���ݱ����ڽṹv_cData_Size�С�[�������������������������������������м�¼����]
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase_GSM::My_LoadSize_Table(CString v_sTable,int v_iNum)
{
	CString v_sSql;														//
	CString v_sTemp;													//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		//KPIʵʱ����̬����
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);
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
void CLog_DataBase_GSM::My_LoadSize_Show(CString v_sTable,CString v_sType,int v_iType,int v_iSum)
{
	CString v_sTemp;													//��ʱ����
	int		v_iItem;													//��ʱ����

	v_sTemp.Format(" %04d",m_cList.GetItemCount()+1);
	v_iItem=m_cList.InsertItem(0xffff,v_sTemp);							//�����б�
	m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sTable);					//
	m_cList.SetItemText(v_iItem,2,(_bstr_t)(v_sType));					//ʵʱ����Ϣ
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
	m_cList.SetItemBkColor(v_iItem,v_iType==2?14805192:(v_iType==0?15647177:16647177));			//����ɫ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ݸ�λ
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase_GSM::My_Data_Reset()
{
	v_cData_Size.v_iReserved=0;											//������
	v_cData_Size.v_iData=0;												//��������
	v_cData_Size.v_iIndex_size=0;										//��������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����Ҹ�ʽ��
//------------------------------------------------------------------------------------------------------			
CString CLog_DataBase_GSM::My_Format(CString v_sTemp)
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
void CLog_DataBase_GSM::OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Excel
//------------------------------------------------------------------------------------------------------
void CLog_DataBase_GSM::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
