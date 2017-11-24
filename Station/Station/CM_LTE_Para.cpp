// CM_LTE_Para.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "CM_LTE_Para.h"
#include "CM_LTE_Para_Input.h"


// CCM_LTE_Para

IMPLEMENT_DYNCREATE(CCM_LTE_Para, CFormView)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CCM_LTE_Para::CCM_LTE_Para()
	: CFormView(CCM_LTE_Para::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CCM_LTE_Para::~CCM_LTE_Para()
{
	v_mCM_File.clear();													//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_LIST3, m_cList);
	DDX_Control(pDX, IDC_QUARY, m_cInput);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_QUARY2, m_cQuery);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCM_LTE_Para, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CCM_LTE_Para::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL2, &CCM_LTE_Para::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CCM_LTE_Para::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CCM_LTE_Para::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_QUARY2, &CCM_LTE_Para::OnBnClickedQuary2)
	ON_BN_CLICKED(IDC_EXCEL3, &CCM_LTE_Para::OnBnClickedExcel3)
	ON_CBN_CLOSEUP(IDC_COMBO2, &CCM_LTE_Para::OnCbnCloseupCombo2)
END_MESSAGE_MAP()


// CCM_LTE_Para ���

#ifdef _DEBUG
void CCM_LTE_Para::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCM_LTE_Para::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCM_LTE_Para ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��������
	v_sFrame_Name="LTE ���ù���_����";									//��������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//��ȡ���Ӵ�ָ��
	//�Ӵ���������
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//�Ӵ���������

	//��ʼ�����б� [BTS]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"����",LVCFMT_CENTER,80);					//
	m_cList.InsertColumn(2,"�ļ�����",LVCFMT_LEFT,120);					//
	m_cList.InsertColumn(3,"���ò���",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(4,"��������",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(5,"�����ӱ����",LVCFMT_CENTER,100);			//
	m_cList.InsertColumn(6,"�����ӱ�����",LVCFMT_LEFT,200);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList.SetNumber(5);

	//��Ϣ��ʼ��
	m_cKey_Select.AddString("��Ϊ");
	m_cKey_Select.AddString("����");
	m_cKey_Select.SetCurSel(0);											//Ĭ�ϣ�ѡ���һ��

	//��Ϣ��ʼ��
	v_iList_Item=-1;													//��Ŀѡ����Ч

	m_cInput.EnableWindow(false);										//�����ɣ���Ч

	My_Load_File();														//��ȡ�������ļ���Ϣ

	//��ѯ
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ϢԤ���� [��ȡ������Ϣ������]
//------------------------------------------------------------------------------------------------------			
BOOL CCM_LTE_Para::PreTranslateMessage(MSG* pMsg)
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
void CCM_LTE_Para::OnDestroy()
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
void CCM_LTE_Para::OnSize(UINT nType, int cx, int cy)
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
void CCM_LTE_Para::OnBnClickedExcel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ��仯
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::OnCbnCloseupCombo2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedQuary2();												//��ѯ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::OnBnClickedExcel3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCM_LTE_Para_Input	v_cDlg;											//ʵ����
	v_cDlg.v_sMaker_OK=m_cKey_Select.GetCurSel()==0?"��Ϊ":"����";		//����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����Ӧ
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
void CCM_LTE_Para::OnBnClickedQuary2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=1;														//�������񣺲�ѯ

	v_pIterator_Frame->second.v_iThread_Exit=1;							//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ñ�����
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::OnBnClickedQuary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString	v_sFile_Key="",v_sMaker,v_sFile="",v_sNum="";				//��ʱ����
	CString	v_sSql,v_sTime;												//��ʱ����
	COleDateTime v_dTime=COleDateTime::GetCurrentTime();				//��ʱ����

	if (m_cKey_Select.GetCurSel()==0)
		v_sMaker="��Ϊ";
	else
		v_sMaker="����";
	v_sTime=v_dTime.Format("%Y%m");										//����
	
	if(MessageBox("���� LTE ���ã�����----"+v_sMaker+"��  �·�----"+v_sTime+"�� ����ȷ�� ? ","���ñ�����ȷ��",MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)	//����ȷ�ϣ�����
		return;

	if (m_cKey_Select.GetCurSel()==0)
		v_sMaker="HW";
	else
		v_sMaker="ZTE";
	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//�����б�
	{
		if (v_sFile!=m_cList.GetItemText(vi,2) || v_sNum!=m_cList.GetItemText(vi,5))	//������(���ӱ�)�仯������
		{
			if (!v_sFile_Key.IsEmpty())									//�����±�
			{
				v_sSql+=")\r\n";										//����
				v_sSql+="CREATE NONCLUSTERED INDEX PK_Xm_LTE_"+v_sMaker+"_"+v_sFile_Key+"_"+v_sTime
							+" ON Xm_LTE_"+v_sMaker+"_"+v_sFile_Key+"_"+v_sTime+"(zTime,zObject)";	//������
				My_ExecuteSQL(v_sSql);									//ִ��
			}
			v_sFile=m_cList.GetItemText(vi,2);							//�ӱ�����
			v_sNum=m_cList.GetItemText(vi,5);							//�ӱ�����
			v_sNum.Trim();												//ȥ�ո�
			if (v_sNum.IsEmpty())										//Ϊ�գ�����
				v_sFile_Key=v_sFile;									//�����ơ������ӱ�
			else
				v_sFile_Key=v_sFile+"_"+v_sNum;							//�����ơ����ӱ�


			v_sSql="CREATE TABLE Xm_LTE_"+v_sMaker+"_"+v_sFile_Key+"_"+v_sTime
					+"([zObject] [varchar](1000) NULL,[zTime] [datetime] NOT NULL,[zDN] [varchar](3000) NULL";	//ʱ�䡢С��
		}
		v_sSql+=",["+m_cList.GetItemText(vi,3)+"] [varchar](3000) NULL";//�����ֶ�
	}
	if (!v_sFile_Key.IsEmpty())											//�����±�
	{
		v_sSql+=")\r\n";												//����
		v_sSql+="CREATE NONCLUSTERED INDEX PK_Xm_LTE_"+v_sMaker+"_"+v_sFile_Key+"_"+v_sTime
					+" ON Xm_LTE_"+v_sMaker+"_"+v_sFile_Key+"_"+v_sTime+"(zTime,zObject)";	//������
		My_ExecuteSQL(v_sSql);											//ִ��
	}
	MessageBox("LTE���ñ�������� !!!");								//��ʾ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CCM_LTE_Para::My_Thread_Inoput(LPVOID lparam)
{
	CCM_LTE_Para *  lp_this = NULL ;
	lp_this = (CCM_LTE_Para *)lparam;

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
void CCM_LTE_Para::My_Input_Main()
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
void CCM_LTE_Para::My_LoadData_Per()
{
	int				v_iItem,v_iID=0,v_iNum;								//��ʱ����
	CString			v_sTemp,v_sSql,v_sFile_Name,v_sNum_Demo,v_sMaker;	//��ʱ����
	CString			v_sCm,v_sCm_Name,v_sNum;							//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList.DeleteAllItems();										//���Items
		v_iList_Item=-1;												//��Ŀѡ����Ч

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Cm_LTE ";

		//��ѯ����
		switch (m_cKey_Select.GetCurSel())								//ѡ�񣿣���
		{
		case 0:															//��Ϊ
			v_sSql+=" where zMaker='��Ϊ'";
			break;
		case 1:															//����
			v_sSql+=" where zMaker='����'";
			break;
		default:
			break;
		}
		v_sSql+=" order by zFile_Name,zNum,zCm";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sMaker = v_cAdoConn.GetField_String("zMaker");			//zMaker
			v_sFile_Name = v_cAdoConn.GetField_String("zFile_Name");	//zFile_Name
			v_sCm = v_cAdoConn.GetField_String("zCm");					//zCm
			v_sCm_Name = v_cAdoConn.GetField_String("zCm_Name");		//zCm_Name
			v_iNum = v_cAdoConn.GetField_Int("zNum");					//zNum

			//��ʾ
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,v_sMaker);					//����
			m_cList.SetItemText(v_iItem,2,v_sFile_Name);				//�ļ�����
			m_cList.SetItemText(v_iItem,3,v_sCm);						//���ò���
			m_cList.SetItemText(v_iItem,4,v_sCm_Name);					//���Ľ���
			if (v_iNum==0)
				v_sNum="";												//�ո�
			else
				v_sNum.Format("%d",v_iNum);								//�ַ���
			m_cList.SetItemText(v_iItem,5,v_sNum);						//�ӱ����
			if (v_iNum!=0)
			{
				mString_String::iterator v_pFile=v_mCM_File.find(v_sMaker+v_sFile_Name+v_sNum);	//���ң���
				if (v_pFile!=v_mCM_File.end())
					m_cList.SetItemText(v_iItem,6,v_pFile->second);		//�ӱ�����
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
//	�������ܣ�װ��CM�ļ���Ϣ
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Para::My_Load_File()
{
	int				v_iID=0,v_iNum;										//��ʱ����
	CString			v_sTemp,v_sSql,v_sNum_Demo,v_sMaker;				//��ʱ����
	CString			v_sFile_Name,v_sFile_Demo,v_sNum;					//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList.DeleteAllItems();										//���Items
		v_iList_Item=-1;												//��Ŀѡ����Ч

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Cm_LTE_File";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//ǿ���˳�������
				break;													//�˳�

			v_sMaker = v_cAdoConn.GetField_String("zMaker");			//zMaker
			v_sNum_Demo = v_cAdoConn.GetField_String("zNum_Demo");		//zNum_Demo
			v_sFile_Name = v_cAdoConn.GetField_String("zFile_Name");	//zFile_Name
			v_sFile_Demo = v_cAdoConn.GetField_String("zFile_Demo");	//zFile_Demo
			v_iNum = v_cAdoConn.GetField_Int("zNum");					//zNum
			
			if (v_iNum!=0)
			{
				v_sNum.Format("%d",v_iNum);								//�ַ���
				v_mCM_File.insert(pair<CString,CString>(v_sMaker+v_sFile_Name+v_sNum,v_sNum_Demo));	//��������
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
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CCM_LTE_Para::My_ExecuteSQL(CString v_sSql)
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
