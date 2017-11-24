#pragma once
// #include "afxcmn.h"
// #include "afxwin.h"
#include "Resource.h"
#include "afxwin.h"

// CPM_LTE_Alarm 对话框

class CPM_LTE_Alarm : public CDialog
{
	DECLARE_DYNAMIC(CPM_LTE_Alarm)

public:
	CPM_LTE_Alarm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPM_LTE_Alarm();

	// 对话框数据
	enum { IDD = IDD_PM_LTE_ALARM  };

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
	afx_msg void OnBnClickedQuary2();

	//自定义
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令
	void		My_LoadData_Per();										//装载信息

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
	CEdit		m_cKey;
	CComboBox	m_cKey_Select;

	//自定义
	int				v_iItem;											//选中的条目序号
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
