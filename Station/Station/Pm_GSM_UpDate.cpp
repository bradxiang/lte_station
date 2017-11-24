// Pm_GSM_UpDate.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Pm_GSM_UpDate.h"


// CPm_GSM_UpDate �Ի���

IMPLEMENT_DYNAMIC(CPm_GSM_UpDate, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CPm_GSM_UpDate::CPm_GSM_UpDate(CWnd* pParent /*=NULL*/)
	: CDialog(CPm_GSM_UpDate::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CPm_GSM_UpDate::~CPm_GSM_UpDate()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CPm_GSM_UpDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cPM_Table);
	DDX_Control(pDX, IDC_EDIT2, m_cType);
	DDX_Control(pDX, IDC_EDIT3, m_cDay);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPm_GSM_UpDate, CDialog)
	ON_BN_CLICKED(IDOK, &CPm_GSM_UpDate::OnBnClickedOk)
END_MESSAGE_MAP()


// CPm_GSM_UpDate ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
// �������ܣ���ʼ��
//------------------------------------------------------------------------------------------------------			
BOOL CPm_GSM_UpDate::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_cPM_Table.SetWindowText(v_sPM_Table);								//��ȡ��������
	m_cType.SetWindowText(v_sType);										//��ȡ������
	m_cDay.SetWindowText(v_sDay);										//��ȡ����������

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CPm_GSM_UpDate::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ��޸�
//------------------------------------------------------------------------------------------------------
void CPm_GSM_UpDate::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString v_sSql,v_sTemp;												//��ʱ����

	m_cPM_Table.GetWindowText(v_sPM_Table);								//��ȡ��������
	v_sPM_Table.Trim();
	m_cDay.GetWindowText(v_sDay);										//��ȡ������
	v_sDay.Trim();

	//�˺���Ч
	if(v_sDay.IsEmpty())
	{
		MessageBox("�����뱣������������","��Ϣ��ʾ",MB_OK);			//��ʾ
		m_cDay.SetFocus();												//�۽�
		return;
	}

	v_sSql="Update A_CAPGROUP_Info set zsavetime="+v_sDay+" where zgroupname='"+v_sPM_Table+"'";	//SQL
	My_ExecuteSQL(v_sSql);												//ִ��

	OnOK();
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CPm_GSM_UpDate::My_ExecuteSQL(CString v_sSql)
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
