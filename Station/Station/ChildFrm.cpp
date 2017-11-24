
// ChildFrm.cpp : CChildFrame 类的实现
//

#include "stdafx.h"
#include "Station.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CChildFrame 构造/析构

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CChildFrame::CChildFrame()
{
	// TODO: 在此添加成员初始化代码
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CChildFrame::~CChildFrame()
{
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：PreCreateWindow
//------------------------------------------------------------------------------------------------------
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：关闭窗体
//------------------------------------------------------------------------------------------------------
void CChildFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString		v_sTitle;												//临时变量

	CView *m_pView=GetActiveView();										//获取：窗体指针
	m_pView->GetParentFrame()->GetWindowText(v_sTitle);					//获得：子窗标题
	
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器
	v_pIterator_Frame = theApp.v_mFrame.find(v_sTitle);					//查找：子窗名称？？？
	if(v_pIterator_Frame != theApp.v_mFrame.end())						//找到？？？
	{
 		if (v_pIterator_Frame->second.v_iThread_Exit==1)				//线程运行？？？
 		{
 			v_pIterator_Frame->second.v_iThread_Exit=2;					//置标记：线程强制退出；
 			return;
 		}
 	}
	CMDIChildWndEx::OnClose();
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
