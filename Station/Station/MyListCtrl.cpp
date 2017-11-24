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
//	�ඨ�壺CMyListCtrl
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��̬������ʼ��
CMyListCtrl* CMyListCtrl::v_pList = NULL;										//��ֵ����

//-------------------------------------------------------------------------------------------------
//	���캯��
//-------------------------------------------------------------------------------------------------
CMyListCtrl::CMyListCtrl()
{
	m_edit.m_hWnd	= NULL;														//Ĭ�ϣ��༭���ΪNULL
	m_bAsc			= true;														//Ĭ�ϣ�����
	m_nSortedCol	= 0;														//Ĭ�ϣ���0������
	m_iOldSortedCol	= -1;														//Ĭ�ϣ����ϴ���header
	crWindow		= ::GetSysColor(COLOR_WINDOW);								//Ĭ�ϣ�������ɫ
	crWindowText	= ::GetSysColor(COLOR_WINDOWFRAME);							//Ĭ�ϣ��ı���ɫ
	m_iCount=0;																	//Ĭ�ϣ�������=0
	v_bList_Show	= false;													//Ĭ�ϣ�����δ��������ȴ���
	for(int vi=0;vi<200;vi++)
		m_bNumber[vi]=false;													//Ĭ�ϣ������ֱ��Ϊ�ַ���

}

//-------------------------------------------------------------------------------------------------
//	��������
//-------------------------------------------------------------------------------------------------
CMyListCtrl::~CMyListCtrl()
{
	//��λ������ɫ
	MapItemColor.clear();														//���������ɫӳ���ֵ��
	MapItemBkColor.clear();														//���������ɫӳ���ֵ��

	//��λ���༭
	v_mEdit.clear();															//���
	m_mItem_Edit.clear();														//���

	//��λ�������б�����
	lList_Data::iterator v_pIter;												//������
	for (v_pIter=v_lList_Data.begin();v_pIter!=v_lList_Data.end();v_pIter++)	//����
		v_pIter->clear();														//�������
	v_lList_Data.clear();														//�������
	v_mList_Rec.clear();														//�������
}

//-------------------------------------------------------------------------------------------------
//	��Ϣӳ��
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
//	���ܣ����˫��
//	���ܣ���ȡ��Ԫ�������(����)
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CRect rcCtrl;																//
	LVHITTESTINFO lvhti;														//

	//[1]��ȡ�С���
	lvhti.pt = point;															//
	int nItem = CListCtrl::SubItemHitTest(&lvhti);								//��ȡ����
	if(nItem == -1)
	   return;
	int nSubItem = lvhti.iSubItem;												//��ȡ����

	//[2]�༭����
	CListCtrl::GetSubItemRect(nItem,nSubItem,LVIR_LABEL,rcCtrl);				//
    ShowEdit(TRUE,nItem,nSubItem,rcCtrl);										//
	CListCtrl::OnLButtonDblClk(nFlags, point);									//
}

//-------------------------------------------------------------------------------------------------
//	���ܣ���ʾ�༭��
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
//	���ܣ��༭��������
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

		//[1]�ɱ༭�ж�
		if (m_bEdit)
		{
			//[2]���Ƿ�����޸�
			set<int>::iterator v_pItem;
			v_pItem=m_mItem_Edit.find(nIndex);									//��ѯ�����Ƿ���Ա༭
			if(v_pItem!=m_mItem_Edit.end())										//�ҵ�������
			{
				//[3]�޸�����
				CListCtrl::SetItemText(nItem,nIndex,strText);					//�����޸ĺ�����ݣ���Ҫʱ��������
				v_mEdit.insert(nItem);											//���棺�б����
			}
		}
	}

    if(lParam == FALSE)
	    m_edit.ShowWindow(SW_HIDE);

	return 0;
}

//-------------------------------------------------------------------------------------------------
//	���ܣ�
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
	//The x coordinate is in the low-order word and the y coordinate is in the high-order word.  x�����ǵ��ֽڣ�������Ϊ���ֽ�
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
//	���ܣ�������Ϣ����  [�ڱ༭ʱ֧��  TAB]
//-------------------------------------------------------------------------------------------------
BOOL CMyListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)										//������Ϣ
	{
		if(pMsg->wParam == VK_TAB && m_edit.m_hWnd != NULL)				//Tab��Ϣ
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
//	���ܣ��ڱ༭ʱ֧�� SHIFT + TAB 
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
//	���ܣ��ڱ༭ʱ֧�� CTRL + TAB
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
//	���ܣ������ӵ�iTem�����ӱ�ǩ���Ա�������������ɫ����Ҫ��
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::OnInsertitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;							//�б�ָ��
	// TODO: Add your control notification handler code here
	CListCtrl::SetItemData(pNMListView->iItem,m_iCount);						//���ã���ǩ(������Ҫ)
	m_iCount++;																	//������
	
	*pResult = 0;
}

//-------------------------------------------------------------------------------------------------
//	���ܣ�����һ�еı�����ɫ
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::SetItemBkColor(DWORD iItem , COLORREF color)
{
	if (color!=RGB(255,255,255))												//��ɫ������
		MapItemBkColor.insert(pair<int,COLORREF>(iItem,color));					//��ӳ������Ӽ�ֵ�ԣ�(��Ŀ--color)
	else
		MapItemBkColor.erase(MapItemBkColor.find(iItem));						//ɾ����ֵ��
	this->RedrawItems(iItem, iItem);											//���»���һ����Ϣ
	UpdateWindow();																//������Ļ
}

//-------------------------------------------------------------------------------------------------
//	���ܣ�����һ�е�������ɫ
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::SetItemTextColor(DWORD iItem , COLORREF color)
{
	if (color!=RGB(255,255,255))												//��ɫ������
		MapItemColor.insert(pair<int,COLORREF>(iItem,color));					//��ӳ������Ӽ�ֵ�ԣ�(��Ŀ--color)
	else
		MapItemColor.erase(MapItemBkColor.find(iItem));							//ɾ����ֵ��
	this->RedrawItems(iItem, iItem);											//���»���һ����Ϣ
	UpdateWindow();																//������Ļ 
}

//-------------------------------------------------------------------------------------------------
//	���ܣ����ListCtrl������Item
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::DeleteAllItems()
{
	CListCtrl::DeleteAllItems();												//���ListCtrl������Item
	MapItemColor.clear();														//���������ɫӳ���ֵ��
	MapItemBkColor.clear();														//���������ɫӳ���ֵ��
	m_iCount=0;																	//Ĭ�ϣ�������=0
	
	//��������е�header---------------------------------------------------------------------------
	CString v_sTemp;															//��ʱ����
	HDITEM hd2;
	TCHAR  szBuffer[256] = _T("");												//���ݻ�����

	hd2.mask = HDI_TEXT;														//hd2����������
	hd2.pszText = szBuffer;
	hd2.cchTextMax = 255;
	if(m_nSortedCol != -1)														//�����к���Ч��
	{
		GetHeaderCtrl()->GetItem(m_nSortedCol, &hd2);							//��ȡ�ϴ��е�hearder
		v_sTemp = hd2.pszText;													//��ȡ�ϴ�header�ַ���
		v_sTemp.Trim();															//�޳��ո�
		if(v_sTemp.Find("��") != -1 || v_sTemp.Find("��") != -1)				//�������־������
			v_sTemp = v_sTemp.Right(v_sTemp.GetLength() - 2);					//ȥ�������־
		hd2.pszText = (LPSTR)(LPCTSTR)v_sTemp;
		GetHeaderCtrl()->SetItem(m_nSortedCol, &hd2);							//�ع�����״̬
		m_iOldSortedCol = -1;													//��header�޸���ɱ��
	}
}

//-------------------------------------------------------------------------------------------------
//	���ܣ�ListCtrl��¼������
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::My_Sort(NMHDR * pNMHDR)
{
	//����ʵ����
	CString v_sTemp;															//��ʱ����
	HDITEM hd,hd2;
	TCHAR  szBuffer[256] = _T("");												//���ݻ�����

	hd.mask = HDI_TEXT;															//hd����������
	hd.pszText = szBuffer;
	hd.cchTextMax = 255;

	hd2.mask = HDI_TEXT;														//hd2����������
	hd2.pszText = szBuffer;
	hd2.cchTextMax = 255;


	//�����־������m_bAsc=1 �������m_bAsc=0 ���򨋣�----------------------------------------------
	LPNMLISTVIEW   pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);		//�����Ϣ�ṹ��ָ��

	if (pNMListView->iSubItem==0)												//��0�У�����
		return;																	//������

	if( pNMListView->iSubItem == m_nSortedCol )									//ͬһ�У�����
		m_bAsc = !m_bAsc;														//ͬһ�У������෴
	else
		m_nSortedCol = pNMListView->iSubItem;									//���棺����������к�

	//����ϴ��е�header----------------------------------------------------------------------------	
	if(m_iOldSortedCol != -1)													//�ϴ��к���Ч��
	{
		GetHeaderCtrl()->GetItem(m_iOldSortedCol, &hd2);						//��ȡ�ϴ��е�hearder
		v_sTemp = hd2.pszText;													//��ȡ�ϴ�header�ַ���
		if(v_sTemp.Find("��") != -1 || v_sTemp.Find("��") != -1)				//�������־������
			v_sTemp = v_sTemp.Right(v_sTemp.GetLength() - 2);					//ȥ�������־
		hd2.pszText = (LPSTR)(LPCTSTR)v_sTemp;
		GetHeaderCtrl()->SetItem(m_iOldSortedCol, &hd2);						//�ع�����״̬
		m_iOldSortedCol = -1;													//��header�޸���ɱ��
	}
	m_iOldSortedCol = m_nSortedCol;												//���棺�ϴ�������к�

	//�޸ı����е�header----------------------------------------------------------------------------
	GetHeaderCtrl()->GetItem(m_nSortedCol, &hd);								//��ȡhearder
	v_sTemp = hd.pszText;														//��ȡ�ϴ���header�ַ���
	if(m_bAsc)																	//˳���ж� 
		v_sTemp = "��" + v_sTemp;												//����
	else														
		v_sTemp ="��" + v_sTemp;												//����
	hd.pszText = (LPSTR)(LPCTSTR)v_sTemp;												
	GetHeaderCtrl()->SetItem(m_nSortedCol, &hd);								//������header�ַ���

	//��������ѡ���С���/����
	//�������ص�����(ListCompare)���б���ָ�룻
	SortItems((PFNLVCOMPARE)CompareFunction,(LPARAM)this);						//���򣺵õ��������б��� 
}

//-------------------------------------------------------------------------------------------------
//	���ܣ�����ص�������int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)��
//	����������Ϊ�����ߴ��ݵ����ݣ���һ�͵ڶ�������Ϊ���ڱȽϵ������ItemData��
//	����ͨ��DWORD CListCtrl::GetItemData( int nItem )/BOOL CListCtrl::SetItemData( int nItem, DWORD dwData )����ÿһ���ItemData���д�ȡ��
//	������	lParam1��	CMyListCtrl�ı�ǩ(���ڻ�ȡ�Ƚϵĵ�һ������)��
//			lParam2��	CMyListCtrl�ı�ǩ(���ڻ�ȡ�Ƚϵĵڶ�������)��
//			lParamSort��CMyListCtrlָ�룻
//	���أ�-1������һ����Ӧ�ڵڶ���ǰ�棻����1������һ����Ӧ�ڵڶ�����棻����0����������ȣ�
//-------------------------------------------------------------------------------------------------
int  CALLBACK CMyListCtrl::CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	//��������
	int			iCompRes = 0;													//
	CString		v_sStr1,v_sStr2;												//�Ƚϵ������ַ��� 
	int			nItem1=0,nItem2=0;												//�Ƚϵ������к�

	//��ȡ��ListCtrl��ָ��
	CMyListCtrl*  pList = (CMyListCtrl*)lParamSort;								//CMyListCtrl��ʵ����
	
	//���ܣ��ɱ�ǩ(lParam1/lParam2)ת���ɶ�Ӧ��Item(ListCtrl����)
	LVFINDINFO FindInfo;														//ʵ����
	FindInfo.flags = LVFI_PARAM;												//����ΪLVFI_PARAM�������FindItem������Ч
	FindInfo.lParam=lParam1;													//���ã�lParam1
	nItem1=pList->FindItem(&FindInfo, -1);										//��item data(lParam1)�õ���Ӧ��item(�к�)
	FindInfo.lParam=lParam2;													//���ã�lParam2
	nItem2=pList->FindItem(&FindInfo, -1);										//��item data(lParam2)�õ���Ӧ��item(�к�)
	if((nItem1==-1)||(nItem2==-1))												//����
		return 0;																//���أ�0��������

	//��ȡ���������У��Ƚ����е�����v_sStr1��v_sStr2
	v_sStr1 = pList->GetItemText(nItem1, pList->m_nSortedCol);					//�����Ӧ��λ�õ�Text
	v_sStr2 = pList->GetItemText(nItem2, pList->m_nSortedCol);					//�����Ӧ��λ�õ�Text

	//�Ƚ���Ϣ���������ʹ���
	if(pList->m_bNumber[pList->m_nSortedCol])									//�����У�����
	{
		v_sStr1.Replace(",","");												//","==>""
		v_sStr2.Replace(",","");												//","==>""
//	}
//	if(IsNumber(v_sStr1) && IsNumber(v_sStr2))									//�ж������֣��ٶ�ͬһ��������������ͬ
//	{
		double i1,i2;															//���ֱȽ�
		i1 = atof(v_sStr1);
		i2 = atof(v_sStr2);
		if(i1 > i2) 
			iCompRes = 1; 
		else if(i1 == i2) 
			iCompRes = 0; 
		else 
			iCompRes = -1;
	}
	else																		//�ַ����Ƚ�
	{		
		if(v_sStr1 > v_sStr2) 
			iCompRes = 1; 
		else if(v_sStr1 == v_sStr2 ) 
			iCompRes = 0; 
		else 
			iCompRes = -1;			
	}

	//����ֵ����-----------------------------------------------------------------------------------
	if(pList->m_bAsc)															//���� 
		return iCompRes;														//����ֵ
	else																		//����
		return iCompRes*(-1);													//����ֵ

	return 0;
}

//-------------------------------------------------------------------------------------------------
//	���ܣ������жϺ���
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
//	���ܣ������У����ֱȽ�
//	������v_iCol���к�
//-------------------------------------------------------------------------------------------------
bool CMyListCtrl::SetNumber(int v_iCol)
{	
	if(v_iCol<0 || v_iCol>=200)													//�к���0--20֮�䣿��
		return false;															//���ڣ�����ʧ��
	m_bNumber[v_iCol]=true;														//�б�־�����ֱ��
	return true;																//���أ��ɹ�
}

//-------------------------------------------------------------------------------------------------
//	���ܣ������У����ֱȽ���Ч
//	������v_iCol���к�
//-------------------------------------------------------------------------------------------------
bool CMyListCtrl::ClearNumber(int v_iCol)
{	
	if(v_iCol<0 || v_iCol>=300)													//�к���0--20֮�䣿��
		return false;															//���ڣ�����ʧ��
	m_bNumber[v_iCol]=false;													//�б�־�����ֱ��
	return true;																//���أ��ɹ�
}

//-------------------------------------------------------------------------------------------------
//	���ܣ����ÿɱ༭״̬
//	������v_bEdit��true���ɱ༭��	false�����ɱ༭��
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::SetEdit(bool v_bEdit)
{	
	m_bEdit=v_bEdit;															//���ã��༭��־
}

//-------------------------------------------------------------------------------------------------
//	���ܣ����ÿɱ༭��
//	������v_iCol���кţ�
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::SetEdit_Col(int v_iCol)
{	
	m_mItem_Edit.insert(v_iCol);												//���ã��ɱ༭��
}

//-------------------------------------------------------------------------------------------------
//	���ܣ��б��ػ�
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = CDRF_DODEFAULT;													//


	mInt_Color::iterator	v_pIter;											//������

	//NM_CUSTOMDRAW��Ϣ�Ĵ��������Ĳ����а��� NMHDR���� CUSTOMDRAW ��֪ͨ�� NMHDR ���Ա�ת����Ϊ NMLVCUSTOMDRAW �ṹ���ýṹ�������б��ؼ�����Ҫ�Ի������ȫ����Ϣ��
	//typedef struct tagNMLVCUSTOMDRAW 
	//{ 
	//	NMCUSTOMDRAW	nmcd;				// �����ͻ��Ի�ؼ���Ϣ�Ľṹ 
	//	COLORREF        clrText;            // �б���ͼ��ʾ���ֵ���ɫ 
    //	COLORREF        clrTextBk;			// �б���ͼ��ʾ���ֵı���ɫ 
	//} NMLVCUSTOMDRAW, *LPNMLVCUSTOMDRAW;
	NMLVCUSTOMDRAW * lplvdr = (NMLVCUSTOMDRAW*)pNMHDR;							//�ͻ�����iTem��ָ��

	//NMCUSTOMDRAW �ṹ�������£�
	//typedef struct tagNMCUSTOMDRAWINFO 
	//{ 
    //	NMHDR		hdr;                    // ����֪ͨ��Ϣ�� NMHDR �ṹ 
    //	DWORD		dwDrawStage;			// Ŀǰ���ƵĲ��� 
    //	HDC         hdc;                    // �豸�����ľ�� 
    //	RECT        rc;                     // ���Ƶ����� 
    //	DWORD		dwItemSpec;				// �������˵�� 
    //	UINT        uItemState;				// ��ǰ���״̬ 
    //	LPARAM		lItemlParam				// Ӧ�ó���������� 
	//} NMCUSTOMDRAW, FAR * LPNMCUSTOMDRAW;
	//NMLVCUSTOMDRAW.nmcd.dwDrawStage �ֶΣ�����Ҫ�������¼���ö��ֵ��
	//CDDS_PREPAINT����ʾ�ڻ滭ǰ�׶Ρ�
	//CDDS_ITEMPREPAINT����ʾ���б���Ļ滭ǰ�׶Ρ�
	//CDDS_SUBITEM����ʾ�������
	//CDDS_ITEM����ʾҪ���������Ϣ�Ѿ����á�
	NMCUSTOMDRAW &nmcd = lplvdr->nmcd;											//�ͻ��Ի�ؼ���Ϣ�Ľṹ

	switch(lplvdr->nmcd.dwDrawStage)											//Ŀǰ���ƵĲ���
	{
	case CDDS_PREPAINT:															//����������봦�����ұ��뽫pResult����Ϊ CDRF_NOTIFYITEMDRAW, ���򸸴�����ôҲ�ղ��� CDDS_ITEMPREPAINT ֪ͨ��Ϣ (GGH) 
		*pResult = CDRF_NOTIFYITEMDRAW;											//// ���¶�λ��ͼ���ڣ����� TreeCtrl �� DefWindowProc �����ػ�									
		break;

	case CDDS_ITEMPREPAINT:														//���ñ�����ǰ����ɫ
		{
			v_pIter=MapItemColor.find(nmcd.dwItemSpec);							//���ң���Ŀ
			if( v_pIter!=MapItemColor.end())									//�ҵ�������
			{
				lplvdr->clrText = v_pIter->second;								//���ã�������ɫ
				*pResult = CDRF_DODEFAULT;										//���ز��� 
			}			
			
			v_pIter=MapItemBkColor.find(nmcd.dwItemSpec);						//���ң���Ŀ
			if( v_pIter!=MapItemBkColor.end())									//�ҵ�������
			{
				lplvdr->clrTextBk = v_pIter->second;							//���ã�������ɫ
				*pResult = CDRF_DODEFAULT;										//���ز���
			}
		}
		break;

	}
}

//=================================================================================================
//	���ܣ������б�
//=================================================================================================

//-------------------------------------------------------------------------------------------------
//	���ܣ������б���������Ϣ[��������һ����Ϣ]
//	������v_mLine_Data������(������[�к�--����])��
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::Put_Line_Data(mInt_String v_mLine_Data)
{	
	mInt_String				v_mData;									//��ʱ����
	lList_Data::iterator	v_pLine;									//������
	mInt_String::iterator	v_pData;									//������

	for (v_pData=v_mLine_Data.begin();v_pData!=v_mLine_Data.end();v_pData++)	//����
		v_mData.insert(pair<int,CString>(v_pData->first,v_pData->second));		//��������
	v_lList_Data.push_back(v_mData);									//�������
}

//-------------------------------------------------------------------------------------------------
//	���ܣ������б��������ݿ�ʼ
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::Put_Line_Begin()
{	
	//��λ��������Ч
	v_bList_Show = false;												//Ĭ�ϣ�����δ��������ȴ���

	//��λ�������б�����
	lList_Data::iterator v_pIter;										//������
	for (v_pIter=v_lList_Data.begin();v_pIter!=v_lList_Data.end();v_pIter++)	//����
		v_pIter->clear();												//�������
	v_lList_Data.clear();												//�������
	v_mList_Rec.clear();												//�������

	//������б���Ϣ
	DeleteAllItems();													//����б���Ϣ
}

//-------------------------------------------------------------------------------------------------
//	���ܣ������б��������ݽ���
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::Put_Line_End()
{	
	int						v_iNum=0,v_iLen=4;							//��ʱ����
	CString					v_sData;									//��ʱ����
	lList_Data::iterator	v_pLine;									//������
	mInt_String::iterator	v_pData;									//������

	//��0����λ�����--λ��ָ��ӳ��
	v_mList_Rec.clear();												//�������

	//��1����ȡ����ų���
	v_pLine=v_lList_Data.begin();
	if (v_pLine!=v_lList_Data.end())
	{
		v_pData=v_pLine->find(0);										//���ң���0��
		if (v_pData!=v_pLine->end())									//�ҵ�������
		{
			v_sData=v_pData->second;									//��ֵ�������
			v_sData.Trim();												//ȥ���ո�
			v_iLen=v_sData.GetLength();									//��ȡ������ų���
		}
	}

	//��2����ȡ�����--λ��ָ��ӳ��
	for (v_pLine=v_lList_Data.begin();v_pLine!=v_lList_Data.end();v_pLine++)	//����
	{
		//���ã����--λ��ָ��ӳ��
		v_mList_Rec.insert(pair<int,lList_Data::iterator>(v_iNum,v_pLine));		//��������
		v_iNum++;
		
		//�޸����
		v_pData=v_pLine->find(0);										//���ң���0��
		if (v_pData!=v_pLine->end())									//�ҵ�������
		{
			if (v_iLen==4)
				v_sData.Format("%04d",v_iNum);
			else if(v_iLen==5)
				v_sData.Format("%05d",v_iNum);
			else 
				v_sData.Format("%06d",v_iNum);

			v_pData->second=v_sData;									//��ֵ�������
		}
	}

	//��3�����ã��б�������
	CListCtrl::DeleteAllItems();										//���ListCtrl������Item
	SetItemCount(v_lList_Data.size());									//���ã������б�������

	//��4�����ã�������Ч
	v_bList_Show = true;												//��λ������׼���ã����Ի�ȡ��
}

//-------------------------------------------------------------------------------------------------
//	���ܣ������б���ȡ��ʾ������
//	�����������б���Ҫ��Ԫ�ز���(�С���)�������������в�ѯ���ݣ���ֵ���б��С�
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::Get_List_Data(NMLVDISPINFO *pDispInfo)
{	
	bool		v_bOK=false;											//��ǣ���Ч
	int			v_iItem;												//��λ���м�����
	CString		v_sData="";												//��ʱ����

	mList_Rec::iterator		v_pIter;									//������
	mInt_String::iterator	v_pData;									//������

	//��0���������㣿����[����׼����]
	if (!v_bList_Show)													//����׼���ã�����
		return;

	//��1����ȡ���б�����(�У�pItem->iItem���У�pItem->iSubItem��)
	LV_ITEM* pItem= &(pDispInfo)->item;									//��ȡ��Item����ָ��
	v_iItem = pItem->iItem;												//��ȡ����ǰ��Ӧlist����

	//��2����ȡ�������в�������ȡ���������������е�λ��(v_pPM_Time)��
	v_pIter=v_mList_Rec.find(v_iItem);									//���ң���
	if(v_pIter==v_mList_Rec.end())										//û�ҵ�������
		return;															//��Ч����

	//��3����ȡ������
	v_pData=v_pIter->second->find(pItem->iSubItem);						//���ң������
	if (v_pData!=v_pIter->second->end())
	{
		v_sData=v_pData->second;										//��ȡ������Ϣ
		v_bOK=true;														//��ǣ���Ч
	}

	//��4����ֵ
	if(v_bOK && pItem->mask&LVIF_TEXT)									//�ַ�����������Ч
	{
		if (v_sData.GetLength()>250)									//����250???
			v_sData=v_sData.Left(250);									//�ض�

		lstrcpy( pItem->pszText, v_sData);
	}
}

//-------------------------------------------------------------------------------------------------
//	���ܣ�ListCtrl��¼������(��ͷ����)
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::My_Sort_Virtual(NMHDR * pNMHDR)
{
	//��0������ʵ����
	v_bList_Show	= false;											//Ĭ�ϣ�����δ��������ȴ���

	CString v_sTemp;													//��ʱ����
	HDITEM hd,hd2;
	TCHAR  szBuffer[256] = _T("");										//���ݻ�����

	hd.mask = HDI_TEXT;													//hd����������
	hd.pszText = szBuffer;
	hd.cchTextMax = 255;

	hd2.mask = HDI_TEXT;												//hd2����������
	hd2.pszText = szBuffer;
	hd2.cchTextMax = 255;


	//��1�������־������m_bAsc=1 �������m_bAsc=0 ���򨋣�----------------------------------------------
	LPNMLISTVIEW   pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);//�����Ϣ�ṹ��ָ��

	if (pNMListView->iSubItem==0)										//��0�У�����
		return;															//������

	if( pNMListView->iSubItem == m_nSortedCol )							//ͬһ�У�����
		m_bAsc = !m_bAsc;												//ͬһ�У������෴
	else
		m_nSortedCol = pNMListView->iSubItem;							//���棺����������к�

	//��2������ϴ��е�header----------------------------------------------------------------------------	
	if(m_iOldSortedCol != -1)											//�ϴ��к���Ч��
	{
		GetHeaderCtrl()->GetItem(m_iOldSortedCol, &hd2);				//��ȡ�ϴ��е�hearder
		v_sTemp = hd2.pszText;											//��ȡ�ϴ�header�ַ���
		if(v_sTemp.Find("��") != -1 || v_sTemp.Find("��") != -1)		//�������־������
			v_sTemp = v_sTemp.Right(v_sTemp.GetLength() - 2);			//ȥ�������־
		hd2.pszText = (LPSTR)(LPCTSTR)v_sTemp;
		GetHeaderCtrl()->SetItem(m_iOldSortedCol, &hd2);				//�ع�����״̬
	}
	m_iOldSortedCol = m_nSortedCol;										//���棺�ϴ�������к�

	//��3���޸ı����е�header----------------------------------------------------------------------------
	GetHeaderCtrl()->GetItem(m_nSortedCol, &hd);						//��ȡhearder
	v_sTemp = hd.pszText;												//��ȡ�ϴ���header�ַ���
	if(m_bAsc)															//˳���ж� 
		v_sTemp = "��" + v_sTemp;										//����
	else														
		v_sTemp ="��" + v_sTemp;										//����
	hd.pszText = (LPSTR)(LPCTSTR)v_sTemp;												
	GetHeaderCtrl()->SetItem(m_nSortedCol, &hd);						//������header�ַ���

	//��4����������
	v_pList = this;														//��ֵ���б�ָ��
	v_lList_Data.sort(My_Comp);											//��������

	//��5���б�����
	Put_Line_End();														//�б��������ݽ���
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������б�����
//------------------------------------------------------------------------------------------------------
bool CMyListCtrl::My_Comp(mInt_String v_cData_1, mInt_String v_cData_2) 
{
	bool			v_bResult;											//��ʱ����
	CString			v_sText1="",v_sText2="";							//��ʱ����
	double			v_dValue1,v_dValue2;								//���ֱȽ�

	mInt_String::iterator		v_pIter;								//���ݵ�����

	//��1����ȡ��������
	v_pIter=v_cData_1.find(v_pList->m_nSortedCol);						//���ң��к�
	if (v_pIter!=v_cData_1.end())										//�ҵ�
		v_sText1=v_pIter->second;										//����1
	v_pIter=v_cData_2.find(v_pList->m_nSortedCol);						//���ң��к�
	if (v_pIter!=v_cData_2.end())										//�ҵ�
		v_sText2=v_pIter->second;										//����2

	//��2���������ʹ�����[���֡��ַ�]
	if(v_pList->m_bNumber[v_pList->m_nSortedCol])						//�����У�����
	{																	//������
		v_dValue1 = atof(v_sText1);
		v_dValue2 = atof(v_sText2);
		//��3����������/���򣬵ó��ȽϽ��
		if (v_pList->m_bAsc)											//����
			v_bResult=v_dValue1 < v_dValue2;							//
		else															//����
			v_bResult=v_dValue1 > v_dValue2;							//
	}
	else																//�ַ���
	{
		//��3����������/���򣬵ó��ȽϽ��
		if (v_pList->m_bAsc)											//����
			v_bResult=v_sText1 < v_sText2;								//
		else															//����
			v_bResult=v_sText1 > v_sText2;								//
	}

	//��4�����ؽ��
	return v_bResult;													//���ؽ��
}

//-------------------------------------------------------------------------------------------------
// ����������д��CSV�ļ����������б���
// ��	 ����v_cList���б���ָ��
//			 v_sFile_Name��������ļ�����
//			 v_bFile_Open���ļ��򿪱�־��[1���򿪣�0�����򿪣�]
//-------------------------------------------------------------------------------------------------
void CMyListCtrl::CSV_OutPut_Virtual(CString v_sFile_Name,int v_bFile_Open) 
{
	int		v_iNum1,v_iNum3;											//��ʱ����
	int		v_iColumn,v_iRow;											//�б���������������
	bool	v_bOK=false;												//��ʱ����
	CString	v_sStr,v_sCSV,v_sPath_Data,v_sCSV_File;						//��ʱ����
	CFile	v_fWriter;													//ʵ����
	CFileFind	finder;

	//Ŀ¼����
	v_sPath_Data=theApp.m_sPath+"\\DATA";								//�ļ�Ŀ¼
	if (!finder.FindFile(v_sPath_Data))									//���ڣ�
	{
		if (!::CreateDirectory(v_sPath_Data, NULL))						//�����ڣ�����Ŀ¼
		{
			::MessageBox(NULL,"�쳣��"+v_sPath_Data+" Ŀ¼����ʧ�ܡ�", "��ʾ", MB_OK);		//ʧ��
			return;
		}
	}

	try
	{
		for(v_iNum3=0;v_iNum3<100;v_iNum3++)
		{
			if (v_iNum3==0)												//�ַ���
				v_sStr="";
			else
				v_sStr.Format("_%02d",v_iNum3);							//�ַ���
			//����CSV�ļ����ļ�����v_sCSV_File
			if (v_sFile_Name.IsEmpty())
				v_sCSV_File = theApp.m_sPath+"\\DATA\\Excel"+v_sStr+".csv";			//���ã�CSVĬ���ļ�����Excel.csv
			else
				v_sCSV_File = theApp.m_sPath+"\\DATA\\"+v_sFile_Name+v_sStr+".csv";	//���ã�CSVָ�����ļ�����
			if(v_fWriter.Open(v_sCSV_File, CFile::modeCreate | CFile::modeWrite))	//���ļ����粻���������ɣ������������ļ���
			{
				v_bOK=true;												//�ļ���Ч
				break;
			}
		}
		if (v_bOK)														//�ļ���Ч������
		{
			v_iRow = GetItemCount();									//ȡ�б�������
			CHeaderCtrl*  pHeaderCtrl =GetHeaderCtrl();					//�б����ͷ
			v_iColumn=pHeaderCtrl->GetItemCount();						//ȡ�б�������

			HDITEM hdi;													//�нṹ
			TCHAR lpBuffer[256]; 
			hdi.mask = HDI_TEXT; 
			hdi.pszText = lpBuffer; 
			hdi.cchTextMax = 256; 	
			//��ͷ������ȡ�б��Ŀ��������ƣ�����д��CSV
			for(v_iNum1=0;v_iNum1<v_iColumn;v_iNum1++)					//��
			{
				pHeaderCtrl->GetItem(v_iNum1,&hdi);						//ȡList������
				v_sStr=hdi.pszText;										//������
				v_sStr.Replace(" ","");									//�޳��ո�
				if(v_iNum1==0)
					v_sCSV=v_sStr;										//��һ��
				else
					v_sCSV += "," + v_sStr;								//�ڶ��С������С�...����n��
			}
			v_sCSV +="\r\n";											//�س�����
			v_fWriter.Write(v_sCSV,v_sCSV.GetLength());					//д���ַ���

			//�����ݴ�����ȡ�б����ݣ�д��CSV
			lList_Data::iterator	v_pIter;							//������
			mInt_String::iterator	v_pData;							//������
			for (v_pIter=v_lList_Data.begin();v_pIter!=v_lList_Data.end();v_pIter++)	//����
			{
				for (v_pData=v_pIter->begin();v_pData!=v_pIter->end();v_pData++)	//����
				{
					v_sStr=v_pData->second;								//ȡ�б�����
					v_sStr.Replace(",",";");
					if(v_pData->first==0)
						v_sCSV=v_sStr;									//��һ��
					else
						v_sCSV += "," + v_sStr;							//�ڶ��С������С�...����n��
				}
				v_sCSV +="\r\n";										//�س�����
				v_fWriter.Write(v_sCSV,v_sCSV.GetLength());				//д���ַ���
			}
			v_fWriter.Close();											//�ļ��ر�

			//���ļ�
			if (v_bFile_Open)
				ShellExecute(NULL,"open", v_sCSV_File, NULL, NULL, SW_SHOWNORMAL);			//���ļ���Excel.csv
		}
		else
			::MessageBox(NULL,"��ر��Ѿ��򿪵ĵ����ļ��������µ���������", "��ʾ", MB_OK);	//��ʾ��Ϣ
	}	
	catch(_com_error e)
	{
		CString v_sError = (char *)e.Description();						//��ã�������Ϣ
		::MessageBox(NULL,"�쳣"+v_sError, "��ʾ", MB_OK);				//��ʾ��Ϣ
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�������CMyListCtrl
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�ඨ�壺ListCtrlEdit
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//	���캯��
//-------------------------------------------------------------------------------------------------
CMyListCtrlEdit::CMyListCtrlEdit()
{
}

//-------------------------------------------------------------------------------------------------
//	��������
//-------------------------------------------------------------------------------------------------
CMyListCtrlEdit::~CMyListCtrlEdit()
{
}

//-------------------------------------------------------------------------------------------------
//	��Ϣӳ��
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
//	�������ܣ�ʧȥ����
//-------------------------------------------------------------------------------------------------
void CMyListCtrlEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	CWnd* pParent = this->GetParent();									//��ã�������
	::PostMessage(pParent->GetSafeHwnd(),WM_USER_EDIT_END,m_bExchange,0);			//���ͣ��û��༭������Ϣ[��Ч]
}

//-------------------------------------------------------------------------------------------------
//	�������ܣ���ϢԤ���� [��Ҫ����������Ϣ]
//-------------------------------------------------------------------------------------------------
BOOL CMyListCtrlEdit::PreTranslateMessage(MSG* pMsg) 
{
	
	if(pMsg->message == WM_KEYDOWN)										//������Ϣ
	{
		if(pMsg->wParam == VK_RETURN)									//�س���Ϣ
		{
			CWnd* pParent = this->GetParent();							//��ã�������
			m_bExchange = TRUE;
			::PostMessage(pParent->GetSafeHwnd(),WM_USER_EDIT_END,m_bExchange,0);	//���ͣ��û��༭������Ϣ[��Ч]
		}
		else if(pMsg->wParam == VK_ESCAPE)								//Esc��Ϣ
		{
           CWnd* pParent = this->GetParent();							//��ã�������
		   m_bExchange = FALSE;
			::PostMessage(pParent->GetSafeHwnd(),WM_USER_EDIT_END,m_bExchange,0);	//���ͣ��û��༭������Ϣ[�˳�]
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

//-------------------------------------------------------------------------------------------------
//	�������ܣ��۽�
//-------------------------------------------------------------------------------------------------
void CMyListCtrlEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
    m_bExchange = TRUE;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�������CMyListCtrlEdit
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////