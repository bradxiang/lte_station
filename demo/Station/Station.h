
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
	CImpIDispatch*		m_pCustDisp;									//JS Call MFC
	virtual int			ExitInstance();									//�˳�

	//���ݿ����
	CString				a_sDatabase_Ip;									//���ݿ��������ַ
	CString				a_sDatabase_Name;								//���ݿ�����
	CString				a_sDatabase_User;								//���ݿ��û�
	CString				a_sDatabase_Password;							//���ݿ�����
	int					a_iDatabase_Type;								//���ݿ�����
	CString				a_sServer_Ip;									//������ͨ��IP
	int					a_iServer_Port;									//������ͨ��Port

	//��¼��Ϣ
	CString				a_sUser_Account;								//�˺�
    CString				a_sUser_Name;									//����
	CString				a_sUser_Password;								//����
	CString				a_sUser_IP;										//�����ַIP
	bool				a_bUser_ReadOnly;								//ֻ��Ȩ��
	bool				a_bUser_Update;									//����Ȩ��
	bool				a_bUser_Delete;									//ɾ��Ȩ��

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
	afx_msg void OnLogSystem();											//
	afx_msg void OnLogBug();											//
	afx_msg void OnLogDatabase();										//
	afx_msg void OnUserLogin();											//
	afx_msg void OnUserSetup();											//
	afx_msg void OnUserPassword();										//
	afx_msg void OnUserRole();											//
	//-------------------------------------------------------------------------
	afx_msg void OnConStationMap();
	afx_msg void OnConLteIndicator();
	afx_msg void OnConAnalyzeIndicator();
};

extern CStaionApp theApp;

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
