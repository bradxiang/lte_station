// PM_LTE_Cal.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "PM_LTE_Cal.h"
#include "PM_LTE_Cal_Input.h"


// CPM_LTE_Cal

IMPLEMENT_DYNCREATE(CPM_LTE_Cal, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CPM_LTE_Cal::CPM_LTE_Cal()
	: CFormView(CPM_LTE_Cal::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CPM_LTE_Cal::~CPM_LTE_Cal()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Cal::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_Station);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_ELEMENT, m_cElement);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPM_LTE_Cal, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CPM_LTE_Cal::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_ELEMENT, &CPM_LTE_Cal::OnBnClickedElement)
	ON_BN_CLICKED(IDC_EXCEL2, &CPM_LTE_Cal::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_COMMAND(ID_PM_LTE_CAL_DEL, &CPM_LTE_Cal::OnConLteCellDel)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CPM_LTE_Cal::OnNMRClickList3)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CPM_LTE_Cal::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CPM_LTE_Cal::OnLvnColumnclickList3)
END_MESSAGE_MAP()


// CPM_LTE_Cal 诊断

#ifdef _DEBUG
void CPM_LTE_Cal::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPM_LTE_Cal::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPM_LTE_Cal 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Cal::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="LTE_性能管理_计算";									//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗 [站址]
	::SendMessage(m_cList_Station.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Station.InsertColumn(0,"序号",LVCFMT_CENTER,60);			//
	m_cList_Station.InsertColumn(1,"LTE性能名称",LVCFMT_LEFT,140);		//
	m_cList_Station.InsertColumn(2,"LTE中文解释",LVCFMT_LEFT,200);		//
	m_cList_Station.InsertColumn(3,"计算公式",LVCFMT_LEFT,400);			//
	m_cList_Station.InsertColumn(4,"保存",LVCFMT_CENTER,80);			//
	m_cList_Station.InsertColumn(5,"预警",LVCFMT_CENTER,80);			//
	m_cList_Station.InsertColumn(6,"预警门限",LVCFMT_RIGHT,80);			//
	m_cList_Station.InsertColumn(7,"预警类型",LVCFMT_CENTER,80);		//
	m_cList_Station.InsertColumn(8,"备注",LVCFMT_LEFT,200);				//
	m_cList_Station.SetExtendedStyle(m_cList_Station.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//信息初始化
	m_cKey_Select.AddString("LTE性能名称");
	m_cKey_Select.AddString("LTE中文解释");
	m_cKey_Select.AddString("计算公式");
	m_cKey_Select.SetCurSel(0);											//默认：选择第一个

	v_iList_Item=-1;													//条目选择无效
	v_iSum=0;															//数据总数

	//初始显示
	OnBnClickedQuary();													//显示：性能
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CPM_LTE_Cal::PreTranslateMessage(MSG* pMsg)
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
void CPM_LTE_Cal::OnDestroy()
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
void CPM_LTE_Cal::OnSize(UINT nType, int cx, int cy)
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
void CPM_LTE_Cal::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Station);										//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：站址导入
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Cal::OnBnClickedElement()
{
	// TODO: 在此添加控件通知处理程序代码
	CPM_LTE_Cal_Input	v_cDlg;											//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表点击响应
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Cal::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
void CPM_LTE_Cal::OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu	aMenu;														//菜单
	CPoint	point;														//位置点

	GetCursorPos(&point);												//获得：鼠标位置
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：点击的列表窗位置
	int v_iSelect = pNMListView->iItem;									//获取：点击列表窗的Item
	if( v_iSelect == -1 )												//有效Item？？？
		return;															//无效：返回
	if(v_iList_Item>=0)													//有效条目序号？？？
		m_cList_Station.SetItemBkColor(v_iList_Item,RGB(255,255,255));	//恢复背景色
	v_iList_Item=v_iSelect;												//获得：选中条目序号
	m_cList_Station.SetItemBkColor(v_iList_Item,RGB(255,0,0));			//设置背景色：红色

	aMenu.LoadMenu(IDR_PM_LTE_CAL_DEL);									//装载：右键菜单
	CMenu *pPopup =aMenu.GetSubMenu(0);									//生成浮动菜单
	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);			//显示：依据鼠标位置[point]

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：删除性能
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Cal::OnConLteCellDel()
{
	// TODO: 在此添加命令处理程序代码
	int		v_iItem,vi;													//临时变量
	CString	v_sCell,v_sTemp,v_sSql;										//临时变量

	vi = m_cList_Station.GetSelectedCount();							//选中条目数量
	v_sTemp.Format("%d",vi);											//字符串
	if(MessageBox("删除选中的 "+v_sTemp+" 个 性能， 删除确认 ? ","数据删除确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)	//删除确认？？？
	{
		//对所有选中的Item进行操作
		while ((v_iItem = m_cList_Station.GetNextItem(-1, LVNI_SELECTED)) != -1)//条目选中？？？
		{
			v_sCell=m_cList_Station.GetItemText(v_iItem,1);				//获得：v_sCell [数据表唯一标识，用于修改数据表]

			//删除：Cell
			v_sSql = "delete from CON_Pm_LTE_Cal where zPm='"+v_sCell+"'";	//SQL删除
			My_ExecuteSQL(v_sSql);										//写表

			//删除：列表信息
			m_cList_Station.DeleteItem(v_iItem);						//删除：选中的v_iItem
			v_iList_Item=-1;											//条目选择无效

			//保存：Log
			COleDateTime v_tTime=COleDateTime::GetCurrentTime();
			CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");
			CString v_sSql="Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"+v_sCurrentTime+"','删除性能(计算)','性能名称："
				+v_sCell+"，操作员："+theApp.a_sUser_Name+"')";
			My_ExecuteSQL(v_sSql);										//数据库执行
		}
		v_iSum-=vi;														//剩余数量
		v_sTemp.Format("%d",v_iSum);									//字符串
		m_cSum.SetWindowText(v_sTemp);									//显示：站址总数
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序 [站址]
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Cal::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList_Station.My_Sort(pNMHDR);										//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询
//------------------------------------------------------------------------------------------------------
void CPM_LTE_Cal::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CPM_LTE_Cal::My_Thread_Inoput(LPVOID lparam)
{
	CPM_LTE_Cal *  lp_this = NULL ;
	lp_this = (CPM_LTE_Cal *)lparam;

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
void CPM_LTE_Cal::My_Input_Main()
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
void CPM_LTE_Cal::My_Query_Station()
{
	int				v_iID=0;											//临时变量
	CString			v_sSql,v_sTemp;										//临时变量
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
	v_sSql += "SELECT * FROM CON_Pm_LTE_Cal"; 

	//查询日期准备
	m_cKey.GetWindowText(v_sLog_Key);									//获得：查询KEY
	v_sLog_Key.Replace("[","[[]");										//限制处理
	v_sLog_Key.Replace("'","");											//限制处理
	v_sLog_Key.Trim();													//去掉空格

	if (!v_sLog_Key.IsEmpty())											//查询KEY处理
	{
		switch (m_cKey_Select.GetCurSel())								//选择？？？
		{
		case 0:															//性能名称
			v_sSql+=" where zPm like '%"+v_sLog_Key+"%'";
			break;
		case 1:															//中文解释
			v_sSql+=" where zPm_Name like '%"+v_sLog_Key+"%'";
			break;
		case 2:															//计算公式
			v_sSql+=" where zPm_Formula like '%"+v_sLog_Key+"%'";
			break;
		default:
			break;
		}
	}
	v_sSql+=" order by zPm";

	My_LoadData_Station_Data(v_sSql);									//获取：站址信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载站址信息
//------------------------------------------------------------------------------------------------------			
void CPM_LTE_Cal::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem,v_iID=0,v_iTemp;							//临时变量
	double			v_dTemp;											//临时变量
	CString			v_sTemp;											//临时变量
	CString			v_sPM,v_sPM_Name,v_sFormula,v_sSave,v_sAlarm_Flag;	//临时变量
	CString			v_sAlarm_Value,v_sAlarm_Type,v_sDemo;				//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList_Station.DeleteAllItems();								//清除Items
		v_iList_Item=-1;												//条目选择无效

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sPM=v_cAdoConn.GetField_String("zPm");					//zPm
			v_sPM_Name=v_cAdoConn.GetField_String("zPm_Name");			//中文解释
			v_sFormula=v_cAdoConn.GetField_String("zPm_Formula");		//计算公式
			v_iTemp=v_cAdoConn.GetField_Int("zSave_Flag");				//zSave_Flag
			v_sSave=(v_iTemp==1?"保存":"不保存");						
			v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Flag");				//zAlarm_Flag
			v_sAlarm_Flag=(v_iTemp==1?"预警":"不预警");						
			v_dTemp=v_cAdoConn.GetField_Double("zAlarm_Value");			//zAlarm_Value
			v_sAlarm_Value.Format("%6.2f",v_dTemp);
			v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Type");				//zAlarm_Type
			v_sAlarm_Type=(v_iTemp==1?"高":"低");						
			v_sDemo=v_cAdoConn.GetField_String("zDemo");				//备注

			//显示
			v_sTemp.Format(" %06d",v_iID+1);
			v_iItem=m_cList_Station.InsertItem(0xffff,v_sTemp);			//加入列表窗
			m_cList_Station.SetItemText(v_iItem,1,v_sPM);				//性能
			m_cList_Station.SetItemText(v_iItem,2,v_sPM_Name);			//性能名称
			m_cList_Station.SetItemText(v_iItem,3,v_sFormula);			//计算公式
			m_cList_Station.SetItemText(v_iItem,4,v_sSave);				//保存标志
			m_cList_Station.SetItemText(v_iItem,5,v_sAlarm_Flag);		//预警标志
			m_cList_Station.SetItemText(v_iItem,6,v_sAlarm_Value);		//预警值
			m_cList_Station.SetItemText(v_iItem,7,v_sAlarm_Type);		//预警类型
			m_cList_Station.SetItemText(v_iItem,8,v_sDemo);				//中文名

			v_pRecordset->MoveNext();									//
			v_iID++;													//计数：告警数量

			if (v_iID%100==0)											//显示：100的整数
			{
				v_sTemp.Format("%d",v_iID);								//
				m_cSum.SetWindowText(v_sTemp);							//显示：总数
			}
		}
		v_sTemp.Format("%d",v_iID);										//
		m_cSum.SetWindowText(v_sTemp);									//显示：总数

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
bool CPM_LTE_Cal::My_ExecuteSQL(CString v_sSql)
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
