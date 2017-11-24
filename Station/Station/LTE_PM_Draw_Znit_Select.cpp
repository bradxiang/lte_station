// LTE_PM_Draw_Znit_Select.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Draw_Znit_Select.h"


// CLTE_PM_Draw_Znit_Select �Ի���

IMPLEMENT_DYNAMIC(CLTE_PM_Draw_Znit_Select, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CLTE_PM_Draw_Znit_Select::CLTE_PM_Draw_Znit_Select(CWnd* pParent /*=NULL*/)
	: CDialog(CLTE_PM_Draw_Znit_Select::IDD, pParent)
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CLTE_PM_Draw_Znit_Select::~CLTE_PM_Draw_Znit_Select()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
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
//	�������ܣ���Ļ��Ϣӳ��
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


// CLTE_PM_Draw_Znit_Select ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CLTE_PM_Draw_Znit_Select::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ʼ�����б� ����ѡ���ܡ�
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"���",LVCFMT_CENTER,40);					//
	m_cList.InsertColumn(1,"����",LVCFMT_LEFT,240);						//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//��ʼ�����б� ����ѡ���ܡ�
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"���",LVCFMT_CENTER,40);					//
	m_cList_PM.InsertColumn(1,"����",LVCFMT_LEFT,75);					//
	m_cList_PM.InsertColumn(2,"����",LVCFMT_LEFT,210);					//
	m_cList_PM.InsertColumn(3,"Ԥ��",LVCFMT_CENTER,50);					//
	m_cList_PM.InsertColumn(4,"Ԥ������",LVCFMT_RIGHT,70);				//
	m_cList_PM.InsertColumn(5,"����",LVCFMT_CENTER,40);					//
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
	m_cType.SetCurSel(0);												//Ĭ�ϣ���һ��

	//��ʾ��Ĭ��ѡ�е�����
	LTE_PM_Struct			v_cPM_Data;									//�ṹ
	map<int,LTE_PM_Struct>	v_mPM_Data;									//����
	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//��ָ��
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//��������
	{
		v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();			//��ָ��
		while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//��������
		{
			if (v_pPM_Name->second.v_bOK)
			{
				v_cPM_Data.v_sType = v_pLTE_PM_Type->first;				//��������
				v_cPM_Data.v_sPM_Name = v_pPM_Name->second.v_sPM_Name;	//��������
				v_cPM_Data.v_sAlarm_Flag=v_pPM_Name->second.v_sAlarm_Flag;		//Ԥ����־
				v_cPM_Data.v_dAlarm_Value=v_pPM_Name->second.v_dAlarm_Value;	//Ԥ������
				v_cPM_Data.v_sAlarm_Type=v_pPM_Name->second.v_sAlarm_Type;		//Ԥ������
				v_mPM_Data.insert(pair<int,LTE_PM_Struct>(v_pPM_Name->second.v_iNum,v_cPM_Data));	//���ӣ�ѡ����Ŀ
			}
			v_pPM_Name++;												//�ݼ�
		}
		v_pLTE_PM_Type++;												//�ݼ�
	}
	map<int,LTE_PM_Struct>::iterator	v_pPM_Data;						//������
	for (v_pPM_Data=v_mPM_Data.begin();v_pPM_Data!=v_mPM_Data.end();v_pPM_Data++)
	{
		int	v_iItem = m_cList_PM.InsertItem(0xffff,_T("0"));
		v_sTemp.Format("%04d",v_iItem+1);
		m_cList_PM.SetItemText(v_iItem,0," "+v_sTemp);					//���
		m_cList_PM.SetItemText(v_iItem,1,v_pPM_Data->second.v_sType);	//��������
		m_cList_PM.SetItemText(v_iItem,2,v_pPM_Data->second.v_sPM_Name);//��������
		m_cList_PM.SetItemText(v_iItem,3,v_pPM_Data->second.v_sAlarm_Flag);	//Ԥ����־
		v_sTemp.Format("%8.2f",v_pPM_Data->second.v_dAlarm_Value);
		m_cList_PM.SetItemText(v_iItem,4,v_sTemp);						//Ԥ������
		m_cList_PM.SetItemText(v_iItem,5,v_pPM_Data->second.v_sAlarm_Type);	//Ԥ������
	}
	v_mPM_Data.clear();													//�������

	//��ʼ����ListCtrl�϶�����
	m_bDragging = false;												//��־���϶���Ч
	m_nDragIndex = -1;													//�϶�Դ
	m_nDropIndex = -1;													//�϶�Ŀ��
	m_pDragImage = NULL;												//�϶�ͼƬ

	OnCbnCloseupCombo7();												//��������ѡ��

	return TRUE;														// return TRUE unless you set the focus to a control
}

//------------------------------------------------------------------------------------------------------			
// ��ϢԤ�����ѻس����任ΪTAB
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Draw_Znit_Select::PreTranslateMessage(MSG* pMsg)
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
void CLTE_PM_Draw_Znit_Select::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��رմ���
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	OnBnClickedButton2();												//ȷ���˳�
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����������ʼ�϶�
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::OnLvnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult)
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
void CLTE_PM_Draw_Znit_Select::OnLvnBegindragList3(NMHDR *pNMHDR, LRESULT *pResult)
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
void CLTE_PM_Draw_Znit_Select::OnMouseMove(UINT nFlags, CPoint point)
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
void CLTE_PM_Draw_Znit_Select::OnLButtonUp(UINT nFlags, CPoint point)
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
//	�������ܣ��϶���������1���������ӣ�2������ɾ����3��ѡ�����ܵ���˳��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList)
{
	CString		v_sType,v_sPM_Name,v_sTemp;								//��ʱ����

	//����ѡ��
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
		else if (m_pDragList==&m_cList_PM)								//ѡ�������ƶ�
		{
			//ѡ�������ƶ�˳��
			if (m_nDropIndex!=m_nDragIndex && m_cOrder.GetCheck())		//Ŀ����Դ��Ϣ��һ�� && ˳�����������
			{
				v_sType=m_cList_PM.GetItemText(m_nDragIndex,1);			//����
				v_sPM_Name=m_cList_PM.GetItemText(m_nDragIndex,2);		//����
				if (m_nDropIndex<0)										//
					m_nDropIndex=0xffff;								//
				m_nDropIndex = m_cList_PM.InsertItem(m_nDropIndex,_T("0"));
				v_sTemp.Format("%04d",m_nDropIndex+1);
				m_cList_PM.SetItemText(m_nDropIndex,0," "+v_sTemp);		//���
				m_cList_PM.SetItemText(m_nDropIndex,1,v_sType);			//��������
				m_cList_PM.SetItemText(m_nDropIndex,2,v_sPM_Name);		//��������

				if (m_nDropIndex<m_nDragIndex)							//Ŀ��<Դ��Ϣ
					m_nDragIndex++;										//Դ��Ϣ++
				else
					m_nDropIndex--;										//Ŀ��
				m_cList_PM.DeleteItem(m_nDragIndex);					//
			}
			// Select the new item we just inserted
			m_cList_PM.SetItemState(m_nDropIndex, LVIS_SELECTED, LVIS_SELECTED);

			//�������
			for (int vi=0;vi<m_cList_PM.GetItemCount();vi++)			//����
			{
				v_sTemp.Format("%04d",vi+1);
				m_cList_PM.SetItemText(vi,0," "+v_sTemp);				//���
			}
		}
	}
	else
	{
		if(pDragList != pDropList)										//�б�Դ!=�б�Ŀ��
		{
			if (m_pDragList==&m_cList)									//��������
			{															//��m_cList��ȡ��Ϣ
				while ((v_iList_Item = m_cList.GetNextItem(-1, LVNI_SELECTED)) != -1)	//��Ŀѡ�У�����
				{
					OnBnClickedButton1();								//��������
					m_cList.SetItemState(v_iList_Item,0,LVIS_SELECTED); 
				}
			}
			else
			{
				while ((v_iList_Item_OK = m_cList_PM.GetNextItem(-1, LVNI_SELECTED)) != -1)	//��Ŀѡ�У�����
				{
					OnBnClickedButton3();								//ɾ������
				}
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������ѡ��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::OnCbnCloseupCombo7()
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
void CLTE_PM_Draw_Znit_Select::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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
void CLTE_PM_Draw_Znit_Select::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
void CLTE_PM_Draw_Znit_Select::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sPM_Name,v_sType;										//��ʱ����
	CString		v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;				//��ʱ����

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//������
	mLTE_PM::iterator		v_pPM_Name;									//������

	//��1����λ����ʼ��
	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//��ָ��
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//����
	{
		for(v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
			v_pPM_Name->second.v_bOK=0;									//��λ��ѡ��״̬
		v_pLTE_PM_Type++;
	}

	//��2����ȡ��ģ����Ϣ
	for(int vi=0;vi<m_cList_PM.GetItemCount();vi++)						//������ģ����Ϣ�б�
	{
		if (m_cList.GetCheck(vi))
		{
			//���£�����״̬
			v_sType=m_cList_PM.GetItemText(vi,1);						//��������
			v_sPM_Name=m_cList_PM.GetItemText(vi,2);					//��������

			v_pLTE_PM_Type=v_mLTE_PM_Type->begin();						//��ָ��
			while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())				//����
			{
				for(v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
				{
					if(v_pLTE_PM_Type->first==v_sType && v_pPM_Name->second.v_sPM_Name==v_sPM_Name)		//��ȣ��������ƣ�����
					{
						v_pPM_Name->second.v_bOK=1;						//��λ��ѡ��״̬
						v_pPM_Name->second.v_iNum=vi;					//��ȡ�������
						break;
					}
				}
				v_pLTE_PM_Type++;
			}
		}
	}

	//��3���˳�
	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ�е�����
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::OnBnClickedButton1()
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
				v_pPM_Name->second.v_iNum=m_cList_PM.GetItemCount();	//��ֵ�������
				v_pPM_Name->second.v_sAlarm_Flag="��Ԥ��";				//Ԥ����־
				v_pPM_Name->second.v_dAlarm_Value=0;					//Ԥ��ֵ
				v_pPM_Name->second.v_sAlarm_Type="��";					//Ԥ������
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
void CLTE_PM_Draw_Znit_Select::OnBnClickedButton3()
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
				if (v_sType==v_sTemp)									//�������Ͳ�һ�£�����
					m_cList.SetItemBkColor(v_iNum,RGB(255,255,255));	//���������ɫ
				break;
			}

			v_pPM_Name++;												//�ݼ�
			v_iNum++;													//������
		}
		v_pLTE_PM_Type++;												//�ݼ�
	}

	//�������
	for (int vi=0;vi<m_cList_PM.GetItemCount();vi++)					//����
	{
		v_sTemp.Format("%04d",vi+1);
		m_cList_PM.SetItemText(vi,0," "+v_sTemp);						//���
	}

	//��ʾ������
	v_sTemp.Format("%d",m_cList_PM.GetItemCount());						//����
	m_cSum.SetWindowText(v_sTemp);										//��ʾ������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�����ѡ�е�����
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::My_PM_Insert(CString v_sType,CString v_sPM_Name)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		v_sTemp;												//��ʱ����

	int		v_iItem = m_cList_PM.InsertItem(0xffff,_T("0"));
	v_sTemp.Format("%04d",v_iItem+1);
	m_cList_PM.SetItemText(v_iItem,0," "+v_sTemp);						//���
	m_cList_PM.SetItemText(v_iItem,1,v_sType);							//��������
	m_cList_PM.SetItemText(v_iItem,2,v_sPM_Name);						//��������
	m_cList_PM.SetItemText(v_iItem,3,"��Ԥ��");							//Ԥ����־
	m_cList_PM.SetItemText(v_iItem,4,"0");								//Ԥ��ֵ
	m_cList_PM.SetItemText(v_iItem,5,"��");								//Ԥ������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�ģ�屣��
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Draw_Znit_Select::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int			v_iLen,vi;												//��ʱ����
	CString		v_sSql,v_sTemp,v_sNew;									//��ʱ����
	CString		v_sAccount,v_sName,v_sType,v_sPM_Name,v_sNum;			//��ʱ����
	CString		v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;				//��ʱ����

	mString_Int					v_mCase;								//������ģ��
	mString_Int					v_mPM_Type;								//����������+��������
	mString_Int::iterator		v_pIter;								//������

	//��1��ȷ��
	m_cPM_Case.GetWindowText(v_sName);									//��ȡ������ģ������
	v_sName.Trim();														//ȥ�ո�
	if (v_sName.IsEmpty())												//Ϊ�գ�����
	{
		MessageBox("  ����������ģ������ !!! ");						//������ʾ
		return;															//��Ч����	
	}

	v_iLen=m_cList_PM.GetItemCount();									//�б���Ŀ����
	if (v_iLen<=0)
	{
		MessageBox("  ��ѡ����Ҫ��������� ������");					//���أ�������
		return;
	}

	//��2����ȡ������� [ģ��]
	My_Load_PM(&v_mCase,&v_mPM_Type);									//��ȡ����Ϣ [���룺v_mPM]

	//��3����ȡ��ѡ�������
	COleDateTime v_tTime=COleDateTime::GetCurrentTime();				//����
	CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");			//
	v_sSql="";															//��ʼ��
	v_sAccount=m_cShare.GetCheck()?"����":theApp.a_sUser_Account;		//��ȡ�˺š�����/����

	//����ȷ��
	v_pIter=v_mCase.find(v_sAccount+v_sName);							//���ң��˺�+ģ��
	if (v_pIter != v_mCase.end())										//�ҵ�������	
	{																	//ģ����ڣ���ʾ�޸�ȷ�ϣ���ɾ��ԭ��ģ������(��������)
		if(MessageBox("����ģ�壺 "+v_sAccount+"--"+v_sName+" �Ѵ��ڣ��޸�ȷ�� ? ","����ģ�屣��ȷ��",MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)	//����ȷ�ϣ�����
			return;														//��Ч����	
		v_sNew="delete from CON_Pm_LTE_Case_PM_Data where zAccount='"+v_sAccount+"' and zName='"+v_sName+"'";	//SQL
		v_sSql+=v_sNew+"\r\n";											//����һ��SQL
	}
	else
	{																	//ģ�岻���ڣ�����ģ��
		if(MessageBox("ȷ����ѡ�е����ܱ��浽�� "+v_sAccount+"--"+v_sName+"��New ģ�壬����ȷ�� ? ","����ģ�屣��ȷ��",MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)	//����ȷ�ϣ�����
			return;														//��Ч����	
		//����ģ��
		v_sNew="Insert into CON_Pm_LTE_Case_PM(zAccount,zName,zDate) values ('"
			+v_sAccount+"','"+v_sName+"','"+v_sCurrentTime+"')";		//SQL
		v_sSql+=v_sNew+"\r\n";											//����һ��SQL
	}
	if(!v_sSql.IsEmpty())												//��SQL��Ϣ������ ��д���ݱ�
		My_ExecuteSQL(v_sSql);											//SQLִ��

	//��������ģ�壬����������Ϣ��
	v_sSql="";															//��ʼ��
	for(vi=0;vi<m_cList_PM.GetItemCount();vi++)							//������ģ����Ϣ�б�
	{
		//��ȡ����������
		v_sNum=m_cList_PM.GetItemText(vi,0);							//���
		v_sType=m_cList_PM.GetItemText(vi,1);							//����
		v_sPM_Name=m_cList_PM.GetItemText(vi,2);						//����
		v_sAlarm_Flag=m_cList_PM.GetItemText(vi,3);						//Ԥ����־
		v_sAlarm_Flag=v_sAlarm_Flag=="Ԥ��"?"1":"0";
		v_sAlarm_Value=m_cList_PM.GetItemText(vi,4);					//Ԥ������
		v_sAlarm_Type=m_cList_PM.GetItemText(vi,5);						//Ԥ������
		v_sAlarm_Type=v_sAlarm_Type=="��"?"1":"0";

		v_pIter=v_mPM_Type.find(v_sType+v_sPM_Name);					//���ң�ģ��
		if (v_pIter == v_mPM_Type.end())								//û�ҵ�������
		{																//�쳣�������ͼ�����
			m_cList_PM.SetItemBkColor(vi,RGB(255,0,0));					//���ã�����ɫ [��ɫ]			
			continue;
		}

		v_sNew="Insert into CON_Pm_LTE_Case_PM_Data(zAccount,zName,zType,zPM_Name,zAlarm_Flag,zAlarm_Value,zAlarm_Type,zNum,zDate) values ('"
				+v_sAccount+"','"+v_sName+"','"+v_sType+"','"+v_sPM_Name+"','"+v_sAlarm_Flag+"','"+v_sAlarm_Value+"','"
				+v_sAlarm_Type+"','"+v_sNum+"','"+v_sCurrentTime+"')";	//SQL
		v_sSql+=v_sNew+"\r\n";										//����һ��SQL
	}

	//��4�����棺����ģ��
	if(!v_sSql.IsEmpty())												//��SQL��Ϣ������ ��д���ݱ�
		My_ExecuteSQL(v_sSql);											//SQLִ��

	//��5���������	
	v_mCase.clear();													//���������
	v_mPM_Type.clear();													//���������

	//��6���������
	MessageBox("  ����ģ�屣����� !!! ");								//������ʾ
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ�װ����Ϣ��ģ�塿
//	��	  ������������Ϣ����������v_mCase������ģ�壻	v_mPM_Type��ԭʼ����+�������ܡ�
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Draw_Znit_Select::My_Load_PM(mString_Int *v_mCase,mString_Int *v_mPM_Type)
{
	CString			v_sSql;												//��ʱ����
	CString			v_sTemp,v_sAccount,v_sName,v_sType,v_sPM_Name;		//��ʱ����

	_RecordsetPtr v_pRecordset;											//��¼��
	CMyADOConn v_cAdoConn;												//���ݿ�ʵ��
	try
	{
		// �������ݿ�
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//��1����ȡ������ģ������
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT zAccount,zName FROM CON_Pm_LTE_Case_PM where zAccount='����' or zAccount='"+theApp.a_sUser_Account+"'";
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										
		while(!v_pRecordset->adoEOF)
		{
			v_sAccount = v_cAdoConn.GetField_String("zAccount");		//�˺�
			v_sName = v_cAdoConn.GetField_String("zName");				//ģ��

			v_mCase->insert(pair<CString,int>(v_sAccount+v_sName,0));	//����������

			v_pRecordset->MoveNext();									//
		}

		//��2����ȡ����������[ԭʼ]
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE order by ztype,ztype_1,zPm";//Sql
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										
		while(!v_pRecordset->adoEOF)
		{
			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sPM_Name = v_cAdoConn.GetField_String("zPm_Name");		//zPm_Name

			v_mPM_Type->insert(pair<CString,int>(v_sType+v_sPM_Name,0));	//����������

			v_pRecordset->MoveNext();									//
		}

		//��3����ȡ�����ܲ������������ܡ�
		v_sType="��������";												//��������								
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Cal order by zPm";			//Sql
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										
		while(!v_pRecordset->adoEOF)
		{
			v_sPM_Name = v_cAdoConn.GetField_String("zPm");				//zPm

			v_mPM_Type->insert(pair<CString,int>(v_sType+v_sPM_Name,0));	//����������

			v_pRecordset->MoveNext();									//
		}

		//��4����ȡ�����ܲ������Զ������ܡ�
		v_sType="�Զ���";												//��������								
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Case_Cal where zAccount='"+theApp.a_sUser_Name+"' order by zName ";	//Sql
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//ִ��SELETE���										
		while(!v_pRecordset->adoEOF)
		{
			v_sPM_Name = v_cAdoConn.GetField_String("zName");			//zPm_Name

			v_mPM_Type->insert(pair<CString,int>(v_sType+v_sPM_Name,0));	//����������

			v_pRecordset->MoveNext();									//
		}

		v_cAdoConn.ExitConnect();										//�Ͽ�����
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//�Ͽ�����
		v_sTemp = (char *)e.Description();								//������Ϣ
		MessageBox(v_sTemp);											//������ʾ
	}
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�SQLִ�к���
//	������v_sSql��SQL���
//------------------------------------------------------------------------------------------------------
bool CLTE_PM_Draw_Znit_Select::My_ExecuteSQL(CString v_sSql)
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
