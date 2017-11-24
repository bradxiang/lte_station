// CON_Para.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "CON_Para.h"


// CCON_Para 对话框

IMPLEMENT_DYNAMIC(CCON_Para, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CCON_Para::CCON_Para(CWnd* pParent /*=NULL*/)
	: CDialog(CCON_Para::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CCON_Para::~CCON_Para()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CCON_Para::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cHour_Month);
	DDX_Control(pDX, IDC_COMBO3, m_cMin_Day);
	DDX_Control(pDX, IDC_COMBO9, m_cFTP_Day);
	DDX_Control(pDX, IDC_COMBO4, m_cCm_Month);
	DDX_Control(pDX, IDC_COMBO20, m_cCm_FTP_Day);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_Para, CDialog)
	ON_BN_CLICKED(IDOK, &CCON_Para::OnBnClickedOk)
END_MESSAGE_MAP()


// CCON_Para 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CCON_Para::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString	v_sTemp;													//临时变量

	//性能参数
	//定时：小时
	for (int vi=24;vi<=36;vi++)
	{
		v_sTemp.Format("%02d",vi);										//字符串
		m_cHour_Month.AddString(v_sTemp);								//设置：小时性格保留月数
	}

	//定时：分钟
	for (int vi=7;vi<=30;vi++)
	{
		v_sTemp.Format("%02d",vi);										//字符串
		m_cMin_Day.AddString(v_sTemp);									//设置：分钟性能保留天数
	}

	//保存：天数
	for (int vi=30;vi<=90;vi++)
	{
		v_sTemp.Format("%02d",vi);										//字符串
		m_cFTP_Day.AddString(v_sTemp);									//设置：FTP_Log天数
	}
	
	//配置参数
	//数据(月)
	for (int vi=3;vi<=12;vi++)
	{
		v_sTemp.Format("%02d",vi);										//字符串
		m_cCm_Month.AddString(v_sTemp);									//设置：小时性格保留月数
	}

	//FTP(天数)
	for (int vi=10;vi<=30;vi++)
	{
		v_sTemp.Format("%02d",vi);										//字符串
		m_cCm_FTP_Day.AddString(v_sTemp);								//设置：FTP_Log天数
	}

	//显示
	My_Get_Para();														//显示配置信息

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CCON_Para::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString v_sText;													//临时变量

	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		CDialog* Pwnd=(CDialog*)GetActiveWindow();						//取得对话框指针
		CWnd *pDlgItem = Pwnd->GetFocus();  
		if(pDlgItem->GetDlgCtrlID()!=0x3f9)								//不是"指令"控件？？？ 
		{
			return true;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获得参数
//------------------------------------------------------------------------------------------------------
void CCON_Para::My_Get_Para()
{
	CString		v_sSql;													//临时变量
	int			v_iHour,v_iMin,v_iFTP_Day,v_iCm_Month,v_iCmFTP_Day;		//临时变量
	CString		v_sTemp;												//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from CON_System";								//查询数据库 

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		if(!v_pRecordset->adoEOF)
		{
			v_iHour=v_cAdoConn.GetField_Int("zPm_LTE_Month");			//zPm_LTE_Month
			v_iMin=v_cAdoConn.GetField_Int("zPm_LTE_Day");				//zPm_LTE_Day
			v_iFTP_Day=v_cAdoConn.GetField_Int("zPm_LTE_Day_FTP");		//zPm_LTE_Day_FTP
			v_iCm_Month=v_cAdoConn.GetField_Int("zCm_LTE_Month");		//zCm_LTE_Month
			v_iCmFTP_Day=v_cAdoConn.GetField_Int("zCm_LTE_Day_FTP");	//zCm_LTE_Day_FTP

			v_pRecordset->MoveNext();									//指针向后移动
			
			//显示
			m_cHour_Month.SetCurSel(v_iHour-24);						//小时性能
			m_cMin_Day.SetCurSel(v_iMin-7);								//分钟性能
			m_cFTP_Day.SetCurSel(v_iFTP_Day-30);						//天数
			m_cCm_Month.SetCurSel(v_iCm_Month-2);						//月数
			m_cCm_FTP_Day.SetCurSel(v_iCmFTP_Day-10);					//天数
		}
		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：确认
//------------------------------------------------------------------------------------------------------
void CCON_Para::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString v_sSql,v_sTemp;												//临时变量
	CString v_sHour,v_sMin,v_sFTP_Day,v_sCm_Month,v_sCm_FTP_Day;		//临时变量

	UpdateData(true);													//获得：屏幕信息

	//数据库操作
	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例

	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//获取：用户信息
		v_sHour.Format("%d",m_cHour_Month.GetCurSel()+24);				//字符串
		v_sMin.Format("%d",m_cMin_Day.GetCurSel()+7);					//字符串
		v_sFTP_Day.Format("%d",m_cFTP_Day.GetCurSel()+30);				//字符串
		v_sCm_Month.Format("%d",m_cCm_Month.GetCurSel()+2);				//字符串
		v_sCm_FTP_Day.Format("%d",m_cCm_FTP_Day.GetCurSel()+10);		//字符串
		v_sSql = "update CON_System set zPm_LTE_Month="+v_sHour			//小时
				+",zPm_LTE_Day="+v_sMin									//分钟
				+",zPm_LTE_Day_FTP="+v_sFTP_Day							//天数
				+",zCm_LTE_Month="+v_sCm_Month							//月数
				+",zCm_LTE_Day_FTP="+v_sCm_FTP_Day;						//天数

		My_ExecuteSQL(v_sSql);											//执行

		v_cAdoConn.ExitConnect();										//断开连接
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
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CCON_Para::My_ExecuteSQL(CString v_sSql)
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
