#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "LTE_PM_Draw.h"

// CLTE_PM_Draw_Znit_Select 对话框

class CLTE_PM_Draw_Znit_Select : public CDialog
{
	DECLARE_DYNAMIC(CLTE_PM_Draw_Znit_Select)

public:
	CLTE_PM_Draw_Znit_Select(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLTE_PM_Draw_Znit_Select();

// 对话框数据
	enum { IDD = IDD_LTE_PM_Draw_Znit_Select };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnCloseupCombo7();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();

	//ListCtrl拖动处理函数
	afx_msg void OnLvnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBegindragList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	//自定义
	bool	My_ExecuteSQL(CString v_sSql);								//执行SQL命令
	void	My_PM_Insert(CString v_sType,CString v_sPM_Name);			//增加：选中的性能
	void	My_Load_PM(mString_Int *v_mCase,mString_Int *v_mPM_Type);	//获取：性能


	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl			m_cList;
	CMyListCtrl			m_cList_PM;
	CComboBox			m_cType;										//
	CButton				m_cOrder;										//顺序
	CEdit				m_cPM_Case;										//性能模板名称
	CEdit				m_cSum;											//
	CButton				m_cShare;										//共享
	
	//自定义
	mLTE_PM_Type*		v_mLTE_PM_Type;									//性能参数
	int					v_iList_Item;									//列表选项
	int					v_iList_Item_OK;								//列表选项

protected:
	//ListCtrl拖动处理
	//变量
	CListCtrl*			m_pDragList;		//Which ListCtrl we are dragging FROM
	CListCtrl*			m_pDropList;		//Which ListCtrl we are dropping ON
	CImageList*			m_pDragImage;		//For creating and managing the drag-image
	BOOL				m_bDragging;		//T during a drag operation
	int					m_nDragIndex;		//Index of selected item in the List we are dragging FROM
	int					m_nDropIndex;		//Index at which to drop item in the List we are dropping ON
	CWnd*				m_pDropWnd;			//Pointer to window we are dropping on (will be cast to CListCtrl* type)
	//函数
	void DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList);

public:
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
