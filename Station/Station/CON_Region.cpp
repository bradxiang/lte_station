// CON_Region.cpp : 实现文件
// 


#include "stdafx.h"
#include "Station.h"
#include "CON_Region.h"


// CCON_Region 对话框

IMPLEMENT_DYNAMIC(CCON_Region, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CCON_Region::CCON_Region(CWnd* pParent /*=NULL*/)
: CDialog(CCON_Region::IDD, pParent)
{
	v_bAdd=false;														//增加标志
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CCON_Region::~CCON_Region()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CCON_Region::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_EDIT1, m_cSum);
	DDX_Control(pDX, IDC_EDIT2, m_cName);
	DDX_Control(pDX, IDC_ADD, m_cAdd);
	DDX_Control(pDX, IDC_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_Region, CDialog)
	ON_BN_CLICKED(IDC_ADD, &CCON_Region::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DELETE, &CCON_Region::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_EXCEL, &CCON_Region::OnBnClickedExcel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ADD2, &CCON_Region::OnBnClickedAdd2)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CCON_Region::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCON_Region::OnBnClickedButton2)
END_MESSAGE_MAP()


// CCON_Region 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CCON_Region::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化：列表窗
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"区域",LVCFMT_LEFT,200);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//变量初始化
	v_iItem=-1;															//条目选择无效[-1]

	//信息显示
	My_LoadData();														//呈现：数据表信息

	return TRUE;  // return TRUE unless you set the focus to a control
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CCON_Region::PreTranslateMessage(MSG* pMsg)
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
void CCON_Region::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	int	v_iX,v_iY;
	v_iX=318;															//对话窗宽度
	v_iY=(v_bAdd?364:(364-80));											//对话窗长度
	MoveWindow(0,0,v_iX,v_iY);											//调整对话窗尺寸
	CenterWindow(); 
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：增加
//------------------------------------------------------------------------------------------------------
void CCON_Region::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	v_bAdd=true;														//增加标志
	PostMessage(WM_SIZE);
	m_cAdd.EnableWindow(false);											//"增加"：无效
	m_cDelete.EnableWindow(false);										//"删除"：无效
	m_cExcel.EnableWindow(false);										//"导出"：无效

	//初始化：赋值
	m_cName.SetWindowText("");											//
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：删除
//------------------------------------------------------------------------------------------------------
void CCON_Region::OnBnClickedDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql;													//临时变量

	if (v_iItem<0)
		MessageBox("请先选择一个有效的区域  ！！！");					//窗体提示
	else
	{
		if(MessageBox("是否删除选中的区域? ","区域_删除确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
		{
			CString v_sAlarm_Name = m_cList.GetItemText(v_iItem,1);
			v_sSql="delete from CON_Region where zRegion='"+v_sAlarm_Name+"'";		//SQL
			My_ExecuteSQL(v_sSql);										//删除
			//列表显示
			My_LoadData();												//显示
		}
	}
	m_cList.SetFocus();													//聚焦
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导出Excel
//------------------------------------------------------------------------------------------------------
void CCON_Region::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
	m_cList.SetFocus();													//聚焦
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击告警类型列表
//------------------------------------------------------------------------------------------------------
void CCON_Region::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if(v_iItem>=0)													//有效条目序号？？？
			m_cList.SetItemBkColor(v_iItem,RGB(255,255,255));			//恢复背景色
		v_iItem=pNMListView->iItem;										//获得：选中条目序号
		m_cList.SetItemBkColor(v_iItem,RGB(255,0,0));					//设置背景色：红色
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：增加确认
//------------------------------------------------------------------------------------------------------
void CCON_Region::OnBnClickedAdd2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql,v_sTemp;											//临时变量
	CString		v_sName;												//临时变量
	COLORREF	v_cBackColor=RGB(255,255,255);							//背景颜色：白色

	UpdateData(true);													//获得：屏幕信息

	m_cName.GetWindowText(v_sName);										//获得：名称
	if (v_sName.GetLength()==0)
		MessageBox("请输入区域  ！！！");								//窗体提示
	else
	{
		if (!My_New_Name(v_sName))										//名称重复检查
			MessageBox("增加的区域重复 ！！！");						//窗体提示
		else
		{
			v_sTemp.Format("%d",v_cBackColor);							//字符串
			v_sSql="insert into CON_Region(zRegion) values ('"+v_sName+"')";
			My_ExecuteSQL(v_sSql);										//增加KPI
			My_LoadData();												//重新装载KPI信息

			OnBnClickedButton2();										//增加/修改结束
		}
	}
	m_cList.SetFocus();													//聚焦
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：增加取消
//------------------------------------------------------------------------------------------------------			
void CCON_Region::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cAdd.EnableWindow(true);											//"增加"：有效
	m_cDelete.EnableWindow(true);										//"删除"：有效
	m_cExcel.EnableWindow(true);										//"导出"：有效

	v_bAdd=false;														//增加标志
	PostMessage(WM_SIZE);												//窗体调整
	m_cList.SetFocus();													//聚焦
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Pool名称重复检查
//	返    回：true：不重复，可以使用；false：名称重复，不可以使用；
//------------------------------------------------------------------------------------------------------			
bool CCON_Region::My_New_Name(CString v_sName)
{
	// TODO: 在此添加控件通知处理程序代码
	bool	v_bNum;														//临时变量
	CString	v_sSql,v_sTemp;												//临时变量

	v_bNum=0;															//初始化
	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from CON_Region where zRegion='"+v_sName+"'";	//查询数据库：MSC KPI 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		if(v_pRecordset->adoEOF)
			v_bNum=1;

		v_cAdoConn.ExitConnect();										//断开连接

		return v_bNum;
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
		return v_bNum;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载告警类型信息
//------------------------------------------------------------------------------------------------------			
void CCON_Region::My_LoadData()
{
	CString		v_sSql;													//临时变量
	int			v_iItem,v_iID=0;										//数量
	CString		v_sName;												//名称
	CString		v_sTemp;												//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList.DeleteAllItems();										//列表复位
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from CON_Region order by zRegion";				//查询数据库：区域

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sName=v_cAdoConn.GetField_String("zRegion");				//zRegion

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sName);			//名称

			v_pRecordset->MoveNext();									//指针向后移动
			v_iID++;													//条目数量加一
		}
		v_cAdoConn.ExitConnect();										//断开连接

		v_sTemp.Format("%d",v_iID);										//字符串转换
		m_cSum.SetWindowTextA(v_sTemp);									//显示：条目数量
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
//	参    数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CCON_Region::My_ExecuteSQL(CString v_sSql)
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
