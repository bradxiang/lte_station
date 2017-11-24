#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"



// CAlarm_PM 窗体视图

class CAlarm_PM : public CFormView
{
	DECLARE_DYNCREATE(CAlarm_PM)

protected:
	CAlarm_PM();           // 动态创建所使用的受保护的构造函数
	virtual ~CAlarm_PM();

public:
	enum { IDD = IDD_Alarm_PM };
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedExcel2();
	afx_msg void OnLvnColumnclickList9(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAlarmPmCell();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedExcel3();

	//自定义
	void		My_LoadData();											//装载日志信息
	void		My_Begin();												//开始查询
	COleDateTime My_LoadData_Time();									//获取性能粒度
	void		My_LoadData_Para();										//装载性能参数信息
	void		My_LoadData_Region();									//性能查询：区域
	void		My_LoadData_Cell();										//性能查询：小区

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
	CMyListCtrl		m_cList;
	CMyListCtrl		m_cList_PM;
	CEdit			m_cLog_Sum;
	CDateTimeCtrl	m_cLog_Date;
	CComboBox		m_cLog_Day;
	CEdit			m_cLog_Key;
	CButton			m_cQuery;
	CButton			m_cExcel;
	CComboBox		m_cType;
	CButton			m_cReal;
	CButton			m_cTime;

	//自定义
	CString		v_sFrame_Name;											//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器

	COleDateTime	v_dPM_Date;											//采集粒度

	COleDateTime	v_dAlarm_Date;										//预警时间
	bool			v_bAlarm;											//告警标志
	int				v_iList_Item;										//选中的列表Item

	//性能数据
	mLTE_PM_Type	v_mLTE_PM_Type;										//容器：性能类型
	set<CString>	v_mTable_Name;										//集合：性能表名称
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

