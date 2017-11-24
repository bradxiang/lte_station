#pragma once
#include "afxcmn.h"
#include "afxcmn.h"

#include "MyChartViewer.h"
#include "afxdtctl.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"


// CLTE_PM_Draw 窗体视图

class CLTE_PM_Draw : public CFormView
{
	DECLARE_DYNCREATE(CLTE_PM_Draw)

protected:
	CLTE_PM_Draw();           // 动态创建所使用的受保护的构造函数
	virtual ~CLTE_PM_Draw();

public:
	enum { IDD = IDD_LTE_PM_DRAW };
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
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDtnCloseupBegDate(NMHDR *pNMHDR, LRESULT *pResult);  //
	afx_msg void OnCbnSelchangeCombo1();                                //
	afx_msg void OnBnClickedRefresh();                                  //
	afx_msg void OnBnClickedDelete();                                   //
	afx_msg void OnBnClickedCkMuti();									//
	afx_msg void OnBnClickedQuary();									//
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);		//
	afx_msg void OnBnClickedInstanceDetail();							//
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnCloseupCombo3();

	//自定义
	void		My_LoadData_Cell();										//装载Cell信息
	void		My_LoadData_Para();										//装载性能数据

	void		My_List_Chart(int v_iList_Item);						//绘图：选中的Item
	void		My_List_Chart_GetNode(int v_iList_Item);				//获得：节点的信息 [用于：查询；v_sItem_Instance；	显示：v_sItem_Instance_Name]
	void		My_Chart_IP();											//装载IP信息
	void		My_Chart_New();											//绘图复位
	void		My_Load_Performance(CString v_sType,CString v_sFile_Key,CString v_sCell,CString v_sCell_ID,CString v_sPerformance,CString v_sField,COleDateTime v_dDate_Start,int v_iDay_Sum);	//获取数据表中的IP数据
	void		My_Chart();												//单线绘图
	void		My_Chart_Muti();										//曲线绘制【单线、多线】
	void		My_Show_Clear();										//清除屏幕显示信息
	void		My_SendToFile();										//把数据导出到csv文件
	CString		My_Get_Table_Num(int v_iDay);							//获取表序号
   

	//分窗
	CStatic			m_cDraw_AAA;										//分窗载体
	CSSplitter		m_SplitterPane_Main;								//垂直线
	CSSplitter		m_SplitterPane_Left;								//水平线

	//m_cTree
	CImageList		v_cImage_Tree;										//树图标：标致
	CImageList		v_cImage_State;										//树图标：状态
	int				v_iItem_Tree;										//选中的Tree条目序号

	//线程：导入
	CWinThread*		v_pThread_Input;									//线程指针
	static UINT		My_Thread_Inoput(LPVOID lparam);					//线程：导入
	void			My_Input_Main();									//导入_主程序

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//
	CDateTimeCtrl	m_cDate;											//日期
	CComboBox		m_cDay_Sum;                                         //天数
	CButton			m_cDay;                                             //单天
	CButton			m_cMuti_Line;                                       //多线
	CButton			m_cKPI_S;                                           //静态
	CButton			m_cLable;                                           //名称
	CButton			m_cPoint;                                           //数据点
	CEdit			m_cSum;												//
	CEdit			m_cKey;												//
	CComboBox		m_cType;
	CButton			m_cQuery;
	CButton			m_cExcel;
	CButton			m_cPM_Select;
	CButton			m_cRefresh;
	CButton			m_cClear;
	CComboBox		m_cPM_Type;
	                                
    //自定义
	CString			v_sFrame_Name;										//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器
	int				v_iWorking;											//工作代号：
	int				v_iList_Item;										//选中的列表Item [Cell]
	
	//性能数据
	mLTE_PM_Type	v_mLTE_PM_Type;										//容器：性能参数
	set<CString>	v_mTable_Name;										//集合：性能表名称
	set<CString>	v_mRegion;											//集合：区域信息

	CString         v_sCell;											//选中条目：Cell名称
	CString         v_sCell_ID;											//选中条目：Cell_ID
//	CString         v_sNetwork;											//选中条目：网络

	SYSTEMTIME		v_dChart_Date;										//查询日期
 	int				v_iChart_Day_Sum;									//查询天的数量

	//------------------------------------------------------------------------------------------------------
	//	Chart定义
	//------------------------------------------------------------------------------------------------------
    //函数：Draw chart
	void			OnChart_Para();										//传递：绘图参数 [从屏幕到Chart变量]
	afx_msg BOOL	OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	//
	afx_msg void	OnViewPortChanged();								//
	afx_msg void	OnMouseMovePlotArea();								//
	afx_msg LRESULT Message_ChartView(WPARAM iParam1,LPARAM iParam2);	//更新ChartView

	//图形变量：Chart
	CMyChartViewer	m_cChartView;										//控件变量：Chart                                               
	bool			v_bShow;											//调整屏幕尺寸标志
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
