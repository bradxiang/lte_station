// LTE_PM_Data_Real_Limit.cpp : ʵ���ļ�
// 


#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Data_Real_Limit.h"


// CLTE_PM_Data_Real_Limit �Ի���

IMPLEMENT_DYNAMIC(CLTE_PM_Data_Real_Limit, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Real_Limit::CLTE_PM_Data_Real_Limit(CWnd* pParent /*=NULL*/)
: CDialog(CLTE_PM_Data_Real_Limit::IDD, pParent)
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Real_Limit::~CLTE_PM_Data_Real_Limit()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real_Limit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_EDIT1, m_cSum);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_CHECK1, m_cPm_Flag);
	DDX_Control(pDX, IDC_COMBO3, m_cPm_Type);
	DDX_Control(pDX, IDC_BUTTON1, m_cUpdate);
	DDX_Control(pDX, IDC_EDIT3, m_cPm_Name);
	DDX_Control(pDX, IDC_EDIT2, m_cPm_Value);
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Data_Real_Limit, CDialog)
	ON_BN_CLICKED(IDC_EXCEL, &CLTE_PM_Data_Real_Limit::OnBnClickedExcel)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Data_Real_Limit::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Data_Real_Limit::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLTE_PM_Data_Real_Limit ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CLTE_PM_Data_Real_Limit::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ʼ�����б�
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,60);
	m_cList.InsertColumn(1,"����",LVCFMT_LEFT,100);						//
	m_cList.InsertColumn(2,"����",LVCFMT_LEFT,270);						//
	m_cList.InsertColumn(3,"Ԥ��",LVCFMT_CENTER,80);					//
	m_cList.InsertColumn(4,"Ԥ������",LVCFMT_RIGHT,80);					//
	m_cList.InsertColumn(5,"Ԥ������",LVCFMT_CENTER,80);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//������ʼ��
	v_iItem=-1;															//��Ŀѡ����Ч[-1]
	m_cPm_Type.AddString("��");
	m_cPm_Type.AddString("��");
	m_cPm_Type.SetCurSel(0);											//Ĭ��
	m_cUpdate.EnableWindow(false);										//��Ч

	//��Ϣ��ʾ��Ĭ��ѡ�е�����
	CString					v_sTemp;									//��ʱ����
	int						v_iItem;									//��ʱ����
	mLTE_PM::iterator		v_pPM_Name;									//������
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	LTE_PM_Struct			v_cPM_Data;									//�ṹ
	map<int,LTE_PM_Struct>	v_mPM_Data;									//����
	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//��ָ��
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//��������
	{
		v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();			//��ָ��
		while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//��������
		{
			if (v_pPM_Name->second.v_bOK)
			{
				v_cPM_Data.v_sType = v_pLTE_PM_Type->first;				//��������
				v_cPM_Data.v_sPM_Name=v_pPM_Name->second.v_sPM_Name;	//��������
				v_cPM_Data.v_sAlarm_Flag=v_pPM_Name->second.v_sAlarm_Flag;		//Ԥ����־
				v_cPM_Data.v_dAlarm_Value=v_pPM_Name->second.v_dAlarm_Value;	//Ԥ������
				v_cPM_Data.v_sAlarm_Type=v_pPM_Name->second.v_sAlarm_Type;		//Ԥ������
				v_mPM_Data.insert(pair<int,LTE_PM_Struct>(v_pPM_Name->second.v_iNum,v_cPM_Data));	//���ӣ�ѡ����Ŀ
			}
			v_pPM_Name++;												//�ݼ�
		}
		v_pLTE_PM_Type++;												//�ݼ�
	}
	map<int,LTE_PM_Struct>::iterator	v_pPM_Data;						//������
	for (v_pPM_Data=v_mPM_Data.begin();v_pPM_Data!=v_mPM_Data.end();v_pPM_Data++)
	{
		v_iItem = m_cList.InsertItem(0xffff,_T("0"));
		v_sTemp.Format("%04d",v_iItem+1);
		m_cList.SetItemText(v_iItem,0," "+v_sTemp);						//���
		m_cList.SetItemText(v_iItem,1,v_pPM_Data->second.v_sType);		//��������
		m_cList.SetItemText(v_iItem,2,v_pPM_Data->second.v_sPM_Name);	//��������
		m_cList.SetItemText(v_iItem,3,v_pPM_Data->second.v_sAlarm_Flag);//Ԥ����־
		v_sTemp.Format("%8.2f",v_pPM_Data->second.v_dAlarm_Value);		//�ַ���
		m_cList.SetItemText(v_iItem,4,v_sTemp);							//Ԥ������
		m_cList.SetItemText(v_iItem,5,v_pPM_Data->second.v_sAlarm_Type);//Ԥ������
	}
	v_sTemp.Format("%d",m_cList.GetItemCount());						//�ַ���
	m_cSum.SetWindowText(v_sTemp);										//��������
	
	v_mPM_Data.clear();													//�������

	return TRUE;  // return TRUE unless you set the focus to a control
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Data_Real_Limit::PreTranslateMessage(MSG* pMsg)
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
void CLTE_PM_Data_Real_Limit::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Excel
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real_Limit::OnBnClickedExcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//����Excel
	m_cList.SetFocus();													//�۽�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����澯�����б�
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real_Limit::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp;												//��ʱ����

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��
	if (pNMListView->iItem>=0)											//�жϣ���Ŀ�����Ч������
	{
		if(v_iItem>=0)													//��Ч��Ŀ��ţ�����
			m_cList.SetItemBkColor(v_iItem,RGB(255,255,255));			//�ָ�����ɫ
		v_iItem=pNMListView->iItem;										//��ã�ѡ����Ŀ���
		m_cList.SetItemBkColor(v_iItem,RGB(255,0,0));					//���ñ���ɫ����ɫ
		
		//���ã��޸�����
		m_cPm_Name.SetWindowText(m_cList.GetItemText(v_iItem,2));		//��������
		if (m_cList.GetItemText(v_iItem,3)=="Ԥ��")
			m_cPm_Flag.SetCheck(1);										//Ԥ��
		else
			m_cPm_Flag.SetCheck(0);										//��Ԥ��
		v_sTemp=m_cList.GetItemText(v_iItem,4);							//Ԥ������
		v_sTemp.Trim();
		m_cPm_Value.SetWindowText(v_sTemp);								//Ԥ������
		if (m_cList.GetItemText(v_iItem,5)=="��")
			m_cPm_Type.SetCurSel(0);									//��
		else
			m_cPm_Type.SetCurSel(1);									//��
		m_cUpdate.EnableWindow(true);									//��Ч
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�Ԥ�������޸�
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real_Limit::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp,v_sSql;											//��ʱ����
	CString		v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;				//��ʱ����
	CString		v_sType,v_sPM_Name;										//��ʱ����

	//��ȡ
	v_sAlarm_Flag=m_cPm_Flag.GetCheck()?"Ԥ��":"��Ԥ��";				//Ԥ����Ч
	m_cPm_Value.GetWindowText(v_sAlarm_Value);							//Ԥ������
	v_sAlarm_Type=m_cPm_Type.GetCurSel()==0?"��":"��";					//Ԥ������
	
	v_sType=m_cList.GetItemText(v_iItem,1);								//��������
	v_sPM_Name=m_cList.GetItemText(v_iItem,2);							//��������

	//��ʾ
	m_cList.SetItemText(v_iItem,3,v_sAlarm_Flag);						//
	m_cList.SetItemText(v_iItem,4,v_sAlarm_Value);						//
	m_cList.SetItemText(v_iItem,5,v_sAlarm_Type);						//

	//����
	mLTE_PM::iterator		v_pPM_Name;									//������
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//��ָ��
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//��������
	{
		v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();			//��ָ��
		while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//��������
		{
			if (v_pLTE_PM_Type->first==v_sType && v_pPM_Name->second.v_sPM_Name==v_sPM_Name)	//���� && ���� ���
			{
				v_pPM_Name->second.v_sAlarm_Flag=v_sAlarm_Flag;			//Ԥ����־
				v_pPM_Name->second.v_dAlarm_Value=atof(v_sAlarm_Value);	//Ԥ������
				v_pPM_Name->second.v_sAlarm_Type=v_sAlarm_Type;			//Ԥ������
				break;													//�˳�
			}
			v_pPM_Name++;												//�ݼ�
		}
		v_pLTE_PM_Type++;												//�ݼ�
	}
	m_cUpdate.EnableWindow(false);										//��Ч
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
