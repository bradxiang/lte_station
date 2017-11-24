// Pm_GSM_UpDate_PM.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Pm_GSM_UpDate_PM.h"


// CPm_GSM_UpDate_PM �Ի���

IMPLEMENT_DYNAMIC(CPm_GSM_UpDate_PM, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CPm_GSM_UpDate_PM::CPm_GSM_UpDate_PM(CWnd* pParent /*=NULL*/)
	: CDialog(CPm_GSM_UpDate_PM::IDD, pParent)
{
	v_bFlag=false;														//��ǣ�0���޸ģ�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CPm_GSM_UpDate_PM::~CPm_GSM_UpDate_PM()
{
	v_mGSM_PM.clear();													//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CPm_GSM_UpDate_PM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO4, m_cPM_Table);
	DDX_Control(pDX, IDC_EDIT2, m_cPM_Name);
	DDX_Control(pDX, IDC_EDIT3, m_cFormula);
	DDX_Control(pDX, IDC_EDIT4, m_cType);
	DDX_Control(pDX, IDC_EDIT5, m_cObjType);
	DDX_Control(pDX, IDC_EDIT7, m_cUnit);
	DDX_Control(pDX, IDOK, m_cOK);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPm_GSM_UpDate_PM, CDialog)
	ON_BN_CLICKED(IDOK, &CPm_GSM_UpDate_PM::OnBnClickedOk)
	ON_CBN_CLOSEUP(IDC_COMBO4, &CPm_GSM_UpDate_PM::OnCbnCloseupCombo4)
END_MESSAGE_MAP()


// CPm_GSM_UpDate_PM ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
// �������ܣ���ʼ��
//------------------------------------------------------------------------------------------------------			
BOOL CPm_GSM_UpDate_PM::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	My_Get_PM_Table();													//��ȡ������������

	m_cPM_Table.SelectString(0,v_sPM_Table);							//��ȡ��������
	m_cType.SetWindowText(v_sType);										//��ȡ������
	m_cPM_Name.SetWindowText(v_sPM_Name);								//��ȡ����������
	m_cUnit.SetWindowText(v_sUnit);										//��ȡ����λ
	m_cFormula.SetWindowText(v_sFormula);								//��ȡ�����㹫ʽ
	m_cObjType.SetWindowText(v_sObjType);								//��ȡ��ObjType

	if (!v_bFlag)														//���ӣ�����
	{
		m_cPM_Table.EnableWindow(false);								//��Ч
		m_cUnit.EnableWindow(false);									//��Ч
		m_cPM_Name.EnableWindow(false);									//��Ч
	}
	else
		m_cOK.SetWindowText("����");									//��ʾ

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CPm_GSM_UpDate_PM::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ�������ѡ��
//------------------------------------------------------------------------------------------------------
void CPm_GSM_UpDate_PM::OnCbnCloseupCombo4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString v_sTemp;													//��ʱ����

	mGSM_PM::iterator	v_pIter;										//������

	m_cPM_Table.GetWindowText(v_sTemp);									//��ȡ��������
	v_pIter=v_mGSM_PM.find(v_sTemp);									//���ң�����
	if (v_pIter!=v_mGSM_PM.end())
		m_cType.SetWindowText(v_pIter->second.v_sType);					//��������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��޸�
//------------------------------------------------------------------------------------------------------
void CPm_GSM_UpDate_PM::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString v_sSql,v_sTemp;												//��ʱ����

	m_cPM_Table.GetWindowText(v_sPM_Table);								//��ȡ��������
	v_sPM_Table.Trim();
	m_cPM_Name.GetWindowText(v_sPM_Name);								//��ȡ����������
	v_sPM_Name.Trim();
	m_cFormula.GetWindowText(v_sFormula);								//��ȡ�����㹫ʽ
	v_sFormula.Trim();
	m_cObjType.GetWindowText(v_sObjType);								//��ȡ��ObjType
	v_sObjType.Trim();
	m_cUnit.GetWindowText(v_sUnit);										//��ȡ����λ
	v_sUnit.Trim();
	v_sTemp="1";														//BSC��������

	//����
	if (v_bFlag==0)														//�޸�
		v_sSql="Update A_CAP_Info set zformula='"+v_sFormula+"',zobjtypes='"+v_sObjType+"' where znum='"+v_sNum+"'";		//SQL
	else																//����
	{
		mGSM_PM::iterator	v_pIter;									//������
		v_pIter=v_mGSM_PM.find(v_sPM_Table);							//���ң�����
		if (v_pIter!=v_mGSM_PM.end())
		{
			v_sSql="insert A_CAP_Info(zgroupnum,zcap,zformula,zobjtypes,zunit,zmark) values ('"
					+v_pIter->second.v_sNum+"','"						//��������
					+v_sPM_Name+"','"									//
					+v_sFormula+"','"									//
					+v_sObjType+"','"									//
					+v_sUnit+"','"										//
					+v_sTemp+"')";										//SQL
		}
	}
	My_ExecuteSQL(v_sSql);												//ִ��

	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ���ܱ�����
//------------------------------------------------------------------------------------------------------
void CPm_GSM_UpDate_PM::My_Get_PM_Table()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString			v_sSql;												//��ʱ����
	CString			v_sTemp,v_sTable,v_sType;							//��ʱ����
	int				v_iNum;												//��ʱ����

	GSM_PM_Struct	v_cGSM_PM_Struct;									//�ṹ

	//��������
	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��ȡ���������ܱ�����
		if (v_bBSC)
			v_sSql="select * from A_CAPGROUP_Info where ztypename like '%BSC%'";		//���ؼ������ܱ���Ϣ
		else
			v_sSql="select * from A_CAPGROUP_Info where ztypename not like '%BSC%'";	//���ؼ������ܱ���Ϣ
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("zgroupname");		//��ȡ�����ܱ�����
			v_sType = v_cAdoConn.GetField_String("ztypename");			//��ȡ����������
			v_iNum = v_cAdoConn.GetField_Int("zgroupnum");				//��ȡ������Ψһ��ʶ

			m_cPM_Table.AddString(v_sTable);							//��ʾ

			v_cGSM_PM_Struct.v_sType=v_sType;							//��������
			v_sTemp.Format("%d",v_iNum);								//�ַ���
			v_cGSM_PM_Struct.v_sNum=v_sTemp;							//����Ψһ��ʶ
			v_mGSM_PM.insert(pair<CString,GSM_PM_Struct>(v_sTable,v_cGSM_PM_Struct));	//��������

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
bool CPm_GSM_UpDate_PM::My_ExecuteSQL(CString v_sSql)
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
