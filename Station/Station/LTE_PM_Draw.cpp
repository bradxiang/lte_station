// LTE_PM_Draw.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Draw.h"
#include <math.h>
#include <vector>
#include <sstream>
#include "LTE_PM_Draw_Select.h"


// CLTE_PM_Draw

IMPLEMENT_DYNCREATE(CLTE_PM_Draw, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Draw::CLTE_PM_Draw()
	: CFormView(CLTE_PM_Draw::IDD)
{
	v_bShow=false;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Draw::~CLTE_PM_Draw()
{
	v_mLTE_PM_Type.clear();												//容器清除
	v_mTable_Name.clear();												//容器清除
	v_mRegion.clear();													//容器清除
	delete m_cChartView.getChart();										//释放资源
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BEG_DATE, m_cDate);
	DDX_Control(pDX, IDC_COMBO1, m_cDay_Sum);
	DDX_Control(pDX, IDC_CK_DAY, m_cDay);
	DDX_Control(pDX, IDC_CK_MUTI, m_cMuti_Line);
	DDX_Control(pDX, IDC_CK_S, m_cKPI_S);
	DDX_Control(pDX, IDC_CK_VALUE, m_cLable);
	DDX_Control(pDX, IDC_YSCALE2, m_cPoint);
	DDX_Control(pDX, IDC_CHARTVIEW, m_cChartView);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_DRAW_AAA, m_cDraw_AAA);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_BUTTON1, m_cExcel);
	DDX_Control(pDX, IDC_INSTANCE_DETAIL, m_cPM_Select);
	DDX_Control(pDX, IDC_REFRESH, m_cRefresh);
	DDX_Control(pDX, IDC_DELETE, m_cClear);
	DDX_Control(pDX, IDC_COMBO3, m_cPM_Type);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Draw, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//Chart
	ON_WM_MOUSEWHEEL()
	ON_CONTROL(CVN_ViewPortChanged, IDC_CHARTVIEW, OnViewPortChanged)
	ON_CONTROL(CVN_MouseMovePlotArea, IDC_CHARTVIEW, OnMouseMovePlotArea)
	ON_MESSAGE(123,&CLTE_PM_Draw::Message_ChartView)					//添加消息映射 

	ON_NOTIFY(DTN_CLOSEUP, IDC_BEG_DATE, &CLTE_PM_Draw::OnDtnCloseupBegDate)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CLTE_PM_Draw::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_REFRESH, &CLTE_PM_Draw::OnBnClickedRefresh)
	ON_BN_CLICKED(IDC_DELETE, &CLTE_PM_Draw::OnBnClickedDelete)
	//MutiTreCtrl：消息 [点击了State]
	ON_BN_CLICKED(IDC_CK_MUTI, &CLTE_PM_Draw::OnBnClickedCkMuti)
	ON_BN_CLICKED(IDC_QUARY, &CLTE_PM_Draw::OnBnClickedQuary)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Draw::OnNMClickList1)
	ON_BN_CLICKED(IDC_INSTANCE_DETAIL, &CLTE_PM_Draw::OnBnClickedInstanceDetail)
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Draw::OnBnClickedButton1)
	ON_CBN_CLOSEUP(IDC_COMBO3, &CLTE_PM_Draw::OnCbnCloseupCombo3)
END_MESSAGE_MAP()


// CLTE_PM_Draw 诊断

#ifdef _DEBUG
void CLTE_PM_Draw::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLTE_PM_Draw::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLTE_PM_Draw 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CString v_sTemp;													//临时变量
	//窗体名称
	v_sFrame_Name="LTE 性能图形查询";									//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗 [小区]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList.InsertColumn(1,"小区",LVCFMT_LEFT,200);  
	m_cList.InsertColumn(2,"小区ID",LVCFMT_LEFT,0);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES );

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
		&m_cDraw_AAA,			// Right pane
		IDC_LTE_PM_Draw_Main,		// this ID is used for saving/restoring splitter position and it must be unique !
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
	m_cDay_Sum.SetCurSel(0);											//默认查询天数：1天；

	//Chart初始化
	v_bShow=true;
    GetLocalTime(&v_dChart_Date);										//获得：查询日期
	v_iChart_Day_Sum=1;													//获得：查询天的数量：1天
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

	//信息显示
	v_iList_Item=-1;													//无效选项
	My_Show_Clear();													//删除Tree控件信息
												
	//变量初始化
	m_cType.AddString("15m粒度");
	m_cType.AddString("1H粒度");
	m_cType.SetCurSel(0);												//默认：15m粒度

	// TODO: 在此添加专用代码和/或调用基类
	My_LoadData_Para();													//获取：性能参数
	OnCbnCloseupCombo3();												//性能类型选择

	OnBnClickedInstanceDetail();										//性能选择
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Draw::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)	//键盘消息？？？
	{
		// Translate dialog key if applicable
		if(::IsDialogMessage(m_hWnd, pMsg))								//转换消息 [把消息发到m_hWnd]
			return TRUE;												    //消息结束 [不会把该消息分发给窗口函数处理]
	}

	return CFormView::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：子窗销毁
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnDestroy()
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
void CLTE_PM_Draw::OnSize(UINT nType, int cx, int cy)
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
		v_pWnd = GetDlgItem(IDC_CHARTVIEW);								//绘图
		if(v_pWnd)
			v_pWnd->MoveWindow(lpRec.left,lpRec.top,10,10);				//定宽、定长

		GetClientRect(&lpRec); 
		m_cChartView.v_cUI_Data.v_iChart_X=lpRec.right;					//窗口宽
		m_cChartView.v_cUI_Data.v_iChart_Y=lpRec.bottom;				//窗口高

		m_cChartView.updateViewPort(true, false);						//绘图Chart
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：更新ChartView
//	工    作：【(1)在线程，性能数据获取；(2)发送消息；(3)消息响应，在用户界面绘制曲线；】
//------------------------------------------------------------------------------------------------------
LRESULT CLTE_PM_Draw::Message_ChartView(WPARAM iParam1,LPARAM iParam2)
{
	//绘图：Chart
	m_cChartView.updateViewPort(true, false);							//图形绘制

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能类型选择
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnCbnCloseupCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp;												//临时变量
	int			v_iItem;												//临时变量
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	//【1】选择范围：小区  or 区域
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
		m_cList.InsertColumn(2,"区域",LVCFMT_LEFT,0);  

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
//	函数功能：点击列表窗 [小区]
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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

			if(!m_cMuti_Line.GetCheck())								//单线
			{
				v_iWorking=2;											//工作任务：单线绘制
				v_pIterator_Frame->second.v_iThread_Exit=1;				//线程：工作
				v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);	//开启查询数据库线程
			}
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：刷新【重新绘制曲线】
//------------------------------------------------------------------------------------------------------	
void CLTE_PM_Draw::OnBnClickedRefresh()
{
 	v_iWorking=3;														//工作任务：曲线绘制【单线、多线】
 	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
 	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：小区查询
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码
	if (v_pIterator_Frame->second.v_iThread_Exit==0)
	{
		v_iWorking=1;													//工作任务：查询

		v_pIterator_Frame->second.v_iThread_Exit=1;						//线程：工作
		v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);			//开启查询数据库线程
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能选择
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnBnClickedInstanceDetail()
{
	// TODO: 在此添加控件通知处理程序代码
	CLTE_PM_Draw_Select	v_cDlg;											//实例化

 	v_cDlg.v_mLTE_PM_Type=&v_mLTE_PM_Type;								//指针：性能参数
	v_cDlg.v_iSelect_PM_Type=m_cPM_Type.GetCurSel();					//选择的性能类型序号

	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能导出
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	My_SendToFile();													//导出到CSV文件
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CLTE_PM_Draw::My_Thread_Inoput(LPVOID lparam)
{
	CLTE_PM_Draw *  lp_this = NULL ;
	lp_this = (CLTE_PM_Draw *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cPM_Type.EnableWindow(false);							//查询：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效
	lp_this->m_cPM_Select.EnableWindow(false);							//导出：无效
	lp_this->m_cRefresh.EnableWindow(false);							//导出：无效
	lp_this->m_cClear.EnableWindow(false);								//导出：无效

	lp_this->My_Input_Main();											//导入_主程序

	if (lp_this->m_cPM_Type.GetCurSel()==0)
	{
		lp_this->m_cQuery.EnableWindow(true);							//查询：有效
	}
	lp_this->m_cPM_Type.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效
	lp_this->m_cPM_Select.EnableWindow(true);							//导出：有效
	lp_this->m_cRefresh.EnableWindow(true);								//导出：有效
	lp_this->m_cClear.EnableWindow(true);								//导出：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Cell();												//装载Cell信息
		break;
	case 2:
		My_Chart();														//单线绘制
		break;
	case 3:
		My_Chart_Muti();												//曲线绘制【单线、多线】
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载Cell信息
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::My_LoadData_Cell()
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
		v_iList_Item=-1;												//无效选项

		//查询日期准备
		m_cKey.GetWindowText(v_sLog_Key);								//获得：查询KEY
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
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sCell);			//Cell
			v_sCell_ID.Format("%d",v_iCell_ID);							//字符串
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sCell_ID);			//Cell_ID

			v_pRecordset->MoveNext();									//
			v_iID++;													//

			if (v_iID%100==0)
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum.SetWindowText(v_sTemp);							//显示：Log总数
			}
		}

		v_sTemp.Format("%d",v_iID);										//
		m_cSum.SetWindowText(v_sTemp);									//显示：Log总数

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Chart
////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------------
//	函数功能：Draw track cursor when mouse is moving over plotarea
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnMouseMovePlotArea()
{
	// Get the focus to ensure being able to receive mouse wheel events
	m_cChartView.SetFocus();

	m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),m_cLable.GetCheck()); 
	m_cChartView.updateDisplay();
}

//------------------------------------------------------------------------------------------------------
//	函数功能：鼠标轮滑动消息 [用途：放大、缩小、左右平移]
//------------------------------------------------------------------------------------------------------
BOOL CLTE_PM_Draw::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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
void CLTE_PM_Draw::OnViewPortChanged()
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
		m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),m_cLable.GetCheck());
		m_cChartView.updateDisplay();
	}
}

//------------------------------------------------------------------------------------------------------
//	函数功能：传递绘图参数  [从屏幕到Chart变量]
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::OnChart_Para()
{
	CString		v_sTemp;												//临时变量

	m_cDate.GetTime(m_cChartView.v_cUI_Data.v_dDate_Start);				//获得：查询开始日期
	m_cChartView.v_cUI_Data.v_iChart_Day_Sum=m_cDay_Sum.GetCurSel()+1;	//查询天数
	m_cChartView.v_cUI_Data.v_iDay=m_cDay.GetCheck();					//单天有效
	m_cChartView.v_cUI_Data.v_iMuti_Line=m_cMuti_Line.GetCheck();		//多线有效
	m_cChartView.v_cUI_Data.v_iKPI_S=m_cKPI_S.GetCheck();				//静态有效
	m_cChartView.v_cUI_Data.v_iPoint=m_cPoint.GetCheck();				//数据点有效

	m_cChartView.v_cUI_Data.v_sKPI_Name = "网络性能";					//题目名称
	
	m_cChartView.v_cUI_Data.v_iY_scale=0;								//y轴刻度有效

	if (m_cType.GetCurSel()==0)											//15m ???
		m_cChartView.v_cUI_Data.v_iTime=0;								//粒度信息：15min
	else
		m_cChartView.v_cUI_Data.v_iTime=1;								//粒度信息：时
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：清除屏幕显示信息
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::My_Show_Clear()									    //清除屏幕显示信息
{
	v_iList_Item=-1;													//选中的Item无效
	My_Chart_New();														//绘图复位
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击多线
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::OnBnClickedCkMuti()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cMuti_Line.GetCheck())										//多线条有效？
	{
 		m_cDay.SetCheck(false);                                         //无效：单天；
 		m_cKPI_S.SetCheck(false);										//无效：静态门限；
	}

	OnBnClickedDelete();												//清除Chart
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：清除图形 [绘图复位]
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::OnBnClickedDelete()
{
	// TODO: 在此添加控件通知处理程序代码

	//清除列表的选中状态
	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//遍历列表
		m_cList.SetCheck(vi,0);											//清除选中状态

	//绘图初始化
	My_Chart_New();														//绘图复位
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：绘图复位
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::My_Chart_New()
{
	//绘图初始化
	OnChart_Para();														//传递：绘图参数
	m_cChartView.initChartViewer(&m_cChartView);						//初始化Chart
	//绘图：Chart
	m_cChartView.updateViewPort(true, false);							//绘图
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：曲线绘制【单线、多线】
//------------------------------------------------------------------------------------------------------	
void CLTE_PM_Draw::My_Chart_Muti()
{
	if(v_iList_Item!=-1)												//有效条目序号？？？
	{

		if(!m_cMuti_Line.GetCheck())									//单线
		{
            My_Chart();													//单线画线
        }
		if (m_cMuti_Line.GetCheck())									//多线
		{
			My_Chart_New();												//绘图复位
	
			for (int vi=0;vi<m_cList.GetItemCount();vi++)				//遍历列表
			{
				if(m_cList.GetCheck(vi))								//判断：选中状态？？？
					My_List_Chart(vi);									//绘制曲线【单线】
			}
      	}
	
     }
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：绘图复位然后找当前节点
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::My_Chart()
{
	//绘图复位
	My_Chart_New();														//绘图复位

	//绘图：遍历Tree，找到选中的节点	
	My_List_Chart(v_iList_Item);										//绘图：选中的Item
}

//------------------------------------------------------------------------------------------------------
//	函数功能：绘图：选中的Tree Node
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::My_List_Chart(int v_iList_Item)
{
	My_List_Chart_GetNode(v_iList_Item);								//获取：节点信息【保存在public变量中】
	My_Chart_IP();														//绘制：性能信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取节点信息 
//	参    数：v_iList_Item：选中的Item
//			 【用于：绘图、显示节点信息，保存在v_sCell变量中】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::My_List_Chart_GetNode(int v_iList_Item)
{
	v_sCell=m_cList.GetItemText(v_iList_Item,1);						//Cell名称
	v_sCell_ID=m_cList.GetItemText(v_iList_Item,2);						//Cell_ID
}

//------------------------------------------------------------------------------------------------------
//	函数功能：Chart画一条线
//	参    数：选中的 IP [v_sItem_IP]
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::My_Chart_IP()
{
	CString					v_sTemp;									//临时变量
	bool					v_bOK=false;								//临时变量
	mLTE_PM_Type::iterator	v_pSelect;									//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	COleDateTime	v_dDate_Start;										//查询开始日期
	m_cDate.GetTime(v_dDate_Start);										//获得：选择粒度日期
	v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//设置时间：00:00:00

	//遍历：性能类型【计算性能、区域性能】
	for (v_pSelect=v_mLTE_PM_Type.begin();v_pSelect!=v_mLTE_PM_Type.end();v_pSelect++)
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//强制退出？？？
			break;														//退出

		m_cPM_Type.GetWindowText(v_sTemp);
		if (v_pSelect->first==v_sTemp)									//与选择的一致？？？
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
					v_pPM_Name->second.v_sTable_Key,					//性能文件名称key
					v_sCell,											//小区名称
					v_sCell_ID,											//小区ID
					v_pPM_Name->second.v_sPM_Name,						//性能名称
					v_pPM_Name->first,									//性能字段
					v_dDate_Start,										//开始日期
					v_iChart_Day_Sum);									//查询天数
					
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
//	函数功能：日期选择
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::OnDtnCloseupBegDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	m_cDate.GetTime(&v_dChart_Date);									//获得：查询开始日期

	My_Chart_New();														//绘图复位

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：天数选择
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iChart_Day_Sum=m_cDay_Sum.GetCurSel()+1;							//获得：查询天的数量

	My_Chart_New();														//绘图复位
}

//------------------------------------------------------------------------------------------------------
//	函数功能：	获取数据表中的性能数据
//	参数：		v_sCell_ID：Cell_ID；	v_dDate_Start：查询的起始日期；	v_iDay_Sum：查询天数
//	读取过程：	从对应表中获取数据，存入v_mIP_Instance容器中；[存放各个粒度的IP数据]
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::My_Load_Performance(CString v_sType,CString v_sFile_Key,CString v_sCell,CString v_sCell_ID,CString v_sPerformance,CString v_sField,COleDateTime v_dDate_Start,int v_iDay_Sum)
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

			if (m_cPM_Type.GetCurSel()==0)								//计算性能？？？
			{
				//确定表名称：小时：5天1个表(每月6个表)；15min：1天1个表
				v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//开始时间
				if (m_cType.GetCurSel()==1)								//1H粒度？？？
				{
					//Hour：5天1个表(每月6个表)
					while (true)
					{
						v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());		//获取表序号
						v_dTime=v_dDate_Start;							//1个表查询结束日期
						//天计数
						v_dDate_Start += COleDateTimeSpan(1,0,0,0);		//增加一天
						v_iDay++;										//
						if (v_iDay>=v_iDay_Sum)
							break;
						v_sTable_Num_New=My_Get_Table_Num(v_dDate_Start.GetDay());	//获取表序号
						if (v_sTable_Num!=v_sTable_Num_New)
							break;
					}
					v_sTime_End=v_dTime.Format("%Y-%m-%d")+" 23:59:59";	//结束时间
					v_sNet="Pm_LTE_"+v_sFile_Key;						//文件名称
					v_sDate=v_dTime.Format("%Y%m");						//年月
					v_sNet+="_"+v_sDate+"_"+v_sTable_Num;				//表名称增加：年月_序号
				}
				else													//15mins
				{
					//15mins：1天1个表
					//获取：表名称
					v_sTime_End=v_dDate_Start.Format("%Y-%m-%d")+" 23:59:59";	//结束时间
					v_sNet="Pm_LTE_"+v_sFile_Key;						//文件名称
					v_sDate=v_dDate_Start.Format("%Y%m%d");				//年月日
					v_sNet+="_"+v_sDate;								//表名称增加：年月日
					//天+1
					v_dDate_Start += COleDateTimeSpan(1,0,0,0);			//增加一天
					v_iDay++;											//
				}
			}
			else
			{															//区域性能
				//区域性能：总共一个表(Pm_LTE_Region)
				v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//开始时间
				//天+1
				v_dDate_Start += COleDateTimeSpan(v_iDay_Sum-1,0,0,0);	//增加：查询天数
				v_iDay+=v_iDay_Sum;										//查询天数
				v_sTime_End=v_dDate_Start.Format("%Y-%m-%d")+" 23:59:59";	//结束时间
				v_sNet="Pm_LTE_"+v_sFile_Key;							//文件名称
			}

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
//	函数功能：获取表序号
//	工	  作：1：1-5；	2：6-10；	3：11-15；	4：16-20；	5：21-25；	6：26--
//------------------------------------------------------------------------------------------------------			
CString CLTE_PM_Draw::My_Get_Table_Num(int v_iDay)
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
void CLTE_PM_Draw::My_LoadData_Para()
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
				v_cLTE_PM.v_bOK=false;									//性能：未选中
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//增加：性能参数
			}

			v_pRecordset->MoveNext();									//
		}
		m_cPM_Type.SetCurSel(0);										//默认：第一个
		m_cType.SetCurSel(0);											//选择：15mins
		m_cType.EnableWindow(false);									//无效

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
				v_cLTE_PM.v_sTable_Key="Region";						//性能：表名称
				v_cLTE_PM.v_bOK=false;									//性能：未选中
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//增加：性能参数
			}

			v_pRecordset->MoveNext();									//
		}
		m_cPM_Type.SetCurSel(1);										//默认：第一个

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

//-------------------------------------------------------------------------------------------------
// 导出处理：写入CSV文件，再用Excel打开；
//-------------------------------------------------------------------------------------------------
void CLTE_PM_Draw::My_SendToFile() 
{
	CString	v_sStr,v_sCSV,v_sPath_Data,v_sCSV_File;						//临时变量
	CFile	v_fWriter;													//实例化
	CFileFind	finder;
	double		v_dKPI;													//KPI值
	mChart_Data::iterator v_pIterator_KPI;								//KPI_Instance的map容器迭代器

	if (m_cChartView.v_mChart_Data.size()==0)							//有无数据？
		return;															//没有：返回

	//目录处理
	v_sPath_Data=theApp.m_sPath+"\\DATA";								//文件目录
	if (!finder.FindFile(v_sPath_Data))									//存在？
	{
		if (!::CreateDirectory(v_sPath_Data, NULL))						//不存在：创建目录
		{
			MessageBox("异常："+v_sPath_Data+" 目录创建失败。");		//失败
			return;
		}
	}

	try
	{
		//开始日期计算
		COleDateTime	v_dDate_Start;									//查询开始日期
		v_dDate_Start = m_cChartView.v_cUI_Data.v_dDate_Start;			//获取：绘图查询开始日期
		v_dDate_Start=COleDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);

		//生成CSV文件：文件名称v_sCSV_File
		v_sCSV_File = theApp.m_sPath+"\\DATA\\Excel.csv";				//设置：CSV文件名称Excel.csv
		if(v_fWriter.Open(v_sCSV_File, CFile::modeCreate | CFile::modeWrite))		//打开文件，如不存在则生成，如存在则清空文件；
		{
			if (m_cChartView.v_mChart_Data.size()>1)
			{
				v_sCSV="序号,时间";										//列名称
				v_pIterator_KPI=m_cChartView.v_mChart_Data.begin();		//从头开始
				while(v_pIterator_KPI != m_cChartView.v_mChart_Data.end())			//结束？？？
				{
					v_sCSV += "," + v_pIterator_KPI->second.v_sName;	//第二列、第三列、...、第n列
					v_pIterator_KPI++;									//迭代器加一
				}
				v_sCSV +="\r\n";										//回车换行
				v_fWriter.Write(v_sCSV,v_sCSV.GetLength());				//写入字符串

				//表内容处理：取列表数据，写入CSV
				for (int vi=0;vi<m_cChartView.v_iTime_Sum;vi++)
				{
					v_sCSV.Format("%d",vi+1);
					v_sCSV+=","+v_dDate_Start.Format("%Y-%m-%d_%H:%M");
					v_pIterator_KPI=m_cChartView.v_mChart_Data.begin();	//从头开始
					while(v_pIterator_KPI != m_cChartView.v_mChart_Data.end())	//结束？？？
					{
						v_dKPI=v_pIterator_KPI->second.v_dChart_Value[vi];	//获得：KPI值
						if (v_dKPI==Chart::NoValue)						//为空值？？？
							v_dKPI=-1;									//赋值：空值
						v_sStr.Format("%6.2f",v_dKPI);					//数据获取
						v_sCSV += "," + v_sStr;							//第二列、第三列、...、第n列
						v_pIterator_KPI++;								//迭代器加一
					}
					v_sCSV +="\r\n";									//回车换行
					v_fWriter.Write(v_sCSV,v_sCSV.GetLength());			//写入字符串

					if (m_cType.GetCurSel()==0)							//15m
						v_dDate_Start += COleDateTimeSpan(0,0,15,0);	//粒度计算
					else
						v_dDate_Start += COleDateTimeSpan(0,1,0,0);		//粒度计算
				}
			}
			else
			{
				v_sCSV="序号,时间,数值";								//列名称
				v_pIterator_KPI=m_cChartView.v_mChart_Data.begin();		//从头开始
				while(v_pIterator_KPI != m_cChartView.v_mChart_Data.end())			//结束？？？
				{
					v_sCSV += "," + v_pIterator_KPI->second.v_sName;	//第二列、第三列、...、第n列
					v_pIterator_KPI++;									//迭代器加一
				}
				v_sCSV +="\r\n";										//回车换行
				v_fWriter.Write(v_sCSV,v_sCSV.GetLength());				//写入字符串

				//表内容处理：取列表数据，写入CSV
				for (int vi=0;vi<m_cChartView.v_iTime_Sum;vi++)
				{
					v_sCSV.Format("%d",vi+1);
					v_sCSV+=","+v_dDate_Start.Format("%Y-%m-%d_%H:%M");
					v_pIterator_KPI=m_cChartView.v_mChart_Data.begin();	//从头开始
					v_dKPI=v_pIterator_KPI->second.v_dChart_Value[vi];		//获得：KPI值
					if (v_dKPI==Chart::NoValue)							//为空值？？？
						v_dKPI=-1;										//赋值：空值
					v_sStr.Format("%6.2f",v_dKPI);						//数据获取
					v_sCSV += "," + v_sStr;								//第二列、第三列、...、第n列
					v_sCSV +="\r\n";									//回车换行
					v_fWriter.Write(v_sCSV,v_sCSV.GetLength());			//写入字符串

					if (m_cType.GetCurSel()==0)							//15m
						v_dDate_Start += COleDateTimeSpan(0,0,15,0);	//粒度计算
					else
						v_dDate_Start += COleDateTimeSpan(0,1,0,0);		//粒度计算
				}
			}
			v_fWriter.Close();											//文件关闭

			//打开文件
			ShellExecute(NULL,"open", v_sCSV_File, NULL, NULL, SW_SHOWNORMAL);		//打开文件：Excel.csv

		}
		else
			MessageBox("请关闭已经打开的导出文件，再重新导出！！！");	//窗体提示
	}	
	catch(_com_error e)
	{
		CString v_sError = (char *)e.Description();						//获得：出错信息
		MessageBox("异常"+v_sError);									//提示信息
	}
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
