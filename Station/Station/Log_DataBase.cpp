// Log_DataBase.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "Log_DataBase.h"


// CLog_DataBase

IMPLEMENT_DYNCREATE(CLog_DataBase, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLog_DataBase::CLog_DataBase()
	: CFormView(CLog_DataBase::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLog_DataBase::~CLog_DataBase()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CLog_DataBase::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_cList);
	DDX_Control(pDX, IDC_STATION_SUM, m_cDataBase);
	DDX_Control(pDX, IDC_STATION_SUM3, m_cSize_DB);
	DDX_Control(pDX, IDC_STATION_SUM4, m_cSize_Log);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLog_DataBase, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, &CLog_DataBase::OnLvnColumnclickList2)
	ON_BN_CLICKED(IDC_EXCEL, &CLog_DataBase::OnBnClickedExcel)
END_MESSAGE_MAP()


// CLog_DataBase 诊断

#ifdef _DEBUG
void CLog_DataBase::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLog_DataBase::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLog_DataBase 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CLog_DataBase::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="数据库容量_LTE";										//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：预警列表窗
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0," 序号",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"表名称",LVCFMT_LEFT,300);  
	m_cList.InsertColumn(2,"类型",LVCFMT_CENTER,60);  
	m_cList.InsertColumn(3,"记录数",LVCFMT_RIGHT,70);  
	m_cList.InsertColumn(4,"占用容量",LVCFMT_RIGHT,70);  
	m_cList.InsertColumn(5,"数据容量",LVCFMT_RIGHT,70);  
	m_cList.InsertColumn(6,"索引容量",LVCFMT_RIGHT,70);  
	m_cList.InsertColumn(7,"索引比例",LVCFMT_RIGHT,60);  
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	for (int vi=3;vi<8;vi++)
		m_cList.SetNumber(vi);											//数字排序有效：对应的列

	//信息显示
	My_LoadData();														//呈现：数据库容量信息
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：子窗销毁
//------------------------------------------------------------------------------------------------------
void CLog_DataBase::OnDestroy()
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
void CLog_DataBase::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd	*v_pWnd;													//调整控件的指针

	int	v_iX0=11;														//X轴位置
	int	v_iY0=45;														//Y轴位置

	v_pWnd = GetDlgItem(IDC_LIST2);										//预警列表
	if(v_pWnd)
		v_pWnd->MoveWindow(v_iX0,v_iY0,cx-v_iX0-10,cy-v_iY0-10);		//宽、高动态调整
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CLog_DataBase::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：装载数据库容量信息
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase::My_LoadData()
{
	CString v_sSql;														//临时变量
	CString v_sTemp,v_sTable;											//临时变量

	//-----------------------------------------------------------------------------------------------------
	//磁盘剩余容量
	My_LoadSize_Disk();													//磁盘剩余容量

	//-----------------------------------------------------------------------------------------------------
	//数据库总容量
	My_LoadSize_Table("",1);											//数据库容量查询，显示在1位置
	v_sTemp.Format("%4.2f",(float)v_cData_Size.v_iReserved/1024);		//容量单位转换：MB--〉G
	m_cDataBase.SetWindowTextA(v_sTemp+" G");							//显示：数据库容量

	//-----------------------------------------------------------------------------------------------------
	//站址
	My_LoadSize("CON_Station","站址信息表",0);							//获取：表容量数据
	//GSM
	My_LoadSize("GSM_BTS","GSM 基站信息表",0);							//获取：表容量数据
	My_LoadSize("GSM_Cell","GSM Cell信息表",0);							//获取：表容量数据
	//TD-LTE
	My_LoadSize("LTE_NODEB","TD-LTE 基站信息表",0);						//获取：表容量数据
	My_LoadSize("LTE_Cell","TD-LTE Cell信息表",0);						//获取：表容量数据)
	//性能数据
	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and (name like 'Pm_%' or name like 'Xm_%') order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//表名称

			if (v_sTable.Find("Pm")>=0)
				My_LoadSize(v_sTable,v_sTable+"_性能表",2);				//获取：表容量数据
			else
				My_LoadSize(v_sTable,v_sTable+"_配置表",2);				//获取：表容量数据

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
//	函数功能：装载表容量信息 [基础表]
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase::My_LoadSize(CString v_sTable,CString v_sAlarm_Name,int v_iType)
{
	My_Data_Reset();													//变量复位
	My_LoadSize_Table(v_sTable,0);										//获取：表名称，表序号：0 [基础表显示位置在0]
	My_LoadSize_Show(v_sAlarm_Name,v_iType,1);							//类型：基础/实时，表数量：1
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：磁盘剩余容量
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase::My_LoadSize_Disk()
{
	CString v_sSql;														//临时变量
	CString v_sTemp;													//临时变量
	double	v_dSize_DB,v_dSize_Log;										//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		//KPI实时、动态容量
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="select * from CON_System";								//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		if(!v_pRecordset->adoEOF)
		{
			v_dSize_DB=v_cAdoConn.GetField_Double("zSize_DB");			//zSize_DB
			v_dSize_Log=v_cAdoConn.GetField_Double("zSize_Log");		//zSize_Log

			//显示
			v_sTemp.Format("%8.2f G",v_dSize_DB);						//字符串
			m_cSize_DB.SetWindowText(v_sTemp);							//显示
			v_sTemp.Format("%8.2f G",v_dSize_Log);						//字符串
			m_cSize_Log.SetWindowText(v_sTemp);							//显示
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
//	函数功能：查询表容量信息
//	参	  数：v_sTable：表名称；[查询的表名称，若为空，则表示查询整个数据库的容量]	
//			  v_iNum：表序号 [对应在列表窗中显示的位置，从0--9，通常基础表在1位置，实时表为0-9位置，动态表为0-5位置]
//	返	  回：查询数据保存在结构v_cData_Size中。[包含：总容量、数据容量、索引容量、表中记录数量]
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase::My_LoadSize_Table(CString v_sTable,int v_iNum)
{
	CString v_sSql;														//
	CString v_sTemp;													//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		//KPI实时、动态容量
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="sp_spaceused";											//SQL
		if (!v_sTable.IsEmpty())										//表名称不为空？
			v_sSql+=" '["+v_sTable+"]'";								//查询表名称 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		if(!v_pRecordset->adoEOF)
		{
			if (v_sTable.IsEmpty())										//表名称为空？
			{
				//整个数据库容量查询
				v_sTemp=v_cAdoConn.GetField_String("database_size");	//database_size
				int vi=v_sTemp.Find("MB",0);
				v_cData_Size.v_iReserved=atoi(v_sTemp.Left(vi));		//数值转换
			}
			else
			{
				//单个表容量查询
				v_sTemp=v_cAdoConn.GetField_String("rows");				//rows
				v_cData_Size.v_iRows[v_iNum]=atoi(v_sTemp);				//数值转换 [放在表序号对应的位置，以便显示]
				v_sTemp=v_cAdoConn.GetField_String("reserved");			//reserved
				v_cData_Size.v_iReserved+=atoi(v_sTemp);				//数值转换
				v_sTemp=v_cAdoConn.GetField_String("data");				//data
				v_cData_Size.v_iData+=atoi(v_sTemp);					//数值转换
				v_sTemp=v_cAdoConn.GetField_String("index_size");		//index_size
				v_cData_Size.v_iIndex_size+=atoi(v_sTemp);				//数值转换
			}
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
//	函数功能：显示，即容量信息添加到列表窗  [基础表记录数显示在1位置，历史表在1--12位置]
//	参	  数：v_sTable：表名称；	
//			  v_iFlag：2：实时；	1：历史；	0：基础；	
//			  v_iSum：表数量；[历史：12； 其它表：1]
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase::My_LoadSize_Show(CString v_sTable,int v_iFlag,int v_iSum)
{
	CString v_sTemp;													//临时变量
	int		v_iItem;													//临时变量

	v_sTemp.Format(" %04d",m_cList.GetItemCount()+1);
	v_iItem=m_cList.InsertItem(0xffff,v_sTemp);							//加入列表窗
	m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sTable);					//
	m_cList.SetItemText(v_iItem,2,(_bstr_t)(v_iFlag==2?"实时":(v_iFlag==1?"历史":"基础")));	//实时表信息
	for (int vi=0;vi<v_iSum;vi++)
	{
		v_sTemp.Format("%d",v_cData_Size.v_iRows[vi]);					//记录数
		v_sTemp=My_Format(v_sTemp);										//货币格式化
		m_cList.SetItemText(v_iItem,3+vi,(_bstr_t)v_sTemp);
	}
	v_sTemp.Format("%d",v_cData_Size.v_iReserved);						//占用容量
	v_sTemp=My_Format(v_sTemp);											//货币格式化
	m_cList.SetItemText(v_iItem,4,(_bstr_t)v_sTemp);
	v_sTemp.Format("%d",v_cData_Size.v_iData);							//数据容量
	v_sTemp=My_Format(v_sTemp);											//货币格式化
	m_cList.SetItemText(v_iItem,5,(_bstr_t)v_sTemp);
	v_sTemp.Format("%d",v_cData_Size.v_iIndex_size);					//索引容量
	v_sTemp=My_Format(v_sTemp);											//货币格式化
	m_cList.SetItemText(v_iItem,6,(_bstr_t)v_sTemp);
	if ((v_cData_Size.v_iData+v_cData_Size.v_iIndex_size)!=0)
	{
		v_sTemp.Format("%4.2f",(float)v_cData_Size.v_iIndex_size*100/(float)(v_cData_Size.v_iData+v_cData_Size.v_iIndex_size));	//索引比率
		m_cList.SetItemText(v_iItem,7,(_bstr_t)v_sTemp);
	}
	m_cList.SetItemBkColor(v_iItem,v_iFlag==2?14805192:(v_iFlag==0?15647177:16647177));			//背景色
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：数据复位
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase::My_Data_Reset()
{
	v_cData_Size.v_iReserved=0;											//总容量
	v_cData_Size.v_iData=0;												//数据容量
	v_cData_Size.v_iIndex_size=0;										//索引容量
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：货币格式化
//------------------------------------------------------------------------------------------------------			
CString CLog_DataBase::My_Format(CString v_sTemp)
{
	int v_iLen=v_sTemp.GetLength();										//长度
	while(v_iLen>3)														//长度>3
	{
		v_sTemp.Insert(v_iLen-3,",");									//插入：","
		v_iLen-=3;														//
	}
	return v_sTemp;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表排序(右击列表任意列)
//------------------------------------------------------------------------------------------------------
void CLog_DataBase::OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导出Excel
//------------------------------------------------------------------------------------------------------
void CLog_DataBase::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
