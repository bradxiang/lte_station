#pragma once
#include "afxwin.h"


// CUser_Password �Ի���

class CUser_Password : public CDialog
{
	DECLARE_DYNAMIC(CUser_Password)

public:
	CUser_Password(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUser_Password();

// �Ի�������
	enum { IDD = IDD_USER_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ����
	afx_msg void OnBnClickedOk();										//ȷ��
	afx_msg void OnBnClickedCancel();									//ȡ��


	//�Զ���
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CEdit	m_cPassword_Old;											//ԭ����
	CEdit	m_cPassword1;												//����1
	CEdit	m_cPassword2;												//����2

	//�Զ���

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
