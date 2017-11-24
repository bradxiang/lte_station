
// MainFrm.cpp : CMainFrame ���ʵ��
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	IDS_ALARM,				//ͨ���쳣��Ϣ
	IDS_USER,				//����Ա
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

//------------------------------------------------------------------------------------------------------			
//	���캯����CMainFrame
//------------------------------------------------------------------------------------------------------
CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);
}

//------------------------------------------------------------------------------------------------------			
//	����������CMainFrame
//------------------------------------------------------------------------------------------------------
CMainFrame::~CMainFrame()
{
}

//------------------------------------------------------------------------------------------------------			
//	���������ɣ��˵���Toolbar��Statusbar��
//------------------------------------------------------------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//�����ź���[���ֻ������һ��]
	::SetProp(m_hWnd,AfxGetApp()->m_pszAppName,(HANDLE)1);				//�����ź���

	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE;				// ����������ʽ...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;						// ����Ϊ FALSE �Ὣ�رհ�ť������ѡ�������Ҳ�
	mdiTabParams.m_bTabIcons = FALSE;									// ����Ϊ TRUE ���� MDI ѡ��������ĵ�ͼ��
	mdiTabParams.m_bAutoColor = TRUE;									// ����Ϊ FALSE ������ MDI ѡ����Զ���ɫ
	mdiTabParams.m_bDocumentMenu = TRUE;								// ��ѡ�������ұ�Ե�����ĵ��˵�
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	//Toolbar����
	if ((!m_wndToolBar.CreateEx(this,TBSTYLE_FLAT,WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC,
		CRect(0,0,0,0),IDR_MAINFRAME))
		||(!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1,0,0,TRUE,0,0,IDB_TOOLBAR)))	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	//StatusBar����
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
//	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
//	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	//Status����
	m_wndStatusBar.SetPaneInfo(1,IDS_ALARM,SBPS_NORMAL,800);			//״̬�����ã�ͨ���쳣��Ϣ
	m_wndStatusBar.SetPaneInfo(2,IDS_USER,SBPS_NORMAL,100);				//״̬�����ã���¼�û�

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	VS2008�Դ�����
//------------------------------------------------------------------------------------------------------
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

// CMainFrame ���

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


// CMainFrame ��Ϣ�������

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
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
//	�ı䴰��ķ��
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
//	Ӧ�ó���˵���Ϣִ��
//------------------------------------------------------------------------------------------------------
void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

//------------------------------------------------------------------------------------------------------			
//	�ػ���Ϣת���������Ӵ���ı仯�������Ӵ����ػ����Ϣ
//------------------------------------------------------------------------------------------------------
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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
//	�ػ���Ļ�������յ��Ĵ���仯��Ϣ [����ͼƬ����]
//------------------------------------------------------------------------------------------------------
void CMainFrame::OnPaint()
{
//	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CMDIFrameWndEx::OnPaint()
	CDC dc, memdc;
	dc.m_hDC=::GetDC(this->m_hWndMDIClient);
	CRect rect;
	int iWidth,iHeight;
	CBitmap bitmap;
	BITMAP szbitmap;
	bitmap.LoadBitmap(IDB_BACKGROUND);									//��ȡ������ͼƬ��
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
//	�رճ����ܣ������յ���Close��Ϣ
//------------------------------------------------------------------------------------------------------
void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString			v_sFrame_Name;											//��������
	bool			v_bExit=false;											//�˳���־����Ч
	mFrame_Window::iterator	v_pIterator_Frame;								//������

	if(MessageBox("�Ƿ��˳� �ƶ������ں������Ż�����ϵͳ ?"," ϵͳ�˳�ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)		//�˳��ж�
	{
		mFrame_Window::iterator	v_pIterator_Frame;						//������

		for(v_pIterator_Frame = theApp.v_mFrame.begin();v_pIterator_Frame != theApp.v_mFrame.end();v_pIterator_Frame++)	//����
		{
			if(v_pIterator_Frame->second.v_iThread_Exit==1)			//���߳������У�����
			{
				MessageBox("ģ���ڶ�ȡ���ݣ����ڽ��������˳�ϵͳ ������");	//���߳�������
				return;													//���أ����˳�
			}
		}

		//�Ƴ��ź���[���ֻ������һ��]
		::RemoveProp(m_hWnd,AfxGetApp()-> m_pszAppName);				//�Ƴ��ź���

		if (!theApp.a_sUser_Name.IsEmpty())
		{
			//Log�������˳�
			theApp.m_MyLog.WriteText("�����˳�--------------------\r\n");	//log

			//�˳���Ϣ��¼����־����
			COleDateTime v_tTime=COleDateTime::GetCurrentTime();
			CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");
			CString v_sSql="Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"+v_sCurrentTime+"','�û��˳�','"
					+theApp.a_sUser_Name+" �˳���IP��"+theApp.a_sUser_IP+"')";
			My_ExecuteSQL(v_sSql);										//���ݿ�ִ��
		}

		CMDIFrameWndEx::OnClose();										//�رմ���
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���ȡ����Ȩ��
//	��    ����v_sPower_Name��Ȩ������		[���ݣ�����Ȩ������a_mUser_Power]
//------------------------------------------------------------------------------------------------------
bool CMainFrame::My_Get_Power(CString v_sPower_Name)
{
	bool	v_bRet=false;												//����ֵ

	mUser_Power::iterator v_pIterator_Power=theApp.a_mUser_Power.find(v_sPower_Name);	//����Ȩ��[����v_sPower_Name]
	if(v_pIterator_Power != theApp.a_mUser_Power.end())									//�ҵ���
		v_bRet=v_pIterator_Power->second;								//��ã�Ȩ��[true/false] 

//	v_bRet=true;
	return v_bRet;														//���أ�Ȩ��
}

//------------------------------------------------------------------------------------------------------			
//	�Ӳ˵�״̬���ã���Ч����Ч�������û��Ĺ���Ȩ��
//------------------------------------------------------------------------------------------------------

//======================================================================================================
//	�Ӳ˵���LTE ���ܲ�ѯ
void CMainFrame::OnUpdateLtePmDraw(CCmdUI *pCmdUI)						//100 LTE ����ͼ�β�ѯ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE ����ͼ�β�ѯ"));
}

void CMainFrame::OnUpdateLtePmData(CCmdUI *pCmdUI)						//101 LTE �������ݲ�ѯ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE �������ݲ�ѯ"));
}

void CMainFrame::OnUpdateLtePmDataCell(CCmdUI *pCmdUI)					//102 LTE �������ݲ�ѯ_С��
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE �������ݲ�ѯ_С��"));
}

void CMainFrame::OnUpdateLtePmDataReal(CCmdUI *pCmdUI)					//103 LTE �������ݲ�ѯ_ʵʱ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE �������ݲ�ѯ_ʵʱ"));
}

void CMainFrame::OnUpdateLtePmDataCal(CCmdUI *pCmdUI)					//104 LTE �������ݼ���_С��
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE �������ݼ���_С��"));
}

void CMainFrame::OnUpdateLtePmDrawInit(CCmdUI *pCmdUI)					//108 LTE ����ͼ�β�ѯ_ԭʼ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE ����ͼ�β�ѯ_ԭʼ"));
}

void CMainFrame::OnUpdateLtePmDataInit(CCmdUI *pCmdUI)					//109 LTE �������ݲ�ѯ_ԭʼ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE �������ݲ�ѯ_ԭʼ"));
}

//======================================================================================================
//	�Ӳ˵���GSM ���ܲ�ѯ
void CMainFrame::OnUpdateGsmPmDraw(CCmdUI *pCmdUI)						//110 GSM ����ͼ�β�ѯ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("GSM ����ͼ�β�ѯ"));
}

void CMainFrame::OnUpdateGsmPmData(CCmdUI *pCmdUI)						//111 GSM �������ݲ�ѯ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("GSM �������ݲ�ѯ"));
}

//======================================================================================================
//	�Ӳ˵�������Ԥ��
void CMainFrame::OnUpdateAlarmPm(CCmdUI *pCmdUI)						//130 ����Ԥ����ѯ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("����Ԥ����ѯ"));
}

void CMainFrame::OnUpdateAlarmLlReal(CCmdUI *pCmdUI)					//131 LTE ������С��ʵʱ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE ������С��ʵʱ"));
}

void CMainFrame::OnUpdateAlarmLlCell(CCmdUI *pCmdUI)					//132 LTE ������С����ѯ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE ������С����ѯ"));
}

//======================================================================================================
//	�Ӳ˵�����ַ���ܲ�ѯ
void CMainFrame::OnUpdateAllPmDraw(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������						//170 ��ַ���ܲ�ѯ
	pCmdUI->Enable(My_Get_Power("��ַ���ܲ�ѯ"));
}

//======================================================================================================
//	�Ӳ˵�����Ԫ��ѯ
void CMainFrame::OnUpdateQuaryStation(CCmdUI *pCmdUI)					//180 վַ��ѯ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("վַ��ѯ"));
}

void CMainFrame::OnUpdateQuaryStationMap(CCmdUI *pCmdUI)				//181 վַ��ͼ��ѯ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("վַ��ͼ��ѯ"));
}

void CMainFrame::OnUpdateQuaryTdlte(CCmdUI *pCmdUI)						//182 TD-LTE��Ԫ��ѯ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("TD-LTE��Ԫ��ѯ"));
}

void CMainFrame::OnUpdateQuaryGsm(CCmdUI *pCmdUI)						//183 GSM��Ԫ��ѯ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("GSM��Ԫ��ѯ"));
}

void CMainFrame::OnUpdateQuaryGsmGt(CCmdUI *pCmdUI)						//184 GSM������Ԫ��ѯ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("GSM������Ԫ��ѯ"));
}

//======================================================================================================
//	�Ӳ˵������ù���
void CMainFrame::OnUpdateCmLteQuary(CCmdUI *pCmdUI)						//190 LTE ���ò�ѯ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE ���ò�ѯ"));
}


void CMainFrame::OnUpdateCmLteRelation(CCmdUI *pCmdUI)					//191 LTE-Tdd���ڹ�ϵ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE-Tdd���ڹ�ϵ"));
}

void CMainFrame::OnUpdateCmLteFile(CCmdUI *pCmdUI)						//192 LTE ���ù���_�ļ�
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE ���ù���_�ļ�"));
}

void CMainFrame::OnUpdateCmLtePara(CCmdUI *pCmdUI)						//193 LTE ���ù���_����
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE ���ù���_����"));
}

//======================================================================================================
//	�Ӳ˵�����������
void CMainFrame::OnUpdateConfigRegion(CCmdUI *pCmdUI)					//210 ��������
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("��������"));
}

void CMainFrame::OnUpdateConfigStation(CCmdUI *pCmdUI)					//211 վַ����
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("վַ����"));
}

void CMainFrame::OnUpdateConfigBts(CCmdUI *pCmdUI)						//212 BTS ����
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("BTS ����"));
}

void CMainFrame::OnUpdateConfigENodeb(CCmdUI *pCmdUI)					//214 E_NodeB ����
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("E_NodeB ����"));
}

void CMainFrame::OnUpdateConfigGsmCell(CCmdUI *pCmdUI)					//215 GSM Cell����
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("GSM Cell����"));
}

void CMainFrame::OnUpdateConfigLteCell(CCmdUI *pCmdUI)					//217 LTE Cell����
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE Cell����"));
}

void CMainFrame::OnUpdateConfigTools(CCmdUI *pCmdUI)					//218 XML�ļ�����
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("XML�ļ�����"));
}

//======================================================================================================
//	�Ӳ˵������ܹ���
void CMainFrame::OnUpdatePmLte(CCmdUI *pCmdUI)							//220 LTE_���ܹ���_ԭʼ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE_���ܹ���_ԭʼ"));
}

void CMainFrame::OnUpdatePmLteCal(CCmdUI *pCmdUI)						//221 LTE_���ܹ���_����
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE_���ܹ���_����"));
}

void CMainFrame::OnUpdatePmLteRegion(CCmdUI *pCmdUI)					//222 LTE_���ܹ���_����
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE_���ܹ���_����"));
}

void CMainFrame::OnUpdatePmLteCas(CCmdUI *pCmdUI)						//223 LTE_���ܲ�ѯģ��_С��
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE_���ܲ�ѯģ��_С��"));
}

void CMainFrame::OnUpdatePmLteCasPm(CCmdUI *pCmdUI)						//224 LTE_���ܲ�ѯģ��_����
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE_���ܲ�ѯģ��_����"));
}

void CMainFrame::OnUpdatePmLteCasCal(CCmdUI *pCmdUI)					//226 LTE_���ܲ�ѯģ��_����
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE_���ܲ�ѯģ��_����"));
}

void CMainFrame::OnUpdatePmLteAlarm(CCmdUI *pCmdUI)						//225 LTE_����Ԥ��_���� 
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTE_����Ԥ��_����"));
}

void CMainFrame::OnUpdatePmGsm(CCmdUI *pCmdUI)							//224 GSM_���ܹ���
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("GSM_���ܹ���"));
}

void CMainFrame::OnUpdatePmGsmCalCell(CCmdUI *pCmdUI)					//225 GSM_���ܼ���_CELL_HO
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("GSM_���ܼ���_CELL_HO"));
}

void CMainFrame::OnUpdatePmGsmCalBsc(CCmdUI *pCmdUI)					//226 GSM_���ܼ���_BSC
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("GSM_���ܼ���_BSC"));
}

//======================================================================================================
//	�Ӳ˵���ϵͳ����
void CMainFrame::OnUpdateConfigPara(CCmdUI *pCmdUI)						//230 ��������
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("��������"));
}

//======================================================================================================
//	�Ӳ˵�״̬���ã���־����
void CMainFrame::OnUpdateLogSystem(CCmdUI *pCmdUI)						//310 ϵͳ��־��ѯ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("ϵͳ��־"));
}

void CMainFrame::OnUpdateLogBug(CCmdUI *pCmdUI)							//311 �쳣��־����
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("ϵͳ�쳣"));
}

void CMainFrame::OnUpdateLogDatabase(CCmdUI *pCmdUI)					//312 ���ݿ�����_LTE
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("���ݿ�����"));
}

void CMainFrame::OnUpdateLogDatabaseGsm(CCmdUI *pCmdUI)					//313 ���ݿ�����_GSM
{
	// TODO: �ڴ������������û����洦��������
//	pCmdUI->Enable(My_Get_Power("���ݿ�����_GSM"));
	pCmdUI->Enable(false);
}

//======================================================================================================
//	�Ӳ˵�״̬���ã��û�����
void CMainFrame::OnUpdateUserSetup(CCmdUI *pCmdUI)						//320 �û�����
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("�û�����"));
}

void CMainFrame::OnUpdateUserPassword(CCmdUI *pCmdUI)					//321 �����޸�
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("�����޸�"));
}

void CMainFrame::OnUpdateUserRole(CCmdUI *pCmdUI)						//322 Ȩ������
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("Ȩ������"));
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CMainFrame::My_ExecuteSQL(CString v_sSql)
{
	// TODO: Add your control notification handler code here
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);

		v_cAdoConn.ExecuteSQL((_bstr_t)v_sSql);							//ִ�����ݿ����

		v_cAdoConn.ExitConnect();										//�Ͽ�����
		return true;
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�ر����ݿ�

		CString v_sError = (char *)e.Description();						//��ã�������Ϣ
		MessageBox(v_sError);											//ʧ��
		return false;
	}
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
