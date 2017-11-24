#pragma once
// #include "afxcmn.h"
// #include "afxwin.h"
#include "Resource.h"
#include "afxwin.h"

// CLTE_PM_Data_Real_Limit 对话框

class CLTE_PM_Data_Real_Limit : public CDialog
{
	DECLARE_DYNAMIC(CLTE_PM_Data_Real_Limit)

public:
	CLTE_PM_Data_Real_Limit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLTE_PM_Data_Real_Limit();

	// 对话框数据
	enum { IDD = IDD_LTE_PM_DATA_REAL_LIMIT  };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();										//对话窗初始化
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//消息预处理
	afx_msg void OnSize(UINT nType, int cx, int cy);					//调整窗体尺寸
	afx_msg void OnBnClickedExcel();									//导出
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);		//点击Pool列表
	afx_msg void OnBnClickedButton1();

	//自定义

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl	m_cList;												//列表窗变量
	CEdit		m_cSum;													//列表窗：条目总数
	CEdit		m_cPm_Value;											//预警门限
	CButton		m_cExcel;
	CButton		m_cPm_Flag;
	CComboBox	m_cPm_Type;
	CButton		m_cUpdate;
	CEdit		m_cPm_Name;

	//自定义
	mLTE_PM_Type*	v_mLTE_PM_Type;										//性能参数
	int				v_iItem;											//选中的条目序号
	CString			v_sItem_ID;											//选中条目的ID
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
