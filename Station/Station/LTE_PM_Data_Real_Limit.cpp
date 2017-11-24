// LTE_PM_Data_Real_Limit.cpp : 实现文件
// 


#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Data_Real_Limit.h"


// CLTE_PM_Data_Real_Limit 对话框

IMPLEMENT_DYNAMIC(CLTE_PM_Data_Real_Limit, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Real_Limit::CLTE_PM_Data_Real_Limit(CWnd* pParent /*=NULL*/)
: CDialog(CLTE_PM_Data_Real_Limit::IDD, pParent)
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Real_Limit::~CLTE_PM_Data_Real_Limit()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real_Limit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_EDIT1, m_cSum);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_CHECK1, m_cPm_Flag);
	DDX_Control(pDX, IDC_COMBO3, m_cPm_Type);
	DDX_Control(pDX, IDC_BUTTON1, m_cUpdate);
	DDX_Control(pDX, IDC_EDIT3, m_cPm_Name);
	DDX_Control(pDX, IDC_EDIT2, m_cPm_Value);
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Data_Real_Limit, CDialog)
	ON_BN_CLICKED(IDC_EXCEL, &CLTE_PM_Data_Real_Limit::OnBnClickedExcel)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Data_Real_Limit::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Data_Real_Limit::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLTE_PM_Data_Real_Limit 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CLTE_PM_Data_Real_Limit::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化：列表窗
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList.InsertColumn(1,"类型",LVCFMT_LEFT,100);						//
	m_cList.InsertColumn(2,"性能",LVCFMT_LEFT,270);						//
	m_cList.InsertColumn(3,"预警",LVCFMT_CENTER,80);					//
	m_cList.InsertColumn(4,"预警门限",LVCFMT_RIGHT,80);					//
	m_cList.InsertColumn(5,"预警类型",LVCFMT_CENTER,80);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//变量初始化
	v_iItem=-1;															//条目选择无效[-1]
	m_cPm_Type.AddString("高");
	m_cPm_Type.AddString("低");
	m_cPm_Type.SetCurSel(0);											//默认
	m_cUpdate.EnableWindow(false);										//无效

	//信息显示：默认选中的性能
	CString					v_sTemp;									//临时变量
	int						v_iItem;									//临时变量
	mLTE_PM::iterator		v_pPM_Name;									//迭代器
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	LTE_PM_Struct			v_cPM_Data;									//结构
	map<int,LTE_PM_Struct>	v_mPM_Data;									//容器
	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//首指针
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//结束？？
	{
		v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();			//首指针
		while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//结束？？
		{
			if (v_pPM_Name->second.v_bOK)
			{
				v_cPM_Data.v_sType = v_pLTE_PM_Type->first;				//性能类型
				v_cPM_Data.v_sPM_Name=v_pPM_Name->second.v_sPM_Name;	//性能名称
				v_cPM_Data.v_sAlarm_Flag=v_pPM_Name->second.v_sAlarm_Flag;		//预警标志
				v_cPM_Data.v_dAlarm_Value=v_pPM_Name->second.v_dAlarm_Value;	//预警门限
				v_cPM_Data.v_sAlarm_Type=v_pPM_Name->second.v_sAlarm_Type;		//预警类型
				v_mPM_Data.insert(pair<int,LTE_PM_Struct>(v_pPM_Name->second.v_iNum,v_cPM_Data));	//增加：选中条目
			}
			v_pPM_Name++;												//递加
		}
		v_pLTE_PM_Type++;												//递加
	}
	map<int,LTE_PM_Struct>::iterator	v_pPM_Data;						//迭代器
	for (v_pPM_Data=v_mPM_Data.begin();v_pPM_Data!=v_mPM_Data.end();v_pPM_Data++)
	{
		v_iItem = m_cList.InsertItem(0xffff,_T("0"));
		v_sTemp.Format("%04d",v_iItem+1);
		m_cList.SetItemText(v_iItem,0," "+v_sTemp);						//序号
		m_cList.SetItemText(v_iItem,1,v_pPM_Data->second.v_sType);		//性能类型
		m_cList.SetItemText(v_iItem,2,v_pPM_Data->second.v_sPM_Name);	//性能名称
		m_cList.SetItemText(v_iItem,3,v_pPM_Data->second.v_sAlarm_Flag);//预警标志
		v_sTemp.Format("%8.2f",v_pPM_Data->second.v_dAlarm_Value);		//字符串
		m_cList.SetItemText(v_iItem,4,v_sTemp);							//预警门限
		m_cList.SetItemText(v_iItem,5,v_pPM_Data->second.v_sAlarm_Type);//预警类型
	}
	v_sTemp.Format("%d",m_cList.GetItemCount());						//字符串
	m_cSum.SetWindowText(v_sTemp);										//性能数量
	
	v_mPM_Data.clear();													//容器清除

	return TRUE;  // return TRUE unless you set the focus to a control
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Data_Real_Limit::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：调整窗体尺寸
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real_Limit::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导出Excel
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real_Limit::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
	m_cList.SetFocus();													//聚焦
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击告警类型列表
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real_Limit::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp;												//临时变量

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if(v_iItem>=0)													//有效条目序号？？？
			m_cList.SetItemBkColor(v_iItem,RGB(255,255,255));			//恢复背景色
		v_iItem=pNMListView->iItem;										//获得：选中条目序号
		m_cList.SetItemBkColor(v_iItem,RGB(255,0,0));					//设置背景色：红色
		
		//设置：修改内容
		m_cPm_Name.SetWindowText(m_cList.GetItemText(v_iItem,2));		//性能名称
		if (m_cList.GetItemText(v_iItem,3)=="预警")
			m_cPm_Flag.SetCheck(1);										//预警
		else
			m_cPm_Flag.SetCheck(0);										//不预警
		v_sTemp=m_cList.GetItemText(v_iItem,4);							//预警门限
		v_sTemp.Trim();
		m_cPm_Value.SetWindowText(v_sTemp);								//预警门限
		if (m_cList.GetItemText(v_iItem,5)=="高")
			m_cPm_Type.SetCurSel(0);									//高
		else
			m_cPm_Type.SetCurSel(1);									//低
		m_cUpdate.EnableWindow(true);									//有效
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：预警配置修改
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real_Limit::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp,v_sSql;											//临时变量
	CString		v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;				//临时变量
	CString		v_sType,v_sPM_Name;										//临时变量

	//获取
	v_sAlarm_Flag=m_cPm_Flag.GetCheck()?"预警":"不预警";				//预警有效
	m_cPm_Value.GetWindowText(v_sAlarm_Value);							//预警门限
	v_sAlarm_Type=m_cPm_Type.GetCurSel()==0?"高":"低";					//预警类型
	
	v_sType=m_cList.GetItemText(v_iItem,1);								//性能类型
	v_sPM_Name=m_cList.GetItemText(v_iItem,2);							//性能名称

	//显示
	m_cList.SetItemText(v_iItem,3,v_sAlarm_Flag);						//
	m_cList.SetItemText(v_iItem,4,v_sAlarm_Value);						//
	m_cList.SetItemText(v_iItem,5,v_sAlarm_Type);						//

	//保存
	mLTE_PM::iterator		v_pPM_Name;									//迭代器
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	v_pLTE_PM_Type=v_mLTE_PM_Type->begin();								//首指针
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())						//结束？？
	{
		v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();			//首指针
		while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//结束？？
		{
			if (v_pLTE_PM_Type->first==v_sType && v_pPM_Name->second.v_sPM_Name==v_sPM_Name)	//类型 && 名称 相等
			{
				v_pPM_Name->second.v_sAlarm_Flag=v_sAlarm_Flag;			//预警标志
				v_pPM_Name->second.v_dAlarm_Value=atof(v_sAlarm_Value);	//预警门限
				v_pPM_Name->second.v_sAlarm_Type=v_sAlarm_Type;			//预警类型
				break;													//退出
			}
			v_pPM_Name++;												//递加
		}
		v_pLTE_PM_Type++;												//递加
	}
	m_cUpdate.EnableWindow(false);										//无效
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
