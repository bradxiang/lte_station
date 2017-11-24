
// StationDoc.cpp : CStaionDoc 类的实现
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


// CStaionDoc 构造/析构

CStaionDoc::CStaionDoc()
{
	// TODO: 在此添加一次性构造代码

}

CStaionDoc::~CStaionDoc()
{
}

BOOL CStaionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CStaionDoc 序列化

void CStaionDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CStaionDoc 诊断

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


// CStaionDoc 命令
