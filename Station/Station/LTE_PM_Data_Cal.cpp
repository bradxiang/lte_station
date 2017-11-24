// LTE_PM_Data_Cal.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Data_Cal.h"
#include "LTE_PM_DATA_CASE.h"											//模板【小区、性能】
#include "LTE_PM_Data_Cal_Select.h"

// CLTE_PM_Data_Cal

IMPLEMENT_DYNCREATE(CLTE_PM_Data_Cal, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cal::CLTE_PM_Data_Cal()
	: CFormView(CLTE_PM_Data_Cal::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cal::~CLTE_PM_Data_Cal()
{
	v_mLTE_PM_Type.clear();												//容器清除
	v_mTable_Name.clear();												//容器清除
	v_mCell.clear();													//容器清除
	v_mCell_ID.clear();													//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_LOG_SUM, m_cLog_Sum);
	DDX_Control(pDX, IDC_BEG_DATE, m_cLog_Date);
	DDX_Control(pDX, IDC_COMBO1, m_cLog_Day);
	DDX_Control(pDX, IDC_KEY, m_cLog_Key);
	DDX_Control(pDX, IDC_BUTTON1, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_LIST3, m_cList_PM);
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_LOG_SUM2, m_cPM_Sum);
	DDX_Control(pDX, IDC_LOG_SUM3, m_cData_Sum);
	DDX_Control(pDX, IDC_CHECK2, m_cCheck);
	DDX_Control(pDX, IDC_BUTTON5, m_cCell_Case);
	DDX_Control(pDX, IDC_EXCEL2, m_cPM_Cal);
	DDX_Control(pDX, IDC_KEY3, m_cCal);
	DDX_Control(pDX, IDC_LOG_SUM4, m_cCase_Type);
	DDX_Control(pDX, IDC_BUTTON2, m_cCal_Select);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Data_Cal, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL, &CLTE_PM_Data_Cal::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CLTE_PM_Data_Cal::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Data_Cal::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Data_Cal::OnNMClickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CLTE_PM_Data_Cal::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_BUTTON5, &CLTE_PM_Data_Cal::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK2, &CLTE_PM_Data_Cal::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_EXCEL2, &CLTE_PM_Data_Cal::OnBnClickedExcel2)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST3, &CLTE_PM_Data_Cal::OnLvnGetdispinfoList3)
	ON_BN_CLICKED(IDC_BUTTON2, &CLTE_PM_Data_Cal::OnBnClickedButton2)
END_MESSAGE_MAP()


// CLTE_PM_Data_Cal 诊断

#ifdef _DEBUG
void CLTE_PM_Data_Cal::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLTE_PM_Data_Cal::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLTE_PM_Data_Cal 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CString v_sTemp;													//临时变量
	//窗体名称
	v_sFrame_Name="LTE 性能数据计算_小区";								//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗【小区】
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList.InsertColumn(1,"小区",LVCFMT_LEFT,200);  
	m_cList.InsertColumn(2,"小区ID",LVCFMT_LEFT,0);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES );

	//初始化：列表窗【性能】
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList_PM.InsertColumn(1,"小区",LVCFMT_LEFT,140);					//第2列：小区
	m_cList_PM.InsertColumn(2,"日期",LVCFMT_CENTER,140);				//第3列：日期
	m_cList_PM.InsertColumn(3,"计算结果",LVCFMT_RIGHT,80);				//第4列：计算性能
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//分窗 [先生成左边的上下关系的水平分隔符，再生成左右关系的垂直分隔符，即2*1]
	CRect	v_cRect;													//矩形
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//获得：指针
	pWnd->GetWindowRect(&v_cRect);										//获得：尺寸
	ScreenToClient(&v_cRect);											//坐标变换
	pWnd->DestroyWindow();												//销毁：指针

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Main.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_cList,				// Left pane
		&m_cList_PM,			// Right pane
		IDC_LTE_PM_Data_Cal_Main,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);

	//刷新界面
	PostMessage(WM_SIZE);

	//变量初始化
	m_cType.AddString("15m粒度");
	m_cType.AddString("1H粒度");
	m_cType.SetCurSel(1);												//默认：1H粒度

	//初始化：天数选择
	for (int vi=1;vi<=c_iSample_Day;vi++)								//最长天数
	{
		v_sTemp.Format("%d",vi);
		m_cLog_Day.AddString(v_sTemp);									//可选天数复选钮
	}
	m_cLog_Day.SetCurSel(0);											//默认查询天数：1天；

	m_cLog_Day.SetCurSel(4);											//默认：查询天数
	v_iList_Item=-1;													//默认：无选中的列表Item
	v_sCase_Name="计算结果";											//默认：性能名称

	//获取：性能参数
	v_iWorking=3;														//工作任务：获取初始数据
	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：子窗销毁
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnDestroy()
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
void CLTE_PM_Data_Cal::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//分窗尺寸调整
		CRect v_cRect;
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=70;
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Data_Cal::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：导出Excel[计算性能]
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_PM);											//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表排序(右击列表任意列)【小区】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cal::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表排序(右击列表任意列)【性能：虚拟列表】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cal::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	m_cList_PM.My_Sort_Virtual(pNMHDR);									//列表排序：虚拟列表

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：虚拟列表消息响应函数
//	工	  作：依据列表需要的元素参数(行、列)，在数据容器中查询数据，赋值到列表中。
//------------------------------------------------------------------------------------------------------	
void CLTE_PM_Data_Cal::OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取：虚拟列表显示数据获取
	m_cList_PM.Get_List_Data(pDispInfo);								//虚拟列表显示数据获取

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：全选【小区】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//遍历
		m_cList.SetCheck(vi,m_cCheck.GetCheck());						//设置状态
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击查询按钮【小区查询】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iList_Item=-1;													//小区选择无效

	v_iWorking=1;														//工作任务：小区查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：小区模板【获取：小区信息，放入m_cList】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp;												//临时变量
	int			v_iItem,v_iID=0;										//临时变量

	LTE_PM_DATA_CASE	v_Dlg;											//实例化
	v_Dlg.v_iSelect=0;													//标志：小区

	if (v_Dlg.DoModal()==IDOK)											//确认返回？？？
	{
		//选择结束：小区信息在容器中(v_Dlg.v_mCell);
		//工作：【1】获取小区；【2】放入v_mList，加入Cell_ID；
		m_cList.DeleteAllItems();										//删除：模板信息
		v_iList_Item=-1;												//复位：条目序号

		mString_String::iterator	v_pIter_Cell;						//迭代器
		set<CString>::iterator		v_pIter;							//迭代器

		//遍历容器
		for(v_pIter=v_Dlg.v_mCell.begin();v_pIter!=v_Dlg.v_mCell.end();v_pIter++)	//遍历：模板信息列表
		{
			//有效小区
			v_pIter_Cell=v_mCell.find(*v_pIter);						//查找：小区存在？？？
			if (v_pIter_Cell==v_mCell.end())							//不存在
				continue;												//下一小区

			//放入列表
			v_sTemp.Format(" %05d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,v_pIter_Cell->first);			//显示：小区
			m_cList.SetItemText(v_iItem,2,v_pIter_Cell->second);		//显示：小区ID
			m_cList.SetCheck(v_iID,TRUE); 
			v_iID++;
		}																//
		v_sTemp.Format("%d",v_iID);										//
		m_cLog_Sum.SetWindowText(v_sTemp);								//显示：Log总数

		m_cCheck.SetCheck(true);										//有效：全选
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击小区信息【性能查询】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item>=0)											//有效条目序号？？？
				m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));	//恢复背景色
			v_iList_Item=pNMListView->iItem;							//获得：选中条目序号
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));			//设置背景色：红色
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能选择【自定义计算性能】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cal::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp,v_sFormula,v_sType;								//临时变量

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	CLTE_PM_Data_Cal_Select v_Dlg;	

	//【1】打开性能选择对话窗
	v_Dlg.v_mLTE_PM_Type=&v_mLTE_PM_Type;								//性能指针
	if (v_Dlg.DoModal()==IDOK)											//确认返回？？？
	{
		//【2】对话窗确认返回：获取选中的性能参数
		v_sFormula=v_Dlg.v_sFormula;									//赋值：性能类型
		v_sCase_Name = v_Dlg.v_sName;									//赋值：模板名称
		v_sType=v_Dlg.v_sType;											//赋值：性能类型
		m_cCal.SetWindowText(v_sFormula);								//赋值：计算公式
		m_cCase_Type.SetWindowText(v_sType);							//赋值：性能类型

		//【3】公式检查(公式中的性能是否在选定性的性能集中)
		v_cFormula.v_mFormula.clear();									//队列清除
		v_cFormula.My_Get_Formula(v_sFormula);							//分解公式表达式(结果在队列中)

		v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);					//查找：性能类型
		//清除性能选择状态
		for(v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();
			v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();
			v_pPM_Name++)
			v_pPM_Name->second.v_bOK=false;								//性能：无效
		//设置性能选择状态[依据计算公式]
		for (v_cFormula.v_pFormula=v_cFormula.v_mFormula.begin();
			v_cFormula.v_pFormula!=v_cFormula.v_mFormula.end();
			v_cFormula.v_pFormula++)									//遍历公式元素
		{
			if (v_cFormula.v_pFormula->v_bFormula==1)					//运算值？？？
			{
				v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.find(v_cFormula.v_pFormula->v_sPM_Formula);	//查找：公式元素是否是原始性能？？？
				if (v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())	//找到？？？
					v_pPM_Name->second.v_bOK=true;						//性能：有效
			}
		}

		//【4】删除列表信息
		m_cList_PM.DeleteAllItems();									//删除所有条目
		while(m_cList_PM.DeleteColumn(0));								//删除所有列：ObjType信息列表

		//【5】在ObjType信息列表添加信息
		m_cList_PM.InsertColumn(0,"序号",LVCFMT_CENTER,50);				//第1列：序号
		m_cList_PM.InsertColumn(1,"小区",LVCFMT_LEFT,140);				//第2列：小区
		m_cList_PM.InsertColumn(2,"日期",LVCFMT_CENTER,140);			//第3列：日期
		m_cList_PM.InsertColumn(3,v_sCase_Name,LVCFMT_RIGHT,80);		//第4列：计算性能名称

		//【6】加入性能名称
		int v_iNum=4;
		m_cCase_Type.GetWindowText(v_sType);							//获得：性能类型
		v_sType.Trim();													//去掉空格
		v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);					//查找：性能类型
		v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();			//首指针
		while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//结束？？
		{
			if (v_pPM_Name->second.v_bOK)								//性能选中？？？
			{
				m_cList_PM.InsertColumn(v_iNum,v_pPM_Name->second.v_sPM_Name,LVCFMT_RIGHT,60);	//第n列：性能
				m_cList_PM.SetNumber(v_iNum);							//列属性：数字
				v_iNum++;												//计数器
			}
			v_pPM_Name++;												//递加
		}

		//【7】显示：性能数量
		v_sTemp.Format("%d",v_pLTE_PM_Type->second.v_mLTE_PM.size());	//性能数量
		m_cPM_Sum.SetWindowText(v_sTemp);								//显示：性能数量
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能计算
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=2;														//工作任务：性能查询，并计算
	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CLTE_PM_Data_Cal::My_Thread_Inoput(LPVOID lparam)
{
	CLTE_PM_Data_Cal *  lp_this = NULL ;
	lp_this = (CLTE_PM_Data_Cal *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cLog_Date.EnableWindow(false);							//日期：无效
	lp_this->m_cLog_Day.EnableWindow(false);							//天数：无效
	lp_this->m_cType.EnableWindow(false);								//查询类型：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效
	lp_this->m_cCell_Case.EnableWindow(false);							//小区模板：无效
	lp_this->m_cPM_Cal.EnableWindow(false);								//性能计算：无效
	lp_this->m_cCal_Select.EnableWindow(false);							//性能计算选择：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效
	lp_this->m_cLog_Date.EnableWindow(true);							//日期：有效
	lp_this->m_cLog_Day.EnableWindow(true);								//天数：有效
	lp_this->m_cType.EnableWindow(true);								//查询类型：有效
	lp_this->m_cCell_Case.EnableWindow(true);							//小区模板：有效
	lp_this->m_cPM_Cal.EnableWindow(true);								//性能计算：有效
	lp_this->m_cCal_Select.EnableWindow(true);							//性能计算选择：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cal::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Cell();												//装载Cell信息
		break;
	case 2:
		My_LoadData_PM();												//装载性能信息
		break;
	case 3:
		My_LoadData_Para();												//获取：性能参数
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能信息
//	依    据：【表名称：15m/1H粒度；性能类型；】【行：小区、日期】【列：性能类型】
//	性 能 表：15mins：每天一个表；  Hour：5天一个表，一月六个表
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cal::My_LoadData_PM()
{
	CString			v_sTemp;											//临时变量
	int				v_iDay_Sum;											//临时变量
	COleDateTime	v_dDate_Start,v_dDate_End,v_dDate;					//查询开始日期、结束日期
	CString			v_sCell="",v_sTime_Start,v_sTime_End,v_sTable_Num;	//临时变量

	//【1】显示清除：清除列表性能信息
	m_cData_Sum.SetWindowText("");										//清除：性能数据总数
	v_iPM_Sum=0;														//复位：获取的性能总数

	//【2】小区获取：选中的小区ID (数据查询)
	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//遍历列表
	{
		if(m_cList.GetCheck(vi))										//判断：选中状态？？？
		{
			if (!v_sCell.IsEmpty())										//不为空？？？
				v_sCell+=",";											//增加：分隔符
			v_sCell+="'"+m_cList.GetItemText(vi,2)+"'";					//获取小区ID
		}
	}
	if(v_sCell.IsEmpty())												//为空？？？
	{
		MessageBox("  请选择有效的小区 !!! ");							//窗体提示
		return;
	}

	//【3】复位：虚拟列表
	m_cList_PM.Put_Line_Begin();										//列表添加数据开始

	//【4】查询日期
	m_cLog_Date.GetTime(v_dDate_Start);									//获得：查询开始日期
	v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//设置时间：00:00:00
	v_iDay_Sum=m_cLog_Day.GetCurSel()+1;								//获得：查询天数
	v_dDate_End =v_dDate_Start + COleDateTimeSpan(v_iDay_Sum,0,0,0);	//获得：查询结束日期

	//【5】性能查询：15mins：每天一个表；  Hour：5天一个表，一月六个表
	if (m_cType.GetCurSel()==0)											//15m粒度
	{
		while(v_dDate_Start<v_dDate_End)								//开始日期<结束日期
		{
			v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//开始时间
			v_sTime_End=v_dDate_Start.Format("%Y-%m-%d")+" 23:59:59";	//结束时间
			v_sTemp=v_dDate_Start.Format("%Y%m%d");						//年月日
			v_sTemp=My_Get_Table()+"_"+v_sTemp;							//文件名称
			My_LoadData_PM_Data(v_sTemp,v_sCell,v_sTime_Start,v_sTime_End);		//装载性能数据
			v_dDate_Start+=COleDateTimeSpan(1,0,0,0);					//增加一天
		}
	}
	else
	{
		v_dDate=v_dDate_Start;											//赋值：当前日期
		v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());			//获取表序号
		while(v_dDate<v_dDate_End)										//时间范围内？？？
		{
			v_dDate+=COleDateTimeSpan(1,0,0,0);							//增加一天
			if (My_Get_Table_Num(v_dDate.GetDay())!=v_sTable_Num || v_dDate>=v_dDate_End)
			{
				v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";		//开始时间
				v_sTime_End=v_dDate.Format("%Y-%m-%d")+" 00:00:00";		//结束时间
				v_sTemp=v_dDate_Start.Format("%Y%m");					//年月
				v_sTemp=My_Get_Table()+"_"+v_sTemp+"_"+v_sTable_Num;	//文件名称
				My_LoadData_PM_Data(v_sTemp,v_sCell,v_sTime_Start,v_sTime_End);		//装载性能数据
				v_dDate_Start=v_dDate;									//变更：开始时间
				v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());	//获取表序号
			}
		}
	}

	//【6】设置：虚拟列表有效
	m_cList_PM.Put_Line_End();											//列表添加数据结束
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取表名称
//	返    回：数据表名称【6个性能类型之一】
//	依    据：性能类型
//------------------------------------------------------------------------------------------------------			
CString CLTE_PM_Data_Cal::My_Get_Table()
{
	CString		v_sTemp;												//临时变量
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器

	//获取：性能名称
	m_cCase_Type.GetWindowText(v_sTemp);								//获得：性能类型
	v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sTemp);						//查找：性能类型

	v_sTemp = "Pm_LTE_"+v_pLTE_PM_Type->second.v_sTable_Key;			//表名称
	return v_sTemp;														//返回
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取表序号
//	工	  作：1：1-5；	2：6-10；	3：11-15；	4：16-20；	5：21-25；	6：26--
//------------------------------------------------------------------------------------------------------			
CString CLTE_PM_Data_Cal::My_Get_Table_Num(int v_iDay)
{
	CString	v_sNum;

	if (v_iDay<=5)
		v_sNum="1";
	else if(v_iDay<=10)
		v_sNum="2";
	else if(v_iDay<=15)
		v_sNum="3";
	else if(v_iDay<=20)
		v_sNum="4";
	else if(v_iDay<=25)
		v_sNum="5";
	else 
		v_sNum="6";

	return v_sNum;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能数据
//	参	  数：v_sTable：表名称；	v_sCell：小区；	v_sDate_Ben：开始日期；		v_sDate_End：结束日期
//	依    据：【表名称：15m/1H粒度；性能类型；】【行：小区、日期】【列：性能类型】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cal::My_LoadData_PM_Data(CString v_sTable,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End)
{
	CString			v_sSql;												//临时变量
	CString			v_sTemp,v_sTime,v_sCell_ID,v_sFormula;				//临时变量
	int				v_iID=0,v_iNum,v_iCell_ID;							//临时变量
	COleDateTime	v_tDate;											//日期
	double			v_dValue;											//性能数值

	mString_Double	v_mPM_Cell;											//容器

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器
	map_Cell_ID::iterator	v_pIter_Cell;								//迭代器
	mString_Double::iterator	v_pIter_Value;							//迭代器

	//虚拟列表
	mInt_String		v_mLine_Data;										//虚拟列表行数据

	_variant_t		Var;
	_RecordsetPtr	v_pRecordset;										//记录集
	CMyADOConn		v_cAdoConn;											//数据库实例
	try
	{
		//【1】获取：性能指针
		m_cCase_Type.GetWindowText(v_sTemp);							//获得：性能类型
		v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sTemp);					//查找：性能类型
		if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())						//没找到
			return;														//无效返回

		//【2】检查：表名称是否有效【依据：表名称集合v_mTable_Name】
		set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//查找？？
		if (v_pTable==v_mTable_Name.end())								//没找到
			return;														//无效返回

		//【3】连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select * from "+v_sTable+" where zCell in("+v_sCell+") and zTime>='"+v_sDate_Ben
				+"' and zTime<'"+v_sDate_End+"' order by zCell,zTime";	//查询数据库：系统日志

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		//【4】获取性能数据
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			//数据复位
			v_mLine_Data.clear();										//容器清除

			v_tDate=v_cAdoConn.GetField_DataTime("zTime");				//zTime
			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_iCell_ID=Var.intVal;									//获取：Cell_ID
				//由ID==>Cell
				v_pIter_Cell=v_mCell_ID.find(v_iCell_ID);				//查找：小区ID存在？？？
				if (v_pIter_Cell==v_mCell_ID.end())						//不存在
				{
					v_pRecordset->MoveNext();							//下一粒度性能
					continue;											//继续
				}
				v_sCell_ID=v_pIter_Cell->second.v_sCell;				//小区
			}
			else
				v_sCell_ID="无效";										//无效小区


			//【5】显示：小区、日期
			v_sTemp.Format(" %06d",v_iPM_Sum+1);
			v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));			//序号
			v_mLine_Data.insert(pair<int,CString>(1,v_sCell_ID));		//小区
			v_sTime=v_tDate.Format("%Y-%m-%d_%H:%M:%S");				//赋值：日期
			v_mLine_Data.insert(pair<int,CString>(2,v_sTime));			//日期


			//【6】显示：性能
			v_mPM_Cell.clear();											//容器清除
			v_iNum=4;
			v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();		//首指针
			while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())	//结束？？
			{
				if (v_pPM_Name->second.v_bOK)							//性能选中???
				{
					v_dValue=v_cAdoConn.GetField_Double(v_pPM_Name->first);		//获取：性能
					v_sTemp.Format("%10.4f",v_dValue);							//字符串
					v_mLine_Data.insert(pair<int,CString>(v_iNum,v_sTemp));		//原始性能[准备：虚拟列表]

					v_mPM_Cell.insert(pair<CString,double>(v_pPM_Name->first,v_dValue));	//容器增加：计算公式
					v_iNum++;											//计数器
				}

				v_pPM_Name++;											//递加
			}

			//【7】性能计算
			//从计算公式里获取需要获取的Counter值【v_mFormula里value为1的为运算值，即Counter名称】
			v_sFormula="";												//复位：计算公式
			for (v_cFormula.v_pFormula=v_cFormula.v_mFormula.begin();
				v_cFormula.v_pFormula!=v_cFormula.v_mFormula.end();
				v_cFormula.v_pFormula++)								//公式遍历
			{
				if (v_cFormula.v_pFormula->v_bFormula==1)				//变量
				{
					v_pIter_Value=v_mPM_Cell.find(v_cFormula.v_pFormula->v_sPM_Formula);	//查找：Counter
					if (v_pIter_Value!=v_mPM_Cell.end())				//找到？？？
					{
						v_sTemp.Format("%f",v_pIter_Value->second);		//字符串
						v_sFormula+=v_sTemp;							//运算值
					}
					else
						v_sFormula+="0";								//运算值
				}
				else
					v_sFormula+=v_cFormula.v_pFormula->v_sPM_Formula;	//运算符
			}
			//计算
			float v_fValue=v_cFormula.simcalc(v_sFormula.GetBuffer(0));	//公式计算
			//显示
			v_sTemp.Format("%8.2f",v_fValue);							//字符串
			v_mLine_Data.insert(pair<int,CString>(3,v_sTemp));			//计算性能

			//配置数据放入列表
			m_cList_PM.Put_Line_Data(v_mLine_Data);						//列表添加添加行数据

			v_pRecordset->MoveNext();									//下一条

			v_iPM_Sum++;												//++
			if ((v_iPM_Sum)%100==0)
			{
				v_sTemp.Format("%d",v_iPM_Sum);							//
				m_cData_Sum.SetWindowText(v_sTemp);						//显示：性能数据总数
			}
		}

		v_sTemp.Format("%d",v_iPM_Sum);									//
		m_cData_Sum.SetWindowText(v_sTemp);								//显示：性能数据总数

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	//字段
	v_mPM_Cell.clear();													//容器清除

	//虚拟列表
	v_mLine_Data.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载Cell信息
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cal::My_LoadData_Cell()
{
	CString		v_sSql;													//临时变量
	int			v_iItem,v_iID=0,v_iCell_ID;								//临时变量
	CString		v_sTemp,v_sCell,v_sCell_ID;								//临时变量
	CString		v_sLog_Key;												//日志查询：KEY

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList.DeleteAllItems();										//清除列表数据

		//查询日期准备
		m_cLog_Key.GetWindowText(v_sLog_Key);							//获得：查询KEY
		v_sLog_Key.Replace("[","[[]");									//限制处理
		v_sLog_Key.Replace("'","");										//限制处理
		v_sLog_Key.Trim();												//去掉空格

		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zCell_ID from LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB where LTE_NODEB.zRNC like '%dalian%'";							//查询数据库
		if (!v_sLog_Key.IsEmpty())										//有关键字查询？？？
			v_sSql+=" and zCell like '%"+v_sLog_Key+"%'";				//查询数据库
		v_sSql+=" order by zCell";										//查询数据库

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sCell=v_cAdoConn.GetField_String("zCell");				//zCell
			v_iCell_ID=v_cAdoConn.GetField_Int("zCell_ID");				//zCell_ID

			v_sTemp.Format(" %05d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sCell);			//
			v_sCell_ID.Format("%d",v_iCell_ID);							//字符串
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sCell_ID);			//Cell_ID

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
//	函数功能：装载性能参数信息
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cal::My_LoadData_Para()
{
	int				v_iID=0,v_iCell_ID;									//临时变量
	CString			v_sTemp,v_sSql,v_sLog_Key,v_sTable;					//临时变量
	CString			v_sType,v_sType_1,v_sPm,v_sPm_Name,v_sTable_Key;	//临时变量
	CString			v_sCell,v_sCell_ID,v_sCell_Name,v_sRegion,v_sOnline;//临时变量
	Cell_ID_Struct	v_cCell_ID;											//结构

	LTE_PM_Struct	v_cLTE_PM;											//结构
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	LTE_PM_Type_Struct		v_cLTE_PM_Type;								//结构

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//【1】获取：性能参数
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE order by ztype,ztype_1,zPm";//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sType_1 = v_cAdoConn.GetField_String("ztype_1");			//ztype_1
			v_sPm = v_cAdoConn.GetField_String("zPm");					//zPm
			v_sPm_Name = v_cAdoConn.GetField_String("zPm_Name");		//zPm_Name
			v_sTable_Key = v_cAdoConn.GetField_String("zTable_Key");	//zTable_Key

			//保存
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//查找：性能类型
			if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())					//没找到？？？
			{
				v_cLTE_PM_Type.v_sTable_Key=v_sTable_Key;				//表名称-Key
				v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//容器增加
			}
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//查找：性能类型
			v_cLTE_PM.v_sPM_Name=v_sPm_Name;							//性能：中文名
			v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//增加：性能参数

			v_pRecordset->MoveNext();									//
		}

		//【2】获取：性能数据表名称【用于发现无效表名称】
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and name like 'Pm_LTE_%' order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//表名称

			v_mTable_Name.insert(v_sTable);								//增加集合

			v_pRecordset->MoveNext();									//指针++
		}	

		//【3】获取：小区信息【用于小区ID处理】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zCell_ID,zCell_Name,LTE_Cell.zOnline,LTE_Cell.zRegion \
				from LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB \
				where LTE_NODEB.zRNC like '%dalian%'";					//查询数据库
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sCell=v_cAdoConn.GetField_String("zCell");				//zCell
			v_iCell_ID=v_cAdoConn.GetField_Int("zCell_ID");				//zCell_ID
			v_sCell_ID.Format("%d",v_iCell_ID);							//字符串
			v_sCell_Name=v_cAdoConn.GetField_String("zCell_Name");		//zCell_Name
			v_sOnline=v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_sRegion=v_cAdoConn.GetField_String("zRegion");			//zRegion

			v_mCell.insert(pair<CString,CString>(v_sCell,v_sCell_ID));	//容器增加
			v_cCell_ID.v_sCell=v_sCell;									//小区
			v_cCell_ID.v_sCell_Name=v_sCell_Name;						//小区中文名
			v_cCell_ID.v_sOnline=v_sOnline;								//状态
			v_cCell_ID.v_sRegion=v_sRegion;								//营业区
			v_mCell_ID.insert(pair<int,Cell_ID_Struct>(v_iCell_ID,v_cCell_ID));	//容器增加

			v_pRecordset->MoveNext();									//指针++
		}	

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
