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
	author:		���Ѻ�
	
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
	//ϵͳ���庯��------------------------------------------------------------------------------------------------------
	virtual ~CMyMutiTreeCtrl();

	//�Զ��庯��--------------------------------------------------------------------------------------------------------
	BOOL SetItemState( HTREEITEM hItem, UINT nState, UINT nStateMask, BOOL bSearch=TRUE);
	bool Set_MutiTree(int v_iNode_Num,CString v_sNode_Name,int v_iParent_Num);				//��MutiTree�м���ڵ��Ҷ��
	bool DeleteAllItems();												//ɾ��MutiTree�����нڵ�
	void ExpandTree_Level(HTREEITEM v_hNode,int v_iNum);				//��չ��n��
	void ExpandTree(HTREEITEM v_hNode);									//��չ�����нڵ�
	void CloseTree(HTREEITEM v_hNode);									//���������нڵ�
	int	 Add_Node(CString v_sNode_Name,int v_iNode_ID);					//����һ���ڵ�(����ѡ�еĽڵ���)
	void Add_Root(CString v_sNode_Name,int v_iNode_ID);					//���Ӹ��ڵ�
	void Get_Node_ID();													//��ã�ѡ�нڵ��ID[���ݿ�ڵ�ID]
	int	 Del_Node();													//ɾ����ѡ�еĽڵ�

	//�Զ������--------------------------------------------------------------------------------------------------------
	HTREEITEM	v_hTree_Node;											//ѡ�е�Tree��Ŀ���
	int			v_iTree_Node_ID;										//ѡ�е�Tree �ڵ�ID
	CString		v_sNode_Path;											//���ӽڵ��·��


	// Generated message map functions
protected:
	//{{AFX_MSG(CMyMutiTreeCtrl)
	//�Զ��庯��--------------------------------------------------------------------------------------------------------
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnStateIconClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	//�Զ��庯��--------------------------------------------------------------------------------------------------------
	void TravelSiblingAndParent(HTREEITEM hItem, int nState);
	void TravelChild(HTREEITEM hItem,int nState);

	//�Զ������--------------------------------------------------------------------------------------------------------
	UINT m_uFlags;
	//Tree�ڵ����������Ŷ�Ӧ����
	typedef map<int,HTREEITEM>		Tree_Node;							//Tree�ڵ����������Ŷ�Ӧ����
	Tree_Node						v_mTree_Node;						//ʵ����
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUTITREECTRL_H__1298A0CF_BFBF_414E_A1E1_BA18D9B39FBB__INCLUDED_)
