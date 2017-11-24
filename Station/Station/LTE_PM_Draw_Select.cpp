// LTE_PM_Draw_Select.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Draw_Select.h"


// CLTE_PM_Draw_Select �Ի���

IMPLEMENT_DYNAMIC(CLTE_PM_Draw_Select, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CLTE_PM_Draw_Select::CLTE_PM_Draw_Select(CWnd* pParent /*=NULL*/)
	: CDialog(CLTE_PM_Draw_Select::IDD, pParent)
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CLTE_PM_Draw_Select::~CLTE_PM_Draw_Select()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_LIST_Master, m_cList);
	DDX_Control(pDX, IDC_LIST_Slave, m_cList_PM);
	DDX_Control(pDX, IDC_LOG_SUM, m_cSum);
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Draw_Select, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Draw_Select::OnBnClickedButton1)
	ON_CBN_CLOSEUP(IDC_COMBO7, &CLTE_PM_Draw_Select::OnCbnCloseupCombo7)
	ON_NOTIFY(NM_CLICK, IDC_LIST_Master, &CLTE_PM_Draw_Select::OnNMClickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST_Slave, &CLTE_PM_Draw_Select::OnNMClickList3)
	ON_BN_CLICKED(IDC_BUTTON3, &CLTE_PM_Draw_Select::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CLTE_PM_Draw_Select::OnBnClickedButton2)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_Master, &CLTE_PM_Draw_Select::OnLvnBegindragListMaster)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_Slave, &CLTE_PM_Draw_Select::OnLvnBegindragListSlave)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CLTE_PM_Draw_Select ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CLTE_PM_Draw_Select::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ʼ�����б� ����ѡ���ܡ�
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,40);					//
	m_cList.InsertColumn(1,"����",LVCFMT_LEFT,280);						//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��ʼ�����б� ����ѡ���ܡ�
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"���",LVCFMT_CENTER,40);					//
	m_cList_PM.InsertColumn(1,"����",LVCFMT_LEFT,100);					//
	m_cList_PM.InsertColumn(2,"����",LVCFMT_LEFT,280);					//
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��ʼ��
	v_iList_Item=-1;													//�б�ѡ���Ч
	v_iList_Item_OK=-1;													//�б�ѡ���Ч

	//��������
	CString					v_sTemp;									//��ʱ����
	mLTE_PM::iterator		v_pPM_Name;									//������
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������

	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//��ָ��
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//����
	{
		m_cType.AddString(v_pLTE_PM_Type->first);						//��������

		v_pLTE_PM_Type++;												//����
	}
	m_cType.SetCurSel(v_iSelect_PM_Type);								//Ĭ�ϣ���һ��

	//��ʾ��Ĭ��ѡ�е�����
	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//��ָ��
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//��������
	{
		v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();			//��ָ��
		while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//��������
		{
			if (v_pPM_Name->second.v_bOK)
				My_PM_Insert(v_pLTE_PM_Type->first,v_pPM_Name->second.v_sPM_Name);	//���ӣ�ѡ����Ŀ

			v_pPM_Name++;												//�ݼ�
		}
		v_pLTE_PM_Type++;												//�ݼ�
	}

	//��ʼ����ListCtrl�϶�����
	m_bDragging = false;												//��־���϶���Ч
	m_nDragIndex = -1;													//�϶�Դ
	m_nDropIndex = -1;													//�϶�Ŀ��
	m_pDragImage = NULL;												//�϶�ͼƬ

	OnCbnCloseupCombo7();												//��������ѡ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Draw_Select::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		CDialog* Pwnd=(CDialog*)GetActiveWindow();						//ȡ�öԻ���ָ�� 
		Pwnd->NextDlgCtrl();											//�л�����һ�����뽹�� 
		return true;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����������С
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��رմ���
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnClose();													//�˳�	
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����������ʼ�϶�
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnLvnBegindragListMaster(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//This routine sets the parameters for a Drag and Drop operation.
	//It sets some variables to track the Drag/Drop as well as creating the drag image to be shown during the drag.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// Save the index of the item being dragged in m_nDragIndex
	// This will be used later for retrieving the info dragged
	m_nDragIndex = pNMListView->iItem;

	// Create a drag image
	POINT pt;
	int nOffset = -10; //offset in pixels for drag image (positive is up and to the left; neg is down and to the right)
	if(m_cList.GetSelectedCount() > 1)									//more than one item is selected
		pt.x = nOffset;
	pt.y = nOffset;

	m_pDragImage = m_cList.CreateDragImage(m_nDragIndex, &pt);
	ASSERT(m_pDragImage);												//make sure it was created
	//We will call delete later (in LButtonUp) to clean this up

	CBitmap bitmap;
//	if(m_cList.GetSelectedCount() > 1)									//more than 1 item in list is selected
//		bitmap.LoadBitmap(IDB_TREE_CHECK_1);
//	else
//		bitmap.LoadBitmap(IDB_TREE_CHECK);
	bitmap.LoadBitmap(IDB_PM_Check);									//ȡ��ͼƬ
	m_pDragImage->Replace(0, &bitmap, &bitmap);

	// Change the cursor to the drag image
	// (still must perform DragMove() in OnMouseMove() to show it moving)
	m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);

	// Set dragging flag and others
	m_bDragging = TRUE;													//we are in a drag and drop operation
	m_nDropIndex = -1;													//we don't have a drop index yet
	m_pDragList = &m_cList;												//make note of which list we are dragging from
	m_pDropWnd = &m_cList;												//at present the drag list is the drop list

	// Capture all mouse messages
	SetCapture ();

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����ܴӴ���ʼ�϶�
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnLvnBegindragListSlave(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//This routine sets the parameters for a Drag and Drop operation.
	//It sets some variables to track the Drag/Drop as well as creating the drag image to be shown during the drag.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// Save the index of the item being dragged in m_nDragIndex
	// This will be used later for retrieving the info dragged
	m_nDragIndex = pNMListView->iItem;

	// Create a drag image
	POINT pt;
	int nOffset = -10; //offset in pixels for drag image
	pt.x = nOffset;
	pt.y = nOffset;

	m_pDragImage = m_cList_PM.CreateDragImage(m_nDragIndex, &pt);
	ASSERT(m_pDragImage);												//make sure it was created
	//We will call delete later (in LButtonUp) to clean this up

	CBitmap bitmap;
//	if(m_cList_PM.GetSelectedCount() > 1)								//more than 1 item in list is selected
//		bitmap.LoadBitmap(IDB_TREE_CHECK_1);
//	else
//		bitmap.LoadBitmap(IDB_TREE_CHECK);
	bitmap.LoadBitmap(IDB_PM_UnCheck);									//ȡ��ͼƬ
	m_pDragImage->Replace(0, &bitmap, &bitmap);

	// Change the cursor to the drag image
	// (still must perform DragMove() in OnMouseMove() to show it moving)
	m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);

	// Set dragging flag and others
	m_bDragging = TRUE;													//we are in a drag and drop operation
	m_nDropIndex = -1;													//we don't have a drop index yet
	m_pDragList = &m_cList_PM;											//make note of which list we are dragging from
	m_pDropWnd = &m_cList_PM;											//at present the drag list is the drop list

	// Capture all mouse messages
	SetCapture ();

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ƶ��������б��ڣ��ƶ�ͼ�����б��⣬�Ͻ�ͼ�񣻡�
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// While the mouse is moving, this routine is called.
	// This routine will redraw the drag image at the present mouse location to display the dragging.
	// Also, while over a CListCtrl, this routine will highlight the item we are hovering over.

	// If we are in a drag/drop procedure (m_bDragging is true)
	if (m_bDragging)													//�϶����У�����
	{
		// Move the drag image
		CPoint pt(point);												//get our current mouse coordinates
		ClientToScreen(&pt);											//convert to screen coordinates
		m_pDragImage->DragMove(pt);										//move the drag image to those coordinates
		// Unlock window updates (this allows the dragging image to be shown smoothly)
		m_pDragImage->DragShowNolock(false);

		// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT(pDropWnd);												//make sure we have a window

		// If we drag outside current window we need to adjust the highlights displayed
		if (pDropWnd != m_pDropWnd)
		{
			if (m_nDropIndex != -1)										//If we drag over the CListCtrl header, turn off the hover highlight
			{
				TRACE("m_nDropIndex is -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				VERIFY (pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED));
				// redraw item
				VERIFY (pList->RedrawItems (m_nDropIndex, m_nDropIndex));
				pList->UpdateWindow ();
				m_nDropIndex = -1;
			}
			else //If we drag out of the CListCtrl altogether
			{
				TRACE("m_nDropIndex is not -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				int i = 0;
				int nCount = pList->GetItemCount();
				for(i = 0; i < nCount; i++)
				{
					pList->SetItemState(i, 0, LVIS_DROPHILITED);
				}
				pList->RedrawItems(0, nCount);
				pList->UpdateWindow();
			}
		}

		// Save current window pointer as the CListCtrl we are dropping onto
		m_pDropWnd = pDropWnd;

		// Convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient(&pt);

		//If we are hovering over a CListCtrl we need to adjust the highlights
		if(pDropWnd->IsKindOf(RUNTIME_CLASS (CListCtrl)))
		{			
			//Note that we can drop here
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			UINT uFlags;
			CListCtrl* pList = (CListCtrl*)pDropWnd;

			// Turn off hilight for previous drop target
			pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
			// Redraw previous item
			pList->RedrawItems (m_nDropIndex, m_nDropIndex);

			// Get the item that is below cursor
			m_nDropIndex = ((CListCtrl*)pDropWnd)->HitTest(pt, &uFlags);
			// Highlight it
//			pList->SetItemState(m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
			// Redraw item
			pList->RedrawItems(m_nDropIndex, m_nDropIndex);
			pList->UpdateWindow();
		}
		else
		{
			//If we are not hovering over a CListCtrl, change the cursor
			// to note that we cannot drop here
			SetCursor(LoadCursor(NULL, IDC_NO));
		}
		// Lock window updates
		m_pDragImage->DragShowNolock(true);
	}

	CDialog::OnMouseMove(nFlags, point);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ͷš��϶������������ݴ���
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// This routine is the end of the drag/drop operation.
	// When the button is released, we are to drop the item.
	// There are a few things we need to do to clean up and
	// finalize the drop:
	//	1) Release the mouse capture
	//	2) Set m_bDragging to false to signify we are not dragging
	//	3) Actually drop the item (we call a separate function to do that)

	//If we are in a drag and drop operation (otherwise we don't do anything)
	if (m_bDragging)
	{
		// Release mouse capture, so that other controls can get control/messages
		ReleaseCapture ();

		// Note that we are NOT in a drag operation
		m_bDragging = FALSE;

		// End dragging image
		m_pDragImage->DragLeave (GetDesktopWindow ());
		m_pDragImage->EndDrag ();
		delete m_pDragImage;	//must delete it because it was created at the beginning of the drag

		CPoint pt (point); //Get current mouse coordinates
		ClientToScreen (&pt); //Convert to screen coordinates
		// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd); //make sure we have a window pointer
		// If window is CListCtrl, we perform the drop
		if (pDropWnd->IsKindOf (RUNTIME_CLASS (CListCtrl)))
		{
			m_pDropList = (CListCtrl*)pDropWnd; //Set pointer to the list we are dropping on
			DropItemOnList(m_pDragList, m_pDropList); //Call routine to perform the actual drop
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��϶������������ݴ���
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList)
{
	//This routine performs the actual drop of the item dragged.
	//It simply grabs the info from the Drag list (pDragList) and puts that info into the list dropped on (pDropList).
	//Send:	pDragList = pointer to CListCtrl we dragged from, pDropList = pointer to CListCtrl we are dropping on.
	//Return: nothing.

	//Variables

	// Unhilight the drop target
//	pDropList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);

	//
	if(pDragList->GetSelectedCount() == 1)								//ѡ��1��
	{
		if(pDragList != pDropList)										//�б�Դ!=�б�Ŀ��
		{
			if (m_pDragList==&m_cList)									//��������
			{
				v_iList_Item=m_nDragIndex;								//�������
				OnBnClickedButton1();									//��������
			}
			else														//ɾ������
			{
				v_iList_Item_OK=m_nDragIndex;							//�������
				OnBnClickedButton3();									//ɾ������
			}
		}
	}
	else //more than 1 item is being dropped
	{
		//We have to parse through all of the selected items from the DragList
		//1) Retrieve the info for the items and store them in memory
		//2) If we are reordering, delete the items from the list
		//3) Insert the items into the list (either same list or different list)
/*
		CList<lvItem*, lvItem*> listItems;
		POSITION listPos;

		//Retrieve the selected items
		POSITION pos = pDragList->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
		while(pos) //so long as we have a valid POSITION, we keep iterating
		{
			plvitem = new LVITEM;
			ZeroMemory(plvitem, sizeof(LVITEM));
			pItem = new lvItem;
			//ZeroMemory(pItem, sizeof(lvItem)); //If you use ZeroMemory on the lvItem struct, it creates an error when you try to set sCol2
			pItem->plvi = plvitem;
			pItem->plvi->iItem = m_nDragIndex;
			pItem->plvi->mask = LVIF_TEXT;
			pItem->plvi->pszText = new char; //since this is a pointer to the string, we need a new pointer to a new string on the heap
			pItem->plvi->cchTextMax = 255;

			m_nDragIndex = pDragList->GetNextSelectedItem(pos);

			//Get the item
			pItem->plvi->iItem = m_nDragIndex; //set the index in the drag list to the selected item
			pDragList->GetItem(pItem->plvi); //retrieve the information
			pItem->sCol2 = pDragList->GetItemText(pItem->plvi->iItem, 1);

			//Save the pointer to the new item in our CList
			listItems.AddTail(pItem);
		} //EO while(pos) -- at this point we have deleted the moving items and stored them in memory

		if(pDragList == pDropList) //we are reordering the list (moving)
		{
			//Delete the selected items
			pos = pDragList->GetFirstSelectedItemPosition();
			while(pos)
			{
				pos = pDragList->GetFirstSelectedItemPosition();
				m_nDragIndex = pDragList->GetNextSelectedItem(pos);

				pDragList->DeleteItem(m_nDragIndex); //since we are MOVING, delete the item
				if(m_nDragIndex < m_nDropIndex) m_nDropIndex--; //must decrement the drop index to account
				//for the deleted items
			} //EO while(pos)
		} //EO if(pDragList...

		//Iterate through the items stored in memory and add them back into the CListCtrl at the drop index
		listPos = listItems.GetHeadPosition();
		while(listPos)
		{
			pItem = listItems.GetNext(listPos);

			m_nDropIndex = (m_nDropIndex == -1) ? pDropList->GetItemCount() : m_nDropIndex;
			pItem->plvi->iItem = m_nDropIndex;
			pDropList->InsertItem(pItem->plvi); //add the item
			pDropList->SetItemText(pItem->plvi->iItem, 1, pItem->sCol2);

			pDropList->SetItemState(pItem->plvi->iItem, LVIS_SELECTED, LVIS_SELECTED); //highlight/select the item we just added

			m_nDropIndex++; //increment the index we are dropping at to keep the dropped items in the same order they were in in the Drag List
			//If we dont' increment this, the items are added in reverse order
		} //EO while(listPos)
*/
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������ѡ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnCbnCloseupCombo7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp,v_sType;										//��ʱ����
	int			v_iNum=0,v_iItem;										//��ʱ����
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	//ɾ���б���Ϣ
	m_cList.DeleteAllItems();											//ɾ��������Ŀ
	v_iList_Item=-1;													//�б�ѡ���Ч

	//������������
	m_cType.GetWindowText(v_sType);										//��ã���������
	v_pLTE_PM_Type=v_mLTE_PM_Type->find(v_sType);						//���ң���������
	v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();				//��ָ��
	while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())			//��������
	{
		v_sTemp.Format(" %04d",v_iNum+1);
		v_iItem=m_cList.InsertItem(0xffff,v_sTemp);						//�����б�
		m_cList.SetItemText(v_iItem,1,(_bstr_t)v_pPM_Name->second.v_sPM_Name);	//��������
		if (v_pPM_Name->second.v_bOK)
			m_cList.SetItemBkColor(v_iItem,RGB(0,255,0));				//���ã�����ɫ��ѡ�С�

		v_pPM_Name++;													//�ݼ�
		v_iNum++;														//������
	}

	//��ʾ������
	v_sTemp.Format("%d",m_cList_PM.GetItemCount());						//����
	m_cSum.SetWindowText(v_sTemp);										//��ʾ������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����б�ѡ�����ܡ�
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��

	v_iList_Item=pNMListView->iItem;									//��ã�ѡ����Ŀ���

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����б�ѡ�����ܡ�
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//��ã���Ŀָ��

	v_iList_Item_OK=pNMListView->iItem;									//��ã�ѡ����Ŀ���

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ѡ���˳�
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ�е�����
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sPM_Name,v_sType,v_sTemp;								//��ʱ����
	int			v_iNum=0;												//��ʱ����
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	if (v_iList_Item==-1)
	{
		MessageBox("  ��ѡ����Ч������ !!! ");							//������ʾ
		return;
	}

	//�������ܣ�����ѡ�е��������ƣ���δѡ��״̬��������ѡ��Item��
	m_cType.GetWindowText(v_sType);										//��ã���������
	v_pLTE_PM_Type=v_mLTE_PM_Type->find(v_sType);						//���ң���������
	v_sPM_Name=m_cList.GetItemText(v_iList_Item,1);						//��ȡ����������
	v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();				//��ָ��
	while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())			//��������
	{
		if(v_pPM_Name->second.v_sPM_Name == v_sPM_Name)					//����������ȣ�����
		{
			if (!v_pPM_Name->second.v_bOK)								//δѡ��״̬����
			{
				v_pPM_Name->second.v_bOK=1;								//��λ��ѡ��״̬
				m_cList.SetItemBkColor(v_iList_Item,RGB(0,255,0));		//���ã�����ɫ��ѡ�С�
				My_PM_Insert(v_sType,v_sPM_Name);						//���ӣ�ѡ����Ŀ
			}
			break;
		}

		v_pPM_Name++;													//�ݼ�
		v_iNum++;														//������
	}

	//��ʾ������
	v_sTemp.Format("%d",m_cList_PM.GetItemCount());						//����
	m_cSum.SetWindowText(v_sTemp);										//��ʾ������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ɾ��ѡ�е�����
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sPM_Name,v_sType,v_sTemp;								//��ʱ����
	int			v_iNum;													//��ʱ����
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	if (v_iList_Item_OK==-1)
	{
		MessageBox("  ��ѡ����Ч������ !!! ");							//������ʾ
		return;
	}

	//ɾ��ѡ�е�Item
	v_sTemp=m_cList_PM.GetItemText(v_iList_Item_OK,1);					//��ȡ����������
	v_sPM_Name=m_cList_PM.GetItemText(v_iList_Item_OK,2);				//��ȡ����������
	m_cList_PM.DeleteItem(v_iList_Item_OK);								//ɾ��ѡ�е�Item

	//�������ܣ�����ѡ�е��������ƣ�����ѡ��ItemΪδѡ��״̬��
	m_cType.GetWindowText(v_sType);										//��ã���������

	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//��ָ��
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//��������
	{
		v_iNum=0;														//��λ��������
		v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();			//��ָ��
		while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//��������
		{
			if(v_pPM_Name->second.v_sPM_Name == v_sPM_Name)				//����������ȣ�����
			{
				v_pPM_Name->second.v_bOK=0;								//��λ��δѡ��״̬
				if (v_sType==v_sTemp && v_sTemp==v_pLTE_PM_Type->first)	//�������Ͳ�һ�£�����
					m_cList.SetItemBkColor(v_iNum,RGB(255,255,255));	//���������ɫ
				break;
			}

			v_pPM_Name++;												//�ݼ�
			v_iNum++;													//������
		}
		v_pLTE_PM_Type++;												//�ݼ�
	}

	//��ʾ������
	v_sTemp.Format("%d",m_cList_PM.GetItemCount());						//����
	m_cSum.SetWindowText(v_sTemp);										//��ʾ������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ�е�����
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::My_PM_Insert(CString v_sType,CString v_sPM_Name)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp;												//��ʱ����

	int		v_iItem = m_cList_PM.InsertItem(0xffff,_T("0"));
	v_sTemp.Format("%04d",v_iItem+1);
	m_cList_PM.SetItemText(v_iItem,0," "+v_sTemp);						//���
	m_cList_PM.SetItemText(v_iItem,1,v_sType);							//��������
	m_cList_PM.SetItemText(v_iItem,2,v_sPM_Name);						//��������
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CLTE_PM_Draw_Select::My_ExecuteSQL(CString v_sSql)
{
	// TODO: Add your control notification handler code here
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);

		v_cAdoConn.ExecuteSQL((_bstr_t)v_sSql);							//ִ�����ݿ����

		v_cAdoConn.ExitConnect();										//�Ͽ�����
		return true;
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�ر����ݿ�

		CString v_sError = (char *)e.Description();						//��ã�������Ϣ
		MessageBox(v_sError);											//ʧ��
		return false;
	}
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
