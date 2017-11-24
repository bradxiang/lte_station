// Alarm_LL_Real_Time.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "Alarm_LL_Real_Time.h"


// CAlarm_LL_Real_Time 对话框

IMPLEMENT_DYNAMIC(CAlarm_LL_Real_Time, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Real_Time::CAlarm_LL_Real_Time(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarm_LL_Real_Time::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Real_Time::~CAlarm_LL_Real_Time()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
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
//	函数功能：屏幕消息映射
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


// CAlarm_LL_Real_Time 消息处理程序


//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CAlarm_LL_Real_Time::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//设置：参数
	m_cPara.AddString("1组");											//参数设置
	m_cPara.AddString("2组");
	m_cPara.AddString("3组");
	m_cPara.AddString("4组");
	m_cPara.AddString("5组");
	m_cPara.SetCurSel(0);												//默认

	//显示参数
	OnCbnSelchangeCombo3();												//显示：参数[默认的参数组]

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CAlarm_LL_Real_Time::PreTranslateMessage(MSG* pMsg)
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
// 消息预处理：关闭对话窗
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real_Time::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：下行流量复选钮响应
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck9()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_cDown.GetCheck())											//选择无效？？？
	{
		m_cDown_Time_1.SetCheck(0);										//粒度无效
		m_cDown_Time_2.SetCheck(0);										//粒度无效
		m_cDown_Time_3.SetCheck(0);										//粒度无效
		m_cDown_Time_4.SetCheck(0);										//粒度无效
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：下行粒度复选钮响应
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cDown_Time_1.GetCheck())										//粒度选择有效？？？
		m_cDown.SetCheck(1);											//下行流量：有效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：下行粒度复选钮响应
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cDown_Time_2.GetCheck())										//粒度选择有效？？？
		m_cDown.SetCheck(1);											//下行流量：有效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：下行粒度复选钮响应
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck6()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cDown_Time_3.GetCheck())										//粒度选择有效？？？
		m_cDown.SetCheck(1);											//下行流量：有效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：下行粒度复选钮响应
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck7()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cDown_Time_4.GetCheck())										//粒度选择有效？？？
		m_cDown.SetCheck(1);											//下行流量：有效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：上行粒度复选钮响应
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck13()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_cUp.GetCheck())												//选择无效？？？
	{
		m_cUp_Time_1.SetCheck(0);										//粒度无效
		m_cUp_Time_2.SetCheck(0);										//粒度无效
		m_cUp_Time_3.SetCheck(0);										//粒度无效
		m_cUp_Time_4.SetCheck(0);										//粒度无效
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：上行粒度复选钮响应
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck8()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cUp_Time_1.GetCheck())										//粒度选择有效？？？
		m_cUp.SetCheck(1);												//下行流量：有效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：上行粒度复选钮响应
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck10()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cUp_Time_2.GetCheck())										//粒度选择有效？？？
		m_cUp.SetCheck(1);												//下行流量：有效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：上行粒度复选钮响应
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck11()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cUp_Time_3.GetCheck())										//粒度选择有效？？？
		m_cUp.SetCheck(1);												//下行流量：有效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：上行粒度复选钮响应
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedCheck12()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cUp_Time_4.GetCheck())										//粒度选择有效？？？
		m_cUp.SetCheck(1);												//下行流量：有效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：参数组选择
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql;													//临时变量
	int			v_iPara;												//临时变量
	CString		v_sTemp;												//临时变量

	int			v_iDown=0,v_iDown_Time_1=0,v_iDown_Time_2=0,v_iDown_Time_3=0,v_iDown_Time_4=0;	//粒度
	int			v_iUp=0,v_iUp_Time_1=0,v_iUp_Time_2=0,v_iUp_Time_3=0,v_iUp_Time_4=0;			//粒度
	double		v_dDown_Value_1=0,v_dDown_Value_2=0,v_dDown_Value_3=0,v_dDown_Value_4=0;		//数值
	double		v_dUp_Value_1=0,v_dUp_Value_2=0,v_dUp_Value_3=0,v_dUp_Value_4=0;				//数值

	//获取：参数组
	v_iPara=m_cPara.GetCurSel()+1;										//获取：参数组

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		//连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//查询：参数，依据登陆帐号+参数组号；
		v_sTemp.Format("%d",v_iPara);									//字符串
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select * from CON_Pm_LTE_LL where zAccount='"+theApp.a_sUser_Account+"' and zPara="+v_sTemp;	//查询数据库

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		if(!v_pRecordset->adoEOF)
		{
			//获取：下行流量参数
			v_iDown=v_cAdoConn.GetField_Int("zDown");					//zDown
			v_iDown_Time_1=v_cAdoConn.GetField_Int("zDown_Time_1");		//zDown_Time_1
			v_iDown_Time_2=v_cAdoConn.GetField_Int("zDown_Time_2");		//zDown_Time_2
			v_iDown_Time_3=v_cAdoConn.GetField_Int("zDown_Time_3");		//zDown_Time_3
			v_iDown_Time_4=v_cAdoConn.GetField_Int("zDown_Time_4");		//zDown_Time_4
			v_dDown_Value_1=v_cAdoConn.GetField_Double("zDown_Value_1");//zDown_Value_1
			v_dDown_Value_2=v_cAdoConn.GetField_Double("zDown_Value_2");//zDown_Value_2
			v_dDown_Value_3=v_cAdoConn.GetField_Double("zDown_Value_3");//zDown_Value_3
			v_dDown_Value_4=v_cAdoConn.GetField_Double("zDown_Value_4");//zDown_Value_4

			//获取：上行流量参数
			v_iUp=v_cAdoConn.GetField_Int("zUp");						//zUp
			v_iUp_Time_1=v_cAdoConn.GetField_Int("zUp_Time_1");			//zUp_Time_1
			v_iUp_Time_2=v_cAdoConn.GetField_Int("zUp_Time_2");			//zUp_Time_2
			v_iUp_Time_3=v_cAdoConn.GetField_Int("zUp_Time_3");			//zUp_Time_3
			v_iUp_Time_4=v_cAdoConn.GetField_Int("zUp_Time_4");			//zUp_Time_4
			v_dUp_Value_1=v_cAdoConn.GetField_Double("zUp_Value_1");	//zUp_Value_1
			v_dUp_Value_2=v_cAdoConn.GetField_Double("zUp_Value_2");	//zUp_Value_2
			v_dUp_Value_3=v_cAdoConn.GetField_Double("zUp_Value_3");	//zUp_Value_3
			v_dUp_Value_4=v_cAdoConn.GetField_Double("zUp_Value_4");	//zUp_Value_4

			//断开数据库
			v_cAdoConn.ExitConnect();									//断开连接
		}
		else															//无组参数：则增加参数组[数据为空]
		{
			//断开数据库
			v_cAdoConn.ExitConnect();									//断开连接

			v_sSql="insert into CON_Pm_LTE_LL(zAccount,zPara) values ('"+theApp.a_sUser_Account+"','"+v_sTemp+"')";	//sql
			My_ExecuteSQL(v_sSql);										//执行
		}

		//显示：下行参数
		m_cDown.SetCheck(v_iDown);										//状态：下行

		//获取：粒度状态
		m_cDown_Time_1.SetCheck(v_iDown_Time_1);						//
		m_cDown_Time_2.SetCheck(v_iDown_Time_2);						//
		m_cDown_Time_3.SetCheck(v_iDown_Time_3);						//
		m_cDown_Time_4.SetCheck(v_iDown_Time_4);						//
		//获取：粒度数值
		v_sTemp.Format("%8.2f",v_dDown_Value_1);						//字符串
		m_cDown_Value_1.SetWindowText(v_sTemp);							//
		v_sTemp.Format("%8.2f",v_dDown_Value_2);						//字符串
		m_cDown_Value_2.SetWindowText(v_sTemp);							//
		v_sTemp.Format("%8.2f",v_dDown_Value_3);						//字符串
		m_cDown_Value_3.SetWindowText(v_sTemp);							//
		v_sTemp.Format("%8.2f",v_dDown_Value_4);						//字符串
		m_cDown_Value_4.SetWindowText(v_sTemp);							//

		//显示：上行参数
		m_cUp.SetCheck(v_iUp);											//状态：下行
		//获取：粒度状态
		m_cUp_Time_1.SetCheck(v_iUp_Time_1);							//
		m_cUp_Time_2.SetCheck(v_iUp_Time_2);							//
		m_cUp_Time_3.SetCheck(v_iUp_Time_3);							//
		m_cUp_Time_4.SetCheck(v_iUp_Time_4);							//
		//获取：粒度数值
		v_sTemp.Format("%8.2f",v_dUp_Value_1);							//字符串
		m_cUp_Value_1.SetWindowText(v_sTemp);							//
		v_sTemp.Format("%8.2f",v_dUp_Value_2);							//字符串
		m_cUp_Value_2.SetWindowText(v_sTemp);							//
		v_sTemp.Format("%8.2f",v_dUp_Value_3);							//字符串
		m_cUp_Value_3.SetWindowText(v_sTemp);							//
		v_sTemp.Format("%8.2f",v_dUp_Value_4);							//字符串
		m_cUp_Value_4.SetWindowText(v_sTemp);							//
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：参数保存
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql;													//临时变量
	int			v_iPara;												//临时变量
	CString		v_sTemp;												//临时变量

	int			v_iDown=0,v_iDown_Time_1=0,v_iDown_Time_2=0,v_iDown_Time_3=0,v_iDown_Time_4=0;	//粒度
	int			v_iUp=0,v_iUp_Time_1=0,v_iUp_Time_2=0,v_iUp_Time_3=0,v_iUp_Time_4=0;			//粒度
	CString		v_sDown=0,v_sDown_Time_1=0,v_sDown_Time_2=0,v_sDown_Time_3=0,v_sDown_Time_4=0;	//粒度
	CString		v_sUp=0,v_sUp_Time_1=0,v_sUp_Time_2=0,v_sUp_Time_3=0,v_sUp_Time_4=0;			//粒度
	CString		v_sDown_Value_1=0,v_sDown_Value_2=0,v_sDown_Value_3=0,v_sDown_Value_4=0;		//数值
	CString		v_sUp_Value_1=0,v_sUp_Value_2=0,v_sUp_Value_3=0,v_sUp_Value_4=0;				//数值

	//获取：参数组
	v_iPara=m_cPara.GetCurSel()+1;										//获取：参数组
	v_sTemp.Format("%d",v_iPara);										//字符串

	//保存
	if(MessageBox("是否保存参数组： "+v_sTemp+" 组","保存确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
	{
		//获取数据：下行数据
		v_iDown=m_cDown.GetCheck();										//状态
		v_sDown.Format("%d",v_iDown);									//字符串

		//获取：粒度状态
		v_iDown_Time_1=m_cDown_Time_1.GetCheck();						//粒度
		v_iDown_Time_2=m_cDown_Time_2.GetCheck();						//粒度
		v_iDown_Time_3=m_cDown_Time_3.GetCheck();						//粒度
		v_iDown_Time_4=m_cDown_Time_4.GetCheck();						//粒度
		v_sDown_Time_1.Format("%d",v_iDown_Time_1);						//字符串
		v_sDown_Time_2.Format("%d",v_iDown_Time_2);						//字符串
		v_sDown_Time_3.Format("%d",v_iDown_Time_3);						//字符串
		v_sDown_Time_4.Format("%d",v_iDown_Time_4);						//字符串

		//获取：粒度数值
		m_cDown_Value_1.GetWindowText(v_sDown_Value_1);					//数值
		m_cDown_Value_2.GetWindowText(v_sDown_Value_2);					//数值
		m_cDown_Value_3.GetWindowText(v_sDown_Value_3);					//数值
		m_cDown_Value_4.GetWindowText(v_sDown_Value_4);					//数值
		
		//获取数据：上行数据
		v_iUp=m_cUp.GetCheck();											//状态
		v_sUp.Format("%d",v_iUp);										//字符串

		//获取：粒度状态
		v_iUp_Time_1=m_cUp_Time_1.GetCheck();							//粒度
		v_iUp_Time_2=m_cUp_Time_2.GetCheck();							//粒度
		v_iUp_Time_3=m_cUp_Time_3.GetCheck();							//粒度
		v_iUp_Time_4=m_cUp_Time_4.GetCheck();							//粒度
		v_sUp_Time_1.Format("%d",v_iUp_Time_1);							//字符串
		v_sUp_Time_2.Format("%d",v_iUp_Time_2);							//字符串
		v_sUp_Time_3.Format("%d",v_iUp_Time_3);							//字符串
		v_sUp_Time_4.Format("%d",v_iUp_Time_4);							//字符串

		//获取：粒度数值
		m_cUp_Value_1.GetWindowText(v_sUp_Value_1);						//数值
		m_cUp_Value_2.GetWindowText(v_sUp_Value_2);						//数值
		m_cUp_Value_3.GetWindowText(v_sUp_Value_3);						//数值
		m_cUp_Value_4.GetWindowText(v_sUp_Value_4);						//数值

		//保存
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
		My_ExecuteSQL(v_sSql);											//执行

		//显示
		OnCbnSelchangeCombo3();											//显示：删除后的数据
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：参数删除
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql;													//临时变量
	int			v_iPara;												//临时变量
	CString		v_sTemp;												//临时变量

	//获取：参数组
	v_iPara=m_cPara.GetCurSel()+1;										//获取：参数组
	v_sTemp.Format("%d",v_iPara);										//字符串

	//删除
	if(MessageBox("是否删除参数组： "+v_sTemp+" 组","删除确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
	{
		v_sSql="delete from CON_Pm_LTE_LL where zAccount='"+theApp.a_sUser_Account+"' and zPara="+v_sTemp;	//查询数据库
		My_ExecuteSQL(v_sSql);											//执行

		//显示
		OnCbnSelchangeCombo3();											//显示：删除后的数据
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：选定退出
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real_Time::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//退出
	OnOK();
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CAlarm_LL_Real_Time::My_ExecuteSQL(CString v_sSql)
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
