// CMyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "comdef.h" 
#include "MyListCtrl.h"
#include "Station.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_EDIT  0X01
#define PROPERTY_ITEM 0x02
#define PROPERTY_SUB  0x03

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	类定义：CMyListCtrl
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//静态变量初始化
CMyListCtrl* CMyListCtrl::v_pList = NULL;										//赋值：空

//-------------------------------------------------------------------------------------------------
//	构造函数
//-------------------------------------------------------------------------------------------------
CMyListCtrl::CMyListCtrl()
{
	m_edit.m_hWnd	= NULL;														//默认：编辑句柄为NULL
	m_bAsc			= true;														//默认：升序
	m_nSortedCol	= 0;														//默认：第0列排序
	m_iOldSortedCol	= -1;														//默认：无上次列header
	crWindow		= ::GetSysColor(COLOR_WINDOW);								//默认：背景颜色
	crWindowText	= ::GetSysColor(COLOR_WINDOWFRAME);							//默认：文本颜色
	m_iCount=0;																	//默认：计数器=0
	v_bList_Show	= false;													//默认：数据未就绪，请等待；
	for(int vi=0;vi<200;vi++)
		m_bNumber[vi]=false;													//默认：列数字标记为字符；

}

//-------------------------------------------------------------------------------------------------
//	析构函数
//-------------------------------------------------------------------------------------------------
CMyListCtrl::~CMyListCtrl()
{
	//复位：背景色
	MapItemColor.clear();														//清除文字颜色映射键值对
	MapItemBkColor.clear();														//清除背景颜色映射键值对

	//复位：编辑
	v_mEdit.clear();															//清除
	m_mItem_Edit.clear();														//清除

	//复位：虚拟列表容器
	lList_Data::iterator v_pIter;												//迭代器
	for (v_pIter=v_lList_Data.begin();v_pIter!=v_lList_Data.end();v_pIter++)	//遍历
		v_pIter->clear();														//清除容器
	v_lList_Data.clear();														//清除队列
	v_mList_Rec.clear();														//清除容器
}

//-------------------------------------------------------------------------------------------------
//	消息映射
//-------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PARENTNOTIFY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_EDIT_END,OnEditEnd)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	ON_NOTIFY_REFLECT(LVN_INSERTITEM, OnInsertitem)
END_MESSAGE_MAP()

//-------------------------------------------------------------------------------------------------
//	功能：左键双击
//	性能：获取单元格的内容(拷贝)
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CRect rcCtrl;																//
	LVHITTESTINFO lvhti;														//

	//[1]获取行、列
	lvhti.pt = point;															//
	int nItem = CListCtrl::SubItemHitTest(&lvhti);								//获取：行
	if(nItem == -1)
	   return;
	int nSubItem = lvhti.iSubItem;												//获取：列

	//[2]编辑处理
	CListCtrl::GetSubItemRect(nItem,nSubItem,LVIR_LABEL,rcCtrl);				//
    ShowEdit(TRUE,nItem,nSubItem,rcCtrl);										//
	CListCtrl::OnLButtonDblClk(nFlags, point);									//
}

//-------------------------------------------------------------------------------------------------
//	功能：显示编辑框
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::ShowEdit(BOOL bShow,int nItem,int nIndex,CRect rcCtrl)
{
    if(m_edit.m_hWnd == NULL)
	{
		m_edit.Create(ES_AUTOHSCROLL|WS_CHILD|ES_LEFT|ES_WANTRETURN|WS_BORDER,CRect(0,0,0,0),this,IDC_EDIT);
		m_edit.ShowWindow(SW_HIDE);

		CFont tpFont;
		tpFont.CreateStockObject(DEFAULT_GUI_FONT);
		m_edit.SetFont(&tpFont);
		tpFont.DeleteObject();
	}

	if(bShow == TRUE)
	{
		CString strItem = CListCtrl::GetItemText(nItem,nIndex);
		m_edit.MoveWindow(rcCtrl);
		m_edit.ShowWindow(SW_SHOW);
		m_edit.SetWindowText(strItem);
		::SetFocus(m_edit.GetSafeHwnd());
		m_edit.SetSel(0,-1);  
		m_edit.SetCtrlData(MAKEWPARAM(nIndex,nItem));	
	}
	else
		m_edit.ShowWindow(SW_HIDE);
}

//-------------------------------------------------------------------------------------------------
//	功能：编辑结束处理
//-------------------------------------------------------------------------------------------------
LRESULT CMyListCtrl::OnEditEnd(WPARAM wParam,LPARAM lParam)
{
	if(wParam == TRUE)
	{
		CString strText(_T(""));
		m_edit.GetWindowText(strText);
		DWORD dwData = m_edit.GetCtrlData();
		int nItem= dwData>>16;
		int nIndex = dwData&0x0000ffff;

		//[1]可编辑判断
		if (m_bEdit)
		{
			//[2]列是否可以修改
			set<int>::iterator v_pItem;
			v_pItem=m_mItem_Edit.find(nIndex);									//查询：列是否可以编辑
			if(v_pItem!=m_mItem_Edit.end())										//找到？？？
			{
				//[3]修改内容
				CListCtrl::SetItemText(nItem,nIndex,strText);					//保存修改后的数据，需要时开启即可
				v_mEdit.insert(nItem);											//保存：列表序号
			}
		}
	}

    if(lParam == FALSE)
	    m_edit.ShowWindow(SW_HIDE);

	return 0;
}

//-------------------------------------------------------------------------------------------------
//	功能：
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::OnParentNotify(UINT message, LPARAM lParam) 
{
	CListCtrl::OnParentNotify(message, lParam);	
	//////////////////////////////////////////////////////////////////////////
	CHeaderCtrl* pHeaderCtrl = CListCtrl::GetHeaderCtrl();
	
	if(pHeaderCtrl == NULL)
		return;

	CRect rcHeader;
	pHeaderCtrl->GetWindowRect(rcHeader);
	ScreenToClient(rcHeader);
	//The x coordinate is in the low-order word and the y coordinate is in the high-order word.  x坐标是低字节，有坐标为高字节
	CPoint pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);

	if(rcHeader.PtInRect(pt) && message == WM_LBUTTONDOWN && m_edit.m_hWnd != NULL)
	{
		DWORD dwStyle = m_edit.GetStyle();
		if((dwStyle&WS_VISIBLE) == WS_VISIBLE)
		{
			m_edit.ShowWindow(SW_HIDE);
		}		
	}	
}

//-------------------------------------------------------------------------------------------------
//	功能：键盘消息处理  [在编辑时支持  TAB]
//-------------------------------------------------------------------------------------------------
BOOL CMyListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)										//按键消息
	{
		if(pMsg->wParam == VK_TAB && m_edit.m_hWnd != NULL)				//Tab消息
		{
		 	DWORD dwStyle = m_edit.GetStyle();
			if((dwStyle&WS_VISIBLE) == WS_VISIBLE)
			{
			     OnEditEnd(TRUE,TRUE);

				 int nItem;
				 int nSub;
				 CRect rcCtrl;	

				 if(FALSE == Key_Ctrl(nItem, nSub))
                    Key_Shift(nItem,nSub);

                 CListCtrl::GetSubItemRect(nItem, nSub, LVIR_LABEL, rcCtrl); 
				 CPoint pt(rcCtrl.left+1,rcCtrl.top+1);

				 OnLButtonDblClk(0,pt);

				 POSITION pos = CListCtrl::GetFirstSelectedItemPosition();
				 if (pos != NULL)
				 {
					 while (pos)
					 {
						 int ntpItem = CListCtrl::GetNextSelectedItem(pos);
						 CListCtrl::SetItemState(ntpItem,0,LVIS_SELECTED);
					 }
				 }
				 CListCtrl::SetItemState(nItem,  LVIS_SELECTED,  LVIS_SELECTED);
				 return TRUE;
			}
		}
	}	
	return CListCtrl::PreTranslateMessage(pMsg);
}

//-------------------------------------------------------------------------------------------------
//	功能：在编辑时支持 SHIFT + TAB 
//-------------------------------------------------------------------------------------------------
BOOL CMyListCtrl::Key_Shift(int& nItem,int& nSub)
{
	int nItemCount = CListCtrl::GetItemCount();

	DWORD dwData = m_edit.GetCtrlData();
	nItem= dwData>>16;
	nSub = dwData&0x0000ffff;
	
	CHeaderCtrl* pHeader = CListCtrl::GetHeaderCtrl();
	if(pHeader == NULL)
		return FALSE;
	
	short sRet = GetKeyState(VK_SHIFT);
	int nSubcCount = pHeader->GetItemCount();
	sRet = sRet >>15;
	if(sRet == 0)
	{
		nSub += 1;
		if(nSub >= nSubcCount)
		{
			if(nItem == nItemCount - 1)
			{
				nItem = 0;
				nSub  = 0;
			}
			else
			{
				nSub = 0;
				nItem += 1;
			}
		}
		if(nItem >= nItemCount)
			nItem = nItemCount - 1;
		return FALSE;
	}
	else
	{
		nSub -= 1;
		if(nSub < 0)
		{
			
			nSub = nSubcCount - 1;
			nItem --;
		}
		
		if(nItem < 0)
			nItem = nItemCount- 1;
		return TRUE;		
	}
	return FALSE;
}

//-------------------------------------------------------------------------------------------------
//	功能：在编辑时支持 CTRL + TAB
//-------------------------------------------------------------------------------------------------
BOOL CMyListCtrl::Key_Ctrl(int& nItem,int &nSub)
{
    short sRet = GetKeyState(VK_CONTROL);
	DWORD dwData = m_edit.GetCtrlData();
	nItem= dwData>>16;
	nSub = dwData&0x0000ffff;
	
	sRet = sRet >>15;
	int nItemCount = CListCtrl::GetItemCount();
	if(sRet != 0)
	{
		nItem = nItem >=nItemCount-1? 0:nItem += 1;
		return TRUE;
	}
	
	return FALSE;
}

//-------------------------------------------------------------------------------------------------
//	功能：对增加的iTem项添加标签，以便于排序、增加颜色的需要；
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::OnInsertitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;							//列表指针
	// TODO: Add your control notification handler code here
	CListCtrl::SetItemData(pNMListView->iItem,m_iCount);						//设置：标签(排序需要)
	m_iCount++;																	//计数器
	
	*pResult = 0;
}

//-------------------------------------------------------------------------------------------------
//	功能：设置一行的背景颜色
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::SetItemBkColor(DWORD iItem , COLORREF color)
{
	if (color!=RGB(255,255,255))												//底色？？？
		MapItemBkColor.insert(pair<int,COLORREF>(iItem,color));					//向映射表添加键值对：(条目--color)
	else
		MapItemBkColor.erase(MapItemBkColor.find(iItem));						//删除键值对
	this->RedrawItems(iItem, iItem);											//重新绘制一行信息
	UpdateWindow();																//更新屏幕
}

//-------------------------------------------------------------------------------------------------
//	功能：设置一行的文字颜色
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::SetItemTextColor(DWORD iItem , COLORREF color)
{
	if (color!=RGB(255,255,255))												//底色？？？
		MapItemColor.insert(pair<int,COLORREF>(iItem,color));					//向映射表添加键值对：(条目--color)
	else
		MapItemColor.erase(MapItemBkColor.find(iItem));							//删除键值对
	this->RedrawItems(iItem, iItem);											//重新绘制一行信息
	UpdateWindow();																//更新屏幕 
}

//-------------------------------------------------------------------------------------------------
//	功能：清除ListCtrl的所有Item
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::DeleteAllItems()
{
	CListCtrl::DeleteAllItems();												//清除ListCtrl的所有Item
	MapItemColor.clear();														//清除文字颜色映射键值对
	MapItemBkColor.clear();														//清除背景颜色映射键值对
	m_iCount=0;																	//默认：计数器=0
	
	//清除排序列的header---------------------------------------------------------------------------
	CString v_sTemp;															//临时变量
	HDITEM hd2;
	TCHAR  szBuffer[256] = _T("");												//数据缓冲区

	hd2.mask = HDI_TEXT;														//hd2：参数配置
	hd2.pszText = szBuffer;
	hd2.cchTextMax = 255;
	if(m_nSortedCol != -1)														//排序列号有效？
	{
		GetHeaderCtrl()->GetItem(m_nSortedCol, &hd2);							//获取上次列的hearder
		v_sTemp = hd2.pszText;													//获取上次header字符串
		v_sTemp.Trim();															//剔除空格
		if(v_sTemp.Find("▲") != -1 || v_sTemp.Find("▼") != -1)				//有排序标志？？？
			v_sTemp = v_sTemp.Right(v_sTemp.GetLength() - 2);					//去除排序标志
		hd2.pszText = (LPSTR)(LPCTSTR)v_sTemp;
		GetHeaderCtrl()->SetItem(m_nSortedCol, &hd2);							//回归正常状态
		m_iOldSortedCol = -1;													//列header修改完成标记
	}
}

//-------------------------------------------------------------------------------------------------
//	功能：ListCtrl记录集排序
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::My_Sort(NMHDR * pNMHDR)
{
	//变量实例化
	CString v_sTemp;															//临时变量
	HDITEM hd,hd2;
	TCHAR  szBuffer[256] = _T("");												//数据缓冲区

	hd.mask = HDI_TEXT;															//hd：参数配置
	hd.pszText = szBuffer;
	hd.cchTextMax = 255;

	hd2.mask = HDI_TEXT;														//hd2：参数配置
	hd2.pszText = szBuffer;
	hd2.cchTextMax = 255;


	//排序标志处理：m_bAsc=1 升序▲；m_bAsc=0 降序▼；----------------------------------------------
	LPNMLISTVIEW   pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);		//获得消息结构体指针

	if (pNMListView->iSubItem==0)												//第0列？？？
		return;																	//不排序

	if( pNMListView->iSubItem == m_nSortedCol )									//同一列？？？
		m_bAsc = !m_bAsc;														//同一列：排序相反
	else
		m_nSortedCol = pNMListView->iSubItem;									//保存：本次排序的列号

	//清除上次列的header----------------------------------------------------------------------------	
	if(m_iOldSortedCol != -1)													//上次列号有效？
	{
		GetHeaderCtrl()->GetItem(m_iOldSortedCol, &hd2);						//获取上次列的hearder
		v_sTemp = hd2.pszText;													//获取上次header字符串
		if(v_sTemp.Find("▲") != -1 || v_sTemp.Find("▼") != -1)				//有排序标志？？？
			v_sTemp = v_sTemp.Right(v_sTemp.GetLength() - 2);					//去除排序标志
		hd2.pszText = (LPSTR)(LPCTSTR)v_sTemp;
		GetHeaderCtrl()->SetItem(m_iOldSortedCol, &hd2);						//回归正常状态
		m_iOldSortedCol = -1;													//列header修改完成标记
	}
	m_iOldSortedCol = m_nSortedCol;												//保存：上次排序的列号

	//修改本次列的header----------------------------------------------------------------------------
	GetHeaderCtrl()->GetItem(m_nSortedCol, &hd);								//获取hearder
	v_sTemp = hd.pszText;														//获取上次列header字符串
	if(m_bAsc)																	//顺序判断 
		v_sTemp = "▲" + v_sTemp;												//升序
	else														
		v_sTemp ="▼" + v_sTemp;												//降序
	hd.pszText = (LPSTR)(LPCTSTR)v_sTemp;												
	GetHeaderCtrl()->SetItem(m_nSortedCol, &hd);								//设置列header字符串

	//排序：依据选定列、升/降序；
	//参数：回调函数(ListCompare)、列表窗指针；
	SortItems((PFNLVCOMPARE)CompareFunction,(LPARAM)this);						//排序：得到排序后的列表窗 
}

//-------------------------------------------------------------------------------------------------
//	功能：排序回调函数：int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)，
//	第三个参数为调用者传递的数据；第一和第二个参数为用于比较的两项的ItemData；
//	可以通过DWORD CListCtrl::GetItemData( int nItem )/BOOL CListCtrl::SetItemData( int nItem, DWORD dwData )来对每一项的ItemData进行存取。
//	参数：	lParam1：	CMyListCtrl的标签(用于获取比较的第一个参数)；
//			lParam2：	CMyListCtrl的标签(用于获取比较的第二个参数)；
//			lParamSort：CMyListCtrl指针；
//	返回：-1代表第一项排应在第二项前面；返回1代表第一项排应在第二项后面；返回0代表两项相等；
//-------------------------------------------------------------------------------------------------
int  CALLBACK CMyListCtrl::CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	//变量定义
	int			iCompRes = 0;													//
	CString		v_sStr1,v_sStr2;												//比较的两个字符串 
	int			nItem1=0,nItem2=0;												//比较的两个行号

	//获取：ListCtrl的指针
	CMyListCtrl*  pList = (CMyListCtrl*)lParamSort;								//CMyListCtrl：实例化
	
	//功能：由标签(lParam1/lParam2)转换成对应的Item(ListCtrl的行)
	LVFINDINFO FindInfo;														//实例化
	FindInfo.flags = LVFI_PARAM;												//设置为LVFI_PARAM，后面的FindItem才能有效
	FindInfo.lParam=lParam1;													//配置：lParam1
	nItem1=pList->FindItem(&FindInfo, -1);										//由item data(lParam1)得到对应的item(行号)
	FindInfo.lParam=lParam2;													//配置：lParam2
	nItem2=pList->FindItem(&FindInfo, -1);										//由item data(lParam2)得到对应的item(行号)
	if((nItem1==-1)||(nItem2==-1))												//出错
		return 0;																//返回：0，不排序

	//获取：排序列中，比较两行的数据v_sStr1、v_sStr2
	v_sStr1 = pList->GetItemText(nItem1, pList->m_nSortedCol);					//排序对应列位置的Text
	v_sStr2 = pList->GetItemText(nItem2, pList->m_nSortedCol);					//排序对应列位置的Text

	//比较信息的数据类型处理
	if(pList->m_bNumber[pList->m_nSortedCol])									//数字列？？？
	{
		v_sStr1.Replace(",","");												//","==>""
		v_sStr2.Replace(",","");												//","==>""
//	}
//	if(IsNumber(v_sStr1) && IsNumber(v_sStr2))									//判断是数字，假定同一列中数据类型相同
//	{
		double i1,i2;															//数字比较
		i1 = atof(v_sStr1);
		i2 = atof(v_sStr2);
		if(i1 > i2) 
			iCompRes = 1; 
		else if(i1 == i2) 
			iCompRes = 0; 
		else 
			iCompRes = -1;
	}
	else																		//字符串比较
	{		
		if(v_sStr1 > v_sStr2) 
			iCompRes = 1; 
		else if(v_sStr1 == v_sStr2 ) 
			iCompRes = 0; 
		else 
			iCompRes = -1;			
	}

	//返回值处理-----------------------------------------------------------------------------------
	if(pList->m_bAsc)															//升序 
		return iCompRes;														//返回值
	else																		//降序
		return iCompRes*(-1);													//返回值

	return 0;
}

//-------------------------------------------------------------------------------------------------
//	功能：数字判断函数
//-------------------------------------------------------------------------------------------------
bool CMyListCtrl::IsNumber( LPCTSTR pszText )
{	
	for( int i = 0; i < lstrlen( pszText ); i++ )
	{
		if(pszText[i]=='.'|| pszText[i]=='-' || pszText[i]=='+')
			continue;
		if( !(pszText[i] >= 0 && pszText[i] <= 127) )
			return false;
		if( !_istdigit( pszText[ i ] ) )
			return false;		
	}
	return true;	
}

//-------------------------------------------------------------------------------------------------
//	功能：设置列：数字比较
//	参数：v_iCol：列号
//-------------------------------------------------------------------------------------------------
bool CMyListCtrl::SetNumber(int v_iCol)
{	
	if(v_iCol<0 || v_iCol>=200)													//列号在0--20之间？？
		return false;															//不在：返回失败
	m_bNumber[v_iCol]=true;														//列标志：数字标记
	return true;																//返回：成功
}

//-------------------------------------------------------------------------------------------------
//	功能：设置列：数字比较无效
//	参数：v_iCol：列号
//-------------------------------------------------------------------------------------------------
bool CMyListCtrl::ClearNumber(int v_iCol)
{	
	if(v_iCol<0 || v_iCol>=300)													//列号在0--20之间？？
		return false;															//不在：返回失败
	m_bNumber[v_iCol]=false;													//列标志：数字标记
	return true;																//返回：成功
}

//-------------------------------------------------------------------------------------------------
//	功能：设置可编辑状态
//	参数：v_bEdit：true：可编辑；	false：不可编辑；
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::SetEdit(bool v_bEdit)
{	
	m_bEdit=v_bEdit;															//设置：编辑标志
}

//-------------------------------------------------------------------------------------------------
//	功能：设置可编辑列
//	参数：v_iCol：列号；
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::SetEdit_Col(int v_iCol)
{	
	m_mItem_Edit.insert(v_iCol);												//设置：可编辑列
}

//-------------------------------------------------------------------------------------------------
//	功能：列表重绘
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = CDRF_DODEFAULT;													//


	mInt_Color::iterator	v_pIter;											//迭代器

	//NM_CUSTOMDRAW消息的处理函数的参数中包含 NMHDR，在 CUSTOMDRAW 的通知下 NMHDR 可以被转换成为 NMLVCUSTOMDRAW 结构，该结构包含了列表控件中需要自绘区域的全部信息：
	//typedef struct tagNMLVCUSTOMDRAW 
	//{ 
	//	NMCUSTOMDRAW	nmcd;				// 包含客户自绘控件信息的结构 
	//	COLORREF        clrText;            // 列表视图显示文字的颜色 
    //	COLORREF        clrTextBk;			// 列表视图显示文字的背景色 
	//} NMLVCUSTOMDRAW, *LPNMLVCUSTOMDRAW;
	NMLVCUSTOMDRAW * lplvdr = (NMLVCUSTOMDRAW*)pNMHDR;							//客户绘制iTem的指针

	//NMCUSTOMDRAW 结构定义如下：
	//typedef struct tagNMCUSTOMDRAWINFO 
	//{ 
    //	NMHDR		hdr;                    // 含有通知信息的 NMHDR 结构 
    //	DWORD		dwDrawStage;			// 目前绘制的步骤 
    //	HDC         hdc;                    // 设备上下文句柄 
    //	RECT        rc;                     // 绘制的区域 
    //	DWORD		dwItemSpec;				// 绘制项的说明 
    //	UINT        uItemState;				// 当前项的状态 
    //	LPARAM		lItemlParam				// 应用程序定义的数据 
	//} NMCUSTOMDRAW, FAR * LPNMCUSTOMDRAW;
	//NMLVCUSTOMDRAW.nmcd.dwDrawStage 字段，它主要包含以下几个枚举值：
	//CDDS_PREPAINT：表示在绘画前阶段。
	//CDDS_ITEMPREPAINT：表示在列表项的绘画前阶段。
	//CDDS_SUBITEM：表示绘制子项。
	//CDDS_ITEM：表示要绘制项的信息已经可用。
	NMCUSTOMDRAW &nmcd = lplvdr->nmcd;											//客户自绘控件信息的结构

	switch(lplvdr->nmcd.dwDrawStage)											//目前绘制的步骤
	{
	case CDDS_PREPAINT:															//这种情况必须处理，且必须将pResult设置为 CDRF_NOTIFYITEMDRAW, 否则父窗口怎么也收不到 CDDS_ITEMPREPAINT 通知消息 (GGH) 
		*pResult = CDRF_NOTIFYITEMDRAW;											//// 重新定位视图窗口，这样 TreeCtrl 的 DefWindowProc 不会重画									
		break;

	case CDDS_ITEMPREPAINT:														//设置背景和前景颜色
		{
			v_pIter=MapItemColor.find(nmcd.dwItemSpec);							//查找：条目
			if( v_pIter!=MapItemColor.end())									//找到？？？
			{
				lplvdr->clrText = v_pIter->second;								//设置：文字颜色
				*pResult = CDRF_DODEFAULT;										//返回参数 
			}			
			
			v_pIter=MapItemBkColor.find(nmcd.dwItemSpec);						//查找：条目
			if( v_pIter!=MapItemBkColor.end())									//找到？？？
			{
				lplvdr->clrTextBk = v_pIter->second;							//设置：背景颜色
				*pResult = CDRF_DODEFAULT;										//返回参数
			}
		}
		break;

	}
}

//=================================================================================================
//	功能：虚拟列表
//=================================================================================================

//-------------------------------------------------------------------------------------------------
//	功能：虚拟列表保存行信息[即：增加一行信息]
//	参数：v_mLine_Data：容器(行数据[列号--数据])；
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::Put_Line_Data(mInt_String v_mLine_Data)
{	
	mInt_String				v_mData;									//临时变量
	lList_Data::iterator	v_pLine;									//迭代器
	mInt_String::iterator	v_pData;									//迭代器

	for (v_pData=v_mLine_Data.begin();v_pData!=v_mLine_Data.end();v_pData++)	//遍历
		v_mData.insert(pair<int,CString>(v_pData->first,v_pData->second));		//容器增加
	v_lList_Data.push_back(v_mData);									//加入队列
}

//-------------------------------------------------------------------------------------------------
//	功能：虚拟列表添加数据开始
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::Put_Line_Begin()
{	
	//复位：数据无效
	v_bList_Show = false;												//默认：数据未就绪，请等待；

	//复位：虚拟列表容器
	lList_Data::iterator v_pIter;										//迭代器
	for (v_pIter=v_lList_Data.begin();v_pIter!=v_lList_Data.end();v_pIter++)	//遍历
		v_pIter->clear();												//清除容器
	v_lList_Data.clear();												//清除队列
	v_mList_Rec.clear();												//清除容器

	//清除：列表信息
	DeleteAllItems();													//清除列表信息
}

//-------------------------------------------------------------------------------------------------
//	功能：虚拟列表添加数据结束
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::Put_Line_End()
{	
	int						v_iNum=0,v_iLen=4;							//临时变量
	CString					v_sData;									//临时变量
	lList_Data::iterator	v_pLine;									//迭代器
	mInt_String::iterator	v_pData;									//迭代器

	//【0】复位：序号--位置指针映射
	v_mList_Rec.clear();												//容器清除

	//【1】获取：序号长度
	v_pLine=v_lList_Data.begin();
	if (v_pLine!=v_lList_Data.end())
	{
		v_pData=v_pLine->find(0);										//查找：第0列
		if (v_pData!=v_pLine->end())									//找到？？？
		{
			v_sData=v_pData->second;									//赋值：行序号
			v_sData.Trim();												//去掉空格
			v_iLen=v_sData.GetLength();									//获取：行序号长度
		}
	}

	//【2】获取：序号--位置指针映射
	for (v_pLine=v_lList_Data.begin();v_pLine!=v_lList_Data.end();v_pLine++)	//遍历
	{
		//设置：序号--位置指针映射
		v_mList_Rec.insert(pair<int,lList_Data::iterator>(v_iNum,v_pLine));		//容器增加
		v_iNum++;
		
		//修改序号
		v_pData=v_pLine->find(0);										//查找：第0列
		if (v_pData!=v_pLine->end())									//找到？？？
		{
			if (v_iLen==4)
				v_sData.Format("%04d",v_iNum);
			else if(v_iLen==5)
				v_sData.Format("%05d",v_iNum);
			else 
				v_sData.Format("%06d",v_iNum);

			v_pData->second=v_sData;									//赋值：行序号
		}
	}

	//【3】设置：列表行数量
	CListCtrl::DeleteAllItems();										//清除ListCtrl的所有Item
	SetItemCount(v_lList_Data.size());									//设置：虚拟列表行数量

	//【4】设置：数据有效
	v_bList_Show = true;												//置位：数据准备好，可以获取；
}

//-------------------------------------------------------------------------------------------------
//	功能：虚拟列表获取显示的数据
//	工作：依据列表需要的元素参数(行、列)，在数据容器中查询数据，赋值到列表中。
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::Get_List_Data(NMLVDISPINFO *pDispInfo)
{	
	bool		v_bOK=false;											//标记：无效
	int			v_iItem;												//复位：行计数器
	CString		v_sData="";												//临时变量

	mList_Rec::iterator		v_pIter;									//迭代器
	mInt_String::iterator	v_pData;									//迭代器

	//【0】条件满足？？？[数据准备好]
	if (!v_bList_Show)													//数据准备好？？？
		return;

	//【1】获取：列表参数(行：pItem->iItem；列：pItem->iSubItem；)
	LV_ITEM* pItem= &(pDispInfo)->item;									//获取：Item操纵指针
	v_iItem = pItem->iItem;												//获取：当前响应list索引

	//【2】获取：依据行参数，获取粒度数据在容器中的位置(v_pPM_Time)；
	v_pIter=v_mList_Rec.find(v_iItem);									//查找：行
	if(v_pIter==v_mList_Rec.end())										//没找到？？？
		return;															//无效返回

	//【3】获取：数据
	v_pData=v_pIter->second->find(pItem->iSubItem);						//查找：列序号
	if (v_pData!=v_pIter->second->end())
	{
		v_sData=v_pData->second;										//获取：列信息
		v_bOK=true;														//标记：有效
	}

	//【4】赋值
	if(v_bOK && pItem->mask&LVIF_TEXT)									//字符串缓冲区有效
	{
		if (v_sData.GetLength()>250)									//大于250???
			v_sData=v_sData.Left(250);									//截断

		lstrcpy( pItem->pszText, v_sData);
	}
}

//-------------------------------------------------------------------------------------------------
//	功能：ListCtrl记录集排序(表头处理)
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::My_Sort_Virtual(NMHDR * pNMHDR)
{
	//【0】变量实例化
	v_bList_Show	= false;											//默认：数据未就绪，请等待；

	CString v_sTemp;													//临时变量
	HDITEM hd,hd2;
	TCHAR  szBuffer[256] = _T("");										//数据缓冲区

	hd.mask = HDI_TEXT;													//hd：参数配置
	hd.pszText = szBuffer;
	hd.cchTextMax = 255;

	hd2.mask = HDI_TEXT;												//hd2：参数配置
	hd2.pszText = szBuffer;
	hd2.cchTextMax = 255;


	//【1】排序标志处理：m_bAsc=1 升序▲；m_bAsc=0 降序▼；----------------------------------------------
	LPNMLISTVIEW   pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);//获得消息结构体指针

	if (pNMListView->iSubItem==0)										//第0列？？？
		return;															//不排序

	if( pNMListView->iSubItem == m_nSortedCol )							//同一列？？？
		m_bAsc = !m_bAsc;												//同一列：排序相反
	else
		m_nSortedCol = pNMListView->iSubItem;							//保存：本次排序的列号

	//【2】清除上次列的header----------------------------------------------------------------------------	
	if(m_iOldSortedCol != -1)											//上次列号有效？
	{
		GetHeaderCtrl()->GetItem(m_iOldSortedCol, &hd2);				//获取上次列的hearder
		v_sTemp = hd2.pszText;											//获取上次header字符串
		if(v_sTemp.Find("▲") != -1 || v_sTemp.Find("▼") != -1)		//有排序标志？？？
			v_sTemp = v_sTemp.Right(v_sTemp.GetLength() - 2);			//去除排序标志
		hd2.pszText = (LPSTR)(LPCTSTR)v_sTemp;
		GetHeaderCtrl()->SetItem(m_iOldSortedCol, &hd2);				//回归正常状态
	}
	m_iOldSortedCol = m_nSortedCol;										//保存：上次排序的列号

	//【3】修改本次列的header----------------------------------------------------------------------------
	GetHeaderCtrl()->GetItem(m_nSortedCol, &hd);						//获取hearder
	v_sTemp = hd.pszText;												//获取上次列header字符串
	if(m_bAsc)															//顺序判断 
		v_sTemp = "▲" + v_sTemp;										//升序
	else														
		v_sTemp ="▼" + v_sTemp;										//降序
	hd.pszText = (LPSTR)(LPCTSTR)v_sTemp;												
	GetHeaderCtrl()->SetItem(m_nSortedCol, &hd);						//设置列header字符串

	//【4】链表排序
	v_pList = this;														//赋值：列表指针
	v_lList_Data.sort(My_Comp);											//链表排序

	//【5】列表处理
	Put_Line_End();														//列表添加数据结束
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：虚拟列表排序
//------------------------------------------------------------------------------------------------------
bool CMyListCtrl::My_Comp(mInt_String v_cData_1, mInt_String v_cData_2) 
{
	bool			v_bResult;											//临时变量
	CString			v_sText1="",v_sText2="";							//临时变量
	double			v_dValue1,v_dValue2;								//数字比较

	mInt_String::iterator		v_pIter;								//数据迭代器

	//【1】获取两个数据
	v_pIter=v_cData_1.find(v_pList->m_nSortedCol);						//查找：列号
	if (v_pIter!=v_cData_1.end())										//找到
		v_sText1=v_pIter->second;										//对象1
	v_pIter=v_cData_2.find(v_pList->m_nSortedCol);						//查找：列号
	if (v_pIter!=v_cData_2.end())										//找到
		v_sText2=v_pIter->second;										//对象2

	//【2】数据类型处理：[数字、字符]
	if(v_pList->m_bNumber[v_pList->m_nSortedCol])						//数字列？？？
	{																	//数字列
		v_dValue1 = atof(v_sText1);
		v_dValue2 = atof(v_sText2);
		//【3】依据升序/降序，得出比较结果
		if (v_pList->m_bAsc)											//升序
			v_bResult=v_dValue1 < v_dValue2;							//
		else															//降序
			v_bResult=v_dValue1 > v_dValue2;							//
	}
	else																//字符列
	{
		//【3】依据升序/降序，得出比较结果
		if (v_pList->m_bAsc)											//升序
			v_bResult=v_sText1 < v_sText2;								//
		else															//降序
			v_bResult=v_sText1 > v_sText2;								//
	}

	//【4】返回结果
	return v_bResult;													//返回结果
}

//-------------------------------------------------------------------------------------------------
// 导出处理：写入CSV文件；【虚拟列表】
// 参	 数：v_cList：列表窗指针
//			 v_sFile_Name：输出的文件名称
//			 v_bFile_Open：文件打开标志；[1：打开；0：不打开；]
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::CSV_OutPut_Virtual(CString v_sFile_Name,int v_bFile_Open) 
{
	int		v_iNum1,v_iNum3;											//临时变量
	int		v_iColumn,v_iRow;											//列表列数量、行数量
	bool	v_bOK=false;												//临时变量
	CString	v_sStr,v_sCSV,v_sPath_Data,v_sCSV_File;						//临时变量
	CFile	v_fWriter;													//实例化
	CFileFind	finder;

	//目录处理
	v_sPath_Data=theApp.m_sPath+"\\DATA";								//文件目录
	if (!finder.FindFile(v_sPath_Data))									//存在？
	{
		if (!::CreateDirectory(v_sPath_Data, NULL))						//不存在：创建目录
		{
			::MessageBox(NULL,"异常："+v_sPath_Data+" 目录创建失败。", "提示", MB_OK);		//失败
			return;
		}
	}

	try
	{
		for(v_iNum3=0;v_iNum3<100;v_iNum3++)
		{
			if (v_iNum3==0)												//字符串
				v_sStr="";
			else
				v_sStr.Format("_%02d",v_iNum3);							//字符串
			//生成CSV文件：文件名称v_sCSV_File
			if (v_sFile_Name.IsEmpty())
				v_sCSV_File = theApp.m_sPath+"\\DATA\\Excel"+v_sStr+".csv";			//设置：CSV默认文件名称Excel.csv
			else
				v_sCSV_File = theApp.m_sPath+"\\DATA\\"+v_sFile_Name+v_sStr+".csv";	//设置：CSV指定的文件名称
			if(v_fWriter.Open(v_sCSV_File, CFile::modeCreate | CFile::modeWrite))	//打开文件，如不存在则生成，如存在则清空文件；
			{
				v_bOK=true;												//文件有效
				break;
			}
		}
		if (v_bOK)														//文件有效？？？
		{
			v_iRow = GetItemCount();									//取列表框行数
			CHeaderCtrl*  pHeaderCtrl =GetHeaderCtrl();					//列表框表头
			v_iColumn=pHeaderCtrl->GetItemCount();						//取列表框列数

			HDITEM hdi;													//列结构
			TCHAR lpBuffer[256]; 
			hdi.mask = HDI_TEXT; 
			hdi.pszText = lpBuffer; 
			hdi.cchTextMax = 256; 	
			//表头处理：取列表的宽及列名称，逐列写入CSV
			for(v_iNum1=0;v_iNum1<v_iColumn;v_iNum1++)					//列
			{
				pHeaderCtrl->GetItem(v_iNum1,&hdi);						//取List列名称
				v_sStr=hdi.pszText;										//列名称
				v_sStr.Replace(" ","");									//剔除空格
				if(v_iNum1==0)
					v_sCSV=v_sStr;										//第一列
				else
					v_sCSV += "," + v_sStr;								//第二列、第三列、...、第n列
			}
			v_sCSV +="\r\n";											//回车换行
			v_fWriter.Write(v_sCSV,v_sCSV.GetLength());					//写入字符串

			//表内容处理：取列表数据，写入CSV
			lList_Data::iterator	v_pIter;							//迭代器
			mInt_String::iterator	v_pData;							//迭代器
			for (v_pIter=v_lList_Data.begin();v_pIter!=v_lList_Data.end();v_pIter++)	//遍历
			{
				for (v_pData=v_pIter->begin();v_pData!=v_pIter->end();v_pData++)	//遍历
				{
					v_sStr=v_pData->second;								//取列表数据
					v_sStr.Replace(",",";");
					if(v_pData->first==0)
						v_sCSV=v_sStr;									//第一列
					else
						v_sCSV += "," + v_sStr;							//第二列、第三列、...、第n列
				}
				v_sCSV +="\r\n";										//回车换行
				v_fWriter.Write(v_sCSV,v_sCSV.GetLength());				//写入字符串
			}
			v_fWriter.Close();											//文件关闭

			//打开文件
			if (v_bFile_Open)
				ShellExecute(NULL,"open", v_sCSV_File, NULL, NULL, SW_SHOWNORMAL);			//打开文件：Excel.csv
		}
		else
			::MessageBox(NULL,"请关闭已经打开的导出文件，再重新导出！！！", "提示", MB_OK);	//提示信息
	}	
	catch(_com_error e)
	{
		CString v_sError = (char *)e.Description();						//获得：出错信息
		::MessageBox(NULL,"异常"+v_sError, "提示", MB_OK);				//提示信息
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	类结束：CMyListCtrl
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	类定义：ListCtrlEdit
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//	构造函数
//-------------------------------------------------------------------------------------------------
CMyListCtrlEdit::CMyListCtrlEdit()
{
}

//-------------------------------------------------------------------------------------------------
//	析构函数
//-------------------------------------------------------------------------------------------------
CMyListCtrlEdit::~CMyListCtrlEdit()
{
}

//-------------------------------------------------------------------------------------------------
//	消息映射
//-------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMyListCtrlEdit, CEdit)
	//{{AFX_MSG_MAP(CMyListCtrlEdit)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//-------------------------------------------------------------------------------------------------
//	
//-------------------------------------------------------------------------------------------------
void CMyListCtrlEdit::SetCtrlData(DWORD dwData)
{
	m_dwData = dwData;
}

//-------------------------------------------------------------------------------------------------
//	
//-------------------------------------------------------------------------------------------------
DWORD CMyListCtrlEdit::GetCtrlData()
{
   return m_dwData;
}

//-------------------------------------------------------------------------------------------------
//	函数功能：失去焦点
//-------------------------------------------------------------------------------------------------
void CMyListCtrlEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	CWnd* pParent = this->GetParent();									//获得：父窗体
	::PostMessage(pParent->GetSafeHwnd(),WM_USER_EDIT_END,m_bExchange,0);			//发送：用户编辑结束消息[有效]
}

//-------------------------------------------------------------------------------------------------
//	函数功能：消息预处理 [主要处理键盘消息]
//-------------------------------------------------------------------------------------------------
BOOL CMyListCtrlEdit::PreTranslateMessage(MSG* pMsg) 
{
	
	if(pMsg->message == WM_KEYDOWN)										//按键消息
	{
		if(pMsg->wParam == VK_RETURN)									//回车消息
		{
			CWnd* pParent = this->GetParent();							//获得：父窗体
			m_bExchange = TRUE;
			::PostMessage(pParent->GetSafeHwnd(),WM_USER_EDIT_END,m_bExchange,0);	//发送：用户编辑结束消息[有效]
		}
		else if(pMsg->wParam == VK_ESCAPE)								//Esc消息
		{
           CWnd* pParent = this->GetParent();							//获得：父窗体
		   m_bExchange = FALSE;
			::PostMessage(pParent->GetSafeHwnd(),WM_USER_EDIT_END,m_bExchange,0);	//发送：用户编辑结束消息[退出]
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

//-------------------------------------------------------------------------------------------------
//	函数功能：聚焦
//-------------------------------------------------------------------------------------------------
void CMyListCtrlEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
    m_bExchange = TRUE;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	类结束：CMyListCtrlEdit
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
