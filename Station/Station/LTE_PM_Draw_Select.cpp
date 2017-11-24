// LTE_PM_Draw_Select.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Draw_Select.h"


// CLTE_PM_Draw_Select 对话框

IMPLEMENT_DYNAMIC(CLTE_PM_Draw_Select, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Draw_Select::CLTE_PM_Draw_Select(CWnd* pParent /*=NULL*/)
	: CDialog(CLTE_PM_Draw_Select::IDD, pParent)
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Draw_Select::~CLTE_PM_Draw_Select()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
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
//	函数功能：屏幕消息映射
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


// CLTE_PM_Draw_Select 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CLTE_PM_Draw_Select::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化：列表窗 【待选性能】
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,40);					//
	m_cList.InsertColumn(1,"性能",LVCFMT_LEFT,280);						//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//初始化：列表窗 【已选性能】
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"序号",LVCFMT_CENTER,40);					//
	m_cList_PM.InsertColumn(1,"类型",LVCFMT_LEFT,100);					//
	m_cList_PM.InsertColumn(2,"性能",LVCFMT_LEFT,280);					//
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//初始化
	v_iList_Item=-1;													//列表选项：无效
	v_iList_Item_OK=-1;													//列表选项：无效

	//网络类型
	CString					v_sTemp;									//临时变量
	mLTE_PM::iterator		v_pPM_Name;									//迭代器
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器

	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//首指针
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//遍历
	{
		m_cType.AddString(v_pLTE_PM_Type->first);						//性能类型

		v_pLTE_PM_Type++;												//递增
	}
	m_cType.SetCurSel(v_iSelect_PM_Type);								//默认：第一个

	//显示：默认选中的性能
	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//首指针
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//结束？？
	{
		v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();			//首指针
		while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//结束？？
		{
			if (v_pPM_Name->second.v_bOK)
				My_PM_Insert(v_pLTE_PM_Type->first,v_pPM_Name->second.v_sPM_Name);	//增加：选中条目

			v_pPM_Name++;												//递加
		}
		v_pLTE_PM_Type++;												//递加
	}

	//初始化：ListCtrl拖动处理
	m_bDragging = false;												//标志：拖动无效
	m_nDragIndex = -1;													//拖动源
	m_nDropIndex = -1;													//拖动目标
	m_pDragImage = NULL;												//拖动图片

	OnCbnCloseupCombo7();												//性能类型选择

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Draw_Select::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		CDialog* Pwnd=(CDialog*)GetActiveWindow();						//取得对话框指针 
		Pwnd->NextDlgCtrl();											//切换到下一个输入焦点 
		return true;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：调整窗体大小
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：关闭窗体
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnClose();													//退出	
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能主窗开始拖动
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnLvnBegindragListMaster(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
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
	bitmap.LoadBitmap(IDB_PM_Check);									//取消图片
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
//	函数功能：性能从窗开始拖动
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnLvnBegindragListSlave(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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
	bitmap.LoadBitmap(IDB_PM_UnCheck);									//取消图片
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
//	函数功能：鼠标移动操作【列表内，移动图像处理；列表外，严禁图像；】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// While the mouse is moving, this routine is called.
	// This routine will redraw the drag image at the present mouse location to display the dragging.
	// Also, while over a CListCtrl, this routine will highlight the item we are hovering over.

	// If we are in a drag/drop procedure (m_bDragging is true)
	if (m_bDragging)													//拖动进行？？？
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
//	函数功能：左键释放【拖动结束处理：数据处理】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
//	函数功能：拖动结束处理【数据处理】
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
	if(pDragList->GetSelectedCount() == 1)								//选中1条
	{
		if(pDragList != pDropList)										//列表源!=列表目标
		{
			if (m_pDragList==&m_cList)									//增加性能
			{
				v_iList_Item=m_nDragIndex;								//性能序号
				OnBnClickedButton1();									//增加性能
			}
			else														//删除性能
			{
				v_iList_Item_OK=m_nDragIndex;							//性能序号
				OnBnClickedButton3();									//删除性能
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
//	函数功能：性能类型选择
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnCbnCloseupCombo7()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp,v_sType;										//临时变量
	int			v_iNum=0,v_iItem;										//临时变量
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	//删除列表信息
	m_cList.DeleteAllItems();											//删除所有条目
	v_iList_Item=-1;													//列表选项：无效

	//加入性能名称
	m_cType.GetWindowText(v_sType);										//获得：性能类型
	v_pLTE_PM_Type=v_mLTE_PM_Type->find(v_sType);						//查找：性能类型
	v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();				//首指针
	while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())			//结束？？
	{
		v_sTemp.Format(" %04d",v_iNum+1);
		v_iItem=m_cList.InsertItem(0xffff,v_sTemp);						//加入列表窗
		m_cList.SetItemText(v_iItem,1,(_bstr_t)v_pPM_Name->second.v_sPM_Name);	//性能名称
		if (v_pPM_Name->second.v_bOK)
			m_cList.SetItemBkColor(v_iItem,RGB(0,255,0));				//设置：背景色【选中】

		v_pPM_Name++;													//递加
		v_iNum++;														//计数器
	}

	//显示：数量
	v_sTemp.Format("%d",m_cList_PM.GetItemCount());						//数量
	m_cSum.SetWindowText(v_sTemp);										//显示：数量
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击列表【选择性能】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针

	v_iList_Item=pNMListView->iItem;									//获得：选中条目序号

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击列表【选中性能】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针

	v_iList_Item_OK=pNMListView->iItem;									//获得：选中条目序号

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：选定退出
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：增加选中的性能
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sPM_Name,v_sType,v_sTemp;								//临时变量
	int			v_iNum=0;												//临时变量
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	if (v_iList_Item==-1)
	{
		MessageBox("  请选择有效的性能 !!! ");							//窗体提示
		return;
	}

	//查找性能：依据选中的性能名称，若未选中状态，则增加选中Item；
	m_cType.GetWindowText(v_sType);										//获得：性能类型
	v_pLTE_PM_Type=v_mLTE_PM_Type->find(v_sType);						//查找：性能类型
	v_sPM_Name=m_cList.GetItemText(v_iList_Item,1);						//获取：性能名称
	v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();				//首指针
	while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())			//结束？？
	{
		if(v_pPM_Name->second.v_sPM_Name == v_sPM_Name)					//性能名称相等？？？
		{
			if (!v_pPM_Name->second.v_bOK)								//未选中状态？？
			{
				v_pPM_Name->second.v_bOK=1;								//置位：选中状态
				m_cList.SetItemBkColor(v_iList_Item,RGB(0,255,0));		//设置：背景色【选中】
				My_PM_Insert(v_sType,v_sPM_Name);						//增加：选中条目
			}
			break;
		}

		v_pPM_Name++;													//递加
		v_iNum++;														//计数器
	}

	//显示：数量
	v_sTemp.Format("%d",m_cList_PM.GetItemCount());						//数量
	m_cSum.SetWindowText(v_sTemp);										//显示：数量
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：删除选中的性能
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sPM_Name,v_sType,v_sTemp;								//临时变量
	int			v_iNum;													//临时变量
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	if (v_iList_Item_OK==-1)
	{
		MessageBox("  请选择有效的性能 !!! ");							//窗体提示
		return;
	}

	//删除选中的Item
	v_sTemp=m_cList_PM.GetItemText(v_iList_Item_OK,1);					//获取：性能类型
	v_sPM_Name=m_cList_PM.GetItemText(v_iList_Item_OK,2);				//获取：性能名称
	m_cList_PM.DeleteItem(v_iList_Item_OK);								//删除选中的Item

	//查找性能：依据选中的性能名称，设置选中Item为未选中状态；
	m_cType.GetWindowText(v_sType);										//获得：性能类型

	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//首指针
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//结束？？
	{
		v_iNum=0;														//复位：计数器
		v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();			//首指针
		while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//结束？？
		{
			if(v_pPM_Name->second.v_sPM_Name == v_sPM_Name)				//性能名称相等？？？
			{
				v_pPM_Name->second.v_bOK=0;								//置位：未选中状态
				if (v_sType==v_sTemp && v_sTemp==v_pLTE_PM_Type->first)	//性能类型不一致？？？
					m_cList.SetItemBkColor(v_iNum,RGB(255,255,255));	//清除：背景色
				break;
			}

			v_pPM_Name++;												//递加
			v_iNum++;													//计数器
		}
		v_pLTE_PM_Type++;												//递加
	}

	//显示：数量
	v_sTemp.Format("%d",m_cList_PM.GetItemCount());						//数量
	m_cSum.SetWindowText(v_sTemp);										//显示：数量
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：增加选中的性能
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Select::My_PM_Insert(CString v_sType,CString v_sPM_Name)
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp;												//临时变量

	int		v_iItem = m_cList_PM.InsertItem(0xffff,_T("0"));
	v_sTemp.Format("%04d",v_iItem+1);
	m_cList_PM.SetItemText(v_iItem,0," "+v_sTemp);						//序号
	m_cList_PM.SetItemText(v_iItem,1,v_sType);							//性能类型
	m_cList_PM.SetItemText(v_iItem,2,v_sPM_Name);						//性能名称
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CLTE_PM_Draw_Select::My_ExecuteSQL(CString v_sSql)
{
	// TODO: Add your control notification handler code here
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);

		v_cAdoConn.ExecuteSQL((_bstr_t)v_sSql);							//执行数据库操作

		v_cAdoConn.ExitConnect();										//断开连接
		return true;
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//关闭数据库

		CString v_sError = (char *)e.Description();						//获得：出错信息
		MessageBox(v_sError);											//失败
		return false;
	}
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
