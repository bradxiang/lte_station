// User_Login.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "User_Login.h"


// CUser_Login 对话框

IMPLEMENT_DYNAMIC(CUser_Login, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CUser_Login::CUser_Login(CWnd* pParent /*=NULL*/)
	: CDialog(CUser_Login::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CUser_Login::~CUser_Login()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CUser_Login::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cAccount);
	DDX_Control(pDX, IDC_EDIT2, m_cPassword);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CUser_Login, CDialog)
	ON_BN_CLICKED(IDOK, &CUser_Login::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, &CUser_Login::OnBnClickedCancel)
END_MESSAGE_MAP()


// CUser_Login 消息处理程序

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CUser_Login::PreTranslateMessage(MSG* pMsg)
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
void CUser_Login::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString v_sSql,v_sTemp;												//临时变量
	CString v_sName,v_sAccount,v_sPassword,v_sRole_ID,v_sPower_Name;	//临时变量

	UpdateData(true);													//获得：屏幕信息

	m_cAccount.GetWindowText(v_sAccount);								//获取：账号
	v_sAccount.Trim();
	m_cPassword.GetWindowText(v_sPassword);								//获取：密码
	v_sPassword.Trim();

	//账号有效
	if(v_sAccount.IsEmpty())
	{
		MessageBox("请输入用户账号！！！","信息提示",MB_OK);			//提示
		m_cAccount.SetFocus();											//聚焦
		return;
	}

	//用户登陆信息：初始化
	if(!theApp.a_sUser_Account.IsEmpty())
	{
		theApp.a_sUser_Account = "";
		theApp.a_sUser_Name = "";
		theApp.a_sUser_Password ="";
		//用户登录权限，如何处理用户登录权限
		theApp.a_mUser_Power.clear();									//清除所有权限
		theApp.a_bUser_ReadOnly=false;									//赋值：只读权限无效 【退服处理需要】
		theApp.a_bUser_Update=false;									//赋值：更新权限 【网元管理需要】
		theApp.a_bUser_Delete=false;									//赋值：删除权限 【网元管理需要】
	}

	//用户验证
	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例

	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//获取：用户信息
		v_sSql = "Select * from BAS_User where zAccount = '"+v_sAccount+"' and zPassWord='"+v_sPassword+"'";
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		if(v_pRecordset->adoEOF)
		{
			MessageBox("该用户不存在或密码错误，请重新输入。");			//窗体提示
			v_cAdoConn.ExitConnect();									//断开连接
			return ;
		}
		v_sName=v_cAdoConn.GetField_String("zName");					//zName
		int	v_iRole_ID=v_cAdoConn.GetField_Int("zRole_ID");				//zRole_ID
		v_sRole_ID.Format("%d",v_iRole_ID);								//字符串
		//用户登陆信息
		theApp.a_sUser_Account = v_sAccount;							//账号
		theApp.a_sUser_Name = v_sName;									//姓名
		theApp.a_sUser_Password =v_sPassword;							//密码

		//获取：权限信息
		v_sSql="SELECT BAS_User_Power.zPower_Name, BAS_User_RP.* FROM BAS_User_RP INNER JOIN \
			    BAS_User_Power ON BAS_User_RP.zPower_ID = BAS_User_Power.zPower_ID where zRole_ID="+v_sRole_ID+" order by BAS_User_Power.zPower_ID";	//查询数据库：USER_RP
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sPower_Name=v_cAdoConn.GetField_String("zPower_Name");	//zPower_Name

			theApp.a_mUser_Power.insert(pair<CString,bool>(v_sPower_Name,true));	//权限有效

			//赋值：只读权限
			if (v_sPower_Name=="只读权限")
				theApp.a_bUser_ReadOnly=true;							//赋值：只读权限 【退服处理需要】
			else	if (v_sPower_Name=="修改权限")
				theApp.a_bUser_Update=true;								//赋值：更新权限 【网元管理需要】
			else	if (v_sPower_Name=="删除权限")
				theApp.a_bUser_Delete=true;								//赋值：删除权限 【网元管理需要】

			v_pRecordset->MoveNext();									//指针向后移动
		}

		v_cAdoConn.ExitConnect();										//断开连接

		//登陆信息记录到日志表中
		//获取网络IP
		char HostName[100];												//缓冲区
		gethostname(HostName,sizeof(HostName));							//获得本机主机名.   
		hostent*   hn;   
		hn = gethostbyname(HostName);									//根据本机主机名得到本机ip
		theApp.a_sUser_IP = inet_ntoa(*(struct in_addr *)hn->h_addr_list[0]);	//把ip换成字符串形式  

		COleDateTime v_tTime=COleDateTime::GetCurrentTime();
		CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");
		CString v_sSql="Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"+v_sCurrentTime+"','用户登录','"+v_sName+" 登录，IP："+theApp.a_sUser_IP+"')";
		My_ExecuteSQL(v_sSql);											//数据库执行
//		My_User_Online();												//在线用户登记
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：取消
//------------------------------------------------------------------------------------------------------
void CUser_Login::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

//------------------------------------------------------------------------------------------------------
//	函数功能：在线用户登记
//------------------------------------------------------------------------------------------------------
void CUser_Login::My_User_Online()
{
	// TODO: Add your control notification handler code here
	CString	v_sSql;														//临时变量

	CMyADOConn v_cAdoConn;												//数据库实例
	_RecordsetPtr v_pRecordset;											//记录集

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//查询
		COleDateTime v_tTime=COleDateTime::GetCurrentTime();
		CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");
		v_sSql = "Select * from BAS_User_Online where zIP = '"+theApp.a_sUser_IP+"'";
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		if(v_pRecordset->adoEOF)
			v_sSql="insert into BAS_USER_Online(zIP,zName,zDate) values ('"+theApp.a_sUser_IP+"','"
					+theApp.a_sUser_Name+"','"+v_sCurrentTime+"')";
		else
			v_sSql="Update BAS_USER_Online set zName='"+theApp.a_sUser_Name+"',zDate='"+v_sCurrentTime+"'";
		v_cAdoConn.ExitConnect();										//关闭数据库
		My_ExecuteSQL(v_sSql);											//数据库执行
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//关闭数据库

		CString v_sError = (char *)e.Description();						//获得：出错信息
		MessageBox(v_sError);											//失败
	}
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CUser_Login::My_ExecuteSQL(CString v_sSql)
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
