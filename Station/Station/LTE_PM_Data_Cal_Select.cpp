// LTE_PM_Data_Cal_Select.cpp : 实现文件
// 


#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Data_Cal_Select.h"


// CLTE_PM_Data_Cal_Select 对话框

IMPLEMENT_DYNAMIC(CLTE_PM_Data_Cal_Select, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cal_Select::CLTE_PM_Data_Cal_Select(CWnd* pParent /*=NULL*/)
: CDialog(CLTE_PM_Data_Cal_Select::IDD, pParent)
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cal_Select::~CLTE_PM_Data_Cal_Select()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal_Select::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_EDIT1, m_cSum);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_COMBO3, m_cPm_Type);
	DDX_Control(pDX, IDC_BUTTON1, m_cSave);
	DDX_Control(pDX, IDC_EDIT3, m_cPm_Name);
	DDX_Control(pDX, IDC_EDIT2, m_cPm_Value);
	DDX_Control(pDX, IDC_BUTTON4, m_cOK);
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Data_Cal_Select, CDialog)
	ON_BN_CLICKED(IDC_EXCEL, &CLTE_PM_Data_Cal_Select::OnBnClickedExcel)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Data_Cal_Select::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Data_Cal_Select::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CLTE_PM_Data_Cal_Select::OnBnClickedButton4)
END_MESSAGE_MAP()


// CLTE_PM_Data_Cal_Select 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CLTE_PM_Data_Cal_Select::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化：列表窗
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,40);			//
	m_cList.InsertColumn(1,"模板名称",LVCFMT_LEFT,120);			//
	m_cList.InsertColumn(2,"类型",LVCFMT_LEFT,60);				//
	m_cList.InsertColumn(3,"计算公式",LVCFMT_LEFT,380);			//
	m_cList.InsertColumn(4,"创建日期",LVCFMT_CENTER,80);		//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//变量初始化
	v_iItem=-1;															//条目选择无效[-1]
	m_cOK.EnableWindow(false);											//无效

	//信息显示：获取自定义的计算性能
	My_Load_Cal();														//获取自定义的计算性能

	//显示：性能类型
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	for (v_pLTE_PM_Type=v_mLTE_PM_Type->begin();v_pLTE_PM_Type!=v_mLTE_PM_Type->end();v_pLTE_PM_Type++)
		m_cPm_Type.AddString(v_pLTE_PM_Type->first);					//性能类型
	m_cPm_Type.SetCurSel(0);											//默认

	return TRUE;  // return TRUE unless you set the focus to a control
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Data_Cal_Select::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：调整窗体尺寸
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal_Select::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导出Excel
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal_Select::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击性能列表
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal_Select::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int		v_iSel;														//临时变量

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if(v_iItem>=0)													//有效条目序号？？？
			m_cList.SetItemBkColor(v_iItem,RGB(255,255,255));			//恢复背景色
		v_iItem=pNMListView->iItem;										//获得：选中条目序号
		m_cList.SetItemBkColor(v_iItem,RGB(255,0,0));					//设置背景色：红色
		
		//设置：修改内容
		m_cPm_Name.SetWindowText(m_cList.GetItemText(v_iItem,1));		//模板名称
		m_cPm_Value.SetWindowText(m_cList.GetItemText(v_iItem,3));		//计算公式
		v_iSel=m_cPm_Type.FindString(0,m_cList.GetItemText(v_iItem,2));	//性能类型
		m_cPm_Type.SetCurSel(v_iSel);									//选择

		m_cOK.EnableWindow(true);										//有效
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：保存性能
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal_Select::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString			v_sSql,v_sName,v_sFormula,v_sType;					//临时变量

	m_cPm_Name.GetWindowText(v_sName);									//模板名称
	m_cPm_Type.GetWindowText(v_sType);									//性能类型
	m_cPm_Value.GetWindowText(v_sFormula);								//计算公式

	COleDateTime v_tTime=COleDateTime::GetCurrentTime();				//日期
	CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");			//

	if (v_sName.IsEmpty() || v_sType.IsEmpty() || v_sFormula.IsEmpty())	//无效信息？？？
	{
		MessageBox("信息缺失，请输入相关信息！！！");					//提示
		return;															//返回
	}

	if (!My_PM_Cal(theApp.a_sUser_Account,v_sName))						//存在：账号+模板名称 ？？？
	{																	//新增
		v_sSql="Insert into CON_PM_LTE_Case_Cal(zAccount,zName,zType,zPm_Formula,zDate) values ('"
			+theApp.a_sUser_Account+"','"+v_sName+"','"+v_sType+"','"+v_sFormula+"','"+v_sCurrentTime+"')";		//SQL
	}
	else
	{
		v_sSql="Update CON_PM_LTE_Case_Cal set zType='"+v_sType+"',zPm_Formula='"+v_sFormula
			+"' where zAccount='"+theApp.a_sUser_Account+"' and zName='"+v_sName+"'";		//SQL
	}
	My_ExecuteSQL(v_sSql);												//SQL执行
	MessageBox("计算性能保存完毕 ！！！");								//提示
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：确认退出
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal_Select::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyFormula		v_cFormula;											//公式计算

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	//【1】数据获取
	m_cPm_Type.GetWindowText(v_sType);									//性能类型
	m_cPm_Value.GetWindowText(v_sFormula);								//计算公式
	m_cPm_Name.GetWindowText(v_sName);									//模板名称

	if (v_sName.IsEmpty() || v_sType.IsEmpty() || v_sFormula.IsEmpty())	//无效信息？？？
	{
		MessageBox("信息缺失，请输入相关信息！！！");					//提示
		return;															//返回
	}

	//【2】公式检查(公式中的性能是否在选定性的性能集中)
	v_cFormula.My_Get_Formula(v_sFormula);								//分解公式表达式(结果在队列中)

	v_pLTE_PM_Type=v_mLTE_PM_Type->find(v_sType);						//查找：性能类型
	for (v_cFormula.v_pFormula=v_cFormula.v_mFormula.begin();
		v_cFormula.v_pFormula!=v_cFormula.v_mFormula.end();
		v_cFormula.v_pFormula++)										//遍历公式元素
	{
		if (v_cFormula.v_pFormula->v_bFormula==1)						//运算值？？？
		{
			v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.find(v_cFormula.v_pFormula->v_sPM_Formula);	//查找：公式元素是否是原始性能？？？
			if (v_pPM_Name==v_pLTE_PM_Type->second.v_mLTE_PM.end())		//没找到
			{
				MessageBox("计算公式中含有无效的性能参数："+v_cFormula.v_pFormula->v_sPM_Formula+"  !!!");	//窗体提示
				return;													//无效返回
			}
		}
	}

	//【3】正确退出
	OnOK();																//退出
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取自定义计算性能
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal_Select::My_Load_Cal()
{
	int				v_iItem,v_iID=0;									//临时变量
	CString			v_sTemp,v_sDate,v_sSql;								//临时变量
	CString			v_sName,v_sFormula,v_sType;							//临时变量
	COleDateTime	v_dDate;											//临时变量

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT * FROM CON_PM_LTE_Case_Cal where zAccount='"+theApp.a_sUser_Account+"' order by zName";

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			v_sName=v_cAdoConn.GetField_String("zName");				//zName
			v_sType=v_cAdoConn.GetField_String("zType");				//类型
			v_sFormula=v_cAdoConn.GetField_String("zPm_Formula");		//计算公式
			v_dDate = v_cAdoConn.GetField_DataTime("zDate");			//zDate
			v_sDate = v_dDate.Format(_T("%Y-%m-%d"));					//字符串

			//显示
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,v_sName);						//模板名称
			m_cList.SetItemText(v_iItem,2,v_sType);						//类型
			m_cList.SetItemText(v_iItem,3,v_sFormula);					//计算公式
			m_cList.SetItemText(v_iItem,4,v_sDate);						//日期

			v_pRecordset->MoveNext();									//
			v_iID++;													//计数：告警数量
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cSum.SetWindowText(v_sTemp);									//显示：总数

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
//	函数功能：账号、模板存在？？？
//------------------------------------------------------------------------------------------------------
bool CLTE_PM_Data_Cal_Select::My_PM_Cal(CString v_sAccount,CString v_sName)
{
	CString			v_sTemp,v_sSql;										//临时变量
	bool			v_bok=false;										//临时变量

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT * FROM CON_PM_LTE_Case_Cal where zAccount='"+v_sAccount+"' and zName='"+v_sName+"'";

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		if(!v_pRecordset->adoEOF)
			v_bok=true;													//存在

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}
	
	return v_bok;														//返回
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CLTE_PM_Data_Cal_Select::My_ExecuteSQL(CString v_sSql)
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
