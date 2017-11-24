
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "Station.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_CLIENT_READCOLOSE WM_USER + 1201

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_Log_System, &CMainFrame::OnUpdateLogSystem)
	ON_UPDATE_COMMAND_UI(ID_Log_Bug, &CMainFrame::OnUpdateLogBug)
	ON_UPDATE_COMMAND_UI(ID_User_PassWord, &CMainFrame::OnUpdateUserPassword)
	ON_UPDATE_COMMAND_UI(ID_User_Setup, &CMainFrame::OnUpdateUserSetup)
	ON_UPDATE_COMMAND_UI(ID_Log_DATABASE, &CMainFrame::OnUpdateLogDatabase)
	ON_UPDATE_COMMAND_UI(ID_User_Role, &CMainFrame::OnUpdateUserRole)
	//-----------------------------------------------------------------------------------
	ON_UPDATE_COMMAND_UI(ID_Config_Station, &CMainFrame::OnUpdateConfigStation)
	ON_UPDATE_COMMAND_UI(ID_Config_Region, &CMainFrame::OnUpdateConfigRegion)
	ON_UPDATE_COMMAND_UI(ID_Config_BTS, &CMainFrame::OnUpdateConfigBts)
	ON_UPDATE_COMMAND_UI(ID_Config_GSM_Cell, &CMainFrame::OnUpdateConfigGsmCell)
	ON_UPDATE_COMMAND_UI(ID_Config_LTE_Cell, &CMainFrame::OnUpdateConfigLteCell)
	ON_UPDATE_COMMAND_UI(ID_Config_E_NodeB, &CMainFrame::OnUpdateConfigENodeb)
	ON_UPDATE_COMMAND_UI(ID_Config_Tools, &CMainFrame::OnUpdateConfigTools)
	ON_UPDATE_COMMAND_UI(ID_QUARY_STATION_Map, &CMainFrame::OnUpdateQuaryStationMap)
	ON_UPDATE_COMMAND_UI(ID_Pm_LTE, &CMainFrame::OnUpdatePmLte)
	ON_UPDATE_COMMAND_UI(ID_Config_Para, &CMainFrame::OnUpdateConfigPara)
	ON_UPDATE_COMMAND_UI(ID_LTE_PM_DRAW, &CMainFrame::OnUpdateLtePmDraw)
	ON_UPDATE_COMMAND_UI(ID_LTE_PM_DATA, &CMainFrame::OnUpdateLtePmData)
	ON_UPDATE_COMMAND_UI(ID_ALL_PM_DRAW, &CMainFrame::OnUpdateAllPmDraw)
	ON_UPDATE_COMMAND_UI(ID_Pm_LTE_Cal, &CMainFrame::OnUpdatePmLteCal)
	ON_UPDATE_COMMAND_UI(ID_Pm_LTE_Cas, &CMainFrame::OnUpdatePmLteCas)
	ON_UPDATE_COMMAND_UI(ID_Pm_GSM, &CMainFrame::OnUpdatePmGsm)
	ON_UPDATE_COMMAND_UI(ID_Pm_GSM_Cal_CELL, &CMainFrame::OnUpdatePmGsmCalCell)
	ON_UPDATE_COMMAND_UI(ID_Pm_GSM_Cal_BSC, &CMainFrame::OnUpdatePmGsmCalBsc)
	ON_UPDATE_COMMAND_UI(ID_Log_DATABASE_GSM, &CMainFrame::OnUpdateLogDatabaseGsm)
	ON_UPDATE_COMMAND_UI(ID_GSM_PM_DRAW, &CMainFrame::OnUpdateGsmPmDraw)
	ON_UPDATE_COMMAND_UI(ID_GSM_PM_DATA, &CMainFrame::OnUpdateGsmPmData)
	ON_UPDATE_COMMAND_UI(ID_LTE_PM_DRAW_Init, &CMainFrame::OnUpdateLtePmDrawInit)
	ON_UPDATE_COMMAND_UI(ID_LTE_PM_DATA_Init, &CMainFrame::OnUpdateLtePmDataInit)
	ON_UPDATE_COMMAND_UI(ID_ALARM_PM, &CMainFrame::OnUpdateAlarmPm)
	ON_UPDATE_COMMAND_UI(ID_Pm_LTE_Region, &CMainFrame::OnUpdatePmLteRegion)
	ON_UPDATE_COMMAND_UI(ID_Pm_LTE_Cas_PM, &CMainFrame::OnUpdatePmLteCasPm)
	ON_UPDATE_COMMAND_UI(ID_LTE_PM_DATA_REAL, &CMainFrame::OnUpdateLtePmDataReal)
	ON_UPDATE_COMMAND_UI(ID_LTE_PM_DATA_CELL, &CMainFrame::OnUpdateLtePmDataCell)
	ON_UPDATE_COMMAND_UI(ID_Pm_LTE_ALARM, &CMainFrame::OnUpdatePmLteAlarm)
	ON_UPDATE_COMMAND_UI(ID_ALARM_LL_CELL, &CMainFrame::OnUpdateAlarmLlCell)
	ON_UPDATE_COMMAND_UI(ID_Cm_LTE_Quary, &CMainFrame::OnUpdateCmLteQuary)
	ON_UPDATE_COMMAND_UI(ID_Cm_LTE_File, &CMainFrame::OnUpdateCmLteFile)
	ON_UPDATE_COMMAND_UI(ID_Cm_LTE_Para, &CMainFrame::OnUpdateCmLtePara)
	ON_UPDATE_COMMAND_UI(ID_LTE_PM_DATA_CAL, &CMainFrame::OnUpdateLtePmDataCal)
	ON_UPDATE_COMMAND_UI(ID_Pm_LTE_Cas_Cal, &CMainFrame::OnUpdatePmLteCasCal)
	ON_UPDATE_COMMAND_UI(ID_QUARY_STATION, &CMainFrame::OnUpdateQuaryStation)
	ON_UPDATE_COMMAND_UI(ID_QUARY_TDLTE, &CMainFrame::OnUpdateQuaryTdlte)
	ON_UPDATE_COMMAND_UI(ID_Quary_GSM, &CMainFrame::OnUpdateQuaryGsm)
	ON_UPDATE_COMMAND_UI(ID_QUARY_GSM_GT, &CMainFrame::OnUpdateQuaryGsmGt)
	ON_UPDATE_COMMAND_UI(ID_ALARM_LL_REAL, &CMainFrame::OnUpdateAlarmLlReal)
	ON_UPDATE_COMMAND_UI(ID_Cm_LTE_Relation, &CMainFrame::OnUpdateCmLteRelation)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	IDS_ALARM,				//通信异常信息
	IDS_USER,				//操作员
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

//------------------------------------------------------------------------------------------------------			
//	构造函数：CMainFrame
//------------------------------------------------------------------------------------------------------
CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);
}

//------------------------------------------------------------------------------------------------------			
//	析构函数：CMainFrame
//------------------------------------------------------------------------------------------------------
CMainFrame::~CMainFrame()
{
}

//------------------------------------------------------------------------------------------------------			
//	程序框架生成：菜单、Toolbar、Statusbar；
//------------------------------------------------------------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//设置信号量[软件只能运行一次]
	::SetProp(m_hWnd,AfxGetApp()->m_pszAppName,(HANDLE)1);				//设置信号量

	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE;				// 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;						// 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = FALSE;									// 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = TRUE;									// 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = TRUE;								// 在选项卡区域的右边缘启用文档菜单
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	//Toolbar生成
	if ((!m_wndToolBar.CreateEx(this,TBSTYLE_FLAT,WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC,
		CRect(0,0,0,0),IDR_MAINFRAME))
		||(!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1,0,0,TRUE,0,0,IDB_TOOLBAR)))	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	//StatusBar生成
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
//	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
//	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	//Status设置
	m_wndStatusBar.SetPaneInfo(1,IDS_ALARM,SBPS_NORMAL,800);			//状态条设置：通信异常信息
	m_wndStatusBar.SetPaneInfo(2,IDS_USER,SBPS_NORMAL,100);				//状态条设置：登录用户

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	VS2008自带函数
//------------------------------------------------------------------------------------------------------
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

//------------------------------------------------------------------------------------------------------			
//	改变窗体的风格
//------------------------------------------------------------------------------------------------------
void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

//------------------------------------------------------------------------------------------------------			
//	应用程序菜单消息执行
//------------------------------------------------------------------------------------------------------
void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

//------------------------------------------------------------------------------------------------------			
//	重绘消息转发：依据子窗体的变化，发出子窗体重绘的消息
//------------------------------------------------------------------------------------------------------
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->hwnd==m_hWndMDIClient && pMsg->message==WM_PAINT)
		PostMessage(WM_PAINT);

	if(pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == m_wndStatusBar.GetSafeHwnd()) 
	{ 
		CRect rect; 
		CPoint pt; 
		pt = pMsg->pt; 
		m_wndStatusBar.GetItemRect(1,&rect); 
		m_wndStatusBar.ClientToScreen(&rect); 
		if(PtInRect(&rect,pt)) 
		{
		}
	} 

	return CMDIFrameWndEx::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	重绘屏幕：依据收到的窗体变化消息 [背景图片调整]
//------------------------------------------------------------------------------------------------------
void CMainFrame::OnPaint()
{
//	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CMDIFrameWndEx::OnPaint()
	CDC dc, memdc;
	dc.m_hDC=::GetDC(this->m_hWndMDIClient);
	CRect rect;
	int iWidth,iHeight;
	CBitmap bitmap;
	BITMAP szbitmap;
	bitmap.LoadBitmap(IDB_BACKGROUND);									//获取：背景图片；
	bitmap.GetObject(sizeof(BITMAP),&szbitmap);
	CSize size(szbitmap.bmWidth,szbitmap.bmHeight);
	memdc.CreateCompatibleDC(&dc);
	CBitmap *oldbitmap=memdc.SelectObject(&bitmap);
	GetClientRect(&rect);
	if (rect.Width() < 1200)			
		iWidth = 1200;
	else
		iWidth = rect.Width();
	if (rect.Height() < 800)
		iHeight = 800;
	else
		iHeight = rect.Height();
	StretchBlt(dc.m_hDC,0,0,iWidth,iHeight,memdc.m_hDC,0,0,size.cx,size.cy,SRCCOPY);
	memdc.SelectObject(oldbitmap);
	memdc.SelectObject(oldbitmap);
	memdc.DeleteDC();
	dc.DeleteDC();
	SetStretchBltMode(dc.GetSafeHdc(),HALFTONE);
	CMDIFrameWnd::OnPaint();
}

//------------------------------------------------------------------------------------------------------			
//	关闭程序框架：依据收到的Close消息
//------------------------------------------------------------------------------------------------------
void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString			v_sFrame_Name;											//窗体名称
	bool			v_bExit=false;											//退出标志：无效
	mFrame_Window::iterator	v_pIterator_Frame;								//迭代器

	if(MessageBox("是否退出 移动多网融合性能优化分析系统 ?"," 系统退出确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)		//退出判断
	{
		mFrame_Window::iterator	v_pIterator_Frame;						//迭代器

		for(v_pIterator_Frame = theApp.v_mFrame.begin();v_pIterator_Frame != theApp.v_mFrame.end();v_pIterator_Frame++)	//遍历
		{
			if(v_pIterator_Frame->second.v_iThread_Exit==1)			//有线程在运行？？？
			{
				MessageBox("模块在读取数据，请在结束后，再退出系统 ！！！");	//有线程在运行
				return;													//返回：不退出
			}
		}

		//移除信号量[软件只能运行一次]
		::RemoveProp(m_hWnd,AfxGetApp()-> m_pszAppName);				//移除信号量

		if (!theApp.a_sUser_Name.IsEmpty())
		{
			//Log：程序退出
			theApp.m_MyLog.WriteText("程序退出--------------------\r\n");	//log

			//退出信息记录到日志表中
			COleDateTime v_tTime=COleDateTime::GetCurrentTime();
			CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");
			CString v_sSql="Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"+v_sCurrentTime+"','用户退出','"
					+theApp.a_sUser_Name+" 退出，IP："+theApp.a_sUser_IP+"')";
			My_ExecuteSQL(v_sSql);										//数据库执行
		}

		CMDIFrameWndEx::OnClose();										//关闭窗口
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取功能权限
//	参    数：v_sPower_Name：权限名称		[依据：功能权限容器a_mUser_Power]
//------------------------------------------------------------------------------------------------------
bool CMainFrame::My_Get_Power(CString v_sPower_Name)
{
	bool	v_bRet=false;												//返回值

	mUser_Power::iterator v_pIterator_Power=theApp.a_mUser_Power.find(v_sPower_Name);	//功能权限[依据v_sPower_Name]
	if(v_pIterator_Power != theApp.a_mUser_Power.end())									//找到？
		v_bRet=v_pIterator_Power->second;								//获得：权限[true/false] 

//	v_bRet=true;
	return v_bRet;														//返回：权限
}

//------------------------------------------------------------------------------------------------------			
//	子菜单状态设置：有效、无效，依据用户的工作权限
//------------------------------------------------------------------------------------------------------

//======================================================================================================
//	子菜单：LTE 性能查询
void CMainFrame::OnUpdateLtePmDraw(CCmdUI *pCmdUI)						//100 LTE 性能图形查询
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE 性能图形查询"));
}

void CMainFrame::OnUpdateLtePmData(CCmdUI *pCmdUI)						//101 LTE 性能数据查询
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE 性能数据查询"));
}

void CMainFrame::OnUpdateLtePmDataCell(CCmdUI *pCmdUI)					//102 LTE 性能数据查询_小区
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE 性能数据查询_小区"));
}

void CMainFrame::OnUpdateLtePmDataReal(CCmdUI *pCmdUI)					//103 LTE 性能数据查询_实时
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE 性能数据查询_实时"));
}

void CMainFrame::OnUpdateLtePmDataCal(CCmdUI *pCmdUI)					//104 LTE 性能数据计算_小区
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE 性能数据计算_小区"));
}

void CMainFrame::OnUpdateLtePmDrawInit(CCmdUI *pCmdUI)					//108 LTE 性能图形查询_原始
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE 性能图形查询_原始"));
}

void CMainFrame::OnUpdateLtePmDataInit(CCmdUI *pCmdUI)					//109 LTE 性能数据查询_原始
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE 性能数据查询_原始"));
}

//======================================================================================================
//	子菜单：GSM 性能查询
void CMainFrame::OnUpdateGsmPmDraw(CCmdUI *pCmdUI)						//110 GSM 性能图形查询
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("GSM 性能图形查询"));
}

void CMainFrame::OnUpdateGsmPmData(CCmdUI *pCmdUI)						//111 GSM 性能数据查询
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("GSM 性能数据查询"));
}

//======================================================================================================
//	子菜单：性能预警
void CMainFrame::OnUpdateAlarmPm(CCmdUI *pCmdUI)						//130 性能预警查询
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("性能预警查询"));
}

void CMainFrame::OnUpdateAlarmLlReal(CCmdUI *pCmdUI)					//131 LTE 低流量小区实时
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE 低流量小区实时"));
}

void CMainFrame::OnUpdateAlarmLlCell(CCmdUI *pCmdUI)					//132 LTE 低流量小区查询
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE 低流量小区查询"));
}

//======================================================================================================
//	子菜单：共址性能查询
void CMainFrame::OnUpdateAllPmDraw(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码						//170 共址性能查询
	pCmdUI->Enable(My_Get_Power("共址性能查询"));
}

//======================================================================================================
//	子菜单：网元查询
void CMainFrame::OnUpdateQuaryStation(CCmdUI *pCmdUI)					//180 站址查询
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("站址查询"));
}

void CMainFrame::OnUpdateQuaryStationMap(CCmdUI *pCmdUI)				//181 站址地图查询
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("站址地图查询"));
}

void CMainFrame::OnUpdateQuaryTdlte(CCmdUI *pCmdUI)						//182 TD-LTE网元查询
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("TD-LTE网元查询"));
}

void CMainFrame::OnUpdateQuaryGsm(CCmdUI *pCmdUI)						//183 GSM网元查询
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("GSM网元查询"));
}

void CMainFrame::OnUpdateQuaryGsmGt(CCmdUI *pCmdUI)						//184 GSM高铁网元查询
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("GSM高铁网元查询"));
}

//======================================================================================================
//	子菜单：配置管理
void CMainFrame::OnUpdateCmLteQuary(CCmdUI *pCmdUI)						//190 LTE 配置查询
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE 配置查询"));
}


void CMainFrame::OnUpdateCmLteRelation(CCmdUI *pCmdUI)					//191 LTE-Tdd相邻关系
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE-Tdd相邻关系"));
}

void CMainFrame::OnUpdateCmLteFile(CCmdUI *pCmdUI)						//192 LTE 配置管理_文件
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE 配置管理_文件"));
}

void CMainFrame::OnUpdateCmLtePara(CCmdUI *pCmdUI)						//193 LTE 配置管理_参数
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE 配置管理_参数"));
}

//======================================================================================================
//	子菜单：网络配置
void CMainFrame::OnUpdateConfigRegion(CCmdUI *pCmdUI)					//210 区域设置
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("区域设置"));
}

void CMainFrame::OnUpdateConfigStation(CCmdUI *pCmdUI)					//211 站址管理
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("站址管理"));
}

void CMainFrame::OnUpdateConfigBts(CCmdUI *pCmdUI)						//212 BTS 配置
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("BTS 配置"));
}

void CMainFrame::OnUpdateConfigENodeb(CCmdUI *pCmdUI)					//214 E_NodeB 配置
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("E_NodeB 配置"));
}

void CMainFrame::OnUpdateConfigGsmCell(CCmdUI *pCmdUI)					//215 GSM Cell配置
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("GSM Cell配置"));
}

void CMainFrame::OnUpdateConfigLteCell(CCmdUI *pCmdUI)					//217 LTE Cell配置
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE Cell配置"));
}

void CMainFrame::OnUpdateConfigTools(CCmdUI *pCmdUI)					//218 XML文件工具
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("XML文件工具"));
}

//======================================================================================================
//	子菜单：性能管理
void CMainFrame::OnUpdatePmLte(CCmdUI *pCmdUI)							//220 LTE_性能管理_原始
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE_性能管理_原始"));
}

void CMainFrame::OnUpdatePmLteCal(CCmdUI *pCmdUI)						//221 LTE_性能管理_计算
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE_性能管理_计算"));
}

void CMainFrame::OnUpdatePmLteRegion(CCmdUI *pCmdUI)					//222 LTE_性能管理_区域
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE_性能管理_区域"));
}

void CMainFrame::OnUpdatePmLteCas(CCmdUI *pCmdUI)						//223 LTE_性能查询模板_小区
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE_性能查询模板_小区"));
}

void CMainFrame::OnUpdatePmLteCasPm(CCmdUI *pCmdUI)						//224 LTE_性能查询模板_性能
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE_性能查询模板_性能"));
}

void CMainFrame::OnUpdatePmLteCasCal(CCmdUI *pCmdUI)					//226 LTE_性能查询模板_计算
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE_性能查询模板_计算"));
}

void CMainFrame::OnUpdatePmLteAlarm(CCmdUI *pCmdUI)						//225 LTE_性能预警_配置 
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("LTE_性能预警_配置"));
}

void CMainFrame::OnUpdatePmGsm(CCmdUI *pCmdUI)							//224 GSM_性能管理
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("GSM_性能管理"));
}

void CMainFrame::OnUpdatePmGsmCalCell(CCmdUI *pCmdUI)					//225 GSM_性能计算_CELL_HO
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("GSM_性能计算_CELL_HO"));
}

void CMainFrame::OnUpdatePmGsmCalBsc(CCmdUI *pCmdUI)					//226 GSM_性能计算_BSC
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("GSM_性能计算_BSC"));
}

//======================================================================================================
//	子菜单：系统参数
void CMainFrame::OnUpdateConfigPara(CCmdUI *pCmdUI)						//230 参数配置
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("参数配置"));
}

//======================================================================================================
//	子菜单状态设置：日志管理
void CMainFrame::OnUpdateLogSystem(CCmdUI *pCmdUI)						//310 系统日志查询
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("系统日志"));
}

void CMainFrame::OnUpdateLogBug(CCmdUI *pCmdUI)							//311 异常日志管理
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("系统异常"));
}

void CMainFrame::OnUpdateLogDatabase(CCmdUI *pCmdUI)					//312 数据库容量_LTE
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("数据库容量"));
}

void CMainFrame::OnUpdateLogDatabaseGsm(CCmdUI *pCmdUI)					//313 数据库容量_GSM
{
	// TODO: 在此添加命令更新用户界面处理程序代码
//	pCmdUI->Enable(My_Get_Power("数据库容量_GSM"));
	pCmdUI->Enable(false);
}

//======================================================================================================
//	子菜单状态设置：用户管理
void CMainFrame::OnUpdateUserSetup(CCmdUI *pCmdUI)						//320 用户管理
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("用户管理"));
}

void CMainFrame::OnUpdateUserPassword(CCmdUI *pCmdUI)					//321 密码修改
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("密码修改"));
}

void CMainFrame::OnUpdateUserRole(CCmdUI *pCmdUI)						//322 权限配置
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(My_Get_Power("权限配置"));
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CMainFrame::My_ExecuteSQL(CString v_sSql)
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
