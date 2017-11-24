// QUA_Station_Map.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "QUA_Station_Map.h"
#include "WebPage.h"													//封装了类CWebPage，用来和HTML，JSP交互


// CQUA_Station_Map

IMPLEMENT_DYNCREATE(CQUA_Station_Map, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CQUA_Station_Map::CQUA_Station_Map()
	: CFormView(CQUA_Station_Map::IDD)
{
	m_pIHTMLDoc		= NULL;												//
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CQUA_Station_Map::~CQUA_Station_Map()
{
	My_Clear();															//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_Station);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_TREE1, m_cTree_Room);
	DDX_Control(pDX, IDC_LOG_SUM, m_cSum_Remote);
	DDX_Control(pDX, IDC_EXPLORER, m_webBrowser);
	DDX_Control(pDX, IDC_CHECK2, m_cBTS);
	DDX_Control(pDX, IDC_BUTTON2, m_cMarker);
	DDX_Control(pDX, IDC_BUTTON5, m_cMap_Clear);
	DDX_Control(pDX, IDC_BUTTON8, m_cMap_Get);
	DDX_Control(pDX, IDC_COMBO12, m_cSelect);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CQUA_Station_Map, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CQUA_Station_Map::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL2, &CQUA_Station_Map::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CQUA_Station_Map::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CQUA_Station_Map::OnLvnColumnclickList3)
	ON_CBN_CLOSEUP(IDC_COMBO11, &CQUA_Station_Map::OnCbnCloseupCombo11)
	ON_BN_CLICKED(IDC_BUTTON2, &CQUA_Station_Map::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CQUA_Station_Map::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON8, &CQUA_Station_Map::OnBnClickedButton7)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST3, &CQUA_Station_Map::OnLvnGetdispinfoList3)

	ON_MESSAGE(123,&CQUA_Station_Map::My_Map_Marker_Show)					//添加消息映射：MAp Marker 

END_MESSAGE_MAP()


// CQUA_Station_Map 诊断

#ifdef _DEBUG
void CQUA_Station_Map::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQUA_Station_Map::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQUA_Station_Map 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="站址地图查询";										//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗 [站址]
	::SendMessage(m_cList_Station.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Station.InsertColumn(0,"序号",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(1,"机房/资源点名称",LVCFMT_LEFT,300);	//
	m_cList_Station.InsertColumn(2,"区域",LVCFMT_LEFT,80);				//
	m_cList_Station.InsertColumn(3,"站型",LVCFMT_CENTER,50);			//
	m_cList_Station.InsertColumn(4,"覆盖",LVCFMT_CENTER,50);			//
	m_cList_Station.InsertColumn(5,"宏_BTS",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(6,"宏_NodeB",LVCFMT_CENTER,80);		//
	m_cList_Station.InsertColumn(7,"宏_E_NodeB",LVCFMT_CENTER,90);		//
	m_cList_Station.InsertColumn(8,"拉远站",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(9,"拉_BTS",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(10,"拉_NodeB",LVCFMT_CENTER,80);		//
	m_cList_Station.InsertColumn(11,"拉_E_NodeB",LVCFMT_CENTER,90);		//
	m_cList_Station.InsertColumn(12,"从站",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(13,"从_BTS",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(14,"从_NodeB",LVCFMT_CENTER,80);		//
	m_cList_Station.InsertColumn(15,"从_E_NodeB",LVCFMT_CENTER,90);		//
	m_cList_Station.InsertColumn(16,"状态",LVCFMT_LEFT,60);				//
	m_cList_Station.InsertColumn(17,"经度",LVCFMT_RIGHT,80);			//
	m_cList_Station.InsertColumn(18,"纬度",LVCFMT_RIGHT,80);			//
	m_cList_Station.InsertColumn(19,"类型",LVCFMT_LEFT,120);			//
	m_cList_Station.InsertColumn(20,"地址",LVCFMT_LEFT,300);			//
	m_cList_Station.InsertColumn(21,"入网日期",LVCFMT_CENTER,100);		//
	m_cList_Station.InsertColumn(22,"更新日期",LVCFMT_CENTER,100);		//
	m_cList_Station.SetExtendedStyle(m_cList_Station.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList_Station.SetNumber(5);										//设置：数字模式
	m_cList_Station.SetNumber(6);										//设置：数字模式
	m_cList_Station.SetNumber(7);										//设置：数字模式
	m_cList_Station.SetNumber(9);										//设置：数字模式
	m_cList_Station.SetNumber(10);										//设置：数字模式
	m_cList_Station.SetNumber(11);										//设置：数字模式
	m_cList_Station.SetNumber(12);										//设置：数字模式
	m_cList_Station.SetNumber(13);										//设置：数字模式
	m_cList_Station.SetNumber(14);										//设置：数字模式
	m_cList_Station.SetNumber(15);										//设置：数字模式

	m_cList_Station.SetNumber(17);										//设置：数字模式
	m_cList_Station.SetNumber(18);										//设置：数字模式
	
	v_iCol_Lon=17;														//经度列号
	v_iCol_Lat=18;														//纬度列号

	//树初始化：m_cTree
	v_cImage_Tree.Create(IDB_TREE_STATION_PG,16,1,RGB(255,255,255));	//树形图象列表生成
	//	v_cImage_State.Create(IDB_TREE_CHECK,13, 1, RGB(255,255,255));
	m_cTree_Room.SetImageList(&v_cImage_Tree,TVSIL_NORMAL);				//树初始化
	m_cTree_Room.SetImageList(&v_cImage_State,TVSIL_STATE);
	m_cTree_Room.SetBkColor(RGB(200,200,255));							//设置树形控件的背景色
	m_cTree_Room.SetTextColor(RGB(127,0,0));							//设置文本背景色

	//分窗
	CRect	v_cRect;													//矩形
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//获得：指针
	pWnd->GetWindowRect(&v_cRect);										//获得：尺寸
	ScreenToClient(&v_cRect);											//坐标变换
	pWnd->DestroyWindow();												//销毁：指针

	//	this creates pane containing list control on the top and edit on the bottom
 	m_SplitterPane_Bottom.Create(											//垂直线
 		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
 		this,					// the parent of the splitter pane
 		&m_cTree_Room,			// Top pane
 		&m_webBrowser,			// Bottom pane
 		IDC_QUA_Station_Map_Tree,	// this ID is used for saving/restoring splitter position and it must be unique !
 		v_cRect					// dimensions of the splitter pane
 		);
 	m_SplitterPane_Bottom.m_bVertSplitter_Parent=true;					//垂直分隔符为父窗
	m_SplitterPane_Main.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList_Station,		// Top pane
		&m_SplitterPane_Bottom,	// Bottom pane
		IDC_QUA_Station_Map,		// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);

	//查询类型
	m_cKey_Select.AddString("机房/资源点");
	m_cKey_Select.AddString("区域");
	m_cKey_Select.AddString("地址");
	m_cKey_Select.SetCurSel(0);											//默认：选择第一个

	//Tree显示类型
	m_cSelect.AddString("机房/拉远");
	m_cSelect.AddString("基站");
	m_cSelect.AddString("小区");
	m_cSelect.SetCurSel(2);												//默认

	//MAP初始化：加载网页信息
	m_webBrowser.Navigate(theApp.m_sHTTP_Map+"/Map_Station_Show.html",NULL,NULL,NULL,NULL);		//加载html页面
	v_bMap=false;														//绘图完毕

	PostMessage(WM_SIZE);												//最大化

	v_iList_Item_Room=-1;												//条目选择无效 【机房】
	m_cBTS.SetCheck(true);												//置位：内含基站

	//无效：在装载网页期间
	m_cQuery.EnableWindow(false);										//查询：无效
	m_cExcel_NodeB.EnableWindow(false);									//导出：无效
	m_cMarker.EnableWindow(false);										//地图标记：无效
	m_cMap_Clear.EnableWindow(false);									//地图清除：无效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CQUA_Station_Map::PreTranslateMessage(MSG* pMsg)
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
void CQUA_Station_Map::OnDestroy()
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
void CQUA_Station_Map::OnSize(UINT nType, int cx, int cy)
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
//	函数功能：站址导出
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(v_iList_Item_Room>=0)											//有效条目序号？？？
	{
		m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,255,255));	//恢复背景色
		v_iList_Item_Room=-1;											//条目选择无效 【机房】
	}

	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Station);										//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序 [机房][虚拟列表]
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(v_iList_Item_Room>=0)											//有效条目序号？？？
	{
		m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,255,255));	//恢复背景色
		v_iList_Item_Room=-1;											//条目选择无效 【机房】
	}

	m_cList_Station.My_Sort_Virtual(pNMHDR);							//列表排序[虚拟列表]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：虚拟列表显示数据获取
//	工	  作：依据列表需要的元素参数(行、列)，在数据容器中查询数据，赋值到列表中。
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	m_cList_Station.Get_List_Data(pDispInfo);							//虚拟列表显示数据获取

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：绘制Marker
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::OnBnClickedButton2()
{
	v_iWorking=2;														//工作任务：地图显示标记信息

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：地图显示标记信息
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::My_Show_Marker()
{
	// TODO: 在此添加控件通知处理程序代码
	CString	v_sLon,v_sLat,v_sType,v_sCover;								//临时变量
	double	v_dLat = 0;													//临时变量
	double	v_dLon = 0;													//临时变量

	if(v_iList_Item_Room>=0)											//有效条目序号？？？
	{
		m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,255,255));	//恢复背景色
		v_iList_Item_Room=-1;											//无选择条目
	}

	for (int vi=0;vi<m_cList_Station.GetItemCount();vi++)				//遍历：列表窗
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//强制退出？？？
			break;														//退出

		v_sType=m_cList_Station.GetItemText(vi,3);						//获取：站型
		v_sCover=m_cList_Station.GetItemText(vi,4);						//获取：覆盖
		//获取：坐标
		v_sLon=m_cList_Station.GetItemText(vi,v_iCol_Lon);				//获取：经度
		v_dLon=atof(v_sLon);											//
		v_sLat=m_cList_Station.GetItemText(vi,v_iCol_Lat);				//获取：维度
		v_dLat=atof(v_sLat);											//

		//设置背景色
		m_cList_Station.SetItemBkColor(vi,RGB(255,192,203));			//恢复背景色：粉色

		//绘制标记
		My_Map_Marker(v_dLon,v_dLat,m_cList_Station.GetItemText(vi,1),v_sType,v_sCover);	//调用JS：绘制Marker

		m_cList_Station.EnsureVisible(vi,false);						//显示最新一条记录
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：显示Marker(线程)
//	参	  数：v_dLongitude：经度；	v_dLatitude：纬度；	v_sName：机房名称；	v_iType：图标类型；
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::My_Map_Marker(double v_dLongitude,double v_dLatitude,CString v_sName,CString v_sType,CString v_sCover)
{
	// TODO: 在此添加控件通知处理程序代码
	v_dMap_Lon=v_dLongitude;											//经度
	v_dMap_Lat=v_dLatitude;												//纬度
	v_sMap_Name = v_sName;												//基站名称
	v_sMap_Type = v_sType;												//类型
	v_sMap_Cover = v_sCover;											//覆盖

	while(v_bMap)
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//强制退出？？？
			break;														//退出

		Sleep(10);														//延时
		continue;														//继续
	}
	if (!v_bMap)
	{
		v_bMap=true;													//绘图开始
		::PostMessage(m_hWnd, 123, (WPARAM) 0, (LPARAM) 0);				//发送消息：更新Map	
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：显示Marker(消息)
//	工    作：【(1)在线程，经纬度数据准备；(2)发送消息；(3)消息响应，在用户界面绘制地图 Marker；】
//------------------------------------------------------------------------------------------------------
LRESULT CQUA_Station_Map::My_Map_Marker_Show(WPARAM iParam1,LPARAM iParam2)
{
	int		v_iSelect;													//临时变量

	//【1】显示Marker
	if (v_sMap_Type=="宏站")
	{
		if (v_sMap_Cover=="室外")
			v_iSelect=0;												//调用JS：绘制Marker(宏站)
		else if (v_sMap_Cover=="室内")
			v_iSelect=1;												//调用JS：绘制Marker(宏站)
		else
			v_iSelect=2;												//调用JS：绘制Marker(宏站)
	}
	else
	{
		if (v_sMap_Cover=="室外")
			v_iSelect=3;												//调用JS：绘制Marker(拉远)
		else if (v_sMap_Cover=="室内")
			v_iSelect=4;												//调用JS：绘制Marker(拉远)
		else
			v_iSelect=5;												//调用JS：绘制Marker(拉远)
	}
	My_Map_Marker_Direct(v_dMap_Lon,v_dMap_Lat,v_sMap_Name,v_iSelect);	//调用JS：绘制Marker(拉远)

	//【2】置标志
	v_bMap=false;														//绘图完毕

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：显示Marker(直接绘制Marker)
//	参	  数：v_dLongitude：经度；	v_dLatitude：纬度；	v_sName：机房名称；	v_iType：图标类型；
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::My_Map_Marker_Direct(double v_dLongitude,double v_dLatitude,CString v_sName,int v_iType)
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sLon,v_sLat;											//临时变量

	double	v_dMarsLat = 0;												//临时变量
	double	v_dMarsLon = 0;												//临时变量
	double	v_dResultLat = 0;											//临时变量
	double	v_dResultLon = 0;											//临时变量

	//坐标转换
	CMyMap_Baidu::transform2Mars(v_dLatitude,v_dLongitude,v_dMarsLat,v_dMarsLon);	//地球坐标转换为火星坐标		
	CMyMap_Baidu::bd_encrypt(v_dMarsLat,v_dMarsLon,v_dResultLat,v_dResultLon);		//火星坐标转换为百度坐标

	v_sLon.Format("%8.6f",v_dResultLon);								//经度
	v_sLat.Format("%8.6f",v_dResultLat);								//纬度

	//【1】显示Marker
	CWebPage	web;													//生成一个页面
	web.SetDocument(m_webBrowser.GetDocument());						//赋值：网页指针

	switch(v_iType)														//正常图标
	{
	case 0:																//宏站：室外
		web.CallJScript("myMarker_Insert_H0",v_sLon,v_sLat,v_sName);	//调用JS：绘制Marker
		break;
	case 1:																//宏站：室内
		web.CallJScript("myMarker_Insert_H1",v_sLon,v_sLat,v_sName);	//调用JS：绘制Marker
		break;
	case 2:																//宏站：室内外
		web.CallJScript("myMarker_Insert_H2",v_sLon,v_sLat,v_sName);	//调用JS：绘制Marker
		break;
	case 3:																//拉远站：室外
		web.CallJScript("myMarker_Insert_R0",v_sLon,v_sLat,v_sName);	//调用JS：绘制Marker
		break;
	case 4:																//拉远站：室内
		web.CallJScript("myMarker_Insert_R1",v_sLon,v_sLat,v_sName);	//调用JS：绘制Marker
		break;
	case 5:																//拉远站：室内外
		web.CallJScript("myMarker_Insert_R2",v_sLon,v_sLat,v_sName);	//调用JS：绘制Marker
		break;
	case 6:																//拉远站：从站：宏站
		web.CallJScript("myMarker_Insert_S0",v_sLon,v_sLat,v_sName);	//调用JS：绘制Marker
		break;
	case 7:																//拉远站：从站：拉远
		web.CallJScript("myMarker_Insert_S1",v_sLon,v_sLat,v_sName);	//调用JS：绘制Marker
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：清除Marker
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CWebPage	web;													//生成一个页面
	web.SetDocument(m_webBrowser.GetDocument());						//赋值：网页指针

	web.CallJScript("myMarker_Clear");									//调用JS：清除Marker
	m_webBrowser.UpdateData();											//网页更新
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：地图画线
//	参	  数：v_dLon_0：起点经度；	v_dLat_0：起点纬度；	v_dLon_1：终点经度；	v_dLat_1：终点纬度；
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::My_Map_Line(double v_dLon_0,double v_dLat_0,double v_dLon_1,double v_dLat_1)
{
	// TODO: 在此添加控件通知处理程序代码
	CStringArray	v_sData;											//临时变量
	CString			v_sTemp;											//临时变量
	double	v_dMarsLat = 0;												//临时变量
	double	v_dMarsLon = 0;												//临时变量
	double	v_dResultLat = 0;											//临时变量
	double	v_dResultLon = 0;											//临时变量

	CWebPage	web;													//生成一个页面
	web.SetDocument(m_webBrowser.GetDocument());						//赋值：网页指针

	//坐标转换
	CMyMap_Baidu::transform2Mars(v_dLat_0,v_dLon_0,v_dMarsLat,v_dMarsLon);		//地球坐标转换为火星坐标		
	CMyMap_Baidu::bd_encrypt(v_dMarsLat,v_dMarsLon,v_dResultLat,v_dResultLon);	//火星坐标转换为百度坐标

	v_sTemp.Format("%8.6f",v_dResultLon);								//字符串
	v_sData.Add(v_sTemp);
	v_sTemp.Format("%8.6f",v_dResultLat);								//字符串
	v_sData.Add(v_sTemp);

	//坐标转换
	CMyMap_Baidu::transform2Mars(v_dLat_1,v_dLon_1,v_dMarsLat,v_dMarsLon);		//地球坐标转换为火星坐标		
	CMyMap_Baidu::bd_encrypt(v_dMarsLat,v_dMarsLon,v_dResultLat,v_dResultLon);	//火星坐标转换为百度坐标

	v_sTemp.Format("%8.6f",v_dResultLon);								//字符串
	v_sData.Add(v_sTemp);
	v_sTemp.Format("%8.6f",v_dResultLat);								//字符串
	v_sData.Add(v_sTemp);

	web.CallJScript("ShowLine",v_sData);								//调用JS：画线
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：设置地图中心点
//	参	  数：v_dRoom_Lon：中心点经度；	v_dRoom_Lat：中心纬度；	v_iLevel：地图层级；
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::My_Set_Center(double v_dRoom_Lon,double v_dRoom_Lat,int v_iLevel)
{
	// TODO: 在此添加控件通知处理程序代码
	CString	v_sLon,v_sLat,v_sLevel;										//临时变量
	double	v_dMarsLat = 0;												//临时变量
	double	v_dMarsLon = 0;												//临时变量
	double	v_dResultLat = 0;											//临时变量
	double	v_dResultLon = 0;											//临时变量

	CWebPage	web;													//生成一个页面
	web.SetDocument(m_webBrowser.GetDocument());						//赋值：网页指针

	//坐标转换
	CMyMap_Baidu::transform2Mars(v_dRoom_Lat,v_dRoom_Lon,v_dMarsLat,v_dMarsLon);	//地球坐标转换为火星坐标		
	CMyMap_Baidu::bd_encrypt(v_dMarsLat,v_dMarsLon,v_dResultLat,v_dResultLon);		//火星坐标转换为百度坐标

	v_sLon.Format("%8.6f",v_dResultLon);								//字符串
	v_sLat.Format("%8.6f",v_dResultLat);								//字符串
	v_sLevel.Format("%d",v_iLevel);										//字符串

	web.CallJScript("MySet_Center",v_sLon,v_sLat,v_sLevel);				//调用JS：设置地图中心点
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击列表窗 [机房]
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sRoom,v_sCovet_Type;									//临时变量
	double		v_dRoom_Lon,v_dRoom_Lat;								//临时变量

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item_Room>=0)									//有效条目序号？？？
				m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,255,255));	//恢复背景色
			v_iList_Item_Room=pNMListView->iItem;						//获得：选中条目序号
			m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,0,0));			//设置背景色：红色
			
			My_Clear();													//容器清除
			v_sRoom=m_cList_Station.GetItemText(v_iList_Item_Room,1);	//获取：机房
			v_sCovet_Type=m_cList_Station.GetItemText(v_iList_Item_Room,4);			//获取：覆盖
			v_dRoom_Lon=atof(m_cList_Station.GetItemText(v_iList_Item_Room,v_iCol_Lon));	//获取：经度
			v_dRoom_Lat=atof(m_cList_Station.GetItemText(v_iList_Item_Room,v_iCol_Lat));	//获取：纬度
			My_LoadData_Tree(v_sRoom,v_sCovet_Type,v_dRoom_Lon,v_dRoom_Lat);				//装载机房信息
			My_Show_Tree();												//显示：Tree信息
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：下拉列表变化【树展开方式】
//------------------------------------------------------------------------------------------------------			
void CQUA_Station_Map::OnCbnCloseupCombo11()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cSelect.GetCurSel()==0)
	{
		m_cTree_Room.CloseTree(v_hBTS_Root);							//树：收缩
		m_cTree_Room.Expand(v_hBTS_Root,TVE_EXPAND);					//树：展开到机房
	}
	else if (m_cSelect.GetCurSel()==1)
	{
		m_cTree_Room.CloseTree(v_hBTS_Root);							//树：收缩
		m_cTree_Room.ExpandTree_Level(v_hBTS_Root,2);					//树：展开2级
	}
	else if (m_cSelect.GetCurSel()==2)
		m_cTree_Room.ExpandTree(v_hBTS_Root);							//树：全展开(Cell)
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：地图获取
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	My_Map_Station();
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CQUA_Station_Map::My_Thread_Inoput(LPVOID lparam)
{
	CQUA_Station_Map *  lp_this = NULL ;
	lp_this = (CQUA_Station_Map *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel_NodeB.EnableWindow(false);						//导出：无效
	lp_this->m_cMarker.EnableWindow(false);								//地图标记：无效
	lp_this->m_cMap_Get.EnableWindow(false);							//地图获取：无效
	lp_this->m_cMap_Clear.EnableWindow(false);							//地图清除：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel_NodeB.EnableWindow(true);							//导出：有效
	lp_this->m_cMarker.EnableWindow(true);								//地图标记：有效
	lp_this->m_cMap_Get.EnableWindow(true);								//地图获取：有效
	lp_this->m_cMap_Clear.EnableWindow(true);							//地图清除：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_Query_Station();												//站址查询
		break;
	case 2:
		My_Show_Marker();												//地图显示标记信息
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询机房信息
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::My_Query_Station()
{
	int				v_iID=0;											//KPI数量
	CString			v_sSql,v_sTemp;										//临时变量
	CString			v_sLog_Key;
	bool			v_bOK=false;										//

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//查询日期准备
	m_cKey.GetWindowText(v_sLog_Key);									//获得：查询KEY
	v_sLog_Key.Replace("[","[[]");										//限制处理
	v_sLog_Key.Replace("'","");											//限制处理
	v_sLog_Key.Trim();													//去掉空格

	v_sSql += "SELECT * FROM CON_Station"; 

	if (!v_sLog_Key.IsEmpty())											//查询KEY处理
	{
		v_bOK=true;														//
		switch (m_cKey_Select.GetCurSel())								//选择？？？
		{
		case 0:															//站址
			v_sSql+=" where zStation like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//区域
			v_sSql+=" where zRegion like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//地址
			v_sSql+=" where zAddress like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	if (m_cBTS.GetCheck())
	{
		if (v_bOK)
			v_sSql+=" and zOK='1'";
		else
			v_sSql+=" where zOK='1'";
	}
	v_sSql+=" order by zStation";

	My_LoadData_Station_Data(v_sSql);									//获取：站址信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询机房信息
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::My_Map_Station()
{
	int				v_iID=0;											//临时变量
	CString			v_sSql,v_sTemp;										//临时变量

	//【1】获取：地图屏幕经纬度
	CComVariant		v_sw_lng,v_sw_lat,v_ne_lng,v_ne_lat;				//返回值：四个经纬度  
	CComVariant		v_varResult;										//存放返回值
	CWebPage		v_cWeb;												//生成一个页面
	v_cWeb.SetDocument(m_webBrowser.GetDocument());						//赋值：网页指针
	v_cWeb.CallJScript("myGet_Longitude",&v_varResult);					//调用JS：地图屏幕经纬度

	CComDispatchDriver spData = v_varResult.pdispVal;					//获取：数组 
	spData.GetPropertyByName(L"sw_lng", &v_sw_lng);  
	spData.GetPropertyByName(L"sw_lat", &v_sw_lat);  
	spData.GetPropertyByName(L"ne_lng", &v_ne_lng);  
	spData.GetPropertyByName(L"ne_lat", &v_ne_lat);  
	double	v_dsw_lng=v_sw_lng.dblVal;
	double	v_dsw_lat=v_sw_lat.dblVal;
	double	v_dne_lng=v_ne_lng.dblVal;
	double	v_dne_lat=v_ne_lat.dblVal;

	//【2】座标转换
	CMyMap_Baidu::transform2bd(v_dsw_lng,v_dsw_lat,v_dne_lng,v_dne_lat);//百度座标==〉经纬度	

	//【3】站址查询
	CString v_sTop,v_sLeft,v_sBottom,v_sRight;							//字符串：四个经纬度
	v_sTop.Format("%8.6f",v_dsw_lng);
	v_sLeft.Format("%8.6f",v_dsw_lat);
	v_sBottom.Format("%8.6f",v_dne_lng);
	v_sRight.Format("%8.6f",v_dne_lat);

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	v_sSql += "SELECT * FROM CON_Station"; 
	v_sSql+=" where zOK='1' and (zLongitude between "+ v_sTop +" and "+ v_sBottom +") and (zLatitude between "+ v_sLeft +" and "+ v_sRight +")";
	v_sSql+=" order by zStation";

	My_LoadData_Station_Data(v_sSql);									//获取：站址信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载站址信息
//------------------------------------------------------------------------------------------------------			
void CQUA_Station_Map::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iID=0;											//临时变量
	int				v_iLoc_BTS,v_iLoc_NodeB,v_iLoc_E_NodeB;				//临时变量
	int				v_iRem_BTS,v_iRem_NodeB,v_iRem_E_NodeB,v_iRemote;	//临时变量
	int				v_iSla_BTS,v_iSla_NodeB,v_iSla_E_NodeB,v_iSlave;	//临时变量
	int				v_iStation;											//
	CString			v_sTemp,v_sCover_Type,v_sRemote;					//临时变量
	bool			v_bBTS=true;										//临时变量
	COleDateTime	v_dDate_Start;										//查询开始日期

	_variant_t	Var;
	CString			v_sStation,v_sType,v_sRegion,v_sAddress,v_sLongitude;		//临时变量
	CString			v_sLatitude,v_sOnline,v_sDate,v_sCurrentTime;		//临时变量
	double			v_dValue;											//临时变量

	//虚拟列表
	mInt_String		v_mLine_Data;										//虚拟列表行数据
	m_cList_Station.Put_Line_Begin();									//列表添加数据开始

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList_Station.DeleteAllItems();								//清除Items
		m_cTree_Room.DeleteAllItems();									//树清除
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
			v_sCover_Type = v_cAdoConn.GetField_String("zCover_Type");	//zCover_Type
			v_sRemote = v_cAdoConn.GetField_String("zRemote");			//zRemote
			v_iLoc_BTS = v_cAdoConn.GetField_Int("zLoc_BTS");			//zLoc_BTS
			v_iLoc_NodeB = v_cAdoConn.GetField_Int("zLoc_NodeB");		//zLoc_NodeB
			v_iLoc_E_NodeB = v_cAdoConn.GetField_Int("zLoc_E_NodeB");	//zLoc_E_NodeB
			v_iRem_BTS = v_cAdoConn.GetField_Int("zRem_BTS");			//zRem_BTS
			v_iRem_NodeB = v_cAdoConn.GetField_Int("zRem_NodeB");		//zRem_NodeB
			v_iRem_E_NodeB = v_cAdoConn.GetField_Int("zRem_E_NodeB");	//zRem_E_NodeB
			v_iSla_BTS = v_cAdoConn.GetField_Int("zSla_BTS");			//zSla_BTS
			v_iSla_NodeB = v_cAdoConn.GetField_Int("zSla_NodeB");		//zSla_NodeB
			v_iSla_E_NodeB = v_cAdoConn.GetField_Int("zSla_E_NodeB");	//zSla_E_NodeB
			v_iRemote = v_cAdoConn.GetField_Int("zRem_Sum");			//zRem_Sum
			v_iSlave = v_cAdoConn.GetField_Int("zSla_Sum");				//zSla_Sum
			v_dValue = v_cAdoConn.GetField_Double("zLongitude");		//zLongitude
			v_sLongitude.Format("%10.6f",v_dValue);
			v_dValue = v_cAdoConn.GetField_Double("zLatitude");			//zLatitude
			v_sLatitude.Format("%10.6f",v_dValue);
			v_sAddress = v_cAdoConn.GetField_String("zAddress");		//zAddress
			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			Var=v_pRecordset->GetCollect((_variant_t)"zDate");			//zDate
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sDate = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sDate="";
			Var=v_pRecordset->GetCollect((_variant_t)"zDate_Update");	//更新日期
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sCurrentTime = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sCurrentTime="";

			//数据复位
			v_mLine_Data.clear();										//容器清除
			//显示：赋值
			v_iStation=v_iLoc_BTS+v_iLoc_NodeB+v_iLoc_E_NodeB;			//宏站BTS数量
			v_sTemp.Format("%06d",v_iID+1);
			v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));			//序号
			v_mLine_Data.insert(pair<int,CString>(1,v_sStation));		//机房/资源点名称
			v_mLine_Data.insert(pair<int,CString>(2,v_sRegion));		//区域
			v_mLine_Data.insert(pair<int,CString>(3,v_sRemote));		//站型[宏站、拉远]
			v_mLine_Data.insert(pair<int,CString>(4,v_sCover_Type));	//覆盖
			if (v_iLoc_BTS!=0)
				v_sTemp.Format("%d",v_iLoc_BTS);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(5,v_sTemp));			//宏_BTS数量
			if (v_iLoc_NodeB!=0)
				v_sTemp.Format("%d",v_iLoc_NodeB);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(6,v_sTemp));			//宏_NodeB数量
			if (v_iLoc_E_NodeB!=0)
				v_sTemp.Format("%d",v_iLoc_E_NodeB);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(7,v_sTemp));			//宏_E_NodeB数量
			if (v_iRemote!=0)
				v_sTemp.Format("%d",v_iRemote);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(8,v_sTemp));			//拉远站数量
			if (v_iRem_BTS!=0)
				v_sTemp.Format("%d",v_iRem_BTS);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(9,v_sTemp));			//拉_BTS数量
			if (v_iRem_NodeB!=0)
				v_sTemp.Format("%d",v_iRem_NodeB);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(10,v_sTemp));			//拉_NodeB数量
			if (v_iRem_E_NodeB!=0)
				v_sTemp.Format("%d",v_iRem_E_NodeB);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(11,v_sTemp));			//拉_E_NodeB数量
			if (v_iSlave!=0)
				v_sTemp.Format("%d",v_iSlave);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(12,v_sTemp));			//从站数量
			if (v_iSla_BTS!=0)
				v_sTemp.Format("%d",v_iSla_BTS);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(13,v_sTemp));			//从_BTS数量
			if (v_iSla_NodeB!=0)
				v_sTemp.Format("%d",v_iSla_NodeB);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(14,v_sTemp));			//从_NodeB数量
			if (v_iSla_E_NodeB!=0)
				v_sTemp.Format("%d",v_iSla_E_NodeB);							
			else
				v_sTemp="";
			v_mLine_Data.insert(pair<int,CString>(15,v_sTemp));			//从_E_NodeB数量
			v_mLine_Data.insert(pair<int,CString>(16,v_sOnline));		//状态
			v_mLine_Data.insert(pair<int,CString>(17,v_sLongitude));	//经度
			v_mLine_Data.insert(pair<int,CString>(18,v_sLatitude));		//纬度
			v_mLine_Data.insert(pair<int,CString>(19,v_sType));			//类型
			v_mLine_Data.insert(pair<int,CString>(20,v_sAddress));		//地址
			v_mLine_Data.insert(pair<int,CString>(21,v_sDate));			//入网日期
			v_mLine_Data.insert(pair<int,CString>(22,v_sCurrentTime));	//更新日期

			//添加：虚拟列表
			m_cList_Station.Put_Line_Data(v_mLine_Data);				//列表添加行数据

			v_iID++;													//计数：告警数量

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

	//虚拟列表
	m_cList_Station.Put_Line_End();										//列表添加数据结束
	v_mLine_Data.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载机房信息
//	工	  作：首先，获取宏站(基站)；其次，获取拉远(小区)；
//------------------------------------------------------------------------------------------------------			
void CQUA_Station_Map::My_LoadData_Tree(CString v_sRoom,CString v_sStation_Cover,double v_dLongitude,double v_dLatitude)
{
	CString		v_sSql,v_sTemp,v_sRNC,v_sCover;							//临时变量
	CString		v_sBTS,v_sBTS_Name,v_sNodeB_Name,v_sStation;			//临时变量
	CString		v_sCell,v_sStation_BTS;									//临时变量

	Tree_Room_Struct	v_cTree_Room;									//结构
	Tree_BTS_Struct		v_cTree_BTS;									//结构

	mTree_Room::iterator	v_pRoom;									//迭代器

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//宏站处理
		//【1】GSM：BTS获取
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT zBTS,zBTS_NAME,zCover_Type FROM GSM_BTS where zStation='"+v_sRoom+"' order by zBTS"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sBTS = v_cAdoConn.GetField_String("zBTS");				//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sCover= v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type

			v_pRoom=v_mTree_Room.find(v_sRoom);							//查询：机房
			if (v_pRoom==v_mTree_Room.end())							//没找到？？？
			{
				v_cTree_Room.v_iType=0;									//本站
				v_cTree_Room.v_sCover_Type=v_sStation_Cover;			//覆盖类型
				v_cTree_Room.v_dLongitude=v_dLongitude;					//经度
				v_cTree_Room.v_dLatitude=v_dLatitude;					//纬度
				v_mTree_Room.insert(pair<CString,Tree_Room_Struct>(v_sRoom,v_cTree_Room));	//容器增加：机房
				v_pRoom=v_mTree_Room.find(v_sRoom);						//查询：机房
			}
			v_cTree_BTS.v_bBreak=false;									//标志：断链
			v_cTree_BTS.v_sBTS_Name=v_sBTS_Name;						//获取：中文名
			v_cTree_BTS.v_sCover=v_sCover;								//获取：覆盖类型
			My_LoadData_Tree_Cell(v_sRoom,v_sBTS,0,&v_cTree_BTS);		//获取：小区信息【BTS】

			v_pRecordset->MoveNext();									//
		}

		//【2】TD_LTE：NodeB获取
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT zNODEB,zRNC,zCover_Type FROM LTE_NODEB where zStation='"+v_sRoom+"' order by zNODEB"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sNodeB_Name = v_cAdoConn.GetField_String("zNODEB");		//zNODEB
			v_sRNC= v_cAdoConn.GetField_String("zRNC");					//zRNC
			v_sCover= v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type

			v_pRoom=v_mTree_Room.find(v_sRoom);							//查询：机房
			if (v_pRoom==v_mTree_Room.end())							//没找到？？？
			{
				v_cTree_Room.v_iType=0;									//本站
				v_cTree_Room.v_sCover_Type=v_sCover;					//覆盖类型
				v_cTree_Room.v_dLongitude=v_dLongitude;					//经度
				v_cTree_Room.v_dLatitude=v_dLatitude;					//纬度
				v_mTree_Room.insert(pair<CString,Tree_Room_Struct>(v_sRoom,v_cTree_Room));	//容器增加：机房
				v_pRoom=v_mTree_Room.find(v_sRoom);						//查询：机房
			}
			v_cTree_BTS.v_bBreak=false;									//标志：断链
			v_cTree_BTS.v_sCover=v_sCover;								//获取：覆盖类型
			My_LoadData_Tree_Cell(v_sRoom,v_sNodeB_Name,1,&v_cTree_BTS);//获取：小区信息【NodeB】

			v_pRecordset->MoveNext();									//
		}

		//【3】GSM GT：BTS获取(高铁)
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT zBTS,zBSC,zCover_Type FROM GT_BTS where zStation='"+v_sRoom+"' order by zBTS"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sBTS = v_cAdoConn.GetField_String("zBTS");				//zBTS
			v_sCover= v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type

			v_pRoom=v_mTree_Room.find(v_sRoom);							//查询：机房
			if (v_pRoom==v_mTree_Room.end())							//没找到？？？
			{
				v_cTree_Room.v_iType=0;									//本站
				v_cTree_Room.v_sCover_Type=v_sCover;					//覆盖类型
				v_cTree_Room.v_dLongitude=v_dLongitude;					//经度
				v_cTree_Room.v_dLatitude=v_dLatitude;					//纬度
				v_mTree_Room.insert(pair<CString,Tree_Room_Struct>(v_sRoom,v_cTree_Room));	//容器增加：机房
				v_pRoom=v_mTree_Room.find(v_sRoom);						//查询：机房
			}
			v_cTree_BTS.v_bBreak=false;									//标志：断链
			v_cTree_BTS.v_sCover=v_sCover;								//获取：覆盖类型
			My_LoadData_Tree_Cell(v_sRoom,v_sBTS,2,&v_cTree_BTS);		//获取：小区信息【GT BTS】

			v_pRecordset->MoveNext();									//
		}

		//拉远站处理(即不是宏站)
		//【4】GSM Cell
		My_LoadData_Tree_Cell_Remote(v_sRoom,0);						//获取：小区信息【GSM Cell】
		//【5】LD-LTE Cell
		My_LoadData_Tree_Cell_Remote(v_sRoom,1);						//获取：小区信息【LTE Cell】
		//【6】GSM GT Cell
		My_LoadData_Tree_Cell_Remote(v_sRoom,2);						//获取：小区信息【GSM GT Cell】

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
//	函数功能：获取小区信息【BTS\LTE\GT】
//	参	  数：v_sRoom：机房；	v_sBTS：基站名称；	v_iType：0：GSM；1：LTE；2：GSM GT；
//------------------------------------------------------------------------------------------------------			
void CQUA_Station_Map::My_LoadData_Tree_Cell(CString v_sRoom,CString v_sBTS,int v_iType,Tree_BTS_Struct* v_cTree_BTS_Data)
{
	CString		v_sSql,v_sTemp;											//临时变量
	CString		v_sCell,v_sStation="",v_sCover;							//临时变量
	double		v_dLongitude,v_dLatitude;								//临时变量

	Tree_Room_Struct		v_cTree_Room;								//结构
	Tree_BTS_Struct			v_cTree_BTS;								//结构
	Tree_Cell_Struct		v_cTree_Cell;								//结构

	mTree_Room::iterator	v_pRoom;									//迭代器
	mTree_BTS::iterator		v_pBTS;										//迭代器

	//【1】定位：机房、BTS
	v_pRoom=v_mTree_Room.find(v_sRoom);									//查询：本站机房
	if (v_pRoom==v_mTree_Room.end())									//没找到？？？
		return;															//无效返回										

	_variant_t	Var;

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//【2】获取：SQL
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		if (v_iType==0)													//GSM
			v_sSql += "SELECT zCell,GSM_Cell.zStation,CON_Station.zCover_Type,CON_Station.zLongitude,CON_Station.zLatitude FROM GSM_Cell \
					  left join CON_Station on GSM_Cell.zStation=CON_Station.zStation where zBTS='"+v_sBTS+"'";
		else if (v_iType==1)											//LTE
			v_sSql += "SELECT zCell,LTE_Cell.zStation,CON_Station.zCover_Type,CON_Station.zLongitude,CON_Station.zLatitude FROM LTE_Cell \
					  left join CON_Station on LTE_Cell.zStation=CON_Station.zStation where zNODEB='"+v_sBTS+"'"; 
		else															//GSM GT
			v_sSql += "SELECT zCell,GT_Cell.zStation,CON_Station.zCover_Type,zLongitude,zLatitude FROM GT_Cell \
					  left join CON_Station on GT_Cell.zStation=CON_Station.zStation where zBTS='"+v_sBTS+"'"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			//【3】获取：小区信息
			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sCover = v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type
			v_dLongitude = v_cAdoConn.GetField_Double("zLongitude");	//zLongitude
			v_dLatitude = v_cAdoConn.GetField_Double("zLatitude");		//zLatitude
			v_sStation.Trim();											//去掉空格

			if (v_sStation==v_sRoom || v_sStation.IsEmpty())
			{
				//【4】本站处理
				if (v_sStation==v_sRoom)								//本站？？？
					v_sStation="本站";									//小区基站==机房
				v_pRoom->second.v_mTree_BTS.insert(pair<CString,Tree_BTS_Struct>(v_sBTS,*v_cTree_BTS_Data));	//容器增加
				v_pBTS=v_pRoom->second.v_mTree_BTS.find(v_sBTS);		//查找：BTS
			}
			else
			{
				//【4】拉远机房处理
				v_pRoom=v_mTree_Room.find(v_sStation);					//查询：拉远
				if (v_pRoom==v_mTree_Room.end())						//没找到？？？
				{
					v_cTree_Room.v_iType=1;								//拉远机房
					v_cTree_Room.v_sCover_Type=v_sCover;				//覆盖类型
					v_cTree_Room.v_dLongitude=v_dLongitude;				//经度
					v_cTree_Room.v_dLatitude=v_dLatitude;				//纬度
					v_mTree_Room.insert(pair<CString,Tree_Room_Struct>(v_sStation,v_cTree_Room));	//容器增加：拉远
					v_pRoom=v_mTree_Room.find(v_sStation);				//查询：机房
				}
				v_pBTS=v_pRoom->second.v_mTree_BTS.find(v_sBTS);		//查找：BTS
				if (v_pBTS==v_pRoom->second.v_mTree_BTS.end())			//没找到？？？
				{
					v_cTree_BTS.v_bBreak=false;							//标志：断链
					v_cTree_BTS.v_sCover=v_cTree_BTS_Data->v_sCover;	//获取：覆盖类型
					v_cTree_BTS.v_sBTS_Name=v_cTree_BTS_Data->v_sBTS_Name;	//获取：代维
					v_pRoom->second.v_mTree_BTS.insert(pair<CString,Tree_BTS_Struct>(v_sBTS,v_cTree_BTS));	//容器增加：基站
					v_pBTS=v_pRoom->second.v_mTree_BTS.find(v_sBTS);	//查找：BTS
				}
			}
			//【5】增加：小区信息
			v_cTree_Cell.v_bStop=false;									//正常
			v_cTree_Cell.v_sStation=v_sStation;							//机房
			v_pBTS->second.v_mTree_Cell.insert(pair<CString,Tree_Cell_Struct>(v_sCell,v_cTree_Cell));		//容器增加：小区

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
//	函数功能：获取小区信息_拉远【BTS\LTE\GT】
//	参	  数：v_sRoom：机房；	v_iType：0：GSM；1：LTE；2：GSM GT；
//------------------------------------------------------------------------------------------------------			
void CQUA_Station_Map::My_LoadData_Tree_Cell_Remote(CString v_sRoom,int v_iType)
{
	CString		v_sSql,v_sTemp,v_sStation_BTS,v_sCover_Type="",v_sCover_Type_Cell;		//临时变量
	CString		v_sCell,v_sStation="",v_sCover,v_sBTS;					//临时变量
	double		v_dLongitude,v_dLatitude;								//临时变量
	double		v_dLongitude_BTS=0,v_dLatitude_BTS=0;					//临时变量

	Tree_Room_Struct		v_cTree_Room;								//结构
	Tree_BTS_Struct			v_cTree_BTS;								//结构
	Tree_Cell_Struct		v_cTree_Cell;								//结构

	mTree_Room::iterator	v_pRoom;									//迭代器
	mTree_BTS::iterator		v_pBTS;										//迭代器

	_variant_t	Var;

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//【2】获取：SQL
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		if (v_iType==0)													//GSM
			v_sSql += "SELECT zCell,zBTS,GSM_Cell.zStation,CON_Station.zCover_Type,CON_Station.zLongitude,CON_Station.zLatitude FROM GSM_Cell \
					  left join CON_Station on GSM_Cell.zStation=CON_Station.zStation where GSM_Cell.zStation='"+v_sRoom+"'";
		else if (v_iType==1)											//LTE
			v_sSql += "SELECT zCell,zNODEB as zBTS,LTE_Cell.zStation,CON_Station.zCover_Type,CON_Station.zLongitude,CON_Station.zLatitude FROM LTE_Cell \
					  left join CON_Station on LTE_Cell.zStation=CON_Station.zStation where LTE_Cell.zStation='"+v_sRoom+"'"; 
		else															//GSM GT
			v_sSql += "SELECT zCell,zBTS,GT_Cell.zStation,CON_Station.zCover_Type,zLongitude,zLatitude FROM GT_Cell \
					  left join CON_Station on GT_Cell.zStation=CON_Station.zStation where GT_Cell.zStation='"+v_sRoom+"'"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			//【3】获取：小区信息
			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sBTS = v_cAdoConn.GetField_String("zBTS");				//zBTS
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sCover_Type_Cell = v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type
			v_dLongitude = v_cAdoConn.GetField_Double("zLongitude");	//zLongitude
			v_dLatitude = v_cAdoConn.GetField_Double("zLatitude");		//zLatitude
			v_sStation.Trim();											//去掉空格

			v_sStation_BTS=My_Get_Station_BTS(v_sBTS,v_iType,&v_cTree_BTS,&v_sCover_Type,&v_dLongitude_BTS,&v_dLatitude_BTS);		//获取：BTS机房
			if (v_sStation_BTS!=v_sStation && !v_sStation_BTS.IsEmpty())		//BTS机房与小区机房不同？？？
			{
				//【4】拉远机房处理
				v_pRoom=v_mTree_Room.find(v_sStation_BTS);				//查询：拉远
				if (v_pRoom==v_mTree_Room.end())						//没找到？？？
				{
					v_cTree_Room.v_iType=2;								//主站(拉远)机房
					v_cTree_Room.v_sRoom_Cell=v_sRoom;					//机房(小区)
					v_cTree_Room.v_sCover_Type_Cell=v_sCover_Type_Cell;	//覆盖类型(小区)
					v_cTree_Room.v_dLongitude_Cell=v_dLongitude;		//经度(小区)
					v_cTree_Room.v_dLatitude_Cell=v_dLatitude;			//纬度(小区)
					v_cTree_Room.v_dLongitude=v_dLongitude_BTS;			//经度_BTS(主站)
					v_cTree_Room.v_dLatitude=v_dLatitude_BTS;			//纬度_BTS(主站)
					v_cTree_Room.v_sCover_Type=v_sCover_Type;			//覆盖类型_BTS(主站)
					v_mTree_Room.insert(pair<CString,Tree_Room_Struct>(v_sStation_BTS,v_cTree_Room));		//容器增加：拉远
					v_pRoom=v_mTree_Room.find(v_sStation_BTS);			//查询：机房
				}
				v_pBTS=v_pRoom->second.v_mTree_BTS.find(v_sBTS);		//查找：BTS
				if (v_pBTS==v_pRoom->second.v_mTree_BTS.end())			//没找到？？？
				{
					v_pRoom->second.v_mTree_BTS.insert(pair<CString,Tree_BTS_Struct>(v_sBTS,v_cTree_BTS));	//容器增加：基站
					v_pBTS=v_pRoom->second.v_mTree_BTS.find(v_sBTS);	//查找：BTS
				}

				//【5】增加：小区信息
				v_cTree_Cell.v_bStop=false;									//正常
				v_cTree_Cell.v_sStation=v_sStation;							//机房
				v_pBTS->second.v_mTree_Cell.insert(pair<CString,Tree_Cell_Struct>(v_sCell,v_cTree_Cell));	//容器增加：小区
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
//	函数功能：获取基站机房【BTS\LTE\GT】
//	参	  数：v_sBTS：基站名称；	v_iType：0：GSM；1：LTE；2：GSM GT；
//------------------------------------------------------------------------------------------------------			
CString CQUA_Station_Map::My_Get_Station_BTS(CString v_sBTS,int v_iType,Tree_BTS_Struct* v_cTree_BTS,CString* v_sCover_Type,double* v_dLongitude,double* v_dLatitude)
{
	CString		v_sSql,v_sTemp;											//临时变量
	CString		v_sStation="",v_sCover;									//临时变量

	_variant_t	Var;

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//【1】获取：SQL
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		if (v_iType==0)													//GSM
			v_sSql += "SELECT zBTS_NAME,GSM_BTS.zCover_Type,GSM_BTS.zStation,CON_Station.zLongitude,CON_Station.zLatitude \
					  ,CON_Station.zCover_Type as zCover_Station FROM GSM_BTS \
					  left join CON_Station on GSM_BTS.zStation=CON_Station.zStation where zBTS='"+v_sBTS+"'"; 
		else if (v_iType==1)											//LTE
			v_sSql += "SELECT LTE_NODEB.zCover_Type,LTE_NODEB.zStation,CON_Station.zLongitude,CON_Station.zLatitude \
					  ,CON_Station.zCover_Type as zCover_Station FROM LTE_NODEB \
					  left join CON_Station on LTE_NODEB.zStation=CON_Station.zStation where zNODEB='"+v_sBTS+"'"; 
		else															//GSM GT
			v_sSql += "SELECT GT_BTS.zCover_Type,GT_BTS.zStation,CON_Station.zLongitude,CON_Station.zLatitude \
					  ,CON_Station.zCover_Type as zCover_Station FROM GT_BTS \
					  left join CON_Station on GT_BTS.zStation=CON_Station.zStation where zBTS='"+v_sBTS+"'"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			//【2】获取：BTS信息
			if (v_iType==0)												//GSM BTS
				v_cTree_BTS->v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");	//zBTS_NAME
			else
				v_cTree_BTS->v_sBTS_Name = "";							//
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			*v_sCover_Type = v_cAdoConn.GetField_String("zCover_Station");	//zCover_Station
			*v_dLongitude = v_cAdoConn.GetField_Double("zLongitude");	//zLongitude
			*v_dLatitude = v_cAdoConn.GetField_Double("zLatitude");		//zLatitude
			v_cTree_BTS->v_bBreak=false;								//
			v_cTree_BTS->v_sCover=v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type
			v_sStation.Trim();											//去掉空格

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
	return	v_sStation;													//返回
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：显示机房信息
//------------------------------------------------------------------------------------------------------			
void CQUA_Station_Map::My_Show_Tree()
{
	HTREEITEM		v_hRoom,v_hBTS_Node;								//根目录句柄、节点句柄；
	CString			v_sTemp;											//临时变量
	double			v_dRoom_Lon,v_dRoom_Lat;							//临时变量
	bool			v_bOK=false;										//临时变量

	mTree_Room::iterator	v_pRoom;									//迭代器
	mTree_BTS::iterator		v_pBTS;										//迭代器
	mTree_Cell::iterator	v_pCell;									//迭代器

	//【0】树复位
	m_cTree_Room.DeleteAllItems();										//树清除
	if (v_mTree_Room.size()==0)											//有机房信息？？？
		return;															//返回：无机房

	//【1】本站处理(本站)
	for (v_pRoom=v_mTree_Room.begin();v_pRoom!=v_mTree_Room.end();v_pRoom++)	//遍历
	{
		if(v_pRoom->second.v_iType==0)									//本站？？？
		{
			//(1)本站：根目录
			v_bOK=true;													//置位
			v_hBTS_Root=m_cTree_Room.InsertItem(v_pRoom->first,2,3,TVI_ROOT);	//树：增加根节点
			v_dRoom_Lon=v_pRoom->second.v_dLongitude;					//本站：经度
			v_dRoom_Lat=v_pRoom->second.v_dLatitude;					//本站：纬度

			//(2)添加：本站信息
			v_hRoom=m_cTree_Room.InsertItem("本站",2,3,v_hBTS_Root);	//树：增加子节点【Room】
			//显示：地图信息
			if (v_pRoom->second.v_sCover_Type=="室外")
				My_Map_Marker_Direct(v_dRoom_Lon,v_dRoom_Lat,v_pRoom->first,0);		//调用JS：绘制Marker(主站：室外)
			else if (v_pRoom->second.v_sCover_Type=="室内")
				My_Map_Marker_Direct(v_dRoom_Lon,v_dRoom_Lat,v_pRoom->first,1);		//调用JS：绘制Marker(主站：室内)
			else													
				My_Map_Marker_Direct(v_dRoom_Lon,v_dRoom_Lat,v_pRoom->first,2);		//调用JS：绘制Marker(主站：室内外)

			//(3)显示：本站信息及状态
			v_pBTS=v_pRoom->second.v_mTree_BTS.begin();					//头指针
			while(v_pBTS!=v_pRoom->second.v_mTree_BTS.end())			//遍历
			{
				//(4)添加：基站信息
				if (v_pBTS->first.GetLength()==6)						//GSM ???
					v_sTemp=v_pBTS->first+"_"+v_pBTS->second.v_sBTS_Name;	//BTS+中文名
				else
					v_sTemp=v_pBTS->first;								//BTS
				v_hBTS_Node=m_cTree_Room.InsertItem(v_sTemp+"     【"+v_pBTS->second.v_sCover+"】",1*8+2,1*8+3,v_hRoom);		//树：增加子节点【Nodeb】

				//(5)添加：小区信息
				v_pCell=v_pBTS->second.v_mTree_Cell.begin();			//头指针
				while(v_pCell!=v_pBTS->second.v_mTree_Cell.end())		//遍历
				{
					m_cTree_Room.InsertItem(v_pCell->first+"     【"+v_pCell->second.v_sStation+"】",2*8+2,2*8+3,v_hBTS_Node);	//树：增加子节点【Cell】

					v_pCell++;											//指针++
				}
				v_pBTS++;												//指针++
			}
		}
	}

	//【2】本站处理(拉远)
	if (v_bOK)															//本站？？？
	{
		for (v_pRoom=v_mTree_Room.begin();v_pRoom!=v_mTree_Room.end();v_pRoom++)	//遍历
		{
			if(v_pRoom->second.v_iType==1)								//拉远？？？
			{
				//(1)显示：Tree信息
				v_hRoom=m_cTree_Room.InsertItem(v_pRoom->first,28,29,v_hBTS_Root);	//树：增加子节点【Room】

				//显示机房信息及状态
				for(v_pBTS=v_pRoom->second.v_mTree_BTS.begin();v_pBTS!=v_pRoom->second.v_mTree_BTS.end();v_pBTS++)	//遍历
				{
					if (v_pBTS->first.GetLength()==6)					//GSM ???
						v_sTemp=v_pBTS->first+"_"+v_pBTS->second.v_sBTS_Name;	//BTS+中文名
					else
						v_sTemp=v_pBTS->first;							//BTS
					v_hBTS_Node=m_cTree_Room.InsertItem(v_sTemp+"     【"+v_pBTS->second.v_sCover+"】",1*8+2,1*8+3,v_hRoom);		//树：增加子节点【Nodeb】

					//添加：小区信息
					v_pCell=v_pBTS->second.v_mTree_Cell.begin();		//头指针
					while(v_pCell!=v_pBTS->second.v_mTree_Cell.end())	//遍历
					{
						m_cTree_Room.InsertItem(v_pCell->first+"     【"+v_pCell->second.v_sStation+"】",2*8+2,2*8+3,v_hBTS_Node);	//树：增加子节点 【Cell】

						v_pCell++;										//指针++
					}
				}

				//(2)地图：标记位置
				if (v_pRoom->second.v_sCover_Type=="室外")
					My_Map_Marker_Direct(v_pRoom->second.v_dLongitude,v_pRoom->second.v_dLatitude,v_pRoom->first,3);		//调用JS：标记位置(拉远：室外)
				else if (v_pRoom->second.v_sCover_Type=="室内")
					My_Map_Marker_Direct(v_pRoom->second.v_dLongitude,v_pRoom->second.v_dLatitude,v_pRoom->first,4);		//调用JS：标记位置(拉远：室内)
				else
					My_Map_Marker_Direct(v_pRoom->second.v_dLongitude,v_pRoom->second.v_dLatitude,v_pRoom->first,5);		//调用JS：标记位置(拉远：室内外)

				//(3)题图：画线
				My_Map_Line(v_dRoom_Lon,v_dRoom_Lat,v_pRoom->second.v_dLongitude,v_pRoom->second.v_dLatitude);	//调用JS：画线
			}
		}
	}

	//【3】从站处理(机房在拉远)
	for (v_pRoom=v_mTree_Room.begin();v_pRoom!=v_mTree_Room.end();v_pRoom++)	//遍历
	{
		if(v_pRoom->second.v_iType==2)									//从站？？？
		{
			//(1)本站：根目录
			v_hBTS_Root=m_cTree_Room.InsertItem(v_pRoom->first,2,3,TVI_ROOT);	//树：增加根节点
			if (!v_bOK)													//本站无效？？？
			{															//获取：位置信息(地图中心)
				v_dRoom_Lon=v_pRoom->second.v_dLongitude;				//主站：经度
				v_dRoom_Lat=v_pRoom->second.v_dLatitude;				//主站：纬度
			}

			//(2)显示：地图信息(主站)
			if (v_pRoom->second.v_sCover_Type=="室外")
				My_Map_Marker_Direct(v_pRoom->second.v_dLongitude,v_pRoom->second.v_dLatitude,v_pRoom->first,0);	//调用JS：绘制Marker(宏站)
			else if (v_pRoom->second.v_sCover_Type=="室内")
				My_Map_Marker_Direct(v_pRoom->second.v_dLongitude,v_pRoom->second.v_dLatitude,v_pRoom->first,1);	//调用JS：绘制Marker(宏站)
			else
				My_Map_Marker_Direct(v_pRoom->second.v_dLongitude,v_pRoom->second.v_dLatitude,v_pRoom->first,2);	//调用JS：绘制Marker(宏站)
			//(3)显示：地图信息(拉远)
			if (v_pRoom->second.v_sCover_Type_Cell=="室外")
				My_Map_Marker_Direct(v_pRoom->second.v_dLongitude_Cell,v_pRoom->second.v_dLatitude_Cell,v_pRoom->second.v_sRoom_Cell,3);	//调用JS：标记位置(拉远)
			else if (v_pRoom->second.v_sCover_Type_Cell=="室内")
				My_Map_Marker_Direct(v_pRoom->second.v_dLongitude_Cell,v_pRoom->second.v_dLatitude_Cell,v_pRoom->second.v_sRoom_Cell,4);	//调用JS：标记位置(拉远)
			else
				My_Map_Marker_Direct(v_pRoom->second.v_dLongitude_Cell,v_pRoom->second.v_dLatitude_Cell,v_pRoom->second.v_sRoom_Cell,5);	//调用JS：标记位置(拉远)

			//(4)显示：地图信息(划线)
			My_Map_Line(v_pRoom->second.v_dLongitude,v_pRoom->second.v_dLatitude,v_pRoom->second.v_dLongitude_Cell,v_pRoom->second.v_dLatitude_Cell);	//调用JS：画线

			//(5)显示：本站信息及状态
			v_pBTS=v_pRoom->second.v_mTree_BTS.begin();					//头指针
			while(v_pBTS!=v_pRoom->second.v_mTree_BTS.end())			//遍历
			{
				//(6)添加：基站信息
				if (v_pBTS->first.GetLength()==6)						//GSM ???
					v_sTemp=v_pBTS->first+"_"+v_pBTS->second.v_sBTS_Name;	//BTS+中文名
				else
					v_sTemp=v_pBTS->first;								//BTS
				v_hBTS_Node=m_cTree_Room.InsertItem(v_sTemp+"     【"+v_pBTS->second.v_sCover+"】",1*8+2,1*8+3,v_hBTS_Root);	//树：增加子节点【Nodeb】

				//(7)添加：小区信息
				v_pCell=v_pBTS->second.v_mTree_Cell.begin();			//头指针
				while(v_pCell!=v_pBTS->second.v_mTree_Cell.end())		//遍历
				{
					m_cTree_Room.InsertItem(v_pCell->first+"     【"+v_pCell->second.v_sStation+"】",2*8+2,2*8+3,v_hBTS_Node);	//树：增加子节点【Cell】

					v_pCell++;											//指针++
				}
				v_pBTS++;												//指针++
			}
		}
	}

	//【4】地图：设置显示中心点(以宏站点为准)
	My_Set_Center(v_dRoom_Lon,v_dRoom_Lat,16);							//调用JS：设置显示中心点

	//【5】树展开
	if (m_cSelect.GetCurSel()==0)
	{
		m_cTree_Room.CloseTree(v_hBTS_Root);							//树：收缩
		m_cTree_Room.Expand(v_hBTS_Root,TVE_EXPAND);					//树：展开到机房
	}
	else if (m_cSelect.GetCurSel()==1)
	{
		m_cTree_Room.CloseTree(v_hBTS_Root);							//树：收缩
		m_cTree_Room.ExpandTree_Level(v_hBTS_Root,2);					//树：展开2级
	}
	else if (m_cSelect.GetCurSel()==2)
		m_cTree_Room.ExpandTree(v_hBTS_Root);							//树：全展开(Cell)
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：容器清除
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::My_Clear()
{
	mTree_Room::iterator	v_pRoom;									//迭代器
	mTree_BTS::iterator		v_pBTS;										//迭代器
	for(v_pRoom=v_mTree_Room.begin();v_pRoom!=v_mTree_Room.end();v_pRoom++)	//遍历
	{
		for(v_pBTS=v_pRoom->second.v_mTree_BTS.begin();v_pBTS!=v_pRoom->second.v_mTree_BTS.end();v_pBTS++)	//遍历
			v_pBTS->second.v_mTree_Cell.clear();						//容器清除
		v_pRoom->second.v_mTree_BTS.clear();							//容器清除
	}
	v_mTree_Room.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：浏览器响应
//------------------------------------------------------------------------------------------------------
BEGIN_EVENTSINK_MAP(CQUA_Station_Map, CFormView)
	ON_EVENT(CQUA_Station_Map, IDC_EXPLORER, 259, CQUA_Station_Map::DocumentCompleteExplorer, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

//------------------------------------------------------------------------------------------------------			
//	函数功能：浏览器文件加载完毕
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Map::DocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT* URL)
{
	// TODO: 在此处添加消息处理程序代码
	IDispatch* pIDisp = NULL;
	pIDisp = m_webBrowser.GetDocument();
	if(pIDisp)
	{
		//置位：操作按钮
		m_cQuery.EnableWindow(true);									//查询：有效
		m_cExcel_NodeB.EnableWindow(true);								//导出：有效
		m_cMarker.EnableWindow(true);									//地图标记：有效
		m_cMap_Clear.EnableWindow(true);								//地图清除：有效

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

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
