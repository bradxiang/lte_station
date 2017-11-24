// User_Role.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "User_Role.h"


// CUser_Role 对话框

IMPLEMENT_DYNAMIC(CUser_Role, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CUser_Role::CUser_Role(CWnd* pParent /*=NULL*/)
	: CDialog(CUser_Role::IDD, pParent)
{
	v_iList_State=0;													//标志：查询
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CUser_Role::~CUser_Role()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CUser_Role::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_LIST2, m_cList_Power);
	DDX_Control(pDX, IDC_CHECK1, m_cRole_Flag);
	DDX_Control(pDX, IDC_STATION_SUM, m_cSum);
	DDX_Control(pDX, IDC_EDIT65, m_cAdd_Role_Name);
	DDX_Control(pDX, IDC_EDIT67, m_cAdd_Role_Marker);
	DDX_Control(pDX, IDC_EDIT69, m_cMod_Role_Marker);
	DDX_Control(pDX, IDC_EDIT72, m_cMod_Role_Name);
	DDX_Control(pDX, IDC_EDIT71, m_cDel_Role_Name);
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CUser_Role, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CUser_Role::OnNMClickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CUser_Role::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_MSC_KPI_Excel, &CUser_Role::OnBnClickedMscKpiExcel)
	ON_BN_CLICKED(IDC_ADD_MSC_KPI2, &CUser_Role::OnBnClickedAddMscKpi2)
	ON_BN_CLICKED(IDC_BUTTON23, &CUser_Role::OnBnClickedButton23)
	ON_BN_CLICKED(IDC_BUTTON25, &CUser_Role::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_CHECK1, &CUser_Role::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_ALL, &CUser_Role::OnBnClickedAll)
	ON_BN_CLICKED(IDC_NULL, &CUser_Role::OnBnClickedNull)
	ON_BN_CLICKED(IDC_SAVE, &CUser_Role::OnBnClickedSave)
END_MESSAGE_MAP()


// CUser_Role 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CUser_Role::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化：列表窗
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"角色名称",LVCFMT_CENTER,100);  
	m_cList.InsertColumn(2,"备注",LVCFMT_LEFT,145);  
	m_cList.InsertColumn(3,"Role_ID",LVCFMT_CENTER,0);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	//初始化：列表窗
	::SendMessage(m_cList_Power.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Power.InsertColumn(0,"                  权  限  名  称",LVCFMT_LEFT,158);  
	m_cList_Power.InsertColumn(1,"Power_ID",LVCFMT_CENTER,0);  
	m_cList_Power.InsertColumn(2,"Power",LVCFMT_CENTER,0);  
	m_cList_Power.SetExtendedStyle(m_cList_Power.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES );

	//变量初始化
	v_iList_State=-1;													//条目选择无效[-1]

	//信息显示
	My_GetPower();														//获得：权限
	My_LoadData();														//呈现：数据表信息

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CUser_Role::PreTranslateMessage(MSG* pMsg)
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
void CUser_Role::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	int v_iX=805;														//对话窗宽度
	int v_iX_Div=282;													//对话窗宽度
	int v_iY=460;														//对话窗长度
	int v_iY_Div=115;													//对话窗隐藏部分长度

	MoveWindow(0,0,(v_iList_State!=0?v_iX:v_iX-v_iX_Div),v_iY);			//调整对话窗尺寸
	CenterWindow(); 
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击列表响应
//------------------------------------------------------------------------------------------------------			
void CUser_Role::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if(v_iList_Item>=0) 											//有效条目序号？？？
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));		//恢复背景色
		v_iList_Item=pNMListView->iItem;								//获得：选中条目序号
		v_sList_Item_ID=m_cList.GetItemText(v_iList_Item,3);			//获得：选中条目的ID
		m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));				//设置背景色：红色
		m_cDel_Role_Name.SetWindowText(m_cList.GetItemText(v_iList_Item,1));	//
		m_cMod_Role_Name.SetWindowText(m_cList.GetItemText(v_iList_Item,1));	//
		m_cMod_Role_Marker.SetWindowText(m_cList.GetItemText(v_iList_Item,2));	//

		My_GetPower_OK(v_sList_Item_ID);								//获取：授权的权限
		My_LoadData_Power();											//呈现：权限信息
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表排序(右击列表任意列)
//------------------------------------------------------------------------------------------------------			
void CUser_Role::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：增加、修改、删除有效； [扩展显示界面]
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cRole_Flag.GetCheck())
		v_iList_State=1;												//标志：扩展显示有效
	else
		v_iList_State=0;												//标志：扩展显示无效

	PostMessage(WM_SIZE);												//屏幕重绘
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导出Excel
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedMscKpiExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
	m_cList.SetFocus();													//聚焦
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：全选
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedAll()
{
	// TODO: 在此添加控件通知处理程序代码
	mUSER_Power::iterator v_pIterator_Power = v_mUSER_Power.begin();	//头位置
	while(v_pIterator_Power != v_mUSER_Power.end())						//结束？
	{
		v_pIterator_Power->second.v_bOk=true;							//权限有效
		v_pIterator_Power++;											//迭代器++
	}
	for (int vi=0;vi<m_cList_Power.GetItemCount();vi++)					//列表
		m_cList_Power.SetCheck(vi);										//选择：有效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：全不选
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedNull()
{
	// TODO: 在此添加控件通知处理程序代码
	mUSER_Power::iterator v_pIterator_Power = v_mUSER_Power.begin();	//头位置
	while(v_pIterator_Power != v_mUSER_Power.end())						//结束？
	{
		v_pIterator_Power->second.v_bOk=false;							//权限有效
		v_pIterator_Power++;											//迭代器++
	}
	for (int vi=0;vi<m_cList_Power.GetItemCount();vi++)					//列表
		m_cList_Power.SetCheck(vi,0);									//选择：无效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：权限保存
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp,v_sSql;											//临时变量

	//【1】清除所有权限
	v_sSql="delete from BAS_User_RP where zRole_ID="+v_sList_Item_ID;	//Sql
	My_ExecuteSQL(v_sSql);												//删除
	//【2】获取需要保存的权限
	v_sSql="";															//初始化
	for (int vi=0;vi<m_cList_Power.GetItemCount();vi++)					//列表
	{
		if (m_cList_Power.GetCheck(vi))									//选中
		{
			v_sSql+="insert into BAS_User_RP(zRole_ID,zPower_ID) values ("
					+v_sList_Item_ID+","+m_cList_Power.GetItemText(vi,1)+")\r\n";	//Sql
		}
	}
	//【3】保存
	if (!v_sSql.IsEmpty())												//有数据需要保存？？？
		My_ExecuteSQL(v_sSql);											//保存
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：增加
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedAddMscKpi2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql,v_sTemp;											//临时变量
	CString		v_sName,v_sMarker;										//临时变量

	UpdateData(true);													//获得：屏幕信息

	m_cAdd_Role_Name.GetWindowText(v_sName);							//获得：名称
	if (v_sName.GetLength()==0)
		MessageBox("请输入Role名称  ！！！");							//窗体提示
	else
	{
		if (!My_New_Name(v_sName))										//名称重复检查
			MessageBox("增加的Role名称重复 ！！！");					//窗体提示
		else
		{
			m_cAdd_Role_Name.GetWindowText(v_sName);					//Role Name
			v_sName.Trim();
			m_cAdd_Role_Marker.GetWindowText(v_sMarker);				//Role Marker
			v_sMarker.Trim();

			v_sSql="insert into BAS_User_Role(zRole_Name,zMarker) values ('"+v_sName+"','"+v_sMarker+"')";
			My_ExecuteSQL(v_sSql);										//增加KPI

			My_LoadData();												//重新装载KPI信息
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：删除
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedButton23()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql;													//临时变量

	if (v_iList_Item<0)
		MessageBox("请先选择一个有效的角色  ！！！");					//窗体提示
	else
	{
		if(MessageBox("是否删除选中的角色： "+m_cList.GetItemText(v_iList_Item,1)+" ？","角色删除确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
		{
			v_sSql="delete from BAS_User_Role where zRole_ID="+v_sList_Item_ID;	//SQL
			My_ExecuteSQL(v_sSql);										//删除：Role表
			v_sSql="delete from BAS_User_RP where zRole_ID="+v_sList_Item_ID;	//SQL
			My_ExecuteSQL(v_sSql);										//删除：Role_Power表
			//列表显示
			My_LoadData();												//显示
		}
	}
	m_cList.SetFocus();													//聚焦
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：修改
//------------------------------------------------------------------------------------------------------
void CUser_Role::OnBnClickedButton25()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql,v_sTemp;											//临时变量
	CString		v_sMarker;												//临时变量

	UpdateData(true);													//获得：屏幕信息

	if (v_iList_Item<0)
		MessageBox("请先选择一个有效的角色  ！！！");					//窗体提示
	else
	{
		m_cMod_Role_Marker.GetWindowText(v_sMarker);					//Role Marker
		v_sMarker.Trim();

		v_sSql="update BAS_User_Role set zMarker='"+v_sMarker+"' where zRole_ID="+v_sList_Item_ID;
		My_ExecuteSQL(v_sSql);											//修改

		My_LoadData();													//重新装载KPI信息
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：USER账号重复检查
//	返    回：true：不重复，可以使用；false：名称重复，不可以使用；
//------------------------------------------------------------------------------------------------------			
bool CUser_Role::My_New_Name(CString v_sName)
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

		v_sSql="select * from BAS_User_Role where zRole_Name='"+v_sName+"'";	//查询数据库：USER Role_Name
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
//	函数功能：装载Role信息
//------------------------------------------------------------------------------------------------------			
void CUser_Role::My_LoadData()
{
	CString			v_sSql;												//临时变量
	int				v_iItem,v_iID=0;									//数量
	int				v_iRole_ID;											//ID
	CString			v_sRole_Name,v_sMarker;								//名称
	CString			v_sTemp;											//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList.DeleteAllItems();										//列表复位
		v_iList_Item=-1;												//条目选择无效[-1]
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from BAS_User_Role order by zRole_Name";		//查询数据库：USER_Role
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_iRole_ID=v_cAdoConn.GetField_Int("zRole_ID");				//zRole_ID
			v_sRole_Name=v_cAdoConn.GetField_String("zRole_Name");		//zRole_Name
			v_sMarker=v_cAdoConn.GetField_String("zMarker");			//zMarker

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sRole_Name);		//角色名称
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sMarker);			//备注
			v_sTemp.Format("%d",v_iRole_ID);
			m_cList.SetItemText(v_iItem,3,(_bstr_t)v_sTemp);			//zRole_ID

			v_pRecordset->MoveNext();									//指针向后移动
			v_iID++;													//条目数量加一
		}
		v_cAdoConn.ExitConnect();										//断开连接

		v_sTemp.Format("%d",v_iID);										//字符串转换
		m_cSum.SetWindowTextA(v_sTemp);									//显示：条目数量

		m_cAdd_Role_Name.SetWindowText("");								//
		m_cAdd_Role_Marker.SetWindowText("");							//
		m_cDel_Role_Name.SetWindowText("");								//
		m_cMod_Role_Name.SetWindowText("");								//
		m_cMod_Role_Marker.SetWindowText("");							//
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载Power信息
//------------------------------------------------------------------------------------------------------			
void CUser_Role::My_LoadData_Power()
{
	CString			v_sSql;												//临时变量
	int				v_iItem;											//数量
	CString			v_sRole_Name,v_sMarker;								//名称
	CString			v_sTemp;											//临时变量

	m_cList_Power.DeleteAllItems();										//列表复位

	mUSER_Power::iterator v_pIterator_Power = v_mUSER_Power.begin();	//头位置
	while(v_pIterator_Power != v_mUSER_Power.end())						//结束？
	{
		v_sTemp="    "+v_pIterator_Power->second.v_sPower;
		v_iItem=m_cList_Power.InsertItem(0xffff,v_sTemp);				//加入列表窗
		v_sTemp.Format("%d",v_pIterator_Power->first);
		m_cList_Power.SetItemText(v_iItem,1,v_sTemp);					//权限ID
		m_cList_Power.SetItemText(v_iItem,2,v_pIterator_Power->second.v_bOk?"1":"0");	//权限有效
		if (v_pIterator_Power->second.v_bOk)							//权限有效
			m_cList_Power.SetCheck(v_iItem);							//加标记

		v_pIterator_Power++;											//迭代器++
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载权限信息
//	参	  数：v_sRolie_ID：Role_ID
//------------------------------------------------------------------------------------------------------			
void CUser_Role::My_GetPower_OK(CString v_sRolie_ID)
{
	CString			v_sSql;												//临时变量
	int				v_iID=0;											//数量
	int				v_iRole_ID,v_iPower_ID;								//ID
	CString			v_sTemp;											//临时变量

	mUSER_Power::iterator v_pIterator_Power = v_mUSER_Power.begin();	//头位置
	while(v_pIterator_Power != v_mUSER_Power.end())						//结束？
	{
		v_pIterator_Power->second.v_bOk=false;							//权限有效
		v_pIterator_Power++;											//迭代器++
	}

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from BAS_User_RP where zRole_ID="+v_sRolie_ID+" order by zPower_ID";	//查询数据库：USER_RP
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_iRole_ID=v_cAdoConn.GetField_Int("zRole_ID");				//zRole_ID
			v_iPower_ID=v_cAdoConn.GetField_Int("zPower_ID");			//zPower_ID

			mUSER_Power::iterator v_pIterator_Power = v_mUSER_Power.find(v_iPower_ID);	//查找v_iPower_ID
			if(v_pIterator_Power != v_mUSER_Power.end())				//存在？
				v_pIterator_Power->second.v_bOk=true;					//权限有效

			v_pRecordset->MoveNext();									//指针向后移动
			v_iID++;													//条目数量加一
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
//	函数功能：获得权限
//------------------------------------------------------------------------------------------------------			
void CUser_Role::My_GetPower()
{
	CString		v_sSql;													//临时变量
	int			v_iPower_ID;											//ID
	CString		v_sPowerName;											//名称
	CString		v_sTemp;												//临时变量
	USER_Power	v_cPower;												//权限

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from BAS_User_Power order by zPower_ID";		//查询数据库：Power

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_iPower_ID=v_cAdoConn.GetField_Int("zPower_ID");			//zPower_ID
			v_sPowerName=v_cAdoConn.GetField_String("zPower_Name");		//zPower_Name

			v_cPower.v_sPower=v_sPowerName;
			v_cPower.v_bOk=false;
			v_mUSER_Power.insert(pair<int,USER_Power>(v_iPower_ID,v_cPower));		//容器保存[ID/权限]

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
bool CUser_Role::My_ExecuteSQL(CString v_sSql)
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
