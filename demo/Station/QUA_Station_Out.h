#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"



// CQUA_Station_Out ������ͼ

class CQUA_Station_Out : public CFormView
{
	DECLARE_DYNCREATE(CQUA_Station_Out)

protected:
	CQUA_Station_Out();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CQUA_Station_Out();

public:
	enum { IDD = IDD_QUA_STATION_OUT };
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
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);	//��Item�仯��Ӧ
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedExcel();
	afx_msg void OnBnClickedButton2();

	//�Զ���
	void		My_Query_Station();										//��ѯ��վַ
	void		My_LoadData_Station_Data(CString v_sSql);				//װ��վַ��Ϣ
	void		My_LoadData_Tree(CString v_sRoom);						//װ�ػ�����Ϣ
	void		My_LoadData_Tree_Node_Cell(CString v_sNodeb,HTREEITEM v_hBTS_Node);		//��ȡ��С����Ϣ��Nodeb��
	void		My_OutPut();											//��ϸ����
	void		MyOutPut_Station(mStation_OutPut *v_mStation_OutPut,CString v_sFile_Name,bool v_bFile_Open);	//���CSV�ļ�

	//�ִ�
	CSSplitter		m_SplitterPane_Main;								//��ֱ��
	CSSplitter		m_SplitterPane_Bottom;								//��ֱ��

	//m_cTree
	CMyMutiTreeCtrl	m_cTree_Station;
	CImageList		v_cImage_Tree;										//��ͼ�꣺����
	CImageList		v_cImage_State;										//��ͼ�꣺״̬
	int				v_iItem_Tree;										//ѡ�е�Tree��Ŀ���
	HTREEITEM		v_hBTS_Root,v_hBTS_Node;							//��Ŀ¼������ڵ�����

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����
	void		My_Input_Main();										//����_������

	//------------------------------------------------------------------------------------------------------
	//	��������
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList_Station;									//Station��Ϣ
	CEdit			m_cSum;												//��Ԫ����
	CEdit			m_cKey;												//Key
	CComboBox		m_cKey_Select;										//
	CButton			m_cQuery;											//
	CButton			m_cExcel;											//
	CButton			m_cOnline;											//����ѡ��
	CButton			m_cOK;												//��Чվַ
	CButton			m_cDetail;											//վַ����

	//�Զ���
	CString			v_sFrame_Name;										//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item_Room;									//ѡ�е��б�Item [Room]
};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
