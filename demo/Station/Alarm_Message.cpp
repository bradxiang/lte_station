// Alarm_Message.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Station.h"
#include "Alarm_Message.h"


// CAlarm_Message �Ի���

IMPLEMENT_DYNAMIC(CAlarm_Message, CDialog)

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����캯��
//------------------------------------------------------------------------------------------------------
CAlarm_Message::CAlarm_Message(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarm_Message::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���������
//------------------------------------------------------------------------------------------------------
CAlarm_Message::~CAlarm_Message()
{
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ����ӳ��
//------------------------------------------------------------------------------------------------------
void CAlarm_Message::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_cSum);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ���Ļ��Ϣӳ��
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAlarm_Message, CDialog)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

// CAlarm_Message ��Ϣ�������

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի�����ʼ��
//------------------------------------------------------------------------------------------------------
BOOL CAlarm_Message::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������С
	CRect rectWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rectWorkArea,SPIF_SENDCHANGE);   
	// ��öԻ����С
	CRect rectDlg;
	GetWindowRect(&rectDlg);
	int nW = rectDlg.Width();
	int nH = rectDlg.Height();
	// ���������õ����½�
	::SetWindowPos(GetSafeHwnd(),HWND_BOTTOM,rectWorkArea.right-nW-20,rectWorkArea.bottom-nH,nW,nH,SWP_NOZORDER);
	AnimateWindow(1000,AW_VER_NEGATIVE|AW_BLEND|AW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ��Ի����ر�
//------------------------------------------------------------------------------------------------------
void CAlarm_Message::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	ShowWindow(SW_HIDE);
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ����öԻ�������ɫ
//------------------------------------------------------------------------------------------------------
void CAlarm_Message::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: �ڴ˴������Ϣ����������
	CRect   rect;  
	GetClientRect(rect);  
	dc.FillSolidRect(rect,RGB(255,0,0));   //����Ϊ��ɫ����

	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

//------------------------------------------------------------------------------------------------------			
//	�������ܣ������ı���ɫ
//------------------------------------------------------------------------------------------------------
HBRUSH CAlarm_Message::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	switch(nCtlColor)  
	{  
	case CTLCOLOR_STATIC: //�����о�̬�ı��ؼ�������  
		{  
			//���ñ���Ϊ͸��  
//			pDC->SetBkColor(RGB(0,255,0));
//			pDC->SetTextColor(RGB(0,255,0)); //����������ɫ  
			break;  
		}  
	default:  
		break;  
	}  

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

void CAlarm_Message::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: �ڴ˴������Ϣ����������
	ShowWindow(SW_HIDE);												//��ʾԤ������
}
