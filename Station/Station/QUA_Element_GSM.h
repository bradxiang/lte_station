#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CQUA_Element_GSM ������ͼ

class CQUA_Element_GSM : public CFormView
{
	DECLARE_DYNCREATE(CQUA_Element_GSM)

protected:
	CQUA_Element_GSM();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CQUA_Element_GSM();

public:
	enum { IDD = IDD_QUA_ELEMENT_GSM };
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
	virtual void OnInitialUpdate();										//��ʼ��
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ����
	afx_msg void OnDestroy();											//
	afx_msg void OnSize(UINT nType, int cx, int cy);					//
	afx_msg void OnBnClickedQuary();									//
	afx_msg void OnBnClickedExcel();									//����
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);//�б�����
	afx_msg void OnBnClickedExcel2();
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);

	//�Զ���
	void		My_Query_BTS();											//��ѯ��BTS
	void		My_Query_Cell();										//��ѯ��Cell
	void		My_Query_BTS_Cell();									//��ѯ��BTS_Cell
	void		My_LoadData_Cell_Data(CString v_sSql);					//װ��Cell��Ϣ
	void		My_LoadData_BTS_Data(CString v_sSql);					//װ��BTS��Ϣ

	//�ִ�
	CSSplitter		m_SplitterPane_Main;								//��ֱ��
	CSSplitter		m_SplitterPane_Right;								//ˮƽ��

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����
	void		My_Input_Main();										//����_������

	//------------------------------------------------------------------------------------------------------
	//	��������
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//List��С����ϸ��Ϣ
	CMyListCtrl		m_cList_NodeB;										//NodeB��Ϣ
	CEdit			m_cSum;												//��Ԫ����
	CEdit			m_cKey;												//Key
	CComboBox		m_cKey_Select;
	CEdit			m_cSum_NodeB;
	CButton			m_cQuery;
	CButton			m_cExcel;
	CButton			m_cExcel_NodeB;
	CButton			m_cBTS;

	//�Զ���
	CString			v_sFrame_Name;										//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item;										//ѡ�е�BTS�б�Item
	afx_msg void OnLvnGetdispinfoList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult);
};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
