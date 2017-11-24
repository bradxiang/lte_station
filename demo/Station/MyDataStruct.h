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

//����ͷ�ļ�
#include "MyADOConn.h"													//ADO��ȡ��
#include "MyExcel.h"													//CSV�ļ�������
#include "MyListCtrl.h"													//�б�������
#include "MyLogWriter.h"												//д��־��
#include "MySplitter.h"

//ϵͳ���в�������������
#define c_iSample_Day		60											//��������ɲ�ѯ����
#define c_iSample_Size		60*96										//���ȣ�����*����
#define c_iSample_Sum_Hour	24											//������ÿ��
#define c_iSample_Sum_Min	96											//������ÿ��

//----------------------------------------------------------------------------------------------------------
//	�������壺ϵͳ
//----------------------------------------------------------------------------------------------------------

//���ĵ����嶨��	[ʹ�ã�CRTSPAAApp]
struct	Frame_Window
{
	CChildFrame*	v_pChildFrame;										//����ָ��
	int				v_iThread_Exit;										//�߳�״̬����0����ʼ��1������	2��ǿ���˳���
};
typedef	map<CString,Frame_Window>	mFrame_Window;						//���ĵ�����map������

//�û�ʹ��Ȩ�޶���	[ʹ�ã�CMainFrame��CUser_Login]
typedef	map<CString,bool> mUser_Power;									//map������mUser_Power [��¼�û�ʹ��Ȩ����Ч���]

//User_Role			[ʹ�ã�User_Setup]
typedef	map<CString,int> mUser_Role;									//map������mUser_Role [��¼Role������Role_ID�Ĺ�ϵ]

//�û�Ȩ����������   [ʹ�ã��û�Ȩ������]
struct	USER_Power
{
	CString	v_sPower;													//Ȩ������
	bool	v_bOk;														//Ȩ����Ч��־
};
typedef	map<int,USER_Power>	mUSER_Power;								//map������mUSER_Power [Ȩ��ID��Ȩ�����Ƽ���Ч��ӳ���ϵ]

//----------------------------------------------------------------------------------------------------------
//	�������壺ͨ������
//----------------------------------------------------------------------------------------------------------

//mString_Bool		[ͨ��������CString_Bool]
typedef	map<CString,bool> mString_Bool;									//map������mString_Bool

//mString_Int		[ͨ��������CString_Int]
typedef	map<CString,int> mString_Int;									//map������mString_Int

//mString_String	[ͨ��������CString_String]
typedef	map<CString,CString> mString_String;							//map������mString_String

//mInt_Int			[ͨ��������CInt_Int]
typedef	map<int,int> mInt_Int;											//map������mInt_Int

//mInt_String		[ͨ��������CInt_String]
typedef	map<int,CString> mInt_String;									//map������mInt_String

//mString_Double		[ͨ��������CString_Double]
typedef	map<CString,double> mString_Double;								//map������mString_Double

//mDate_Int			[ͨ��������Date_Int]
typedef	map<COleDateTime,int>	mDate_Int;								//map������mDate_Int

//------------------------------------------------------------------------------------------------------
//	ASN/XML�ļ������ṹ����
//------------------------------------------------------------------------------------------------------
//Object�ṹ�嶨��
struct File_Object														//Object�ṹ��
{
	CString			v_sObject_ID;										//Object_ID
	CString			v_sObject_DN;										//Object_DN
	mString_String	v_mCounter;											//COunter����
};
typedef	list<File_Object>					list_Object;				//list_Object����
//ObjType�ṹ�嶨��
typedef map<CString,list_Object>			map_ObjType;				//Objtype��map����

//LTE ���ܶ�ȡ-------------------------------------------------------------------------------------------
typedef map<CString,mString_String>			map_C_Data;					//������map_C_Data��Cell-Counter���
typedef map<CString,map_C_Data>				map_Child;					//������map_Child������������

//Object�ṹ�嶨��
struct File_Object_Child												//Object_Child�ṹ��
{
	list_Object		v_lObject;											//Object����
	map_Child		v_mChild;											//Child����
};
//ObjType�ṹ�嶨��
typedef map<CString,File_Object_Child>		map_ObjType_Child;			//Objtype_Child��map����

//------------------------------------------------------------------------------------------------------
//	��������
//------------------------------------------------------------------------------------------------------

//��ͼ
typedef	map<double,int> mTime_Array;									//map������mTime_Array [��¼����������λ�ù�ϵ]

struct	Chart_Data
{
	CString	v_sName;													//��������
	double*	v_dChart_Value;												//������ֵ[�������ȵ�����]
	int		v_iColor;													//��ͼ��ɫ
};
typedef	map<CString,Chart_Data> mChart_Data;							//map��������ͼ����


//----------------------------------------------------------------------------------------------------------
//	�������壺ҵ��
//----------------------------------------------------------------------------------------------------------

//������Station_Tj
struct  Station_Tj_Struct
{  
	mString_Int		v_mGSM;												//GSM_��վ�������ڡ�
	mString_Int		v_mGSM_OUT;											//GSM_��վ�������⡿
	mString_Int		v_mTD;												//TD_��վ�������ڡ�
	mString_Int		v_mTD_OUT;											//TD_��վ�������⡿
	mString_Int		v_mLTE;												//LTE_��վ�������ڡ�
	mString_Int		v_mLTE_OUT;											//LTE_��վ�������⡿
};  
typedef map<CString,Station_Tj_Struct> mStation_Tj;						//������mStation_Tj

//������Station_OutPut_BTS
struct  Station_OutPut_BTS_Struct
{  
	CString			v_sName;											//������
	CString			v_sNetwork;											//�������ͣ�2��3��4
	CString			v_sStation;											//վַ
	mString_String	v_mCell;											//Cell����
};  
typedef map<CString,Station_OutPut_BTS_Struct> mStation_OutPut_BTS;		//������mStation_OutPut_BTS

//������Station_OutPut
struct  Station_OutPut_Struct
{  
	CString					v_sRegion;									//����
	bool					v_bNet_GSM;									//��ַ���ࣺGSM
	bool					v_bNet_TD;									//��ַ���ࣺTD
	bool					v_bNet_LTE;									//��ַ���ࣺLTE
	mStation_OutPut_BTS		v_mGSM;										//GSM_��վ
	mStation_OutPut_BTS		v_mTD;										//TD_��վ
	mStation_OutPut_BTS		v_mLTE;										//LTE_��վ
};  
typedef map<CString,Station_OutPut_Struct> mStation_OutPut;				//������mStation_OutPut

//������Node_Cell
struct  Node_Cell_Struct
{  
	CString					v_sCell;									//Cell
	CString					v_sNetwork;									//����
};  
typedef map<HTREEITEM,Node_Cell_Struct> mNode_Cell;						//������mNode_Cell

//������Performance_Select
struct  Performance_Select_Struct
{  
	CString					v_sField;									//�ֶ�
	bool					v_bOK;										//��Ч
};  
typedef map<CString,Performance_Select_Struct> mPerformance_Select;		//������mPerformance_Select

//������Cell_Performance
struct  Cell_Performance_Struct
{  
	mString_Double			v_mPerformance;								//��������
};  
typedef map<CString,Cell_Performance_Struct> mCell_Performance;			//������mCell_Performance
struct  DateTime_Performance_Struct
{  
	mCell_Performance	v_mCell;										//С����������
};  
typedef map<COleDateTime,DateTime_Performance_Struct> mDateTime_Performance;	//������mDateTime_Performance

//������Performance_Tj
struct  Performance_Tj_Struct
{  
	double			v_dTotal;											//���ܺϼ�
	int				v_iSum;												//ʱ�κϼ�
	double			v_dMax;												//�������
	COleDateTime	v_dTime;											//�������ʱ��
};  
typedef map<CString,Performance_Tj_Struct> mPerformance_Tj;				//������mPerformance_Tj

//������Cell_Performance_Tj
struct  Cell_Performance_Tj_Struct
{  
	mPerformance_Tj			v_mPerformance_Tj;							//��������
};  
typedef map<CString,Cell_Performance_Tj_Struct> mCell_Performance_Tj;	//������mCell_Performance_Tj

//----------------------------------------------------------------------------------------------------------
//������LTE_PM
struct  LTE_PM_Struct
{  
	bool			v_bOK;												//ѡ��״̬
	CString			v_sType;											//��������
	CString			v_sPM_Name;											//����������
	CString			v_sTable_Key;										//������-Key
	CString			v_sAlarm_Flag;										//Ԥ����־
	double			v_dAlarm_Value;										//Ԥ������
	CString			v_sAlarm_Type;										//Ԥ������
	int				v_iNum;												//�����
};  
typedef map<CString,LTE_PM_Struct> mLTE_PM;								//������mLTE_PM

struct  LTE_PM_Type_Struct
{  
	CString			v_sTable_Key;										//������-Key
	mLTE_PM			v_mLTE_PM;											//��������
};  
typedef map<CString,LTE_PM_Type_Struct> mLTE_PM_Type;					//������mLTE_PM_Type

//----------------------------------------------------------------------------------------------------------
//������GSM_PM
struct  GSM_PM_Struct
{  
	CString			v_sType;											//��������
	CString			v_sNum;												//Ψһ��ʶ
};  
typedef map<CString,GSM_PM_Struct> mGSM_PM;								//������mGSM_PM

//LTE ���ܶ�ȡ-------------------------------------------------------------------------------------------
typedef map<COleDateTime,mString_Double>	map_PM_Time;				//������map_Time��ʱ�䡿
typedef map<CString,map_PM_Time>			map_PM_Cell;				//����������_Cell

//LTE ����ͳ��-------------------------------------------------------------------------------------------
struct Time_PM_Data
{
	int				v_iSum;												//ͳ�ƴ���
	mString_Double	v_mPM_Data;											//����
};
typedef map<CString,Time_PM_Data>			map_PM_Time_P;				//������map_Time_P��ʱ�Ρ�
typedef map<CString,map_PM_Time_P>			map_PM_Cell_P;				//����������_Cell_P

//С����ѯ����ʱ��ѡ��
struct Date_Select
{
	bool			v_bFlag;											//��Ч��־
	COleDateTime	v_dDate;											//����
	int				v_iDay;												//����
};

struct Time_Select
{
	bool			v_bFlag;											//��Ч��־
	int				v_iHour_1;											//��ʼСʱ
	int				v_iHour_2;											//����Сʱ
};

//----------------------------------------------------------------------------------------------------------
//	End
//----------------------------------------------------------------------------------------------------------
