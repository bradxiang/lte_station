#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"



// CPM_LTE_Case_Cell 窗体视图

class CPM_LTE_Case_Cell : public CFormView
{
	DECLARE_DYNCREATE(CPM_LTE_Case_Cell)

protected:
	CPM_LTE_Case_Cell();           // 动态创建所使用的受保护的构造函数
	virtual ~CPM_LTE_Case_Cell();

public:
	enum { IDD = IDD_PM_LTE_CASE_CELL };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual void OnInitialUpdate();										//初始化
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//消息预处理
	afx_msg void OnDestroy();											//
	afx_msg void OnSize(UINT nType, int cx, int cy);					//
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);	//树Item变化相应
	afx_msg void OnBnClickedExcel2();
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedQuary2();
	afx_msg void OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPmLteCaseDel();
	afx_msg void OnPmLteCaseWDel();
	afx_msg void OnBnClickedExcel3();
	afx_msg void OnNMRClickList5(NMHDR *pNMHDR, LRESULT *pResult);

	//自定义
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令
	void		My_LoadData_Per();										//装载信息
	void		My_LoadData_Case();										//装载信息
	void		My_Input_PER();											//数据导入

	//线程：导入
	CWinThread* v_pThread_Input;										//线程指针
	static UINT My_Thread_Inoput(LPVOID lparam);						//线程：导入
	void		My_Input_Main();										//导入_主程序

	//分窗
	CSSplitter		m_SplitterPane_Main;								//垂直线
	CSSplitter		m_SplitterPane_Bottom;								//垂直线

	//------------------------------------------------------------------------------------------------------
	//	变量定义
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//信息
	CMyListCtrl		m_cList_Case;										//信息
	CEdit			m_cSum;												//网元数量
	CButton			m_cQuery;
	CButton			m_cExcel;
	CEdit			m_cCase;

	//自定义
	CString			v_sFrame_Name;										//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器
	int				v_iWorking;											//工作代号：
	int				v_iList_Item;										//选中的列表Item
	CString			v_sName_Account,v_sName_Case;						//选中的列表Item的模板名称
};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
