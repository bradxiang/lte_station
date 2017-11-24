// User_Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "User_Login.h"


// CUser_Login �Ի���

IMPLEMENT_DYNAMIC(CUser_Login, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CUser_Login::CUser_Login(CWnd* pParent /*=NULL*/)
	: CDialog(CUser_Login::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CUser_Login::~CUser_Login()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CUser_Login::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cAccount);
	DDX_Control(pDX, IDC_EDIT2, m_cPassword);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CUser_Login, CDialog)
	ON_BN_CLICKED(IDOK, &CUser_Login::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, &CUser_Login::OnBnClickedCancel)
END_MESSAGE_MAP()


// CUser_Login ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CUser_Login::PreTranslateMessage(MSG* pMsg)
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
//	�������ܣ�ȷ��
//------------------------------------------------------------------------------------------------------
void CUser_Login::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString v_sSql,v_sTemp;												//��ʱ����
	CString v_sName,v_sAccount,v_sPassword,v_sRole_ID,v_sPower_Name;	//��ʱ����

	UpdateData(true);													//��ã���Ļ��Ϣ

	m_cAccount.GetWindowText(v_sAccount);								//��ȡ���˺�
	v_sAccount.Trim();
	m_cPassword.GetWindowText(v_sPassword);								//��ȡ������
	v_sPassword.Trim();

	//�˺���Ч
	if(v_sAccount.IsEmpty())
	{
		MessageBox("�������û��˺ţ�����","��Ϣ��ʾ",MB_OK);			//��ʾ
		m_cAccount.SetFocus();											//�۽�
		return;
	}

	//�û���½��Ϣ����ʼ��
	if(!theApp.a_sUser_Account.IsEmpty())
	{
		theApp.a_sUser_Account = "";
		theApp.a_sUser_Name = "";
		theApp.a_sUser_Password ="";
		//�û���¼Ȩ�ޣ���δ����û���¼Ȩ��
		theApp.a_mUser_Power.clear();									//�������Ȩ��
		theApp.a_bUser_ReadOnly=false;									//��ֵ��ֻ��Ȩ����Ч ���˷�������Ҫ��
		theApp.a_bUser_Update=false;									//��ֵ������Ȩ�� ����Ԫ������Ҫ��
		theApp.a_bUser_Delete=false;									//��ֵ��ɾ��Ȩ�� ����Ԫ������Ҫ��
	}

	//�û���֤
	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��

	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��ȡ���û���Ϣ
		v_sSql = "Select * from BAS_User where zAccount = '"+v_sAccount+"' and zPassWord='"+v_sPassword+"'";
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		if(v_pRecordset->adoEOF)
		{
			MessageBox("���û������ڻ�����������������롣");			//������ʾ
			v_cAdoConn.ExitConnect();									//�Ͽ�����
			return ;
		}
		v_sName=v_cAdoConn.GetField_String("zName");					//zName
		int	v_iRole_ID=v_cAdoConn.GetField_Int("zRole_ID");				//zRole_ID
		v_sRole_ID.Format("%d",v_iRole_ID);								//�ַ���
		//�û���½��Ϣ
		theApp.a_sUser_Account = v_sAccount;							//�˺�
		theApp.a_sUser_Name = v_sName;									//����
		theApp.a_sUser_Password =v_sPassword;							//����

		//��ȡ��Ȩ����Ϣ
		v_sSql="SELECT BAS_User_Power.zPower_Name, BAS_User_RP.* FROM BAS_User_RP INNER JOIN \
			    BAS_User_Power ON BAS_User_RP.zPower_ID = BAS_User_Power.zPower_ID where zRole_ID="+v_sRole_ID+" order by BAS_User_Power.zPower_ID";	//��ѯ���ݿ⣺USER_RP
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		while(!v_pRecordset->adoEOF)
		{
			v_sPower_Name=v_cAdoConn.GetField_String("zPower_Name");	//zPower_Name

			theApp.a_mUser_Power.insert(pair<CString,bool>(v_sPower_Name,true));	//Ȩ����Ч

			//��ֵ��ֻ��Ȩ��
			if (v_sPower_Name=="ֻ��Ȩ��")
				theApp.a_bUser_ReadOnly=true;							//��ֵ��ֻ��Ȩ�� ���˷�������Ҫ��
			else	if (v_sPower_Name=="�޸�Ȩ��")
				theApp.a_bUser_Update=true;								//��ֵ������Ȩ�� ����Ԫ������Ҫ��
			else	if (v_sPower_Name=="ɾ��Ȩ��")
				theApp.a_bUser_Delete=true;								//��ֵ��ɾ��Ȩ�� ����Ԫ������Ҫ��

			v_pRecordset->MoveNext();									//ָ������ƶ�
		}

		v_cAdoConn.ExitConnect();										//�Ͽ�����

		//��½��Ϣ��¼����־����
		//��ȡ����IP
		char HostName[100];												//������
		gethostname(HostName,sizeof(HostName));							//��ñ���������.   
		hostent*   hn;   
		hn = gethostbyname(HostName);									//���ݱ����������õ�����ip
		theApp.a_sUser_IP = inet_ntoa(*(struct in_addr *)hn->h_addr_list[0]);	//��ip�����ַ�����ʽ  

		COleDateTime v_tTime=COleDateTime::GetCurrentTime();
		CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");
		CString v_sSql="Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"+v_sCurrentTime+"','�û���¼','"+v_sName+" ��¼��IP��"+theApp.a_sUser_IP+"')";
		My_ExecuteSQL(v_sSql);											//���ݿ�ִ��
//		My_User_Online();												//�����û��Ǽ�
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
//	�������ܣ�ȡ��
//------------------------------------------------------------------------------------------------------
void CUser_Login::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ������û��Ǽ�
//------------------------------------------------------------------------------------------------------
void CUser_Login::My_User_Online()
{
	// TODO: Add your control notification handler code here
	CString	v_sSql;														//��ʱ����

	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	_RecordsetPtr v_pRecordset;											//��¼��

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��ѯ
		COleDateTime v_tTime=COleDateTime::GetCurrentTime();
		CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");
		v_sSql = "Select * from BAS_User_Online where zIP = '"+theApp.a_sUser_IP+"'";
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���
		if(v_pRecordset->adoEOF)
			v_sSql="insert into BAS_USER_Online(zIP,zName,zDate) values ('"+theApp.a_sUser_IP+"','"
					+theApp.a_sUser_Name+"','"+v_sCurrentTime+"')";
		else
			v_sSql="Update BAS_USER_Online set zName='"+theApp.a_sUser_Name+"',zDate='"+v_sCurrentTime+"'";
		v_cAdoConn.ExitConnect();										//�ر����ݿ�
		My_ExecuteSQL(v_sSql);											//���ݿ�ִ��
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�ر����ݿ�

		CString v_sError = (char *)e.Description();						//��ã�������Ϣ
		MessageBox(v_sError);											//ʧ��
	}
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CUser_Login::My_ExecuteSQL(CString v_sSql)
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
