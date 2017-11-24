// CM_LTE_Relation.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "CM_LTE_Relation.h"

// CCM_LTE_Relation

IMPLEMENT_DYNCREATE(CCM_LTE_Relation, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CCM_LTE_Relation::CCM_LTE_Relation()
	: CFormView(CCM_LTE_Relation::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CCM_LTE_Relation::~CCM_LTE_Relation()
{
	v_mTable_Name.clear();												//�������
	v_mField_Main.clear();												//�������
	v_mCell.clear();													//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cLog_Day);
	DDX_Control(pDX, IDC_KEY, m_cLog_Key);
	DDX_Control(pDX, IDC_BUTTON1, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_LIST3, m_cList_PM);
	DDX_Control(pDX, IDC_COMBO3, m_cType_2);
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_LOG_SUM3, m_cData_Sum);
	DDX_Control(pDX, IDC_KEY3, m_cLog_Key1);
	DDX_Control(pDX, IDC_COMBO8, m_cRelation);
	DDX_Control(pDX, IDC_CHECK2, m_cSave);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCM_LTE_Relation, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL, &CCM_LTE_Relation::OnBnClickedExcel)
	ON_BN_CLICKED(IDC_BUTTON1, &CCM_LTE_Relation::OnBnClickedButton1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CCM_LTE_Relation::OnLvnColumnclickList3)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST3, &CCM_LTE_Relation::OnLvnGetdispinfoList3)
	ON_CBN_CLOSEUP(IDC_COMBO1, &CCM_LTE_Relation::OnCbnCloseupCombo1)
END_MESSAGE_MAP()


// CCM_LTE_Relation ���

#ifdef _DEBUG
void CCM_LTE_Relation::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCM_LTE_Relation::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCM_LTE_Relation ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	CString v_sTemp;													//��ʱ����
	//��������
	v_sFrame_Name="LTE-Tdd���ڹ�ϵ";									//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б������á�
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"���",LVCFMT_CENTER,60);
	m_cList_PM.InsertColumn(1,"ʱ��",LVCFMT_CENTER,80);  
	m_cList_PM.InsertColumn(2,"DN",LVCFMT_CENTER,120);  
	m_cList_PM.InsertColumn(3,"Ŀ��С��",LVCFMT_LEFT,120);				//
	m_cList_PM.InsertColumn(4,"����С��",LVCFMT_LEFT,120);				//
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��ʼ��������ѡ��
	m_cLog_Day.AddString("����");										//
	m_cLog_Day.AddString("��Ϊ");										//
	m_cLog_Day.SetCurSel(0);											//Ĭ�ϲ�ѯ���������ˣ�

	m_cRelation.AddString("��");
	m_cRelation.AddString("��");
	m_cRelation.SetCurSel(0);											//Ĭ��

	v_sTable_Pro="";													//��ʼ�����ļ�ǰ׺
	v_iList_Item=-1;													//Ĭ�ϣ���ѡ�е��б�Item

	//��ȡ�����ܲ���
	My_LoadData_Para();													//��ȡ�����ܲ���
	My_Get_Cm_Field();													//����ѡ��

	//��ȡ��������Ϣ
	v_iWorking=2;														//�������񣺻�ȡ������Ϣ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ӵ�����
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::OnDestroy()
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
void CCM_LTE_Relation::OnSize(UINT nType, int cx, int cy)
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
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CCM_LTE_Relation::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ�����Excel
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_PM);											//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Log�б�����(�һ��б�������)�����á�
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Relation::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	m_cList_PM.My_Sort_Virtual(pNMHDR);									//�б����������б�

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ��
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::OnCbnCloseupCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ�����ò���(���ݣ����ڡ��ļ�)
	My_Get_Cm_Field();													//��ȡ���ò���(����ʾ)
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ���ò���(����ʾ)
//	��	  ����v_sType���ļ�������
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::My_Get_Cm_Field()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp,v_sTable,v_sMaker,v_sNum;						//��ʱ����
	int			v_iNum;													//��ʱ����

	//��0����ʼ��
	m_cList_PM.DeleteAllItems();										//ɾ��������Ŀ
	while(m_cList_PM.DeleteColumn(0));									//ɾ�������У�ObjType��Ϣ�б�
	m_cType.ResetContent();												//ɾ��
	m_cType_2.ResetContent();											//ɾ��
	m_cData_Sum.SetWindowText("");										//�����������

	//��1����ȡ���ļ����� (�ļ�����+ѡ������)
	if (m_cLog_Day.GetCurSel()==1)
		v_sMaker="HW";													//��Ϊ
	else
		v_sMaker="ZTE";													//����
	v_sTable_Pro = "Xm_LTE_"+v_sMaker+"_EutranRelationTdd_Day";			//������

	//��2���жϣ��ļ��Ƿ���ڣ�����
	set<CString>::iterator v_pFile;										//������
	v_pFile=v_mTable_Name.find(v_sTable_Pro);							//�����ļ�
	if (v_pFile==v_mTable_Name.end())									//�����ڣ�����
	{
		MessageBox("��Чѡ��!!!  ������ѡ��");							//��ʾ
		return;															//����
	}

	//��3����ȡ���ļ��ֶ�(��ʾ)
	v_mField_Main.clear();												//�������
	My_Get_File_Field(&v_mField_Main,v_sTable_Pro);						//��ȡ���ļ��ֶ�

	//��4����ʾ���ļ��ֶ���Ϣ(����Ϣ)

	//��ObjType��Ϣ�б������Ϣ
	m_cList_PM.InsertColumn(0,"���",LVCFMT_CENTER,60);					//
	m_cList_PM.InsertColumn(1,"ʱ��",LVCFMT_CENTER,80);					//
	m_cList_PM.InsertColumn(2,"DN",LVCFMT_LEFT,120);					//
	m_cList_PM.InsertColumn(3,"Ŀ��С��",LVCFMT_LEFT,120);				//
	m_cList_PM.InsertColumn(4,"����С��",LVCFMT_LEFT,120);				//

	v_sOutput="���,ʱ��,DN,Ŀ��С��,����С��";							//

	//�����С������б�
	m_cType.AddString("Ŀ��С��");										//
	m_cType_2.AddString("Ŀ��С��");									//
	m_cType.AddString("����С��");										//
	m_cType_2.AddString("����С��");									//
	mString_Int::iterator v_pField;										//������
	v_iNum=5;
	for(v_pField=v_mField_Main.begin();v_pField!=v_mField_Main.end();v_pField++)	//����
	{
		//������
		m_cList_PM.InsertColumn(v_iNum,v_pField->first,LVCFMT_LEFT,120);//��n�У�����

		v_sOutput+=","+v_pField->first;									//�ļ�

		v_iNum++;														//������
		
		//��������
//		m_cType.AddString(v_pField->first);								//
//		m_cType_2.AddString(v_pField->first);							//
	}
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	m_cType.SetCurSel(0);												//Ĭ��
	m_cType_2.SetCurSel(0);												//Ĭ��
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������б���Ϣ��Ӧ����
//	��	  ���������б���Ҫ��Ԫ�ز���(�С���)�������������в�ѯ���ݣ���ֵ���б��С�
//------------------------------------------------------------------------------------------------------	
void CCM_LTE_Relation::OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ�������б���ʾ���ݻ�ȡ
	m_cList_PM.Get_List_Data(pDispInfo);								//�����б���ʾ���ݻ�ȡ

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������ѯ��ť�����ò�ѯ��
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//��������С����ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CCM_LTE_Relation::My_Thread_Inoput(LPVOID lparam)
{
	CCM_LTE_Relation *  lp_this = NULL ;
	lp_this = (CCM_LTE_Relation *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(false);								//��������Ч
	lp_this->m_cType_2.EnableWindow(false);								//�������ͣ���Ч
	lp_this->m_cType.EnableWindow(false);								//��ѯ���ͣ���Ч
	lp_this->m_cLog_Day.EnableWindow(false);							//��������Ч
	lp_this->m_cLog_Key.EnableWindow(false);							//���룺��Ч
	lp_this->m_cLog_Key1.EnableWindow(false);							//���룺��Ч
	lp_this->m_cRelation.EnableWindow(false);							//����ϵ����Ч
	lp_this->m_cSave.EnableWindow(false);								//�����ļ�����Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cQuery.EnableWindow(true);								//��ѯ����Ч
	lp_this->m_cExcel.EnableWindow(true);								//��������Ч
	lp_this->m_cType_2.EnableWindow(true);								//�������ͣ���Ч
	lp_this->m_cType.EnableWindow(true);								//��ѯ���ͣ���Ч
	lp_this->m_cLog_Day.EnableWindow(true);								//��������Ч
	lp_this->m_cLog_Key.EnableWindow(true);								//���룺��Ч
	lp_this->m_cLog_Key1.EnableWindow(true);							//���룺��Ч
	lp_this->m_cRelation.EnableWindow(true);							//����ϵ����Ч
	lp_this->m_cSave.EnableWindow(true);								//�����ļ�����Ч

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_CM();												//װ��������Ϣ
		break;
	case 2:
		My_LoadData_Cell();												//��ȡ��С����Ϣ
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ����������
//	��    �ݣ�
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Relation::My_LoadData_CM()
{
	CString			v_sTemp;											//��ʱ����
	COleDateTime	v_dDate_Start,v_dDate_End,v_dDate;					//��ѯ��ʼ���ڡ���������
	CString			v_sTime_Start,v_sTime_End,v_sTable_Num;				//����

	//��1����ʾ���������б�������Ϣ
	m_cData_Sum.SetWindowText("");										//�����������������
	v_iCM_Sum=0;														//��ȡ����������

	//��2����λ�������б�
	m_cList_PM.Put_Line_Begin();										//�б�������ݿ�ʼ

	//��4�����ò�ѯ���ձ�  
	My_LoadData_CM_Data(v_sTable_Pro);									//��ȡ��������

	//��5�����ã������б���Ч
	m_cList_PM.Put_Line_End();											//�б�������ݽ���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ����������
//	��	  ����v_sTable�������ƣ�
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Relation::My_LoadData_CM_Data(CString v_sTable)
{
	CString			v_sSql,v_sCSV,v_sStr,v_sCSV_File,v_sPath_Data;		//��ʱ����
	CString			v_sTemp,v_sTime,v_sLog_Key,v_sLog_Key1;				//��ʱ����
	int				v_iItem,v_iLen;										//��ʱ����
	CString			v_sObject,v_sDN,v_sValues,v_sSelect,v_sSelect1;		//��ʱ����
	CString			v_sRelation="",v_sRelation1="",v_sCell_ID,v_sE_NodeB_ID,v_sCell,v_sCell_N;	//��ʱ����
	bool			v_bOK,v_bOK1;										//��ʱ����
	COleDateTime	v_dTime;											//����

	CFile	v_fWriter;													//ʵ����
	CFileFind	finder;

	if (m_cSave.GetCheck())												//�����ļ���������
	{
		//Ŀ¼����
		v_sPath_Data=theApp.m_sPath+"\\DATA";							//�ļ�Ŀ¼
		if (!finder.FindFile(v_sPath_Data))								//���ڣ�
		{
			if (!::CreateDirectory(v_sPath_Data, NULL))					//�����ڣ�����Ŀ¼
			{
				MessageBoxA("�쳣��"+v_sPath_Data+" Ŀ¼����ʧ�ܡ�");	//ʧ��
				return;
			}
		}

		for(int v_iNum3=0;v_iNum3<100;v_iNum3++)
		{
			if (v_iNum3==0)												//�ַ���
				v_sStr="";
			else
				v_sStr.Format("_%02d",v_iNum3);							//�ַ���
			//����CSV�ļ����ļ�����v_sCSV_File
			v_sCSV_File = theApp.m_sPath+"\\DATA\\Excel"+v_sStr+".csv";	//���ã�CSVĬ���ļ�����Excel.csv
			if(v_fWriter.Open(v_sCSV_File, CFile::modeCreate | CFile::modeWrite))	//���ļ����粻���������ɣ������������ļ���
			{
				v_sOutput +="\r\n";										//�س�����
				v_fWriter.Write(v_sOutput,v_sOutput.GetLength());		//д���ַ���
				break;
			}
		}
	}

	mString_Int		v_mField_Table;										//��ȡ����ֶ�����
	mString_Int::iterator v_pField_Table;								//������
	mString_Int::iterator v_pField_Main;								//������
	mString_String::iterator v_pCell;									//������

	//�����б�
	mInt_String		v_mLine_Data;										//�����б�������

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		//��1����飺�������Ƿ���Ч�����ݣ������Ƽ���v_mTable_Name��
		set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//���ң���
		if (v_pTable==v_mTable_Name.end())								//û�ҵ�
			return;														//��Ч����

		My_Get_File_Field(&v_mField_Table,v_sTable);					//��ȡ���ļ��ֶ�

		//��2���������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select * from "+v_sTable;
		
		m_cLog_Key.GetWindowText(v_sLog_Key);							//��ã���ѯKEY
		v_sLog_Key.Replace("[","[[]");									//���ƴ���
		v_sLog_Key.Replace("'","");										//���ƴ���
		v_sLog_Key.Trim();												//ȥ���ո�

		m_cLog_Key1.GetWindowText(v_sLog_Key1);							//��ã���ѯKEY1
		v_sLog_Key1.Replace("[","[[]");									//���ƴ���
		v_sLog_Key1.Replace("'","");									//���ƴ���
		v_sLog_Key1.Trim();												//ȥ���ո�

		if (!v_sLog_Key.IsEmpty())										//��ѯKEY����
		{
			if(m_cType.GetCurSel()>=2)									//ѡ�񣺶���
			{
				m_cType.GetLBText(m_cType.GetCurSel(),v_sSelect);
				v_pField_Main=v_mField_Main.find(v_sSelect);			//���ң��ֶ�
				if (v_pField_Main!=v_mField_Main.end())					//�ҵ�����
					v_sRelation="["+v_sSelect+"] like '%"+v_sLog_Key+"%'";		//ѡ�������
			}
			else
			{
				if (m_cLog_Day.GetCurSel()==1)							//��Ϊ������
				{
					if (m_cType.GetCurSel()==0)							//Ŀ��
						v_sRelation="[LocalCellName] like '%"+v_sLog_Key+"%'";		//ѡ�������
					else
						v_sRelation="[zObject] like '%"+v_sLog_Key+"%'";			//ѡ�������
				}
			}
		}
		if (!v_sLog_Key1.IsEmpty())										//��ѯKEY1����
		{
			if(m_cType_2.GetCurSel()>=2)								//ѡ�񣺶���
			{
				m_cType_2.GetLBText(m_cType_2.GetCurSel(),v_sSelect1);
				v_pField_Main=v_mField_Main.find(v_sSelect1);			//���ң��ֶ�
				if (v_pField_Main!=v_mField_Main.end())					//�ҵ�����
					v_sRelation1="["+v_sSelect1+"] like '%"+v_sLog_Key1+"%'";	//ѡ�������
			}
			else
			{
				if (m_cLog_Day.GetCurSel()==1)							//��Ϊ������
				{
					if (m_cType_2.GetCurSel()==0)						//Ŀ��
						v_sRelation1="[LocalCellName] like '%"+v_sLog_Key1+"%'";	//ѡ�������
					else
						v_sRelation1="[zObject] like '%"+v_sLog_Key1+"%'";			//ѡ�������
				}
			}
		}
		if (!v_sRelation.IsEmpty() && !v_sRelation1.IsEmpty())			//������
		{
			if (m_cRelation.GetCurSel()==0)								//��
				v_sSql+=" where ("+v_sRelation+" and "+v_sRelation1+")";//��
			else														//��
				v_sSql+=" where ("+v_sRelation+" or "+v_sRelation1+")";	//��
		}
		else if (!v_sRelation.IsEmpty())								//����
			v_sSql+=" where "+v_sRelation;
		else if (!v_sRelation1.IsEmpty())								//����1
			v_sSql+=" where "+v_sRelation1;

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		//��3����ȡ��������
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			//���ݸ�λ
			v_mLine_Data.clear();										//�������

			//��4����ȡ����
			v_dTime=v_cAdoConn.GetField_DataTime("zTime");				//zTime
			v_sTime=v_dTime.Format("%Y-%m-%d");							//�ַ���
			v_sDN=v_cAdoConn.GetField_String("zDN");					//zDN

			//��ȡ��Ŀ��С�����ھ�С��
			if (m_cLog_Day.GetCurSel()==1)								//��Ϊ������
			{
				//��Ϊ��ֱ�Ӷ�ȡ�ֶ�
				v_sCell=v_cAdoConn.GetField_String("LocalCellName");	//LocalCellName
				v_sCell_N=v_cAdoConn.GetField_String("zObject");		//zObject[�ھ�]
			}
			else
			{
				//���ˣ���DN�н��������� [����DN][ZTE-CMLN-SY2;SubNetwork=ltetdd45;ManagedElement=ENODEBME927500;EnbFunction=927500;EutranCellTdd=12;EutranRelationTdd=0:460:00:275597:1]
				v_sCell="";
				v_sCell_N="";
				v_iLen=v_sDN.Find("EnbFunction=");
				if (v_iLen>=0)
				{
					v_sCell_ID="";
					v_sE_NodeB_ID=v_sDN.Mid(v_iLen+12);
					v_iLen=v_sE_NodeB_ID.Find("EutranCellTdd=");
					if (v_iLen>=0)
					{
						v_sCell_ID=v_sE_NodeB_ID.Mid(v_iLen+14);
						v_iLen=v_sCell_ID.Find(";");
						if (v_iLen>=0)
							v_sCell_ID=v_sCell_ID.Left(v_iLen);
					}
					v_iLen=v_sE_NodeB_ID.Find(";");
					if (v_iLen>=0)
						v_sE_NodeB_ID=v_sE_NodeB_ID.Left(v_iLen);
					if (!v_sE_NodeB_ID.IsEmpty() && !v_sCell_ID.IsEmpty())
					{
						v_pCell=v_mCell.find(v_sE_NodeB_ID+"|"+v_sCell_ID);	//С����ѯ
						if (v_pCell!=v_mCell.end())							//�ҵ�������
							v_sCell=v_pCell->second;						//��ȡ��Ŀ��С��
					}
				}

				v_iLen=v_sDN.Find(":");
				if (v_iLen>=0)
				{
					v_sE_NodeB_ID=v_sDN.Mid(v_iLen+1);
					v_iLen=v_sE_NodeB_ID.Find(":");
					if (v_iLen>=0)
					{
						v_sE_NodeB_ID=v_sE_NodeB_ID.Mid(v_iLen+1);
						v_iLen=v_sE_NodeB_ID.Find(":");
						if (v_iLen>=0)
						{
							v_sCell_ID="";
							v_sE_NodeB_ID=v_sE_NodeB_ID.Mid(v_iLen+1);
							v_iLen=v_sE_NodeB_ID.Find(":");
							if (v_iLen>=0)
							{
								v_sCell_ID=v_sE_NodeB_ID.Mid(v_iLen+1);
								v_sE_NodeB_ID=v_sE_NodeB_ID.Left(v_iLen);
							}
							if (!v_sE_NodeB_ID.IsEmpty() && !v_sCell_ID.IsEmpty())
							{
								v_pCell=v_mCell.find(v_sE_NodeB_ID+"|"+v_sCell_ID);	//С����ѯ
								if (v_pCell!=v_mCell.end())							//�ҵ�������
									v_sCell_N=v_pCell->second;						//��ȡ���ھ�С��
							}
						}
					}
				}

				//���ˣ������ѯ
				v_bOK=true;												//��Ч
				v_bOK1=true;											//��Ч

				if (!v_sLog_Key.IsEmpty())								//����
				{
					if (m_cType.GetCurSel()==0)							//Ŀ��
					{
						if (v_sCell.Find(v_sLog_Key)<0)					//�����㣿����
							v_bOK=false;								//��Ч
					}
					else
					{
						if (v_sCell_N.Find(v_sLog_Key)<0)				//�����㣿����
							v_bOK=false;								//��Ч
					}
				}
				if (!v_sLog_Key1.IsEmpty())								//����1
				{
					if (m_cType_2.GetCurSel()==0)						//Ŀ��
					{
						if (v_sCell.Find(v_sLog_Key1)<0)				//�����㣿����
							v_bOK1=false;								//��Ч
					}
					else
					{
						if (v_sCell_N.Find(v_sLog_Key1)<0)				//�����㣿����
							v_bOK1=false;								//��Ч
					}
				}

				if (!v_sLog_Key.IsEmpty() && !v_sLog_Key1.IsEmpty())	//������
				{
					if (m_cRelation.GetCurSel()==0)						//��
					{
						if (!v_bOK || !v_bOK1)
						{
							v_pRecordset->MoveNext();					//��һ��
							continue;									//��һ��
						}
					}
					else												//��
					{
						if (!v_bOK && !v_bOK1)
						{
							v_pRecordset->MoveNext();					//��һ��
							continue;									//��һ��
						}
					}
				}
				else if (!v_sLog_Key.IsEmpty())							//����
				{
					if (!v_bOK)
					{
						v_pRecordset->MoveNext();						//��һ��
						continue;										//��һ��
					}
				}
				else if (!v_sLog_Key1.IsEmpty())						//����1
				{
					if (!v_bOK1)
					{
						v_pRecordset->MoveNext();						//��һ��
						continue;										//��һ��
					}
				}
			}
		
			v_sTemp.Format(" %06d",v_iCM_Sum+1);
			if (!m_cSave.GetCheck())									//��ʾ������
			{
				v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));		//���
				v_mLine_Data.insert(pair<int,CString>(1,v_sTime));		//ʱ��
				v_mLine_Data.insert(pair<int,CString>(2,v_sDN));		//DN
				v_mLine_Data.insert(pair<int,CString>(3,v_sCell));		//Ŀ��С��
				v_mLine_Data.insert(pair<int,CString>(4,v_sCell_N));	//����С��
			}
			else														//�����ļ�
				v_sCSV=v_sTemp+","+v_sTime+","+v_sDN+","+v_sCell+","+v_sCell_N;

			//��5����ʾ������
			v_iItem=5;													//���
			v_pField_Table=v_mField_Table.begin();						//�ֶ���ָ��
			while(v_pField_Table!=v_mField_Table.end())					//�ֶν�������
			{
				//�Ƿ���Ч�ֶΣ������������ֶα�v_mField_Main��
				v_pField_Main=v_mField_Main.find(v_pField_Table->first);//���ң��ֶ�
				if (v_pField_Main!=v_mField_Main.end())					//�ҵ�����
				{
					v_sValues=v_cAdoConn.GetField_String(v_pField_Table->first);	//��ȡ����������
					if (!m_cSave.GetCheck())							//��ʾ������
						v_mLine_Data.insert(pair<int,CString>(v_iItem,v_sValues));	//��������
					else												//�����ļ�
						v_sCSV+=","+v_sValues;
				}
		
				v_iItem++;												//�ݼ�
				v_pField_Table++;										//�ݼ�
			}
			
			//�������ݷ����б�
			if (!m_cSave.GetCheck())									//��ʾ������
				m_cList_PM.Put_Line_Data(v_mLine_Data);					//�б�������������
			else														//�����ļ�
			{
				v_sCSV +="\r\n";										//�س�����
				v_fWriter.Write(v_sCSV,v_sCSV.GetLength());				//д���ַ���
			}

			v_pRecordset->MoveNext();									//��һ��

			v_iCM_Sum++;												//++
			if ((v_iCM_Sum)%500==0)
			{
				v_sTemp.Format("%d",v_iCM_Sum);							//
				m_cData_Sum.SetWindowText(v_sTemp);						//��ʾ��������������
			}
		}
		
		v_sTemp.Format("%d",v_iCM_Sum);									//
		m_cData_Sum.SetWindowText(v_sTemp);								//��ʾ��������������

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	if (!m_cSave.GetCheck())											//��ʾ������
	{
		//�ֶ�
		v_mField_Table.clear();											//�������

		//�����б�
		v_mLine_Data.clear();											//�������
	}
	else
	{
		v_fWriter.Close();												//�ļ��ر�

		ShellExecute(NULL,"open", v_sCSV_File, NULL, NULL, SW_SHOWNORMAL);		//���ļ���Excel.csv
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ�����ܲ�����Ϣ
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Relation::My_LoadData_Para()
{
	int				v_iID=0;											//��ʱ����
	CString			v_sTemp,v_sSql,v_sLog_Key,v_sTable;					//��ʱ����
	CString			v_sType,v_sPm,v_sNum;								//��ʱ����
	CString			v_sMaker,v_sNum_Demo,v_sFile_Name;					//��ʱ����

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
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and name like 'Xm_LTE_%' order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//������

			v_mTable_Name.insert(v_sTable);								//���Ӽ���

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
//	�������ܣ�װ��С����Ϣ
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Relation::My_LoadData_Cell()
{
	CString			v_sTemp,v_sSql;										//��ʱ����
	CString			v_sE_NodeB_ID,v_sCell_ID,v_sCell;					//��ʱ����

	mString_String::iterator	v_pIter;								//������

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SELECT zCell,zCell_Num,LTE_NODEB.zNODEB_Name FROM LTE_Cell left join LTE_NODEB ON LTE_Cell.zNODEB=LTE_NODEB.zNODEB where LTE_NODEB.zRNC='dalian_ZTE'";	//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sE_NodeB_ID = v_cAdoConn.GetField_String("zNODEB_Name");	//E_NodeB_ID
			v_sCell_ID = v_cAdoConn.GetField_String("zCell_Num");		//Cell_ID
			v_sCell = v_cAdoConn.GetField_String("zCell");				//Cell

			if (!v_sE_NodeB_ID.IsEmpty())								//����E_NodeB������
			{
				v_pIter=v_mCell.find(v_sE_NodeB_ID+"|"+v_sCell_ID);		//���ң�����
				if (v_pIter==v_mCell.end())
					v_mCell.insert(pair<CString,CString>(v_sE_NodeB_ID+"|"+v_sCell_ID,v_sCell));	//���Ӽ���
				else
				{
					int aaa=1;											//�쳣
				}
			}

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
//	�������ܣ���ȡ�ļ��ֶ�(��ʾ)
//	��	  ����v_mField_Main���ֶ�������
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Relation::My_Get_File_Field(mString_Int* v_mField,CString v_sTable)
{
	CString			v_sSql;												//��ʱ����
	CString			v_sTemp,v_sName;									//��ʱ����

	//��ȡ��������
	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��1����ȡ�ֶ�����
		v_sSql="select name from syscolumns where id=OBJECT_ID('"+v_sTable+"')";	//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sName = v_cAdoConn.GetField_String("name");				//������

			if (v_sName!="zObject" && v_sName!="zTime" && v_sName!="zDN")
				v_mField->insert(pair<CString,int>(v_sName,0));			//����������v_iNum������š�

			v_pRecordset->MoveNext();									//ָ��++
		}

		v_cAdoConn.ExitConnect();										//�Ͽ�����

		//��2���ֶ���������
		mString_Int::iterator	v_pIter;								//������
		int						v_iNum=4;								//��ʱ����
		for (v_pIter=v_mField->begin();v_pIter!=v_mField->end();v_pIter++)		//����
		{
			v_pIter->second=v_iNum;										//�ֶ����
			v_iNum++;													//������
		}
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
