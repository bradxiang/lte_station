#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"
#include "webbrowser2.h"												//��CWebBrowser2���ڵ�ͷ�ļ�
#include <MSHTML.H>


// CCon_Station_Map ������ͼ

class CCon_Station_Map : public CFormView, public IDispatch
{
	DECLARE_DYNCREATE(CCon_Station_Map)

protected:
	CCon_Station_Map();													// ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CCon_Station_Map();

public:
	IHTMLDocument2*		m_pIHTMLDoc;									//

	enum { IDD = IDD_CON_STATION_MAP};
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual void OnInitialUpdate();											//��ʼ��
	virtual BOOL PreTranslateMessage(MSG* pMsg);							//��ϢԤ����
	afx_msg void OnDestroy();												//
	afx_msg void OnSize(UINT nType, int cx, int cy);						//
	afx_msg void OnBnClickedQuary();										//
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);		//��Item�仯��Ӧ
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedExcel();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedQuary3();
	afx_msg void OnBnClickedExcel2();

	//�Զ���
	void		My_Query_Station();																				//��ѯ��վַ
	void		My_LoadData_Station_Data(CString v_sSql);														//װ��վַ��Ϣ
	void		My_OutPut();																					//��ϸ����
	void		MyOutPut_Station(mStation_OutPut *v_mStation_OutPut,CString v_sFile_Name,bool v_bFile_Open);	//���CSV�ļ�
	void		My_Map_Station();																				//��ͼ��ȡ
	void		My_Marker();																					//����Marker
	void		GetStationBounds();

	//JS Call MFC
	void		My_Station_Select(CString v_sTop, CString v_sLeft,CString v_sBottom, CString v_sRight);
	afx_msg void OnDocumentCompleteExplorerMain(LPDISPATCH pDisp, VARIANT FAR* URL);

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
	CEdit			m_cSum;												//��Ԫ����
	CEdit			m_cKey;												//Key
	CComboBox		m_cKey_Select;										//
	CButton			m_cQuery;											//
	CButton			m_cExcel;											//
	CButton			m_cOnline;											//����ѡ��
	CButton			m_cOK;												//��Чվַ
	CButton			m_cDetail;											//վַ����
	CButton			m_cMarker;
	CButton			m_cClear;

	//�Զ���
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������
	CString			v_sFrame_Name;										//��������
	int				v_iWorking;											//�������ţ�
	int				v_iList_Item_Room;									//ѡ�е��б�Item [Room]
	bool			v_bShow;											//������Ļ�ߴ��־
	bool			v_bLaod;											//HTML�ļ����ñ�־
	bool			time_control;										//���ƶ�ʱ������
	CWebBrowser2	m_webBrowser;										//������ؼ�

	//JS����C++
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo);
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);
	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
	virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE	AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	//���Ժ���
	DWORD	GetProcessID();
	void	ShowMessageBox(const wchar_t *msg);	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
