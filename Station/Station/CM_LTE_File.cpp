// CM_LTE_File.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "CM_LTE_File.h"


// CCM_LTE_File

IMPLEMENT_DYNCREATE(CCM_LTE_File, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CCM_LTE_File::CCM_LTE_File()
	: CFormView(CCM_LTE_File::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CCM_LTE_File::~CCM_LTE_File()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CCM_LTE_File::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_LIST3, m_cList);
	DDX_Control(pDX, IDC_EXCEL2, m_cExcel_NodeB);
	DDX_Control(pDX, IDC_QUARY2, m_cQuery);
	DDX_Control(pDX, IDC_COMBO2, m_cKey_Select);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCM_LTE_File, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEL2, &CCM_LTE_File::OnBnClickedExcel2)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CCM_LTE_File::OnNMClickList3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CCM_LTE_File::OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_QUARY2, &CCM_LTE_File::OnBnClickedQuary2)
END_MESSAGE_MAP()


// CCM_LTE_File 诊断

#ifdef _DEBUG
void CCM_LTE_File::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCM_LTE_File::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCM_LTE_File 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CCM_LTE_File::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="LTE 配置管理_文件";									//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗 [BTS]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"厂家",LVCFMT_CENTER,80);					//
	m_cList.InsertColumn(2,"文件名称",LVCFMT_LEFT,120);					//
	m_cList.InsertColumn(3,"中文解释",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(4,"子表名称",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(5,"子表序号",LVCFMT_CENTER,80);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	m_cList.SetNumber(5);

	//信息初始化
	m_cKey_Select.AddString("华为");
	m_cKey_Select.AddString("中兴");
	m_cKey_Select.SetCurSel(0);											//默认：选择第一个

	//信息初始化
	v_iList_Item=-1;													//条目选择无效

	//查询
	v_iWorking=1;														//工作任务：查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CCM_LTE_File::PreTranslateMessage(MSG* pMsg)
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
void CCM_LTE_File::OnDestroy()
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
void CCM_LTE_File::OnSize(UINT nType, int cx, int cy)
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
void CCM_LTE_File::OnBnClickedExcel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show(&m_cList);												//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表窗排序
//------------------------------------------------------------------------------------------------------
void CCM_LTE_File::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList.My_Sort(pNMHDR);											//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：列表点击响应
//------------------------------------------------------------------------------------------------------
void CCM_LTE_File::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
void CCM_LTE_File::OnBnClickedQuary2()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：查询

	v_pIterator_Frame->second.v_iThread_Exit=1;							//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CCM_LTE_File::My_Thread_Inoput(LPVOID lparam)
{
	CCM_LTE_File *  lp_this = NULL ;
	lp_this = (CCM_LTE_File *)lparam;

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
void CCM_LTE_File::My_Input_Main()
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
void CCM_LTE_File::My_LoadData_Per()
{
	int				v_iItem,v_iID=0,v_iNum;								//临时变量
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
		v_sSql += "SELECT * FROM CON_Cm_LTE_File ";
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
		v_sSql+=" order by zFile_Name,zNum";

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

			//显示
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,v_sMaker);					//厂家
			m_cList.SetItemText(v_iItem,2,v_sFile_Name);				//文件名称
			m_cList.SetItemText(v_iItem,3,v_sFile_Demo);				//中文解释
			m_cList.SetItemText(v_iItem,4,v_sNum_Demo);					//子表名称
			v_sNum.Format("%d",v_iNum);									//字符串
			m_cList.SetItemText(v_iItem,5,v_sNum);						//子表序号

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
//	END
//------------------------------------------------------------------------------------------------------
