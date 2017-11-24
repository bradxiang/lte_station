#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "LTE_PM_Data_Cell.h"
#include "afxdtctl.h"

// LTE_PM_Data_Cell_Time 对话框

class CLTE_PM_Data_Cell_Time : public CDialog
{
	DECLARE_DYNAMIC(CLTE_PM_Data_Cell_Time)

public:
	CLTE_PM_Data_Cell_Time(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLTE_PM_Data_Cell_Time();

// 对话框数据
	enum { IDD = IDD_LTE_PM_DATA_CELL_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();

	//自定义


	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CButton			m_cDate_Check_1;
	CButton			m_cDate_Check_2;
	CButton			m_cDate_Check_3;
	CButton			m_cDate_Check_4;
	CButton			m_cDate_Check_5;
	CDateTimeCtrl	m_cDate_Date_1;
	CDateTimeCtrl	m_cDate_Date_2;
	CDateTimeCtrl	m_cDate_Date_3;
	CDateTimeCtrl	m_cDate_Date_4;
	CDateTimeCtrl	m_cDate_Date_5;
	CComboBox		m_cDate_Day_1;
	CComboBox		m_cDate_Day_2;
	CComboBox		m_cDate_Day_3;
	CComboBox		m_cDate_Day_4;
	CComboBox		m_cDate_Day_5;
	CButton			m_cTime_Check_1;
	CButton			m_cTime_Check_2;
	CButton			m_cTime_Check_3;
	CButton			m_cTime_Check_4;
	CButton			m_cTime_Check_5;
	CComboBox		m_cTime_Hour1_1;
	CComboBox		m_cTime_Hour1_2;
	CComboBox		m_cTime_Hour1_3;
	CComboBox		m_cTime_Hour1_4;
	CComboBox		m_cTime_Hour1_5;
	CComboBox		m_cTime_Hour2_1;
	CComboBox		m_cTime_Hour2_2;
	CComboBox		m_cTime_Hour2_3;
	CComboBox		m_cTime_Hour2_4;
	CComboBox		m_cTime_Hour2_5;

	//自定义
	Date_Select*	v_dDate_Select;										//日期选择
	Time_Select*	v_dTime_Select;										//时段选择
	mInt_String*	v_mTime;											//容器

	COleDateTime	v_dDate_1;											//日期1
	int				v_iDay_1;											//天数1

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
