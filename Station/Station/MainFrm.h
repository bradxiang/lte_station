
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//------------------------------------------------------------------------------------------------------
//	��������	
//------------------------------------------------------------------------------------------------------
protected:																//�ؼ���Ƕ���Ա
	CMFCMenuBar       m_wndMenuBar;										//MenuBar 
	CMFCToolBar       m_wndToolBar;										//ToolBar

//------------------------------------------------------------------------------------------------------
//	��Ϣӳ�亯������	
//------------------------------------------------------------------------------------------------------
protected:
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnWindowManager();
	afx_msg void	OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void	OnApplicationLook(UINT id);
	afx_msg void	OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

//------------------------------------------------------------------------------------------------------
//	��������
//------------------------------------------------------------------------------------------------------
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//�ػ���Ϣת��
	afx_msg void OnPaint();												//�ػ���Ļ
	afx_msg void OnClose();												//�رճ�����
	afx_msg void OnUpdateLogSystem(CCmdUI *pCmdUI);						//
	afx_msg void OnUpdateLogBug(CCmdUI *pCmdUI);						//
	afx_msg void OnUpdateLogDatabase(CCmdUI *pCmdUI);					//
	afx_msg void OnUpdateUserSetup(CCmdUI *pCmdUI);						//
	afx_msg void OnUpdateUserPassword(CCmdUI *pCmdUI);					//
	afx_msg void OnUpdateUserRole(CCmdUI *pCmdUI);						//

	//�Զ���
	bool	My_Get_Power(CString v_sPower_Name);						//��ȡ������Ȩ��[��/��]
	bool	My_ExecuteSQL(CString v_sSql);								//ִ��SQL����

	//------------------------------------------------------------------------------------------------------
	//	��������
	//------------------------------------------------------------------------------------------------------
	CMFCStatusBar	m_wndStatusBar;										//StatusBar

	//�Զ���
	afx_msg void OnUpdateConfigStation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConfigRegion(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConfigBts(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConfigGsmCell(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConfigLteCell(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConfigENodeb(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConfigTools(CCmdUI *pCmdUI);
	afx_msg void OnUpdateQuaryStationMap(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePmLte(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConfigPara(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLtePmDraw(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLtePmData(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAllPmDraw(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePmLteCal(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePmLteCas(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePmGsm(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePmGsmCalCell(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePmGsmCalBsc(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLogDatabaseGsm(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGsmPmDraw(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGsmPmData(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLtePmDrawInit(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLtePmDataInit(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAlarmPm(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePmLteRegion(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePmLteCasPm(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLtePmDataReal(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLtePmDataCell(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePmLteAlarm(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAlarmLlCell(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCmLteQuary(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCmLteFile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCmLtePara(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLtePmDataCal(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePmLteCasCal(CCmdUI *pCmdUI);
	afx_msg void OnUpdateQuaryStation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateQuaryTdlte(CCmdUI *pCmdUI);
	afx_msg void OnUpdateQuaryGsm(CCmdUI *pCmdUI);
	afx_msg void OnUpdateQuaryGsmGt(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAlarmLlReal(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCmLteRelation(CCmdUI *pCmdUI);
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

