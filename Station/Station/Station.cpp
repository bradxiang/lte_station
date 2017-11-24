
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
#include "MyChartViewer.h"

#include "ImpIDispatch.h"

//ϵͳ�˵����ܺ���
//1�У�LTE ���ܹ���
#include "LTE_PM_Data.h"
#include "LTE_PM_Draw.h"
#include "LTE_PM_Data_Cell.h"
#include "LTE_PM_Data_Real.h"
#include "LTE_PM_Data_Cal.h"
#include "LTE_PM_Data_Init.h"
#include "LTE_PM_Draw_Znit.h"

//2�У�GSM ���ܹ���

//3�У�����Ԥ��
#include "Alarm_PM.h"
#include "Alarm_LL_Real.h"
#include "Alarm_LL_Cell.h"

//4�У���ַ����
#include "All_PM_Draw.h"

//4�У���վ��ѯ
#include "QUA_Station.h"
#include "QUA_Station_Map.h"
#include "QUA_Element_TD.h"
#include "QUA_Element_GSM.h"

//5�У����ù���
#include "CM_LTE_File.h"
#include "CM_LTE_Para.h"
#include "CM_LTE_Quary.h"
#include "CM_LTE_Relation.h"

//5�У���վ����
#include "CON_Station.h"
#include "CON_Region.h"
#include "CON_BTS.h"
#include "CON_E_NodeB.h"
#include "CON_GSM_Cell.h"
#include "CON_LTE_Cell.h"
#include "CON_Tools_XML.h"

//6�У����ܹ���
#include "Pm_LTE.h"														//LTE_���ܹ���_ԭʼ
#include "PM_LTE_Cal.h"													//LTE_���ܹ���_����
#include "PM_LTE_Region.h"												//LTE_���ܹ���_����
#include "PM_LTE_Case_Cell.h"											//LTE_���ܲ�ѯģ��_С��
#include "PM_LTE_Case_PM.h"												//LTE_���ܲ�ѯģ��_����
#include "PM_LTE_Case_Cal.h"											//LTE_���ܲ�ѯģ��_����
#include "PM_LTE_Alarm.h"												//LTE_����Ԥ��_����
#include "Pm_GSM.h"														//GSM_���ܹ���
#include "Pm_GSM_Cell_HO.h"												//
#include "Pm_GSM_BSC.h"

//8�У�ϵͳ����
#include "CON_Para.h"													//��������

//9�У���־����
#include "Log_System.h"													//��־��ѯ
#include "Log_Bug.h"													//�쳣��ѯ
#include "Log_DataBase.h"												//���ݿ�������ѯ
#include "Log_DataBase_GSM.h"

//10�У��û�����
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
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CStaionApp::CStaionApp()
{
	m_bHiColorIcons = TRUE;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	COleDateTime vd(2000,1,1,0,0,0);									//Ĭ�ϣ���Ч����

	a_tData_Default=vd;
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

	AfxEnableControlContainer();

	CoInitialize(NULL);													//

	m_pCustDisp = new CImpIDispatch;									//

	//��ʼ���׽���
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ��ʼ�� OLE ��
//	if (!AfxOleInit())
//	{
//		AfxMessageBox(IDP_OLE_INIT_FAILED);
//		return FALSE;
//	}

	////////////////////////////////////////////////////////////////////////////////////////////////////			
	//����һ����Ӧ�ó����������ź��������ڣ�ֻ������һ�θ���� [�˳����ʱ����Ҫ�Ƴ����ź���]
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

	////////////////////////////////////////////////////////////////////////////////////////////////////			
	//��ȡ����Ĭ��Ŀ¼
	const int nBufSize = 512;											//�ߴ磺������
	TCHAR chBuf[nBufSize];												//���壺������
	ZeroMemory(chBuf,nBufSize);											//��λ��������
 	::GetModuleFileNameA(NULL,chBuf,nBufSize);							//��ȡĬ���ļ�·��
	TCHAR* lpStrPath = chBuf;											//ָ��
	PathRemoveFileSpec(lpStrPath);										//ȥ�����ļ�����
	m_sPath=chBuf;														//��ȡ��Ĭ��·��

	////////////////////////////////////////////////////////////////////////////////////////////////////			
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

	GetPrivateProfileString("DATABASE_SERVER_GSM","DATABASE_SERVER_GSM",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_GSM_Ip.Format("%s",v_cBuffer);															//���ݿ��������ַ
	GetPrivateProfileString("DATABASE_NAME_GSM","DATABASE_NAME_GSM",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_GSM_Name.Format("%s",v_cBuffer);														//������ݿ�����
	GetPrivateProfileString("DATABASE_USER_GSM","DATABASE_USER_GSM",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_GSM_User.Format("%s",v_cBuffer);														//������ݿ��û�
	GetPrivateProfileString("DATABASE_PASSWORD_GSM","DATABASE_PASSWORD_GSM",NULL,v_cBuffer,512,".\\config.ini");
	a_sDatabase_GSM_Password.Format("%s",v_cBuffer);													//������ݿ�����

	GetPrivateProfileString("HTTP_MAP","HTTP_MAP",NULL,v_cBuffer,512,".\\config.ini");
	m_sHTTP_Map.Format("%s",v_cBuffer);																	//���Web·��

	if(a_sDatabase_Ip.IsEmpty())
	{
		AfxMessageBox("ϵͳ�����ļ�( config.ini )�����ڣ�����");										//�쳣�������ļ�û�ҵ�   
		return   false;																					//ϵͳ�˳�
	}

	//���ܣ����ݿ�����
	//DecrypteParam();													//�������

	//�û���½��Ϣ����ʼ��
	a_sUser_Account = "";												//�˺�
	a_sUser_Name = "";													//����
	a_sUser_Password ="";												//����

	Chart::setLicenseCode("RDST-25ZA-3JX6-GCTB-A07B-8F0E");				//Chart��License

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
		theApp.m_MyLog.WriteText("ϵͳ��ʼ********************\r\n");	//Log
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
//	�������ܣ�LTE ���ܹ���_LTE ����ͼ�β�ѯ	��100��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmDraw()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE ����ͼ�β�ѯ";										//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Draw);				//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�LTE ���ܹ���_LTE �������ݲ�ѯ	��101��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmData()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE �������ݲ�ѯ";										//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Data);				//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�LTE ���ܹ���_LTE �������ݲ�ѯ_С��	��102��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmDataCell()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE �������ݲ�ѯ_С��";									//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Data_Cell);			//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�LTE ���ܹ���_LTE �������ݲ�ѯ_ʵʱ	��103��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmDataReal()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE �������ݲ�ѯ_ʵʱ";									//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Data_Real);			//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�LTE ���ܹ���_LTE �������ݼ���_С��	��104��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmDataCal()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE �������ݼ���_С��";									//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Data_Cal);			//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�LTE ���ܹ���_LTE ����ͼ�β�ѯ_ԭʼ	��108��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmDrawInit()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE ����ͼ�β�ѯ_ԭʼ";									//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Draw_Znit);			//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�LTE ���ܹ���_LTE �������ݲ�ѯ_ԭʼ	��109��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLtePmDataInit()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE �������ݲ�ѯ_ԭʼ";									//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CLTE_PM_Data_Init);			//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�GSM ���ܹ���_GSM ����ͼ�β�ѯ	��110��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnGsmPmDraw()
{
	// TODO: �ڴ���������������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�GSM ���ܹ���_GSM �������ݲ�ѯ	��111��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnGsmPmData()
{
	// TODO: �ڴ���������������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Ԥ��_����Ԥ����ѯ	��130��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnAlarmPm()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "����Ԥ����ѯ";											//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CAlarm_PM);					//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Ԥ��_LTE ������С����ѯ	��131��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnAlarmLlReal()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE ������С��ʵʱ";									//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CAlarm_LL_Real);			//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����Ԥ��_LTE ������С����ѯ	��132��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnAlarmLlCell()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE ������С����ѯ";									//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CAlarm_LL_Cell);			//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܹ���_��ַ���ܲ�ѯ	��3.1��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnAllPmDraw()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "��ַ���ܲ�ѯ";											//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CAll_PM_Draw);				//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ԫ��ѯ_վַ��ѯ	��4.1��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnQuaryStation()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "վַ��ѯ";												//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CQUA_Station);				//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ԫ��ѯ_վַ��ͼ��ѯ	��4.2��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnQuaryStationMap()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "վַ��ͼ��ѯ";											//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CQUA_Station_Map);			//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ԫ��ѯ_TD-LTE��Ԫ��ѯ	��4.3��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnQuaryTdlte()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "TD-LTE��Ԫ��ѯ";										//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CQUA_Element_TD);			//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ԫ��ѯ_GSM��Ԫ��ѯ	��4.4��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnQuaryGsm()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "GSM��Ԫ��ѯ";											//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CQUA_Element_GSM);			//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ԫ��ѯ_GSM������Ԫ��ѯ	��4.5��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnQuaryGsmGt()
{
	// TODO: �ڴ���������������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ù���_LTE ���ò�ѯ
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnCmLteQuary()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE ���ò�ѯ";											//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CCM_LTE_Quary);				//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ù���_LTE-Tdd���ڹ�ϵ
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnCmLteRelation()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE-Tdd���ڹ�ϵ";										//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CCM_LTE_Relation);			//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ù���_LTE ���ù���_�ļ�
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnCmLteFile()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE ���ù���_�ļ�";										//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CCM_LTE_File);				//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ù���_LTE ���ù���_����
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnCmLtePara()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE ���ù���_����";										//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CCM_LTE_Para);				//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������_��������	��5.1��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigRegion()
{
	// TODO: �ڴ���������������
	CCON_Region	v_cDlg;													//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������_վַ����	��5.2��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigStation()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "վַ����";												//�Ӵ����ƣ�վַ����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CCON_Station);			//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������_BTS ����	��5.3��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigBts()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "BTS ����";												//�Ӵ����ƣ�BTS ����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CCON_BTS);					//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������_E_NodeB ����		��5.5��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigENodeb()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "E_NodeB ����";											//�Ӵ����ƣ�E_NodeB ����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CCON_E_NodeB);				//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������_GSM Cell ����	��5.6��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigGsmCell()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "GSM Cell ����";											//�Ӵ����ƣ�GSM Cell ����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CCON_GSM_Cell);				//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������_LTE Cell ����	��5.8��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigLteCell()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE Cell ����";											//�Ӵ����ƣ�LTE Cell ����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CCON_LTE_Cell);				//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������_XML�ļ�����  ��5.9��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigTools()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "XML�ļ�����";											//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CCON_Tools_XML);			//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܹ���_LTE_���ܹ��� ��220��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLte()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE_���ܹ���_ԭʼ";										//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CPm_LTE);					//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܹ���_LTE_���ܹ���_���� ��221��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLteCal()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE_���ܹ���_����";										//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CPM_LTE_Cal);				//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܹ���_LTE_���ܹ���_���� ��222��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLteRegion()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE_���ܹ���_����";										//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CPM_LTE_Region);			//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܹ���_LTE_���ܲ�ѯģ��_С�� ��223��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLteCas()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE_���ܲ�ѯģ��_С��";									//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CPM_LTE_Case_Cell);			//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܹ���_LTE_���ܲ�ѯģ��_���� ��224��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLteCasPm()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE_���ܲ�ѯģ��_����";									//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CPM_LTE_Case_PM);			//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܹ���_LTE_���ܲ�ѯģ��_���� ��226��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLteCasCal()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "LTE_���ܲ�ѯģ��_����";									//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CPM_LTE_Case_Cal);			//�����
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܹ���_LTE_����Ԥ��_���� ��225��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmLteAlarm()
{
	// TODO: �ڴ���������������
	CPM_LTE_Alarm	v_cDlg;												//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܹ���_GSM_���ܹ��� ��6.4��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmGsm()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "GSM_���ܹ���";											//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CPm_GSM);					//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܹ���_GSM_���ܼ���_CELL_HO ��6.5��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmGsmCalCell()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "GSM_���ܼ���_CELL_HO";									//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CPm_GSM_Cell_HO);			//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܹ���_GSM_���ܼ���_BSC ��6.6��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnPmGsmCalBsc()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "GSM_���ܼ���_BSC";										//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CPm_GSM_BSC);				//�����XML�ļ������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ϵͳ����_��������  ��8.1��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnConfigPara()
{
	// TODO: �ڴ���������������
	CCON_Para	v_cDlg;													//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���־����_ϵͳ��־ ��9.1��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogSystem()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "ϵͳ��־��ѯ";											//�Ӵ����ƣ�ϵͳ��־��ѯ

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CLog_System);				//�����KPIMSC���ݲ�ѯ�Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���־����_ϵͳ�쳣 ��9.2��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogBug()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "ϵͳ�쳣��ѯ";											//�Ӵ����ƣ�ϵͳ�쳣��ѯ

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CLog_Bug);					//�����KPIMSC���ݲ�ѯ�Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ݿ�����_LTE ��9.3��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogDatabase()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "���ݿ�����_LTE";										//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CLog_DataBase);				//����󣺷ַ����������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ݿ�����_GSM ��9.4��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnLogDatabaseGsm()
{
	// TODO: �ڴ���������������
	CString		v_sFrame;												//��ʱ����	
	Frame_Window	v_mFrame_Window;									//��ʱ����	
	v_mFrame_Window.v_iThread_Exit=0;									//��ʼ״̬
	mFrame_Window::iterator	v_pIterator_Frame;							//������

	v_sFrame = "���ݿ�����_GSM";										//�Ӵ�����

	v_pIterator_Frame = v_mFrame.find(v_sFrame);						//�����Ӵ�����
	if(v_pIterator_Frame != v_mFrame.end())								//�Ӵ����ڣ�
	{
		v_pIterator_Frame->second.v_pChildFrame->ShowWindow(SW_SHOWMAXIMIZED);	//���ڣ��Ӵ����
		return;															//����
	}
	CChildFrame* v_pFrame= new CChildFrame();							//ʵ�������Ӵ���
	v_mFrame_Window.v_pChildFrame=v_pFrame;								//ָ��
	v_mFrame.insert(pair<CString,Frame_Window>(v_sFrame,v_mFrame_Window));	//�������洰��ָ��							
	CCreateContext context ;
	context.m_pCurrentDoc=m_pdoc;										//����ÿ�ζ�����һ�����ĵ�
	context.m_pNewViewClass = RUNTIME_CLASS(CLog_DataBase_GSM);			//����󣺷ַ����������Ӵ�
	context.m_pNewDocTemplate = pDocTemplate;							//
	context.m_pLastView=(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ? ((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);		//
	context.m_pCurrentFrame = ((CMainFrame*)m_pMainWnd)->GetActiveFrame();		//
	if(!v_pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE ,m_pMainWnd, &context ))		//�����Ӵ�
		return ;
	v_pFrame->ShowWindow(SW_SHOWMAXIMIZED);								//�Ӵ����
	v_pFrame->InitialUpdateFrame(m_pdoc,true);							//��ʼ�����´���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��û�����_�û����� ��10.1��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserSetup()
{
	// TODO: �ڴ���������������
	CUser_Setup	v_cDlg;													//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��û�����_�޸����� ��10.2��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserPassword()
{
	// TODO: �ڴ���������������
	CUser_Password	v_cDlg;												//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��û�����_�û���¼ ��10.3��
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
//	�������ܣ��û�����_Ȩ������ ��10.4��
//------------------------------------------------------------------------------------------------------
void CStaionApp::OnUserRole()
{
	// TODO: �ڴ���������������
	CUser_Role	v_cDlg;													//ʵ����
	v_cDlg.DoModal();													//�򿪶Ի���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�RSA���� [���ݿ�����]
//------------------------------------------------------------------------------------------------------
bool CStaionApp::DecrypteParam()
{
	CRSA Rsa;															//ʵ����
	Rsa.SetDecrypteParam(PASSWORDRSAD,PASSWORDRSAN);					//�������ݿ�������ܲ���
	a_sDatabase_User = Rsa.DecryptStr(a_sDatabase_User);				//���ܣ��û�����
	a_sDatabase_Password = Rsa.DecryptStr(a_sDatabase_Password);		//���ܣ��û�����

 	a_sDatabase_GSM_User = Rsa.DecryptStr(a_sDatabase_GSM_User);		//���ܣ��û�����
	a_sDatabase_GSM_Password = Rsa.DecryptStr(a_sDatabase_GSM_Password);//���ܣ��û�����

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
