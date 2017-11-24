// Pm_GSM_BSC_Add.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Pm_GSM_BSC_Add.h"


// CPm_GSM_BSC_Add �Ի���

IMPLEMENT_DYNAMIC(CPm_GSM_BSC_Add, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CPm_GSM_BSC_Add::CPm_GSM_BSC_Add(CWnd* pParent /*=NULL*/)
	: CDialog(CPm_GSM_BSC_Add::IDD, pParent)
{
	v_iList_State=0;													//��־����ѯ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CPm_GSM_BSC_Add::~CPm_GSM_BSC_Add()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC_Add::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_EDIT7, m_cText);
	DDX_Control(pDX, IDC_EDIT3, m_cObjType);
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPm_GSM_BSC_Add, CDialog)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CPm_GSM_BSC_Add::OnLvnColumnclickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CPm_GSM_BSC_Add::OnNMClickList1)
	ON_BN_CLICKED(IDC_ADD, &CPm_GSM_BSC_Add::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_MODIFY, &CPm_GSM_BSC_Add::OnBnClickedModify)
	ON_BN_CLICKED(IDC_DELETE, &CPm_GSM_BSC_Add::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_EXCEL, &CPm_GSM_BSC_Add::OnBnClickedExcel)
	ON_BN_CLICKED(IDC_ADD2, &CPm_GSM_BSC_Add::OnBnClickedAdd2)
	ON_BN_CLICKED(IDC_BUTTON1, &CPm_GSM_BSC_Add::OnBnClickedButton1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, &CPm_GSM_BSC_Add::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPm_GSM_BSC_Add ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CPm_GSM_BSC_Add::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ�����б�
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"��������",LVCFMT_LEFT,200);  
	m_cList.InsertColumn(2,"����",LVCFMT_LEFT,60);  
	m_cList.InsertColumn(3,"���㹫ʽ",LVCFMT_LEFT,300);  
	m_cList.InsertColumn(4,"ObjType",LVCFMT_LEFT,160);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//������ʼ��
	v_iList_Item=-1;													//��Ŀѡ����Ч[-1]

	//����
	m_cRole.AddString("DATA");
	m_cRole.AddString("PM");
	m_cRole.SetCurSel(0);												//Ĭ��


	//��Ϣ��ʾ
	My_LoadData();														//���֣����ݱ���Ϣ

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CPm_GSM_BSC_Add::PreTranslateMessage(MSG* pMsg)
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
void CPm_GSM_BSC_Add::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	int v_iX=830;														//�Ի������
	int v_iY=694;														//�Ի�������
	int v_iY_Div=115;													//�Ի������ز��ֳ���

	MoveWindow(0,0,v_iX,(v_iList_State!=0?v_iY:(v_iY-v_iY_Div)));		//�����Ի����ߴ�
	CenterWindow(); 
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����¡Ʊ�
//------------------------------------------------------------------------------------------------------			
void CPm_GSM_BSC_Add::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString	v_sSql;														//��ʱ����

	//ɾ����BSC_PM��
	v_sSql="DROP TABLE BSC_PM";											//ɾ����
	My_ExecuteSQL(v_sSql);												//ִ��

	//���ɣ�BSC_PM��
	v_sSql="CREATE TABLE BSC_PM([zdate] [varchar](8) NOT NULL,[zbsc] [varchar](10) NOT NULL";		//ʱ�䡢BSC
	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//�����б�
	{
		v_sSql+=",["+m_cList.GetItemText(vi,1)+"] [float] NULL";		//�����ֶ�
	}
	v_sSql+=")";														//SQL
	My_ExecuteSQL(v_sSql);												//ִ��
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��б�����(�һ��б�������)
//------------------------------------------------------------------------------------------------------			
void CPm_GSM_BSC_Add::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cList.My_Sort(pNMHDR);											//�б�����

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����б���Ӧ
//------------------------------------------------------------------------------------------------------			
void CPm_GSM_BSC_Add::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if((v_iList_Item>=0) && (v_iList_Item<m_cList.GetItemCount()))	//��Ч��Ŀ��ţ�����
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));		//�ָ�����ɫ
		v_iList_Item=pNMListView->iItem;								//��ã�ѡ����Ŀ���
		m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));				//���ñ���ɫ����ɫ
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC_Add::OnBnClickedAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PostMessage(WM_SIZE);
	m_cAdd.EnableWindow(false);											//"����"����Ч
	m_cModify.EnableWindow(false);										//"�޸�"����Ч
	m_cDelete.EnableWindow(false);										//"ɾ��"����Ч
	m_cExcel.EnableWindow(false);										//"����"����Ч
	m_cName.EnableWindow(true);											//"����"����Ч
	m_cRole.SetCurSel(0);												//Ĭ�ϣ���һ��Role

	//��ʼ������ֵ
	m_cName.SetWindowText("");											//

	v_iList_State=1;													//��־������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��޸�
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC_Add::OnBnClickedModify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (v_iList_Item<0)
	{
		MessageBox("����ѡ��һ����Ч������  ������");					//������ʾ
		m_cList.SetFocus();												//�۽�
		return;
	}
	m_cName.SetWindowText(m_cList.GetItemText(v_iList_Item,1));			//zName
	m_cRole.SelectString(0,m_cList.GetItemText(v_iList_Item,2));		//zType
	m_cText.SetWindowText(m_cList.GetItemText(v_iList_Item,3));			//zFlrmla
	m_cObjType.SetWindowText(m_cList.GetItemText(v_iList_Item,4));		//zObjType

	PostMessage(WM_SIZE);
	m_cAdd.EnableWindow(false);											//"����"����Ч
	m_cModify.EnableWindow(false);										//"�޸�"����Ч
	m_cDelete.EnableWindow(false);										//"ɾ��"����Ч
	m_cExcel.EnableWindow(false);										//"����"����Ч
	m_cName.EnableWindow(false);										//"MSC����"����Ч

	v_iList_State=2;													//��־������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ɾ��
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC_Add::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql;													//��ʱ����

	if (v_iList_Item<0)
		MessageBox("����ѡ��һ����Ч������  ������");					//������ʾ
	else
	{
		if(MessageBox("�Ƿ�ɾ�����ܣ� "+m_cList.GetItemText(v_iList_Item,1),"����ɾ��ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
		{
			v_sSql="delete from A_Cap_All_Info where zPM_Name='"+m_cList.GetItemText(v_iList_Item,1)+"'";	//SQL
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
void CPm_GSM_BSC_Add::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
	m_cList.SetFocus();													//�۽�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��޸�/����ȷ��
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC_Add::OnBnClickedAdd2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql;													//��ʱ����
	CString		v_sName,v_sFormula,v_sType,v_sObjType;					//��ʱ����

	m_cName.GetWindowText(v_sName);
	v_sName.Trim();
	m_cObjType.GetWindowText(v_sObjType);
	v_sObjType.Trim();
	m_cText.GetWindowText(v_sFormula);
	v_sFormula.Trim();

	if (v_iList_State==1 && !My_New_Name(v_sName))						//�����ظ����
		MessageBox("���ӵ����������ظ� ������");						//������ʾ
	else
	{
		m_cRole.GetWindowText(v_sType);									//����
		v_sType.Trim();

		if (v_iList_State==1)											//����
			v_sSql="insert into A_Cap_All_Info(zPM_Name,zType,zCounter,zObjType) values \
				   ('"+v_sName+"','"+v_sType+"','"+v_sFormula+"','"+v_sObjType+"')";
		else															//�޸�
			v_sSql="update A_Cap_All_Info set zType='"+v_sType+"',zCounter='"+v_sFormula
					+"',zObjType='"+v_sObjType+"' where zPM_Name='"+v_sName+"'";
		My_ExecuteSQL(v_sSql);											//����

		My_LoadData();													//����װ��KPI��Ϣ
		OnBnClickedButton1();											//����/�޸Ľ���
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��޸�/����ȡ��/����
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC_Add::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cAdd.EnableWindow(true);											//"����"����Ч
	m_cModify.EnableWindow(true);										//"�޸�"����Ч
	m_cDelete.EnableWindow(true);										//"ɾ��"����Ч
	m_cExcel.EnableWindow(true);										//"����"����Ч
	m_cName.EnableWindow(true);											//"MSC����"����Ч

	v_iList_State=0;													//��־����ѯ

	PostMessage(WM_SIZE);												//�������
	m_cList.SetFocus();													//�۽�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��ظ����
//	��    �أ�true�����ظ�������ʹ�ã�false�������ظ���������ʹ�ã�
//------------------------------------------------------------------------------------------------------			
bool CPm_GSM_BSC_Add::My_New_Name(CString v_sName)
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
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from A_Cap_All_Info where zPM_Name='"+v_sName+"'";	//��ѯ���ݿ⣺USER
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
void CPm_GSM_BSC_Add::My_LoadData()
{
	CString			v_sSql;												//��ʱ����
	int				v_iItem,v_iID=0;									//����
	CString			v_sName,v_sType,v_sFormula,v_sObjType;				//����
	CString			v_sTemp;											//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList.DeleteAllItems();										//�б�λ
		v_iList_Item=-1;												//��Ŀѡ����Ч[-1]
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from A_Cap_All_Info order by zPM_Name";		//��ѯ���ݿ�
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sName=v_cAdoConn.GetField_String("zPM_Name");				//zPm_Name
			v_sType=v_cAdoConn.GetField_String("zType");				//zType
			v_sFormula=v_cAdoConn.GetField_String("zCounter");			//zCounter
			v_sObjType=v_cAdoConn.GetField_String("zObjType");			//zObjType

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sName);			//v_sName
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sType);			//v_sType
			m_cList.SetItemText(v_iItem,3,(_bstr_t)v_sFormula);			//���㹫ʽ
			m_cList.SetItemText(v_iItem,4,(_bstr_t)v_sObjType);			//ObjType

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
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CPm_GSM_BSC_Add::My_ExecuteSQL(CString v_sSql)
{
	// TODO: Add your control notification handler code here
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);

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
