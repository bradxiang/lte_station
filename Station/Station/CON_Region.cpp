// CON_Region.cpp : ʵ���ļ�
// 


#include "stdafx.h"
#include "Station.h"
#include "CON_Region.h"


// CCON_Region �Ի���

IMPLEMENT_DYNAMIC(CCON_Region, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CCON_Region::CCON_Region(CWnd* pParent /*=NULL*/)
: CDialog(CCON_Region::IDD, pParent)
{
	v_bAdd=false;														//���ӱ�־
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CCON_Region::~CCON_Region()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CCON_Region::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_EDIT1, m_cSum);
	DDX_Control(pDX, IDC_EDIT2, m_cName);
	DDX_Control(pDX, IDC_ADD, m_cAdd);
	DDX_Control(pDX, IDC_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_Region, CDialog)
	ON_BN_CLICKED(IDC_ADD, &CCON_Region::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DELETE, &CCON_Region::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_EXCEL, &CCON_Region::OnBnClickedExcel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ADD2, &CCON_Region::OnBnClickedAdd2)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CCON_Region::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCON_Region::OnBnClickedButton2)
END_MESSAGE_MAP()


// CCON_Region ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CCON_Region::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ʼ�����б�
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"����",LVCFMT_LEFT,200);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//������ʼ��
	v_iItem=-1;															//��Ŀѡ����Ч[-1]

	//��Ϣ��ʾ
	My_LoadData();														//���֣����ݱ���Ϣ

	return TRUE;  // return TRUE unless you set the focus to a control
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CCON_Region::PreTranslateMessage(MSG* pMsg)
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
void CCON_Region::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	int	v_iX,v_iY;
	v_iX=318;															//�Ի������
	v_iY=(v_bAdd?364:(364-80));											//�Ի�������
	MoveWindow(0,0,v_iX,v_iY);											//�����Ի����ߴ�
	CenterWindow(); 
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����
//------------------------------------------------------------------------------------------------------
void CCON_Region::OnBnClickedAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_bAdd=true;														//���ӱ�־
	PostMessage(WM_SIZE);
	m_cAdd.EnableWindow(false);											//"����"����Ч
	m_cDelete.EnableWindow(false);										//"ɾ��"����Ч
	m_cExcel.EnableWindow(false);										//"����"����Ч

	//��ʼ������ֵ
	m_cName.SetWindowText("");											//
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ɾ��
//------------------------------------------------------------------------------------------------------
void CCON_Region::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql;													//��ʱ����

	if (v_iItem<0)
		MessageBox("����ѡ��һ����Ч������  ������");					//������ʾ
	else
	{
		if(MessageBox("�Ƿ�ɾ��ѡ�е�����? ","����_ɾ��ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
		{
			CString v_sAlarm_Name = m_cList.GetItemText(v_iItem,1);
			v_sSql="delete from CON_Region where zRegion='"+v_sAlarm_Name+"'";		//SQL
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
void CCON_Region::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
	m_cList.SetFocus();													//�۽�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����澯�����б�
//------------------------------------------------------------------------------------------------------
void CCON_Region::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if(v_iItem>=0)													//��Ч��Ŀ��ţ�����
			m_cList.SetItemBkColor(v_iItem,RGB(255,255,255));			//�ָ�����ɫ
		v_iItem=pNMListView->iItem;										//��ã�ѡ����Ŀ���
		m_cList.SetItemBkColor(v_iItem,RGB(255,0,0));					//���ñ���ɫ����ɫ
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ȷ��
//------------------------------------------------------------------------------------------------------
void CCON_Region::OnBnClickedAdd2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql,v_sTemp;											//��ʱ����
	CString		v_sName;												//��ʱ����
	COLORREF	v_cBackColor=RGB(255,255,255);							//������ɫ����ɫ

	UpdateData(true);													//��ã���Ļ��Ϣ

	m_cName.GetWindowText(v_sName);										//��ã�����
	if (v_sName.GetLength()==0)
		MessageBox("����������  ������");								//������ʾ
	else
	{
		if (!My_New_Name(v_sName))										//�����ظ����
			MessageBox("���ӵ������ظ� ������");						//������ʾ
		else
		{
			v_sTemp.Format("%d",v_cBackColor);							//�ַ���
			v_sSql="insert into CON_Region(zRegion) values ('"+v_sName+"')";
			My_ExecuteSQL(v_sSql);										//����KPI
			My_LoadData();												//����װ��KPI��Ϣ

			OnBnClickedButton2();										//����/�޸Ľ���
		}
	}
	m_cList.SetFocus();													//�۽�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ȡ��
//------------------------------------------------------------------------------------------------------			
void CCON_Region::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cAdd.EnableWindow(true);											//"����"����Ч
	m_cDelete.EnableWindow(true);										//"ɾ��"����Ч
	m_cExcel.EnableWindow(true);										//"����"����Ч

	v_bAdd=false;														//���ӱ�־
	PostMessage(WM_SIZE);												//�������
	m_cList.SetFocus();													//�۽�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Pool�����ظ����
//	��    �أ�true�����ظ�������ʹ�ã�false�������ظ���������ʹ�ã�
//------------------------------------------------------------------------------------------------------			
bool CCON_Region::My_New_Name(CString v_sName)
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

		v_sSql="select * from CON_Region where zRegion='"+v_sName+"'";	//��ѯ���ݿ⣺MSC KPI 
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
//	�������ܣ�װ�ظ澯������Ϣ
//------------------------------------------------------------------------------------------------------			
void CCON_Region::My_LoadData()
{
	CString		v_sSql;													//��ʱ����
	int			v_iItem,v_iID=0;										//����
	CString		v_sName;												//����
	CString		v_sTemp;												//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		m_cList.DeleteAllItems();										//�б�λ
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from CON_Region order by zRegion";				//��ѯ���ݿ⣺����

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sName=v_cAdoConn.GetField_String("zRegion");				//zRegion

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sName);			//����

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
//	��    ����v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CCON_Region::My_ExecuteSQL(CString v_sSql)
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
