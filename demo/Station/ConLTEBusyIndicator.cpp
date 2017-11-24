// ConLTEBusyIndicator.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "ConLTEBusyIndicator.h"


// CConLTEBusyIndicator

IMPLEMENT_DYNCREATE(CConLTEBusyIndicator, CFormView)

CConLTEBusyIndicator::CConLTEBusyIndicator()
	: CFormView(CConLTEBusyIndicator::IDD)
{

}

CConLTEBusyIndicator::~CConLTEBusyIndicator()
{
}

void CConLTEBusyIndicator::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_cList);
	DDX_Control(pDX, IDC_KEY, m_cFile_Name);
	DDX_Control(pDX, IDC_BUTTON_FILE_SELECT, m_cFile);
	DDX_Control(pDX, IDC_BUTTON_INPUT_DATABASE, m_cInput);
	DDX_Control(pDX, IDC_BUTTON_OUTPUT_DATABASE, m_cOutput);
	DDX_Control(pDX, IDC_BUTTON_READ_CSV, m_cFileRead);
}

BEGIN_MESSAGE_MAP(CConLTEBusyIndicator, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SELECT, &CConLTEBusyIndicator::OnBnClickedButtonFileSelect)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_DATABASE, &CConLTEBusyIndicator::OnBnClickedButtonInputDatabase)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_DATABASE, &CConLTEBusyIndicator::OnBnClickedButtonOutputDatabase)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_READ_CSV, &CConLTEBusyIndicator::OnBnClickedButtonReadCsv)
END_MESSAGE_MAP()


// CConLTEBusyIndicator 诊断

#ifdef _DEBUG
void CConLTEBusyIndicator::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CConLTEBusyIndicator::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CConLTEBusyIndicator 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//窗体名称
	v_sFrame_Name="LTE忙时指标";											//窗体名称
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//获取：子窗指针
	//子窗标题设置
	GetParentFrame()->SetWindowText(v_sFrame_Name);						//子窗标题设置
	//初始化：列表窗 [小区]
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,50);					
	m_cList.InsertColumn(1,"小区",LVCFMT_LEFT,100);					
	m_cList.InsertColumn(2,"营业区",LVCFMT_LEFT,80);					
	m_cList.InsertColumn(3,"状态",LVCFMT_LEFT,80);						
	m_cList.InsertColumn(4,"日期数",LVCFMT_LEFT,280);	
	m_cList.InsertColumn(5,"无线利用率",LVCFMT_CENTER,80);		
	m_cList.InsertColumn(6,"小区用户面上行字节数",LVCFMT_CENTER,150);					
	m_cList.InsertColumn(7,"小区用户面上行字节数",LVCFMT_RIGHT,150);					
	m_cList.InsertColumn(8,"有效RRC连接平均数",LVCFMT_CENTER,150);				
	m_cList.InsertColumn(9,"E-RAB建立成功数",LVCFMT_LEFT,120);						
	m_cList.InsertColumn(10,"下行PDSCHPRB可用平均数",LVCFMT_CENTER,150);					
	m_cList.InsertColumn(11,"下行PDSCHPRB占用平均数",LVCFMT_RIGHT,150);					
	m_cList.InsertColumn(12,"上行PUSCHPRB可用平均数",LVCFMT_CENTER,150);				
	m_cList.InsertColumn(13,"上行PUSCHPRB占用平均数",LVCFMT_CENTER,150);				
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );
	

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：消息预处理 [截取键盘消息到窗体]
//------------------------------------------------------------------------------------------------------			
BOOL CConLTEBusyIndicator::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：SQL执行函数
//	参    数：v_sSql：SQL语句
//------------------------------------------------------------------------------------------------------
bool CConLTEBusyIndicator::My_ExecuteSQL(CString v_sSql)
{
	// TODO: Add your control notification handler code here
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(1000*200);
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
//	函数功能：List显示csv指标
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::ShowList()
{
	CMyExcel csv_read;
	CString temp;
	m_cFile_Name.GetWindowText(temp);
	csv_read.Get_CSV(temp,&m_cList,false);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------		
//函数功能：执行bulkinsert语句
//参    数：url:文件路径，table_suffix：表后缀,control_count :控制批处理数量
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::ExcecuteBulkinsert(CString url,CString table_suffix,int control_count)
{
	CString sql,count;
	bool flag;
	count.Format("%d",control_count);
	sql="bulk insert LTE_Load_Metric" + table_suffix + " FROM '"+ url +  "' WITH ( FIELDTERMINATOR =',', FIRSTROW = 2 , BATCHSIZE = " + count + ")";
	flag = My_ExecuteSQL(sql);
	if (flag)
	{
		MessageBox("导入数据完成");
	} 
	else
	{
		MessageBox("导入数据失败");
	}	
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：性能数据导入
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::OutputCellBusyIndicator()
{
	CString		v_sTemp;												//临时变量
	CFileDialog *filedialog;											//选择文件对话窗指针
	filedialog = new CFileDialog(false,"csv","Excel.csv",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"(*.csv;*.txt;*.xlsx)|*.csv;*.txt;*.xlsx||");				//实例化
	if(filedialog->DoModal()==IDOK)										//打开选择文件对话窗
	{
		v_sFilename = filedialog->GetPathName();						//获得：选择的文件名称
	}
	else
	{
		return;
	}
	CString sql,cell_name,before_cell_name(""),file_write,temp;
	_RecordsetPtr v_pRecordset;											//记录集
	int count(0);
	float	UpOctDl(0),temp_UpOctDl(0),UpOctUl(0),temp_UpOctUl(0),wireless_Utility(0),temp_wireless_Utility(0);
	float   temp_EffectiveConnMean(0),EffectiveConnMean(0),PdschPrbMeanTot(0),temp_PdschPrbMeanTot(0),PdschPrbTotMeanDl(0),temp_PdschPrbTotMeanDl(0);	
	float   PuschPrbMeanTot(0),temp_PuschPrbMeanTot(0),PuschPrbTotMeanUl(0),temp_PuschPrbTotMeanUl(0);	
	CMyADOConn		v_cAdoConn;											//数据库实例
	Busy_Indicator	indicator;                                          //指标
	CFile			Pm_file;                                           //文件实例 
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(1000*200);
		sql="select * from"
			"(select * from [Station].[dbo].[LTE_Load_Metric]) as t1"
			" right join"
			"(select [小区] as [小区_1]"
			",max([无线利用率]) as [无线利用率_1]" 
			",convert(varchar(10),[日期],120) as [日期_1]"
			" from [Station].[dbo].[LTE_Load_Metric] group by [小区]"
			",convert(varchar(10),[日期],120)) as t2"
			" on (t1.[无线利用率] = t2.[无线利用率_1] and t1.[小区] = t2.[小区_1]) where t1.[无线利用率]!=0 order by t2.[小区_1]";

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)sql);								//执行SELETE语句					
		while(!v_pRecordset->adoEOF)
		{
			cell_name = v_cAdoConn.GetField_String("小区");									//小区名称
			temp_wireless_Utility = v_cAdoConn.GetField_Double("无线利用率");					//小区无线利用率
			temp_UpOctDl = v_cAdoConn.GetField_Double("小区用户面下行字节数");			//小区用户面下行字节数
			temp_UpOctUl = v_cAdoConn.GetField_Double("小区用户面上行字节数");			//小区用户面上行字节数
			temp_EffectiveConnMean = v_cAdoConn.GetField_Double("有效RRC连接平均数");		//有效RRC连接平均数
			temp_PdschPrbMeanTot = v_cAdoConn.GetField_Double("下行PDSCHPRB可用平均数");	//下行PDSCHPRB可用平均数
			temp_PdschPrbTotMeanDl = v_cAdoConn.GetField_Double("下行PDSCHPRB占用平均数");	//下行PDSCHPRB占用平均数
			temp_PuschPrbMeanTot = v_cAdoConn.GetField_Double("上行PUSCHPRB可用平均数");	//上行PUSCHPRB可用平均数
			temp_PuschPrbTotMeanUl = v_cAdoConn.GetField_Double("上行PUSCHPRB占用平均数");	//上行PUSCHPRB占用平均数
			v_pRecordset->MoveNext();														
			if (cell_name == before_cell_name) {
				before_cell_name = cell_name;
				wireless_Utility += temp_wireless_Utility;
				UpOctDl += temp_UpOctDl;
				UpOctUl += temp_UpOctUl;
				EffectiveConnMean += temp_EffectiveConnMean;
				PdschPrbMeanTot += temp_PdschPrbMeanTot;
				PdschPrbTotMeanDl += temp_PdschPrbTotMeanDl;
				PuschPrbMeanTot += temp_PuschPrbMeanTot;
				PuschPrbTotMeanUl += temp_PuschPrbTotMeanUl;
				count ++;
			}
			else if(before_cell_name =="") {
				before_cell_name = cell_name;
				wireless_Utility += temp_wireless_Utility;
				UpOctDl += temp_UpOctDl;
				UpOctUl += temp_UpOctUl;
				EffectiveConnMean += temp_EffectiveConnMean;		
				PdschPrbMeanTot += temp_PdschPrbMeanTot;
				PdschPrbTotMeanDl += temp_PdschPrbTotMeanDl;
				PuschPrbMeanTot += temp_PuschPrbMeanTot;
				PuschPrbTotMeanUl += temp_PuschPrbTotMeanUl;
				count ++;
			}
			else{
				wireless_Utility = wireless_Utility / count;
				UpOctDl = UpOctDl / count;
				UpOctUl = UpOctUl / count;
				EffectiveConnMean = EffectiveConnMean / count;		
				PdschPrbMeanTot = PdschPrbMeanTot / count;	
				PdschPrbTotMeanDl = PdschPrbTotMeanDl / count;
				PuschPrbMeanTot = PuschPrbMeanTot / count;	
				PuschPrbTotMeanUl = PuschPrbTotMeanUl / count;
				indicator.cellname = before_cell_name;
				indicator.wireless_Utility = wireless_Utility;
				indicator.UpOctDl = UpOctDl;
				indicator.UpOctUl = UpOctUl;
				indicator.EffectiveConnMean = EffectiveConnMean;		
				indicator.PdschPrbMeanTot = PdschPrbMeanTot;
				indicator. PdschPrbTotMeanDl =  PdschPrbTotMeanDl;
				indicator.PuschPrbMeanTot = PuschPrbMeanTot;
				indicator. PuschPrbTotMeanUl =  PuschPrbTotMeanUl;
				busy_indicator_.push_back(indicator);
				count = 1;
				before_cell_name = cell_name;
				wireless_Utility = temp_wireless_Utility;
				UpOctDl = temp_UpOctDl;
				UpOctUl = temp_UpOctUl;
				EffectiveConnMean = temp_EffectiveConnMean;
				PdschPrbMeanTot = temp_PdschPrbMeanTot;
				PdschPrbTotMeanDl =  temp_PdschPrbTotMeanDl;
				PuschPrbMeanTot = temp_PuschPrbMeanTot;
				PuschPrbTotMeanUl =  temp_PuschPrbTotMeanUl;
			}
		}
		Pm_file.Open(v_sFilename,CFile::modeWrite|CFile::modeRead|CFile::modeCreate);
		while(busy_indicator_.size() > 0){
			indicator = busy_indicator_.front();
			file_write = indicator.cellname;
			temp.Format("%f",indicator.wireless_Utility);
			file_write += "," + temp;
			temp.Format("%f",indicator.UpOctDl);
			file_write += "," + temp;
			temp.Format("%f",indicator.UpOctUl);
			file_write += "," + temp;
			temp.Format("%f",indicator.EffectiveConnMean);
			file_write += "," + temp;
			temp.Format("%f",indicator.PdschPrbMeanTot);
			file_write += "," + temp;
			temp.Format("%f",indicator.PdschPrbTotMeanDl);
			file_write += "," + temp;
			temp.Format("%f",indicator.PuschPrbMeanTot);
			file_write += "," + temp;
			temp.Format("%f",indicator.PuschPrbTotMeanUl);
			file_write += "," + temp;
			file_write += "\r\n";
			Pm_file.Write(file_write,file_write.GetLength());
			busy_indicator_.pop_front();
		}
		//关闭文件
		Pm_file.Close();
		MessageBox("文件导出成功");
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//关闭数据库

		CString v_sError = (char *)e.Description();						//获得：出错信息
		MessageBox(v_sError);											//失败
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：性能数据导入
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::My_Input_Cell()
{
	//指标导入数据库
	CString temp;
	m_cFile_Name.GetWindowText(temp);
	ExcecuteBulkinsert(temp,"",3000);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取CSV文件信息
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::My_File()
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
	}
	delete(filedialog);
	v_mMap.clear();														//清除容器
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 0:
		My_File();														//文件选择
		break;
	case 1:
		My_Input_Cell();												//Cell导入小区
		break;
	case 2:
		OutputCellBusyIndicator();										//Cell小区导出
		break;
	case 3:
		ShowList();														//Cell小区指标显示
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CConLTEBusyIndicator::My_Thread_Inoput(LPVOID lparam)
{
	CConLTEBusyIndicator *  lp_this = NULL ;
	lp_this = (CConLTEBusyIndicator *)lparam;

	lp_this->m_cFile.EnableWindow(false);								//文件选择：无效
	lp_this->m_cInput.EnableWindow(false);								//开始导入：无效
	lp_this->m_cOutput.EnableWindow(false);								//开始导出：无效
	lp_this->m_cFileRead.EnableWindow(false);	
	lp_this->My_Input_Main();											//导入_主程序
	lp_this->m_cFile.EnableWindow(true);								//文件选择：有效
	lp_this->m_cInput.EnableWindow(true);								//开始导入：有效
	lp_this->m_cOutput.EnableWindow(true);								//开始导出：有效
	lp_this->m_cFileRead.EnableWindow(true);
	lp_this->v_iThread_Exit=0;											//线程：退出

	return 0;
}


//------------------------------------------------------------------------------------------------------			
//	函数功能：获取CSV文件信息
//------------------------------------------------------------------------------------------------------
void CConLTEBusyIndicator::OnBnClickedButtonFileSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=0;														//工作任务：华为文件选择
	v_iThread_Exit=1;													//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

void CConLTEBusyIndicator::OnBnClickedButtonInputDatabase()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=1;														//工作任务：华为数据导入
	v_iThread_Exit=1;													//线程开启：有效
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//导入
}

void CConLTEBusyIndicator::OnBnClickedButtonOutputDatabase()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=2;														//工作任务：华为数据导入
	v_iThread_Exit=1;													//线程开启：有效
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//导入
}

void CConLTEBusyIndicator::OnBnClickedButtonReadCsv()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=3;														//工作任务：华为数据导入
	v_iThread_Exit=1;													//线程开启：有效
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//导入
}

void CConLTEBusyIndicator::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	mFrame_Window::iterator	v_pIterator_Frame;							//迭代器
	v_pIterator_Frame = theApp.v_mFrame.find(v_sFrame_Name);			//查找子窗名称
	if(v_pIterator_Frame != theApp.v_mFrame.end())						//子窗存在？
		theApp.v_mFrame.erase(v_pIterator_Frame);						//在容器中清除子窗名称
}

void CConLTEBusyIndicator::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	CWnd	*pCellList;													//临时变量
	int		v_iY;														//临时变量
	pCellList = GetDlgItem(IDC_LIST);
	v_iY = 50;															//赋值：Y轴
	if(pCellList)
	{
		pCellList->MoveWindow(14,v_iY,cx-24,cy-v_iY-10);
	}		
}


