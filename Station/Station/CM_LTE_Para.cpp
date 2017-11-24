// CM_LTE_Para.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "CM_LTE_Para.h"
#include "CM_LTE_Para_Input.h"


// CCM_LTE_Para

IMPLEMENT_DYNCREATE(CCM_LTE_Para, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CCM_LTE_Para::CCM_LTE_Para()
	: CFormView(CCM_LTE_Para::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CCM_LTE_Para::~CCM_LTE_Para()
{
	v_mCM_File.clear();													//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_LIST3, m_cList);
	DDX_Control(pDX, IDC_QUARY, m_cInput);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_QUARY2, m_cQuery);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCM_LTE_Para, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUARY, &CCM_LTE_Para::OnBnClickedQuary)
	ON_BN_CLICKED(IDC_EXCEL2, &CCM_LTE_Para::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CCM_LTE_Para::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CCM_LTE_Para::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_QUARY2, &CCM_LTE_Para::OnBnClickedQuary2)
	ON_BN_CLICKED(IDC_EXCEL3, &CCM_LTE_Para::OnBnClickedExcel3)
	ON_CBN_CLOSEUP(IDC_COMBO2, &CCM_LTE_Para::OnCbnCloseupCombo2)
END_MESSAGE_MAP()


// CCM_LTE_Para 诊断

#ifdef _DEBUG
void CCM_LTE_Para::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCM_LTE_Para::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCM_LTE_Para 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="LTE 配置管理_参数";									//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗 [BTS]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"厂家",LVCFMT_CENTER,80);					//
	m_cList.InsertColumn(2,"文件名称",LVCFMT_LEFT,120);					//
	m_cList.InsertColumn(3,"配置参数",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(4,"参数解释",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(5,"保存子表序号",LVCFMT_CENTER,100);			//
	m_cList.InsertColumn(6,"保存子表名称",LVCFMT_LEFT,200);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList.SetNumber(5);

	//信息初始化
	m_cKey_Select.AddString("华为");
	m_cKey_Select.AddString("中兴");
	m_cKey_Select.SetCurSel(0);											//默认：选择第一个

	//信息初始化
	v_iList_Item=-1;													//条目选择无效

	m_cInput.EnableWindow(false);										//表生成：无效

	My_Load_File();														//获取：配置文件信息

	//查询
	v_iWorking=1;														//工作任务：查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CCM_LTE_Para::PreTranslateMessage(MSG* pMsg)
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
void CCM_LTE_Para::OnDestroy()
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
void CCM_LTE_Para::OnSize(UINT nType, int cx, int cy)
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
void CCM_LTE_Para::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：厂家选择变化
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::OnCbnCloseupCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedQuary2();												//查询
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::OnBnClickedExcel3()
{
	// TODO: 在此添加控件通知处理程序代码
	CCM_LTE_Para_Input	v_cDlg;											//实例化
	v_cDlg.v_sMaker_OK=m_cKey_Select.GetCurSel()==0?"华为":"中兴";		//厂家
	v_cDlg.DoModal();													//打开对话窗
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表点击响应
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
void CCM_LTE_Para::OnBnClickedQuary2()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：配置表生成
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::OnBnClickedQuary()
{
	// TODO: 在此添加控件通知处理程序代码
	CString	v_sFile_Key="",v_sMaker,v_sFile="",v_sNum="";				//临时变量
	CString	v_sSql,v_sTime;												//临时变量
	COleDateTime v_dTime=COleDateTime::GetCurrentTime();				//临时变量

	if (m_cKey_Select.GetCurSel()==0)
		v_sMaker="华为";
	else
		v_sMaker="中兴";
	v_sTime=v_dTime.Format("%Y%m");										//日期
	
	if(MessageBox("生成 LTE 配置：厂家----"+v_sMaker+"；  月份----"+v_sTime+"， 生成确认 ? ","配置表生成确认",MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)	//生成确认？？？
		return;

	if (m_cKey_Select.GetCurSel()==0)
		v_sMaker="HW";
	else
		v_sMaker="ZTE";
	for (int vi=0;vi<m_cList.GetItemCount();vi++)						//遍历列表窗
	{
		if (v_sFile!=m_cList.GetItemText(vi,2) || v_sNum!=m_cList.GetItemText(vi,5))	//表名称(或子表)变化？？？
		{
			if (!v_sFile_Key.IsEmpty())									//生成新表
			{
				v_sSql+=")\r\n";										//结束
				v_sSql+="CREATE NONCLUSTERED INDEX PK_Xm_LTE_"+v_sMaker+"_"+v_sFile_Key+"_"+v_sTime
							+" ON Xm_LTE_"+v_sMaker+"_"+v_sFile_Key+"_"+v_sTime+"(zTime,zObject)";	//建索引
				My_ExecuteSQL(v_sSql);									//执行
			}
			v_sFile=m_cList.GetItemText(vi,2);							//子表名称
			v_sNum=m_cList.GetItemText(vi,5);							//子表名称
			v_sNum.Trim();												//去空格
			if (v_sNum.IsEmpty())										//为空？？？
				v_sFile_Key=v_sFile;									//表名称【不带子表】
			else
				v_sFile_Key=v_sFile+"_"+v_sNum;							//表名称【带子表】


			v_sSql="CREATE TABLE Xm_LTE_"+v_sMaker+"_"+v_sFile_Key+"_"+v_sTime
					+"([zObject] [varchar](1000) NULL,[zTime] [datetime] NOT NULL,[zDN] [varchar](3000) NULL";	//时间、小区
		}
		v_sSql+=",["+m_cList.GetItemText(vi,3)+"] [varchar](3000) NULL";//增加字段
	}
	if (!v_sFile_Key.IsEmpty())											//生成新表
	{
		v_sSql+=")\r\n";												//结束
		v_sSql+="CREATE NONCLUSTERED INDEX PK_Xm_LTE_"+v_sMaker+"_"+v_sFile_Key+"_"+v_sTime
					+" ON Xm_LTE_"+v_sMaker+"_"+v_sFile_Key+"_"+v_sTime+"(zTime,zObject)";	//建索引
		My_ExecuteSQL(v_sSql);											//执行
	}
	MessageBox("LTE配置表生成完毕 !!!");								//提示
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CCM_LTE_Para::My_Thread_Inoput(LPVOID lparam)
{
	CCM_LTE_Para *  lp_this = NULL ;
	lp_this = (CCM_LTE_Para *)lparam;

	lp_this->m_cQuery.EnableWindow(false);								//查询：无效
	lp_this->m_cExcel_NodeB.EnableWindow(false);						//NodeB导出：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cQuery.EnableWindow(true);								//查询：有效
	lp_this->m_cExcel_NodeB.EnableWindow(true);							//NodeB导出：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CCM_LTE_Para::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 1:
		My_LoadData_Per();												//查询
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载性能参数信息
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Para::My_LoadData_Per()
{
	int				v_iItem,v_iID=0,v_iNum;								//临时变量
	CString			v_sTemp,v_sSql,v_sFile_Name,v_sNum_Demo,v_sMaker;	//临时变量
	CString			v_sCm,v_sCm_Name,v_sNum;							//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList.DeleteAllItems();										//清除Items
		v_iList_Item=-1;												//条目选择无效

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Cm_LTE ";

		//查询条件
		switch (m_cKey_Select.GetCurSel())								//选择？？？
		{
		case 0:															//华为
			v_sSql+=" where zMaker='华为'";
			break;
		case 1:															//中兴
			v_sSql+=" where zMaker='中兴'";
			break;
		default:
			break;
		}
		v_sSql+=" order by zFile_Name,zNum,zCm";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sMaker = v_cAdoConn.GetField_String("zMaker");			//zMaker
			v_sFile_Name = v_cAdoConn.GetField_String("zFile_Name");	//zFile_Name
			v_sCm = v_cAdoConn.GetField_String("zCm");					//zCm
			v_sCm_Name = v_cAdoConn.GetField_String("zCm_Name");		//zCm_Name
			v_iNum = v_cAdoConn.GetField_Int("zNum");					//zNum

			//显示
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,v_sMaker);					//厂家
			m_cList.SetItemText(v_iItem,2,v_sFile_Name);				//文件名称
			m_cList.SetItemText(v_iItem,3,v_sCm);						//配置参数
			m_cList.SetItemText(v_iItem,4,v_sCm_Name);					//中文解释
			if (v_iNum==0)
				v_sNum="";												//空格
			else
				v_sNum.Format("%d",v_iNum);								//字符串
			m_cList.SetItemText(v_iItem,5,v_sNum);						//子表序号
			if (v_iNum!=0)
			{
				mString_String::iterator v_pFile=v_mCM_File.find(v_sMaker+v_sFile_Name+v_sNum);	//查找？？
				if (v_pFile!=v_mCM_File.end())
					m_cList.SetItemText(v_iItem,6,v_pFile->second);		//子表名称
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
//	函数功能：装载CM文件信息
//------------------------------------------------------------------------------------------------------			
void CCM_LTE_Para::My_Load_File()
{
	int				v_iID=0,v_iNum;										//临时变量
	CString			v_sTemp,v_sSql,v_sNum_Demo,v_sMaker;				//临时变量
	CString			v_sFile_Name,v_sFile_Demo,v_sNum;					//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		m_cList.DeleteAllItems();										//清除Items
		v_iList_Item=-1;												//条目选择无效

		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT * FROM CON_Cm_LTE_File";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_sMaker = v_cAdoConn.GetField_String("zMaker");			//zMaker
			v_sNum_Demo = v_cAdoConn.GetField_String("zNum_Demo");		//zNum_Demo
			v_sFile_Name = v_cAdoConn.GetField_String("zFile_Name");	//zFile_Name
			v_sFile_Demo = v_cAdoConn.GetField_String("zFile_Demo");	//zFile_Demo
			v_iNum = v_cAdoConn.GetField_Int("zNum");					//zNum
			
			if (v_iNum!=0)
			{
				v_sNum.Format("%d",v_iNum);								//字符串
				v_mCM_File.insert(pair<CString,CString>(v_sMaker+v_sFile_Name+v_sNum,v_sNum_Demo));	//容器增加
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
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CCM_LTE_Para::My_ExecuteSQL(CString v_sSql)
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
