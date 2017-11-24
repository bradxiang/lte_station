// CON_Station.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "CON_Station.h"
#include "CON_Station_Input.h"
#include "CON_Station_Input_R.h"
#include "CON_Station_Input_L.h"
#include "CON_Station_Region.h"


// CCON_Station

IMPLEMENT_DYNCREATE(CCON_Station, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CCON_Station::CCON_Station()
	: CFormView(CCON_Station::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CCON_Station::~CCON_Station()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CCON_Station::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_Station);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_ELEMENT, m_cElement);
	DDX_Control(pDX, IDC_ELEMENT2, m_cRemote);
	DDX_Control(pDX, IDC_ELEMENT3, m_cStation);
	DDX_Control(pDX, IDC_BUTTON2, m_cCell);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_Station, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CCON_Station::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_ELEMENT, &CCON_Station::OnBnClickedElement)
	ON_BN_CLICKED(IDC_EXCEL2, &CCON_Station::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CCON_Station::OnNMClickList3)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CCON_Station::OnNMRClickList3)
	ON_COMMAND(ID_CON_STATION_DEL, &CCON_Station::OnConStationDel)
	ON_BN_CLICKED(IDC_ELEMENT2, &CCON_Station::OnBnClickedElement2)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CCON_Station::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_ELEMENT3, &CCON_Station::OnBnClickedElement3)
	ON_BN_CLICKED(IDC_BUTTON2, &CCON_Station::OnBnClickedButton2)
END_MESSAGE_MAP()


// CCON_Station 诊断

#ifdef _DEBUG
void CCON_Station::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCON_Station::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCON_Station 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="站址管理";											//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗 [站址]
	::SendMessage(m_cList_Station.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Station.InsertColumn(0,"序号",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(1,"机房/资源点名称",LVCFMT_LEFT,300);	//
	m_cList_Station.InsertColumn(2,"类型",LVCFMT_LEFT,120);				//
	m_cList_Station.InsertColumn(3,"区域",LVCFMT_LEFT,80);				//
	m_cList_Station.InsertColumn(4,"地址",LVCFMT_LEFT,300);				//
	m_cList_Station.InsertColumn(5,"经度",LVCFMT_RIGHT,100);			//
	m_cList_Station.InsertColumn(6,"纬度",LVCFMT_RIGHT,100);			//
	m_cList_Station.InsertColumn(7,"状态",LVCFMT_CENTER,80);			//
	m_cList_Station.InsertColumn(8,"更新日期",LVCFMT_CENTER,100);		//
	m_cList_Station.SetExtendedStyle(m_cList_Station.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//信息初始化
	m_cKey_Select.AddString("机房/资源点");
	m_cKey_Select.AddString("类型");
	m_cKey_Select.AddString("区域");
	m_cKey_Select.AddString("地址");
	m_cKey_Select.SetCurSel(0);											//默认：选择第一个

	v_iList_Item=-1;													//条目选择无效
	v_iSum=0;															//数据总数
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CCON_Station::PreTranslateMessage(MSG* pMsg)
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
void CCON_Station::OnDestroy()
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
void CCON_Station::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd	*pCellList;													//临时变量
	int		v_iY;														//临时变量

	pCellList = GetDlgItem(IDC_LIST3);
	v_iY = 75;															//赋值：Y轴
	if(pCellList)
		pCellList->MoveWindow(14,v_iY,cx-24,cy-v_iY-10);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：站址导出
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Station);										//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：机房导入
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnBnClickedElement()
{
	// TODO: 在此添加控件通知处理程序代码
	CCON_Station_Input	v_cDlg;											//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：资源点导入
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnBnClickedElement2()
{
	// TODO: 在此添加控件通知处理程序代码
	CCON_Station_Input_R	v_cDlg;										//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：站点导入
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnBnClickedElement3()
{
	// TODO: 在此添加控件通知处理程序代码
	CCON_Station_Input_L	v_cDlg;										//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序 [站址]
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList_Station.My_Sort(pNMHDR);										//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表点击响应
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==0)
		{
			if(v_iList_Item>=0)											//有效条目序号？？？
				m_cList_Station.SetItemBkColor(v_iList_Item,RGB(255,255,255));		//恢复背景色
			v_iList_Item=pNMListView->iItem;							//获得：选中条目序号
			m_cList_Station.SetItemBkColor(v_iList_Item,RGB(255,0,0));	//设置背景色：红色
		}
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表右击响应
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu	aMenu;														//菜单
	CPoint	point;														//位置点

	GetCursorPos(&point);											//获得：鼠标位置
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;				//获得：点击的列表窗位置
	int v_iSelect = pNMListView->iItem;								//获取：点击列表窗的Item
	if( v_iSelect == -1 )											//有效Item？？？
		return;														//无效：返回
	if(v_iList_Item>=0)												//有效条目序号？？？
		m_cList_Station.SetItemBkColor(v_iList_Item,RGB(255,255,255));		//恢复背景色
	v_iList_Item=v_iSelect;											//获得：选中条目序号
	m_cList_Station.SetItemBkColor(v_iList_Item,RGB(255,0,0));		//设置背景色：红色

	aMenu.LoadMenu(IDR_STATION_DEL);								//装载：IDR_GSM_BTS菜单
	CMenu *pPopup =aMenu.GetSubMenu(0);								//生成浮动菜单
	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);		//显示：依据鼠标位置[point]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：删除站址
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnConStationDel()
{
	// TODO: 在此添加命令处理程序代码
	int		v_iItem,vi;													//临时变量
	CString	v_sCell,v_sTemp,v_sSql;										//临时变量

	vi = m_cList_Station.GetSelectedCount();							//选中条目数量
	v_sTemp.Format("%d",vi);											//字符串
	if(MessageBox("删除选中的 "+v_sTemp+" 个站址， 删除确认 ? ","数据删除确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)	//删除确认？？？
	{
		//对所有选中的Item进行操作
		while ((v_iItem = m_cList_Station.GetNextItem(-1, LVNI_SELECTED)) != -1)//条目选中？？？
		{
			v_sCell=m_cList_Station.GetItemText(v_iItem,1);				//获得：v_sCell [数据表唯一标识，用于修改数据表]

			//删除：Cell
			v_sSql = "delete from CON_Station where zStation='"+v_sCell+"'";	//SQL删除
			My_ExecuteSQL(v_sSql);										//写表

			//删除：列表信息
			m_cList_Station.DeleteItem(v_iItem);						//删除：选中的v_iItem

			//保存：Log
			COleDateTime v_tTime=COleDateTime::GetCurrentTime();
			CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");
			CString v_sSql="Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"+v_sCurrentTime+"','删除站址','站址名称："
				+v_sCell+"，操作员："+theApp.a_sUser_Name+"')";
			My_ExecuteSQL(v_sSql);										//数据库执行
		}
		v_iSum-=vi;														//剩余数量
		v_sTemp.Format("%d",v_iSum);									//字符串
		m_cSum.SetWindowText(v_sTemp);									//显示：站址总数
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------
//	函数功能：区域同步
//------------------------------------------------------------------------------------------------------
void CCON_Station::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CCON_Station_Region	v_cDlg;											//
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CCON_Station::My_Thread_Inoput(LPVOID lparam)
{
	CCON_Station *  lp_this = NULL ;
	lp_this = (CCON_Station *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel_NodeB.EnableWindow(false);						//NodeB导出：无效
	lp_this->m_cElement.EnableWindow(false);							//站址导入：无效
	lp_this->m_cRemote.EnableWindow(false);								//资源点导入：无效
	lp_this->m_cStation.EnableWindow(false);							//站点导入：无效
	lp_this->m_cCell.EnableWindow(false);								//区域核查：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel_NodeB.EnableWindow(true);							//NodeB导出：有效
	lp_this->m_cElement.EnableWindow(true);								//站址导入：有效
	lp_this->m_cRemote.EnableWindow(true);								//资源点导入：有效
	lp_this->m_cStation.EnableWindow(true);								//站点导入：有效
	lp_this->m_cCell.EnableWindow(true);								//区域核查：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CCON_Station::My_Input_Main()
{
	m_cList_Station.DeleteAllItems();									//清除Items
	v_iList_Item=-1;													//条目选择无效

	switch(v_iWorking)
	{
	case 1:
		My_Query_Station();												//站址查询
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询站址信息
//------------------------------------------------------------------------------------------------------
void CCON_Station::My_Query_Station()
{
	int				v_iID=0;											//临时变量
	CString			v_sSql,v_sTemp;										//临时变量
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//查询日期准备
	m_cKey.GetWindowText(v_sLog_Key);									//获得：查询KEY
	v_sLog_Key.Replace("[","[[]");										//限制处理
	v_sLog_Key.Replace("'","");											//限制处理
	v_sLog_Key.Trim();													//去掉空格

	v_sSql += "SELECT * FROM CON_Station"; 

	if (!v_sLog_Key.IsEmpty())											//查询KEY处理
	{
		switch (m_cKey_Select.GetCurSel())								//选择？？？
		{
		case 0:															//站址
			v_sSql+=" where zStation like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//类型
			v_sSql+=" where zType like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//区域
			v_sSql+=" where zRegion like '%"+v_sLog_Key+"%'";
			break;
		case 3:															//地址
			v_sSql+=" where zAddress like '%"+v_sLog_Key+"%'";
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
void CCON_Station::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem;											//临时变量
	CString			v_sTemp;											//临时变量
	COleDateTime	v_dDate_Start;										//查询开始日期

	_variant_t	Var;
	CString			v_sStation,v_sType,v_sRegion,v_sAddress,v_sLongitude;	//临时变量
	CString			v_sLatitude,v_sOnline,v_sDate;						//临时变量
	double			v_dValue;											//临时变量

	v_iSum=0;															//初始值

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion
			v_dValue = v_cAdoConn.GetField_Double("zLongitude");		//zLongitude
			v_sLongitude.Format("%10.6f",v_dValue);
			v_dValue = v_cAdoConn.GetField_Double("zLatitude");			//zLatitude
			v_sLatitude.Format("%10.6f",v_dValue);
			v_sAddress = v_cAdoConn.GetField_String("zAddress");		//zAddress
			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline
			Var=v_pRecordset->GetCollect((_variant_t)"zDate");			//zDate
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sDate = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sDate="";

			//显示
			v_sTemp.Format(" %04d",v_iSum+1);
			v_iItem=m_cList_Station.InsertItem(0xffff,v_sTemp);			//加入列表窗
			m_cList_Station.SetItemText(v_iItem,1,v_sStation);			//机房/资源点名称
			m_cList_Station.SetItemText(v_iItem,2,v_sType);				//类型
			m_cList_Station.SetItemText(v_iItem,3,v_sRegion);			//区域
			m_cList_Station.SetItemText(v_iItem,4,v_sAddress);			//地址
			m_cList_Station.SetItemText(v_iItem,5,v_sLongitude);		//经度
			m_cList_Station.SetItemText(v_iItem,6,v_sLatitude);			//纬度
			m_cList_Station.SetItemText(v_iItem,7,v_sOnline);			//状态
			m_cList_Station.SetItemText(v_iItem,8,v_sDate);				//更新日期

			v_pRecordset->MoveNext();									//
			v_iSum++;													//计数：告警数量

			if (v_iSum%100==0)											//显示：100的整数
			{
				v_sTemp.Format("%d",v_iSum);							//
				m_cSum.SetWindowText(v_sTemp);							//显示：站址总数
			}
		}
		v_sTemp.Format("%d",v_iSum);									//
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
//	函数功能：区域核查
//------------------------------------------------------------------------------------------------------			
void CCON_Station::My_Query_Region()
{
	CString		v_sSql,v_sTemp,v_sRegion,v_sRNC,v_sRegion_Station;		//临时变量
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
				  GSM_BTS.zBTS_NAME,GSM_BTS.zStation as zStation_BTS,GSM_Cell.zRegion,CON_Station.zRegion as zRegion_Station \
				  FROM GSM_Cell left join GSM_BTS on GSM_Cell.zBTS=GSM_BTS.zBTS left join CON_Station on GSM_Cell.zStation=CON_Station.zStation"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zBTS
			v_sBTS_Name = v_cAdoConn.GetField_String("zBTS_NAME");		//zBTS_NAME
			v_sRegion_Station = v_cAdoConn.GetField_String("zRegion_Station");	//zRegion_Station
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell
			v_sRegion = v_cAdoConn.GetField_String("zRegion");		//zRegion

			v_pStation_OutPut=v_mStation_OutPut.find(v_sSation_Cell);	//查找：站址
			if (v_pStation_OutPut==v_mStation_OutPut.end())
			{
				//增加：站址
				v_cStation_OutPut.v_sRegion=v_sRegion_Station;			//区域
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
			v_pmStation_OutPut_BTS->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sRegion));	//容器增加

			v_pRecordset->MoveNext();									//下一条
		}

		//【TD-LTE】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select LTE_Cell.zCell,LTE_Cell.zNODEB as zBTS_BTS,LTE_Cell.zStation as zStation_Cell, \
				  LTE_NODEB.zStation as zStation_BTS,LTE_NODEB.zRNC,LTE_Cell.zRegion,CON_Station.zRegion as zRegion_Station \
				  FROM LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB left join CON_Station on LTE_Cell.zStation=CON_Station.zStation"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sCell = v_cAdoConn.GetField_String("zCell");				//zCell
			v_sBTS = v_cAdoConn.GetField_String("zBTS_BTS");			//zBTS
			v_sRNC = v_cAdoConn.GetField_String("zRNC");				//zRNC
			v_sRegion_Station = v_cAdoConn.GetField_String("zRegion_Station");	//zRegion_Station
			v_sSation = v_cAdoConn.GetField_String("zStation_BTS");		//zStation_BTS
			v_sSation_Cell = v_cAdoConn.GetField_String("zStation_Cell");	//zStation_Cell
			v_sRegion = v_cAdoConn.GetField_String("zRegion");		//zRegion

			v_pStation_OutPut=v_mStation_OutPut.find(v_sSation_Cell);	//查找：站址
			if (v_pStation_OutPut==v_mStation_OutPut.end())
			{
				//增加：站址
				v_cStation_OutPut.v_sRegion=v_sRegion_Station;			//区域
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
				v_pmStation_OutPut_BTS->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sRegion));	//容器增加
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
				v_pmStation_OutPut_BTS->second.v_mCell.insert(pair<CString,CString>(v_sCell,v_sRegion));	//容器增加
			}

			v_pRecordset->MoveNext();									//
		}

		v_cAdoConn.ExitConnect();										//断开连接

		//生成导出文件
		My_Show_Region(&v_mStation_OutPut);								//显示区域核查结果
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
// 导出Excel处理：加入列表；
// 参         数：mStation_OutPut：容器指针
//-------------------------------------------------------------------------------------------------
void CCON_Station::My_Show_Region(mStation_OutPut *v_mStation_OutPut) 
{
	CString		v_sTemp,v_sNetwork,v_sBTS,v_sBTS_Name;
	int			v_iItem,v_iID=0;

	mStation_OutPut::iterator		v_pStation_OutPut;					//迭代器
	mStation_OutPut_BTS::iterator	v_pmStation_OutPut_BTS;				//迭代器
	mString_String::iterator		v_pCell;							//迭代器

	for(v_pStation_OutPut=v_mStation_OutPut->begin();v_pStation_OutPut!=v_mStation_OutPut->end();v_pStation_OutPut++)
	{
		if (v_pIterator_Frame->second.v_iThread_Exit==2)				//强制退出？？？
			break;														//退出

		if ((v_pStation_OutPut->second.v_mGSM.size()+v_pStation_OutPut->second.v_mTD.size()+v_pStation_OutPut->second.v_mLTE.size())==0)
			continue;													//下一站址

		if(v_pStation_OutPut->second.v_bNet_GSM)
			v_sNetwork="2";												//共址分类
		if(v_pStation_OutPut->second.v_bNet_TD)
		{
			if(!v_sNetwork.IsEmpty())
				v_sNetwork+="_";										//共址分类
			v_sNetwork+="3";											//共址分类
		}
		if(v_pStation_OutPut->second.v_bNet_LTE)
		{
			if(!v_sNetwork.IsEmpty())
				v_sNetwork+="_";										//共址分类
			v_sNetwork+="4";											//共址分类
		}
		//GSM BTS
		if (v_pStation_OutPut->second.v_mGSM.size()>0)					//GSM BTS
		{
			v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mGSM.begin();		//头迭代器
			while(v_pmStation_OutPut_BTS!=v_pStation_OutPut->second.v_mGSM.end())	//尾迭代器
			{
				v_sBTS=v_pmStation_OutPut_BTS->first;			//BTS
				v_sBTS_Name=v_pmStation_OutPut_BTS->second.v_sName;		//BTS_Name

				v_pCell=v_pmStation_OutPut_BTS->second.v_mCell.begin();	//头指针
				while(v_pCell!=v_pmStation_OutPut_BTS->second.v_mCell.end())
				{
					//显示
					v_sTemp.Format(" %04d",v_iID+1);
					v_iItem=m_cList_Station.InsertItem(0xffffff,v_sTemp);			//加入列表窗
					m_cList_Station.SetItemText(v_iItem,1,v_pStation_OutPut->first);//机房/资源点名称
					m_cList_Station.SetItemText(v_iItem,2,v_sNetwork);				//类型
					m_cList_Station.SetItemText(v_iItem,3,v_pStation_OutPut->second.v_sRegion);		//区域
					m_cList_Station.SetItemText(v_iItem,4,v_sBTS);					//BTS
					m_cList_Station.SetItemText(v_iItem,5,v_pCell->first);			//Cell
					m_cList_Station.SetItemText(v_iItem,6,v_pCell->second);			//区域
					if (!v_pCell->second.IsEmpty() && v_pStation_OutPut->second.v_sRegion==v_pCell->second)		//区域不同？？？
						m_cList_Station.SetItemText(v_iItem,7,"★");				//区域核查

					v_pCell++;											//迭代器++
				}

				v_pmStation_OutPut_BTS++;								//迭代器++
			}
		}
		//TD NodeB
		if (v_pStation_OutPut->second.v_mTD.size()>0)					//TD NodeB
		{
			v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mTD.begin();			//头迭代器
			while(v_pmStation_OutPut_BTS!=v_pStation_OutPut->second.v_mTD.end())	//尾迭代器
			{
				v_sBTS=v_pmStation_OutPut_BTS->first;					//BTS

				v_pCell=v_pmStation_OutPut_BTS->second.v_mCell.begin();	//头指针
				while(v_pCell!=v_pmStation_OutPut_BTS->second.v_mCell.end())
				{
					//显示
					v_sTemp.Format(" %04d",v_iID+1);
					v_iItem=m_cList_Station.InsertItem(0xffffff,v_sTemp);			//加入列表窗
					m_cList_Station.SetItemText(v_iItem,1,v_pStation_OutPut->first);//机房/资源点名称
					m_cList_Station.SetItemText(v_iItem,2,v_sNetwork);				//类型
					m_cList_Station.SetItemText(v_iItem,3,v_pStation_OutPut->second.v_sRegion);		//区域
					m_cList_Station.SetItemText(v_iItem,4,v_sBTS);					//BTS
					m_cList_Station.SetItemText(v_iItem,5,v_pCell->first);			//Cell
					m_cList_Station.SetItemText(v_iItem,6,v_pCell->second);			//区域
					if (!v_pCell->second.IsEmpty() && v_pStation_OutPut->second.v_sRegion==v_pCell->second)		//区域不同？？？
						m_cList_Station.SetItemText(v_iItem,7,"★");				//区域核查

					v_pCell++;											//迭代器++
				}

				v_pmStation_OutPut_BTS++;								//迭代器++
			}
		}
		//LTE E_NodeB
		if (v_pStation_OutPut->second.v_mLTE.size()>0)					//LTE E_NodeB
		{
			v_pmStation_OutPut_BTS=v_pStation_OutPut->second.v_mLTE.begin();		//头迭代器
			while(v_pmStation_OutPut_BTS!=v_pStation_OutPut->second.v_mLTE.end())	//尾迭代器
			{
				v_sBTS=v_pmStation_OutPut_BTS->first;					//BTS

				v_pCell=v_pmStation_OutPut_BTS->second.v_mCell.begin();	//头指针
				while(v_pCell!=v_pmStation_OutPut_BTS->second.v_mCell.end())
				{
					//显示
					v_sTemp.Format(" %04d",v_iID+1);
					v_iItem=m_cList_Station.InsertItem(0xffffff,v_sTemp);			//加入列表窗
					m_cList_Station.SetItemText(v_iItem,1,v_pStation_OutPut->first);//机房/资源点名称
					m_cList_Station.SetItemText(v_iItem,2,v_sNetwork);				//类型
					m_cList_Station.SetItemText(v_iItem,3,v_pStation_OutPut->second.v_sRegion);		//区域
					m_cList_Station.SetItemText(v_iItem,4,v_sBTS);					//BTS
					m_cList_Station.SetItemText(v_iItem,5,v_pCell->first);			//Cell
					m_cList_Station.SetItemText(v_iItem,6,v_pCell->second);			//区域
					if (!v_pCell->second.IsEmpty() && v_pStation_OutPut->second.v_sRegion==v_pCell->second)		//区域不同？？？
						m_cList_Station.SetItemText(v_iItem,7,"★");				//区域核查

					v_pCell++;											//迭代器++
				}
				v_pmStation_OutPut_BTS++;								//迭代器++
			}
		}
		v_iID++;														//计数器
		if (v_iID%100==0)												//显示：100的整数
		{
			v_sTemp.Format("%d",v_iID);									//
			m_cSum.SetWindowText(v_sTemp);								//显示：站址总数
		}
	}
	v_sTemp.Format("%d",v_iID);											//
	m_cSum.SetWindowText(v_sTemp);										//显示：站址总数
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CCON_Station::My_ExecuteSQL(CString v_sSql)
{
	// TODO: Add your control notification handler code here
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);

		v_cAdoConn.ExecuteSQL((_bstr_t)v_sSql);							//执行数据库操作

		v_cAdoConn.ExitConnect();										//断开连接
		return true;
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//关闭数据库

		CString v_sError = (char *)e.Description();						//获得：出错信息
		MessageBox(v_sError);											//失败
		return false;
	}
}



//	距离计算	
// 	mStation_Long		v_mStation_Long;								//临时变量
// 	Station_Long_Struct	v_cStation_Long;								//结构
// 	CString				v_sLongitude,v_sLatitude,v_sStation;
// 	int					v_Sum=0;
// 
// 	CEarthPoint        pt1, pt2;
// 	double            dDistance;
// 
// 	//获取站址信息
// 	for (int vi=0;vi<m_cList_Station.GetItemCount();vi++)				//遍历
// 	{
// 		v_sStation=m_cList_Station.GetItemText(vi,1);					//站址
// 		v_sLongitude=m_cList_Station.GetItemText(vi,5);					//经度
// 		v_sLatitude=m_cList_Station.GetItemText(vi,6);					//纬度
// 		v_cStation_Long.v_dLongitude=atof(v_sLongitude);				//经度
// 		v_cStation_Long.v_dLatitude=atof(v_sLatitude);					//纬度
// 		v_mStation_Long.insert(pair<CString,Station_Long_Struct>(v_sStation,v_cStation_Long));	//容器增加
// 	}
// 
// 	//距离计算【循环遍历，每两个站址的距离】
// 	mStation_Long::iterator		v_pStation;								//迭代器
// 	while(v_mStation_Long.size()>0)
// 	{
// 		v_pStation=v_mStation_Long.begin();								//头指针
// 		pt1.SetPos(v_pStation->second.v_dLongitude,v_pStation->second.v_dLatitude);		//起点
// 		v_pStation++;													//迭代器++
// 		while(v_pStation!=v_mStation_Long.end())						//结束？？？
// 		{
// 			pt2.SetPos(v_pStation->second.v_dLongitude,v_pStation->second.v_dLatitude);	//终点
// 			dDistance = CEarthPoint::Distance(pt1, pt2);
// 
// 			if (dDistance<0.12)
// 			{
// 				v_Sum++;
// 			}
// 
// 			v_pStation++;												//迭代器++
// 		}
// 		v_mStation_Long.erase(v_mStation_Long.begin());					//删除头指针
// 	}
// 
// 	v_mStation_Long.clear();											//容器清除

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
