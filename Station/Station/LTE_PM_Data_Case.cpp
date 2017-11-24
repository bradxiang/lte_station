// LTE_PM_DATA_CASE.cpp : 实现文件
//

#include "stdafx.h"
#include "Station.h"
#include <vector>
#include "LTE_PM_DATA_CASE.h"
#include "LTE_PM_Data.h"


// LTE_PM_DATA_CASE 对话框

IMPLEMENT_DYNAMIC(LTE_PM_DATA_CASE, CDialog)

LTE_PM_DATA_CASE::LTE_PM_DATA_CASE(CWnd* pParent /*=NULL*/)
	: CDialog(LTE_PM_DATA_CASE::IDD, pParent)
{

}

LTE_PM_DATA_CASE::~LTE_PM_DATA_CASE()
{
	v_mCell.clear();													//容器清除
}



//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕消息映射
//------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(LTE_PM_DATA_CASE, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_enter, &LTE_PM_DATA_CASE::OnBnClickedenter)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &LTE_PM_DATA_CASE::OnNMClickList2)

	ON_BN_CLICKED(IDC_CHECK1, &LTE_PM_DATA_CASE::OnBnClickedCheck1)
END_MESSAGE_MAP()
// LTE_PM_DATA_CASE 消息处理程序
//------------------------------------------------------------------------------------------------------			
//	函数功能：屏幕数据映射
//------------------------------------------------------------------------------------------------------
void LTE_PM_DATA_CASE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2,m_cList);
	DDX_Control(pDX, IDC_LIST7,m_cList_Cell);
	DDX_Control(pDX, IDC_CHECK1, m_cCheck);
	DDX_Control(pDX, IDC_LOG_SUM, m_cSum);
}
// LTE_PM_DATA_CASE 消息处理程序

//------------------------------------------------------------------------------------------------------			
//	函数功能：对话窗初始化
//------------------------------------------------------------------------------------------------------
BOOL LTE_PM_DATA_CASE::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化：列表窗 【待选性能】
	::SendMessage(m_cList.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList.InsertColumn(0,"序号",LVCFMT_CENTER,40);					//
	m_cList.InsertColumn(1,"帐号",LVCFMT_LEFT,60);						//
	m_cList.InsertColumn(2,"模板",LVCFMT_LEFT,150);						//
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT );

	//初始化：列表窗 【已选性能】
	::SendMessage(m_cList_Cell.m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);	
	m_cList_Cell.InsertColumn(0,"序号",LVCFMT_CENTER,60);				//
	if(v_iSelect==0)
		m_cList_Cell.InsertColumn(1,"小区",LVCFMT_LEFT,280);			//
	else
	{
		SetWindowText((LPCTSTR)"性能模板");
		m_cList_Cell.InsertColumn(1,"类型",LVCFMT_LEFT,100);			//
		m_cList_Cell.InsertColumn(2,"性能",LVCFMT_LEFT,280);			//
		m_cList_Cell.InsertColumn(3,"预警标志",LVCFMT_LEFT,0);			//
		m_cList_Cell.InsertColumn(4,"预警值",LVCFMT_LEFT,0);			//
		m_cList_Cell.InsertColumn(5,"预警类型",LVCFMT_LEFT,0);			//
	}
	m_cList_Cell.SetExtendedStyle(m_cList_Cell.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES );

	v_iList_Item=-1;													//默认：无选中的列表Item

	My_LoadData_Case();													//装载方案信息

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//------------------------------------------------------------------------------------------------------			
// 消息预处理：把回车键变换为TAB
//------------------------------------------------------------------------------------------------------			
BOOL LTE_PM_DATA_CASE::PreTranslateMessage(MSG* pMsg)
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
void LTE_PM_DATA_CASE::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：关闭窗体
//------------------------------------------------------------------------------------------------------
void LTE_PM_DATA_CASE::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnClose();													//退出	
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：全选【小区、性能】
//------------------------------------------------------------------------------------------------------
void LTE_PM_DATA_CASE::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int vi=0;vi<m_cList_Cell.GetItemCount();vi++)					//遍历
		m_cList_Cell.SetCheck(vi,m_cCheck.GetCheck());					//设置状态
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：确认
//	返    回：(1)小区：小区信息在容器v_mCell中；(2)性能：性能信息在容器v_mLTE_PM_Type(指针)中；
//------------------------------------------------------------------------------------------------------
void LTE_PM_DATA_CASE::OnBnClickedenter()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sPM_Name,v_sType;										//临时变量
	CString		v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;				//临时变量

	mLTE_PM_Type::iterator	v_pLTE_PM_Type;								//迭代器
	mLTE_PM::iterator		v_pPM_Name;									//迭代器

	//【1】复位：初始化
	v_mCell.clear();													//容器清除
	if (v_iSelect==1)													//性能
	{
		v_pLTE_PM_Type=v_mLTE_PM_Type->begin();							//首指针
		while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())					//遍历
		{
			for(v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
				v_pPM_Name->second.v_bOK=0;								//复位：选中状态
			v_pLTE_PM_Type++;
		}
	}

	//【2】获取：模板信息
	for(int vi=0;vi<m_cList_Cell.GetItemCount();vi++)					//遍历：模板信息列表
	{
		if (m_cList_Cell.GetCheck(vi))
		{
			if(v_iSelect==0)											//小区
				v_mCell.insert(m_cList_Cell.GetItemText(vi,1));			//增加：小区
			else														//性能
			{
				//更新：性能状态
				v_sType=m_cList_Cell.GetItemText(vi,1);					//性能类型
				v_sPM_Name=m_cList_Cell.GetItemText(vi,2);				//性能名称
				v_sAlarm_Flag=m_cList_Cell.GetItemText(vi,3);			//预警标志
				v_sAlarm_Value=m_cList_Cell.GetItemText(vi,4);			//预警值
				v_sAlarm_Type=m_cList_Cell.GetItemText(vi,5);			//预警类型

				v_pLTE_PM_Type=v_mLTE_PM_Type->begin();					//首指针
				while(v_pLTE_PM_Type!=v_mLTE_PM_Type->end())			//遍历
				{
					for(v_pPM_Name=v_pLTE_PM_Type->second.v_mLTE_PM.begin();v_pPM_Name!=v_pLTE_PM_Type->second.v_mLTE_PM.end();v_pPM_Name++)
					{
						if(v_pLTE_PM_Type->first==v_sType && v_pPM_Name->second.v_sPM_Name==v_sPM_Name)		//相等：性能名称？？？
						{
							v_pPM_Name->second.v_bOK=1;					//置位：选中状态
							v_pPM_Name->second.v_iNum=vi;				//获取：列序号
							v_pPM_Name->second.v_sAlarm_Flag=v_sAlarm_Flag;			//获取：预警标志
							v_pPM_Name->second.v_dAlarm_Value=atof(v_sAlarm_Value);	//获取：预警值
							v_pPM_Name->second.v_sAlarm_Type=v_sAlarm_Type;			//获取：预警类型
							break;
						}
					}
					v_pLTE_PM_Type++;
				}
			}
		}
	}

	//【3】退出
	OnOK();
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：点击模板响应
//------------------------------------------------------------------------------------------------------
void LTE_PM_DATA_CASE::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString		v_sAccount,v_sName;										//临时变量

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;					//获得：条目指针
	if (pNMListView->iItem>=0)											//判断：条目序号有效？？？
	{
		if(v_iList_Item>=0)												//有效条目序号？？？
			m_cList.SetItemBkColor(v_iList_Item,RGB(255,255,255));		//恢复背景色
		v_iList_Item=pNMListView->iItem;								//获得：选中条目序号
		m_cList.SetItemBkColor(v_iList_Item,RGB(255,0,0));				//设置背景色：红色

		//获取：模板信息

		m_cList_Cell.DeleteAllItems();									//清除列表数据
		v_sAccount=m_cList.GetItemText(v_iList_Item,1);					//帐号
		v_sName=m_cList.GetItemText(v_iList_Item,2);					//模板名称
		My_LoadData_Case_All(v_sAccount,v_sName);						//获取明细(小区或性能)
	}

	*pResult = 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：装载模板信息
//------------------------------------------------------------------------------------------------------			
void LTE_PM_DATA_CASE::My_LoadData_Case()
{
	CString		v_sSql;													//临时变量
	int			v_iItem,v_iID=0;										//临时变量
	CString		v_sTemp,v_sName,v_sAccount;								//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		if(v_iSelect==0)												//小区
			v_sSql="select zAccount,zName from CON_Pm_LTE_Case_Cell where zAccount='共享' or zAccount='"
					+theApp.a_sUser_Account+"' order by zAccount,zName";//查询数据库
		else															//性能
			v_sSql="select zAccount,zName from CON_Pm_LTE_Case_PM where zAccount='共享' or zAccount='"
					+theApp.a_sUser_Account+"' order by zAccount,zName";//查询数据库

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			v_sAccount=v_cAdoConn.GetField_String("zAccount");			//zAccount
			v_sName=v_cAdoConn.GetField_String("zName");				//zName
			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList.InsertItem(0xffff,v_sTemp);					//加入列表窗
			m_cList.SetItemText(v_iItem,1,(_bstr_t)v_sAccount);			//帐号
			m_cList.SetItemText(v_iItem,2,(_bstr_t)v_sName);			//模板名称

			v_pRecordset->MoveNext();									//
			v_iID++;	
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
//	函数功能：加载模板详细信息【小区或性能】
//	参    数：v_sName：模板名称；
//------------------------------------------------------------------------------------------------------
void LTE_PM_DATA_CASE::My_LoadData_Case_All(CString v_sAccount,CString v_sName)
{
	CString		v_sSql,v_sCell,v_sType,v_sPM_Name;						//临时变量
	int			v_iItem,v_iID=0,v_iTemp;								//临时变量
	CString		v_sTemp;												//临时变量
	double		v_dTemp;												//临时变量
	CString		v_sAlarm_Value,v_sAlarm_Type,v_sAlarm_Flag;				//临时变量

	_RecordsetPtr v_pRecordset;											//记录集
	CMyADOConn v_cAdoConn;												//数据库实例
	try
	{
		// 连接数据库
		v_cAdoConn.OnInitADOConn(theApp.a_iDatabase_Type,theApp.a_sDatabase_Ip,theApp.a_sDatabase_Name,theApp.a_sDatabase_User,theApp.a_sDatabase_Password);
		v_cAdoConn.SetCommandTimeout(100);

		if(v_iSelect==0)												//小区
			v_sSql="select * from CON_Pm_LTE_Case_Cell_Data where zAccount='"
				+v_sAccount+"'and zName='"+v_sName+"' order by zCell";	//查询数据库
		else															//性能
			v_sSql="select * from CON_Pm_LTE_Case_PM_Data where zAccount='"
				+v_sAccount+"'and zName='"+v_sName+"' order by zNum";	//查询数据库

		v_pRecordset = v_cAdoConn.GetRecordSet((_bstr_t)v_sSql);		//执行SELETE语句										

		while(!v_pRecordset->adoEOF)
		{
			if(v_iSelect==0)											//小区
				v_sCell=v_cAdoConn.GetField_String("zCell");			//zCell
			else
			{															//性能
				v_sType=v_cAdoConn.GetField_String("zType");			//zType
				v_sPM_Name=v_cAdoConn.GetField_String("zPM_Name");		//zPM_Name
				v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Flag");			//zAlarm_Flag
				v_sAlarm_Flag=(v_iTemp==1?"预警":"不预警");						
				v_dTemp=v_cAdoConn.GetField_Double("zAlarm_Value");		//zAlarm_Value
				v_sAlarm_Value.Format("%8.2f",v_dTemp);
				v_iTemp=v_cAdoConn.GetField_Int("zAlarm_Type");			//zAlarm_Type
				v_sAlarm_Type=(v_iTemp==1?"高":"低");						
			}

			v_sTemp.Format(" %04d",v_iID+1);
			v_iItem=m_cList_Cell.InsertItem(0xffff,v_sTemp);			//加入列表窗
			if(v_iSelect==0)											//小区
				m_cList_Cell.SetItemText(v_iItem,1,(_bstr_t)v_sCell);	//小区
			else
			{															//性能
				m_cList_Cell.SetItemText(v_iItem,1,(_bstr_t)v_sType);	//类型
				m_cList_Cell.SetItemText(v_iItem,2,(_bstr_t)v_sPM_Name);//性能
				m_cList_Cell.SetItemText(v_iItem,3,v_sAlarm_Flag);		//预警标志
				m_cList_Cell.SetItemText(v_iItem,4,v_sAlarm_Value);		//预警值
				m_cList_Cell.SetItemText(v_iItem,5,v_sAlarm_Type);		//预警类型
			}
			m_cList_Cell.SetCheck(v_iID,TRUE);							//有效

			v_pRecordset->MoveNext();									//
			v_iID++;	
		}

		m_cCheck.SetCheck(true);										//有效：全选
		v_sTemp.Format("%d",v_iID);										//数量
		m_cSum.SetWindowText(v_sTemp);									//显示：数量

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
