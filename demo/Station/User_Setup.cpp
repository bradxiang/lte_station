// User_Setup.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "User_Setup.h"


// CUser_Setup 对话框

IMPLEMENT_DYNAMIC(CUser_Setup, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CUser_Setup::CUser_Setup(CWnd* pParent /*=NULL*/)
	: CDialog(CUser_Setup::IDD, pParent)
{
	v_iList_State=0;													//标志：查询
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CUser_Setup::~CUser_Setup()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CUser_Setup::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_PASSWORD, m_cClear);
	DDX_Control(pDX, IDC_EDIT6, m_cAccount);
	DDX_Control(pDX, IDC_SEND_FLAG, m_cFlag);
	DDX_Control(pDX, IDC_EDIT7, m_cMarker);
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CUser_Setup, CDialog)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CUser_Setup::OnLvnColumnclickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CUser_Setup::OnNMClickList1)
	ON_BN_CLICKED(IDC_ADD, &CUser_Setup::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_MODIFY, &CUser_Setup::OnBnClickedModify)
	ON_BN_CLICKED(IDC_PASSWORD, &CUser_Setup::OnBnClickedPassword)
	ON_BN_CLICKED(IDC_DELETE, &CUser_Setup::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_EXCEL, &CUser_Setup::OnBnClickedExcel)
	ON_BN_CLICKED(IDC_ADD2, &CUser_Setup::OnBnClickedAdd2)
	ON_BN_CLICKED(IDC_BUTTON1, &CUser_Setup::OnBnClickedButton1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CUser_Setup 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CUser_Setup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化：列表窗
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"用户姓名",LVCFMT_CENTER,80);  
	m_cList.InsertColumn(2,"账号",LVCFMT_LEFT,100);  
	m_cList.InsertColumn(3,"角色",LVCFMT_LEFT,100);  
	m_cList.InsertColumn(4,"创建日期",LVCFMT_CENTER,120);  
	m_cList.InsertColumn(5,"有效",LVCFMT_CENTER,50);  
	m_cList.InsertColumn(6,"备注",LVCFMT_LEFT,290);  
	m_cList.InsertColumn(7,"Auto_ID",LVCFMT_CENTER,0);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//变量初始化
	v_iList_Item=-1;													//条目选择无效[-1]

	//信息显示
	My_GetRole();														//获得：USER_Role
	m_cRole.SetCurSel(0);												//默认：第一个Pool
	My_LoadData();														//呈现：数据表信息

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CUser_Setup::PreTranslateMessage(MSG* pMsg)
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
void CUser_Setup::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	int v_iX=830;														//对话窗宽度
	int v_iY=556;														//对话窗长度
	int v_iY_Div=115;													//对话窗隐藏部分长度

	MoveWindow(0,0,v_iX,(v_iList_State!=0?v_iY:(v_iY-v_iY_Div)));		//调整对话窗尺寸
	CenterWindow(); 
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表排序(右击列表任意列)
//------------------------------------------------------------------------------------------------------			
void CUser_Setup::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击列表响应
//------------------------------------------------------------------------------------------------------			
void CUser_Setup::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if((v_iList_Item>=0) && (v_iList_Item<m_cList.GetItemCount()))	//有效条目序号？？？
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));		//恢复背景色
		v_iList_Item=pNMListView->iItem;								//获得：选中条目序号
		v_sList_Item_ID=m_cList.GetItemText(v_iList_Item,7);			//获得：选中条目的ID
		m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));				//设置背景色：红色
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：增加
//------------------------------------------------------------------------------------------------------
void CUser_Setup::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_SIZE);
	m_cAdd.EnableWindow(false);											//"增加"：无效
	m_cModify.EnableWindow(false);										//"修改"：无效
	m_cClear.EnableWindow(false);										//"清除密码"：无效
	m_cDelete.EnableWindow(false);										//"删除"：无效
	m_cExcel.EnableWindow(false);										//"导出"：无效
	m_cName.EnableWindow(true);											//"名称"：有效
	m_cRole.SetCurSel(0);												//默认：第一个Role

	//初始化：赋值
	m_cName.SetWindowText("");											//
	m_cAccount.SetWindowText("");										//
	m_cFlag.SetCheck(false);											//
	m_cMarker.SetWindowText("");										//

	v_iList_State=1;													//标志：增加
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：修改
//------------------------------------------------------------------------------------------------------
void CUser_Setup::OnBnClickedModify()
{
	// TODO: 在此添加控件通知处理程序代码
	if (v_iList_Item<0)
	{
		MessageBox("请先选择一个有效的用户  ！！！");					//窗体提示
		m_cList.SetFocus();												//聚焦
		return;
	}
	m_cName.SetWindowText(m_cList.GetItemText(v_iList_Item,1));			//zName
	m_cAccount.SetWindowText(m_cList.GetItemText(v_iList_Item,2));		//zAccount
	m_cRole.SelectString(0,m_cList.GetItemText(v_iList_Item,3));		//zRole_Name
	CString v_sFlag=m_cList.GetItemText(v_iList_Item,5);				//zFlag
	m_cFlag.SetCheck(v_sFlag=="√"?1:0);								//
	m_cMarker.SetWindowText(m_cList.GetItemText(v_iList_Item,6));		//zMarker

	PostMessage(WM_SIZE);
	m_cAdd.EnableWindow(false);											//"增加"：无效
	m_cModify.EnableWindow(false);										//"修改"：无效
	m_cClear.EnableWindow(false);										//"清除密码"：无效
	m_cDelete.EnableWindow(false);										//"删除"：无效
	m_cExcel.EnableWindow(false);										//"导出"：无效
	m_cName.EnableWindow(false);										//"MSC名称"：无效

	v_iList_State=2;													//标志：增加
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：清除密码
//------------------------------------------------------------------------------------------------------
void CUser_Setup::OnBnClickedPassword()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql;													//临时变量

	if (v_iList_Item<0)
		MessageBox("请先选择一个有效的用户  ！！！");					//窗体提示
	else
	{
		if(MessageBox("是否清除用户密码： "+m_cList.GetItemText(v_iList_Item,1)+"  ？","用户密码清除确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
		{
			v_sSql="update BAS_User set zPassword='' where zAuto_ID="+v_sList_Item_ID;
			My_ExecuteSQL(v_sSql);										//删除
			//列表显示
			My_LoadData();												//显示
		}
	}
	m_cList.SetFocus();													//聚焦
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：删除
//------------------------------------------------------------------------------------------------------
void CUser_Setup::OnBnClickedDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql;													//临时变量

	if (v_iList_Item<0)
		MessageBox("请先选择一个有效的用户  ！！！");					//窗体提示
	else
	{
		if(MessageBox("是否删除用户： "+m_cList.GetItemText(v_iList_Item,1),"用户删除确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
		{
			v_sSql="delete from BAS_User where zAuto_ID="+v_sList_Item_ID;	//SQL
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
void CUser_Setup::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
	m_cList.SetFocus();													//聚焦
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：修改/增加确认
//------------------------------------------------------------------------------------------------------
void CUser_Setup::OnBnClickedAdd2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql,v_sRole_ID,v_sFlag,v_sDate;						//临时变量
	CString		v_sName,v_sAccount,v_sRole_Name,v_sMarker;				//临时变量
	COleDateTime v_dDate_Role;											//日期

	UpdateData(true);													//获得：屏幕信息

	m_cAccount.GetWindowText(v_sName);									//获得：名称
	if (v_sName.GetLength()==0)
		MessageBox("请输入用户名称  ！！！");							//窗体提示
	else
	{
		if (v_iList_State==1 && !My_New_Name(v_sName))					//名称重复检查
			MessageBox("增加的用户名称重复 ！！！");					//窗体提示
		else
		{
			m_cName.GetWindowText(v_sName);								//zName
			v_sName.Trim();
			m_cAccount.GetWindowText(v_sAccount);						//zAccount
			v_sAccount.Trim();
			m_cRole.GetWindowText(v_sRole_Name);						//zRole_Name
			v_sRole_Name.Trim();
			int vi=0;
			mUser_Role::iterator v_pIterator_Role = v_mUser_Role.find(v_sRole_Name);	//查找v_sRole_Name
			if(v_pIterator_Role != v_mUser_Role.end())					//存在？
				vi=v_pIterator_Role->second;							//Role_ID
			v_sRole_ID.Format("%d",vi);
			vi=m_cFlag.GetCheck();										//zFlag
			v_sFlag.Format("%d",vi);
			m_cMarker.GetWindowText(v_sMarker);							//zMarker
			v_sMarker.Trim();
			v_dDate_Role=COleDateTime::GetCurrentTime();				//获得：当前日期
			v_sDate=v_dDate_Role.Format("%Y-%m-%d %H:%M:%S");			//字符串

			if (v_iList_State==1)										//增加
				v_sSql="insert into BAS_User(zName,zAccount,zPassword,zRole_ID,zCreat_Time,zFlag,zMarker) values \
					   ('"+v_sName+"','"+v_sAccount+"','"+v_sAccount+"',"+v_sRole_ID+",'"+v_sDate+"',"+v_sFlag+",'"+v_sMarker+"')";
			else														//修改
				v_sSql="update BAS_User set zAccount='"+v_sAccount+"',zRole_ID="+v_sRole_ID+",zFlag="+
				v_sFlag+",zMarker='"+v_sMarker+"' where zAuto_ID="+v_sList_Item_ID;
			My_ExecuteSQL(v_sSql);										//增加KPI

			My_LoadData();												//重新装载KPI信息
			OnBnClickedButton1();										//增加/修改结束
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：修改/增加取消/结束
//------------------------------------------------------------------------------------------------------
void CUser_Setup::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cAdd.EnableWindow(true);											//"增加"：有效
	m_cModify.EnableWindow(true);										//"修改"：有效
	m_cClear.EnableWindow(true);										//"清除密码"：有效
	m_cDelete.EnableWindow(true);										//"删除"：有效
	m_cExcel.EnableWindow(true);										//"导出"：有效
	m_cName.EnableWindow(true);											//"MSC名称"：有效

	v_iList_State=0;													//标志：查询

	PostMessage(WM_SIZE);												//窗体调整
	m_cList.SetFocus();													//聚焦
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：USER账号重复检查
//	返    回：true：不重复，可以使用；false：名称重复，不可以使用；
//------------------------------------------------------------------------------------------------------			
bool CUser_Setup::My_New_Name(CString v_sName)
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

		v_sSql="select * from BAS_User where zAccount='"+v_sName+"'";	//查询数据库：USER
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
void CUser_Setup::My_LoadData()
{
	CString			v_sSql;												//临时变量
	int				v_iItem,v_iID=0;									//数量
	int				v_iAuto_ID,v_iFlag;									//ID
	CString			v_sName,v_sFtp_IP,v_sAccount,v_sMarker,v_sRole_Name;//名称
	COleDateTime	v_dDate;											//日期
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

		v_sSql="select BAS_User.*,BAS_User_Role.zRole_Name from BAS_User LEFT OUTER JOIN BAS_User_Role ON BAS_User.zRole_ID = BAS_User_Role.zRole_ID order by zName";	//查询数据库：USER
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_iAuto_ID=v_cAdoConn.GetField_Int("zAuto_ID");				//zAuto_ID
			v_sName=v_cAdoConn.GetField_String("zName");				//zName
			v_sAccount=v_cAdoConn.GetField_String("zAccount");			//zAccount
			v_sRole_Name=v_cAdoConn.GetField_String("zRole_Name");		//zRole_Name
			v_iFlag=v_cAdoConn.GetField_Int("zFlag");					//zFlag
			v_sMarker=v_cAdoConn.GetField_String("zMarker");			//zMarker
			v_dDate=v_cAdoConn.GetField_DataTime("zCreat_Time");		//zCreat_Time

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sName);			//用户名称
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sAccount);			//用户账号
			m_cList.SetItemText(v_iItem,3,(_bstr_t)v_sRole_Name);		//角色名称
			v_sTemp=v_dDate.Format("%Y-%m-%d_%H:%M:%S");
			m_cList.SetItemText(v_iItem,4,(_bstr_t)v_sTemp);			//生成日期
			v_sTemp=(v_iFlag==1?"√":"╳");
			m_cList.SetItemText(v_iItem,5,(_bstr_t)v_sTemp);			//有效
			m_cList.SetItemText(v_iItem,6,(_bstr_t)v_sMarker);			//备注
			v_sTemp.Format("%d",v_iAuto_ID);
			m_cList.SetItemText(v_iItem,7,(_bstr_t)v_sTemp);			//Auto_ID

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
//	函数功能：获得Role名称
//------------------------------------------------------------------------------------------------------			
void CUser_Setup::My_GetRole()
{
	CString		v_sSql;													//临时变量
	int			v_iRole_ID;												//ID
	CString		v_sName;												//名称
	CString		v_sTemp;												//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from BAS_User_Role order by zRole_Name";		//查询数据库：MSC KPI 

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_iRole_ID=v_cAdoConn.GetField_Int("zRole_ID");				//zRole_ID
			v_sName=v_cAdoConn.GetField_String("zRole_Name");			//zRole_Name

			m_cRole.AddString(v_sName);									//Role名称
			v_mUser_Role.insert(pair<CString,int>(v_sName,v_iRole_ID));	//容器保存

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
bool CUser_Setup::My_ExecuteSQL(CString v_sSql)
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
