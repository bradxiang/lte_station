#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"



// CLog_Bug 窗体视图

class CLog_Bug : public CFormView
{
	DECLARE_DYNCREATE(CLog_Bug)

protected:
	CLog_Bug();           // 动态创建所使用的受保护的构造函数
	virtual ~CLog_Bug();

public:
	enum { IDD = IDD_LOG_BUG };
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
	//屏幕
	virtual void OnInitialUpdate();										//初始化
	afx_msg void OnDestroy();											//销毁窗体
	afx_msg void OnSize(UINT nType, int cx, int cy);					//调整尺寸
	afx_msg void OnBnClickedQuary();									//查询
	afx_msg void OnBnClickedExcel();									//导出
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);//排序
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//消息预处理 [截取键盘消息到窗体]

	//自定义
	void		My_LoadData();											//装载日志信息

	//线程：导入
	CWinThread* v_pThread_Input;										//线程指针
	static UINT My_Thread_Inoput(LPVOID lparam);						//线程：导入
	void		My_Input_Main();										//导入_主程序

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;
	CEdit			m_cLog_Sum;
	CDateTimeCtrl	m_cLog_Date;
	CComboBox		m_cLog_Day;
	CEdit			m_cLog_Key;
	CButton			m_cQuery;
	CButton			m_cExcel;

	//自定义
	CString		v_sFrame_Name;											//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

