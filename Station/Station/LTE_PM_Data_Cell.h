#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"


// CLTE_PM_Data_Cell 窗体视图

class CLTE_PM_Data_Cell : public CFormView
{
	DECLARE_DYNCREATE(CLTE_PM_Data_Cell)

protected:
	CLTE_PM_Data_Cell();           // 动态创建所使用的受保护的构造函数
	virtual ~CLTE_PM_Data_Cell();

public:
	enum { IDD = IDD_LTE_PM_DATA_CELL };
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
	afx_msg void OnBnClickedExcel2();
	afx_msg void OnBnClickedExcel3();
	afx_msg void OnBnClickedExcel4();
	afx_msg void OnBnClickedExcel5();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedExcel8();
	afx_msg void OnBnClickedExcel6();
	afx_msg void OnBnClickedExcel7();
	afx_msg void OnLvnColumnclickList9(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult);

	//自定义
	void		My_LoadData_Cell();										//装载Cell信息
	void		My_LoadData_PM();										//装载性能信息
	void		My_LoadData_PM_Tj();									//性能统计
	void		My_LoadData_PM_Table(CString v_sDate,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End,int v_iNum,int v_iSum);	//装载性能表数据
	void		My_LoadData_PM_Data(mLTE_PM_Type::iterator v_pLTE_PM_Type,CString v_sTable,CString v_sField,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End);	//装载性能数据
	void		My_LoadData_PM_Data_Cal(CString v_sTable,CString v_sName_Cal,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End);	//装载性能数据
	bool		My_Get_Formula(CString v_sName,CString* v_sFormula,CString* v_sType);	//获取：性能类型、自定义公式
	void		My_LoadData_Para();										//装载性能参数信息
	void		My_Load_Columns(set<CString> *v_pTable_Columns,CString v_sTable);	//装载表结构
	void		My_PM_Data_Delete();									//容器清除：性能数据
	void		My_PM_Data_P_Delete();									//容器清除：统计的时段性能数据
	void		My_PM_Show();											//选择性能：列添加
	void		My_PM_Show_Data();										//显示：获取的性能明细
	void		My_PM_Show_Data_P();									//显示：统计的时段性能明细
	void		My_PM_Show_Data_Tj(mString_Double* v_mPM_Tj_Date,CString v_sCell,CString v_sRegion,CString v_sOnline,CString v_sDate,int v_iSum_Time);		//显示：时段性能明细
	CString		My_Get_Table_Num(int v_iDay);							//获取表序号

	//线程：导入
	CWinThread* v_pThread_Input;										//线程指针
	static UINT My_Thread_Inoput(LPVOID lparam);						//线程：导入
	void		My_Input_Main();										//导入_主程序

	//分窗
	CSSplitter		m_SplitterPane_Main;								//垂直线
	CSSplitter		m_SplitterPane_Left;								//垂直线

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//
	CMyListCtrl		m_cList_PM;
	CMyListCtrl		m_cList_Tj;
	CEdit			m_cLog_Sum;											//
	CDateTimeCtrl	m_cLog_Date;										//
	CComboBox		m_cLog_Day;											//
	CEdit			m_cLog_Key;											//
	CButton			m_cQuery;											//
	CButton			m_cExcel;											//
	CComboBox		m_cType;
	CEdit			m_cPM_Sum;
	CButton			m_cCheck;
	CButton			m_cPM_Cell;
	CButton			m_cCase_Cell;
	CButton			m_cCase_PM;
	CButton			m_cCase_Select;
	CButton			m_cTime_Check;
	CComboBox		m_cTj_Type;
	CButton			m_cSetup_Time;
	CButton			m_cTj;
	CButton			m_cExcel_Tj;
	CEdit			m_cSpan;
	CEdit			m_cQuary_Data;
	CStatic			m_cQuary_Name;

	//自定义
	CString			v_sFrame_Name;										//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器
	int				v_iWorking;											//工作代号：
	int				v_iList_Item;										//选中的列表Item

	//性能数据
	mLTE_PM_Type	v_mLTE_PM_Type;										//容器：性能类型
	set<CString>	v_mTable_Name;										//集合：性能表名称

	//小区信息
	mString_String	v_mCell;											//容器：小区   -- 小区ID
	map_Cell_ID		v_mCell_ID;											//容器：小区ID -- 小区

	mLTE_PM			v_mPM_Field_Num;									//容器：性能名称映射列序号

	map_PM_Cell		v_mPM_Cell;											//容器：性能数据【保存获取的性能数据】
	map_PM_Cell_P	v_mPM_Cell_P;										//容器：性能数据【保存统计的时段性能数据】
	mInt_String		v_mTime;											//容器：时段信息【08，08--10映射】

	Date_Select		v_dDate_Select[5];									//日期选择
	Time_Select		v_dTime_Select[5];									//时段选择

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

