// All_PM_Draw.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "All_PM_Draw.h"
#include <math.h>
#include <vector>
#include <sstream>
#include "LTE_PM_Draw_Select.h"


// CAll_PM_Draw

IMPLEMENT_DYNCREATE(CAll_PM_Draw, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CAll_PM_Draw::CAll_PM_Draw()
	: CFormView(CAll_PM_Draw::IDD)
{
	v_bShow=false;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CAll_PM_Draw::~CAll_PM_Draw()
{
	v_mTree_All.clear();												//容器清除
	v_mLTE_PM_Type.clear();												//容器清除
	v_mTable_Name.clear();												//容器清除
	delete m_cChartView.getChart();										//释放资源
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE1, m_cTree);
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
	DDX_Control(pDX, IDC_COMBO3, m_cOut_In);
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_INSTANCE_DETAIL, m_cPM_Select);
	DDX_Control(pDX, IDC_REFRESH, m_cRefresh);
	DDX_Control(pDX, IDC_DELETE, m_cClear);
	DDX_Control(pDX, IDC_BUTTON1, m_cExcel);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAll_PM_Draw, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CAll_PM_Draw::OnTvnSelchangedTree1)
	//Chart
	ON_WM_MOUSEWHEEL()
	ON_CONTROL(CVN_ViewPortChanged, IDC_CHARTVIEW, OnViewPortChanged)
	ON_CONTROL(CVN_MouseMovePlotArea, IDC_CHARTVIEW, OnMouseMovePlotArea)
	ON_MESSAGE(123,&CAll_PM_Draw::Message_ChartView)					//添加消息映射 

	ON_NOTIFY(DTN_CLOSEUP, IDC_BEG_DATE, &CAll_PM_Draw::OnDtnCloseupBegDate)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAll_PM_Draw::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_REFRESH, &CAll_PM_Draw::OnBnClickedRefresh)
	ON_BN_CLICKED(IDC_DELETE, &CAll_PM_Draw::OnBnClickedDelete)
	//MutiTreCtrl：消息 [点击了State]
	ON_BN_CLICKED(IDC_CK_MUTI, &CAll_PM_Draw::OnBnClickedCkMuti)
	ON_BN_CLICKED(IDC_QUARY, &CAll_PM_Draw::OnBnClickedQuary)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CAll_PM_Draw::OnNMClickList1)
	ON_BN_CLICKED(IDC_INSTANCE_DETAIL, &CAll_PM_Draw::OnBnClickedInstanceDetail)
	ON_BN_CLICKED(IDC_BUTTON1, &CAll_PM_Draw::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAll_PM_Draw 诊断

#ifdef _DEBUG
void CAll_PM_Draw::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAll_PM_Draw::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAll_PM_Draw 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CString v_sTemp;													//临时变量
	//窗体名称
	v_sFrame_Name="共址性能查询";										//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗 [站址]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,40);					//
	m_cList.InsertColumn(1,"机房/资源点名称",LVCFMT_LEFT,240);			//
	m_cList.InsertColumn(2,"区域",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(3,"2G-3G-4G",LVCFMT_CENTER,60);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//树初始化：m_cTree
	m_cTree.ModifyStyle(0,TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES);
	v_cImage_Tree.Create(IDB_TREE_STATION,16,1,RGB(255,255,255));		//树形图象列表生成
	m_cTree.SetImageList(&v_cImage_Tree,TVSIL_NORMAL);					//树初始化
	v_cImage_State.Create(IDB_TREE_CHECK,13, 1, RGB(255,255,255));
	m_cTree.SetImageList(&v_cImage_State,TVSIL_STATE);
	m_cTree.SetBkColor(RGB(200,200,255));								//设置树形控件的背景色
	m_cTree.SetTextColor(RGB(127,0,0));									//设置文本背景色

	//室外、室内共址选择
	m_cOut_In.AddString("室外");										//
	m_cOut_In.AddString("室内");										//
	m_cOut_In.SetCurSel(0);												//默认

	//分窗 [先生成左边的上下关系的水平分隔符，再生成左右关系的垂直分隔符，即2*1]
	CRect	v_cRect;													//矩形
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//获得：指针
	pWnd->GetWindowRect(&v_cRect);										//获得：尺寸
	ScreenToClient(&v_cRect);											//坐标变换
	pWnd->DestroyWindow();												//销毁：指针

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Left.Create(											//水平线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList,				// Top pane
		&m_cTree,				// Bottom nested pane
		IDC_All_PM_Draw_HORIZ,	// this ID is used for saving/restoring splitter position and therefore it must be unique !
		v_cRect					// dimensions of the splitter pane
		);
	m_SplitterPane_Left.m_bHorizSplitter_Parent=true;					//水平分隔符为父窗
	m_SplitterPane_Main.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_SplitterPane_Left,	// Left pane
		&m_cDraw_AAA,			// Right pane
		IDC_All_PM_Draw_Main,		// this ID is used for saving/restoring splitter position and it must be unique !
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

	//变量初始化
    v_hTree_Current=NULL;                                               //选中的Tree节点

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

	//变量初始化
	m_cType.AddString("15m粒度");
	m_cType.AddString("1H粒度");
	m_cType.SetCurSel(0);												//默认：15m粒度

	//信息显示
	My_Show_Clear();													//删除Tree控件信息
												
	// TODO: 在此添加专用代码和/或调用基类
	My_LoadData_Para();													//获取：性能数据

//	OnBnClickedInstanceDetail();										//性能选择
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CAll_PM_Draw::PreTranslateMessage(MSG* pMsg)
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
void CAll_PM_Draw::OnDestroy()
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
void CAll_PM_Draw::OnSize(UINT nType, int cx, int cy)
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
LRESULT CAll_PM_Draw::Message_ChartView(WPARAM iParam1,LPARAM iParam2)
{
	//绘图：Chart
	m_cChartView.updateViewPort(true, false);							//图形绘制

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击列表窗 [站址]
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item_Room>=0)									//有效条目序号？？？
				m_cList.SetItemBkColor(v_iList_Item_Room,RGB(255,255,255));		//恢复背景色
			v_iList_Item_Room=pNMListView->iItem;						//获得：选中条目序号
			m_cList.SetItemBkColor(v_iList_Item_Room,RGB(255,0,0));		//设置背景色：红色

			My_LoadData_Tree(m_cList.GetItemText(v_iList_Item_Room,1));	//获取机房信息 【绘制Tree】
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：站址查询
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能选择
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::OnBnClickedInstanceDetail()
{
	// TODO: 在此添加控件通知处理程序代码
	CLTE_PM_Draw_Select	v_cDlg;											//实例化

	v_cDlg.v_mLTE_PM_Type=&v_mLTE_PM_Type;								//指针：性能参数

	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能导出
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	My_SendToFile();													//导出到CSV文件
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CAll_PM_Draw::My_Thread_Inoput(LPVOID lparam)
{
	CAll_PM_Draw *  lp_this = NULL ;
	lp_this = (CAll_PM_Draw *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效
	lp_this->m_cPM_Select.EnableWindow(false);							//导出：无效
	lp_this->m_cRefresh.EnableWindow(false);							//导出：无效
	lp_this->m_cClear.EnableWindow(false);								//导出：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
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
void CAll_PM_Draw::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_Query_Station();												//站址查询
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
//	函数功能：查询机房信息
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::My_Query_Station()
{
	int				v_iID=0;											//临时变量
	CString			v_sSql,v_sTemp;										//临时变量
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//查询日期准备
	m_cKey.GetWindowText(v_sLog_Key);									//获得：查询KEY
	v_sLog_Key.Replace("[","[[]");										//限制处理
	v_sLog_Key.Replace("'","");											//限制处理
	v_sLog_Key.Trim();													//去掉空格

	v_sSql += "SELECT * FROM CON_Station"; 

	if (!v_sLog_Key.IsEmpty())											//查询KEY处理
	{
		v_sSql+=" where zStation like '%"+v_sLog_Key+"%'";
	}
	v_sSql+=" order by zStation";

	My_LoadData_Station_Data(v_sSql);									//获取：站址信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载站址信息
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem,v_iID=0;									//临时变量
	CString			v_sTemp;											//临时变量
	COleDateTime	v_dDate_Start;										//查询开始日期
	int				v_iGSM_BTS,v_iTD_NodeB,v_iLTE_NodeB,v_iSum;			//临时变量

	_variant_t	Var;
	CString			v_sStation,v_sRegion,v_sLongitude;					//临时变量
	CString			v_sLatitude,v_sOnline;								//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		//树复位
		m_cTree.DeleteAllItems();										//树清除
		//列表复位
		m_cList.DeleteAllItems();										//清除Items
		v_iList_Item_Room=-1;											//无效选项

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			if (m_cOut_In.GetCurSel()==0)								
			{															//室外
				v_iGSM_BTS = v_cAdoConn.GetField_Int("zGSM_BTS_OUT");	//zGSM_BTS_OUT
				v_iTD_NodeB = v_cAdoConn.GetField_Int("zTD_NodeB_OUT");	//zTD_NodeB_OUT
				v_iLTE_NodeB=v_cAdoConn.GetField_Int("zLTE_NodeB_OUT");	//zLTE_NodeB_OUT
			}
			else
			{															//室内
				v_iGSM_BTS = v_cAdoConn.GetField_Int("zGSM_BTS");		//zGSM_BTS
				v_iTD_NodeB = v_cAdoConn.GetField_Int("zTD_NodeB");		//zTD_NodeB
				v_iLTE_NodeB = v_cAdoConn.GetField_Int("zLTE_NodeB");	//zLTE_NodeB
			}
			//显示
			v_iSum=v_iGSM_BTS+v_iTD_NodeB+v_iLTE_NodeB;					//基站总计
			if (v_iSum != 0)
			{
				v_sTemp.Format(" %04d",v_iID+1);
				v_iItem=m_cList.InsertItem(0xffff,v_sTemp);				//加入列表窗
				m_cList.SetItemText(v_iItem,1,v_sStation);				//机房/资源点名称
				m_cList.SetItemText(v_iItem,2,v_sRegion);				//区域
				v_sTemp.Format("%d-%d-%d",v_iGSM_BTS,v_iTD_NodeB,v_iLTE_NodeB);
				m_cList.SetItemText(v_iItem,3,v_sTemp);					//GSM_BTS

				v_iID++;												//计数：告警数量
			}
			v_pRecordset->MoveNext();									//

			if (v_iID%100==0)											//显示：100的整数
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum.SetWindowText(v_sTemp);							//显示：站址总数
			}
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cSum.SetWindowText(v_sTemp);									//显示：站址总数

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
//	函数功能：装载站址信息【Tree】
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_LoadData_Tree(CString v_sRoom)
{
	CString		v_sSql,v_sTemp,v_sRNC;									//临时变量
	CString		v_sBTS,v_sBTS_Name,v_sNodeB_Name,v_sSation,v_sCell,v_sSation_Cell;	//临时变量

	mStation_OutPut_BTS				v_mNodeb;							//Nodeb/BTS 【Tree记录：value=1，为BTS;value=2，为Nodeb;】
	mStation_OutPut_BTS::iterator	v_pNodeb;							//迭代器
	Station_OutPut_BTS_Struct		v_cNodeb;							//结构
	mString_String::iterator		v_pCell;							//迭代器
	Node_Cell_Struct				v_cNode_Cell;						//结构

	//树复位
	m_cTree.DeleteAllItems();											//树清除
	v_mTree_All.clear();												//容器清除

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//
		m_cOut_In.GetWindowText(v_sRNC);								//选择结果：室外、室内；
		//小区共址查询 【查询小区站址为v_sRoom对应的基站信息，加入容器v_mNodeb】
		//【GSM小区】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select GSM_Cell.zCell,GSM_Cell.zBTS as zBTS_BTS,GSM_Cell.zStation as zStation_Cell, \
				  GSM_BTS.zBTS_NAME,GSM_BTS.zStation as zStation_BTS \
				  FROM GSM_Cell left join GSM_BTS on GSM_Cell.zBTS=GSM_BTS.zBTS \
				  where GSM_BTS.zType='"+v_sRNC+"' and GSM_Cell.zStation='"+v_sRoom+"' order by GSM_BTS.zStation,GSM_Cell.zCell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell

			v_pNodeb=v_mNodeb.find(v_sBTS);								//查找：BTS
			if (v_pNodeb==v_mNodeb.end())
			{
				//增加：BTS
				v_cNodeb.v_sName=v_sBTS_Name;							//中文名
				v_cNodeb.v_sNetwork="2";								//网络类型
				v_cNodeb.v_sStation=v_sSation;							//站址
				v_mNodeb.insert(pair<CString,Station_OutPut_BTS_Struct>(v_sBTS,v_cNodeb));	//容器增加
			}
			v_pNodeb=v_mNodeb.find(v_sBTS);								//查找：BTS
			//增加：Cell
			v_pNodeb->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sSation_Cell));	//容器增加

			v_pRecordset->MoveNext();									//下一条
		}

		//【TD-LTE】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select LTE_Cell.zCell,LTE_Cell.zNODEB as zBTS_BTS,LTE_Cell.zStation as zStation_Cell, \
				  LTE_NODEB.zStation as zStation_BTS,LTE_NODEB.zRNC \
				  FROM LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB \
				  where LTE_NODEB.zType='"+v_sRNC+"' and LTE_Cell.zStation='"+v_sRoom+"' order by LTE_NODEB.zStation,LTE_Cell.zCell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sRNC = v_cAdoConn.GetField_String("zRNC");				//zRNC
			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zBTS
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell

			v_pNodeb=v_mNodeb.find(v_sBTS);								//查找：BTS
			if (v_pNodeb==v_mNodeb.end())
			{
				//增加：E_NodeB
				v_cNodeb.v_sName=v_sBTS;								//中文名
				if (v_sRNC.Find("dalian_")>=0)
					v_cNodeb.v_sNetwork="4";							//网络类型：4G
				else
					v_cNodeb.v_sNetwork="3";							//网络类型：3G
				v_cNodeb.v_sStation=v_sSation;							//站址
				v_mNodeb.insert(pair<CString,Station_OutPut_BTS_Struct>(v_sBTS,v_cNodeb));	//容器增加
			}
			v_pNodeb=v_mNodeb.find(v_sBTS);								//查找：BTS
			//增加：Cell
			v_pNodeb->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sSation_Cell));	//容器增加

			v_pRecordset->MoveNext();									//
		}

		HTREEITEM	v_hRoot,v_hNode_BTS,v_hNode_Cell;					//根目录句柄、节点句柄；
		//依据新增加的基站，绘制Tree信息
		v_hRoot=m_cTree.InsertItem(v_sRoom,6,7,TVI_ROOT);			//树：增加根节点 [机房/资源点]		
		for (v_pNodeb=v_mNodeb.begin();v_pNodeb!=v_mNodeb.end();v_pNodeb++)		//遍历：容器
		{
			if(v_pNodeb->second.v_sNetwork=="2")						//BTS
			{
				//GSM：BSC获取
				v_hNode_BTS=m_cTree.InsertItem(v_pNodeb->first+"_"+v_pNodeb->second.v_sName
					+"     【"+v_pNodeb->second.v_sStation+"】",4,5,v_hRoot);	//树：增加子节点 [BTS]
				for (v_pCell=v_pNodeb->second.v_mCell.begin();v_pCell!=v_pNodeb->second.v_mCell.end();v_pCell++)	//遍历：容器
				{
					v_hNode_Cell=m_cTree.InsertItem(v_pCell->first+"     【"+v_pCell->second+"】",0,1,v_hNode_BTS);	//树：增加子节点 [Cell]
					v_cNode_Cell.v_sCell=v_pCell->first;				//Cell
					v_cNode_Cell.v_sNetwork=v_pNodeb->second.v_sNetwork;//Network
					v_mTree_All.insert(pair<HTREEITEM,Node_Cell_Struct>(v_hNode_Cell,v_cNode_Cell));				//容器增加
				}
			}
			else														//Nodeb
			{
				//TD_LTE：Nodeb获取
				v_hNode_BTS=m_cTree.InsertItem(v_pNodeb->first+"     【"
					+v_pNodeb->second.v_sStation+"】",4,5,v_hRoot);		//树：增加子节点 [NodeB]
				for (v_pCell=v_pNodeb->second.v_mCell.begin();v_pCell!=v_pNodeb->second.v_mCell.end();v_pCell++)	//遍历：容器
				{
					v_hNode_Cell=m_cTree.InsertItem(v_pCell->first+"     【"+v_pCell->second+"】",0,1,v_hNode_BTS);	//树：增加子节点 [Cell]
					v_cNode_Cell.v_sCell=v_pCell->first;				//Cell
					v_cNode_Cell.v_sNetwork=v_pNodeb->second.v_sNetwork;//Network
					v_mTree_All.insert(pair<HTREEITEM,Node_Cell_Struct>(v_hNode_Cell,v_cNode_Cell));				//容器增加
				}
			}
		}

		//树展开
		v_hNode_BTS=m_cTree.GetRootItem();
		while (v_hNode_BTS!=NULL)
		{
			m_cTree.ExpandTree(v_hNode_BTS);							//树：展开
			v_hNode_BTS=m_cTree.GetNextSiblingItem(v_hNode_BTS);		//获得：邻居节点
		}

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	v_mNodeb.clear();													//容器清除
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Chart
////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------------
//	函数功能：Draw track cursor when mouse is moving over plotarea
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::OnMouseMovePlotArea()
{
	// Get the focus to ensure being able to receive mouse wheel events
	m_cChartView.SetFocus();

	m_cChartView.trackLineLabel((XYChart *)m_cChartView.getChart(), m_cChartView.getPlotAreaMouseX(),m_cLable.GetCheck()); 
	m_cChartView.updateDisplay();
}

//------------------------------------------------------------------------------------------------------
//	函数功能：鼠标轮滑动消息 [用途：放大、缩小、左右平移]
//------------------------------------------------------------------------------------------------------
BOOL CAll_PM_Draw::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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
void CAll_PM_Draw::OnViewPortChanged()
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
void CAll_PM_Draw::OnChart_Para()
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
//	函数功能：Tree树形控件条目变化响应
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		HTREEITEM v_hCurrent = m_cTree.GetSelectedItem();
		if(v_hCurrent!= NULL && !m_cTree.ItemHasChildren(v_hCurrent))	//选中Node为叶子？？
		{
			if (!m_cMuti_Line.GetCheck())								//单线？？？
			{
				if (v_hTree_Current != NULL && v_hTree_Current != v_hCurrent)							//去掉上次的选项
					m_cTree.SetItemState(v_hTree_Current,INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);//勾选无效
				v_hTree_Current=v_hCurrent;								//获取：Tree节点
				m_cTree.SetItemState(v_hTree_Current,INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK);	//勾选有效
				v_iWorking=2;											//工作任务：单线绘制
				v_pIterator_Frame->second.v_iThread_Exit=1;				//线程：工作
				v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);	//开启查询数据库线程
			}
			else
			{
				v_hTree_Current=v_hCurrent;								//获取：Tree节点
				if (m_cTree.GetItemState(v_hTree_Current,TVIS_STATEIMAGEMASK)>>12 == 1)
				{
					m_cTree.SetItemState(v_hTree_Current,INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK);//勾选有效
					m_set_Selected.insert(m_cTree.GetSelectedItem());	//容器中存储所选节点的信息
				}
				else
				{
					m_cTree.SetItemState(v_hTree_Current,INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);//勾选无效
					set<HTREEITEM>::iterator v_it_item = m_set_Selected.find(v_hTree_Current);          //检测是否有已选中节点被撤销
					if(v_it_item != m_set_Selected.end())				//
						m_set_Selected.erase(v_it_item);				//如果发现某一个原来选中的节点现在撤销了，我们就对容器的此节点擦除
				}
			}
		}
	}
	
	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：清除屏幕显示信息
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_Show_Clear()									    //清除屏幕显示信息
{
	m_cTree.DeleteAllItems();											//删除Tree控件信息
	
	v_hTree_Current=NULL;												//选中的Tree节点
	My_Chart_New();														//绘图复位
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击多线
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::OnBnClickedCkMuti()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cMuti_Line.GetCheck())										//多线条有效？
	{
		m_set_Selected.clear();											//多线时，容器清空；
		m_cDay.SetCheck(false);                                         //无效：单天；
		m_cKPI_S.SetCheck(false);										//无效：静态门限；
		if(v_hTree_Current != NULL)										//有效节点？？？
			v_hTree_Current = NULL;										//选中的节点：无效
	}
	OnBnClickedDelete();												//清除Chart
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：清除图形 [绘图复位]
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::OnBnClickedDelete()
{
	// TODO: 在此添加控件通知处理程序代码

	//清除Tree Node选中状态
	HTREEITEM item;
	for(item = m_cTree.GetRootItem();item != NULL;item = m_cTree.GetNextItem(item,TVGN_NEXT))
		My_Tree_Clear(item);								//清除Tree Node选中状态

	//绘图初始化
	My_Chart_New();														//绘图复位
}

//------------------------------------------------------------------------------------------------------
//	函数功能：清除Tree Node选中状态
//-----------------------------------------------------------------------------------------------------
void CAll_PM_Draw::My_Tree_Clear(HTREEITEM v_hNode)
{
	if(!m_cTree.ItemHasChildren(v_hNode))
	{
		m_cTree.SetItemState(v_hNode,INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);	//勾选无效
		return;
	}
	HTREEITEM hNextItem = m_cTree.GetChildItem(v_hNode);
	while (hNextItem != NULL)
	{
		My_Tree_Clear(hNextItem);
		hNextItem = m_cTree.GetNextItem(hNextItem, TVGN_NEXT);
	}
	if(!m_cTree.ItemHasChildren(v_hNode))
		m_cTree.SetItemState(v_hNode,INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);	//勾选无效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：绘图复位
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_Chart_New()
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
void CAll_PM_Draw::My_Chart_Muti()
{
	if(v_hTree_Current!=NULL)											//有效条目序号？？？
	{

		if(!m_cMuti_Line.GetCheck())									//单天
		{
			m_cMuti_Line.SetCheck(false);								//多线无效
			My_Chart();													//画线
		}
		if (m_cMuti_Line.GetCheck())									//多线
		{
			My_Chart_New();												//绘图复位
			for(set<HTREEITEM>::iterator v_itSel = m_set_Selected.begin();v_itSel != m_set_Selected.end();++v_itSel)
				My_Tree_Chart(*v_itSel);								//树遍历一遍，把选中的节点信息进行绘制
		}

	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：刷新【重新绘制曲线】
//------------------------------------------------------------------------------------------------------	
void CAll_PM_Draw::OnBnClickedRefresh()
{
	v_iWorking=3;														//工作任务：曲线绘制【单线、多线】
	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：绘图复位然后找当前节点
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_Chart()
{
	//绘图复位
	My_Chart_New();														//绘图复位

	//绘图：遍历Tree，找到选中的节点	
	My_Tree_Chart(m_cTree.GetSelectedItem());							//绘图：选中的节点

}

//------------------------------------------------------------------------------------------------------
//	函数功能：绘图：选中的Tree Node
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::My_Tree_Chart(HTREEITEM v_hNode)
{
	if(!m_cTree.ItemHasChildren(v_hNode))								//无孩子节点：？？？ 【即Cell节点，绘制性能曲线】
	{																	//无孩子节点
		if (m_cTree.GetItemState(v_hNode,TVIS_STATEIMAGEMASK)>>12 == 3)	//选中？？？
		{
			My_Tree_Chart_GetNode(v_hNode);								//获取：节点信息【保存在public变量中】
			My_Chart_IP();												//绘制：性能信息
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取节点信息 
//	参    数：v_hNode：节点句柄
//			 【用于：绘图、显示节点信息，保存在v_sNetwork、v_sCell变量中】
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_Tree_Chart_GetNode(HTREEITEM v_hNode)
{
	mNode_Cell::iterator v_pNode=v_mTree_All.find(v_hNode);	            //查找：节点信息	
	if (v_pNode != v_mTree_All.end())
	{
		v_sCell=v_pNode->second.v_sCell;								//Cell
		v_sNetwork=v_pNode->second.v_sNetwork;							//Netowrk
		v_hAll=v_hNode;                                                 //当然节点句柄
	}
}

//------------------------------------------------------------------------------------------------------
//	函数功能：Chart画一条线
//	参    数：选中的 IP [v_sItem_IP]
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::My_Chart_IP()
{
	mLTE_PM_Type::iterator	v_pSelect;									//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	COleDateTime	v_dDate_Start;										//查询开始日期
	m_cDate.GetTime(v_dDate_Start);										//获得：选择粒度日期
	v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//设置时间：00:00:00

	for (v_pSelect=v_mLTE_PM_Type.begin();v_pSelect!=v_mLTE_PM_Type.end();v_pSelect++)
	{
		for (v_pPM_Name=v_pSelect->second.v_mLTE_PM.begin();v_pPM_Name!=v_pSelect->second.v_mLTE_PM.end();v_pPM_Name++)
		{
			//读取性能数据库，并绘图
			if (v_pPM_Name->second.v_bOK)								//性能选中？？？
				My_Load_Performance(v_pSelect->first,					//性能类型
				v_pPM_Name->second.v_sTable_Key,						//性能文件名称key
				v_sCell,												//小区
				v_pPM_Name->second.v_sPM_Name,							//性能名称
				v_pPM_Name->first,										//性能字段
				v_dDate_Start,											//开始日期
				v_iChart_Day_Sum);										//查询天数	
		}
	}

	//绘图：Chart
	::PostMessage(m_hWnd, 123, (WPARAM) 0, (LPARAM) 0);					//发送消息：更新ChartView	
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：日期选择
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::OnDtnCloseupBegDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	m_cDate.GetTime(&v_dChart_Date);									//获得：查询开始日期

	My_Chart_New();														//绘图复位

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：天数选择
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iChart_Day_Sum=m_cDay_Sum.GetCurSel()+1;							//获得：查询天的数量

	My_Chart_New();														//绘图复位
}

//------------------------------------------------------------------------------------------------------
//	函数功能：	获取数据表中的KPI_Instance数据
//	参数：		v_sIP：IP；	v_dDate_Start：查询的起始日期；	v_iDay_Sum：查询天数
//	读取过程：	从对应表中获取数据，存入v_mIP_Instance容器中；[存放各个粒度的IP数据]
//------------------------------------------------------------------------------------------------------
void CAll_PM_Draw::My_Load_Performance(CString v_sType,CString v_sFile_Key,CString v_sCell,CString v_sPerformance,CString v_sField,COleDateTime v_dDate_Start,int v_iDay_Sum)
{
	CString			v_sSql,v_sTemp,v_sTime;								//临时变量
	CString			v_sTime_Start,v_sTime_End;							//日期
	int				v_iNum_Old=-1;										//记录：序号
	int				v_iTable_Num,v_iTable_Num_New;						//实时表序号
	CString			v_sInstance;										//Instance_ID
	COleDateTime	v_dTime;											//查询日期
	Chart_Data		v_cChart_Data;										//KPI粒度
	double			v_dValue;											//取值
	_variant_t Var;														//临时变量
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

		//从数据表中数据读取：1次读1个表的数据 [一个表为1月数据]
		int v_iDay=0;													//读取天数
		while(v_iDay<v_iDay_Sum)										//循环：查询天数
		{
			//读取：1个表的数据
			v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//开始时间
			while (true)
			{
				v_iTable_Num=v_dDate_Start.GetMonth();					//实时表序号：月份
				v_dTime=v_dDate_Start;									//1个表查询结束日期
				//天计数
				v_dDate_Start += COleDateTimeSpan(1,0,0,0);				//增加一天
				v_iDay++;												//
				if (v_iDay>=v_iDay_Sum)
					break;
				v_iTable_Num_New=v_dDate_Start.GetMonth();				//实时表序号：月份
				if (v_iTable_Num!=v_iTable_Num_New)
					break;
			}
			v_sTime_End=v_dTime.Format("%Y-%m-%d")+" 23:59:59";			//结束时间
			v_sNet="Pm_LTE_"+v_sFile_Key;								//文件名称
			if (m_cType.GetCurSel()==1)									//1H粒度？？？
			{
				v_sDate=v_dTime.Format("%Y%m");							//年月
				v_sNet+="_"+v_sDate;									//增加：年月
				//检查：表名称是否有效【依据：表名称集合v_mTable_Name】
				set<CString>::iterator v_pTable=v_mTable_Name.find(v_sNet);		//查找？？
				if (v_pTable==v_mTable_Name.end())						//没找到
					continue;											//无效返回
			}

			//字段处理【常规性能：一个性能字段；计算性能：多个性能字段】
			v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
			v_sSql+="select zTime,["+v_sField+"]";

			v_sSql+=" from "+v_sNet+" where zCell='"+ v_sCell+"' and zTime>='"
				+ v_sTime_Start +"' and  zTime<='"+ v_sTime_End +"' order by zTime";	//查询数据库：性能表
			v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);	//执行SELETE语句										

			while(!v_pRecordset->adoEOF)
			{
				//直接读取字段：目的是提高读取速度；[利用字段读取函数取值效率低！！！]
				Var=v_pRecordset->GetCollect((_variant_t)"zTime");		//日期
				if (Var.vt != VT_NULL)
					v_dTime=Var.date;									//
				else
					v_dTime=COleDateTime::GetCurrentTime();				//默认：当前日期

				//字段读取【常规性能：一个性能字段；计算性能：多个性能字段，并计算】
				v_dValue=v_cAdoConn.GetField_Double(v_sField);			//性能

				double	m_nextDataTime = Chart::chartTime(v_dTime.GetYear(),v_dTime.GetMonth(),v_dTime.GetDay(),v_dTime.GetHour(),v_dTime.GetMinute(),0);		//精确到分
				mTime_Array::iterator v_pIterator_Time=m_cChartView.v_mTime_Array.find(m_nextDataTime);					//查找粒度[依据m_nextDataTime]
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
//	函数功能：装载性能参数信息
//------------------------------------------------------------------------------------------------------			
void CAll_PM_Draw::My_LoadData_Para()
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

		//【1】性能类型
		v_sType="计算性能";												//性能类型								
		v_cLTE_PM_Type.v_sTable_Key=v_sType;							//表名称-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//容器增加

		//【2】获取：性能参数【常规】
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
				v_cLTE_PM.v_sTable_Key="Formula_Performance";			//性能：表名称
				v_cLTE_PM.v_bOK=false;									//性能：未选中
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//增加：性能参数
			}

			v_pRecordset->MoveNext();									//
		}

		//【3】获取：性能数据表名称【用于发现无效表名称】
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and name like 'Pm_LTE_Formula_Performance%' order by [name]";		//SQL
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

//-------------------------------------------------------------------------------------------------
// 导出处理：写入CSV文件，再用Excel打开；
//-------------------------------------------------------------------------------------------------
void CAll_PM_Draw::My_SendToFile() 
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
					v_pIterator_KPI=m_cChartView.v_mChart_Data.begin();//从头开始
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
					v_dKPI=v_pIterator_KPI->second.v_dChart_Value[vi];	//获得：KPI值
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
