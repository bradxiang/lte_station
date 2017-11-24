// QUA_Element_TD.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "QUA_Element_TD.h"


// CQUA_Element_TD

IMPLEMENT_DYNCREATE(CQUA_Element_TD, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CQUA_Element_TD::CQUA_Element_TD()
	: CFormView(CQUA_Element_TD::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CQUA_Element_TD::~CQUA_Element_TD()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_COMBO1, m_cArea);
	DDX_Control(pDX, IDC_LIST3, m_cList_NodeB);
	DDX_Control(pDX, IDC_SUM_ALARM2, m_cSum_NodeB);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_CHECK1, m_cLocation);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CQUA_Element_TD, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CQUA_Element_TD::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL, &CQUA_Element_TD::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CQUA_Element_TD::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_EXCEL2, &CQUA_Element_TD::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CQUA_Element_TD::OnLvnColumnclickList3)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CQUA_Element_TD::OnNMClickList3)
	ON_BN_CLICKED(IDC_CHECK1, &CQUA_Element_TD::OnBnClickedCheck1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CQUA_Element_TD::OnCbnSelchangeCombo1)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CQUA_Element_TD::OnLvnGetdispinfoList1)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST3, &CQUA_Element_TD::OnLvnGetdispinfoList3)
END_MESSAGE_MAP()


// CQUA_Element_TD 诊断

#ifdef _DEBUG
void CQUA_Element_TD::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQUA_Element_TD::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQUA_Element_TD 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="TD-LTE网元查询";										//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗 [NodeB]
	::SendMessage(m_cList_NodeB.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_NodeB.InsertColumn(0,"序号",LVCFMT_CENTER,60);				//
	m_cList_NodeB.InsertColumn(1,"RNC名称",LVCFMT_LEFT,100);			//
	m_cList_NodeB.InsertColumn(2,"NodeB",LVCFMT_LEFT,200);				//
	m_cList_NodeB.InsertColumn(3,"中文名",LVCFMT_LEFT,200);				//
	m_cList_NodeB.InsertColumn(4,"硬件",LVCFMT_LEFT,100);				//
	m_cList_NodeB.InsertColumn(5,"软件版本",LVCFMT_LEFT,200);			//
	m_cList_NodeB.InsertColumn(6,"区域",LVCFMT_LEFT,80);				//
	m_cList_NodeB.InsertColumn(7,"所属机房/资源点",LVCFMT_LEFT,260);	//
	m_cList_NodeB.InsertColumn(8,"状态",LVCFMT_LEFT,60);				//
	m_cList_NodeB.InsertColumn(9,"覆盖类型",LVCFMT_LEFT,60);			//
	m_cList_NodeB.InsertColumn(10,"更新日期[实网]",LVCFMT_CENTER,120);	//
	m_cList_NodeB.InsertColumn(11,"更新日期[资管]",LVCFMT_CENTER,100);	//
	m_cList_NodeB.SetExtendedStyle(m_cList_NodeB.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//初始化：列表窗 [Cell]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"RNC名称",LVCFMT_LEFT,100);					//
	m_cList.InsertColumn(2,"区域",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(3,"NODEB位置",LVCFMT_LEFT,80);					//
	m_cList.InsertColumn(4,"UTRANCELL位置",LVCFMT_LEFT,80);				//
	m_cList.InsertColumn(5,"NodeB",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(6,"NodeB资管状态",LVCFMT_LEFT,80);				//
	m_cList.InsertColumn(7,"Cell",LVCFMT_LEFT,200);						//
	m_cList.InsertColumn(8,"中文名",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(9,"Cell资管状态",LVCFMT_LEFT,80);				//
	m_cList.InsertColumn(10,"LAC",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(11,"覆盖类型",LVCFMT_LEFT,60);					//
	m_cList.InsertColumn(12,"更新日期[实网]",LVCFMT_CENTER,120);		//
	m_cList.InsertColumn(13,"更新日期[资管]",LVCFMT_CENTER,100);		//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cArea.AddString("All");
	m_cArea.AddString("3G");
	m_cArea.AddString("4G");
	m_cArea.SetCurSel(0);												//默认：全部

	//分窗
	CRect	v_cRect;													//矩形
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//获得：指针
	pWnd->GetWindowRect(&v_cRect);										//获得：尺寸
	ScreenToClient(&v_cRect);											//坐标变换
	pWnd->DestroyWindow();												//销毁：指针

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Main.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList_NodeB,			// Top pane
		&m_cList,				// Bottom pane
		IDC_QUA_Element_TD,		// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);

	PostMessage(WM_SIZE);												//最大化

	//信息初始化
	v_iList_Item=-1;													//条目选择无效
	m_cLocation.SetCheck(true);											//选中：NodeB
	OnBnClickedCheck1();												//显示：选择类型
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CQUA_Element_TD::PreTranslateMessage(MSG* pMsg)
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
void CQUA_Element_TD::OnDestroy()
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
void CQUA_Element_TD::OnSize(UINT nType, int cx, int cy)
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
//	函数功能：小区导出
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：NodeB导出
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_NodeB);										//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序 [Cell]
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort_Virtual(pNMHDR);									//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序 [NodeB]
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList_NodeB.My_Sort_Virtual(pNMHDR);								//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：虚拟列表显示数据获取
//	工	  作：依据列表需要的元素参数(行、列)，在数据容器中查询数据，赋值到列表中。
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnLvnGetdispinfoList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	m_cList.Get_List_Data(pDispInfo);									//虚拟列表显示数据获取

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：虚拟列表显示数据获取
//	工	  作：依据列表需要的元素参数(行、列)，在数据容器中查询数据，赋值到列表中。
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	m_cList_NodeB.Get_List_Data(pDispInfo);								//虚拟列表显示数据获取

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击网元类型
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=2;														//工作任务：装载RNC数据
	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击列表窗 [NodeB]
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item>=0)											//有效条目序号？？？
				m_cList_NodeB.SetItemBkColor(v_iList_Item,RGB(255,255,255));	//恢复背景色
			v_iList_Item=pNMListView->iItem;							//获得：选中条目序号
			m_cList_NodeB.SetItemBkColor(v_iList_Item,RGB(255,0,0));	//设置背景色：红色

			v_iWorking=3;												//工作任务：树节点点击有效响应
			v_pIterator_Frame->second.v_iThread_Exit=1;					//线程：工作
			v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);		//开启查询数据库线程
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：NodeB选择
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cKey_Select.ResetContent();										//清除选择内容
	if (m_cLocation.GetCheck())											//BTS选中？？？
	{																	//选中
		m_SplitterPane_Main.ShowTopPane();								//显示：NodeB/Cell
		m_cKey_Select.AddString("NodeB");
		m_cKey_Select.AddString("中文名");
		m_cKey_Select.AddString("RNC");
		m_cKey_Select.AddString("机房/资源点");
	}
	else
	{																	//没选中
		m_SplitterPane_Main.HideTopPane();								//Hide：NodeB/Cell
		m_cKey_Select.AddString("Cell");
		m_cKey_Select.AddString("中文名");
		m_cKey_Select.AddString("NodeB");
		m_cKey_Select.AddString("机房/资源点");
	}
	m_cKey_Select.SetCurSel(0);											//默认：选择第一个
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：小区查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CQUA_Element_TD::My_Thread_Inoput(LPVOID lparam)
{
	CQUA_Element_TD *  lp_this = NULL ;
	lp_this = (CQUA_Element_TD *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效
	lp_this->m_cExcel_NodeB.EnableWindow(false);						//NodeB导出：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效
	lp_this->m_cExcel_NodeB.EnableWindow(true);							//NodeB导出：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		if (m_cLocation.GetCheck())										//站点选中？？？
			My_Query_NodeB();											//BTS查询
		else
			My_Query_Cell();											//Cell查询
		break;
	case 3:
		My_LoadData_NodeB_Cell();										//NodeB列表点击响应
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：NodeB查询
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::My_Query_NodeB()
{
	// TODO: 在此添加控件通知处理程序代码
	int				v_iID=0;											//KPI数量
	CString			v_sSql,v_sTemp;										//临时变量
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT LTE_NODEB.*  FROM LTE_NODEB";

	//查询日期准备
	m_cKey.GetWindowText(v_sLog_Key);									//获得：查询KEY
	v_sLog_Key.Replace("[","[[]");										//限制处理
	v_sLog_Key.Replace("'","");											//限制处理
	v_sLog_Key.Trim();													//去掉空格

	bool v_bFlag=false;													//标记
	if (!v_sLog_Key.IsEmpty())											//查询KEY处理
	{
		v_bFlag=true;													//标记：有效
		switch (m_cKey_Select.GetCurSel())								//选择？？？
		{
		case 0:															//NodeB
			v_sSql+=" where zNODEB like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//中文名
			v_sSql+=" where zNODEB_Name like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//RNC
			v_sSql+=" where zRNC like '%"+v_sLog_Key+"%'";
			break;
		case 3:															//所属机房/资源点
			v_sSql+=" where LTE_NODEB.zStation like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	if (m_cArea.GetCurSel()==1)
	{
		if (v_bFlag)
			v_sSql+=" and zRNC like '%DAL-%'";
		else
			v_sSql+=" where zRNC like '%DAL-%'";
	}
	else if (m_cArea.GetCurSel()==2)
	{
		if (v_bFlag)
			v_sSql+=" and zRNC like '%dalian_%'";
		else
			v_sSql+=" where zRNC like '%dalian_%'";
	}

	v_sSql+=" order by zNODEB";

	My_LoadData_NodeB_Data(v_sSql);										//获取：Cell信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载NodeB信息
//------------------------------------------------------------------------------------------------------			
void CQUA_Element_TD::My_LoadData_NodeB_Data(CString v_sSql)
{
	int				v_iID=0;											//KPI数量
	CString			v_sTemp,v_sCover_Type;								//临时变量
	CString			v_sDate,v_sHardware,v_sIP,v_sVer,v_sStation,v_sOnline,v_sDate_Update,v_sDate_Creat,v_sMaintence;	//临时变量
	CString			v_sRNC_Name,v_sNodeB_Name,v_sNodeB_ID,v_sArea,v_sAgency,v_sAgency_Phone,v_sAgency_Oper;	//临时变量
	COleDateTime	v_dDate_Start;										//查询开始日期

	//虚拟列表
	mInt_String		v_mLine_Data;										//虚拟列表行数据
	m_cList_NodeB.Put_Line_Begin();										//列表添加数据开始

	_variant_t	Var;
	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		m_cList_NodeB.DeleteAllItems();									//列表复位
		m_cList.DeleteAllItems();										//列表复位
		v_iList_Item=-1;												//条目选择无效
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sRNC_Name = v_cAdoConn.GetField_String("zRNC");			//zRNC
			v_sNodeB_Name = v_cAdoConn.GetField_String("zNODEB");		//zNODEB
			v_sNodeB_ID = v_cAdoConn.GetField_String("zNODEB_Name");	//zNODEB_Name
			v_sHardware = v_cAdoConn.GetField_String("zHardware");		//zHardware
			v_sVer = v_cAdoConn.GetField_String("zNODEB_Version");		//zNODEB_Version
			v_sArea = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_sCover_Type = v_cAdoConn.GetField_String("zCover_Type");	//zCover_Type
			Var=v_pRecordset->GetCollect((_variant_t)"zDate");			//更新日期
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sDate = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sDate="";
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			Var=v_pRecordset->GetCollect((_variant_t)"zDate_Update");	//zDate_Update
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sDate_Update = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sDate_Update="";

			//数据复位
			v_mLine_Data.clear();										//容器清除

			//显示
			v_sTemp.Format("%04d",v_iID+1);
			v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));			//序号
			v_mLine_Data.insert(pair<int,CString>(1,v_sRNC_Name));		//网元名称
			v_mLine_Data.insert(pair<int,CString>(2,v_sNodeB_Name));	//NodeB
			if (v_sNodeB_Name.Find("-EL")>=0)
				v_mLine_Data.insert(pair<int,CString>(3,v_sNodeB_ID));	//zNODEB_ID [网管E_Nodeb名称]
			v_mLine_Data.insert(pair<int,CString>(4,v_sHardware));		//v_sHardware
			v_mLine_Data.insert(pair<int,CString>(5,v_sVer));			//软件版本
			v_mLine_Data.insert(pair<int,CString>(6,v_sArea));			//区域
			v_mLine_Data.insert(pair<int,CString>(7,v_sStation));		//所属机房/资源点
			v_mLine_Data.insert(pair<int,CString>(8,v_sOnline));		//状态
			v_mLine_Data.insert(pair<int,CString>(9,v_sCover_Type));	//覆盖类型
			v_mLine_Data.insert(pair<int,CString>(10,v_sDate));			//更新日期[实网]
			v_mLine_Data.insert(pair<int,CString>(11,v_sDate_Update));	//更新日期[资管]

			//添加：虚拟列表
			m_cList_NodeB.Put_Line_Data(v_mLine_Data);					//列表添加行数据

			v_pRecordset->MoveNext();									//
			v_iID++;													//计数：NodeB数量

			if (v_iID%100==0)
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum_NodeB.SetWindowText(v_sTemp);					//显示：NodeB总数
			}
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cSum_NodeB.SetWindowText(v_sTemp);							//显示：NodeB总数

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	//虚拟列表
	m_cList_NodeB.Put_Line_End();										//列表添加数据结束
	v_mLine_Data.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：小区查询
//------------------------------------------------------------------------------------------------------
void CQUA_Element_TD::My_Query_Cell()
{
	// TODO: 在此添加控件通知处理程序代码
	int				v_iID=0;											//KPI数量
	CString			v_sSql,v_sTemp;										//临时变量
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT LTE_Cell.*, LTE_NODEB.zRNC,LTE_NODEB.zStation as zStation_NodeB,\
			  LTE_NODEB.zOnline AS zOnline_NodeB,LTE_NODEB.zRegion AS zRegion_NodeB FROM LTE_Cell \
			  left JOIN LTE_NODEB ON LTE_Cell.zNODEB = LTE_NODEB.zNODEB";
	//查询日期准备
	m_cKey.GetWindowText(v_sLog_Key);									//获得：查询KEY
	v_sLog_Key.Replace("[","[[]");										//限制处理
	v_sLog_Key.Replace("'","");											//限制处理
	v_sLog_Key.Trim();													//去掉空格

	bool v_bFlag=false;													//标记
	if (!v_sLog_Key.IsEmpty())											//查询KEY处理
	{
		v_bFlag=true;													//标记：有效
		switch (m_cKey_Select.GetCurSel())								//选择？？？
		{
		case 0:															//小区名称
			v_sSql+=" where zCell like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//中文名
			v_sSql+=" where zCell_Name like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//NodeB
			v_sSql+=" where LTE_Cell.zNODEB like '%"+v_sLog_Key+"%'";
			break;
		case 3:															//所属机房/资源点
			v_sSql+=" where LTE_Cell.zStation like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	if (m_cArea.GetCurSel()==1)
	{
		if (v_bFlag)
			v_sSql+=" and zRNC like '%DAL-%'";
		else
			v_sSql+=" where zRNC like '%DAL-%'";
	}
	else if (m_cArea.GetCurSel()==2)
	{
		if (v_bFlag)
			v_sSql+=" and zRNC like '%dalian_%'";
		else
			v_sSql+=" where zRNC like '%dalian_%'";
	}

	v_sSql+=" order by zCell";

	My_LoadData_Cell_Data(v_sSql);										//获取：Cell信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载 NodeB_Cell 信息
//------------------------------------------------------------------------------------------------------			
void CQUA_Element_TD::My_LoadData_NodeB_Cell()
{
	CString v_sBTS=m_cList_NodeB.GetItemText(v_iList_Item,2);			//v_sBTS
	CString v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT LTE_Cell.*, LTE_NODEB.zRNC,LTE_NODEB.zStation as zStation_NodeB,\
			  LTE_NODEB.zOnline AS zOnline_NodeB,LTE_NODEB.zRegion AS zRegion_NodeB FROM LTE_Cell \
			  left JOIN LTE_NODEB ON LTE_Cell.zNODEB = LTE_NODEB.zNODEB";
	v_sSql+=" where LTE_Cell.zNODEB='"+v_sBTS+"' order by zCell";		//SQL
	My_LoadData_Cell_Data(v_sSql);										//执行SQL
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载小区信息
//------------------------------------------------------------------------------------------------------			
void CQUA_Element_TD::My_LoadData_Cell_Data(CString v_sSql)
{
	int				v_iID=0;											//临时变量
	CString			v_sTemp;											//临时变量
	COleDateTime	v_dDate_Start;										//查询开始日期
	mString_String::iterator v_pNodeB;									//迭代器

	//虚拟列表
	mInt_String		v_mLine_Data;										//虚拟列表行数据
	m_cList.Put_Line_Begin();											//列表添加数据开始

	_variant_t	Var;
	CString		v_sElementName,v_sNodeB_Name,v_sCell,v_sDate,v_sAgency_Oper,v_sStation_NodeB,v_sOnline_NodeB;	//
	CString		v_sLac,v_sArea,v_sCover,v_sStation,v_sOnline,v_sDate_Update,v_sCell_C;	//
	CString		v_sMaintence;											//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList.DeleteAllItems();										//清除Items

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			Var=v_pRecordset->GetCollect((_variant_t)"zRNC");			//RNC
			if (Var.vt != VT_NULL)
				v_sElementName=Var.bstrVal;
			else
				v_sElementName="";
			Var=v_pRecordset->GetCollect((_variant_t)"zNodeB");			//NodeB
			if (Var.vt != VT_NULL)
				v_sNodeB_Name=Var.bstrVal;
			else
				v_sNodeB_Name="";
			Var=v_pRecordset->GetCollect((_variant_t)"zCell");			//小区号
			if (Var.vt != VT_NULL)
				v_sCell=Var.bstrVal;
			else
				v_sCell="";
			Var=v_pRecordset->GetCollect((_variant_t)"zCell_Name");		//中文名
			if (Var.vt != VT_NULL)
				v_sCell_C=Var.bstrVal;
			else
				v_sCell_C="";
			Var=v_pRecordset->GetCollect((_variant_t)"zLAC");			//LAC
			if (Var.vt != VT_NULL)
				v_sLac=Var.bstrVal;
			else
				v_sLac="";
			Var=v_pRecordset->GetCollect((_variant_t)"zRegion_NodeB");	//区域
			if (Var.vt != VT_NULL)
				v_sArea=Var.bstrVal;
			else
				v_sArea="";
			Var=v_pRecordset->GetCollect((_variant_t)"zCover_Type");	//覆盖类型
			if (Var.vt != VT_NULL)
				v_sCover=Var.bstrVal;
			else
				v_sCover="";
			Var=v_pRecordset->GetCollect((_variant_t)"zDate");			//更新日期
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sDate = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sDate="";
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_sStation_NodeB = v_cAdoConn.GetField_String("zStation_NodeB");	//zStation_NodeB
			v_sOnline_NodeB = v_cAdoConn.GetField_String("zOnline_NodeB");		//zOnline_NodeB
			Var=v_pRecordset->GetCollect((_variant_t)"zDate_Update");	//zDate_Update
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sDate_Update = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sDate_Update="";

			//数据复位
			v_mLine_Data.clear();										//容器清除

			//显示
			v_sTemp.Format("%06d",v_iID+1);
			v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));			//序号
			v_mLine_Data.insert(pair<int,CString>(1,v_sElementName));	//网元名称
			v_mLine_Data.insert(pair<int,CString>(2,v_sArea));			//区域
			v_mLine_Data.insert(pair<int,CString>(3,v_sStation_NodeB));	//NODEB位置
			v_mLine_Data.insert(pair<int,CString>(4,v_sStation));		//UTRANCELL位置
			v_mLine_Data.insert(pair<int,CString>(5,v_sNodeB_Name));	//NodeB
			v_mLine_Data.insert(pair<int,CString>(6,v_sOnline_NodeB));	//状态
			v_mLine_Data.insert(pair<int,CString>(7,v_sCell));			//小区号
			if (v_sNodeB_Name.Find("-EL")>=0)
				v_mLine_Data.insert(pair<int,CString>(8,v_sCell_C));	//中文名
			v_mLine_Data.insert(pair<int,CString>(9,v_sOnline));		//状态
			v_mLine_Data.insert(pair<int,CString>(10,v_sLac));			//LAC
			v_mLine_Data.insert(pair<int,CString>(11,v_sCover));		//覆盖
			v_mLine_Data.insert(pair<int,CString>(12,v_sDate));			//更新日期[实网]
			v_mLine_Data.insert(pair<int,CString>(13,v_sDate_Update));	//更新日期[资管]

			//添加：虚拟列表
			m_cList.Put_Line_Data(v_mLine_Data);						//列表添加行数据

			v_pRecordset->MoveNext();									//
			v_iID++;													//计数：告警数量

			if (v_iID%100==0)											//显示：100的整数
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum.SetWindowText(v_sTemp);							//显示：小区总数
			}
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cSum.SetWindowText(v_sTemp);									//显示：Alarm总数

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	//虚拟列表
	m_cList.Put_Line_End();												//列表添加数据结束
	v_mLine_Data.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
