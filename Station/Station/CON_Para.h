#pragma once
#include "afxwin.h"


// CCON_Para 对话框

class CCON_Para : public CDialog
{
	DECLARE_DYNAMIC(CCON_Para)

public:
	CCON_Para(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCON_Para();

// 对话框数据
	enum { IDD = IDD_CON_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	//自定义
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令
	void		My_Get_Para();											//获得参数

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CComboBox	m_cHour_Month;
	CComboBox	m_cMin_Day;
	CComboBox	m_cFTP_Day;
	CComboBox	m_cCm_Month;
	CComboBox	m_cCm_FTP_Day;

	//自定义
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
