#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"


// CLTE_PM_Data_Real 窗体视图

class CLTE_PM_Data_Real : public CFormView
{
	DECLARE_DYNCREATE(CLTE_PM_Data_Real)

protected:
	CLTE_PM_Data_Real();           // 动态创建所使用的受保护的构造函数
	virtual ~CLTE_PM_Data_Real();

public:
	enum { IDD = IDD_LTE_PM_DATA_REAL };
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedExcel6();
	afx_msg void OnLvnColumnclickList9(NMHDR *pNMHDR, LRESULT *pResult);

	//自定义
	void		My_LoadData_Cell();										//装载Cell信息
	void		My_LoadData_PM();										//装载性能信息
	void		My_LoadData_PM_Table(CString v_sDate,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End);	//装载性能表数据
	void		My_LoadData_PM_Data(mLTE_PM_Type::iterator v_pLTE_PM_Type,CString v_sTable,CString v_sField,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End);	//装载性能数据
	void		My_LoadData_Para();										//装载性能参数信息
	void		My_PM_Data_Delete();									//容器清除：性能数据
	void		My_PM_Show();											//选择性能：列添加
	COleDateTime My_LoadData_Time();									//获取性能粒度
	void		My_Alarm_Insert(COleDateTime v_dTime,CString v_sPM_Name,CString v_sObject,CString v_sPm_Value,CString v_sPm_Limite);	//增加：预警信息
	void		My_Alarm_Show(CString v_sTime,CString v_sPM_Name,CString v_sObject,CString v_sPm_Value,CString v_sPm_Limite,int v_iSum,int v_iSum_Cell);	//显示：预警信息
	void		My_DeleteFiles(CString v_sDirName);						//删除文件[*.xml]？

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
	CButton			m_cExcel_Tj;
	CButton			m_cAuto;											//自动刷新
	CStatic			m_cTime;											//时间
	CButton			m_cAlarm;											//
	CButton			m_cAlarm_All;										//单粒度预警

	//自定义
	CString			v_sFrame_Name;										//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器
	int				v_iWorking;											//工作代号：
	int				v_iList_Item;										//选中的列表Item
	CString			v_sPM_Working;										//性能获取进度
	COleDateTime	v_dPM_Date;											//采集粒度

	//性能数据
	mLTE_PM_Type	v_mLTE_PM_Type;										//容器：性能类型
	set<CString>	v_mTable_Name;										//集合：性能表名称

	//预警信息【各个预警的统计信息，即每个性能+小区在连续的粒度里预警的统计数据(粒度数量，预警数量)】
	struct  PM_Cell_Real_Alarm_Struct
	{
		CString				v_sPM_Name;									//告警性能名称
		CString				v_sObject;									//告警对象
		mDate_Int			v_dDate_Real;								//容器：告警粒度时间
	};  
	typedef map<CString,PM_Cell_Real_Alarm_Struct> mPM_Cell_Real_Alarm;	//容器：mPM_Cell_Real_Alarm

	mDate_Int				v_dDate_Real;								//容器：粒度时间
	mPM_Cell_Real_Alarm		v_mCell_Real_Alarm;							//容器：小区预警

	//小区信息
	mString_String	v_mCell;											//容器：小区   -- 小区ID
	map_Cell_ID		v_mCell_ID;											//容器：小区ID -- 小区

	mLTE_PM			v_mPM_Field_Num;									//容器：性能名称映射列序号

	map_PM_Cell		v_mPM_Cell;											//容器：性能数据【保存获取的性能数据】
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

