#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"

#include "MyChartViewer.h"


// CAlarm_LL_Real 窗体视图

class CAlarm_LL_Real : public CFormView
{
	DECLARE_DYNCREATE(CAlarm_LL_Real)

protected:
	CAlarm_LL_Real();           // 动态创建所使用的受保护的构造函数
	virtual ~CAlarm_LL_Real();

public:
	enum { IDD = IDD_ALARM_LL_REAL };
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
	afx_msg void OnBnClickedQuary3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedExcel4();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo3();

	//自定义
	void		My_LoadData_Per();										//查询小区流量信息
	void		My_LoadData_PM_Data(int v_iTime,int v_iType,CString v_sTable,bool v_bCell,map_Alarm_LL_PM_DATA* v_mAlarm_LL_Real);		//装载性能数据
	CString		My_Get_Table_Num(int v_iDay);							//获取表序号
	void		My_LoadData_Para();										//装载性能参数信息
	void		My_LoadData_Cell();										//装载小区信息
	COleDateTime My_LoadData_Time();									//获取性能粒度

	//绘图
	void		My_List_Chart(int v_iList_Item);						//绘图：选中的Item
	void		My_List_Chart_GetNode(int v_iList_Item);				//获得：节点的信息 [用于：查询；v_sItem_Instance；	显示：v_sItem_Instance_Name]
	void		My_Chart_IP();											//装载IP信息
	void		My_Chart_New();											//绘图复位
	void		My_Load_Performance(CString v_sType,CString v_sFile_Key,CString v_sCell,CString v_sCell_ID,CString v_sPerformance,CString v_sField,COleDateTime v_dDate_Start,int v_iDay_Sum);	//获取数据表中的IP数据
	void		My_Chart();												//单线绘图
	void		My_Show_Clear();										//清除屏幕显示信息

	//分窗
	CStatic		m_cDraw_AAA;											//绘图
	CSSplitter	m_SplitterPane_Main;									//垂直线
	CSSplitter	m_SplitterPane_Left;									//水平线

	//线程：导入
	CWinThread* v_pThread_Input;										//线程指针
	static UINT My_Thread_Inoput(LPVOID lparam);						//线程：导入
	void		My_Input_Main();										//导入_主程序

	//------------------------------------------------------------------------------------------------------
	//	变量定义
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//低流量信息
	CMyListCtrl		m_cList_Cell;										//小区信息
	CEdit			m_cSum;												//网元数量
	CButton			m_cQuery;
	CButton			m_cExcel;
	CButton			m_cTime;
	CEdit			m_cLog_Sum;
	CEdit			m_cLog_Key;
	CButton			m_cCell;
	CStatic			m_cTime_Real;
	CButton			m_cAuto;
	CButton			m_cOnline;
	CButton			m_cDay;
	CButton			m_cPoint;
	CComboBox		m_cDay_Sum;											//天数
	CComboBox		m_cPara;											//参数

	//自定义
	CString			v_sFrame_Name;										//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器
	int				v_iWorking;											//工作代号：
	int				v_iList_Item;										//选中的列表Item[低流量]
	int				v_iList_Item_Cell;									//选中的列表Item[小区]
	bool			v_bSelect;											//标记：true：小区；false：低流量；
	CString			v_sFilename;										//文件名称
	COleDateTime	v_dPM_Date;											//采集粒度

	//查询参数
	bool			v_bDown,v_bUp;										//标志：性能有效(下行、上行)
	Alarm_LL_Cell	v_dDown_Select[4];									//日期选择：下行
	Alarm_LL_Cell	v_dUp_Select[4];									//日期选择：上行

	//获取小区结果
	map_Alarm_LL_PM_DATA	v_mAlarm_LL_Real_Down;						//容器：小区性能数据_下行
	map_Alarm_LL_PM_DATA	v_mAlarm_LL_Real_Up;						//容器：小区性能数据_上行

	//性能数据
	mLTE_PM_Type	v_mLTE_PM_Type;										//容器：性能参数
	set<CString>	v_mTable_Name;										//集合：性能表名称

	//小区信息
	mString_String	v_mCell;											//容器：小区   -- 小区ID
	map_Cell_ID		v_mCell_ID;											//容器：小区ID -- 小区

	CString         v_sCell;											//选中条目：Cell名称
	CString         v_sCell_ID;											//选中条目：Cell_ID
	CString			v_sCell_All;										//选择的小区[小区模板]

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
//	End
//------------------------------------------------------------------------------------------------------
