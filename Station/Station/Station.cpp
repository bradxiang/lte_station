
// Station.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"

#include "MyRSA.h"

//0列：系统类
#include "Station.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "StationDoc.h"
#include "StationView.h"
#include "MyChartViewer.h"

#include "ImpIDispatch.h"

//系统菜单功能函数
//1列：LTE 性能管理
#include "LTE_PM_Data.h"
#include "LTE_PM_Draw.h"
#include "LTE_PM_Data_Cell.h"
#include "LTE_PM_Data_Real.h"
#include "LTE_PM_Data_Cal.h"
#include "LTE_PM_Data_Init.h"
#include "LTE_PM_Draw_Znit.h"

//2列：GSM 性能管理

//3列：性能预警
#include "Alarm_PM.h"
#include "Alarm_LL_Real.h"
#include "Alarm_LL_Cell.h"

//4列：共址性能
#include "All_PM_Draw.h"

//4列：共站查询
#include "QUA_Station.h"
#include "QUA_Station_Map.h"
#include "QUA_Element_TD.h"
#include "QUA_Element_GSM.h"

//5列：配置管理
#include "CM_LTE_File.h"
#include "CM_LTE_Para.h"
#include "CM_LTE_Quary.h"
#include "CM_LTE_Relation.h"

//5列：共站配置
#include "CON_Station.h"
#include "CON_Region.h"
#include "CON_BTS.h"
#include "CON_E_NodeB.h"
#include "CON_GSM_Cell.h"
#include "CON_LTE_Cell.h"
#include "CON_Tools_XML.h"

//6列：性能管理
#include "Pm_LTE.h"														//LTE_性能管理_原始
#include "PM_LTE_Cal.h"													//LTE_性能管理_计算
#include "PM_LTE_Region.h"												//LTE_性能管理_区域
#include "PM_LTE_Case_Cell.h"											//LTE_性能查询模板_小区
#include "PM_LTE_Case_PM.h"												//LTE_性能查询模板_性能
#include "PM_LTE_Case_Cal.h"											//LTE_性能查询模板_计算
#include "PM_LTE_Alarm.h"												//LTE_性能预警_配置
#include "Pm_GSM.h"														//GSM_性能管理
#include "Pm_GSM_Cell_HO.h"												//
#include "Pm_GSM_BSC.h"

//8列：系统参数
#include "CON_Para.h"													//参数配置

//9列：日志管理
#include "Log_System.h"													//日志查询
#include "Log_Bug.h"													//异常查询
#include "Log_DataBase.h"												//数据库容量查询
#include "Log_DataBase_GSM.h"

//10列：用户管理
#include "User_Setup.h"													//用户管理
#include "User_Password.h"												//密码修改
#include "User_Login.h"													//用户登录
#include "User_Role.h"													//权限配置

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
// CStaionApp
BEGIN_MESSAGE_MAP(CStaionApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CStaionApp::OnAppAbout)
	ON_COMMAND(ID_User_Login, &CStaionApp::OnUserLogin)
	ON_COMMAND(ID_Log_System, &CStaionApp::OnLogSystem)
	ON_COMMAND(ID_Log_Bug, &CStaionApp::OnLogBug)
	ON_COMMAND(ID_Log_DATABASE, &CStaionApp::OnLogDatabase)
	ON_COMMAND(ID_User_Setup, &CStaionApp::OnUserSetup)
	ON_COMMAND(ID_User_PassWord, &CStaionApp::OnUserPassword)
	ON_COMMAND(ID_User_Role, &CStaionApp::OnUserRole)
	//---------------------------------------------------------------------------------
	ON_COMMAND(ID_Config_Station, &CStaionApp::OnConfigStation)
	ON_COMMAND(ID_Config_Region, &CStaionApp::OnConfigRegion)
	ON_COMMAND(ID_Config_BTS, &CStaionApp::OnConfigBts)
	ON_COMMAND(ID_Config_GSM_Cell, &CStaionApp::OnConfigGsmCell)
	ON_COMMAND(ID_Config_LTE_Cell, &CStaionApp::OnConfigLteCell)
	ON_COMMAND(ID_Config_E_NodeB, &CStaionApp::OnConfigENodeb)
	ON_COMMAND(ID_Config_Tools, &CStaionApp::OnConfigTools)
	ON_COMMAND(ID_QUARY_STATION_Map, &CStaionApp::OnQuaryStationMap)
	ON_COMMAND(ID_Pm_LTE, &CStaionApp::OnPmLte)
	ON_COMMAND(ID_Config_Para, &CStaionApp::OnConfigPara)
	ON_COMMAND(ID_LTE_PM_DRAW, &CStaionApp::OnLtePmDraw)
	ON_COMMAND(ID_LTE_PM_DATA, &CStaionApp::OnLtePmData)
	ON_COMMAND(ID_ALL_PM_DRAW, &CStaionApp::OnAllPmDraw)
	ON_COMMAND(ID_Pm_LTE_Cal, &CStaionApp::OnPmLteCal)
	ON_COMMAND(ID_Pm_LTE_Cas, &CStaionApp::OnPmLteCas)
	ON_COMMAND(ID_Pm_GSM, &CStaionApp::OnPmGsm)
	ON_COMMAND(ID_Pm_GSM_Cal_CELL, &CStaionApp::OnPmGsmCalCell)
	ON_COMMAND(ID_Pm_GSM_Cal_BSC, &CStaionApp::OnPmGsmCalBsc)
	ON_COMMAND(ID_Log_DATABASE_GSM, &CStaionApp::OnLogDatabaseGsm)
	ON_COMMAND(ID_GSM_PM_DRAW, &CStaionApp::OnGsmPmDraw)
	ON_COMMAND(ID_GSM_PM_DATA, &CStaionApp::OnGsmPmData)
	ON_COMMAND(ID_LTE_PM_DRAW_Init, &CStaionApp::OnLtePmDrawInit)
	ON_COMMAND(ID_LTE_PM_DATA_Init, &CStaionApp::OnLtePmDataInit)
	ON_COMMAND(ID_ALARM_PM, &CStaionApp::OnAlarmPm)
	ON_COMMAND(ID_Pm_LTE_Region, &CStaionApp::OnPmLteRegion)
	ON_COMMAND(ID_Pm_LTE_Cas_PM, &CStaionApp::OnPmLteCasPm)
	ON_COMMAND(ID_LTE_PM_DATA_REAL, &CStaionApp::OnLtePmDataReal)
	ON_COMMAND(ID_LTE_PM_DATA_CELL, &CStaionApp::OnLtePmDataCell)
	ON_COMMAND(ID_Pm_LTE_ALARM, &CStaionApp::OnPmLteAlarm)
	ON_COMMAND(ID_ALARM_LL_CELL, &CStaionApp::OnAlarmLlCell)
	ON_COMMAND(ID_Cm_LTE_Quary, &CStaionApp::OnCmLteQuary)
	ON_COMMAND(ID_Cm_LTE_File, &CStaionApp::OnCmLteFile)
	ON_COMMAND(ID_Cm_LTE_Para, &CStaionApp::OnCmLtePara)
	ON_COMMAND(ID_LTE_PM_DATA_CAL, &CStaionApp::OnLtePmDataCal)
	ON_COMMAND(ID_Pm_LTE_Cas_Cal, &CStaionApp::OnPmLteCasCal)
	ON_COMMAND(ID_QUARY_STATION, &CStaionApp::OnQuaryStation)
	ON_COMMAND(ID_QUARY_TDLTE, &CStaionApp::OnQuaryTdlte)
	ON_COMMAND(ID_Quary_GSM, &CStaionApp::OnQuaryGsm)
	ON_COMMAND(ID_QUARY_GSM_GT, &CStaionApp::OnQuaryGsmGt)
	ON_COMMAND(ID_ALARM_LL_REAL, &CStaionApp::OnAlarmLlReal)
	ON_COMMAND(ID_Cm_LTE_Relation, &CStaionApp::OnCmLteRelation)
END_MESSAGE_MAP()


//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CStaionApp::CStaionApp()
{
	m_bHiColorIcons = TRUE;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	COleDateTime vd(2000,1,1,0,0,0);									//默认：无效日期

	a_tData_Default=vd;
}

// 唯一的一个 CStaionApp 对象
CStaionApp theApp;

//------------------------------------------------------------------------------------------------------			
//	函数功能：初始化
//------------------------------------------------------------------------------------------------------
BOOL CStaionApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	// 则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	CoInitialize(NULL);													//

	m_pCustDisp = new CImpIDispatch;									//

	//初始化套接字
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 初始化 OLE 库
//	if (!AfxOleInit())
//	{
//		AfxMessageBox(IDP_OLE_INIT_FAILED);
//		return FALSE;
//	}

	////////////////////////////////////////////////////////////////////////////////////////////////////			
	//开启一个用应用程序名创建信号量，用于：只能运行一次该软件 [退出软件时，需要移除该信号量]
	HANDLE hSem = CreateSemaphore(NULL,1,1,m_pszAppName);				//生成：信号量句柄  
	if   (GetLastError()   ==   ERROR_ALREADY_EXISTS)					//信号量已存在？   
	{   																//失败：即号量存在，则程序已有一个实例运行   
		CloseHandle(hSem);   											//关闭：信号量句柄   
		//寻找先前已经打开的窗口   
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(),GW_CHILD);	//获得：窗体
		while (::IsWindow(hWndPrevious))								//有效窗体？
		{   
			if(::GetProp(hWndPrevious,m_pszAppName))					//检查：窗口预设的标记==m_pszAppName(即：软件设置的特定字符串)
			{															//有，则是寻找的主窗   
				if (!::IsIconic(hWndPrevious))							//主窗口没有最小化？
					::ShowWindow(hWndPrevious,SW_SHOWMINIMIZED);		//最小化：主窗口
				::ShowWindow(hWndPrevious,SW_SHOWMAXIMIZED);			//最大化：主窗口
				return   false;   										//退出本实例   
			}   
			hWndPrevious = ::GetWindow(hWndPrevious,GW_HWNDNEXT); 		//获得：继续寻找下一个窗口
		}   
		AfxMessageBox("程序已运行！");									//前一实例已存在，但找不到其主窗，可能出错了   
		return   false;													//退出本实例   
	}  

	////////////////////////////////////////////////////////////////////////////////////////////////////			
	//获取程序默认目录
	const int nBufSize = 512;											//尺寸：缓冲区
	TCHAR chBuf[nBufSize];												//定义：缓冲区
	ZeroMemory(chBuf,nBufSize);											//复位：缓冲区
 	::GetModuleFileNameA(NULL,chBuf,nBufSize);							//获取默认文件路径
	TCHAR* lpStrPath = chBuf;											//指针
	PathRemoveFileSpec(lpStrPath);										//去除：文件名称
	m_sPath=chBuf;														//获取：默认路径

	////////////////////////////////////////////////////////////////////////////////////////////////////			
	//读取配置文件：config.ini
	char v_cBuffer[512];
	GetPrivateProfileString("DATABASE_SERVER","DATABASE_SERVER",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_Ip.Format("%s",v_cBuffer);																//数据库服务器地址
	GetPrivateProfileString("DATABASE_NAME","DATABASE_NAME",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_Name.Format("%s",v_cBuffer);															//获得数据库名称
	GetPrivateProfileString("DATABASE_USER","DATABASE_USER",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_User.Format("%s",v_cBuffer);															//获得数据库用户
	GetPrivateProfileString("DATABASE_PASSWORD","DATABASE_PASSWORD",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_Password.Format("%s",v_cBuffer);														//获得数据库密码
	a_iDatabase_Type=GetPrivateProfileInt("DATABASE_TYPE","DATABASE_TYPE",0,".\\config.ini");			//获得数据库类型

	GetPrivateProfileString("DATABASE_SERVER_GSM","DATABASE_SERVER_GSM",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_GSM_Ip.Format("%s",v_cBuffer);															//数据库服务器地址
	GetPrivateProfileString("DATABASE_NAME_GSM","DATABASE_NAME_GSM",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_GSM_Name.Format("%s",v_cBuffer);														//获得数据库名称
	GetPrivateProfileString("DATABASE_USER_GSM","DATABASE_USER_GSM",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_GSM_User.Format("%s",v_cBuffer);														//获得数据库用户
	GetPrivateProfileString("DATABASE_PASSWORD_GSM","DATABASE_PASSWORD_GSM",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_GSM_Password.Format("%s",v_cBuffer);													//获得数据库密码

	GetPrivateProfileString("HTTP_MAP","HTTP_MAP",NULL,v_cBuffer,512,".\\config.ini");
	m_sHTTP_Map.Format("%s",v_cBuffer);																	//获得Web路径

	if(a_sDatabase_Ip.IsEmpty())
	{
		AfxMessageBox("系统配置文件( config.ini )不存在！！！");										//异常：配置文件没找到   
		return   false;																					//系统退出
	}

	//解密：数据库密码
	//DecrypteParam();													//密码解密

	//用户登陆信息：初始化
	a_sUser_Account = "";												//账号
	a_sUser_Name = "";													//名称
	a_sUser_Password ="";												//密码

	Chart::setLicenseCode("RDST-25ZA-3JX6-GCTB-A07B-8F0E");				//Chart：License

	// 标准初始化：如果未使用这些功能并希望减小最终可执行文件的大小，则应移除下列不需要的特定初始化例程更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_RTSPAATYPE,
		RUNTIME_CLASS(CStaionDoc),
		RUNTIME_CLASS(CChildFrame),										//自定义 MDI 子框架
		RUNTIME_CLASS(CStaionView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生


	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	pMainFrame->Invalidate();											//刷新屏幕

	//初始化：服务器通信异常窗体
	a_pDlg_Alarm_Message = new CAlarm_Message;							//预警消息
	a_pDlg_Alarm_Message->Create(IDD_ALARM_MESSAGE);					//生成窗体
	a_pDlg_Alarm_Message->ShowWindow(SW_HIDE);							//隐藏窗体

	//Log：程序开始
	OnUserLogin();														//用户登录
	if (!theApp.a_sUser_Name.IsEmpty())
	{
		//Log：程序开始
		theApp.m_MyLog.WriteText("系统开始********************\r\n");	//Log
	}

	return TRUE;
}


//------------------------------------------------------------------------------------------------------			
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
//------------------------------------------------------------------------------------------------------
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CStaionApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

//------------------------------------------------------------------------------------------------------			
// CStaionApp 自定义加载/保存方法
//------------------------------------------------------------------------------------------------------
void CStaionApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：加载应用程序状态框架
//------------------------------------------------------------------------------------------------------
void CStaionApp::LoadCustomState()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：保存应用程序的状态对注册表后,框架调用此方法
//------------------------------------------------------------------------------------------------------
void CStaionApp::SaveCustomState()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：LTE 性能管理_LTE 性能图形查询	【100】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmDraw()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE 性能图形查询";										//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Draw);				//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：LTE 性能管理_LTE 性能数据查询	【101】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmData()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE 性能数据查询";										//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Data);				//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：LTE 性能管理_LTE 性能数据查询_小区	【102】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmDataCell()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE 性能数据查询_小区";									//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Data_Cell);			//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：LTE 性能管理_LTE 性能数据查询_实时	【103】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmDataReal()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE 性能数据查询_实时";									//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Data_Real);			//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：LTE 性能管理_LTE 性能数据计算_小区	【104】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmDataCal()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE 性能数据计算_小区";									//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Data_Cal);			//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：LTE 性能管理_LTE 性能图形查询_原始	【108】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmDrawInit()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE 性能图形查询_原始";									//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Draw_Znit);			//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：LTE 性能管理_LTE 性能数据查询_原始	【109】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmDataInit()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE 性能数据查询_原始";									//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Data_Init);			//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：GSM 性能管理_GSM 性能图形查询	【110】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnGsmPmDraw()
{
	// TODO: 在此添加命令处理程序代码
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：GSM 性能管理_GSM 性能数据查询	【111】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnGsmPmData()
{
	// TODO: 在此添加命令处理程序代码
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能预警_性能预警查询	【130】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnAlarmPm()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "性能预警查询";											//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CAlarm_PM);					//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能预警_LTE 低流量小区查询	【131】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnAlarmLlReal()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE 低流量小区实时";									//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CAlarm_LL_Real);			//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能预警_LTE 低流量小区查询	【132】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnAlarmLlCell()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE 低流量小区查询";									//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CAlarm_LL_Cell);			//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能管理_共址性能查询	【3.1】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnAllPmDraw()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "共址性能查询";											//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CAll_PM_Draw);				//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：网元查询_站址查询	【4.1】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnQuaryStation()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "站址查询";												//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CQUA_Station);				//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：网元查询_站址地图查询	【4.2】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnQuaryStationMap()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "站址地图查询";											//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CQUA_Station_Map);			//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：网元查询_TD-LTE网元查询	【4.3】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnQuaryTdlte()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "TD-LTE网元查询";										//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CQUA_Element_TD);			//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：网元查询_GSM网元查询	【4.4】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnQuaryGsm()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "GSM网元查询";											//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CQUA_Element_GSM);			//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：网元查询_GSM高铁网元查询	【4.5】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnQuaryGsmGt()
{
	// TODO: 在此添加命令处理程序代码
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：配置管理_LTE 配置查询
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnCmLteQuary()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE 配置查询";											//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CCM_LTE_Quary);				//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：配置管理_LTE-Tdd相邻关系
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnCmLteRelation()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE-Tdd相邻关系";										//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CCM_LTE_Relation);			//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：配置管理_LTE 配置管理_文件
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnCmLteFile()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE 配置管理_文件";										//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CCM_LTE_File);				//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：配置管理_LTE 配置管理_参数
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnCmLtePara()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE 配置管理_参数";										//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CCM_LTE_Para);				//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：数据配置_区域设置	【5.1】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigRegion()
{
	// TODO: 在此添加命令处理程序代码
	CCON_Region	v_cDlg;													//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：数据配置_站址管理	【5.2】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigStation()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "站址管理";												//子窗名称：站址管理

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CCON_Station);			//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：数据配置_BTS 配置	【5.3】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigBts()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "BTS 配置";												//子窗名称：BTS 配置

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CCON_BTS);					//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：数据配置_E_NodeB 配置		【5.5】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigENodeb()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "E_NodeB 配置";											//子窗名称：E_NodeB 配置

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CCON_E_NodeB);				//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：数据配置_GSM Cell 配置	【5.6】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigGsmCell()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "GSM Cell 配置";											//子窗名称：GSM Cell 配置

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CCON_GSM_Cell);				//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：数据配置_LTE Cell 配置	【5.8】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigLteCell()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE Cell 配置";											//子窗名称：LTE Cell 配置

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CCON_LTE_Cell);				//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：数据配置_XML文件工具  【5.9】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigTools()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "XML文件工具";											//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CCON_Tools_XML);			//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能管理_LTE_性能管理 【220】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLte()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE_性能管理_原始";										//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CPm_LTE);					//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能管理_LTE_性能管理_计算 【221】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLteCal()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE_性能管理_计算";										//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CPM_LTE_Cal);				//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能管理_LTE_性能管理_区域 【222】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLteRegion()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE_性能管理_区域";										//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CPM_LTE_Region);			//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能管理_LTE_性能查询模板_小区 【223】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLteCas()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE_性能查询模板_小区";									//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CPM_LTE_Case_Cell);			//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能管理_LTE_性能查询模板_性能 【224】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLteCasPm()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE_性能查询模板_性能";									//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CPM_LTE_Case_PM);			//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能管理_LTE_性能查询模板_计算 【226】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLteCasCal()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "LTE_性能查询模板_计算";									//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CPM_LTE_Case_Cal);			//类对象
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能管理_LTE_性能预警_配置 【225】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLteAlarm()
{
	// TODO: 在此添加命令处理程序代码
	CPM_LTE_Alarm	v_cDlg;												//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能管理_GSM_性能管理 【6.4】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmGsm()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "GSM_性能管理";											//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CPm_GSM);					//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能管理_GSM_性能计算_CELL_HO 【6.5】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmGsmCalCell()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "GSM_性能计算_CELL_HO";									//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CPm_GSM_Cell_HO);			//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能管理_GSM_性能计算_BSC 【6.6】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmGsmCalBsc()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "GSM_性能计算_BSC";										//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CPm_GSM_BSC);				//类对象：XML文件工具视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：系统参数_参数配置  【8.1】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigPara()
{
	// TODO: 在此添加命令处理程序代码
	CCON_Para	v_cDlg;													//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：日志管理_系统日志 【9.1】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogSystem()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "系统日志查询";											//子窗名称：系统日志查询

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CLog_System);				//类对象：KPIMSC数据查询视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：日志管理_系统异常 【9.2】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogBug()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "系统异常查询";											//子窗名称：系统异常查询

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CLog_Bug);					//类对象：KPIMSC数据查询视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：数据库容量_LTE 【9.3】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogDatabase()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "数据库容量_LTE";										//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CLog_DataBase);				//类对象：分发参数设置视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：数据库容量_GSM 【9.4】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogDatabaseGsm()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;												//临时变量	
	Frame_Window	v_mFrame_Window;									//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;									//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_sFrame = "数据库容量_GSM";										//子窗名称

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())								//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在：视窗最大化
		return;															//返回
	}
	CChildFrame* v_pFrame= new CChildFrame();							//实例：新子窗体
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//指针
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//避免每次都产生一个新文档
	context.m_pNewViewClass = RUNTIME_CLASS(CLog_DataBase_GSM);			//类对象：分发参数设置视窗
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//视窗最大化
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//初始化更新窗体
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：用户管理_用户管理 【10.1】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserSetup()
{
	// TODO: 在此添加命令处理程序代码
	CUser_Setup	v_cDlg;													//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：用户管理_修改密码 【10.2】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserPassword()
{
	// TODO: 在此添加命令处理程序代码
	CUser_Password	v_cDlg;												//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：用户管理_用户登录 【10.3】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserLogin()
{
	// TODO: 在此添加命令处理程序代码
	CUser_Login	v_cDlg;													//实例化
	if (v_cDlg.DoModal() == IDOK)										//打开对话窗
	{
		//显示：用户登录信息
		CMainFrame* pMain = (CMainFrame*)(AfxGetApp()->m_pMainWnd);		//MainFrame指针
		CString v_sUser="用户：";
		v_sUser+=theApp.a_sUser_Name;									//用户名称
		pMain->m_wndStatusBar.SetPaneText(2,v_sUser);					//Status显示
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：用户管理_权限配置 【10.4】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserRole()
{
	// TODO: 在此添加命令处理程序代码
	CUser_Role	v_cDlg;													//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：RSA解密 [数据库密码]
//------------------------------------------------------------------------------------------------------
bool CStaionApp::DecrypteParam()
{
	CRSA Rsa;															//实例化
	Rsa.SetDecrypteParam(PASSWORDRSAD,PASSWORDRSAN);					//设置数据库密码解密参数
	a_sDatabase_User = Rsa.DecryptStr(a_sDatabase_User);				//解密：用户名称
	a_sDatabase_Password = Rsa.DecryptStr(a_sDatabase_Password);		//解密：用户密码

 	a_sDatabase_GSM_User = Rsa.DecryptStr(a_sDatabase_GSM_User);		//解密：用户名称
	a_sDatabase_GSM_Password = Rsa.DecryptStr(a_sDatabase_GSM_Password);//解密：用户密码

	return TRUE;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：退出处理【COM接口】
//------------------------------------------------------------------------------------------------------
int CStaionApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pCustDisp)
	{
		delete m_pCustDisp;
		m_pCustDisp = NULL;
	}
	CoUninitialize();
	return CWinApp::ExitInstance();
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
