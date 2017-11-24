#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"
#include "webbrowser2.h"												//��CWebBrowser2���ڵ�ͷ�ļ�

// CQUA_Station_Map ������ͼ

class CQUA_Station_Map : public CFormView
{
	DECLARE_DYNCREATE(CQUA_Station_Map)

protected:
	CQUA_Station_Map();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CQUA_Station_Map();

public:
	enum { IDD = IDD_QUA_STATION_MAP };
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
	afx_msg void OnCbnCloseupCombo11();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult);

	//�Զ���
	void		My_Query_Station();										//��ѯ��վַ
	void		My_LoadData_Station_Data(CString v_sSql);				//װ��վַ��Ϣ
	void		My_LoadData_Tree(CString v_sRoom,CString v_sStation_Cover,double v_dLongitude,double v_dLatitude);	//װ�ػ�����Ϣ
	void		My_Show_Tree();											//��ʾ������Ϣ
	void		My_LoadData_Tree_Cell(CString v_sRoom,CString v_sBTS,int v_iType,Tree_BTS_Struct* v_cTree_BTS);		//��ȡ��С����Ϣ��BTS\NodeB\GT BTS��
	void		My_LoadData_Tree_Cell_Remote(CString v_sRoom,int v_iType);		//��ȡ��С����Ϣ_��Զ��BTS\NodeB\GT BTS��
	CString		My_Get_Station_BTS(CString v_sBTS,int v_iType,Tree_BTS_Struct* v_cTree_BTS,CString* v_sCover_Type,double* v_dLongitude,double* v_dLatitude);	//��ȡ����վ������BTS\NodeB\GT BTS��
	void		My_Clear();												//�������

	//�ִ�
	CSSplitter		m_SplitterPane_Main;								//��ֱ��
	CSSplitter		m_SplitterPane_Bottom;								//��ֱ��

	//m_cTree
	CMyMutiTreeCtrl	m_cTree_Room;
	CImageList		v_cImage_Tree;										//��ͼ�꣺����
	CImageList		v_cImage_State;										//��ͼ�꣺״̬
	int				v_iItem_Tree;										//ѡ�е�Tree��Ŀ���
	HTREEITEM		v_hBTS_Root,v_hBTS_Node;							//��Ŀ¼������ڵ�����

	//�̣߳�����
	CWinThread* v_pThread_Input;										//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);						//�̣߳�����
	void		My_Input_Main();										//����_������

	//������ؼ�
	DECLARE_EVENTSINK_MAP()
	void DocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT* URL);		//��ҳװ�����

	CWebBrowser2	m_webBrowser;										//�����ʵ����
	IHTMLDocument2*	m_pIHTMLDoc;										//

	//��ͼ����
	void		My_Map_Marker(double v_dLongitude,double v_dLatitude,CString v_sName,CString v_sType,CString v_sCover);	//��ʾ�����(�߳�)
	void		My_Map_Marker_Direct(double v_dLongitude,double v_dLatitude,CString v_sName,int v_iType);	//��ʾ�����(UI)
	void		My_Map_Line(double v_dLon_0,double v_dLat_0,double v_dLon_1,double v_dLat_1);		//��ʾ������
	void		My_Set_Center(double v_dRoom_Lon,double v_dRoom_Lat,int v_iLevel);					//��ʾ�����ĵ�
	void		My_Show_Marker();										//��ͼ��ʾ�����Ϣ
	void		My_Map_Station();										//��ͼ��ȡ
	afx_msg LRESULT My_Map_Marker_Show(WPARAM iParam1,LPARAM iParam2);	//����Map Marker��Ϣ(�߳�)

	CString		v_sMap_Name,v_sMap_Type,v_sMap_Cover;					//��ʱ����
	double		v_dMap_Lon,v_dMap_Lat;									//��ʱ����
	bool		v_bMap;													//Marker��ʾ���

	//------------------------------------------------------------------------------------------------------
	//	��������
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList_Station;									//Station��Ϣ
	CEdit			m_cSum;												//��Ԫ����
	CEdit			m_cKey;												//Key
	CComboBox		m_cKey_Select;
	CButton			m_cQuery;
	CButton			m_cExcel_NodeB;
	CEdit			m_cSum_Remote;
	CButton			m_cBTS;
	CButton			m_cMarker;
	CButton			m_cMap_Clear;
	CButton			m_cMap_Get;
	CComboBox		m_cSelect;

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
