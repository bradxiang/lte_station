// LTE_PM_Wait.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Wait.h"


// CLTE_PM_Wait 对话框

IMPLEMENT_DYNAMIC(CLTE_PM_Wait, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Wait::CLTE_PM_Wait(CWnd* pParent /*=NULL*/)
	: CDialog(CLTE_PM_Wait::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Wait::~CLTE_PM_Wait()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Wait::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cName);
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Wait, CDialog)
	ON_WM_TIMER()
//	ON_WM_CLOSE()
//ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLTE_PM_Wait 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CLTE_PM_Wait::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(1,100,NULL);												//定时1：100ms(检查线程工作状态)

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
//	消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Wait::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		CDialog* Pwnd=(CDialog*)GetActiveWindow();						//取得对话框指针 
		Pwnd->NextDlgCtrl();											//切换到下一个输入焦点 
		return true;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	功能：定时器
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Wait::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1:
		if (v_iParent==0)												//小区性能查询
		{
			if (v_pLTE_PM_Data_Cell->v_pIterator_Frame->second.v_iThread_Exit!=0)	//线程工作？？？
				m_cName.SetWindowText(v_pLTE_PM_Data_Cell->v_sPM_Working);			//显示：线程工作进展
			else
				PostMessage(WM_CLOSE,0,0);								//发送消息：强制退出
			break;
		}
		else if(v_iParent==1)											//实时性能查询
		{
			if (v_pLTE_PM_Data_Real->v_pIterator_Frame->second.v_iThread_Exit!=0)	//线程工作？？？
				m_cName.SetWindowText(v_pLTE_PM_Data_Real->v_sPM_Working);			//显示：线程工作进展
			else
				PostMessage(WM_CLOSE,0,0);								//发送消息：强制退出
			break;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
