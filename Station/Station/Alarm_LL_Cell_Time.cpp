// Alarm_LL_Cell_Time.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Alarm_LL_Cell_Time.h"


// CAlarm_LL_Cell_Time �Ի���

IMPLEMENT_DYNAMIC(CAlarm_LL_Cell_Time, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Cell_Time::CAlarm_LL_Cell_Time(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarm_LL_Cell_Time::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Cell_Time::~CAlarm_LL_Cell_Time()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell_Time::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK9, m_cDown);
	DDX_Control(pDX, IDC_CHECK10, m_cUp);
	DDX_Control(pDX, IDC_CHECK2, m_cTime_1);
	DDX_Control(pDX, IDC_CHECK5, m_cTime_2);
	DDX_Control(pDX, IDC_CHECK6, m_cTime_3);
	DDX_Control(pDX, IDC_CHECK7, m_cTime_4);
	DDX_Control(pDX, IDC_CHECK8, m_cTime_5);
	DDX_Control(pDX, IDC_BEG_DATE, m_cDate_1);
	DDX_Control(pDX, IDC_BEG_DATE2, m_cDate_2);
	DDX_Control(pDX, IDC_BEG_DATE3, m_cDate_3);
	DDX_Control(pDX, IDC_BEG_DATE4, m_cDate_4);
	DDX_Control(pDX, IDC_BEG_DATE5, m_cDate_5);
	DDX_Control(pDX, IDC_COMBO15, m_cHour_1);
	DDX_Control(pDX, IDC_COMBO16, m_cHour_2);
	DDX_Control(pDX, IDC_COMBO17, m_cHour_3);
	DDX_Control(pDX, IDC_COMBO18, m_cHour_4);
	DDX_Control(pDX, IDC_COMBO19, m_cHour_5);
	DDX_Control(pDX, IDC_COMBO10, m_cCal_1);
	DDX_Control(pDX, IDC_COMBO11, m_cCal_2);
	DDX_Control(pDX, IDC_COMBO12, m_cCal_3);
	DDX_Control(pDX, IDC_COMBO13, m_cCal_4);
	DDX_Control(pDX, IDC_COMBO14, m_cCal_5);
	DDX_Control(pDX, IDC_KEY, m_cValues_1);
	DDX_Control(pDX, IDC_KEY2, m_cValues_2);
	DDX_Control(pDX, IDC_KEY3, m_cValues_3);
	DDX_Control(pDX, IDC_KEY4, m_cValues_4);
	DDX_Control(pDX, IDC_KEY5, m_cValues_5);
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAlarm_LL_Cell_Time, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CAlarm_LL_Cell_Time::OnBnClickedButton2)
END_MESSAGE_MAP()


// CAlarm_LL_Cell_Time ��Ϣ�������


//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CAlarm_LL_Cell_Time::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	int		vi;															//��ʱ����
	CString	v_sTemp;													//��ʱ����

	//���ã�Сʱ
	for (vi=0;vi<24;vi++)
	{
		v_sTemp.Format("%d",vi);										//�ַ���
		m_cHour_1.AddString(v_sTemp);									//Сʱ
		m_cHour_2.AddString(v_sTemp);									//Сʱ
		m_cHour_3.AddString(v_sTemp);									//Сʱ
		m_cHour_4.AddString(v_sTemp);									//Сʱ
		m_cHour_5.AddString(v_sTemp);									//Сʱ
	}

	//���ã�����С�ڵȺ�
	m_cCal_1.AddString("<");											//С��
	m_cCal_1.AddString(">");											//����
	m_cCal_1.AddString("=");											//����
	m_cCal_2.AddString("<");											//С��
	m_cCal_2.AddString(">");											//����
	m_cCal_2.AddString("=");											//����
	m_cCal_3.AddString("<");											//С��
	m_cCal_3.AddString(">");											//����
	m_cCal_3.AddString("=");											//����
	m_cCal_4.AddString("<");											//С��
	m_cCal_4.AddString(">");											//����
	m_cCal_4.AddString("=");											//����
	m_cCal_5.AddString("<");											//С��
	m_cCal_5.AddString(">");											//����
	m_cCal_5.AddString("=");											//����

	//��ʾ�����������Ѿ�ѡ��Ĳ�����
	m_cDown.SetCheck(v_bDown);											//״̬������
	m_cUp.SetCheck(v_bUp);												//״̬������

	//������Ч
	m_cTime_1.SetCheck(v_dDate_Select[0].v_bFlag);						//��־������
	m_cTime_2.SetCheck(v_dDate_Select[1].v_bFlag);						//��־������
	m_cTime_3.SetCheck(v_dDate_Select[2].v_bFlag);						//��־������
	m_cTime_4.SetCheck(v_dDate_Select[3].v_bFlag);						//��־������
	m_cTime_5.SetCheck(v_dDate_Select[4].v_bFlag);						//��־������

	//����
	m_cDate_1.SetTime(v_dDate_Select[0].v_dDate);						//���ã�����
	m_cDate_2.SetTime(v_dDate_Select[1].v_dDate);						//���ã�����
	m_cDate_3.SetTime(v_dDate_Select[2].v_dDate);						//���ã�����
	m_cDate_4.SetTime(v_dDate_Select[3].v_dDate);						//���ã�����
	m_cDate_5.SetTime(v_dDate_Select[4].v_dDate);						//���ã�����

	//Сʱ
	m_cHour_1.SetCurSel(v_dDate_Select[0].v_iHour);						//���ã�Сʱ
	m_cHour_2.SetCurSel(v_dDate_Select[1].v_iHour);						//���ã�Сʱ
	m_cHour_3.SetCurSel(v_dDate_Select[2].v_iHour);						//���ã�Сʱ
	m_cHour_4.SetCurSel(v_dDate_Select[3].v_iHour);						//���ã�Сʱ
	m_cHour_5.SetCurSel(v_dDate_Select[4].v_iHour);						//���ã�Сʱ

	//�ȽϷ���
	m_cCal_1.SetCurSel(v_dDate_Select[0].v_iCal);						//���ã��ȽϷ���
	m_cCal_2.SetCurSel(v_dDate_Select[1].v_iCal);						//���ã��ȽϷ���
	m_cCal_3.SetCurSel(v_dDate_Select[2].v_iCal);						//���ã��ȽϷ���
	m_cCal_4.SetCurSel(v_dDate_Select[3].v_iCal);						//���ã��ȽϷ���
	m_cCal_5.SetCurSel(v_dDate_Select[4].v_iCal);						//���ã��ȽϷ���

	//��ֵ
	v_sTemp.Format("%8.2f",v_dDate_Select[0].v_dValues);				//�ַ���
	m_cValues_1.SetWindowText(v_sTemp);									//���ã���ֵ
	v_sTemp.Format("%8.2f",v_dDate_Select[1].v_dValues);				//�ַ���
	m_cValues_2.SetWindowText(v_sTemp);									//���ã���ֵ
	v_sTemp.Format("%8.2f",v_dDate_Select[2].v_dValues);				//�ַ���
	m_cValues_3.SetWindowText(v_sTemp);									//���ã���ֵ
	v_sTemp.Format("%8.2f",v_dDate_Select[3].v_dValues);				//�ַ���
	m_cValues_4.SetWindowText(v_sTemp);									//���ã���ֵ
	v_sTemp.Format("%8.2f",v_dDate_Select[4].v_dValues);				//�ַ���
	m_cValues_5.SetWindowText(v_sTemp);									//���ã���ֵ
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CAlarm_LL_Cell_Time::PreTranslateMessage(MSG* pMsg)
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
void CAlarm_LL_Cell_Time::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnClose();
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ѡ���˳�
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell_Time::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString	v_sTemp;													//��ʱ����

	//��ȡ�����С�����״̬
	v_bDown=m_cDown.GetCheck();											//״̬������
	v_bUp=m_cUp.GetCheck();												//״̬������

	//��ȡ������״̬
	v_dDate_Select[0].v_bFlag=m_cTime_1.GetCheck();						//��ȡ��״̬
	v_dDate_Select[1].v_bFlag=m_cTime_2.GetCheck();						//��ȡ��״̬
	v_dDate_Select[2].v_bFlag=m_cTime_3.GetCheck();						//��ȡ��״̬
	v_dDate_Select[3].v_bFlag=m_cTime_4.GetCheck();						//��ȡ��״̬
	v_dDate_Select[4].v_bFlag=m_cTime_5.GetCheck();						//��ȡ��״̬

	//��ȡ������
	m_cDate_1.GetTime(v_dDate_Select[0].v_dDate);						//��ȡ������
	m_cDate_2.GetTime(v_dDate_Select[1].v_dDate);						//��ȡ������
	m_cDate_3.GetTime(v_dDate_Select[2].v_dDate);						//��ȡ������
	m_cDate_4.GetTime(v_dDate_Select[3].v_dDate);						//��ȡ������
	m_cDate_5.GetTime(v_dDate_Select[4].v_dDate);						//��ȡ������

	//��ȡ��Сʱ
	v_dDate_Select[0].v_iHour=m_cHour_1.GetCurSel();					//��ȡ��Сʱ
	v_dDate_Select[1].v_iHour=m_cHour_2.GetCurSel();					//��ȡ��Сʱ
	v_dDate_Select[2].v_iHour=m_cHour_3.GetCurSel();					//��ȡ��Сʱ
	v_dDate_Select[3].v_iHour=m_cHour_4.GetCurSel();					//��ȡ��Сʱ
	v_dDate_Select[4].v_iHour=m_cHour_5.GetCurSel();					//��ȡ��Сʱ

	//��ȡ���ȽϷ�
	v_dDate_Select[0].v_iCal=m_cCal_1.GetCurSel();						//��ȡ���ȽϷ�
	v_dDate_Select[1].v_iCal=m_cCal_2.GetCurSel();						//��ȡ���ȽϷ�
	v_dDate_Select[2].v_iCal=m_cCal_3.GetCurSel();						//��ȡ���ȽϷ�
	v_dDate_Select[3].v_iCal=m_cCal_4.GetCurSel();						//��ȡ���ȽϷ�
	v_dDate_Select[4].v_iCal=m_cCal_5.GetCurSel();						//��ȡ���ȽϷ�

	//��ȡ����ֵ
	m_cValues_1.GetWindowText(v_sTemp);									//��ȡ����ֵ
	v_dDate_Select[0].v_dValues=atof(v_sTemp);							//�ַ���-->flote
	m_cValues_2.GetWindowText(v_sTemp);									//��ȡ����ֵ
	v_dDate_Select[1].v_dValues=atof(v_sTemp);							//�ַ���-->flote
	m_cValues_3.GetWindowText(v_sTemp);									//��ȡ����ֵ
	v_dDate_Select[2].v_dValues=atof(v_sTemp);							//�ַ���-->flote
	m_cValues_4.GetWindowText(v_sTemp);									//��ȡ����ֵ
	v_dDate_Select[3].v_dValues=atof(v_sTemp);							//�ַ���-->flote
	m_cValues_5.GetWindowText(v_sTemp);									//��ȡ����ֵ
	v_dDate_Select[4].v_dValues=atof(v_sTemp);							//�ַ���-->flote

	//�˳�
	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
