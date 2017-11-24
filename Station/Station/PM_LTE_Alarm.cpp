// PM_LTE_Alarm.cpp : 实现文件
// 


#include "stdafx.h"
#include "Station.h"
#include "PM_LTE_Alarm.h"


// CPM_LTE_Alarm 对话框

IMPLEMENT_DYNAMIC(CPM_LTE_Alarm, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CPM_LTE_Alarm::CPM_LTE_Alarm(CWnd* pParent /*=NULL*/)
: CDialog(CPM_LTE_Alarm::IDD, pParent)
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CPM_LTE_Alarm::~CPM_LTE_Alarm()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Alarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_EDIT1, m_cSum);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_CHECK1, m_cPm_Flag);
	DDX_Control(pDX, IDC_COMBO3, m_cPm_Type);
	DDX_Control(pDX, IDC_BUTTON1, m_cUpdate);
	DDX_Control(pDX, IDC_EDIT3, m_cPm_Name);
	DDX_Control(pDX, IDC_EDIT2, m_cPm_Value);
	DDX_Control(pDX, IDC_EDIT4, m_cKey);
	DDX_Control(pDX, IDC_COMBO5, m_cKey_Select);
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPM_LTE_Alarm, CDialog)
	ON_BN_CLICKED(IDC_EXCEL, &CPM_LTE_Alarm::OnBnClickedExcel)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CPM_LTE_Alarm::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CPM_LTE_Alarm::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_QUARY2, &CPM_LTE_Alarm::OnBnClickedQuary2)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CPM_LTE_Alarm::OnLvnColumnclickList1)
END_MESSAGE_MAP()


// CPM_LTE_Alarm 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CPM_LTE_Alarm::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化：列表窗
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList.InsertColumn(1,"类型",LVCFMT_LEFT,100);						//
	m_cList.InsertColumn(2,"性能",LVCFMT_LEFT,200);						//
	m_cList.InsertColumn(3,"中文解释",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(4,"预警",LVCFMT_CENTER,80);					//
	m_cList.InsertColumn(5,"预警门限",LVCFMT_RIGHT,80);					//
	m_cList.InsertColumn(6,"预警类型",LVCFMT_CENTER,80);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList.SetNumber(5);												//设置：数字模式

	//变量初始化
	v_iItem=-1;															//条目选择无效[-1]
	m_cPm_Type.AddString("高");
	m_cPm_Type.AddString("低");
	m_cPm_Type.SetCurSel(0);											//默认
	m_cUpdate.EnableWindow(false);										//无效

	m_cKey_Select.AddString("LTE性能");
	m_cKey_Select.AddString("中文解释");
	m_cKey_Select.SetCurSel(0);											//默认：选择第一个

	//获取性能数据
	My_LoadData_Per();													//获取性能数据

	return TRUE;  // return TRUE unless you set the focus to a control
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CPM_LTE_Alarm::PreTranslateMessage(MSG* pMsg)
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
void CPM_LTE_Alarm::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导出Excel
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Alarm::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
	m_cList.SetFocus();													//聚焦
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Alarm::OnBnClickedQuary2()
{
	// TODO: 在此添加控件通知处理程序代码
	My_LoadData_Per();													//获取性能数据
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Alarm::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击告警类型列表
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Alarm::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp;												//临时变量

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if(v_iItem>=0)													//有效条目序号？？？
			m_cList.SetItemBkColor(v_iItem,RGB(255,255,255));			//恢复背景色
		v_iItem=pNMListView->iItem;										//获得：选中条目序号
		m_cList.SetItemBkColor(v_iItem,RGB(255,0,0));					//设置背景色：红色
		
		//设置：修改内容
		m_cPm_Name.SetWindowText(m_cList.GetItemText(v_iItem,3));		//性能名称
		if (m_cList.GetItemText(v_iItem,4)=="预警")
			m_cPm_Flag.SetCheck(1);										//预警
		else
			m_cPm_Flag.SetCheck(0);										//不预警
		v_sTemp=m_cList.GetItemText(v_iItem,5);							//预警门限
		v_sTemp.Trim();
		m_cPm_Value.SetWindowText(v_sTemp);								//预警门限
		if (m_cList.GetItemText(v_iItem,6)=="高")
			m_cPm_Type.SetCurSel(0);									//高
		else
			m_cPm_Type.SetCurSel(1);									//低
		m_cUpdate.EnableWindow(true);									//有效
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：预警配置修改
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Alarm::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp,v_sSql;											//临时变量
	CString		v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;				//临时变量
	CString		v_sType,v_sPM;											//临时变量

	//获取
	v_sAlarm_Flag=m_cPm_Flag.GetCheck()?"预警":"不预警";				//预警有效
	m_cPm_Value.GetWindowText(v_sAlarm_Value);							//预警门限
	v_sAlarm_Type=m_cPm_Type.GetCurSel()==0?"高":"低";					//预警类型
	
	v_sType=m_cList.GetItemText(v_iItem,1);								//性能类型
	v_sPM=m_cList.GetItemText(v_iItem,2);								//性能名称

	//显示
	m_cList.SetItemText(v_iItem,4,v_sAlarm_Flag);						//
	m_cList.SetItemText(v_iItem,5,v_sAlarm_Value);						//
	m_cList.SetItemText(v_iItem,6,v_sAlarm_Type);						//

	v_sAlarm_Flag=(v_sAlarm_Flag=="预警")?"1":"0";						//预警有效
	v_sAlarm_Type=(v_sAlarm_Type=="高")?"1":"0";						//预警类型
	//保存
	v_sSql="Update CON_Pm_LTE set zAlarm_Flag='"+v_sAlarm_Flag+"',zAlarm_Value='"
			+v_sAlarm_Value+"',zAlarm_Type='"+v_sAlarm_Type
			+"' where zType='"+v_sType+"' and zPM='"+v_sPM+"'";			//SQL
	My_ExecuteSQL(v_sSql);												//SQL执行

	m_cUpdate.EnableWindow(false);										//无效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能参数信息
//------------------------------------------------------------------------------------------------------			
void CPM_LTE_Alarm::My_LoadData_Per()
{
	int				v_iItem,v_iID=0,v_iTemp;							//临时变量
	CString			v_sTemp,v_sSql,v_sLog_Key,v_sAlarm_Flag;			//临时变量
	CString			v_sType,v_sAlarm_Value,v_sPm,v_sPm_Name,v_sAlarm_Type;	//临时变量
	double			v_dTemp;											//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList.DeleteAllItems();										//清除Items
		v_iItem=-1;														//条目选择无效

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE ";

		//查询条件
		m_cKey.GetWindowText(v_sLog_Key);								//获得：查询KEY
		v_sLog_Key.Replace("[","[[]");									//限制处理
		v_sLog_Key.Replace("'","");										//限制处理
		v_sLog_Key.Trim();												//去掉空格

		if (!v_sLog_Key.IsEmpty())										//查询KEY处理
		{
			switch (m_cKey_Select.GetCurSel())							//选择？？？
			{
			case 0:														//LTE性能
				v_sSql+=" where zPm like '%"+v_sLog_Key+"%'";
				break;
			case 1:														//中文解释
				v_sSql+=" where  zPm_Name like '%"+v_sLog_Key+"%'";
				break;
			default:
				break;
			}
		}
		v_sSql+=" order by ztype,ztype_1,zPm";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sPm = v_cAdoConn.GetField_String("zPm");					//zPm
			v_sPm_Name = v_cAdoConn.GetField_String("zPm_Name");		//zPm_Name
			v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Flag");				//zAlarm_Flag
			v_sAlarm_Flag=(v_iTemp==1?"预警":"不预警");						
			v_dTemp=v_cAdoConn.GetField_Double("zAlarm_Value");			//zAlarm_Value
			v_sAlarm_Value.Format("%6.2f",v_dTemp);
			v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Type");				//zAlarm_Type
			v_sAlarm_Type=(v_iTemp==1?"高":"低");						

			//显示
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,v_sType);						//大类
			m_cList.SetItemText(v_iItem,2,v_sPm);						//性能
			m_cList.SetItemText(v_iItem,3,v_sPm_Name);					//中文解释
			m_cList.SetItemText(v_iItem,4,v_sAlarm_Flag);				//预警标志
			m_cList.SetItemText(v_iItem,5,v_sAlarm_Value);				//预警值
			m_cList.SetItemText(v_iItem,6,v_sAlarm_Type);				//预警类型

			v_pRecordset->MoveNext();									//
			v_iID++;													//计数：告警数量

			if (v_iID%100==0)											//显示：100的整数
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum.SetWindowText(v_sTemp);							//显示：站址总数
			}
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cSum.SetWindowText(v_sTemp);									//显示：站址总数

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
bool CPM_LTE_Alarm::My_ExecuteSQL(CString v_sSql)
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
