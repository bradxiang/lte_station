#pragma once
#include "afxwin.h"


// CCON_Station_Region 对话框

class CCON_Station_Region : public CDialog
{
	DECLARE_DYNAMIC(CCON_Station_Region)

public:
	CCON_Station_Region(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCON_Station_Region();

// 对话框数据
	enum { IDD = IDD_CON_Station_Region };

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

	//线程：导入
	CWinThread* v_pThread_Input;										//线程指针
	static UINT My_Thread_Inoput(LPVOID lparam);						//线程：导入
	int			v_iThread_Exit;											//线程退出标记：2：强制退出；1：工作；0：线程停止

	//自定义
	void		My_Input_Main();										//导入_主程序
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令
	void		My_Region();											//区域同步

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CEdit	m_cAccount;
	CEdit	m_cPassword;
	CButton m_cRegion;

	//自定义
	int				v_iWorking;											//工作代号：

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
