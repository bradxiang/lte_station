
// Station.h : Station Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"													//������
#include "StationDoc.h"
#include "ChildFrm.h"
#include "Alarm_Message.h"


class	CImpIDispatch;													//COM�ӿڣ�JS Call MFC


#define  PASSWORDRSAD           32341
#define  PASSWORDRSAN           276995897
#define  KEYRSAD                16843
#define  KEYRSAN                453130003
#define  POWERNUM               600
// CStaionApp:
// �йش����ʵ�֣������ Station.cpp
//

class CStaionApp : public CWinAppEx
{
public:
	CStaionApp();

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
public:
	//VS2008ϵͳ�Դ�
	virtual BOOL	InitInstance();										//
	UINT			m_nAppLook;											//
	BOOL			m_bHiColorIcons;									//
	
	//JS Call MFC
	virtual int			ExitInstance();									//�˳�
	CImpIDispatch*		m_pCustDisp;									//JS Call MFC

	//���ݿ������Station��
	CString				a_sDatabase_Ip;									//���ݿ��������ַ
	CString				a_sDatabase_Name;								//���ݿ�����
	CString				a_sDatabase_User;								//���ݿ��û�
	CString				a_sDatabase_Password;							//���ݿ�����
	int					a_iDatabase_Type;								//���ݿ�����
	CString				a_sServer_Ip;									//������ͨ��IP
	int					a_iServer_Port;									//������ͨ��Port
	//�ڶ������ݿ⡾bsc_data��
	CString				a_sDatabase_GSM_Ip;								//���ݿ��������ַ
	CString				a_sDatabase_GSM_Name;							//���ݿ�����
	CString				a_sDatabase_GSM_User;							//���ݿ��û�
	CString				a_sDatabase_GSM_Password;						//���ݿ�����

	//��¼��Ϣ
	CString				a_sUser_Account;								//�˺�
    CString				a_sUser_Name;									//����
	CString				a_sUser_Password;								//����
	CString				a_sUser_IP;										//�����ַIP
	bool				a_bUser_ReadOnly;								//ֻ��Ȩ��
	bool				a_bUser_Update;									//����Ȩ��
	bool				a_bUser_Delete;									//ɾ��Ȩ��

	COleDateTime		a_tData_Default;								//Ĭ�ϣ���Ч����

	//��ͼ·��
	CString				m_sHTTP_Map;									//Ĭ��Web·��

	//�Զ������
	CStaionDoc*			m_pdoc;											//���ĵ�
	CMultiDocTemplate*	pDocTemplate;									//���ĵ�ģ��
	mFrame_Window		v_mFrame;										//���ĵ�����map����

	CMyLogWriter		m_MyLog;										//д��־
	CString				m_sPath;										//Ĭ���ļ�·��

	//ϵͳ����Ȩ��
	mUser_Power			a_mUser_Power;									//����Ȩ��

	//������ͨ���쳣����
	CAlarm_Message*		a_pDlg_Alarm_Message;							//��ʾ���壺��������Ϣ

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
public:
	//VS2008ϵͳ�Դ�
	virtual void PreLoadState();										//
	virtual void LoadCustomState();										//
	virtual void SaveCustomState();										//
	afx_msg void OnAppAbout();											//ϵͳ����

	bool	DecrypteParam();											//�������

	DECLARE_MESSAGE_MAP()												//��Ϣӳ��

	//�Զ��庯��
	void CStaionApp::My_Load_MDI(CString v_sFrame,class v_cMDI);		//�򿪶��ĵ�

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
