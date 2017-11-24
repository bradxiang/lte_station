// Pm_GSM_UpDate.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "Pm_GSM_UpDate.h"


// CPm_GSM_UpDate 对话框

IMPLEMENT_DYNAMIC(CPm_GSM_UpDate, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CPm_GSM_UpDate::CPm_GSM_UpDate(CWnd* pParent /*=NULL*/)
	: CDialog(CPm_GSM_UpDate::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CPm_GSM_UpDate::~CPm_GSM_UpDate()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CPm_GSM_UpDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cPM_Table);
	DDX_Control(pDX, IDC_EDIT2, m_cType);
	DDX_Control(pDX, IDC_EDIT3, m_cDay);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPm_GSM_UpDate, CDialog)
	ON_BN_CLICKED(IDOK, &CPm_GSM_UpDate::OnBnClickedOk)
END_MESSAGE_MAP()


// CPm_GSM_UpDate 消息处理程序

//------------------------------------------------------------------------------------------------------			
// 函数功能：初始化
//------------------------------------------------------------------------------------------------------			
BOOL CPm_GSM_UpDate::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_cPM_Table.SetWindowText(v_sPM_Table);								//获取：性能组
	m_cType.SetWindowText(v_sType);										//获取：类型
	m_cDay.SetWindowText(v_sDay);										//获取：保留天数

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CPm_GSM_UpDate::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：修改
//------------------------------------------------------------------------------------------------------
void CPm_GSM_UpDate::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString v_sSql,v_sTemp;												//临时变量

	m_cPM_Table.GetWindowText(v_sPM_Table);								//获取：性能组
	v_sPM_Table.Trim();
	m_cDay.GetWindowText(v_sDay);										//获取：类型
	v_sDay.Trim();

	//账号有效
	if(v_sDay.IsEmpty())
	{
		MessageBox("请输入保留天数！！！","信息提示",MB_OK);			//提示
		m_cDay.SetFocus();												//聚焦
		return;
	}

	v_sSql="Update A_CAPGROUP_Info set zsavetime="+v_sDay+" where zgroupname='"+v_sPM_Table+"'";	//SQL
	My_ExecuteSQL(v_sSql);												//执行

	OnOK();
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CPm_GSM_UpDate::My_ExecuteSQL(CString v_sSql)
{
	// TODO: Add your control notification handler code here
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);

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
