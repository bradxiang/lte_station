// LTE_PM_Data_Cell_Time.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Data_Cell_Time.h"


// CLTE_PM_Data_Cell_Time �Ի���

IMPLEMENT_DYNAMIC(CLTE_PM_Data_Cell_Time, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cell_Time::CLTE_PM_Data_Cell_Time(CWnd* pParent /*=NULL*/)
	: CDialog(CLTE_PM_Data_Cell_Time::IDD, pParent)
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cell_Time::~CLTE_PM_Data_Cell_Time()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK2, m_cDate_Check_1);
	DDX_Control(pDX, IDC_CHECK5, m_cDate_Check_2);
	DDX_Control(pDX, IDC_CHECK6, m_cDate_Check_3);
	DDX_Control(pDX, IDC_CHECK7, m_cDate_Check_4);
	DDX_Control(pDX, IDC_CHECK8, m_cDate_Check_5);
	DDX_Control(pDX, IDC_CHECK9, m_cTime_Check_1);
	DDX_Control(pDX, IDC_CHECK10, m_cTime_Check_2);
	DDX_Control(pDX, IDC_CHECK11, m_cTime_Check_3);
	DDX_Control(pDX, IDC_CHECK12, m_cTime_Check_4);
	DDX_Control(pDX, IDC_CHECK13, m_cTime_Check_5);
	DDX_Control(pDX, IDC_COMBO15, m_cTime_Hour1_1);
	DDX_Control(pDX, IDC_COMBO16, m_cTime_Hour1_2);
	DDX_Control(pDX, IDC_COMBO17, m_cTime_Hour1_3);
	DDX_Control(pDX, IDC_COMBO18, m_cTime_Hour1_4);
	DDX_Control(pDX, IDC_COMBO19, m_cTime_Hour1_5);
	DDX_Control(pDX, IDC_COMBO10, m_cTime_Hour2_1);
	DDX_Control(pDX, IDC_COMBO11, m_cTime_Hour2_2);
	DDX_Control(pDX, IDC_COMBO12, m_cTime_Hour2_3);
	DDX_Control(pDX, IDC_COMBO13, m_cTime_Hour2_4);
	DDX_Control(pDX, IDC_COMBO14, m_cTime_Hour2_5);
	DDX_Control(pDX, IDC_BEG_DATE, m_cDate_Date_1);
	DDX_Control(pDX, IDC_BEG_DATE2, m_cDate_Date_2);
	DDX_Control(pDX, IDC_BEG_DATE3, m_cDate_Date_3);
	DDX_Control(pDX, IDC_BEG_DATE4, m_cDate_Date_4);
	DDX_Control(pDX, IDC_BEG_DATE5, m_cDate_Date_5);
	DDX_Control(pDX, IDC_COMBO1, m_cDate_Day_1);
	DDX_Control(pDX, IDC_COMBO3, m_cDate_Day_2);
	DDX_Control(pDX, IDC_COMBO2, m_cDate_Day_3);
	DDX_Control(pDX, IDC_COMBO4, m_cDate_Day_4);
	DDX_Control(pDX, IDC_COMBO5, m_cDate_Day_5);
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Data_Cell_Time, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CLTE_PM_Data_Cell_Time::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK2, &CLTE_PM_Data_Cell_Time::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON9, &CLTE_PM_Data_Cell_Time::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CLTE_PM_Data_Cell_Time::OnBnClickedButton10)
END_MESSAGE_MAP()


// CLTE_PM_Data_Cell_Time ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CLTE_PM_Data_Cell_Time::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	int		vi;															//��ʱ����
	CString	v_sTemp;													//��ʱ����
		
	//��������
	for (vi=1;vi<=c_iSample_Day;vi++)
	{
		v_sTemp.Format("%d",vi);										//�ַ���
		m_cDate_Day_1.AddString(v_sTemp);								//����
		m_cDate_Day_2.AddString(v_sTemp);								//����
		m_cDate_Day_3.AddString(v_sTemp);								//����
		m_cDate_Day_4.AddString(v_sTemp);								//����
		m_cDate_Day_5.AddString(v_sTemp);								//����
	}

	//Сʱ����
	for (vi=0;vi<24;vi++)
	{
		v_sTemp.Format("%d",vi);										//�ַ���
		m_cTime_Hour1_1.AddString(v_sTemp);								//��ʼʱ��
		m_cTime_Hour1_2.AddString(v_sTemp);								//��ʼʱ��
		m_cTime_Hour1_3.AddString(v_sTemp);								//��ʼʱ��
		m_cTime_Hour1_4.AddString(v_sTemp);								//��ʼʱ��
		m_cTime_Hour1_5.AddString(v_sTemp);								//��ʼʱ��
		m_cTime_Hour2_1.AddString(v_sTemp);								//����ʱ��
		m_cTime_Hour2_2.AddString(v_sTemp);								//����ʱ��
		m_cTime_Hour2_3.AddString(v_sTemp);								//����ʱ��
		m_cTime_Hour2_4.AddString(v_sTemp);								//����ʱ��
		m_cTime_Hour2_5.AddString(v_sTemp);								//����ʱ��
	}

	//Ĭ�����ã�����������������Ϣ
	m_cDate_Check_1.SetCheck(v_dDate_Select[0].v_bFlag);				//����1����־
	m_cDate_Date_1.SetTime(v_dDate_Select[0].v_dDate);					//����1������
	m_cDate_Day_1.SetCurSel(v_dDate_Select[0].v_iDay);					//����1������
	m_cDate_Check_2.SetCheck(v_dDate_Select[1].v_bFlag);				//����2����־
	m_cDate_Date_2.SetTime(v_dDate_Select[1].v_dDate);					//����2������
	m_cDate_Day_2.SetCurSel(v_dDate_Select[1].v_iDay);					//����2������
	m_cDate_Check_3.SetCheck(v_dDate_Select[2].v_bFlag);				//����3����־
	m_cDate_Date_3.SetTime(v_dDate_Select[2].v_dDate);					//����3������
	m_cDate_Day_3.SetCurSel(v_dDate_Select[2].v_iDay);					//����3������
	m_cDate_Check_4.SetCheck(v_dDate_Select[3].v_bFlag);				//����4����־
	m_cDate_Date_4.SetTime(v_dDate_Select[3].v_dDate);					//����4������
	m_cDate_Day_4.SetCurSel(v_dDate_Select[3].v_iDay);					//����4������
	m_cDate_Check_5.SetCheck(v_dDate_Select[4].v_bFlag);				//����5����־
	m_cDate_Date_5.SetTime(v_dDate_Select[4].v_dDate);					//����5������
	m_cDate_Day_5.SetCurSel(v_dDate_Select[4].v_iDay);					//����5������

	m_cTime_Check_1.SetCheck(v_dTime_Select[0].v_bFlag);				//ʱ��1����־
	m_cTime_Hour1_1.SetCurSel(v_dTime_Select[0].v_iHour_1);				//ʱ��1����ʼСʱ
	m_cTime_Hour2_1.SetCurSel(v_dTime_Select[0].v_iHour_2);				//ʱ��1������Сʱ
	m_cTime_Check_2.SetCheck(v_dTime_Select[1].v_bFlag);				//ʱ��2����־
	m_cTime_Hour1_2.SetCurSel(v_dTime_Select[1].v_iHour_1);				//ʱ��2����ʼСʱ
	m_cTime_Hour2_2.SetCurSel(v_dTime_Select[1].v_iHour_2);				//ʱ��2������Сʱ
	m_cTime_Check_3.SetCheck(v_dTime_Select[2].v_bFlag);				//ʱ��3����־
	m_cTime_Hour1_3.SetCurSel(v_dTime_Select[2].v_iHour_1);				//ʱ��3����ʼСʱ
	m_cTime_Hour2_3.SetCurSel(v_dTime_Select[2].v_iHour_2);				//ʱ��3������Сʱ
	m_cTime_Check_4.SetCheck(v_dTime_Select[3].v_bFlag);				//ʱ��4����־
	m_cTime_Hour1_4.SetCurSel(v_dTime_Select[3].v_iHour_1);				//ʱ��4����ʼСʱ
	m_cTime_Hour2_4.SetCurSel(v_dTime_Select[3].v_iHour_2);				//ʱ��4������Сʱ
	m_cTime_Check_5.SetCheck(v_dTime_Select[4].v_bFlag);				//ʱ��5����־
	m_cTime_Hour1_5.SetCurSel(v_dTime_Select[4].v_iHour_1);				//ʱ��5����ʼСʱ
	m_cTime_Hour2_5.SetCurSel(v_dTime_Select[4].v_iHour_2);				//ʱ��5������Сʱ

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Data_Cell_Time::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ����������С
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��رմ���
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnClose();													//�˳�	
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����1ѡ����Ч��������Ч��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cDate_Check_1.SetCheck(1);										//��Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��������
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cDate_Check_2.SetCheck(false);									//��Ч
	m_cDate_Check_3.SetCheck(false);									//��Ч
	m_cDate_Check_4.SetCheck(false);									//��Ч
	m_cDate_Check_5.SetCheck(false);									//��Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ʱ�����
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::OnBnClickedButton10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cTime_Check_1.SetCheck(false);									//��Ч
	m_cTime_Check_2.SetCheck(false);									//��Ч
	m_cTime_Check_3.SetCheck(false);									//��Ч
	m_cTime_Check_4.SetCheck(false);									//��Ч
	m_cTime_Check_5.SetCheck(false);									//��Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ѡ���˳�
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int				vi;													//��ʱ����
	CString			v_sTemp;											//��ʱ����
	bool			v_bOK=true;											//�쳣��־

	//��1���쳣�ж�
	mInt_String::iterator	v_pTime;									//������

	v_mTime->clear();													//�������
	//ʱ��1
	if (m_cTime_Check_1.GetCheck())										//ʱ��1
	{
		if (m_cTime_Hour1_1.GetCurSel()<=m_cTime_Hour2_1.GetCurSel())	//��ʼСʱ<����Сʱ
		{
			v_sTemp.Format("%02d--%02d",m_cTime_Hour1_1.GetCurSel(),m_cTime_Hour2_1.GetCurSel());	//��ȡ��ʱ����Ϣ��08--10��
			vi=m_cTime_Hour1_1.GetCurSel();								//��ʼСʱ
			while(vi<=m_cTime_Hour2_1.GetCurSel())
			{
				v_pTime=v_mTime->find(vi);								//��ѯСʱ
				if (v_pTime==v_mTime->end())							//û�ҵ�������
					v_mTime->insert(pair<int,CString>(vi,v_sTemp));		//��������
				else
					v_bOK=false;										//�쳣

				vi++;													//Сʱ++
			}
		}
		else
			v_bOK=false;												//�쳣
	}
	//ʱ��2
	if (m_cTime_Check_2.GetCheck())										//ʱ��2
	{
		if (m_cTime_Hour1_2.GetCurSel()<=m_cTime_Hour2_2.GetCurSel())	//��ʼСʱ<����Сʱ
		{
			v_sTemp.Format("%02d--%02d",m_cTime_Hour1_2.GetCurSel(),m_cTime_Hour2_2.GetCurSel());	//��ȡ��ʱ����Ϣ��08--10��
			vi=m_cTime_Hour1_2.GetCurSel();								//��ʼСʱ
			while(vi<=m_cTime_Hour2_2.GetCurSel())
			{
				v_pTime=v_mTime->find(vi);								//��ѯСʱ
				if (v_pTime==v_mTime->end())							//û�ҵ�������
					v_mTime->insert(pair<int,CString>(vi,v_sTemp));		//��������
				else
					v_bOK=false;										//�쳣

				vi++;													//Сʱ++
			}
		}
		else
			v_bOK=false;												//�쳣
	}
	//ʱ��3
	if (m_cTime_Check_3.GetCheck())										//ʱ��3
	{
		if (m_cTime_Hour1_3.GetCurSel()<=m_cTime_Hour2_3.GetCurSel())	//��ʼСʱ<����Сʱ
		{
			v_sTemp.Format("%02d--%02d",m_cTime_Hour1_3.GetCurSel(),m_cTime_Hour2_3.GetCurSel());	//��ȡ��ʱ����Ϣ��08--10��
			vi=m_cTime_Hour1_3.GetCurSel();								//��ʼСʱ
			while(vi<=m_cTime_Hour2_3.GetCurSel())
			{
				v_pTime=v_mTime->find(vi);								//��ѯСʱ
				if (v_pTime==v_mTime->end())							//û�ҵ�������
					v_mTime->insert(pair<int,CString>(vi,v_sTemp));		//��������
				else
					v_bOK=false;										//�쳣

				vi++;													//Сʱ++
			}
		}
		else
			v_bOK=false;												//�쳣
	}
	//ʱ��4
	if (m_cTime_Check_4.GetCheck())										//ʱ��4
	{
		if (m_cTime_Hour1_4.GetCurSel()<=m_cTime_Hour2_4.GetCurSel())	//��ʼСʱ<����Сʱ
		{
			v_sTemp.Format("%02d--%02d",m_cTime_Hour1_4.GetCurSel(),m_cTime_Hour2_4.GetCurSel());	//��ȡ��ʱ����Ϣ��08--10��
			vi=m_cTime_Hour1_4.GetCurSel();								//��ʼСʱ
			while(vi<=m_cTime_Hour2_4.GetCurSel())
			{
				v_pTime=v_mTime->find(vi);								//��ѯСʱ
				if (v_pTime==v_mTime->end())							//û�ҵ�������
					v_mTime->insert(pair<int,CString>(vi,v_sTemp));		//��������
				else
					v_bOK=false;										//�쳣

				vi++;													//Сʱ++
			}
		}
		else
			v_bOK=false;												//�쳣
	}
	//ʱ��5
	if (m_cTime_Check_5.GetCheck())										//ʱ��5
	{
		if (m_cTime_Hour1_5.GetCurSel()<=m_cTime_Hour2_5.GetCurSel())	//��ʼСʱ<����Сʱ
		{
			v_sTemp.Format("%02d--%02d",m_cTime_Hour1_5.GetCurSel(),m_cTime_Hour2_5.GetCurSel());	//��ȡ��ʱ����Ϣ��08--10��
			vi=m_cTime_Hour1_5.GetCurSel();								//��ʼСʱ
			while(vi<=m_cTime_Hour2_5.GetCurSel())
			{
				v_pTime=v_mTime->find(vi);								//��ѯСʱ
				if (v_pTime==v_mTime->end())							//û�ҵ�������
					v_mTime->insert(pair<int,CString>(vi,v_sTemp));		//��������
				else
					v_bOK=false;										//�쳣

				vi++;													//Сʱ++
			}
		}
		else
			v_bOK=false;												//�쳣
	}
	//�쳣�ж�
	if(!v_bOK)
	{
		v_mTime->clear();												//�������
		MessageBox("ʱ����Ϣ�쳣����ʼ���ڽ�������Сʱ�ظ���������ѡ�� ������");		//������ʾ	
		return;															//����
	}

	//��2�����ڻ�ȡ
//	v_dDate_Select[0].v_bFlag=m_cDate_Check_1.GetCheck();				//����1����Ч��־
	m_cDate_Date_1.GetTime(v_dDate_Select[0].v_dDate);					//����1����ʼ����
	v_dDate_Select[0].v_iDay=m_cDate_Day_1.GetCurSel();					//����1������
	v_dDate_Select[1].v_bFlag=m_cDate_Check_2.GetCheck();				//����2����Ч��־
	m_cDate_Date_2.GetTime(v_dDate_Select[1].v_dDate);					//����2����ʼ����
	v_dDate_Select[1].v_iDay=m_cDate_Day_2.GetCurSel();					//����2������
	v_dDate_Select[2].v_bFlag=m_cDate_Check_3.GetCheck();				//����3����Ч��־
	m_cDate_Date_3.GetTime(v_dDate_Select[2].v_dDate);					//����3����ʼ����
	v_dDate_Select[2].v_iDay=m_cDate_Day_3.GetCurSel();					//����3������
	v_dDate_Select[3].v_bFlag=m_cDate_Check_4.GetCheck();				//����4����Ч��־
	m_cDate_Date_4.GetTime(v_dDate_Select[3].v_dDate);					//����4����ʼ����
	v_dDate_Select[3].v_iDay=m_cDate_Day_4.GetCurSel();					//����4������
	v_dDate_Select[4].v_bFlag=m_cDate_Check_5.GetCheck();				//����5����Ч��־
	m_cDate_Date_5.GetTime(v_dDate_Select[4].v_dDate);					//����5����ʼ����
	v_dDate_Select[4].v_iDay=m_cDate_Day_5.GetCurSel();					//����5������


	//��3��ʱ�λ�ȡ
	v_dTime_Select[0].v_bFlag=m_cTime_Check_1.GetCheck();				//ʱ��1����Ч��־
	v_dTime_Select[0].v_iHour_1=m_cTime_Hour1_1.GetCurSel();			//ʱ��1����ʼСʱ
	v_dTime_Select[0].v_iHour_2=m_cTime_Hour2_1.GetCurSel();			//ʱ��1������Сʱ
	v_dTime_Select[1].v_bFlag=m_cTime_Check_2.GetCheck();				//ʱ��2����Ч��־
	v_dTime_Select[1].v_iHour_1=m_cTime_Hour1_2.GetCurSel();			//ʱ��2����ʼСʱ
	v_dTime_Select[1].v_iHour_2=m_cTime_Hour2_2.GetCurSel();			//ʱ��2������Сʱ
	v_dTime_Select[2].v_bFlag=m_cTime_Check_3.GetCheck();				//ʱ��3����Ч��־
	v_dTime_Select[2].v_iHour_1=m_cTime_Hour1_3.GetCurSel();			//ʱ��3����ʼСʱ
	v_dTime_Select[2].v_iHour_2=m_cTime_Hour2_3.GetCurSel();			//ʱ��3������Сʱ
	v_dTime_Select[3].v_bFlag=m_cTime_Check_4.GetCheck();				//ʱ��4����Ч��־
	v_dTime_Select[3].v_iHour_1=m_cTime_Hour1_4.GetCurSel();			//ʱ��4����ʼСʱ
	v_dTime_Select[3].v_iHour_2=m_cTime_Hour2_4.GetCurSel();			//ʱ��4������Сʱ
	v_dTime_Select[4].v_bFlag=m_cTime_Check_5.GetCheck();				//ʱ��5����Ч��־
	v_dTime_Select[4].v_iHour_1=m_cTime_Hour1_5.GetCurSel();			//ʱ��5����ʼСʱ
	v_dTime_Select[4].v_iHour_2=m_cTime_Hour2_5.GetCurSel();			//ʱ��5������Сʱ

	//��4���˳�
	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
