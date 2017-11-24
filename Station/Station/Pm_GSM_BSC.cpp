// Pm_GSM_BSC.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "Pm_GSM_BSC.h"
#include "Pm_GSM_UpDate.h"
#include "Pm_GSM_UpDate_PM.h"
#include "Pm_GSM_BSC_Add.h"


// CPm_GSM_BSC

IMPLEMENT_DYNCREATE(CPm_GSM_BSC, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CPm_GSM_BSC::CPm_GSM_BSC()
	: CFormView(CPm_GSM_BSC::IDD)
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CPm_GSM_BSC::~CPm_GSM_BSC()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_KEY, m_cKey);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
	DDX_Control(pDX, IDC_LIST3, m_cList_Station);
	DDX_Control(pDX, IDC_QUARY, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_COMBO4, m_cType);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPm_GSM_BSC, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CPm_GSM_BSC::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL2, &CPm_GSM_BSC::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CPm_GSM_BSC::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CPm_GSM_BSC::OnLvnColumnclickList3)
	ON_CBN_CLOSEUP(IDC_COMBO4, &CPm_GSM_BSC::OnCbnCloseupCombo4)
	ON_BN_CLICKED(IDC_EXCEL3, &CPm_GSM_BSC::OnBnClickedExcel3)
	ON_BN_CLICKED(IDC_EXCEL4, &CPm_GSM_BSC::OnBnClickedExcel4)
	ON_BN_CLICKED(IDC_EXCEL5, &CPm_GSM_BSC::OnBnClickedExcel5)
	ON_BN_CLICKED(IDC_EXCEL6, &CPm_GSM_BSC::OnBnClickedExcel6)
END_MESSAGE_MAP()


// CPm_GSM_BSC 诊断

#ifdef _DEBUG
void CPm_GSM_BSC::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPm_GSM_BSC::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPm_GSM_BSC 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="GSM_性能计算_BSC";									//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗
	::SendMessage(m_cList_Station.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Station.SetExtendedStyle(m_cList_Station.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//信息初始化
	m_cType.AddString("性能组");
	m_cType.AddString("性能名称");
	m_cType.SetCurSel(0);												//默认：选择第一个

	v_iList_Item=-1;													//条目选择无效

	//显示
	OnCbnCloseupCombo4();												//类型选择响应
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CPm_GSM_BSC::PreTranslateMessage(MSG* pMsg)
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
void CPm_GSM_BSC::OnDestroy()
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
void CPm_GSM_BSC::OnSize(UINT nType, int cx, int cy)
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
//	函数功能：导出
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_Station);										//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：类型选择
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnCbnCloseupCombo4()
{
	// TODO: 在此添加控件通知处理程序代码

	m_cList_Station.DeleteAllItems();									//列表清除
	while(m_cList_Station.DeleteColumn(0));								//删除所有列

	//查询类型选择
	m_cKey_Select.ResetContent();										//清除
	if (m_cType.GetCurSel()==0)											//ObjType
	{
		m_cKey_Select.AddString("性能组");
		m_cKey_Select.AddString("类型");
		m_cKey_Select.SetCurSel(0);										//默认：选择第一个

		m_cList_Station.InsertColumn(0,"序号",LVCFMT_CENTER,60);		//
		m_cList_Station.InsertColumn(1,"性能组",LVCFMT_LEFT,210);		//
		m_cList_Station.InsertColumn(2,"类型",LVCFMT_LEFT,80);			//
		m_cList_Station.InsertColumn(3,"保留天数",LVCFMT_RIGHT,120);	//

		m_cList_Station.SetNumber(3);									//数字排序有效
	}
	else																//Counter
	{
		m_cKey_Select.AddString("性能组");
		m_cKey_Select.AddString("类型");
		m_cKey_Select.AddString("性能名称");
		m_cKey_Select.AddString("计算公式");
		m_cKey_Select.AddString("ObjType");
		m_cKey_Select.SetCurSel(0);										//默认：选择第一个

		m_cList_Station.InsertColumn(0,"序号",LVCFMT_CENTER,60);		//
		m_cList_Station.InsertColumn(1,"性能组",LVCFMT_LEFT,210);		//
		m_cList_Station.InsertColumn(2,"类型",LVCFMT_LEFT,80);			//
		m_cList_Station.InsertColumn(3,"性能名称",LVCFMT_LEFT,300);		//
		m_cList_Station.InsertColumn(4,"计算公式",LVCFMT_LEFT,500);		//
		m_cList_Station.InsertColumn(5,"ObjType",LVCFMT_LEFT,200);		//
		m_cList_Station.InsertColumn(6,"单位",LVCFMT_LEFT,60);			//
		m_cList_Station.InsertColumn(7,"标记",LVCFMT_CENTER,90);		//
		m_cList_Station.InsertColumn(8,"唯一标识",LVCFMT_CENTER,0);		//

		m_cList_Station.ClearNumber(3);									//数字排序无效
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表点击响应
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
//	函数功能：列表窗排序
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList_Station.My_Sort(pNMHDR);									//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：增加
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnBnClickedExcel3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cType.GetCurSel()==1)											//性能名称
	{
		CPm_GSM_UpDate_PM	v_cDlg;										//实例化
		v_cDlg.v_sPM_Table=m_cList_Station.GetItemText(v_iList_Item,1);	//赋值：性能组
		v_cDlg.v_sType=m_cList_Station.GetItemText(v_iList_Item,2);		//赋值：类型
		v_cDlg.v_sPM_Name=m_cList_Station.GetItemText(v_iList_Item,3);	//赋值：性能名称
		v_cDlg.v_sFormula=m_cList_Station.GetItemText(v_iList_Item,4);	//赋值：计算公式
		v_cDlg.v_sObjType=m_cList_Station.GetItemText(v_iList_Item,5);	//赋值：ObjType
		v_cDlg.v_sUnit=m_cList_Station.GetItemText(v_iList_Item,6);		//赋值：单位
		v_cDlg.v_sNum=m_cList_Station.GetItemText(v_iList_Item,8);		//赋值：唯一标识
		v_cDlg.v_bFlag=true;											//增加
		v_cDlg.v_bBSC=true;												//BSC
		if (v_cDlg.DoModal()==IDOK)
			OnBnClickedQuary();											//显示：最新结果
	}
				
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：修改
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnBnClickedExcel4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (v_iList_Item==-1)
	{
		MessageBox("请选择一条有效的信息！！！","信息提示",MB_OK);		//提示
		return;
	}

	if (m_cType.GetCurSel()==0)											//性能组
	{
		CPm_GSM_UpDate	v_cDlg;											//实例化
		v_cDlg.v_sPM_Table=m_cList_Station.GetItemText(v_iList_Item,1);	//赋值：性能组
		v_cDlg.v_sType=m_cList_Station.GetItemText(v_iList_Item,2);		//赋值：类型
		v_cDlg.v_sDay=m_cList_Station.GetItemText(v_iList_Item,3);		//赋值：保留天数
		if (v_cDlg.DoModal()==IDOK)
			m_cList_Station.SetItemText(v_iList_Item,3,v_cDlg.v_sDay);	//显示：保留天数
	}
	else
	{
		CPm_GSM_UpDate_PM	v_cDlg;										//实例化
		v_cDlg.v_sPM_Table=m_cList_Station.GetItemText(v_iList_Item,1);	//赋值：性能组
		v_cDlg.v_sType=m_cList_Station.GetItemText(v_iList_Item,2);		//赋值：类型
		v_cDlg.v_sPM_Name=m_cList_Station.GetItemText(v_iList_Item,3);	//赋值：性能名称
		v_cDlg.v_sFormula=m_cList_Station.GetItemText(v_iList_Item,4);	//赋值：计算公式
		v_cDlg.v_sObjType=m_cList_Station.GetItemText(v_iList_Item,5);	//赋值：ObjType
		v_cDlg.v_sUnit=m_cList_Station.GetItemText(v_iList_Item,6);		//赋值：单位
		v_cDlg.v_sNum=m_cList_Station.GetItemText(v_iList_Item,8);		//赋值：唯一标识
		if (v_cDlg.DoModal()==IDOK)
		{
			m_cList_Station.SetItemText(v_iList_Item,4,v_cDlg.v_sFormula);	//显示：计算公式
			m_cList_Station.SetItemText(v_iList_Item,5,v_cDlg.v_sObjType);	//显示：ObjType
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：删除
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnBnClickedExcel5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString v_sSql;														//临时变量

	if (v_iList_Item==-1)
	{
		MessageBox("请选择一条有效的信息！！！","信息提示",MB_OK);		//提示
		return;
	}
	if(MessageBox("是否删除性能： "+m_cList_Station.GetItemText(v_iList_Item,3),"数据删除确认",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
	{
		v_sSql="delete from A_CAP_Info where znum='"+m_cList_Station.GetItemText(v_iList_Item,8)+"'";	//SQL
		My_ExecuteSQL(v_sSql);											//执行
		OnBnClickedQuary();												//显示：最新结果
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：∑数据管理
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnBnClickedExcel6()
{
	// TODO: 在此添加控件通知处理程序代码
	CPm_GSM_BSC_Add	v_cDlg;												//实例化
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CPm_GSM_BSC::My_Thread_Inoput(LPVOID lparam)
{
	CPm_GSM_BSC *  lp_this = NULL ;
	lp_this = (CPm_GSM_BSC *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel_NodeB.EnableWindow(false);						//NodeB导出：无效
	lp_this->m_cType.EnableWindow(false);								//类型：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel_NodeB.EnableWindow(true);							//NodeB导出：有效
	lp_this->m_cType.EnableWindow(true);								//类型：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_Query_Station();												//站址查询
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询信息
//------------------------------------------------------------------------------------------------------
void CPm_GSM_BSC::My_Query_Station()
{
	int				v_iID=0;											//临时变量
	CString			v_sSql,v_sTemp;										//临时变量
	CString			v_sLog_Key;

	v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";

	//查询条件
	m_cKey.GetWindowText(v_sLog_Key);									//获得：查询KEY
	v_sLog_Key.Replace("[","[[]");										//限制处理
	v_sLog_Key.Replace("'","");											//限制处理
	v_sLog_Key.Trim();													//去掉空格

	if (m_cType.GetCurSel()==0)											//性能组
	{
		v_sSql += "SELECT * FROM A_CAPGROUP_Info where ztypename like '%BSC%'"; 

		if (!v_sLog_Key.IsEmpty())										//查询KEY处理
		{
			switch (m_cKey_Select.GetCurSel())							//选择？？？
			{
			case 0:														//性能组
				v_sSql+=" where zgroupname like '%"+v_sLog_Key+"%'";
				break;
			case 1:														//类型
				v_sSql+=" where ztypename like '%"+v_sLog_Key+"%'";
				break;
			default:
				break;
			}
		}
		v_sSql+=" order by zgroupname";
	}
	else
	{
		v_sSql += "SELECT znum,PM_G.zgroupname,PM_G.ztypename,zcap,zformula,zobjtypes,zunit,zmark FROM A_CAP_Info \
				  left join A_CAPGROUP_Info as PM_G on PM_G.zgroupnum=A_CAP_Info.zgroupnum where ztypename like '%BSC%'"; 

		if (!v_sLog_Key.IsEmpty())										//查询KEY处理
		{
			switch (m_cKey_Select.GetCurSel())							//选择？？？
			{
			case 0:														//性能组
				v_sSql+=" and zgroupname like '%"+v_sLog_Key+"%'";
				break;
			case 1:														//类型
				v_sSql+=" and ztypename like '%"+v_sLog_Key+"%'";
				break;
			case 2:														//性能名称
				v_sSql+=" and zcap like '%"+v_sLog_Key+"%'";
				break;
			case 3:														//计算公式
				v_sSql+=" and zformula like '%"+v_sLog_Key+"%'";
				break;
			case 4:														//ObjType
				v_sSql+=" and zobjtypes like '%"+v_sLog_Key+"%'";
				break;
			default:
				break;
			}
		}
		v_sSql+=" order by zgroupname,zcap";
	}

	My_LoadData_Station_Data(v_sSql);									//获取：站址信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载信息
//------------------------------------------------------------------------------------------------------			
void CPm_GSM_BSC::My_LoadData_Station_Data(CString v_sSql)
{
	int				v_iItem,v_iID=0;									//临时变量
	CString			v_sTemp;											//临时变量

	CString			v_sPM_Table,v_sType,v_sPM_Name,v_sFormula,v_sObjType,v_sUnit,v_sMark;	//临时变量
	int				v_iDay,v_iMark,v_iNum;								//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList_Station.DeleteAllItems();								//清除Items
		v_iList_Item=-1;												//条目选择无效

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			if (m_cType.GetCurSel()==0)									//性能组
			{
				v_sPM_Table = v_cAdoConn.GetField_String("zgroupname");	//性能组
				v_sType = v_cAdoConn.GetField_String("ztypename");		//类型
				v_iDay = v_cAdoConn.GetField_Int("zsavetime");			//保留天数

				//显示
				v_sTemp.Format(" %04d",v_iID+1);
				v_iItem=m_cList_Station.InsertItem(0xffff,v_sTemp);		//加入列表窗
				m_cList_Station.SetItemText(v_iItem,1,v_sPM_Table);		//性能组
				m_cList_Station.SetItemText(v_iItem,2,v_sType);			//类型
				v_sTemp.Format("%d",v_iDay);							//字符串
				m_cList_Station.SetItemText(v_iItem,3,v_sTemp);			//保留天数
			}
			else
			{
				v_sPM_Table = v_cAdoConn.GetField_String("zgroupname");	//性能组
				v_sType = v_cAdoConn.GetField_String("ztypename");		//类型
				v_sPM_Name = v_cAdoConn.GetField_String("zcap");		//性能名称
				v_sFormula = v_cAdoConn.GetField_String("zformula");	//计算公式
				v_sObjType = v_cAdoConn.GetField_String("zobjtypes");	//ObjType
				v_sUnit = v_cAdoConn.GetField_String("zunit");			//单位
				v_iMark = v_cAdoConn.GetField_Int("zmark");				//标记
				v_iNum = v_cAdoConn.GetField_Int("znum");				//唯一标识号

				//显示
				v_sTemp.Format(" %04d",v_iID+1);
				v_iItem=m_cList_Station.InsertItem(0xffff,v_sTemp);		//加入列表窗
				m_cList_Station.SetItemText(v_iItem,1,v_sPM_Table);		//性能组
				m_cList_Station.SetItemText(v_iItem,2,v_sType);			//类型
				m_cList_Station.SetItemText(v_iItem,3,v_sPM_Name);		//性能名称
				m_cList_Station.SetItemText(v_iItem,4,v_sFormula);		//计算公式
				m_cList_Station.SetItemText(v_iItem,5,v_sObjType);		//ObjType
				m_cList_Station.SetItemText(v_iItem,6,v_sUnit);			//单位
				v_sTemp=v_iMark==1?"BSC性能计算":"";					//标记
				m_cList_Station.SetItemText(v_iItem,7,v_sTemp);			//标记
				v_sTemp.Format("%d",v_iNum);							//字符串
				m_cList_Station.SetItemText(v_iItem,8,v_sTemp);			//唯一标识号
			}

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
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CPm_GSM_BSC::My_ExecuteSQL(CString v_sSql)
{
	// TODO: Add your control notification handler code here
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);

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
