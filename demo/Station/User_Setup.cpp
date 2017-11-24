// User_Setup.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "User_Setup.h"


// CUser_Setup �Ի���

IMPLEMENT_DYNAMIC(CUser_Setup, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CUser_Setup::CUser_Setup(CWnd* pParent /*=NULL*/)
	: CDialog(CUser_Setup::IDD, pParent)
{
	v_iList_State=0;													//��־����ѯ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CUser_Setup::~CUser_Setup()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CUser_Setup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_EDIT1, m_cSum);
	DDX_Control(pDX, IDC_POOL, m_cRole);
	DDX_Control(pDX, IDC_ADD, m_cAdd);
	DDX_Control(pDX, IDC_MODIFY, m_cModify);
	DDX_Control(pDX, IDC_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_EDIT2, m_cName);
	DDX_Control(pDX, IDC_PASSWORD, m_cClear);
	DDX_Control(pDX, IDC_EDIT6, m_cAccount);
	DDX_Control(pDX, IDC_SEND_FLAG, m_cFlag);
	DDX_Control(pDX, IDC_EDIT7, m_cMarker);
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CUser_Setup, CDialog)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CUser_Setup::OnLvnColumnclickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CUser_Setup::OnNMClickList1)
	ON_BN_CLICKED(IDC_ADD, &CUser_Setup::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_MODIFY, &CUser_Setup::OnBnClickedModify)
	ON_BN_CLICKED(IDC_PASSWORD, &CUser_Setup::OnBnClickedPassword)
	ON_BN_CLICKED(IDC_DELETE, &CUser_Setup::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_EXCEL, &CUser_Setup::OnBnClickedExcel)
	ON_BN_CLICKED(IDC_ADD2, &CUser_Setup::OnBnClickedAdd2)
	ON_BN_CLICKED(IDC_BUTTON1, &CUser_Setup::OnBnClickedButton1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CUser_Setup ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CUser_Setup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ�����б�
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"�û�����",LVCFMT_CENTER,80);  
	m_cList.InsertColumn(2,"�˺�",LVCFMT_LEFT,100);  
	m_cList.InsertColumn(3,"��ɫ",LVCFMT_LEFT,100);  
	m_cList.InsertColumn(4,"��������",LVCFMT_CENTER,120);  
	m_cList.InsertColumn(5,"��Ч",LVCFMT_CENTER,50);  
	m_cList.InsertColumn(6,"��ע",LVCFMT_LEFT,290);  
	m_cList.InsertColumn(7,"Auto_ID",LVCFMT_CENTER,0);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//������ʼ��
	v_iList_Item=-1;													//��Ŀѡ����Ч[-1]

	//��Ϣ��ʾ
	My_GetRole();														//��ã�USER_Role
	m_cRole.SetCurSel(0);												//Ĭ�ϣ���һ��Pool
	My_LoadData();														//���֣����ݱ���Ϣ

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CUser_Setup::PreTranslateMessage(MSG* pMsg)
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
void CUser_Setup::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	int v_iX=830;														//�Ի������
	int v_iY=556;														//�Ի�������
	int v_iY_Div=115;													//�Ի������ز��ֳ���

	MoveWindow(0,0,v_iX,(v_iList_State!=0?v_iY:(v_iY-v_iY_Div)));		//�����Ի����ߴ�
	CenterWindow(); 
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����(�һ��б�������)
//------------------------------------------------------------------------------------------------------			
void CUser_Setup::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����б���Ӧ
//------------------------------------------------------------------------------------------------------			
void CUser_Setup::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if((v_iList_Item>=0) && (v_iList_Item<m_cList.GetItemCount()))	//��Ч��Ŀ��ţ�����
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));		//�ָ�����ɫ
		v_iList_Item=pNMListView->iItem;								//��ã�ѡ����Ŀ���
		v_sList_Item_ID=m_cList.GetItemText(v_iList_Item,7);			//��ã�ѡ����Ŀ��ID
		m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));				//���ñ���ɫ����ɫ
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����
//------------------------------------------------------------------------------------------------------
void CUser_Setup::OnBnClickedAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PostMessage(WM_SIZE);
	m_cAdd.EnableWindow(false);											//"����"����Ч
	m_cModify.EnableWindow(false);										//"�޸�"����Ч
	m_cClear.EnableWindow(false);										//"�������"����Ч
	m_cDelete.EnableWindow(false);										//"ɾ��"����Ч
	m_cExcel.EnableWindow(false);										//"����"����Ч
	m_cName.EnableWindow(true);											//"����"����Ч
	m_cRole.SetCurSel(0);												//Ĭ�ϣ���һ��Role

	//��ʼ������ֵ
	m_cName.SetWindowText("");											//
	m_cAccount.SetWindowText("");										//
	m_cFlag.SetCheck(false);											//
	m_cMarker.SetWindowText("");										//

	v_iList_State=1;													//��־������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��޸�
//------------------------------------------------------------------------------------------------------
void CUser_Setup::OnBnClickedModify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (v_iList_Item<0)
	{
		MessageBox("����ѡ��һ����Ч���û�  ������");					//������ʾ
		m_cList.SetFocus();												//�۽�
		return;
	}
	m_cName.SetWindowText(m_cList.GetItemText(v_iList_Item,1));			//zName
	m_cAccount.SetWindowText(m_cList.GetItemText(v_iList_Item,2));		//zAccount
	m_cRole.SelectString(0,m_cList.GetItemText(v_iList_Item,3));		//zRole_Name
	CString v_sFlag=m_cList.GetItemText(v_iList_Item,5);				//zFlag
	m_cFlag.SetCheck(v_sFlag=="��"?1:0);								//
	m_cMarker.SetWindowText(m_cList.GetItemText(v_iList_Item,6));		//zMarker

	PostMessage(WM_SIZE);
	m_cAdd.EnableWindow(false);											//"����"����Ч
	m_cModify.EnableWindow(false);										//"�޸�"����Ч
	m_cClear.EnableWindow(false);										//"�������"����Ч
	m_cDelete.EnableWindow(false);										//"ɾ��"����Ч
	m_cExcel.EnableWindow(false);										//"����"����Ч
	m_cName.EnableWindow(false);										//"MSC����"����Ч

	v_iList_State=2;													//��־������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������
//------------------------------------------------------------------------------------------------------
void CUser_Setup::OnBnClickedPassword()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql;													//��ʱ����

	if (v_iList_Item<0)
		MessageBox("����ѡ��һ����Ч���û�  ������");					//������ʾ
	else
	{
		if(MessageBox("�Ƿ�����û����룺 "+m_cList.GetItemText(v_iList_Item,1)+"  ��","�û��������ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
		{
			v_sSql="update BAS_User set zPassword='' where zAuto_ID="+v_sList_Item_ID;
			My_ExecuteSQL(v_sSql);										//ɾ��
			//�б���ʾ
			My_LoadData();												//��ʾ
		}
	}
	m_cList.SetFocus();													//�۽�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ɾ��
//------------------------------------------------------------------------------------------------------
void CUser_Setup::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql;													//��ʱ����

	if (v_iList_Item<0)
		MessageBox("����ѡ��һ����Ч���û�  ������");					//������ʾ
	else
	{
		if(MessageBox("�Ƿ�ɾ���û��� "+m_cList.GetItemText(v_iList_Item,1),"�û�ɾ��ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
		{
			v_sSql="delete from BAS_User where zAuto_ID="+v_sList_Item_ID;	//SQL
			My_ExecuteSQL(v_sSql);										//ɾ��
			//�б���ʾ
			My_LoadData();												//��ʾ
		}
	}
	m_cList.SetFocus();													//�۽�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Excel
//------------------------------------------------------------------------------------------------------
void CUser_Setup::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
	m_cList.SetFocus();													//�۽�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��޸�/����ȷ��
//------------------------------------------------------------------------------------------------------
void CUser_Setup::OnBnClickedAdd2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql,v_sRole_ID,v_sFlag,v_sDate;						//��ʱ����
	CString		v_sName,v_sAccount,v_sRole_Name,v_sMarker;				//��ʱ����
	COleDateTime v_dDate_Role;											//����

	UpdateData(true);													//��ã���Ļ��Ϣ

	m_cAccount.GetWindowText(v_sName);									//��ã�����
	if (v_sName.GetLength()==0)
		MessageBox("�������û�����  ������");							//������ʾ
	else
	{
		if (v_iList_State==1 && !My_New_Name(v_sName))					//�����ظ����
			MessageBox("���ӵ��û������ظ� ������");					//������ʾ
		else
		{
			m_cName.GetWindowText(v_sName);								//zName
			v_sName.Trim();
			m_cAccount.GetWindowText(v_sAccount);						//zAccount
			v_sAccount.Trim();
			m_cRole.GetWindowText(v_sRole_Name);						//zRole_Name
			v_sRole_Name.Trim();
			int vi=0;
			mUser_Role::iterator v_pIterator_Role = v_mUser_Role.find(v_sRole_Name);	//����v_sRole_Name
			if(v_pIterator_Role != v_mUser_Role.end())					//���ڣ�
				vi=v_pIterator_Role->second;							//Role_ID
			v_sRole_ID.Format("%d",vi);
			vi=m_cFlag.GetCheck();										//zFlag
			v_sFlag.Format("%d",vi);
			m_cMarker.GetWindowText(v_sMarker);							//zMarker
			v_sMarker.Trim();
			v_dDate_Role=COleDateTime::GetCurrentTime();				//��ã���ǰ����
			v_sDate=v_dDate_Role.Format("%Y-%m-%d %H:%M:%S");			//�ַ���

			if (v_iList_State==1)										//����
				v_sSql="insert into BAS_User(zName,zAccount,zPassword,zRole_ID,zCreat_Time,zFlag,zMarker) values \
					   ('"+v_sName+"','"+v_sAccount+"','"+v_sAccount+"',"+v_sRole_ID+",'"+v_sDate+"',"+v_sFlag+",'"+v_sMarker+"')";
			else														//�޸�
				v_sSql="update BAS_User set zAccount='"+v_sAccount+"',zRole_ID="+v_sRole_ID+",zFlag="+
				v_sFlag+",zMarker='"+v_sMarker+"' where zAuto_ID="+v_sList_Item_ID;
			My_ExecuteSQL(v_sSql);										//����KPI

			My_LoadData();												//����װ��KPI��Ϣ
			OnBnClickedButton1();										//����/�޸Ľ���
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��޸�/����ȡ��/����
//------------------------------------------------------------------------------------------------------
void CUser_Setup::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cAdd.EnableWindow(true);											//"����"����Ч
	m_cModify.EnableWindow(true);										//"�޸�"����Ч
	m_cClear.EnableWindow(true);										//"�������"����Ч
	m_cDelete.EnableWindow(true);										//"ɾ��"����Ч
	m_cExcel.EnableWindow(true);										//"����"����Ч
	m_cName.EnableWindow(true);											//"MSC����"����Ч

	v_iList_State=0;													//��־����ѯ

	PostMessage(WM_SIZE);												//�������
	m_cList.SetFocus();													//�۽�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�USER�˺��ظ����
//	��    �أ�true�����ظ�������ʹ�ã�false�������ظ���������ʹ�ã�
//------------------------------------------------------------------------------------------------------			
bool CUser_Setup::My_New_Name(CString v_sName)
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

		v_sSql="select * from BAS_User where zAccount='"+v_sName+"'";	//��ѯ���ݿ⣺USER
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
//	�������ܣ�װ��USER��Ϣ
//------------------------------------------------------------------------------------------------------			
void CUser_Setup::My_LoadData()
{
	CString			v_sSql;												//��ʱ����
	int				v_iItem,v_iID=0;									//����
	int				v_iAuto_ID,v_iFlag;									//ID
	CString			v_sName,v_sFtp_IP,v_sAccount,v_sMarker,v_sRole_Name;//����
	COleDateTime	v_dDate;											//����
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

		v_sSql="select BAS_User.*,BAS_User_Role.zRole_Name from BAS_User LEFT OUTER JOIN BAS_User_Role ON BAS_User.zRole_ID = BAS_User_Role.zRole_ID order by zName";	//��ѯ���ݿ⣺USER
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_iAuto_ID=v_cAdoConn.GetField_Int("zAuto_ID");				//zAuto_ID
			v_sName=v_cAdoConn.GetField_String("zName");				//zName
			v_sAccount=v_cAdoConn.GetField_String("zAccount");			//zAccount
			v_sRole_Name=v_cAdoConn.GetField_String("zRole_Name");		//zRole_Name
			v_iFlag=v_cAdoConn.GetField_Int("zFlag");					//zFlag
			v_sMarker=v_cAdoConn.GetField_String("zMarker");			//zMarker
			v_dDate=v_cAdoConn.GetField_DataTime("zCreat_Time");		//zCreat_Time

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sName);			//�û�����
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sAccount);			//�û��˺�
			m_cList.SetItemText(v_iItem,3,(_bstr_t)v_sRole_Name);		//��ɫ����
			v_sTemp=v_dDate.Format("%Y-%m-%d_%H:%M:%S");
			m_cList.SetItemText(v_iItem,4,(_bstr_t)v_sTemp);			//��������
			v_sTemp=(v_iFlag==1?"��":"�w");
			m_cList.SetItemText(v_iItem,5,(_bstr_t)v_sTemp);			//��Ч
			m_cList.SetItemText(v_iItem,6,(_bstr_t)v_sMarker);			//��ע
			v_sTemp.Format("%d",v_iAuto_ID);
			m_cList.SetItemText(v_iItem,7,(_bstr_t)v_sTemp);			//Auto_ID

			v_pRecordset->MoveNext();									//ָ������ƶ�
			v_iID++;													//��Ŀ������һ
		}
		v_cAdoConn.ExitConnect();										//�Ͽ�����

		v_sTemp.Format("%d",v_iID);										//�ַ���ת��
		m_cSum.SetWindowTextA(v_sTemp);									//��ʾ����Ŀ����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����Role����
//------------------------------------------------------------------------------------------------------			
void CUser_Setup::My_GetRole()
{
	CString		v_sSql;													//��ʱ����
	int			v_iRole_ID;												//ID
	CString		v_sName;												//����
	CString		v_sTemp;												//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from BAS_User_Role order by zRole_Name";		//��ѯ���ݿ⣺MSC KPI 

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_iRole_ID=v_cAdoConn.GetField_Int("zRole_ID");				//zRole_ID
			v_sName=v_cAdoConn.GetField_String("zRole_Name");			//zRole_Name

			m_cRole.AddString(v_sName);									//Role����
			v_mUser_Role.insert(pair<CString,int>(v_sName,v_iRole_ID));	//��������

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
bool CUser_Setup::My_ExecuteSQL(CString v_sSql)
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
