#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"


// CCM_LTE_Quary 窗体视图

class CCM_LTE_Quary : public CFormView
{
	DECLARE_DYNCREATE(CCM_LTE_Quary)

protected:
	CCM_LTE_Quary();           // 动态创建所使用的受保护的构造函数
	virtual ~CCM_LTE_Quary();

public:
	enum { IDD = IDD_CM_LTE_QUARY };
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
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnCloseupBegDate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult);

	//自定义
	void		My_LoadData_CM();										//装载配置数据
	void		My_LoadData_CM_Data(CString v_sTable,CString v_sDate_Ben,CString v_sDate_End);	//获取配置数据

	void		My_LoadData_Para();										//装载性能参数信息
	void		My_Get_Cm_Field(CString v_sType);						//获取配置参数(列显示)
	void		My_Get_File_Field(mString_Int* v_mField,CString v_sTable);		//获取文件字段

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
	CComboBox		m_cType_2;
	CDateTimeCtrl	m_cLog_Date;										//
	CComboBox		m_cLog_Day;											//
	CEdit			m_cLog_Key;											//
	CEdit			m_cLog_Key1;
	CButton			m_cQuery;											//
	CButton			m_cExcel;											//
	CComboBox		m_cType;
	CEdit			m_cData_Sum;
	CComboBox		m_cRelation;
	CButton			m_cDay;

	//自定义
	CString			v_sFrame_Name;										//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器
	int				v_iWorking;											//工作代号：
	int				v_iList_Item;										//选中的列表Item
	COleDateTime	v_dDate;											//选择的日期
	CString			v_sTable_Pro;										//文件前缀【Cm_LTE_HW_AAA_1】(就差月份)
	CString			v_sTable_All;										//文件全程【Cm_LTE_HW_AAA_1_201608】
	int				v_iCM_Sum;											//获取的配置总数

	//性能数据
	set<CString>	v_mTable_Name;										//集合：性能表名称
	mString_Int		v_mField_Main;										//选择的字段容器

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

