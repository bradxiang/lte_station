// Alarm_PM.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "Alarm_PM.h"

// CAlarm_PM

IMPLEMENT_DYNCREATE(CAlarm_PM, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CAlarm_PM::CAlarm_PM()
	: CFormView(CAlarm_PM::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CAlarm_PM::~CAlarm_PM()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_LOG_SUM, m_cLog_Sum);
	DDX_Control(pDX, IDC_BEG_DATE, m_cLog_Date);
	DDX_Control(pDX, IDC_COMBO1, m_cLog_Day);
	DDX_Control(pDX, IDC_KEY, m_cLog_Key);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_COMBO2, m_cType);
	DDX_Control(pDX, IDC_CHECK1, m_cReal);
	DDX_Control(pDX, IDC_CHECK4, m_cTime);
	DDX_Control(pDX, IDC_LIST9, m_cList_PM);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAlarm_PM, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CAlarm_PM::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL, &CAlarm_PM::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CAlarm_PM::OnLvnColumnclickList1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_EXCEL2, &CAlarm_PM::OnBnClickedExcel2)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST9, &CAlarm_PM::OnLvnColumnclickList9)
	ON_COMMAND(ID_ALARM_PM_CELL, &CAlarm_PM::OnAlarmPmCell)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CAlarm_PM::OnNMClickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CAlarm_PM::OnNMRClickList1)
	ON_BN_CLICKED(IDC_EXCEL3, &CAlarm_PM::OnBnClickedExcel3)
END_MESSAGE_MAP()


// CAlarm_PM 诊断

#ifdef _DEBUG
void CAlarm_PM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAlarm_PM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAlarm_PM 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="性能预警查询";										//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗【预警】
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"日    期",LVCFMT_CENTER,120);  
	m_cList.InsertColumn(2,"性能",LVCFMT_LEFT,140);  
	m_cList.InsertColumn(3,"预警对象",LVCFMT_LEFT,140);  
	m_cList.InsertColumn(4,"预警类型",LVCFMT_LEFT,80);  
	m_cList.InsertColumn(5,"性能参数",LVCFMT_RIGHT,80);  
	m_cList.InsertColumn(6,"预警门限",LVCFMT_RIGHT,80);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList.SetNumber(5);												//数字设置
	m_cList.SetNumber(6);												//数字设置

	//初始化：列表窗【性能Top20】
	m_cList_PM.ModifyStyle( 0, LVS_REPORT|LVS_SHOWSELALWAYS, TRUE );
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList_PM.InsertColumn(1,"小区",LVCFMT_LEFT,200);					//第1列：小区
	m_cList_PM.InsertColumn(2,"营业区",LVCFMT_LEFT,60);					//第2列：营业区
	m_cList_PM.InsertColumn(3,"状态",LVCFMT_LEFT,40);					//第3列：状态
	m_cList_PM.InsertColumn(4,"日期",LVCFMT_CENTER,120);				//第4列：日期
	m_cList_PM.InsertColumn(5,"性能",LVCFMT_LEFT,100);					//第5列：性能
	m_cList_PM.InsertColumn(6,"参数",LVCFMT_RIGHT,80);					//第6列：数据
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList_PM.SetNumber(6);											//数字设置

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
		IDC_Alarm_PM,			// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);
	//变量初始化
	m_cLog_Day.SetCurSel(4);											//默认：查询天数

	SetTimer(1,60000,NULL);												//1 mins定时 

	v_dAlarm_Date=COleDateTime::GetCurrentTime();						//获取：告警时间

	//选择
	m_cType.AddString("性能");
	m_cType.AddString("预警对象");
	m_cType.SetCurSel(0);												//默认

	v_iList_Item=-1;													//条目选择无效

	//刷新界面
	PostMessage(WM_SIZE);

	//获取：初始数据
	My_LoadData_Para();													//获取：初始数据
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：子窗销毁
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnDestroy()
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
void CAlarm_PM::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//分窗尺寸调整
		CRect v_cRect;
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=40;												//性能查询：80(显示查询过程)；其它：40
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CAlarm_PM::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：定时器
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString			v_sTemp;											//临时变量
	COleDateTime	v_dTime;											//临时变量

	if (m_cReal.GetCheck())												//有效：实时？？？
	{
		v_dTime=My_LoadData_Time();										//获取：性能粒度
		if (v_dTime!=v_dPM_Date)										//性能粒度变化？？？
		{
			v_bAlarm=true;												//有效：新告警提示
			v_dPM_Date=v_dTime;											//获取性能时间
			My_Begin();													//开始查询
		}
	}

	CFormView::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击查询按钮
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码

	v_bAlarm=false;														//无效：新告警提示
	My_Begin();															//开始查询
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：开始查询
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::My_Begin()
{
	// TODO: 在此添加控件通知处理程序代码
	//【1】获取：查询粒度
	v_dPM_Date=My_LoadData_Time();										//获取：性能粒度

	//【2】开始查询(线程)
	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导出Excel
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导出Excel
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnBnClickedExcel3()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_PM);											//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表排序(右击列表任意列)【预警信息】
//------------------------------------------------------------------------------------------------------			
void CAlarm_PM::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表排序(右击列表任意列)【性能明细】
//------------------------------------------------------------------------------------------------------			
void CAlarm_PM::OnLvnColumnclickList9(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList_PM.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表点击响应
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表右击响应
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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

	aMenu.LoadMenu(IDR_ALARM_PM);										//装载：右键菜单
	CMenu *pPopup =aMenu.GetSubMenu(0);									//生成浮动菜单
	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);			//显示：依据鼠标位置[point]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能查询
//	依    据：根据右键选择，判断预警类型：区域==》My_LoadData_Region；小区==》My_LoadData_Cell
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnAlarmPmCell()
{
	CString v_sType=m_cList.GetItemText(v_iList_Item,4);				//预警类型

	if (v_sType=="区域")
		My_LoadData_Region();											//性能查询：区域
	else if (v_sType=="小区计算" || v_sType=="小区预警")
		My_LoadData_Cell();												//性能查询：小区
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能查询：区域【Top 30】
//	依    据：【表名称：计算性能15m粒度；性能类型：区域预警；】
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::My_LoadData_Region()
{
	CString			v_sTemp,v_sCell,v_sTime,v_sPm_Name,v_sRegion,v_sOnline;		//临时变量
	bool			v_bPM=false;										//临时变量
	COleDateTime	v_dDate_Start;										//查询开始日期
	CString			v_sTable,v_sSql,v_sAlarm_Type;						//日期
	double			v_dValue;											//临时变量

	//【1】数据初始化
	m_cList_PM.DeleteAllItems();										//清除列表数据

	v_sTime=m_cList.GetItemText(v_iList_Item,1);						//时间
	v_sTime.Replace("_"," ");											//替换
	v_sPm_Name=m_cList.GetItemText(v_iList_Item,2);						//性能
	v_sRegion=m_cList.GetItemText(v_iList_Item,3);						//预警对象：区域

	//【2】获取性能表名称(15m粒度，每天一个表)
	v_dDate_Start.SetDateTime(atoi(v_sTime.Left(4)),atoi(v_sTime.Mid(5,2)),atoi(v_sTime.Mid(8,2)),atoi(v_sTime.Mid(11,2)),atoi(v_sTime.Mid(14,2)),0);	//设置时间
	v_sTemp=v_dDate_Start.Format("%Y%m%d");								//年月日
	v_sTable = "Pm_LTE_Cal_"+v_sTemp;									//表名称【计算性能表】

	//【3】检查：表名称是否有效【依据：表名称集合v_mTable_Name】【如果表名称无效，则查不出性能数据。】
	set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);		//查找？？
	if (v_pTable==v_mTable_Name.end())									//没找到：数据表
		return;															//无效返回

	//【4】检查：性能是否有效【依据：性能集合v_mLTE_PM_Type】
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pLTE_PM_Name;								//迭代器

	v_pLTE_PM_Type=v_mLTE_PM_Type.find("计算性能");						//查找：性能类型
	if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())							//没找到？？？
		return;															//无效返回
	v_pLTE_PM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.find(v_sPm_Name);	//查找：性能名称
	if (v_pLTE_PM_Name==v_pLTE_PM_Type->second.v_mLTE_PM.end())			//没找到？？？
		return;															//无效返回
	v_sAlarm_Type=v_pLTE_PM_Name->second.v_sAlarm_Type;					//获取：告警类型

	//【5】性能数据获取
	_variant_t		Var;
	_RecordsetPtr	v_pRecordset;										//记录集
	CMyADOConn		v_cAdoConn;											//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select LTE_Cell.zCell,LTE_Cell.zRegion,LTE_Cell.zOnline,"+v_sPm_Name+" from "+v_sTable+" left join LTE_Cell ON "+v_sTable
				+".zCell= LTE_Cell.zCell_ID where zTime='"+v_sTime+"' and LTE_Cell.zRegion='"+v_sRegion
				+"' order by "+v_sPm_Name;								//SQL
		if (v_sAlarm_Type=="1")											//高有效？？？
			v_sSql+=" asc";												//SQL
		else															//低有效
			v_sSql+=" desc";											//SQL

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_sCell=Var.bstrVal;									//获取：Cell
				v_dValue=v_cAdoConn.GetField_Double(v_sPm_Name);		//获取：性能
				v_sRegion=v_cAdoConn.GetField_String("zRegion");		//获取：营业区
				v_sOnline=v_cAdoConn.GetField_String("zOnline");		//获取：状态

				if (v_dValue!=-1)
				{
					//【6】显示
					int	v_iItem = m_cList_PM.InsertItem(0xffff,_T("0"));
					v_sTemp.Format("%04d",v_iItem+1);
					m_cList_PM.SetItemText(v_iItem,0," "+v_sTemp);		//序号
					m_cList_PM.SetItemText(v_iItem,1,v_sCell);			//小区
					m_cList_PM.SetItemText(v_iItem,2,v_sRegion);		//性能名称
					m_cList_PM.SetItemText(v_iItem,3,v_sOnline);		//预警对象
					m_cList_PM.SetItemText(v_iItem,4,v_sTime);			//时间
					m_cList_PM.SetItemText(v_iItem,5,v_sPm_Name);		//性能
					v_sTemp.Format("%8.2f",v_dValue);					//字符串
					m_cList_PM.SetItemText(v_iItem,6,v_sTemp);			//性能参数
				}
				if (m_cList_PM.GetItemCount()>=30)						//Top 30 ？？？
					break;												//退出
			}
			v_pRecordset->MoveNext();									//
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
//	函数功能：性能查询：小区【查询小区，预警当天的所有性能】
//	依    据：【表名称：计算性能15m粒度；性能类型：小区计算、小区预警；】
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::My_LoadData_Cell()
{
	CString			v_sTemp,v_sCell,v_sTime,v_sPm_Name,v_sRegion,v_sOnline;		//临时变量
	bool			v_bPM=false;										//临时变量
	COleDateTime	v_dDate_Start;										//查询开始日期
	CString			v_sTable,v_sSql,v_sAlarm_Type,v_sType;				//日期
	double			v_dValue;											//临时变量

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pLTE_PM_Name;								//迭代器

	//【1】数据初始化
	m_cList_PM.DeleteAllItems();										//清除列表数据

	v_sTime=m_cList.GetItemText(v_iList_Item,1);						//时间
	v_sTime.Replace("_"," ");											//替换
	v_sPm_Name=m_cList.GetItemText(v_iList_Item,2);						//性能
	v_sCell=m_cList.GetItemText(v_iList_Item,3);						//预警对象：小区
	v_sType=m_cList.GetItemText(v_iList_Item,4);						//预警类型：小区计算、小区预警

	//【2】获取性能表名称(15m粒度，每天一个表)
	v_dDate_Start.SetDateTime(atoi(v_sTime.Left(4)),atoi(v_sTime.Mid(5,2)),atoi(v_sTime.Mid(8,2)),atoi(v_sTime.Mid(11,2)),atoi(v_sTime.Mid(14,2)),0);	//设置时间
	v_sTemp=v_dDate_Start.Format("%Y%m%d");								//年月日
	if (v_sType=="小区计算")											//计算性能表
	{
		v_sTable = "Pm_LTE_Cal_"+v_sTemp;								//表名称【计算性能表】
		//【3】检查：性能是否有效【依据：性能集合v_mLTE_PM_Type】
		v_pLTE_PM_Type=v_mLTE_PM_Type.find("计算性能");					//查找：性能类型
		if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())						//没找到？？？
			return;														//无效返回
		v_pLTE_PM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.find(v_sPm_Name);	//查找：性能名称
		if (v_pLTE_PM_Name==v_pLTE_PM_Type->second.v_mLTE_PM.end())		//没找到？？？
			return;														//无效返回
		v_sAlarm_Type=v_pLTE_PM_Name->second.v_sAlarm_Type;				//获取：告警类型
	}
	else																//原始性能表
	{
		//【3】检查：性能是否有效【依据：性能集合v_mLTE_PM_Type】
		for(v_pLTE_PM_Type=v_mLTE_PM_Type.begin();v_pLTE_PM_Type!=v_mLTE_PM_Type.end();v_pLTE_PM_Type++)	//遍历
		{
			v_pLTE_PM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.find(v_sPm_Name);	//查找：性能名称
			if (v_pLTE_PM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())	//找到？？？
				break;
		}
		if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())						//没找到？？？
			return;														//无效返回
		
		//获取性能信息
		v_sTable = "Pm_LTE_"+v_pLTE_PM_Name->second.v_sTable_Key+"_"+v_sTemp;	//表名称【原始性能表】
		v_sAlarm_Type=v_pLTE_PM_Name->second.v_sAlarm_Type;				//获取：告警类型
	}

	//【4】检查：表名称是否有效【依据：表名称集合v_mTable_Name】【如果表名称无效，则查不出性能数据。】
	set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);		//查找？？
	if (v_pTable==v_mTable_Name.end())									//没找到：数据表
		return;															//无效返回
	
	//【5】Cell_ID ==> Cell


	//【6】性能数据获取
	_variant_t		Var;
	_RecordsetPtr	v_pRecordset;										//记录集
	CMyADOConn		v_cAdoConn;											//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select LTE_Cell.zCell,LTE_Cell.zRegion,LTE_Cell.zOnline,"+v_sTable+".zTime,["+v_sPm_Name
				+"] from "+v_sTable+" left join LTE_Cell ON "+v_sTable
				+".zCell= LTE_Cell.zCell_ID where LTE_Cell.zCell='"+v_sCell+"' order by zTime asc";	//SQL

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_sCell=Var.bstrVal;									//获取：Cell
				v_dValue=v_cAdoConn.GetField_Double(v_sPm_Name);		//获取：性能
				v_sRegion=v_cAdoConn.GetField_String("zRegion");		//获取：营业区
				v_sOnline=v_cAdoConn.GetField_String("zOnline");		//获取：状态
				v_dDate_Start=v_cAdoConn.GetField_DataTime("zTime");	//获取：时间
				v_sTime=v_dDate_Start.Format("%Y-%m-%d_%H:%M:%S");		//字符串
		
				if (v_dValue!=-1)
				{
					//【6】显示
					int	v_iItem = m_cList_PM.InsertItem(0xffff,_T("0"));
					v_sTemp.Format("%04d",v_iItem+1);
					m_cList_PM.SetItemText(v_iItem,0," "+v_sTemp);		//序号
					m_cList_PM.SetItemText(v_iItem,1,v_sCell);			//小区
					m_cList_PM.SetItemText(v_iItem,2,v_sRegion);		//性能名称
					m_cList_PM.SetItemText(v_iItem,3,v_sOnline);		//预警对象
					m_cList_PM.SetItemText(v_iItem,4,v_sTime);			//时间
					m_cList_PM.SetItemText(v_iItem,5,v_sPm_Name);		//性能
					v_sTemp.Format("%8.2f",v_dValue);					//字符串
					m_cList_PM.SetItemText(v_iItem,6,v_sTemp);			//性能参数
				}
			}
			v_pRecordset->MoveNext();									//
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
//	函数功能：预警确认
//------------------------------------------------------------------------------------------------------
void CAlarm_PM::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	v_dAlarm_Date=COleDateTime::GetCurrentTime();						//获取：告警时间
	OnBnClickedQuary();													//预警查询
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CAlarm_PM::My_Thread_Inoput(LPVOID lparam)
{
	CAlarm_PM *  lp_this = NULL ;
	lp_this = (CAlarm_PM *)lparam;

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
void CAlarm_PM::My_Input_Main()
{
	m_cList_PM.DeleteAllItems();										//清除列表数据
	m_cList.DeleteAllItems();											//清除列表数据
	v_iList_Item=-1;													//条目选择无效
	My_LoadData();														//装载Bug信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载预警信息
//------------------------------------------------------------------------------------------------------			
void CAlarm_PM::My_LoadData()
{
	CString			v_sSql,v_sTemp,v_sLog_Key,v_sAlarm_Type;			//临时变量
	int				v_iItem,v_iID=0;									//临时变量
	int				v_iDay_Sum;											//查询天数
	CString			v_sTime_Start,v_sTime_End;							//日期
	CString			v_sAlarm_PM,v_sAlarm_Object,v_sAlarm_Value,v_sAlarm_Level,v_sAlarm_Content;	//预警内容
	double			v_dValue;											//数值
	COleDateTime	v_dDate_Start,v_dDate_End;							//查询开始日期、结束日期
	bool			v_bMessage=false;									//临时变量
	int				v_iAlarm_Sum=0;										//预警总数
	

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		//查询日期准备
		m_cLog_Date.GetTime(v_dDate_Start);								//获得：查询开始日期
		v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//设置时间：00:00:00
		v_iDay_Sum=m_cLog_Day.GetCurSel()+1;							//获得：查询天数
		v_dDate_End =v_dDate_Start + COleDateTimeSpan(v_iDay_Sum,0,0,0);//获得：查询结束日期
		v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";		//开始时间
		v_sTime_End=v_dDate_End.Format("%Y-%m-%d")+" 00:00:00";			//结束时间
		m_cLog_Key.GetWindowText(v_sLog_Key);							//获得：查询KEY
		v_sLog_Key.Replace("[","[[]");									//限制处理
		v_sLog_Key.Replace("'","");										//限制处理
		v_sLog_Key.Trim();												//去掉空格

		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql = "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select * from PM_Alarm where zTime>='"+v_sTime_Start+"' and zTime<='"+v_sTime_End+"'";	//查询
		if (!v_sLog_Key.IsEmpty())										//有关键字查询？？？
		{
			switch(m_cType.GetCurSel())
			{
			case 0:
				v_sSql += " and zAlarm_PM like '%"+v_sLog_Key+"%'";		//性能
				break;
			case 1:
				v_sSql += " and zAlarm_Object like '%"+v_sLog_Key+"%'";		//性能
				break;
			}
		}

		v_sSql += " order by zTime";									//排序：时间
		if (m_cTime.GetCheck())											//有效：降序？？？
			v_sSql += " desc";											//降序

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_dDate_Start=v_cAdoConn.GetField_DataTime("zTime");		//zTime
			v_sAlarm_PM=v_cAdoConn.GetField_String("zAlarm_PM");		//zAlarm_PM
			v_sAlarm_Object=v_cAdoConn.GetField_String("zAlarm_Object");//zAlarm_Object
			v_sAlarm_Type=v_cAdoConn.GetField_String("zAlarm_Type");	//zAlarm_Type
			v_dValue=v_cAdoConn.GetField_Double("zAlarm_Value");		//zAlarm_Value
			v_sAlarm_Value.Format("%8.2f",v_dValue);
			v_dValue=v_cAdoConn.GetField_Double("zAlarm_Level");		//zAlarm_Level
			v_sAlarm_Level.Format("%8.2f",v_dValue);
			v_sAlarm_Content=v_cAdoConn.GetField_String("zAlarm_Content");	//zAlarm_Content

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d_%H:%M:%S");	//赋值：日期
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sTime_Start);		//
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sAlarm_PM);		//
			m_cList.SetItemText(v_iItem,3,(_bstr_t)v_sAlarm_Object);	//
			m_cList.SetItemText(v_iItem,4,(_bstr_t)v_sAlarm_Type);		//
			m_cList.SetItemText(v_iItem,5,(_bstr_t)v_sAlarm_Value);		//
			m_cList.SetItemText(v_iItem,6,(_bstr_t)v_sAlarm_Level);		//
			m_cList.SetItemText(v_iItem,7,(_bstr_t)v_sAlarm_Content);	//

			if (v_dAlarm_Date<(v_dDate_Start+COleDateTimeSpan(0,0,30,0)))	//预警 && 预警时间<产生时间
			{
				m_cList.SetItemBkColor(v_iItem,RGB(255,0,0));			//
				v_bMessage=true;										//有效：消息
				v_iAlarm_Sum++;											//预警总数++
			}

			v_pRecordset->MoveNext();									//
			v_iID++;													//

			if (v_iID%100==0)
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cLog_Sum.SetWindowText(v_sTemp);						//显示：Bug总数
			}

		}
		v_sTemp.Format("%d",v_iID);										//
		m_cLog_Sum.SetWindowText(v_sTemp);								//显示：Bug总数

		if (v_bAlarm && v_bMessage)
		{
			v_sTemp.Format("%d",v_iAlarm_Sum);							//预警总数
			theApp.a_pDlg_Alarm_Message->m_cSum.SetWindowText("新增 "+v_sTemp+" 个预警");	//预警信息
			theApp.a_pDlg_Alarm_Message->ShowWindow(SW_SHOW);			//显示预警窗体
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
//	函数功能：获取性能粒度
//------------------------------------------------------------------------------------------------------			
COleDateTime CAlarm_PM::My_LoadData_Time()
{
	CString			v_sTemp,v_sSql;										//临时变量
	COleDateTime	v_sTime=COleDateTime::GetCurrentTime();				//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT top 1 * FROM CON_System";						//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		if(!v_pRecordset->adoEOF)
		{
			v_sTime = v_cAdoConn.GetField_DataTime("zPm_LTE_Time_Data");//zPm_LTE_Time_Data [性能粒度]
		}

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	return	v_sTime;													//返回
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能参数信息
//------------------------------------------------------------------------------------------------------			
void CAlarm_PM::My_LoadData_Para()
{
	int				v_iID=0,v_iAlarm_Type;								//临时变量
	CString			v_sTemp,v_sSql,v_sLog_Key,v_sTable;					//临时变量
	CString			v_sType,v_sType_1,v_sPm,v_sPm_Name,v_sTable_Key;	//临时变量
	CString			v_sCell,v_sCell_ID,v_sCell_Name,v_sRegion,v_sOnline;//临时变量
	Cell_ID_Struct	v_cCell_ID;											//结构

	LTE_PM_Struct	v_cLTE_PM;											//结构
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	LTE_PM_Type_Struct		v_cLTE_PM_Type;								//结构

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//【1】获取：性能参数【常规】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE order by ztype,ztype_1,zPm";//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sType_1 = v_cAdoConn.GetField_String("ztype_1");			//ztype_1
			v_sPm = v_cAdoConn.GetField_String("zPm");					//zPm
			v_sPm_Name = v_cAdoConn.GetField_String("zPm_Name");		//zPm_Name
			v_sTable_Key = v_cAdoConn.GetField_String("zTable_Key");	//zTable_Key

			//保存
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//查找：性能类型
			if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())					//没找到？？？
			{
				v_cLTE_PM_Type.v_sTable_Key=v_sTable_Key;				//表名称-Key
				v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//容器增加
			}
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//查找：性能类型
			v_cLTE_PM.v_sTable_Key=v_sTable_Key;						//表名称-Key
			v_cLTE_PM.v_sPM_Name=v_sPm_Name;							//性能：中文名
			v_cLTE_PM.v_bOK=false;										//性能：未选中
			v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//增加：性能参数

			v_pRecordset->MoveNext();									//
		}

		//【2】获取：性能参数【计算性能】
		v_sType="计算性能";												//性能类型								
		v_cLTE_PM_Type.v_sTable_Key = "Cal";							//表名称-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//容器增加

		//获取：性能参数【计算性能】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Cal order by zPm";			//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sPm = v_cAdoConn.GetField_String("zPm");					//zPm
			v_iAlarm_Type = v_cAdoConn.GetField_Int("zAlarm_Type");		//zAlarm_Type

			//保存
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//查找：性能类型
			if (v_pLTE_PM_Type!=v_mLTE_PM_Type.end())					//找到？？？
			{
				v_cLTE_PM.v_sPM_Name=v_sPm;								//性能：中文名
				v_cLTE_PM.v_sTable_Key="Cal";							//性能：表名称
				v_cLTE_PM.v_bOK=false;									//性能：未选中
				v_sTemp.Format("%d",v_iAlarm_Type);						//字符串
				v_cLTE_PM.v_sAlarm_Type=v_sTemp;						//性能：预警类型
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//增加：性能参数
			}

			v_pRecordset->MoveNext();									//
		}

		//【3】获取：性能数据表名称【用于发现无效表名称】
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_LTE_%') order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//表名称

			v_mTable_Name.insert(v_sTable);								//增加集合

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
