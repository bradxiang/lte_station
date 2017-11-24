#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"



// CPM_LTE_Cal 窗体视图

class CPM_LTE_Cal : public CFormView
{
	DECLARE_DYNCREATE(CPM_LTE_Cal)

protected:
	CPM_LTE_Cal();           // 动态创建所使用的受保护的构造函数
	virtual ~CPM_LTE_Cal();

public:
	enum { IDD = IDD_PM_LTE_CAL };
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
	afx_msg void OnBnClickedQuary();									//
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);	//树Item变化相应
	afx_msg void OnBnClickedElement();									//网元管理
	afx_msg void OnBnClickedExcel2();
	afx_msg void OnConLteCellDel();
	afx_msg void OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);

	//自定义
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令
	void		My_Query_Station();										//查询：站址
	void		My_LoadData_Station_Data(CString v_sSql);				//装载站址信息

	//线程：导入
	CWinThread* v_pThread_Input;										//线程指针
	static UINT My_Thread_Inoput(LPVOID lparam);						//线程：导入
	void		My_Input_Main();										//导入_主程序

	//------------------------------------------------------------------------------------------------------
	//	变量定义
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList_Station;									//Station信息
	CEdit			m_cSum;												//网元数量
	CEdit			m_cKey;												//Key
	CComboBox		m_cKey_Select;
	CButton			m_cQuery;
	CButton			m_cExcel;
	CButton			m_cExcel_NodeB;
	CButton			m_cElement;

	//自定义
	CString			v_sFrame_Name;										//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器
	int				v_iWorking;											//工作代号：
	int				v_iList_Item;										//选中的列表Item
	int				v_iSum;												//数据总数
};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
