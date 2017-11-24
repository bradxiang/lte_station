
// ChildFrm.cpp : CChildFrame ���ʵ��
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

// CChildFrame ����/����

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CChildFrame::CChildFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CChildFrame::~CChildFrame()
{
}


//------------------------------------------------------------------------------------------------------			
//	�������ܣ�PreCreateWindow
//------------------------------------------------------------------------------------------------------
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame ���

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

// CChildFrame ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��رմ���
//------------------------------------------------------------------------------------------------------
void CChildFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString		v_sTitle;												//��ʱ����

	CView *m_pView=GetActiveView();										//��ȡ������ָ��
	m_pView->GetParentFrame()->GetWindowText(v_sTitle);					//��ã��Ӵ�����
	
	mFrame_Window::iterator	v_pIterator_Frame;							//������
	v_pIterator_Frame = theApp.v_mFrame.find(v_sTitle);					//���ң��Ӵ����ƣ�����
	if(v_pIterator_Frame != theApp.v_mFrame.end())						//�ҵ�������
	{
 		if (v_pIterator_Frame->second.v_iThread_Exit==1)				//�߳����У�����
 		{
 			v_pIterator_Frame->second.v_iThread_Exit=2;					//�ñ�ǣ��߳�ǿ���˳���
 			return;
 		}
 	}
	CMDIChildWndEx::OnClose();
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
