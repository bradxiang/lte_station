#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"



// CLog_Bug ������ͼ

class CLog_Bug : public CFormView
{
	DECLARE_DYNCREATE(CLog_Bug)

protected:
	CLog_Bug();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLog_Bug();

public:
	enum { IDD = IDD_LOG_BUG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	//��Ļ
	virtual void OnInitialUpdate();										//��ʼ��
	afx_msg void OnDestroy();											//���ٴ���
	afx_msg void OnSize(UINT nType, int cx, int cy);					//�����ߴ�
	afx_msg void OnBnClickedQuary();									//��ѯ
	afx_msg void OnBnClickedExcel();									//����
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);//����
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ���� [��ȡ������Ϣ������]

	//�Զ���
	void		My_LoadData();											//װ����־��Ϣ

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����
	void		My_Input_Main();										//����_������

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;
	CEdit			m_cLog_Sum;
	CDateTimeCtrl	m_cLog_Date;
	CComboBox		m_cLog_Day;
	CEdit			m_cLog_Key;
	CButton			m_cQuery;
	CButton			m_cExcel;

	//�Զ���
	CString		v_sFrame_Name;											//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

