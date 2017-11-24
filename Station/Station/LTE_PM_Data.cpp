// LTE_PM_Data.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Data.h"

// CLTE_PM_Data

IMPLEMENT_DYNCREATE(CLTE_PM_Data, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data::CLTE_PM_Data()
	: CFormView(CLTE_PM_Data::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data::~CLTE_PM_Data()
{
	v_mLTE_PM_Type.clear();												//容器清除
	v_mTable_Name.clear();												//容器清除
	v_mRegion.clear();													//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_LOG_SUM, m_cLog_Sum);
	DDX_Control(pDX, IDC_BEG_DATE, m_cLog_Date);
	DDX_Control(pDX, IDC_COMBO1, m_cLog_Day);
	DDX_Control(pDX, IDC_KEY, m_cLog_Key);
	DDX_Control(pDX, IDC_BUTTON1, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_LIST3, m_cList_PM);
	DDX_Control(pDX, IDC_COMBO3, m_cPM_Type);
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_LOG_SUM2, m_cPM_Sum);
	DDX_Control(pDX, IDC_LOG_SUM3, m_cData_Sum);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Data, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL, &CLTE_PM_Data::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CLTE_PM_Data::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Data::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Data::OnNMClickList1)
	ON_CBN_CLOSEUP(IDC_COMBO3, &CLTE_PM_Data::OnCbnCloseupCombo3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CLTE_PM_Data::OnLvnColumnclickList3)
END_MESSAGE_MAP()


// CLTE_PM_Data 诊断

#ifdef _DEBUG
void CLTE_PM_Data::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLTE_PM_Data::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLTE_PM_Data 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CString v_sTemp;													//临时变量
	//窗体名称
	v_sFrame_Name="LTE 性能数据查询";									//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗【小区】
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList.InsertColumn(1,"小区",LVCFMT_LEFT,200);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	m_cList.InsertColumn(2,"小区ID",LVCFMT_LEFT,0);  

	//初始化：列表窗【性能】
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList_PM.InsertColumn(1,"时间",LVCFMT_CENTER,140);  
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//分窗 [先生成左边的上下关系的水平分隔符，再生成左右关系的垂直分隔符，即2*1]
	CRect	v_cRect;													//矩形
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//获得：指针
	pWnd->GetWindowRect(&v_cRect);										//获得：尺寸
	ScreenToClient(&v_cRect);											//坐标变换
	pWnd->DestroyWindow();												//销毁：指针

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Main.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_cList,				// Left pane
		&m_cList_PM,			// Right pane
		IDC_LTE_PM_Data_Main,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);

	//刷新界面
	PostMessage(WM_SIZE);

	//变量初始化
	m_cType.AddString("15m粒度");
	m_cType.AddString("1H粒度");
	m_cType.SetCurSel(0);												//默认：15m粒度

	//初始化：天数选择
	for (int vi=1;vi<=c_iSample_Day;vi++)								//最长天数
	{
		v_sTemp.Format("%d",vi);
		m_cLog_Day.AddString(v_sTemp);									//可选天数复选钮
	}
	m_cLog_Day.SetCurSel(0);											//默认查询天数：1天；

	m_cLog_Day.SetCurSel(4);											//默认：查询天数
	v_iList_Item=-1;													//默认：无选中的列表Item

	//获取：性能参数
	My_LoadData_Para();													//获取：性能参数
	OnCbnCloseupCombo3();												//性能类型选择【修改列标题】
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：子窗销毁
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::OnDestroy()
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
void CLTE_PM_Data::OnSize(UINT nType, int cx, int cy)
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
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Data::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：导出Excel
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_PM);											//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Log列表排序(右击列表任意列)【小区】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Log列表排序(右击列表任意列)【性能】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList_PM.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能类型选择【修改列标题】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::OnCbnCloseupCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp;												//临时变量
	int			v_iNum,v_iItem;											//临时变量
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	//【1】删除列表信息
	m_cList_PM.DeleteAllItems();										//删除所有条目
	while(m_cList_PM.DeleteColumn(0));									//删除所有列：ObjType信息列表
	//【2】在ObjType信息列表添加信息
	m_cList_PM.InsertColumn(0,"序号",LVCFMT_CENTER,50);					//第1列：序号
	m_cList_PM.InsertColumn(1,"小区",LVCFMT_LEFT,140);					//第2列：小区
	m_cList_PM.InsertColumn(2,"日期",LVCFMT_CENTER,140);				//第3列：日期
	//加入性能名称
	v_iNum=3;
	m_cPM_Type.GetWindowText(v_sTemp);									//获得：性能类型
	v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sTemp);						//查找：性能类型
	v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();				//首指针
	while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())			//结束？？
	{
		m_cList_PM.InsertColumn(v_iNum,v_pPM_Name->second.v_sPM_Name,LVCFMT_RIGHT,60);	//第n列：性能
		v_pPM_Name++;													//递加
		v_iNum++;														//计数器
	}
	//【3】显示：性能数量
	v_sTemp.Format("%d",v_pLTE_PM_Type->second.v_mLTE_PM.size());		//性能数量
	m_cPM_Sum.SetWindowText(v_sTemp);									//显示：性能数量
	//【4】选择范围：小区  or 区域
	m_cList.DeleteAllItems();											//删除所有条目
	v_iList_Item=-1;													//复位
	while(m_cList.DeleteColumn(0));										//删除所有列：小区、区域列表
	if (m_cPM_Type.GetCurSel()==0)										//计算性能
	{
		m_cType.EnableWindow(true);										//有效
		m_cQuery.EnableWindow(true);									//有效
		m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);
		m_cList.InsertColumn(1,"小区",LVCFMT_LEFT,200); 
		m_cList.InsertColumn(2,"小区ID",LVCFMT_LEFT,0);  
	}
	else																//区域性能
	{
		m_cType.SetCurSel(0);											//选择：15mins
		m_cType.EnableWindow(false);									//无效
		m_cQuery.EnableWindow(false);									//无效
		m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);
		m_cList.InsertColumn(1,"区域",LVCFMT_LEFT,200);  
		m_cList.InsertColumn(2,"小区ID",LVCFMT_LEFT,0);  

		v_iItem = m_cList.InsertItem(0xffff,_T("0"));
		v_sTemp.Format(" %04d",v_iItem+1);
		m_cList.SetItemText(v_iItem,0," "+v_sTemp);						//序号
		m_cList.SetItemText(v_iItem,1,(_bstr_t)"全网");					//全网
		m_cList.SetItemText(v_iItem,2,(_bstr_t)"全网");					//全网
		v_iItem = m_cList.InsertItem(0xffff,_T("0"));
		v_sTemp.Format(" %04d",v_iItem+1);
		m_cList.SetItemText(v_iItem,0," "+v_sTemp);						//序号
		m_cList.SetItemText(v_iItem,1,(_bstr_t)"华为");					//华为
		m_cList.SetItemText(v_iItem,2,(_bstr_t)"华为");					//华为
		v_iItem = m_cList.InsertItem(0xffff,_T("0"));
		v_sTemp.Format(" %04d",v_iItem+1);
		m_cList.SetItemText(v_iItem,0," "+v_sTemp);						//序号
		m_cList.SetItemText(v_iItem,1,(_bstr_t)"中兴");					//中兴
		m_cList.SetItemText(v_iItem,2,(_bstr_t)"中兴");					//中兴
		set<CString>::iterator v_pTable=v_mRegion.begin();				//开始
		while (v_pTable!=v_mRegion.end())								//结束？？？
		{
			v_iItem = m_cList.InsertItem(0xffff,_T("0"));
			v_sTemp.Format(" %04d",v_iItem+1);
			m_cList.SetItemText(v_iItem,0," "+v_sTemp);					//序号
			m_cList.SetItemText(v_iItem,1,*v_pTable);					//区域
			m_cList.SetItemText(v_iItem,2,*v_pTable);					//区域

			v_pTable++;
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击查询按钮【小区查询】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：小区查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击小区列表【小区对应的性能查询】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));			//设置背景色：红色
		}

		if(v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			v_iWorking=2;												//工作任务：性能查询
			v_pIterator_Frame->second.v_iThread_Exit=1;					//线程：工作
			v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);		//开启查询数据库线程
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CLTE_PM_Data::My_Thread_Inoput(LPVOID lparam)
{
	CLTE_PM_Data *  lp_this = NULL ;
	lp_this = (CLTE_PM_Data *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效
	lp_this->m_cPM_Type.EnableWindow(false);							//性能类型：无效
	lp_this->m_cLog_Date.EnableWindow(false);							//日期：无效
	lp_this->m_cLog_Day.EnableWindow(false);							//天数：无效
	lp_this->m_cType.EnableWindow(false);								//查询类型：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效
	lp_this->m_cPM_Type.EnableWindow(true);								//性能类型：有效
	lp_this->m_cLog_Date.EnableWindow(true);							//日期：有效
	lp_this->m_cLog_Day.EnableWindow(true);								//天数：有效
	if (lp_this->m_cPM_Type.GetCurSel()==0)								//计算性能
		lp_this->m_cType.EnableWindow(true);							//查询类型：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Cell();												//装载Cell信息
		break;
	case 2:
		My_LoadData_PM();												//装载性能信息
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能信息
//	依    据：【表名称：15m/1H粒度；性能类型；】【行：小区、日期】【列：性能类型】
//	性 能 表：15mins：每天一个表；  Hour：5天一个表，一月六个表
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::My_LoadData_PM()
{
	CString			v_sTemp;											//临时变量
	int				v_iDay_Sum;											//临时变量
	COleDateTime	v_dDate_Start,v_dDate_End,v_dDate;					//查询开始日期、结束日期
	CString			v_sTime_Start,v_sTime_End,v_sTable_Num;				//日期

	//【1】显示清除：清除列表性能信息
	m_cList_PM.DeleteAllItems();										//清除：列表数据
	m_cData_Sum.SetWindowText("");										//清除：性能数据总数

	//【2】获取查询日期：输入的查询信息
	m_cLog_Date.GetTime(v_dDate_Start);									//获得：查询开始日期
	v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//设置时间：00:00:00
	v_iDay_Sum=m_cLog_Day.GetCurSel()+1;								//获得：查询天数
	v_dDate_End =v_dDate_Start + COleDateTimeSpan(v_iDay_Sum,0,0,0);	//获得：查询结束日期

	//【3】性能查询：(计算性能：15mins：每天一个表；  Hour：5天一个表，一月六个表)  
	//				 (区域性能：都是15mins，就一个表)
	if (m_cPM_Type.GetCurSel()==0)										//计算性能
	{
		if (m_cType.GetCurSel()==0)										//15m粒度【每天一个表：Pm_LTE_Cal_20160701】
		{
			while(v_dDate_Start<v_dDate_End)							//开始日期<结束日期
			{
				v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";		//开始时间
				v_sTime_End=v_dDate_Start.Format("%Y-%m-%d")+" 23:59:59";		//结束时间
				v_sTemp=v_dDate_Start.Format("%Y%m%d");					//年月日
				v_sTemp="Pm_LTE_Cal_"+v_sTemp;							//文件名称
				My_LoadData_PM_Data(v_sTemp,m_cList.GetItemText(v_iList_Item,1),m_cList.GetItemText(v_iList_Item,2),v_sTime_Start,v_sTime_End);		//装载性能数据
				v_dDate_Start+=COleDateTimeSpan(1,0,0,0);				//增加一天
			}
		}
		else
		{																//Hour
			v_dDate=v_dDate_Start;										//赋值：当前日期
			v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());		//获取表序号
			while(v_dDate<v_dDate_End)									//时间范围内？？？
			{
				v_dDate+=COleDateTimeSpan(1,0,0,0);						//增加一天
				if (My_Get_Table_Num(v_dDate.GetDay())!=v_sTable_Num || v_dDate>=v_dDate_End)
				{
					v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//开始时间
					v_sTime_End=v_dDate.Format("%Y-%m-%d")+" 00:00:00";			//结束时间
					v_sTemp=v_dDate_Start.Format("%Y%m");				//年月
					v_sTemp="Pm_LTE_Cal_"+v_sTemp+"_"+v_sTable_Num;		//文件名称
					My_LoadData_PM_Data(v_sTemp,m_cList.GetItemText(v_iList_Item,1),m_cList.GetItemText(v_iList_Item,2),v_sTime_Start,v_sTime_End);	//装载性能数据
					v_dDate_Start=v_dDate;								//变更：开始时间
					v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());		//获取表序号
				}
			}
		}
	}
	else																//区域性能【就一个表：Pm_LTE_Region】
	{
		v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";		//开始时间
		v_sTime_End=v_dDate_End.Format("%Y-%m-%d")+" 00:00:00";			//结束时间
		My_LoadData_PM_Data("Pm_LTE_Region",m_cList.GetItemText(v_iList_Item,1),m_cList.GetItemText(v_iList_Item,2),v_sTime_Start,v_sTime_End);		//装载性能数据
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取表序号
//	工	  作：1：1-5；	2：6-10；	3：11-15；	4：16-20；	5：21-25；	6：26--
//------------------------------------------------------------------------------------------------------			
CString CLTE_PM_Data::My_Get_Table_Num(int v_iDay)
{
	CString	v_sNum;

	if (v_iDay<=5)
		v_sNum="1";
	else if(v_iDay<=10)
		v_sNum="2";
	else if(v_iDay<=15)
		v_sNum="3";
	else if(v_iDay<=20)
		v_sNum="4";
	else if(v_iDay<=25)
		v_sNum="5";
	else 
		v_sNum="6";

	return v_sNum;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能数据
//	参	  数：v_sTable：表名称；	v_sCell：小区；		v_sCell_ID：小区ID;		v_sDate_Ben：开始日期；		v_sDate_End：结束日期
//	依    据：【表名称：15m/1H粒度；性能类型；】【行：小区、日期】【列：性能类型】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::My_LoadData_PM_Data(CString v_sTable,CString v_sCell,CString v_sCell_ID,CString v_sDate_Ben,CString v_sDate_End)
{
	CString			v_sSql;												//临时变量
	CString			v_sTemp,v_sTime;									//临时变量
	int				v_iItem=0,v_iNum;									//临时变量
	COleDateTime	v_tDate;											//日期
	double			v_dValue;											//性能数值

	set<CString>	v_mTable_Columns;									//表结构
	set<CString>::iterator	v_pTable_Columns;							//表结构

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		//【1】获取：性能指针
		m_cPM_Type.GetWindowText(v_sTemp);								//获得：性能类型
		v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sTemp);					//查找：性能类型
		if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())						//没找到
			return;														//无效返回

		//【2】检查：表名称是否有效【依据：表名称集合v_mTable_Name】
		set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//查找？？
		if (v_pTable==v_mTable_Name.end())								//没找到
			return;														//无效返回

		//【3】获取数据表结构
		My_Load_Columns(&v_mTable_Columns,v_sTable);

		//【4】连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select * from "+v_sTable+" where zCell='"+v_sCell_ID+"' and zTime>='"+v_sDate_Ben
				+"' and zTime<'"+v_sDate_End+"' order by zTime";		//查询数据库：系统日志

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		//【5】获取性能数据
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_tDate=v_cAdoConn.GetField_DataTime("zTime");				//zTime

			//【6】显示：小区、日期
			v_iItem = m_cList_PM.InsertItem(0xffff,_T("0"));
			v_sTemp.Format(" %04d",v_iItem+1);
			m_cList_PM.SetItemText(v_iItem,0,(_bstr_t)" "+v_sTemp);		//序号
			m_cList_PM.SetItemText(v_iItem,1,(_bstr_t)v_sCell);			//小区
			v_sTime=v_tDate.Format("%Y-%m-%d_%H:%M:%S");				//字符串
			m_cList_PM.SetItemText(v_iItem,2,(_bstr_t)v_sTime);			//日期

			//【7】显示：性能
			v_iNum=3;
			v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();		//首指针
			while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())	//结束？？
			{
				//【8】字段存在？？？
				v_pTable_Columns=v_mTable_Columns.find(v_pPM_Name->first);
				if (v_pTable_Columns!=v_mTable_Columns.end())			//存在？？？
				{
					v_dValue=v_cAdoConn.GetField_Double(v_pPM_Name->first);		//获取：性能
					v_sTemp.Format("%8.2f",v_dValue);							//字符串
					m_cList_PM.SetItemText(v_iItem,v_iNum,(_bstr_t)v_sTemp);	//性能
				}

				v_pPM_Name++;											//递加
				v_iNum++;												//计数器
			}
			
			v_pRecordset->MoveNext();									//下一条

			v_iItem++;													//++
			if ((v_iItem)%100==0)
			{
				v_sTemp.Format("%d",v_iItem);							//
				m_cData_Sum.SetWindowText(v_sTemp);						//显示：性能数据总数
			}
		}
		
		v_iItem=m_cList_PM.GetItemCount();								//获取：列表条目数量
		v_sTemp.Format("%d",v_iItem);									//
		m_cData_Sum.SetWindowText(v_sTemp);								//显示：性能数据总数

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	//【9】容器清除
	v_mTable_Columns.clear();											//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载Cell信息
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::My_LoadData_Cell()
{
	CString		v_sSql;													//临时变量
	int			v_iItem,v_iID=0,v_iCell_ID;								//临时变量
	CString		v_sTemp,v_sCell,v_sCell_ID;								//临时变量
	CString		v_sLog_Key;												//日志查询：KEY

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList.DeleteAllItems();										//清除列表数据

		//查询日期准备
		m_cLog_Key.GetWindowText(v_sLog_Key);							//获得：查询KEY
		v_sLog_Key.Replace("[","[[]");									//限制处理
		v_sLog_Key.Replace("'","");										//限制处理
		v_sLog_Key.Trim();												//去掉空格

		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zCell_ID from LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB where LTE_NODEB.zRNC like '%dalian%'";							//查询数据库
		if (!v_sLog_Key.IsEmpty())										//有关键字查询？？？
			v_sSql+=" and zCell like '%"+v_sLog_Key+"%'";				//查询数据库
		v_sSql+=" order by zCell";										//查询数据库

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sCell=v_cAdoConn.GetField_String("zCell");				//zCell
			v_iCell_ID=v_cAdoConn.GetField_Int("zCell_ID");				//zCell_ID

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sCell);			//
			v_sCell_ID.Format("%d",v_iCell_ID);							//字符串
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sCell_ID);			//Cell_ID

			v_pRecordset->MoveNext();									//
			v_iID++;													//

			if (v_iID%100==0)
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cLog_Sum.SetWindowText(v_sTemp);						//显示：Log总数
			}
		}

		v_sTemp.Format("%d",v_iID);										//
		m_cLog_Sum.SetWindowText(v_sTemp);								//显示：Log总数

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
//	函数功能：装载性能参数信息
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::My_LoadData_Para()
{
	int				v_iID=0;											//临时变量
	CString			v_sTemp,v_sSql,v_sLog_Key,v_sTable;					//临时变量
	CString			v_sType,v_sPm;										//临时变量

	LTE_PM_Struct	v_cLTE_PM;											//结构
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	LTE_PM_Type_Struct		v_cLTE_PM_Type;								//结构

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//【1】性能类型：计算性能
		v_sType="计算性能";												//性能类型								
		v_cLTE_PM_Type.v_sTable_Key=v_sType;							//表名称-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//容器增加
		//显示：性能类型
		m_cPM_Type.AddString(v_sType);									//性能类型

		//获取：性能参数
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Cal order by zPm";			//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sPm = v_cAdoConn.GetField_String("zPm");					//zPm

			//保存
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//查找：性能类型
			if (v_pLTE_PM_Type!=v_mLTE_PM_Type.end())					//找到？？？
			{
				v_cLTE_PM.v_sPM_Name=v_sPm;								//性能名称
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//增加：性能参数
			}

			v_pRecordset->MoveNext();									//
		}
		m_cPM_Type.SetCurSel(0);										//默认：第一个

		//【2】性能类型：区域性能
		v_sType="区域性能";												//性能类型								
		v_cLTE_PM_Type.v_sTable_Key=v_sType;							//表名称-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//容器增加
		//显示：性能类型
		m_cPM_Type.AddString(v_sType);									//性能类型

		//获取：性能参数
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Region order by zPm";		//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sPm = v_cAdoConn.GetField_String("zPm");					//zPm

			//保存
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//查找：性能类型
			if (v_pLTE_PM_Type!=v_mLTE_PM_Type.end())					//找到？？？
			{
				v_cLTE_PM.v_sPM_Name=v_sPm;								//性能名称
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//增加：性能参数
			}

			v_pRecordset->MoveNext();									//
		}
		m_cPM_Type.SetCurSel(0);										//默认：第一个

		//【3】获取：性能数据表名称【用于发现无效表名称】
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_LTE_Cal%' or name like 'Pm_LTE_Region%') order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//表名称

			v_mTable_Name.insert(v_sTable);								//增加集合

			v_pRecordset->MoveNext();									//指针++
		}	

		//【4】获取：区域信息【用于区域查询】
		v_sSql="select * from CON_Region";								//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("zRegion");			//zRegion

			v_mRegion.insert(v_sTable);									//增加

			v_pRecordset->MoveNext();									//指针++
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
//	函数功能：装载表结构
//	参	  数：v_pTable_Columns：容器指针
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data::My_Load_Columns(set<CString> *v_pTable_Columns,CString v_sTable)
{
	CString			v_sTemp,v_sSql,v_sColumns;							//临时变量


	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from sys.columns where object_id=object_id('"+v_sTable+"')";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sColumns = v_cAdoConn.GetField_String("name");			//结构名称

			v_pTable_Columns->insert(v_sColumns);						//增加集合

			v_pRecordset->MoveNext();									//指针++
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
//	END
//------------------------------------------------------------------------------------------------------
