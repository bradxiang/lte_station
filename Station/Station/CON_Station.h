#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"

// CCON_Station ������ͼ

class CCON_Station : public CFormView
{
	DECLARE_DYNCREATE(CCON_Station)

protected:
	CCON_Station();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CCON_Station();

public:
	enum { IDD = IDD_CON_STATION };
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
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);	//��Item�仯��Ӧ
	afx_msg void OnBnClickedElement();									//��Ԫ����
	afx_msg void OnBnClickedExcel2();
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnConStationDel();
	afx_msg void OnBnClickedElement2();
	afx_msg void OnBnClickedElement3();
	afx_msg void OnBnClickedButton2();

	//�Զ���
	bool		My_ExecuteSQL(CString v_sSql);							//ִ��SQL����
	void		My_Query_Station();										//��ѯ��վַ
	void		My_Query_Region();										//����˲�
	void		My_LoadData_Station_Data(CString v_sSql);				//װ��վַ��Ϣ
	void		My_Show_Region(mStation_OutPut *v_mStation_OutPut);		//������ʾ

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
	CComboBox		m_cKey_Select;
	CButton			m_cQuery;
	CButton			m_cExcel;
	CButton			m_cExcel_NodeB;
	CButton			m_cElement;

	//�Զ���
	CString			v_sFrame_Name;										//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item;										//ѡ�е��б�Item
	int				v_iSum;												//��������

	//������Station_Long
	struct  Station_Long_Struct
	{
		double	v_dLongitude;											//����
		double	v_dLatitude;											//γ��
	};  
	typedef map<CString,Station_Long_Struct> mStation_Long;				//������mStation_Long
	CButton m_cRemote;
	CButton m_cStation;
	CButton m_cCell;
};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
