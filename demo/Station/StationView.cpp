
// StationView.cpp : CStaionView 类的实现
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

// CStaionView 构造/析构

CStaionView::CStaionView()
{
	// TODO: 在此处添加构造代码

}

CStaionView::~CStaionView()
{
}

BOOL CStaionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CStaionView 绘制

void CStaionView::OnDraw(CDC* /*pDC*/)
{
	CStaionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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


// CStaionView 诊断

#ifdef _DEBUG
void CStaionView::AssertValid() const
{
	CView::AssertValid();
}

void CStaionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CStaionDoc* CStaionView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStaionDoc)));
	return (CStaionDoc*)m_pDocument;
}
#endif //_DEBUG


// CStaionView 消息处理程序
