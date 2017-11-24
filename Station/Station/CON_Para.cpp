// CON_Para.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "CON_Para.h"


// CCON_Para �Ի���

IMPLEMENT_DYNAMIC(CCON_Para, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CCON_Para::CCON_Para(CWnd* pParent /*=NULL*/)
	: CDialog(CCON_Para::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CCON_Para::~CCON_Para()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CCON_Para::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cHour_Month);
	DDX_Control(pDX, IDC_COMBO3, m_cMin_Day);
	DDX_Control(pDX, IDC_COMBO9, m_cFTP_Day);
	DDX_Control(pDX, IDC_COMBO4, m_cCm_Month);
	DDX_Control(pDX, IDC_COMBO20, m_cCm_FTP_Day);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_Para, CDialog)
	ON_BN_CLICKED(IDOK, &CCON_Para::OnBnClickedOk)
END_MESSAGE_MAP()


// CCON_Para ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CCON_Para::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString	v_sTemp;													//��ʱ����

	//���ܲ���
	//��ʱ��Сʱ
	for (int vi=24;vi<=36;vi++)
	{
		v_sTemp.Format("%02d",vi);										//�ַ���
		m_cHour_Month.AddString(v_sTemp);								//���ã�Сʱ�Ը�������
	}

	//��ʱ������
	for (int vi=7;vi<=30;vi++)
	{
		v_sTemp.Format("%02d",vi);										//�ַ���
		m_cMin_Day.AddString(v_sTemp);									//���ã��������ܱ�������
	}

	//���棺����
	for (int vi=30;vi<=90;vi++)
	{
		v_sTemp.Format("%02d",vi);										//�ַ���
		m_cFTP_Day.AddString(v_sTemp);									//���ã�FTP_Log����
	}
	
	//���ò���
	//����(��)
	for (int vi=3;vi<=12;vi++)
	{
		v_sTemp.Format("%02d",vi);										//�ַ���
		m_cCm_Month.AddString(v_sTemp);									//���ã�Сʱ�Ը�������
	}

	//FTP(����)
	for (int vi=10;vi<=30;vi++)
	{
		v_sTemp.Format("%02d",vi);										//�ַ���
		m_cCm_FTP_Day.AddString(v_sTemp);								//���ã�FTP_Log����
	}

	//��ʾ
	My_Get_Para();														//��ʾ������Ϣ

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CCON_Para::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	CString v_sText;													//��ʱ����

	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		CDialog* Pwnd=(CDialog*)GetActiveWindow();						//ȡ�öԻ���ָ��
		CWnd *pDlgItem = Pwnd->GetFocus();  
		if(pDlgItem->GetDlgCtrlID()!=0x3f9)								//����"ָ��"�ؼ������� 
		{
			return true;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ò���
//------------------------------------------------------------------------------------------------------
void CCON_Para::My_Get_Para()
{
	CString		v_sSql;													//��ʱ����
	int			v_iHour,v_iMin,v_iFTP_Day,v_iCm_Month,v_iCmFTP_Day;		//��ʱ����
	CString		v_sTemp;												//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from CON_System";								//��ѯ���ݿ� 

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		if(!v_pRecordset->adoEOF)
		{
			v_iHour=v_cAdoConn.GetField_Int("zPm_LTE_Month");			//zPm_LTE_Month
			v_iMin=v_cAdoConn.GetField_Int("zPm_LTE_Day");				//zPm_LTE_Day
			v_iFTP_Day=v_cAdoConn.GetField_Int("zPm_LTE_Day_FTP");		//zPm_LTE_Day_FTP
			v_iCm_Month=v_cAdoConn.GetField_Int("zCm_LTE_Month");		//zCm_LTE_Month
			v_iCmFTP_Day=v_cAdoConn.GetField_Int("zCm_LTE_Day_FTP");	//zCm_LTE_Day_FTP

			v_pRecordset->MoveNext();									//ָ������ƶ�
			
			//��ʾ
			m_cHour_Month.SetCurSel(v_iHour-24);						//Сʱ����
			m_cMin_Day.SetCurSel(v_iMin-7);								//��������
			m_cFTP_Day.SetCurSel(v_iFTP_Day-30);						//����
			m_cCm_Month.SetCurSel(v_iCm_Month-2);						//����
			m_cCm_FTP_Day.SetCurSel(v_iCmFTP_Day-10);					//����
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
//	�������ܣ�ȷ��
//------------------------------------------------------------------------------------------------------
void CCON_Para::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString v_sSql,v_sTemp;												//��ʱ����
	CString v_sHour,v_sMin,v_sFTP_Day,v_sCm_Month,v_sCm_FTP_Day;		//��ʱ����

	UpdateData(true);													//��ã���Ļ��Ϣ

	//���ݿ����
	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��

	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��ȡ���û���Ϣ
		v_sHour.Format("%d",m_cHour_Month.GetCurSel()+24);				//�ַ���
		v_sMin.Format("%d",m_cMin_Day.GetCurSel()+7);					//�ַ���
		v_sFTP_Day.Format("%d",m_cFTP_Day.GetCurSel()+30);				//�ַ���
		v_sCm_Month.Format("%d",m_cCm_Month.GetCurSel()+2);				//�ַ���
		v_sCm_FTP_Day.Format("%d",m_cCm_FTP_Day.GetCurSel()+10);		//�ַ���
		v_sSql = "update CON_System set zPm_LTE_Month="+v_sHour			//Сʱ
				+",zPm_LTE_Day="+v_sMin									//����
				+",zPm_LTE_Day_FTP="+v_sFTP_Day							//����
				+",zCm_LTE_Month="+v_sCm_Month							//����
				+",zCm_LTE_Day_FTP="+v_sCm_FTP_Day;						//����

		My_ExecuteSQL(v_sSql);											//ִ��

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	OnOK();
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CCON_Para::My_ExecuteSQL(CString v_sSql)
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
