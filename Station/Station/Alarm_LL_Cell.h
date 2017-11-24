#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"



// CAlarm_LL_Cell 窗体视图

class CAlarm_LL_Cell : public CFormView
{
	DECLARE_DYNCREATE(CAlarm_LL_Cell)

protected:
	CAlarm_LL_Cell();           // 动态创建所使用的受保护的构造函数
	virtual ~CAlarm_LL_Cell();

public:
	enum { IDD = IDD_ALARM_LL_CELL };
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

	//自定义
	void		My_LoadData_Per();										//查询小区流量信息
	void		My_LoadData_PM_Data(int v_iTime,int v_iType,CString v_sTable,bool v_bCell);		//装载性能数据
	CString		My_Get_Table_Num(int v_iDay);							//获取表序号
	void		My_LoadData_Para();										//装载性能参数信息
	COleDateTime My_LoadData_Time();									//获取性能粒度

	//线程：导入
	CWinThread* v_pThread_Input;										//线程指针
	static UINT My_Thread_Inoput(LPVOID lparam);						//线程：导入
	void		My_Input_Main();										//导入_主程序

	//------------------------------------------------------------------------------------------------------
	//	变量定义
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//Station信息
	CEdit			m_cSum;												//网元数量
	CButton			m_cQuery;
	CButton			m_cExcel;
	CButton			m_cTime;

	//自定义
	CString			v_sFrame_Name;										//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器
	int				v_iWorking;											//工作代号：
	int				v_iList_Item;										//选中的列表Item
	CString			v_sFilename;										//文件名称

	//查询参数
	Alarm_LL_Cell	v_dDate_Select[5];									//日期选择
	bool			v_bDown,v_bUp;										//标志：性能有效(下行、上行)

	//获取小区结果
	map_Alarm_LL_PM_DATA	v_mAlarm_LL_Cell;							//容器：小区性能数据

	//性能数据
	set<CString>	v_mTable_Name;										//集合：性能表名称

	//小区信息
	map_Cell_ID		v_mCell_ID;											//容器：小区ID -- 小区

};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
