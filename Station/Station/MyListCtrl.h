/* **********************************************************
* 功能： 
*		1、设置某一行的颜色
*		2、按表头排序
*		3、双击可编辑 （编辑后修改已屏蔽，通过OnEditEnd函数修改）
* 修改日期：
* 		2011.11.16----------王克锋
* **********************************************************/

#if !defined(AFX_MYLISTCTRL_H__B26FE6EC_7377_4DA6_BEC1_E3C656DACFB8__INCLUDED_)
#define AFX_MYLISTCTRL_H__B26FE6EC_7377_4DA6_BEC1_E3C656DACFB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <Afxtempl.h>
//#include <windowsx.h>
#define WM_USER_EDIT_END  WM_USER + 1001


// 下面是列表控件中编辑所需要的编辑框--------------------------------------------------------------------------------------------------------------------------------

// ListCtrlEdit.h : header file
//
class CMyListCtrlEdit : public CEdit
{
// Construction
public:
	CMyListCtrlEdit();

// Attributes
public:
	void  SetCtrlData(DWORD dwData);
	DWORD GetCtrlData();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEdit)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyListCtrlEdit();
	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	DWORD m_dwData;
	BOOL m_bExchange;
};

// 自己的ListControl 控件------------------------------------------------------------------------------------------------------------------------------
// MyListCtrl.h : header file
class CMyListCtrl : public CListCtrl
{
// Construction
public:
	CMyListCtrl();

// Attributes
public:

// Operations
public:
	void SetItemBkColor(DWORD iItem , COLORREF color);							//设置背景颜色
	void SetItemTextColor(DWORD iItem , COLORREF color);						//设置背景颜色
	void DeleteAllItems();														//清除ListCtrl的所有Item
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListCtrl)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyListCtrl();              

	//自定义函数--------------------------------------------------------------------------------------------------------

	//排序
	bool	SetNumber(int v_iCol);												//设置列：数字比较
	bool	ClearNumber(int v_iCol);											//设置列：数字比较无效
	void	My_Sort(NMHDR * pNMHDR);											//ListCtrl排序

	//编辑
	void	SetEdit(bool v_bEdit);												//设置：可编辑
	void	SetEdit_Col(int v_iCol);											//设置：可编辑列

	set<int> v_mEdit;															//集合：编辑行号

	//虚拟列表
	void	Put_Line_Begin();													//虚拟列表添加数据开始
	void	Put_Line_Data(mInt_String v_mLine_Data);							//虚拟列表保存行信息
	void	Put_Line_End();														//虚拟列表添加数据结束
	void	Get_List_Data(NMLVDISPINFO *pDispInfo);								//虚拟列表获取显示的数据
	void	CSV_OutPut_Virtual(CString v_sFile_Name,int v_bFile_Open);			//输出CSV文件[虚拟列表]
	void	My_Sort_Virtual(NMHDR * pNMHDR);									//ListCtrl排序(表头处理)
	static bool	My_Comp(mInt_String v_cData_1, mInt_String v_cData_2);			//列表排序函数
	static CMyListCtrl	*v_pList;												//虚拟列表指针(排序需要)

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyListCtrl)
	afx_msg void	OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void	OnInsertitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void	OnParentNotify(UINT message, LPARAM lParam);
	//}}AFX_MSG
	afx_msg LRESULT OnEditEnd(WPARAM ,LPARAM lParam = FALSE);
	afx_msg void	OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

	//自定义函数--------------------------------------------------------------------------------------------------------
	static	int		CALLBACK CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);	// 排序回调函函数
	static	bool	IsNumber(  LPCTSTR pszText );													// 数字判断	
	static	bool	IsNumber_Flag(  LPCTSTR pszText );												// 数字判断	
	
private:	
	// 自定义变量-------------------------------------------------------------------------------------------------------	
	BOOL    m_bAsc;																//是否顺序排序
	int     m_nSortedCol;														//当前排序的列
	int		m_iOldSortedCol;													//上一次的排序列
	int		m_nItem;															//行
	int		m_nSub;																//列
	int		m_iCount;															//计算器
	bool	m_bNumber[200];														//数字列设置： true：数字；false：字符；
	bool	m_bEdit;															//编辑标志
	set<int> 	m_mItem_Edit;													//可编辑列

	COLORREF crWindowText,crWindow;												//文字颜色、背景颜色
	mInt_Color			MapItemColor;											//文字颜色
	mInt_Color			MapItemBkColor;											//背景颜色

	CMyListCtrlEdit		m_edit;													//

	//编辑
	BOOL Key_Ctrl(int& nItem,int &nSub);										//
	BOOL Key_Shift(int& nItem,int& nSub);										//
	void ShowEdit(BOOL bShow,int nItem,int nIndex,CRect rc = CRect(0,0,0,0));	//

	//虚拟列表
	typedef list<mInt_String>				lList_Data;							//队列定义：存储虚拟列表数据【行、列】
	typedef map<int,lList_Data::iterator>	mList_Rec;							//队列定义：存储虚拟列表数据【行、列】

	lList_Data					v_lList_Data;									//队列：存储虚拟列表数据【行、列】
	mList_Rec					v_mList_Rec;									//容器：存储虚拟列表行与队列迭代器的映射【由行快速找到行数据】

	bool						v_bList_Show;									//数据提供标志[1：数据完成，可以提供；0：数据未就绪，请等待；]
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL_H__B26FE6EC_7377_4DA6_BEC1_E3C656DACFB8__INCLUDED_)
