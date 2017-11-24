// LTE_PM_Data_Cal_Select.cpp : ʵ���ļ�
// 


#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Data_Cal_Select.h"


// CLTE_PM_Data_Cal_Select �Ի���

IMPLEMENT_DYNAMIC(CLTE_PM_Data_Cal_Select, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cal_Select::CLTE_PM_Data_Cal_Select(CWnd* pParent /*=NULL*/)
: CDialog(CLTE_PM_Data_Cal_Select::IDD, pParent)
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cal_Select::~CLTE_PM_Data_Cal_Select()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal_Select::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_EDIT1, m_cSum);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_COMBO3, m_cPm_Type);
	DDX_Control(pDX, IDC_BUTTON1, m_cSave);
	DDX_Control(pDX, IDC_EDIT3, m_cPm_Name);
	DDX_Control(pDX, IDC_EDIT2, m_cPm_Value);
	DDX_Control(pDX, IDC_BUTTON4, m_cOK);
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Data_Cal_Select, CDialog)
	ON_BN_CLICKED(IDC_EXCEL, &CLTE_PM_Data_Cal_Select::OnBnClickedExcel)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Data_Cal_Select::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Data_Cal_Select::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CLTE_PM_Data_Cal_Select::OnBnClickedButton4)
END_MESSAGE_MAP()


// CLTE_PM_Data_Cal_Select ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CLTE_PM_Data_Cal_Select::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ʼ�����б�
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,40);			//
	m_cList.InsertColumn(1,"ģ������",LVCFMT_LEFT,120);			//
	m_cList.InsertColumn(2,"����",LVCFMT_LEFT,60);				//
	m_cList.InsertColumn(3,"���㹫ʽ",LVCFMT_LEFT,380);			//
	m_cList.InsertColumn(4,"��������",LVCFMT_CENTER,80);		//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//������ʼ��
	v_iItem=-1;															//��Ŀѡ����Ч[-1]
	m_cOK.EnableWindow(false);											//��Ч

	//��Ϣ��ʾ����ȡ�Զ���ļ�������
	My_Load_Cal();														//��ȡ�Զ���ļ�������

	//��ʾ����������
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	for (v_pLTE_PM_Type=v_mLTE_PM_Type->begin();v_pLTE_PM_Type!=v_mLTE_PM_Type->end();v_pLTE_PM_Type++)
		m_cPm_Type.AddString(v_pLTE_PM_Type->first);					//��������
	m_cPm_Type.SetCurSel(0);											//Ĭ��

	return TRUE;  // return TRUE unless you set the focus to a control
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Data_Cal_Select::PreTranslateMessage(MSG* pMsg)
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
void CLTE_PM_Data_Cal_Select::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Excel
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal_Select::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������б�
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal_Select::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int		v_iSel;														//��ʱ����

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if(v_iItem>=0)													//��Ч��Ŀ��ţ�����
			m_cList.SetItemBkColor(v_iItem,RGB(255,255,255));			//�ָ�����ɫ
		v_iItem=pNMListView->iItem;										//��ã�ѡ����Ŀ���
		m_cList.SetItemBkColor(v_iItem,RGB(255,0,0));					//���ñ���ɫ����ɫ
		
		//���ã��޸�����
		m_cPm_Name.SetWindowText(m_cList.GetItemText(v_iItem,1));		//ģ������
		m_cPm_Value.SetWindowText(m_cList.GetItemText(v_iItem,3));		//���㹫ʽ
		v_iSel=m_cPm_Type.FindString(0,m_cList.GetItemText(v_iItem,2));	//��������
		m_cPm_Type.SetCurSel(v_iSel);									//ѡ��

		m_cOK.EnableWindow(true);										//��Ч
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal_Select::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString			v_sSql,v_sName,v_sFormula,v_sType;					//��ʱ����

	m_cPm_Name.GetWindowText(v_sName);									//ģ������
	m_cPm_Type.GetWindowText(v_sType);									//��������
	m_cPm_Value.GetWindowText(v_sFormula);								//���㹫ʽ

	COleDateTime v_tTime=COleDateTime::GetCurrentTime();				//����
	CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");			//

	if (v_sName.IsEmpty() || v_sType.IsEmpty() || v_sFormula.IsEmpty())	//��Ч��Ϣ������
	{
		MessageBox("��Ϣȱʧ�������������Ϣ������");					//��ʾ
		return;															//����
	}

	if (!My_PM_Cal(theApp.a_sUser_Account,v_sName))						//���ڣ��˺�+ģ������ ������
	{																	//����
		v_sSql="Insert into CON_PM_LTE_Case_Cal(zAccount,zName,zType,zPm_Formula,zDate) values ('"
			+theApp.a_sUser_Account+"','"+v_sName+"','"+v_sType+"','"+v_sFormula+"','"+v_sCurrentTime+"')";		//SQL
	}
	else
	{
		v_sSql="Update CON_PM_LTE_Case_Cal set zType='"+v_sType+"',zPm_Formula='"+v_sFormula
			+"' where zAccount='"+theApp.a_sUser_Account+"' and zName='"+v_sName+"'";		//SQL
	}
	My_ExecuteSQL(v_sSql);												//SQLִ��
	MessageBox("�������ܱ������ ������");								//��ʾ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ȷ���˳�
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal_Select::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyFormula		v_cFormula;											//��ʽ����

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	//��1�����ݻ�ȡ
	m_cPm_Type.GetWindowText(v_sType);									//��������
	m_cPm_Value.GetWindowText(v_sFormula);								//���㹫ʽ
	m_cPm_Name.GetWindowText(v_sName);									//ģ������

	if (v_sName.IsEmpty() || v_sType.IsEmpty() || v_sFormula.IsEmpty())	//��Ч��Ϣ������
	{
		MessageBox("��Ϣȱʧ�������������Ϣ������");					//��ʾ
		return;															//����
	}

	//��2����ʽ���(��ʽ�е������Ƿ���ѡ���Ե����ܼ���)
	v_cFormula.My_Get_Formula(v_sFormula);								//�ֽ⹫ʽ���ʽ(����ڶ�����)

	v_pLTE_PM_Type=v_mLTE_PM_Type->find(v_sType);						//���ң���������
	for (v_cFormula.v_pFormula=v_cFormula.v_mFormula.begin();
		v_cFormula.v_pFormula!=v_cFormula.v_mFormula.end();
		v_cFormula.v_pFormula++)										//������ʽԪ��
	{
		if (v_cFormula.v_pFormula->v_bFormula==1)						//����ֵ������
		{
			v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.find(v_cFormula.v_pFormula->v_sPM_Formula);	//���ң���ʽԪ���Ƿ���ԭʼ���ܣ�����
			if (v_pPM_Name==v_pLTE_PM_Type->second.v_mLTE_PM.end())		//û�ҵ�
			{
				MessageBox("���㹫ʽ�к�����Ч�����ܲ�����"+v_cFormula.v_pFormula->v_sPM_Formula+"  !!!");	//������ʾ
				return;													//��Ч����
			}
		}
	}

	//��3����ȷ�˳�
	OnOK();																//�˳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ�Զ����������
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal_Select::My_Load_Cal()
{
	int				v_iItem,v_iID=0;									//��ʱ����
	CString			v_sTemp,v_sDate,v_sSql;								//��ʱ����
	CString			v_sName,v_sFormula,v_sType;							//��ʱ����
	COleDateTime	v_dDate;											//��ʱ����

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT * FROM CON_PM_LTE_Case_Cal where zAccount='"+theApp.a_sUser_Account+"' order by zName";

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		while(!v_pRecordset->adoEOF)
		{
			v_sName=v_cAdoConn.GetField_String("zName");				//zName
			v_sType=v_cAdoConn.GetField_String("zType");				//����
			v_sFormula=v_cAdoConn.GetField_String("zPm_Formula");		//���㹫ʽ
			v_dDate = v_cAdoConn.GetField_DataTime("zDate");			//zDate
			v_sDate = v_dDate.Format(_T("%Y-%m-%d"));					//�ַ���

			//��ʾ
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//�����б�
			m_cList.SetItemText(v_iItem,1,v_sName);						//ģ������
			m_cList.SetItemText(v_iItem,2,v_sType);						//����
			m_cList.SetItemText(v_iItem,3,v_sFormula);					//���㹫ʽ
			m_cList.SetItemText(v_iItem,4,v_sDate);						//����

			v_pRecordset->MoveNext();									//
			v_iID++;													//�������澯����
		}
		v_sTemp.Format("%d",v_iID);										//
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
//	�������ܣ��˺š�ģ����ڣ�����
//------------------------------------------------------------------------------------------------------
bool CLTE_PM_Data_Cal_Select::My_PM_Cal(CString v_sAccount,CString v_sName)
{
	CString			v_sTemp,v_sSql;										//��ʱ����
	bool			v_bok=false;										//��ʱ����

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT * FROM CON_PM_LTE_Case_Cal where zAccount='"+v_sAccount+"' and zName='"+v_sName+"'";

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		if(!v_pRecordset->adoEOF)
			v_bok=true;													//����

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}
	
	return v_bok;														//����
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CLTE_PM_Data_Cal_Select::My_ExecuteSQL(CString v_sSql)
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
