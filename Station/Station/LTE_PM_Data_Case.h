#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
//#include "LTE_PM_Data.h"
#include "MyDataStruct.h"


// LTE_PM_DATA_CASE 对话框

class LTE_PM_DATA_CASE : public CDialog
{
	DECLARE_DYNAMIC(LTE_PM_DATA_CASE)

public:
	LTE_PM_DATA_CASE(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LTE_PM_DATA_CASE();

// 对话框数据
	enum { IDD = IDD_LTE_PM_DATA_CASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedenter();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();

	//------------------------------------------------------------------------------------------------------
	//	变量函数	
	//------------------------------------------------------------------------------------------------------
	void	My_PM_Insert(CString v_sType,CString v_sPM_Name);			//增加：选中的性能

	void	My_LoadData_Case();											//获取模板
	void    My_LoadData_Case_All(CString v_sAccount,CString v_sName);	//获取模板详细信息【小区、性能】

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//
	CMyListCtrl		m_cList_Cell;										//

	int				v_iList_Item;										//列表选项
	int				v_iList_Item_OK;									//列表选项
	int				v_iSelect;											//标志：1：性能；0：小区；

	mLTE_PM_Type*	v_mLTE_PM_Type;										//容器：性能参数
	set<CString>	v_mCell;											//容器：小区
	CButton m_cCheck;
	afx_msg void OnBnClickedCheck1();
	CEdit m_cSum;
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
