// CON_Station_Input_L.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "CON_Station_Input_L.h"


// CCON_Station_Input_L 对话框

IMPLEMENT_DYNAMIC(CCON_Station_Input_L, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CCON_Station_Input_L::CCON_Station_Input_L(CWnd* pParent /*=NULL*/)
	: CDialog(CCON_Station_Input_L::IDD, pParent)
{
	v_iThread_Exit=0;													//线程：停止
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CCON_Station_Input_L::~CCON_Station_Input_L()
{
	v_mLocation_Tj.clear();												//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input_L::DoDataExchange(CDataExchange* pDX)
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
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_Station_Input_L, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON4, &CCON_Station_Input_L::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CCON_Station_Input_L::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON6, &CCON_Station_Input_L::OnBnClickedButton6)
END_MESSAGE_MAP()


// CCON_Station_Input_L 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL CCON_Station_Input_L::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化：列表窗 【站点】
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,60);					//
	m_cList.InsertColumn(1,"站点名称",LVCFMT_LEFT,300);					//
	m_cList.InsertColumn(2,"站点类型",LVCFMT_LEFT,120);					//
	m_cList.InsertColumn(3,"区域",LVCFMT_LEFT,80);						//
	m_cList.InsertColumn(4,"地址",LVCFMT_LEFT,300);						//
	m_cList.InsertColumn(5,"经度",LVCFMT_RIGHT,100);					//
	m_cList.InsertColumn(6,"纬度",LVCFMT_RIGHT,100);					//
	m_cList.InsertColumn(7,"状态",LVCFMT_CENTER,80);					//
	m_cList.InsertColumn(8,"入网日期",LVCFMT_CENTER,100);				//
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
		IDC_CON_Station_Input,	// this ID is used for saving/restoring splitter position and it must be unique !
		v_cRect					// dimensions of the splitter pane
		);

	//显示
	ShowWindow(SW_MAXIMIZE);											//对话窗最大化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CCON_Station_Input_L::PreTranslateMessage(MSG* pMsg)
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
void CCON_Station_Input_L::OnSize(UINT nType, int cx, int cy)
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
void CCON_Station_Input_L::OnClose()
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
void CCON_Station_Input_L::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExcel MyExcel;
	MyExcel.Show_Name(&m_cList_Bug,"导入异常_站点",m_cFile_Open.GetCheck());		//导出Excel
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取资管数据CSV文件信息
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input_L::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=0;														//工作任务：华为文件选择

	v_iThread_Exit=1;													//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：把列表窗中的数据保存到数据表
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input_L::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：华为数据导入

	v_iThread_Exit=1;													//线程开启：有效
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//导入
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动	[静态门限计算]
//------------------------------------------------------------------------------------------------------
UINT CCON_Station_Input_L::My_Thread_Inoput(LPVOID lparam)
{
	CCON_Station_Input_L *  lp_this = NULL ;
	lp_this = (CCON_Station_Input_L *)lparam;

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
void CCON_Station_Input_L::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 0:
		My_File();														//文件选择
		break;
	case 1:
		My_Input_Station();												//Station
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取CSV文件信息
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input_L::My_File()
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
		v_mMap.insert(pair<CString,int>("站点名称",1));					//1：站点名称
		v_mMap.insert(pair<CString,int>("站点类型",2));					//2：类型
		v_mMap.insert(pair<CString,int>("所属区县名称",3));				//3：区域
		v_mMap.insert(pair<CString,int>("所在地址",4));					//4：地址
		v_mMap.insert(pair<CString,int>("经度",5));						//5：经度
		v_mMap.insert(pair<CString,int>("纬度",6));						//6：纬度
		v_mMap.insert(pair<CString,int>("生命周期状态",7));				//7：状态
		v_mMap.insert(pair<CString,int>("入网日期",8));					//8：入网日期
		if (!Get_CSV_Cell(v_sFilename,&v_mMap,true))					//获取：CSV文件信息
			MessageBox("无有效的 站点 文件，请重新选择 ！！！");		//窗体提示	
	}
	delete(filedialog);
	v_mMap.clear();														//清除容器
}

//----------------------------------------------------------------------------------------------------
//	函数功能：导入CSV文件到 m_cList 控件中
//	参    数：CSV文件路径，	v_mMap：列映射关系，	v_bHead：文件带表头标记；
//----------------------------------------------------------------------------------------------------
bool CCON_Station_Input_L::Get_CSV_Cell(CString v_sFile,mString_Int *v_mHead,bool v_bHead)
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
				if (v_pCol->second==1)									//站点
					v_sName=v_sTemp;									//保存：在线
			}
			v_iNum++;													//列号：++
		}
		if (!v_sName.IsEmpty())											//条件：数据有效？？？
		{
			v_pRepeat=v_mRepeat.find(v_sName);							//机房存在？？？
			if (v_pRepeat == v_mRepeat.end())							//资源点：不存在
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
				My_ShowList_Bug("站点异常","异常：站点名称重复 【站点名称："+v_sName+"】");		//显示：异常信息
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
//	函数功能：数据导入
//------------------------------------------------------------------------------------------------------
void CCON_Station_Input_L::My_Input_Station()
{
	int		v_iLen,vi;													//临时变量
	CString	v_sSql,v_sTemp,v_sSql_New;									//临时变量
	CString	v_sStation,v_sLongitude,v_sLatitude,v_sLocation,v_sNew,v_sAddr;		//临时变量
	int		v_iSum=0;													//
	int		v_iSum_Sql=0;												//
	double	v_dLongitude,v_dLatitude;									//

	Location_Tj_Struct			v_cLocation;							//结构
	mLocation_Tj::iterator		v_pLocation;							//迭代器

	if(MessageBox("站点 数据 导入确认 ? ","数据导入确认",MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)	//导入确认？？？
		return;															//返回：退出

	//第一步：获取站点信息【经度、纬度】
	v_iLen=m_cList.GetItemCount();										//列表条目数量
	if (v_iLen<=0)
	{
		MessageBox("没有可以导入的 站点 数据 ！！！");					//返回：无数据
		return;
	}
	v_mLocation_Tj.clear();												//容器清除
	for (vi=0;vi<v_iLen;vi++)
	{
		if (v_iThread_Exit==2)											//强制退出？？？
			break;														//退出

		//获取：数据
		v_sStation=m_cList.GetItemText(vi,1);							//zStation
		v_sAddr=m_cList.GetItemText(vi,4);								//zAddress
		v_sLongitude=m_cList.GetItemText(vi,5);							//zLongitude
		v_sLatitude=m_cList.GetItemText(vi,6);							//zLatitude
		
		v_dLongitude=atof(v_sLongitude);								//
		v_dLatitude=atof(v_sLatitude);									//
		v_cLocation.v_dLongitude=v_dLongitude;							//
		v_cLocation.v_dLatitude=v_dLatitude;							//
		v_cLocation.v_sAddr=v_sAddr;									//
		v_mLocation_Tj.insert(pair<CString,Location_Tj_Struct>(v_sStation,v_cLocation));	//容器增加
	}

	//第三步：站点信息保存 【存在：更新】 ==》站址表
	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例

	v_sSql="";															//初始化
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//获取：NodeB
		v_sSql_New="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql_New += "SELECT zStation,zLocation,zLongitude,zLatitude FROM CON_Station where zLocation is not null";
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql_New);	//执行SELETE语句										
		while(!v_pRecordset->adoEOF)
		{
			v_sStation = v_cAdoConn.GetField_String("zStation");		//zStation
			v_sLocation = v_cAdoConn.GetField_String("zLocation");		//zLocation
			v_dLongitude = v_cAdoConn.GetField_Double("zLongitude");	//zLongitude
			v_dLatitude = v_cAdoConn.GetField_Double("zLatitude");		//zLatitude

			//Station存在：更新；不存在：异常;
			v_pLocation=v_mLocation_Tj.find(v_sLocation);				//查找：v_sLocation
			if (v_pLocation != v_mLocation_Tj.end())					//找到？？？
			{															//存在
				if (v_pLocation->second.v_dLongitude!=v_dLongitude || v_pLocation->second.v_dLatitude!=v_dLatitude)
				{
					v_iSum++;											//总数：Station
					v_sLongitude.Format("%10.6f",v_pLocation->second.v_dLongitude);
					v_sLatitude.Format("%10.6f",v_pLocation->second.v_dLatitude);
					v_sNew="Update CON_Station set zLongitude='"+v_sLongitude+"',zLatitude='"+v_sLatitude+"',zAddress='"+v_sAddr
						+"' where zStation='"+v_sStation+"'";			//SQL
				}
			}
			//数据库操作
			v_sSql+=v_sNew+"\r\n";										//增加一条SQL
			v_iSum_Sql++;												//SQL数量++
			if(v_iSum_Sql==1000)										//每1000条，写数据表
			{
				My_ExecuteSQL(v_sSql);									//SQL执行
				v_sSql="";												//初始化
				v_iSum_Sql=0;											//初始化
			}

			if (v_iSum%100==0)
			{
				v_sTemp.Format("%d",v_iSum);							//总数：行
				m_cSum.SetWindowText(v_sTemp);							//总数：行
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
	if(v_iSum_Sql>0)													//有SQL信息？？？ 【写数据表】
		My_ExecuteSQL(v_sSql);											//SQL执行

	//日志
	COleDateTime v_tTime=COleDateTime::GetCurrentTime();				//日期
	CString v_sCurrentTime=v_tTime.Format("%Y-%m-%d %H:%M:%S");			//
	v_sSql="Insert into BAS_Log(zTime,zLog_Type,zLog_Content) values('"+v_sCurrentTime+"','站点 数据导入','[用户："+theApp.a_sUser_Name+"]')";//SQL
	My_ExecuteSQL(v_sSql);												//数据库执行

	v_sTemp.Format("%d",v_iSum);										//总数：行
	m_cSum.SetWindowText(v_sTemp);										//总数：行
	v_sTemp.Format("站点 总数：%d",v_iSum);
	MessageBox("站点 数据导入完成："+v_sTemp);							//返回：无数据
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载信息【Station】
//------------------------------------------------------------------------------------------------------			
void CCON_Station_Input_L::My_Load_Station(mString_Int *v_mStation)
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
		v_sSql += "SELECT zStation FROM CON_Station";
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										
		while(!v_pRecordset->adoEOF)
		{
			v_sName = v_cAdoConn.GetField_String("zStation");			//zStation

			v_mStation->insert(pair<CString,int>(v_sName,0));			//容器：增加

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
void CCON_Station_Input_L::My_ShowList_Bug(CString v_sBug_Type,CString v_sBug)
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
bool CCON_Station_Input_L::My_ExecuteSQL(CString v_sSql)
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
