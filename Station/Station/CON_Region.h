#pragma once
// #include "afxcmn.h"
// #include "afxwin.h"
#include "Resource.h"
#include "afxwin.h"

// CCON_Region 对话框

class CCON_Region : public CDialog
{
	DECLARE_DYNAMIC(CCON_Region)

public:
	CCON_Region(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCON_Region();

	// 对话框数据
	enum { IDD = IDD_CON_REGION  };

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
	afx_msg void OnBnClickedAdd();										//增加
	afx_msg void OnBnClickedDelete();									//删除
	afx_msg void OnBnClickedExcel();									//导出
	afx_msg void OnBnClickedAdd2();										//增加确认
	afx_msg void OnBnClickedButton2();									//增加取消
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);		//点击Pool列表

	//自定义
	void		My_LoadData();											//呈现：数据表信息
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令
	bool		My_New_Name(CString v_sName);							//名称重复检查

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl	m_cList;												//列表窗变量
	CEdit		m_cSum;													//列表窗：条目总数
	CEdit		m_cName;												//名称输入
	CButton		m_cAdd;
	CButton		m_cDelete;
	CButton		m_cExcel;

	//自定义
	bool		v_bAdd;													//增加标志
	int			v_iItem;												//选中的条目序号
	CString		v_sItem_ID;												//选中条目的ID
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
