// LTE_PM_DATA_CASE.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include <vector>
#include "LTE_PM_DATA_CASE.h"
#include "LTE_PM_Data.h"


// LTE_PM_DATA_CASE �Ի���

IMPLEMENT_DYNAMIC(LTE_PM_DATA_CASE, CDialog)

LTE_PM_DATA_CASE::LTE_PM_DATA_CASE(CWnd* pParent /*=NULL*/)
	: CDialog(LTE_PM_DATA_CASE::IDD, pParent)
{

}

LTE_PM_DATA_CASE::~LTE_PM_DATA_CASE()
{
	v_mCell.clear();													//�������
}



//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(LTE_PM_DATA_CASE, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_enter, &LTE_PM_DATA_CASE::OnBnClickedenter)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &LTE_PM_DATA_CASE::OnNMClickList2)

	ON_BN_CLICKED(IDC_CHECK1, &LTE_PM_DATA_CASE::OnBnClickedCheck1)
END_MESSAGE_MAP()
// LTE_PM_DATA_CASE ��Ϣ�������
//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void LTE_PM_DATA_CASE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2,m_cList);
	DDX_Control(pDX, IDC_LIST7,m_cList_Cell);
	DDX_Control(pDX, IDC_CHECK1, m_cCheck);
	DDX_Control(pDX, IDC_LOG_SUM, m_cSum);
}
// LTE_PM_DATA_CASE ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL LTE_PM_DATA_CASE::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ʼ�����б� ����ѡ���ܡ�
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,40);					//
	m_cList.InsertColumn(1,"�ʺ�",LVCFMT_LEFT,60);						//
	m_cList.InsertColumn(2,"ģ��",LVCFMT_LEFT,150);						//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��ʼ�����б� ����ѡ���ܡ�
	::SendMessage(m_cList_Cell.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Cell.InsertColumn(0,"���",LVCFMT_CENTER,60);				//
	if(v_iSelect==0)
		m_cList_Cell.InsertColumn(1,"С��",LVCFMT_LEFT,280);			//
	else
	{
		SetWindowText((LPCTSTR)"����ģ��");
		m_cList_Cell.InsertColumn(1,"����",LVCFMT_LEFT,100);			//
		m_cList_Cell.InsertColumn(2,"����",LVCFMT_LEFT,280);			//
		m_cList_Cell.InsertColumn(3,"Ԥ����־",LVCFMT_LEFT,0);			//
		m_cList_Cell.InsertColumn(4,"Ԥ��ֵ",LVCFMT_LEFT,0);			//
		m_cList_Cell.InsertColumn(5,"Ԥ������",LVCFMT_LEFT,0);			//
	}
	m_cList_Cell.SetExtendedStyle(m_cList_Cell.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES );

	v_iList_Item=-1;													//Ĭ�ϣ���ѡ�е��б�Item

	My_LoadData_Case();													//װ�ط�����Ϣ

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL LTE_PM_DATA_CASE::PreTranslateMessage(MSG* pMsg)
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
void LTE_PM_DATA_CASE::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��رմ���
//------------------------------------------------------------------------------------------------------
void LTE_PM_DATA_CASE::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnClose();													//�˳�	
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ȫѡ��С�������ܡ�
//------------------------------------------------------------------------------------------------------
void LTE_PM_DATA_CASE::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int vi=0;vi<m_cList_Cell.GetItemCount();vi++)					//����
		m_cList_Cell.SetCheck(vi,m_cCheck.GetCheck());					//����״̬
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ȷ��
//	��    �أ�(1)С����С����Ϣ������v_mCell�У�(2)���ܣ�������Ϣ������v_mLTE_PM_Type(ָ��)�У�
//------------------------------------------------------------------------------------------------------
void LTE_PM_DATA_CASE::OnBnClickedenter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sPM_Name,v_sType;										//��ʱ����
	CString		v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;				//��ʱ����

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	//��1����λ����ʼ��
	v_mCell.clear();													//�������
	if (v_iSelect==1)													//����
	{
		v_pLTE_PM_Type=v_mLTE_PM_Type->begin();							//��ָ��
		while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())					//����
		{
			for(v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
				v_pPM_Name->second.v_bOK=0;								//��λ��ѡ��״̬
			v_pLTE_PM_Type++;
		}
	}

	//��2����ȡ��ģ����Ϣ
	for(int vi=0;vi<m_cList_Cell.GetItemCount();vi++)					//������ģ����Ϣ�б�
	{
		if (m_cList_Cell.GetCheck(vi))
		{
			if(v_iSelect==0)											//С��
				v_mCell.insert(m_cList_Cell.GetItemText(vi,1));			//���ӣ�С��
			else														//����
			{
				//���£�����״̬
				v_sType=m_cList_Cell.GetItemText(vi,1);					//��������
				v_sPM_Name=m_cList_Cell.GetItemText(vi,2);				//��������
				v_sAlarm_Flag=m_cList_Cell.GetItemText(vi,3);			//Ԥ����־
				v_sAlarm_Value=m_cList_Cell.GetItemText(vi,4);			//Ԥ��ֵ
				v_sAlarm_Type=m_cList_Cell.GetItemText(vi,5);			//Ԥ������

				v_pLTE_PM_Type=v_mLTE_PM_Type->begin();					//��ָ��
				while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())			//����
				{
					for(v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
					{
						if(v_pLTE_PM_Type->first==v_sType && v_pPM_Name->second.v_sPM_Name==v_sPM_Name)		//��ȣ��������ƣ�����
						{
							v_pPM_Name->second.v_bOK=1;					//��λ��ѡ��״̬
							v_pPM_Name->second.v_iNum=vi;				//��ȡ�������
							v_pPM_Name->second.v_sAlarm_Flag=v_sAlarm_Flag;			//��ȡ��Ԥ����־
							v_pPM_Name->second.v_dAlarm_Value=atof(v_sAlarm_Value);	//��ȡ��Ԥ��ֵ
							v_pPM_Name->second.v_sAlarm_Type=v_sAlarm_Type;			//��ȡ��Ԥ������
							break;
						}
					}
					v_pLTE_PM_Type++;
				}
			}
		}
	}

	//��3���˳�
	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ģ����Ӧ
//------------------------------------------------------------------------------------------------------
void LTE_PM_DATA_CASE::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sAccount,v_sName;										//��ʱ����

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if(v_iList_Item>=0)												//��Ч��Ŀ��ţ�����
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));		//�ָ�����ɫ
		v_iList_Item=pNMListView->iItem;								//��ã�ѡ����Ŀ���
		m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));				//���ñ���ɫ����ɫ

		//��ȡ��ģ����Ϣ

		m_cList_Cell.DeleteAllItems();									//����б�����
		v_sAccount=m_cList.GetItemText(v_iList_Item,1);					//�ʺ�
		v_sName=m_cList.GetItemText(v_iList_Item,2);					//ģ������
		My_LoadData_Case_All(v_sAccount,v_sName);						//��ȡ��ϸ(С��������)
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��ģ����Ϣ
//------------------------------------------------------------------------------------------------------			
void LTE_PM_DATA_CASE::My_LoadData_Case()
{
	CString		v_sSql;													//��ʱ����
	int			v_iItem,v_iID=0;										//��ʱ����
	CString		v_sTemp,v_sName,v_sAccount;								//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		if(v_iSelect==0)												//С��
			v_sSql="select zAccount,zName from CON_Pm_LTE_Case_Cell where zAccount='����' or zAccount='"
					+theApp.a_sUser_Account+"' order by zAccount,zName";//��ѯ���ݿ�
		else															//����
			v_sSql="select zAccount,zName from CON_Pm_LTE_Case_PM where zAccount='����' or zAccount='"
					+theApp.a_sUser_Account+"' order by zAccount,zName";//��ѯ���ݿ�

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			v_sAccount=v_cAdoConn.GetField_String("zAccount");			//zAccount
			v_sName=v_cAdoConn.GetField_String("zName");				//zName
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sAccount);			//�ʺ�
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sName);			//ģ������

			v_pRecordset->MoveNext();									//
			v_iID++;	
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
//	�������ܣ�����ģ����ϸ��Ϣ��С�������ܡ�
//	��    ����v_sName��ģ�����ƣ�
//------------------------------------------------------------------------------------------------------
void LTE_PM_DATA_CASE::My_LoadData_Case_All(CString v_sAccount,CString v_sName)
{
	CString		v_sSql,v_sCell,v_sType,v_sPM_Name;						//��ʱ����
	int			v_iItem,v_iID=0,v_iTemp;								//��ʱ����
	CString		v_sTemp;												//��ʱ����
	double		v_dTemp;												//��ʱ����
	CString		v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;				//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		if(v_iSelect==0)												//С��
			v_sSql="select * from CON_Pm_LTE_Case_Cell_Data where zAccount='"
				+v_sAccount+"'and zName='"+v_sName+"' order by zCell";	//��ѯ���ݿ�
		else															//����
			v_sSql="select * from CON_Pm_LTE_Case_PM_Data where zAccount='"
				+v_sAccount+"'and zName='"+v_sName+"' order by zNum";	//��ѯ���ݿ�

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			if(v_iSelect==0)											//С��
				v_sCell=v_cAdoConn.GetField_String("zCell");			//zCell
			else
			{															//����
				v_sType=v_cAdoConn.GetField_String("zType");			//zType
				v_sPM_Name=v_cAdoConn.GetField_String("zPM_Name");		//zPM_Name
				v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Flag");			//zAlarm_Flag
				v_sAlarm_Flag=(v_iTemp==1?"Ԥ��":"��Ԥ��");						
				v_dTemp=v_cAdoConn.GetField_Double("zAlarm_Value");		//zAlarm_Value
				v_sAlarm_Value.Format("%8.2f",v_dTemp);
				v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Type");			//zAlarm_Type
				v_sAlarm_Type=(v_iTemp==1?"��":"��");						
			}

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList_Cell.InsertItem(0xffff,v_sTemp);			//�����б�
			if(v_iSelect==0)											//С��
				m_cList_Cell.SetItemText(v_iItem,1,(_bstr_t)v_sCell);	//С��
			else
			{															//����
				m_cList_Cell.SetItemText(v_iItem,1,(_bstr_t)v_sType);	//����
				m_cList_Cell.SetItemText(v_iItem,2,(_bstr_t)v_sPM_Name);//����
				m_cList_Cell.SetItemText(v_iItem,3,v_sAlarm_Flag);		//Ԥ����־
				m_cList_Cell.SetItemText(v_iItem,4,v_sAlarm_Value);		//Ԥ��ֵ
				m_cList_Cell.SetItemText(v_iItem,5,v_sAlarm_Type);		//Ԥ������
			}
			m_cList_Cell.SetCheck(v_iID,TRUE);							//��Ч

			v_pRecordset->MoveNext();									//
			v_iID++;	
		}

		m_cCheck.SetCheck(true);										//��Ч��ȫѡ
		v_sTemp.Format("%d",v_iID);										//����
		m_cSum.SetWindowText(v_sTemp);									//��ʾ������

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
