
// MainFrm.h : CMainFrame 类的接口
//

#pragma once

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//------------------------------------------------------------------------------------------------------
//	变量定义	
//------------------------------------------------------------------------------------------------------
protected:																//控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;										//MenuBar 
	CMFCToolBar       m_wndToolBar;										//ToolBar

//------------------------------------------------------------------------------------------------------
//	消息映射函数定义	
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
//	函数定义
//------------------------------------------------------------------------------------------------------
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//重绘消息转发
	afx_msg void OnPaint();												//重绘屏幕
	afx_msg void OnClose();												//关闭程序框架
	
	//------------------------------------------------------------------------------------------------------
	//	变量定义
	//------------------------------------------------------------------------------------------------------
	CMFCStatusBar	m_wndStatusBar;										//StatusBar

	//自定义【权限授予】
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

	//自定义
	bool	My_Get_Power(CString v_sPower_Name);						//获取：功能权限[有/无]
	bool	My_ExecuteSQL(CString v_sSql);								//执行SQL命令
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

