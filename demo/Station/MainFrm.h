
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
	
	//------------------------------------------------------------------------------------------------------
	//	��������
	//------------------------------------------------------------------------------------------------------
	CMFCStatusBar	m_wndStatusBar;										//StatusBar

	//�Զ��塾Ȩ�����衿
	afx_msg void OnUpdateLogSystem(CCmdUI *pCmdUI);						//
	afx_msg void OnUpdateLogBug(CCmdUI *pCmdUI);						//
	afx_msg void OnUpdateLogDatabase(CCmdUI *pCmdUI);					//
	//------------------------------------------------------------------------------------------------------
	afx_msg void OnUpdateUserSetup(CCmdUI *pCmdUI);						//
	afx_msg void OnUpdateUserPassword(CCmdUI *pCmdUI);					//
	afx_msg void OnUpdateUserRole(CCmdUI *pCmdUI);						//
	//------------------------------------------------------------------------------------------------------
	afx_msg void OnUpdateConStationMap(CCmdUI *pCmdUI);					//
	afx_msg void OnUpdateConLTEIndicator(CCmdUI *pCmdUI);				//
	afx_msg void OnUpdateConAnalysisIndicator(CCmdUI *pCmdUI);				//

	//�Զ���
	bool	My_Get_Power(CString v_sPower_Name);						//��ȡ������Ȩ��[��/��]
	bool	My_ExecuteSQL(CString v_sSql);								//ִ��SQL����
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

