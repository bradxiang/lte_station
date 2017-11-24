// LTE_PM_Draw_Znit_Select.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Draw_Znit_Select.h"


// CLTE_PM_Draw_Znit_Select 对话框

IMPLEMENT_DYNAMIC(CLTE_PM_Draw_Znit_Select, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Draw_Znit_Select::CLTE_PM_Draw_Znit_Select(CWnd* pParent /*=NULL*/)
	: CDialog(CLTE_PM_Draw_Znit_Select::IDD, pParent)
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Draw_Znit_Select::~CLTE_PM_Draw_Znit_Select()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_LIST3, m_cList_PM);
	DDX_Control(pDX, IDC_LOG_SUM, m_cSum);
	DDX_Control(pDX, IDC_CHECK2, m_cOrder);
	DDX_Control(pDX, IDC_KEY, m_cPM_Case);
	DDX_Control(pDX, IDC_CHECK5, m_cShare);
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Draw_Znit_Select, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Draw_Znit_Select::OnBnClickedButton1)
	ON_CBN_CLOSEUP(IDC_COMBO7, &CLTE_PM_Draw_Znit_Select::OnCbnCloseupCombo7)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Draw_Znit_Select::OnNMClickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CLTE_PM_Draw_Znit_Select::OnNMClickList3)
	ON_BN_CLICKED(IDC_BUTTON3, &CLTE_PM_Draw_Znit_Select::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CLTE_PM_Draw_Znit_Select::OnBnClickedButton2)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, &CLTE_PM_Draw_Znit_Select::OnLvnBegindragList1)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST3, &CLTE_PM_Draw_Znit_Select::OnLvnBegindragList3)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON5, &CLTE_PM_Draw_Znit_Select::OnBnClickedButton5)
END_MESSAGE_MAP()


// CLTE_PM_Draw_Znit_Select 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CLTE_PM_Draw_Znit_Select::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化：列表窗 【待选性能】
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,40);					//
	m_cList.InsertColumn(1,"性能",LVCFMT_LEFT,240);						//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//初始化：列表窗 【已选性能】
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"序号",LVCFMT_CENTER,40);					//
	m_cList_PM.InsertColumn(1,"类型",LVCFMT_LEFT,75);					//
	m_cList_PM.InsertColumn(2,"性能",LVCFMT_LEFT,210);					//
	m_cList_PM.InsertColumn(3,"预警",LVCFMT_CENTER,50);					//
	m_cList_PM.InsertColumn(4,"预警门限",LVCFMT_RIGHT,70);				//
	m_cList_PM.InsertColumn(5,"类型",LVCFMT_CENTER,40);					//
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
	m_cType.SetCurSel(0);												//默认：第一个

	//显示：默认选中的性能
	LTE_PM_Struct			v_cPM_Data;									//结构
	map<int,LTE_PM_Struct>	v_mPM_Data;									//容器
	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//首指针
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//结束？？
	{
		v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();			//首指针
		while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//结束？？
		{
			if (v_pPM_Name->second.v_bOK)
			{
				v_cPM_Data.v_sType = v_pLTE_PM_Type->first;				//性能类型
				v_cPM_Data.v_sPM_Name = v_pPM_Name->second.v_sPM_Name;	//性能名称
				v_cPM_Data.v_sAlarm_Flag=v_pPM_Name->second.v_sAlarm_Flag;		//预警标志
				v_cPM_Data.v_dAlarm_Value=v_pPM_Name->second.v_dAlarm_Value;	//预警门限
				v_cPM_Data.v_sAlarm_Type=v_pPM_Name->second.v_sAlarm_Type;		//预警类型
				v_mPM_Data.insert(pair<int,LTE_PM_Struct>(v_pPM_Name->second.v_iNum,v_cPM_Data));	//增加：选中条目
			}
			v_pPM_Name++;												//递加
		}
		v_pLTE_PM_Type++;												//递加
	}
	map<int,LTE_PM_Struct>::iterator	v_pPM_Data;						//迭代器
	for (v_pPM_Data=v_mPM_Data.begin();v_pPM_Data!=v_mPM_Data.end();v_pPM_Data++)
	{
		int	v_iItem = m_cList_PM.InsertItem(0xffff,_T("0"));
		v_sTemp.Format("%04d",v_iItem+1);
		m_cList_PM.SetItemText(v_iItem,0," "+v_sTemp);					//序号
		m_cList_PM.SetItemText(v_iItem,1,v_pPM_Data->second.v_sType);	//性能类型
		m_cList_PM.SetItemText(v_iItem,2,v_pPM_Data->second.v_sPM_Name);//性能名称
		m_cList_PM.SetItemText(v_iItem,3,v_pPM_Data->second.v_sAlarm_Flag);	//预警标志
		v_sTemp.Format("%8.2f",v_pPM_Data->second.v_dAlarm_Value);
		m_cList_PM.SetItemText(v_iItem,4,v_sTemp);						//预警门限
		m_cList_PM.SetItemText(v_iItem,5,v_pPM_Data->second.v_sAlarm_Type);	//预警类型
	}
	v_mPM_Data.clear();													//容器清除

	//初始化：ListCtrl拖动处理
	m_bDragging = false;												//标志：拖动无效
	m_nDragIndex = -1;													//拖动源
	m_nDropIndex = -1;													//拖动目标
	m_pDragImage = NULL;												//拖动图片

	OnCbnCloseupCombo7();												//性能类型选择

	return TRUE;														// return TRUE unless you set the focus to a control
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Draw_Znit_Select::PreTranslateMessage(MSG* pMsg)
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
void CLTE_PM_Draw_Znit_Select::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：关闭窗体
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnBnClickedButton2();												//确认退出
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能主窗开始拖动
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::OnLvnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult)
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
void CLTE_PM_Draw_Znit_Select::OnLvnBegindragList3(NMHDR *pNMHDR, LRESULT *pResult)
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
void CLTE_PM_Draw_Znit_Select::OnMouseMove(UINT nFlags, CPoint point)
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
void CLTE_PM_Draw_Znit_Select::OnLButtonUp(UINT nFlags, CPoint point)
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
//	函数功能：拖动结束处理【1：性能增加；2：性能删除；3：选中性能调整顺序】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList)
{
	CString		v_sType,v_sPM_Name,v_sTemp;								//临时变量

	//单个选择
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
		else if (m_pDragList==&m_cList_PM)								//选中性能移动
		{
			//选中性能移动顺序
			if (m_nDropIndex!=m_nDragIndex && m_cOrder.GetCheck())		//目标与源信息不一致 && 顺序调整？？？
			{
				v_sType=m_cList_PM.GetItemText(m_nDragIndex,1);			//类型
				v_sPM_Name=m_cList_PM.GetItemText(m_nDragIndex,2);		//性能
				if (m_nDropIndex<0)										//
					m_nDropIndex=0xffff;								//
				m_nDropIndex = m_cList_PM.InsertItem(m_nDropIndex,_T("0"));
				v_sTemp.Format("%04d",m_nDropIndex+1);
				m_cList_PM.SetItemText(m_nDropIndex,0," "+v_sTemp);		//序号
				m_cList_PM.SetItemText(m_nDropIndex,1,v_sType);			//性能类型
				m_cList_PM.SetItemText(m_nDropIndex,2,v_sPM_Name);		//性能名称

				if (m_nDropIndex<m_nDragIndex)							//目标<源信息
					m_nDragIndex++;										//源信息++
				else
					m_nDropIndex--;										//目标
				m_cList_PM.DeleteItem(m_nDragIndex);					//
			}
			// Select the new item we just inserted
			m_cList_PM.SetItemState(m_nDropIndex, LVIS_SELECTED, LVIS_SELECTED);

			//调整序号
			for (int vi=0;vi<m_cList_PM.GetItemCount();vi++)			//遍历
			{
				v_sTemp.Format("%04d",vi+1);
				m_cList_PM.SetItemText(vi,0," "+v_sTemp);				//序号
			}
		}
	}
	else
	{
		if(pDragList != pDropList)										//列表源!=列表目标
		{
			if (m_pDragList==&m_cList)									//增加性能
			{															//从m_cList获取信息
				while ((v_iList_Item = m_cList.GetNextItem(-1, LVNI_SELECTED)) != -1)	//条目选中？？？
				{
					OnBnClickedButton1();								//增加性能
					m_cList.SetItemState(v_iList_Item,0,LVIS_SELECTED); 
				}
			}
			else
			{
				while ((v_iList_Item_OK = m_cList_PM.GetNextItem(-1, LVNI_SELECTED)) != -1)	//条目选中？？？
				{
					OnBnClickedButton3();								//删除性能
				}
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能类型选择
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::OnCbnCloseupCombo7()
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
void CLTE_PM_Draw_Znit_Select::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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
void CLTE_PM_Draw_Znit_Select::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
void CLTE_PM_Draw_Znit_Select::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sPM_Name,v_sType;										//临时变量
	CString		v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;				//临时变量

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	//【1】复位：初始化
	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//首指针
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//遍历
	{
		for(v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
			v_pPM_Name->second.v_bOK=0;									//复位：选中状态
		v_pLTE_PM_Type++;
	}

	//【2】获取：模板信息
	for(int vi=0;vi<m_cList_PM.GetItemCount();vi++)						//遍历：模板信息列表
	{
		if (m_cList.GetCheck(vi))
		{
			//更新：性能状态
			v_sType=m_cList_PM.GetItemText(vi,1);						//性能类型
			v_sPM_Name=m_cList_PM.GetItemText(vi,2);					//性能名称

			v_pLTE_PM_Type=v_mLTE_PM_Type->begin();						//首指针
			while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())				//遍历
			{
				for(v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
				{
					if(v_pLTE_PM_Type->first==v_sType && v_pPM_Name->second.v_sPM_Name==v_sPM_Name)		//相等：性能名称？？？
					{
						v_pPM_Name->second.v_bOK=1;						//置位：选中状态
						v_pPM_Name->second.v_iNum=vi;					//获取：列序号
						break;
					}
				}
				v_pLTE_PM_Type++;
			}
		}
	}

	//【3】退出
	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：增加选中的性能
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::OnBnClickedButton1()
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
				v_pPM_Name->second.v_iNum=m_cList_PM.GetItemCount();	//赋值：列序号
				v_pPM_Name->second.v_sAlarm_Flag="不预警";				//预警标志
				v_pPM_Name->second.v_dAlarm_Value=0;					//预警值
				v_pPM_Name->second.v_sAlarm_Type="低";					//预警类型
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
void CLTE_PM_Draw_Znit_Select::OnBnClickedButton3()
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
				if (v_sType==v_sTemp)									//性能类型不一致？？？
					m_cList.SetItemBkColor(v_iNum,RGB(255,255,255));	//清除：背景色
				break;
			}

			v_pPM_Name++;												//递加
			v_iNum++;													//计数器
		}
		v_pLTE_PM_Type++;												//递加
	}

	//调整序号
	for (int vi=0;vi<m_cList_PM.GetItemCount();vi++)					//遍历
	{
		v_sTemp.Format("%04d",vi+1);
		m_cList_PM.SetItemText(vi,0," "+v_sTemp);						//序号
	}

	//显示：数量
	v_sTemp.Format("%d",m_cList_PM.GetItemCount());						//数量
	m_cSum.SetWindowText(v_sTemp);										//显示：数量
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：增加选中的性能
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::My_PM_Insert(CString v_sType,CString v_sPM_Name)
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp;												//临时变量

	int		v_iItem = m_cList_PM.InsertItem(0xffff,_T("0"));
	v_sTemp.Format("%04d",v_iItem+1);
	m_cList_PM.SetItemText(v_iItem,0," "+v_sTemp);						//序号
	m_cList_PM.SetItemText(v_iItem,1,v_sType);							//性能类型
	m_cList_PM.SetItemText(v_iItem,2,v_sPM_Name);						//性能名称
	m_cList_PM.SetItemText(v_iItem,3,"不预警");							//预警标志
	m_cList_PM.SetItemText(v_iItem,4,"0");								//预警值
	m_cList_PM.SetItemText(v_iItem,5,"低");								//预警类型
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：模板保存
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	int			v_iLen,vi;												//临时变量
	CString		v_sSql,v_sTemp,v_sNew;									//临时变量
	CString		v_sAccount,v_sName,v_sType,v_sPM_Name,v_sNum;			//临时变量
	CString		v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;				//临时变量

	mString_Int					v_mCase;								//容器：模板
	mString_Int					v_mPM_Type;								//容器：类型+性能名称
	mString_Int::iterator		v_pIter;								//迭代器

	//【1】确认
	m_cPM_Case.GetWindowText(v_sName);									//获取：性能模板名称
	v_sName.Trim();														//去空格
	if (v_sName.IsEmpty())												//为空？？？
	{
		MessageBox("  请输入性能模板名称 !!! ");						//窗体提示
		return;															//无效返回	
	}

	v_iLen=m_cList_PM.GetItemCount();									//列表条目数量
	if (v_iLen<=0)
	{
		MessageBox("  请选择需要导入的性能 ！！！");					//返回：无数据
		return;
	}

	//【2】获取相关数据 [模板]
	My_Load_PM(&v_mCase,&v_mPM_Type);									//获取：信息 [放入：v_mPM]

	//【3】获取：选择的性能
	COleDateTime v_tTime=COleDateTime::GetCurrentTime();				//日期
	CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");			//
	v_sSql="";															//初始化
	v_sAccount=m_cShare.GetCheck()?"共享":theApp.a_sUser_Account;		//获取账号【个人/共享】

	//保存确认
	v_pIter=v_mCase.find(v_sAccount+v_sName);							//查找：账号+模板
	if (v_pIter != v_mCase.end())										//找到？？？	
	{																	//模板存在：提示修改确认，并删除原有模板性能(重新增加)
		if(MessageBox("性能模板： "+v_sAccount+"--"+v_sName+" 已存在，修改确认 ? ","性能模板保存确认",MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)	//保存确认？？？
			return;														//无效返回	
		v_sNew="delete from CON_Pm_LTE_Case_PM_Data where zAccount='"+v_sAccount+"' and zName='"+v_sName+"'";	//SQL
		v_sSql+=v_sNew+"\r\n";											//增加一条SQL
	}
	else
	{																	//模板不存在：增加模板
		if(MessageBox("确定把选中的性能保存到【 "+v_sAccount+"--"+v_sName+"】New 模板，保存确认 ? ","性能模板保存确认",MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)	//保存确认？？？
			return;														//无效返回	
		//增加模板
		v_sNew="Insert into CON_Pm_LTE_Case_PM(zAccount,zName,zDate) values ('"
			+v_sAccount+"','"+v_sName+"','"+v_sCurrentTime+"')";		//SQL
		v_sSql+=v_sNew+"\r\n";											//增加一条SQL
	}
	if(!v_sSql.IsEmpty())												//有SQL信息？？？ 【写数据表】
		My_ExecuteSQL(v_sSql);											//SQL执行

	//基于性能模板，增加性能信息；
	v_sSql="";															//初始化
	for(vi=0;vi<m_cList_PM.GetItemCount();vi++)							//遍历：模板信息列表
	{
		//获取：性能数据
		v_sNum=m_cList_PM.GetItemText(vi,0);							//序号
		v_sType=m_cList_PM.GetItemText(vi,1);							//类型
		v_sPM_Name=m_cList_PM.GetItemText(vi,2);						//性能
		v_sAlarm_Flag=m_cList_PM.GetItemText(vi,3);						//预警标志
		v_sAlarm_Flag=v_sAlarm_Flag=="预警"?"1":"0";
		v_sAlarm_Value=m_cList_PM.GetItemText(vi,4);					//预警门限
		v_sAlarm_Type=m_cList_PM.GetItemText(vi,5);						//预警类型
		v_sAlarm_Type=v_sAlarm_Type=="高"?"1":"0";

		v_pIter=v_mPM_Type.find(v_sType+v_sPM_Name);					//查找：模板
		if (v_pIter == v_mPM_Type.end())								//没找到？？？
		{																//异常性能类型及名称
			m_cList_PM.SetItemBkColor(vi,RGB(255,0,0));					//设置：背景色 [红色]			
			continue;
		}

		v_sNew="Insert into CON_Pm_LTE_Case_PM_Data(zAccount,zName,zType,zPM_Name,zAlarm_Flag,zAlarm_Value,zAlarm_Type,zNum,zDate) values ('"
				+v_sAccount+"','"+v_sName+"','"+v_sType+"','"+v_sPM_Name+"','"+v_sAlarm_Flag+"','"+v_sAlarm_Value+"','"
				+v_sAlarm_Type+"','"+v_sNum+"','"+v_sCurrentTime+"')";	//SQL
		v_sSql+=v_sNew+"\r\n";										//增加一条SQL
	}

	//【4】保存：性能模板
	if(!v_sSql.IsEmpty())												//有SQL信息？？？ 【写数据表】
		My_ExecuteSQL(v_sSql);											//SQL执行

	//【5】清除数据	
	v_mCase.clear();													//清除：容器
	v_mPM_Type.clear();													//清除：容器

	//【6】保存完毕
	MessageBox("  性能模板保存完毕 !!! ");								//窗体提示
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载信息【模板】
//	工	  作：把性能信息放入容器【v_mCase：性能模板；	v_mPM_Type：原始性能+计算性能】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw_Znit_Select::My_Load_PM(mString_Int *v_mCase,mString_Int *v_mPM_Type)
{
	CString			v_sSql;												//临时变量
	CString			v_sTemp,v_sAccount,v_sName,v_sType,v_sPM_Name;		//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//【1】获取：性能模板数据
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT zAccount,zName FROM CON_Pm_LTE_Case_PM where zAccount='共享' or zAccount='"+theApp.a_sUser_Account+"'";
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										
		while(!v_pRecordset->adoEOF)
		{
			v_sAccount = v_cAdoConn.GetField_String("zAccount");		//账号
			v_sName = v_cAdoConn.GetField_String("zName");				//模板

			v_mCase->insert(pair<CString,int>(v_sAccount+v_sName,0));	//容器：增加

			v_pRecordset->MoveNext();									//
		}

		//【2】获取：性能数据[原始]
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE order by ztype,ztype_1,zPm";//Sql
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										
		while(!v_pRecordset->adoEOF)
		{
			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sPM_Name = v_cAdoConn.GetField_String("zPm_Name");		//zPm_Name

			v_mPM_Type->insert(pair<CString,int>(v_sType+v_sPM_Name,0));	//容器：增加

			v_pRecordset->MoveNext();									//
		}

		//【3】获取：性能参数【计算性能】
		v_sType="计算性能";												//性能类型								
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Cal order by zPm";			//Sql
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										
		while(!v_pRecordset->adoEOF)
		{
			v_sPM_Name = v_cAdoConn.GetField_String("zPm");				//zPm

			v_mPM_Type->insert(pair<CString,int>(v_sType+v_sPM_Name,0));	//容器：增加

			v_pRecordset->MoveNext();									//
		}

		//【4】获取：性能参数【自定义性能】
		v_sType="自定义";												//性能类型								
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Case_Cal where zAccount='"+theApp.a_sUser_Name+"' order by zName ";	//Sql
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										
		while(!v_pRecordset->adoEOF)
		{
			v_sPM_Name = v_cAdoConn.GetField_String("zName");			//zPm_Name

			v_mPM_Type->insert(pair<CString,int>(v_sType+v_sPM_Name,0));	//容器：增加

			v_pRecordset->MoveNext();									//
		}

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CLTE_PM_Draw_Znit_Select::My_ExecuteSQL(CString v_sSql)
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
