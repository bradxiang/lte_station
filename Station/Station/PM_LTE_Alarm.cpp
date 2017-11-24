// PM_LTE_Alarm.cpp : ʵ���ļ�
// 


#include "stdafx.h"
#include "Station.h"
#include "PM_LTE_Alarm.h"


// CPM_LTE_Alarm �Ի���

IMPLEMENT_DYNAMIC(CPM_LTE_Alarm, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CPM_LTE_Alarm::CPM_LTE_Alarm(CWnd* pParent /*=NULL*/)
: CDialog(CPM_LTE_Alarm::IDD, pParent)
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CPM_LTE_Alarm::~CPM_LTE_Alarm()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Alarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_EDIT1, m_cSum);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_CHECK1, m_cPm_Flag);
	DDX_Control(pDX, IDC_COMBO3, m_cPm_Type);
	DDX_Control(pDX, IDC_BUTTON1, m_cUpdate);
	DDX_Control(pDX, IDC_EDIT3, m_cPm_Name);
	DDX_Control(pDX, IDC_EDIT2, m_cPm_Value);
	DDX_Control(pDX, IDC_EDIT4, m_cKey);
	DDX_Control(pDX, IDC_COMBO5, m_cKey_Select);
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPM_LTE_Alarm, CDialog)
	ON_BN_CLICKED(IDC_EXCEL, &CPM_LTE_Alarm::OnBnClickedExcel)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CPM_LTE_Alarm::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CPM_LTE_Alarm::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_QUARY2, &CPM_LTE_Alarm::OnBnClickedQuary2)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CPM_LTE_Alarm::OnLvnColumnclickList1)
END_MESSAGE_MAP()


// CPM_LTE_Alarm ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CPM_LTE_Alarm::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ʼ�����б�
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);
	m_cList.InsertColumn(1,"����",LVCFMT_LEFT,100);						//
	m_cList.InsertColumn(2,"����",LVCFMT_LEFT,200);						//
	m_cList.InsertColumn(3,"���Ľ���",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(4,"Ԥ��",LVCFMT_CENTER,80);					//
	m_cList.InsertColumn(5,"Ԥ������",LVCFMT_RIGHT,80);					//
	m_cList.InsertColumn(6,"Ԥ������",LVCFMT_CENTER,80);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList.SetNumber(5);												//���ã�����ģʽ

	//������ʼ��
	v_iItem=-1;															//��Ŀѡ����Ч[-1]
	m_cPm_Type.AddString("��");
	m_cPm_Type.AddString("��");
	m_cPm_Type.SetCurSel(0);											//Ĭ��
	m_cUpdate.EnableWindow(false);										//��Ч

	m_cKey_Select.AddString("LTE����");
	m_cKey_Select.AddString("���Ľ���");
	m_cKey_Select.SetCurSel(0);											//Ĭ�ϣ�ѡ���һ��

	//��ȡ��������
	My_LoadData_Per();													//��ȡ��������

	return TRUE;  // return TRUE unless you set the focus to a control
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CPM_LTE_Alarm::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ���������ߴ�
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Alarm::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Excel
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Alarm::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
	m_cList.SetFocus();													//�۽�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ѯ
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Alarm::OnBnClickedQuary2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	My_LoadData_Per();													//��ȡ��������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Alarm::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����澯�����б�
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Alarm::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp;												//��ʱ����

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if(v_iItem>=0)													//��Ч��Ŀ��ţ�����
			m_cList.SetItemBkColor(v_iItem,RGB(255,255,255));			//�ָ�����ɫ
		v_iItem=pNMListView->iItem;										//��ã�ѡ����Ŀ���
		m_cList.SetItemBkColor(v_iItem,RGB(255,0,0));					//���ñ���ɫ����ɫ
		
		//���ã��޸�����
		m_cPm_Name.SetWindowText(m_cList.GetItemText(v_iItem,3));		//��������
		if (m_cList.GetItemText(v_iItem,4)=="Ԥ��")
			m_cPm_Flag.SetCheck(1);										//Ԥ��
		else
			m_cPm_Flag.SetCheck(0);										//��Ԥ��
		v_sTemp=m_cList.GetItemText(v_iItem,5);							//Ԥ������
		v_sTemp.Trim();
		m_cPm_Value.SetWindowText(v_sTemp);								//Ԥ������
		if (m_cList.GetItemText(v_iItem,6)=="��")
			m_cPm_Type.SetCurSel(0);									//��
		else
			m_cPm_Type.SetCurSel(1);									//��
		m_cUpdate.EnableWindow(true);									//��Ч
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Ԥ�������޸�
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Alarm::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp,v_sSql;											//��ʱ����
	CString		v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;				//��ʱ����
	CString		v_sType,v_sPM;											//��ʱ����

	//��ȡ
	v_sAlarm_Flag=m_cPm_Flag.GetCheck()?"Ԥ��":"��Ԥ��";				//Ԥ����Ч
	m_cPm_Value.GetWindowText(v_sAlarm_Value);							//Ԥ������
	v_sAlarm_Type=m_cPm_Type.GetCurSel()==0?"��":"��";					//Ԥ������
	
	v_sType=m_cList.GetItemText(v_iItem,1);								//��������
	v_sPM=m_cList.GetItemText(v_iItem,2);								//��������

	//��ʾ
	m_cList.SetItemText(v_iItem,4,v_sAlarm_Flag);						//
	m_cList.SetItemText(v_iItem,5,v_sAlarm_Value);						//
	m_cList.SetItemText(v_iItem,6,v_sAlarm_Type);						//

	v_sAlarm_Flag=(v_sAlarm_Flag=="Ԥ��")?"1":"0";						//Ԥ����Ч
	v_sAlarm_Type=(v_sAlarm_Type=="��")?"1":"0";						//Ԥ������
	//����
	v_sSql="Update CON_Pm_LTE set zAlarm_Flag='"+v_sAlarm_Flag+"',zAlarm_Value='"
			+v_sAlarm_Value+"',zAlarm_Type='"+v_sAlarm_Type
			+"' where zType='"+v_sType+"' and zPM='"+v_sPM+"'";			//SQL
	My_ExecuteSQL(v_sSql);												//SQLִ��

	m_cUpdate.EnableWindow(false);										//��Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ�����ܲ�����Ϣ
//------------------------------------------------------------------------------------------------------			
void CPM_LTE_Alarm::My_LoadData_Per()
{
	int				v_iItem,v_iID=0,v_iTemp;							//��ʱ����
	CString			v_sTemp,v_sSql,v_sLog_Key,v_sAlarm_Flag;			//��ʱ����
	CString			v_sType,v_sAlarm_Value,v_sPm,v_sPm_Name,v_sAlarm_Type;	//��ʱ����
	double			v_dTemp;											//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList.DeleteAllItems();										//���Items
		v_iItem=-1;														//��Ŀѡ����Ч

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE ";

		//��ѯ����
		m_cKey.GetWindowText(v_sLog_Key);								//��ã���ѯKEY
		v_sLog_Key.Replace("[","[[]");									//���ƴ���
		v_sLog_Key.Replace("'","");										//���ƴ���
		v_sLog_Key.Trim();												//ȥ���ո�

		if (!v_sLog_Key.IsEmpty())										//��ѯKEY����
		{
			switch (m_cKey_Select.GetCurSel())							//ѡ�񣿣���
			{
			case 0:														//LTE����
				v_sSql+=" where zPm like '%"+v_sLog_Key+"%'";
				break;
			case 1:														//���Ľ���
				v_sSql+=" where  zPm_Name like '%"+v_sLog_Key+"%'";
				break;
			default:
				break;
			}
		}
		v_sSql+=" order by ztype,ztype_1,zPm";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sPm = v_cAdoConn.GetField_String("zPm");					//zPm
			v_sPm_Name = v_cAdoConn.GetField_String("zPm_Name");		//zPm_Name
			v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Flag");				//zAlarm_Flag
			v_sAlarm_Flag=(v_iTemp==1?"Ԥ��":"��Ԥ��");						
			v_dTemp=v_cAdoConn.GetField_Double("zAlarm_Value");			//zAlarm_Value
			v_sAlarm_Value.Format("%6.2f",v_dTemp);
			v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Type");				//zAlarm_Type
			v_sAlarm_Type=(v_iTemp==1?"��":"��");						

			//��ʾ
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,v_sType);						//����
			m_cList.SetItemText(v_iItem,2,v_sPm);						//����
			m_cList.SetItemText(v_iItem,3,v_sPm_Name);					//���Ľ���
			m_cList.SetItemText(v_iItem,4,v_sAlarm_Flag);				//Ԥ����־
			m_cList.SetItemText(v_iItem,5,v_sAlarm_Value);				//Ԥ��ֵ
			m_cList.SetItemText(v_iItem,6,v_sAlarm_Type);				//Ԥ������

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
bool CPM_LTE_Alarm::My_ExecuteSQL(CString v_sSql)
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
