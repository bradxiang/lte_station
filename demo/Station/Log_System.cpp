// Log_System.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "Log_System.h"

// CLog_System

IMPLEMENT_DYNCREATE(CLog_System, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLog_System::CLog_System()
	: CFormView(CLog_System::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLog_System::~CLog_System()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CLog_System::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_LOG_SUM, m_cLog_Sum);
	DDX_Control(pDX, IDC_BEG_DATE, m_cLog_Date);
	DDX_Control(pDX, IDC_COMBO1, m_cLog_Day);
	DDX_Control(pDX, IDC_KEY, m_cLog_Key);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLog_System, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CLog_System::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL, &CLog_System::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CLog_System::OnLvnColumnclickList1)
END_MESSAGE_MAP()


// CLog_System 诊断

#ifdef _DEBUG
void CLog_System::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLog_System::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLog_System 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CLog_System::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="系统日志查询";										//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText("系统日志查询");					//子窗标题设置

	//初始化：列表窗KPI
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"日    期",LVCFMT_CENTER,140);  
	m_cList.InsertColumn(2,"类型",LVCFMT_LEFT,160);  
	m_cList.InsertColumn(3,"日志内容",LVCFMT_LEFT,1000);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//变量初始化
	m_cLog_Day.SetCurSel(4);											//默认：查询天数
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：子窗销毁
//------------------------------------------------------------------------------------------------------
void CLog_System::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器

	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//查找子窗名称
	if(v_pIterator_Frame != theApp.v_mFrame.end())						//子窗存在？
		theApp.v_mFrame.erase(v_pIterator_Frame);						//在容器中清除子窗名称
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：调整窗体大小
//------------------------------------------------------------------------------------------------------
void CLog_System::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd	*pCellList;													//临时变量
	int		v_iY;														//临时变量

	pCellList = GetDlgItem(IDC_LIST1);
	v_iY = 50;															//赋值：Y轴
	if(pCellList)
		pCellList->MoveWindow(14,v_iY,cx-24,cy-v_iY-10);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CLog_System::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)	//键盘消息？？？
	{
		// Translate dialog key if applicable
		if(::IsDialogMessage(m_hWnd, pMsg))								//转换消息 [把消息发到m_hWnd]
			return TRUE;												//消息结束 [不会把该消息分发给窗口函数处理]
	}

	return CFormView::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击查询按钮
//------------------------------------------------------------------------------------------------------
void CLog_System::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);													//获取屏幕信息

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导出Excel
//------------------------------------------------------------------------------------------------------
void CLog_System::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Log列表排序(右击列表任意列)
//------------------------------------------------------------------------------------------------------			
void CLog_System::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CLog_System::My_Thread_Inoput(LPVOID lparam)
{
	CLog_System *  lp_this = NULL ;
	lp_this = (CLog_System *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CLog_System::My_Input_Main()
{
	m_cList.DeleteAllItems();											//清除列表数据
	My_LoadData();														//装载Log信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载Log信息
//------------------------------------------------------------------------------------------------------			
void CLog_System::My_LoadData()
{
	CString		v_sSql;													//临时变量
	int			v_iItem,v_iID=0;										//KPI数量
	CString		v_sKPI_Name;											//
	CString		v_sTemp;												//临时变量
	COleDateTime	v_dDate_Start,v_dDate_End;							//查询开始日期、结束日期
	int				v_iDay_Sum;											//查询天数
	CString		v_sTime_Start,v_sTime_End;								//日期
	CString		v_sLog_Content,v_sLog_Type;								//日志内容
	CString		v_sLog_Key;												//日志查询：KEY

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		//查询日期准备
		m_cLog_Date.GetTime(v_dDate_Start);								//获得：查询开始日期
		v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//设置时间：00:00:00
		v_iDay_Sum=m_cLog_Day.GetCurSel()+1;							//获得：查询天数
		v_dDate_End =v_dDate_Start + COleDateTimeSpan(v_iDay_Sum,0,0,0);//获得：查询结束日期
		v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";		//开始时间
		v_sTime_End=v_dDate_End.Format("%Y-%m-%d")+" 00:00:00";			//结束时间
		m_cLog_Key.GetWindowText(v_sLog_Key);							//获得：查询KEY
		v_sLog_Key.Trim();												//去掉空格

		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		if (v_sLog_Key.IsEmpty())										//有关键字查询？？？
			v_sSql="select * from BAS_Log where zTime>='"+v_sTime_Start+"' and zTime<='"+v_sTime_End+"' order by zTime";	//查询数据库：系统日志
		else
			v_sSql="select * from BAS_Log where zTime>='"+v_sTime_Start+"' and zTime<='"+v_sTime_End+"' and zLog_Content like '%"+v_sLog_Key+"%' order by zTime";	//查询数据库：系统日志

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_dDate_Start=v_cAdoConn.GetField_DataTime("zTime");		//zTime
			v_sLog_Type=v_cAdoConn.GetField_String("zLog_Type");		//zLog_Type
			v_sLog_Content=v_cAdoConn.GetField_String("zLog_Content");	//zLog_Content

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d_%H:%M:%S");	//赋值：日期
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sTime_Start);		//
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sLog_Type);		//
			m_cList.SetItemText(v_iItem,3,(_bstr_t)v_sLog_Content);		//

			v_pRecordset->MoveNext();									//
			v_iID++;													//
			
			if (v_iID%100==0)
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cLog_Sum.SetWindowText(v_sTemp);						//显示：Log总数
			}
		}

		v_sTemp.Format("%d",v_iID);										//
		m_cLog_Sum.SetWindowText(v_sTemp);								//显示：Log总数

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
