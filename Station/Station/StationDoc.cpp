
// StationDoc.cpp : CStaionDoc ���ʵ��
//

#include "stdafx.h"
#include "Station.h"

#include "StationDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStaionDoc

IMPLEMENT_DYNCREATE(CStaionDoc, CDocument)

BEGIN_MESSAGE_MAP(CStaionDoc, CDocument)
END_MESSAGE_MAP()


// CStaionDoc ����/����

CStaionDoc::CStaionDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CStaionDoc::~CStaionDoc()
{
}

BOOL CStaionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CStaionDoc ���л�

void CStaionDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CStaionDoc ���

#ifdef _DEBUG
void CStaionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStaionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStaionDoc ����
