// ConLTEBusyIndicator.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "ConLTEBusyIndicator.h"


// CConLTEBusyIndicator

IMPLEMENT_DYNCREATE(CConLTEBusyIndicator, CFormView)

CConLTEBusyIndicator::CConLTEBusyIndicator()
	: CFormView(CConLTEBusyIndicator::IDD)
{

}

CConLTEBusyIndicator::~CConLTEBusyIndicator()
{
}

void CConLTEBusyIndicator::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_cList);
	DDX_Control(pDX, IDC_KEY, m_cFile_Name);
	DDX_Control(pDX, IDC_BUTTON_FILE_SELECT, m_cFile);
	DDX_Control(pDX, IDC_BUTTON_INPUT_DATABASE, m_cInput);
	DDX_Control(pDX, IDC_BUTTON_OUTPUT_DATABASE, m_cOutput);
	DDX_Control(pDX, IDC_BUTTON_READ_CSV, m_cFileRead);
}

BEGIN_MESSAGE_MAP(CConLTEBusyIndicator, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SELECT, &CConLTEBusyIndicator::OnBnClickedButtonFileSelect)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_DATABASE, &CConLTEBusyIndicator::OnBnClickedButtonInputDatabase)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_DATABASE, &CConLTEBusyIndicator::OnBnClickedButtonOutputDatabase)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_READ_CSV, &CConLTEBusyIndicator::OnBnClickedButtonReadCsv)
END_MESSAGE_MAP()


// CConLTEBusyIndicator ���

#ifdef _DEBUG
void CConLTEBusyIndicator::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CConLTEBusyIndicator::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CConLTEBusyIndicator ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="LTEæʱָ��";											//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������
	//��ʼ�����б� [С��]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,50);					
	m_cList.InsertColumn(1,"С��",LVCFMT_LEFT,100);					
	m_cList.InsertColumn(2,"Ӫҵ��",LVCFMT_LEFT,80);					
	m_cList.InsertColumn(3,"״̬",LVCFMT_LEFT,80);						
	m_cList.InsertColumn(4,"������",LVCFMT_LEFT,280);	
	m_cList.InsertColumn(5,"����������",LVCFMT_CENTER,80);		
	m_cList.InsertColumn(6,"С���û��������ֽ���",LVCFMT_CENTER,150);					
	m_cList.InsertColumn(7,"С���û��������ֽ���",LVCFMT_RIGHT,150);					
	m_cList.InsertColumn(8,"��ЧRRC����ƽ����",LVCFMT_CENTER,150);				
	m_cList.InsertColumn(9,"E-RAB�����ɹ���",LVCFMT_LEFT,120);						
	m_cList.InsertColumn(10,"����PDSCHPRB����ƽ����",LVCFMT_CENTER,150);					
	m_cList.InsertColumn(11,"����PDSCHPRBռ��ƽ����",LVCFMT_RIGHT,150);					
	m_cList.InsertColumn(12,"����PUSCHPRB����ƽ����",LVCFMT_CENTER,150);				
	m_cList.InsertColumn(13,"����PUSCHPRBռ��ƽ����",LVCFMT_CENTER,150);				
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CConLTEBusyIndicator::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ�SQLִ�к���
//	��    ����v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CConLTEBusyIndicator::My_ExecuteSQL(CString v_sSql)
{
	// TODO: Add your control notification handler code here
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(1000*200);
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
//	�������ܣ�List��ʾcsvָ��
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::ShowList()
{
	CMyExcel csv_read;
	CString temp;
	m_cFile_Name.GetWindowText(temp);
	csv_read.Get_CSV(temp,&m_cList,false);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------		
//�������ܣ�ִ��bulkinsert���
//��    ����url:�ļ�·����table_suffix�����׺,control_count :��������������
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::ExcecuteBulkinsert(CString url,CString table_suffix,int control_count)
{
	CString sql,count;
	bool flag;
	count.Format("%d",control_count);
	sql="bulk insert LTE_Load_Metric" + table_suffix + " FROM '"+ url +  "' WITH ( FIELDTERMINATOR =',', FIRSTROW = 2 , BATCHSIZE = " + count + ")";
	flag = My_ExecuteSQL(sql);
	if (flag)
	{
		MessageBox("�����������");
	} 
	else
	{
		MessageBox("��������ʧ��");
	}	
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������ݵ���
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::OutputCellBusyIndicator()
{
	CString		v_sTemp;												//��ʱ����
	CFileDialog *filedialog;											//ѡ���ļ��Ի���ָ��
	filedialog = new CFileDialog(false,"csv","Excel.csv",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"(*.csv;*.txt;*.xlsx)|*.csv;*.txt;*.xlsx||");				//ʵ����
	if(filedialog->DoModal()==IDOK)										//��ѡ���ļ��Ի���
	{
		v_sFilename = filedialog->GetPathName();						//��ã�ѡ����ļ�����
	}
	else
	{
		return;
	}
	CString sql,cell_name,before_cell_name(""),file_write,temp;
	_RecordsetPtr v_pRecordset;											//��¼��
	int count(0);
	float	UpOctDl(0),temp_UpOctDl(0),UpOctUl(0),temp_UpOctUl(0),wireless_Utility(0),temp_wireless_Utility(0);
	float   temp_EffectiveConnMean(0),EffectiveConnMean(0),PdschPrbMeanTot(0),temp_PdschPrbMeanTot(0),PdschPrbTotMeanDl(0),temp_PdschPrbTotMeanDl(0);	
	float   PuschPrbMeanTot(0),temp_PuschPrbMeanTot(0),PuschPrbTotMeanUl(0),temp_PuschPrbTotMeanUl(0);	
	CMyADOConn		v_cAdoConn;											//���ݿ�ʵ��
	Busy_Indicator	indicator;                                          //ָ��
	CFile			Pm_file;                                           //�ļ�ʵ�� 
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(1000*200);
		sql="select * from"
			"(select * from [Station].[dbo].[LTE_Load_Metric]) as t1"
			" right join"
			"(select [С��] as [С��_1]"
			",max([����������]) as [����������_1]" 
			",convert(varchar(10),[����],120) as [����_1]"
			" from [Station].[dbo].[LTE_Load_Metric] group by [С��]"
			",convert(varchar(10),[����],120)) as t2"
			" on (t1.[����������] = t2.[����������_1] and t1.[С��] = t2.[С��_1]) where t1.[����������]!=0 order by t2.[С��_1]";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)sql);								//ִ��SELETE���					
		while(!v_pRecordset->adoEOF)
		{
			cell_name = v_cAdoConn.GetField_String("С��");									//С������
			temp_wireless_Utility = v_cAdoConn.GetField_Double("����������");					//С������������
			temp_UpOctDl = v_cAdoConn.GetField_Double("С���û��������ֽ���");			//С���û��������ֽ���
			temp_UpOctUl = v_cAdoConn.GetField_Double("С���û��������ֽ���");			//С���û��������ֽ���
			temp_EffectiveConnMean = v_cAdoConn.GetField_Double("��ЧRRC����ƽ����");		//��ЧRRC����ƽ����
			temp_PdschPrbMeanTot = v_cAdoConn.GetField_Double("����PDSCHPRB����ƽ����");	//����PDSCHPRB����ƽ����
			temp_PdschPrbTotMeanDl = v_cAdoConn.GetField_Double("����PDSCHPRBռ��ƽ����");	//����PDSCHPRBռ��ƽ����
			temp_PuschPrbMeanTot = v_cAdoConn.GetField_Double("����PUSCHPRB����ƽ����");	//����PUSCHPRB����ƽ����
			temp_PuschPrbTotMeanUl = v_cAdoConn.GetField_Double("����PUSCHPRBռ��ƽ����");	//����PUSCHPRBռ��ƽ����
			v_pRecordset->MoveNext();														
			if (cell_name == before_cell_name) {
				before_cell_name = cell_name;
				wireless_Utility += temp_wireless_Utility;
				UpOctDl += temp_UpOctDl;
				UpOctUl += temp_UpOctUl;
				EffectiveConnMean += temp_EffectiveConnMean;
				PdschPrbMeanTot += temp_PdschPrbMeanTot;
				PdschPrbTotMeanDl += temp_PdschPrbTotMeanDl;
				PuschPrbMeanTot += temp_PuschPrbMeanTot;
				PuschPrbTotMeanUl += temp_PuschPrbTotMeanUl;
				count ++;
			}
			else if(before_cell_name =="") {
				before_cell_name = cell_name;
				wireless_Utility += temp_wireless_Utility;
				UpOctDl += temp_UpOctDl;
				UpOctUl += temp_UpOctUl;
				EffectiveConnMean += temp_EffectiveConnMean;		
				PdschPrbMeanTot += temp_PdschPrbMeanTot;
				PdschPrbTotMeanDl += temp_PdschPrbTotMeanDl;
				PuschPrbMeanTot += temp_PuschPrbMeanTot;
				PuschPrbTotMeanUl += temp_PuschPrbTotMeanUl;
				count ++;
			}
			else{
				wireless_Utility = wireless_Utility / count;
				UpOctDl = UpOctDl / count;
				UpOctUl = UpOctUl / count;
				EffectiveConnMean = EffectiveConnMean / count;		
				PdschPrbMeanTot = PdschPrbMeanTot / count;	
				PdschPrbTotMeanDl = PdschPrbTotMeanDl / count;
				PuschPrbMeanTot = PuschPrbMeanTot / count;	
				PuschPrbTotMeanUl = PuschPrbTotMeanUl / count;
				indicator.cellname = before_cell_name;
				indicator.wireless_Utility = wireless_Utility;
				indicator.UpOctDl = UpOctDl;
				indicator.UpOctUl = UpOctUl;
				indicator.EffectiveConnMean = EffectiveConnMean;		
				indicator.PdschPrbMeanTot = PdschPrbMeanTot;
				indicator. PdschPrbTotMeanDl =  PdschPrbTotMeanDl;
				indicator.PuschPrbMeanTot = PuschPrbMeanTot;
				indicator. PuschPrbTotMeanUl =  PuschPrbTotMeanUl;
				busy_indicator_.push_back(indicator);
				count = 1;
				before_cell_name = cell_name;
				wireless_Utility = temp_wireless_Utility;
				UpOctDl = temp_UpOctDl;
				UpOctUl = temp_UpOctUl;
				EffectiveConnMean = temp_EffectiveConnMean;
				PdschPrbMeanTot = temp_PdschPrbMeanTot;
				PdschPrbTotMeanDl =  temp_PdschPrbTotMeanDl;
				PuschPrbMeanTot = temp_PuschPrbMeanTot;
				PuschPrbTotMeanUl =  temp_PuschPrbTotMeanUl;
			}
		}
		Pm_file.Open(v_sFilename,CFile::modeWrite|CFile::modeRead|CFile::modeCreate);
		while(busy_indicator_.size() > 0){
			indicator = busy_indicator_.front();
			file_write = indicator.cellname;
			temp.Format("%f",indicator.wireless_Utility);
			file_write += "," + temp;
			temp.Format("%f",indicator.UpOctDl);
			file_write += "," + temp;
			temp.Format("%f",indicator.UpOctUl);
			file_write += "," + temp;
			temp.Format("%f",indicator.EffectiveConnMean);
			file_write += "," + temp;
			temp.Format("%f",indicator.PdschPrbMeanTot);
			file_write += "," + temp;
			temp.Format("%f",indicator.PdschPrbTotMeanDl);
			file_write += "," + temp;
			temp.Format("%f",indicator.PuschPrbMeanTot);
			file_write += "," + temp;
			temp.Format("%f",indicator.PuschPrbTotMeanUl);
			file_write += "," + temp;
			file_write += "\r\n";
			Pm_file.Write(file_write,file_write.GetLength());
			busy_indicator_.pop_front();
		}
		//�ر��ļ�
		Pm_file.Close();
		MessageBox("�ļ������ɹ�");
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�ر����ݿ�

		CString v_sError = (char *)e.Description();						//��ã�������Ϣ
		MessageBox(v_sError);											//ʧ��
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������ݵ���
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::My_Input_Cell()
{
	//ָ�굼�����ݿ�
	CString temp;
	m_cFile_Name.GetWindowText(temp);
	ExcecuteBulkinsert(temp,"",3000);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡCSV�ļ���Ϣ
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::My_File()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp;												//��ʱ����
	mString_Int	v_mMap;													//������CSV�ļ������ӳ����ʾ�����

	CFileDialog *filedialog;											//ѡ���ļ��Ի���ָ��

	filedialog = new CFileDialog(true,(LPCTSTR)"csv");					//ʵ����
	if(filedialog->DoModal()==IDOK)										//��ѡ���ļ��Ի���
	{
		v_sFilename = filedialog->GetPathName();						//��ã�ѡ����ļ�����
		m_cFile_Name.SetWindowText(v_sFilename);						//���֣�ѡ�е��ļ�����
	}
	delete(filedialog);
	v_mMap.clear();														//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 0:
		My_File();														//�ļ�ѡ��
		break;
	case 1:
		My_Input_Cell();												//Cell����С��
		break;
	case 2:
		OutputCellBusyIndicator();										//CellС������
		break;
	case 3:
		ShowList();														//CellС��ָ����ʾ
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CConLTEBusyIndicator::My_Thread_Inoput(LPVOID lparam)
{
	CConLTEBusyIndicator *  lp_this = NULL ;
	lp_this = (CConLTEBusyIndicator *)lparam;

	lp_this->m_cFile.EnableWindow(false);								//�ļ�ѡ����Ч
	lp_this->m_cInput.EnableWindow(false);								//��ʼ���룺��Ч
	lp_this->m_cOutput.EnableWindow(false);								//��ʼ��������Ч
	lp_this->m_cFileRead.EnableWindow(false);	
	lp_this->My_Input_Main();											//����_������
	lp_this->m_cFile.EnableWindow(true);								//�ļ�ѡ����Ч
	lp_this->m_cInput.EnableWindow(true);								//��ʼ���룺��Ч
	lp_this->m_cOutput.EnableWindow(true);								//��ʼ��������Ч
	lp_this->m_cFileRead.EnableWindow(true);
	lp_this->v_iThread_Exit=0;											//�̣߳��˳�

	return 0;
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡCSV�ļ���Ϣ
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::OnBnClickedButtonFileSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=0;														//�������񣺻�Ϊ�ļ�ѡ��
	v_iThread_Exit=1;													//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

void CConLTEBusyIndicator::OnBnClickedButtonInputDatabase()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺻�Ϊ���ݵ���
	v_iThread_Exit=1;													//�߳̿�������Ч
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//����
}

void CConLTEBusyIndicator::OnBnClickedButtonOutputDatabase()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=2;														//�������񣺻�Ϊ���ݵ���
	v_iThread_Exit=1;													//�߳̿�������Ч
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//����
}

void CConLTEBusyIndicator::OnBnClickedButtonReadCsv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=3;														//�������񣺻�Ϊ���ݵ���
	v_iThread_Exit=1;													//�߳̿�������Ч
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//����
}

void CConLTEBusyIndicator::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	mFrame_Window::iterator	v_pIterator_Frame;							//������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//�����Ӵ�����
	if(v_pIterator_Frame != theApp.v_mFrame.end())						//�Ӵ����ڣ�
		theApp.v_mFrame.erase(v_pIterator_Frame);						//������������Ӵ�����
}

void CConLTEBusyIndicator::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
	CWnd	*pCellList;													//��ʱ����
	int		v_iY;														//��ʱ����
	pCellList = GetDlgItem(IDC_LIST);
	v_iY = 50;															//��ֵ��Y��
	if(pCellList)
	{
		pCellList->MoveWindow(14,v_iY,cx-24,cy-v_iY-10);
	}		
}


