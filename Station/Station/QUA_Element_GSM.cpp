// QUA_Element_GSM.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "QUA_Element_GSM.h"


// CQUA_Element_GSM

IMPLEMENT_DYNCREATE(CQUA_Element_GSM, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CQUA_Element_GSM::CQUA_Element_GSM()
	: CFormView(CQUA_Element_GSM::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CQUA_Element_GSM::~CQUA_Element_GSM()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_NodeB);
	DDX_Control(pDX, IDC_SUM_ALARM2, m_cSum_NodeB);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_CHECK1, m_cBTS);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CQUA_Element_GSM, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CQUA_Element_GSM::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL, &CQUA_Element_GSM::OnBnClickedExcel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CQUA_Element_GSM::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_EXCEL2, &CQUA_Element_GSM::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CQUA_Element_GSM::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_CHECK1, &CQUA_Element_GSM::OnBnClickedCheck1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CQUA_Element_GSM::OnNMClickList1)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CQUA_Element_GSM::OnLvnGetdispinfoList1)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST3, &CQUA_Element_GSM::OnLvnGetdispinfoList3)
END_MESSAGE_MAP()


// CQUA_Element_GSM 诊断

#ifdef _DEBUG
void CQUA_Element_GSM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQUA_Element_GSM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQUA_Element_GSM 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="GSM网元查询";										//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗 [BTS]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"BTS",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(2,"中文名",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(3,"BSC",LVCFMT_LEFT,70);						//
	m_cList.InsertColumn(4,"区域",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(5,"所属机房/资源点",LVCFMT_LEFT,200);			//
	m_cList.InsertColumn(6,"硬件",LVCFMT_LEFT,100);						//
	m_cList.InsertColumn(7,"软件",LVCFMT_LEFT,100);						//
	m_cList.InsertColumn(8,"站型",LVCFMT_LEFT,100);						//
	m_cList.InsertColumn(9,"VVIP",LVCFMT_LEFT,60);						//
	m_cList.InsertColumn(10,"状态",LVCFMT_LEFT,60);						//
	m_cList.InsertColumn(11,"覆盖类型",LVCFMT_LEFT,60);					//
	m_cList.InsertColumn(12,"更新时间[实网]",LVCFMT_CENTER,140);		//
	m_cList.InsertColumn(13,"更新时间[资管]",LVCFMT_CENTER,100);		//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//初始化：列表窗 [Cell]
	::SendMessage(m_cList_NodeB.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_NodeB.InsertColumn(0,"序号",LVCFMT_CENTER,60);				//
	m_cList_NodeB.InsertColumn(1,"BTS",LVCFMT_LEFT,60);
	m_cList_NodeB.InsertColumn(2,"小 区 号",LVCFMT_LEFT,60);
	m_cList_NodeB.InsertColumn(3,"中    文    名",LVCFMT_LEFT,140);
	m_cList_NodeB.InsertColumn(4,"区    域",LVCFMT_LEFT,80);
	m_cList_NodeB.InsertColumn(5,"所属机房/资源点",LVCFMT_LEFT,200);	//
	m_cList_NodeB.InsertColumn(6,"状态",LVCFMT_LEFT,60);				//
	m_cList_NodeB.InsertColumn(7,"覆盖类型",LVCFMT_LEFT,60);			//
	m_cList_NodeB.InsertColumn(8,"更新日期[实网]",LVCFMT_CENTER,120);
	m_cList_NodeB.InsertColumn(9,"更新日期[资管]",LVCFMT_CENTER,120);
	m_cList_NodeB.SetExtendedStyle(m_cList_NodeB.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//分窗
	CRect	v_cRect;													//矩形
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//获得：指针
	pWnd->GetWindowRect(&v_cRect);										//获得：尺寸
	ScreenToClient(&v_cRect);											//坐标变换
	pWnd->DestroyWindow();												//销毁：指针

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Main.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList,				// Top pane
		&m_cList_NodeB,			// Bottom pane
		IDC_QUA_Element_GSM,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);

	PostMessage(WM_SIZE);												//最大化

	//信息初始化
	m_cBTS.SetCheck(true);												//站点：有效
	OnBnClickedCheck1();												//查询类型

	v_iList_Item=-1;													//条目选择无效
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CQUA_Element_GSM::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：子窗销毁
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnDestroy()
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
void CQUA_Element_GSM::OnSize(UINT nType, int cx, int cy)
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
//	函数功能：BTS导出
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Cell导出
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_NodeB);										//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序 [BTS]
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort_Virtual(pNMHDR);									//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序 [Cell]
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList_NodeB.My_Sort_Virtual(pNMHDR);								//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：虚拟列表显示数据获取
//	工	  作：依据列表需要的元素参数(行、列)，在数据容器中查询数据，赋值到列表中。
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnLvnGetdispinfoList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	m_cList.Get_List_Data(pDispInfo);									//虚拟列表显示数据获取

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：虚拟列表显示数据获取
//	工	  作：依据列表需要的元素参数(行、列)，在数据容器中查询数据，赋值到列表中。
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	m_cList_NodeB.Get_List_Data(pDispInfo);								//虚拟列表显示数据获取

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：BTS选中
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cKey_Select.ResetContent();										//清除选择内容
	if (m_cBTS.GetCheck())												//BTS选中？？？
	{																	//选中
		m_SplitterPane_Main.ShowTopPane();								//显示：BTS
		m_cKey_Select.AddString("BTS");
		m_cKey_Select.AddString("中文名");
		m_cKey_Select.AddString("BSC");
		m_cKey_Select.AddString("机房/资源点");
	}
	else
	{																	//没选中
		m_SplitterPane_Main.HideTopPane();								//隐藏：BTS
		m_cKey_Select.AddString("Cell");
		m_cKey_Select.AddString("BTS");
		m_cKey_Select.AddString("中文名");
		m_cKey_Select.AddString("机房/资源点");
	}
	m_cKey_Select.SetCurSel(0);											//默认：选择第一个
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击列表窗 [BTS]
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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

			v_iWorking=3;												//工作任务：站点点击有效响应

			v_pIterator_Frame->second.v_iThread_Exit=1;					//线程：工作
			v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);		//开启查询数据库线程
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：小区查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动	[静态门限计算]
//------------------------------------------------------------------------------------------------------
UINT CQUA_Element_GSM::My_Thread_Inoput(LPVOID lparam)
{
	CQUA_Element_GSM *  lp_this = NULL ;
	lp_this = (CQUA_Element_GSM *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效
	lp_this->m_cExcel_NodeB.EnableWindow(false);						//NodeB导出：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效
	lp_this->m_cExcel_NodeB.EnableWindow(true);							//NodeB导出：有效


	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		if (m_cBTS.GetCheck())											//BSC选中？？？
			My_Query_BTS();												//BTS查询
		else
			My_Query_Cell();											//Cell查询
		break;
	case 3:
		My_Query_BTS_Cell();											//BTS列表点击响应
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：BTS查询
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::My_Query_BTS()
{
	// TODO: 在此添加控件通知处理程序代码
	int				v_iID=0;											//KPI数量
	CString			v_sSql,v_sTemp;										//临时变量
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//查询日期准备
	m_cKey.GetWindowText(v_sLog_Key);									//获得：查询KEY
	v_sLog_Key.Replace("[","[[]");										//限制处理
	v_sLog_Key.Replace("'","");											//限制处理
	v_sLog_Key.Trim();													//去掉空格

	v_sSql += "SELECT * FROM GSM_BTS";

	if (!v_sLog_Key.IsEmpty())											//查询KEY处理
	{
		switch (m_cKey_Select.GetCurSel())								//选择？？？
		{
		case 0:															//BTS
			v_sSql+=" where zBTS like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//中文名
			v_sSql+=" where zBTS_Name like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//BSC
			v_sSql+=" where zBSC like '%"+v_sLog_Key+"%'";
			break;
		case 3:															//所属机房/资源点
			v_sSql+=" where zStation like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	v_sSql+=" order by zBTS";

	My_LoadData_BTS_Data(v_sSql);										//获取：BTS信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载BTS信息
//------------------------------------------------------------------------------------------------------			
void CQUA_Element_GSM::My_LoadData_BTS_Data(CString v_sSql)
{
	int				v_iID=0;											//KPI数量
	CString			v_sTemp,v_sDate_Creat;								//临时变量
	CString			v_sBTS,v_sBTS_Name,v_sBSC,v_sRegion,v_sStation,v_sHardware,v_sOper,v_sTel;		//临时变量
	CString			v_sSoftware,v_sVVIP,v_sRF,v_sAgency,v_sOnline,v_sDate,v_sCurrentTime,v_sDevice;	//临时变量
	CString			v_sMaintence,v_sCover_Type;							//临时变量
	COleDateTime	v_dDate_Start;										//查询开始日期

	//虚拟列表
	mInt_String		v_mLine_Data;										//虚拟列表行数据
	m_cList.Put_Line_Begin();											//列表添加数据开始

	_variant_t	Var;
	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		m_cList.DeleteAllItems();										//列表复位
		v_iList_Item=-1;												//条目选择无效
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sBTS = v_cAdoConn.GetField_String("zBTS");				//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sBSC = v_cAdoConn.GetField_String("zBSC");				//zBSC
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sHardware = v_cAdoConn.GetField_String("zHardware");		//zHardware
			v_sSoftware = v_cAdoConn.GetField_String("zBTS_Version");	//zBTS_Version
			v_sDevice = v_cAdoConn.GetField_String("zDevice");			//zDevice
			v_sVVIP = v_cAdoConn.GetField_String("zVVIP");				//zVVIP
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_sCover_Type = v_cAdoConn.GetField_String("zCover_Type");	//zCover_Type
			v_dDate_Start = v_cAdoConn.GetField_DataTime("zDate");		//zDate
			if (v_dDate_Start!=theApp.a_tData_Default)					//日期有效？？？
				v_sDate = v_dDate_Start.Format(_T("%Y-%m-%d"));
			else
				v_sDate="";												//无效日期
			v_dDate_Start = v_cAdoConn.GetField_DataTime("zDate_Update");	//zDate_Update
			if (v_dDate_Start!=theApp.a_tData_Default)					//更新日期？？？
				v_sCurrentTime = v_dDate_Start.Format(_T("%Y-%m-%d"));
			else
				v_sCurrentTime="";										//无效日期

			//数据复位
			v_mLine_Data.clear();										//容器清除

			//显示
			v_sTemp.Format(" %04d",v_iID+1);
			v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));			//序号
			v_mLine_Data.insert(pair<int,CString>(1,v_sBTS));			//BTS
			v_mLine_Data.insert(pair<int,CString>(2,v_sBTS_Name));		//中文名
			v_mLine_Data.insert(pair<int,CString>(3,v_sBSC));			//BSC
			v_mLine_Data.insert(pair<int,CString>(4,v_sRegion));		//区域
			v_mLine_Data.insert(pair<int,CString>(5,v_sStation));		//所属机房/资源点
			v_mLine_Data.insert(pair<int,CString>(6,v_sHardware));		//硬件
			v_mLine_Data.insert(pair<int,CString>(7,v_sSoftware));		//软件
			v_mLine_Data.insert(pair<int,CString>(8,v_sDevice));		//站型
			v_mLine_Data.insert(pair<int,CString>(9,v_sVVIP));			//VVIP
			v_mLine_Data.insert(pair<int,CString>(10,v_sOnline));		//状态
			v_mLine_Data.insert(pair<int,CString>(11,v_sCover_Type));	//覆盖类型
			v_mLine_Data.insert(pair<int,CString>(12,v_sDate));			//入网日期
			v_mLine_Data.insert(pair<int,CString>(13,v_sCurrentTime));	//更新日期

			v_mLine_Data.insert(pair<int,CString>(17,v_sDate_Creat));	//生成日期

			//添加：虚拟列表
			m_cList.Put_Line_Data(v_mLine_Data);						//列表添加行数据

			v_pRecordset->MoveNext();									//
			v_iID++;													//计数：NodeB数量

			if (v_iID%100==0)
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum_NodeB.SetWindowText(v_sTemp);					//显示：NodeB总数
			}
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cSum_NodeB.SetWindowText(v_sTemp);							//显示：NodeB总数

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	//虚拟列表
	m_cList.Put_Line_End();												//列表添加数据结束
	v_mLine_Data.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询Cell信息
//------------------------------------------------------------------------------------------------------
void CQUA_Element_GSM::My_Query_Cell()
{
	int				v_iID=0;											//KPI数量
	CString			v_sSql,v_sTemp;										//临时变量
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//查询日期准备
	m_cKey.GetWindowText(v_sLog_Key);									//获得：查询KEY
	v_sLog_Key.Replace("[","[[]");										//限制处理
	v_sLog_Key.Replace("'","");											//限制处理
	v_sLog_Key.Trim();													//去掉空格

	v_sSql += "SELECT GSM_Cell.*, GSM_BTS.zBTS_NAME, GSM_BTS.zRegion as zRegion_BTS FROM GSM_Cell \
			  left JOIN GSM_BTS ON GSM_Cell.zBTS = GSM_BTS.zBTS";
	if (!v_sLog_Key.IsEmpty())											//查询KEY处理
	{
		switch (m_cKey_Select.GetCurSel())								//选择？？？
		{
		case 0:															//Cell
			v_sSql+=" where zCell like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//BTS
			v_sSql+=" where zBTS like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//中文名
			v_sSql+=" where zBTS_NAME like '%"+v_sLog_Key+"%'";
			break;
		case 3:															//所属机房/资源点
			v_sSql+=" where GSM_BTS.zStation like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	v_sSql+=" order by zCell";

	My_LoadData_Cell_Data(v_sSql);										//获取：Cell信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载 BTS_Cell 信息
//------------------------------------------------------------------------------------------------------			
void CQUA_Element_GSM::My_Query_BTS_Cell()
{
	CString v_sBTS=m_cList.GetItemText(v_iList_Item,1);					//v_sBTS
	CString v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT GSM_Cell.*, GSM_BTS.zBTS_NAME, GSM_BTS.zRegion as zRegion_BTS FROM GSM_Cell \
			  left JOIN GSM_BTS ON GSM_Cell.zBTS = GSM_BTS.zBTS";
	v_sSql+=" where GSM_Cell.zBTS='"+v_sBTS+"' order by zCell";			//SQL
	My_LoadData_Cell_Data(v_sSql);										//执行SQL
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载Cell信息
//------------------------------------------------------------------------------------------------------			
void CQUA_Element_GSM::My_LoadData_Cell_Data(CString v_sSql)
{
	int				v_iID=0;											//KPI数量
	CString			v_sTemp;											//临时变量
	COleDateTime	v_dDate_Start;										//查询开始日期

	_variant_t	Var;
	CString		v_sCover,v_sBTS,v_sBTS_Name,v_sCell,v_sRegion,v_sStation;	//临时变量
	CString		v_sAgency,v_sOnline,v_sDate,v_sCurrentTime,v_sStie_Type;//
	CString		v_sEquip,v_sCGI,v_sTG,v_sTrx,v_sDip,v_sDip_Num,v_sTrx_Type;	//临时变量
	CString		v_sMaintianer,v_sTel,v_sDXU,v_sCDU,v_sDate_Update,v_sOper;

	//虚拟列表
	mInt_String		v_mLine_Data;										//虚拟列表行数据
	m_cList_NodeB.Put_Line_Begin();										//列表添加数据开始

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList_NodeB.DeleteAllItems();									//清除Items

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sBTS = v_cAdoConn.GetField_String("zBTS");				//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sRegion = v_cAdoConn.GetField_String("zRegion_BTS");		//zRegion_BTS
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_sCover = v_cAdoConn.GetField_String("zCover_Type");		//zCover_Type

			v_dDate_Start = v_cAdoConn.GetField_DataTime("zDate");		//zDate 更新时间[实网]
			if (v_dDate_Start!=theApp.a_tData_Default)					//更新日期？？？
				v_sDate = v_dDate_Start.Format(_T("%Y-%m-%d"));
			else
				v_sDate="";												//无效日期
			v_dDate_Start = v_cAdoConn.GetField_DataTime("zDate_Update");	//zDate_Update 更新时间[资管]
			if (v_dDate_Start!=theApp.a_tData_Default)					//更新日期？？？
				v_sDate_Update = v_dDate_Start.Format(_T("%Y-%m-%d"));
			else
				v_sDate_Update="";										//无效日期

			//数据复位
			v_mLine_Data.clear();										//容器清除

			//显示
			v_sTemp.Format("%06d",v_iID+1);
			v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));			//序号
			v_mLine_Data.insert(pair<int,CString>(1,v_sBTS));			//BTS
			v_mLine_Data.insert(pair<int,CString>(2,v_sCell));			//Cell
			v_mLine_Data.insert(pair<int,CString>(3,v_sBTS_Name));		//中文名
			v_mLine_Data.insert(pair<int,CString>(4,v_sRegion));		//区域
			v_mLine_Data.insert(pair<int,CString>(5,v_sStation));		//所属机房/资源点
			v_mLine_Data.insert(pair<int,CString>(6,v_sOnline));		//状态
			v_mLine_Data.insert(pair<int,CString>(7,v_sCover));			//覆盖类型
			v_mLine_Data.insert(pair<int,CString>(8,v_sDate));			//更新日期[实网]
			v_mLine_Data.insert(pair<int,CString>(9,v_sDate_Update));	//更新日期[资管]

			//添加：虚拟列表
			m_cList_NodeB.Put_Line_Data(v_mLine_Data);					//列表添加行数据

			v_pRecordset->MoveNext();									//
			v_iID++;													//计数：告警数量

			if (v_iID%100==0)											//显示：100的整数
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum.SetWindowText(v_sTemp);							//显示：站址总数
			}
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cSum.SetWindowText(v_sTemp);									//显示：站址总数

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	//虚拟列表
	m_cList_NodeB.Put_Line_End();										//列表添加数据结束
	v_mLine_Data.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
