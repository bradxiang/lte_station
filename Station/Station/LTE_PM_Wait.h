#pragma once
#include "afxwin.h"

#include "LTE_PM_Data_Cell.h"											//小区性能查询
#include "LTE_PM_Data_Real.h"											//实时性能查询


// CLTE_PM_Wait 对话框

class CLTE_PM_Wait : public CDialog
{
	DECLARE_DYNAMIC(CLTE_PM_Wait)

public:
	CLTE_PM_Wait(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLTE_PM_Wait();

// 对话框数据
	enum { IDD = IDD_LTE_PM_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();										//
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//
	afx_msg void OnTimer(UINT_PTR nIDEvent);							//定时器


	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CEdit		m_cName;												//工作内容
	int			v_iParent;												//父窗体标志：  0：CIP_Flow；	1：CKPI_Flow；

	CLTE_PM_Data_Cell		*v_pLTE_PM_Data_Cell;						//指向父类的指针
	CLTE_PM_Data_Real		*v_pLTE_PM_Data_Real;						//指向父类的指针
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
