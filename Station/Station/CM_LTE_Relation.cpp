// CM_LTE_Relation.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "CM_LTE_Relation.h"

// CCM_LTE_Relation

IMPLEMENT_DYNCREATE(CCM_LTE_Relation, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CCM_LTE_Relation::CCM_LTE_Relation()
	: CFormView(CCM_LTE_Relation::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CCM_LTE_Relation::~CCM_LTE_Relation()
{
	v_mTable_Name.clear();												//容器清除
	v_mField_Main.clear();												//容器清除
	v_mCell.clear();													//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cLog_Day);
	DDX_Control(pDX, IDC_KEY, m_cLog_Key);
	DDX_Control(pDX, IDC_BUTTON1, m_cQuery);
	DDX_Control(pDX, IDC_EXCEL, m_cExcel);
	DDX_Control(pDX, IDC_LIST3, m_cList_PM);
	DDX_Control(pDX, IDC_COMBO3, m_cType_2);
	DDX_Control(pDX, IDC_COMBO7, m_cType);
	DDX_Control(pDX, IDC_LOG_SUM3, m_cData_Sum);
	DDX_Control(pDX, IDC_KEY3, m_cLog_Key1);
	DDX_Control(pDX, IDC_COMBO8, m_cRelation);
	DDX_Control(pDX, IDC_CHECK2, m_cSave);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCM_LTE_Relation, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL, &CCM_LTE_Relation::OnBnClickedExcel)
	ON_BN_CLICKED(IDC_BUTTON1, &CCM_LTE_Relation::OnBnClickedButton1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CCM_LTE_Relation::OnLvnColumnclickList3)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST3, &CCM_LTE_Relation::OnLvnGetdispinfoList3)
	ON_CBN_CLOSEUP(IDC_COMBO1, &CCM_LTE_Relation::OnCbnCloseupCombo1)
END_MESSAGE_MAP()


// CCM_LTE_Relation 诊断

#ifdef _DEBUG
void CCM_LTE_Relation::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCM_LTE_Relation::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCM_LTE_Relation 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CString v_sTemp;													//临时变量
	//窗体名称
	v_sFrame_Name="LTE-Tdd相邻关系";									//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗【配置】
	::SendMessage(m_cList_PM.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_PM.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_cList_PM.InsertColumn(1,"时间",LVCFMT_CENTER,80);  
	m_cList_PM.InsertColumn(2,"DN",LVCFMT_CENTER,120);  
	m_cList_PM.InsertColumn(3,"目标小区",LVCFMT_LEFT,120);				//
	m_cList_PM.InsertColumn(4,"相邻小区",LVCFMT_LEFT,120);				//
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//初始化：天数选择
	m_cLog_Day.AddString("中兴");										//
	m_cLog_Day.AddString("华为");										//
	m_cLog_Day.SetCurSel(0);											//默认查询天数：中兴；

	m_cRelation.AddString("与");
	m_cRelation.AddString("或");
	m_cRelation.SetCurSel(0);											//默认

	v_sTable_Pro="";													//初始化：文件前缀
	v_iList_Item=-1;													//默认：无选中的列表Item

	//获取：性能参数
	My_LoadData_Para();													//获取：性能参数
	My_Get_Cm_Field();													//厂家选择

	//获取：配置信息
	v_iWorking=2;														//工作任务：获取配置信息

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：子窗销毁
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::OnDestroy()
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
void CCM_LTE_Relation::OnSize(UINT nType, int cx, int cy)
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
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CCM_LTE_Relation::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：导出Excel
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::OnBnClickedExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList_PM);											//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Log列表排序(右击列表任意列)【配置】
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Relation::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	m_cList_PM.My_Sort_Virtual(pNMHDR);									//列表排序：虚拟列表

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：厂家选择
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::OnCbnCloseupCombo1()
{
	// TODO: 在此添加控件通知处理程序代码

	//获取：配置参数(依据：日期、文件)
	My_Get_Cm_Field();													//获取配置参数(列显示)
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取配置参数(列显示)
//	参	  数：v_sType：文件、日期
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::My_Get_Cm_Field()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp,v_sTable,v_sMaker,v_sNum;						//临时变量
	int			v_iNum;													//临时变量

	//【0】初始化
	m_cList_PM.DeleteAllItems();										//删除所有条目
	while(m_cList_PM.DeleteColumn(0));									//删除所有列：ObjType信息列表
	m_cType.ResetContent();												//删除
	m_cType_2.ResetContent();											//删除
	m_cData_Sum.SetWindowText("");										//清除配置数量

	//【1】获取：文件名称 (文件参数+选择日期)
	if (m_cLog_Day.GetCurSel()==1)
		v_sMaker="HW";													//华为
	else
		v_sMaker="ZTE";													//中兴
	v_sTable_Pro = "Xm_LTE_"+v_sMaker+"_EutranRelationTdd_Day";			//表名称

	//【2】判断：文件是否存在？？？
	set<CString>::iterator v_pFile;										//迭代器
	v_pFile=v_mTable_Name.find(v_sTable_Pro);							//查找文件
	if (v_pFile==v_mTable_Name.end())									//不存在？？？
	{
		MessageBox("无效选择!!!  请重新选择");							//提示
		return;															//返回
	}

	//【3】获取：文件字段(显示)
	v_mField_Main.clear();												//清除容器
	My_Get_File_Field(&v_mField_Main,v_sTable_Pro);						//获取：文件字段

	//【4】显示：文件字段信息(列信息)

	//在ObjType信息列表添加信息
	m_cList_PM.InsertColumn(0,"序号",LVCFMT_CENTER,60);					//
	m_cList_PM.InsertColumn(1,"时间",LVCFMT_CENTER,80);					//
	m_cList_PM.InsertColumn(2,"DN",LVCFMT_LEFT,120);					//
	m_cList_PM.InsertColumn(3,"目标小区",LVCFMT_LEFT,120);				//
	m_cList_PM.InsertColumn(4,"相邻小区",LVCFMT_LEFT,120);				//

	v_sOutput="序号,时间,DN,目标小区,相邻小区";							//

	//加入列、下拉列表
	m_cType.AddString("目标小区");										//
	m_cType_2.AddString("目标小区");									//
	m_cType.AddString("相邻小区");										//
	m_cType_2.AddString("相邻小区");									//
	mString_Int::iterator v_pField;										//迭代器
	v_iNum=5;
	for(v_pField=v_mField_Main.begin();v_pField!=v_mField_Main.end();v_pField++)	//遍历
	{
		//加入列
		m_cList_PM.InsertColumn(v_iNum,v_pField->first,LVCFMT_LEFT,120);//第n列：性能

		v_sOutput+=","+v_pField->first;									//文件

		v_iNum++;														//计数器
		
		//加入下拉
//		m_cType.AddString(v_pField->first);								//
//		m_cType_2.AddString(v_pField->first);							//
	}
	m_cList_PM.SetExtendedStyle(m_cList_PM.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	m_cType.SetCurSel(0);												//默认
	m_cType_2.SetCurSel(0);												//默认
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：虚拟列表消息响应函数
//	工	  作：依据列表需要的元素参数(行、列)，在数据容器中查询数据，赋值到列表中。
//------------------------------------------------------------------------------------------------------	
void CCM_LTE_Relation::OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取：虚拟列表显示数据获取
	m_cList_PM.Get_List_Data(pDispInfo);								//虚拟列表显示数据获取

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击查询按钮【配置查询】
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：小区查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CCM_LTE_Relation::My_Thread_Inoput(LPVOID lparam)
{
	CCM_LTE_Relation *  lp_this = NULL ;
	lp_this = (CCM_LTE_Relation *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel.EnableWindow(false);								//导出：无效
	lp_this->m_cType_2.EnableWindow(false);								//性能类型：无效
	lp_this->m_cType.EnableWindow(false);								//查询类型：无效
	lp_this->m_cLog_Day.EnableWindow(false);							//天数：无效
	lp_this->m_cLog_Key.EnableWindow(false);							//输入：无效
	lp_this->m_cLog_Key1.EnableWindow(false);							//输入：无效
	lp_this->m_cRelation.EnableWindow(false);							//与或关系：无效
	lp_this->m_cSave.EnableWindow(false);								//保存文件：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel.EnableWindow(true);								//导出：有效
	lp_this->m_cType_2.EnableWindow(true);								//性能类型：有效
	lp_this->m_cType.EnableWindow(true);								//查询类型：有效
	lp_this->m_cLog_Day.EnableWindow(true);								//天数：有效
	lp_this->m_cLog_Key.EnableWindow(true);								//输入：有效
	lp_this->m_cLog_Key1.EnableWindow(true);							//输入：有效
	lp_this->m_cRelation.EnableWindow(true);							//与或关系：有效
	lp_this->m_cSave.EnableWindow(true);								//保存文件：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Relation::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_CM();												//装载性能信息
		break;
	case 2:
		My_LoadData_Cell();												//获取：小区信息
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载配置数据
//	依    据：
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Relation::My_LoadData_CM()
{
	CString			v_sTemp;											//临时变量
	COleDateTime	v_dDate_Start,v_dDate_End,v_dDate;					//查询开始日期、结束日期
	CString			v_sTime_Start,v_sTime_End,v_sTable_Num;				//日期

	//【1】显示清除：清除列表性能信息
	m_cData_Sum.SetWindowText("");										//清除：性能数据总数
	v_iCM_Sum=0;														//获取的配置总数

	//【2】复位：虚拟列表
	m_cList_PM.Put_Line_Begin();										//列表添加数据开始

	//【4】配置查询：日表  
	My_LoadData_CM_Data(v_sTable_Pro);									//获取配置数据

	//【5】设置：虚拟列表有效
	m_cList_PM.Put_Line_End();											//列表添加数据结束
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载配置数据
//	参	  数：v_sTable：表名称；
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Relation::My_LoadData_CM_Data(CString v_sTable)
{
	CString			v_sSql,v_sCSV,v_sStr,v_sCSV_File,v_sPath_Data;		//临时变量
	CString			v_sTemp,v_sTime,v_sLog_Key,v_sLog_Key1;				//临时变量
	int				v_iItem,v_iLen;										//临时变量
	CString			v_sObject,v_sDN,v_sValues,v_sSelect,v_sSelect1;		//临时变量
	CString			v_sRelation="",v_sRelation1="",v_sCell_ID,v_sE_NodeB_ID,v_sCell,v_sCell_N;	//临时变量
	bool			v_bOK,v_bOK1;										//临时变量
	COleDateTime	v_dTime;											//日期

	CFile	v_fWriter;													//实例化
	CFileFind	finder;

	if (m_cSave.GetCheck())												//保存文件　？？？
	{
		//目录处理
		v_sPath_Data=theApp.m_sPath+"\\DATA";							//文件目录
		if (!finder.FindFile(v_sPath_Data))								//存在？
		{
			if (!::CreateDirectory(v_sPath_Data, NULL))					//不存在：创建目录
			{
				MessageBoxA("异常："+v_sPath_Data+" 目录创建失败。");	//失败
				return;
			}
		}

		for(int v_iNum3=0;v_iNum3<100;v_iNum3++)
		{
			if (v_iNum3==0)												//字符串
				v_sStr="";
			else
				v_sStr.Format("_%02d",v_iNum3);							//字符串
			//生成CSV文件：文件名称v_sCSV_File
			v_sCSV_File = theApp.m_sPath+"\\DATA\\Excel"+v_sStr+".csv";	//设置：CSV默认文件名称Excel.csv
			if(v_fWriter.Open(v_sCSV_File, CFile::modeCreate | CFile::modeWrite))	//打开文件，如不存在则生成，如存在则清空文件；
			{
				v_sOutput +="\r\n";										//回车换行
				v_fWriter.Write(v_sOutput,v_sOutput.GetLength());		//写入字符串
				break;
			}
		}
	}

	mString_Int		v_mField_Table;										//读取表的字段容器
	mString_Int::iterator v_pField_Table;								//迭代器
	mString_Int::iterator v_pField_Main;								//迭代器
	mString_String::iterator v_pCell;									//迭代器

	//虚拟列表
	mInt_String		v_mLine_Data;										//虚拟列表行数据

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		//【1】检查：表名称是否有效【依据：表名称集合v_mTable_Name】
		set<CString>::iterator v_pTable=v_mTable_Name.find(v_sTable);	//查找？？
		if (v_pTable==v_mTable_Name.end())								//没找到
			return;														//无效返回

		My_Get_File_Field(&v_mField_Table,v_sTable);					//获取：文件字段

		//【2】连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql+="select * from "+v_sTable;
		
		m_cLog_Key.GetWindowText(v_sLog_Key);							//获得：查询KEY
		v_sLog_Key.Replace("[","[[]");									//限制处理
		v_sLog_Key.Replace("'","");										//限制处理
		v_sLog_Key.Trim();												//去掉空格

		m_cLog_Key1.GetWindowText(v_sLog_Key1);							//获得：查询KEY1
		v_sLog_Key1.Replace("[","[[]");									//限制处理
		v_sLog_Key1.Replace("'","");									//限制处理
		v_sLog_Key1.Trim();												//去掉空格

		if (!v_sLog_Key.IsEmpty())										//查询KEY处理
		{
			if(m_cType.GetCurSel()>=2)									//选择：对象
			{
				m_cType.GetLBText(m_cType.GetCurSel(),v_sSelect);
				v_pField_Main=v_mField_Main.find(v_sSelect);			//查找：字段
				if (v_pField_Main!=v_mField_Main.end())					//找到？？
					v_sRelation="["+v_sSelect+"] like '%"+v_sLog_Key+"%'";		//选择的配置
			}
			else
			{
				if (m_cLog_Day.GetCurSel()==1)							//华为？？？
				{
					if (m_cType.GetCurSel()==0)							//目标
						v_sRelation="[LocalCellName] like '%"+v_sLog_Key+"%'";		//选择的配置
					else
						v_sRelation="[zObject] like '%"+v_sLog_Key+"%'";			//选择的配置
				}
			}
		}
		if (!v_sLog_Key1.IsEmpty())										//查询KEY1处理
		{
			if(m_cType_2.GetCurSel()>=2)								//选择：对象
			{
				m_cType_2.GetLBText(m_cType_2.GetCurSel(),v_sSelect1);
				v_pField_Main=v_mField_Main.find(v_sSelect1);			//查找：字段
				if (v_pField_Main!=v_mField_Main.end())					//找到？？
					v_sRelation1="["+v_sSelect1+"] like '%"+v_sLog_Key1+"%'";	//选择的配置
			}
			else
			{
				if (m_cLog_Day.GetCurSel()==1)							//华为？？？
				{
					if (m_cType_2.GetCurSel()==0)						//目标
						v_sRelation1="[LocalCellName] like '%"+v_sLog_Key1+"%'";	//选择的配置
					else
						v_sRelation1="[zObject] like '%"+v_sLog_Key1+"%'";			//选择的配置
				}
			}
		}
		if (!v_sRelation.IsEmpty() && !v_sRelation1.IsEmpty())			//两条件
		{
			if (m_cRelation.GetCurSel()==0)								//与
				v_sSql+=" where ("+v_sRelation+" and "+v_sRelation1+")";//与
			else														//或
				v_sSql+=" where ("+v_sRelation+" or "+v_sRelation1+")";	//与
		}
		else if (!v_sRelation.IsEmpty())								//条件
			v_sSql+=" where "+v_sRelation;
		else if (!v_sRelation1.IsEmpty())								//条件1
			v_sSql+=" where "+v_sRelation1;

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		//【3】获取性能数据
		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			//数据复位
			v_mLine_Data.clear();										//容器清除

			//【4】获取数据
			v_dTime=v_cAdoConn.GetField_DataTime("zTime");				//zTime
			v_sTime=v_dTime.Format("%Y-%m-%d");							//字符串
			v_sDN=v_cAdoConn.GetField_String("zDN");					//zDN

			//获取：目标小区、邻居小区
			if (m_cLog_Day.GetCurSel()==1)								//华为？？？
			{
				//华为：直接读取字段
				v_sCell=v_cAdoConn.GetField_String("LocalCellName");	//LocalCellName
				v_sCell_N=v_cAdoConn.GetField_String("zObject");		//zObject[邻居]
			}
			else
			{
				//中兴：从DN中解析、过滤 [依据DN][ZTE-CMLN-SY2;SubNetwork=ltetdd45;ManagedElement=ENODEBME927500;EnbFunction=927500;EutranCellTdd=12;EutranRelationTdd=0:460:00:275597:1]
				v_sCell="";
				v_sCell_N="";
				v_iLen=v_sDN.Find("EnbFunction=");
				if (v_iLen>=0)
				{
					v_sCell_ID="";
					v_sE_NodeB_ID=v_sDN.Mid(v_iLen+12);
					v_iLen=v_sE_NodeB_ID.Find("EutranCellTdd=");
					if (v_iLen>=0)
					{
						v_sCell_ID=v_sE_NodeB_ID.Mid(v_iLen+14);
						v_iLen=v_sCell_ID.Find(";");
						if (v_iLen>=0)
							v_sCell_ID=v_sCell_ID.Left(v_iLen);
					}
					v_iLen=v_sE_NodeB_ID.Find(";");
					if (v_iLen>=0)
						v_sE_NodeB_ID=v_sE_NodeB_ID.Left(v_iLen);
					if (!v_sE_NodeB_ID.IsEmpty() && !v_sCell_ID.IsEmpty())
					{
						v_pCell=v_mCell.find(v_sE_NodeB_ID+"|"+v_sCell_ID);	//小区查询
						if (v_pCell!=v_mCell.end())							//找到？？？
							v_sCell=v_pCell->second;						//获取：目标小区
					}
				}

				v_iLen=v_sDN.Find(":");
				if (v_iLen>=0)
				{
					v_sE_NodeB_ID=v_sDN.Mid(v_iLen+1);
					v_iLen=v_sE_NodeB_ID.Find(":");
					if (v_iLen>=0)
					{
						v_sE_NodeB_ID=v_sE_NodeB_ID.Mid(v_iLen+1);
						v_iLen=v_sE_NodeB_ID.Find(":");
						if (v_iLen>=0)
						{
							v_sCell_ID="";
							v_sE_NodeB_ID=v_sE_NodeB_ID.Mid(v_iLen+1);
							v_iLen=v_sE_NodeB_ID.Find(":");
							if (v_iLen>=0)
							{
								v_sCell_ID=v_sE_NodeB_ID.Mid(v_iLen+1);
								v_sE_NodeB_ID=v_sE_NodeB_ID.Left(v_iLen);
							}
							if (!v_sE_NodeB_ID.IsEmpty() && !v_sCell_ID.IsEmpty())
							{
								v_pCell=v_mCell.find(v_sE_NodeB_ID+"|"+v_sCell_ID);	//小区查询
								if (v_pCell!=v_mCell.end())							//找到？？？
									v_sCell_N=v_pCell->second;						//获取：邻居小区
							}
						}
					}
				}

				//过滤：满足查询
				v_bOK=true;												//有效
				v_bOK1=true;											//有效

				if (!v_sLog_Key.IsEmpty())								//条件
				{
					if (m_cType.GetCurSel()==0)							//目标
					{
						if (v_sCell.Find(v_sLog_Key)<0)					//不满足？？？
							v_bOK=false;								//无效
					}
					else
					{
						if (v_sCell_N.Find(v_sLog_Key)<0)				//不满足？？？
							v_bOK=false;								//无效
					}
				}
				if (!v_sLog_Key1.IsEmpty())								//条件1
				{
					if (m_cType_2.GetCurSel()==0)						//目标
					{
						if (v_sCell.Find(v_sLog_Key1)<0)				//不满足？？？
							v_bOK1=false;								//无效
					}
					else
					{
						if (v_sCell_N.Find(v_sLog_Key1)<0)				//不满足？？？
							v_bOK1=false;								//无效
					}
				}

				if (!v_sLog_Key.IsEmpty() && !v_sLog_Key1.IsEmpty())	//两条件
				{
					if (m_cRelation.GetCurSel()==0)						//与
					{
						if (!v_bOK || !v_bOK1)
						{
							v_pRecordset->MoveNext();					//下一条
							continue;									//下一条
						}
					}
					else												//或
					{
						if (!v_bOK && !v_bOK1)
						{
							v_pRecordset->MoveNext();					//下一条
							continue;									//下一条
						}
					}
				}
				else if (!v_sLog_Key.IsEmpty())							//条件
				{
					if (!v_bOK)
					{
						v_pRecordset->MoveNext();						//下一条
						continue;										//下一条
					}
				}
				else if (!v_sLog_Key1.IsEmpty())						//条件1
				{
					if (!v_bOK1)
					{
						v_pRecordset->MoveNext();						//下一条
						continue;										//下一条
					}
				}
			}
		
			v_sTemp.Format(" %06d",v_iCM_Sum+1);
			if (!m_cSave.GetCheck())									//显示？？？
			{
				v_mLine_Data.insert(pair<int,CString>(0,v_sTemp));		//序号
				v_mLine_Data.insert(pair<int,CString>(1,v_sTime));		//时间
				v_mLine_Data.insert(pair<int,CString>(2,v_sDN));		//DN
				v_mLine_Data.insert(pair<int,CString>(3,v_sCell));		//目标小区
				v_mLine_Data.insert(pair<int,CString>(4,v_sCell_N));	//相邻小区
			}
			else														//保存文件
				v_sCSV=v_sTemp+","+v_sTime+","+v_sDN+","+v_sCell+","+v_sCell_N;

			//【5】显示：性能
			v_iItem=5;													//序号
			v_pField_Table=v_mField_Table.begin();						//字段首指针
			while(v_pField_Table!=v_mField_Table.end())					//字段结束？？
			{
				//是否有效字段？？？【与主字段比v_mField_Main】
				v_pField_Main=v_mField_Main.find(v_pField_Table->first);//查找：字段
				if (v_pField_Main!=v_mField_Main.end())					//找到？？
				{
					v_sValues=v_cAdoConn.GetField_String(v_pField_Table->first);	//获取：配置数据
					if (!m_cSave.GetCheck())							//显示？？？
						v_mLine_Data.insert(pair<int,CString>(v_iItem,v_sValues));	//配置数据
					else												//保存文件
						v_sCSV+=","+v_sValues;
				}
		
				v_iItem++;												//递加
				v_pField_Table++;										//递加
			}
			
			//配置数据放入列表
			if (!m_cSave.GetCheck())									//显示？？？
				m_cList_PM.Put_Line_Data(v_mLine_Data);					//列表添加添加行数据
			else														//保存文件
			{
				v_sCSV +="\r\n";										//回车换行
				v_fWriter.Write(v_sCSV,v_sCSV.GetLength());				//写入字符串
			}

			v_pRecordset->MoveNext();									//下一条

			v_iCM_Sum++;												//++
			if ((v_iCM_Sum)%500==0)
			{
				v_sTemp.Format("%d",v_iCM_Sum);							//
				m_cData_Sum.SetWindowText(v_sTemp);						//显示：性能数据总数
			}
		}
		
		v_sTemp.Format("%d",v_iCM_Sum);									//
		m_cData_Sum.SetWindowText(v_sTemp);								//显示：性能数据总数

		v_cAdoConn.ExitConnect();										//断开连接
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	if (!m_cSave.GetCheck())											//显示？？？
	{
		//字段
		v_mField_Table.clear();											//容器清除

		//虚拟列表
		v_mLine_Data.clear();											//容器清除
	}
	else
	{
		v_fWriter.Close();												//文件关闭

		ShellExecute(NULL,"open", v_sCSV_File, NULL, NULL, SW_SHOWNORMAL);		//打开文件：Excel.csv
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能参数信息
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Relation::My_LoadData_Para()
{
	int				v_iID=0;											//临时变量
	CString			v_sTemp,v_sSql,v_sLog_Key,v_sTable;					//临时变量
	CString			v_sType,v_sPm,v_sNum;								//临时变量
	CString			v_sMaker,v_sNum_Demo,v_sFile_Name;					//临时变量

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
		v_sSql="select [id], [name] from [sysobjects] where [type] = 'u' and name like 'Xm_LTE_%' order by [name]";		//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sTable = v_cAdoConn.GetField_String("name");				//表名称

			v_mTable_Name.insert(v_sTable);								//增加集合

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
//	函数功能：装载小区信息
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Relation::My_LoadData_Cell()
{
	CString			v_sTemp,v_sSql;										//临时变量
	CString			v_sE_NodeB_ID,v_sCell_ID,v_sCell;					//临时变量

	mString_String::iterator	v_pIter;								//迭代器

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SELECT zCell,zCell_Num,LTE_NODEB.zNODEB_Name FROM LTE_Cell left join LTE_NODEB ON LTE_Cell.zNODEB=LTE_NODEB.zNODEB where LTE_NODEB.zRNC='dalian_ZTE'";	//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sE_NodeB_ID = v_cAdoConn.GetField_String("zNODEB_Name");	//E_NodeB_ID
			v_sCell_ID = v_cAdoConn.GetField_String("zCell_Num");		//Cell_ID
			v_sCell = v_cAdoConn.GetField_String("zCell");				//Cell

			if (!v_sE_NodeB_ID.IsEmpty())								//配置E_NodeB？？？
			{
				v_pIter=v_mCell.find(v_sE_NodeB_ID+"|"+v_sCell_ID);		//查找？？？
				if (v_pIter==v_mCell.end())
					v_mCell.insert(pair<CString,CString>(v_sE_NodeB_ID+"|"+v_sCell_ID,v_sCell));	//增加集合
				else
				{
					int aaa=1;											//异常
				}
			}

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
//	函数功能：获取文件字段(显示)
//	参	  数：v_mField_Main：字段容器；
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Relation::My_Get_File_Field(mString_Int* v_mField,CString v_sTable)
{
	CString			v_sSql;												//临时变量
	CString			v_sTemp,v_sName;									//临时变量

	//获取配置数据
	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//【1】获取字段数据
		v_sSql="select name from syscolumns where id=OBJECT_ID('"+v_sTable+"')";	//SQL
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			v_sName = v_cAdoConn.GetField_String("name");				//表名称

			if (v_sName!="zObject" && v_sName!="zTime" && v_sName!="zDN")
				v_mField->insert(pair<CString,int>(v_sName,0));			//增加容器【v_iNum：列序号】

			v_pRecordset->MoveNext();									//指针++
		}

		v_cAdoConn.ExitConnect();										//断开连接

		//【2】字段数据排序
		mString_Int::iterator	v_pIter;								//迭代器
		int						v_iNum=4;								//临时变量
		for (v_pIter=v_mField->begin();v_pIter!=v_mField->end();v_pIter++)		//遍历
		{
			v_pIter->second=v_iNum;										//字段序号
			v_iNum++;													//计数器
		}
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
