// CON_Station_Input.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "CON_Station_Input.h"


// CCON_Station_Input �Ի���

IMPLEMENT_DYNAMIC(CCON_Station_Input, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CCON_Station_Input::CCON_Station_Input(CWnd* pParent /*=NULL*/)
	: CDialog(CCON_Station_Input::IDD, pParent)
{
	v_iThread_Exit=0;													//�̣߳�ֹͣ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CCON_Station_Input::~CCON_Station_Input()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cFile_Name);
	DDX_Control(pDX, IDC_BUTTON4, m_cFile);
	DDX_Control(pDX, IDC_BUTTON1, m_cInput);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_LIST3, m_cList_Bug);
	DDX_Control(pDX, IDC_STATION_SUM, m_cSum);
	DDX_Control(pDX, IDC_BUTTON6, m_cExcel);
	DDX_Control(pDX, IDC_CHECK2, m_cFile_Open);
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_Station_Input, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON4, &CCON_Station_Input::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CCON_Station_Input::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON6, &CCON_Station_Input::OnBnClickedButton6)
END_MESSAGE_MAP()


// CCON_Station_Input ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CCON_Station_Input::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ�����б� ��������
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"��������",LVCFMT_LEFT,300);					//
	m_cList.InsertColumn(2,"����վ��",LVCFMT_LEFT,300);					//
	m_cList.InsertColumn(3,"��������",LVCFMT_LEFT,120);					//
	m_cList.InsertColumn(4,"����",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(5,"״̬",LVCFMT_CENTER,80);					//
	m_cList.InsertColumn(6,"��������",LVCFMT_CENTER,100);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��ʼ�����б� [�쳣]
	::SendMessage(m_cList_Bug.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_cList_Bug.InsertColumn(0,(LPCTSTR)"���",LVCFMT_CENTER,40);		//Ϊ�б�ؼ������
	m_cList_Bug.InsertColumn(1,(LPCTSTR)"ʱ��",LVCFMT_LEFT,140);
	m_cList_Bug.InsertColumn(2,(LPCTSTR)"�쳣����",LVCFMT_LEFT,100);
	m_cList_Bug.InsertColumn(3,(LPCTSTR)"�쳣��Ϣ",LVCFMT_LEFT,600);
	m_cList_Bug.SetExtendedStyle(m_cList_Bug.GetExtendedStyle()|LVS_EX_GRIDLINES);

	//�ִ�
	CRect	v_cRect;													//����
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//��ã�ָ��
	pWnd->GetWindowRect(&v_cRect);										//��ã��ߴ�
	ScreenToClient(&v_cRect);											//����任
	pWnd->DestroyWindow();												//���٣�ָ��

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Main.Create(											//��ֱ��
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList,				// Top pane
		&m_cList_Bug,			// Bottom pane
		IDC_CON_Station_Input,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);

	//��ʾ
	ShowWindow(SW_MAXIMIZE);											//�Ի������

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CCON_Station_Input::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		CDialog* Pwnd=(CDialog*)GetActiveWindow();						//ȡ�öԻ���ָ�� 
		Pwnd->NextDlgCtrl();											//�л�����һ�����뽹�� 
		return true;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����������С
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect	v_cRect;

	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//�ִ��ߴ����
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=60;
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��رմ���
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (v_iThread_Exit>0)												//�߳����У�����
	{
		v_iThread_Exit=2;												//ǿ���˳�
		return;															//����
	}

	CDialog::OnClose();													//�˳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��쳣����
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show_Name(&m_cList_Bug,"�����쳣_����",m_cFile_Open.GetCheck());		//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ�ʹ�����CSV�ļ���Ϣ
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=0;														//�������񣺻�Ϊ�ļ�ѡ��

	v_iThread_Exit=1;													//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����б��е����ݱ��浽���ݱ�
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺻�Ϊ���ݵ���

	v_iThread_Exit=1;													//�߳̿�������Ч
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//����
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����	[��̬���޼���]
//------------------------------------------------------------------------------------------------------
UINT CCON_Station_Input::My_Thread_Inoput(LPVOID lparam)
{
	CCON_Station_Input *  lp_this = NULL ;
	lp_this = (CCON_Station_Input *)lparam;

	lp_this->m_cFile.EnableWindow(false);								//�ļ�ѡ����Ч
	lp_this->m_cInput.EnableWindow(false);								//��ʼ���룺��Ч
	lp_this->m_cExcel.EnableWindow(false);								//��ʼ��������Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cFile.EnableWindow(true);								//�ļ�ѡ����Ч
	lp_this->m_cInput.EnableWindow(true);								//��ʼ���룺��Ч
	lp_this->m_cExcel.EnableWindow(true);								//��ʼ��������Ч

	lp_this->v_iThread_Exit=0;											//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 0:
		My_File();														//�ļ�ѡ��
		break;
	case 1:
		My_Input_Station();												//Station
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡCSV�ļ���Ϣ
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input::My_File()
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

		m_cList.DeleteAllItems();										//ɾ��������Ŀ
		m_cList_Bug.DeleteAllItems();									//ɾ��������Ŀ

		//�������ļ������ƶ�Ӧ����ʾ����š��ļ�������--��ʾ����š�
		v_mMap.insert(pair<CString,int>("��������",1));					//1����������
		v_mMap.insert(pair<CString,int>("����վ��",2));					//2������վ��
		v_mMap.insert(pair<CString,int>("��������",3));					//3������
		v_mMap.insert(pair<CString,int>("��������",4));					//4������
		v_mMap.insert(pair<CString,int>("��������״̬",5));				//5��״̬
		v_mMap.insert(pair<CString,int>("��������",6));					//6����������
		if (!Get_CSV_Cell(v_sFilename,&v_mMap,true))					//��ȡ��CSV�ļ���Ϣ
			MessageBox("����Ч�� ���� �ļ���������ѡ�� ������");		//������ʾ	
	}
	delete(filedialog);
	v_mMap.clear();														//�������
}

//----------------------------------------------------------------------------------------------------
//	�������ܣ�����CSV�ļ��� m_cList �ؼ���
//	��    ����CSV�ļ�·����	v_mMap����ӳ���ϵ��	v_bHead���ļ�����ͷ��ǣ�
//----------------------------------------------------------------------------------------------------
bool CCON_Station_Input::Get_CSV_Cell(CString v_sFile,mString_Int *v_mHead,bool v_bHead)
{
	CStdioFile	v_fRead;												//ʵ����	
	CString		v_sStr,v_sTemp;											//��ʱ����
	bool		v_Flag_Head=true;										//��ʱ����
	int			v_iLen,v_iNum,v_iItem,v_iSum;							//��ʱ����
	CString		v_sType,v_sOnline,v_sName;								//��ʱ����

	mString_Int::iterator	v_pCol_Head;								//������
	mInt_Int				v_mMap;										//����
	mInt_Int::iterator		v_pCol;										//������
	mInt_String				v_mData;									//����������Ϣ���
	mInt_String::iterator	v_pIter;									//������
	mString_Int				v_mRepeat;									//�������ظ���Ϣ���
	mString_Int::iterator	v_pRepeat;									//������

	if (!v_fRead.Open(_T(v_sFile),CFile::modeRead))						//���ļ�
	{
		MessageBoxA("�쳣���ļ���ʧ�ܣ�����", "��ʾ", MB_OK);			//��ʾ��Ϣ
		return false;													//���أ�ʧ��
	}

	v_iSum=1;															//�����
	while(v_fRead.ReadString(v_sStr))									//���ݶ�ȡ��һ��
	{
		if (v_iThread_Exit==2)											//ǿ���˳�������
			break;														//�˳�

		v_sStr.Replace("\"","");										//ȥ��"
		if (v_bHead && v_Flag_Head)										//����ͷ��
		{
			v_Flag_Head=false;											//����ͷ�����Ե�
			//��ͷ������ͷ����==����ͷ���
			v_iNum=1;													//�к�
			while (true)
			{
				if (v_sStr.IsEmpty())									//Ϊ�գ�����
					break;												//�˳�
				v_iLen=v_sStr.Find(",",0);								//����","
				if (v_iLen<0)											//�ҵ�������
				{
					v_sTemp=v_sStr;										//��ȡ����Ч��Ϣ
					v_sStr="";											//���㣺��һ����Ϣλ��
				}
				else
				{
					v_sTemp=v_sStr.Left(v_iLen);						//��ȡ����Ч��Ϣ
					v_sStr=v_sStr.Mid(v_iLen+1);						//���㣺��һ����Ϣλ��
				}
				v_pCol_Head = v_mHead->find(v_sTemp);					//���ұ�ͷ���ƣ�����
				if (v_pCol_Head != v_mHead->end())						//�ҵ�������
					v_mMap.insert(pair<int,int>(v_iNum,v_pCol_Head->second));	//�У�1��2��3...
				v_iNum++;												//�кţ�++
			}
			continue;													//
		}
		//һ�����ݴ���
		v_sType="";														//����
		v_sOnline="";													//״̬
		v_iNum=1;														//�к�
		while (true)
		{
			if (v_sStr.IsEmpty())										//Ϊ�գ�����
				break;													//�˳�
			v_iLen=v_sStr.Find(",",0);									//����","
			if (v_iLen<0)												//�ҵ�������
			{
				v_sTemp=v_sStr;											//��ȡ����Ч��Ϣ
				v_sStr="";												//���㣺��һ����Ϣλ��
			}
			else
			{
				v_sTemp=v_sStr.Left(v_iLen);							//��ȡ����Ч��Ϣ
				v_sStr=v_sStr.Mid(v_iLen+1);							//���㣺��һ����Ϣλ��
			}
			v_pCol=v_mMap.find(v_iNum);									//�����кţ�����
			if (v_pCol != v_mMap.end())									//�ҵ�������
			{
				v_mData.insert(pair<int,CString>(v_pCol->second,v_sTemp));		//�У�1��2��3...
				if (v_pCol->second==1)									//Station
					v_sName=v_sTemp;									//���棺����
			}
			v_iNum++;													//�кţ�++
		}
		if (!v_sName.IsEmpty())											//������������Ч������
		{
			v_pRepeat=v_mRepeat.find(v_sName);							//�������ڣ�����
			if (v_pRepeat == v_mRepeat.end())							//������������
			{
				v_mRepeat.insert(pair<CString,int>(v_sName,0));			//����������
				v_sTemp.Format("%06d",v_iSum);							//�к�
				v_iItem=m_cList.InsertItem(0xffff,v_sTemp);				//�к�
				v_pIter=v_mData.begin();								//������ͷ
				while(v_pIter!=v_mData.end())							//������β������
				{
					m_cList.SetItemText(v_iItem,v_pIter->first,v_pIter->second);	//��ʾ					
					v_pIter++;											//������++
				}
				if (v_iSum%500==0)
				{
					v_sTemp.Format("%d",v_iSum);						//��������
					m_cSum.SetWindowText(v_sTemp);						//��������
				}
				v_iSum++;												//���++
			}
			else														//���������ظ�
			{
				My_ShowList_Bug("�����쳣","�쳣�����������ظ� ���������ƣ�"+v_sName+"��");		//��ʾ���쳣��Ϣ
			}
		}
		v_mData.clear();												//���������
	}
	v_fRead.Close();													//�ر��ļ�
	v_mRepeat.clear();													//���������
	v_mMap.clear();														//���������
	v_sTemp.Format("%d",m_cList.GetItemCount());						//��������
	m_cSum.SetWindowText(v_sTemp);										//��������
	return true;														//���أ��ɹ�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ݵ���
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input::My_Input_Station()
{
	int		v_iLen,vi;													//��ʱ����
	CString	v_sSql,v_sTemp;												//��ʱ����
	CString	v_sAddr,v_sType,v_sRegion,v_sLocation;						//��ʱ����
	CString v_sOnline,v_sDate,v_sNew,v_sStation,v_sTxfx,v_sTxgd,v_sTxqj;//��ʱ����
	int		v_iSum=0;													//
	int		v_iSum_Sql=0;												//

	mString_Int					v_mStation;								//������v_mStation
	mString_Int::iterator		v_pIter;								//������

	if(MessageBox("���� ���� ����ȷ�� ? ","���ݵ���ȷ��",MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)	//����ȷ�ϣ�����
		return;															//���أ��˳�

	//��һ�������ݳ�ʼ��
	v_iLen=m_cList.GetItemCount();										//�б���Ŀ����
	if (v_iLen<=0)
	{
		MessageBox("û�п��Ե���� ���� ���� ������");					//���أ�������
		return;
	}

	//�ڶ�������ȡ������� [վ�㡢����]
	My_Load_Station(&v_mStation);										//��ȡ����Ϣ [���룺v_mStation]

	//�����������ݱ��� [���ڣ����£������ڣ�����]
	COleDateTime v_tTime=COleDateTime::GetCurrentTime();				//����
	CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");			//
	v_sSql="";															//��ʼ��
	for (vi=0;vi<v_iLen;vi++)
	{
		if (v_iThread_Exit==2)											//ǿ���˳�������
			break;														//�˳�

		//��ȡ������
		v_sStation=m_cList.GetItemText(vi,1);							//��������/��Դ��
		v_sLocation=m_cList.GetItemText(vi,2);							//����վ��
		v_sType=m_cList.GetItemText(vi,3);								//��������
		v_sRegion=m_cList.GetItemText(vi,4);							//����
		v_sOnline=m_cList.GetItemText(vi,5);							//״̬

		if (v_sType!="���߻���" && v_sType!="��������")					//��Ч�������ͣ�����
			continue;													//��Ч������

		if (!v_sStation.IsEmpty())										//��Ч������
		{
			//Station���ڣ����£������ڣ��쳣;
			v_iSum++;													//������Station
			v_pIter=v_mStation.find(v_sStation);						//���ң�v_sStation
			if (v_pIter == v_mStation.end())							//û�ҵ�������
			{															//����
				v_sNew="Insert into CON_Station(zStation,zType,zRegion,zLocation,zOnline,zDate) values \
					   ('"+v_sStation+"','"+v_sType+"','"+v_sRegion+"','"+v_sLocation+"','"+v_sOnline
					   +"','"+v_sCurrentTime+"')";						//SQL
				m_cList.SetItemBkColor(vi,RGB(0,255,0));				//���ã�����ɫ [��ɫ]
			}
			else
			{
				//����ʱ�����ٸ�������վַ������С������ͬ����
				v_sNew="Update CON_Station set zType='"+v_sType+"',zLocation='"+v_sLocation
					+"',zOnline='"+v_sOnline+"',zDate='"+v_sCurrentTime
					+"' where zStation='"+v_sStation+"'";				//SQL
				m_cList.SetItemBkColor(vi,RGB(67,240,250));				//���ã�����ɫ [��ɫ]			
			}
			//���ݿ����
			v_sSql+=v_sNew+"\r\n";										//����һ��SQL
			v_iSum_Sql++;												//SQL����++
			if(v_iSum_Sql==1000)										//ÿ1000����д���ݱ�
			{
				My_ExecuteSQL(v_sSql);									//SQLִ��
				v_sSql="";												//��ʼ��
				v_iSum_Sql=0;											//��ʼ��
			}

			if (v_iSum%100==0)
			{
				v_sTemp.Format("%d",v_iSum);							//��������
				m_cSum.SetWindowText(v_sTemp);							//��������
			}
		}
		else
		{
			My_ShowList_Bug("���� �쳣","�쳣�����������쳣�����������ƣ�"+v_sStation+"��");	//��ʾ���쳣��Ϣ
			m_cList.SetItemBkColor(vi,RGB(255,0,0));					//���ã�����ɫ [��ɫ]
		}
		m_cList.EnsureVisible(vi,false);								//��ʾ���ȶԼ�¼
	}
	if(v_iSum_Sql>0)													//��SQL��Ϣ������ ��д���ݱ�
		My_ExecuteSQL(v_sSql);											//SQLִ��

	v_mStation.clear();													//���������

	//��־
	v_sSql="Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"+v_sCurrentTime+"','���� ���ݵ���','[�û���"+theApp.a_sUser_Name+"]')";//SQL
	My_ExecuteSQL(v_sSql);												//���ݿ�ִ��

	v_sTemp.Format("%d",v_iSum);										//��������
	m_cSum.SetWindowText(v_sTemp);										//��������
	v_sTemp.Format("���� ������%d",v_iSum);
	MessageBox("���� ���ݵ�����ɣ�"+v_sTemp);							//���أ�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ����Ϣ��Station��
//------------------------------------------------------------------------------------------------------			
void CCON_Station_Input::My_Load_Station(mString_Int *v_mStation)
{
	CString			v_sSql;												//��ʱ����
	CString			v_sTemp,v_sName;									//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��ȡ��NodeB
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT zStation FROM CON_Station";
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										
		while(!v_pRecordset->adoEOF)
		{
			v_sName = v_cAdoConn.GetField_String("zStation");			//zStation

			v_mStation->insert(pair<CString,int>(v_sName,0));			//����������

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
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Bug�б�������һ��ͨ����Ϣ
//	��    ����v_sBug_Type���쳣���ͣ�	v_sBug���쳣��Ϣ
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input::My_ShowList_Bug(CString v_sBug_Type,CString v_sBug)
{
	int		v_iIndex;													//��ʱ����
	CString v_sTemp;													//��ʱ����
	CString v_sCurTime;													//��ʱ����
	CTime	v_CurTime=CTime::GetCurrentTime();

	v_sCurTime=v_CurTime.Format("%Y.%m.%d_%H:%M:%S");
	v_iIndex = m_cList_Bug.InsertItem(0xffff,_T("0"));
	v_sTemp.Format("%04d",v_iIndex+1);
	m_cList_Bug.SetItemText(v_iIndex,0," "+v_sTemp);					//���
	m_cList_Bug.SetItemText(v_iIndex,1,v_sCurTime);						//����ʱ��
	m_cList_Bug.SetItemText(v_iIndex,2,v_sBug_Type);					//�쳣����
	m_cList_Bug.SetItemText(v_iIndex,3,v_sBug);							//�쳣��Ϣ

	//�б��������ƣ�����1000����ɾ��300����
// 	m_cList_Bug.SetRedraw(FALSE);										//�����½���
// 	if (m_cList_Bug.GetItemCount()>=1000)								//�����¼������1000
// 	{
// 		for(vi=0;vi<300;vi++)											//ɾ��ǰ300��
// 			m_cList_Bug.DeleteItem(0);									//ɾ����һ�����ݼ�¼
// 		for(vi=0;vi<m_cList_Bug.GetItemCount();vi++)					//���·������
// 		{
// 			v_sTemp.Format("%04d",vi+1);
// 			m_cList_Bug.SetItemText(vi,0," "+v_sTemp);
// 		}
// 	}
	m_cList_Bug.SetRedraw(true);										//���½���
	m_cList_Bug.EnsureVisible(m_cList_Bug.GetItemCount()-1,false);		//��ʾ����һ����¼
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CCON_Station_Input::My_ExecuteSQL(CString v_sSql)
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
