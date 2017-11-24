#pragma once
#include "afxwin.h"


// CCON_Station_Region �Ի���

class CCON_Station_Region : public CDialog
{
	DECLARE_DYNAMIC(CCON_Station_Region)

public:
	CCON_Station_Region(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCON_Station_Region();

// �Ի�������
	enum { IDD = IDD_CON_Station_Region };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ����
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����
	int			v_iThread_Exit;											//�߳��˳���ǣ�2��ǿ���˳���1��������0���߳�ֹͣ

	//�Զ���
	void		My_Input_Main();										//����_������
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����
	void		My_Region();											//����ͬ��

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CEdit	m_cAccount;
	CEdit	m_cPassword;
	CButton m_cRegion;

	//�Զ���
	int				v_iWorking;											//�������ţ�

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
