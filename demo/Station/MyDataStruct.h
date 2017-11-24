#pragma once
#include <afxtempl.h>
#include <afxmt.h>
#include "ChildFrm.h"
#include <map>
#include <set>
#include "afxwin.h"
using namespace std; 

#include <string>
#include <list>
#include <iostream>  
#include <fstream> 
#include <sstream>
#include <math.h>
#include <vector>
#include <stack>
using std::stack;

//公共头文件
#include "MyADOConn.h"													//ADO读取类
#include "MyExcel.h"													//CSV文件导出类
#include "MyListCtrl.h"													//列表窗排序类
#include "MyLogWriter.h"												//写日志类
#include "MySplitter.h"

//系统运行参数：常量定义
#define c_iSample_Day		60											//天数：最长可查询天数
#define c_iSample_Size		60*96										//粒度：天数*点数
#define c_iSample_Sum_Hour	24											//点数：每天
#define c_iSample_Sum_Min	96											//点数：每天

//----------------------------------------------------------------------------------------------------------
//	容器定义：系统
//----------------------------------------------------------------------------------------------------------

//多文档窗体定义	[使用：CRTSPAAApp]
struct	Frame_Window
{
	CChildFrame*	v_pChildFrame;										//窗体指针
	int				v_iThread_Exit;										//线程状态：　0：初始；1：运行	2：强制退出；
};
typedef	map<CString,Frame_Window>	mFrame_Window;						//多文档窗体map容器类

//用户使用权限定义	[使用：CMainFrame、CUser_Login]
typedef	map<CString,bool> mUser_Power;									//map容器：mUser_Power [记录用户使用权限有效与否]

//User_Role			[使用：User_Setup]
typedef	map<CString,int> mUser_Role;									//map容器：mUser_Role [记录Role名称与Role_ID的关系]

//用户权限设置容器   [使用：用户权限设置]
struct	USER_Power
{
	CString	v_sPower;													//权限名称
	bool	v_bOk;														//权限有效标志
};
typedef	map<int,USER_Power>	mUSER_Power;								//map容器：mUSER_Power [权限ID与权限名称及有效的映射关系]

//----------------------------------------------------------------------------------------------------------
//	容器定义：通用容器
//----------------------------------------------------------------------------------------------------------

//mString_Bool		[通用容器：CString_Bool]
typedef	map<CString,bool> mString_Bool;									//map容器：mString_Bool

//mString_Int		[通用容器：CString_Int]
typedef	map<CString,int> mString_Int;									//map容器：mString_Int

//mString_String	[通用容器：CString_String]
typedef	map<CString,CString> mString_String;							//map容器：mString_String

//mInt_Int			[通用容器：CInt_Int]
typedef	map<int,int> mInt_Int;											//map容器：mInt_Int

//mInt_String		[通用容器：CInt_String]
typedef	map<int,CString> mInt_String;									//map容器：mInt_String

//mString_Double		[通用容器：CString_Double]
typedef	map<CString,double> mString_Double;								//map容器：mString_Double

//mDate_Int			[通用容器：Date_Int]
typedef	map<COleDateTime,int>	mDate_Int;								//map容器：mDate_Int

//------------------------------------------------------------------------------------------------------
//	ASN/XML文件变量结构定义
//------------------------------------------------------------------------------------------------------
//Object结构体定义
struct File_Object														//Object结构体
{
	CString			v_sObject_ID;										//Object_ID
	CString			v_sObject_DN;										//Object_DN
	mString_String	v_mCounter;											//COunter内容
};
typedef	list<File_Object>					list_Object;				//list_Object链表
//ObjType结构体定义
typedef map<CString,list_Object>			map_ObjType;				//Objtype的map容器

//LTE 性能读取-------------------------------------------------------------------------------------------
typedef map<CString,mString_String>			map_C_Data;					//容器：map_C_Data【Cell-Counter子项】
typedef map<CString,map_C_Data>				map_Child;					//容器：map_Child【子项容器】

//Object结构体定义
struct File_Object_Child												//Object_Child结构体
{
	list_Object		v_lObject;											//Object内容
	map_Child		v_mChild;											//Child内容
};
//ObjType结构体定义
typedef map<CString,File_Object_Child>		map_ObjType_Child;			//Objtype_Child的map容器

//------------------------------------------------------------------------------------------------------
//	容器设置
//------------------------------------------------------------------------------------------------------

//绘图
typedef	map<double,int> mTime_Array;									//map容器：mTime_Array [记录粒度与数组位置关系]

struct	Chart_Data
{
	CString	v_sName;													//曲线名称
	double*	v_dChart_Value;												//曲线数值[各个粒度的数据]
	int		v_iColor;													//绘图颜色
};
typedef	map<CString,Chart_Data> mChart_Data;							//map容器：绘图数据


//----------------------------------------------------------------------------------------------------------
//	容器定义：业务
//----------------------------------------------------------------------------------------------------------

//容器：Station_Tj
struct  Station_Tj_Struct
{  
	mString_Int		v_mGSM;												//GSM_基站数【室内】
	mString_Int		v_mGSM_OUT;											//GSM_基站数【室外】
	mString_Int		v_mTD;												//TD_基站数【室内】
	mString_Int		v_mTD_OUT;											//TD_基站数【室外】
	mString_Int		v_mLTE;												//LTE_基站数【室内】
	mString_Int		v_mLTE_OUT;											//LTE_基站数【室外】
};  
typedef map<CString,Station_Tj_Struct> mStation_Tj;						//容器：mStation_Tj

//容器：Station_OutPut_BTS
struct  Station_OutPut_BTS_Struct
{  
	CString			v_sName;											//中文名
	CString			v_sNetwork;											//网络类型：2、3、4
	CString			v_sStation;											//站址
	mString_String	v_mCell;											//Cell容器
};  
typedef map<CString,Station_OutPut_BTS_Struct> mStation_OutPut_BTS;		//容器：mStation_OutPut_BTS

//容器：Station_OutPut
struct  Station_OutPut_Struct
{  
	CString					v_sRegion;									//区域
	bool					v_bNet_GSM;									//共址分类：GSM
	bool					v_bNet_TD;									//共址分类：TD
	bool					v_bNet_LTE;									//共址分类：LTE
	mStation_OutPut_BTS		v_mGSM;										//GSM_基站
	mStation_OutPut_BTS		v_mTD;										//TD_基站
	mStation_OutPut_BTS		v_mLTE;										//LTE_基站
};  
typedef map<CString,Station_OutPut_Struct> mStation_OutPut;				//容器：mStation_OutPut

//容器：Node_Cell
struct  Node_Cell_Struct
{  
	CString					v_sCell;									//Cell
	CString					v_sNetwork;									//网络
};  
typedef map<HTREEITEM,Node_Cell_Struct> mNode_Cell;						//容器：mNode_Cell

//容器：Performance_Select
struct  Performance_Select_Struct
{  
	CString					v_sField;									//字段
	bool					v_bOK;										//有效
};  
typedef map<CString,Performance_Select_Struct> mPerformance_Select;		//容器：mPerformance_Select

//容器：Cell_Performance
struct  Cell_Performance_Struct
{  
	mString_Double			v_mPerformance;								//性能容器
};  
typedef map<CString,Cell_Performance_Struct> mCell_Performance;			//容器：mCell_Performance
struct  DateTime_Performance_Struct
{  
	mCell_Performance	v_mCell;										//小区性能容器
};  
typedef map<COleDateTime,DateTime_Performance_Struct> mDateTime_Performance;	//容器：mDateTime_Performance

//容器：Performance_Tj
struct  Performance_Tj_Struct
{  
	double			v_dTotal;											//性能合计
	int				v_iSum;												//时段合计
	double			v_dMax;												//最大性能
	COleDateTime	v_dTime;											//最大性能时段
};  
typedef map<CString,Performance_Tj_Struct> mPerformance_Tj;				//容器：mPerformance_Tj

//容器：Cell_Performance_Tj
struct  Cell_Performance_Tj_Struct
{  
	mPerformance_Tj			v_mPerformance_Tj;							//性能容器
};  
typedef map<CString,Cell_Performance_Tj_Struct> mCell_Performance_Tj;	//容器：mCell_Performance_Tj

//----------------------------------------------------------------------------------------------------------
//容器：LTE_PM
struct  LTE_PM_Struct
{  
	bool			v_bOK;												//选择状态
	CString			v_sType;											//性能类型
	CString			v_sPM_Name;											//性能中文名
	CString			v_sTable_Key;										//表名称-Key
	CString			v_sAlarm_Flag;										//预警标志
	double			v_dAlarm_Value;										//预警门限
	CString			v_sAlarm_Type;										//预警类型
	int				v_iNum;												//列序号
};  
typedef map<CString,LTE_PM_Struct> mLTE_PM;								//容器：mLTE_PM

struct  LTE_PM_Type_Struct
{  
	CString			v_sTable_Key;										//表名称-Key
	mLTE_PM			v_mLTE_PM;											//性能容器
};  
typedef map<CString,LTE_PM_Type_Struct> mLTE_PM_Type;					//容器：mLTE_PM_Type

//----------------------------------------------------------------------------------------------------------
//容器：GSM_PM
struct  GSM_PM_Struct
{  
	CString			v_sType;											//性能类型
	CString			v_sNum;												//唯一标识
};  
typedef map<CString,GSM_PM_Struct> mGSM_PM;								//容器：mGSM_PM

//LTE 性能读取-------------------------------------------------------------------------------------------
typedef map<COleDateTime,mString_Double>	map_PM_Time;				//容器：map_Time【时间】
typedef map<CString,map_PM_Time>			map_PM_Cell;				//容器：性能_Cell

//LTE 性能统计-------------------------------------------------------------------------------------------
struct Time_PM_Data
{
	int				v_iSum;												//统计次数
	mString_Double	v_mPM_Data;											//性能
};
typedef map<CString,Time_PM_Data>			map_PM_Time_P;				//容器：map_Time_P【时段】
typedef map<CString,map_PM_Time_P>			map_PM_Cell_P;				//容器：性能_Cell_P

//小区查询日期时段选择
struct Date_Select
{
	bool			v_bFlag;											//有效标志
	COleDateTime	v_dDate;											//日期
	int				v_iDay;												//天数
};

struct Time_Select
{
	bool			v_bFlag;											//有效标志
	int				v_iHour_1;											//开始小时
	int				v_iHour_2;											//结束小时
};

//----------------------------------------------------------------------------------------------------------
//	End
//----------------------------------------------------------------------------------------------------------
