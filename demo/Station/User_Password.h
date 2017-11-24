#pragma once
#include "afxwin.h"


// CUser_Password 对话框

class CUser_Password : public CDialog
{
	DECLARE_DYNAMIC(CUser_Password)

public:
	CUser_Password(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUser_Password();

// 对话框数据
	enum { IDD = IDD_USER_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//消息预处理
	afx_msg void OnBnClickedOk();										//确认
	afx_msg void OnBnClickedCancel();									//取消


	//自定义
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CEdit	m_cPassword_Old;											//原密码
	CEdit	m_cPassword1;												//密码1
	CEdit	m_cPassword2;												//密码2

	//自定义

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
