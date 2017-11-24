
// Station.h : Station 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"													//主符号
#include "StationDoc.h"
#include "ChildFrm.h"
#include "Alarm_Message.h"


class	CImpIDispatch;													//COM接口：JS Call MFC


#define  PASSWORDRSAD           32341
#define  PASSWORDRSAN           276995897
#define  KEYRSAD                16843
#define  KEYRSAN                453130003
#define  POWERNUM               600
// CStaionApp:
// 有关此类的实现，请参阅 Station.cpp
//

class CStaionApp : public CWinAppEx
{
public:
	CStaionApp();

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
public:
	//VS2008系统自带
	virtual BOOL	InitInstance();										//
	UINT			m_nAppLook;											//
	BOOL			m_bHiColorIcons;									//
	
	//JS Call MFC
	virtual int			ExitInstance();									//退出
	CImpIDispatch*		m_pCustDisp;									//JS Call MFC

	//数据库参数【Station】
	CString				a_sDatabase_Ip;									//数据库服务器地址
	CString				a_sDatabase_Name;								//数据库名称
	CString				a_sDatabase_User;								//数据库用户
	CString				a_sDatabase_Password;							//数据库密码
	int					a_iDatabase_Type;								//数据库类型
	CString				a_sServer_Ip;									//服务器通信IP
	int					a_iServer_Port;									//服务器通信Port
	//第二个数据库【bsc_data】
	CString				a_sDatabase_GSM_Ip;								//数据库服务器地址
	CString				a_sDatabase_GSM_Name;							//数据库名称
	CString				a_sDatabase_GSM_User;							//数据库用户
	CString				a_sDatabase_GSM_Password;						//数据库密码

	//登录信息
	CString				a_sUser_Account;								//账号
    CString				a_sUser_Name;									//姓名
	CString				a_sUser_Password;								//密码
	CString				a_sUser_IP;										//网络地址IP
	bool				a_bUser_ReadOnly;								//只读权限
	bool				a_bUser_Update;									//更新权限
	bool				a_bUser_Delete;									//删除权限

	COleDateTime		a_tData_Default;								//默认：无效日期

	//地图路径
	CString				m_sHTTP_Map;									//默认Web路径

	//自定义变量
	CStaionDoc*			m_pdoc;											//多文档
	CMultiDocTemplate*	pDocTemplate;									//多文档模版
	mFrame_Window		v_mFrame;										//多文档窗体map容器

	CMyLogWriter		m_MyLog;										//写日志
	CString				m_sPath;										//默认文件路径

	//系统功能权限
	mUser_Power			a_mUser_Power;									//功能权限

	//服务器通信异常窗体
	CAlarm_Message*		a_pDlg_Alarm_Message;							//提示窗体：服务器信息

	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
public:
	//VS2008系统自带
	virtual void PreLoadState();										//
	virtual void LoadCustomState();										//
	virtual void SaveCustomState();										//
	afx_msg void OnAppAbout();											//系统帮助

	bool	DecrypteParam();											//密码解密

	DECLARE_MESSAGE_MAP()												//消息映射

	//自定义函数
	void CStaionApp::My_Load_MDI(CString v_sFrame,class v_cMDI);		//打开多文档

	afx_msg void OnLogSystem();											//
	afx_msg void OnLogBug();											//
	afx_msg void OnLogDatabase();										//
	afx_msg void OnUserLogin();											//
	afx_msg void OnUserSetup();											//
	afx_msg void OnUserPassword();										//
	afx_msg void OnUserRole();											//
	//-------------------------------------------------------------------------
	afx_msg void OnConfigStation();
	afx_msg void OnConfigRegion();
	afx_msg void OnConfigBts();
	afx_msg void OnConfigGsmCell();
	afx_msg void OnConfigLteCell();
	afx_msg void OnConfigENodeb();
	afx_msg void OnConfigTools();
	afx_msg void OnQuaryStationMap();
	afx_msg void OnPmLte();
	afx_msg void OnConfigPara();
	afx_msg void OnLtePmDraw();
	afx_msg void OnLtePmData();
	afx_msg void OnAllPmDraw();
	afx_msg void OnPmLteCal();
	afx_msg void OnPmLteCas();
	afx_msg void OnPmGsm();
	afx_msg void OnPmGsmCalCell();
	afx_msg void OnPmGsmCalBsc();
	afx_msg void OnLogDatabaseGsm();
	afx_msg void OnGsmPmDraw();
	afx_msg void OnGsmPmData();
	afx_msg void OnLtePmDrawInit();
	afx_msg void OnLtePmDataInit();
	afx_msg void OnAlarmPm();
	afx_msg void OnPmLteRegion();
	afx_msg void OnPmLteCasPm();
	afx_msg void OnLtePmDataReal();
	afx_msg void OnLtePmDataCell();
	afx_msg void OnPmLteAlarm();
	afx_msg void OnAlarmLlCell();
	afx_msg void OnCmLteQuary();
	afx_msg void OnCmLteFile();
	afx_msg void OnCmLtePara();
	afx_msg void OnLtePmDataCal();
	afx_msg void OnPmLteCasCal();
	afx_msg void OnQuaryStation();
	afx_msg void OnQuaryTdlte();
	afx_msg void OnQuaryGsm();
	afx_msg void OnQuaryGsmGt();
	afx_msg void OnAlarmLlReal();
	afx_msg void OnCmLteRelation();
};

extern CStaionApp theApp;

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
