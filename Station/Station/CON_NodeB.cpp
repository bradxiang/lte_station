// CON_NodeB.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "CON_NodeB.h"
#include "CON_NodeB_Input.h"


// CCON_NodeB

IMPLEMENT_DYNCREATE(CCON_NodeB, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CCON_NodeB::CCON_NodeB()
	: CFormView(CCON_NodeB::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CCON_NodeB::~CCON_NodeB()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CCON_NodeB::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_ELEMENT, m_cElement);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_NodeB, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CCON_NodeB::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_ELEMENT, &CCON_NodeB::OnBnClickedElement)
	ON_BN_CLICKED(IDC_EXCEL2, &CCON_NodeB::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CCON_NodeB::OnNMClickList3)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CCON_NodeB::OnNMRClickList3)
	ON_COMMAND(ID_CON_TD_NODEB_DEL, &CCON_NodeB::OnConTdNodebDel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CCON_NodeB::OnLvnColumnclickList3)
END_MESSAGE_MAP()


// CCON_NodeB 诊断

#ifdef _DEBUG
void CCON_NodeB::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCON_NodeB::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCON_NodeB 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CCON_NodeB::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="NodeB 配置";											//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗 [站址]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"RNC名称",LVCFMT_LEFT,100);					//
	m_cList.InsertColumn(2,"NodeB",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(3,"中文名",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(4,"硬件",LVCFMT_LEFT,200);						//
	m_cList.InsertColumn(5,"IP",LVCFMT_LEFT,100);						//
	m_cList.InsertColumn(6,"软件版本",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(7,"区域",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(8,"所属机房/资源点",LVCFMT_LEFT,260);			//
	m_cList.InsertColumn(9,"覆盖类型",LVCFMT_LEFT,70);					//
	m_cList.InsertColumn(10,"状态",LVCFMT_LEFT,60);						//
	m_cList.InsertColumn(11,"更新日期",LVCFMT_CENTER,120);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//信息初始化
	m_cKey_Select.AddString("NodeB");
	m_cKey_Select.AddString("中文名");
	m_cKey_Select.AddString("RNC");
	m_cKey_Select.AddString("机房/资源点");
	m_cKey_Select.SetCurSel(0);											//默认：选择第一个

	v_iList_Item=-1;													//条目选择无效
	v_iSum=0;															//数据总数
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CCON_NodeB::PreTranslateMessage(MSG* pMsg)
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
void CCON_NodeB::OnDestroy()
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
void CCON_NodeB::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd	*pCellList;													//临时变量
	int		v_iY;														//临时变量

	pCellList = GetDlgItem(IDC_LIST3);
	v_iY = 50;															//赋值：Y轴
	if(pCellList)
		pCellList->MoveWindow(14,v_iY,cx-24,cy-v_iY-10);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：站址导出
//------------------------------------------------------------------------------------------------------
void CCON_NodeB::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：站址导入
//------------------------------------------------------------------------------------------------------
void CCON_NodeB::OnBnClickedElement()
{
	// TODO: 在此添加控件通知处理程序代码
	CCON_NodeB_Input	v_cDlg;											//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序 [站址]
//------------------------------------------------------------------------------------------------------
void CCON_NodeB::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表点击响应
//------------------------------------------------------------------------------------------------------
void CCON_NodeB::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
//	函数功能：列表右击响应
//------------------------------------------------------------------------------------------------------
void CCON_NodeB::OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
		m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));		//恢复背景色
	v_iList_Item=v_iSelect;											//获得：选中条目序号
	m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));				//设置背景色：红色

	aMenu.LoadMenu(IDR_TD_NODEB_DEL);								//装载：IDR_GSM_BTS菜单
	CMenu *pPopup =aMenu.GetSubMenu(0);								//生成浮动菜单
	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);		//显示：依据鼠标位置[point]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：删除基站
//------------------------------------------------------------------------------------------------------
void CCON_NodeB::OnConTdNodebDel()
{
	// TODO: 在此添加命令处理程序代码
	int		v_iItem,vi;													//临时变量
	CString	v_sCell,v_sTemp,v_sSql;										//临时变量

	vi = m_cList.GetSelectedCount();									//选中条目数量
	v_sTemp.Format("%d",vi);											//字符串
	if(MessageBox("删除选中的 "+v_sTemp+" 个 TD NodeB， 删除确认 ? ","数据删除确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)	//删除确认？？？
	{
		//对所有选中的Item进行操作
		while ((v_iItem = m_cList.GetNextItem(-1, LVNI_SELECTED)) != -1)//条目选中？？？
		{
			v_sCell=m_cList.GetItemText(v_iItem,2);						//获得：v_sCell [数据表唯一标识，用于修改数据表]

			//删除：Cell
			v_sSql = "delete from LTE_NODEB where zNODEB='"+v_sCell+"'";//SQL删除
			My_ExecuteSQL(v_sSql);										//写表

			//删除：列表信息
			m_cList.DeleteItem(v_iItem);								//删除：选中的v_iItem

			//保存：Log
			COleDateTime v_tTime=COleDateTime::GetCurrentTime();
			CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");
			CString v_sSql="Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"+v_sCurrentTime+"','删除NodeB','NodeB名称："
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
void CCON_NodeB::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CCON_NodeB::My_Thread_Inoput(LPVOID lparam)
{
	CCON_NodeB *  lp_this = NULL ;
	lp_this = (CCON_NodeB *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel_NodeB.EnableWindow(false);						//NodeB导出：无效
	lp_this->m_cElement.EnableWindow(false);							//站址导入：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel_NodeB.EnableWindow(true);							//NodeB导出：有效
	lp_this->m_cElement.EnableWindow(true);								//站址导入：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CCON_NodeB::My_Input_Main()
{
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
void CCON_NodeB::My_Query_Station()
{
	int				v_iID=0;											//KPI数量
	CString			v_sSql,v_sTemp;										//临时变量
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT LTE_NODEB.* FROM LTE_NODEB where zRNC like '%DAL-%'";

	//查询日期准备
	m_cKey.GetWindowText(v_sLog_Key);									//获得：查询KEY
	v_sLog_Key.Trim();													//去掉空格

	if (!v_sLog_Key.IsEmpty())											//查询KEY处理
	{
		switch (m_cKey_Select.GetCurSel())								//选择？？？
		{
		case 0:															//NodeB
			v_sSql+=" and zNODEB like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//中文名
			v_sSql+=" and zNODEB_Name like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//RNC
			v_sSql+=" and zRNC like '%"+v_sLog_Key+"%'";
			break;
		case 3:															//所属机房/资源点
			v_sSql+=" and zStation like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	v_sSql+=" order by zNODEB";

	My_LoadData_Station_Data(v_sSql);									//获取：站址信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载站址信息
//------------------------------------------------------------------------------------------------------			
void CCON_NodeB::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem,v_iID=0;									//KPI数量
	CString			v_sTemp;											//临时变量
	COleDateTime	v_dDate_Start;										//查询开始日期

	_variant_t	Var;
	CString			v_sStation,v_sRNC_Name,v_sNodeB,v_sNodeB_Name,v_sHardware,v_sIP,v_sVer;		//临时变量
	CString			v_sOnline,v_sDate,v_sArea,v_sType;					//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList.DeleteAllItems();										//清除Items
		v_iList_Item=-1;												//条目选择无效

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sRNC_Name = v_cAdoConn.GetField_String("zRNC");			//zRNC
			v_sNodeB = v_cAdoConn.GetField_String("zNODEB");			//zNODEB
			v_sNodeB_Name = v_cAdoConn.GetField_String("zNODEB_Name");	//zNODEB_Name
			v_sHardware = v_cAdoConn.GetField_String("zHardware");		//zHardware
			v_sIP = v_cAdoConn.GetField_String("zNODEB_Ip");			//zNODEB_Ip
			v_sVer = v_cAdoConn.GetField_String("zNODEB_Version");		//zNODEB_Version
			v_sArea = v_cAdoConn.GetField_String("zRegion");			//zRegion
			Var=v_pRecordset->GetCollect((_variant_t)"zDate");			//更新日期
			if (Var.vt != VT_NULL)
			{
				v_dDate_Start=Var.date;
				v_sDate = v_dDate_Start.Format(_T("%Y-%m-%d"));
			}
			else
				v_sDate="";
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sType = v_cAdoConn.GetField_String("zType");				//zType
			v_sOnline = v_cAdoConn.GetField_String("zOnline");			//zOnline

			//显示
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,v_sRNC_Name);					//网元名称
			m_cList.SetItemText(v_iItem,2,v_sNodeB);					//NodeB
			m_cList.SetItemText(v_iItem,3,v_sNodeB_Name);				//中文名
			m_cList.SetItemText(v_iItem,4,v_sHardware);					//硬件
			m_cList.SetItemText(v_iItem,5,v_sIP);						//IP
			m_cList.SetItemText(v_iItem,6,v_sVer);						//软件版本
			m_cList.SetItemText(v_iItem,7,v_sArea);						//区域
			m_cList.SetItemText(v_iItem,8,v_sStation);					//所属机房/资源点
			m_cList.SetItemText(v_iItem,9,v_sType);						//覆盖类型
			m_cList.SetItemText(v_iItem,10,v_sOnline);					//状态
			m_cList.SetItemText(v_iItem,11,v_sDate);					//更新日期

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

		v_iSum=v_iID;													//总数

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
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CCON_NodeB::My_ExecuteSQL(CString v_sSql)
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

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
