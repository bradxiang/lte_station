// PM_LTE_Case_PM.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "PM_LTE_Case_PM.h"
#include "PM_LTE_Case_PM_Input.h"


// CPM_LTE_Case_PM

IMPLEMENT_DYNCREATE(CPM_LTE_Case_PM, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CPM_LTE_Case_PM::CPM_LTE_Case_PM()
	: CFormView(CPM_LTE_Case_PM::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CPM_LTE_Case_PM::~CPM_LTE_Case_PM()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_LIST3, m_cList);
	DDX_Control(pDX, IDC_QUARY2, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel);
	DDX_Control(pDX, IDC_SUM_ALARM2, m_cCase);
	DDX_Control(pDX, IDC_LIST5, m_cList_Case);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPM_LTE_Case_PM, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL2, &CPM_LTE_Case_PM::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CPM_LTE_Case_PM::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CPM_LTE_Case_PM::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_QUARY2, &CPM_LTE_Case_PM::OnBnClickedQuary2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CPM_LTE_Case_PM::OnNMRClickList3)
	ON_COMMAND(ID_PM_LTE_CASE_PM_DEL, &CPM_LTE_Case_PM::OnPmLteCaseDel)
	ON_COMMAND(ID_PM_LTE_CASE_PM_M_DEL, &CPM_LTE_Case_PM::OnPmLteCaseWDel)
	ON_BN_CLICKED(IDC_EXCEL3, &CPM_LTE_Case_PM::OnBnClickedExcel3)
	ON_NOTIFY(NM_RCLICK, IDC_LIST5, &CPM_LTE_Case_PM::OnNMRClickList5)
END_MESSAGE_MAP()


// CPM_LTE_Case_PM 诊断

#ifdef _DEBUG
void CPM_LTE_Case_PM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPM_LTE_Case_PM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPM_LTE_Case_PM 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="LTE_性能查询模板_性能";								//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗 [模板]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"用户账号",LVCFMT_LEFT,100);					//
	m_cList.InsertColumn(2,"模板名称",LVCFMT_LEFT,120);					//
	m_cList.InsertColumn(3,"创建日期",LVCFMT_CENTER,120);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	//初始化：列表窗 [小区]
	::SendMessage(m_cList_Case.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Case.InsertColumn(0,"序号",LVCFMT_CENTER,60);				//
	m_cList_Case.InsertColumn(1,"用户账号",LVCFMT_LEFT,100);			//
	m_cList_Case.InsertColumn(2,"模板名称",LVCFMT_LEFT,120);			//
	m_cList_Case.InsertColumn(3,"类型",LVCFMT_LEFT,100);				//
	m_cList_Case.InsertColumn(4,"性能",LVCFMT_LEFT,280);				//
	m_cList_Case.InsertColumn(5,"预警",LVCFMT_CENTER,80);				//
	m_cList_Case.InsertColumn(6,"预警门限",LVCFMT_RIGHT,80);			//
	m_cList_Case.InsertColumn(7,"预警类型",LVCFMT_CENTER,80);			//
	m_cList_Case.InsertColumn(8,"创建日期",LVCFMT_CENTER,120);			//
	m_cList_Case.SetExtendedStyle(m_cList_Case.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//分窗
	CRect	v_cRect;													//矩形
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//获得：指针
	pWnd->GetWindowRect(&v_cRect);										//获得：尺寸
	ScreenToClient(&v_cRect);											//坐标变换
	pWnd->DestroyWindow();												//销毁：指针

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Main.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_cList,				// Top pane
		&m_cList_Case,			// Bottom pane
		IDC_PM_LTE_Case_PM,		// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);
	m_SplitterPane_Bottom.m_bVertSplitter_Parent=true;					//垂直分隔符为父窗

	PostMessage(WM_SIZE);												//最大化

	//信息初始化
	v_iList_Item=-1;													//条目选择无效

	//查询
	v_iWorking=1;														//工作任务：查询
	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CPM_LTE_Case_PM::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)	//键盘消息？？？
	{
		// Translate dialog key if applicable
		if(::IsDialogMessage(m_hWnd, pMsg))								//转换消息 [把消息发到m_hWnd]
			return TRUE;												//消息结束 [不会把该消息分发给窗口函数处理]
	}

	return CFormView::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：子窗销毁
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//查找子窗名称
	if(v_pIterator_Frame != theApp.v_mFrame.end())						//子窗存在？
		theApp.v_mFrame.erase(v_pIterator_Frame);						//在容器中清除子窗名称
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：调整窗体大小
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//分窗尺寸调整
		CRect v_cRect;
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=45;
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导出
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Case);										//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表点击响应
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item>=0)											//有效条目序号？？？
				m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));	//恢复背景色
			v_iList_Item=pNMListView->iItem;							//获得：选中条目序号
			v_sName_Account=m_cList.GetItemText(v_iList_Item,1);		//获取：选择的帐号
			v_sName_Case=m_cList.GetItemText(v_iList_Item,2);			//获取：模板名称
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));			//设置背景色：红色

			//查询
			v_iWorking=2;												//工作任务：查询
			v_pIterator_Frame->second.v_iThread_Exit=1;					//线程：工作
			v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);		//开启查询数据库线程
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表右击响应【模板】
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu	aMenu;														//菜单
	CPoint	point;														//位置点

	GetCursorPos(&point);												//获得：鼠标位置
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：点击的列表窗位置
	int v_iSelect = pNMListView->iItem;									//获取：点击列表窗的Item
	if( v_iSelect == -1 )												//有效Item？？？
		return;															//无效：返回
	if(v_iList_Item>=0)													//有效条目序号？？？
		m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));			//恢复背景色
	v_iList_Item=v_iSelect;												//获得：选中条目序号
	m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));					//设置背景色：红色

	aMenu.LoadMenu(IDR_PM_LTE_CASE_PM_DEL);								//装载
	CMenu *pPopup =aMenu.GetSubMenu(0);									//生成浮动菜单
	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);			//显示：依据鼠标位置[point]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：删除性能查询模板
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnPmLteCaseDel()
{
	// TODO: 在此添加命令处理程序代码
	int				v_iItem,vi;											//临时变量
	CString			v_sTemp,v_sSql,v_sName,v_sAccount;					//临时变量
	COleDateTime	v_tTime=COleDateTime::GetCurrentTime();
	CString			v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");

	vi = m_cList.GetSelectedCount();									//选中条目数量
	v_sTemp.Format("%d",vi);											//字符串
	if(MessageBox("删除选中的 "+v_sTemp+" 个 性能查询模板_性能， 删除确认 ? ","数据删除确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)	//删除确认？？？
	{
		//对所有选中的Item进行操作
		while ((v_iItem = m_cList.GetNextItem(-1, LVNI_SELECTED)) != -1)//条目选中？？？
		{
			v_sAccount=m_cList.GetItemText(v_iItem,1);					//获得：帐号
			v_sName=m_cList.GetItemText(v_iItem,2);						//获得：模板名称

			//删除：Cell
			v_sSql = "delete from CON_PM_LTE_Case_PM where zAccount='"
						+v_sAccount+"' and zName='"+v_sName+"'\r\n";	//SQL删除
			v_sSql += "delete from CON_PM_LTE_Case_PM_Data where zAccount='"
						+v_sAccount+"' and zName='"+v_sName+"'";		//SQL删除
			My_ExecuteSQL(v_sSql);										//写表

			//删除：列表信息
			m_cList.DeleteItem(v_iItem);								//删除：选中的v_iItem

			//保存：Log
			v_sSql = "Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"
						+v_sCurrentTime+"','性能查询模板_性能_删除','模板名称："
						+v_sName+" 【操作员："+theApp.a_sUser_Name+"】')";
			My_ExecuteSQL(v_sSql);										//数据库执行
		}
		OnBnClickedQuary2();											//获取模板
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表右击响应【性能】
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnNMRClickList5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu	aMenu;														//菜单
	CPoint	point;														//位置点

	GetCursorPos(&point);												//获得：鼠标位置
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：点击的列表窗位置
	int v_iSelect = pNMListView->iItem;									//获取：点击列表窗的Item
	if( v_iSelect == -1 )												//有效Item？？？
		return;															//无效：返回

	aMenu.LoadMenu(IDR_PM_LTE_CASE_PM_M_DEL);							//装载
	CMenu *pPopup =aMenu.GetSubMenu(0);									//生成浮动菜单
	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);			//显示：依据鼠标位置[point]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：删除性能查询模板【性能】
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnPmLteCaseWDel()
{
	// TODO: 在此添加命令处理程序代码
	int				v_iItem,vi;											//临时变量
	CString			v_sTemp,v_sSql,v_sName,v_sType,v_sPM_Name,v_sAccount;	//临时变量
	COleDateTime	v_tTime=COleDateTime::GetCurrentTime();
	CString			v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");

	vi = m_cList_Case.GetSelectedCount();								//选中条目数量
	v_sTemp.Format("%d",vi);											//字符串
	if(MessageBox("删除选中的 "+v_sTemp+" 个 性能查询模板_性能， 删除确认 ? ","数据删除确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)	//删除确认？？？
	{
		//对所有选中的Item进行操作
		while ((v_iItem = m_cList_Case.GetNextItem(-1, LVNI_SELECTED)) != -1)//条目选中？？？
		{
			v_sAccount=m_cList_Case.GetItemText(v_iItem,1);				//获得：帐号
			v_sName=m_cList_Case.GetItemText(v_iItem,2);				//获得：模板名称
			v_sType=m_cList_Case.GetItemText(v_iItem,3);				//获得：类型
			v_sPM_Name=m_cList_Case.GetItemText(v_iItem,4);				//获得：性能

			//删除：Cell
			v_sSql = "delete from CON_PM_LTE_Case_PM_Data where zAccount='"+v_sAccount
						+"' and zName='"+v_sName+"' and zType='"+v_sType+"' and zPM_Name='"+v_sPM_Name+"'";		//SQL删除
			My_ExecuteSQL(v_sSql);										//写表

			//删除：列表信息
			m_cList_Case.DeleteItem(v_iItem);							//删除：选中的v_iItem

			//保存：Log
			v_sSql = "Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"
				+v_sCurrentTime+"','性能查询模板_性能_删除','模板名称："
				+v_sName+"；性能名称："+v_sPM_Name+" 【操作员："+theApp.a_sUser_Name+"】')";
			My_ExecuteSQL(v_sSql);										//数据库执行
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnBnClickedQuary2()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::OnBnClickedExcel3()
{
	// TODO: 在此添加控件通知处理程序代码
	CPM_LTE_Case_PM_Input	v_cDlg;										//实例化
	v_cDlg.DoModal();													//打开对话窗
	OnBnClickedQuary2();												//获取模板
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CPM_LTE_Case_PM::My_Thread_Inoput(LPVOID lparam)
{
	CPM_LTE_Case_PM *  lp_this = NULL ;
	lp_this = (CPM_LTE_Case_PM *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Case_PM::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Per();												//查询
		break;
	case 2:
		My_LoadData_Case();												//查询
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载信息【模板】
//------------------------------------------------------------------------------------------------------			
void CPM_LTE_Case_PM::My_LoadData_Per()
{
	int				v_iItem,v_iID=0;									//临时变量
	CString			v_sTemp,v_sSql;										//临时变量
	CString			v_sUser,v_sName,v_sDate;							//临时变量
	COleDateTime	v_dDate;											//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList.DeleteAllItems();										//清除Items
		m_cList_Case.DeleteAllItems();									//清除Items
		v_iList_Item=-1;												//条目选择无效

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_PM_LTE_Case_PM where zAccount='共享' or zAccount='"
					+theApp.a_sUser_Account+"' order by zAccount,zName";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sUser = v_cAdoConn.GetField_String("zAccount");			//zAccount
			v_sName = v_cAdoConn.GetField_String("zName");				//zName
			v_dDate = v_cAdoConn.GetField_DataTime("zDate");			//zDate
			v_sDate = v_dDate.Format(_T("%Y-%m-%d"));					//字符串

			//显示
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,v_sUser);						//v_sUser
			m_cList.SetItemText(v_iItem,2,v_sName);						//v_sName
			m_cList.SetItemText(v_iItem,3,v_sDate);						//v_sDate

			v_pRecordset->MoveNext();									//
			v_iID++;													//计数：告警数量

			if (v_iID%100==0)											//显示：100的整数
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum.SetWindowText(v_sTemp);							//显示：总数
			}
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
//	函数功能：装载信息【性能】
//------------------------------------------------------------------------------------------------------			
void CPM_LTE_Case_PM::My_LoadData_Case()
{
	int				v_iItem,v_iID=0,v_iTemp;							//临时变量
	double			v_dTemp;											//临时变量
	CString			v_sTemp,v_sSql;										//临时变量
	CString			v_sUser,v_sName,v_sType,v_sPM_Name,v_sDate;			//临时变量
	COleDateTime	v_dDate;											//临时变量
	CString			v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;			//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList_Case.DeleteAllItems();									//清除Items

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_PM_LTE_Case_PM_Data where zAccount='"+v_sName_Account+"' and zName='"+v_sName_Case+"' order by zNum";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sUser = v_cAdoConn.GetField_String("zAccount");			//zAccount
			v_sName = v_cAdoConn.GetField_String("zName");				//zName
			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sPM_Name = v_cAdoConn.GetField_String("zPM_Name");		//zPM_Name
			v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Flag");				//zAlarm_Flag
			v_sAlarm_Flag=(v_iTemp==1?"预警":"不预警");						
			v_dTemp=v_cAdoConn.GetField_Double("zAlarm_Value");			//zAlarm_Value
			v_sAlarm_Value.Format("%6.2f",v_dTemp);
			v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Type");				//zAlarm_Type
			v_sAlarm_Type=(v_iTemp==1?"高":"低");						
			v_dDate = v_cAdoConn.GetField_DataTime("zDate");			//zDate
			v_sDate = v_dDate.Format(_T("%Y-%m-%d"));					//字符串

			//显示
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList_Case.InsertItem(0xffff,v_sTemp);			//加入列表窗
			m_cList_Case.SetItemText(v_iItem,1,v_sUser);				//v_sUser
			m_cList_Case.SetItemText(v_iItem,2,v_sName);				//v_sName
			m_cList_Case.SetItemText(v_iItem,3,v_sType);				//v_sType
			m_cList_Case.SetItemText(v_iItem,4,v_sPM_Name);				//v_sPM_Name
			m_cList_Case.SetItemText(v_iItem,5,v_sAlarm_Flag);			//预警标志
			m_cList_Case.SetItemText(v_iItem,6,v_sAlarm_Value);			//预警值
			m_cList_Case.SetItemText(v_iItem,7,v_sAlarm_Type);			//预警类型
			m_cList_Case.SetItemText(v_iItem,8,v_sDate);				//v_sDate

			v_pRecordset->MoveNext();									//
			v_iID++;													//计数：告警数量

			if (v_iID%100==0)											//显示：100的整数
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cCase.SetWindowText(v_sTemp);							//显示：总数
			}
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cCase.SetWindowText(v_sTemp);									//显示：总数

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
bool CPM_LTE_Case_PM::My_ExecuteSQL(CString v_sSql)
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
