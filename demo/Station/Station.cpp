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
#include "ImpIDispatch.h"

//------------------------------------------------------------------------------------------------------			
//	系统菜单功能函数
//------------------------------------------------------------------------------------------------------

//1列：小区数据预处理
#include "ConLTEBusyIndicator.h"                                        //LTE忙时指标
#include "Con_Station_Map.h"                                            //离线地图
#include "ConAnalyzeIndicator.h"                                        //指标分析

//2列：日志管理
#include "Log_System.h"													//日志查询
#include "Log_Bug.h"													//异常查询
#include "Log_DataBase.h"												//数据库容量查询

//3列：用户管理
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
	//---------------------------------------------------------------------------------
	ON_COMMAND(ID_Log_System, &CStaionApp::OnLogSystem)
	ON_COMMAND(ID_Log_Bug, &CStaionApp::OnLogBug)
	ON_COMMAND(ID_Log_DATABASE, &CStaionApp::OnLogDatabase)
	//---------------------------------------------------------------------------------
	ON_COMMAND(ID_User_PassWord, &CStaionApp::OnUserPassword)
	ON_COMMAND(ID_User_Login, &CStaionApp::OnUserLogin)
	ON_COMMAND(ID_User_Setup, &CStaionApp::OnUserSetup)
	ON_COMMAND(ID_User_Role, &CStaionApp::OnUserRole)
	//---------------------------------------------------------------------------------
	ON_COMMAND(ID_CON_STATION_MAP, &CStaionApp::OnConStationMap)
	ON_COMMAND(ID_CON_LTE_INDICATOR, &CStaionApp::OnConLteIndicator)
	ON_COMMAND(ID_CON_ANALYZE_INDICATOR, &CStaionApp::OnConAnalyzeIndicator)
END_MESSAGE_MAP()


//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CStaionApp::CStaionApp()
{
	m_bHiColorIcons = TRUE;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
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

	//初始化自定义IDispath 
	CoInitialize(NULL);													//
	m_pCustDisp = new CImpIDispatch;									//

	//初始化套接字
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	
	//开启一个用应用程序名创建信号量，用于：只能运行一次该软件【退出软件时，需要移除该信号量】
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
		
	//获取程序默认目录
	const int nBufSize = 512;											//尺寸：缓冲区
	TCHAR chBuf[nBufSize];												//定义：缓冲区
	ZeroMemory(chBuf,nBufSize);											//复位：缓冲区
 	::GetModuleFileNameA(NULL,chBuf,nBufSize);							//获取默认文件路径
	TCHAR* lpStrPath = chBuf;											//指针
	PathRemoveFileSpec(lpStrPath);										//去除：文件名称
	m_sPath=chBuf;														//获取：默认路径
		
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
	if(a_sDatabase_Ip.IsEmpty())
	{
		AfxMessageBox("系统配置文件config.ini不存在！！！");											//异常：配置文件没找到   
		return   false;																					//系统退出
	}

	//解密：数据库密码
	//DecrypteParam();													//密码解密

	//用户登陆信息：初始化
	a_sUser_Account = "";												//账号
	a_sUser_Name = "";													//名称
	a_sUser_Password ="";												//密码


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
		theApp.m_MyLog.WriteText("系统启动************************\r\n");	//Log
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
//	函数功能：配置管理_忙时性能指标提取 【2.1】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConLteIndicator()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;														//子窗名称
	Frame_Window	v_mFrame_Window;											//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;											//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;									//迭代器
	v_sFrame = "LTE忙时指标";													//子窗名称
	v_pIterator_Frame = v_mFrame.find(v_sFrame);								//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())										//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在【视窗最大化】
		return;															
	}
	else
	{
		CChildFrame* v_pFrame= new CChildFrame();								//不存在【新子窗体】
		v_mFrame_Window.v_pChildFrame=v_pFrame;									//指针
		v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
		CCreateContext context ;
		context.m_pCurrentDoc=m_pdoc;											//避免每次都产生一个新文档
		context.m_pNewViewClass = RUNTIME_CLASS(CConLTEBusyIndicator);			//类对象：KPIMSC数据查询视窗
		context.m_pNewDocTemplate = pDocTemplate;							
		context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		
		context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		
		if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
			return ;
		v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);									//视窗最大化
		v_pFrame->InitialUpdateFrame(m_pdoc,true);								//初始化更新窗体
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：配置管理_离线地图 【2.2】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConStationMap()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;														//子窗名称
	Frame_Window	v_mFrame_Window;											//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;											//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;									//迭代器
	v_sFrame = "离线地图";														//子窗名称
	v_pIterator_Frame = v_mFrame.find(v_sFrame);								//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())										//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在【视窗最大化】
		return;															
	}
	else
	{
		CChildFrame* v_pFrame= new CChildFrame();								//不存在【新子窗体】
		v_mFrame_Window.v_pChildFrame=v_pFrame;									//指针
		v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
		CCreateContext context ;
		context.m_pCurrentDoc=m_pdoc;											//避免每次都产生一个新文档
		context.m_pNewViewClass = RUNTIME_CLASS(CCon_Station_Map);				//类对象：KPIMSC数据查询视窗
		context.m_pNewDocTemplate = pDocTemplate;							
		context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		
		context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		
		if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
			return ;
		v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);									//视窗最大化
		v_pFrame->InitialUpdateFrame(m_pdoc,true);								//初始化更新窗体
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：日志管理_系统日志 【2.1】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogSystem()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;														//子窗名称
	Frame_Window	v_mFrame_Window;											//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;											//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;									//迭代器
	v_sFrame = "系统日志查询";													//子窗名称【系统日志查询】
	v_pIterator_Frame = v_mFrame.find(v_sFrame);								//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())										//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在【视窗最大化】
		return;															
	}
	else
	{
		CChildFrame* v_pFrame= new CChildFrame();								//不存在【新子窗体】
		v_mFrame_Window.v_pChildFrame=v_pFrame;									//指针
		v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
		CCreateContext context ;
		context.m_pCurrentDoc=m_pdoc;											//避免每次都产生一个新文档
		context.m_pNewViewClass = RUNTIME_CLASS(CLog_System);					//类对象：KPIMSC数据查询视窗
		context.m_pNewDocTemplate = pDocTemplate;							
		context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		
		context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		
		if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
			return ;
		v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);									//视窗最大化
		v_pFrame->InitialUpdateFrame(m_pdoc,true);								//初始化更新窗体
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：日志管理_系统异常 【2.2】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogBug()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;														//子窗名称
	Frame_Window	v_mFrame_Window;											//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;											//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;									//迭代器
	v_sFrame = "系统异常查询";													//子窗名称
	v_pIterator_Frame = v_mFrame.find(v_sFrame);								//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())										//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在【视窗最大化】
		return;															
	}
	else
	{
		CChildFrame* v_pFrame= new CChildFrame();								//不存在【新子窗体】
		v_mFrame_Window.v_pChildFrame=v_pFrame;									//指针
		v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
		CCreateContext context ;
		context.m_pCurrentDoc=m_pdoc;											//避免每次都产生一个新文档
		context.m_pNewViewClass = RUNTIME_CLASS(CLog_Bug);						//类对象：KPIMSC数据查询视窗
		context.m_pNewDocTemplate = pDocTemplate;							
		context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		
		context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		
		if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
			return ;
		v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);									//视窗最大化
		v_pFrame->InitialUpdateFrame(m_pdoc,true);								//初始化更新窗体
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：数据库容量【2.3】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogDatabase()
{
	// TODO: 在此添加命令处理程序代码
	CString		v_sFrame;														//子窗名称
	Frame_Window	v_mFrame_Window;											//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;											//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;									//迭代器
	v_sFrame = "数据库容量";													//子窗名称
	v_pIterator_Frame = v_mFrame.find(v_sFrame);								//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())										//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在【视窗最大化】
		return;															
	}
	else
	{
		CChildFrame* v_pFrame= new CChildFrame();								//不存在【新子窗体】
		v_mFrame_Window.v_pChildFrame=v_pFrame;									//指针
		v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
		CCreateContext context ;
		context.m_pCurrentDoc=m_pdoc;											//避免每次都产生一个新文档
		context.m_pNewViewClass = RUNTIME_CLASS(CLog_DataBase);					//类对象：KPIMSC数据查询视窗
		context.m_pNewDocTemplate = pDocTemplate;							
		context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		
		context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		
		if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
			return ;
		v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);									//视窗最大化
		v_pFrame->InitialUpdateFrame(m_pdoc,true);								//初始化更新窗体
	}
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：用户管理_用户管理 【3.1】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserSetup()
{
	// TODO: 在此添加命令处理程序代码
	CUser_Setup	v_cDlg;													//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：用户管理_修改密码 【3.2】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserPassword()
{
	// TODO: 在此添加命令处理程序代码
	CUser_Password	v_cDlg;												//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：用户管理_用户登录 【3.3】
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
//	函数功能：用户管理_权限配置 【3.4】
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserRole()
{
	// TODO: 在此添加命令处理程序代码
	CUser_Role	v_cDlg;													//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：RSA解密 【数据库密码】
//------------------------------------------------------------------------------------------------------
bool CStaionApp::DecrypteParam()
{
	CRSA Rsa;															//实例化
	Rsa.SetDecrypteParam(PASSWORDRSAD,PASSWORDRSAN);					//设置数据库密码解密参数
	a_sDatabase_User = Rsa.DecryptStr(a_sDatabase_User);				//解密：用户名称
	a_sDatabase_Password = Rsa.DecryptStr(a_sDatabase_Password);		//解密：用户密码
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



void CStaionApp::OnConAnalyzeIndicator()
{
	// TODO: 在此添加命令处理程序代码
		CString		v_sFrame;														//子窗名称
	Frame_Window	v_mFrame_Window;											//临时变量	
	v_mFrame_Window.v_iThread_Exit=0;											//初始状态
	mFrame_Window::iterator	v_pIterator_Frame;									//迭代器
	v_sFrame = "分析指标";													//子窗名称
	v_pIterator_Frame = v_mFrame.find(v_sFrame);								//查找子窗名称
	if(v_pIterator_Frame != v_mFrame.end())										//子窗存在？
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//存在【视窗最大化】
		return;															
	}
	else
	{
		CChildFrame* v_pFrame= new CChildFrame();								//不存在【新子窗体】
		v_mFrame_Window.v_pChildFrame=v_pFrame;									//指针
		v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//容器保存窗体指针							
		CCreateContext context ;
		context.m_pCurrentDoc=m_pdoc;											//避免每次都产生一个新文档
		context.m_pNewViewClass = RUNTIME_CLASS(CConAnalyzeIndicator);					//类对象：KPIMSC数据查询视窗
		context.m_pNewDocTemplate = pDocTemplate;							
		context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		
		context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		
		if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//加载视窗
			return ;
		v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);									//视窗最大化
		v_pFrame->InitialUpdateFrame(m_pdoc,true);								//初始化更新窗体
	}
}
