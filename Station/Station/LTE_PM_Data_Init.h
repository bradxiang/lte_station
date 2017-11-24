#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"


// CLTE_PM_Data_Init 窗体视图

class CLTE_PM_Data_Init : public CFormView
{
	DECLARE_DYNCREATE(CLTE_PM_Data_Init)

protected:
	CLTE_PM_Data_Init();           // 动态创建所使用的受保护的构造函数
	virtual ~CLTE_PM_Data_Init();

public:
	enum { IDD = IDD_LTE_PM_DATA_Init };
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//消息预处理 [截取键盘消息到窗体]
	afx_msg void OnDestroy();											//销毁窗体
	afx_msg void OnSize(UINT nType, int cx, int cy);					//调整尺寸
	afx_msg void OnBnClickedExcel();									//
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);//
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnCloseupCombo3();
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);

	//自定义
	void		My_LoadData_Cell();										//装载Cell信息
	void		My_LoadData_PM();										//装载性能信息
	void		My_LoadData_PM_Data(CString v_sTable,CString v_sCell,CString v_sCell_ID,CString v_sDate_Ben,CString v_sDate_End);	//装载性能数据
	CString		My_Get_Table();											//获取表名称
	void		My_LoadData_Para();										//装载性能参数信息
	CString		My_Get_Table_Num(int v_iDay);							//获取表序号

	//线程：导入
	CWinThread* v_pThread_Input;										//线程指针
	static UINT My_Thread_Inoput(LPVOID lparam);						//线程：导入
	void		My_Input_Main();										//导入_主程序

	//分窗
	CSSplitter		m_SplitterPane_Main;								//垂直线

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//
	CMyListCtrl		m_cList_PM;
	CComboBox		m_cPM_Type;
	CEdit			m_cLog_Sum;											//
	CDateTimeCtrl	m_cLog_Date;										//
	CComboBox		m_cLog_Day;											//
	CEdit			m_cLog_Key;											//
	CButton			m_cQuery;											//
	CButton			m_cExcel;											//
	CComboBox		m_cType;											//
	CEdit			m_cPM_Sum;											//
	CEdit			m_cData_Sum;										//

	//自定义
	CString		v_sFrame_Name;											//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器
	int				v_iWorking;											//工作代号：
	int				v_iList_Item;										//选中的列表Item

	//性能数据
	mLTE_PM_Type	v_mLTE_PM_Type;										//容器：性能类型
	set<CString>	v_mTable_Name;										//集合：性能表名称

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

