#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPm_GSM_BSC_Add 对话框

class CPm_GSM_BSC_Add : public CDialog
{
	DECLARE_DYNAMIC(CPm_GSM_BSC_Add)

public:
	CPm_GSM_BSC_Add(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPm_GSM_BSC_Add();

// 对话框数据
	enum { IDD = IDD_PM_GSM_BSC_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//消息预处理
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedExcel();
	afx_msg void OnBnClickedAdd2();
	afx_msg void OnBnClickedButton1();


	//自定义
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令
	void		My_LoadData();											//呈现：数据表信息
	bool		My_New_Name(CString v_sName);							//名称重复检查

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//列表窗：用户
	CEdit			m_cSum;												//用户总数
	CComboBox		m_cRole;											//下拉窗：Role
	CButton			m_cAdd;
	CButton			m_cModify;
	CButton			m_cDelete;
	CButton			m_cExcel;
	CEdit			m_cName;
	CEdit			m_cObjType;
	CEdit			m_cText;
	
	//自定义
	int				v_iList_State;										//标志：0：查询；	1：增加；	2：修改；
	int				v_iList_Item;										//选中的条目序号

	afx_msg void OnBnClickedButton2();
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
