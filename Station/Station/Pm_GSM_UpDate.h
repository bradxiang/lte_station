#pragma once
#include "afxwin.h"


// CPm_GSM_UpDate 对话框

class CPm_GSM_UpDate : public CDialog
{
	DECLARE_DYNAMIC(CPm_GSM_UpDate)

public:
	CPm_GSM_UpDate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPm_GSM_UpDate();

// 对话框数据
	enum { IDD = IDD_PM_GSM_UPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//消息预处理
	afx_msg void OnBnClickedOk();

	//自定义
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CEdit	m_cPM_Table;
	CEdit	m_cType;
	CEdit	m_cDay;

	//自定义
	CString v_sPM_Table;												//性能组
	CString	v_sType;													//类型
	CString v_sDay;														//保留天数

protected:
public:
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
