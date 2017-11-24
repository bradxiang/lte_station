// Pm_GSM_BSC_Add.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "Pm_GSM_BSC_Add.h"


// CPm_GSM_BSC_Add 对话框

IMPLEMENT_DYNAMIC(CPm_GSM_BSC_Add, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CPm_GSM_BSC_Add::CPm_GSM_BSC_Add(CWnd* pParent /*=NULL*/)
	: CDialog(CPm_GSM_BSC_Add::IDD, pParent)
{
	v_iList_State=0;													//标志：查询
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CPm_GSM_BSC_Add::~CPm_GSM_BSC_Add()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC_Add::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_EDIT1, m_cSum);
	DDX_Control(pDX, IDC_POOL, m_cRole);
	DDX_Control(pDX, IDC_ADD, m_cAdd);
	DDX_Control(pDX, IDC_MODIFY, m_cModify);
	DDX_Control(pDX, IDC_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_EDIT2, m_cName);
	DDX_Control(pDX, IDC_EDIT7, m_cText);
	DDX_Control(pDX, IDC_EDIT3, m_cObjType);
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPm_GSM_BSC_Add, CDialog)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CPm_GSM_BSC_Add::OnLvnColumnclickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CPm_GSM_BSC_Add::OnNMClickList1)
	ON_BN_CLICKED(IDC_ADD, &CPm_GSM_BSC_Add::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_MODIFY, &CPm_GSM_BSC_Add::OnBnClickedModify)
	ON_BN_CLICKED(IDC_DELETE, &CPm_GSM_BSC_Add::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_EXCEL, &CPm_GSM_BSC_Add::OnBnClickedExcel)
	ON_BN_CLICKED(IDC_ADD2, &CPm_GSM_BSC_Add::OnBnClickedAdd2)
	ON_BN_CLICKED(IDC_BUTTON1, &CPm_GSM_BSC_Add::OnBnClickedButton1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, &CPm_GSM_BSC_Add::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPm_GSM_BSC_Add 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CPm_GSM_BSC_Add::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化：列表窗
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"性能名称",LVCFMT_LEFT,200);  
	m_cList.InsertColumn(2,"类型",LVCFMT_LEFT,60);  
	m_cList.InsertColumn(3,"计算公式",LVCFMT_LEFT,300);  
	m_cList.InsertColumn(4,"ObjType",LVCFMT_LEFT,160);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//变量初始化
	v_iList_Item=-1;													//条目选择无效[-1]

	//类型
	m_cRole.AddString("DATA");
	m_cRole.AddString("PM");
	m_cRole.SetCurSel(0);												//默认


	//信息显示
	My_LoadData();														//呈现：数据表信息

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CPm_GSM_BSC_Add::PreTranslateMessage(MSG* pMsg)
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
void CPm_GSM_BSC_Add::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	int v_iX=830;														//对话窗宽度
	int v_iY=694;														//对话窗长度
	int v_iY_Div=115;													//对话窗隐藏部分长度

	MoveWindow(0,0,v_iX,(v_iList_State!=0?v_iY:(v_iY-v_iY_Div)));		//调整对话窗尺寸
	CenterWindow(); 
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：更新∑表
//------------------------------------------------------------------------------------------------------			
void CPm_GSM_BSC_Add::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString	v_sSql;														//临时变量

	//删除：BSC_PM表
	v_sSql="DROP TABLE BSC_PM";											//删除表
	My_ExecuteSQL(v_sSql);												//执行

	//生成：BSC_PM表
	v_sSql="CREATE TABLE BSC_PM([zdate] [varchar](8) NOT NULL,[zbsc] [varchar](10) NOT NULL";		//时间、BSC
	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//遍历列表窗
	{
		v_sSql+=",["+m_cList.GetItemText(vi,1)+"] [float] NULL";		//增加字段
	}
	v_sSql+=")";														//SQL
	My_ExecuteSQL(v_sSql);												//执行
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表排序(右击列表任意列)
//------------------------------------------------------------------------------------------------------			
void CPm_GSM_BSC_Add::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击列表响应
//------------------------------------------------------------------------------------------------------			
void CPm_GSM_BSC_Add::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if((v_iList_Item>=0) && (v_iList_Item<m_cList.GetItemCount()))	//有效条目序号？？？
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));		//恢复背景色
		v_iList_Item=pNMListView->iItem;								//获得：选中条目序号
		m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));				//设置背景色：红色
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：增加
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC_Add::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_SIZE);
	m_cAdd.EnableWindow(false);											//"增加"：无效
	m_cModify.EnableWindow(false);										//"修改"：无效
	m_cDelete.EnableWindow(false);										//"删除"：无效
	m_cExcel.EnableWindow(false);										//"导出"：无效
	m_cName.EnableWindow(true);											//"名称"：有效
	m_cRole.SetCurSel(0);												//默认：第一个Role

	//初始化：赋值
	m_cName.SetWindowText("");											//

	v_iList_State=1;													//标志：增加
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：修改
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC_Add::OnBnClickedModify()
{
	// TODO: 在此添加控件通知处理程序代码
	if (v_iList_Item<0)
	{
		MessageBox("请先选择一个有效的性能  ！！！");					//窗体提示
		m_cList.SetFocus();												//聚焦
		return;
	}
	m_cName.SetWindowText(m_cList.GetItemText(v_iList_Item,1));			//zName
	m_cRole.SelectString(0,m_cList.GetItemText(v_iList_Item,2));		//zType
	m_cText.SetWindowText(m_cList.GetItemText(v_iList_Item,3));			//zFlrmla
	m_cObjType.SetWindowText(m_cList.GetItemText(v_iList_Item,4));		//zObjType

	PostMessage(WM_SIZE);
	m_cAdd.EnableWindow(false);											//"增加"：无效
	m_cModify.EnableWindow(false);										//"修改"：无效
	m_cDelete.EnableWindow(false);										//"删除"：无效
	m_cExcel.EnableWindow(false);										//"导出"：无效
	m_cName.EnableWindow(false);										//"MSC名称"：无效

	v_iList_State=2;													//标志：增加
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：删除
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC_Add::OnBnClickedDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql;													//临时变量

	if (v_iList_Item<0)
		MessageBox("请先选择一个有效的性能  ！！！");					//窗体提示
	else
	{
		if(MessageBox("是否删除性能： "+m_cList.GetItemText(v_iList_Item,1),"数据删除确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
		{
			v_sSql="delete from A_Cap_All_Info where zPM_Name='"+m_cList.GetItemText(v_iList_Item,1)+"'";	//SQL
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
void CPm_GSM_BSC_Add::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
	m_cList.SetFocus();													//聚焦
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：修改/增加确认
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC_Add::OnBnClickedAdd2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql;													//临时变量
	CString		v_sName,v_sFormula,v_sType,v_sObjType;					//临时变量

	m_cName.GetWindowText(v_sName);
	v_sName.Trim();
	m_cObjType.GetWindowText(v_sObjType);
	v_sObjType.Trim();
	m_cText.GetWindowText(v_sFormula);
	v_sFormula.Trim();

	if (v_iList_State==1 && !My_New_Name(v_sName))						//名称重复检查
		MessageBox("增加的性能名称重复 ！！！");						//窗体提示
	else
	{
		m_cRole.GetWindowText(v_sType);									//类型
		v_sType.Trim();

		if (v_iList_State==1)											//增加
			v_sSql="insert into A_Cap_All_Info(zPM_Name,zType,zCounter,zObjType) values \
				   ('"+v_sName+"','"+v_sType+"','"+v_sFormula+"','"+v_sObjType+"')";
		else															//修改
			v_sSql="update A_Cap_All_Info set zType='"+v_sType+"',zCounter='"+v_sFormula
					+"',zObjType='"+v_sObjType+"' where zPM_Name='"+v_sName+"'";
		My_ExecuteSQL(v_sSql);											//增加

		My_LoadData();													//重新装载KPI信息
		OnBnClickedButton1();											//增加/修改结束
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：修改/增加取消/结束
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC_Add::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cAdd.EnableWindow(true);											//"增加"：有效
	m_cModify.EnableWindow(true);										//"修改"：有效
	m_cDelete.EnableWindow(true);										//"删除"：有效
	m_cExcel.EnableWindow(true);										//"导出"：有效
	m_cName.EnableWindow(true);											//"MSC名称"：有效

	v_iList_State=0;													//标志：查询

	PostMessage(WM_SIZE);												//窗体调整
	m_cList.SetFocus();													//聚焦
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：重复检查
//	返    回：true：不重复，可以使用；false：名称重复，不可以使用；
//------------------------------------------------------------------------------------------------------			
bool CPm_GSM_BSC_Add::My_New_Name(CString v_sName)
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
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from A_Cap_All_Info where zPM_Name='"+v_sName+"'";	//查询数据库：USER
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
//	函数功能：装载USER信息
//------------------------------------------------------------------------------------------------------			
void CPm_GSM_BSC_Add::My_LoadData()
{
	CString			v_sSql;												//临时变量
	int				v_iItem,v_iID=0;									//数量
	CString			v_sName,v_sType,v_sFormula,v_sObjType;				//名称
	CString			v_sTemp;											//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList.DeleteAllItems();										//列表复位
		v_iList_Item=-1;												//条目选择无效[-1]
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from A_Cap_All_Info order by zPM_Name";		//查询数据库
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sName=v_cAdoConn.GetField_String("zPM_Name");				//zPm_Name
			v_sType=v_cAdoConn.GetField_String("zType");				//zType
			v_sFormula=v_cAdoConn.GetField_String("zCounter");			//zCounter
			v_sObjType=v_cAdoConn.GetField_String("zObjType");			//zObjType

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sName);			//v_sName
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sType);			//v_sType
			m_cList.SetItemText(v_iItem,3,(_bstr_t)v_sFormula);			//计算公式
			m_cList.SetItemText(v_iItem,4,(_bstr_t)v_sObjType);			//ObjType

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
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CPm_GSM_BSC_Add::My_ExecuteSQL(CString v_sSql)
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
