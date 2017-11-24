// CON_Station_Region.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include "CON_Station_Region.h"


// CCON_Station_Region 对话框

IMPLEMENT_DYNAMIC(CCON_Station_Region, CDialog)

//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CCON_Station_Region::CCON_Station_Region(CWnd* pParent /*=NULL*/)
	: CDialog(CCON_Station_Region::IDD, pParent)
{

}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CCON_Station_Region::~CCON_Station_Region()
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void CCON_Station_Region::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cAccount);
	DDX_Control(pDX, IDC_EDIT2, m_cPassword);
	DDX_Control(pDX, IDOK, m_cRegion);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCON_Station_Region, CDialog)
	ON_BN_CLICKED(IDOK, &CCON_Station_Region::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, &CCON_Station_Region::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCON_Station_Region 消息处理程序

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL CCON_Station_Region::PreTranslateMessage(MSG* pMsg)
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
//	函数功能：区域同步
//------------------------------------------------------------------------------------------------------
void CCON_Station_Region::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	v_iWorking=0;														//工作任务：华为文件选择

	v_iThread_Exit=1;													//线程：工作
	v_pThread_Input=AfxBeginThread(My_Thread_Inoput,this);				//开启查询数据库线程
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：线程启动
//------------------------------------------------------------------------------------------------------
UINT CCON_Station_Region::My_Thread_Inoput(LPVOID lparam)
{
	CCON_Station_Region *  lp_this = NULL ;
	lp_this = (CCON_Station_Region *)lparam;

	lp_this->m_cRegion.EnableWindow(false);								//文件选择：无效

	lp_this->My_Input_Main();											//导入_主程序

	lp_this->m_cRegion.EnableWindow(true);								//文件选择：有效

	lp_this->v_iThread_Exit=0;											//线程：退出

	return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：导入_主程序
//------------------------------------------------------------------------------------------------------
void CCON_Station_Region::My_Input_Main()
{
	switch(v_iWorking)
	{
	case 0:
		My_Region();													//区域同步
		break;
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：区域同步
//  操作过程：【1】获取GSM\TD-LTE小区的站址、区域；
//			  【2】获取站址的区域，与小区的站址区域比较，不同就同步；
//------------------------------------------------------------------------------------------------------
void CCON_Station_Region::My_Region()
{
	CString		v_sSql,v_sTemp,v_sRegion,v_sSation,v_sNew;				//临时变量
	int		v_iSum=0;													//
	int		v_iSum_Sql=0;												//

	//站址容器
	mString_String				v_mStation_Region;						//站址区域容器
	mString_String::iterator	v_pStation_Region;						//迭代器

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例

	try
	{
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		//【GSM】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select zStation,zRegion FROM GSM_Cell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			if (v_iThread_Exit==2)										//强制退出？？？
				break;													//退出

			v_sSation = v_cAdoConn.GetField_String("zStation");			//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion

			v_pStation_Region=v_mStation_Region.find(v_sSation);		//查找：站址
			if (v_pStation_Region==v_mStation_Region.end())
				v_mStation_Region.insert(pair<CString,CString>(v_sSation,v_sRegion));	//容器增加

			if (v_mStation_Region.size()%100==0)
			{
				v_sTemp.Format("%d",v_mStation_Region.size());			//总数：行
				m_cAccount.SetWindowText(v_sTemp);						//总数：行
			}

			v_pRecordset->MoveNext();									//下一条
		}

		//【TD-LTE】
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select zStation,zRegion FROM LTE_Cell"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		while(!v_pRecordset->adoEOF)
		{
			if (v_iThread_Exit==2)										//强制退出？？？
				break;													//退出

			v_sSation = v_cAdoConn.GetField_String("zStation");			//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion

			v_pStation_Region=v_mStation_Region.find(v_sSation);		//查找：站址
			if (v_pStation_Region==v_mStation_Region.end())
				v_mStation_Region.insert(pair<CString,CString>(v_sSation,v_sRegion));	//容器增加

			if (v_mStation_Region.size()%100==0)
			{
				v_sTemp.Format("%d",v_mStation_Region.size());			//总数：行
				m_cAccount.SetWindowText(v_sTemp);						//总数：行
			}

			v_pRecordset->MoveNext();									//
		}
		v_sTemp.Format("%d",v_mStation_Region.size());					//总数：行
		m_cAccount.SetWindowText(v_sTemp);								//总数：行

		//同步
		v_sSql="SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ";
		v_sSql += "select zStation,zRegion FROM CON_Station"; 
		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句
		v_sSql="";														//初始化
		v_iSum=0;														//初始化
		while(!v_pRecordset->adoEOF)
		{
			if (v_iThread_Exit==2)										//强制退出？？？
				break;													//退出

			v_sSation = v_cAdoConn.GetField_String("zStation");			//zStation
			v_sRegion = v_cAdoConn.GetField_String("zRegion");			//zRegion

			v_pStation_Region=v_mStation_Region.find(v_sSation);		//查找：站址
			if (v_pStation_Region!=v_mStation_Region.end())
			{
				if (v_sRegion!=v_pStation_Region->second)				//站址不相等？？？
				{
					v_iSum++;											//总数：Station
					v_sNew="Update CON_Station set zRegion='"+v_pStation_Region->second
							+"' where zStation='"+v_sSation+"'";		//SQL

					//数据库操作
					v_sSql+=v_sNew+"\r\n";								//增加一条SQL
					v_iSum_Sql++;										//SQL数量++
					if(v_iSum_Sql==1000)								//每1000条，写数据表
					{
						My_ExecuteSQL(v_sSql);							//SQL执行
						v_sSql="";										//初始化
						v_iSum_Sql=0;									//初始化
					}

					if (v_iSum%100==0)
					{
						v_sTemp.Format("%d",v_iSum);					//总数：行
						m_cPassword.SetWindowText(v_sTemp);				//总数：行
					}
				}
			}

			v_pRecordset->MoveNext();									//下一条
		}
		v_sTemp.Format("%d",v_iSum);									//总数：行
		m_cPassword.SetWindowText(v_sTemp);								//总数：行

		if(v_iSum_Sql>0)												//有SQL信息？？？ 【写数据表】
			My_ExecuteSQL(v_sSql);										//SQL执行
	}
	catch(_com_error e)
	{
		v_cAdoConn.ExitConnect();										//断开连接
		v_sTemp = (char *)e.Description();								//错误信息
		MessageBox(v_sTemp);											//窗体提示
	}

	v_mStation_Region.clear();											//容器清除
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：取消
//------------------------------------------------------------------------------------------------------
void CCON_Station_Region::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

//------------------------------------------------------------------------------------------------------
//	函数功能：SQL执行函数
//	参数：v_sSql：SQL命令；
//------------------------------------------------------------------------------------------------------
bool CCON_Station_Region::My_ExecuteSQL(CString v_sSql)
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
