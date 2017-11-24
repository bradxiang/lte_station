// User_Password.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "User_Password.h"


// CUser_Password 对话框

IMPLEMENT_DYNAMIC(CUser_Password, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CUser_Password::CUser_Password(CWnd* pParent /*=NULL*/)
	: CDialog(CUser_Password::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CUser_Password::~CUser_Password()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CUser_Password::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_cPassword1);
	DDX_Control(pDX, IDC_EDIT4, m_cPassword2);
	DDX_Control(pDX, IDC_EDIT1, m_cPassword_Old);
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CUser_Password, CDialog)
	ON_BN_CLICKED(IDOK, &CUser_Password::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUser_Password::OnBnClickedCancel)
END_MESSAGE_MAP()


// CUser_Password 消息处理程序

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CUser_Password::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		CDialog* Pwnd=(CDialog*)GetActiveWindow();						//取得对话框指针 
		Pwnd->NextDlgCtrl();											//切换到下一个输入焦点 
		return true;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：确认
//------------------------------------------------------------------------------------------------------
void CUser_Password::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp,v_sTemp2,v_sSql;								//临时变量

	UpdateData(true);													//获取：屏幕信息

	m_cPassword_Old.GetWindowText(v_sTemp);								//原密码
	if(v_sTemp.Trim() != theApp.a_sUser_Password)
	{
		MessageBox("原始密码输入错误","信息提示",MB_OK);
		return;
	}

	m_cPassword1.GetWindowText(v_sTemp);								//新密码1
	m_cPassword2.GetWindowText(v_sTemp2);								//新密码2
	if(v_sTemp.Trim()!=v_sTemp2.Trim())
	{
		MessageBox("两次密码输入不同，请重新输入！！！","信息提示",MB_OK);
		return;
	}

	theApp.a_sUser_Password = v_sTemp;									//保存：新密码

	v_sSql="update BAS_User set zPassword='"+v_sTemp+"' where zAccount='"+theApp.a_sUser_Account+"'";
	My_ExecuteSQL(v_sSql);												//修改

	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：取消
//------------------------------------------------------------------------------------------------------
void CUser_Password::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CUser_Password::My_ExecuteSQL(CString v_sSql)
{
	// TODO: Add your control notification handler code here
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);

		v_cAdoConn.ExecuteSQL((_bstr_t)v_sSql);							//执行数据库操作

		v_cAdoConn.ExitConnect();										//断开连接
		return true;
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//关闭数据库

		CString v_sError = (char *)e.Description();						//获得：出错信息
		MessageBox(v_sError);											//失败
		return false;
	}
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
