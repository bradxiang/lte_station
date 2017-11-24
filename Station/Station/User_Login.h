#pragma once
#include "afxwin.h"


// CUser_Login 对话框

class CUser_Login : public CDialog
{
	DECLARE_DYNAMIC(CUser_Login)

public:
	CUser_Login(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUser_Login();

// 对话框数据
	enum { IDD = IDD_USER_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//消息预处理
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//自定义
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令
	void		My_User_Online();										//在线用户登记

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CEdit	m_cAccount;
	CEdit	m_cPassword;

	//自定义

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
