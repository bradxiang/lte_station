// Alarm_LL_Real.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "Alarm_LL_Real.h"
#include "Alarm_LL_Real_Time.h"
#include "LTE_PM_DATA_CASE.h"											//模板【小区、性能】


// CAlarm_LL_Real

IMPLEMENT_DYNCREATE(CAlarm_LL_Real, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Real::CAlarm_LL_Real()
	: CFormView(CAlarm_LL_Real::IDD)
{
	v_bShow=false;														//
	v_sCell_All="";														//选择的小区[小区模板]
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Real::~CAlarm_LL_Real()
{
	v_mAlarm_LL_Real_Down.clear();										//容器清除
	v_mAlarm_LL_Real_Up.clear();										//容器清除
	v_mTable_Name.clear();												//容器清除
	v_mCell.clear();													//容器清除
	v_mCell_ID.clear();													//容器清除
	v_mLTE_PM_Type.clear();												//容器清除

	delete m_cChartView.getChart();										//释放资源
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_LIST3, m_cList);
	DDX_Control(pDX, IDC_QUARY2, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel);
	DDX_Control(pDX, IDC_QUARY3, m_cTime);
	DDX_Control(pDX, IDC_LOG_SUM, m_cLog_Sum);
	DDX_Control(pDX, IDC_KEY3, m_cLog_Key);
	DDX_Control(pDX, IDC_BUTTON1, m_cCell);
	DDX_Control(pDX, IDC_TIME, m_cTime_Real);
	DDX_Control(pDX, IDC_LIST1, m_cList_Cell);
	DDX_Control(pDX, IDC_DRAW_AAA, m_cDraw_AAA);
	DDX_Control(pDX, IDC_CHARTVIEW2, m_cChartView);
	DDX_Control(pDX, IDC_CHECK3, m_cAuto);
	DDX_Control(pDX, IDC_CHECK14, m_cOnline);
	DDX_Control(pDX, IDC_CK_DAY, m_cDay);
	DDX_Control(pDX, IDC_YSCALE2, m_cPoint);
	DDX_Control(pDX, IDC_COMBO1, m_cDay_Sum);
	DDX_Control(pDX, IDC_COMBO3, m_cPara);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAlarm_LL_Real, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()

	//Chart
	ON_WM_MOUSEWHEEL()
	ON_CONTROL(CVN_ViewPortChanged, IDC_CHARTVIEW2, OnViewPortChanged)
	ON_CONTROL(CVN_MouseMovePlotArea, IDC_CHARTVIEW2, OnMouseMovePlotArea)
	ON_MESSAGE(123,&CAlarm_LL_Real::Message_ChartView)					//添加消息映射 

	ON_BN_CLICKED(IDC_EXCEL2, &CAlarm_LL_Real::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CAlarm_LL_Real::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CAlarm_LL_Real::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_QUARY2, &CAlarm_LL_Real::OnBnClickedQuary2)
	ON_BN_CLICKED(IDC_QUARY3, &CAlarm_LL_Real::OnBnClickedQuary3)
	ON_BN_CLICKED(IDC_BUTTON1, &CAlarm_LL_Real::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_EXCEL4, &CAlarm_LL_Real::OnBnClickedExcel4)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CAlarm_LL_Real::OnNMClickList1)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAlarm_LL_Real::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CAlarm_LL_Real::OnCbnSelchangeCombo3)
END_MESSAGE_MAP()


// CAlarm_LL_Real 诊断

#ifdef _DEBUG
void CAlarm_LL_Real::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAlarm_LL_Real::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAlarm_LL_Real 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CString v_sTemp;													//临时变量

	//窗体名称
	v_sFrame_Name="LTE 低流量小区实时";									//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗【小区】
	::SendMessage(m_cList_Cell.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Cell.InsertColumn(0,"序号",LVCFMT_CENTER,50);
	m_cList_Cell.InsertColumn(1,"小区",LVCFMT_LEFT,300);  
	m_cList_Cell.InsertColumn(2,"小区ID",LVCFMT_LEFT,0);  
	m_cList_Cell.SetExtendedStyle(m_cList_Cell.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT);

	//初始化：列表窗 [低流量]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"小区",LVCFMT_LEFT,200);						//第1列：小区
	m_cList.InsertColumn(2,"小区ID",LVCFMT_LEFT,0);						//第2列：小区ID
	m_cList.InsertColumn(3,"营业区",LVCFMT_LEFT,60);					//第3列：营业区
	m_cList.InsertColumn(4,"状态",LVCFMT_CENTER,40);					//第4列：状态
	m_cList.InsertColumn(5,"类型",LVCFMT_CENTER,60);					//第5列：类型
	m_cList.InsertColumn(6,"日期",LVCFMT_CENTER,120);					//第6列：日期
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//分窗 [先生成左边的上下关系的水平分隔符，再生成左右关系的垂直分隔符，即2*1]
	CRect	v_cRect;													//矩形
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//获得：指针
	pWnd->GetWindowRect(&v_cRect);										//获得：尺寸
	ScreenToClient(&v_cRect);											//坐标变换
	pWnd->DestroyWindow();												//销毁：指针

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Left.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList,				// Left pane
		&m_cDraw_AAA,			// Right pane
		IDC_ALARM_LL_REAL_1,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);
	m_SplitterPane_Left.m_bHorizSplitter_Parent=true;					//垂直分隔符为父窗
	m_SplitterPane_Main.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_cList_Cell,			// Left pane
		&m_SplitterPane_Left,	// Right pane
		IDC_ALARM_LL_REAL_0,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);

	//初始化：天数选择
	for (int vi=1;vi<=c_iSample_Day;vi++)								//最长天数
	{
		v_sTemp.Format("%d",vi);
		m_cDay_Sum.AddString(v_sTemp);									//可选天数复选钮
	}
	m_cDay_Sum.SetCurSel(7);											//默认查询天数：8天；[一周]

	m_cPara.AddString("1组");											//参数设置
	m_cPara.AddString("2组");
	m_cPara.AddString("3组");
	m_cPara.AddString("4组");
	m_cPara.AddString("5组");
	m_cPara.SetCurSel(0);												//默认

	//Chart初始化
	v_bShow=true;
	GetLocalTime(&v_dChart_Date);										//获得：查询日期
	v_iChart_Day_Sum=m_cDay_Sum.GetCurSel()+1;							//获得：查询天的数量
	CRect v_pRec; 
	m_cChartView.GetWindowRect(&v_pRec); 
	ScreenToClient(&v_pRec); 
	m_cChartView.v_cUI_Data.v_iChart_X0=v_pRec.left;
	m_cChartView.v_cUI_Data.v_iChart_Y0=v_pRec.top;						//起点
	// Initialize the CChartViewer
	m_cChartView.getDefaultBgColor();									//设置默认背景色
	OnChart_Para();														//传递：绘图参数
	m_cChartView.initChartViewer(&m_cChartView);						//初始化Chart
	// Trigger the ViewPortChanged event to draw the chart
	m_cChartView.updateViewPort(true, true);							//绘图：Chart

	//刷新界面
	PostMessage(WM_SIZE);

	//定时设置
	SetTimer(1,60000,NULL);												//定时：1 mins 
	m_cAuto.SetCheck(true);												//有效：自动刷新

	//信息初始化
	v_iList_Item=-1;													//条目选择无效
	v_iList_Item_Cell=-1;												//条目选择无效
	v_dPM_Date=COleDateTime::GetCurrentTime();							//默认：当前时间

	//设置默认：查询参数
	v_bDown=false;														//无效
	v_bUp=false;														//无效
	for(int vi=0;vi<4;vi++)
	{
		//下行
		v_dDown_Select[vi].v_bFlag=false;								//无效
		v_dDown_Select[vi].v_iCal=0;									//<
		v_dDown_Select[vi].v_dDate=COleDateTime::GetCurrentTime();		//当前时间
		v_dDown_Select[vi].v_dValues=100;								//值
		v_dDown_Select[vi].v_iHour=8;									//8点
		//上行
		v_dUp_Select[vi].v_bFlag=false;									//无效
		v_dUp_Select[vi].v_iCal=0;										//<
		v_dUp_Select[vi].v_dDate=COleDateTime::GetCurrentTime();		//当前时间
		v_dUp_Select[vi].v_dValues=100;									//值
		v_dUp_Select[vi].v_iHour=8;										//8点
	}

	//获取：性能粒度
	v_dPM_Date=My_LoadData_Time();										//获取：性能时间
	v_sTemp=v_dPM_Date.Format("%Y-%m-%d_%H:%S:%S");						//字符串[小时粒度]
	m_cTime_Real.SetWindowText(v_sTemp);								//显示：性能时间

	//获取：性能参数
	v_iWorking=2;														//工作任务：获取初始数据
	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CAlarm_LL_Real::PreTranslateMessage(MSG* pMsg)
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
void CAlarm_LL_Real::OnDestroy()
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
void CAlarm_LL_Real::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd	*v_pWnd;													//调整控件的指针

	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//分窗尺寸调整
		CRect v_cRect;
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=45;
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}

	//Chart
	if (v_bShow)														//Chart绘制
	{
		CRect lpRec; 
		m_cDraw_AAA.GetWindowRect(&lpRec); 
		ScreenToClient(&lpRec); 
		m_cChartView.v_cUI_Data.v_iChart_X0=lpRec.left;
		m_cChartView.v_cUI_Data.v_iChart_Y0=lpRec.top;					//起点
		v_pWnd = GetDlgItem(IDC_CHARTVIEW2);							//绘图
		if(v_pWnd)
			v_pWnd->MoveWindow(lpRec.left,lpRec.top,10,10);				//定宽、定长

		GetClientRect(&lpRec); 
		m_cChartView.v_cUI_Data.v_iChart_X=lpRec.right;					//窗口宽
		m_cChartView.v_cUI_Data.v_iChart_Y=lpRec.bottom;				//窗口高

		m_cChartView.updateViewPort(true, false);						//绘图Chart
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：定时器
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString			v_sTemp;											//临时变量
	COleDateTime	v_dTime;											//临时变量

	if (m_cAuto.GetCheck())												//自动刷新？？？
	{
		v_dTime=My_LoadData_Time();										//获取：性能粒度
		if (v_dTime!=v_dPM_Date)										//性能粒度变化？？？
		{
			v_dPM_Date=v_dTime;											//获取性能时间
			OnBnClickedQuary2();										//性能查询
		}
	}

	CFormView::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：更新ChartView
//	工    作：【(1)在线程，性能数据获取；(2)发送消息；(3)消息响应，在用户界面绘制曲线；】
//------------------------------------------------------------------------------------------------------
LRESULT CAlarm_LL_Real::Message_ChartView(WPARAM iParam1,LPARAM iParam2)
{
	//绘图：Chart
	m_cChartView.updateViewPort(true, false);							//图形绘制

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导出
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：参数组选择
//	工	  作：依据选择的参数组号，获取低流量参数；
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sSql,v_sTemp_1;										//临时变量
	int			v_iPara,v_iTime,v_iNum;									//临时变量
	CString		v_sTemp;												//临时变量
	COleDateTime v_tDate;												//临时变量

	int			v_iDown=0,v_iDown_Time[4];								//粒度
	int			v_iUp=0,v_iUp_Time[4];									//粒度
	double		v_dDown_Value[4],v_dUp_Value[4];						//数值

	//初始化
	for (v_iTime=0;v_iTime<4;v_iTime++)									//遍历：粒度
	{
		//读取参数设置表
		v_iDown_Time[v_iTime]=0;										//初始化
		v_iUp_Time[v_iTime]=0;											//初始化
		v_dDown_Value[v_iTime]=0;										//初始化
		v_dUp_Value[v_iTime]=0;											//初始化

		//查询性能数据表
		v_dDown_Select[v_iTime].v_bFlag=false;							//选择：无效
		v_dDown_Select[v_iTime].v_dValues=0;							//数值：复位
		v_dUp_Select[v_iTime].v_bFlag=false;							//选择：无效
		v_dUp_Select[v_iTime].v_dValues=0;								//数值：复位
	}

	//获取：参数组
	v_iPara=m_cPara.GetCurSel()+1;										//获取：参数组

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		//连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//查询：参数，依据登陆帐号+参数组号；
		v_sTemp.Format("%d",v_iPara);									//字符串
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select * from CON_Pm_LTE_LL where zAccount='"+theApp.a_sUser_Account+"' and zPara="+v_sTemp;	//查询数据库

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		if(!v_pRecordset->adoEOF)
		{
			//获取：下行流量参数
			v_iDown=v_cAdoConn.GetField_Int("zDown");					//zDown
			v_iDown_Time[0]=v_cAdoConn.GetField_Int("zDown_Time_1");	//zDown_Time_1
			v_iDown_Time[1]=v_cAdoConn.GetField_Int("zDown_Time_2");	//zDown_Time_2
			v_iDown_Time[2]=v_cAdoConn.GetField_Int("zDown_Time_3");	//zDown_Time_3
			v_iDown_Time[3]=v_cAdoConn.GetField_Int("zDown_Time_4");	//zDown_Time_4
			v_dDown_Value[0]=v_cAdoConn.GetField_Double("zDown_Value_1");//zDown_Value_1
			v_dDown_Value[1]=v_cAdoConn.GetField_Double("zDown_Value_2");//zDown_Value_2
			v_dDown_Value[2]=v_cAdoConn.GetField_Double("zDown_Value_3");//zDown_Value_3
			v_dDown_Value[3]=v_cAdoConn.GetField_Double("zDown_Value_4");//zDown_Value_4

			//获取：上行流量参数
			v_iUp=v_cAdoConn.GetField_Int("zUp");						//zUp
			v_iUp_Time[0]=v_cAdoConn.GetField_Int("zUp_Time_1");		//zUp_Time_1
			v_iUp_Time[1]=v_cAdoConn.GetField_Int("zUp_Time_2");		//zUp_Time_2
			v_iUp_Time[2]=v_cAdoConn.GetField_Int("zUp_Time_3");		//zUp_Time_3
			v_iUp_Time[3]=v_cAdoConn.GetField_Int("zUp_Time_4");		//zUp_Time_4
			v_dUp_Value[0]=v_cAdoConn.GetField_Double("zUp_Value_1");	//zUp_Value_1
			v_dUp_Value[1]=v_cAdoConn.GetField_Double("zUp_Value_2");	//zUp_Value_2
			v_dUp_Value[2]=v_cAdoConn.GetField_Double("zUp_Value_3");	//zUp_Value_3
			v_dUp_Value[3]=v_cAdoConn.GetField_Double("zUp_Value_4");	//zUp_Value_4
		}
		//断开数据库
		v_cAdoConn.ExitConnect();										//断开连接

		//参数处理
		v_bDown=false;													//下行无效
		v_bUp=false;													//上行无效
		for (v_iTime=0;v_iTime<4;v_iTime++)								//遍历：粒度
		{
			//下行
			if (v_iDown_Time[v_iTime])									//有效：粒度？？？
			{
				v_bDown=true;											//下行有效

				v_dDown_Select[v_iTime].v_bFlag=true;					//选中
				v_dDown_Select[v_iTime].v_dValues=v_dDown_Value[v_iTime];	//数值
				
				//设置：运算符、粒度；
				if(v_iTime==0)											//当前粒度
				{
					v_tDate=v_dPM_Date;									//获取：采集粒度
					v_dDown_Select[v_iTime].v_iCal=0;					//小于
				}
				else if(v_iTime==1)										//上周粒度
				{
					v_tDate=v_dPM_Date-COleDateTimeSpan(7,0,0,0);		//获取：上周粒度
					v_dDown_Select[v_iTime].v_iCal=1;					//大于
				}
				else if(v_iTime==2)										//昨天粒度
				{
					v_tDate=v_dPM_Date-COleDateTimeSpan(1,0,0,0);		//获取：昨天粒度
					v_dDown_Select[v_iTime].v_iCal=1;					//大于
				}
				else													//前一粒度
				{
					v_tDate=v_dPM_Date-COleDateTimeSpan(0,1,0,0);		//获取：前一粒度
					v_dDown_Select[v_iTime].v_iCal=1;					//大于
				}
				v_dDown_Select[v_iTime].v_dDate.SetDateTime(v_tDate.GetYear(),v_tDate.GetMonth(),v_tDate.GetDay(),0,0,0);	//日期
				v_dDown_Select[v_iTime].v_iHour=v_tDate.GetHour();		//小时
			}
			//上行
			if (v_iUp_Time[v_iTime])									//有效：粒度？？？
			{
				v_bUp=true;												//上行有效

				v_dUp_Select[v_iTime].v_bFlag=true;						//选中
				v_dUp_Select[v_iTime].v_dValues=v_dUp_Value[v_iTime];	//数值

				//设置：运算符、粒度；
				if(v_iTime==0)											//当前粒度
				{
					v_tDate=v_dPM_Date;									//获取：采集粒度
					v_dUp_Select[v_iTime].v_iCal=0;						//小于
				}
				else if(v_iTime==1)										//上周粒度
				{
					v_tDate=v_dPM_Date-COleDateTimeSpan(7,0,0,0);		//获取：上周粒度
					v_dUp_Select[v_iTime].v_iCal=1;						//大于
				}
				else if(v_iTime==2)										//昨天粒度
				{
					v_tDate=v_dPM_Date-COleDateTimeSpan(1,0,0,0);		//获取：昨天粒度
					v_dUp_Select[v_iTime].v_iCal=1;						//大于
				}
				else													//前一粒度
				{
					v_tDate=v_dPM_Date-COleDateTimeSpan(0,1,0,0);		//获取：前一粒度
					v_dUp_Select[v_iTime].v_iCal=1;						//大于
				}
				v_dUp_Select[v_iTime].v_dDate.SetDateTime(v_tDate.GetYear(),v_tDate.GetMonth(),v_tDate.GetDay(),0,0,0);	//日期
				v_dUp_Select[v_iTime].v_iHour=v_tDate.GetHour();		//小时
			}
		}

		
		//调整：列信息【选择的粒度】
		m_cList.DeleteAllItems();										//删除列表窗信息
		while(m_cList.DeleteColumn(0));									//删除所有列
		m_cSum.SetWindowText("");										//清除总数

		//默认列信息
		m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);				//
		m_cList.InsertColumn(1,"小区",LVCFMT_LEFT,200);					//第1列：小区
		m_cList.InsertColumn(2,"小区ID",LVCFMT_LEFT,0);					//第2列：小区ID
		m_cList.InsertColumn(3,"营业区",LVCFMT_LEFT,60);				//第3列：营业区
		m_cList.InsertColumn(4,"状态",LVCFMT_CENTER,40);				//第4列：状态
		m_cList.InsertColumn(5,"类型",LVCFMT_CENTER,60);				//第5列：类型

		v_iNum=6;														//列序号
		//下行有效
		if (v_bDown)													//有效：下行？？？
		{
			for (v_iTime=0;v_iTime<4;v_iTime++)							//遍历：粒度
			{
				if (v_dDown_Select[v_iTime].v_bFlag)					//有效：粒度？？？
				{
					v_sTemp=v_dDown_Select[v_iTime].v_dDate.Format("%Y-%m-%d");	//日期
					v_sTemp_1.Format("%02d",v_dDown_Select[v_iTime].v_iHour);	//小时
					v_sTemp="下行_"+v_sTemp+"_"+v_sTemp_1;						//列名称
					m_cList.InsertColumn(v_iNum,v_sTemp,LVCFMT_RIGHT,130);		//设置：列名称
					m_cList.SetNumber(v_iNum);							//设置：数字形式
					v_iNum++;											//计数器
				}
			}
		}

		//上行有效
		if (v_bUp)														//有效：上行？？？
		{
			for (v_iTime=0;v_iTime<4;v_iTime++)							//遍历：粒度
			{
				if (v_dUp_Select[v_iTime].v_bFlag)						//有效：粒度？？？
				{
					v_sTemp=v_dUp_Select[v_iTime].v_dDate.Format("%Y-%m-%d");	//日期
					v_sTemp_1.Format("%02d",v_dUp_Select[v_iTime].v_iHour);		//小时
					v_sTemp="上行_"+v_sTemp+"_"+v_sTemp_1;						//列名称
					m_cList.InsertColumn(v_iNum,v_sTemp,LVCFMT_RIGHT,130);		//设置：列名称
					m_cList.SetNumber(v_iNum);							//设置：数字形式
					v_iNum++;											//计数器
				}
			}
		}
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：时间参数设置
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnBnClickedQuary3()
{
	// TODO: 在此添加控件通知处理程序代码
	CAlarm_LL_Real_Time	v_cDlg;											//实例化

	if(v_cDlg.DoModal()==IDOK)											//确认返回？？？
	{
		OnCbnSelchangeCombo3();											//参数组选择
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表点击响应[小区]
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item_Cell>=0)									//有效条目序号？？？
				m_cList_Cell.SetItemBkColor(v_iList_Item_Cell,RGB(255,255,255));	//恢复背景色
			v_iList_Item_Cell=pNMListView->iItem;						//获得：选中条目序号
			m_cList_Cell.SetItemBkColor(v_iList_Item_Cell,RGB(255,0,0));//设置背景色：红色

			//绘制小区流量曲线
			v_iWorking=4;												//工作任务：单线绘制
			v_pIterator_Frame->second.v_iThread_Exit=1;					//线程：工作
			v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);		//开启查询数据库线程
			v_bSelect=true;												//标记：true：小区；false：低流量；
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表点击响应[低流量小区]
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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

			//绘制小区流量曲线
			v_iWorking=4;												//工作任务：单线绘制
			v_pIterator_Frame->second.v_iThread_Exit=1;					//线程：工作
			v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);		//开启查询数据库线程
			v_bSelect=false;											//标记：true：小区；false：低流量；
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：小区查询
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=3;														//工作任务：小区查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：小区模板【获取：小区信息，放入m_cList_Cell】
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnBnClickedExcel4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp;												//临时变量
	int			v_iItem,v_iID=0;										//临时变量

	LTE_PM_DATA_CASE	v_Dlg;											//实例化
	v_Dlg.v_iSelect=0;													//标志：小区

	if (v_Dlg.DoModal()==IDOK)											//确认返回？？？
	{
		//选择结束：小区信息在容器中(v_Dlg.v_mCell);
		//工作：【1】获取小区；【2】放入v_mList，加入Cell_ID；
		m_cList_Cell.DeleteAllItems();									//删除：模板信息
		My_Chart_New();													//绘图复位
		v_iList_Item_Cell=-1;											//复位：条目序号

		mString_String::iterator	v_pIter_Cell;						//迭代器
		set<CString>::iterator		v_pIter;							//迭代器

		v_sCell_All="";													//复位：选择的小区
		//遍历容器
		for(v_pIter=v_Dlg.v_mCell.begin();v_pIter!=v_Dlg.v_mCell.end();v_pIter++)	//遍历：模板信息列表
		{
			//有效小区
			v_pIter_Cell=v_mCell.find(*v_pIter);						//查找：小区存在？？？
			if (v_pIter_Cell==v_mCell.end())							//不存在
				continue;												//下一小区
	
			//选择的小区
			if (!v_sCell_All.IsEmpty())
				v_sCell_All+=",";										//分隔符
			v_sCell_All+=v_pIter_Cell->second;							//增加小区

			//放入列表
			v_sTemp.Format(" %05d",v_iID+1);
			v_iItem=m_cList_Cell.InsertItem(0xffff,v_sTemp);			//加入列表窗
			m_cList_Cell.SetItemText(v_iItem,1,v_pIter_Cell->first);	//显示：小区
			m_cList_Cell.SetItemText(v_iItem,2,v_pIter_Cell->second);	//显示：小区ID
			v_iID++;
		}																//
		v_sTemp.Format("%d",v_iID);										//
		m_cLog_Sum.SetWindowText(v_sTemp);								//显示：Log总数
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnBnClickedQuary2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString			v_sTemp;											//临时变量
	COleDateTime	v_dTime;											//临时变量

	//【1】获取：查询粒度
	v_dPM_Date=My_LoadData_Time();										//获取：性能粒度
	v_sTemp=v_dPM_Date.Format("%Y-%m-%d_%H:%S:%S");						//字符串[小时粒度]
	m_cTime_Real.SetWindowText(v_sTemp);								//显示：性能时间

	//【2】查询：低流量
	v_iWorking=1;														//工作任务：查询
	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CAlarm_LL_Real::My_Thread_Inoput(LPVOID lparam)
{
	CAlarm_LL_Real *  lp_this = NULL ;
	lp_this = (CAlarm_LL_Real *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效
	lp_this->m_cTime.EnableWindow(false);								//时间设置：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效
	lp_this->m_cTime.EnableWindow(true);								//时间设置：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Per();												//低流量小区查询
		break;
	case 2:
		My_LoadData_Para();												//获取：性能参数
		break;
	case 3:
		My_LoadData_Cell();												//获取：小区信息
		break;
	case 4:
		My_Chart();														//单线绘制
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询小区流量信息
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_LoadData_Per()
{
	bool		v_bOK=false,v_bCell_Down=false,v_bCell_Up=false;		//临时变量
	CString		v_sTable,v_sTemp;										//临时变量
	int			v_iTime,v_iNum,v_iID=0;									//临时变量

	//【1】初始化
	My_Chart_New();														//绘图复位
	v_iList_Item=-1;													//无选择项
	m_cList.DeleteAllItems();											//删除列表窗信息
	v_mAlarm_LL_Real_Down.clear();										//容器清除
	v_mAlarm_LL_Real_Up.clear();										//容器清除
	m_cSum.SetWindowText("");											//清除总数

	if (!v_bDown && !v_bUp)												//无效粒度选择？？？
	{
		MessageBox("  请选择有效的性能 !!! ");							//窗体提示
		return;
	}

	//【2】获取：性能数据【由于各种查询都是或的关系，所以第一次查询是增加，以后查询都是剔除；】
	for (v_iTime=0;v_iTime<4;v_iTime++)									//遍历：粒度
	{
		//下行
		if (v_dDown_Select[v_iTime].v_bFlag)							//有效：粒度？？？
		{
			v_bOK=true;													//设置有效：粒度选择

			//【3】获取：表名称
			v_sTable=v_dDown_Select[v_iTime].v_dDate.Format("%Y%m");	//年月
			v_sTable="Pm_LTE_Cal_"+v_sTable+"_"+My_Get_Table_Num(v_dDown_Select[v_iTime].v_dDate.GetDay());	//文件名称(表序号)

			//【4】检查：表名称是否有效【依据：表名称集合v_mTable_Name】【如果表名称无效，则查不出性能数据。】
			set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//查找？？
			if (v_pTable==v_mTable_Name.end())							//没找到：数据表
			{
				v_sTable=v_dDown_Select[v_iTime].v_dDate.Format("%Y%m%d");	//年月日
				MessageBox("  请选择有效的粒度："+v_sTable);			//窗体提示
				break;
			}
			else
			{
				My_LoadData_PM_Data(v_iTime,1,v_sTable,v_bCell_Down,&v_mAlarm_LL_Real_Down);	//性能获取
				v_bCell_Down=true;										//设置：第一次获取有效
			}
		}
		//上行
		if (v_dUp_Select[v_iTime].v_bFlag)								//有效：粒度？？？
		{
			v_bOK=true;													//设置有效：粒度选择

			//【3】获取：表名称
			v_sTable=v_dUp_Select[v_iTime].v_dDate.Format("%Y%m");		//年月
			v_sTable="Pm_LTE_Cal_"+v_sTable+"_"+My_Get_Table_Num(v_dUp_Select[v_iTime].v_dDate.GetDay());	//文件名称(表序号)

			//【4】检查：表名称是否有效【依据：表名称集合v_mTable_Name】【如果表名称无效，则查不出性能数据。】
			set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//查找？？
			if (v_pTable==v_mTable_Name.end())							//没找到：数据表
			{
				v_sTable=v_dUp_Select[v_iTime].v_dDate.Format("%Y%m%d");	//年月日
				MessageBox("  请选择有效的粒度："+v_sTable);			//窗体提示
				break;
			}
			else
			{
				My_LoadData_PM_Data(v_iTime,0,v_sTable,v_bCell_Up,&v_mAlarm_LL_Real_Up);	//性能获取
				v_bCell_Up=true;										//设置：第一次获取有效
			}
		}
	}
	if (!v_bOK)															//无效粒度选择？？？
	{
		MessageBox("  请选择有效的粒度 !!! ");							//窗体提示
		return;
	}

	//【4】下行、上行数据合并：或处理[即：满足上行、下行任意一个就显示][把上行数据合并到下行数据里]
	Alarm_LL_PM_DATA		v_cPM_DATA;									//结构
	map_Alarm_LL_PM_DATA::iterator	v_pPM_Cell,v_pPM_Cell_Up;			//迭代器
	for (v_pPM_Cell_Up=v_mAlarm_LL_Real_Up.begin();v_pPM_Cell_Up!=v_mAlarm_LL_Real_Up.end();v_pPM_Cell_Up++)	//遍历：上行小区
	{
		//下行：上行小区不存在，则增加；
		v_pPM_Cell=v_mAlarm_LL_Real_Down.find(v_pPM_Cell_Up->first);	//查询：上行小区不存在？？？
		if(v_pPM_Cell==v_mAlarm_LL_Real_Down.end())						//增加
		{
			v_cPM_DATA.v_sOnline=v_pPM_Cell_Up->second.v_sOnline;		//状态
			v_cPM_DATA.v_sRegion=v_pPM_Cell_Up->second.v_sRegion;		//区域
			v_cPM_DATA.v_sCell_ID=v_pPM_Cell_Up->second.v_sCell_ID;		//小区ID
			v_cPM_DATA.v_sType="上行";									//类型
			for(int vi=0;vi<4;vi++)										//数据初始化
			{
				v_cPM_DATA.v_dD_Values[vi]=-1;							//下行
				v_cPM_DATA.v_dU_Values[vi]=-1;							//上行
			}
			v_mAlarm_LL_Real_Down.insert(pair<CString,Alarm_LL_PM_DATA>(v_pPM_Cell_Up->first,v_cPM_DATA));		//容器增加
			v_pPM_Cell=v_mAlarm_LL_Real_Down.find(v_pPM_Cell_Up->first);//查询：上行小区
		}
		else
			v_pPM_Cell->second.v_sType="上、下行";						//类型

		//下行：上行小区存在，则补充下行数据；
		for (v_iTime=0;v_iTime<4;v_iTime++)								//遍历：粒度
		{
			if (v_dUp_Select[v_iTime].v_bFlag)							//有效：粒度？？？
				v_pPM_Cell->second.v_dU_Values[v_iTime]=v_pPM_Cell_Up->second.v_dU_Values[v_iTime];		//数据复制
		}
		
	}

	//【5】显示：性能数据[基于上、下行合并的数据]
	for (v_pPM_Cell=v_mAlarm_LL_Real_Down.begin();v_pPM_Cell!=v_mAlarm_LL_Real_Down.end();v_pPM_Cell++)	//遍历：小区
	{
		if (!m_cOnline.GetCheck())										//工程无效？？？
		{
			if (v_pPM_Cell->second.v_sOnline != "在网")					//工程：或略 [不监控]
				continue;
		}
		int v_iIndex = m_cList.InsertItem(0xffffff,_T("0"));
		v_sTemp.Format("%06d",v_iIndex+1);
		m_cList.SetItemText(v_iIndex,0,v_sTemp);						//序号
		m_cList.SetItemText(v_iIndex,1,v_pPM_Cell->first);				//Cell
		m_cList.SetItemText(v_iIndex,2,v_pPM_Cell->second.v_sCell_ID);	//Cell_ID
		m_cList.SetItemText(v_iIndex,3,v_pPM_Cell->second.v_sRegion);	//营业区
		m_cList.SetItemText(v_iIndex,4,v_pPM_Cell->second.v_sOnline);	//状态
		m_cList.SetItemText(v_iIndex,5,v_pPM_Cell->second.v_sType);		//类型
		v_iNum=6;														//列序号
		if (v_bDown)													//有效：下行？？？
		{
			for (v_iTime=0;v_iTime<4;v_iTime++)							//遍历：粒度
			{
				if (v_dDown_Select[v_iTime].v_bFlag)					//有效：粒度？？？
				{
					v_sTemp.Format("%8.2f",v_pPM_Cell->second.v_dD_Values[v_iTime]);	//字符串
					v_sTemp.Trim();
					if (v_sTemp=="-1.00") 
						v_sTemp="";
					m_cList.SetItemText(v_iIndex,v_iNum,v_sTemp);		//性能数值
					v_iNum++;											//计数器
				}
			}
		}
		//上行有效
		if (v_bUp)														//有效：上行？？？
		{
			for (v_iTime=0;v_iTime<4;v_iTime++)							//遍历：粒度
			{
				if (v_dUp_Select[v_iTime].v_bFlag)						//有效：粒度？？？
				{
					v_sTemp.Format("%8.2f",v_pPM_Cell->second.v_dU_Values[v_iTime]);	//字符串
					v_sTemp.Trim();
					if (v_sTemp=="-1.00") 
						v_sTemp="";
					m_cList.SetItemText(v_iIndex,v_iNum,v_sTemp);		//性能数值
					v_iNum++;											//计数器
				}
			}
		}
	
		v_iID++;														//计数器
		if (v_iID%100==0)
		{
			v_sTemp.Format("%d",v_iID);									//
			m_cSum.SetWindowText(v_sTemp);								//显示：总数
		}
	}
	v_sTemp.Format("%d",v_iID);											//
	m_cSum.SetWindowText(v_sTemp);										//显示：Log总数
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取性能数据
//	参    数：v_iTime：粒度序号；	v_iType：(1：下行；0：上行；)	v_sTable：表名称；	v_bCell：有效/无效；	v_mAlarm_LL_Real：容器指针；
//	结    果：性能数据放入容器
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_LoadData_PM_Data(int v_iTime,int v_iType,CString v_sTable,bool v_bCell,map_Alarm_LL_PM_DATA* v_mAlarm_LL_Real)
{
	CString			v_sSql,v_sPM_Cell,v_sRegion,v_sOnline;				//临时变量
	CString			v_sTemp;											//临时变量
	COleDateTime	v_tDate;											//日期
	double			v_dValue;											//性能数值
	int				v_iCell_ID;											//临时变量

	_variant_t		Var;

	Alarm_LL_PM_DATA		v_cPM_DATA;									//结构

	map_Cell_ID::iterator	v_pIter_Cell;								//迭代器
	map_Alarm_LL_PM_DATA::iterator	v_pPM_Cell;							//迭代器

	CString		v_sField,v_sValue,v_sTime,v_sCal;						//临时变量


	//【1】初始化
	if(v_iType)															//下行
	{
		v_sField = "下行流量(M)";										//字段名称
		v_sValue.Format("%12.4f",v_dDown_Select[v_iTime].v_dValues);	//数值(含计算公式)
		v_sTime=v_dDown_Select[v_iTime].v_dDate.Format("%Y%m%d");		//日期
		v_sTemp.Format("%02d:00:00",v_dDown_Select[v_iTime].v_iHour);	//小时
		v_sTime+=" "+v_sTemp;											//粒度时间
		v_sCal=(v_dDown_Select[v_iTime].v_iCal==0)?"<":((v_dDown_Select[v_iTime].v_iCal==1)?">":"=");	//比较符
	}
	else
	{
		v_sField = "上行流量(M)";										//字段名称
		v_sValue.Format("%12.4f",v_dUp_Select[v_iTime].v_dValues);		//数值(含计算公式)
		v_sTime=v_dUp_Select[v_iTime].v_dDate.Format("%Y%m%d");			//日期
		v_sTemp.Format("%02d:00:00",v_dUp_Select[v_iTime].v_iHour);		//小时
		v_sTime+=" "+v_sTemp;											//粒度时间
		v_sCal=(v_dUp_Select[v_iTime].v_iCal==0)?"<":((v_dUp_Select[v_iTime].v_iCal==1)?">":"=");	//比较符
	}
	v_sValue.Trim();													//去掉空格

	//【2】做标记 【从第二次查询起，删除与前次不匹配的小区。】
	if (v_bCell)														//大于一次查询
	{
		for (v_pPM_Cell=v_mAlarm_LL_Real->begin();v_pPM_Cell!=v_mAlarm_LL_Real->end();v_pPM_Cell++)	//遍历：小区
			v_pPM_Cell->second.v_bFlag=false;							//初始化
	}

	//【3】性能数据获取
	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(10*60);							//超时设置(10分钟)

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zTime,["+v_sField+"] from "+v_sTable;		//SQL
			
		if (v_sCell_All.IsEmpty())										//所有小区？？？
			v_sSql+=" where ";											//SQL
		else															//选择的小区
			v_sSql+=" where zCell in("+v_sCell_All+") and ";			//SQL
		
		v_sSql+="zTime='"+v_sTime+"' and ["+v_sField+"]"+v_sCal+v_sValue;	//SQL

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_iCell_ID=Var.intVal;									//获取：Cell_ID
				v_pIter_Cell=v_mCell_ID.find(v_iCell_ID);				//查找：小区ID存在？？？
				if (v_pIter_Cell==v_mCell_ID.end())						//不存在
				{
					v_pRecordset->MoveNext();							//下一小区性能
					continue;											//继续
				}
				v_sPM_Cell=v_pIter_Cell->second.v_sCell;				//小区
				v_sOnline=v_pIter_Cell->second.v_sOnline;				//状态
				v_sRegion=v_pIter_Cell->second.v_sRegion;				//营业区
			}
			else
				v_sPM_Cell="";

			Var=v_pRecordset->GetCollect((_variant_t)v_sField);			//获取：性能数值
			if (Var.vt != VT_NULL)
				v_dValue=Var.dblVal;
			else
				v_dValue=0;

			if (!v_bCell)												//第一次查询？？？
			{															//增加
				v_pPM_Cell=v_mAlarm_LL_Real->find(v_sPM_Cell);			//查找：小区 ？？？
				if (v_pPM_Cell==v_mAlarm_LL_Real->end())				//不存在
				{
					v_cPM_DATA.v_sOnline=v_sOnline;						//状态
					v_cPM_DATA.v_sRegion=v_sRegion;						//区域
					v_cPM_DATA.v_sType="下行";							//类型
					for(int vi=0;vi<4;vi++)								//数据初始化
					{
						v_cPM_DATA.v_dD_Values[vi]=-1;					//下行
						v_cPM_DATA.v_dU_Values[vi]=-1;					//上行
					}
					v_sTemp.Format("%d",v_iCell_ID);					//字符串
					v_cPM_DATA.v_sCell_ID=v_sTemp;						//小区ID
					if (v_iType)										//下行
						v_cPM_DATA.v_dD_Values[v_iTime]=v_dValue;		//性能
					else												//上行
						v_cPM_DATA.v_dU_Values[v_iTime]=v_dValue;		//性能

					v_mAlarm_LL_Real->insert(pair<CString,Alarm_LL_PM_DATA>(v_sPM_Cell,v_cPM_DATA));	//容器增加
				}
			}	
			else
			{
				v_pPM_Cell=v_mAlarm_LL_Real->find(v_sPM_Cell);			//查找：小区 ？？？
				if (v_pPM_Cell!=v_mAlarm_LL_Real->end())				//存在
				{
					v_pPM_Cell->second.v_bFlag=true;					//设置标志：查询匹配
					if (v_iType)										//下行
						v_pPM_Cell->second.v_dD_Values[v_iTime]=v_dValue;		//性能
					else												//上行
						v_pPM_Cell->second.v_dU_Values[v_iTime]=v_dValue;		//性能
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

	//【4】删除不匹配的小区 【从第二次查询起，删除与前次不匹配的小区。】
	if (v_bCell)														//大于一次查询
	{
		v_pPM_Cell=v_mAlarm_LL_Real->begin();							//头指针
		while(v_pPM_Cell!=v_mAlarm_LL_Real->end())						//遍历：小区
		{
			if (!v_pPM_Cell->second.v_bFlag)							//没匹配？？？
				v_pPM_Cell=v_mAlarm_LL_Real->erase(v_pPM_Cell);			//删除小区，返回下一指针
			else
				v_pPM_Cell++;											//指针++
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取表序号
//	工	  作：1：1-5；	2：6-10；	3：11-15；	4：16-20；	5：21-25；	6：26--
//------------------------------------------------------------------------------------------------------			
CString CAlarm_LL_Real::My_Get_Table_Num(int v_iDay)
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
//	函数功能：装载性能参数信息
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_LoadData_Para()
{
	int				v_iID=0,v_iCell_ID;									//临时变量
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


		//【1】获取：性能数据表名称【用于发现无效表名称】
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_LTE_%') order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//表名称

			v_mTable_Name.insert(v_sTable);								//增加集合

			v_pRecordset->MoveNext();									//指针++
		}	

		//【2】获取：小区信息【用于小区ID处理】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zCell_ID,zCell_Name,LTE_Cell.zOnline,LTE_Cell.zRegion \
				from LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB";		//查询数据库
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sCell=v_cAdoConn.GetField_String("zCell");				//zCell
			v_iCell_ID=v_cAdoConn.GetField_Int("zCell_ID");				//zCell_ID
			v_sCell_ID.Format("%d",v_iCell_ID);							//字符串
			v_sCell_Name=v_cAdoConn.GetField_String("zCell_Name");		//zCell_Name
			v_sOnline=v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_sRegion=v_cAdoConn.GetField_String("zRegion");			//zRegion

			v_mCell.insert(pair<CString,CString>(v_sCell,v_sCell_ID));	//容器增加
			v_cCell_ID.v_sCell=v_sCell;									//小区
			v_cCell_ID.v_sCell_Name=v_sCell_Name;						//小区中文名
			v_cCell_ID.v_sOnline=v_sOnline;								//状态
			v_cCell_ID.v_sRegion=v_sRegion;								//营业区
			v_mCell_ID.insert(pair<int,Cell_ID_Struct>(v_iCell_ID,v_cCell_ID));	//容器增加

			v_pRecordset->MoveNext();									//指针++
		}	

		//【3】性能类型：计算性能
		v_sType="计算性能";												//性能类型								
		v_cLTE_PM_Type.v_sTable_Key=v_sType;							//表名称-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//容器增加

		//获取：性能参数【常规】
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
				v_cLTE_PM.v_sPM_Name=v_sPm;								//性能：中文名
				v_cLTE_PM.v_sTable_Key="Cal";							//性能：表名称
				if (v_sPm.Find("流量")>=0)								//流量？？？
					v_cLTE_PM.v_bOK=true;								//性能：选中(需要绘制)
				else
					v_cLTE_PM.v_bOK=false;								//性能：未选中(不需要绘制)

				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//增加：性能参数
			}

			v_pRecordset->MoveNext();									//
		}

		v_cAdoConn.ExitConnect();										//断开连接

		//参数选择
		OnCbnSelchangeCombo3();											//参数选择
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载Cell信息
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_LoadData_Cell()
{
	CString		v_sSql;													//临时变量
	int			v_iItem,v_iID=0,v_iCell_ID;								//临时变量
	CString		v_sTemp,v_sCell,v_sCell_ID;								//临时变量
	CString		v_sLog_Key;												//日志查询：KEY

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList_Cell.DeleteAllItems();									//清除列表数据
		v_iList_Item_Cell=-1;											//默认：无选中的列表Item
		My_Chart_New();													//绘图复位

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

		v_sCell_All="";													//复位：选择的小区

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sCell=v_cAdoConn.GetField_String("zCell");				//zCell
			v_iCell_ID=v_cAdoConn.GetField_Int("zCell_ID");				//zCell_ID

			//选择的小区
			if (!v_sCell_All.IsEmpty())
				v_sCell_All+=",";										//分隔符
			v_sCell_All+=v_sCell_ID;									//增加小区

			//列表窗
			v_sTemp.Format(" %05d",v_iID+1);
			v_iItem=m_cList_Cell.InsertItem(0xffff,v_sTemp);			//加入列表窗
			m_cList_Cell.SetItemText(v_iItem,1,(_bstr_t)v_sCell);		//
			v_sCell_ID.Format("%d",v_iCell_ID);							//字符串
			m_cList_Cell.SetItemText(v_iItem,2,(_bstr_t)v_sCell_ID);	//Cell_ID

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
//	函数功能：获取性能粒度
//------------------------------------------------------------------------------------------------------			
COleDateTime CAlarm_LL_Real::My_LoadData_Time()
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
			v_sTime = v_cAdoConn.GetField_DataTime("zPm_LTE_Time_Data");	//zPm_LTE_Time_Data [性能粒度]
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

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Chart
////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------------
//	函数功能：Draw track cursor when mouse is moving over plotarea
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnMouseMovePlotArea()
{
	// Get the focus to ensure being able to receive mouse wheel events
	m_cChartView.SetFocus();

//	m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),m_cLable.GetCheck()); 
	m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),true); 
	m_cChartView.updateDisplay();
}

//------------------------------------------------------------------------------------------------------
//	函数功能：鼠标轮滑动消息 [用途：放大、缩小、左右平移]
//------------------------------------------------------------------------------------------------------
BOOL CAlarm_LL_Real::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// Process the mouse wheel only if the mouse is over the plot area
	if (!m_cChartView.isMouseOnPlotArea())
		return FALSE;

	// We zoom in or out by 10% depending on the mouse wheel direction.
	double newVpWidth = m_cChartView.getViewPortWidth() * (zDelta > 0 ? 0.7 : 1 / 0.7);
	double newVpHeight = m_cChartView.getViewPortHeight() * (zDelta > 0 ? 0.7 : 1 / 0.7);

	// We do not zoom beyond the zoom width or height limits.
	newVpWidth = max(m_cChartView.getZoomInWidthLimit(), min(newVpWidth,
		m_cChartView.getZoomOutWidthLimit()));
	newVpHeight = max(m_cChartView.getZoomInHeightLimit(), min(newVpWidth,
		m_cChartView.getZoomOutHeightLimit()));

	if ((newVpWidth != m_cChartView.getViewPortWidth()) || 
		(newVpHeight != m_cChartView.getViewPortHeight()))
	{
		// Set the view port position and size so that the point under the mouse remains under
		// the mouse after zooming.

		double deltaX = (m_cChartView.getPlotAreaMouseX() - m_cChartView.getPlotAreaLeft()) * 
			(m_cChartView.getViewPortWidth() - newVpWidth) / m_cChartView.getPlotAreaWidth();
		m_cChartView.setViewPortLeft(m_cChartView.getViewPortLeft() + deltaX);
		m_cChartView.setViewPortWidth(newVpWidth);

		double deltaY = (m_cChartView.getPlotAreaMouseY() - m_cChartView.getPlotAreaTop()) *
			(m_cChartView.getViewPortHeight() - newVpHeight) / m_cChartView.getPlotAreaHeight();
		m_cChartView.setViewPortTop(m_cChartView.getViewPortTop() + deltaY);
		m_cChartView.setViewPortHeight(newVpHeight);

		m_cChartView.updateViewPort(true, false);
	}

	return true;
}

//------------------------------------------------------------------------------------------------------
//	函数功能：View port changed event
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnViewPortChanged()
{
	// Update the chart if necessary
	if (m_cChartView.needUpdateChart())
	{
		OnChart_Para();													//获得：绘图参数
		m_cChartView.drawChart(&m_cChartView);							//绘图
	}

	// We need to update the track line too. If the mouse is moving on the chart (eg. if 
	// the user drags the mouse on the chart to scroll it), the track line will be updated
	// in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
	if ((!m_cChartView.isInMouseMoveEvent()) && m_cChartView.isMouseOnPlotArea())
	{
//		m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),m_cLable.GetCheck());
		m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),true);
		m_cChartView.updateDisplay();
	}
}

//------------------------------------------------------------------------------------------------------
//	函数功能：传递绘图参数  [从屏幕到Chart变量]
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::OnChart_Para()
{
	CString		v_sTemp;												//临时变量

	m_cChartView.v_cUI_Data.v_dDate_Start=COleDateTime::GetCurrentTime()-COleDateTimeSpan(v_iChart_Day_Sum-1,0,0,0);	//获得：查询开始日期
 	m_cChartView.v_cUI_Data.v_iChart_Day_Sum=v_iChart_Day_Sum;			//查询天数
 	m_cChartView.v_cUI_Data.v_iMuti_Line=false;							//多线有效
 	m_cChartView.v_cUI_Data.v_iKPI_S=false;								//静态有效
	m_cChartView.v_cUI_Data.v_iDay=m_cDay.GetCheck();					//单天有效
	m_cChartView.v_cUI_Data.v_iPoint=m_cPoint.GetCheck();				//数据点有效

	m_cChartView.v_cUI_Data.v_sKPI_Name = "小区上下行流量";				//题目名称

	m_cChartView.v_cUI_Data.v_iY_scale=0;								//y轴刻度有效

	m_cChartView.v_cUI_Data.v_iTime=1;									//粒度信息：时
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：清除屏幕显示信息
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_Show_Clear()									//清除屏幕显示信息
{
	v_iList_Item_Cell=-1;												//选中的Item无效
	My_Chart_New();														//绘图复位
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：天数选择
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iChart_Day_Sum=m_cDay_Sum.GetCurSel()+1;							//获得：查询天的数量

	My_Chart_New();														//绘图复位
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：绘图复位
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_Chart_New()
{
	//绘图初始化
	OnChart_Para();														//传递：绘图参数
	m_cChartView.initChartViewer(&m_cChartView);						//初始化Chart
	//绘图：Chart
	m_cChartView.updateViewPort(true, false);							//绘图
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：绘图复位然后找当前节点
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_Chart()
{
	//绘图复位
	My_Chart_New();														//绘图复位

	//绘图：遍历Tree，找到选中的节点	
	if (v_bSelect)														//标记：true：小区；false：低流量；
		My_List_Chart(v_iList_Item_Cell);								//绘图：选中的Item
	else
		My_List_Chart(v_iList_Item);									//绘图：选中的Item
}

//------------------------------------------------------------------------------------------------------
//	函数功能：绘图：选中的Tree Node
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::My_List_Chart(int v_iList_Item)
{
	My_List_Chart_GetNode(v_iList_Item);								//获取：节点信息【保存在public变量中】
	My_Chart_IP();														//绘制：性能信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取节点信息 
//	参    数：v_iList_Item：选中的Item
//			 【用于：绘图、显示节点信息，保存在v_sCell变量中】
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Real::My_List_Chart_GetNode(int v_iList_Item)
{
	if (v_bSelect)														//标记：true：小区；false：低流量；
	{
		v_sCell=m_cList_Cell.GetItemText(v_iList_Item,1);				//Cell名称
		v_sCell_ID=m_cList_Cell.GetItemText(v_iList_Item,2);			//Cell_ID
	}
	else
	{
		v_sCell=m_cList.GetItemText(v_iList_Item,1);					//Cell名称
		v_sCell_ID=m_cList.GetItemText(v_iList_Item,2);					//Cell_ID
	}
}

//------------------------------------------------------------------------------------------------------
//	函数功能：Chart画一条线
//	参    数：选中的 IP [v_sItem_IP]
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::My_Chart_IP()
{
	CString					v_sTemp;									//临时变量
	bool					v_bOK=false;								//临时变量
	mLTE_PM_Type::iterator	v_pSelect;									//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	COleDateTime	v_dDate_Start=COleDateTime::GetCurrentTime()-COleDateTimeSpan(v_iChart_Day_Sum-1,0,0,0);	//查询开始日期：当前时间-查询天数 [默认一周]
	v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//设置时间：00:00:00

	//遍历：性能类型【计算性能、区域性能】
	for (v_pSelect=v_mLTE_PM_Type.begin();v_pSelect!=v_mLTE_PM_Type.end();v_pSelect++)
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//强制退出？？？
			break;														//退出

		if (v_pSelect->first=="计算性能")								//与选择的一致？？？
		{
			//基于选择的性能类型，遍历选择的性能，进行绘图；
			for (v_pPM_Name=v_pSelect->second.v_mLTE_PM.begin();v_pPM_Name!=v_pSelect->second.v_mLTE_PM.end();v_pPM_Name++)
			{
				if (v_pIterator_Frame->second.v_iThread_Exit==2)		//强制退出？？？
					break;												//退出

				//读取性能数据库，并绘图
				if (v_pPM_Name->second.v_bOK)							//性能选中？？？
				{
					My_Load_Performance(v_pSelect->first,				//性能类型
						v_pPM_Name->second.v_sTable_Key,				//性能文件名称key
						v_sCell,										//小区名称
						v_sCell_ID,										//小区ID
						v_pPM_Name->second.v_sPM_Name,					//性能名称
						v_pPM_Name->first,								//性能字段
						v_dDate_Start,									//开始日期
						v_iChart_Day_Sum);								//查询天数

					v_bOK=true;											//标志：绘图有效
				}
			}
		}
	}
	if (!v_bOK)
		MessageBox("  请选择有效的性能 !!! ");							//窗体提示
	else
		::PostMessage(m_hWnd, 123, (WPARAM) 0, (LPARAM) 0);				//发送消息：更新ChartView	
}

//------------------------------------------------------------------------------------------------------
//	函数功能：	获取数据表中的性能数据
//	参数：		v_sCell_ID：Cell_ID；	v_dDate_Start：查询的起始日期；	v_iDay_Sum：查询天数
//	读取过程：	从对应表中获取数据，存入v_mIP_Instance容器中；[存放各个粒度的IP数据]
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Real::My_Load_Performance(CString v_sType,CString v_sFile_Key,CString v_sCell,CString v_sCell_ID,CString v_sPerformance,CString v_sField,COleDateTime v_dDate_Start,int v_iDay_Sum)
{
	CString			v_sSql,v_sTemp,v_sTime;								//临时变量
	CString			v_sTime_Start,v_sTime_End;							//日期
	int				v_iNum_Old=-1;										//记录：序号
	CString			v_sTable_Num,v_sTable_Num_New;						//实时表序号
	double			v_dValue;											//临时变量
	CString			v_sInstance;										//Instance_ID
	COleDateTime	v_dTime;											//查询日期
	Chart_Data		v_cChart_Data;										//粒度数据
	_variant_t		Var;												//临时变量
	CString			v_sNet,v_sDate;										//临时变量

	//【1】数据初始化：曲线数据申请内存
	v_cChart_Data.v_dChart_Value = new double[m_cChartView.v_iTime_Sum];//申请变量空间
	for(int vi=0;vi<m_cChartView.v_iTime_Sum;vi++)
		v_cChart_Data.v_dChart_Value[vi]=Chart::NoValue;				//初始化：数据

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//【 2】从数据表中读取性能数据
		int v_iDay=0;													//读取天数
		while(v_iDay<v_iDay_Sum)										//循环：查询天数
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			//计算性能
			v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//开始时间
			//Hour：5天1个表(每月6个表)
			while (true)
			{
				v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());	//获取表序号
				v_dTime=v_dDate_Start;									//1个表查询结束日期
				//天计数
				v_dDate_Start += COleDateTimeSpan(1,0,0,0);				//增加一天
				v_iDay++;												//
				if (v_iDay>=v_iDay_Sum)
					break;
				v_sTable_Num_New=My_Get_Table_Num(v_dDate_Start.GetDay());	//获取表序号
				if (v_sTable_Num!=v_sTable_Num_New)
					break;
			}
			v_sTime_End=v_dTime.Format("%Y-%m-%d")+" 23:59:59";			//结束时间
			v_sNet="Pm_LTE_"+v_sFile_Key;								//文件名称
			v_sDate=v_dTime.Format("%Y%m");								//年月
			v_sNet+="_"+v_sDate+"_"+v_sTable_Num;						//表名称增加：年月_序号

			//检查：表名称是否有效【依据：表名称集合v_mTable_Name】
			set<CString>::iterator v_pTable=v_mTable_Name.find(v_sNet);	//查找？？
			if (v_pTable==v_mTable_Name.end())							//没找到
				continue;												//无效返回

			//字段处理【一个字段】
			v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
			v_sSql+="select zTime,["+v_sField+"]";

			//SQL：语句组织
			v_sSql+=" from "+v_sNet+" where zCell='"+ v_sCell_ID+"' and zTime>='"
				+ v_sTime_Start +"' and  zTime<='"+ v_sTime_End +"' order by zTime";	//查询数据库：性能表
			v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);	//执行SELETE语句										

			//数据获取
			while(!v_pRecordset->adoEOF)
			{
				if (v_pIterator_Frame->second.v_iThread_Exit==2)		//强制退出？？？
					break;												//退出

				//直接读取字段：目的是提高读取速度；[利用字段读取函数取值效率低！！！]
				Var=v_pRecordset->GetCollect((_variant_t)"zTime");		//日期
				if (Var.vt != VT_NULL)
					v_dTime=Var.date;									//
				else
					v_dTime=COleDateTime::GetCurrentTime();				//默认：当前日期

				//字段读取
				v_dValue=v_cAdoConn.GetField_Double(v_sField);			//性能
				if(v_dValue==-1)										//-1：分母为零，无效数据
					v_dValue=Chart::NoValue;							//无效数据，画虚线；

				//数据保存：把获取的各粒度数据保存到v_cChart_Data.v_dChart_Value，前提是该粒度在有效的范围内；
				double	m_nextDataTime = Chart::chartTime(v_dTime.GetYear(),v_dTime.GetMonth(),v_dTime.GetDay(),v_dTime.GetHour(),v_dTime.GetMinute(),0);		//精确到分
				mTime_Array::iterator v_pIterator_Time=m_cChartView.v_mTime_Array.find(m_nextDataTime);		//查找粒度[依据m_nextDataTime]
				if(v_pIterator_Time != m_cChartView.v_mTime_Array.end())//找到？
				{
					int v_iNum=v_pIterator_Time->second;				//获得：数组序号
					v_cChart_Data.v_dChart_Value[v_iNum]=v_dValue;		//赋值：性能
				}

				v_pRecordset->MoveNext();								//
			}
		}
		v_cAdoConn.ExitConnect();										//断开连接

		//【3】数据迁移(把曲线数据容器加入绘图缓冲区)
		v_cChart_Data.v_sName = v_sCell+"_"+v_sPerformance;				//显示：名称
		m_cChartView.v_mChart_Data.insert(pair<CString,Chart_Data>(v_sCell+"_"+v_sPerformance,v_cChart_Data));	//添加：map容器：IP_Instance
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
