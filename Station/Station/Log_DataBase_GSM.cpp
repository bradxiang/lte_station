// Log_DataBase_GSM.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "Log_DataBase_GSM.h"


// CLog_DataBase_GSM

IMPLEMENT_DYNCREATE(CLog_DataBase_GSM, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CLog_DataBase_GSM::CLog_DataBase_GSM()
	: CFormView(CLog_DataBase_GSM::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CLog_DataBase_GSM::~CLog_DataBase_GSM()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CLog_DataBase_GSM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_cList);
	DDX_Control(pDX, IDC_STATION_SUM, m_cDataBase);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLog_DataBase_GSM, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, &CLog_DataBase_GSM::OnLvnColumnclickList2)
	ON_BN_CLICKED(IDC_EXCEL, &CLog_DataBase_GSM::OnBnClickedExcel)
END_MESSAGE_MAP()


// CLog_DataBase_GSM 诊断

#ifdef _DEBUG
void CLog_DataBase_GSM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLog_DataBase_GSM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLog_DataBase_GSM 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CLog_DataBase_GSM::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="数据库容量_GSM";										//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：预警列表窗
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0," 序号",LVCFMT_CENTER,40);
	m_cList.InsertColumn(1,"表名称",LVCFMT_LEFT,180);  
	m_cList.InsertColumn(2,"类型",LVCFMT_CENTER,80);  
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
void CLog_DataBase_GSM::OnDestroy()
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
void CLog_DataBase_GSM::OnSize(UINT nType, int cx, int cy)
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
BOOL CLog_DataBase_GSM::PreTranslateMessage(MSG* pMsg)
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
void CLog_DataBase_GSM::My_LoadData()
{
	CString v_sSql;														//临时变量
	CString v_sTemp,v_sTable,v_sType;									//临时变量
	int		v_iType;													//临时变量

	mString_String	v_mObjType;											//容器：ObjType
	mString_String	v_mPM_Table;										//容器：ObjType
	mString_String::iterator	v_pIter;								//迭代器

	//-----------------------------------------------------------------------------------------------------
	//数据库总容量
	My_LoadSize_Table("",1);											//数据库容量查询，显示在1位置
	v_sTemp.Format("%4.2f",(float)v_cData_Size.v_iReserved/1024);		//容量单位转换：MB--〉G
	m_cDataBase.SetWindowTextA(v_sTemp+" G");							//显示：数据库容量

	//-----------------------------------------------------------------------------------------------------
	//GSM
	My_LoadSize("A_BSC_Info","BSC信息表","基础",0);						//获取：表容量数据
	My_LoadSize("A_CELL_Info","Cell信息表","基础",0);					//获取：表容量数据

	//性能数据
	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//获取：ObjType表名称
		v_sSql="select * from A_OBJTYPE_Info";							//加载Objtype信息
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("zobjtype");			//获取：zobjtype
			v_iType=v_cAdoConn.GetField_Int("ztype");					//获取：类型
			v_sType=(v_iType==1?"BSC":(v_iType==2?"CELL":v_iType==3?"HANGOVER":"Others"));

			v_mObjType.insert(pair<CString,CString>(v_sTable,v_sType));	//容器增加

			v_pRecordset->MoveNext();									//指针向后移动
		}

		//获取：计算性能表名称
		v_sSql="select * from A_CAPGROUP_Info";							//加载计算性能表信息
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("zgroupname");		//获取：性能表名称
			v_sType = v_cAdoConn.GetField_String("ztypename");			//获取：性能类型

			v_mPM_Table.insert(pair<CString,CString>(v_sTable,v_sType));//容器增加

			v_pRecordset->MoveNext();									//指针向后移动
		}

		v_cAdoConn.ExitConnect();										//断开连接

		//表容量
		for(v_pIter=v_mObjType.begin();v_pIter!=v_mObjType.end();v_pIter++)
		{
			My_LoadSize(v_pIter->first,v_pIter->first,v_pIter->second,2);	//获取：表容量数据		
		}
		for(v_pIter=v_mPM_Table.begin();v_pIter!=v_mPM_Table.end();v_pIter++)
		{
			My_LoadSize(v_pIter->first,v_pIter->first,v_pIter->second,2);	//获取：表容量数据		
		}
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	v_mObjType.clear();													//容器清除
	v_mPM_Table.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载表容量信息 [基础表]
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase_GSM::My_LoadSize(CString v_sTable,CString v_sAlarm_Name,CString v_sType,int v_iType)
{
	My_Data_Reset();													//变量复位
	My_LoadSize_Table(v_sTable,0);										//获取：表名称，表序号：0 [基础表显示位置在0]
	My_LoadSize_Show(v_sAlarm_Name,v_sType,v_iType,1);					//类型：基础/实时，表数量：1
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：查询表容量信息
//	参	  数：v_sTable：表名称；[查询的表名称，若为空，则表示查询整个数据库的容量]	
//			  v_iNum：表序号 [对应在列表窗中显示的位置，从0--9，通常基础表在1位置，实时表为0-9位置，动态表为0-5位置]
//	返	  回：查询数据保存在结构v_cData_Size中。[包含：总容量、数据容量、索引容量、表中记录数量]
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase_GSM::My_LoadSize_Table(CString v_sTable,int v_iNum)
{
	CString v_sSql;														//
	CString v_sTemp;													//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		//KPI实时、动态容量
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_GSM_Ip,theApp.a_sDatabase_GSM_Name,theApp.a_sDatabase_GSM_User,theApp.a_sDatabase_GSM_Password);
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
void CLog_DataBase_GSM::My_LoadSize_Show(CString v_sTable,CString v_sType,int v_iType,int v_iSum)
{
	CString v_sTemp;													//临时变量
	int		v_iItem;													//临时变量

	v_sTemp.Format(" %04d",m_cList.GetItemCount()+1);
	v_iItem=m_cList.InsertItem(0xffff,v_sTemp);							//加入列表窗
	m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sTable);					//
	m_cList.SetItemText(v_iItem,2,(_bstr_t)(v_sType));					//实时表信息
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
	m_cList.SetItemBkColor(v_iItem,v_iType==2?14805192:(v_iType==0?15647177:16647177));			//背景色
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：数据复位
//------------------------------------------------------------------------------------------------------			
void CLog_DataBase_GSM::My_Data_Reset()
{
	v_cData_Size.v_iReserved=0;											//总容量
	v_cData_Size.v_iData=0;												//数据容量
	v_cData_Size.v_iIndex_size=0;										//索引容量
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：货币格式化
//------------------------------------------------------------------------------------------------------			
CString CLog_DataBase_GSM::My_Format(CString v_sTemp)
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
void CLog_DataBase_GSM::OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导出Excel
//------------------------------------------------------------------------------------------------------
void CLog_DataBase_GSM::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
