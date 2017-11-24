#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"
#include "webbrowser2.h"												//��CWebBrowser2���ڵ�ͷ�ļ�

// CQUA_Station ������ͼ

class CQUA_Station : public CFormView
{
	DECLARE_DYNCREATE(CQUA_Station)

protected:
	CQUA_Station();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CQUA_Station();

public:
	enum { IDD = IDD_QUA_STATION };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	//������Tree_Cell
	struct  Tree_Cell_Struct
	{
		bool	v_bStop;												//���ϱ�־
		CString	v_sStation;												//С������
	};  
	typedef map<CString,Tree_Cell_Struct>	mTree_Cell;					//������С����Ϣ
	//������Tree_BTS
	struct  Tree_BTS_Struct
	{
		bool		v_bBreak;											//������־
		CString		v_sBTS_Name;										//������
		CString		v_sCover;											//��������
		CString		v_sAgency;											//��ά
		mTree_Cell	v_mTree_Cell;										//С����Ϣ
	};  
	typedef map<CString,Tree_BTS_Struct> mTree_BTS;						//������BTS

	//������Tree_Room
	struct  Tree_Room_Struct
	{
		CString		v_sCover_Type;										//��������
		CString		v_sCover_Type_Cell;									//��������
		CString		v_sRoom_Cell;										//����(С��)
		double		v_dLongitude_Cell;									//����(С��)
		double		v_dLatitude_Cell;									//γ��(С��)
		double		v_dLongitude;										//����_BTS
		double		v_dLatitude;										//γ��_BTS
		int			v_iType;											//�������ͣ�0����վ��1����Զ��2����վ(��������Զ)��
		mTree_BTS	v_mTree_BTS;										//������BTS
	};  
	typedef map<CString,Tree_Room_Struct> mTree_Room;					//����������

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
	afx_msg void OnBnClickedExcel2();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton7();
	afx_msg void OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult);

	//�Զ���
	void		My_Query_Station();										//��ѯ��վַ
	void		My_LoadData_Station_Data(CString v_sSql);				//װ��վַ��Ϣ
	void		My_LoadData_Tree(CString v_sRoom,CString v_sStation_Cover,double v_dLongitude,double v_dLatitude);	//װ�ػ�����Ϣ
	void		My_Show_Tree(CString v_sRoom);							//��ʾ������Ϣ
	void		My_LoadData_Tree_Cell(CString v_sRoom,CString v_sBTS,int v_iType,Tree_BTS_Struct* v_cTree_BTS);		//��ȡ��С����Ϣ��BTS\NodeB\GT BTS��
	void		My_LoadData_Tree_Cell_Remote(CString v_sRoom,int v_iType);		//��ȡ��С����Ϣ_��Զ��BTS\NodeB\GT BTS��
	CString		My_Get_Station_BTS(CString v_sBTS,int v_iType,Tree_BTS_Struct* v_cTree_BTS,CString* v_sCover_Type,double* v_dLongitude,double* v_dLatitude);	//��ȡ����վ������BTS\NodeB\GT BTS��
	void		My_Clear();												//�������
	void		My_ShowList_BTS(CString v_s1,CString v_s2,CString v_s3,CString v_s4,CString v_s5,CString v_s6);		//�б���ʾ

	//�ִ�
	CSSplitter		m_SplitterPane_Main;								//��ֱ��
	CSSplitter		m_SplitterPane_Bottom;								//��ֱ��

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����
	void		My_Input_Main();										//����_������

	//------------------------------------------------------------------------------------------------------
	//	��������
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList_Station;									//Station��Ϣ
	CMyListCtrl		m_cList_BTS;
	CEdit			m_cSum;												//��Ԫ����
	CEdit			m_cKey;												//Key
	CComboBox		m_cKey_Select;
	CButton			m_cQuery;
	CButton			m_cExcel_NodeB;
	CButton			m_cMap_Get;
	CButton			m_cBTS;

	//�Զ���
	CString			v_sFrame_Name;										//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item_Room;									//ѡ�е��б�Item [Room]
	int				v_iCol_Lon,v_iCol_Lat;								//�к�:���ȡ�γ��

	mTree_Room		v_mTree_Room;										//��������
};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
