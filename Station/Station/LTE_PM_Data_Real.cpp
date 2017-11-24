// LTE_PM_Data_Real.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Data_Real.h"
#include "LTE_PM_Draw_Znit_Select.h"									//性能选择
#include "LTE_PM_DATA_CASE.h"											//模板【小区、性能】
#include "LTE_PM_Data_Real_Limit.h"										//预警设置

// CLTE_PM_Data_Real

IMPLEMENT_DYNCREATE(CLTE_PM_Data_Real, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Real::CLTE_PM_Data_Real()
	: CFormView(CLTE_PM_Data_Real::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Real::~CLTE_PM_Data_Real()
{
	v_mLTE_PM_Type.clear();												//容器清除
	v_mTable_Name.clear();												//容器清除
	v_mPM_Field_Num.clear();											//容器清除
	v_mCell.clear();													//容器清除
	v_mCell_ID.clear();													//容器清除
	v_mCell_Real_Alarm.clear();											//容器清除
	v_dDate_Real.clear();												//容器清除
	My_PM_Data_Delete();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_LOG_SUM, m_cLog_Sum);
	DDX_Control(pDX, IDC_KEY, m_cLog_Key);
	DDX_Control(pDX, IDC_BUTTON1, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_LIST3, m_cList_PM);
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_LOG_SUM2, m_cPM_Sum);
	DDX_Control(pDX, IDC_CHECK2, m_cCheck);
	DDX_Control(pDX, IDC_EXCEL3, m_cPM_Cell);
	DDX_Control(pDX, IDC_EXCEL4, m_cCase_Cell);
	DDX_Control(pDX, IDC_EXCEL5, m_cCase_PM);
	DDX_Control(pDX, IDC_EXCEL2, m_cCase_Select);
	DDX_Control(pDX, IDC_LIST9, m_cList_Tj);
	DDX_Control(pDX, IDC_EXCEL8, m_cExcel_Tj);
	DDX_Control(pDX, IDC_CHECK3, m_cAuto);
	DDX_Control(pDX, IDC_TIME, m_cTime);
	DDX_Control(pDX, IDC_EXCEL6, m_cAlarm);
	DDX_Control(pDX, IDC_CHECK5, m_cAlarm_All);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Data_Real, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL, &CLTE_PM_Data_Real::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CLTE_PM_Data_Real::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Data_Real::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Data_Real::OnNMClickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CLTE_PM_Data_Real::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_EXCEL2, &CLTE_PM_Data_Real::OnBnClickedExcel2)
	ON_BN_CLICKED(IDC_EXCEL3, &CLTE_PM_Data_Real::OnBnClickedExcel3)
	ON_BN_CLICKED(IDC_EXCEL4, &CLTE_PM_Data_Real::OnBnClickedExcel4)
	ON_BN_CLICKED(IDC_EXCEL5, &CLTE_PM_Data_Real::OnBnClickedExcel5)
	ON_BN_CLICKED(IDC_CHECK2, &CLTE_PM_Data_Real::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_EXCEL8, &CLTE_PM_Data_Real::OnBnClickedExcel8)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_EXCEL6, &CLTE_PM_Data_Real::OnBnClickedExcel6)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST9, &CLTE_PM_Data_Real::OnLvnColumnclickList9)
END_MESSAGE_MAP()


// CLTE_PM_Data_Real 诊断

#ifdef _DEBUG
void CLTE_PM_Data_Real::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLTE_PM_Data_Real::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLTE_PM_Data_Real 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CString v_sTemp;													//临时变量

	//窗体名称
	v_sFrame_Name="LTE 性能数据查询_实时";								//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗【小区】
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList.InsertColumn(1,"小区",LVCFMT_LEFT,300);  
	m_cList.InsertColumn(2,"小区ID",LVCFMT_LEFT,0);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES );

	//初始化：列表窗【性能】
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList_PM.InsertColumn(1,"小区",LVCFMT_LEFT,200);					//第1列：小区
	m_cList_PM.InsertColumn(2,"营业区",LVCFMT_LEFT,60);					//第2列：营业区
	m_cList_PM.InsertColumn(3,"状态",LVCFMT_LEFT,40);					//第3列：状态
	m_cList_PM.InsertColumn(4,"日期",LVCFMT_CENTER,120);				//第4列：日期
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//初始化：列表窗【预警】
	::SendMessage(m_cList_Tj.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Tj.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList_Tj.InsertColumn(1,"日    期",LVCFMT_CENTER,120);  
	m_cList_Tj.InsertColumn(2,"性能",LVCFMT_LEFT,200);  
	m_cList_Tj.InsertColumn(3,"预警对象",LVCFMT_LEFT,300);  
	m_cList_Tj.InsertColumn(4,"性能参数",LVCFMT_RIGHT,80);  
	m_cList_Tj.InsertColumn(5,"预警门限",LVCFMT_RIGHT,80);  
	m_cList_Tj.InsertColumn(6,"粒度数",LVCFMT_RIGHT,60);  
	m_cList_Tj.InsertColumn(7,"告警数",LVCFMT_RIGHT,60);  
	m_cList_Tj.SetExtendedStyle(m_cList_Tj.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList_Tj.SetNumber(4);											//数字设置
	m_cList_Tj.SetNumber(5);											//数字设置
	m_cList_Tj.SetNumber(6);											//数字设置
	m_cList_Tj.SetNumber(7);											//数字设置

	//分窗 [先生成左边的上下关系的水平分隔符，再生成左右关系的垂直分隔符，即2*1]
	CRect	v_cRect;													//矩形
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//获得：指针
	pWnd->GetWindowRect(&v_cRect);										//获得：尺寸
	ScreenToClient(&v_cRect);											//坐标变换
	pWnd->DestroyWindow();												//销毁：指针

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Left.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList_PM,			// Left pane
		&m_cList_Tj,			// Right pane
		IDC_LTE_PM_Data_Real_Main,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);
	m_SplitterPane_Left.m_bVertSplitter_Parent=true;					//垂直分隔符为父窗
	m_SplitterPane_Main.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_cList,				// Left pane
		&m_SplitterPane_Left,	// Right pane
		IDC_LTE_PM_Data_Real_Main_1,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);

	//刷新界面
	PostMessage(WM_SIZE);

	SetTimer(1,60000,NULL);												//1 mins定时 

	//变量初始化
	m_cType.AddString("1H");
	m_cType.AddString("15m");
	m_cType.SetCurSel(1);												//默认：15mins粒度
	m_cType.EnableWindow(false);										//不能修改
	m_cAlarm_All.SetCheck(true);										//标志：单粒度预警
	m_cAuto.SetCheck(true);												//标志：自动刷新

	v_iList_Item=-1;													//默认：无选中的列表Item
	v_dPM_Date=COleDateTime::GetCurrentTime();							//默认：当前时间
	v_sPM_Working="";													//默认：性能获取进度

	//获取：性能粒度
	COleDateTime v_dTime=My_LoadData_Time();							//获取：性能时间
	v_sTemp=v_dTime.Format("%Y-%m-%d_%H:%M:%S");						//字符串
	m_cTime.SetWindowText(v_sTemp);										//显示：性能时间

	//删除临时文件
	My_DeleteFiles("\\DATA\\");											//删除上次FTP文件

	//获取：性能参数
	v_iWorking=3;														//工作任务：获取初始数据
	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：子窗销毁
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::OnDestroy()
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
void CLTE_PM_Data_Real::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//分窗尺寸调整
		CRect v_cRect;
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=45;
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Data_Real::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：定时器
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString			v_sTemp;											//临时变量
	COleDateTime	v_dTime;											//临时变量

	if (m_cAuto.GetCheck())												//自动刷新？？？
	{
		v_dTime=My_LoadData_Time();										//获取：性能粒度
		if (v_dTime!=v_dPM_Date)										//性能粒度变化？？？
		{
			v_dPM_Date=v_dTime;											//获取性能时间
			OnBnClickedExcel3();										//性能查询
		}
	}

	CFormView::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：明细导出
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_PM);											//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：统计导出
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::OnBnClickedExcel8()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Tj);											//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Log列表排序(右击列表任意列)【小区】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Real::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Log列表排序(右击列表任意列)【性能】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Real::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList_PM.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Log列表排序(右击列表任意列)【预警】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Real::OnLvnColumnclickList9(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList_Tj.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：全选【小区】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//遍历
		m_cList.SetCheck(vi,m_cCheck.GetCheck());						//设置状态
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击查询按钮【小区查询】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::OnBnClickedButton1()
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
void CLTE_PM_Data_Real::OnBnClickedExcel4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp;												//临时变量
	int			v_iItem,v_iID=0;										//临时变量

	LTE_PM_DATA_CASE	v_Dlg;											//实例化

	v_Dlg.v_iSelect=0;													//标志：小区
	if (v_Dlg.DoModal()==IDOK)											//确认返回？？？
	{
		m_cList.DeleteAllItems();										//删除：模板信息
		v_iList_Item=-1;												//复位：条目序号

		mString_String::iterator	v_pIter_Cell;						//迭代器
		set<CString>::iterator v_pIter;									//迭代器

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
//	函数功能：性能模板【获取：性能信息，放入m_cList_PM 列信息】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::OnBnClickedExcel5()
{
	// TODO: 在此添加控件通知处理程序代码
	LTE_PM_DATA_CASE	v_Dlg;											//实例化

	v_Dlg.v_iSelect=1;													//标志：性能
	v_Dlg.v_mLTE_PM_Type=&v_mLTE_PM_Type;								//指针：性能参数
	if (v_Dlg.DoModal()==IDOK)											//确认返回？？？
		My_PM_Show();													//选择性能列添加
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能选择
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CLTE_PM_Draw_Znit_Select	v_cDlg;									//实例化

	v_cDlg.v_mLTE_PM_Type=&v_mLTE_PM_Type;								//指针：性能参数
	if (v_cDlg.DoModal()==IDOK)											//确认返回？？？
		My_PM_Show();													//选择性能列添加
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：选择性能列添加
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::My_PM_Show()
{
	CString		v_sTemp;												//临时变量
	int			v_iNum;													//临时变量

	mInt_String					v_mPM_Field;							//性能名称
	mInt_String					v_mPM_Field_PM;							//性能名称(字段)
	mInt_String::iterator		v_pPM_Field;							//迭代器
	mInt_String::iterator		v_pPM_Field_PM;							//迭代器
	mLTE_PM::iterator			v_pPM_Field_Num;						//迭代器

	mLTE_PM_Type::iterator		v_pSelect;								//迭代器
	mLTE_PM::iterator			v_pPM_Name;								//迭代器

	//【1】性能排序：在容器里【v_mPM_Field】
	v_mPM_Field_Num.clear();											//容器清除
	for (v_pSelect=v_mLTE_PM_Type.begin();v_pSelect!=v_mLTE_PM_Type.end();v_pSelect++)
	{
		for (v_pPM_Name=v_pSelect->second.v_mLTE_PM.begin();v_pPM_Name!=v_pSelect->second.v_mLTE_PM.end();v_pPM_Name++)
		{
			if (v_pPM_Name->second.v_bOK)								//性能选中？？？【按列序号，加入容器】
			{
				v_mPM_Field.insert(pair<int,CString>(v_pPM_Name->second.v_iNum,v_pPM_Name->second.v_sPM_Name));	//容器增加：列序号映射性能名称
				v_mPM_Field_PM.insert(pair<int,CString>(v_pPM_Name->second.v_iNum,v_pPM_Name->first));			//容器增加：列序号映射性能名称(字段)
				v_mPM_Field_Num.insert(pair<CString,LTE_PM_Struct>(v_pPM_Name->first,v_pPM_Name->second));		//容器增加：性能名称映射列序号
			}
		}
	}

	//【2】性能明细列表
	//删除列表信息
	m_cList_Tj.DeleteAllItems();										//删除所有条目
	m_cList_PM.DeleteAllItems();										//删除所有条目
	while(m_cList_PM.DeleteColumn(0));									//删除所有列：ObjType信息列表
	v_mCell_Real_Alarm.clear();											//容器清除
	v_dDate_Real.clear();												//容器清除

	//添加列信息：性能名称
	m_cList_PM.InsertColumn(0,"序号",LVCFMT_CENTER,50);					//第1列：序号
	m_cList_PM.InsertColumn(1,"小区",LVCFMT_LEFT,200);					//第2列：小区
	m_cList_PM.InsertColumn(2,"营业区",LVCFMT_LEFT,60);					//第2列：营业区
	m_cList_PM.InsertColumn(3,"状态",LVCFMT_LEFT,40);					//第3列：状态
	m_cList_PM.InsertColumn(4,"日期",LVCFMT_CENTER,120);				//第4列：日期

	//加入列：性能名称【按列序号，从容器中获取性能名称】
	v_iNum=5;
	for (v_pPM_Field=v_mPM_Field.begin();v_pPM_Field!=v_mPM_Field.end();v_pPM_Field++)	//遍历				
	{
		//列名称
		m_cList_PM.InsertColumn(v_iNum,v_pPM_Field->second,LVCFMT_RIGHT,80);	//第n列：性能
		m_cList_PM.SetNumber(v_iNum);									//设置：数字排序

		//性能名称重新排序(去掉空缺的序号)[列信息同步]
		v_pPM_Field_PM=v_mPM_Field_PM.find(v_pPM_Field->first);			//寻找：序号
		if(v_pPM_Field_PM!=v_mPM_Field_PM.end())						//找到？？？
		{
			v_pPM_Field_Num=v_mPM_Field_Num.find(v_pPM_Field_PM->second);	//查找：性能
			if (v_pPM_Field_Num!=v_mPM_Field_Num.end())					//找到？？？
				v_pPM_Field_Num->second.v_iNum=v_iNum;					//获取：列序号
		}

		v_iNum++;														//计数器
	}

	v_sTemp.Format("%d",v_iNum-5);										//性能总数
	m_cPM_Sum.SetWindowText(v_sTemp);									//显示：性能总数

	//【3】复位
	v_mPM_Field.clear();												//容器清除
	v_mPM_Field_PM.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击小区列表
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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
//	函数功能：预警配置
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::OnBnClickedExcel6()
{
	// TODO: 在此添加控件通知处理程序代码

	//【1】预警配置
	CLTE_PM_Data_Real_Limit	v_Dlg;										//实例化
	v_Dlg.v_mLTE_PM_Type=&v_mLTE_PM_Type;								//指针：性能参数
	v_Dlg.DoModal();													//确认返回？？？

	//【2】保存：配置结果
	mLTE_PM::iterator		v_pPM_Field_Num;							//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	v_pLTE_PM_Type=v_mLTE_PM_Type.begin();								//首指针
	while(v_pLTE_PM_Type!=v_mLTE_PM_Type.end())							//结束？？
	{
		v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();			//首指针
		while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//结束？？
		{
			if (v_pPM_Name->second.v_bOK)								//选中？？？
			{
				v_pPM_Field_Num = v_mPM_Field_Num.find(v_pPM_Name->first);	//查找：性能名称
				if (v_pPM_Field_Num!=v_mPM_Field_Num.end())				//找到？？？
				{
					//保存：配置结果
					v_pPM_Field_Num->second.v_sAlarm_Flag=v_pPM_Name->second.v_sAlarm_Flag;		//预警标志
					v_pPM_Field_Num->second.v_dAlarm_Value=v_pPM_Name->second.v_dAlarm_Value;	//预警值
					v_pPM_Field_Num->second.v_sAlarm_Type=v_pPM_Name->second.v_sAlarm_Type;		//预警类型
				}
			}
			v_pPM_Name++;												//递加
		}
		v_pLTE_PM_Type++;												//递加
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能查询
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::OnBnClickedExcel3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString			v_sTemp;											//临时变量
	COleDateTime	v_dTime;											//临时变量

	//【1】获取：查询粒度
	v_dPM_Date=My_LoadData_Time();										//获取：性能粒度
	v_sTemp=v_dPM_Date.Format("%Y-%m-%d_%H:%M:%S");						//字符串
	m_cTime.SetWindowText(v_sTemp);										//显示：性能时间

	//【2】开始查询(线程)
	v_iWorking=2;														//工作任务：性能查询
	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CLTE_PM_Data_Real::My_Thread_Inoput(LPVOID lparam)
{
	CLTE_PM_Data_Real *  lp_this = NULL ;
	lp_this = (CLTE_PM_Data_Real *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//导出：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效
	lp_this->m_cAlarm.EnableWindow(false);								//导出：无效
	lp_this->m_cCheck.EnableWindow(false);								//导出：无效
	lp_this->m_cPM_Cell.EnableWindow(false);							//导出：无效
	lp_this->m_cCase_Cell.EnableWindow(false);							//导出：无效
	lp_this->m_cCase_PM.EnableWindow(false);							//导出：无效
	lp_this->m_cCase_Select.EnableWindow(false);						//导出：无效
	lp_this->m_cExcel_Tj.EnableWindow(false);							//导出：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效
	lp_this->m_cAlarm.EnableWindow(true);								//导出：有效
	lp_this->m_cCheck.EnableWindow(true);								//导出：有效
	lp_this->m_cPM_Cell.EnableWindow(true);								//导出：有效
	lp_this->m_cCase_Cell.EnableWindow(true);							//导出：有效
	lp_this->m_cCase_PM.EnableWindow(true);								//导出：有效
	lp_this->m_cCase_Select.EnableWindow(true);							//导出：有效
	lp_this->m_cExcel_Tj.EnableWindow(true);							//导出：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::My_Input_Main()
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
//	函数功能：装载性能信息【月份处理】
//	依    据：【表名称：15m/1H粒度；性能类型；】【行：小区、日期】【列：性能类型】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Real::My_LoadData_PM()
{
	CString			v_sTemp,v_sCell="",v_sTime="";						//临时变量
	int				v_iIndex;											//临时变量
	bool			v_bPM=false;										//临时变量
	COleDateTime	v_dDate_Start,v_dDate_End,v_dDate;					//查询开始日期、结束日期
	CString			v_sTime_Start,v_sTime_End="";						//日期

	//【1】数据初始化
	My_PM_Data_Delete();												//容器清除
	m_cList_PM.DeleteAllItems();										//清除列表数据
	m_cList_Tj.DeleteAllItems();										//清除列表数据

	//【2】小区获取：选中的小区
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

	//【3】性能处理：有选中的性能？？？
	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name_C;								//迭代器
	for (v_pLTE_PM_Type=v_mLTE_PM_Type.begin();v_pLTE_PM_Type!=v_mLTE_PM_Type.end();v_pLTE_PM_Type++)
	{
		for (v_pPM_Name_C=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name_C!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name_C++)
		{
			if (v_pPM_Name_C->second.v_bOK)								//性能选中？？？	
			{
				v_bPM=true;												//性能有效
				break;													//退出
			}
		}
		if (v_bPM)														//性能有效？？
			break;														//退出
	}
	if(!v_bPM)															//性能有效？？？
	{
		MessageBox("  请选择有效的性能 !!! ");							//窗体提示
		return;
	}

	//【4】获取性能表(15m粒度，每天一个表)
	v_dDate_Start=v_dPM_Date;											//获得：保存时间粒度
	v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),v_dDate_Start.GetHour(),v_dDate_Start.GetMinute(),0);	//设置时间

	v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d %H:%M:%S");			//开始时间
	v_sTemp=v_dDate_Start.Format("%Y%m%d");								//年月日
	My_LoadData_PM_Table(v_sTemp,v_sCell,v_sTime_Start,v_sTime_End);	//装载性能数据

	//【5】显示：在列表中(数据在容器中：小区、时间、性能数据)
	CString					v_sPm_Value,v_sPm_Limite;					//临时变量
	mLTE_PM::iterator		v_pPM_Field_Num;							//迭代器
	map_PM_Cell::iterator	v_pPM_Cell;									//迭代器
	map_PM_Time::iterator	v_pPM_Time;									//迭代器
	mString_Double::iterator v_pPM_Name;								//迭代器

	//记录：粒度信息
	mDate_Int::iterator		v_pData_Real;								//迭代器
	v_pData_Real=v_dDate_Real.find(v_dDate_Start);						//该粒度查询过？？？
	if (v_pData_Real==v_dDate_Real.end())								//没查询过？？？
		v_dDate_Real.insert(pair<COleDateTime,int>(v_dDate_Start,0));	//增加容器
	
	m_cList_PM.SetRedraw(false);										//禁止闪烁

	//遍历：小区
	for (v_pPM_Cell=v_mPM_Cell.begin();v_pPM_Cell!=v_mPM_Cell.end();v_pPM_Cell++)	//遍历：小区 ？？？
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//强制退出？？？
			break;														//退出

		//遍历：时间
		for(v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.begin();v_pPM_Time!=v_pPM_Cell->second.v_mTime_PM_Data.end();v_pPM_Time++)	//遍历：粒度 ？？？
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_iIndex = m_cList_PM.InsertItem(0xffff,_T("0"));
			v_sTemp.Format("%06d",v_iIndex+1);
			m_cList_PM.SetItemText(v_iIndex,0,v_sTemp);					//序号
			m_cList_PM.SetItemText(v_iIndex,1,v_pPM_Cell->first);		//Cell
			m_cList_PM.SetItemText(v_iIndex,2,v_pPM_Cell->second.v_sRegion);	//营业区
			m_cList_PM.SetItemText(v_iIndex,3,v_pPM_Cell->second.v_sOnline);	//状态
			v_sTime=v_pPM_Time->first.Format("%Y-%m-%d_%H:%M:%S");		//赋值：日期
			m_cList_PM.SetItemText(v_iIndex,4,v_sTime);					//粒度

			//遍历：性能
			for (v_pPM_Name=v_pPM_Time->second.begin();v_pPM_Name!=v_pPM_Time->second.end();v_pPM_Name++)	//遍历：性能？？？
			{
				v_pPM_Field_Num = v_mPM_Field_Num.find(v_pPM_Name->first);	//查找：性能名称
				if (v_pPM_Field_Num!=v_mPM_Field_Num.end())				//找到？？？
				{
					//显示
					v_sPm_Value.Format("%8.2f",v_pPM_Name->second);		//字符串
					m_cList_PM.SetItemText(v_iIndex,v_pPM_Field_Num->second.v_iNum,v_sPm_Value);	//性能

					//预警
					if (v_pPM_Field_Num->second.v_sAlarm_Flag=="预警" && v_pPM_Name->second!=-1)	//预警有效？？？【-1：无效信息】
					{
						if (v_pPM_Field_Num->second.v_sAlarm_Type=="低")//预警类型
						{
							if (v_pPM_Name->second>=v_pPM_Field_Num->second.v_dAlarm_Value)			//超出门限
							{
								v_sPm_Limite.Format("%8.2f",v_pPM_Field_Num->second.v_dAlarm_Value);//字符串
								My_Alarm_Insert(v_pPM_Time->first,v_pPM_Field_Num->second.v_sPM_Name,v_pPM_Cell->first,v_sPm_Value,v_sPm_Limite);	//显示：预警信息
							}
						}
						else											//低：有效
						{
							if (v_pPM_Name->second<=v_pPM_Field_Num->second.v_dAlarm_Value)			//低于门限
							{
								v_sPm_Limite.Format("%8.2f",v_pPM_Field_Num->second.v_dAlarm_Value);//字符串
								My_Alarm_Insert(v_pPM_Time->first,v_pPM_Field_Num->second.v_sPM_Name,v_pPM_Cell->first,v_sPm_Value,v_sPm_Limite);	//显示：预警信息
							}
						}
					}						
				}
			}
		}
	}

	//【6】多粒度预警显示
	if (!m_cAlarm_All.GetCheck())										//多粒度预警显示？？？
	{
		mPM_Cell_Real_Alarm::iterator	v_pIter;						//迭代器
		mDate_Int::iterator				v_pData_Real;					//迭代器

		v_pIter=v_mCell_Real_Alarm.begin();								//遍历：预警对象
		while(v_pIter!=v_mCell_Real_Alarm.end())						//结束？？？
		{
			//增加：粒度信息
			v_pData_Real=v_pIter->second.v_dDate_Real.find(v_dDate_Start);		//该粒度查询存在？？？
			if (v_pData_Real==v_pIter->second.v_dDate_Real.end())		//【不存在，则显示；存在，不显示】
				My_Alarm_Show(v_sTime,v_pIter->second.v_sPM_Name,v_pIter->second.v_sObject,"","",v_dDate_Real.size(),v_pIter->second.v_dDate_Real.size());	//显示：预警信息

			v_pIter++;													//迭代器++
		}	
	}

	m_cList_PM.SetRedraw(true);											//列表：显示
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：预警信息统计
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::My_Alarm_Insert(COleDateTime v_dTime,CString v_sPM_Name,CString v_sObject,CString v_sPm_Value,CString v_sPm_Limite)
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp,v_sTime;										//临时变量

	//【1】统计
	PM_Cell_Real_Alarm_Struct		v_cCell_Real_Alarm_Struct;			//结构
	mPM_Cell_Real_Alarm::iterator	v_pIter;							//迭代器

	v_sTime=v_dTime.Format("%Y-%m-%d_%H:%M:%S");						//赋值：日期
	//增加：性能+小区
	v_pIter=v_mCell_Real_Alarm.find(v_sPM_Name+v_sObject);				//预警存在？？
	if (v_pIter==v_mCell_Real_Alarm.end())								//没找到？？？
	{
		v_cCell_Real_Alarm_Struct.v_sPM_Name=v_sPM_Name;				//预警性能名称
		v_cCell_Real_Alarm_Struct.v_sObject=v_sObject;					//预警对象
		v_mCell_Real_Alarm.insert(pair<CString,PM_Cell_Real_Alarm_Struct>(v_sPM_Name+v_sObject,v_cCell_Real_Alarm_Struct));	//容器增加
		v_pIter=v_mCell_Real_Alarm.find(v_sPM_Name+v_sObject);			//查找：性能+小区
	}

	//增加：粒度信息
	mDate_Int::iterator		v_pData_Real;								//迭代器
	v_pData_Real=v_pIter->second.v_dDate_Real.find(v_dTime);			//该粒度查询过？？？
	if (v_pData_Real==v_pIter->second.v_dDate_Real.end())				//没查询过？？？
		v_pIter->second.v_dDate_Real.insert(pair<COleDateTime,int>(v_dTime,0));		//增加容器

	//【2】显示
	My_Alarm_Show(v_sTime,v_sPM_Name,v_sObject,v_sPm_Value,v_sPm_Limite,v_dDate_Real.size(),v_pIter->second.v_dDate_Real.size());
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：预警信息显示
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::My_Alarm_Show(CString v_sTime,CString v_sPM_Name,CString v_sObject,CString v_sPm_Value,CString v_sPm_Limite,int v_iSum,int v_iSum_Cell)
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp;												//临时变量

	int		v_iItem = m_cList_Tj.InsertItem(0xffff,_T("0"));
	v_sTemp.Format("%04d",v_iItem+1);
	m_cList_Tj.SetItemText(v_iItem,0," "+v_sTemp);						//序号
	m_cList_Tj.SetItemText(v_iItem,1,v_sTime);							//时间
	m_cList_Tj.SetItemText(v_iItem,2,v_sPM_Name);						//性能名称
	m_cList_Tj.SetItemText(v_iItem,3,v_sObject);						//预警对象
	m_cList_Tj.SetItemText(v_iItem,4,v_sPm_Value);						//性能值
	m_cList_Tj.SetItemText(v_iItem,5,v_sPm_Limite);						//预警门限
	v_sTemp.Format("%d",v_iSum);										//字符串
	m_cList_Tj.SetItemText(v_iItem,6,v_sTemp);							//粒度次数
	v_sTemp.Format("%d",v_iSum_Cell);									//字符串
	m_cList_Tj.SetItemText(v_iItem,7,v_sTemp);							//预警次数
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能表【一次；每月一次，每次获取多个性能表】
//	参	  数：v_sTable：年月日【20160701】；	v_sCell：小区；		v_sDate_Ben：开始日期；		v_sDate_End：结束日期；
//	依    据：【表名称：15m/1H粒度；性能类型；】【行：小区、日期】【列：性能类型】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Real::My_LoadData_PM_Table(CString v_sDate,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End)
{
	CString			v_sTemp,v_sTable,v_sField;							//临时变量
	int				v_iPM_Num,v_iPM_Sum;								//临时变量

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	//【1】性能统计(获取性能表总数)
	v_iPM_Sum=0;														//总数
	for (v_pLTE_PM_Type=v_mLTE_PM_Type.begin();v_pLTE_PM_Type!=v_mLTE_PM_Type.end();v_pLTE_PM_Type++)
	{
		for (v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
		{
			if (v_pPM_Name->second.v_bOK)								//性能选中？？？	
			{
				v_iPM_Sum++;											//总数
				break;													//退出
			}
		}
	}

	//【2】性能获取(依据性能种类(7种)，每种性能获取一次)
	v_iPM_Num=0;														//计数器
	for (v_pLTE_PM_Type=v_mLTE_PM_Type.begin();v_pLTE_PM_Type!=v_mLTE_PM_Type.end();v_pLTE_PM_Type++)
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//强制退出？？？
			break;														//退出

		//【3】获取：字段信息
		v_sField="";													//复位：字段
		for (v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
		{
			if (v_pPM_Name->second.v_bOK)								//性能选中？？？	
			{
				if (!v_sField.IsEmpty())								//字段不为空？？？
					v_sField+=",";										//分隔符
				v_sField+="["+v_pPM_Name->first+"]";					//获取：字段信息
			}
		}
		
		//【4】获取PM数据(15mins：表名称_年月日；)
		if (!v_sField.IsEmpty())										//字段有效？？？
		{
			v_sTable = "Pm_LTE_"+v_pLTE_PM_Type->second.v_sTable_Key+"_"+v_sDate;	//表名称【6个原始表】
			//获取：性能【1个性能表】
			v_iPM_Num++;												//计数器
			v_sPM_Working.Format("  查询性能组：%d/%d，请稍候......",v_iPM_Num,v_iPM_Sum);			//性能查询进度【日期(多个)+性能(多个)】
			My_LoadData_PM_Data(v_pLTE_PM_Type,v_sTable,v_sField,v_sCell,v_sDate_Ben,v_sDate_End);	//装载性能数据
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能数据
//	参    数：v_pLTE_PM_Type：迭代器；	v_sTable：表名称；	v_sField：字段；	v_sCell：小区；		v_sDate_Ben：开始日期；		v_sDate_End：结束日期
//	结    果：性能数据放入容器
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Real::My_LoadData_PM_Data(mLTE_PM_Type::iterator v_pLTE_PM_Type,CString v_sTable,CString v_sField,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End)
{
	CString			v_sSql,v_sPM_Cell,v_sRegion,v_sOnline;				//临时变量
	CString			v_sTemp;											//临时变量
	COleDateTime	v_tDate;											//日期
	double			v_dValue;											//性能数值
	int				v_iCell_ID=-1,v_iCell_ID_New=-1;					//临时变量
	PM_Time_Struct	v_cPM_Time;											//结构

	_variant_t		Var;

	map_PM_Time				v_mPM_Time;									//容器
	mString_Double			v_mPM_Name;									//容器

	mLTE_PM::iterator		v_pPM_Name;									//迭代器
	map_PM_Cell::iterator	v_pPM_Cell;									//迭代器
	map_PM_Time::iterator	v_pPM_Time;									//迭代器
	mString_Double::iterator v_pPM_Value;								//迭代器
	map_Cell_ID::iterator	v_pIter_Cell;								//迭代器

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		//检查：表名称是否有效【依据：表名称集合v_mTable_Name】【如果表名称无效，则查不出性能数据。】
		set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//查找？？
		if (v_pTable==v_mTable_Name.end())								//没找到：数据表
			return;														//无效返回

		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zTime,"+v_sField+" from "+v_sTable+" where zCell in("+v_sCell+") and zTime='"+v_sDate_Ben
				+"' order by zCell,zTime";								//查询数据库：系统日志

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_iCell_ID=Var.intVal;									//获取：Cell_ID
				if (v_iCell_ID!=v_iCell_ID_New)							//变化：v_iCell_ID？？？
				{
					v_pIter_Cell=v_mCell_ID.find(v_iCell_ID);			//查找：小区ID存在？？？
					if (v_pIter_Cell==v_mCell_ID.end())					//不存在
					{
						v_pRecordset->MoveNext();						//下一粒度性能
						continue;										//继续
					}
					v_iCell_ID_New=v_iCell_ID;							//
					v_sPM_Cell=v_pIter_Cell->second.v_sCell;			//小区
					v_sOnline=v_pIter_Cell->second.v_sOnline;			//状态
					v_sRegion=v_pIter_Cell->second.v_sRegion;			//营业区
				}
			}
			else
				v_sPM_Cell="";

			v_tDate=v_cAdoConn.GetField_DataTime("zTime");				//zTime

			v_pPM_Cell=v_mPM_Cell.find(v_sPM_Cell);						//查找：小区 ？？？
			if (v_pPM_Cell==v_mPM_Cell.end())							//不存在
			{
				v_cPM_Time.v_sOnline=v_sOnline;							//状态
				v_cPM_Time.v_sRegion=v_sRegion;							//区域
				v_mPM_Cell.insert(pair<CString,PM_Time_Struct>(v_sPM_Cell,v_cPM_Time));	//容器增加
			}
			v_pPM_Cell=v_mPM_Cell.find(v_sPM_Cell);						//
			v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.find(v_tDate);//查找：粒度 ？？？
			if (v_pPM_Time==v_pPM_Cell->second.v_mTime_PM_Data.end())	//不存在
				v_pPM_Cell->second.v_mTime_PM_Data.insert(pair<COleDateTime,mString_Double>(v_tDate,v_mPM_Name));	//容器增加
			v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.find(v_tDate);//

			//字段获取：遍历
			v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();		//首指针
			while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())	//结束？？
			{
				if (v_pPM_Name->second.v_bOK)							//选中？？？
				{
					v_dValue=v_cAdoConn.GetField_Double(v_pPM_Name->first);	//获取：性能

					v_pPM_Value=v_pPM_Time->second.find(v_pPM_Name->first);	//查找：性能名称？？？
					if (v_pPM_Value==v_pPM_Time->second.end())				//不存在？？？
						v_pPM_Time->second.insert(pair<CString,double>(v_pPM_Name->first,v_dValue));	//容器增加
				}

				v_pPM_Name++;											//递加
			}
			v_pRecordset->MoveNext();									//
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
//	函数功能：性能数据删除
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Real::My_PM_Data_Delete()
{
	map_PM_Cell::iterator	v_pPM_Cell;									//迭代器
	map_PM_Time::iterator	v_pPM_Time;									//迭代器
	for (v_pPM_Cell=v_mPM_Cell.begin();v_pPM_Cell!=v_mPM_Cell.end();v_pPM_Cell++)
	{
		for(v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.begin();v_pPM_Time!=v_pPM_Cell->second.v_mTime_PM_Data.end();v_pPM_Time++)
			v_pPM_Time->second.clear();									//容器清除
		v_pPM_Cell->second.v_mTime_PM_Data.clear();						//容器清除
	}
	v_mPM_Cell.clear();													//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载Cell信息
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Real::My_LoadData_Cell()
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
		v_iList_Item=-1;												//默认：无选中的列表Item

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
void CLTE_PM_Data_Real::My_LoadData_Para()
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

		//【1】获取：性能参数【常规】
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
			v_cLTE_PM.v_sTable_Key=v_sTable_Key;						//表名称-Key
			v_cLTE_PM.v_sPM_Name=v_sPm_Name;							//性能：中文名
			v_cLTE_PM.v_bOK=false;										//性能：未选中
			v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//增加：性能参数

			v_pRecordset->MoveNext();									//
		}

		//【2】获取：性能参数【计算性能】
		v_sType="计算性能";												//性能类型								
		v_cLTE_PM_Type.v_sTable_Key = "Cal";							//表名称-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//容器增加

		//获取：性能参数【计算性能】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Cal order by zPm";			//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sPm = v_cAdoConn.GetField_String("zPm");					//zPm

			//保存
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//查找：性能类型
			if (v_pLTE_PM_Type!=v_mLTE_PM_Type.end())					//找到？？？
			{
				v_cLTE_PM.v_sPM_Name=v_sPm;								//性能：中文名
				v_cLTE_PM.v_sTable_Key="Cal";							//性能：表名称
				v_cLTE_PM.v_bOK=false;									//性能：未选中
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//增加：性能参数
			}

			v_pRecordset->MoveNext();									//
		}

		//【3】获取：性能数据表名称【用于发现无效表名称】
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_LTE_%') order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//表名称

			v_mTable_Name.insert(v_sTable);								//增加集合

			v_pRecordset->MoveNext();									//指针++
		}	

		//【4】获取：小区信息【用于小区ID处理】
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
//	函数功能：获取性能粒度
//------------------------------------------------------------------------------------------------------			
COleDateTime CLTE_PM_Data_Real::My_LoadData_Time()
{
	CString			v_sTemp,v_sSql;										//临时变量
	COleDateTime	v_sTime=COleDateTime::GetCurrentTime();				//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT top 1 * FROM CON_System";						//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		if(!v_pRecordset->adoEOF)
		{
			v_sTime = v_cAdoConn.GetField_DataTime("zPm_LTE_Time_Real");//zPm_LTE_Time_Real [性能粒度]
		}

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	return	v_sTime;													//返回
}

//------------------------------------------------------------------------------------------------------
//	函数功能：删除文件[*.csv]
//	参    数：v_sFile：文件目录；
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Real::My_DeleteFiles(CString v_sDirName) 
{
	CString		v_sFile_Name,v_sTemp;									//
	CFileFind   tempFind;												//
	BOOL   IsFinded=(BOOL)tempFind.FindFile((LPCSTR)(theApp.m_sPath+v_sDirName+"*.*"));	//判断文件夹
	while(IsFinded)														//有效文件夹？？？
	{ 
		IsFinded=(BOOL)tempFind.FindNextFile();							//寻找下一个文件
		v_sFile_Name= tempFind.GetFileName();							//获取：文件名称/目录名称
		if(!tempFind.IsDots())											//..或者.？？？  
		{
			v_sFile_Name = tempFind.GetFileName();						//获取：文件名称/目录名称
			if(tempFind.IsDirectory())									//目录？？？
				RemoveDirectory((LPCSTR)(v_sDirName+v_sFile_Name));		//删除目录
			else
			{
				v_sTemp = v_sFile_Name.Right(3);	
				if (v_sTemp.Find("csv")>=0)								//选择"txt"文件
					DeleteFile((LPCSTR)(theApp.m_sPath+v_sDirName+v_sFile_Name));	//删除文件
			}
		}
	} 
	tempFind.Close(); 													//当寻找文件的事件结束就结束寻找功能
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
