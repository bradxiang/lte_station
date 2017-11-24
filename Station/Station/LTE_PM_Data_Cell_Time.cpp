// LTE_PM_Data_Cell_Time.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Data_Cell_Time.h"


// CLTE_PM_Data_Cell_Time 对话框

IMPLEMENT_DYNAMIC(CLTE_PM_Data_Cell_Time, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cell_Time::CLTE_PM_Data_Cell_Time(CWnd* pParent /*=NULL*/)
	: CDialog(CLTE_PM_Data_Cell_Time::IDD, pParent)
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cell_Time::~CLTE_PM_Data_Cell_Time()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK2, m_cDate_Check_1);
	DDX_Control(pDX, IDC_CHECK5, m_cDate_Check_2);
	DDX_Control(pDX, IDC_CHECK6, m_cDate_Check_3);
	DDX_Control(pDX, IDC_CHECK7, m_cDate_Check_4);
	DDX_Control(pDX, IDC_CHECK8, m_cDate_Check_5);
	DDX_Control(pDX, IDC_CHECK9, m_cTime_Check_1);
	DDX_Control(pDX, IDC_CHECK10, m_cTime_Check_2);
	DDX_Control(pDX, IDC_CHECK11, m_cTime_Check_3);
	DDX_Control(pDX, IDC_CHECK12, m_cTime_Check_4);
	DDX_Control(pDX, IDC_CHECK13, m_cTime_Check_5);
	DDX_Control(pDX, IDC_COMBO15, m_cTime_Hour1_1);
	DDX_Control(pDX, IDC_COMBO16, m_cTime_Hour1_2);
	DDX_Control(pDX, IDC_COMBO17, m_cTime_Hour1_3);
	DDX_Control(pDX, IDC_COMBO18, m_cTime_Hour1_4);
	DDX_Control(pDX, IDC_COMBO19, m_cTime_Hour1_5);
	DDX_Control(pDX, IDC_COMBO10, m_cTime_Hour2_1);
	DDX_Control(pDX, IDC_COMBO11, m_cTime_Hour2_2);
	DDX_Control(pDX, IDC_COMBO12, m_cTime_Hour2_3);
	DDX_Control(pDX, IDC_COMBO13, m_cTime_Hour2_4);
	DDX_Control(pDX, IDC_COMBO14, m_cTime_Hour2_5);
	DDX_Control(pDX, IDC_BEG_DATE, m_cDate_Date_1);
	DDX_Control(pDX, IDC_BEG_DATE2, m_cDate_Date_2);
	DDX_Control(pDX, IDC_BEG_DATE3, m_cDate_Date_3);
	DDX_Control(pDX, IDC_BEG_DATE4, m_cDate_Date_4);
	DDX_Control(pDX, IDC_BEG_DATE5, m_cDate_Date_5);
	DDX_Control(pDX, IDC_COMBO1, m_cDate_Day_1);
	DDX_Control(pDX, IDC_COMBO3, m_cDate_Day_2);
	DDX_Control(pDX, IDC_COMBO2, m_cDate_Day_3);
	DDX_Control(pDX, IDC_COMBO4, m_cDate_Day_4);
	DDX_Control(pDX, IDC_COMBO5, m_cDate_Day_5);
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Data_Cell_Time, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CLTE_PM_Data_Cell_Time::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK2, &CLTE_PM_Data_Cell_Time::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON9, &CLTE_PM_Data_Cell_Time::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CLTE_PM_Data_Cell_Time::OnBnClickedButton10)
END_MESSAGE_MAP()


// CLTE_PM_Data_Cell_Time 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CLTE_PM_Data_Cell_Time::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int		vi;															//临时变量
	CString	v_sTemp;													//临时变量
		
	//天数设置
	for (vi=1;vi<=c_iSample_Day;vi++)
	{
		v_sTemp.Format("%d",vi);										//字符串
		m_cDate_Day_1.AddString(v_sTemp);								//天数
		m_cDate_Day_2.AddString(v_sTemp);								//天数
		m_cDate_Day_3.AddString(v_sTemp);								//天数
		m_cDate_Day_4.AddString(v_sTemp);								//天数
		m_cDate_Day_5.AddString(v_sTemp);								//天数
	}

	//小时设置
	for (vi=0;vi<24;vi++)
	{
		v_sTemp.Format("%d",vi);										//字符串
		m_cTime_Hour1_1.AddString(v_sTemp);								//开始时间
		m_cTime_Hour1_2.AddString(v_sTemp);								//开始时间
		m_cTime_Hour1_3.AddString(v_sTemp);								//开始时间
		m_cTime_Hour1_4.AddString(v_sTemp);								//开始时间
		m_cTime_Hour1_5.AddString(v_sTemp);								//开始时间
		m_cTime_Hour2_1.AddString(v_sTemp);								//结束时间
		m_cTime_Hour2_2.AddString(v_sTemp);								//结束时间
		m_cTime_Hour2_3.AddString(v_sTemp);								//结束时间
		m_cTime_Hour2_4.AddString(v_sTemp);								//结束时间
		m_cTime_Hour2_5.AddString(v_sTemp);								//结束时间
	}

	//默认设置：根据主窗送来的信息
	m_cDate_Check_1.SetCheck(v_dDate_Select[0].v_bFlag);				//日期1：标志
	m_cDate_Date_1.SetTime(v_dDate_Select[0].v_dDate);					//日期1：日期
	m_cDate_Day_1.SetCurSel(v_dDate_Select[0].v_iDay);					//日期1：天数
	m_cDate_Check_2.SetCheck(v_dDate_Select[1].v_bFlag);				//日期2：标志
	m_cDate_Date_2.SetTime(v_dDate_Select[1].v_dDate);					//日期2：日期
	m_cDate_Day_2.SetCurSel(v_dDate_Select[1].v_iDay);					//日期2：天数
	m_cDate_Check_3.SetCheck(v_dDate_Select[2].v_bFlag);				//日期3：标志
	m_cDate_Date_3.SetTime(v_dDate_Select[2].v_dDate);					//日期3：日期
	m_cDate_Day_3.SetCurSel(v_dDate_Select[2].v_iDay);					//日期3：天数
	m_cDate_Check_4.SetCheck(v_dDate_Select[3].v_bFlag);				//日期4：标志
	m_cDate_Date_4.SetTime(v_dDate_Select[3].v_dDate);					//日期4：日期
	m_cDate_Day_4.SetCurSel(v_dDate_Select[3].v_iDay);					//日期4：天数
	m_cDate_Check_5.SetCheck(v_dDate_Select[4].v_bFlag);				//日期5：标志
	m_cDate_Date_5.SetTime(v_dDate_Select[4].v_dDate);					//日期5：日期
	m_cDate_Day_5.SetCurSel(v_dDate_Select[4].v_iDay);					//日期5：天数

	m_cTime_Check_1.SetCheck(v_dTime_Select[0].v_bFlag);				//时段1：标志
	m_cTime_Hour1_1.SetCurSel(v_dTime_Select[0].v_iHour_1);				//时段1：开始小时
	m_cTime_Hour2_1.SetCurSel(v_dTime_Select[0].v_iHour_2);				//时段1：结束小时
	m_cTime_Check_2.SetCheck(v_dTime_Select[1].v_bFlag);				//时段2：标志
	m_cTime_Hour1_2.SetCurSel(v_dTime_Select[1].v_iHour_1);				//时段2：开始小时
	m_cTime_Hour2_2.SetCurSel(v_dTime_Select[1].v_iHour_2);				//时段2：结束小时
	m_cTime_Check_3.SetCheck(v_dTime_Select[2].v_bFlag);				//时段3：标志
	m_cTime_Hour1_3.SetCurSel(v_dTime_Select[2].v_iHour_1);				//时段3：开始小时
	m_cTime_Hour2_3.SetCurSel(v_dTime_Select[2].v_iHour_2);				//时段3：结束小时
	m_cTime_Check_4.SetCheck(v_dTime_Select[3].v_bFlag);				//时段4：标志
	m_cTime_Hour1_4.SetCurSel(v_dTime_Select[3].v_iHour_1);				//时段4：开始小时
	m_cTime_Hour2_4.SetCurSel(v_dTime_Select[3].v_iHour_2);				//时段4：结束小时
	m_cTime_Check_5.SetCheck(v_dTime_Select[4].v_bFlag);				//时段5：标志
	m_cTime_Hour1_5.SetCurSel(v_dTime_Select[4].v_iHour_1);				//时段5：开始小时
	m_cTime_Hour2_5.SetCurSel(v_dTime_Select[4].v_iHour_2);				//时段5：结束小时

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Data_Cell_Time::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：调整窗体大小
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：关闭窗体
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnClose();													//退出	
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：日期1选择有效【不可无效】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cDate_Check_1.SetCheck(1);										//有效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：日期清除
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cDate_Check_2.SetCheck(false);									//无效
	m_cDate_Check_3.SetCheck(false);									//无效
	m_cDate_Check_4.SetCheck(false);									//无效
	m_cDate_Check_5.SetCheck(false);									//无效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：时段清除
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cTime_Check_1.SetCheck(false);									//无效
	m_cTime_Check_2.SetCheck(false);									//无效
	m_cTime_Check_3.SetCheck(false);									//无效
	m_cTime_Check_4.SetCheck(false);									//无效
	m_cTime_Check_5.SetCheck(false);									//无效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：选定退出
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell_Time::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	int				vi;													//临时变量
	CString			v_sTemp;											//临时变量
	bool			v_bOK=true;											//异常标志

	//【1】异常判断
	mInt_String::iterator	v_pTime;									//迭代器

	v_mTime->clear();													//容器清除
	//时段1
	if (m_cTime_Check_1.GetCheck())										//时段1
	{
		if (m_cTime_Hour1_1.GetCurSel()<=m_cTime_Hour2_1.GetCurSel())	//开始小时<结束小时
		{
			v_sTemp.Format("%02d--%02d",m_cTime_Hour1_1.GetCurSel(),m_cTime_Hour2_1.GetCurSel());	//获取：时段信息【08--10】
			vi=m_cTime_Hour1_1.GetCurSel();								//开始小时
			while(vi<=m_cTime_Hour2_1.GetCurSel())
			{
				v_pTime=v_mTime->find(vi);								//查询小时
				if (v_pTime==v_mTime->end())							//没找到？？？
					v_mTime->insert(pair<int,CString>(vi,v_sTemp));		//增加容器
				else
					v_bOK=false;										//异常

				vi++;													//小时++
			}
		}
		else
			v_bOK=false;												//异常
	}
	//时段2
	if (m_cTime_Check_2.GetCheck())										//时段2
	{
		if (m_cTime_Hour1_2.GetCurSel()<=m_cTime_Hour2_2.GetCurSel())	//开始小时<结束小时
		{
			v_sTemp.Format("%02d--%02d",m_cTime_Hour1_2.GetCurSel(),m_cTime_Hour2_2.GetCurSel());	//获取：时段信息【08--10】
			vi=m_cTime_Hour1_2.GetCurSel();								//开始小时
			while(vi<=m_cTime_Hour2_2.GetCurSel())
			{
				v_pTime=v_mTime->find(vi);								//查询小时
				if (v_pTime==v_mTime->end())							//没找到？？？
					v_mTime->insert(pair<int,CString>(vi,v_sTemp));		//增加容器
				else
					v_bOK=false;										//异常

				vi++;													//小时++
			}
		}
		else
			v_bOK=false;												//异常
	}
	//时段3
	if (m_cTime_Check_3.GetCheck())										//时段3
	{
		if (m_cTime_Hour1_3.GetCurSel()<=m_cTime_Hour2_3.GetCurSel())	//开始小时<结束小时
		{
			v_sTemp.Format("%02d--%02d",m_cTime_Hour1_3.GetCurSel(),m_cTime_Hour2_3.GetCurSel());	//获取：时段信息【08--10】
			vi=m_cTime_Hour1_3.GetCurSel();								//开始小时
			while(vi<=m_cTime_Hour2_3.GetCurSel())
			{
				v_pTime=v_mTime->find(vi);								//查询小时
				if (v_pTime==v_mTime->end())							//没找到？？？
					v_mTime->insert(pair<int,CString>(vi,v_sTemp));		//增加容器
				else
					v_bOK=false;										//异常

				vi++;													//小时++
			}
		}
		else
			v_bOK=false;												//异常
	}
	//时段4
	if (m_cTime_Check_4.GetCheck())										//时段4
	{
		if (m_cTime_Hour1_4.GetCurSel()<=m_cTime_Hour2_4.GetCurSel())	//开始小时<结束小时
		{
			v_sTemp.Format("%02d--%02d",m_cTime_Hour1_4.GetCurSel(),m_cTime_Hour2_4.GetCurSel());	//获取：时段信息【08--10】
			vi=m_cTime_Hour1_4.GetCurSel();								//开始小时
			while(vi<=m_cTime_Hour2_4.GetCurSel())
			{
				v_pTime=v_mTime->find(vi);								//查询小时
				if (v_pTime==v_mTime->end())							//没找到？？？
					v_mTime->insert(pair<int,CString>(vi,v_sTemp));		//增加容器
				else
					v_bOK=false;										//异常

				vi++;													//小时++
			}
		}
		else
			v_bOK=false;												//异常
	}
	//时段5
	if (m_cTime_Check_5.GetCheck())										//时段5
	{
		if (m_cTime_Hour1_5.GetCurSel()<=m_cTime_Hour2_5.GetCurSel())	//开始小时<结束小时
		{
			v_sTemp.Format("%02d--%02d",m_cTime_Hour1_5.GetCurSel(),m_cTime_Hour2_5.GetCurSel());	//获取：时段信息【08--10】
			vi=m_cTime_Hour1_5.GetCurSel();								//开始小时
			while(vi<=m_cTime_Hour2_5.GetCurSel())
			{
				v_pTime=v_mTime->find(vi);								//查询小时
				if (v_pTime==v_mTime->end())							//没找到？？？
					v_mTime->insert(pair<int,CString>(vi,v_sTemp));		//增加容器
				else
					v_bOK=false;										//异常

				vi++;													//小时++
			}
		}
		else
			v_bOK=false;												//异常
	}
	//异常判断
	if(!v_bOK)
	{
		v_mTime->clear();												//容器清除
		MessageBox("时段信息异常，开始大于结束或者小时重复，请重新选择 ！！！");		//窗体提示	
		return;															//返回
	}

	//【2】日期获取
//	v_dDate_Select[0].v_bFlag=m_cDate_Check_1.GetCheck();				//日期1：有效标志
	m_cDate_Date_1.GetTime(v_dDate_Select[0].v_dDate);					//日期1：开始日期
	v_dDate_Select[0].v_iDay=m_cDate_Day_1.GetCurSel();					//日期1：天数
	v_dDate_Select[1].v_bFlag=m_cDate_Check_2.GetCheck();				//日期2：有效标志
	m_cDate_Date_2.GetTime(v_dDate_Select[1].v_dDate);					//日期2：开始日期
	v_dDate_Select[1].v_iDay=m_cDate_Day_2.GetCurSel();					//日期2：天数
	v_dDate_Select[2].v_bFlag=m_cDate_Check_3.GetCheck();				//日期3：有效标志
	m_cDate_Date_3.GetTime(v_dDate_Select[2].v_dDate);					//日期3：开始日期
	v_dDate_Select[2].v_iDay=m_cDate_Day_3.GetCurSel();					//日期3：天数
	v_dDate_Select[3].v_bFlag=m_cDate_Check_4.GetCheck();				//日期4：有效标志
	m_cDate_Date_4.GetTime(v_dDate_Select[3].v_dDate);					//日期4：开始日期
	v_dDate_Select[3].v_iDay=m_cDate_Day_4.GetCurSel();					//日期4：天数
	v_dDate_Select[4].v_bFlag=m_cDate_Check_5.GetCheck();				//日期5：有效标志
	m_cDate_Date_5.GetTime(v_dDate_Select[4].v_dDate);					//日期5：开始日期
	v_dDate_Select[4].v_iDay=m_cDate_Day_5.GetCurSel();					//日期5：天数


	//【3】时段获取
	v_dTime_Select[0].v_bFlag=m_cTime_Check_1.GetCheck();				//时段1：有效标志
	v_dTime_Select[0].v_iHour_1=m_cTime_Hour1_1.GetCurSel();			//时段1：开始小时
	v_dTime_Select[0].v_iHour_2=m_cTime_Hour2_1.GetCurSel();			//时段1：结束小时
	v_dTime_Select[1].v_bFlag=m_cTime_Check_2.GetCheck();				//时段2：有效标志
	v_dTime_Select[1].v_iHour_1=m_cTime_Hour1_2.GetCurSel();			//时段2：开始小时
	v_dTime_Select[1].v_iHour_2=m_cTime_Hour2_2.GetCurSel();			//时段2：结束小时
	v_dTime_Select[2].v_bFlag=m_cTime_Check_3.GetCheck();				//时段3：有效标志
	v_dTime_Select[2].v_iHour_1=m_cTime_Hour1_3.GetCurSel();			//时段3：开始小时
	v_dTime_Select[2].v_iHour_2=m_cTime_Hour2_3.GetCurSel();			//时段3：结束小时
	v_dTime_Select[3].v_bFlag=m_cTime_Check_4.GetCheck();				//时段4：有效标志
	v_dTime_Select[3].v_iHour_1=m_cTime_Hour1_4.GetCurSel();			//时段4：开始小时
	v_dTime_Select[3].v_iHour_2=m_cTime_Hour2_4.GetCurSel();			//时段4：结束小时
	v_dTime_Select[4].v_bFlag=m_cTime_Check_5.GetCheck();				//时段5：有效标志
	v_dTime_Select[4].v_iHour_1=m_cTime_Hour1_5.GetCurSel();			//时段5：开始小时
	v_dTime_Select[4].v_iHour_2=m_cTime_Hour2_5.GetCurSel();			//时段5：结束小时

	//【4】退出
	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
