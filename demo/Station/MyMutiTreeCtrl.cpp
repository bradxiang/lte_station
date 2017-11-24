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
	author:		���Ѻ�
	
	purpose:	
*********************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CMyMutiTreeCtrl

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CMyMutiTreeCtrl::CMyMutiTreeCtrl()
{
	m_uFlags=0;
	v_hTree_Node = NULL;												//ѡ�����Ŀ��Ч
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CMyMutiTreeCtrl::~CMyMutiTreeCtrl()
{
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
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
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::OnStateIconClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_uFlags&TVHT_ONITEMSTATEICON) *pResult=1;
	else *pResult = 0;
}


//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	HTREEITEM hItem =HitTest(point, &m_uFlags);
	if ( (m_uFlags&TVHT_ONITEMSTATEICON ))
	{
		//nState: 0->��ѡ��ť 1->û��ѡ�� 2->����ѡ�� 3->ȫ��ѡ��
		UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
		nState=(nState==3)?1:3;
			(this->GetParent())->PostMessage(WM_USER + 12,0,(LPARAM)hItem);
		SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
	
	}
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
 	//����ո��
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
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	
	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
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
//	�������ܣ��ݹ������ӽڵ�
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::TravelChild(HTREEITEM hItem, int nState)
{
	HTREEITEM hChildItem,hBrotherItem; // �� ��
	
	//�����ӽڵ㣬û�оͽ���
	hChildItem=GetChildItem(hItem);
	if(hChildItem!=NULL)
	{
			this->GetParent()->PostMessage(WM_USER+12,0,(LPARAM)hChildItem);

		//�����ӽڵ��״̬�뵱ǰ�ڵ��״̬һ��
		CTreeCtrl::SetItemState( hChildItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
		//�ٵݹ鴦���ӽڵ���ӽڵ���ֵܽڵ�
		TravelChild(hChildItem, nState);
		
		//�����ӽڵ���ֵܽڵ�����ӽڵ�
		hBrotherItem=GetNextSiblingItem(hChildItem);
		while (hBrotherItem)
		{
			this->GetParent()->PostMessage(WM_USER+12,0,(LPARAM)hBrotherItem);
			//�����ӽڵ���ֵܽڵ�״̬�뵱ǰ�ڵ��״̬һ��
			int nState1 = GetItemState( hBrotherItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=0)
			{
				CTreeCtrl::SetItemState( hBrotherItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
			}
			//�ٵݹ鴦���ӽڵ���ֵܽڵ���ӽڵ���ֵܽڵ�
			TravelChild(hBrotherItem, nState);
			hBrotherItem=GetNextSiblingItem(hBrotherItem);
		}
	}
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ��ݹ������֡����ڵ�
//	��������
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::TravelSiblingAndParent(HTREEITEM hItem, int nState)
{
	HTREEITEM hNextSiblingItem,hPrevSiblingItem,hParentItem;  //��һ�������ֵ� ��һ�������ֵ�  ���׽ڵ�
	
	//���Ҹ��ڵ㣬û�оͽ���
	hParentItem=GetParentItem(hItem);
	if(hParentItem!=NULL)
	{
		int nState1=nState;//���ʼֵ����ֹû���ֵܽڵ�ʱ����
		
		//���ҵ�ǰ�ڵ�������ֵܽڵ��״̬
		hNextSiblingItem=GetNextSiblingItem(hItem);
		while(hNextSiblingItem!=NULL)
		{
			nState1 = GetItemState( hNextSiblingItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=nState && nState1!=0) break;
			else hNextSiblingItem=GetNextSiblingItem(hNextSiblingItem);
		}
		
		if(nState1==nState)
		{
			//���ҵ�ǰ�ڵ�������ֵܽڵ��״̬
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
				//���״̬һ�£��򸸽ڵ��״̬�뵱ǰ�ڵ��״̬һ��
				CTreeCtrl::SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
			}
			//�ٵݹ鴦���ڵ���ֵܽڵ���丸�ڵ�
			TravelSiblingAndParent(hParentItem,nState);
		}
		else
		{
			//״̬��һ�£���ǰ�ڵ�ĸ��ڵ㡢���ڵ�ĸ��ڵ㡭��״̬��Ϊ����̬
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
//	�������ܣ���MutiTree�м���ڵ��Ҷ��
//	������v_iNode_Num���ڵ���ţ�v_sNode_Name���ڵ����ƣ�v_iParent_Num�����ڵ���ţ�
//------------------------------------------------------------------------------------------------------
bool CMyMutiTreeCtrl::Set_MutiTree(int v_iNode_Num,CString v_sNode_Name,int v_iParent_Num)
{
	HTREEITEM v_hItem;													//�ڵ���
	Tree_Node::iterator v_pIterator_Node;								//������
		
	if(v_iParent_Num==0)												//���ڵ㣿
	{
		if (v_sNode_Name.Find("Pool")>=0)
			v_hItem=InsertItem(v_sNode_Name,4,5,TVI_ROOT);				//��Ӹ��ڵ�
		else
			v_hItem=InsertItem(v_sNode_Name,2,3,TVI_ROOT);				//��Ӹ��ڵ�
		v_hTree_Node=v_hItem;											//��ã��ڵ���Ϣ
	}
	else
	{
		v_pIterator_Node=v_mTree_Node.find(v_iParent_Num);				//����Node[����v_iParent_Num]
		if(v_pIterator_Node != v_mTree_Node.end())						//�ҵ���
		{
			v_hItem=InsertItem(v_sNode_Name,0,1,v_pIterator_Node->second);		//����ӽڵ�
			v_hTree_Node=v_hItem;										//��ã��ڵ���Ϣ
		}
		else
			return false;												//û�ҵ����ڵ㣬ʧ�ܷ��أ�
	}
	v_mTree_Node.insert(pair<int,HTREEITEM>(v_iNode_Num,v_hItem));		//�����������ڵ��ż����������

	//��ã����ӽڵ��·��
	v_sNode_Path=v_sNode_Name;											//��ֵ���ڵ�����
	v_hItem=GetParentItem(v_hItem);										//��ã�ѡ�нڵ�ĸ��ڵ�
	while(v_hItem != NULL)												//���ڵ��Ƿ���ڣ�����
	{
		v_sNode_Path=GetItemText(v_hItem)+"."+v_sNode_Path;				//�ڵ�·��
		v_hItem=GetParentItem(v_hItem);									//��ã�ѡ�нڵ�ĸ��ڵ�
	}

	return true;
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�ɾ��Tree��Ϣ
//------------------------------------------------------------------------------------------------------
bool CMyMutiTreeCtrl::DeleteAllItems()
{
	CTreeCtrl::DeleteAllItems();										//ɾ��Tree�ڵ���Ϣ
	v_mTree_Node.clear();												//���Tree�ڵ����������Ŷ�Ӧ����
	v_hTree_Node=NULL;													//��ѡ�е���Ч�ڵ�[��������½ڵ�]
	return true;
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ���չ�����нڵ�
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
//	�������ܣ����Ӹ��ڵ�
//	������v_sNode_Name���ڵ����ƣ�v_iNode_ID���ڵ�ID(���ݿ�ʹ��)��
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::Add_Root(CString v_sNode_Name,int v_iNode_ID)
{
	HTREEITEM v_hItem;													//��ʱ����

	if (v_sNode_Name.Find("Pool")>=0)
		v_hItem=InsertItem(v_sNode_Name,4,5,TVI_ROOT);					//��Ӹ��ڵ�
	else
		v_hItem=InsertItem(v_sNode_Name,2,3,TVI_ROOT);					//��Ӹ��ڵ�
	v_mTree_Node.insert(pair<int,HTREEITEM>(v_iNode_ID,v_hItem));		//�����������ڵ��ż����������
	ExpandTree(GetRootItem());											//չ����
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�����һ���ڵ�(����ѡ�еĽڵ���)
//	������v_sNode_Name���ڵ����ƣ�v_iNode_ID���ڵ�ID(���ݿ�ʹ��)��
//	���أ�-1��û��ѡ�����Ľڵ㣻	-2���ڵ������ظ���	������ѡ�нڵ�Ľڵ�ID[���ڻ�����ӽڵ�ĸ��ڵ�ID]
//------------------------------------------------------------------------------------------------------
int CMyMutiTreeCtrl::Add_Node(CString v_sNode_Name,int v_iNode_ID)
{
//	Tree_Node::iterator v_pIterator_Node;								//������
	HTREEITEM v_hItem;													//��ʱ����

	if(v_hTree_Node == NULL)											//��Ч�Ľڵ���
		return -1;														//��Ч�Ľڵ���
	v_hItem=GetChildItem(v_hTree_Node);									//��ã�ѡ�нڵ���ӽڵ�
	while(v_hItem != NULL)												//�ڵ������Ƿ��ظ�������
	{
		if (v_sNode_Name==GetItemText(v_hItem))							//�ڵ������ظ�������
			return -2;													//�ڵ������ظ�
		v_hItem=GetNextSiblingItem(v_hItem);							//�����һ���ֵܽڵ㣻
	}
	v_hItem=InsertItem(v_sNode_Name,0,1,v_hTree_Node);					//����ӽڵ�
	v_mTree_Node.insert(pair<int,HTREEITEM>(v_iNode_ID,v_hItem));		//�����������ڵ��ż����������

	//��ã����ӽڵ��·��
	v_sNode_Path=v_sNode_Name;											//��ֵ���ڵ�����
	v_hItem=GetParentItem(v_hItem);										//��ã�ѡ�нڵ�ĸ��ڵ�
	while(v_hItem != NULL)												//���ڵ��Ƿ���ڣ�����
	{
		v_sNode_Path=GetItemText(v_hItem)+"."+v_sNode_Path;				//�ڵ�·��
		v_hItem=GetParentItem(v_hItem);									//��ã�ѡ�нڵ�ĸ��ڵ�
	}

	ExpandTree(GetRootItem());											//չ����
	return v_iTree_Node_ID;												//���أ����ڵ�Ľڵ�ID[�������ݿⱣ�棺˫�ױ�ʾ��]
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ����ѡ�нڵ��ID
//	�����v_iTree_Node_ID��ֵΪѡ�нڵ��ID
//------------------------------------------------------------------------------------------------------
void CMyMutiTreeCtrl::Get_Node_ID()
{
	Tree_Node::iterator v_pIterator_Node;								//������

	v_pIterator_Node=v_mTree_Node.begin();								//��ʼλ��
	while(v_pIterator_Node !=v_mTree_Node.end())						//����λ�ã�����
	{
		if (v_pIterator_Node->second==v_hTree_Node)						//�ҵ��ڵ���������
		{
			v_iTree_Node_ID=v_pIterator_Node->first;					//��ã��ڵ�ID
			break;
		}
		v_pIterator_Node++;												//����
	}
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�ɾ��ѡ�еĽڵ�
//	��������нڵ�ɾ����[v_hTree_Node]
//------------------------------------------------------------------------------------------------------
int CMyMutiTreeCtrl::Del_Node()
{
	Tree_Node::iterator v_pIterator_Node;								//������
	HTREEITEM v_hItem;													//��ʱ����

	if(v_hTree_Node == NULL)											//��Ч�Ľڵ���
		return -1;														//��Ч�Ľڵ���
	v_hItem=GetChildItem(v_hTree_Node);									//��ã�ѡ�нڵ���ӽڵ�
	if(v_hItem != NULL)													//�ڵ����ӽڵ㣿����
		return -2;														//�����ӽڵ㣬����ɾ�����˳���

	DeleteItem(v_hTree_Node);											//ɾ��ѡ��Ľڵ�
	return 1;
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
