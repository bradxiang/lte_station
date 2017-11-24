// Alarm_LL_Cell.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "Alarm_LL_Cell.h"
#include "Alarm_LL_Cell_Time.h"


// CAlarm_LL_Cell

IMPLEMENT_DYNCREATE(CAlarm_LL_Cell, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Cell::CAlarm_LL_Cell()
	: CFormView(CAlarm_LL_Cell::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CAlarm_LL_Cell::~CAlarm_LL_Cell()
{
	v_mAlarm_LL_Cell.clear();											//容器清除
	v_mTable_Name.clear();												//容器清除
	v_mCell_ID.clear();													//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_LIST3, m_cList);
	DDX_Control(pDX, IDC_QUARY2, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel);
	DDX_Control(pDX, IDC_QUARY3, m_cTime);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAlarm_LL_Cell, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL2, &CAlarm_LL_Cell::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CAlarm_LL_Cell::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CAlarm_LL_Cell::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_QUARY2, &CAlarm_LL_Cell::OnBnClickedQuary2)
	ON_BN_CLICKED(IDC_QUARY3, &CAlarm_LL_Cell::OnBnClickedQuary3)
END_MESSAGE_MAP()


// CAlarm_LL_Cell 诊断

#ifdef _DEBUG
void CAlarm_LL_Cell::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAlarm_LL_Cell::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAlarm_LL_Cell 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="LTE 低流量小区查询";									//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗 [BTS]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"小区",LVCFMT_LEFT,200);						//第1列：小区
	m_cList.InsertColumn(2,"营业区",LVCFMT_LEFT,60);					//第2列：营业区
	m_cList.InsertColumn(3,"状态",LVCFMT_LEFT,40);						//第3列：状态
	m_cList.InsertColumn(4,"日期",LVCFMT_CENTER,120);					//第4列：日期
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//信息初始化
	v_iList_Item=-1;													//条目选择无效

	//设置默认：查询参数
	v_bDown=false;														//无效
	v_bUp=false;														//无效
	for(int vi=0;vi<5;vi++)
	{
		v_dDate_Select[vi].v_bFlag=false;								//无效
		v_dDate_Select[vi].v_iCal=0;									//<
		v_dDate_Select[vi].v_dDate=COleDateTime::GetCurrentTime();		//当前时间
		v_dDate_Select[vi].v_dValues=100;								//值
		v_dDate_Select[vi].v_iHour=8;									//8点
	}

	//获取：性能参数
	v_iWorking=2;														//工作任务：获取初始数据
	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CAlarm_LL_Cell::PreTranslateMessage(MSG* pMsg)
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
void CAlarm_LL_Cell::OnDestroy()
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
void CAlarm_LL_Cell::OnSize(UINT nType, int cx, int cy)
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
void CAlarm_LL_Cell::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：时间参数设置
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell::OnBnClickedQuary3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString	v_sTemp,v_sTemp_1;											//临时变量
	int		v_iNum,v_iTime;												//临时变量

	CAlarm_LL_Cell_Time	v_cDlg;											//实例化

	//赋值：已选择的参数
	v_cDlg.v_dDate_Select=&v_dDate_Select[0];							//日期选择
	v_cDlg.v_bDown=v_bDown;												//状态：下行
	v_cDlg.v_bUp=v_bUp;													//状态：上行
	
	if(v_cDlg.DoModal()!=IDOK)											//确认返回？？？
		return;															//无效：返回

	//获取：新选择的参数
	v_bDown=v_cDlg.v_bDown;												//状态：下行
	v_bUp=v_cDlg.v_bUp;													//状态：上行

	//调整：列信息【选择的粒度】
	m_cList.DeleteAllItems();											//删除列表窗信息
	while(m_cList.DeleteColumn(0));										//删除所有列
	m_cSum.SetWindowText("");											//清除总数
	
	//默认列信息
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"小区",LVCFMT_LEFT,200);						//第1列：小区
	m_cList.InsertColumn(2,"营业区",LVCFMT_LEFT,60);					//第2列：营业区
	m_cList.InsertColumn(3,"状态",LVCFMT_LEFT,40);						//第3列：状态

	v_iNum=4;															//列序号
	//下行有效
	if (v_bDown)														//有效：下行？？？
	{
		for (v_iTime=0;v_iTime<5;v_iTime++)								//遍历：粒度
		{
			if (v_dDate_Select[v_iTime].v_bFlag)						//有效：粒度？？？
			{
				v_sTemp=v_dDate_Select[v_iTime].v_dDate.Format("%Y-%m-%d");	//日期
				v_sTemp_1.Format("%02d",v_dDate_Select[v_iTime].v_iHour);	//小时
				v_sTemp="下行_"+v_sTemp+"_"+v_sTemp_1;					//列名称
				m_cList.InsertColumn(v_iNum,v_sTemp,LVCFMT_RIGHT,130);	//设置：列名称
				m_cList.SetNumber(v_iNum);								//设置：数字形式
				v_iNum++;												//计数器
			}
		}
	}
	//上行有效
	if (v_bUp)															//有效：上行？？？
	{
		for (v_iTime=0;v_iTime<5;v_iTime++)								//遍历：粒度
		{
			if (v_dDate_Select[v_iTime].v_bFlag)						//有效：粒度？？？
			{
				v_sTemp=v_dDate_Select[v_iTime].v_dDate.Format("%Y-%m-%d");	//日期
				v_sTemp_1.Format("%02d",v_dDate_Select[v_iTime].v_iHour);	//小时
				v_sTemp="上行_"+v_sTemp+"_"+v_sTemp_1;					//列名称
				m_cList.InsertColumn(v_iNum,v_sTemp,LVCFMT_RIGHT,130);	//设置：列名称
				m_cList.SetNumber(v_iNum);								//设置：数字形式
				v_iNum++;												//计数器
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表点击响应
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
//	函数功能：查询
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell::OnBnClickedQuary2()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CAlarm_LL_Cell::My_Thread_Inoput(LPVOID lparam)
{
	CAlarm_LL_Cell *  lp_this = NULL ;
	lp_this = (CAlarm_LL_Cell *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效
	lp_this->m_cTime.EnableWindow(false);								//时间设置：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效
	lp_this->m_cTime.EnableWindow(true);								//时间设置：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CAlarm_LL_Cell::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Per();												//查询
		break;
	case 2:
		My_LoadData_Para();												//获取：性能参数
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询小区流量信息
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Cell::My_LoadData_Per()
{
	bool		v_bOK=false,v_bCell=false;								//临时变量
	CString		v_sTable,v_sTemp;										//临时变量
	int			v_iTime,v_iNum,v_iID=0;									//临时变量

	//【1】初始化
	m_cList.DeleteAllItems();											//删除列表窗信息
	v_mAlarm_LL_Cell.clear();											//容器清除
	m_cSum.SetWindowText("");											//清除总数

	if (!v_bDown && !v_bUp)												//无效粒度选择？？？
	{
		MessageBox("  请选择有效的性能 !!! ");							//窗体提示
		return;
	}

	//【2】获取：性能数据【由于各种查询都是或的关系，所以第一次查询是增加，以后查询都是剔除；】
	for (v_iTime=0;v_iTime<5;v_iTime++)									//遍历：粒度
	{
		if (v_dDate_Select[v_iTime].v_bFlag)							//有效：粒度？？？
		{
			v_bOK=true;													//设置有效：粒度选择

			//【3】获取：表名称
			v_sTable=v_dDate_Select[v_iTime].v_dDate.Format("%Y%m");	//年月
			v_sTable="Pm_LTE_PDCP_"+v_sTable+"_"+My_Get_Table_Num(v_dDate_Select[v_iTime].v_dDate.GetDay());	//文件名称(表序号)

			//【4】检查：表名称是否有效【依据：表名称集合v_mTable_Name】【如果表名称无效，则查不出性能数据。】
			set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//查找？？
			if (v_pTable==v_mTable_Name.end())							//没找到：数据表
			{
				v_sTable=v_dDate_Select[v_iTime].v_dDate.Format("%Y%m%d");	//年月日
				MessageBox("  请选择有效的粒度："+v_sTable);			//窗体提示
				break;
			}
			else
			{
				if (v_bDown)											//下行有效？？？
				{
					My_LoadData_PM_Data(v_iTime,1,v_sTable,v_bCell);	//性能获取
					v_bCell=true;										//设置：第一次获取有效
				}
				if (v_bUp)												//上行有效？？？
				{
					My_LoadData_PM_Data(v_iTime,0,v_sTable,v_bCell);	//性能获取
					v_bCell=true;										//设置：第一次获取有效
				}
			}

			//【5】
		}
	}
	if (!v_bOK)															//无效粒度选择？？？
	{
		MessageBox("  请选择有效的粒度 !!! ");							//窗体提示
		return;
	}

	//【3】显示：性能数据
	map_Alarm_LL_PM_DATA::iterator	v_pPM_Cell;							//迭代器
	for (v_pPM_Cell=v_mAlarm_LL_Cell.begin();v_pPM_Cell!=v_mAlarm_LL_Cell.end();v_pPM_Cell++)	//遍历：小区
	{
		int v_iIndex = m_cList.InsertItem(0xffffff,_T("0"));
		v_sTemp.Format("%06d",v_iIndex+1);
		m_cList.SetItemText(v_iIndex,0,v_sTemp);						//序号
		m_cList.SetItemText(v_iIndex,1,v_pPM_Cell->first);				//Cell
		m_cList.SetItemText(v_iIndex,2,v_pPM_Cell->second.v_sRegion);	//营业区
		m_cList.SetItemText(v_iIndex,3,v_pPM_Cell->second.v_sOnline);	//状态
		v_iNum=4;														//列序号
		if (v_bDown)													//有效：下行？？？
		{
			for (v_iTime=0;v_iTime<5;v_iTime++)							//遍历：粒度
			{
				if (v_dDate_Select[v_iTime].v_bFlag)					//有效：粒度？？？
				{
					v_sTemp.Format("%8.2f",v_pPM_Cell->second.v_dD_Values[v_iTime]);	//字符串
					m_cList.SetItemText(v_iIndex,v_iNum,v_sTemp);		//性能数值
					v_iNum++;											//计数器
				}
			}
		}
		//上行有效
		if (v_bUp)														//有效：上行？？？
		{
			for (v_iTime=0;v_iTime<5;v_iTime++)							//遍历：粒度
			{
				if (v_dDate_Select[v_iTime].v_bFlag)					//有效：粒度？？？
				{
					v_sTemp.Format("%8.2f",v_pPM_Cell->second.v_dU_Values[v_iTime]);	//字符串
					m_cList.SetItemText(v_iIndex,v_iNum,v_sTemp);		//性能数值
					v_iNum++;											//计数器
				}
			}
		}
	
		v_iID++;														//计数器
		if (v_iID%100==0)
		{
			v_sTemp.Format("%d",v_iID);									//
			m_cSum.SetWindowText(v_sTemp);								//显示：总数
		}
	}
	v_sTemp.Format("%d",v_iID);											//
	m_cSum.SetWindowText(v_sTemp);										//显示：Log总数
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取性能数据
//	参    数：v_iTime：粒度序号；	v_iType：(1：下行；0：上行；)	v_sTable：表名称
//	结    果：性能数据放入容器
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Cell::My_LoadData_PM_Data(int v_iTime,int v_iType,CString v_sTable,bool v_bCell)
{
	CString			v_sSql,v_sPM_Cell,v_sRegion,v_sOnline;				//临时变量
	CString			v_sTemp;											//临时变量
	COleDateTime	v_tDate;											//日期
	double			v_dValue;											//性能数值
	int				v_iCell_ID;											//临时变量

	_variant_t		Var;

	Alarm_LL_PM_DATA		v_cPM_DATA;									//结构

	map_Cell_ID::iterator	v_pIter_Cell;								//迭代器
	map_Alarm_LL_PM_DATA::iterator	v_pPM_Cell;							//迭代器

	CString		v_sField,v_sValue,v_sTime,v_sCal;						//临时变量


	//【1】初始化
	v_sField=v_iType?"PDCP.UpOctDl":"PDCP.UpOctUl";						//字段名称
	v_sValue.Format("%12.4f",v_dDate_Select[v_iTime].v_dValues*1024*1024/1000);	//数值(含计算公式)
	v_sTime=v_dDate_Select[v_iTime].v_dDate.Format("%Y%m%d");			//日期
	v_sTemp.Format("%02d:00:00",v_dDate_Select[v_iTime].v_iHour);		//小时
	v_sTime+=" "+v_sTemp;												//粒度时间
	v_sCal=(v_dDate_Select[v_iTime].v_iCal==0)?"<":((v_dDate_Select[v_iTime].v_iCal==1)?">":"=");	//比较符

	//【2】做标记 【从第二次查询起，删除与前次不匹配的小区。】
	if (v_bCell)														//大于一次查询
	{
		for (v_pPM_Cell=v_mAlarm_LL_Cell.begin();v_pPM_Cell!=v_mAlarm_LL_Cell.end();v_pPM_Cell++)	//遍历：小区
			v_pPM_Cell->second.v_bFlag=false;							//初始化
	}

	//【3】性能数据获取
	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(10*60);							//超时设置(10分钟)

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zTime,["+v_sField+"] from "+v_sTable+" where zTime='"+v_sTime+"' and ["+v_sField+"]"+v_sCal+v_sValue;		//SQL

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			Var=v_pRecordset->GetCollect((_variant_t)"zCell");
			if (Var.vt != VT_NULL)
			{
				v_iCell_ID=Var.intVal;									//获取：Cell_ID
				v_pIter_Cell=v_mCell_ID.find(v_iCell_ID);				//查找：小区ID存在？？？
				if (v_pIter_Cell==v_mCell_ID.end())						//不存在
				{
					v_pRecordset->MoveNext();							//下一小区性能
					continue;											//继续
				}
				v_sPM_Cell=v_pIter_Cell->second.v_sCell;				//小区
				v_sOnline=v_pIter_Cell->second.v_sOnline;				//状态
				v_sRegion=v_pIter_Cell->second.v_sRegion;				//营业区
			}
			else
				v_sPM_Cell="";

			Var=v_pRecordset->GetCollect((_variant_t)v_sField);			//获取：性能数值
			if (Var.vt != VT_NULL)
			{
				v_dValue=Var.dblVal;
				v_dValue=v_dValue*1000/1024/1024;						//流量：计算公式
			}
			else
				v_dValue=0;

			if (!v_bCell)												//第一次查询？？？
			{															//增加
				v_pPM_Cell=v_mAlarm_LL_Cell.find(v_sPM_Cell);			//查找：小区 ？？？
				if (v_pPM_Cell==v_mAlarm_LL_Cell.end())					//不存在
				{
					v_cPM_DATA.v_sOnline=v_sOnline;						//状态
					v_cPM_DATA.v_sRegion=v_sRegion;						//区域
					if (v_iType)										//下行
						v_cPM_DATA.v_dD_Values[v_iTime]=v_dValue;		//性能
					else												//上行
						v_cPM_DATA.v_dU_Values[v_iTime]=v_dValue;		//性能

					v_mAlarm_LL_Cell.insert(pair<CString,Alarm_LL_PM_DATA>(v_sPM_Cell,v_cPM_DATA));	//容器增加
				}
			}	
			else
			{
				v_pPM_Cell=v_mAlarm_LL_Cell.find(v_sPM_Cell);			//查找：小区 ？？？
				if (v_pPM_Cell!=v_mAlarm_LL_Cell.end())					//存在
				{
					v_pPM_Cell->second.v_bFlag=true;					//设置标志：查询匹配
					if (v_iType)										//下行
						v_pPM_Cell->second.v_dD_Values[v_iTime]=v_dValue;		//性能
					else												//上行
						v_pPM_Cell->second.v_dU_Values[v_iTime]=v_dValue;		//性能
				}
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

	//【4】删除不匹配的小区 【从第二次查询起，删除与前次不匹配的小区。】
	if (v_bCell)														//大于一次查询
	{
		v_pPM_Cell=v_mAlarm_LL_Cell.begin();							//头指针
		while(v_pPM_Cell!=v_mAlarm_LL_Cell.end())						//遍历：小区
		{
			if (!v_pPM_Cell->second.v_bFlag)							//没匹配？？？
				v_pPM_Cell=v_mAlarm_LL_Cell.erase(v_pPM_Cell);			//删除小区，返回下一指针
			else
				v_pPM_Cell++;											//指针++
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取表序号
//	工	  作：1：1-5；	2：6-10；	3：11-15；	4：16-20；	5：21-25；	6：26--
//------------------------------------------------------------------------------------------------------			
CString CAlarm_LL_Cell::My_Get_Table_Num(int v_iDay)
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
//	函数功能：装载性能参数信息
//------------------------------------------------------------------------------------------------------			
void CAlarm_LL_Cell::My_LoadData_Para()
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


		//【1】获取：性能数据表名称【用于发现无效表名称】
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_LTE_%') order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//表名称

			v_mTable_Name.insert(v_sTable);								//增加集合

			v_pRecordset->MoveNext();									//指针++
		}	

		//【2】获取：小区信息【用于小区ID处理】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select zCell,zCell_ID,zCell_Name,LTE_Cell.zOnline,LTE_Cell.zRegion \
				from LTE_Cell left join LTE_NODEB on LTE_Cell.zNODEB=LTE_NODEB.zNODEB";		//查询数据库
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sCell=v_cAdoConn.GetField_String("zCell");				//zCell
			v_iCell_ID=v_cAdoConn.GetField_Int("zCell_ID");				//zCell_ID
			v_sCell_ID.Format("%d",v_iCell_ID);							//字符串
			v_sCell_Name=v_cAdoConn.GetField_String("zCell_Name");		//zCell_Name
			v_sOnline=v_cAdoConn.GetField_String("zOnline");			//zOnline
			v_sRegion=v_cAdoConn.GetField_String("zRegion");			//zRegion

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
