#pragma once
#include "afxwin.h"


// CUser_Login �Ի���

class CUser_Login : public CDialog
{
	DECLARE_DYNAMIC(CUser_Login)

public:
	CUser_Login(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUser_Login();

// �Ի�������
	enum { IDD = IDD_USER_LOGIN };

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

	//�Զ���
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����
	void		My_User_Online();										//�����û��Ǽ�

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CEdit	m_cAccount;
	CEdit	m_cPassword;

	//�Զ���

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
