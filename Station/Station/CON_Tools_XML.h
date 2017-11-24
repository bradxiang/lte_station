#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyFile_XML.h"


// CCON_Tools_XML ������ͼ

class CCON_Tools_XML : public CFormView
{
	DECLARE_DYNCREATE(CCON_Tools_XML)

protected:
	CCON_Tools_XML();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CCON_Tools_XML();

public:
	enum { IDD = IDD_CON_XML_TOOLS };
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ���� [��ȡ������Ϣ������]
	afx_msg void OnDestroy();											//���ٴ���
	afx_msg void OnSize(UINT nType, int cx, int cy);					//�����ߴ�
	afx_msg void OnLvnColumnclickList6(NMHDR *pNMHDR, LRESULT *pResult);//ObjType�б�����(�һ��б�������)
	afx_msg void OnNMClickList6(NMHDR *pNMHDR, LRESULT *pResult);		//���ObjType�б�
	afx_msg void OnLvnColumnclickList4(NMHDR *pNMHDR, LRESULT *pResult);//Object�б�����(�һ��б�������)
	afx_msg void OnBnClickedButton4();									//��ȡXML�ļ���Ϣ
	afx_msg void OnBnClickedButton1();									//ObjType����
	afx_msg void OnBnClickedButton2();									//Object����
	afx_msg void OnNMClickList7(NMHDR *pNMHDR, LRESULT *pResult);		//
	afx_msg void OnBnClickedButton5();

	//�Զ���
	void		My_Show_Object();										//��ʾObject��Ϣ
	void		My_Show_Object_Data(list_Object* v_pObject);			//��ʾObject_Data��Ϣ
	void		My_Show_Object_Child();									//��ʾObject_Child��Ϣ

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����

	//�ִ�
	CSSplitter		m_SplitterPane_Main;								//��ֱ��
	CSSplitter		m_SplitterPane_Right;								//ˮƽ��

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	//��Ļ
	CMyListCtrl m_cList_ObjType;										//ObjType�б�
	CMyListCtrl m_cList_Object;											//Object�б�
	CMyListCtrl m_cList_Child;											//PM����
	CEdit		m_cFile_Name;											//ѡ�е��ļ�����
	CButton		m_cOpen_File;											//������
	CComboBox	m_cType;												//�ļ����͡����á��澯��
	CEdit		m_cSum;													//
	CButton		m_cFile;												//

	//�Զ���
	CString		v_sFrame_Name;											//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������

	CMyFile_XML	v_cFile_XML;											//XML�ļ�����
	int			v_iItem_ObjType;										//ѡ�е�ObjType��Ŀ���
	int			v_iItem_ObjType_Child;									//ѡ�е�ObjType_Child��Ŀ���
	CString		v_sName_ObjType;										//ѡ�е�ObjType��Ŀ����
	CString		v_sName_Child;											//ѡ�е�Child��Ŀ����
	int			v_iWorking;												//����״̬��0��ObjType��	1��ObjType_Child��
	unsigned int	v_iChild_Col_Max;									//������������
	bool		v_bRead;												//�������ͣ�0�����á����ܣ�1������ֱ����
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
