// Pm_GSM_UpDate_PM.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "Pm_GSM_UpDate_PM.h"


// CPm_GSM_UpDate_PM 对话框

IMPLEMENT_DYNAMIC(CPm_GSM_UpDate_PM, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CPm_GSM_UpDate_PM::CPm_GSM_UpDate_PM(CWnd* pParent /*=NULL*/)
	: CDialog(CPm_GSM_UpDate_PM::IDD, pParent)
{
	v_bFlag=false;														//标记：0：修改；
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CPm_GSM_UpDate_PM::~CPm_GSM_UpDate_PM()
{
	v_mGSM_PM.clear();													//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CPm_GSM_UpDate_PM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO4, m_cPM_Table);
	DDX_Control(pDX, IDC_EDIT2, m_cPM_Name);
	DDX_Control(pDX, IDC_EDIT3, m_cFormula);
	DDX_Control(pDX, IDC_EDIT4, m_cType);
	DDX_Control(pDX, IDC_EDIT5, m_cObjType);
	DDX_Control(pDX, IDC_EDIT7, m_cUnit);
	DDX_Control(pDX, IDOK, m_cOK);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPm_GSM_UpDate_PM, CDialog)
	ON_BN_CLICKED(IDOK, &CPm_GSM_UpDate_PM::OnBnClickedOk)
	ON_CBN_CLOSEUP(IDC_COMBO4, &CPm_GSM_UpDate_PM::OnCbnCloseupCombo4)
END_MESSAGE_MAP()


// CPm_GSM_UpDate_PM 消息处理程序

//------------------------------------------------------------------------------------------------------			
// 函数功能：初始化
//------------------------------------------------------------------------------------------------------			
BOOL CPm_GSM_UpDate_PM::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	My_Get_PM_Table();													//获取：性能组名称

	m_cPM_Table.SelectString(0,v_sPM_Table);							//获取：性能组
	m_cType.SetWindowText(v_sType);										//获取：类型
	m_cPM_Name.SetWindowText(v_sPM_Name);								//获取：性能名称
	m_cUnit.SetWindowText(v_sUnit);										//获取：单位
	m_cFormula.SetWindowText(v_sFormula);								//获取：计算公式
	m_cObjType.SetWindowText(v_sObjType);								//获取：ObjType

	if (!v_bFlag)														//增加？？？
	{
		m_cPM_Table.EnableWindow(false);								//无效
		m_cUnit.EnableWindow(false);									//无效
		m_cPM_Name.EnableWindow(false);									//无效
	}
	else
		m_cOK.SetWindowText("增加");									//显示

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CPm_GSM_UpDate_PM::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：性能组选择
//------------------------------------------------------------------------------------------------------
void CPm_GSM_UpDate_PM::OnCbnCloseupCombo4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString v_sTemp;													//临时变量

	mGSM_PM::iterator	v_pIter;										//迭代器

	m_cPM_Table.GetWindowText(v_sTemp);									//获取：性能组
	v_pIter=v_mGSM_PM.find(v_sTemp);									//查找？？？
	if (v_pIter!=v_mGSM_PM.end())
		m_cType.SetWindowText(v_pIter->second.v_sType);					//性能类型
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：修改
//------------------------------------------------------------------------------------------------------
void CPm_GSM_UpDate_PM::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString v_sSql,v_sTemp;												//临时变量

	m_cPM_Table.GetWindowText(v_sPM_Table);								//获取：性能组
	v_sPM_Table.Trim();
	m_cPM_Name.GetWindowText(v_sPM_Name);								//获取：性能名称
	v_sPM_Name.Trim();
	m_cFormula.GetWindowText(v_sFormula);								//获取：计算公式
	v_sFormula.Trim();
	m_cObjType.GetWindowText(v_sObjType);								//获取：ObjType
	v_sObjType.Trim();
	m_cUnit.GetWindowText(v_sUnit);										//获取：单位
	v_sUnit.Trim();
	v_sTemp="1";														//BSC计算性能

	//保存
	if (v_bFlag==0)														//修改
		v_sSql="Update A_CAP_Info set zformula='"+v_sFormula+"',zobjtypes='"+v_sObjType+"' where znum='"+v_sNum+"'";		//SQL
	else																//增加
	{
		mGSM_PM::iterator	v_pIter;									//迭代器
		v_pIter=v_mGSM_PM.find(v_sPM_Table);							//查找？？？
		if (v_pIter!=v_mGSM_PM.end())
		{
			v_sSql="insert A_CAP_Info(zgroupnum,zcap,zformula,zobjtypes,zunit,zmark) values ('"
					+v_pIter->second.v_sNum+"','"						//性能组编号
					+v_sPM_Name+"','"									//
					+v_sFormula+"','"									//
					+v_sObjType+"','"									//
					+v_sUnit+"','"										//
					+v_sTemp+"')";										//SQL
		}
	}
	My_ExecuteSQL(v_sSql);												//执行

	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取性能表名称
//------------------------------------------------------------------------------------------------------
void CPm_GSM_UpDate_PM::My_Get_PM_Table()
{
	// TODO: 在此添加控件通知处理程序代码
	CString			v_sSql;												//临时变量
	CString			v_sTemp,v_sTable,v_sType;							//临时变量
	int				v_iNum;												//临时变量

	GSM_PM_Struct	v_cGSM_PM_Struct;									//结构

	//性能数据
	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//获取：计算性能表名称
		if (v_bBSC)
			v_sSql="select * from A_CAPGROUP_Info where ztypename like '%BSC%'";		//加载计算性能表信息
		else
			v_sSql="select * from A_CAPGROUP_Info where ztypename not like '%BSC%'";	//加载计算性能表信息
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("zgroupname");		//获取：性能表名称
			v_sType = v_cAdoConn.GetField_String("ztypename");			//获取：性能类型
			v_iNum = v_cAdoConn.GetField_Int("zgroupnum");				//获取：性能唯一标识

			m_cPM_Table.AddString(v_sTable);							//显示

			v_cGSM_PM_Struct.v_sType=v_sType;							//性能类型
			v_sTemp.Format("%d",v_iNum);								//字符串
			v_cGSM_PM_Struct.v_sNum=v_sTemp;							//性能唯一标识
			v_mGSM_PM.insert(pair<CString,GSM_PM_Struct>(v_sTable,v_cGSM_PM_Struct));	//增加容器

			v_pRecordset->MoveNext();									//指针向后移动
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
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CPm_GSM_UpDate_PM::My_ExecuteSQL(CString v_sSql)
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
