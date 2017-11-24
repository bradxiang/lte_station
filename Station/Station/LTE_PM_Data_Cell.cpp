// LTE_PM_Data_Cell.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "LTE_PM_Data_Cell.h"
#include "LTE_PM_Draw_Znit_Select.h"									//性能选择
#include "LTE_PM_DATA_CASE.h"											//模板【小区、性能】
#include "LTE_PM_Data_Cell_Time.h"										//时段选择

// CLTE_PM_Data_Cell

IMPLEMENT_DYNCREATE(CLTE_PM_Data_Cell, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cell::CLTE_PM_Data_Cell()
	: CFormView(CLTE_PM_Data_Cell::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLTE_PM_Data_Cell::~CLTE_PM_Data_Cell()
{
	v_mLTE_PM_Type.clear();												//容器清除
	v_mTable_Name.clear();												//容器清除
	v_mTime.clear();													//容器清除
	v_mPM_Field_Num.clear();											//容器清除
	v_mCell.clear();													//容器清除
	v_mCell_ID.clear();													//容器清除
	My_PM_Data_Delete();												//容器清除
	My_PM_Data_P_Delete();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_CHECK2, m_cCheck);
	DDX_Control(pDX, IDC_EXCEL3, m_cPM_Cell);
	DDX_Control(pDX, IDC_EXCEL4, m_cCase_Cell);
	DDX_Control(pDX, IDC_EXCEL5, m_cCase_PM);
	DDX_Control(pDX, IDC_EXCEL2, m_cCase_Select);
	DDX_Control(pDX, IDC_LIST9, m_cList_Tj);
	DDX_Control(pDX, IDC_CHECK3, m_cTime_Check);
	DDX_Control(pDX, IDC_COMBO8, m_cTj_Type);
	DDX_Control(pDX, IDC_EXCEL6, m_cSetup_Time);
	DDX_Control(pDX, IDC_EXCEL7, m_cTj);
	DDX_Control(pDX, IDC_EXCEL8, m_cExcel_Tj);
	DDX_Control(pDX, IDC_KEY2, m_cSpan);
	DDX_Control(pDX, IDC_QUARY_DATA, m_cQuary_Data);
	DDX_Control(pDX, IDC_QUARY_NAME, m_cQuary_Name);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLTE_PM_Data_Cell, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL, &CLTE_PM_Data_Cell::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CLTE_PM_Data_Cell::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLTE_PM_Data_Cell::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLTE_PM_Data_Cell::OnNMClickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CLTE_PM_Data_Cell::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_EXCEL2, &CLTE_PM_Data_Cell::OnBnClickedExcel2)
	ON_BN_CLICKED(IDC_EXCEL3, &CLTE_PM_Data_Cell::OnBnClickedExcel3)
	ON_BN_CLICKED(IDC_EXCEL4, &CLTE_PM_Data_Cell::OnBnClickedExcel4)
	ON_BN_CLICKED(IDC_EXCEL5, &CLTE_PM_Data_Cell::OnBnClickedExcel5)
	ON_BN_CLICKED(IDC_CHECK2, &CLTE_PM_Data_Cell::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_EXCEL8, &CLTE_PM_Data_Cell::OnBnClickedExcel8)
	ON_BN_CLICKED(IDC_EXCEL6, &CLTE_PM_Data_Cell::OnBnClickedExcel6)
	ON_BN_CLICKED(IDC_EXCEL7, &CLTE_PM_Data_Cell::OnBnClickedExcel7)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST9, &CLTE_PM_Data_Cell::OnLvnColumnclickList9)
	//消息响应：列表显示
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST3, &CLTE_PM_Data_Cell::OnLvnGetdispinfoList3)
END_MESSAGE_MAP()


// CLTE_PM_Data_Cell 诊断

#ifdef _DEBUG
void CLTE_PM_Data_Cell::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLTE_PM_Data_Cell::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLTE_PM_Data_Cell 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CString v_sTemp;													//临时变量
	int		vi;															//临时变量

	//窗体名称
	v_sFrame_Name="LTE 性能数据查询_小区";								//窗体名称
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
	m_cList_PM.ModifyStyle( 0, LVS_REPORT|LVS_SHOWSELALWAYS, TRUE );
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList_PM.InsertColumn(1,"小区",LVCFMT_LEFT,200);					//第1列：小区
	m_cList_PM.InsertColumn(2,"营业区",LVCFMT_LEFT,60);					//第2列：营业区
	m_cList_PM.InsertColumn(3,"状态",LVCFMT_LEFT,40);					//第3列：状态
	m_cList_PM.InsertColumn(4,"日期",LVCFMT_CENTER,120);				//第4列：日期
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//初始化：列表窗【统计】
	::SendMessage(m_cList_Tj.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Tj.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList_Tj.InsertColumn(1,"小区",LVCFMT_LEFT,200);  
	m_cList_Tj.InsertColumn(2,"营业区",LVCFMT_LEFT,60);					//第2列：营业区
	m_cList_Tj.InsertColumn(3,"状态",LVCFMT_LEFT,40);					//第3列：状态
	m_cList_Tj.InsertColumn(4,"日期",LVCFMT_CENTER,120);				//第4列：日期
	m_cList_Tj.SetExtendedStyle(m_cList_Tj.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

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
		IDC_LTE_PM_Data_Cell_Main,	// this ID is used for saving/restoring splitter position and it must be unique !
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
		IDC_LTE_PM_Data_Cell_Main_1,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect,				// dimensions of the splitter pane
		80,						// left constraint for splitter position
		140						// right constraint for splitter position
		);

	//变量初始化
	m_cType.AddString("1H");
	m_cType.AddString("15m");
	m_cType.SetCurSel(0);												//默认：1H粒度

	m_cTj_Type.AddString("求和");
	m_cTj_Type.AddString("平均");
	m_cTj_Type.SetCurSel(0);											//默认：求和

	//初始化：天数选择
	for (vi=1;vi<=c_iSample_Day;vi++)									//最长天数
	{
		v_sTemp.Format("%d",vi);
		m_cLog_Day.AddString(v_sTemp);									//可选天数复选钮
	}
	m_cLog_Day.SetCurSel(0);											//默认查询天数：1天；

	v_iList_Item=-1;													//默认：无选中的列表Item
	v_iWorking=0;														//复位

	//日期选择、时间选择
	for (vi=0;vi<5;vi++)												//设置数量
	{
		v_dDate_Select[vi].v_bFlag=false;								//无效
		v_dDate_Select[vi].v_dDate=COleDateTime::GetCurrentTime();		//当前日期
		v_dDate_Select[vi].v_iDay=0;									//1天

		v_dTime_Select[vi].v_bFlag=false;								//无效
		v_dTime_Select[vi].v_iHour_1=8;									//8点
		v_dTime_Select[vi].v_iHour_2=10;								//10点
	}
	v_dDate_Select[0].v_bFlag=true;										//有效：日期1

	//刷新界面
	PostMessage(WM_SIZE);

	//获取：性能参数
	v_iWorking=4;														//工作任务：获取初始数据
	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：子窗销毁
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnDestroy()
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
void CLTE_PM_Data_Cell::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//分窗尺寸调整
		CRect v_cRect;
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=((v_iWorking==2)?80:40);							//性能查询：80(显示查询过程)；其它：40
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CLTE_PM_Data_Cell::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：明细导出
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_PM);											//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：统计导出
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel8()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Tj);											//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Log列表排序(右击列表任意列)【小区】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表排序(右击列表任意列)【性能：虚拟列表】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	m_cList_PM.My_Sort_Virtual(pNMHDR);									//列表排序：虚拟列表

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Log列表排序(右击列表任意列)【统计】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::OnLvnColumnclickList9(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList_Tj.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：全选【小区】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//遍历
		m_cList.SetCheck(vi,m_cCheck.GetCheck());						//设置状态
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击查询按钮【小区查询】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedButton1()
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
void CLTE_PM_Data_Cell::OnBnClickedExcel4()
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
//	函数功能：性能模板【获取：性能信息，放入m_cList_PM 列信息】
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel5()
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
void CLTE_PM_Data_Cell::OnBnClickedExcel2()
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
void CLTE_PM_Data_Cell::My_PM_Show()
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
	m_cList_PM.DeleteAllItems();										//删除所有条目
	while(m_cList_PM.DeleteColumn(0));									//删除所有列：ObjType信息列表

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

	//【3】性能明细列表
	//删除列表信息
	m_cList_Tj.DeleteAllItems();										//删除所有条目
	while(m_cList_Tj.DeleteColumn(0));									//删除所有列：ObjType信息列表

	//添加列信息：性能名称
	m_cList_Tj.InsertColumn(0,"序号",LVCFMT_CENTER,50);					//第1列：序号
	m_cList_Tj.InsertColumn(1,"小区",LVCFMT_LEFT,200);					//第2列：小区
	m_cList_Tj.InsertColumn(2,"营业区",LVCFMT_LEFT,60);					//第2列：营业区
	m_cList_Tj.InsertColumn(3,"状态",LVCFMT_LEFT,40);					//第3列：状态
	m_cList_Tj.InsertColumn(4,"日期",LVCFMT_CENTER,120);				//第4列：日期

	//加入列：性能名称【按列序号，从容器中获取性能名称】
	v_iNum=5;
	for (v_pPM_Field=v_mPM_Field.begin();v_pPM_Field!=v_mPM_Field.end();v_pPM_Field++)	//遍历				
	{
		//列名称
		m_cList_Tj.InsertColumn(v_iNum,v_pPM_Field->second,LVCFMT_RIGHT,80);	//第n列：性能
		m_cList_Tj.SetNumber(v_iNum);									//设置：数字排序

		v_iNum++;														//计数器
	}

	v_sTemp.Format("%d",v_iNum-5);										//性能总数
	m_cPM_Sum.SetWindowText(v_sTemp);									//显示：性能总数

	//【4】复位
	v_mPM_Field.clear();												//容器清除
	v_mPM_Field_PM.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能查询
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(v_pIterator_Frame->second.v_iThread_Exit==0)
	{
		m_cTime_Check.SetCheck(false);									//无效：时段；【性能查询，必须是无时段统计。】

		//获取：日期1
		m_cLog_Date.GetTime(v_dDate_Select[0].v_dDate);					//获取：日期
		v_dDate_Select[0].v_iDay=m_cLog_Day.GetCurSel();				//获取：天数

		v_iWorking=2;													//工作任务：性能查询
		v_pIterator_Frame->second.v_iThread_Exit=1;						//线程：工作
		v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);			//开启查询数据库线程
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击小区列表
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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
//	函数功能：时段选择
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel6()
{
	// TODO: 在此添加控件通知处理程序代码
	CLTE_PM_Data_Cell_Time	v_cDlg;										//实例化

	//获取：日期1
	m_cLog_Date.GetTime(v_dDate_Select[0].v_dDate);						//获取：日期
	v_dDate_Select[0].v_iDay=m_cLog_Day.GetCurSel();					//获取：天数

	//时段选择
	v_cDlg.v_dDate_Select=&v_dDate_Select[0];							//日期选择
	v_cDlg.v_dTime_Select=&v_dTime_Select[0];							//时段选择
	v_cDlg.v_mTime=&v_mTime;											//时段容器
	if (v_cDlg.DoModal()==IDOK)											//确认返回？？？
	{
		m_cLog_Date.SetTime(v_dDate_Select[0].v_dDate);					//日期1：日期
		m_cLog_Day.SetCurSel(v_dDate_Select[0].v_iDay);					//日期1：天数
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：时段性能数据统计
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::OnBnClickedExcel7()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=3;													//工作任务：性能统计
	v_pIterator_Frame->second.v_iThread_Exit=1;						//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);			//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CLTE_PM_Data_Cell::My_Thread_Inoput(LPVOID lparam)
{
	CLTE_PM_Data_Cell *  lp_this = NULL ;
	lp_this = (CLTE_PM_Data_Cell *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//导出：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效
	lp_this->m_cLog_Date.EnableWindow(false);							//导出：无效
	lp_this->m_cLog_Day.EnableWindow(false);							//导出：无效
	lp_this->m_cType.EnableWindow(false);								//导出：无效
	lp_this->m_cCheck.EnableWindow(false);								//导出：无效
	lp_this->m_cPM_Cell.EnableWindow(false);							//导出：无效
	lp_this->m_cCase_Cell.EnableWindow(false);							//导出：无效
	lp_this->m_cCase_PM.EnableWindow(false);							//导出：无效
	lp_this->m_cCase_Select.EnableWindow(false);						//导出：无效
	lp_this->m_cTime_Check.EnableWindow(false);							//导出：无效
	lp_this->m_cTj_Type.EnableWindow(false);							//导出：无效
	lp_this->m_cSetup_Time.EnableWindow(false);							//导出：无效
	lp_this->m_cTj.EnableWindow(false);									//导出：无效
	lp_this->m_cExcel_Tj.EnableWindow(false);							//导出：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效
	lp_this->m_cLog_Date.EnableWindow(true);							//导出：有效
	lp_this->m_cLog_Day.EnableWindow(true);								//导出：有效
	lp_this->m_cType.EnableWindow(true);								//导出：有效
	lp_this->m_cCheck.EnableWindow(true);								//导出：有效
	lp_this->m_cPM_Cell.EnableWindow(true);								//导出：有效
	lp_this->m_cCase_Cell.EnableWindow(true);							//导出：有效
	lp_this->m_cCase_PM.EnableWindow(true);								//导出：有效
	lp_this->m_cCase_Select.EnableWindow(true);							//导出：有效
	lp_this->m_cTime_Check.EnableWindow(true);							//导出：有效
	lp_this->m_cTj_Type.EnableWindow(true);								//导出：有效
	lp_this->m_cSetup_Time.EnableWindow(true);							//导出：有效
	lp_this->m_cTj.EnableWindow(true);									//导出：有效
	lp_this->m_cExcel_Tj.EnableWindow(true);							//导出：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CLTE_PM_Data_Cell::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Cell();												//Cell查询
		break;
	case 2:
		m_cQuary_Data.ShowWindow(SW_SHOW);								//显示
		m_cQuary_Name.ShowWindow(SW_SHOW);								//显示
		PostMessage(WM_SIZE);											//刷新：

		My_LoadData_PM();												//装载性能信息

		v_iWorking=0;													//复位
		m_cQuary_Data.ShowWindow(SW_HIDE);								//隐藏
		m_cQuary_Name.ShowWindow(SW_HIDE);								//隐藏
		PostMessage(WM_SIZE);											//刷新：
		break;
	case 3:
		My_LoadData_PM_Tj();											//性能统计
		break;
	case 4:
		My_LoadData_Para();												//获取：性能参数
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能信息
//	依    据：【表名称：15m/1H粒度；性能类型；】【行：小区、日期】【列：性能类型】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_LoadData_PM()
{
	CString			v_sTemp,v_sCell="",v_sTime;							//临时变量
	int				v_iDay_Sum;											//临时变量
	int				v_iNum;												//临时变量
	bool			v_bPM=false;										//临时变量
	COleDateTime	v_dDate_Start,v_dDate_End,v_dDate;					//查询开始日期、结束日期
	CString			v_sTime_Start,v_sTime_End,v_sTable_Num;				//日期

	mDate_Int		v_mDate_Select;										//容器
	COleDateTime	v_dBeg,v_dEnd;

	v_dBeg=COleDateTime::GetCurrentTime();

	//【1】数据初始化
	My_PM_Data_Delete();												//容器清除
	m_cList_PM.DeleteAllItems();										//清除列表数据
	m_cList_Tj.DeleteAllItems();										//清除列表数据

	//获取：【日期1】数据
	for (int vi=0;vi<5;vi++)											//遍历5个日期
	{
		if (v_dDate_Select[vi].v_bFlag)									//选中？？？
			v_mDate_Select.insert(pair<COleDateTime,int>(v_dDate_Select[vi].v_dDate,v_dDate_Select[vi].v_iDay+1));	//容器增加【日期】
	}

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

	//【4】获取性能表(15mins：每天一个表；  Hour：5天一个表，一月六个表；)
	v_iNum=0;															//计数器
	mDate_Int::iterator		v_pDate;									//迭代器
	v_pDate=v_mDate_Select.begin();										//首指针
	while(v_pDate!=v_mDate_Select.end())								//结束？？？
	{
		//获取：日期范围
		v_dDate_Start=v_pDate->first;									//获得：查询开始日期
		v_dDate_Start.SetDateTime(v_dDate_Start.GetYear(),v_dDate_Start.GetMonth(),v_dDate_Start.GetDay(),0,0,0);	//设置时间：00:00:00
		v_iDay_Sum=v_pDate->second;										//获得：查询天数
		v_dDate_End =v_dDate_Start + COleDateTimeSpan(v_iDay_Sum,0,0,0);//获得：查询结束日期

		//获取：性能数据
		v_iNum++;														//计数器
		if (m_cType.GetCurSel()==1)										//15m粒度
		{
			while(v_dDate_Start<v_dDate_End)							//开始日期<结束日期
			{
				v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";	//开始时间
				v_sTime_End=v_dDate_Start.Format("%Y-%m-%d")+" 23:59:59";	//结束时间
				v_sTemp=v_dDate_Start.Format("%Y%m%d");					//年月日
				My_LoadData_PM_Table(v_sTemp,v_sCell,v_sTime_Start,v_sTime_End,v_iNum,v_mDate_Select.size());	//装载性能数据
				v_dDate_Start+=COleDateTimeSpan(1,0,0,0);				//增加一天
			}
		}
		else
		{
			v_dDate=v_dDate_Start;										//赋值：当前日期
			v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());		//获取表序号
			while(v_dDate<v_dDate_End)									//时间范围内？？？
			{
				if (v_pIterator_Frame->second.v_iThread_Exit==2)		//强制退出？？？
					break;												//退出

				v_dDate+=COleDateTimeSpan(1,0,0,0);						//增加一天
				if (My_Get_Table_Num(v_dDate.GetDay())!=v_sTable_Num || v_dDate>=v_dDate_End)
				{
					v_sTime_Start=v_dDate_Start.Format("%Y-%m-%d")+" 00:00:00";		//开始时间
					v_sTime_End=v_dDate.Format("%Y-%m-%d")+" 00:00:00";	//结束时间
					v_sTemp=v_dDate_Start.Format("%Y%m");				//年月
					v_sTemp=v_sTemp+"_"+v_sTable_Num;					//文件名称
					My_LoadData_PM_Table(v_sTemp,v_sCell,v_sTime_Start,v_sTime_End,v_iNum,v_mDate_Select.size());	//装载性能数据
					v_dDate_Start=v_dDate;									//变更：开始时间
					v_sTable_Num=My_Get_Table_Num(v_dDate_Start.GetDay());	//获取表序号
				}
			}
		}

		v_pDate++;														//递加
	}

	v_dEnd=COleDateTime::GetCurrentTime();

	COleDateTimeSpan v_dSpan=v_dEnd-v_dBeg;								//时差
	int  ss=v_dSpan.GetTotalSeconds();
	v_sTemp.Format("%d",ss);
	m_cSpan.SetWindowText(v_sTemp);										//显示：时差

	//【5】显示：在列表中(数据在容器中)
	My_PM_Show_Data();													//显示获得的性能数据
	
	//【6】容器清除
	v_mDate_Select.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取表序号
//	工	  作：1：1-5；	2：6-10；	3：11-15；	4：16-20；	5：21-25；	6：26--
//------------------------------------------------------------------------------------------------------			
CString CLTE_PM_Data_Cell::My_Get_Table_Num(int v_iDay)
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
//	函数功能：虚拟列表消息响应函数
//	工	  作：依据列表需要的元素参数(行、列)，在数据容器中查询数据，赋值到列表中。
//------------------------------------------------------------------------------------------------------	
void CLTE_PM_Data_Cell::OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取：虚拟列表显示数据获取
	m_cList_PM.Get_List_Data(pDispInfo);								//虚拟列表显示数据获取

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能信息【性能统计】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_LoadData_PM_Tj()
{
	CString			v_sTemp,v_sCell="",v_sTime,v_sDate;					//临时变量
	bool			v_bPM=false;										//临时变量
	COleDateTime	v_dDate_Start,v_dDate_End,v_dDate;					//查询开始日期、结束日期
	CString			v_sTime_Start,v_sTime_End;							//日期
	int				v_iSum_Time,v_iSum_Cell;							//临时变量

	int				v_iHour,v_iDay=-1;									//小时、天
	map_PM_Time_P	v_mPM_Time_P;										//容器
	Time_PM_Data	v_cTime_PM_Data;									//结构
	mString_Double	v_mPM_Tj_Date;										//性能统计：日期
	mString_Double	v_mPM_Tj_Cell;										//性能统计：Cell

	//【1】数据初始化
	My_PM_Data_P_Delete();												//容器清除
	m_cList_PM.DeleteAllItems();										//清除列表数据
	m_cList_Tj.DeleteAllItems();										//清除列表数据

	//【2】性能处理：有选中的性能？？？
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

	//【3】时段性能统计
	double					v_dValue;									//临时变量
	PM_Time_P_Struct		v_cPM_Time_P;								//结构
	mInt_String::iterator	v_pTime;									//迭代器

	map_PM_Cell::iterator	v_pPM_Cell;									//迭代器
	map_PM_Time::iterator	v_pPM_Time;									//迭代器
	mString_Double::iterator v_pPM_Name;								//迭代器

	map_PM_Cell_P::iterator	v_pPM_Cell_P;								//迭代器
	map_PM_Time_P::iterator	v_pPM_Time_P;								//迭代器
	mString_Double::iterator v_pPM_Name_P;								//迭代器

	m_cList_Tj.SetRedraw(false);										//禁止闪烁
	for(v_pPM_Cell=v_mPM_Cell.begin();v_pPM_Cell!=v_mPM_Cell.end();v_pPM_Cell++)	//遍历：小区 ？？？
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//强制退出？？？
			break;														//退出

		v_mPM_Tj_Cell.clear();											//容器清除
		v_iSum_Cell=0;													//初始化：小区时段数量
		v_mPM_Tj_Date.clear();											//容器清除
		v_iSum_Time=0;													//初始化：日期数量
		v_iDay=-1;														//初始化：日期
		for(v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.begin();v_pPM_Time!=v_pPM_Cell->second.v_mTime_PM_Data.end();v_pPM_Time++)	//遍历：粒度 ？？？
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			if(v_iDay!=v_pPM_Time->first.GetDay())						//天变化？？？
			{
				if(v_mPM_Tj_Date.size()!=0)								//日期统计？？？
					My_PM_Show_Data_Tj(&v_mPM_Tj_Date,v_pPM_Cell->first,v_pPM_Cell->second.v_sRegion,v_pPM_Cell->second.v_sOnline,v_sDate,v_iSum_Time);	//显示：日期统计

				v_iDay=v_pPM_Time->first.GetDay();						//获取：天
				v_sDate=v_pPM_Time->first.Format("%Y-%m-%d");			//赋值：日期
				v_mPM_Tj_Date.clear();									//容器清除
				v_iSum_Time=0;											//初始化：日期数量
			}
			if (m_cTime_Check.GetCheck())								//时段有效？？？
			{
				//获取：小时，粒度在时段内，则统计；
				v_iHour=v_pPM_Time->first.GetHour();					//获取：粒度小时
				v_sTime=v_pPM_Time->first.Format("%Y-%m-%d_");			//赋值：日期
				v_pTime=v_mTime.find(v_iHour);							//查找：小时？？？？
				if (v_pTime==v_mTime.end())								//粒度：不在时段内？？？
					continue;											//下一粒度

				//开始统计：
				v_sTime+=v_pTime->second;								//获取：时段
				v_pPM_Cell_P=v_mPM_Cell_P.find(v_pPM_Cell->first);		//小区存在？？？
				if (v_pPM_Cell_P==v_mPM_Cell_P.end())					//不存在？？？
				{
					v_cPM_Time_P.v_sOnline=v_pPM_Cell->second.v_sOnline;//状态
					v_cPM_Time_P.v_sRegion=v_pPM_Cell->second.v_sRegion;//营业区
					v_mPM_Cell_P.insert(pair<CString,PM_Time_P_Struct>(v_pPM_Cell->first,v_cPM_Time_P));	//容器增加
				}
				v_pPM_Cell_P=v_mPM_Cell_P.find(v_pPM_Cell->first);

				v_pPM_Time_P=v_pPM_Cell_P->second.v_mTime_PM_Data.find(v_sTime);	//查找：时段 ？？？
				if (v_pPM_Time_P==v_pPM_Cell_P->second.v_mTime_PM_Data.end())		//不存在
				{
					v_cTime_PM_Data.v_iSum=0;							//初始化：时段数量
					v_pPM_Cell_P->second.v_mTime_PM_Data.insert(pair<CString,Time_PM_Data>(v_sTime,v_cTime_PM_Data));	//容器增加
				}
				v_pPM_Time_P=v_pPM_Cell_P->second.v_mTime_PM_Data.find(v_sTime);	//
				v_pPM_Time_P->second.v_iSum++;							//时段总数++
			}
			v_iSum_Cell++;												//计数器：小区时段数量
			v_iSum_Time++;												//计数器：日期时段数量
			for (v_pPM_Name=v_pPM_Time->second.begin();v_pPM_Name!=v_pPM_Time->second.end();v_pPM_Name++)	//遍历：性能？？？
			{
				if (v_pPM_Name->second!=-1)								//数据异常？？？
					v_dValue=v_pPM_Name->second;						//赋值：有效数据
				else
					v_dValue=0;											//数据异常：赋值为0
				//(1) 时段统计
				if (m_cTime_Check.GetCheck())							//时段有效？？？
				{
					v_pPM_Name_P=v_pPM_Time_P->second.v_mPM_Data.find(v_pPM_Name->first);	//查找：性能？？？
					if (v_pPM_Name_P==v_pPM_Time_P->second.v_mPM_Data.end())	//没找到？？？
						v_pPM_Time_P->second.v_mPM_Data.insert(pair<CString,double>(v_pPM_Name->first,v_dValue));	//容器增加
					else
						v_pPM_Name_P->second+=v_dValue;					//求和
				}

				//(2) 日期统计
				v_pPM_Name_P=v_mPM_Tj_Date.find(v_pPM_Name->first);		//查找：性能？？？
				if (v_pPM_Name_P==v_mPM_Tj_Date.end())					//没找到？？？
					v_mPM_Tj_Date.insert(pair<CString,double>(v_pPM_Name->first,v_dValue));		//容器增加
				else
					v_pPM_Name_P->second+=v_dValue;						//求和

				//(3) 小区统计
				v_pPM_Name_P=v_mPM_Tj_Cell.find(v_pPM_Name->first);		//查找：性能？？？
				if (v_pPM_Name_P==v_mPM_Tj_Cell.end())					//没找到？？？
					v_mPM_Tj_Cell.insert(pair<CString,double>(v_pPM_Name->first,v_dValue));		//容器增加
				else
					v_pPM_Name_P->second+=v_dValue;						//求和
			}
		}
		if (v_mPM_Tj_Date.size()!=0)									//日期统计？？？
			My_PM_Show_Data_Tj(&v_mPM_Tj_Date,v_pPM_Cell->first,v_pPM_Cell->second.v_sRegion,v_pPM_Cell->second.v_sOnline,v_sDate,v_iSum_Time);		//显示：日期统计

		if (v_mPM_Tj_Cell.size()!=0)									//小区统计？？？
			My_PM_Show_Data_Tj(&v_mPM_Tj_Cell,v_pPM_Cell->first,v_pPM_Cell->second.v_sRegion,v_pPM_Cell->second.v_sOnline,"小区总计",v_iSum_Cell);	//显示：小区统计
	}
	m_cList_Tj.SetRedraw(true);											//列表：显示

	//【4】显示：在列表中(数据在容器中)
	m_cQuary_Data.SetWindowText("  显示查询结果......");				//显示：查询过程

	My_PM_Show_Data();													//显示获得的性能数据

	//【5】容器清除
	v_mPM_Tj_Date.clear();												//容器清除
	v_mPM_Tj_Cell.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能数据加入虚拟列表【数据在容器v_mPM_Cell中】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_PM_Show_Data()
{
	int		v_iNum3,v_iIndex=0;											//临时变量
	CString	v_sStr,v_sTime,v_sTemp,v_sPm_Value;							//临时变量

	mInt_Double	v_mInt_Double;											//临时变量

	mLTE_PM::iterator		v_pPM_Field_Num;							//迭代器
	map_PM_Cell::iterator	v_pPM_Cell;									//迭代器
	map_PM_Time::iterator	v_pPM_Time;									//迭代器
	mString_Double::iterator v_pPM_Name;								//迭代器

	//时段统计变量
	map_PM_Cell_P::iterator	v_pPM_Cell_P;								//迭代器
	map_PM_Time_P::iterator	v_pPM_Time_P;								//迭代器

	//【1】复位：虚拟列表
	mInt_String		v_mLine_Data;										//虚拟列表行数据
	m_cList_PM.Put_Line_Begin();										//列表添加数据开始

	//【2】判断：明细、时段？？？
	if (!m_cTime_Check.GetCheck())										//时段无效？？？
	{																	//性能明细显示
		//【3】性能明细
		for (v_pPM_Cell=v_mPM_Cell.begin();v_pPM_Cell!=v_mPM_Cell.end();v_pPM_Cell++)	//遍历：小区 ？？？
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			//遍历：时间
			for(v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.begin();v_pPM_Time!=v_pPM_Cell->second.v_mTime_PM_Data.end();v_pPM_Time++)	//遍历：粒度 ？？？
			{
				if (v_pIterator_Frame->second.v_iThread_Exit==2)		//强制退出？？？
					break;												//退出

				//数据复位
				v_mLine_Data.clear();									//容器清除

				v_iIndex++;												//计数器
				v_sTemp.Format(" %06d",v_iIndex);
				v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));							//序号
				v_mLine_Data.insert(pair<int,CString>(1,v_pPM_Cell->first));				//Cell
				v_mLine_Data.insert(pair<int,CString>(2,v_pPM_Cell->second.v_sRegion));		//营业区
				v_mLine_Data.insert(pair<int,CString>(3,v_pPM_Cell->second.v_sOnline));		//状态
				v_sTime=v_pPM_Time->first.Format("%Y-%m-%d_%H:%M:%S");						//赋值：日期
				v_mLine_Data.insert(pair<int,CString>(4,v_sTime));							//日期时间


				//遍历：性能
				v_mInt_Double.clear();									//容器清除
				for (v_pPM_Name=v_pPM_Time->second.begin();v_pPM_Name!=v_pPM_Time->second.end();v_pPM_Name++)	//遍历：性能？？？
				{
					v_pPM_Field_Num = v_mPM_Field_Num.find(v_pPM_Name->first);	//查找：性能名称
					if (v_pPM_Field_Num!=v_mPM_Field_Num.end())			//找到？？？
						v_mInt_Double.insert(pair<int,double>(v_pPM_Field_Num->second.v_iNum,v_pPM_Name->second));	//增加容器：
				}

				//从容器中获取性能数据
				v_iNum3=5;												//起始列
				mInt_Double::iterator v_pNum_Value;						//迭代器
				for (v_pNum_Value=v_mInt_Double.begin();v_pNum_Value!=v_mInt_Double.end();v_pNum_Value++)
				{
					while(v_pNum_Value->first>v_iNum3)					//有空列？？
					{
						v_mLine_Data.insert(pair<int,CString>(v_iNum3,""));		//字段数据：空
						v_iNum3++;										//计数器
					}
					v_sStr.Format("%8.2f",v_pNum_Value->second);		//字符串
					v_mLine_Data.insert(pair<int,CString>(v_iNum3,v_sStr));		//字段数据
					v_iNum3++;											//计数器
				}

				//配置数据放入列表
				m_cList_PM.Put_Line_Data(v_mLine_Data);					//列表添加添加行数据
			}
		}
	}
	else
	{
		//【3】性能时段
		for (v_pPM_Cell_P=v_mPM_Cell_P.begin();v_pPM_Cell_P!=v_mPM_Cell_P.end();v_pPM_Cell_P++)	//遍历：小区 ？？？
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			//遍历：时间
			for(v_pPM_Time_P=v_pPM_Cell_P->second.v_mTime_PM_Data.begin();v_pPM_Time_P!=v_pPM_Cell_P->second.v_mTime_PM_Data.end();v_pPM_Time_P++)	//遍历：粒度 ？？？
			{
				if (v_pIterator_Frame->second.v_iThread_Exit==2)		//强制退出？？？
					break;												//退出

				//数据复位
				v_mLine_Data.clear();									//容器清除

				v_iIndex++;												//计数器
				v_sTemp.Format(" %06d",v_iIndex);
				v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));							//序号
				v_mLine_Data.insert(pair<int,CString>(1,v_pPM_Cell_P->first));				//Cell
				v_mLine_Data.insert(pair<int,CString>(2,v_pPM_Cell_P->second.v_sRegion));	//营业区
				v_mLine_Data.insert(pair<int,CString>(3,v_pPM_Cell_P->second.v_sOnline));	//状态
				v_mLine_Data.insert(pair<int,CString>(4,v_pPM_Time_P->first));				//日期时间

				//遍历：性能
				v_mInt_Double.clear();									//容器清除
				for (v_pPM_Name=v_pPM_Time_P->second.v_mPM_Data.begin();v_pPM_Name!=v_pPM_Time_P->second.v_mPM_Data.end();v_pPM_Name++)	//遍历：性能？？？
				{
					v_pPM_Field_Num = v_mPM_Field_Num.find(v_pPM_Name->first);	//查找：性能名称
					if (v_pPM_Field_Num!=v_mPM_Field_Num.end())			//找到？？？
					{
						if (m_cTj_Type.GetCurSel()==0)					//显示类型：求和
							v_mInt_Double.insert(pair<int,double>(v_pPM_Field_Num->second.v_iNum,v_pPM_Name->second));	//增加容器：
						else											//显示类型：平均
						{
							if (v_pPM_Time_P->second.v_iSum!=0)			//统计次数不为零？？？
								v_mInt_Double.insert(pair<int,double>(v_pPM_Field_Num->second.v_iNum,v_pPM_Name->second/v_pPM_Time_P->second.v_iSum));	//增加容器：平均
						}
					}
				}

				//从容器中获取性能数据
				v_iNum3=5;												//起始列
				mInt_Double::iterator v_pNum_Value;						//迭代器
				for (v_pNum_Value=v_mInt_Double.begin();v_pNum_Value!=v_mInt_Double.end();v_pNum_Value++)
				{
					while(v_pNum_Value->first>v_iNum3)					//有空列？？
					{
						v_mLine_Data.insert(pair<int,CString>(v_iNum3,"-1"));		//字段数据：空
						v_iNum3++;										//计数器
					}
					v_sStr.Format("%8.2f",v_pNum_Value->second);		//字符串
					v_mLine_Data.insert(pair<int,CString>(v_iNum3,v_sStr));		//字段数据
					v_iNum3++;											//计数器
				}

				//配置数据放入列表
				m_cList_PM.Put_Line_Data(v_mLine_Data);					//列表添加添加行数据
			}
		}
	}

	//【4】复位
	v_mInt_Double.clear();												//容器清除

	//【5】设置：虚拟列表有效
	v_mLine_Data.clear();												//容器清除
	m_cList_PM.Put_Line_End();											//列表添加数据结束
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：显示统计的时段性能明细【在容器v_mPM_Cell_P中】(已由虚拟列表替换)
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_PM_Show_Data_P()
{
	CString					v_sPm_Value,v_sTime,v_sTemp;				//临时变量
	int						v_iIndex;									//临时变量

	mLTE_PM::iterator		v_pPM_Field_Num;							//迭代器
	map_PM_Cell_P::iterator	v_pPM_Cell_P;								//迭代器
	map_PM_Time_P::iterator	v_pPM_Time_P;								//迭代器
	mString_Double::iterator v_pPM_Name;								//迭代器

	m_cList_PM.SetRedraw(false);										//禁止闪烁
	//遍历：小区
	for (v_pPM_Cell_P=v_mPM_Cell_P.begin();v_pPM_Cell_P!=v_mPM_Cell_P.end();v_pPM_Cell_P++)	//遍历：小区 ？？？
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//强制退出？？？
			break;														//退出

		//遍历：时间
		for(v_pPM_Time_P=v_pPM_Cell_P->second.v_mTime_PM_Data.begin();v_pPM_Time_P!=v_pPM_Cell_P->second.v_mTime_PM_Data.end();v_pPM_Time_P++)	//遍历：粒度 ？？？
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_iIndex = m_cList_PM.InsertItem(0xffffff,_T("0"));
			v_sTemp.Format("%06d",v_iIndex+1);
			m_cList_PM.SetItemText(v_iIndex,0,v_sTemp);					//序号
			m_cList_PM.SetItemText(v_iIndex,1,v_pPM_Cell_P->first);		//Cell
			m_cList_PM.SetItemText(v_iIndex,2,v_pPM_Cell_P->second.v_sRegion);	//营业区
			m_cList_PM.SetItemText(v_iIndex,3,v_pPM_Cell_P->second.v_sOnline);	//状态
			m_cList_PM.SetItemText(v_iIndex,4,v_pPM_Time_P->first);		//

			//遍历：性能
			for (v_pPM_Name=v_pPM_Time_P->second.v_mPM_Data.begin();v_pPM_Name!=v_pPM_Time_P->second.v_mPM_Data.end();v_pPM_Name++)	//遍历：性能？？？
			{
				v_pPM_Field_Num = v_mPM_Field_Num.find(v_pPM_Name->first);	//查找：性能名称
				if (v_pPM_Field_Num!=v_mPM_Field_Num.end())				//找到？？？
				{
					//显示
					if (m_cTj_Type.GetCurSel()==0)						//显示类型：求和
						v_sTemp.Format("%8.2f",v_pPM_Name->second);		//字符串：求和
					else												//显示类型：平均
					{
						if (v_pPM_Time_P->second.v_iSum!=0)				//统计次数不为零？？？
							v_sTemp.Format("%8.2f",v_pPM_Name->second/v_pPM_Time_P->second.v_iSum);	//字符串：平均
						else
							v_sTemp="-1";								//无效
					}
					m_cList_PM.SetItemText(v_iIndex,v_pPM_Field_Num->second.v_iNum,v_sTemp);	//性能
				}
			}
		}
	}
	m_cList_PM.SetRedraw(true);											//列表：显示
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：显示时段性能数据
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_PM_Show_Data_Tj(mString_Double* v_mPM_Tj_Date,CString v_sCell,CString v_sRegion,CString v_sOnline,CString v_sDate,int v_iSum_Time)
{
	CString					v_sTemp;									//临时变量
	int						v_iIndex;									//临时变量

	mLTE_PM::iterator		v_pPM_Field_Num;							//迭代器
	mString_Double::iterator v_pPM_Name;								//迭代器

	v_iIndex = m_cList_Tj.InsertItem(0xffffff,_T("0"));
	v_sTemp.Format("%06d",v_iIndex+1);
	m_cList_Tj.SetItemText(v_iIndex,0,v_sTemp);							//序号
	m_cList_Tj.SetItemText(v_iIndex,1,v_sCell);							//Cell
	m_cList_Tj.SetItemText(v_iIndex,2,v_sRegion);						//营业区
	m_cList_Tj.SetItemText(v_iIndex,3,v_sOnline);						//状态
	m_cList_Tj.SetItemText(v_iIndex,4,v_sDate);							//日期

	for (v_pPM_Name=v_mPM_Tj_Date->begin();v_pPM_Name!=v_mPM_Tj_Date->end();v_pPM_Name++)	//遍历：性能？？？
	{
		v_pPM_Field_Num = v_mPM_Field_Num.find(v_pPM_Name->first);		//查找：性能名称
		if (v_pPM_Field_Num!=v_mPM_Field_Num.end())						//找到？？？
		{
			if (m_cTj_Type.GetCurSel()==0)								//显示类型：求和
				v_sTemp.Format("%12.2f",v_pPM_Name->second);			//字符串
			else
			{
				if (v_iSum_Time!=0)										//统计次数不为零？？？
					v_sTemp.Format("%8.2f",v_pPM_Name->second/v_iSum_Time);		//字符串：平均
				else
					v_sTemp="-1";										//无效
			}
			m_cList_Tj.SetItemText(v_iIndex,v_pPM_Field_Num->second.v_iNum,v_sTemp);		//性能
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能表【每次获取多个性能表】
//	参	  数：v_sTable：年月【201605】；	v_sCell：小区；		v_sDate_Ben：开始日期；		v_sDate_End：结束日期；	v_iNum：日期序号；v_iSum：日期总数；
//	依    据：【表名称：15m/1H粒度；性能类型；】【行：小区、日期】【列：性能类型】
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_LoadData_PM_Table(CString v_sDate,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End,int v_iNum,int v_iSum)
{
	CString			v_sTemp,v_sTable,v_sField;							//临时变量
	int				v_iPM_Num,v_iPM_Sum;								//临时变量
	CString			v_sWorking_1,v_sWorking_2,v_sWorking_3;				//性能获取进度

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

	//【2】性能获取(依据性能种类(原始：6种，计算性能1种，自定义1种，共8种)，每种性能获取一次)
	v_iPM_Num=0;														//计数器
	for (v_pLTE_PM_Type=v_mLTE_PM_Type.begin();v_pLTE_PM_Type!=v_mLTE_PM_Type.end();v_pLTE_PM_Type++)
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//强制退出？？？
			break;														//退出

		//分为两类：(1)自定义；(2)其它：原始、计算；
		if (v_pLTE_PM_Type->first!="自定义")							//类1：其它
		{
			//【3】获取：字段信息
			v_sField="";												//复位：字段
			for (v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
			{
				if (v_pPM_Name->second.v_bOK)							//性能选中？？？	
				{
					if (!v_sField.IsEmpty())							//字段不为空？？？
						v_sField+=",";									//分隔符
					v_sField+=v_pPM_Name->first;						//获取：字段信息
				}
			}

			//【4】获取PM数据(15mins：表名称_年月日；1h：表名称_年月_序号；)
			if (!v_sField.IsEmpty())									//字段有效？？？
			{
				//获取：性能【1个性能表】
				v_iPM_Num++;											//计数器
				v_sTable = "Pm_LTE_"+v_pLTE_PM_Type->second.v_sTable_Key+"_"+v_sDate;					//表名称
				My_LoadData_PM_Data(v_pLTE_PM_Type,v_sTable,v_sField,v_sCell,v_sDate_Ben,v_sDate_End);	//装载性能数据
			}
		}
		else															//类2：自定义
		{
			v_sField="";												//复位：字段
			for (v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
			{
				if (v_pPM_Name->second.v_bOK)							//性能选中？？？	
				{
					v_sTable = "Pm_LTE_"+v_pLTE_PM_Type->second.v_sTable_Key+"_"+v_sDate;				//表名称
					My_LoadData_PM_Data_Cal(v_sTable,v_pPM_Name->first,v_sCell,v_sDate_Ben,v_sDate_End);//装载性能数据
					v_sField="执行";									//设置：有效标志
				}
			}
			if (!v_sField.IsEmpty())									//字段有效？？？
				v_iPM_Num++;											//计数器
		}
		
		//显示：工作进度
		v_sWorking_1.Format("  查询日期组：%d/%d，",v_iNum,v_iSum);		//性能查询进度【日期(多个)】
		v_sWorking_2=v_sDate_Ben.Left(10);								//查询日期
		v_sWorking_3.Format("，查询性能组：%d/%d，请稍候......",v_iPM_Num,v_iPM_Sum);		//性能查询进度【性能(多个)】
		m_cQuary_Data.SetWindowText(v_sWorking_1+"查询日期："+v_sWorking_2+v_sWorking_3);	//显示：查询过程
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能数据
//	参    数：v_pLTE_PM_Type：迭代器；	v_sTable：表名称；	v_sField：字段；	v_sCell：小区；		v_sDate_Ben：开始日期；		v_sDate_End：结束日期
//	结    果：性能数据放入容器
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_LoadData_PM_Data(mLTE_PM_Type::iterator v_pLTE_PM_Type,CString v_sTable,CString v_sField,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End)
{
	CString			v_sSql,v_sPM_Cell,v_sRegion,v_sOnline,v_sField_New="";	//临时变量
	CString			v_sTemp;											//临时变量
	COleDateTime	v_tDate;											//日期
	double			v_dValue;											//性能数值
	int				v_iCell_ID=-1,v_iCell_ID_New=-1,v_iLen;				//临时变量
	PM_Time_Struct	v_cPM_Time;											//结构

	set<CString>	v_mTable_Columns;									//表结构
	set<CString>::iterator	v_pTable_Columns;							//表结构

	map_PM_Time				v_mPM_Time;									//容器
	mString_Double			v_mPM_Name;									//容器

	mLTE_PM::iterator		v_pPM_Name;									//迭代器
	map_PM_Cell::iterator	v_pPM_Cell;									//迭代器
	map_PM_Time::iterator	v_pPM_Time;									//迭代器
	mString_Double::iterator v_pPM_Value;								//迭代器
	map_Cell_ID::iterator	v_pIter_Cell;								//迭代器

	_variant_t		Var;
	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		//【1】检查：表名称是否有效【依据：表名称集合v_mTable_Name】【如果表名称无效，则查不出性能数据。】
		set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//查找？？
		if (v_pTable==v_mTable_Name.end())								//没找到：数据表
			return;														//无效返回

		//【2】获取数据表结构，调整查询字段信息
		My_Load_Columns(&v_mTable_Columns,v_sTable);

		while(!v_sField.IsEmpty())										//字段比较：表存在，ok； 不存在：剔出；
		{
			v_iLen=v_sField.Find(",");									//
			if (v_iLen>=0)
			{
				v_sTemp=v_sField.Left(v_iLen);							//获取：字段
				v_sField=v_sField.Mid(v_iLen+1);						//剔出
			}
			else
			{
				v_sTemp=v_sField;										//获取：字段
				v_sField="";											//无效
			}
			
			v_pTable_Columns=v_mTable_Columns.find(v_sTemp);			//字段存在？？？
			if (v_pTable_Columns!=v_mTable_Columns.end())				//存在？？？
			{
				if (!v_sField_New.IsEmpty())							//有字段？？？
					v_sField_New+=",";									//加：逗号
				v_sField_New+="["+v_sTemp+"]";							//叠加
			}
		}

		//【3】 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(10*60);							//超时设置(10分钟)

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zTime,"+v_sField_New+" from "+v_sTable+" where zCell in("+v_sCell+") and zTime>='"+v_sDate_Ben
				+"' and zTime<'"+v_sDate_End+"' order by zCell";	//查询数据库：系统日志

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			//【4】小区获取：小区_ID ==> 小区名称
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
				v_sPM_Cell="无效";

			//【5】粒度获取
			Var=v_pRecordset->GetCollect((_variant_t)"zTime");
			if (Var.vt != VT_NULL)
				v_tDate=Var.date;
			else
				v_tDate=COleDateTime::GetCurrentTime();					//默认：当前日期


			//【6】获取：小区状态、区域
			v_pPM_Cell=v_mPM_Cell.find(v_sPM_Cell);						//查找：小区 ？？？
			if (v_pPM_Cell==v_mPM_Cell.end())							//不存在
			{
				v_cPM_Time.v_sOnline=v_sOnline;							//状态
				v_cPM_Time.v_sRegion=v_sRegion;							//区域
				v_mPM_Cell.insert(pair<CString,PM_Time_Struct>(v_sPM_Cell,v_cPM_Time));	//容器增加
			}
			v_pPM_Cell=v_mPM_Cell.find(v_sPM_Cell);						//
			
			//【7】粒度增加
			v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.find(v_tDate);//查找：粒度 ？？？
			if (v_pPM_Time==v_pPM_Cell->second.v_mTime_PM_Data.end())	//不存在
				v_pPM_Cell->second.v_mTime_PM_Data.insert(pair<COleDateTime,mString_Double>(v_tDate,v_mPM_Name));	//容器增加
			v_pPM_Time=v_pPM_Cell->second.v_mTime_PM_Data.find(v_tDate);//

			//【8】字段获取：遍历
			v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();		//首指针
			while(v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())	//结束？？
			{
				if (v_pPM_Name->second.v_bOK)							//选中？？？
				{
					//【9】字段存在
					v_pTable_Columns=v_mTable_Columns.find(v_pPM_Name->first);
					if (v_pTable_Columns!=v_mTable_Columns.end())		//存在？？？
					{
						Var=v_pRecordset->GetCollect((_variant_t)v_pPM_Name->first);
						if (Var.vt != VT_NULL)
							v_dValue=Var.dblVal;
						else
							v_dValue=0;

						v_pPM_Value=v_pPM_Time->second.find(v_pPM_Name->first);		//查找：性能名称？？？
						if (v_pPM_Value==v_pPM_Time->second.end())					//不存在？？？
							v_pPM_Time->second.insert(pair<CString,double>(v_pPM_Name->first,v_dValue));	//容器增加
					}
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

	//【10】容器清除
	v_mTable_Columns.clear();											//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载自定义性能数据
//	参	  数：v_sTable：表名称；	v_sName_Cal：性能名称；	v_sCell：小区；	v_sDate_Ben：开始日期；		v_sDate_End：结束日期
//	依    据：【表名称：15m/1H粒度；性能类型；】【行：小区、日期】【列：性能类型】
//	工	  作：依据性能名称(v_sName_Cal)，确定性能类型及其有效性能，获取性能数据，依据自定义公式计算性能，保存；
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_LoadData_PM_Data_Cal(CString v_sTable,CString v_sName_Cal,CString v_sCell,CString v_sDate_Ben,CString v_sDate_End)
{
	CString			v_sSql;												//临时变量
	CString			v_sTemp,v_sTime,v_sFormula,v_sType,v_sKey,v_sField;	//临时变量
	CString			v_sPM_Cell,v_sRegion,v_sOnline;						//临时变量
	int				v_iID=0,v_iCell_ID;									//临时变量
	int				v_iCell_ID_New=-1;									//临时变量
	COleDateTime	v_tDate;											//日期
	double			v_dValue;											//性能数值
	PM_Time_Struct	v_cPM_Time;											//结构

	//计算公式
	CMyFormula		v_cFormula;											//公式计算

	mString_Double	v_mLTE_PM;											//自定义性能容器(有效性能)
	mString_Double	v_mPM_Name;											//容器

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器
	map_Cell_ID::iterator	v_pIter_Cell;								//迭代器
	mString_Double::iterator	v_pIter_Value;							//迭代器
	map_PM_Cell::iterator	v_pPM_Cell;									//迭代器
	map_PM_Time::iterator	v_pPM_Time;									//迭代器

	//【1】获取：自定义性能信息[性能类型、自定义公式等]
	if(!My_Get_Formula(v_sName_Cal,&v_sFormula,&v_sType))				//获取：性能类型、自定义公式、表Key
		return;															//获取失败
	
	if (v_sType=="上下文建立")											//类型==>表名称
		v_sKey="TXT";
	else if (v_sType=="接通")
		v_sKey="RRC";
	else if (v_sType=="切换")
		v_sKey="THO";
	else if (v_sType=="MAC层")
		v_sKey="MAC";
	else if (v_sType=="流量")
		v_sKey="PDCP";
	else if (v_sType=="其他")
		v_sKey="PHY";

	v_sTable.Replace("Def",v_sKey);										//调整表名称[由自定义改为性能类型]

	//【2】检查：表名称是否有效【依据：表名称集合v_mTable_Name】【如果表名称无效，则查不出性能数据。】
	set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);		//查找？？
	if (v_pTable==v_mTable_Name.end())									//没找到：数据表
		return;															//无效返回

	//【3】公式检查，1[v_mLTE_PM]
	v_cFormula.v_mFormula.clear();										//队列清除
	v_cFormula.My_Get_Formula(v_sFormula);								//分解公式表达式(结果在队列中)

	v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);						//查找：性能类型
	if (v_pLTE_PM_Type==v_mLTE_PM_Type.end())							//没找到？？？
		return;															//查找失败

	//设置：有效的性能选择状态[依据计算公式]
	v_sField="";														//默认
	for (v_cFormula.v_pFormula=v_cFormula.v_mFormula.begin();
		v_cFormula.v_pFormula!=v_cFormula.v_mFormula.end();
		v_cFormula.v_pFormula++)										//遍历公式元素
	{
		if (v_cFormula.v_pFormula->v_bFormula==1)						//运算值？？？
		{
			v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.find(v_cFormula.v_pFormula->v_sPM_Formula);	//查找：公式元素是否是原始性能？？？
			if (v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end())		//找到？？？
			{
				v_mLTE_PM.insert(pair<CString,double>(v_pPM_Name->first,0));	//增加容器

				//数据获取字段
				if (!v_sField.IsEmpty())								//字段不为空？？？
					v_sField+=",";										//分隔符
				v_sField+="["+v_pPM_Name->first+"]";					//获取：字段信息
			}
		}
	}

	//【4】数据库操作
	_variant_t		Var;
	_RecordsetPtr	v_pRecordset;										//记录集
	CMyADOConn		v_cAdoConn;											//数据库实例
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zTime,"+v_sField+" from "+v_sTable+" where zCell in("+v_sCell+") and zTime>='"+v_sDate_Ben
				+"' and zTime<'"+v_sDate_End+"' order by zCell";		//查询数据库：系统日志

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		//【5】获取性能数据
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_tDate=v_cAdoConn.GetField_DataTime("zTime");				//zTime
			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_iCell_ID=Var.intVal;									//获取：Cell_ID
				//由ID==>Cell
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
				v_sPM_Cell="无效";										//无效小区

			//【5】加入数据容器：小区、日期
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

			//【6】获取：性能数值
			v_pIter_Value=v_mLTE_PM.begin();							//首指针
			while(v_pIter_Value!=v_mLTE_PM.end())						//结束？？[遍历性能：读取数值]
			{
				v_dValue=v_cAdoConn.GetField_Double(v_pIter_Value->first);	//获取：性能数值
				v_pIter_Value->second=v_dValue;							//赋值：性能数据

				v_pIter_Value++;										//递加
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
					v_pIter_Value=v_mLTE_PM.find(v_cFormula.v_pFormula->v_sPM_Formula);	//查找：Counter
					if (v_pIter_Value!=v_mLTE_PM.end())					//找到？？？
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
		
			//【8】保存到数据容器
			v_pPM_Time->second.insert(pair<CString,double>(v_sName_Cal,v_fValue));	//容器增加

			v_pRecordset->MoveNext();									//下一条
		}
		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	//性能容器
	v_mLTE_PM.clear();													//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取性能类型、自定义公式
//------------------------------------------------------------------------------------------------------			
bool CLTE_PM_Data_Cell::My_Get_Formula(CString v_sName,CString* v_sFormula,CString* v_sType)
{
	CString		v_sSql;													//临时变量
	CString		v_sTemp;												//临时变量
	bool		v_bOK=false;											//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select * from CON_Pm_LTE_Case_Cal where zAccount='"+theApp.a_sUser_Name+"' and zName='"+v_sName+"'";	//查询数据库

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		if(!v_pRecordset->adoEOF)
		{
			*v_sType=v_cAdoConn.GetField_String("zType");				//zType
			*v_sFormula=v_cAdoConn.GetField_String("zPm_Formula");		//zPm_Formula
			v_bOK=true;													//置位
		}
		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	return	v_bOK;														//返回
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能数据删除
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_PM_Data_Delete()
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
//	函数功能：统计的时段性能数据删除
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_PM_Data_P_Delete()
{
	map_PM_Cell_P::iterator	v_pPM_Cell_P;								//迭代器
	map_PM_Time_P::iterator	v_pPM_Time_P;								//迭代器
	for (v_pPM_Cell_P=v_mPM_Cell_P.begin();v_pPM_Cell_P!=v_mPM_Cell_P.end();v_pPM_Cell_P++)
	{
		for(v_pPM_Time_P=v_pPM_Cell_P->second.v_mTime_PM_Data.begin();v_pPM_Time_P!=v_pPM_Cell_P->second.v_mTime_PM_Data.end();v_pPM_Time_P++)
			v_pPM_Time_P->second.v_mPM_Data.clear();					//容器清除
		v_pPM_Cell_P->second.v_mTime_PM_Data.clear();					//容器清除
	}
	v_mPM_Cell_P.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载Cell信息
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_LoadData_Cell()
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
void CLTE_PM_Data_Cell::My_LoadData_Para()
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

		//【3】获取：性能参数【自定义】
		v_sType="自定义";												//性能类型								
		v_cLTE_PM_Type.v_sTable_Key = "Def";							//表名称-Key
		v_mLTE_PM_Type.insert(pair<CString,LTE_PM_Type_Struct>(v_sType,v_cLTE_PM_Type));	//容器增加

		//获取：性能参数【计算性能】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Pm_LTE_Case_Cal where zAccount='"+theApp.a_sUser_Name+"' order by zName ";	//Sql

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sPm = v_cAdoConn.GetField_String("zName");				//zName

			//保存
			v_pLTE_PM_Type=v_mLTE_PM_Type.find(v_sType);				//查找：性能类型
			if (v_pLTE_PM_Type!=v_mLTE_PM_Type.end())					//找到？？？
			{
				v_cLTE_PM.v_sPM_Name=v_sPm;								//性能：中文名
				v_cLTE_PM.v_sTable_Key="Def";							//性能：表名称
				v_cLTE_PM.v_bOK=false;									//性能：未选中
				v_pLTE_PM_Type->second.v_mLTE_PM.insert(pair<CString,LTE_PM_Struct>(v_sPm,v_cLTE_PM));	//增加：性能参数
			}

			v_pRecordset->MoveNext();									//
		}

		//【4】获取：性能数据表名称【用于发现无效表名称】
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_LTE_%') order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//表名称

			v_mTable_Name.insert(v_sTable);								//增加集合

			v_pRecordset->MoveNext();									//指针++
		}	

		//【5】获取：小区信息【用于小区ID处理】
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
//	函数功能：装载表结构
//	参	  数：v_pTable_Columns：容器指针
//------------------------------------------------------------------------------------------------------			
void CLTE_PM_Data_Cell::My_Load_Columns(set<CString> *v_pTable_Columns,CString v_sTable)
{
	CString			v_sTemp,v_sSql,v_sColumns;							//临时变量


	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from sys.columns where object_id=object_id('"+v_sTable+"')";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sColumns = v_cAdoConn.GetField_String("name");			//结构名称

			v_pTable_Columns->insert(v_sColumns);						//增加集合

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
