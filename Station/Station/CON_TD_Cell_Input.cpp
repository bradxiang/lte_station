// CON_TD_Cell_Input.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "CON_TD_Cell_Input.h"


// CCON_TD_Cell_Input 对话框

IMPLEMENT_DYNAMIC(CCON_TD_Cell_Input, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CCON_TD_Cell_Input::CCON_TD_Cell_Input(CWnd* pParent /*=NULL*/)
	: CDialog(CCON_TD_Cell_Input::IDD, pParent)
{
	v_iThread_Exit=0;													//线程：停止
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CCON_TD_Cell_Input::~CCON_TD_Cell_Input()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CCON_TD_Cell_Input::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cFile_Name);
	DDX_Control(pDX, IDC_BUTTON4, m_cFile);
	DDX_Control(pDX, IDC_BUTTON1, m_cInput);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Control(pDX, IDC_LIST3, m_cList_Bug);
	DDX_Control(pDX, IDC_STATION_SUM, m_cSum);
	DDX_Control(pDX, IDC_BUTTON6, m_cExcel);
	DDX_Control(pDX, IDC_CHECK2, m_cFile_Open);
	DDX_Control(pDX, IDC_CHECK3, m_cOnline);
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_TD_Cell_Input, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON4, &CCON_TD_Cell_Input::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CCON_TD_Cell_Input::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON6, &CCON_TD_Cell_Input::OnBnClickedButton6)
END_MESSAGE_MAP()


// CCON_TD_Cell_Input 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CCON_TD_Cell_Input::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化：列表窗 [小区]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"小区",LVCFMT_LEFT,200);						//
	m_cList.InsertColumn(2,"E_NodeB",LVCFMT_LEFT,200);					//
	m_cList.InsertColumn(3,"LAC",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(4,"区域",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(5,"所属机房/资源点",LVCFMT_LEFT,260);			//
	m_cList.InsertColumn(6,"覆盖类型",LVCFMT_LEFT,70);					//
	m_cList.InsertColumn(7,"状态",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(8,"入网日期",LVCFMT_CENTER,120);				//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//初始化：列表窗 [异常]
	::SendMessage(m_cList_Bug.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_cList_Bug.InsertColumn(0,(LPCTSTR)"序号",LVCFMT_CENTER,40);		//为列表控件添加列
	m_cList_Bug.InsertColumn(1,(LPCTSTR)"时间",LVCFMT_LEFT,140);
	m_cList_Bug.InsertColumn(2,(LPCTSTR)"异常类型",LVCFMT_LEFT,100);
	m_cList_Bug.InsertColumn(3,(LPCTSTR)"异常信息",LVCFMT_LEFT,600);
	m_cList_Bug.SetExtendedStyle(m_cList_Bug.GetExtendedStyle()|LVS_EX_GRIDLINES);

	//分窗
	CRect	v_cRect;													//矩形
	CWnd*	pWnd = GetDlgItem(IDC_STATIC_SLIT_PANE);					//获得：指针
	pWnd->GetWindowRect(&v_cRect);										//获得：尺寸
	ScreenToClient(&v_cRect);											//坐标变换
	pWnd->DestroyWindow();												//销毁：指针

	//	this creates pane containing list control on the top and edit on the bottom
	m_SplitterPane_Main.Create(											//垂直线
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_HORIZ, 		 
		this,					// the parent of the splitter pane
		&m_cList,				// Top pane
		&m_cList_Bug,			// Bottom pane
		IDC_CON_TD_Cell_Input,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);

	//显示
	ShowWindow(SW_MAXIMIZE);											//对话窗最大化

//	m_cOnline.SetCheck(true);											//有效：在网

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CCON_TD_Cell_Input::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		CDialog* Pwnd=(CDialog*)GetActiveWindow();						//取得对话框指针 
		Pwnd->NextDlgCtrl();											//切换到下一个输入焦点 
		return true;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：调整窗体大小
//------------------------------------------------------------------------------------------------------
void CCON_TD_Cell_Input::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect	v_cRect;

	if ( m_SplitterPane_Main.GetSafeHwnd() ){							//分窗尺寸调整
		GetClientRect( &v_cRect );
		v_cRect.DeflateRect(10,10);
		v_cRect.top+=60;
		m_SplitterPane_Main.MoveWindow(&v_cRect);
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：关闭窗体
//------------------------------------------------------------------------------------------------------
void CCON_TD_Cell_Input::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (v_iThread_Exit>0)												//线程运行？？？
	{
		v_iThread_Exit=2;												//强制退出
		return;															//返回
	}

	CDialog::OnClose();													//退出
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：异常导出
//------------------------------------------------------------------------------------------------------
void CCON_TD_Cell_Input::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show_Name(&m_cList_Bug,"导入异常_TD_Cell",m_cFile_Open.GetCheck());		//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取资管数据CSV文件信息
//------------------------------------------------------------------------------------------------------
void CCON_TD_Cell_Input::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=0;														//工作任务：华为文件选择

	v_iThread_Exit=1;													//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：把列表窗中的数据保存到数据表
//------------------------------------------------------------------------------------------------------
void CCON_TD_Cell_Input::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：华为数据导入

	v_iThread_Exit=1;													//线程开启：有效
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//导入
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动	[静态门限计算]
//------------------------------------------------------------------------------------------------------
UINT CCON_TD_Cell_Input::My_Thread_Inoput(LPVOID lparam)
{
	CCON_TD_Cell_Input *  lp_this = NULL ;
	lp_this = (CCON_TD_Cell_Input *)lparam;

	lp_this->m_cFile.EnableWindow(false);								//文件选择：无效
	lp_this->m_cInput.EnableWindow(false);								//开始导入：无效
	lp_this->m_cExcel.EnableWindow(false);								//开始导出：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cFile.EnableWindow(true);								//文件选择：有效
	lp_this->m_cInput.EnableWindow(true);								//开始导入：有效
	lp_this->m_cExcel.EnableWindow(true);								//开始导出：有效

	lp_this->v_iThread_Exit=0;											//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CCON_TD_Cell_Input::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 0:
		My_File();														//文件选择
		break;
	case 1:
		My_Input_Cell();												//Cell
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取CSV文件信息
//------------------------------------------------------------------------------------------------------
void CCON_TD_Cell_Input::My_File()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sTemp;												//临时变量
	mString_Int	v_mMap;													//容器：CSV文件列序号映射显示列序号

	CFileDialog *filedialog;											//选择文件对话窗指针

	filedialog = new CFileDialog(true,(LPCTSTR)"csv");					//实例化
	if(filedialog->DoModal()==IDOK)										//打开选择文件对话窗
	{
		v_sFilename = filedialog->GetPathName();						//获得：选择的文件名称
		m_cFile_Name.SetWindowText(v_sFilename);						//呈现：选中的文件名称

		m_cList.DeleteAllItems();										//删除所有条目
		m_cList_Bug.DeleteAllItems();									//删除所有条目

		//容器：文件列名称对应的显示列序号【文件列名称--显示列序号】
		v_mMap.insert(pair<CString,int>("UTRANCELL名称",1));			//1：Cell
		v_mMap.insert(pair<CString,int>("所属NODEB",2));				//2：NODEB
		v_mMap.insert(pair<CString,int>("位置区编码LAC",3));			//3：LAC
		v_mMap.insert(pair<CString,int>("区/县名称",4));				//4：区域
		v_mMap.insert(pair<CString,int>("所属机房/资源点",5));			//5：所属机房/资源点
		v_mMap.insert(pair<CString,int>("覆盖类型",6));					//6：覆盖类型
		v_mMap.insert(pair<CString,int>("生命周期状态",7));				//7：状态
		v_mMap.insert(pair<CString,int>("入网时间",8));					//8：入网日期
		if (!Get_CSV_Cell(v_sFilename,&v_mMap,true))					//获取：CSV文件信息
			MessageBox("无有效的Cell文件，请重新选择 ！！！");			//窗体提示	
	}
	delete(filedialog);
	v_mMap.clear();														//清除容器
}

//----------------------------------------------------------------------------------------------------
//	函数功能：导入CSV文件到 m_cList 控件中
//	参    数：CSV文件路径，	v_mMap：列映射关系，	v_bHead：文件带表头标记；
//----------------------------------------------------------------------------------------------------
bool CCON_TD_Cell_Input::Get_CSV_Cell(CString v_sFile,mString_Int *v_mHead,bool v_bHead)
{
	CStdioFile	v_fRead;												//实例化	
	CString		v_sStr,v_sTemp;											//临时变量
	bool		v_Flag_Head=true;										//临时变量
	int			v_iLen,v_iNum,v_iItem,v_iSum;							//临时变量
	CString		v_sType,v_sOnline,v_sName;								//临时变量

	mString_Int::iterator	v_pCol_Head;								//迭代器
	mInt_Int				v_mMap;										//容器
	mInt_Int::iterator		v_pCol;										//迭代器
	mInt_String				v_mData;									//容器：列信息存放
	mInt_String::iterator	v_pIter;									//迭代器
	mString_Int				v_mRepeat;									//容器：重复信息检查
	mString_Int::iterator	v_pRepeat;									//迭代器

	if (!v_fRead.Open(_T(v_sFile),CFile::modeRead))						//打开文件
	{
		MessageBoxA("异常：文件打开失败！！！", "提示", MB_OK);			//提示信息
		return false;													//返回：失败
	}

	v_iSum=1;															//行序号
	while(v_fRead.ReadString(v_sStr))									//数据读取：一行
	{
		if (v_iThread_Exit==2)											//强制退出？？？
			break;														//退出

		v_sStr.Replace("\"","");										//去掉"
		if (v_bHead && v_Flag_Head)										//带表头？
		{
			v_Flag_Head=false;											//带表头：忽略掉
			//表头处理：表头名称==》表头序号
			v_iNum=1;													//列号
			while (true)
			{
				if (v_sStr.IsEmpty())									//为空？？？
					break;												//退出
				v_iLen=v_sStr.Find(",",0);								//查找","
				if (v_iLen<0)											//找到？？？
				{
					v_sTemp=v_sStr;										//获取：有效信息
					v_sStr="";											//计算：下一个信息位置
				}
				else
				{
					v_sTemp=v_sStr.Left(v_iLen);						//获取：有效信息
					v_sStr=v_sStr.Mid(v_iLen+1);						//计算：下一个信息位置
				}
				v_pCol_Head = v_mHead->find(v_sTemp);					//查找表头名称？？？
				if (v_pCol_Head != v_mHead->end())						//找到？？？
					v_mMap.insert(pair<int,int>(v_iNum,v_pCol_Head->second));	//列：1、2、3...
				v_iNum++;												//列号：++
			}
			continue;													//
		}
		//一行内容处理
		v_sType="";														//类型
		v_sOnline="";													//状态
		v_iNum=1;														//列号
		while (true)
		{
			if (v_sStr.IsEmpty())										//为空？？？
				break;													//退出
			v_iLen=v_sStr.Find(",",0);									//查找","
			if (v_iLen<0)												//找到？？？
			{
				v_sTemp=v_sStr;											//获取：有效信息
				v_sStr="";												//计算：下一个信息位置
			}
			else
			{
				v_sTemp=v_sStr.Left(v_iLen);							//获取：有效信息
				v_sStr=v_sStr.Mid(v_iLen+1);							//计算：下一个信息位置
			}
			v_pCol=v_mMap.find(v_iNum);									//查找列号？？？
			if (v_pCol != v_mMap.end())									//找到？？？
			{
				v_mData.insert(pair<int,CString>(v_pCol->second,v_sTemp));		//列：1、2、3...
				if (v_pCol->second==1)									//Cell
					v_sName=v_sTemp;									//保存：在线
			}
			v_iNum++;													//列号：++
		}
		if (!v_sName.IsEmpty())											//条件：数据有效？？？
		{
			v_pRepeat=v_mRepeat.find(v_sName);							//机房存在？？？
			if (v_pRepeat == v_mRepeat.end())							//机房：不存在
			{
				v_mRepeat.insert(pair<CString,int>(v_sName,0));			//容器：增加
				v_sTemp.Format("%06d",v_iSum);							//行号
				v_iItem=m_cList.InsertItem(0xffff,v_sTemp);				//行号
				v_pIter=v_mData.begin();								//容器：头
				while(v_pIter!=v_mData.end())							//容器：尾？？？
				{
					m_cList.SetItemText(v_iItem,v_pIter->first,v_pIter->second);	//显示					
					v_pIter++;											//迭代器++
				}
				if (v_iSum%500==0)
				{
					v_sTemp.Format("%d",v_iSum);						//总数：行
					m_cSum.SetWindowText(v_sTemp);						//总数：行
				}
				v_iSum++;												//序号++
			}
			else														//机房名称重复
			{
				My_ShowList_Bug("Cell异常","异常：Cell名称重复 【Cell名称："+v_sName+"】");		//显示：异常信息
			}
		}
		v_mData.clear();												//容器：清除
	}
	v_fRead.Close();													//关闭文件
	v_mRepeat.clear();													//容器：清除
	v_mMap.clear();														//容器：清除
	v_sTemp.Format("%d",m_cList.GetItemCount());						//总数：行
	m_cSum.SetWindowText(v_sTemp);										//总数：行
	return true;														//返回：成功
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Cell数据导入
//------------------------------------------------------------------------------------------------------
void CCON_TD_Cell_Input::My_Input_Cell()
{
	int		v_iLen,vi;													//临时变量
	CString	v_sSql,v_sTemp,v_sRegion;									//临时变量
	CString	v_sCell,v_sType,v_sLAC,v_sBTS,v_sBTS_Name,v_sJd,v_sWd;		//临时变量
	CString v_sOnline,v_sDate,v_sNew,v_sStation,v_sTxfx,v_sTxgd,v_sTxqj;//临时变量
	int		v_iSum=0;													//
	int		v_iSum_Sql=0;												//

	m_CON_Location				v_mLocation;							//容器：v_mLocation
	m_CON_Location::iterator	v_pLocation;							//迭代器
	mString_Int					v_mCell;								//容器：v_mCell
	mString_Int::iterator		v_pIter;								//迭代器
	mString_Int					v_mBTS;									//容器：v_mBTS
	mString_Int::iterator		v_pIter_BTS;							//迭代器

	if(MessageBox("Cell数据 导入确认 ? ","数据导入确认",MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)	//导入确认？？？
		return;															//返回：退出

	//第一步：数据初始化
	v_iLen=m_cList.GetItemCount();										//列表条目数量
	if (v_iLen<=0)
	{
		MessageBox("没有可以导入的Cell数据 ！！！");					//返回：无数据
		return;
	}

	//第二部：获取相关数据 [站点、机房]
	My_Load_Cell(&v_mCell);												//获取：信息 [放入：v_mCell]
	My_Load_BTS(&v_mBTS);												//获取：信息 [放入：v_mBTS]

	//第三步：数据表保存 [存在：更新；不存在：增加]
	COleDateTime v_tTime=COleDateTime::GetCurrentTime();				//日期
	CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");			//
	v_sSql="";															//初始化
	for (vi=0;vi<v_iLen;vi++)
	{
		if (v_iThread_Exit==2)											//强制退出？？？
			break;														//退出

		//获取：数据
		v_sCell=m_cList.GetItemText(vi,1);								//Cell
		v_sBTS=m_cList.GetItemText(vi,2);								//NodeB
		v_sLAC=m_cList.GetItemText(vi,3);								//LAC
		v_sRegion=m_cList.GetItemText(vi,4);							//区域
		v_sStation=m_cList.GetItemText(vi,5);							//所属机房/资源点
		v_sType=m_cList.GetItemText(vi,6);								//覆盖类型
		v_sOnline=m_cList.GetItemText(vi,7);							//状态

		if (m_cOnline.GetCheck() && v_sOnline!="在网")					//在网有效？？？
			continue;													//下一条

		if (!v_sBTS.IsEmpty())											//有效？？？
		{
			//v_sLocation存在：更新；不存在：异常;
			v_pIter_BTS=v_mBTS.find(v_sBTS);							//查找：v_sBTS
			if (v_pIter_BTS != v_mBTS.end())							//找到？？？
			{
				v_iSum++;												//总数：Cell
				v_pIter=v_mCell.find(v_sCell);							//查找：v_sCell
				if (v_pIter == v_mCell.end())							//没找到？？？
				{														//新增
					v_sNew="Insert into LTE_Cell(zCell,zNODEB,zStation,zLAC,zType,zRegion,zOnline,zDate) values ('"
						+v_sCell+"','"+v_sBTS+"','"+v_sStation+"','"+v_sLAC+"','"+v_sType+"','"+v_sRegion+"','"
						+v_sOnline+"','"+v_sCurrentTime+"')";		//SQL
					m_cList.SetItemBkColor(vi,RGB(0,255,0));			//设置：背景色 [绿色]
				}
				else
				{
					v_sNew="Update LTE_Cell set zNODEB='"+v_sBTS+"',zStation='"+v_sStation+"',zLAC='"+v_sLAC
						+"',zType='"+v_sType+"',zRegion='"+v_sRegion+"',zOnline='"+v_sOnline+"',zDate='"+v_sCurrentTime
						+"' where zCell='"+v_sCell+"'";					//SQL
					m_cList.SetItemBkColor(vi,RGB(67,240,250));			//设置：背景色 [亮绿色]			
				}
				//数据库操作
				v_sSql+=v_sNew+"\r\n";									//增加一条SQL
				v_iSum_Sql++;											//SQL数量++
				if(v_iSum_Sql==1000)									//每0100条，写数据表
				{
					My_ExecuteSQL(v_sSql);								//SQL执行
					v_sSql="";											//初始化
					v_iSum_Sql=0;										//初始化
				}

				if (v_iSum%100==0)
				{
					v_sTemp.Format("%d",v_iSum);						//总数：行
					m_cSum.SetWindowText(v_sTemp);						//总数：行
				}
			}
			else
			{
				My_ShowList_Bug("Cell异常","异常：NodeB不存在；【Cell名称："+v_sCell+"； 所属NodeB："+v_sBTS+"  不存在】");	//显示：异常信息
				m_cList.SetItemBkColor(vi,RGB(255,0,0));				//设置：背景色 [红色]
			}
		}
		else
		{
			My_ShowList_Bug("Cell异常","异常：NodeB名称异常；【Cell名称："+v_sCell+"； 所属NodeB："+v_sBTS+"】");			//显示：异常信息
			m_cList.SetItemBkColor(vi,RGB(255,0,0));					//设置：背景色 [红色]
		}
		m_cList.EnsureVisible(vi,false);								//显示：比对记录
	}
	if(v_iSum_Sql>0)													//有SQL信息？？？ 【写数据表】
		My_ExecuteSQL(v_sSql);											//SQL执行

	v_mCell.clear();													//清除：容器
	v_mBTS.clear();														//清除：容器

	//日志
	v_sSql="Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"+v_sCurrentTime+"','TD Cell 数据导入','[用户："+theApp.a_sUser_Name+"]')";//SQL
	My_ExecuteSQL(v_sSql);												//数据库执行

	v_sTemp.Format("%d",v_iSum);										//总数：行
	m_cSum.SetWindowText(v_sTemp);										//总数：行
	v_sTemp.Format("TD Cell总数：%d",v_iSum);
	MessageBox("TD Cell数据导入完成："+v_sTemp);						//返回：无数据
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载信息【Cell】
//------------------------------------------------------------------------------------------------------			
void CCON_TD_Cell_Input::My_Load_Cell(mString_Int *v_mCell)
{
	CString			v_sSql;												//临时变量
	CString			v_sTemp,v_sName;									//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//获取：NodeB
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT zCell FROM LTE_Cell";
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										
		while(!v_pRecordset->adoEOF)
		{
			v_sName = v_cAdoConn.GetField_String("zCell");				//zCell

			v_mCell->insert(pair<CString,int>(v_sName,0));				//容器：增加

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
//	函数功能：装载信息【BTS】
//------------------------------------------------------------------------------------------------------			
void CCON_TD_Cell_Input::My_Load_BTS(mString_Int *v_mBTS)
{
	CString			v_sSql;												//临时变量
	CString			v_sTemp,v_sName;									//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//获取：NodeB
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "SELECT zNODEB FROM LTE_NODEB";
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										
		while(!v_pRecordset->adoEOF)
		{
			v_sName = v_cAdoConn.GetField_String("zNODEB");				//zNODEB

			v_mBTS->insert(pair<CString,int>(v_sName,0));				//容器：增加

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
//	函数功能：Bug列表窗中增加一行通信信息
//	参    数：v_sBug_Type：异常类型；	v_sBug：异常信息
//------------------------------------------------------------------------------------------------------
void CCON_TD_Cell_Input::My_ShowList_Bug(CString v_sBug_Type,CString v_sBug)
{
	int		v_iIndex;													//临时变量
	CString v_sTemp;													//临时变量
	CString v_sCurTime;													//临时变量
	CTime	v_CurTime=CTime::GetCurrentTime();

	v_sCurTime=v_CurTime.Format("%Y.%m.%d_%H:%M:%S");
	v_iIndex = m_cList_Bug.InsertItem(0xffff,_T("0"));
	v_sTemp.Format("%04d",v_iIndex+1);
	m_cList_Bug.SetItemText(v_iIndex,0," "+v_sTemp);					//序号
	m_cList_Bug.SetItemText(v_iIndex,1,v_sCurTime);						//发生时间
	m_cList_Bug.SetItemText(v_iIndex,2,v_sBug_Type);					//异常类型
	m_cList_Bug.SetItemText(v_iIndex,3,v_sBug);							//异常信息

	//列表窗条数控制：超过1000条，删除300条；
// 	m_cList_Bug.SetRedraw(FALSE);										//不更新界面
// 	if (m_cList_Bug.GetItemCount()>=1000)								//如果纪录数大于1000
// 	{
// 		for(vi=0;vi<300;vi++)											//删除前300条
// 			m_cList_Bug.DeleteItem(0);									//删除第一条数据记录
// 		for(vi=0;vi<m_cList_Bug.GetItemCount();vi++)					//重新分配序号
// 		{
// 			v_sTemp.Format("%04d",vi+1);
// 			m_cList_Bug.SetItemText(vi,0," "+v_sTemp);
// 		}
// 	}
	m_cList_Bug.SetRedraw(true);										//更新界面
	m_cList_Bug.EnsureVisible(m_cList_Bug.GetItemCount()-1,false);		//显示最新一条记录
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CCON_TD_Cell_Input::My_ExecuteSQL(CString v_sSql)
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
