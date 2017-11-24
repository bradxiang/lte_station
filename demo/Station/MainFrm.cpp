
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
	//------------------------------------------------------------------------------------------------------
	//	��Ϣ�Զ���
	//------------------------------------------------------------------------------------------------------
	ON_UPDATE_COMMAND_UI(ID_Log_System, &CMainFrame::OnUpdateLogSystem)
	ON_UPDATE_COMMAND_UI(ID_Log_Bug, &CMainFrame::OnUpdateLogBug)
	ON_UPDATE_COMMAND_UI(ID_Log_DATABASE, &CMainFrame::OnUpdateLogDatabase)
	//-----------------------------------------------------------------------------------
	ON_UPDATE_COMMAND_UI(ID_User_PassWord, &CMainFrame::OnUpdateUserPassword)
	ON_UPDATE_COMMAND_UI(ID_User_Setup, &CMainFrame::OnUpdateUserSetup)	
	ON_UPDATE_COMMAND_UI(ID_User_Role, &CMainFrame::OnUpdateUserRole)
	//-----------------------------------------------------------------------------------
	ON_UPDATE_COMMAND_UI(ID_CON_LTE_INDICATOR, &CMainFrame::OnUpdateConLTEIndicator)	
	ON_UPDATE_COMMAND_UI(ID_CON_STATION_MAP, &CMainFrame::OnUpdateConStationMap)
	ON_UPDATE_COMMAND_UI(ID_CON_ANALYZE_INDICATOR, &CMainFrame::OnUpdateConAnalysisIndicator)
	//-----------------------------------------------------------------------------------
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

	if(MessageBox("�Ƿ��˳� demoϵͳ ?"," ϵͳ�˳�ȷ��",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)		//�˳��ж�
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
		v_bRet=v_pIterator_Power->second;												//��ã�Ȩ��[true/false] 

//	v_bRet=true;
	return v_bRet;																		//���أ�Ȩ��
}

//------------------------------------------------------------------------------------------------------			
//	�Ӳ˵�״̬���ã���Ч����Ч�������û��Ĺ���Ȩ��
//------------------------------------------------------------------------------------------------------
//======================================================================================================
//	�Ӳ˵�״̬���ã���������
void CMainFrame::OnUpdateConStationMap(CCmdUI *pCmdUI)						//���ߵ�ͼ
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("LTEæʱָ��"));
}

void CMainFrame::OnUpdateConLTEIndicator(CCmdUI *pCmdUI)			        //LTEæʱָ��		
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("���ߵ�ͼ"));
}
void CMainFrame::OnUpdateConAnalysisIndicator(CCmdUI *pCmdUI)			    //����ָ��		
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(My_Get_Power("����ָ��"));
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
