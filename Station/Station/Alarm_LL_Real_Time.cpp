// Alarm_LL_Real_Time.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Alarm_LL_Real_Time.h"


// CAlarm_LL_Real_Time �Ի���

IMPLEMENT_DYNAMIC(CAlarm_LL_Real_Time, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Real_Time::CAlarm_LL_Real_Time(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarm_LL_Real_Time::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Real_Time::~CAlarm_LL_Real_Time()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK9, m_cDown);
	DDX_Control(pDX, IDC_CHECK13, m_cUp);
	DDX_Control(pDX, IDC_CHECK2, m_cDown_Time_1);
	DDX_Control(pDX, IDC_CHECK5, m_cDown_Time_2);
	DDX_Control(pDX, IDC_CHECK6, m_cDown_Time_3);
	DDX_Control(pDX, IDC_CHECK7, m_cDown_Time_4);
	DDX_Control(pDX, IDC_KEY, m_cDown_Value_1);
	DDX_Control(pDX, IDC_KEY2, m_cDown_Value_2);
	DDX_Control(pDX, IDC_KEY3, m_cDown_Value_3);
	DDX_Control(pDX, IDC_KEY4, m_cDown_Value_4);
	DDX_Control(pDX, IDC_CHECK8, m_cUp_Time_1);
	DDX_Control(pDX, IDC_CHECK10, m_cUp_Time_2);
	DDX_Control(pDX, IDC_CHECK11, m_cUp_Time_3);
	DDX_Control(pDX, IDC_CHECK12, m_cUp_Time_4);
	DDX_Control(pDX, IDC_KEY5, m_cUp_Value_1);
	DDX_Control(pDX, IDC_KEY6, m_cUp_Value_2);
	DDX_Control(pDX, IDC_KEY7, m_cUp_Value_3);
	DDX_Control(pDX, IDC_KEY8, m_cUp_Value_4);
	DDX_Control(pDX, IDC_COMBO3, m_cPara);
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAlarm_LL_Real_Time, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CAlarm_LL_Real_Time::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON9, &CAlarm_LL_Real_Time::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CAlarm_LL_Real_Time::OnBnClickedButton10)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CAlarm_LL_Real_Time::OnCbnSelchangeCombo3)
	ON_BN_CLICKED(IDC_CHECK9, &CAlarm_LL_Real_Time::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_CHECK2, &CAlarm_LL_Real_Time::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK5, &CAlarm_LL_Real_Time::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &CAlarm_LL_Real_Time::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, &CAlarm_LL_Real_Time::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK13, &CAlarm_LL_Real_Time::OnBnClickedCheck13)
	ON_BN_CLICKED(IDC_CHECK8, &CAlarm_LL_Real_Time::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK10, &CAlarm_LL_Real_Time::OnBnClickedCheck10)
	ON_BN_CLICKED(IDC_CHECK11, &CAlarm_LL_Real_Time::OnBnClickedCheck11)
	ON_BN_CLICKED(IDC_CHECK12, &CAlarm_LL_Real_Time::OnBnClickedCheck12)
END_MESSAGE_MAP()


// CAlarm_LL_Real_Time ��Ϣ�������


//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CAlarm_LL_Real_Time::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//���ã�����
	m_cPara.AddString("1��");											//��������
	m_cPara.AddString("2��");
	m_cPara.AddString("3��");
	m_cPara.AddString("4��");
	m_cPara.AddString("5��");
	m_cPara.SetCurSel(0);												//Ĭ��

	//��ʾ����
	OnCbnSelchangeCombo3();												//��ʾ������[Ĭ�ϵĲ�����]

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CAlarm_LL_Real_Time::PreTranslateMessage(MSG* pMsg)
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
// ��ϢԤ�����رնԻ���
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real_Time::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnClose();
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����������ѡť��Ӧ
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_cDown.GetCheck())											//ѡ����Ч������
	{
		m_cDown_Time_1.SetCheck(0);										//������Ч
		m_cDown_Time_2.SetCheck(0);										//������Ч
		m_cDown_Time_3.SetCheck(0);										//������Ч
		m_cDown_Time_4.SetCheck(0);										//������Ч
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������ȸ�ѡť��Ӧ
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_cDown_Time_1.GetCheck())										//����ѡ����Ч������
		m_cDown.SetCheck(1);											//������������Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������ȸ�ѡť��Ӧ
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_cDown_Time_2.GetCheck())										//����ѡ����Ч������
		m_cDown.SetCheck(1);											//������������Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������ȸ�ѡť��Ӧ
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_cDown_Time_3.GetCheck())										//����ѡ����Ч������
		m_cDown.SetCheck(1);											//������������Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������ȸ�ѡť��Ӧ
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_cDown_Time_4.GetCheck())										//����ѡ����Ч������
		m_cDown.SetCheck(1);											//������������Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������ȸ�ѡť��Ӧ
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_cUp.GetCheck())												//ѡ����Ч������
	{
		m_cUp_Time_1.SetCheck(0);										//������Ч
		m_cUp_Time_2.SetCheck(0);										//������Ч
		m_cUp_Time_3.SetCheck(0);										//������Ч
		m_cUp_Time_4.SetCheck(0);										//������Ч
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������ȸ�ѡť��Ӧ
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_cUp_Time_1.GetCheck())										//����ѡ����Ч������
		m_cUp.SetCheck(1);												//������������Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������ȸ�ѡť��Ӧ
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_cUp_Time_2.GetCheck())										//����ѡ����Ч������
		m_cUp.SetCheck(1);												//������������Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������ȸ�ѡť��Ӧ
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_cUp_Time_3.GetCheck())										//����ѡ����Ч������
		m_cUp.SetCheck(1);												//������������Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������ȸ�ѡť��Ӧ
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck12()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_cUp_Time_4.GetCheck())										//����ѡ����Ч������
		m_cUp.SetCheck(1);												//������������Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�������ѡ��
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnCbnSelchangeCombo3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql;													//��ʱ����
	int			v_iPara;												//��ʱ����
	CString		v_sTemp;												//��ʱ����

	int			v_iDown=0,v_iDown_Time_1=0,v_iDown_Time_2=0,v_iDown_Time_3=0,v_iDown_Time_4=0;	//����
	int			v_iUp=0,v_iUp_Time_1=0,v_iUp_Time_2=0,v_iUp_Time_3=0,v_iUp_Time_4=0;			//����
	double		v_dDown_Value_1=0,v_dDown_Value_2=0,v_dDown_Value_3=0,v_dDown_Value_4=0;		//��ֵ
	double		v_dUp_Value_1=0,v_dUp_Value_2=0,v_dUp_Value_3=0,v_dUp_Value_4=0;				//��ֵ

	//��ȡ��������
	v_iPara=m_cPara.GetCurSel()+1;										//��ȡ��������

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		//�������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��ѯ�����������ݵ�½�ʺ�+������ţ�
		v_sTemp.Format("%d",v_iPara);									//�ַ���
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select * from CON_Pm_LTE_LL where zAccount='"+theApp.a_sUser_Account+"' and zPara="+v_sTemp;	//��ѯ���ݿ�

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										

		if(!v_pRecordset->adoEOF)
		{
			//��ȡ��������������
			v_iDown=v_cAdoConn.GetField_Int("zDown");					//zDown
			v_iDown_Time_1=v_cAdoConn.GetField_Int("zDown_Time_1");		//zDown_Time_1
			v_iDown_Time_2=v_cAdoConn.GetField_Int("zDown_Time_2");		//zDown_Time_2
			v_iDown_Time_3=v_cAdoConn.GetField_Int("zDown_Time_3");		//zDown_Time_3
			v_iDown_Time_4=v_cAdoConn.GetField_Int("zDown_Time_4");		//zDown_Time_4
			v_dDown_Value_1=v_cAdoConn.GetField_Double("zDown_Value_1");//zDown_Value_1
			v_dDown_Value_2=v_cAdoConn.GetField_Double("zDown_Value_2");//zDown_Value_2
			v_dDown_Value_3=v_cAdoConn.GetField_Double("zDown_Value_3");//zDown_Value_3
			v_dDown_Value_4=v_cAdoConn.GetField_Double("zDown_Value_4");//zDown_Value_4

			//��ȡ��������������
			v_iUp=v_cAdoConn.GetField_Int("zUp");						//zUp
			v_iUp_Time_1=v_cAdoConn.GetField_Int("zUp_Time_1");			//zUp_Time_1
			v_iUp_Time_2=v_cAdoConn.GetField_Int("zUp_Time_2");			//zUp_Time_2
			v_iUp_Time_3=v_cAdoConn.GetField_Int("zUp_Time_3");			//zUp_Time_3
			v_iUp_Time_4=v_cAdoConn.GetField_Int("zUp_Time_4");			//zUp_Time_4
			v_dUp_Value_1=v_cAdoConn.GetField_Double("zUp_Value_1");	//zUp_Value_1
			v_dUp_Value_2=v_cAdoConn.GetField_Double("zUp_Value_2");	//zUp_Value_2
			v_dUp_Value_3=v_cAdoConn.GetField_Double("zUp_Value_3");	//zUp_Value_3
			v_dUp_Value_4=v_cAdoConn.GetField_Double("zUp_Value_4");	//zUp_Value_4

			//�Ͽ����ݿ�
			v_cAdoConn.ExitConnect();									//�Ͽ�����
		}
		else															//��������������Ӳ�����[����Ϊ��]
		{
			//�Ͽ����ݿ�
			v_cAdoConn.ExitConnect();									//�Ͽ�����

			v_sSql="insert into CON_Pm_LTE_LL(zAccount,zPara) values ('"+theApp.a_sUser_Account+"','"+v_sTemp+"')";	//sql
			My_ExecuteSQL(v_sSql);										//ִ��
		}

		//��ʾ�����в���
		m_cDown.SetCheck(v_iDown);										//״̬������

		//��ȡ������״̬
		m_cDown_Time_1.SetCheck(v_iDown_Time_1);						//
		m_cDown_Time_2.SetCheck(v_iDown_Time_2);						//
		m_cDown_Time_3.SetCheck(v_iDown_Time_3);						//
		m_cDown_Time_4.SetCheck(v_iDown_Time_4);						//
		//��ȡ��������ֵ
		v_sTemp.Format("%8.2f",v_dDown_Value_1);						//�ַ���
		m_cDown_Value_1.SetWindowText(v_sTemp);							//
		v_sTemp.Format("%8.2f",v_dDown_Value_2);						//�ַ���
		m_cDown_Value_2.SetWindowText(v_sTemp);							//
		v_sTemp.Format("%8.2f",v_dDown_Value_3);						//�ַ���
		m_cDown_Value_3.SetWindowText(v_sTemp);							//
		v_sTemp.Format("%8.2f",v_dDown_Value_4);						//�ַ���
		m_cDown_Value_4.SetWindowText(v_sTemp);							//

		//��ʾ�����в���
		m_cUp.SetCheck(v_iUp);											//״̬������
		//��ȡ������״̬
		m_cUp_Time_1.SetCheck(v_iUp_Time_1);							//
		m_cUp_Time_2.SetCheck(v_iUp_Time_2);							//
		m_cUp_Time_3.SetCheck(v_iUp_Time_3);							//
		m_cUp_Time_4.SetCheck(v_iUp_Time_4);							//
		//��ȡ��������ֵ
		v_sTemp.Format("%8.2f",v_dUp_Value_1);							//�ַ���
		m_cUp_Value_1.SetWindowText(v_sTemp);							//
		v_sTemp.Format("%8.2f",v_dUp_Value_2);							//�ַ���
		m_cUp_Value_2.SetWindowText(v_sTemp);							//
		v_sTemp.Format("%8.2f",v_dUp_Value_3);							//�ַ���
		m_cUp_Value_3.SetWindowText(v_sTemp);							//
		v_sTemp.Format("%8.2f",v_dUp_Value_4);							//�ַ���
		m_cUp_Value_4.SetWindowText(v_sTemp);							//
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql;													//��ʱ����
	int			v_iPara;												//��ʱ����
	CString		v_sTemp;												//��ʱ����

	int			v_iDown=0,v_iDown_Time_1=0,v_iDown_Time_2=0,v_iDown_Time_3=0,v_iDown_Time_4=0;	//����
	int			v_iUp=0,v_iUp_Time_1=0,v_iUp_Time_2=0,v_iUp_Time_3=0,v_iUp_Time_4=0;			//����
	CString		v_sDown=0,v_sDown_Time_1=0,v_sDown_Time_2=0,v_sDown_Time_3=0,v_sDown_Time_4=0;	//����
	CString		v_sUp=0,v_sUp_Time_1=0,v_sUp_Time_2=0,v_sUp_Time_3=0,v_sUp_Time_4=0;			//����
	CString		v_sDown_Value_1=0,v_sDown_Value_2=0,v_sDown_Value_3=0,v_sDown_Value_4=0;		//��ֵ
	CString		v_sUp_Value_1=0,v_sUp_Value_2=0,v_sUp_Value_3=0,v_sUp_Value_4=0;				//��ֵ

	//��ȡ��������
	v_iPara=m_cPara.GetCurSel()+1;										//��ȡ��������
	v_sTemp.Format("%d",v_iPara);										//�ַ���

	//����
	if(MessageBox("�Ƿ񱣴�����飺 "+v_sTemp+" ��","����ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
	{
		//��ȡ���ݣ���������
		v_iDown=m_cDown.GetCheck();										//״̬
		v_sDown.Format("%d",v_iDown);									//�ַ���

		//��ȡ������״̬
		v_iDown_Time_1=m_cDown_Time_1.GetCheck();						//����
		v_iDown_Time_2=m_cDown_Time_2.GetCheck();						//����
		v_iDown_Time_3=m_cDown_Time_3.GetCheck();						//����
		v_iDown_Time_4=m_cDown_Time_4.GetCheck();						//����
		v_sDown_Time_1.Format("%d",v_iDown_Time_1);						//�ַ���
		v_sDown_Time_2.Format("%d",v_iDown_Time_2);						//�ַ���
		v_sDown_Time_3.Format("%d",v_iDown_Time_3);						//�ַ���
		v_sDown_Time_4.Format("%d",v_iDown_Time_4);						//�ַ���

		//��ȡ��������ֵ
		m_cDown_Value_1.GetWindowText(v_sDown_Value_1);					//��ֵ
		m_cDown_Value_2.GetWindowText(v_sDown_Value_2);					//��ֵ
		m_cDown_Value_3.GetWindowText(v_sDown_Value_3);					//��ֵ
		m_cDown_Value_4.GetWindowText(v_sDown_Value_4);					//��ֵ
		
		//��ȡ���ݣ���������
		v_iUp=m_cUp.GetCheck();											//״̬
		v_sUp.Format("%d",v_iUp);										//�ַ���

		//��ȡ������״̬
		v_iUp_Time_1=m_cUp_Time_1.GetCheck();							//����
		v_iUp_Time_2=m_cUp_Time_2.GetCheck();							//����
		v_iUp_Time_3=m_cUp_Time_3.GetCheck();							//����
		v_iUp_Time_4=m_cUp_Time_4.GetCheck();							//����
		v_sUp_Time_1.Format("%d",v_iUp_Time_1);							//�ַ���
		v_sUp_Time_2.Format("%d",v_iUp_Time_2);							//�ַ���
		v_sUp_Time_3.Format("%d",v_iUp_Time_3);							//�ַ���
		v_sUp_Time_4.Format("%d",v_iUp_Time_4);							//�ַ���

		//��ȡ��������ֵ
		m_cUp_Value_1.GetWindowText(v_sUp_Value_1);						//��ֵ
		m_cUp_Value_2.GetWindowText(v_sUp_Value_2);						//��ֵ
		m_cUp_Value_3.GetWindowText(v_sUp_Value_3);						//��ֵ
		m_cUp_Value_4.GetWindowText(v_sUp_Value_4);						//��ֵ

		//����
		v_sSql="Update CON_Pm_LTE_LL set zDown='"+v_sDown				//
				+"',zDown_Time_1='"+v_sDown_Time_1						//
				+"',zDown_Time_2='"+v_sDown_Time_2						//
				+"',zDown_Time_3='"+v_sDown_Time_3						//
				+"',zDown_Time_4='"+v_sDown_Time_4						//
				+"',zDown_Value_1='"+v_sDown_Value_1					//
				+"',zDown_Value_2='"+v_sDown_Value_2					//
				+"',zDown_Value_3='"+v_sDown_Value_3					//
				+"',zDown_Value_4='"+v_sDown_Value_4					//
				+"',zUp='"+v_sUp										//
				+"', zUp_Time_1='"+v_sUp_Time_1							//
				+"', zUp_Time_2='"+v_sUp_Time_2							//
				+"', zUp_Time_3='"+v_sUp_Time_3							//
				+"', zUp_Time_4='"+v_sUp_Time_4							//
				+"', zUp_Value_1='"+v_sUp_Value_1						//
				+"', zUp_Value_2='"+v_sUp_Value_2						//
				+"', zUp_Value_3='"+v_sUp_Value_3						//
				+"', zUp_Value_4='"+v_sUp_Value_4						//
				+"' where zAccount='"+theApp.a_sUser_Account+"' and zPara="+v_sTemp;	//sql
		My_ExecuteSQL(v_sSql);											//ִ��

		//��ʾ
		OnCbnSelchangeCombo3();											//��ʾ��ɾ���������
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ɾ��
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedButton10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sSql;													//��ʱ����
	int			v_iPara;												//��ʱ����
	CString		v_sTemp;												//��ʱ����

	//��ȡ��������
	v_iPara=m_cPara.GetCurSel()+1;										//��ȡ��������
	v_sTemp.Format("%d",v_iPara);										//�ַ���

	//ɾ��
	if(MessageBox("�Ƿ�ɾ�������飺 "+v_sTemp+" ��","ɾ��ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
	{
		v_sSql="delete from CON_Pm_LTE_LL where zAccount='"+theApp.a_sUser_Account+"' and zPara="+v_sTemp;	//��ѯ���ݿ�
		My_ExecuteSQL(v_sSql);											//ִ��

		//��ʾ
		OnCbnSelchangeCombo3();											//��ʾ��ɾ���������
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ѡ���˳�
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�˳�
	OnOK();
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CAlarm_LL_Real_Time::My_ExecuteSQL(CString v_sSql)
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
