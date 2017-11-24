#if !defined(AFX_MUTITREECTRL_H__1298A0CF_BFBF_414E_A1E1_BA18D9B39FBB__INCLUDED_)
#define AFX_MUTITREECTRL_H__1298A0CF_BFBF_414E_A1E1_BA18D9B39FBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MutiTreeCtrl.h : header file

/********************************************************************
	created:	2003/05/06
	created:	6:5:2003   12:08
	filename: 	d:\w\mutitreectrl.h
	file path:	d:\w
	file base:	mutitreectrl
	file ext:	h
	author:		王佳豪
	
	purpose:	
*********************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CMyMutiTreeCtrl window

class CMyMutiTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CMyMutiTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyMutiTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	//系统定义函数------------------------------------------------------------------------------------------------------
	virtual ~CMyMutiTreeCtrl();

	//自定义函数--------------------------------------------------------------------------------------------------------
	BOOL SetItemState( HTREEITEM hItem, UINT nState, UINT nStateMask, BOOL bSearch=TRUE);
	bool Set_MutiTree(int v_iNode_Num,CString v_sNode_Name,int v_iParent_Num);				//在MutiTree中加入节点或叶子
	bool DeleteAllItems();												//删除MutiTree的所有节点
	void ExpandTree_Level(HTREEITEM v_hNode,int v_iNum);				//树展开n级
	void ExpandTree(HTREEITEM v_hNode);									//树展开所有节点
	void CloseTree(HTREEITEM v_hNode);									//树收缩所有节点
	int	 Add_Node(CString v_sNode_Name,int v_iNode_ID);					//增加一个节点(基于选中的节点句柄)
	void Add_Root(CString v_sNode_Name,int v_iNode_ID);					//增加根节点
	void Get_Node_ID();													//获得：选中节点的ID[数据库节点ID]
	int	 Del_Node();													//删除：选中的节点

	//自定义变量--------------------------------------------------------------------------------------------------------
	HTREEITEM	v_hTree_Node;											//选中的Tree条目句柄
	int			v_iTree_Node_ID;										//选中的Tree 节点ID
	CString		v_sNode_Path;											//增加节点的路径


	// Generated message map functions
protected:
	//{{AFX_MSG(CMyMutiTreeCtrl)
	//自定义函数--------------------------------------------------------------------------------------------------------
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnStateIconClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	//自定义函数--------------------------------------------------------------------------------------------------------
	void TravelSiblingAndParent(HTREEITEM hItem, int nState);
	void TravelChild(HTREEITEM hItem,int nState);

	//自定义变量--------------------------------------------------------------------------------------------------------
	UINT m_uFlags;
	//Tree节点序号与树编号对应容器
	typedef map<int,HTREEITEM>		Tree_Node;							//Tree节点序号与树编号对应容器
	Tree_Node						v_mTree_Node;						//实例化
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUTITREECTRL_H__1298A0CF_BFBF_414E_A1E1_BA18D9B39FBB__INCLUDED_)
