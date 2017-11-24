// MutiTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyMutiTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/********************************************************************
	created:	2003/05/06
	created:	6:5:2003   12:07
	filename: 	d:\Project\mutitreectrl.cpp
	file path:	d:\Project
	file base:	mutitreectrl
	file ext:	cpp
	author:		王佳豪
	
	purpose:	
*********************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CMyMutiTreeCtrl

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CMyMutiTreeCtrl::CMyMutiTreeCtrl()
{
	m_uFlags=0;
	v_hTree_Node = NULL;												//选择的条目无效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CMyMutiTreeCtrl::~CMyMutiTreeCtrl()
{
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMyMutiTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CMyMutiTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, OnStateIconClick)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyMutiTreeCtrl message handlers

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::OnStateIconClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_uFlags&TVHT_ONITEMSTATEICON) *pResult=1;
	else *pResult = 0;
}


//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	HTREEITEM hItem =HitTest(point, &m_uFlags);
	if ( (m_uFlags&TVHT_ONITEMSTATEICON ))
	{
		//nState: 0->无选择钮 1->没有选择 2->部分选择 3->全部选择
		UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
		nState=(nState==3)?1:3;
			(this->GetParent())->PostMessage(WM_USER + 12,0,(LPARAM)hItem);
		SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
	
	}
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
 	//处理空格键
 	if(nChar==0x20)
 	{
 		HTREEITEM hItem =GetSelectedItem();
 		UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
 		if(nState!=0)
 		{
 			nState=(nState==3)?1:3;
 			SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
 		}
 	}
	else CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	
	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
BOOL CMyMutiTreeCtrl::SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask, BOOL bSearch)
{
	BOOL bReturn=CTreeCtrl::SetItemState( hItem, nState, nStateMask );

	UINT iState = nState >> 12;
	if(iState!=0)
	{
//		if(bSearch) 
//			TravelChild(hItem, iState);
//		TravelSiblingAndParent(hItem,iState);
	}
	return bReturn;
}

//------------------------------------------------------------------------------------------------------
//	函数功能：递归搜索子节点
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::TravelChild(HTREEITEM hItem, int nState)
{
	HTREEITEM hChildItem,hBrotherItem; // 子 兄
	
	//查找子节点，没有就结束
	hChildItem=GetChildItem(hItem);
	if(hChildItem!=NULL)
	{
			this->GetParent()->PostMessage(WM_USER+12,0,(LPARAM)hChildItem);

		//设置子节点的状态与当前节点的状态一致
		CTreeCtrl::SetItemState( hChildItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
		//再递归处理子节点的子节点和兄弟节点
		TravelChild(hChildItem, nState);
		
		//处理子节点的兄弟节点和其子节点
		hBrotherItem=GetNextSiblingItem(hChildItem);
		while (hBrotherItem)
		{
			this->GetParent()->PostMessage(WM_USER+12,0,(LPARAM)hBrotherItem);
			//设置子节点的兄弟节点状态与当前节点的状态一致
			int nState1 = GetItemState( hBrotherItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=0)
			{
				CTreeCtrl::SetItemState( hBrotherItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
			}
			//再递归处理子节点的兄弟节点的子节点和兄弟节点
			TravelChild(hBrotherItem, nState);
			hBrotherItem=GetNextSiblingItem(hBrotherItem);
		}
	}
}

//------------------------------------------------------------------------------------------------------
//	函数功能：递归搜索兄、父节点
//	参数：；
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::TravelSiblingAndParent(HTREEITEM hItem, int nState)
{
	HTREEITEM hNextSiblingItem,hPrevSiblingItem,hParentItem;  //下一个姐妹兄弟 上一个姐妹兄弟  父亲节点
	
	//查找父节点，没有就结束
	hParentItem=GetParentItem(hItem);
	if(hParentItem!=NULL)
	{
		int nState1=nState;//设初始值，防止没有兄弟节点时出错
		
		//查找当前节点下面的兄弟节点的状态
		hNextSiblingItem=GetNextSiblingItem(hItem);
		while(hNextSiblingItem!=NULL)
		{
			nState1 = GetItemState( hNextSiblingItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=nState && nState1!=0) break;
			else hNextSiblingItem=GetNextSiblingItem(hNextSiblingItem);
		}
		
		if(nState1==nState)
		{
			//查找当前节点上面的兄弟节点的状态
			hPrevSiblingItem=GetPrevSiblingItem(hItem);
			while(hPrevSiblingItem!=NULL)
			{
				nState1 = GetItemState( hPrevSiblingItem, TVIS_STATEIMAGEMASK ) >> 12;
				if(nState1!=nState && nState1!=0) break;
				else hPrevSiblingItem=GetPrevSiblingItem(hPrevSiblingItem);
			}
		}
		
		if(nState1==nState || nState1==0)
		{
			nState1 = GetItemState( hParentItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=0)
			{
				//如果状态一致，则父节点的状态与当前节点的状态一致
				CTreeCtrl::SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
			}
			//再递归处理父节点的兄弟节点和其父节点
			TravelSiblingAndParent(hParentItem,nState);
		}
		else
		{
			//状态不一致，则当前节点的父节点、父节点的父节点……状态均为第三态
			hParentItem=GetParentItem(hItem);
			while(hParentItem!=NULL)
			{
				nState1 = GetItemState( hParentItem, TVIS_STATEIMAGEMASK ) >> 12;
				if(nState1!=0)
				{
					CTreeCtrl::SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );
				}
				hParentItem=GetParentItem(hParentItem);
			}
		}
	}	
}

//------------------------------------------------------------------------------------------------------
//	函数功能：在MutiTree中加入节点或叶子
//	参数：v_iNode_Num：节点序号；v_sNode_Name：节点名称；v_iParent_Num：父节点序号；
//------------------------------------------------------------------------------------------------------
bool CMyMutiTreeCtrl::Set_MutiTree(int v_iNode_Num,CString v_sNode_Name,int v_iParent_Num)
{
	HTREEITEM v_hItem;													//节点句柄
	Tree_Node::iterator v_pIterator_Node;								//迭代器
		
	if(v_iParent_Num==0)												//根节点？
	{
		if (v_sNode_Name.Find("Pool")>=0)
			v_hItem=InsertItem(v_sNode_Name,4,5,TVI_ROOT);				//添加根节点
		else
			v_hItem=InsertItem(v_sNode_Name,2,3,TVI_ROOT);				//添加根节点
		v_hTree_Node=v_hItem;											//获得：节点信息
	}
	else
	{
		v_pIterator_Node=v_mTree_Node.find(v_iParent_Num);				//查找Node[依据v_iParent_Num]
		if(v_pIterator_Node != v_mTree_Node.end())						//找到？
		{
			v_hItem=InsertItem(v_sNode_Name,0,1,v_pIterator_Node->second);		//添加子节点
			v_hTree_Node=v_hItem;										//获得：节点信息
		}
		else
			return false;												//没找到父节点，失败返回；
	}
	v_mTree_Node.insert(pair<int,HTREEITEM>(v_iNode_Num,v_hItem));		//加入容器：节点编号及其树句柄；

	//获得：增加节点的路径
	v_sNode_Path=v_sNode_Name;											//赋值：节点名称
	v_hItem=GetParentItem(v_hItem);										//获得：选中节点的父节点
	while(v_hItem != NULL)												//父节点是否存在？？？
	{
		v_sNode_Path=GetItemText(v_hItem)+"."+v_sNode_Path;				//节点路径
		v_hItem=GetParentItem(v_hItem);									//获得：选中节点的父节点
	}

	return true;
}

//------------------------------------------------------------------------------------------------------
//	函数功能：删除Tree信息
//------------------------------------------------------------------------------------------------------
bool CMyMutiTreeCtrl::DeleteAllItems()
{
	CTreeCtrl::DeleteAllItems();										//删除Tree节点信息
	v_mTree_Node.clear();												//清楚Tree节点序号与树编号对应容器
	v_hTree_Node=NULL;													//无选中的有效节点[用于添加新节点]
	return true;
}

//------------------------------------------------------------------------------------------------------
//	函数功能：树展开所有节点
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::ExpandTree(HTREEITEM v_hNode)
{
	if(!ItemHasChildren(v_hNode))
		return;
	HTREEITEM hNextItem = GetChildItem(v_hNode);
	while (hNextItem != NULL)
	{
		ExpandTree(hNextItem);
		hNextItem = GetNextItem(hNextItem, TVGN_NEXT);
	}
	Expand(v_hNode,TVE_EXPAND);
}

//------------------------------------------------------------------------------------------------------
//	函数功能：增加根节点
//	参数：v_sNode_Name：节点名称；v_iNode_ID：节点ID(数据库使用)；
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::Add_Root(CString v_sNode_Name,int v_iNode_ID)
{
	HTREEITEM v_hItem;													//临时变量

	if (v_sNode_Name.Find("Pool")>=0)
		v_hItem=InsertItem(v_sNode_Name,4,5,TVI_ROOT);					//添加根节点
	else
		v_hItem=InsertItem(v_sNode_Name,2,3,TVI_ROOT);					//添加根节点
	v_mTree_Node.insert(pair<int,HTREEITEM>(v_iNode_ID,v_hItem));		//加入容器：节点编号及其树句柄；
	ExpandTree(GetRootItem());											//展开树
}

//------------------------------------------------------------------------------------------------------
//	函数功能：增加一个节点(基于选中的节点句柄)
//	参数：v_sNode_Name：节点名称；v_iNode_ID：节点ID(数据库使用)；
//	返回：-1：没有选择树的节点；	-2：节点名称重复；	其它：选中节点的节点ID[用于获得增加节点的父节点ID]
//------------------------------------------------------------------------------------------------------
int CMyMutiTreeCtrl::Add_Node(CString v_sNode_Name,int v_iNode_ID)
{
//	Tree_Node::iterator v_pIterator_Node;								//迭代器
	HTREEITEM v_hItem;													//临时变量

	if(v_hTree_Node == NULL)											//有效的节点句柄
		return -1;														//无效的节点句柄
	v_hItem=GetChildItem(v_hTree_Node);									//获得：选中节点的子节点
	while(v_hItem != NULL)												//节点名称是否重复？？？
	{
		if (v_sNode_Name==GetItemText(v_hItem))							//节点名称重复？？？
			return -2;													//节点名称重复
		v_hItem=GetNextSiblingItem(v_hItem);							//获得下一个兄弟节点；
	}
	v_hItem=InsertItem(v_sNode_Name,0,1,v_hTree_Node);					//添加子节点
	v_mTree_Node.insert(pair<int,HTREEITEM>(v_iNode_ID,v_hItem));		//加入容器：节点编号及其树句柄；

	//获得：增加节点的路径
	v_sNode_Path=v_sNode_Name;											//赋值：节点名称
	v_hItem=GetParentItem(v_hItem);										//获得：选中节点的父节点
	while(v_hItem != NULL)												//父节点是否存在？？？
	{
		v_sNode_Path=GetItemText(v_hItem)+"."+v_sNode_Path;				//节点路径
		v_hItem=GetParentItem(v_hItem);									//获得：选中节点的父节点
	}

	ExpandTree(GetRootItem());											//展开树
	return v_iTree_Node_ID;												//返回：父节点的节点ID[用于数据库保存：双亲表示法]
}

//------------------------------------------------------------------------------------------------------
//	函数功能：获得选中节点的ID
//	结果：v_iTree_Node_ID赋值为选中节点的ID
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::Get_Node_ID()
{
	Tree_Node::iterator v_pIterator_Node;								//迭代器

	v_pIterator_Node=v_mTree_Node.begin();								//开始位置
	while(v_pIterator_Node !=v_mTree_Node.end())						//结束位置？？？
	{
		if (v_pIterator_Node->second==v_hTree_Node)						//找到节点句柄？？？
		{
			v_iTree_Node_ID=v_pIterator_Node->first;					//获得：节点ID
			break;
		}
		v_pIterator_Node++;												//递增
	}
}

//------------------------------------------------------------------------------------------------------
//	函数功能：删除选中的节点
//	结果：树中节点删除；[v_hTree_Node]
//------------------------------------------------------------------------------------------------------
int CMyMutiTreeCtrl::Del_Node()
{
	Tree_Node::iterator v_pIterator_Node;								//迭代器
	HTREEITEM v_hItem;													//临时变量

	if(v_hTree_Node == NULL)											//有效的节点句柄
		return -1;														//无效的节点句柄
	v_hItem=GetChildItem(v_hTree_Node);									//获得：选中节点的子节点
	if(v_hItem != NULL)													//节点有子节点？？？
		return -2;														//含有子节点，不能删除，退出；

	DeleteItem(v_hTree_Node);											//删除选择的节点
	return 1;
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
