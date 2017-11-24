// Alarm_Message.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "Alarm_Message.h"


// CAlarm_Message 对话框

IMPLEMENT_DYNAMIC(CAlarm_Message, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CAlarm_Message::CAlarm_Message(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarm_Message::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CAlarm_Message::~CAlarm_Message()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CAlarm_Message::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_cSum);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAlarm_Message, CDialog)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

// CAlarm_Message 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CAlarm_Message::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 获得桌面大小
	CRect rectWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rectWorkArea,SPIF_SENDCHANGE);   
	// 获得对话框大小
	CRect rectDlg;
	GetWindowRect(&rectDlg);
	int nW = rectDlg.Width();
	int nH = rectDlg.Height();
	// 将窗口设置到右下角
	::SetWindowPos(GetSafeHwnd(),HWND_BOTTOM,rectWorkArea.right-nW-20,rectWorkArea.bottom-nH,nW,nH,SWP_NOZORDER);
	AnimateWindow(1000,AW_VER_NEGATIVE|AW_BLEND|AW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗关闭
//------------------------------------------------------------------------------------------------------
void CAlarm_Message::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	ShowWindow(SW_HIDE);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：设置对话窗背景色
//------------------------------------------------------------------------------------------------------
void CAlarm_Message::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: 在此处添加消息处理程序代码
	CRect   rect;  
	GetClientRect(rect);  
	dc.FillSolidRect(rect,RGB(255,0,0));   //设置为绿色背景

	// 不为绘图消息调用 CDialog::OnPaint()
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：设置文本颜色
//------------------------------------------------------------------------------------------------------
HBRUSH CAlarm_Message::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	switch(nCtlColor)  
	{  
	case CTLCOLOR_STATIC: //对所有静态文本控件的设置  
		{  
			//设置背景为透明  
//			pDC->SetBkColor(RGB(0,255,0));
//			pDC->SetTextColor(RGB(0,255,0)); //设置字体颜色  
			break;  
		}  
	default:  
		break;  
	}  

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

void CAlarm_Message::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	ShowWindow(SW_HIDE);												//显示预警窗体
}
