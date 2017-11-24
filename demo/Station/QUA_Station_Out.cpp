// QUA_Station_Out.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "QUA_Station_Out.h"

#include "comdef.h" 

// CQUA_Station_Out

IMPLEMENT_DYNCREATE(CQUA_Station_Out, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CQUA_Station_Out::CQUA_Station_Out()
	: CFormView(CQUA_Station_Out::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CQUA_Station_Out::~CQUA_Station_Out()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_Station);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_TREE1, m_cTree_Station);
	DDX_Control(pDX, IDC_CHECK1, m_cOnline);
	DDX_Control(pDX, IDC_CHECK3, m_cOK);
	DDX_Control(pDX, IDC_BUTTON2, m_cDetail);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CQUA_Station_Out, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CQUA_Station_Out::OnBnClickedQuary)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CQUA_Station_Out::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CQUA_Station_Out::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_EXCEL, &CQUA_Station_Out::OnBnClickedExcel)
	ON_BN_CLICKED(IDC_BUTTON2, &CQUA_Station_Out::OnBnClickedButton2)
END_MESSAGE_MAP()


// CQUA_Station_Out 诊断

#ifdef _DEBUG
void CQUA_Station_Out::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQUA_Station_Out::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQUA_Station_Out 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="室外共址查询";										//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗 [站址]
	::SendMessage(m_cList_Station.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Station.InsertColumn(0,"序号",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(1,"机房/资源点名称",LVCFMT_LEFT,260);	//
	m_cList_Station.InsertColumn(2,"区域",LVCFMT_LEFT,70);				//
	m_cList_Station.InsertColumn(3,"BTS",LVCFMT_RIGHT,60);				//
	m_cList_Station.InsertColumn(4,"Cell",LVCFMT_RIGHT,60);				//
	m_cList_Station.InsertColumn(5,"NodeB",LVCFMT_RIGHT,60);			//
	m_cList_Station.InsertColumn(6,"Cell",LVCFMT_RIGHT,60);				//
	m_cList_Station.InsertColumn(7,"E_NodeB",LVCFMT_RIGHT,60);			//
	m_cList_Station.InsertColumn(8,"Cell",LVCFMT_RIGHT,60);				//
	m_cList_Station.InsertColumn(9,"经度",LVCFMT_RIGHT,80);				//
	m_cList_Station.InsertColumn(10,"纬度",LVCFMT_RIGHT,80);			//
	m_cList_Station.SetExtendedStyle(m_cList_Station.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList_Station.SetNumber(3);										//数字比较
	m_cList_Station.SetNumber(4);
	m_cList_Station.SetNumber(5);
	m_cList_Station.SetNumber(6);
	m_cList_Station.SetNumber(7);
	m_cList_Station.SetNumber(8);
	m_cList_Station.SetNumber(9);
	m_cList_Station.SetNumber(10);

	//树初始化：m_cTree
	v_cImage_Tree.Create(IDB_TREE_STATION,16,1,RGB(255,255,255));		//树形图象列表生成
	//	v_cImage_State.Create(IDB_TREE_CHECK,13, 1, RGB(255,255,255));
	m_cTree_Station.SetImageList(&v_cImage_Tree,TVSIL_NORMAL);			//树初始化
	m_cTree_Station.SetImageList(&v_cImage_State,TVSIL_STATE);
	m_cTree_Station.SetBkColor(RGB(200,200,255));						//设置树形控件的背景色
	m_cTree_Station.SetTextColor(RGB(127,0,0));							//设置文本背景色

	//分窗
	CRect	v_cRect;													//矩形
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//获得：指针
	pWnd->GetWindowRect(&v_cRect);										//获得：尺寸
	ScreenToClient(&v_cRect);											//坐标变换
	pWnd->DestroyWindow();												//销毁：指针

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Main.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_cList_Station,		// Top pane
		&m_cTree_Station,		// Bottom pane
		IDC_QUA_Station_Out_Tree,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);
	m_SplitterPane_Bottom.m_bVertSplitter_Parent=true;					//垂直分隔符为父窗

	PostMessage(WM_SIZE);												//最大化

	//信息初始化
	m_cKey_Select.AddString("机房/资源点");
	m_cKey_Select.AddString("区域");
	m_cKey_Select.SetCurSel(0);											//默认：选择第一个

//	m_cOnline.SetCheck(1);												//在线有效
	m_cOK.SetCheck(1);													//有效站址

	v_iList_Item_Room=-1;												//条目选择无效 【机房】
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CQUA_Station_Out::PreTranslateMessage(MSG* pMsg)
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
void CQUA_Station_Out::OnDestroy()
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
void CQUA_Station_Out::OnSize(UINT nType, int cx, int cy)
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
//	函数功能：统计导出
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show_Name(&m_cList_Station,"室外共址_统计",true);			//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：明细导出
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=2;														//工作任务：明细导出

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序 [站址]
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList_Station.My_Sort(pNMHDR);									//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击列表窗 [站址]
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item_Room>=0)									//有效条目序号？？？
				m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,255,255));	//恢复背景色
			v_iList_Item_Room=pNMListView->iItem;						//获得：选中条目序号
			m_cList_Station.SetItemBkColor(v_iList_Item_Room,RGB(255,0,0));			//设置背景色：红色
			
			My_LoadData_Tree(m_cList_Station.GetItemText(v_iList_Item_Room,1));		//装载机房信息 【绘制Tree】
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询【站址】
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动	[静态门限计算]
//------------------------------------------------------------------------------------------------------
UINT CQUA_Station_Out::My_Thread_Inoput(LPVOID lparam)
{
	CQUA_Station_Out *  lp_this = NULL ;
	lp_this = (CQUA_Station_Out *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效
	lp_this->m_cDetail.EnableWindow(false);								//导出：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效
	lp_this->m_cDetail.EnableWindow(true);								//导出：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_Query_Station();												//站址查询
		break;
	case 2:
		My_OutPut();													//明细导出
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询机房信息
//------------------------------------------------------------------------------------------------------
void CQUA_Station_Out::My_Query_Station()
{
	int				v_iID=0;											//KPI数量
	CString			v_sSql,v_sTemp;										//临时变量
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//查询日期准备
	m_cKey.GetWindowText(v_sLog_Key);									//获得：查询KEY
	v_sLog_Key.Trim();													//去掉空格

	v_sSql += "SELECT * FROM CON_Station"; 

	if (!v_sLog_Key.IsEmpty())											//查询KEY处理
	{
		switch (m_cKey_Select.GetCurSel())								//选择？？？
		{
		case 0:															//站址
			v_sSql+=" where zStation like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//区域
			v_sSql+=" where zRegion like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	v_sSql+=" order by zStation";

	My_LoadData_Station_Data(v_sSql);									//获取：站址信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载站址信息
//------------------------------------------------------------------------------------------------------			
void CQUA_Station_Out::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem,v_iID=0;									//KPI数量
	CString			v_sTemp;											//临时变量
	COleDateTime	v_dDate_Start;										//查询开始日期

	_variant_t	Var;
	CString			v_sStation,v_sRegion,v_sLongitude;					//临时变量
	CString			v_sLatitude,v_sOnline;								//临时变量
	double			v_dValue;											//临时变量
	int				v_iGSM_BTS,v_iGSM_Cell,v_iTD_NodeB,v_iTD_Cell,v_iLTE_NodeB,v_iLTE_Cell,v_iSum;	//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		//树复位
		m_cTree_Station.DeleteAllItems();								//树清除
		//列表复位
		m_cList_Station.DeleteAllItems();								//清除Items
		v_iList_Item_Room=-1;											//无效选项

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_dValue = v_cAdoConn.GetField_Double("zLongitude");		//zLongitude
			v_sLongitude.Format("%10.6f",v_dValue);
			v_dValue = v_cAdoConn.GetField_Double("zLatitude");			//zLatitude
			v_sLatitude.Format("%10.6f",v_dValue);
			v_iGSM_BTS = v_cAdoConn.GetField_Int("zGSM_BTS_OUT");		//zGSM_BTS_OUT
			v_iGSM_Cell = v_cAdoConn.GetField_Int("zGSM_Cell_OUT");		//zGSM_Cell_OUT
			v_iTD_NodeB = v_cAdoConn.GetField_Int("zTD_NodeB_OUT");		//zTD_NodeB_OUT
			v_iTD_Cell = v_cAdoConn.GetField_Int("zTD_Cell_OUT");		//zTD_Cell_OUT
			v_iLTE_NodeB = v_cAdoConn.GetField_Int("zLTE_NodeB_OUT");	//zLTE_NodeB_OUT
			v_iLTE_Cell = v_cAdoConn.GetField_Int("zLTE_Cell_OUT");		//zLTE_Cell_OUT

			//显示
			if (!m_cOnline.GetCheck() || (m_cOnline.GetCheck() && v_sOnline == "在网"))
			{
				v_iSum=v_iGSM_BTS+v_iTD_NodeB+v_iLTE_NodeB;				//基站总计
				if (!m_cOK.GetCheck() || (m_cOK.GetCheck() && v_iSum != 0))
				{
					v_sTemp.Format(" %04d",v_iID+1);
					v_iItem=m_cList_Station.InsertItem(0xffff,v_sTemp);	//加入列表窗
					m_cList_Station.SetItemText(v_iItem,1,v_sStation);	//机房/资源点名称
					m_cList_Station.SetItemText(v_iItem,2,v_sRegion);	//区域
					v_sTemp.Format("%d",v_iGSM_BTS);
					m_cList_Station.SetItemText(v_iItem,3,v_sTemp);		//GSM_BTS
					v_sTemp.Format("%d",v_iGSM_Cell);
					m_cList_Station.SetItemText(v_iItem,4,v_sTemp);		//GSM_Cell
					v_sTemp.Format("%d",v_iTD_NodeB);
					m_cList_Station.SetItemText(v_iItem,5,v_sTemp);		//TD_NodeB
					v_sTemp.Format("%d",v_iTD_Cell);
					m_cList_Station.SetItemText(v_iItem,6,v_sTemp);		//TD_Cell
					v_sTemp.Format("%d",v_iLTE_NodeB);
					m_cList_Station.SetItemText(v_iItem,7,v_sTemp);		//LTE_NodeB
					v_sTemp.Format("%d",v_iLTE_Cell);
					m_cList_Station.SetItemText(v_iItem,8,v_sTemp);		//LTE_Cell
					m_cList_Station.SetItemText(v_iItem,9,v_sLongitude);//经度
					m_cList_Station.SetItemText(v_iItem,10,v_sLatitude);//纬度

					v_iID++;											//计数：告警数量
				}
			}

			v_pRecordset->MoveNext();									//

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
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载站址信息【Tree】
//------------------------------------------------------------------------------------------------------			
void CQUA_Station_Out::My_LoadData_Tree(CString v_sRoom)
{
	CString		v_sSql,v_sTemp;											//临时变量
	CString		v_sBTS,v_sBTS_Name,v_sNodeB_Name,v_sSation,v_sCell,v_sSation_Cell;	//临时变量
	bool		v_bRoot=false;											//临时变量

	mStation_OutPut_BTS				v_mNodeb;							//Nodeb/BTS 【Tree记录：value=1，为BTS;value=2，为Nodeb;】
	mStation_OutPut_BTS::iterator	v_pNodeb;							//迭代器
	Station_OutPut_BTS_Struct		v_cNodeb;							//结构
	mString_String::iterator		v_pCell;							//迭代器

		//树复位
	m_cTree_Station.DeleteAllItems();									//树清除

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//小区共址查询 【查询小区站址为v_sRoom对应的基站信息，加入容器v_mNodeb】
		//【GSM小区】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select GSM_Cell.zCell,GSM_Cell.zBTS as zBTS_BTS,GSM_Cell.zStation as zStation_Cell, \
				  GSM_BTS.zBTS_NAME,GSM_BTS.zStation as zStation_BTS \
				  FROM GSM_Cell left join GSM_BTS on GSM_Cell.zBTS=GSM_BTS.zBTS \
				  where GSM_BTS.zType='室外' and GSM_Cell.zStation='"+v_sRoom+"' order by GSM_BTS.zStation,GSM_Cell.zCell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell

			v_pNodeb=v_mNodeb.find(v_sBTS);								//查找：BTS
			if (v_pNodeb==v_mNodeb.end())
			{
				//增加：BTS
				v_cNodeb.v_sName=v_sBTS_Name;							//中文名
				v_cNodeb.v_sNetwork="2";								//网络类型
				v_cNodeb.v_sStation=v_sSation;							//站址
				v_mNodeb.insert(pair<CString,Station_OutPut_BTS_Struct>(v_sBTS,v_cNodeb));	//容器增加
			}
			v_pNodeb=v_mNodeb.find(v_sBTS);								//查找：BTS
			//增加：Cell
			v_pNodeb->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sSation_Cell));	//容器增加

			v_pRecordset->MoveNext();									//下一条
		}

		//【TD-LTE】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select LTE_Cell.zCell,LTE_Cell.zNODEB as zBTS_BTS,LTE_Cell.zStation as zStation_Cell, \
				  LTE_NODEB.zNODEB_Name,LTE_NODEB.zStation as zStation_BTS \
				  FROM LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB \
				  where LTE_NODEB.zType='室外' and LTE_Cell.zStation='"+v_sRoom+"' order by LTE_NODEB.zStation,LTE_Cell.zCell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zNODEB
			v_sBTS_Name = v_cAdoConn.GetField_String("zNODEB_Name");	//zNODEB_Name
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell

			v_pNodeb=v_mNodeb.find(v_sBTS);								//查找：BTS
			if (v_pNodeb==v_mNodeb.end())
			{
				//增加：E_NodeB
				v_cNodeb.v_sName=v_sBTS_Name;							//中文名
				v_cNodeb.v_sNetwork="3/4";								//网络类型：3/4G
				v_cNodeb.v_sStation=v_sSation;							//站址
				v_mNodeb.insert(pair<CString,Station_OutPut_BTS_Struct>(v_sBTS,v_cNodeb));	//容器增加
			}
			v_pNodeb=v_mNodeb.find(v_sBTS);								//查找：BTS
			//增加：Cell
			v_pNodeb->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sSation_Cell));	//容器增加

			v_pRecordset->MoveNext();									//
		}

		//依据新增加的基站，绘制Tree信息 【依据v_mNodeb的Value为1】
		v_hBTS_Root=m_cTree_Station.InsertItem(v_sRoom,6,7,TVI_ROOT);	//树：增加根节点 [机房/资源点]		
		for (v_pNodeb=v_mNodeb.begin();v_pNodeb!=v_mNodeb.end();v_pNodeb++)		//遍历：容器
		{
			if(v_pNodeb->second.v_sNetwork=="2")						//BTS
			{
				//GSM：BSC获取
				v_hBTS_Node=m_cTree_Station.InsertItem(v_pNodeb->first+"_"+v_pNodeb->second.v_sName
					+"     【"+v_pNodeb->second.v_sStation+"】",4,5,v_hBTS_Root);	//树：增加子节点 [BTS]
				for (v_pCell=v_pNodeb->second.v_mCell.begin();v_pCell!=v_pNodeb->second.v_mCell.end();v_pCell++)	//遍历：容器
					m_cTree_Station.InsertItem(v_pCell->first+"     【"+v_pCell->second+"】",0,1,v_hBTS_Node);		//树：增加子节点 [Cell]
			}
			else														//Nodeb
			{
				//TD_LTE：Nodeb获取
				v_sTemp=v_pNodeb->first;								//NodeB
				if (v_pNodeb->first.Find("-EL")>=0)						//爱立信4G ？？？
					v_sTemp+="_"+v_pNodeb->second.v_sName;				//+中文名
				v_hBTS_Node=m_cTree_Station.InsertItem(v_sTemp+"     【"
					+v_pNodeb->second.v_sStation+"】",4,5,v_hBTS_Root);				//树：增加子节点 [NodeB]
				for (v_pCell=v_pNodeb->second.v_mCell.begin();v_pCell!=v_pNodeb->second.v_mCell.end();v_pCell++)	//遍历：容器
					m_cTree_Station.InsertItem(v_pCell->first+"     【"+v_pCell->second+"】",0,1,v_hBTS_Node);		//树：增加子节点 [Cell]
			}
		}

		//树展开
		v_hBTS_Node=m_cTree_Station.GetRootItem();
		while (v_hBTS_Node!=NULL)
		{
			m_cTree_Station.ExpandTree(v_hBTS_Node);					//树：展开
			v_hBTS_Node=m_cTree_Station.GetNextSiblingItem(v_hBTS_Node);//获得：邻居节点
		}

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	v_mNodeb.clear();													//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：明细导出
//------------------------------------------------------------------------------------------------------			
void CQUA_Station_Out::My_OutPut()
{
	CString		v_sSql,v_sTemp,v_sRegion,v_sRNC,v_sOnline;				//临时变量
	CString		v_sBTS,v_sCell,v_sBTS_Name,v_sNodeB_Name,v_sSation,v_sSation_Cell;

	//站址容器
	mStation_OutPut				v_mStation_OutPut;						//站址容器
	mStation_OutPut::iterator	v_pStation_OutPut;						//迭代器
	Station_OutPut_Struct		v_cStation_OutPut;						//结构
	mStation_OutPut_BTS::iterator	v_pmStation_OutPut_BTS;				//迭代器
	Station_OutPut_BTS_Struct	v_cStation_OutPut_BTS;					//结构

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//【GSM】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select GSM_Cell.zCell,GSM_Cell.zBTS as zBTS_BTS,GSM_Cell.zStation as zStation_Cell, \
				  GSM_BTS.zBTS_NAME,GSM_BTS.zStation as zStation_BTS,GSM_BTS.zRegion,GSM_Cell.zOnline as zOnline_Cell \
				  FROM GSM_Cell left join GSM_BTS on GSM_Cell.zBTS=GSM_BTS.zBTS \
				  where GSM_BTS.zType='室外' order by GSM_BTS.zStation,GSM_Cell.zCell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell
			v_sOnline = v_cAdoConn.GetField_String("zOnline_Cell");		//zOnline_Cell

			v_pStation_OutPut=v_mStation_OutPut.find(v_sSation_Cell);	//查找：站址
			if (v_pStation_OutPut==v_mStation_OutPut.end())
			{
				//增加：站址
				v_cStation_OutPut.v_sRegion=v_sRegion;					//区域
				v_cStation_OutPut.v_bNet_GSM=1;							//共址分类
				v_cStation_OutPut.v_bNet_TD=0;							//共址分类
				v_cStation_OutPut.v_bNet_LTE=0;							//共址分类
				v_mStation_OutPut.insert(pair<CString,Station_OutPut_Struct>(v_sSation_Cell,v_cStation_OutPut));	//容器增加
			}
			v_pStation_OutPut=v_mStation_OutPut.find(v_sSation_Cell);	//查找：站址
			v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mGSM.find(v_sBTS);	//查找：BTS
			if (v_pmStation_OutPut_BTS==v_pStation_OutPut->second.v_mGSM.end())
			{
				//增加：BTS
				v_cStation_OutPut_BTS.v_sName=v_sBTS_Name;				//中文名
				v_cStation_OutPut_BTS.v_sNetwork="2";					//网络类型
				v_pStation_OutPut->second.v_mGSM.insert(pair<CString,Station_OutPut_BTS_Struct>(v_sBTS,v_cStation_OutPut_BTS));	//容器增加
			}
			v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mGSM.find(v_sBTS);	//查找：BTS
			//增加：Cell
			v_pmStation_OutPut_BTS->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sOnline));	//容器增加

			v_pRecordset->MoveNext();									//下一条
		}

		//【TD-LTE】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select LTE_Cell.zCell,LTE_Cell.zNODEB as zBTS_BTS,LTE_Cell.zStation as zStation_Cell, \
					LTE_NODEB.zStation as zStation_BTS,LTE_NODEB.zRegion,LTE_NODEB.zRNC,LTE_Cell.zOnline as zOnline_Cell \
					FROM LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB \
					where LTE_NODEB.zType='室外' order by LTE_NODEB.zStation,LTE_Cell.zCell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zBTS
			v_sRNC = v_cAdoConn.GetField_String("zRNC");				//zRNC
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell
			v_sOnline = v_cAdoConn.GetField_String("zOnline_Cell");		//zOnline_Cell

			v_pStation_OutPut=v_mStation_OutPut.find(v_sSation_Cell);	//查找：站址
			if (v_pStation_OutPut==v_mStation_OutPut.end())
			{
				//增加：站址
				v_cStation_OutPut.v_sRegion=v_sRegion;					//区域
				v_cStation_OutPut.v_bNet_GSM=0;							//共址分类
				v_cStation_OutPut.v_bNet_TD=0;							//共址分类
				v_cStation_OutPut.v_bNet_LTE=0;							//共址分类
				v_mStation_OutPut.insert(pair<CString,Station_OutPut_Struct>(v_sSation_Cell,v_cStation_OutPut));	//容器增加
			}
			v_pStation_OutPut=v_mStation_OutPut.find(v_sSation_Cell);	//查找：站址
			if (v_sRNC.Find("dalian_")>=0)								//4G？？？
			{
				v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mLTE.find(v_sBTS);	//查找：BTS
				if (v_pmStation_OutPut_BTS==v_pStation_OutPut->second.v_mLTE.end())
				{
					//修改：共址分类
					v_pStation_OutPut->second.v_bNet_LTE=1;				//共址分类
					//增加：E_NodeB
					v_cStation_OutPut_BTS.v_sName=v_sBTS;				//中文名
					v_cStation_OutPut_BTS.v_sNetwork="4";				//网络类型：4G
					v_pStation_OutPut->second.v_mLTE.insert(pair<CString,Station_OutPut_BTS_Struct>(v_sBTS,v_cStation_OutPut_BTS));	//容器增加
				}
				v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mLTE.find(v_sBTS);	//查找：BTS
				//增加：Cell
				v_pmStation_OutPut_BTS->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sOnline));	//容器增加
			}
			else
			{
				v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mTD.find(v_sBTS);	//查找：BTS
				if (v_pmStation_OutPut_BTS==v_pStation_OutPut->second.v_mTD.end())
				{
					//修改：共址分类
					v_pStation_OutPut->second.v_bNet_TD=1;				//共址分类
					//增加：NodeB
					v_cStation_OutPut_BTS.v_sName=v_sBTS;				//中文名
					v_cStation_OutPut_BTS.v_sNetwork="3";				//网络类型：3G
					v_pStation_OutPut->second.v_mTD.insert(pair<CString,Station_OutPut_BTS_Struct>(v_sBTS,v_cStation_OutPut_BTS));	//容器增加
				}
				v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mTD.find(v_sBTS);	//查找：BTS
				//增加：Cell
				v_pmStation_OutPut_BTS->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sOnline));	//容器增加
			}

			v_pRecordset->MoveNext();									//
		}

		v_cAdoConn.ExitConnect();										//断开连接

		//生成导出文件
		MyOutPut_Station(&v_mStation_OutPut,"室外共址_明细",true);		//导出Excel
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	v_mStation_OutPut.clear();											//容器清除
}

//-------------------------------------------------------------------------------------------------
// 导出Excel处理：写入CSV文件；
// 参数：mStation_OutPut：容器指针
//		 v_sFile_Name：输出的文件名称
//		 v_bFile_Open：文件打开标志；[1：打开；0：不打开；]
//-------------------------------------------------------------------------------------------------
void CQUA_Station_Out::MyOutPut_Station(mStation_OutPut *v_mStation_OutPut,CString v_sFile_Name,bool v_bFile_Open) 
{
	int		v_iNum1=0;													//临时变量
	CString	v_sStr,v_sCSV,v_sPath_Data,v_sCSV_File,v_sStation;			//临时变量
	CString	v_sRegion,v_sBTS,v_sBTS_Name,v_sHead,v_sNetwork;			//临时变量
	CString	v_sLong,v_sLan;												//临时变量
	CFile	v_fWriter;													//实例化
	CFileFind	finder;

	mStation_OutPut::iterator		v_pStation_OutPut;					//迭代器
	mStation_OutPut_BTS::iterator	v_pmStation_OutPut_BTS;				//迭代器
	mString_String::iterator		v_pCell;							//迭代器

	//目录处理
	v_sPath_Data=theApp.m_sPath+"\\DATA";								//文件目录
	if (!finder.FindFile(v_sPath_Data))									//存在？
	{
		if (!::CreateDirectory(v_sPath_Data, NULL))						//不存在：创建目录
		{
			MessageBox("异常："+v_sPath_Data+" 目录创建失败。");		//失败
			return;
		}
	}

	try
	{
		//生成CSV文件：文件名称v_sCSV_File
		if (v_sFile_Name.IsEmpty())
			v_sCSV_File = theApp.m_sPath+"\\DATA\\Excel.csv";			//设置：CSV默认文件名称Excel.csv
		else
			v_sCSV_File = theApp.m_sPath+"\\DATA\\"+v_sFile_Name+".csv";//设置：CSV指定的文件名称
		if(v_fWriter.Open(v_sCSV_File, CFile::modeCreate | CFile::modeWrite))		//打开文件，如不存在则生成，如存在则清空文件；
		{
			//表头处理：取列表的宽及列名称，逐列写入CSV
			v_sCSV = "序号,所属机房/资源点,经度,纬度,网络制式-站点,BTS/NODEB/ENODEB,CELL,区域,共站分类,小区号,网络制式-小区号,状态";												//第二列、第三列、...、第n列
			v_sCSV +="\r\n";											//回车换行
			v_fWriter.Write(v_sCSV,v_sCSV.GetLength());					//写入字符串

			//表内容处理：取容器数据，写入CSV
			for (int vi=0;vi<m_cList_Station.GetItemCount();vi++)
			{
				v_sStation=m_cList_Station.GetItemText(vi,1);			//获得：站址
				v_sLong=m_cList_Station.GetItemText(vi,9);				//获得：经度
				v_sLan=m_cList_Station.GetItemText(vi,10);				//获得：纬度
				v_pStation_OutPut=v_mStation_OutPut->find(v_sStation);	//查找：站址
				if(v_pStation_OutPut==v_mStation_OutPut->end())			//没找到？？？
					continue;											//没找到：下一条

				v_iNum1++;												//站址计数器
					
				v_sHead.Format("%d",v_iNum1);							//序号
				v_sHead+=","+v_pStation_OutPut->first+",";				//站址
				v_sHead+=v_sLong+",";									//经度
				v_sHead+=v_sLan+",";									//纬度
				v_sRegion=v_pStation_OutPut->second.v_sRegion;			//区域
				v_sNetwork="";											//共址分类
				if(v_pStation_OutPut->second.v_bNet_GSM)
					v_sNetwork="2";										//共址分类
				if(v_pStation_OutPut->second.v_bNet_TD)
				{
					if(!v_sNetwork.IsEmpty())
						v_sNetwork+="&";								//共址分类
					v_sNetwork+="3";									//共址分类
				}
				if(v_pStation_OutPut->second.v_bNet_LTE)
				{
					if(!v_sNetwork.IsEmpty())
						v_sNetwork+="&";								//共址分类
					v_sNetwork+="4";									//共址分类
				}
				//GSM BTS
				if (v_pStation_OutPut->second.v_mGSM.size()>0)			//GSM BTS
				{
					v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mGSM.begin();		//头迭代器
					while(v_pmStation_OutPut_BTS!=v_pStation_OutPut->second.v_mGSM.end())	//尾迭代器
					{
						v_sBTS=v_pmStation_OutPut_BTS->first;			//BTS
						v_sBTS_Name=v_pmStation_OutPut_BTS->second.v_sName;		//BTS_Name

						v_pCell=v_pmStation_OutPut_BTS->second.v_mCell.begin();	//头指针
						while(v_pCell!=v_pmStation_OutPut_BTS->second.v_mCell.end())
						{
							//BTS信息
							v_sCSV =v_sHead+"GSM-"+v_pmStation_OutPut_BTS->second.v_sName+",";	//网络制式-站点
							v_sCSV +=v_pmStation_OutPut_BTS->first+",";	//BTS
							//Cell信息
							v_sCSV +=v_pCell->first+",";				//Cell
							v_sCSV +=v_sRegion+",";						//区域
							v_sCSV +=v_sNetwork+",";					//共址分类
							v_sCSV +=v_pCell->first.Right(1)+",";		//小区号
							v_sCSV +="GSM-"+v_sBTS_Name+v_pCell->first.Right(1)+",";//网络制式-小区号
							v_sCSV +=v_pCell->second;					//状态
							v_sCSV +="\r\n";							//回车换行
							v_fWriter.Write(v_sCSV,v_sCSV.GetLength());	//写入字符串

							v_pCell++;									//迭代器++
						}

						v_pmStation_OutPut_BTS++;						//迭代器++
					}
				}
				//TD NodeB
				if (v_pStation_OutPut->second.v_mTD.size()>0)			//TD NodeB
				{
					v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mTD.begin();			//头迭代器
					while(v_pmStation_OutPut_BTS!=v_pStation_OutPut->second.v_mTD.end())	//尾迭代器
					{
						v_sBTS=v_pmStation_OutPut_BTS->first;			//BTS

						v_pCell=v_pmStation_OutPut_BTS->second.v_mCell.begin();	//头指针
						while(v_pCell!=v_pmStation_OutPut_BTS->second.v_mCell.end())
						{
							//BTS信息
							v_sCSV =v_sHead+"TD-"+v_pmStation_OutPut_BTS->second.v_sName+",";	//网络制式-站点
							v_sCSV +=v_pmStation_OutPut_BTS->first+",";	//BTS
							//Cell信息
							v_sCSV +=v_pCell->first+",";				//Cell
							v_sCSV +=v_sRegion+",";						//区域
							v_sCSV +=v_sNetwork+",";					//共址分类
							v_sCSV +=v_pCell->first.Right(1)+",";		//小区号
							v_sCSV +="TD-"+v_pCell->first+",";			//网络制式-小区号
							v_sCSV +=v_pCell->second;					//状态
							v_sCSV +="\r\n";							//回车换行
							v_fWriter.Write(v_sCSV,v_sCSV.GetLength());	//写入字符串

							v_pCell++;									//迭代器++
						}

						v_pmStation_OutPut_BTS++;						//迭代器++
					}
				}
				//LTE E_NodeB
				if (v_pStation_OutPut->second.v_mLTE.size()>0)			//LTE E_NodeB
				{
					v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mLTE.begin();		//头迭代器
					while(v_pmStation_OutPut_BTS!=v_pStation_OutPut->second.v_mLTE.end())	//尾迭代器
					{
						v_sBTS=v_pmStation_OutPut_BTS->first;			//BTS

						v_pCell=v_pmStation_OutPut_BTS->second.v_mCell.begin();	//头指针
						while(v_pCell!=v_pmStation_OutPut_BTS->second.v_mCell.end())
						{
							//BTS信息
							v_sCSV =v_sHead+"LTE-"+v_pmStation_OutPut_BTS->second.v_sName+",";	//网络制式-站点
							v_sCSV +=v_pmStation_OutPut_BTS->first+",";	//BTS
							//Cell信息
							v_sCSV +=v_pCell->first+",";				//Cell
							v_sCSV +=v_sRegion+",";						//区域
							v_sCSV +=v_sNetwork+",";					//共址分类
							v_sCSV +=v_pCell->first.Right(1)+",";		//小区号
							v_sCSV +="LTE-"+v_pCell->first+",";			//网络制式-小区号
							v_sCSV +=v_pCell->second;					//状态
							v_sCSV +="\r\n";							//回车换行
							v_fWriter.Write(v_sCSV,v_sCSV.GetLength());	//写入字符串

							v_pCell++;									//迭代器++
						}

						v_pmStation_OutPut_BTS++;						//迭代器++
					}
				}
			}
			
			v_fWriter.Close();											//文件关闭

			//打开文件
			if (v_bFile_Open)
				ShellExecute(NULL,"open", v_sCSV_File, NULL, NULL, SW_SHOWNORMAL);		//打开文件：Excel.csv
		}
		else
			MessageBox("请关闭已经打开的导出文件，再重新导出！！！");	//提示信息
	}	
	catch(_com_error e)
	{
		CString v_sError = (char *)e.Description();						//获得：出错信息
		MessageBox("异常"+v_sError);									//提示信息
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取小区信息【Nodeb】
//------------------------------------------------------------------------------------------------------			
void CQUA_Station_Out::My_LoadData_Tree_Node_Cell(CString v_sNodeb,HTREEITEM v_hBTS_Node)
{
	CString		v_sSql,v_sTemp;											//临时变量
	CString		v_sCell,v_sStation;

	_variant_t	Var;

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//GSM：BSC获取
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT zCell,zStation FROM LTE_Cell where zNODEB='"+v_sNodeb+"' order by zCell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation

			m_cTree_Station.InsertItem(v_sCell+"     【"+v_sStation+"】",0,1,v_hBTS_Node);	//树：增加子节点 [Nodeb]

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
//	END
//------------------------------------------------------------------------------------------------------
