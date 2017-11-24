// ConAnalyzeIndicator.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "ConAnalyzeIndicator.h"
#include "WebPage.h"													//封装了类CWebPage，用来和HTML，JSP交互
#include "comdef.h" 
#include <atlbase.h>

IMPLEMENT_DYNCREATE(CConAnalyzeIndicator, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CConAnalyzeIndicator::CConAnalyzeIndicator()
	: CFormView(CConAnalyzeIndicator::IDD)
{
	v_bShow=false;
	v_bLaod=false;														//HTML文件调用标志

	m_pIHTMLDoc		= NULL;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CConAnalyzeIndicator::~CConAnalyzeIndicator()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_Station);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_CHECK1, m_cOnline);
	DDX_Control(pDX, IDC_CHECK3, m_cOK);
	DDX_Control(pDX, IDC_EXPLORER, m_webBrowser);
	DDX_Control(pDX, IDC_BUTTON2, m_cMarker);
	DDX_Control(pDX, IDC_QUARY3, m_cDetail);
	DDX_Control(pDX, IDC_EXCEL, m_cClear);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CConAnalyzeIndicator, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CConAnalyzeIndicator::OnBnClickedQuary)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CConAnalyzeIndicator::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CConAnalyzeIndicator::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_EXCEL, &CConAnalyzeIndicator::OnBnClickedExcel)
	ON_BN_CLICKED(IDC_BUTTON2, &CConAnalyzeIndicator::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_QUARY3, &CConAnalyzeIndicator::OnBnClickedQuary3)
	ON_BN_CLICKED(IDC_EXCEL2, &CConAnalyzeIndicator::OnBnClickedExcel2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CConAnalyzeIndicator 诊断

#ifdef _DEBUG
void CConAnalyzeIndicator::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CConAnalyzeIndicator::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CConAnalyzeIndicator 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="分析指标";											//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	SetTimer(TIMER1, 1000, 0);
	time_control = true;
	//初始化：列表窗 [站址]
	::SendMessage(m_cList_Station.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Station.InsertColumn(0,"序号",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(1,"机房/资源点名称",LVCFMT_LEFT,260);	//
	m_cList_Station.InsertColumn(2,"区域",LVCFMT_LEFT,70);				//
	m_cList_Station.InsertColumn(3,"BTS",LVCFMT_RIGHT,60);				//
	m_cList_Station.InsertColumn(4,"Cell",LVCFMT_RIGHT,60);				//
	m_cList_Station.InsertColumn(5,"NodeB",LVCFMT_RIGHT,60);			//
	m_cList_Station.InsertColumn(6,"Cell",LVCFMT_RIGHT,60);				//
	m_cList_Station.InsertColumn(7,"E_NodeB",LVCFMT_RIGHT,60);			//
	m_cList_Station.InsertColumn(8,"Cell",LVCFMT_RIGHT,60);				//
	m_cList_Station.InsertColumn(9,"经度",LVCFMT_RIGHT,80);				//
	m_cList_Station.InsertColumn(10,"纬度",LVCFMT_RIGHT,80);			//
	m_cList_Station.SetExtendedStyle(m_cList_Station.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList_Station.SetNumber(3);										//数字比较
	m_cList_Station.SetNumber(4);
	m_cList_Station.SetNumber(5);
	m_cList_Station.SetNumber(6);
	m_cList_Station.SetNumber(7);
	m_cList_Station.SetNumber(8);
	m_cList_Station.SetNumber(9);
	m_cList_Station.SetNumber(10);

	//不显示js错误
	m_webBrowser.SetSilent(TRUE);
	m_webBrowser.Navigate("http://192.168.0.39:9002", NULL, NULL, NULL, NULL);	//加载html页面
	//显示标志
	PostMessage(WM_SIZE);												//最大化
	v_bShow=true;
	SetTimer(1,1000,NULL);												//1s定时   

	//信息初始化
	m_cKey_Select.AddString("机房/资源点");
	m_cKey_Select.AddString("区域");
	m_cKey_Select.SetCurSel(0);											//默认：选择第一个

	m_cOK.SetCheck(1);													//有效站址

	v_iList_Item_Room=-1;												//条目选择无效 【机房】
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CConAnalyzeIndicator::PreTranslateMessage(MSG* pMsg)
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
void CConAnalyzeIndicator::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!v_bLaod)
	{
		v_bLaod=true;													//置位：HTML调入标志
		PostMessage(WM_SIZE);											//最大化
	}
	switch (nIDEvent)
	{
	case TIMER1:
		{
			//if (time_control)
			//{
			//	time_control = false;
			//	//网页获得Dlg指针
			//	CComQIPtr<IHTMLDocument2> document = m_webBrowser.GetDocument();
			//	CComDispatchDriver script;
			//	document->get_Script(&script);
			//	CComVariant var(static_cast<IDispatch*>(this));
			//	script.Invoke1(L"SaveCppObject", &var);
			//}
			break;
		}
	case TIMER2:
		{
			time_control = false;
			break;
		}
	default:
		break;
	}
	CFormView::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：子窗销毁
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnDestroy()
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
void CConAnalyzeIndicator::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	CWnd	*pCellList;													//临时变量
	int		v_iY;														//临时变量
	pCellList = GetDlgItem(IDC_EXPLORER);
	v_iY = 50;															//赋值：Y轴
	if(pCellList)
	{
		CRect v_cRect;
		GetClientRect( &v_cRect );
		pCellList->MoveWindow(v_cRect);
	}	
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序 [站址]
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_SIZE);												//最大化

	m_cList_Station.My_Sort(pNMHDR);									//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：绘制Marker
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	My_Marker();
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：清除Marker
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CWebPage	web;													//生成一个页面
	web.SetDocument(m_webBrowser.GetDocument());						//赋值：网页指针
	web.CallJScript("myMarker_Clear");									//调用JS：清除Marker
	m_webBrowser.UpdateData();											//网页更新
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：站址导出
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show_Name(&m_cList_Station,"站址_统计",true);				//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击列表窗 [站址]
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;						//获得：条目指针
	if (pNMListView->iItem>=0)												//判断：条目序号有效？？？
	{
		CString str;
		if (v_iList_Item_Room != -1)
		{
			m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,255,255));		//恢复背景色
		} 		
		//首先得到点击的位置
		POSITION pos = m_cList_Station.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			return;
		}
		//得到行号，通过POSITION转化
		v_iList_Item_Room = (int)m_cList_Station.GetNextSelectedItem(pos);								//获得：选中条目序号
		m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,0,0));			//设置背景色：红色
		str = m_cList_Station.GetItemText(v_iList_Item_Room, 0);
		//显示站址位置
		CString content(str);
		content.Trim();
		if (!content.IsEmpty())
		{
			CWebPage	web;														//生成一个页面
			web.SetDocument(m_webBrowser.GetDocument());							//赋值：网页指针
			web.CallJScript("ShowStationSite", content);									//调用JS：查找位置
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：地图获取【站址】
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnBnClickedQuary3()
{
	My_Map_Station();
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询【站址】
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：查询
	//this->v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动	[静态门限计算]
//------------------------------------------------------------------------------------------------------
UINT CConAnalyzeIndicator::My_Thread_Inoput(LPVOID lparam)
{
	CConAnalyzeIndicator *  lp_this = NULL ;
	lp_this = (CConAnalyzeIndicator *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效
	lp_this->m_cDetail.EnableWindow(false);								//地图获取：无效
	lp_this->m_cMarker.EnableWindow(false);								//地图标记：无效
	lp_this->m_cClear.EnableWindow(false);								//标记清除：无效
	lp_this->m_cKey_Select.EnableWindow(false);							//机房选择：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效
	lp_this->m_cDetail.EnableWindow(true);								//地图获取：有效
	lp_this->m_cMarker.EnableWindow(true);								//地图标记：有效
	lp_this->m_cClear.EnableWindow(true);								//标记清除：有效
	lp_this->m_cKey_Select.EnableWindow(true);							//机房选择：有效

	//lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_Query_Station();												//站址查询
		break;
	case 2:
		My_Map_Station();												//地图获取
		break;
	case 3:
		My_Marker();													//绘制Marker
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：绘制Marker
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::My_Marker()
{
	// TODO: 在此添加控件通知处理程序代码
	CWebPage	web;													//生成一个页面
	web.SetDocument(m_webBrowser.GetDocument());						//赋值：网页指针
	for (int vi = 0; vi<m_cList_Station.GetItemCount(); vi++)				//遍历：列表窗
	{
		web.CallJScript("myMarker_Insert", m_cList_Station.GetItemText(vi, 9), m_cList_Station.GetItemText(vi, 10), m_cList_Station.GetItemText(vi, 1));	//调用JS：绘制Marker
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询机房信息
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::My_Map_Station()
{
	int				v_iID = 0;												//KPI数量
	CString			v_sSql, v_sTemp;											//临时变量

	//获取：地图屏幕经纬度
	CComVariant		v_sw_lng, v_sw_lat, v_ne_lng, v_ne_lat;					//返回值：四个经纬度  
	CComVariant		v_varResult;											//存放返回值
	CWebPage		v_cWeb;													//生成一个页面
	v_cWeb.SetDocument(m_webBrowser.GetDocument());							//赋值：网页指针
	v_cWeb.CallJScript("myGet_Longitude", &v_varResult);						//调用JS：地图屏幕经纬度
	CComDispatchDriver spData = v_varResult.pdispVal;						//获取：数组 
	spData.GetPropertyByName(L"sw_lng", &v_sw_lng);
	spData.GetPropertyByName(L"sw_lat", &v_sw_lat);
	spData.GetPropertyByName(L"ne_lng", &v_ne_lng);
	spData.GetPropertyByName(L"ne_lat", &v_ne_lat);
	double	v_dsw_lng = v_sw_lng.dblVal;
	double	v_dsw_lat = v_sw_lat.dblVal;
	double	v_dne_lng = v_ne_lng.dblVal;
	double	v_dne_lat = v_ne_lat.dblVal;

	//??????????????????????????如何准确？？？？？？？？？？？？？？？？？？？？？？？
	double x_pi = 3.1415926;
	double bd_x = v_dsw_lng - 0.0065;
	double bd_y = v_dsw_lat - 0.006;
	double z = sqrt(bd_x * bd_x + bd_y * bd_y) - 0.00002 * sin(bd_y * x_pi);
	double theta = atan2(bd_y, bd_x) - 0.000003 * cos(bd_x * x_pi);
	v_dsw_lng = z * cos(theta) - 0.005;
	v_dsw_lat = z * sin(theta) - 0.001;

	bd_x = v_dne_lng - 0.0065;
	bd_y = v_dne_lat - 0.006;
	z = sqrt(bd_x * bd_x + bd_y * bd_y) - 0.00002 * sin(bd_y * x_pi);
	theta = atan2(bd_y, bd_x) - 0.000003 * cos(bd_x * x_pi);
	v_dne_lng = z * cos(theta) - 0.005;
	v_dne_lat = z * sin(theta) - 0.001;

	CString v_sTop, v_sLeft, v_sBottom, v_sRight;							//字符串：四个经纬度
	v_sTop.Format("%8.6f", v_dsw_lng);
	v_sLeft.Format("%8.6f", v_dsw_lat);
	v_sBottom.Format("%8.6f", v_dne_lng);
	v_sRight.Format("%8.6f", v_dne_lat);

	v_sSql = "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT * FROM CON_Station";
	v_sSql += " where (zLongitude between " + v_sTop + " and " + v_sBottom + ") and (zLatitude between " + v_sLeft + " and " + v_sRight + ")";
	v_sSql += " order by zStation";
	My_LoadData_Station_Data(v_sSql);									//获取：站址信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询机房信息
//------------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::My_Query_Station()
{
	int				v_iID=0;											//KPI数量
	CString			v_sSql,v_sTemp;										//临时变量
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//查询日期准备
	m_cKey.GetWindowText(v_sLog_Key);										//获得：查询KEY
	v_sLog_Key.Trim();														//去掉空格

	v_sSql += "SELECT * FROM CON_Station"; 

	if (!v_sLog_Key.IsEmpty())												//查询KEY处理
	{
		switch (m_cKey_Select.GetCurSel())									//选择？？？
		{
		case 0:																//站址
			v_sSql = "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
			v_sSql += "SELECT * FROM CON_Station";
			v_sSql += " where (zStation like '%" + v_sLog_Key + "%')";
			break;
		case 1:		
			v_sSql = "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
			v_sSql += "SELECT * FROM CON_Station";
			v_sSql += " where (zRegion like '%" + v_sLog_Key + "%')";		//区域
			break;
		default:
			break;
		}
	}
	v_sSql+=" order by zStation";

	My_LoadData_Station_Data(v_sSql);									//获取：站址信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载站址信息
//------------------------------------------------------------------------------------------------------			
void CConAnalyzeIndicator::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem, v_iID = 0;									//KPI数量
	CString			v_sTemp;											//临时变量
	COleDateTime	v_dDate_Start;										//查询开始日期

	_variant_t	Var;
	CString			v_sStation, v_sRegion, v_sLongitude;					//临时变量
	CString			v_sLatitude, v_sOnline;								//临时变量
	double			v_dValue;											//临时变量
	int				v_iGSM_BTS, v_iGSM_Cell, v_iTD_NodeB, v_iTD_Cell, v_iLTE_NodeB, v_iLTE_Cell, v_iSum;	//临时变量
	int				v_iGSM_BTS_OUT, v_iGSM_Cell_OUT, v_iTD_NodeB_OUT, v_iTD_Cell_OUT, v_iLTE_NodeB_OUT, v_iLTE_Cell_OUT;	//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn		v_cAdoConn;											//数据库实例
	try
	{
		//列表复位
		m_cList_Station.DeleteAllItems();										//清除Items

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type, theApp.a_sDatabase_Ip, theApp.a_sDatabase_Name, theApp.a_sDatabase_User, theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while (!v_pRecordset->adoEOF)
		{
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_dValue = v_cAdoConn.GetField_Double("zLongitude");		//zLongitude
			v_sLongitude.Format("%10.6f", v_dValue);
			v_dValue = v_cAdoConn.GetField_Double("zLatitude");			//zLatitude
			v_sLatitude.Format("%10.6f", v_dValue);

			v_iGSM_BTS = 0;												//zGSM_BTS
			v_iGSM_Cell = 0;											//zGSM_Cell
			v_iTD_NodeB = 0;											//zTD_NodeB
			v_iTD_Cell = 0;												//zTD_Cell
			v_iLTE_NodeB = 0;											//zLTE_NodeB
			v_iLTE_Cell = 0;											//zLTE_Cell
			v_iGSM_BTS_OUT = 0;											//zGSM_BTS
			v_iGSM_Cell_OUT = 0;										//zGSM_Cell
			v_iTD_NodeB_OUT = 0;										//zTD_NodeB
			v_iTD_Cell_OUT = 0;											//zTD_Cell
			v_iLTE_NodeB_OUT = 0;										//zLTE_NodeB
			v_iLTE_Cell_OUT = 0;										//zLTE_Cell

			v_iGSM_BTS_OUT = v_cAdoConn.GetField_Int("zGSM_BTS_OUT");		//zGSM_BTS_OUT
			v_iGSM_Cell_OUT = v_cAdoConn.GetField_Int("zGSM_Cell_OUT");		//zGSM_Cell_OUT
			v_iTD_NodeB_OUT = v_cAdoConn.GetField_Int("zTD_NodeB_OUT");		//zTD_NodeB_OUT
			v_iTD_Cell_OUT = v_cAdoConn.GetField_Int("zTD_Cell_OUT");		//zTD_Cell_OUT
			v_iLTE_NodeB_OUT = v_cAdoConn.GetField_Int("zLTE_NodeB_OUT");	//zLTE_NodeB_OUT
			v_iLTE_Cell_OUT = v_cAdoConn.GetField_Int("zLTE_Cell_OUT");		//zLTE_Cell_OUT

			v_iGSM_BTS = v_cAdoConn.GetField_Int("zGSM_BTS");		//zGSM_BTS
			v_iGSM_Cell = v_cAdoConn.GetField_Int("zGSM_Cell");		//zGSM_Cell
			v_iTD_NodeB = v_cAdoConn.GetField_Int("zTD_NodeB");		//zTD_NodeB
			v_iTD_Cell = v_cAdoConn.GetField_Int("zTD_Cell");		//zTD_Cell
			v_iLTE_NodeB = v_cAdoConn.GetField_Int("zLTE_NodeB");	//zLTE_NodeB
			v_iLTE_Cell = v_cAdoConn.GetField_Int("zLTE_Cell");		//zLTE_Cell

																	//显示
			if (true)
			{
				v_iSum = v_iGSM_BTS + v_iTD_NodeB + v_iLTE_NodeB + v_iGSM_BTS_OUT + v_iTD_NodeB_OUT + v_iLTE_NodeB_OUT;	//基站总计
				if (v_iSum != 0)
				{
					v_sTemp.Format(" %04d", v_iID + 1);
					v_iItem = m_cList_Station.InsertItem(0xffff, v_sTemp);			//加入列表窗
					m_cList_Station.SetItemText(v_iItem, 1, v_sStation);			//机房/资源点名称
					m_cList_Station.SetItemText(v_iItem, 2, v_sRegion);			//区域
					v_sTemp.Format("%d", v_iGSM_BTS + v_iGSM_BTS_OUT);
					m_cList_Station.SetItemText(v_iItem, 3, v_sTemp);				//GSM_BTS
					v_sTemp.Format("%d", v_iGSM_Cell + v_iGSM_Cell_OUT);
					m_cList_Station.SetItemText(v_iItem, 4, v_sTemp);				//GSM_Cell
					v_sTemp.Format("%d", v_iTD_NodeB + v_iTD_NodeB_OUT);
					m_cList_Station.SetItemText(v_iItem, 5, v_sTemp);				//TD_NodeB
					v_sTemp.Format("%d", v_iTD_Cell + v_iTD_Cell_OUT);
					m_cList_Station.SetItemText(v_iItem, 6, v_sTemp);				//TD_Cell
					v_sTemp.Format("%d", v_iLTE_NodeB + v_iLTE_NodeB_OUT);
					m_cList_Station.SetItemText(v_iItem, 7, v_sTemp);				//LTE_NodeB
					v_sTemp.Format("%d", v_iLTE_Cell + v_iLTE_Cell_OUT);
					m_cList_Station.SetItemText(v_iItem, 8, v_sTemp);				//LTE_Cell
					m_cList_Station.SetItemText(v_iItem, 9, v_sLongitude);		//经度
					m_cList_Station.SetItemText(v_iItem, 10, v_sLatitude);		//纬度

					v_iID++;											//计数：告警数量
				}
			}

			v_pRecordset->MoveNext();									//

			if (v_iID % 100 == 0)											//显示：100的整数
			{
				v_sTemp.Format("%d", v_iID);								//
				m_cSum.SetWindowText(v_sTemp);							//显示：站址总数
			}
		}
		v_sTemp.Format("%d", v_iID);										//
		m_cSum.SetWindowText(v_sTemp);									//显示：站址总数

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch (_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}
}

//-------------------------------------------------------------------------------------------------
//	函数功能：JS调用响应【传递：地图屏幕经纬度，4个参数】
//	参    数：mStation_OutPut：容器指针
//-------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::My_Station_Select(CString v_sTop, CString v_sLeft,CString v_sBottom, CString v_sRight)
{
	CString		str;
	str.Format("第一个参数:%s,第二个参数:%s,第三个参数:%s,第四个参数:%s",v_sTop,v_sLeft,v_sBottom,v_sRight);
	MessageBox(str);
}

//=================================================================================================
//	函数功能：事件映射【浏览器控件--函数】
//=================================================================================================
BEGIN_EVENTSINK_MAP(CConAnalyzeIndicator, CFormView)
	//{{AFX_EVENTSINK_MAP(CMFCHtmlDlg)
	ON_EVENT(CConAnalyzeIndicator, IDC_EXPLORER, 259 /* DocumentComplete */, OnDocumentCompleteExplorerMain, VTS_DISPATCH VTS_PVARIANT)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

//-------------------------------------------------------------------------------------------------
//	函数功能：时间响应
//-------------------------------------------------------------------------------------------------
void CConAnalyzeIndicator::OnDocumentCompleteExplorerMain(LPDISPATCH pDisp, VARIANT FAR* URL) 
{
	// TODO: Add your control notification handler code here
	IDispatch*				pIDisp			= NULL;
	pIDisp = m_webBrowser.GetDocument();
	if(pIDisp)
	{
		if(m_pIHTMLDoc)
		{
			m_pIHTMLDoc->Release();
			m_pIHTMLDoc = NULL;
		}
		pIDisp->QueryInterface(IID_IHTMLDocument2, (void**)&m_pIHTMLDoc);
		pIDisp->Release();
		pIDisp = NULL;
	}
}

/***************************************************************************************************************
Description:函数拟定的唯一ID，这个ID可以取0-16384之间的任意数
****************************************************************************************************************/
enum
{
	FUNCTION_ShowMessageBox = 1,
	FUNCTION_GetProcessID = 2,
	FUNCTION_GetStationBounds = 3,
};

/***************************************************************************************************************
Description: 不用实现，直接返回E_NOTIMPL
****************************************************************************************************************/
HRESULT STDMETHODCALLTYPE CConAnalyzeIndicator::GetTypeInfoCount(UINT *pctinfo)
{
	return E_NOTIMPL;
}

/***************************************************************************************************************
Description: 不用实现，直接返回E_NOTIMPL
****************************************************************************************************************/
HRESULT STDMETHODCALLTYPE CConAnalyzeIndicator::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	return E_NOTIMPL;
}

/***************************************************************************************************************
Description: JavaScript调用这个对象的方法时，会把方法名放到rgszNames中，我们需要给这个方法名拟定一个唯一的数字ID，用
			 rgDispId传回给它。同理JavaScript存取这个对象的属性时，会把属性名放到rgszNames中，我们需要给这个属性名拟定
			 一个唯一的数字ID，用rgDispId传回给它，紧接着JavaScript会调用Invoke，并把这个ID作为参数传递进来
****************************************************************************************************************/
HRESULT STDMETHODCALLTYPE CConAnalyzeIndicator::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
	//rgszNames是个字符串数组，cNames指明这个数组中有几个字符串，如果不是1个字符串，忽略它
	if (cNames != 1)
		return E_NOTIMPL;
	//如果字符串是ShowMessageBox，说明JavaScript在调用我这个对象的ShowMessageBox方法，我就把我拟定的ID通过rgDispId告诉它
	if (wcscmp(rgszNames[0], L"ShowMessageBox") == 0)
	{
		*rgDispId = FUNCTION_ShowMessageBox;
		return S_OK;
	}
	//同理，如果字符串是GetProcessID，说明JavaScript在调用我这个对象的GetProcessID方法
	else if (wcscmp(rgszNames[0], L"GetProcessID") == 0)
	{
		*rgDispId = FUNCTION_GetProcessID; FUNCTION_GetStationBounds;
		return S_OK;
	}
	//同理，如果字符串是GetStationBounds，说明JavaScript在调用我这个对象的GetStationBounds方法
	else if (wcscmp(rgszNames[0], L"GetStationBounds") == 0)
	{
		*rgDispId = FUNCTION_GetStationBounds;
		return S_OK;
	}
	else
		return E_NOTIMPL;
}

/***************************************************************************************************************
Description:  JavaScript通过GetIDsOfNames拿到我的对象的方法的ID后，会调用Invoke，dispIdMember就是刚才我告诉它的我自己
			  拟定的ID，wFlags指明JavaScript对我的对象干了什么事情！如果是DISPATCH_METHOD，说明JavaScript在调用这个对
			  象的方法，比如cpp_object.ShowMessageBox();如果是DISPATCH_PROPERTYGET，说明JavaScript在获取这个对象的属
			  性，比如var n = cpp_object.num;如果是DISPATCH_PROPERTYPUT，说明JavaScript在修改这个对象的属性，比如
			  cpp_object.num = 10;如果是DISPATCH_PROPERTYPUTREF，说明JavaScript在通过引用修改这个对象，具体我也不懂
			  示例代码并没有涉及到wFlags和对象属性的使用，需要的请自行研究，用法是一样的。/pDispParams就是JavaScript调用
			  我的对象的方法时传递进来的参数，里面有一个数组保存着所有参数。pDispParams->cArgs就是数组中有多少个参数。
			  pDispParams->rgvarg就是保存着参数的数组，请使用[]下标来访问，每个参数都是VARIANT类型，可以保存各种类型的值
			  具体是什么类型用VARIANT::vt来判断，不多解释了，VARIANT这东西大家都懂，pVarResult就是我们给JavaScript的返回值
****************************************************************************************************************/
HRESULT STDMETHODCALLTYPE CConAnalyzeIndicator::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
	WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	//通过ID我就知道JavaScript想调用哪个方法
	if (dispIdMember == FUNCTION_ShowMessageBox)
	{
		//检查是否只有一个参数
		if (pDispParams->cArgs != 1)
			return E_NOTIMPL;
		//检查这个参数是否是字符串类型
		if (pDispParams->rgvarg[0].vt != VT_BSTR)
			return E_NOTIMPL;
		//放心调用
		ShowMessageBox(pDispParams->rgvarg[0].bstrVal);
		return S_OK;
	}
	else if (dispIdMember == FUNCTION_GetProcessID)
	{
		DWORD id = GetProcessID();
		*pVarResult = CComVariant(id);
		return S_OK;
	}
	else if (dispIdMember == FUNCTION_GetStationBounds)
	{
		GetStationBounds();
		return S_OK;
	}
	else
		return E_NOTIMPL;
}
/***************************************************************************************************************
Description: JavaScript拿到我们传递给它的指针后，由于它不清楚我们的对象是什么东西，会调用QueryInterface来询问我们“你是什
			 么鬼东西？”它会通过riid来问我们是什么东西，只有它问到我们是不是IID_IDispatch或我们是不是IID_IUnknown时，我
			 们才能肯定的回答它S_OK因为我们的对象继承于IDispatch，而IDispatch又继承于IUnknown，我们只实现了这两个接口，
			 所以只能这样来回答它的询问
****************************************************************************************************************/
HRESULT STDMETHODCALLTYPE CConAnalyzeIndicator::QueryInterface(REFIID riid, void **ppvObject)
{
	if (riid == IID_IDispatch || riid == IID_IUnknown)
	{
		//对的，我是一个IDispatch，把我自己(this)交给你
		*ppvObject = static_cast<IDispatch*>(this);
		return S_OK;
	}
	else
		return E_NOINTERFACE;
}

/***************************************************************************************************************
Description:  我们知道COM对象使用引用计数来管理对象生命周期，我们的CJsCallCppDlg对象的生命周期就是整个程序的生命周期
			  我的这个对象不需要你JavaScript来管，我自己会管，所以我不用实现AddRef()和Release()，这里乱写一些。
			  你要return 1;return 2;return 3;return 4;return 5;都可以
****************************************************************************************************************/
ULONG STDMETHODCALLTYPE CConAnalyzeIndicator::AddRef()
{
	return 1;
}

/***************************************************************************************************************
Description:  我们知道COM对象使用引用计数来管理对象生命周期，我们的CJsCallCppDlg对象的生命周期就是整个程序的生命周期
			  我的这个对象不需要你JavaScript来管，我自己会管，所以我不用实现AddRef()和Release()，这里乱写一些。
			  你要return 1;return 2;return 3;return 4;return 5都可以;当然如果你要new出一个c++对象来并扔给JavaScript来管，
			  你就需要实现AddRef()和Release()，在引用计数归零时delete this;
****************************************************************************************************************/
ULONG STDMETHODCALLTYPE CConAnalyzeIndicator::Release()
{
	return 1;
}

DWORD CConAnalyzeIndicator::GetProcessID()
{
	return GetCurrentProcessId();
}

void CConAnalyzeIndicator::ShowMessageBox(const wchar_t *msg)
{
	CString v_sPara;
	CString content;
    //处理字符串参数【szGBK为处理完成参数】
	int len = WideCharToMultiByte(CP_ACP, 0, msg, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, msg, -1, szGBK, len, NULL, NULL);

	content = (CString)szGBK + "hello c++!";
	v_sPara.Format("%s", content);
	MessageBox(v_sPara);

	delete[] szGBK;
}


void CConAnalyzeIndicator::GetStationBounds()
{
	int				v_iID = 0;												//KPI数量
	CString			v_sSql, v_sTemp,content;								//临时变量
	//获取：地图屏幕经纬度
	CComVariant		v_sw_lng, v_sw_lat, v_ne_lng, v_ne_lat;					//返回值：四个经纬度  
	CComVariant		v_varResult;											//存放返回值
	CWebPage		v_cWeb;													//生成一个页面
	v_cWeb.SetDocument(m_webBrowser.GetDocument());							//赋值：网页指针
	v_cWeb.CallJScript("myGet_Longitude", &v_varResult);					//调用JS：地图屏幕经纬度
	CComDispatchDriver spData = v_varResult.pdispVal;						//获取：数组 
	spData.GetPropertyByName(L"sw_lng", &v_sw_lng);
	spData.GetPropertyByName(L"sw_lat", &v_sw_lat);
	spData.GetPropertyByName(L"ne_lng", &v_ne_lng);
	spData.GetPropertyByName(L"ne_lat", &v_ne_lat);
	double	v_dsw_lng = v_sw_lng.dblVal;
	double	v_dsw_lat = v_sw_lat.dblVal;
	double	v_dne_lng = v_ne_lng.dblVal;
	double	v_dne_lat = v_ne_lat.dblVal;

	//??????????????????????????如何准确？？？？？？？？？？？？？？？？？？？？？？？
	double x_pi = 3.1415926;
	double bd_x = v_dsw_lng - 0.0065;
	double bd_y = v_dsw_lat - 0.006;
	double z = sqrt(bd_x * bd_x + bd_y * bd_y) - 0.00002 * sin(bd_y * x_pi);
	double theta = atan2(bd_y, bd_x) - 0.000003 * cos(bd_x * x_pi);
	v_dsw_lng = z * cos(theta) - 0.005;
	v_dsw_lat = z * sin(theta) - 0.001;

	bd_x = v_dne_lng - 0.0065;
	bd_y = v_dne_lat - 0.006;
	z = sqrt(bd_x * bd_x + bd_y * bd_y) - 0.00002 * sin(bd_y * x_pi);
	theta = atan2(bd_y, bd_x) - 0.000003 * cos(bd_x * x_pi);
	v_dne_lng = z * cos(theta) - 0.005;
	v_dne_lat = z * sin(theta) - 0.001;

	CString v_sTop, v_sLeft, v_sBottom, v_sRight;							//字符串：四个经纬度
	v_sTop.Format("%8.6f", v_dsw_lng);
	v_sLeft.Format("%8.6f", v_dsw_lat);
	v_sBottom.Format("%8.6f", v_dne_lng);
	v_sRight.Format("%8.6f", v_dne_lat);

	//显示GPS经纬度
	content = v_sBottom + "," + v_sLeft + "," + v_sTop + "," + v_sRight;
	content.Trim();
	if (!content.IsEmpty())
	{
		CWebPage	web;														//生成一个页面
		web.SetDocument(m_webBrowser.GetDocument());							//赋值：网页指针
		web.CallJScript("ShowBounds", content);									//调用JS：查找位置
	}
}
//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
