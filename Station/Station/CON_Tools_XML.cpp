// CON_Tools_XML.cpp : 实现文件
//
#include "stdafx.h"
#include "Station.h"
#include "CON_Tools_XML.h"

#define _CRTDBG_MAP_ALLOC 
#include<stdlib.h> 
#include<crtdbg.h> 

// CCON_Tools_XML

IMPLEMENT_DYNCREATE(CCON_Tools_XML, CFormView)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CCON_Tools_XML::CCON_Tools_XML()
	: CFormView(CCON_Tools_XML::IDD)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CCON_Tools_XML::~CCON_Tools_XML()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST6, m_cList_ObjType);
	DDX_Control(pDX, IDC_LIST7, m_cList_Child);
	DDX_Control(pDX, IDC_EDIT1, m_cFile_Name);
	DDX_Control(pDX, IDC_Open_File, m_cOpen_File);
	DDX_Control(pDX, IDC_COMBO3, m_cType);
	DDX_Control(pDX, IDC_SUM_ALARM, m_cSum);
	DDX_Control(pDX, IDC_BUTTON4, m_cFile);
	DDX_Control(pDX, IDC_LIST8, m_cList_Object);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_Tools_XML, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON4, &CCON_Tools_XML::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CCON_Tools_XML::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCON_Tools_XML::OnBnClickedButton2)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST6, &CCON_Tools_XML::OnLvnColumnclickList6)
	ON_NOTIFY(NM_CLICK, IDC_LIST6, &CCON_Tools_XML::OnNMClickList6)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST4, &CCON_Tools_XML::OnLvnColumnclickList4)
	ON_NOTIFY(NM_CLICK, IDC_LIST7, &CCON_Tools_XML::OnNMClickList7)
	ON_BN_CLICKED(IDC_BUTTON5, &CCON_Tools_XML::OnBnClickedButton5)
END_MESSAGE_MAP()


// CCON_Tools_XML 诊断

#ifdef _DEBUG
void CCON_Tools_XML::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCON_Tools_XML::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCON_Tools_XML 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="XML文件工具";										//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针

	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置

	//初始化：列表窗ObjType
	::SendMessage(m_cList_ObjType.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_ObjType.InsertColumn(0,"序号",LVCFMT_CENTER,40);
	m_cList_ObjType.InsertColumn(1,"ObjType",LVCFMT_LEFT,280);  
	m_cList_ObjType.SetExtendedStyle(m_cList_ObjType.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	//初始化：列表窗Child
	::SendMessage(m_cList_Child.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Child.InsertColumn(0,"序号",LVCFMT_CENTER,40);
	m_cList_Child.InsertColumn(1,"Counter",LVCFMT_LEFT,280);  
	m_cList_Child.SetExtendedStyle(m_cList_Child.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	//初始化：列表窗Object
	::SendMessage(m_cList_Object.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Object.InsertColumn(0,"序号",LVCFMT_CENTER,40);
	m_cList_Object.InsertColumn(1,"Counter",LVCFMT_LEFT,280);  
	m_cList_Object.SetExtendedStyle(m_cList_Object.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//分窗
	CRect	v_cRect;													//矩形
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//获得：指针
	pWnd->GetWindowRect(&v_cRect);										//获得：尺寸
	ScreenToClient(&v_cRect);											//坐标变换
	pWnd->DestroyWindow();												//销毁：指针

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Right.Create(											//水平线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList_ObjType,		// Top pane
		&m_cList_Child,			// Bottom pane
		IDC_Tools_XML_2,		// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);
	m_SplitterPane_Right.m_bVertSplitter_Parent=true;					//垂直分隔符为父窗
	m_SplitterPane_Main.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT, 		 
		this,					// the parent of the splitter pane
		&m_SplitterPane_Right,	// Top pane
		&m_cList_Object,		// Bottom pane
		IDC_Tools_XML_1,		// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);

	PostMessage(WM_SIZE);												//最大化

	//变量设置
	m_cOpen_File.SetCheck(true);										//导出打开复选钮：有效
	m_cType.AddString("配置");											//文件类型：配置
	m_cType.AddString("性能");											//文件类型：性能
	m_cType.AddString("性能_直读");										//文件类型：性能【直接读取】
	m_cType.SetCurSel(0);												//默认类型：配置

	v_bRead=false;														//数据类型：0：配置、性能；
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CCON_Tools_XML::PreTranslateMessage(MSG* pMsg)
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
void CCON_Tools_XML::OnDestroy()
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
void CCON_Tools_XML::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//分窗尺寸调整
		CRect v_cRect;
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=45;
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取XML文件信息
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	int			v_iVi;													//临时变量
	CString		v_sTemp;												//临时变量
	map_ObjType::iterator		v_pObjType;								//迭代器：v_mObjType
	map_ObjType_Child::iterator v_pObjType_Child;						//迭代器：v_mObjType_Child

	CString		v_sFilename;											//文件名称
	CFileDialog *filedialog;											//选择文件对话窗指针

	filedialog = new CFileDialog(true,(LPCTSTR)"bak");					//实例化
	if(filedialog->DoModal()==IDOK)										//打开选择文件对话窗
	{
		v_sFilename = filedialog->GetPathName();						//获得：选择的文件名称
		m_cFile_Name.SetWindowText(v_sFilename);						//呈现：选中的文件名称
		//读取XML文件内容：[ObjType/Object]
		switch(m_cType.GetCurSel())
		{
		case 0:															//配置文件
			if (!v_cFile_XML.Read_XML(v_sFilename.GetBuffer(0)))		//获取：XML文件信息[ObjType/Object]
				MessageBox("无有效的XML文件，请重新选择 ！！！");		//窗体提示	
			break;
		case 1:															//性能文件
			if (!v_cFile_XML.Read_XML_PM(v_sFilename.GetBuffer(0)))		//获取：XML文件信息[ObjType/Object]
				MessageBox("无有效的XML文件，请重新选择 ！！！");		//窗体提示	
			break;
		case 2:															//性能文件【直接读取】
			if (!v_cFile_XML.Read_XML_PM_Dir(v_sFilename))				//获取：XML文件信息
				MessageBox("无有效的XML文件，请重新选择 ！！！");		//窗体提示	
			break;
		}

		//清除显示列表信息
		m_cList_ObjType.DeleteAllItems();								//删除所有列表条目
		m_cList_Child.DeleteAllItems();									//删除所有列表条目

		switch(m_cType.GetCurSel())
		{
		case 0:															//配置文件
		case 1:															//性能文件
			//显示：ObjType
			for(v_pObjType=v_cFile_XML.v_mObjType.begin();v_pObjType!=v_cFile_XML.v_mObjType.end();v_pObjType++)
			{
				v_iVi = m_cList_ObjType.InsertItem(0xffffff,_T("0"));	//添加一行
				v_sTemp.Format("%04d",v_iVi+1);							//获得：字符串
				m_cList_ObjType.SetItemText(v_iVi,0,v_sTemp);			//第0列：序号
				m_cList_ObjType.SetItemText(v_iVi,1,(_bstr_t)v_pObjType->first);	//ObjType
			}
			v_bRead=false;												//数据类型：0：配置、性能；
			break;
		case 2:															//性能文件【直接读取】
			//显示：ObjType_Child
			for(v_pObjType_Child=v_cFile_XML.v_mObjType_Child.begin();v_pObjType_Child!=v_cFile_XML.v_mObjType_Child.end();v_pObjType_Child++)
			{
				//显示：ObjType
				v_iVi = m_cList_ObjType.InsertItem(0xffffff,_T("0"));	//添加一行
				v_sTemp.Format("%04d",v_iVi+1);							//获得：字符串
				m_cList_ObjType.SetItemText(v_iVi,0,v_sTemp);			//第0列：序号
				m_cList_ObjType.SetItemText(v_iVi,1,(_bstr_t)v_pObjType_Child->first);	//ObjType
			}
			v_bRead=true;												//数据类型：1：性能直读；
			break;
		}

		//复位
		v_iItem_ObjType=-1;												//ObjType列表条目选择无效[-1]
		v_iItem_ObjType_Child=-1;										//ObjType_Child列表条目选择无效[-1]
		m_cList_Object.DeleteAllItems();								//删除所有条目
	}
	delete(filedialog);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：ObjType导出
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show_Name(&m_cList_ObjType,"ObjType",m_cOpen_File.GetCheck());	//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Object导出
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	v_sName_ObjType.Replace("/","_");									//非法文件名称字符替换
	MyExcel.Show_Name(&m_cList_Object,v_sName_ObjType,m_cOpen_File.GetCheck());	//导出Excel：指定文件名称；
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：ObjType列表排序(右击列表任意列)
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnLvnColumnclickList6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList_ObjType.My_Sort(pNMHDR);									//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Object列表排序(右击列表任意列)
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnLvnColumnclickList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_cList_Object.My_Sort(pNMHDR);										//列表排序

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击ObjType列表
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnNMClickList6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if(v_iItem_ObjType>=0)											//有效条目序号？？？
			m_cList_ObjType.SetItemBkColor(v_iItem_ObjType,RGB(255,255,255));		//恢复背景色
		v_iItem_ObjType=pNMListView->iItem;								//获得：选中条目序号
		m_cList_ObjType.SetItemBkColor(v_iItem_ObjType,RGB(255,0,0));	//设置背景色
		v_sName_ObjType=m_cList_ObjType.GetItemText(v_iItem_ObjType,1);	//获得：选中条目名称

		v_iWorking=0;													//ObjType
		v_pIterator_Frame->second.v_iThread_Exit=1;						//线程：工作
		v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);			//开启查询数据库线程
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击ObjType_Child列表
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnNMClickList7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if(v_iItem_ObjType_Child>=0)									//有效条目序号？？？
			m_cList_Child.SetItemBkColor(v_iItem_ObjType_Child,RGB(255,255,255));		//恢复背景色
		v_iItem_ObjType_Child=pNMListView->iItem;						//获得：选中条目序号
		m_cList_Child.SetItemBkColor(v_iItem_ObjType_Child,RGB(255,0,0));	//设置背景色
		v_sName_Child=m_cList_Child.GetItemText(v_iItem_ObjType_Child,1);//获得：选中条目名称

		v_iWorking=1;													//ObjType_Child
		v_pIterator_Frame->second.v_iThread_Exit=1;						//线程：工作
		v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);			//开启查询数据库线程
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：显示子项列表
//------------------------------------------------------------------------------------------------------
void CCON_Tools_XML::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	int	v_iVi=0,v_iNum;													//临时变量
	CString	v_sTemp;													//临时变量
	map_ObjType_Child::iterator		v_pObjType;							//迭代器：map_ObjType_Child
	map_Child::iterator				v_pChild;							//迭代器：map_Child
	map_C_Data::iterator			v_pChild_Data;						//迭代器：map_Child_Data
	mString_String::iterator		v_pCounter;							//迭代器：map_Counter

	//【1】初始化：列信息
	m_cList_Object.DeleteAllItems();									//删除所有条目
	while(m_cList_Object.DeleteColumn(0));								//删除所有列：ObjType信息列表

	//【2】在ObjType信息列表添加信息
	m_cList_Object.InsertColumn(0,"序号",LVCFMT_CENTER,50);				//第0列：序号
	m_cList_Object.InsertColumn(1,"Counter",LVCFMT_LEFT,100);			//第1列：Counter
	//添加列表：列名称
	for (unsigned int vi=1;vi<=v_iChild_Col_Max;vi++)
	{
		v_sTemp.Format("列_%03d",vi);									//字符串		
		m_cList_Object.InsertColumn(vi+1,v_sTemp,LVCFMT_RIGHT,60);		//添加列名称【列序号】
	}
	m_cList_Object.SetRedraw(false);									//禁止闪烁

	//【3】显示：子项信息【添加列表：行信息】
	m_cList_Object.SetRedraw(false);									//禁止闪烁

	//获得：ObjType
	v_pObjType=v_cFile_XML.v_mObjType_Child.find(v_sName_ObjType.GetBuffer(0));		//查询：v_sObjType
	if (v_pObjType != v_cFile_XML.v_mObjType_Child.end())
	{
		v_pChild=v_pObjType->second.v_mChild.begin();					//获取：首地址
		while(v_pChild!=v_pObjType->second.v_mChild.end())				//遍历
		{
			v_iVi = m_cList_Object.InsertItem(0xffffff,_T("0"));		//添加一行
			v_sTemp.Format("%06d",v_iVi+1);								//获得：字符串
			m_cList_Object.SetItemText(v_iVi,0,v_sTemp);				//第0列：序号
			m_cList_Object.SetItemText(v_iVi,1,v_pChild->first);		//第1列：Counter

			//添加子项信息
			v_iNum=2;													//从第2列起添加
			v_pChild_Data=v_pChild->second.begin();						//获得：map_Child_Data迭代器起始位置
			v_pCounter=v_pChild_Data->second.begin();					//获得：Counter迭代器起始位置
			while(v_pCounter != v_pChild_Data->second.end())			//条件：Counter迭代器终止位置
			{       
				m_cList_Object.SetItemText(v_iVi,v_iNum,v_pCounter->first);	//第1列：v_sObject
				v_pCounter++;													//迭代器++
				v_iNum++;												//列号++
			}

			v_pChild++;													//迭代器++
		}
	}
	m_cList_Object.SetRedraw(true);										//列表：显示

	v_sTemp.Format("%d",v_iVi+1);										//
	m_cSum.SetWindowText(v_sTemp);										//显示：总数
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CCON_Tools_XML::My_Thread_Inoput(LPVOID lparam)
{
	CCON_Tools_XML *  lp_this = NULL ;
	lp_this = (CCON_Tools_XML *)lparam;

	lp_this->m_cFile.EnableWindow(false);								//文件选择：无效
	lp_this->m_cList_ObjType.EnableWindow(false);						//列表：无效

	if (lp_this->v_iWorking==0)
		lp_this->My_Show_Object();										//导入_主程序【Object】
	else if (lp_this->v_iWorking==1)
		lp_this->My_Show_Object_Child();								//导入_主程序【Object_Child】

	lp_this->m_cFile.EnableWindow(true);								//文件选择：有效
	lp_this->m_cList_ObjType.EnableWindow(true);						//列表：有效

	lp_this->v_pIterator_Frame->second.v_iThread_Exit=0;				//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：显示Object信息
//------------------------------------------------------------------------------------------------------			
void CCON_Tools_XML::My_Show_Object()
{
	int			v_iVi;													//临时变量
	CString		v_sTemp;												//临时变量

	map_ObjType::iterator		v_pObjType;								//迭代器：ObjType
	map_ObjType_Child::iterator	v_pObjType_Child;						//迭代器：ObjType_Child
	map_Child::iterator			v_pChild;								//迭代器：map_Child
	map_C_Data::iterator		v_pChild_Data;							//迭代器：map_Child_Data

	if (!v_bRead)														//标志：配置、性能
	{
		//查询：ObjType
		v_pObjType=v_cFile_XML.v_mObjType.find(v_sName_ObjType.GetBuffer(0));		//查询：v_sObjType
		if (v_pObjType != v_cFile_XML.v_mObjType.end())
		{
			My_Show_Object_Data(&v_pObjType->second);					//显示：Object_Date信息
		}
	}
	else																//标志：性能直读
	{
		//查询：ObjType
		v_pObjType_Child=v_cFile_XML.v_mObjType_Child.find(v_sName_ObjType.GetBuffer(0));		//查询：v_sObjType
		if (v_pObjType_Child != v_cFile_XML.v_mObjType_Child.end())
		{
			v_iChild_Col_Max=0;											//子项最大列序号
			//显示：Child
			m_cList_Child.DeleteAllItems();								//删除所有列表条目
			v_iItem_ObjType_Child=-1;									//ObjType_Child列表条目选择无效[-1]
			for(v_pChild=v_pObjType_Child->second.v_mChild.begin();v_pChild!=v_pObjType_Child->second.v_mChild.end();v_pChild++)
			{
				v_iVi = m_cList_Child.InsertItem(0xffffff,_T("0"));		//添加一行
				v_sTemp.Format("%04d",v_iVi+1);							//获得：字符串
				m_cList_Child.SetItemText(v_iVi,0,v_sTemp);				//第0列：序号
				m_cList_Child.SetItemText(v_iVi,1,(_bstr_t)v_pChild->first);		//ObjType_Child

				//获取：子项最大列序号
				v_pChild_Data=v_pChild->second.begin();					//首指针
				if (v_pChild_Data->second.size()>v_iChild_Col_Max)
					v_iChild_Col_Max=v_pChild_Data->second.size();		//获取：最大值
			}

			//显示：Object
			My_Show_Object_Data(&v_pObjType_Child->second.v_lObject);	//显示：Object_Date信息
		}
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：显示Object_Date信息
//------------------------------------------------------------------------------------------------------			
void CCON_Tools_XML::My_Show_Object_Data(list_Object* v_pObject)
{
	int	v_iVi,v_iV_Num;													//临时变量
	CString	v_sTemp;													//临时变量

	list_Object::iterator		v_pIterator_Object;						//list_Object迭代器
	mString_String::iterator	v_pIterator_Counter;					//map_Counter迭代器

	m_cList_Object.DeleteAllItems();									//删除所有条目
	while(m_cList_Object.DeleteColumn(0));								//删除所有列：ObjType信息列表
	//在ObjType信息列表添加信息
	m_cList_Object.InsertColumn(0,"序号",LVCFMT_CENTER,50);				//第0列：序号
	m_cList_Object.InsertColumn(1,"Object",LVCFMT_LEFT,100);			//第1列：Object
	m_cList_Object.InsertColumn(2,"Dn",LVCFMT_LEFT,240);				//第2列：Object
	//添加信息
	if(!v_pObject->empty())												//ObjType中有Object？[即：Object数量为空？？？]
	{
		//添加列表：列名称，即Counter名称；
		v_iVi=3;														//从第3列起添加
		v_pIterator_Counter=v_pObject->begin()->v_mCounter.begin();		//获得：Counter迭代器起始位置
		while(v_pIterator_Counter != v_pObject->begin()->v_mCounter.end())	//条件：Counter迭代器终止位置
		{       
			m_cList_Object.InsertColumn(v_iVi,v_pIterator_Counter->first,LVCFMT_RIGHT,60);	//添加列名称：即Counter名称
			v_pIterator_Counter++;										//迭代器++
			v_iVi++;													//列号++
		}
		m_cList_Object.SetRedraw(false);								//禁止闪烁
		//添加列表：行信息，即Object信息
		v_pIterator_Object=v_pObject->begin();							//获得：Object迭代器起始位置
		while(v_pIterator_Object != v_pObject->end())					//条件：Object迭代器终止位置
		{       
			if (v_pIterator_Frame->second.v_iThread_Exit==2)			//强制退出？？？
				break;													//退出

			v_iVi = m_cList_Object.InsertItem(0xffffff,_T("0"));		//添加一行
			v_sTemp.Format("%06d",v_iVi+1);								//获得：字符串
			m_cList_Object.SetItemText(v_iVi,0,v_sTemp);				//第1列：序号
			m_cList_Object.SetItemText(v_iVi,1,v_pIterator_Object->v_sObject_ID);	//第2列：v_sObject
			m_cList_Object.SetItemText(v_iVi,2,v_pIterator_Object->v_sObject_DN);	//第3列：Dn
			v_iV_Num=3;													//Counter数据从第2列起
			v_pIterator_Counter=v_pIterator_Object->v_mCounter.begin();				//获得：Counter迭代器起始位置
			while(v_pIterator_Counter != v_pIterator_Object->v_mCounter.end())		//条件：Counter迭代器终止位置
			{  
				v_sTemp=v_pIterator_Counter->second;					//替换
				v_sTemp.Replace(",",";");								//","换为";"
				v_sTemp.Replace("\n","");								//去掉换行
				m_cList_Object.SetItemText(v_iVi,v_iV_Num,v_sTemp);		//添加：Object列表
				v_pIterator_Counter++;									//迭代器++
				v_iV_Num++;												//列号++
			}
			v_pIterator_Object++;

			if ((v_iVi+1)%1000==0)										//显示：100的整数
			{
				v_sTemp.Format("%d",v_iVi+1);							//
				m_cSum.SetWindowText(v_sTemp);							//显示：总数
			}
		}
		m_cList_Object.SetRedraw(true);									//列表：显示

		v_sTemp.Format("%d",v_iVi+1);									//
		m_cSum.SetWindowText(v_sTemp);									//显示：总数
	}	
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：显示Object_Child信息
//------------------------------------------------------------------------------------------------------			
void CCON_Tools_XML::My_Show_Object_Child()
{
	int	v_iVi,v_iV_Num;													//临时变量
	CString	v_sTemp;													//临时变量
	map_ObjType_Child::iterator		v_pObjType;							//迭代器：map_ObjType_Child
	map_Child::iterator				v_pChild;							//迭代器：map_Child
	map_C_Data::iterator			v_pIterator_Object;					//迭代器：map_Object
	mString_String::iterator		v_pIterator_Counter;				//迭代器：map_Counter

	//获得：ObjType
	v_pObjType=v_cFile_XML.v_mObjType_Child.find(v_sName_ObjType.GetBuffer(0));		//查询：v_sObjType
	if (v_pObjType == v_cFile_XML.v_mObjType_Child.end())
		return;															//无效返回
	v_pChild=v_pObjType->second.v_mChild.find(v_sName_Child.GetBuffer(0));			//查询：v_sName_Child
	if (v_pChild != v_pObjType->second.v_mChild.end())
	{
		m_cList_Object.DeleteAllItems();								//删除所有条目
		while(m_cList_Object.DeleteColumn(0));							//删除所有列：ObjType信息列表
		//在ObjType信息列表添加信息
		m_cList_Object.InsertColumn(0,"序号",LVCFMT_CENTER,50);			//第1列：序号
		m_cList_Object.InsertColumn(1,"Object",LVCFMT_LEFT,100);		//第2列：Object
		//添加信息
		if(v_pChild->second.size()>0)									//ObjType中有Object？[即：Object数量为空？？？]
		{
			//添加列表：列名称，即Counter名称；
			v_iVi=2;													//从第2列起添加
			v_pIterator_Object=v_pChild->second.begin();				//迭代器：第一个小区
			v_pIterator_Counter=v_pIterator_Object->second.begin();			//获得：Counter迭代器起始位置
			while(v_pIterator_Counter != v_pIterator_Object->second.end())	//条件：Counter迭代器终止位置
			{       
				m_cList_Object.InsertColumn(v_iVi,v_pIterator_Counter->first,LVCFMT_RIGHT,60);		//添加列名称：即Counter名称
				v_pIterator_Counter++;																//迭代器++
				v_iVi++;												//列号++
			}
			m_cList_Object.SetRedraw(false);							//禁止闪烁
			//添加列表：行信息，即Object信息
			v_pIterator_Object=v_pChild->second.begin();				//获得：Object迭代器起始位置
			while(v_pIterator_Object != v_pChild->second.end())			//条件：Object迭代器终止位置
			{       
				if (v_pIterator_Frame->second.v_iThread_Exit==2)		//强制退出？？？
					break;												//退出

				v_iVi = m_cList_Object.InsertItem(0xffffff,_T("0"));	//添加一行
				v_sTemp.Format("%06d",v_iVi+1);							//获得：字符串
				m_cList_Object.SetItemText(v_iVi,0,v_sTemp);			//第0列：序号
				m_cList_Object.SetItemText(v_iVi,1,v_pIterator_Object->first);				//第1列：v_sObject
				v_iV_Num=2;												//Counter数据从第2列起
				v_pIterator_Counter=v_pIterator_Object->second.begin();				//获得：Counter迭代器起始位置
				while(v_pIterator_Counter != v_pIterator_Object->second.end())		//条件：Counter迭代器终止位置
				{  
					v_sTemp=v_pIterator_Counter->second;				//替换
					v_sTemp.Replace(",",";");							//","换为";"
					v_sTemp.Replace("\n","");							//去掉换行
					m_cList_Object.SetItemText(v_iVi,v_iV_Num,v_sTemp);	//添加：Object列表
					v_pIterator_Counter++;								//迭代器++
					v_iV_Num++;											//列号++
				}
				v_pIterator_Object++;

				if ((v_iVi+1)%1000==0)									//显示：100的整数
				{
					v_sTemp.Format("%d",v_iVi+1);						//
					m_cSum.SetWindowText(v_sTemp);						//显示：总数
				}
			}
			m_cList_Object.SetRedraw(true);								//列表：显示

			v_sTemp.Format("%d",v_iVi+1);								//
			m_cSum.SetWindowText(v_sTemp);								//显示：总数
		}	
	}
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
