// Station.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "MyRSA.h"

//0�У�ϵͳ��
#include "Station.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "StationDoc.h"
#include "StationView.h"
#include "ImpIDispatch.h"

//------------------------------------------------------------------------------------------------------			
//	ϵͳ�˵����ܺ���
//------------------------------------------------------------------------------------------------------

//1�У�С������Ԥ����
#include "ConLTEBusyIndicator.h"                                        //LTEæʱָ��
#include "Con_Station_Map.h"                                            //���ߵ�ͼ
#include "ConAnalyzeIndicator.h"                                        //ָ�����

//2�У���־����
#include "Log_System.h"													//��־��ѯ
#include "Log_Bug.h"													//�쳣��ѯ
#include "Log_DataBase.h"												//���ݿ�������ѯ

//3�У��û�����
#include "User_Setup.h"													//�û�����
#include "User_Password.h"												//�����޸�
#include "User_Login.h"													//�û���¼
#include "User_Role.h"													//Ȩ������

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
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
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CStaionApp::CStaionApp()
{
	m_bHiColorIcons = TRUE;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CStaionApp ����
CStaionApp theApp;

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CStaionApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	// ����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õĹ����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	//��ʼ���Զ���IDispath 
	CoInitialize(NULL);													//
	m_pCustDisp = new CImpIDispatch;									//

	//��ʼ���׽���
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	
	//����һ����Ӧ�ó����������ź��������ڣ�ֻ������һ�θ�������˳����ʱ����Ҫ�Ƴ����ź�����
	HANDLE hSem = CreateSemaphore(NULL,1,1,m_pszAppName);				//���ɣ��ź������  
	if   (GetLastError()   ==   ERROR_ALREADY_EXISTS)					//�ź����Ѵ��ڣ�   
	{   																//ʧ�ܣ����������ڣ����������һ��ʵ������   
		CloseHandle(hSem);   											//�رգ��ź������   
		//Ѱ����ǰ�Ѿ��򿪵Ĵ���   
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(),GW_CHILD);	//��ã�����
		while (::IsWindow(hWndPrevious))								//��Ч���壿
		{   
			if(::GetProp(hWndPrevious,m_pszAppName))					//��飺����Ԥ��ı��==m_pszAppName(����������õ��ض��ַ���)
			{															//�У�����Ѱ�ҵ�����   
				if (!::IsIconic(hWndPrevious))							//������û����С����
					::ShowWindow(hWndPrevious,SW_SHOWMINIMIZED);		//��С����������
				::ShowWindow(hWndPrevious,SW_SHOWMAXIMIZED);			//��󻯣�������
				return   false;   										//�˳���ʵ��   
			}   
			hWndPrevious = ::GetWindow(hWndPrevious,GW_HWNDNEXT); 		//��ã�����Ѱ����һ������
		}   
		AfxMessageBox("���������У�");									//ǰһʵ���Ѵ��ڣ����Ҳ��������������ܳ�����   
		return   false;													//�˳���ʵ��   
	}  
		
	//��ȡ����Ĭ��Ŀ¼
	const int nBufSize = 512;											//�ߴ磺������
	TCHAR chBuf[nBufSize];												//���壺������
	ZeroMemory(chBuf,nBufSize);											//��λ��������
 	::GetModuleFileNameA(NULL,chBuf,nBufSize);							//��ȡĬ���ļ�·��
	TCHAR* lpStrPath = chBuf;											//ָ��
	PathRemoveFileSpec(lpStrPath);										//ȥ�����ļ�����
	m_sPath=chBuf;														//��ȡ��Ĭ��·��
		
	//��ȡ�����ļ���config.ini
	char v_cBuffer[512];
	GetPrivateProfileString("DATABASE_SERVER","DATABASE_SERVER",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_Ip.Format("%s",v_cBuffer);																//���ݿ��������ַ
	GetPrivateProfileString("DATABASE_NAME","DATABASE_NAME",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_Name.Format("%s",v_cBuffer);															//������ݿ�����
	GetPrivateProfileString("DATABASE_USER","DATABASE_USER",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_User.Format("%s",v_cBuffer);															//������ݿ��û�
	GetPrivateProfileString("DATABASE_PASSWORD","DATABASE_PASSWORD",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_Password.Format("%s",v_cBuffer);														//������ݿ�����
	a_iDatabase_Type=GetPrivateProfileInt("DATABASE_TYPE","DATABASE_TYPE",0,".\\config.ini");			//������ݿ�����
	if(a_sDatabase_Ip.IsEmpty())
	{
		AfxMessageBox("ϵͳ�����ļ�config.ini�����ڣ�����");											//�쳣�������ļ�û�ҵ�   
		return   false;																					//ϵͳ�˳�
	}

	//���ܣ����ݿ�����
	//DecrypteParam();													//�������

	//�û���½��Ϣ����ʼ��
	a_sUser_Account = "";												//�˺�
	a_sUser_Name = "";													//����
	a_sUser_Password ="";												//����


	// ��׼��ʼ�������δʹ����Щ���ܲ�ϣ����С���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ����в���Ҫ���ض���ʼ�����̸������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ����������޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_RTSPAATYPE,
		RUNTIME_CLASS(CStaionDoc),
		RUNTIME_CLASS(CChildFrame),										//�Զ��� MDI �ӿ��
		RUNTIME_CLASS(CStaionView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������


	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	pMainFrame->Invalidate();											//ˢ����Ļ

	//��ʼ����������ͨ���쳣����
	a_pDlg_Alarm_Message = new CAlarm_Message;							//Ԥ����Ϣ
	a_pDlg_Alarm_Message->Create(IDD_ALARM_MESSAGE);					//���ɴ���
	a_pDlg_Alarm_Message->ShowWindow(SW_HIDE);							//���ش���

	//Log������ʼ
	OnUserLogin();														//�û���¼
	if (!theApp.a_sUser_Name.IsEmpty())
	{
		//Log������ʼ
		theApp.m_MyLog.WriteText("ϵͳ����************************\r\n");	//Log
	}

	return TRUE;
}


//------------------------------------------------------------------------------------------------------			
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
//------------------------------------------------------------------------------------------------------
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CStaionApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

//------------------------------------------------------------------------------------------------------			
// CStaionApp �Զ������/���淽��
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
//	�������ܣ�����Ӧ�ó���״̬���
//------------------------------------------------------------------------------------------------------
void CStaionApp::LoadCustomState()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Ӧ�ó����״̬��ע����,��ܵ��ô˷���
//------------------------------------------------------------------------------------------------------
void CStaionApp::SaveCustomState()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ù���_æʱ����ָ����ȡ ��2.1��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConLteIndicator()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;														//�Ӵ�����
	Frame_Window	v_mFrame_Window;											//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;											//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;									//������
	v_sFrame = "LTEæʱָ��";													//�Ӵ�����
	v_pIterator_Frame = v_mFrame.find(v_sFrame);								//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())										//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڡ��Ӵ���󻯡�
		return;															
	}
	else
	{
		CChildFrame* v_pFrame= new CChildFrame();								//�����ڡ����Ӵ��塿
		v_mFrame_Window.v_pChildFrame=v_pFrame;									//ָ��
		v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
		CCreateContext context ;
		context.m_pCurrentDoc=m_pdoc;											//����ÿ�ζ�����һ�����ĵ�
		context.m_pNewViewClass = RUNTIME_CLASS(CConLTEBusyIndicator);			//�����KPIMSC���ݲ�ѯ�Ӵ�
		context.m_pNewDocTemplate = pDocTemplate;							
		context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		
		context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		
		if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
			return ;
		v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);									//�Ӵ����
		v_pFrame->InitialUpdateFrame(m_pdoc,true);								//��ʼ�����´���
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ù���_���ߵ�ͼ ��2.2��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConStationMap()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;														//�Ӵ�����
	Frame_Window	v_mFrame_Window;											//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;											//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;									//������
	v_sFrame = "���ߵ�ͼ";														//�Ӵ�����
	v_pIterator_Frame = v_mFrame.find(v_sFrame);								//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())										//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڡ��Ӵ���󻯡�
		return;															
	}
	else
	{
		CChildFrame* v_pFrame= new CChildFrame();								//�����ڡ����Ӵ��塿
		v_mFrame_Window.v_pChildFrame=v_pFrame;									//ָ��
		v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
		CCreateContext context ;
		context.m_pCurrentDoc=m_pdoc;											//����ÿ�ζ�����һ�����ĵ�
		context.m_pNewViewClass = RUNTIME_CLASS(CCon_Station_Map);				//�����KPIMSC���ݲ�ѯ�Ӵ�
		context.m_pNewDocTemplate = pDocTemplate;							
		context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		
		context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		
		if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
			return ;
		v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);									//�Ӵ����
		v_pFrame->InitialUpdateFrame(m_pdoc,true);								//��ʼ�����´���
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���־����_ϵͳ��־ ��2.1��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogSystem()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;														//�Ӵ�����
	Frame_Window	v_mFrame_Window;											//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;											//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;									//������
	v_sFrame = "ϵͳ��־��ѯ";													//�Ӵ����ơ�ϵͳ��־��ѯ��
	v_pIterator_Frame = v_mFrame.find(v_sFrame);								//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())										//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڡ��Ӵ���󻯡�
		return;															
	}
	else
	{
		CChildFrame* v_pFrame= new CChildFrame();								//�����ڡ����Ӵ��塿
		v_mFrame_Window.v_pChildFrame=v_pFrame;									//ָ��
		v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
		CCreateContext context ;
		context.m_pCurrentDoc=m_pdoc;											//����ÿ�ζ�����һ�����ĵ�
		context.m_pNewViewClass = RUNTIME_CLASS(CLog_System);					//�����KPIMSC���ݲ�ѯ�Ӵ�
		context.m_pNewDocTemplate = pDocTemplate;							
		context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		
		context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		
		if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
			return ;
		v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);									//�Ӵ����
		v_pFrame->InitialUpdateFrame(m_pdoc,true);								//��ʼ�����´���
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���־����_ϵͳ�쳣 ��2.2��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogBug()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;														//�Ӵ�����
	Frame_Window	v_mFrame_Window;											//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;											//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;									//������
	v_sFrame = "ϵͳ�쳣��ѯ";													//�Ӵ�����
	v_pIterator_Frame = v_mFrame.find(v_sFrame);								//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())										//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڡ��Ӵ���󻯡�
		return;															
	}
	else
	{
		CChildFrame* v_pFrame= new CChildFrame();								//�����ڡ����Ӵ��塿
		v_mFrame_Window.v_pChildFrame=v_pFrame;									//ָ��
		v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
		CCreateContext context ;
		context.m_pCurrentDoc=m_pdoc;											//����ÿ�ζ�����һ�����ĵ�
		context.m_pNewViewClass = RUNTIME_CLASS(CLog_Bug);						//�����KPIMSC���ݲ�ѯ�Ӵ�
		context.m_pNewDocTemplate = pDocTemplate;							
		context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		
		context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		
		if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
			return ;
		v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);									//�Ӵ����
		v_pFrame->InitialUpdateFrame(m_pdoc,true);								//��ʼ�����´���
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ݿ�������2.3��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogDatabase()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;														//�Ӵ�����
	Frame_Window	v_mFrame_Window;											//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;											//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;									//������
	v_sFrame = "���ݿ�����";													//�Ӵ�����
	v_pIterator_Frame = v_mFrame.find(v_sFrame);								//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())										//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڡ��Ӵ���󻯡�
		return;															
	}
	else
	{
		CChildFrame* v_pFrame= new CChildFrame();								//�����ڡ����Ӵ��塿
		v_mFrame_Window.v_pChildFrame=v_pFrame;									//ָ��
		v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
		CCreateContext context ;
		context.m_pCurrentDoc=m_pdoc;											//����ÿ�ζ�����һ�����ĵ�
		context.m_pNewViewClass = RUNTIME_CLASS(CLog_DataBase);					//�����KPIMSC���ݲ�ѯ�Ӵ�
		context.m_pNewDocTemplate = pDocTemplate;							
		context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		
		context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		
		if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
			return ;
		v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);									//�Ӵ����
		v_pFrame->InitialUpdateFrame(m_pdoc,true);								//��ʼ�����´���
	}
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ��û�����_�û����� ��3.1��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserSetup()
{
	// TODO: �ڴ���������������
	CUser_Setup	v_cDlg;													//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��û�����_�޸����� ��3.2��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserPassword()
{
	// TODO: �ڴ���������������
	CUser_Password	v_cDlg;												//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��û�����_�û���¼ ��3.3��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserLogin()
{
	// TODO: �ڴ���������������
	CUser_Login	v_cDlg;													//ʵ����
	if (v_cDlg.DoModal() == IDOK)										//�򿪶Ի���
	{
		//��ʾ���û���¼��Ϣ
		CMainFrame* pMain = (CMainFrame*)(AfxGetApp()->m_pMainWnd);		//MainFrameָ��
		CString v_sUser="�û���";
		v_sUser+=theApp.a_sUser_Name;									//�û�����
		pMain->m_wndStatusBar.SetPaneText(2,v_sUser);					//Status��ʾ
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��û�����_Ȩ������ ��3.4��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserRole()
{
	// TODO: �ڴ���������������
	CUser_Role	v_cDlg;													//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�RSA���� �����ݿ����롿
//------------------------------------------------------------------------------------------------------
bool CStaionApp::DecrypteParam()
{
	CRSA Rsa;															//ʵ����
	Rsa.SetDecrypteParam(PASSWORDRSAD,PASSWORDRSAN);					//�������ݿ�������ܲ���
	a_sDatabase_User = Rsa.DecryptStr(a_sDatabase_User);				//���ܣ��û�����
	a_sDatabase_Password = Rsa.DecryptStr(a_sDatabase_Password);		//���ܣ��û�����
	return TRUE;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��˳�����COM�ӿڡ�
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
	// TODO: �ڴ���������������
		CString		v_sFrame;														//�Ӵ�����
	Frame_Window	v_mFrame_Window;											//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;											//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;									//������
	v_sFrame = "����ָ��";													//�Ӵ�����
	v_pIterator_Frame = v_mFrame.find(v_sFrame);								//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())										//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڡ��Ӵ���󻯡�
		return;															
	}
	else
	{
		CChildFrame* v_pFrame= new CChildFrame();								//�����ڡ����Ӵ��塿
		v_mFrame_Window.v_pChildFrame=v_pFrame;									//ָ��
		v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
		CCreateContext context ;
		context.m_pCurrentDoc=m_pdoc;											//����ÿ�ζ�����һ�����ĵ�
		context.m_pNewViewClass = RUNTIME_CLASS(CConAnalyzeIndicator);					//�����KPIMSC���ݲ�ѯ�Ӵ�
		context.m_pNewDocTemplate = pDocTemplate;							
		context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		
		context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		
		if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
			return ;
		v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);									//�Ӵ����
		v_pFrame->InitialUpdateFrame(m_pdoc,true);								//��ʼ�����´���
	}
}
