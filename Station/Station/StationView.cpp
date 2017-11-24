
// StationView.cpp : CStaionView ���ʵ��
//

#include "stdafx.h"
#include "Station.h"

#include "StationDoc.h"
#include "StationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStaionView

IMPLEMENT_DYNCREATE(CStaionView, CView)

BEGIN_MESSAGE_MAP(CStaionView, CView)
END_MESSAGE_MAP()

// CStaionView ����/����

CStaionView::CStaionView()
{
	// TODO: �ڴ˴���ӹ������

}

CStaionView::~CStaionView()
{
}

BOOL CStaionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CStaionView ����

void CStaionView::OnDraw(CDC* /*pDC*/)
{
	CStaionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CStaionView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CStaionView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CStaionView ���

#ifdef _DEBUG
void CStaionView::AssertValid() const
{
	CView::AssertValid();
}

void CStaionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CStaionDoc* CStaionView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStaionDoc)));
	return (CStaionDoc*)m_pDocument;
}
#endif //_DEBUG


// CStaionView ��Ϣ�������
