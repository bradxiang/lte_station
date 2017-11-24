// CON_Station_Region.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "CON_Station_Region.h"


// CCON_Station_Region �Ի���

IMPLEMENT_DYNAMIC(CCON_Station_Region, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CCON_Station_Region::CCON_Station_Region(CWnd* pParent /*=NULL*/)
	: CDialog(CCON_Station_Region::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CCON_Station_Region::~CCON_Station_Region()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CCON_Station_Region::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cAccount);
	DDX_Control(pDX, IDC_EDIT2, m_cPassword);
	DDX_Control(pDX, IDOK, m_cRegion);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_Station_Region, CDialog)
	ON_BN_CLICKED(IDOK, &CCON_Station_Region::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, &CCON_Station_Region::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCON_Station_Region ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CCON_Station_Region::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ�����ͬ��
//------------------------------------------------------------------------------------------------------
void CCON_Station_Region::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	v_iWorking=0;														//�������񣺻�Ϊ�ļ�ѡ��

	v_iThread_Exit=1;													//�̣߳�����
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//������ѯ���ݿ��߳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��߳�����
//------------------------------------------------------------------------------------------------------
UINT CCON_Station_Region::My_Thread_Inoput(LPVOID lparam)
{
	CCON_Station_Region *  lp_this = NULL ;
	lp_this = (CCON_Station_Region *)lparam;

	lp_this->m_cRegion.EnableWindow(false);								//�ļ�ѡ����Ч

	lp_this->My_Input_Main();											//����_������

	lp_this->m_cRegion.EnableWindow(true);								//�ļ�ѡ����Ч

	lp_this->v_iThread_Exit=0;											//�̣߳��˳�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����_������
//------------------------------------------------------------------------------------------------------
void CCON_Station_Region::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 0:
		My_Region();													//����ͬ��
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ͬ��
//  �������̣���1����ȡGSM\TD-LTEС����վַ������
//			  ��2����ȡվַ��������С����վַ����Ƚϣ���ͬ��ͬ����
//------------------------------------------------------------------------------------------------------
void CCON_Station_Region::My_Region()
{
	CString		v_sSql,v_sTemp,v_sRegion,v_sSation,v_sNew;				//��ʱ����
	int		v_iSum=0;													//
	int		v_iSum_Sql=0;												//

	//վַ����
	mString_String				v_mStation_Region;						//վַ��������
	mString_String::iterator	v_pStation_Region;						//������

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��GSM��
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select zStation,zRegion FROM GSM_Cell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			if (v_iThread_Exit==2)										//ǿ���˳�������
				break;													//�˳�

			v_sSation = v_cAdoConn.GetField_String("zStation");			//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion

			v_pStation_Region=v_mStation_Region.find(v_sSation);		//���ң�վַ
			if (v_pStation_Region==v_mStation_Region.end())
				v_mStation_Region.insert(pair<CString,CString>(v_sSation,v_sRegion));	//��������

			if (v_mStation_Region.size()%100==0)
			{
				v_sTemp.Format("%d",v_mStation_Region.size());			//��������
				m_cAccount.SetWindowText(v_sTemp);						//��������
			}

			v_pRecordset->MoveNext();									//��һ��
		}

		//��TD-LTE��
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select zStation,zRegion FROM LTE_Cell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			if (v_iThread_Exit==2)										//ǿ���˳�������
				break;													//�˳�

			v_sSation = v_cAdoConn.GetField_String("zStation");			//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion

			v_pStation_Region=v_mStation_Region.find(v_sSation);		//���ң�վַ
			if (v_pStation_Region==v_mStation_Region.end())
				v_mStation_Region.insert(pair<CString,CString>(v_sSation,v_sRegion));	//��������

			if (v_mStation_Region.size()%100==0)
			{
				v_sTemp.Format("%d",v_mStation_Region.size());			//��������
				m_cAccount.SetWindowText(v_sTemp);						//��������
			}

			v_pRecordset->MoveNext();									//
		}
		v_sTemp.Format("%d",v_mStation_Region.size());					//��������
		m_cAccount.SetWindowText(v_sTemp);								//��������

		//ͬ��
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select zStation,zRegion FROM CON_Station"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		v_sSql="";														//��ʼ��
		v_iSum=0;														//��ʼ��
		while(!v_pRecordset->adoEOF)
		{
			if (v_iThread_Exit==2)										//ǿ���˳�������
				break;													//�˳�

			v_sSation = v_cAdoConn.GetField_String("zStation");			//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion

			v_pStation_Region=v_mStation_Region.find(v_sSation);		//���ң�վַ
			if (v_pStation_Region!=v_mStation_Region.end())
			{
				if (v_sRegion!=v_pStation_Region->second)				//վַ����ȣ�����
				{
					v_iSum++;											//������Station
					v_sNew="Update CON_Station set zRegion='"+v_pStation_Region->second
							+"' where zStation='"+v_sSation+"'";		//SQL

					//���ݿ����
					v_sSql+=v_sNew+"\r\n";								//����һ��SQL
					v_iSum_Sql++;										//SQL����++
					if(v_iSum_Sql==1000)								//ÿ1000����д���ݱ�
					{
						My_ExecuteSQL(v_sSql);							//SQLִ��
						v_sSql="";										//��ʼ��
						v_iSum_Sql=0;									//��ʼ��
					}

					if (v_iSum%100==0)
					{
						v_sTemp.Format("%d",v_iSum);					//��������
						m_cPassword.SetWindowText(v_sTemp);				//��������
					}
				}
			}

			v_pRecordset->MoveNext();									//��һ��
		}
		v_sTemp.Format("%d",v_iSum);									//��������
		m_cPassword.SetWindowText(v_sTemp);								//��������

		if(v_iSum_Sql>0)												//��SQL��Ϣ������ ��д���ݱ�
			My_ExecuteSQL(v_sSql);										//SQLִ��
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}

	v_mStation_Region.clear();											//�������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ȡ��
//------------------------------------------------------------------------------------------------------
void CCON_Station_Region::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CCON_Station_Region::My_ExecuteSQL(CString v_sSql)
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
