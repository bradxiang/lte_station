// User_Role.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "User_Role.h"


// CUser_Role �Ի���

IMPLEMENT_DYNAMIC(CUser_Role, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CUser_Role::CUser_Role(CWnd* pParent /*=NULL*/)
	: CDialog(CUser_Role::IDD, pParent)
{
	v_iList_State=0;													//��־����ѯ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CUser_Role::~CUser_Role()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CUser_Role::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_LIST2, m_cList_Power);
	DDX_Control(pDX, IDC_CHECK1, m_cRole_Flag);
	DDX_Control(pDX, IDC_STATION_SUM, m_cSum);
	DDX_Control(pDX, IDC_EDIT65, m_cAdd_Role_Name);
	DDX_Control(pDX, IDC_EDIT67, m_cAdd_Role_Marker);
	DDX_Control(pDX, IDC_EDIT69, m_cMod_Role_Marker);
	DDX_Control(pDX, IDC_EDIT72, m_cMod_Role_Name);
	DDX_Control(pDX, IDC_EDIT71, m_cDel_Role_Name);
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CUser_Role, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CUser_Role::OnNMClickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CUser_Role::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_MSC_KPI_Excel, &CUser_Role::OnBnClickedMscKpiExcel)
	ON_BN_CLICKED(IDC_ADD_MSC_KPI2, &CUser_Role::OnBnClickedAddMscKpi2)
	ON_BN_CLICKED(IDC_BUTTON23, &CUser_Role::OnBnClickedButton23)
	ON_BN_CLICKED(IDC_BUTTON25, &CUser_Role::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_CHECK1, &CUser_Role::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_ALL, &CUser_Role::OnBnClickedAll)
	ON_BN_CLICKED(IDC_NULL, &CUser_Role::OnBnClickedNull)
	ON_BN_CLICKED(IDC_SAVE, &CUser_Role::OnBnClickedSave)
END_MESSAGE_MAP()


// CUser_Role ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CUser_Role::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ�����б�
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"��ɫ����",LVCFMT_CENTER,100);  
	m_cList.InsertColumn(2,"��ע",LVCFMT_LEFT,145);  
	m_cList.InsertColumn(3,"Role_ID",LVCFMT_CENTER,0);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	//��ʼ�����б�
	::SendMessage(m_cList_Power.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Power.InsertColumn(0,"                  Ȩ  ��  ��  ��",LVCFMT_LEFT,158);  
	m_cList_Power.InsertColumn(1,"Power_ID",LVCFMT_CENTER,0);  
	m_cList_Power.InsertColumn(2,"Power",LVCFMT_CENTER,0);  
	m_cList_Power.SetExtendedStyle(m_cList_Power.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES );

	//������ʼ��
	v_iList_State=-1;													//��Ŀѡ����Ч[-1]

	//��Ϣ��ʾ
	My_GetPower();														//��ã�Ȩ��
	My_LoadData();														//���֣����ݱ���Ϣ

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CUser_Role::PreTranslateMessage(MSG* pMsg)
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
void CUser_Role::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	int v_iX=805;														//�Ի������
	int v_iX_Div=282;													//�Ի������
	int v_iY=460;														//�Ի�������
	int v_iY_Div=115;													//�Ի������ز��ֳ���

	MoveWindow(0,0,(v_iList_State!=0?v_iX:v_iX-v_iX_Div),v_iY);			//�����Ի����ߴ�
	CenterWindow(); 
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����б���Ӧ
//------------------------------------------------------------------------------------------------------			
void CUser_Role::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if(v_iList_Item>=0) 											//��Ч��Ŀ��ţ�����
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));		//�ָ�����ɫ
		v_iList_Item=pNMListView->iItem;								//��ã�ѡ����Ŀ���
		v_sList_Item_ID=m_cList.GetItemText(v_iList_Item,3);			//��ã�ѡ����Ŀ��ID
		m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));				//���ñ���ɫ����ɫ
		m_cDel_Role_Name.SetWindowText(m_cList.GetItemText(v_iList_Item,1));	//
		m_cMod_Role_Name.SetWindowText(m_cList.GetItemText(v_iList_Item,1));	//
		m_cMod_Role_Marker.SetWindowText(m_cList.GetItemText(v_iList_Item,2));	//

		My_GetPower_OK(v_sList_Item_ID);								//��ȡ����Ȩ��Ȩ��
		My_LoadData_Power();											//���֣�Ȩ����Ϣ
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����(�һ��б�������)
//------------------------------------------------------------------------------------------------------			
void CUser_Role::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ӡ��޸ġ�ɾ����Ч�� [��չ��ʾ����]
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_cRole_Flag.GetCheck())
		v_iList_State=1;												//��־����չ��ʾ��Ч
	else
		v_iList_State=0;												//��־����չ��ʾ��Ч

	PostMessage(WM_SIZE);												//��Ļ�ػ�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Excel
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedMscKpiExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
	m_cList.SetFocus();													//�۽�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ȫѡ
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mUSER_Power::iterator v_pIterator_Power = v_mUSER_Power.begin();	//ͷλ��
	while(v_pIterator_Power != v_mUSER_Power.end())						//������
	{
		v_pIterator_Power->second.v_bOk=true;							//Ȩ����Ч
		v_pIterator_Power++;											//������++
	}
	for (int vi=0;vi<m_cList_Power.GetItemCount();vi++)					//�б�
		m_cList_Power.SetCheck(vi);										//ѡ����Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ȫ��ѡ
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedNull()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mUSER_Power::iterator v_pIterator_Power = v_mUSER_Power.begin();	//ͷλ��
	while(v_pIterator_Power != v_mUSER_Power.end())						//������
	{
		v_pIterator_Power->second.v_bOk=false;							//Ȩ����Ч
		v_pIterator_Power++;											//������++
	}
	for (int vi=0;vi<m_cList_Power.GetItemCount();vi++)					//�б�
		m_cList_Power.SetCheck(vi,0);									//ѡ����Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Ȩ�ޱ���
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp,v_sSql;											//��ʱ����

	//��1���������Ȩ��
	v_sSql="delete from BAS_User_RP where zRole_ID="+v_sList_Item_ID;	//Sql
	My_ExecuteSQL(v_sSql);												//ɾ��
	//��2����ȡ��Ҫ�����Ȩ��
	v_sSql="";															//��ʼ��
	for (int vi=0;vi<m_cList_Power.GetItemCount();vi++)					//�б�
	{
		if (m_cList_Power.GetCheck(vi))									//ѡ��
		{
			v_sSql+="insert into BAS_User_RP(zRole_ID,zPower_ID) values ("
					+v_sList_Item_ID+","+m_cList_Power.GetItemText(vi,1)+")\r\n";	//Sql
		}
	}
	//��3������
	if (!v_sSql.IsEmpty())												//��������Ҫ���棿����
		My_ExecuteSQL(v_sSql);											//����
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedAddMscKpi2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql,v_sTemp;											//��ʱ����
	CString		v_sName,v_sMarker;										//��ʱ����

	UpdateData(true);													//��ã���Ļ��Ϣ

	m_cAdd_Role_Name.GetWindowText(v_sName);							//��ã�����
	if (v_sName.GetLength()==0)
		MessageBox("������Role����  ������");							//������ʾ
	else
	{
		if (!My_New_Name(v_sName))										//�����ظ����
			MessageBox("���ӵ�Role�����ظ� ������");					//������ʾ
		else
		{
			m_cAdd_Role_Name.GetWindowText(v_sName);					//Role Name
			v_sName.Trim();
			m_cAdd_Role_Marker.GetWindowText(v_sMarker);				//Role Marker
			v_sMarker.Trim();

			v_sSql="insert into BAS_User_Role(zRole_Name,zMarker) values ('"+v_sName+"','"+v_sMarker+"')";
			My_ExecuteSQL(v_sSql);										//����KPI

			My_LoadData();												//����װ��KPI��Ϣ
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ɾ��
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedButton23()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql;													//��ʱ����

	if (v_iList_Item<0)
		MessageBox("����ѡ��һ����Ч�Ľ�ɫ  ������");					//������ʾ
	else
	{
		if(MessageBox("�Ƿ�ɾ��ѡ�еĽ�ɫ�� "+m_cList.GetItemText(v_iList_Item,1)+" ��","��ɫɾ��ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
		{
			v_sSql="delete from BAS_User_Role where zRole_ID="+v_sList_Item_ID;	//SQL
			My_ExecuteSQL(v_sSql);										//ɾ����Role��
			v_sSql="delete from BAS_User_RP where zRole_ID="+v_sList_Item_ID;	//SQL
			My_ExecuteSQL(v_sSql);										//ɾ����Role_Power��
			//�б���ʾ
			My_LoadData();												//��ʾ
		}
	}
	m_cList.SetFocus();													//�۽�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��޸�
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedButton25()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql,v_sTemp;											//��ʱ����
	CString		v_sMarker;												//��ʱ����

	UpdateData(true);													//��ã���Ļ��Ϣ

	if (v_iList_Item<0)
		MessageBox("����ѡ��һ����Ч�Ľ�ɫ  ������");					//������ʾ
	else
	{
		m_cMod_Role_Marker.GetWindowText(v_sMarker);					//Role Marker
		v_sMarker.Trim();

		v_sSql="update BAS_User_Role set zMarker='"+v_sMarker+"' where zRole_ID="+v_sList_Item_ID;
		My_ExecuteSQL(v_sSql);											//�޸�

		My_LoadData();													//����װ��KPI��Ϣ
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�USER�˺��ظ����
//	��    �أ�true�����ظ�������ʹ�ã�false�������ظ���������ʹ�ã�
//------------------------------------------------------------------------------------------------------			
bool CUser_Role::My_New_Name(CString v_sName)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool	v_bNum;														//��ʱ����
	CString	v_sSql,v_sTemp;												//��ʱ����

	v_bNum=0;															//��ʼ��
	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from BAS_User_Role where zRole_Name='"+v_sName+"'";	//��ѯ���ݿ⣺USER Role_Name
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		if(v_pRecordset->adoEOF)
			v_bNum=1;

		v_cAdoConn.ExitConnect();										//�Ͽ�����

		return v_bNum;
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
		return v_bNum;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��Role��Ϣ
//------------------------------------------------------------------------------------------------------			
void CUser_Role::My_LoadData()
{
	CString			v_sSql;												//��ʱ����
	int				v_iItem,v_iID=0;									//����
	int				v_iRole_ID;											//ID
	CString			v_sRole_Name,v_sMarker;								//����
	CString			v_sTemp;											//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList.DeleteAllItems();										//�б�λ
		v_iList_Item=-1;												//��Ŀѡ����Ч[-1]
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from BAS_User_Role order by zRole_Name";		//��ѯ���ݿ⣺USER_Role
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_iRole_ID=v_cAdoConn.GetField_Int("zRole_ID");				//zRole_ID
			v_sRole_Name=v_cAdoConn.GetField_String("zRole_Name");		//zRole_Name
			v_sMarker=v_cAdoConn.GetField_String("zMarker");			//zMarker

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sRole_Name);		//��ɫ����
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sMarker);			//��ע
			v_sTemp.Format("%d",v_iRole_ID);
			m_cList.SetItemText(v_iItem,3,(_bstr_t)v_sTemp);			//zRole_ID

			v_pRecordset->MoveNext();									//ָ������ƶ�
			v_iID++;													//��Ŀ������һ
		}
		v_cAdoConn.ExitConnect();										//�Ͽ�����

		v_sTemp.Format("%d",v_iID);										//�ַ���ת��
		m_cSum.SetWindowTextA(v_sTemp);									//��ʾ����Ŀ����

		m_cAdd_Role_Name.SetWindowText("");								//
		m_cAdd_Role_Marker.SetWindowText("");							//
		m_cDel_Role_Name.SetWindowText("");								//
		m_cMod_Role_Name.SetWindowText("");								//
		m_cMod_Role_Marker.SetWindowText("");							//
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��Power��Ϣ
//------------------------------------------------------------------------------------------------------			
void CUser_Role::My_LoadData_Power()
{
	CString			v_sSql;												//��ʱ����
	int				v_iItem;											//����
	CString			v_sRole_Name,v_sMarker;								//����
	CString			v_sTemp;											//��ʱ����

	m_cList_Power.DeleteAllItems();										//�б�λ

	mUSER_Power::iterator v_pIterator_Power = v_mUSER_Power.begin();	//ͷλ��
	while(v_pIterator_Power != v_mUSER_Power.end())						//������
	{
		v_sTemp="    "+v_pIterator_Power->second.v_sPower;
		v_iItem=m_cList_Power.InsertItem(0xffff,v_sTemp);				//�����б�
		v_sTemp.Format("%d",v_pIterator_Power->first);
		m_cList_Power.SetItemText(v_iItem,1,v_sTemp);					//Ȩ��ID
		m_cList_Power.SetItemText(v_iItem,2,v_pIterator_Power->second.v_bOk?"1":"0");	//Ȩ����Ч
		if (v_pIterator_Power->second.v_bOk)							//Ȩ����Ч
			m_cList_Power.SetCheck(v_iItem);							//�ӱ��

		v_pIterator_Power++;											//������++
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ��Ȩ����Ϣ
//	��	  ����v_sRolie_ID��Role_ID
//------------------------------------------------------------------------------------------------------			
void CUser_Role::My_GetPower_OK(CString v_sRolie_ID)
{
	CString			v_sSql;												//��ʱ����
	int				v_iID=0;											//����
	int				v_iRole_ID,v_iPower_ID;								//ID
	CString			v_sTemp;											//��ʱ����

	mUSER_Power::iterator v_pIterator_Power = v_mUSER_Power.begin();	//ͷλ��
	while(v_pIterator_Power != v_mUSER_Power.end())						//������
	{
		v_pIterator_Power->second.v_bOk=false;							//Ȩ����Ч
		v_pIterator_Power++;											//������++
	}

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from BAS_User_RP where zRole_ID="+v_sRolie_ID+" order by zPower_ID";	//��ѯ���ݿ⣺USER_RP
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_iRole_ID=v_cAdoConn.GetField_Int("zRole_ID");				//zRole_ID
			v_iPower_ID=v_cAdoConn.GetField_Int("zPower_ID");			//zPower_ID

			mUSER_Power::iterator v_pIterator_Power = v_mUSER_Power.find(v_iPower_ID);	//����v_iPower_ID
			if(v_pIterator_Power != v_mUSER_Power.end())				//���ڣ�
				v_pIterator_Power->second.v_bOk=true;					//Ȩ����Ч

			v_pRecordset->MoveNext();									//ָ������ƶ�
			v_iID++;													//��Ŀ������һ
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
//	�������ܣ����Ȩ��
//------------------------------------------------------------------------------------------------------			
void CUser_Role::My_GetPower()
{
	CString		v_sSql;													//��ʱ����
	int			v_iPower_ID;											//ID
	CString		v_sPowerName;											//����
	CString		v_sTemp;												//��ʱ����
	USER_Power	v_cPower;												//Ȩ��

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from BAS_User_Power order by zPower_ID";		//��ѯ���ݿ⣺Power

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_iPower_ID=v_cAdoConn.GetField_Int("zPower_ID");			//zPower_ID
			v_sPowerName=v_cAdoConn.GetField_String("zPower_Name");		//zPower_Name

			v_cPower.v_sPower=v_sPowerName;
			v_cPower.v_bOk=false;
			v_mUSER_Power.insert(pair<int,USER_Power>(v_iPower_ID,v_cPower));		//��������[ID/Ȩ��]

			v_pRecordset->MoveNext();									//ָ������ƶ�
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
bool CUser_Role::My_ExecuteSQL(CString v_sSql)
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
