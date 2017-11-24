// Alarm_LL_Cell_Time.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "Alarm_LL_Cell_Time.h"


// CAlarm_LL_Cell_Time 对话框

IMPLEMENT_DYNAMIC(CAlarm_LL_Cell_Time, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Cell_Time::CAlarm_LL_Cell_Time(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarm_LL_Cell_Time::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Cell_Time::~CAlarm_LL_Cell_Time()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell_Time::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK9, m_cDown);
	DDX_Control(pDX, IDC_CHECK10, m_cUp);
	DDX_Control(pDX, IDC_CHECK2, m_cTime_1);
	DDX_Control(pDX, IDC_CHECK5, m_cTime_2);
	DDX_Control(pDX, IDC_CHECK6, m_cTime_3);
	DDX_Control(pDX, IDC_CHECK7, m_cTime_4);
	DDX_Control(pDX, IDC_CHECK8, m_cTime_5);
	DDX_Control(pDX, IDC_BEG_DATE, m_cDate_1);
	DDX_Control(pDX, IDC_BEG_DATE2, m_cDate_2);
	DDX_Control(pDX, IDC_BEG_DATE3, m_cDate_3);
	DDX_Control(pDX, IDC_BEG_DATE4, m_cDate_4);
	DDX_Control(pDX, IDC_BEG_DATE5, m_cDate_5);
	DDX_Control(pDX, IDC_COMBO15, m_cHour_1);
	DDX_Control(pDX, IDC_COMBO16, m_cHour_2);
	DDX_Control(pDX, IDC_COMBO17, m_cHour_3);
	DDX_Control(pDX, IDC_COMBO18, m_cHour_4);
	DDX_Control(pDX, IDC_COMBO19, m_cHour_5);
	DDX_Control(pDX, IDC_COMBO10, m_cCal_1);
	DDX_Control(pDX, IDC_COMBO11, m_cCal_2);
	DDX_Control(pDX, IDC_COMBO12, m_cCal_3);
	DDX_Control(pDX, IDC_COMBO13, m_cCal_4);
	DDX_Control(pDX, IDC_COMBO14, m_cCal_5);
	DDX_Control(pDX, IDC_KEY, m_cValues_1);
	DDX_Control(pDX, IDC_KEY2, m_cValues_2);
	DDX_Control(pDX, IDC_KEY3, m_cValues_3);
	DDX_Control(pDX, IDC_KEY4, m_cValues_4);
	DDX_Control(pDX, IDC_KEY5, m_cValues_5);
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAlarm_LL_Cell_Time, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CAlarm_LL_Cell_Time::OnBnClickedButton2)
END_MESSAGE_MAP()


// CAlarm_LL_Cell_Time 消息处理程序


//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CAlarm_LL_Cell_Time::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int		vi;															//临时变量
	CString	v_sTemp;													//临时变量

	//设置：小时
	for (vi=0;vi<24;vi++)
	{
		v_sTemp.Format("%d",vi);										//字符串
		m_cHour_1.AddString(v_sTemp);									//小时
		m_cHour_2.AddString(v_sTemp);									//小时
		m_cHour_3.AddString(v_sTemp);									//小时
		m_cHour_4.AddString(v_sTemp);									//小时
		m_cHour_5.AddString(v_sTemp);									//小时
	}

	//设置：大于小于等号
	m_cCal_1.AddString("<");											//小于
	m_cCal_1.AddString(">");											//大于
	m_cCal_1.AddString("=");											//等于
	m_cCal_2.AddString("<");											//小于
	m_cCal_2.AddString(">");											//大于
	m_cCal_2.AddString("=");											//等于
	m_cCal_3.AddString("<");											//小于
	m_cCal_3.AddString(">");											//大于
	m_cCal_3.AddString("=");											//等于
	m_cCal_4.AddString("<");											//小于
	m_cCal_4.AddString(">");											//大于
	m_cCal_4.AddString("=");											//等于
	m_cCal_5.AddString("<");											//小于
	m_cCal_5.AddString(">");											//大于
	m_cCal_5.AddString("=");											//等于

	//显示参数【依据已经选择的参数】
	m_cDown.SetCheck(v_bDown);											//状态：下行
	m_cUp.SetCheck(v_bUp);												//状态：上行

	//粒度有效
	m_cTime_1.SetCheck(v_dDate_Select[0].v_bFlag);						//标志：粒度
	m_cTime_2.SetCheck(v_dDate_Select[1].v_bFlag);						//标志：粒度
	m_cTime_3.SetCheck(v_dDate_Select[2].v_bFlag);						//标志：粒度
	m_cTime_4.SetCheck(v_dDate_Select[3].v_bFlag);						//标志：粒度
	m_cTime_5.SetCheck(v_dDate_Select[4].v_bFlag);						//标志：粒度

	//日期
	m_cDate_1.SetTime(v_dDate_Select[0].v_dDate);						//设置：日期
	m_cDate_2.SetTime(v_dDate_Select[1].v_dDate);						//设置：日期
	m_cDate_3.SetTime(v_dDate_Select[2].v_dDate);						//设置：日期
	m_cDate_4.SetTime(v_dDate_Select[3].v_dDate);						//设置：日期
	m_cDate_5.SetTime(v_dDate_Select[4].v_dDate);						//设置：日期

	//小时
	m_cHour_1.SetCurSel(v_dDate_Select[0].v_iHour);						//设置：小时
	m_cHour_2.SetCurSel(v_dDate_Select[1].v_iHour);						//设置：小时
	m_cHour_3.SetCurSel(v_dDate_Select[2].v_iHour);						//设置：小时
	m_cHour_4.SetCurSel(v_dDate_Select[3].v_iHour);						//设置：小时
	m_cHour_5.SetCurSel(v_dDate_Select[4].v_iHour);						//设置：小时

	//比较符号
	m_cCal_1.SetCurSel(v_dDate_Select[0].v_iCal);						//设置：比较符号
	m_cCal_2.SetCurSel(v_dDate_Select[1].v_iCal);						//设置：比较符号
	m_cCal_3.SetCurSel(v_dDate_Select[2].v_iCal);						//设置：比较符号
	m_cCal_4.SetCurSel(v_dDate_Select[3].v_iCal);						//设置：比较符号
	m_cCal_5.SetCurSel(v_dDate_Select[4].v_iCal);						//设置：比较符号

	//数值
	v_sTemp.Format("%8.2f",v_dDate_Select[0].v_dValues);				//字符串
	m_cValues_1.SetWindowText(v_sTemp);									//设置：数值
	v_sTemp.Format("%8.2f",v_dDate_Select[1].v_dValues);				//字符串
	m_cValues_2.SetWindowText(v_sTemp);									//设置：数值
	v_sTemp.Format("%8.2f",v_dDate_Select[2].v_dValues);				//字符串
	m_cValues_3.SetWindowText(v_sTemp);									//设置：数值
	v_sTemp.Format("%8.2f",v_dDate_Select[3].v_dValues);				//字符串
	m_cValues_4.SetWindowText(v_sTemp);									//设置：数值
	v_sTemp.Format("%8.2f",v_dDate_Select[4].v_dValues);				//字符串
	m_cValues_5.SetWindowText(v_sTemp);									//设置：数值
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CAlarm_LL_Cell_Time::PreTranslateMessage(MSG* pMsg)
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
// 消息预处理：关闭对话窗
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Cell_Time::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：选定退出
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell_Time::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString	v_sTemp;													//临时变量

	//获取：下行、上行状态
	v_bDown=m_cDown.GetCheck();											//状态：下行
	v_bUp=m_cUp.GetCheck();												//状态：上行

	//获取：粒度状态
	v_dDate_Select[0].v_bFlag=m_cTime_1.GetCheck();						//获取：状态
	v_dDate_Select[1].v_bFlag=m_cTime_2.GetCheck();						//获取：状态
	v_dDate_Select[2].v_bFlag=m_cTime_3.GetCheck();						//获取：状态
	v_dDate_Select[3].v_bFlag=m_cTime_4.GetCheck();						//获取：状态
	v_dDate_Select[4].v_bFlag=m_cTime_5.GetCheck();						//获取：状态

	//获取：日期
	m_cDate_1.GetTime(v_dDate_Select[0].v_dDate);						//获取：日期
	m_cDate_2.GetTime(v_dDate_Select[1].v_dDate);						//获取：日期
	m_cDate_3.GetTime(v_dDate_Select[2].v_dDate);						//获取：日期
	m_cDate_4.GetTime(v_dDate_Select[3].v_dDate);						//获取：日期
	m_cDate_5.GetTime(v_dDate_Select[4].v_dDate);						//获取：日期

	//获取：小时
	v_dDate_Select[0].v_iHour=m_cHour_1.GetCurSel();					//获取：小时
	v_dDate_Select[1].v_iHour=m_cHour_2.GetCurSel();					//获取：小时
	v_dDate_Select[2].v_iHour=m_cHour_3.GetCurSel();					//获取：小时
	v_dDate_Select[3].v_iHour=m_cHour_4.GetCurSel();					//获取：小时
	v_dDate_Select[4].v_iHour=m_cHour_5.GetCurSel();					//获取：小时

	//获取：比较符
	v_dDate_Select[0].v_iCal=m_cCal_1.GetCurSel();						//获取：比较符
	v_dDate_Select[1].v_iCal=m_cCal_2.GetCurSel();						//获取：比较符
	v_dDate_Select[2].v_iCal=m_cCal_3.GetCurSel();						//获取：比较符
	v_dDate_Select[3].v_iCal=m_cCal_4.GetCurSel();						//获取：比较符
	v_dDate_Select[4].v_iCal=m_cCal_5.GetCurSel();						//获取：比较符

	//获取：数值
	m_cValues_1.GetWindowText(v_sTemp);									//获取：数值
	v_dDate_Select[0].v_dValues=atof(v_sTemp);							//字符串-->flote
	m_cValues_2.GetWindowText(v_sTemp);									//获取：数值
	v_dDate_Select[1].v_dValues=atof(v_sTemp);							//字符串-->flote
	m_cValues_3.GetWindowText(v_sTemp);									//获取：数值
	v_dDate_Select[2].v_dValues=atof(v_sTemp);							//字符串-->flote
	m_cValues_4.GetWindowText(v_sTemp);									//获取：数值
	v_dDate_Select[3].v_dValues=atof(v_sTemp);							//字符串-->flote
	m_cValues_5.GetWindowText(v_sTemp);									//获取：数值
	v_dDate_Select[4].v_dValues=atof(v_sTemp);							//字符串-->flote

	//退出
	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
