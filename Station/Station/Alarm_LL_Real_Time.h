#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CAlarm_LL_Real_Time 对话框

class CAlarm_LL_Real_Time : public CDialog
{
	DECLARE_DYNAMIC(CAlarm_LL_Real_Time)

public:
	CAlarm_LL_Real_Time(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAlarm_LL_Real_Time();

// 对话框数据
	enum { IDD = IDD_ALARM_LL_REAL_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck13();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedCheck10();
	afx_msg void OnBnClickedCheck11();
	afx_msg void OnBnClickedCheck12();
	afx_msg void OnCbnSelchangeCombo3();

	//自定义
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令


	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CButton		m_cDown;
	CButton		m_cUp;
	CButton		m_cDown_Time_1;
	CButton		m_cDown_Time_2;
	CButton		m_cDown_Time_3;
	CButton		m_cDown_Time_4;
	CEdit		m_cDown_Value_1;
	CEdit		m_cDown_Value_2;
	CEdit		m_cDown_Value_3;
	CEdit		m_cDown_Value_4;
	CButton		m_cUp_Time_1;
	CButton		m_cUp_Time_2;
	CButton		m_cUp_Time_3;
	CButton		m_cUp_Time_4;
	CEdit		m_cUp_Value_1;
	CEdit		m_cUp_Value_2;
	CEdit		m_cUp_Value_3;
	CEdit		m_cUp_Value_4;

	CComboBox	m_cPara;

	//自定义

};
//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
