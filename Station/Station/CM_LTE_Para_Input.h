#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CCM_LTE_Para_Input �Ի���

//TD_ELEMENT
struct s_CON_Location
{
	CString		v_sAddress;											//��ַ
	double		v_dLongitude;										//����
	double		v_dLatitude;										//γ��
};
typedef map <CString,s_CON_Location>	m_CON_Location;				//������[վ��]


class CCM_LTE_Para_Input : public CDialog
{
	DECLARE_DYNAMIC(CCM_LTE_Para_Input)

public:
	CCM_LTE_Para_Input(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCM_LTE_Para_Input();

// �Ի�������
	enum { IDD = IDD_CM_LTE_PARA_Input };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton6();

	//�Զ���
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����
	void		My_Input_Main();										//����_������
	void		My_File();												//�ļ�ѡ��
	bool		Get_CSV_Cell(CString v_sFile,mString_Int *v_mHead,bool v_bHead);		//����CSV�ļ�
	void		My_Input_Cell();										//Cell
	void		My_ShowList_Bug(CString v_sBug_Type,CString v_sBug);	//��ʾ���쳣��Ϣ

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����
	int			v_iThread_Exit;											//�߳��˳���ǣ�2��ǿ���˳���1��������0���߳�ֹͣ

	//�ִ�
	CSSplitter		m_SplitterPane_Main;								//��ֱ��

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;
	CMyListCtrl		m_cList_Bug;
	CEdit			m_cFile_Name;										//
	CButton			m_cFile;											//
	CButton			m_cInput;											//
	CString			v_sFilename;										//�ļ�����
	CEdit			m_cSum;
	CButton			m_cExcel;
	CButton			m_cFile_Open;
	CEdit			m_cMaker;											//

	//�Զ���
	int				v_iWorking;											//�������ţ�
	CString			v_sMaker_OK;										//����
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
