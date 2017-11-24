// User_Password.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "User_Password.h"


// CUser_Password �Ի���

IMPLEMENT_DYNAMIC(CUser_Password, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CUser_Password::CUser_Password(CWnd* pParent /*=NULL*/)
	: CDialog(CUser_Password::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CUser_Password::~CUser_Password()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CUser_Password::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_cPassword1);
	DDX_Control(pDX, IDC_EDIT4, m_cPassword2);
	DDX_Control(pDX, IDC_EDIT1, m_cPassword_Old);
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CUser_Password, CDialog)
	ON_BN_CLICKED(IDOK, &CUser_Password::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUser_Password::OnBnClickedCancel)
END_MESSAGE_MAP()


// CUser_Password ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CUser_Password::PreTranslateMessage(MSG* pMsg)
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
void CUser_Password::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp,v_sTemp2,v_sSql;								//��ʱ����

	UpdateData(true);													//��ȡ����Ļ��Ϣ

	m_cPassword_Old.GetWindowText(v_sTemp);								//ԭ����
	if(v_sTemp.Trim() != theApp.a_sUser_Password)
	{
		MessageBox("ԭʼ�����������","��Ϣ��ʾ",MB_OK);
		return;
	}

	m_cPassword1.GetWindowText(v_sTemp);								//������1
	m_cPassword2.GetWindowText(v_sTemp2);								//������2
	if(v_sTemp.Trim()!=v_sTemp2.Trim())
	{
		MessageBox("�����������벻ͬ�����������룡����","��Ϣ��ʾ",MB_OK);
		return;
	}

	theApp.a_sUser_Password = v_sTemp;									//���棺������

	v_sSql="update BAS_User set zPassword='"+v_sTemp+"' where zAccount='"+theApp.a_sUser_Account+"'";
	My_ExecuteSQL(v_sSql);												//�޸�

	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ȡ��
//------------------------------------------------------------------------------------------------------
void CUser_Password::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CUser_Password::My_ExecuteSQL(CString v_sSql)
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
