
// StationView.h : CStaionView ��Ľӿ�
//


#pragma once


class CStaionView : public CView
{
protected: // �������л�����
	CStaionView();
	DECLARE_DYNCREATE(CStaionView)

// ����
public:
	CStaionDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CStaionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // StationView.cpp �еĵ��԰汾
inline CStaionDoc* CStaionView::GetDocument() const
   { return reinterpret_cast<CStaionDoc*>(m_pDocument); }
#endif

